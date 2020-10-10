#ifndef COMMIT_H
#define COMMIT_H

#include <string>
#include <time.h>

#include "file_io.h"
#include "sha.h"

class Commit {
public:
  //   // Parent member
  //   std::string o_id;
  //   const char *type;
  //   std::string tree_id;
  //   std::string commiter;
  //   std::string author;
  //   std::string message;
  //   time_t created_at;
  //
  //   Commit(auto tree_id, auto message, auto author) {
  //     this->type = "COMMIT";
  //     this->tree_id = tree_id;
  //     this->commiter = author;
  //     this->author = author;
  //     this->message = message;
  //     time(&this->created_at);
  //   };
  //
  //   static Commit *create(auto tree_id, auto message, auto author) {
  //     auto commit = new Commit(tree_id, message, author);
  //     // commit->o_id = calc_sha1("blob", );
  //     auto d = ".git/objects/" + commit->o_id.substr(0, 2);
  //     mkdir(d.c_str(), S_IRWXU);
  //     write_line(commit->o_id.substr(2, 38), commit->format());
  //     return commit;
  //   };
  //
  //   std::string format() {
  //     return (boost::format("tree %1%\nauthor %2% %3%\ncommiter %4%
  //     %5%\n\n%6%") %
  //             this->tree_id % this->author % this->created_at %
  //             this->commiter % this->created_at % this->message)
  //         .str();
  //   };
};

#endif
