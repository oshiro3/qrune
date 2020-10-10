#ifndef OBJECT_H
#define OBJECT_H

#include <stdio.h>
#include <string>
#include <sys/stat.h>

#include <boost/format.hpp>

#define TREE_TYPE "TREE"
#define BLOB_TYPE "BLOB"
#define COMMIT_TYPE "COMMIT"

class GitObject {
public:
  std::string o_id;
  const char *type;
};

#endif
