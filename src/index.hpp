#ifndef INDEX_H
#define INDEX_H

#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

#define INDEX_FILE_PATH ".git/index"

class Entry {
public:
  int ctime;
  int ctime_nano;
  int mtime;
  int mtime_nano;
  int dev;
  int ino;
  int mode;
  int uid;
  int gid;
  int filesize;
  char *object_id;
  unsigned char *object_id_b;
  // bool flag;
  short filepath_length;
  char *filepath;
  Entry(int ctime, int ctime_nano, int mtime, int mtime_nano, int dev, int ino,
        int mode, int uid, int gid, short filesize, char *object_id,
        short filepath_length, char *filepath, unsigned char *id_b) {
    this->ctime = ctime;
    this->ctime_nano = ctime_nano;
    this->mtime = mtime;
    this->mtime_nano = mtime_nano;
    this->dev = dev;
    this->ino = ino;
    this->mode = mode;
    this->uid = uid;
    this->gid = gid;
    this->filesize = filesize;
    this->object_id = object_id;
    this->filepath_length = filepath_length;
    this->filepath =
        strdup(filepath); // pointer を更新しないと参照先が壊れる場合がある
    this->object_id_b = id_b;
  }
};

class Index {
public:
  std::vector<Entry> entries;
  Index() {
    char *indexfile = INDEX_FILE_PATH;
    std::ifstream fin(indexfile, std::ios::in | std::ios::binary);
    if (!fin) {
      return;
    }

    char d[5];
    int ver, entry_size, ctime, ctime_nano, mtime, mtime_nano, dev, ino, mode,
        uid, gid, size;
    short flags;
    char *blob_id;

    fin.seekg(0);
    fin.read((char *)&d, 4);

    fin.read((char *)&ver, 4);
    ver = bswap_32(ver);

    fin.read((char *)&entry_size, 4);
    entry_size = bswap_32(entry_size);

    fin.read((char *)&ctime, 4);
    ctime = bswap_32(ctime);

    fin.read((char *)&ctime_nano, 4);
    ctime_nano = bswap_32(ctime_nano);

    fin.read((char *)&mtime, 4);
    mtime = bswap_32(mtime);

    fin.read((char *)&mtime_nano, 4);
    mtime_nano = bswap_32(mtime_nano);

    fin.read((char *)&dev, 4);
    dev = bswap_32(dev);

    fin.read((char *)&ino, 4);
    ino = bswap_32(ino);

    fin.read((char *)&mode, 4);
    mode = bswap_32(mode);

    fin.read((char *)&uid, 4);
    uid = bswap_32(uid);

    fin.read((char *)&gid, 4);
    gid = bswap_32(gid);

    fin.read((char *)&size, 4);
    size = bswap_32(size);

    unsigned char tmp[20] = "";
    fin.read((char *)&tmp, 20);
    blob_id = sha1_to_hex(tmp);

    fin.read((char *)&flags, 2);
    flags = bswap_16(flags);

    char path[flags + 1] = "";
    fin.read((char *)&path, flags);
    path[flags] = '\0';

    fin.close();

    Entry entry = Entry(ctime, ctime_nano, mtime, mtime_nano, dev, ino, mode,
                        uid, gid, size, blob_id, flags, path, tmp);
    entries.push_back(entry);
  }
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
    ofs.open(indexfile, std::ios::out | std::ios::binary | std::ios::app);
    for (int i = 0; i < 20; i++) {
      ofs.write(reinterpret_cast<char *>(&index_sha1[i]), 1);
    }
  }
};
#endif
