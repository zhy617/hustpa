#include "fs.h"
#include "ramdisk.h"
#include "device.h"

extern size_t ramdisk_read(void *buf, size_t offset, size_t len);
extern size_t ramdisk_write(const void *buf, size_t offset, size_t len);

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  size_t open_offset;
  ReadFn read;
  WriteFn write;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_EVENTS, FD_FB, FD_FBSYNC};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin", 0, 0, 0, invalid_read, invalid_write},
  {"stdout", -1, 0, 0, invalid_read, serial_write},
  {"stderr", -1, 0, 0, invalid_read, serial_write},
  {"/dev/events", -1, 0, 0, events_read, invalid_write},
  {"dev/fb", 0, 0, 0, invalid_read, fb_write},
  {"dev/fbsync", 0, 0, 0, invalid_read, fbsync_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void init_fs() {
  // TODO: initialize the size of /dev/fb
  file_table[FD_FB].size = screen_width() * screen_height() * 4;
}

int fs_open(const char *pathname, int flags, int mode) {
  for (int i = 0; i < NR_FILES; i++) {
    if (strcmp(pathname, file_table[i].name) == 0) {
      return i;
    }
  }
  panic("File %s not found\n", pathname);
  return -1;
}

size_t fs_read(int fd, void *buf, size_t len) {
  // printf("fs_read: fd=%d, buf=%x, len=%d\n", fd, buf, len);
  assert(fd >= 0 && fd < NR_FILES);
  
  Finfo *f = &file_table[fd];
  size_t offset = f->open_offset;
  if (offset + len > f->size) {
    len = f->size - offset;
  } 
  assert(len >= 0);

  if (f->read) {
    size_t ret = f->read(buf, offset, len);
    switch (fd) {
      case FD_EVENTS:
        // do not update open_offset for /dev/events and stdin
        break;
      default:
        f->open_offset += ret;
    }
    return ret;
  } else {
    ramdisk_read(buf, f->disk_offset + offset, len);
    f->open_offset += len;
    return len;
  }
}

size_t fs_write(int fd, const void *buf, size_t len) {
  // printf("fs_write: fd=%d, buf=%x, len=%d\n", fd, buf, len);
  // printf("%s\n", file_table[fd].name);
  // printf("%s\n", buf);
  assert(fd >= 0 && fd < NR_FILES);

  Finfo *f = &file_table[fd];
  size_t offset = f->open_offset;
  // printf("fuckyou!!\n");
  if (offset + len > f->size) {
    len = f->size - offset;
  }
  assert(len >= 0);
  // printf("fufu!!!\n");
  if (f->write) {
    size_t ret = f->write(buf, offset, len);
    switch (fd) {
      case FD_STDOUT:
      case FD_STDERR:
        // do not update open_offset for stdout and stderr
        break;
      default:
        f->open_offset += ret;
    }
    // printf("fufufu!!!\n");
    return ret;
  } else {
    ramdisk_write(buf, f->disk_offset + offset, len);
    f->open_offset += len;
    return len;
  }
}

size_t fs_lseek(int fd, size_t offset, int whence) {
  assert(fd >= 0 && fd < NR_FILES);

  Finfo *f = &file_table[fd];
  size_t new_offset = 0;
  switch (whence) {
    case SEEK_SET: new_offset = offset; break;
    case SEEK_CUR: new_offset = f->open_offset + offset; break;
    case SEEK_END: new_offset = f->size + offset; break;
    default: panic("whence = %d is invalid\n", whence);
  }
  assert(new_offset <= f->size);
  f->open_offset = new_offset;
  return f->open_offset;
}

int fs_close(int fd) {
  assert(fd >= 0 && fd < NR_FILES);
  Finfo *f = &file_table[fd];
  f->open_offset = 0;
  return 0;
}