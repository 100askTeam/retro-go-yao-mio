// FAQ: https://forums.100ask.net/c/esp/49

// Target definition
#define RG_TARGET_NAME             "YAO-MIO"

// Storage
#define RG_STORAGE_DRIVER           1                   // 0 = Host, 1 = SDSPI, 2 = SDMMC, 3 = USB, 4 = Flash
#define RG_STORAGE_HOST             SPI2_HOST           // Used by driver 1 and 2
#define RG_STORAGE_SPEED            SDMMC_FREQ_DEFAULT  // Used by driver 1 and 2
#define RG_STORAGE_ROOT             "/sd"               // Storage mount point

// network
#define RG_ENABLE_NETWORKING        1   // 0 = Disable, 1 = Enable

// Audio
#define RG_AUDIO_USE_INT_DAC        0   // 0 = Disable, 1 = GPIO25, 2 = GPIO26, 3 = Both
#define RG_AUDIO_USE_EXT_DAC        1   // 0 = Disable, 1 = Enable

// Video
#define RG_SCREEN_DRIVER            0   // 0 = ILI9341
#define RG_SCREEN_HOST              SPI2_HOST
#define RG_SCREEN_SPEED             SPI_MASTER_FREQ_40M
#define RG_SCREEN_TYPE              7   // 4 = ESPLAY-ST7789V2, 7 = Retro Yao-Mio Screen
#define RG_SCREEN_ROTATE            1   // 0 = 0°, 1 = 90°,2 = 180° 3 = 270°
#define RG_SCREEN_WIDTH             480
#define RG_SCREEN_HEIGHT            320
#define RG_SCREEN_MARGIN_TOP        0
#define RG_SCREEN_MARGIN_BOTTOM     0
#define RG_SCREEN_MARGIN_LEFT       0
#define RG_SCREEN_MARGIN_RIGHT      0

// Input
#define RG_GAMEPAD_DRIVER           3   // 1 = ODROID-GO, 2 = Serial, 3 = I2C
#define RG_GAMEPAD_HAS_MENU_BTN     1
#define RG_GAMEPAD_HAS_OPTION_BTN   1
// Note: Depending on the driver, the button map can be a bitmask, an index, or a GPIO.
// Refer to rg_input.h to see all available RG_KEY_*
#define RG_GAMEPAD_MAP {\
    {RG_KEY_UP,     RG_GAMEPAD_MAP_UP},\
    {RG_KEY_RIGHT,  RG_GAMEPAD_MAP_RIGHT},\
    {RG_KEY_DOWN,   RG_GAMEPAD_MAP_DOWN},\
    {RG_KEY_LEFT,   RG_GAMEPAD_MAP_LEFT},\
    {RG_KEY_SELECT, RG_GAMEPAD_MAP_SELECT},\
    {RG_KEY_START,  RG_GAMEPAD_MAP_START},\
    {RG_KEY_MENU,   RG_GAMEPAD_MAP_MENU},\
    {RG_KEY_OPTION, RG_GAMEPAD_MAP_OPTION},\
    {RG_KEY_A,      RG_GAMEPAD_MAP_A},\
    {RG_KEY_B,      RG_GAMEPAD_MAP_B},\
    {RG_KEY_X,      RG_GAMEPAD_MAP_X},\
    {RG_KEY_Y,      RG_GAMEPAD_MAP_Y},\
    {RG_KEY_L,      RG_GAMEPAD_MAP_L},\
    {RG_KEY_R,      RG_GAMEPAD_MAP_R},\
}


// Battery
#define RG_BATTERY_ADC_CHANNEL          ADC1_CHANNEL_1
#define RG_BATTERY_CALC_PERCENT(raw)    (((raw) - 1600.f) / (1900.f - 1600.f) * 100.f)
#define RG_BATTERY_CALC_VOLTAGE(raw)    ((raw + 300) * 2.f * 0.001f)
#define RG_BATTERY_LOW_POWER            15 /*percent*/   
#define RG_BATTERY_SAMPLES              16
   
// Status LED
#define RG_GPIO_LED                 GPIO_NUM_21

