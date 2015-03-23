#define CATCH_CONFIG_MAIN
#include <fstream>
#include <stdexcept>
#include "catch.hpp"
#include "reader/reader.h"

#define arquivo "arquivo"

using namespace std;

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
