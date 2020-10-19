#ifndef FILE_IO_LIB_H
#define FILE_IO_LIB_H

int write_line(std::string, std::string);

std::string to_filename(const unsigned char *);

void write_hex(std::ofstream *, std::string);

// std::string bin_str_to_hex(std::string);

#endif
