#include <experimental/filesystem>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "file_io.hpp"
#include "qrune.hpp"
#include "zlib.hpp"

namespace fs = std::experimental::filesystem;

class Qrune {
private:
  const char *git_index_file;

public:
  Qrune() { this->git_index_file = ".git/index"; }
};

static int write_buff(int fd, void const *buf, unsigned long len) {
  if (write_in_full(fd, (const char *)buf, len) < 0)
    exit(1);
  return 0;
}

void git_write_loose_object(const unsigned char *sha1, char *hdr, int hdrlen,
                            const void *buf, unsigned long len, time_t mtime) {
  int ret;
  int fd;
  unsigned char compressed[4096];
  git_zstream stream;

  FILE *fp;
  fp = fopen("/tmp/git_tmpfile", "wb");
  fd = fileno(fp);

  // Set up
  memset(&stream, 0, sizeof(stream));
  git_deflate_init(&stream, Z_BEST_SPEED);
  stream.next_out = compressed;
  stream.avail_out = sizeof(compressed);

  // header
  stream.next_in = (unsigned char *)hdr;
  stream.avail_in = hdrlen;
  while (git_deflate(&stream, 0) == Z_OK)
    ;

  // data
  stream.next_in = (unsigned char *)buf;
  stream.avail_in = len;
  do {
    ret = git_deflate(&stream, Z_FINISH);

    if (write_buff(fd, compressed, stream.next_out - compressed) < 0)
      printf("unable to write sha1 file");
    stream.next_out = compressed;
    stream.avail_out = sizeof(compressed);
  } while (ret == Z_OK);

  fclose(fp);

  auto filename = to_filename(sha1);
  fs::path file_path = filename;
  auto directory = file_path.remove_filename();
  if (!fs::exists(directory)) {
    mkdir(directory.c_str(), S_IRWXU | S_IRWXO | S_IRWXG);
  }

  rename("/tmp/git_tmpfile", filename.c_str());
  return;
}

int _rev_parse(const char *_rev, char *sha1_string) {
  char filename[256] = {};
  const char *dir = ".git/refs/heads/";
  char rev[100];
  // HEADの場合は強引にmasterに変更する
  if (strcmp(_rev, "HEAD") == 0) {
    strcpy(rev, "master");
  } else {
    strcpy(rev, _rev);
  }

  //引数が40文字なら、強引にsha1とみなす。
  if (strlen(rev) == 40) {
    strcpy(sha1_string, rev);
    return 0;
  }
  strcat(filename, dir);
  strcat(filename, rev);

  FILE *fp;

  if ((fp = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "_rev_parse:Can't open %s\n", filename);
    exit(1);
  }

  fgets(sha1_string, 41, fp);
  fclose(fp);

  return 0;
}
