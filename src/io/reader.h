#ifndef READER_H
#define READER_H

#include <map>
#include <cstdint>
#include <string>
#include "coder/coder.h"

std::map<uint8_t, unsigned int> read(std::string file);
table_t read_header(std::istream &stream);
#endif /* READER_H */
