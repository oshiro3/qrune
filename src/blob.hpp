//オブジェクトタイプ'blob' + ' ' + ファイルコンテンツの長さ + null文字'\0' +
//////ファイルコンテンツ
#ifndef BLOB_H_
#define BLOB_H_

#include <cstdio>
#include <sys/stat.h>

class Blob {
  char object_id[41];

public:
  Blob(auto o_id) { object_id = o_id; }
  static void create(const char *, struct stat64 *, unsigned char *);
};

#endif
