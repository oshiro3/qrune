#include <bitset>
#include <byteswap.h>
#include <cstring>
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>

#include "blob.h"
#include "file_io.h"
#include "qrune.h"
#include "zlib.h"

namespace fs = std::experimental::filesystem;

#define GIT_ROOT "/home/yosuke/works/qrune/"

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

// Blob *create_blob(const char *path) {
// fs::path p = fs::absolute(path);
// auto blob = Blob::create(path);
// p.remove_filename();
// // tree_update_b(blob, p);
// return blob;
// }
// ;
//
// Tree *tree_update_b(Entry blob, fs::path d) {
//   return Tree::create("blob", blob, d.c_str());
// };
//
// Tree *tree_update(Tree *object, fs::path d) {
//   object = Tree::create("TREE", *object, d.c_str());
//   if (!std::strcmp(d.c_str(), GIT_ROOT)) {
//     return object;
//   };
//   d.remove_filename();
//   return tree_update(object, d);
// };
//
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
;

void create_commit(const char *message) {
  char tree_sha1[41] = "decd3339b94705aefe6229c1b54150dc7f04c389";
  // char *message = argv[2];

  unsigned char new_sha1[21];
  char parent[41] = "";
  char buf[4096];
  char *author = "OSHIRO <oshiro3@example.com> 1601799532 +0900";
  char *commiter = "OSHIRO <oshiro3@example.com> 1601799532 +0900";
  _rev_parse("HEAD", parent);
  sprintf(buf, "tree %s\nparent %s\nauthor %s\ncommiter %s\n\n%s\n", tree_sha1,
          parent, author, commiter, message);
  size_t obj_size = strlen(buf) + 1;
  calc_object_sha1("commit", buf, obj_size, new_sha1);
  char hdr[1024];
  char *obj_type = "commit";
  sprintf(hdr, "%s %ld", obj_type, (long)obj_size);

  int hdrlen = strlen(hdr) + 1;
  git_write_loose_object(new_sha1, hdr, hdrlen, buf, obj_size, 0);
  write_line(".git/refs/heads/master", sha1_to_hex(new_sha1));
  printf("[master %s] %s\n", sha1_to_hex(new_sha1), message);
  // auto author = "oshiro_3";
  // auto commit = Commit::create(tree->o_id, message, author);
  // std::cout << commit->format() << std::endl;
}

// void update_index2(auto path, auto stat, auto sha1) {}
void update_index(auto path, auto stat, auto sha1) {

  char *indexfile = ".git/index2";

  char header[5] = "DIRC";

  char *bin;
  std::ofstream ofs(indexfile, std::ios::out | std::ios::binary);
  for (int i = 0; i < strlen(header); i++) {
    ofs.write(reinterpret_cast<char *>(&header[i]), sizeof(header[i]));
  }

  // ofs.write(*bin, 12);
  int ver = 2;
  auto ui = bswap_32(ver);
  ofs.write(reinterpret_cast<char *>(&ui), sizeof(4));
  auto entry_size = bswap_32(1);
  ofs.write(reinterpret_cast<char *>(&entry_size), sizeof(4));

  /* Entry */
  int ctime = bswap_32(stat.st_ctime);
  int mtime = bswap_32(stat.st_mtime);
  int ctime_nano = 953541800;
  int mtime_nano = 963587600;

  int dev = bswap_32(stat.st_dev);
  int ino = bswap_32(stat.st_ino);
  int mode = bswap_32(stat.st_mode);
  int uid = bswap_32(stat.st_uid);
  int gid = bswap_32(stat.st_gid);
  int size = bswap_32(stat.st_size);
  short zero = 0;
  // bin = reinterpret_cast<char *>(&ctime);
  // ss.write(reinterpret_cast<char *>(&ctime), sizeof(4));

  // std::cout << sizeof(m.mont) << std::endl;
  // std::cout << sha1 << std::endl;
  // std::cout << sizeof(sha1) << std::endl;
  // std::cout << sha1_to_hex(sha1) << std::endl;
  // std::cout << sizeof(sha1_to_hex(sha1)) << std::endl;
  // ofs.write(bin, sizeof(4));
  ofs.write(reinterpret_cast<char *>(&ctime), sizeof(4));
  write_hex(&ofs, std::string("396f2e10"));
  // ofs.write(reinterpret_cast<char *>(&zero), sizeof(4));
  // ofs.write(reinterpret_cast<char *>(&ctime_nano), sizeof(4));
  ofs.write(reinterpret_cast<char *>(&mtime), sizeof(4));
  write_hex(&ofs, std::string("38d5e4a8"));
  // ofs.write(reinterpret_cast<c>(&zero8), sizeof(4));
  // ofs.write(reinterpret_cast<char *>(&mtime_nano), sizeof(4));
  ofs.write(reinterpret_cast<char *>(&dev), sizeof(4));
  ofs.write(reinterpret_cast<char *>(&ino), sizeof(4));
  ofs.write(reinterpret_cast<char *>(&mode), sizeof(4));
  ofs.write(reinterpret_cast<char *>(&uid), sizeof(4));
  ofs.write(reinterpret_cast<char *>(&gid), sizeof(4));
  ofs.write(reinterpret_cast<char *>(&size), sizeof(4));
  write_hex(&ofs, std::string(sha1_to_hex(sha1)));
  // ofs.write(reinterpret_cast<char *>(&f), sizeof(1));

  std::bitset<16> flags = 0;
  flags = flags <<= 15;
  flags = flags | std::bitset<16>(8);
  std::string sss = flags.to_string();
  std::string te = bin_str_to_hex(sss);

  write_hex(&ofs, te);
  std::string p = std::string(path);
  for (int i = 0; i < p.length(); i++) {
    ofs.write(reinterpret_cast<char *>(&p[i]), sizeof(p[i]));
  }
  ofs.write(reinterpret_cast<char *>(&zero), sizeof(zero));

  std::string buf = "test";

  unsigned char index_sha1[41];
  calc_sha1(&buf, sizeof(buf), index_sha1);
  auto index_sha1_hex = sha1_to_hex(index_sha1);
  std::cout << "hex: " << index_sha1_hex << std::endl;
  index_sha1_hex = "a381625508a87f5a27c7df7e9f85ec9366dcb07d";
  std::cout << "hex: " << index_sha1_hex << std::endl;
  write_hex(&ofs, index_sha1_hex);
}

int add(const char *path) {
  unsigned char sha1[41];
  struct stat st;
  lstat(path, &st);
  Blob::create(path, &st, sha1);
  update_index(path, st, sha1);
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
