#include <bitset>
#include <byteswap.h>
#include <cstring>
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>

#include "blob.h"
#include "commit.h"
#include "file_io.h"
#include "index.h"
#include "qrune.h"
#include "zlib.h"

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

// Tree *create_trees() {
// auto i = Index::get_current_index();
// // TODO: commit 時の tree の実装中
// // tree 作成に必要な情報は entry に格納して渡す
// Tree *root_tree;
// for (auto &blob : i->entries) {
//   // blob のパスをパースしてディレクトリに分解してからtree作成に移る->
//   staging
//       // を実装して上記処理をstagingを利用して実装
//       auto tree = tree_update_b(blob, blob.absolute_path);
//   fs::path p = fs::path(blob.absolute_path);
//   root_tree = tree_update(tree, p.remove_filename().remove_filename());
// }
// return root_tree;
// }

void create_commit(const char *message) {
  char tree_sha1[41] = "decd3339b94705aefe6229c1b54150dc7f04c389";
  Commit::create(message, tree_sha1);
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
  // create_trees();
  // create_commit(root_tree, message);
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
