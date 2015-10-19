#define CATCH_CONFIG_MAIN
#include <fstream>
#include <stdexcept>
#include "catch.hpp"
#include "reader/reader.h"
#include "coder/coder.h"

#define arquivo "arquivo"

using namespace std;
using boost::dynamic_bitset;

TEST_CASE( "Reader tests", "[read]") {
    SECTION("Trying to read a no existent file")
        REQUIRE_THROWS(read("NoExiste"));
    {
        ofstream stream(arquivo);
        if ((stream.rdstate() & ios_base::failbit))
            throw runtime_error("[TEST] Cannot create the file!");

        SECTION("See if a empty file produces a empty map") {
            REQUIRE_NOTHROW(read(arquivo));
            map<uint8_t, unsigned int> empty;
            REQUIRE(read(arquivo) == empty);
        }

        char buffer[] = { '\x02', 'a', 'b', 'a', 'c', 'b' };
        stream.write(buffer, sizeof(buffer));

        stream.flush();
    }

    SECTION("See if a fill file produces the right map") {
        auto freq = read(arquivo);
        decltype(freq) hell {
            {'\x02', 1},
                {'a', 2},
                    {'b', 2},
                        {'c', 1}
        };
        REQUIRE(freq == hell);
    }
}

TEST_CASE("Coder test case", "[coder]") {
    map<uint8_t, unsigned int> frequency {
        {'m', 1},
            {'i', 5},
                {'s', 4},
                    {'p', 2},
                        {'r', 2},
                            {'v', 1},
                                {'e', 1},
                                    {'\x20', 1}
    };

    auto ret = codetable(frequency);
    decltype(ret) correct {
        {'i', dynamic_bitset<>{2, 0x0}},
            {'s', dynamic_bitset<>{2, 0x3}},
                {'p', dynamic_bitset<>{3, 0x2}},
                    {'r', dynamic_bitset<>{3, 0x6}},
                        {'m', dynamic_bitset<>{4, 0x5}},
                            {'v', dynamic_bitset<>{4, 0xd}},
                                {'e', dynamic_bitset<>{4, 0x9}},
                                    {'\x20', dynamic_bitset<>{4, 0x1}}
    };

    REQUIRE(ret.at('i').size() == correct.at('i').size());
    REQUIRE(ret.at('s').size() == correct.at('s').size());
    REQUIRE(ret.at('p').size() == correct.at('p').size());
    REQUIRE(ret.at('r').size() == correct.at('r').size());
    REQUIRE(ret.at('m').size() == correct.at('m').size());
    REQUIRE(ret.at('v').size() == correct.at('v').size());
    REQUIRE(ret.at('e').size() == correct.at('e').size());
    REQUIRE(ret.at('\x20').size() == correct.at('\x20').size());
}
