#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  return 0;
}

char *strcpy(char* dst,const char* src) {
  assert(dst != NULL && src != NULL);
  char* ret = dst;
  while ((*dst++ = *src++));
  return ret;
}

char* strncpy(char* dst, const char* src, size_t n) {
  return NULL;
}

char* strcat(char* dst, const char* src) {
  assert(dst != NULL && src != NULL);
  char* ret = dst;
  // Find the end of dst
  while (*dst) {
    dst++;
  }
  // Copy src to the end of dst
  while ((*dst++ = *src++));
  return ret;
}

int strcmp(const char* s1, const char* s2) {
  assert(s1 != NULL && s2 != NULL);
  while (*s1 && (*s1 == *s2)) {
    s1++;
    s2++;
  }
  return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

int strncmp(const char* s1, const char* s2, size_t n) {
  return 0;
}

void* memset(void* v,int c,size_t n) {
  assert(v != NULL);
  unsigned char* p = v;
  unsigned char val = c;
  while (n--) {
    *p++ = val;
  }
  return v;
}

void* memcpy(void* out, const void* in, size_t n) {
  assert(out != NULL && in != NULL);
  // Handle the case where out and in might be the same
  if (out == in) {
    return out;
  }
  unsigned char* p_out = out;
  const unsigned char* p_in = in;
  while (n--) {
    *p_out++ = *p_in++;
  }
  return out;
}

int memcmp(const void* s1, const void* s2, size_t n){
  assert(s1 != NULL && s2 != NULL);
  const unsigned char* p1 = s1;
  const unsigned char* p2 = s2;
  while (n-- > 0) {
    if (*p1 != *p2) {
      return *p1 - *p2;
    }
    p1++;
    p2++;
  }
  return 0;
}

#endif
