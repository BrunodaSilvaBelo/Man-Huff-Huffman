#include "writer.h"

void write_header(std::ostream &stream, const table_t &table)
{
    typedef std::ostream::char_type char_type;
    typedef boost::dynamic_bitset<>::size_type size_type;

    for (const auto &e: table) {
        assert(e.second.size() > 0);

        auto ch = (char_type)(e.first);
        stream.put(ch);

        size_type nbits = e.second.size();
        size_type nbytes = nbits / 8 + ((nbits % 8) ? 1 : 0);

        ch = nbits;
        stream.put(ch);

        for (size_type i = 0 ; i != nbytes ; ++i) {
            size_type inner_loop_count = 8;
            bool last_byte = i == nbytes - 1;
            if (last_byte)
                inner_loop_count = nbits % 8;

            ch = '\0';
            for (size_type j = 0 ; j != inner_loop_count ; ++j) {
                size_type idx = i * 8 + j;
                idx = e.second.size() - 1 - idx;
                char_type c = static_cast<bool>(e.second[idx]);
                ch <<= 1;
                ch |= c;
            }
            if (last_byte)
                ch <<= 8 - inner_loop_count;
            stream.put(ch);
        }
    }

    stream.put('a');
    stream.put('\x00');
}
