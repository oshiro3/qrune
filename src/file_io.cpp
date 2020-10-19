#include <fstream>
#include <iostream>
#include <sstream>

#include "file_io.hpp"
#include "sha.hpp"

std::string to_filename(const unsigned char *sha1) {
  const char *objdir = ".git/objects";
  std::string filename;
  std::string str_sha1 = (std::string)sha1_to_hex(sha1);
  auto dir = str_sha1.substr(0, 2);
  auto file = str_sha1.substr(2, 38);
  filename = ".git/objects/" + dir + "/" + file + "\0";
  return filename;
}

int write_line(auto filename, auto body) {
  std::ofstream ofs(filename, std::ios::trunc);
  if (!ofs) {
    std::cout << "err" << std::endl;
    return 1;
  };
  ofs << body << std::endl;
  return 0;
}

void write_hex(std::ofstream *ofs, std::string hex) {
  std::basic_string<uint8_t> bytes;

  for (size_t i = 0; i < hex.length(); i += 2) {
    uint8_t byte;
    std::string nextbyte = hex.substr(i, 2);
    std::istringstream(nextbyte) >> std::hex >> byte;

    bytes.push_back(static_cast<uint8_t>(byte));
  }

  std::string result(begin(bytes), end(bytes));
  if (ofs->is_open()) {
    *ofs << result;
  } else {
    std::cout << "Error could not create file." << std::endl;
  };
}
