#include <experimental/filesystem>
#include <string>
#include <vector>

#include "blob.hpp"
#include "index.hpp"
#include "sha.hpp"

#define GIT_ROOT "/home/yosuke/works/qrune/"

std::vector<uint8_t> hexstr2num(char *o_id) {
  std::string hex = std::string(o_id);
  std::vector<uint8_t> bytes;
  for (int i = 0; i < hex.length(); i += 2) {
    uint8_t byte;
    std::string nextbyte = hex.substr(i, 2);
    byte = static_cast<uint8_t>(strtol(nextbyte.c_str(), NULL, 16));
    bytes.push_back(byte);
  }
  return bytes;
}

class Content {
public:
  char *object_id;
  char *name;
  int mode;
  Content(char *o_id, char *name, int mode) {
    this->object_id = o_id;
    this->name = name;
    this->mode = mode;
  };
  void format(char *buf) {
    // std::vector<uint8_t> nums = hexstr2num(this->object_id);
    // char *buf;
    // unsigned int x;
    // char tmp[28 + strlen(this->name)] = "";
    sprintf(buf, "%d %s%s", 100644, this->name, this->object_id);
    // std::cout << "tmp: " << tmp << std::endl;
    // std::cout << "size: " << strlen(tmp) << std::endl;
  }
};

// forma: tree [content-length]\0[mode name\0Object_id]
class Tree {
public:
  std::vector<Content *> contents;
  char *object_id;
  Tree(){};

  static Tree *Create(std::vector<Entry> entries) {
    Tree *tree = new Tree();
    char *t;
    for (auto entry : entries) {
      Content *f = new Content(entry.object_id, entry.filepath, 100644);
      f->format(t);
      tree->contents.push_back(f);
    }
    unsigned char *t2 = (unsigned char *)t;
    unsigned char sha[20];
    calc_object_sha1("tree", t2, 36, sha);
    tree->object_id = sha1_to_hex(sha);
    return tree;
  };
};
