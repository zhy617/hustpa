#include "common.h"
#include <amdev.h>

size_t serial_write(const void *buf, size_t offset, size_t len) {
  // printf("serial_write: buf=%x, offset=%d, len=%d\n", buf, offset, len);
  // printf("%s\n", (char *)buf);
  for(size_t i = 0; i < len; i++) {
    _putc(*((char *)buf + i));
  }
  // printf("fuckyoufuckyou!!!\n");
  return len;
}

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

size_t events_read(void *buf, size_t offset, size_t len) {
  int key = read_key();
  // printf("key: 0x%x\n", key);
  // printf("len: %d\n", len);
  bool is_keydown = key & 0x8000? true : false;
  key = key & 0x7fff;
  int n = 0;
  if (key == _KEY_NONE) {
    uint32_t time = uptime();
    n = snprintf(buf, len, "t %d\n", time);
  } else {
    n = snprintf(buf, len, "%s %s\n", is_keydown ? "kd" : "ku", keyname[key]);
  }
  return n;
}

static char dispinfo[128] __attribute__((used)) = {};

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  return 0;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

size_t fbsync_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
}
