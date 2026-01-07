#include <am.h>
#include <amdev.h>
#include <nemu.h>

size_t __am_video_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_INFO: {
      _DEV_VIDEO_INFO_t *info = (_DEV_VIDEO_INFO_t *)buf;
      // uint32_t screen_info = inl(SCREEN_ADDR);
      // info->width = (screen_info >> 16) & 0xffff;
      // info->height = screen_info & 0xffff;

      info->width = 0;
      info->height = 0;
      return sizeof(_DEV_VIDEO_INFO_t);
    }
  }
  return 0;
}

size_t __am_video_write(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_FBCTL: {
      _DEV_VIDEO_FBCTL_t *ctl = (_DEV_VIDEO_FBCTL_t *)buf;
      // int x = ctl->x;;
      // int y = ctl->y;
      // int w = ctl->w;
      // int h = ctl->h;
      // int W = inl(SCREEN_ADDR) >> 16;
      // int H = inl(SCREEN_ADDR) & 0xffff;
      // if (x < 0 || y < 0 || x + w > W || y + h > H) {
      //   return 0;
      // }
      // uint32_t addr = FB_ADDR + (y * W + x) * sizeof(uint32_t);
      // uint32_t *pixels = ctl->pixels;
      // for (int i = 0; i < h; i++) {
      //   for (int j = 0; j < w; j++) {
      //     outl(addr + (i * W + j) * sizeof(uint32_t), pixels[i * w + j]);
      //   }
      // }
      if (ctl->sync) {
        outl(SYNC_ADDR, 0);
      }
      return size;
    }
  }
  return 0;
}

void __am_vga_init() {
  // int i;
  // int size = screen_width() * screen_height();
  // uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  // for (i = 0; i < size; i ++) fb[i] = i;
  // draw_sync();
}
