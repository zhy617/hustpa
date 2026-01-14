#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  char buf[2048];
  va_list ap;
  va_start(ap, fmt);
  int ret = vsprintf(buf, fmt, ap);
  va_end(ap);
  for (char* p = buf; *p != '\0'; p++) {
    _putc(*p);
  }
  return ret;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  char* start = out;
  char temp_str[33];
  for (; *fmt; ++fmt) {
    if (*fmt != '%') {
      *out++ = *fmt;
      continue;
    }
    ++fmt;
    switch (*fmt) {
      case 'c': {
        char ch = (char)va_arg(ap, int);
        *out++ = ch;
        break;
      }
      case 's': {
        char* str = va_arg(ap, char*);
        while (*str) {
          *out++ = *str++;
        }
        break;
      }
      case 'd': {
        int val = va_arg(ap, int);
        int is_negative = 0;
        if (val < 0) {
          is_negative = 1;
          val = -val;
        }
        char* p = temp_str + 32;
        *p = '\0';
        do {
          *(--p) = (val % 10) + '0';
          val /= 10;
        } while (val != 0);
        if (is_negative) {
          *(--p) = '-';
        }
        while (*p) {
          *out++ = *p++;
        }
        break;
      }
      case 'x': {
        unsigned int val = va_arg(ap, unsigned int);
        char* hex_chars = "0123456789abcdef";
        char* p = temp_str + 32;
        *p = '\0';
        do {
          *(--p) = hex_chars[val % 16];
          val /= 16;
        } while (val != 0);
        while (*p) {
          *out++ = *p++;
        }
        break;
      }
      default:
        // Unsupported format specifier, ignore
        *out++ = '%';
        *out++ = *fmt;
        break;
    }
  }
  *out = '\0';
  return out - start;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int ret = vsprintf(out, fmt, ap);
  va_end(ap);
  return ret;
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  char *start = out;
  char *end = out + n; // 缓冲区的末尾（不包含）
  char temp_str[33];
  int count = 0; // 记录理论上应该写入的字符数

  // 确保至少能写入一个 '\0'
  if (n == 0) return 0;

  for (; *fmt; ++fmt) {
    if (*fmt != '%') {
      if (out < end - 1) { // 留出一位给 '\0'
        *out++ = *fmt;
      }
      count++;
      continue;
    }
    ++fmt;
    switch (*fmt) {
      case 'c': {
        char ch = (char)va_arg(ap, int);
        if (out < end - 1) {
          *out++ = ch;
        }
        count++;
        break;
      }
      case 's': {
        char* str = va_arg(ap, char*);
        if (str == NULL) str = "(null)";
        while (*str) {
          if (out < end - 1) {
            *out++ = *str;
          }
          str++;
          count++;
        }
        break;
      }
      case 'd': {
        int val = va_arg(ap, int);
        int is_negative = 0;
        if (val < 0) {
          is_negative = 1;
          val = -val;
        }
        char* p = temp_str + 32;
        *p = '\0';
        do {
          *(--p) = (val % 10) + '0';
          val /= 10;
        } while (val != 0);
        if (is_negative) {
          *(--p) = '-';
        }
        while (*p) {
          if (out < end - 1) {
            *out++ = *p;
          }
          p++;
          count++;
        }
        break;
      }
      case 'x': {
        unsigned int val = va_arg(ap, unsigned int);
        char* hex_chars = "0123456789abcdef";
        char* p = temp_str + 32;
        *p = '\0';
        do {
          *(--p) = hex_chars[val % 16];
          val /= 16;
        } while (val != 0);
        while (*p) {
          if (out < end - 1) {
            *out++ = *p;
          }
          p++;
          count++;
        }
        break;
      }
      default:
        if (out < end - 1) *out++ = '%';
        count++;
        if (out < end - 1) *out++ = *fmt;
        count++;
        break;
    }
  }
  *out = '\0'; // 写入 null 终止符
  return count; // 返回理论上的字符数
}
int snprintf(char *out, size_t n, const char *fmt, ...) {
  // assert(0);
  va_list ap;
  va_start(ap, fmt);
  int ret = vsnprintf(out, n, fmt, ap);
  va_end(ap);
  return 0;
}

#endif
