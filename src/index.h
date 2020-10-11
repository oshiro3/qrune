#ifndef INDEX_H
#define INDEX_H

#include <cstdlib>
#include <fstream>
#include <string>

#define INDEX_FILE_PATH ".git/index2"

class Entry {
public:
};

class Index {
public:
  static void update(const char *path, struct stat64 *stat,
                     unsigned char *blob_id) {
    char *indexfile = INDEX_FILE_PATH;
    char header[5] = "DIRC";
    std::ofstream ofs(indexfile, std::ios::out | std::ios::binary);
    for (int i = 0; i < strlen(header); i++) {
      ofs.write(reinterpret_cast<char *>(&header[i]), sizeof(header[i]));
    }

    int ver = 2;
    auto ui = bswap_32(ver);
    ofs.write(reinterpret_cast<char *>(&ui), sizeof(4));
    auto entry_size = bswap_32(1);
    ofs.write(reinterpret_cast<char *>(&entry_size), sizeof(4));

    /* Entry */
    int ctime = bswap_32(stat->st_ctime);
    int mtime = bswap_32(stat->st_mtime);
    int ctime_nano = bswap_32(stat->st_ctim.tv_nsec);
    int mtime_nano = bswap_32(stat->st_mtim.tv_nsec);

    int dev = bswap_32(stat->st_dev);
    int ino = bswap_32(stat->st_ino);
    int mode = bswap_32(stat->st_mode);
    int uid = bswap_32(stat->st_uid);
    int gid = bswap_32(stat->st_gid);
    int size = bswap_32(stat->st_size);
    short zero = 0;

    ofs.write(reinterpret_cast<char *>(&ctime), sizeof(4));
    ofs.write(reinterpret_cast<char *>(&ctime_nano), sizeof(4));
    ofs.write(reinterpret_cast<char *>(&mtime), sizeof(4));
    ofs.write(reinterpret_cast<char *>(&mtime_nano), sizeof(4));

    ofs.write(reinterpret_cast<char *>(&dev), sizeof(4));
    ofs.write(reinterpret_cast<char *>(&ino), sizeof(4));
    ofs.write(reinterpret_cast<char *>(&mode), sizeof(4));
    ofs.write(reinterpret_cast<char *>(&uid), sizeof(4));
    ofs.write(reinterpret_cast<char *>(&gid), sizeof(4));
    ofs.write(reinterpret_cast<char *>(&size), sizeof(4));

    for (int i = 0; i < 20; i++) {
      ofs.write(reinterpret_cast<char *>(&blob_id[i]), 1);
    }

    // assume-valid flag, extended flag, stage を0にする
    //
    // std::bitset<16> flags = 0;
    // flags = flags <<= 15;
    // flags = flags | std::bitset<16>(8);
    // std::string sss = flags.to_string();
    // std::string te = bin_str_to_hex(sss);
    // write_hex(&ofs, te);
    short filename_length = bswap_16(strlen(path));
    ofs.write(reinterpret_cast<char *>(&filename_length), 2);

    char p[strlen(path) + 1];
    strcpy(p, path);
    for (int i = 0; i < strlen(path); i++) {
      ofs.write(reinterpret_cast<char *>(&p[i]), 1);
    }
    ofs.write(reinterpret_cast<char *>(&zero), 2);
    ofs.close();

    struct stat s;
    lstat(indexfile, &s);
    unsigned char buf[s.st_size];
    unsigned char index_sha1[41];
    FILE *ptr;
    ptr = fopen(indexfile, "rb");
    fread(buf, sizeof(buf), 1, ptr);

    calc_sha1(&buf, sizeof(buf), index_sha1);
    auto index_sha1_hex = sha1_to_hex(index_sha1);
    std::cout << "index hex: " << index_sha1_hex << std::endl;
    ofs.open(indexfile, std::ios::out | std::ios::binary | std::ios::app);
    for (int i = 0; i < 20; i++) {
      ofs.write(reinterpret_cast<char *>(&index_sha1[i]), 1);
    }
  }
};
#endif
