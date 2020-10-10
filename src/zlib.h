#ifndef QRUNE_ZLIB_H_
#define QRUNE_ZLIB_H_

#include <zlib.h>
#define ZLIB_BUF_MAX ((uInt)1024 * 1024 * 1024) /* 1GB */
#define MAX_IO_SIZE (8 * 1024 * 1024)

// git より引用
typedef struct git_zstream {
  z_stream z;
  unsigned long avail_in;
  unsigned long avail_out;
  unsigned long total_in;
  unsigned long total_out;
  unsigned char *next_in;
  unsigned char *next_out;
} git_zstream;

static inline uInt zlib_buf_cap(unsigned long);

static int write_buff(int, void const *, unsigned long);

static void zlib_post_call(git_zstream *);

ssize_t xwrite(int, void const *, size_t);

ssize_t write_in_full(int, const char *, size_t);

static void zlib_pre_call(git_zstream *);

void git_deflate_init(git_zstream *, int);

int git_deflate(git_zstream *, int);

#endif
