#ifndef READER_H
#define READER_H

#include <map>
#include <cstdint>
#include <string>
#include "coder/coder.h"

#define compense(size) ((size > 8 || size % 8) ? (8 - (size % 8)) : 0)

std::map<uint8_t, unsigned int> read(std::string file);
table_t read_header(std::istream &stream);
#endif /* READER_H */
