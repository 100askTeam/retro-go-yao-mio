/*
 * This file is part of factory.
 * Copyright (c) 2023 Shenzhen Baiwenwang Technology Co., Ltd (https://forums.100ask.net).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <sys/dirent.h>
#include <sys/unistd.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <rg_system.h>

#include "esp_system.h"
#include "esp_ota_ops.h"


#if defined(RG_TARGET_YAO_MIO)
#define FIRMWARE_LOCATION RG_STORAGE_ROOT "/yaomio/firmware"
#endif

#define AUDIO_SAMPLE_RATE   (32000)

#define UPDATE_BUFFER_SIZE  (1024)

static rg_app_t *app;

const char *fw_exedir(void)
{
     return FIRMWARE_LOCATION;
}

bool is_iwad(const char *path)
{
    return true;
}

void app_main()
{
    esp_err_t err;
    esp_app_desc_t running_fw;
    esp_app_desc_t update_fw;
    esp_app_desc_t invalid_fw;
    esp_ota_handle_t update_handle = 0 ;
    const esp_partition_t *running = NULL;
    const esp_partition_t *update_partition = NULL;

    static char temp_buffer[UPDATE_BUFFER_SIZE];
    uint32_t update_size = 0;

    app = rg_system_init(AUDIO_SAMPLE_RATE, NULL, NULL);

    RG_LOGI("configNs=%s, app->romPath=%s", app->configNs, app->romPath);

    if (!rg_storage_ready())
    {
        rg_display_clear(C_SKY_BLUE);
        rg_gui_alert("SD Card Error", "Storage mount failed.\nMake sure the card is FAT32.");
    }

    const char *fw_filename = NULL;
    char fw_filename_aux[256];
    
    FILE *fp;

    if ((fp = fopen(app->romPath, "rb")))
    {
        fw_filename = app->romPath;
        RG_LOGI("fw_filename=%s", fw_filename);
        fclose(fp);
    }

    if (!fw_filename)
    {
        fw_filename = rg_gui_file_picker("Select firmware file", fw_exedir(), is_iwad);
        sprintf(fw_filename_aux, "%s/%s", FIRMWARE_LOCATION, fw_filename);
        fw_filename = fw_filename_aux;
        RG_LOGI("fw_filename=%s, fw_filename_aux=%s", fw_filename, fw_filename_aux);
    }

    RG_LOGI("Running firmware version: %s", app->version);

    // Get app data information to compare with the future version.
    //running = esp_ota_get_running_partition();
    //err = esp_ota_get_partition_description(running, &running_fw);
    //if(err != ESP_OK){
    //    RG_LOGE("Error getting running firmware information.");
    //    goto fail;
    //}

    //RG_LOGI("Running firmware version: %s", running_fw.version);

    // We're on the factory partition, get the OTA partition
    update_partition = esp_ota_get_next_update_partition(NULL);
    if(update_partition == NULL){
        RG_PANIC("Failed to get OTA partition!");
    }
    
    RG_LOGI("Found available partition subtype %d size %d at offset 0x%x with label %s," \
                                                        ,update_partition->subtype \
                                                        ,update_partition->size \
                                                        ,update_partition->address \
                                                        ,update_partition->label);

    // Get app data information of the last failed update
    //const esp_partition_t* last_invalid_app = esp_ota_get_last_invalid_partition();
    //esp_app_desc_t invalid_app_info;
    //if (esp_ota_get_partition_description(last_invalid_app, &invalid_fw) == ESP_OK) {
    //    RG_LOGI("Last invalid firmware version: %s", invalid_fw.version);
    //}

    //Open the new fw file
    fp = fopen(fw_filename, "rb");
    if(fp == NULL){
        RG_LOGE("Opening error with: %s",fw_filename);
        goto fail;
    }

    err = esp_ota_begin(update_partition, OTA_SIZE_UNKNOWN, &update_handle);
    if (err != ESP_OK) {
        RG_LOGE("esp_ota_begin failed (%s)", esp_err_to_name(err));
        goto fail;
    }
#if 0
    // Check the header to know if it's a valid version of the firmware
    size_t data_header = fread(temp_buffer, 1, UPDATE_BUFFER_SIZE, fp);

    if(data_header > sizeof(esp_image_header_t) + sizeof(esp_image_segment_header_t) + sizeof(esp_app_desc_t)){
        memcpy(&update_fw, &temp_buffer[sizeof(esp_image_header_t) + sizeof(esp_image_segment_header_t)], sizeof(esp_app_desc_t));

        // Check if you're triying to flash the same firmware
        if(!memcmp(update_fw.version, running_fw.version, sizeof(running_fw.version))){
            //Not sure if i should stop the update if it's the same version?
            RG_LOGE("Same firmware version.");
            //goto fail;
        }

        if(last_invalid_app != NULL){
            if (!memcmp(invalid_app_info.version, update_fw.version, sizeof(update_fw.version))) {
                RG_LOGE("New firmware is the same previous invalid version");
                goto fail;
            }
        }
    }

    RG_LOGE("update_fw.version=%s", update_fw.version);

    fseek(fp, 0, SEEK_SET);
#endif
    //fseek(fp, update_partition->address, SEEK_SET);
    memset(temp_buffer, 0, sizeof(temp_buffer));

    // The update is fine, so we will copy the new FW from the SD card to the OTA partition
    size_t data_read = 0;
    size_t data_read_count = 0;
    while(1){
        if((update_partition->size - data_read_count) >= UPDATE_BUFFER_SIZE)
        {
            data_read = fread(temp_buffer, 1, UPDATE_BUFFER_SIZE, fp);
        }
        else
        {
            data_read = fread(temp_buffer, 1, (update_partition->size - data_read_count), fp);
        }

        data_read_count += data_read;
        RG_LOGI("data_read: %d",data_read);

        if (data_read > 0)
        {
            err = esp_ota_write( update_handle, (const void *)temp_buffer, data_read);
            if (err != ESP_OK) {
                RG_PANIC("OTA write error");
            }
        }

        update_size += data_read;
        RG_LOGI("Written to OTA: %d",update_size);

        
        if((data_read < UPDATE_BUFFER_SIZE) || ((update_partition->size - data_read_count) < UPDATE_BUFFER_SIZE)) break;

    }

    fclose(fp);

    RG_LOGI("Update process succed!");

    //Close OTA and check if the files are fine.
    err = esp_ota_end(update_handle);
    if (err != ESP_OK) {
        if (err == ESP_ERR_OTA_VALIDATE_FAILED) {
            RG_LOGE("Image validation failed, image is corrupted");
            goto fail;
        }
        RG_LOGE("esp_ota_end failed (%s)!", esp_err_to_name(err));
        goto fail;
    }

    RG_LOGI("Changing boot partition to OTA");
    err = esp_ota_set_boot_partition(update_partition);
    if (err != ESP_OK) {
        RG_LOGE("esp_ota_set_boot_partition failed (%s)!", esp_err_to_name(err));
    } 

fail:
    RG_PANIC("Update fail!");
    //RG_LOGE("Update fail!");
    //rg_system_switch_app(RG_APP_LAUNCHER, RG_APP_LAUNCHER, 0, 0);
}
