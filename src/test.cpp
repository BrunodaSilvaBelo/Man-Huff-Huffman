#define CATCH_CONFIG_MAIN
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "catch.hpp"
#include "io/reader.h"
#include "io/writer.h"
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

TEST_CASE( "Writer header tests", "[write]") {
    SECTION("Simplest writer of all")
        {
            ostringstream stream;
            table_t table{
                {'a', dynamic_bitset<>{9, 0x1280 >> compense(9)}}
            };
            write_header(stream, table);
            auto out = stream.str();
            REQUIRE(out.size() == 6);

            REQUIRE(out.at(0) == 'a');
            REQUIRE(out.at(1) == '\x09');
            REQUIRE(out.at(2) == '\x12');
            REQUIRE(out.at(3) == '\x80');
            REQUIRE(out.at(5) == '\x00');
        }
}

TEST_CASE("Reader header tests", "[read]") {
    stringstream ss;
    SECTION("Table with no character") {
        ss.str("a\x00");

        table_t table;
        auto ret = read_header(ss);

        REQUIRE(ret == table);
    }
    SECTION("Table with a character") {
        ss.str("a\x09\x12\x80\x0b\x00");

        table_t table {
            {'a', dynamic_bitset<>{9, 0x1280 >> compense(9)}}
        };
        auto ret = read_header(ss);

        REQUIRE(ret == table);
    }
    SECTION("Table with 2 character, one with 6 bits, and another with 9 bits") {
        ss.str("a\x09\x12\x80\x69\x06\x2f\x0d\x00");

        table_t table {
            {'a', dynamic_bitset<>{9, 0x1280 >> compense(9)}},
                {'\x69', dynamic_bitset<>{6, 0x2f >> compense(6)}}
        };
            auto ret = read_header(ss);

            REQUIRE(ret == table);
    }
    SECTION("Table with 1 character with 8 bits") {
        ss.str("a\x08\x07\x05\x00");
        table_t table {
            {'a', dynamic_bitset<>{8, 0x7 >> compense(8)}}
        };

        auto ret = read_header(ss);

        REQUIRE(ret == table);
    }
    SECTION("Table with 2 character, one with 8 bits, other with 16 bit") {
        ss.str("a\x08\x07\x05\x75\x10\x1f\xff\x04\x00");
        table_t table {
            {'a', dynamic_bitset<>{8, 0x7 >> compense(8)}}
        };

        auto ret = read_header(ss);

        REQUIRE(ret == table);
    }
    SECTION("Table with 3 character, one with 8 bits, other with 16 bit and"
            " other with 1 bit") {
        ss.str("a\x08\x07\x05\x75\x10\x1f\xff\x74\x01\x01\x04\x00");
        table_t table {
            {'a', dynamic_bitset<>{8, 0x7 >> compense(8)}}
        };

        auto ret = read_header(ss);

        REQUIRE(ret == table);
    }
}

TEST_CASE("Integration test for reader and writer", "[read][write]") {
    SECTION("Table with 1 character with 9 bits"){
        stringstream stream;

        table_t table {
            {'a', dynamic_bitset<>{9, 0x1280 >> compense(9)}}
        };

        write_header(stream, table);

        auto ret = read_header(stream);

        REQUIRE(ret == table);
    }
}
