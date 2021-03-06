#include <iostream>
#include <openssl/sha.h>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// git より引用
char *sha1_to_hex(const unsigned char *sha1) {
  static int buf_size;
  static char hexbuffer[4][50];
  static const char hex[] = "0123456789abcdef";
  char *buffer = hexbuffer[3 & ++buf_size], *buf = buffer;

  for (int i = 0; i < 20; i++) {
    unsigned int val = *sha1++;
    *buf++ = hex[val >> 4];
    *buf++ = hex[val & 0xf];
  }
  *buf = '\0';

  return buffer;
}

void calc_object_sha1(const char *type, const void *body, unsigned long len,
                      unsigned char *sha1) {
  int hdrlen;
  char hdr[256];
  SHA_CTX c;

  sprintf(hdr, "%s %ld", type, len);
  hdrlen = strlen(hdr) + 1;

  SHA1_Init(&c);
  SHA1_Update(&c, hdr, hdrlen);
  SHA1_Update(&c, body, len);
  SHA1_Final(sha1, &c);
}

void calc_sha1(const void *body, unsigned long len, unsigned char *sha1) {
  SHA_CTX c;

  SHA1_Init(&c);
  SHA1_Update(&c, body, len);
  SHA1_Final(sha1, &c);
}
