#ifndef INDEX_H
#define INDEX_H

// #include <boost/foreach.hpp>
// #include <boost/optional.hpp>
// #include <boost/property_tree/json_parser.hpp>
// #include <boost/property_tree/ptree.hpp>
//
// #include <chrono>
// #include <cstdlib>
// #include <ctime>
// #include <experimental/filesystem>
// #include <fstream>
// #include <iomanip>
// #include <iostream>
// #include <string>
// #include <vector>
//
// using namespace boost::property_tree;
//
// namespace fs = std::experimental::filesystem;
//
// class Entry {
// public:
//   std::string o_id;
//   std::string absolute_path;
//   std::string name;
//   int mode;
//
//   Entry(){};
//   Entry(const char *o_id, auto name, auto mode, auto path) {
//     this->o_id = o_id;
//     this->name = name;
//     this->absolute_path = path;
//     this->mode = static_cast<int>(mode);
//   };
//   std::string format() {
//     std::stringstream buf;
//     buf << std::oct << this->mode;
//     buf << " ";
//     buf << this->o_id;
//     buf << " ";
//     buf << this->name;
//     return buf.str();
//   };
// };
//
// // index は各blobについてmode/hash/status/相対パス の情報を持つ
// // status: 0->異常なし
// // index は Tree の情報を知る必要はない
// class Index {
// public:
//   std::vector<Entry> entries;
//
//   void update(Blob *blob) {
//     Entry *entry = new Entry(blob->o_id.c_str(), blob->name, blob->mode,
//                              blob->absolute_path);
//     entries.push_back(*entry);
//     ptree pt;
//     ptree child;
//     for (auto &e : entries) {
//       std::stringstream ss;
//       ss << std::oct << static_cast<int>(e.mode) << std::endl;
//       {
//         ptree entry;
//         entry.put("mode", ss.str());
//         entry.put("name", e.name);
//         entry.put("path", e.absolute_path);
//         entry.put("o_id", e.o_id);
//         child.push_back(std::make_pair("", entry));
//       }
//     };
//     pt.add_child("entries", child);
//     write_json(".git/index", pt);
//   };
//
//   static Index *get_current_index() {
//     Index *i = new Index;
//     ptree pt;
//     read_json(".git/index", pt);
//     BOOST_FOREACH (const ptree::value_type &child, pt.get_child("entries")) {
//       const ptree &info = child.second;
//       Entry e;
//       e.o_id = info.get_optional<std::string>("o_id").get();
//       e.mode = info.get_optional<int>("mode").get();
//       e.name = info.get_optional<std::string>("name").get();
//       e.absolute_path = info.get_optional<std::string>("path").get();
//       i->entries.push_back(e);
//     }
//     return i;
//     ;
//   };
// };
//
#endif
