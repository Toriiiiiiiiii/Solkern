#include "../../include/ftm.h"
#include "../../include/flanterm.h"
#include "../../include/flanterm_backends/fb.h"
#include "../../include/multiboot.h"
#include "../../include/liballoc.h"

int ftm_descriptor = 0;
static struct flanterm_context *ft_ctx = NULL;

int ftm_install() {
    ftm_descriptor = mkpipe("ftm", ftm_write, ftm_read); 

    uint8_t* fb = (uint8_t*)(4096 * 1024);
    ft_ctx = flanterm_fb_init(
            malloc, (void*)(void*)free,
            (uint32_t*)fb, fb_info.framebuffer_width, 
            fb_info.framebuffer_height, fb_info.framebuffer_pitch,
            fb_info.color_info[1], fb_info.color_info[0],
            fb_info.color_info[3], fb_info.color_info[2],
            fb_info.color_info[5], fb_info.color_info[4],
            NULL, NULL, NULL,
            NULL, NULL, NULL,
            NULL, NULL,
            0, 0, 1, 0, 0, 0
    );

    if(!ft_ctx) return 1;
    return 0;
}

void ftm_write(uint32_t n, void* data) {
    flanterm_write(ft_ctx, (char*)data, n);
}

void ftm_read(uint32_t n, void* buf) {
}
