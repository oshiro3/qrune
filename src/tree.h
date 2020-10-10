#include <experimental/filesystem>
#include <string>
#include <vector>

#include "blob.h"
#include "object.h"

#define GIT_ROOT "/home/yosuke/works/qrune/"

typedef struct {
  std::string type;
  std::string name;
  std::string hash;
  int mode;
} Info;

std::string value_format(Info info) {
  std::stringstream ss;
  ss << std::oct << static_cast<int>(info.mode);
  return (boost::format("%1% %2% %3% %4%") % ss.str() % info.type % info.hash %
          info.name)
      .str();
};

void write_tree(auto tree) {
  auto d = tree->o_id.substr(0, 2);
  auto f = tree->o_id.substr(2, 38);
  std::string base = GIT_ROOT + std::string(".qrune/objects/") + d;

  mkdir(base.c_str(), S_IRWXU);

  std::ofstream ofs((base + "/" + f).c_str());
  if (!ofs) {
    std::cout << "err" << std::endl;
    std::exit(1);
  };
  // TODO: zlib
  for (auto i : tree->infos) {
    // TODO: mode が正しく出ていない
    ofs << std::oct << value_format(i) << std::endl;
  };
};

class Tree : public GitObject {
public:
  const char *type;
  std::string o_id;
  std::string value;
  std::string name;
  std::string hash;
  // std::experimental::filesystem::v1::perms mode;
  int mode;
  std::vector<Info> infos;
  Tree(const char *path) {
    type = TREE_TYPE;
    this->name =
        std::experimental::filesystem::path(path).parent_path().filename();
    this->mode = static_cast<int>(
        std::experimental::filesystem::status(path).permissions());
  };

  static Tree *create(auto type, auto entry, const char *path) {
    Tree *tree = new Tree(path);
    Info info = Info{type, entry.name, entry.o_id, entry.mode};
    std::cout << "hash: " << info.hash << std::endl;
    std::cout << "name: " << info.name << std::endl;
    std::cout << "mode: " << info.mode << std::endl;

    tree->infos.push_back(info);
    std::stringstream ss;
    for (auto i : tree->infos) {
      ss << value_format(i) << std::endl;
    };
    // tree->o_id = sha256(ss.str());
    write_tree(tree);

    return tree;
  };
};
