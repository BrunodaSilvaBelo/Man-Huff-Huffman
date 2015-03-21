#ifndef READER_H
#define READER_H

#include <map>
#include <cstdint>
#include <string>

std::map<uint8_t, unsigned int> read(std::string file);

#endif /* READER_H */
