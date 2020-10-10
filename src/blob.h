//オブジェクトタイプ'blob' + ' ' + ファイルコンテンツの長さ + null文字'\0' +
//////ファイルコンテンツ
#ifndef BLOB_H
#define BLOB_H
#include <cstddef>
#include <experimental/filesystem>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>

#include <boost/format.hpp>
#include <openssl/sha.h>

#include "file_io.h"
#include "object.h"
#include "sha.h"

#define GIT_ROOT "/home/yosuke/works/qrune/"

// void read(std::string path, std::string *content) {
// std::ifstream ifs(path);
// std::string buf;
// while (std::getline(ifs, buf)) {
//   *content += buf + "\n";
// };
// if (!ifs.eof()) {
//   std::exit(1);
// };
// }
;

void write(std::string o_id, std::string value) {
  auto d = o_id.substr(0, 2);
  auto f = o_id.substr(2, 38);
  std::string base = GIT_ROOT + std::string(".qrune/objects/") + d;
  mkdir(base.c_str(), S_IRWXU);
  write_line((base + "/" + f), value.c_str());
};

class Blob : public GitObject {
public:
  // Parent member
  std::string o_id;
  const char *type;

  std::string body;
  std::string value;
  std::string name;
  std::string absolute_path;
  std::experimental::filesystem::perms mode;
  std::uintmax_t size;

  Blob(std::string path) {
    type = "blob";
    std::string content;
    // read(path, &content);
    content.pop_back();
    body = content;
    absolute_path = std::experimental::filesystem::absolute(path);
    mode = std::experimental::filesystem::status(path).permissions();
    size = std::experimental::filesystem::file_size(path);
    name = std::experimental::filesystem::path(path).filename();
  };

  const char *call() { return type; };

  static Blob *create(const char *path) {
    Blob *blob = new Blob(path);
    blob->set_id();
    write(blob->o_id, blob->value);
    return blob;
  };

private:
  void set_id() { this->calc_hash(this->body, this->size); };
  void calc_hash(std::string b, auto size) {
    // auto hash = (boost::format("blob %1%%2%%3%") % size % "\0" % b).str();
    // this->value = hash;

    unsigned char buffer;
    calc_object_sha1(this->type, this->body.c_str(), this->body.length(),
                     &buffer);
    std::cout << buffer << std::endl;
    // std::stringstream ss;
    // hash_data_t::const_iterator itr = t.begin();
    // const hash_data_t::const_iterator end_itr = t.end();
    // for (; itr != end_itr; ++itr) {
    //   ss << ((*itr & 0xf0) >> 4) << (*itr & 0x0f);
    // }
    // this->o_id = ss.str();
  };
};

#endif
