#include <cstdlib>
#include <string.h>

#include "zlib.hpp"

static inline uInt zlib_buf_cap(unsigned long len) {
  return (ZLIB_BUF_MAX < len) ? ZLIB_BUF_MAX : len;
}

static void zlib_post_call(git_zstream *s) {
  unsigned long bytes_consumed;
  unsigned long bytes_produced;

  bytes_consumed = s->z.next_in - s->next_in;
  bytes_produced = s->z.next_out - s->next_out;

  s->total_out = s->z.total_out;
  s->total_in = s->z.total_in;
  s->next_in = s->z.next_in;
  s->next_out = s->z.next_out;
  s->avail_in -= bytes_consumed;
  s->avail_out -= bytes_produced;
}

ssize_t xwrite(int fd, const void *buf, size_t len) {
  ssize_t nr;
  if (len > MAX_IO_SIZE)
    len = MAX_IO_SIZE;
  while (1) {
    nr = write(fd, buf, len);
    if ((nr < 0))
      continue;
    return nr;
  }
}

ssize_t write_in_full(int fd, const char *buf, size_t count) {
  const char *p = buf;
  ssize_t total = 0;

  while (count > 0) {
    ssize_t written = xwrite(fd, p, count);
    if (written < 0)
      return -1;
    if (!written) {
      // errno = 777;
      return -1;
    }
    count -= written;
    p += written;
    total += written;
  }

  return total;
}

static void zlib_pre_call(git_zstream *s) {
  s->z.next_in = s->next_in;
  s->z.next_out = s->next_out;
  s->z.total_in = s->total_in;
  s->z.total_out = s->total_out;
  s->z.avail_in = zlib_buf_cap(s->avail_in);
  s->z.avail_out = zlib_buf_cap(s->avail_out);
}

void git_deflate_init(git_zstream *strm, int level) {
  int status;

  memset(strm, 0, sizeof(*strm));
  zlib_pre_call(strm);
  status = deflateInit(&strm->z, level);
  zlib_post_call(strm);
  if (status == Z_OK)
    return;
}

int git_deflate(git_zstream *strm, int flush) {
  int status;

  for (;;) {
    zlib_pre_call(strm);

    /* Never say Z_FINISH unless we are feeding everything */
    status =
        deflate(&strm->z, (strm->z.avail_in != strm->avail_in) ? 0 : flush);
    zlib_post_call(strm);

    /*
     * Let zlib work another round, while we can still
     * make progress.
     */
    if ((strm->avail_out && !strm->z.avail_out) &&
        (status == Z_OK || status == Z_BUF_ERROR))
      continue;
    break;
  }

  switch (status) {
  /* Z_BUF_ERROR: normal, needs more space in the output buffer */
  case Z_BUF_ERROR:
  case Z_OK:
  case Z_STREAM_END:
    return status;
  default:
    break;
  }
  return status;
}
