#include <my_timers.h>
#include <rg_system.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
    const char *name;
    int64_t start_time;
    int64_t end_time;
    int64_t total_time;
} my_timer_t;

static my_timer_t my_timers[timer_count] = {
    [timer_total] = {"total", 0, 0, 0},
    [timer_misc] = {"misc", 0, 0, 0},
    [timer_m68k_run] = {"m68k_run", 0, 0, 0},
    [timer_z80_run] = {"z80_run", 0, 0, 0},
    [timer_ym2612_run] = {"ym2612_run", 0, 0, 0},
    [timer_gwenesis_vdp_render_line] = {"gwenesis_vdp_render_line", 0, 0, 0},
    [timer_gwenesis_SN76489_run] = {"gwenesis_SN76489_run", 0, 0, 0},
};

void timer_start(size_t index)
{
    my_timer_t *timer = &my_timers[index];
    timer->start_time = rg_system_timer();
}

void timer_stop(size_t index)
{
    my_timer_t *timer = &my_timers[index];
    timer->end_time = rg_system_timer();
    timer->total_time += timer->end_time - timer->start_time;
}

void timer_dump(void)
{
    int64_t total_time = my_timers[0].total_time;
    RG_LOGI("Start Timers:");
    for (size_t i = 1; i < timer_count; ++i)
    {
        const my_timer_t *timer = &my_timers[i];
        RG_LOGI("    Timer %s: %.2f%% (%d ms)", timer->name, (float)timer->total_time / total_time * 100, (int)(timer->total_time / 1000));
    }
    RG_LOGI("    Total: %.2f%% (%d ms)", 100.f, (int)(total_time / 1000));

    RG_LOGI("End Timers:");
}

void timer_reset()
{
    for (size_t i = 0; i < timer_count; ++i)
    {
        my_timers[i].total_time = 0;
    }
}