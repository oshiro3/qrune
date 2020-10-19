#ifndef COMMIT_H
#define COMMIT_H

#include <ctime>
#include <string>

#include "file_io.hpp"
#include "qrune.hpp"
#include "sha.hpp"

class Commit {
public:
  static void create(const char *message, auto root_tree_id) {
    unsigned char new_sha1[21];
    char parent[41] = "";
    char buf[4096];
    time_t current_time = time(NULL);
    char author[128];
    char commiter[128];
    sprintf(author, "OSHIRO <oshiro3@example.com> %ld +0900", current_time);
    sprintf(commiter, "OSHIRO <oshiro3@example.com> %ld +0900", current_time);
    _rev_parse("HEAD", parent);
    sprintf(buf, "tree %s\nparent %s\nauthor %s\ncommiter %s\n\n%s\n",
            root_tree_id, parent, author, commiter, message);
    size_t obj_size = strlen(buf) + 1;
    calc_object_sha1("commit", buf, obj_size, new_sha1);
    char hdr[1024];
    char *obj_type = "commit";
    sprintf(hdr, "%s %ld", obj_type, (long)obj_size);

    int hdrlen = strlen(hdr) + 1;
    git_write_loose_object(new_sha1, hdr, hdrlen, buf, obj_size, 0);
    write_line(".git/refs/heads/master", sha1_to_hex(new_sha1));
    std::cout << "[master " << sha1_to_hex(new_sha1) << "] " << message
              << std::endl;
  }
};

#endif
