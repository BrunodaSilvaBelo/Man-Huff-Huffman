#ifndef CODER_H
#define CODER_H

#include <map>
#include <boost/dynamic_bitset.hpp>
#include <cstdint>

std::map<std::uint8_t, boost::dynamic_bitset<>>
    codetable(std::map<std::uint8_t, unsigned int> frequency);

#endif /* CODER_H */
