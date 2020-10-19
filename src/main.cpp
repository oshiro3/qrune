#include <bitset>
#include <byteswap.h>
#include <cstdlib>
#include <cstring>
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>

#include "blob.hpp"
#include "commit.hpp"
#include "file_io.hpp"
#include "index.hpp"
#include "qrune.hpp"
#include "tree.hpp"
#include "zlib.hpp"

namespace fs = std::experimental::filesystem;

#define GIT_ROOT "~/works/qrune/"

int write_line(std::string filename, std::string body) {
  std::ofstream ofs(filename, std::ios::trunc);
  if (!ofs) {
    std::cout << "err" << std::endl;
    return 1;
  };
  ofs << body << std::endl;
  return 0;
}

std::string bin_str_to_hex(const std::string &s) {
  std::string out;
  for (uint i = 0; i < s.size(); i += 4) {
    int8_t n = 0;
    for (uint j = i; j < i + 4; ++j) {
      n <<= 1;
      if (s[j] == '1')
        n |= 1;
    }

    if (n <= 9)
      out.push_back('0' + n);
    else
      out.push_back('A' + n - 10);
  }

  return out;
}

#define INDEX_FILE_PATH ".git/index"
char *create_trees(Index *index) {
  Tree *t = Tree::Create(index->entries);
  return t->object_id;
}

void create_commit(char *tree_id, const char *message) {
  Commit::create(message, tree_id);
}

int add(const char *path) {
  unsigned char sha1[41];
  struct stat64 st;
  lstat64(path, &st);
  Blob::create(path, &st, sha1);
  Index::update(path, &st, sha1);
  return 0;
}

int commit(const char *message) {
  Index *i = new Index();
  char *tree_id = create_trees(i);
  create_commit(tree_id, message);
  return 0;
}

int main(int argc, char *argv[]) {
  if (!strcmp(argv[1], "add")) {
    add(argv[2]);
  } else if (!strcmp(argv[1], "commit")) {
    return commit(argv[2]);
  } else {
    std::exit(1);
  }
}
