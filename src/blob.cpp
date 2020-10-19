#include <fstream>
#include <string.h>
#include <string>
#include <sys/stat.h>

#include "blob.hpp"
#include "qrune.hpp"
#include "sha.hpp"

void Blob::create(const char *path, struct stat64 *st, unsigned char *sha1) {
  unsigned char *buf;
  void *b = operator new(st->st_size);
  buf = (unsigned char *)b;
  std::ifstream ifs(path);
  std::string buff;
  std::string buffer;
  while (std::getline(ifs, buff)) {
    buffer += buff + "\n";
  };
  buf = (unsigned char *)strdup(buffer.c_str());

  calc_object_sha1("blob", buf, st->st_size, sha1);
  char hdr[1024];
  char *obj_type = "blob";
  sprintf(hdr, "%s %ld", obj_type, (long)st->st_size);

  int hdrlen = strlen(hdr) + 1;
  git_write_loose_object(sha1, hdr, hdrlen, buf, st->st_size, 0);
};
