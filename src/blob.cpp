//オブジェクトタイプ'blob' + ' ' + ファイルコンテンツの長さ + null文字'\0' +
//////ファイルコンテンツ
#include <cstdio>
#include <string>
#include <vector>

template <typename... Args>
std::string format(const std::string &fmt, Args... args) {
  size_t len = std::snprintf(nullptr, 0, fmt.c_str(), args...);
  std::vector<char> buf(len + 1);
  std::snprintf(&buf[0], len + 1, fmt.c_str(), args...);
  return std::string(&buf[0], &buf[0] + len);
}

class Blob {
public:
  std::string blob;
  Blob(std::string content) { blob = format("blob %s", content); };
};
