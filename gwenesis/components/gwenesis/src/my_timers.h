#include <stdint.h>
#include <stddef.h>
enum
{
    timer_total,
    timer_misc,
    timer_m68k_run,
    timer_z80_run,
    timer_ym2612_run,
    timer_gwenesis_vdp_render_line,
    timer_gwenesis_SN76489_run,
    timer_count,
};
void timer_start(size_t index);
void timer_stop(size_t index);
void timer_dump(void);
void timer_reset(void);