// I2C BUS
#if (RG_GAMEPAD_DRIVER == 3 || RG_AUDIO_USE_EXT_DAC == 1)
    #define RG_GPIO_I2C_SDA             GPIO_NUM_17
    #define RG_GPIO_I2C_SCL             GPIO_NUM_18
#endif

#if(RG_GAMEPAD_DRIVER == 2)
    // SNES-style gamepad
    #define RG_GPIO_GAMEPAD_LATCH       GPIO_NUM_41 /*D+*/
    #define RG_GPIO_GAMEPAD_CLOCK       GPIO_NUM_40 /*ID*/
    #define RG_GPIO_GAMEPAD_DATA        GPIO_NUM_42 /*D-*/
#endif

// Built-in gamepad
#if (RG_GAMEPAD_DRIVER == 3)    // 100ask i2c I/O expanders
    #define RG_GAMEPAD_MAP_UP           (1<<0)
    #define RG_GAMEPAD_MAP_DOWN         (1<<1)
    #define RG_GAMEPAD_MAP_LEFT         (1<<2)
    #define RG_GAMEPAD_MAP_RIGHT        (1<<3)
    #define RG_GAMEPAD_MAP_A            (1<<4)
    #define RG_GAMEPAD_MAP_B            (1<<5)
    #define RG_GAMEPAD_MAP_X            (1<<6)
    #define RG_GAMEPAD_MAP_Y            (1<<7)
    #define RG_GAMEPAD_MAP_START        (1<<8)
    #define RG_GAMEPAD_MAP_SELECT       (1<<9)
    #define RG_GAMEPAD_MAP_MENU         (1<<10)
    #define RG_GAMEPAD_MAP_OPTION       (1<<11)
    #define RG_GAMEPAD_MAP_L            (1<<12)
    #define RG_GAMEPAD_MAP_R            (1<<13)
#elif (RG_GAMEPAD_DRIVER == 2)  // 100ask fc joypad
    #define RG_GAMEPAD_MAP_A            (1<<0)
    #define RG_GAMEPAD_MAP_B            (1<<1)
    //#define RG_GAMEPAD_MAP_SELECT       (1<<2)
    #define RG_GAMEPAD_MAP_MENU         (1<<2)
    #define RG_GAMEPAD_MAP_START        (1<<3)
    #define RG_GAMEPAD_MAP_UP           (1<<4)
    #define RG_GAMEPAD_MAP_DOWN         (1<<5)
    #define RG_GAMEPAD_MAP_LEFT         (1<<6)
    #define RG_GAMEPAD_MAP_RIGHT        (1<<7)
    //#define RG_GAMEPAD_MAP_MENU         (1<<8)
    //#define RG_GAMEPAD_MAP_OPTION       (1<<11)
    //#define RG_GAMEPAD_MAP_L            (0)
    //#define RG_GAMEPAD_MAP_R            (0)
#endif

// SPI Display
#ifdef RG_SCREEN_DRIVER
    #define RG_GPIO_LCD_MISO            GPIO_NUM_13
    #define RG_GPIO_LCD_MOSI            GPIO_NUM_11
    #define RG_GPIO_LCD_CLK             GPIO_NUM_12
    #define RG_GPIO_LCD_CS              GPIO_NUM_10
    #define RG_GPIO_LCD_DC              GPIO_NUM_5
    #define RG_GPIO_LCD_BCKL            GPIO_NUM_7
    #define RG_GPIO_LCD_RST             GPIO_NUM_6
#endif

#if (RG_STORAGE_DRIVER == 1)
    #define RG_GPIO_SDSPI_MISO          GPIO_NUM_13
    #define RG_GPIO_SDSPI_MOSI          GPIO_NUM_11
    #define RG_GPIO_SDSPI_CLK           GPIO_NUM_12
    #define RG_GPIO_SDSPI_CS            GPIO_NUM_8
#endif

// External I2S DAC
#if(RG_AUDIO_USE_EXT_DAC == 1)
    #define RG_GPIO_SND_I2S_BCK         GPIO_NUM_14
    #define RG_GPIO_SND_I2S_WS          GPIO_NUM_15
    #define RG_GPIO_SND_I2S_DATA        GPIO_NUM_16
    //#define RG_GPIO_SND_AMP_ENABLE      GPIO_NUM_9
#endif
