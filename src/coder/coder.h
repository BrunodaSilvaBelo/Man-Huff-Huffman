#ifndef CODER_H
#define CODER_H

#include <map>
#include <boost/dynamic_bitset.hpp>
#include <cstdint>

typedef std::map<std::uint8_t, boost::dynamic_bitset<>> table_t;

table_t codetable(std::map<std::uint8_t, unsigned int> frequency);

#endif /* CODER_H */
