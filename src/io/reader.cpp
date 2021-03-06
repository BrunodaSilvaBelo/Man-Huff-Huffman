#include <cassert>
#include <fstream>
#include <stdexcept>
#include <iterator>
#include <deque>
#include <utility>
#include "reader.h"

using namespace std;
using boost::dynamic_bitset;

template<class T, class CharT = char, class Traits = char_traits<CharT>,
         class Distance = ptrdiff_t >
struct readeriterator
    : public iterator<input_iterator_tag, T, Distance, const T*, const T&>
{
public:
    readeriterator() = default;

    readeriterator(basic_istream<CharT, Traits> &stream) :
        stream(&stream)
    {
        stream.seekg(0, ios_base::end);
        file_size = stream.tellg();
        if (file_size)
            fill();
        else
            this->stream = NULL;
    }

    readeriterator &operator++()
    {
        assert(stream);
        pos += 1;
        if (pos == file_size)
            stream = NULL;
        else
            fill();
        return *this;
    }

    bool operator==(const readeriterator &rhs)
    {
        return (stream && rhs.stream) ? (pos == rhs.pos)
            : (stream == rhs.stream);
    }

    bool operator!=(const readeriterator &rhs)
    {
        return !(*this == rhs);
    }

    T operator*() const
    {
        return current.value;
    }

private:
    void fill()
    {
        stream->seekg(pos);
        for (unsigned i = 0;i != sizeof(T);++i) {
            char c;
            stream->read(&c, 1);
            current.pieces[i] = c;
        }
    }

    basic_istream<CharT, Traits> *stream = NULL;
    typename basic_istream<CharT, Traits>::pos_type pos = 0;
    typename basic_istream<CharT, Traits>::pos_type file_size;
    mutable union
    {
        T value;
        uint8_t pieces[sizeof(T)];
    } current;
};

class Reader {
public:
    Reader(istream &stream);
    readeriterator<uint8_t> begin();
    readeriterator<uint8_t> end();
private:
    readeriterator<uint8_t> begin_;
};

Reader::Reader(istream &stream)
    : begin_(stream) {
}

readeriterator<uint8_t> Reader::begin() {
    return begin_;
}

readeriterator<uint8_t> Reader::end() {
    return {};
}

map<uint8_t, unsigned int> read(string file) {
    ifstream stream(file);
    if (stream.rdstate() & ios_base::failbit)
        throw runtime_error("File do not exist!");

    map<uint8_t, unsigned int> map;

    for (auto character : Reader(stream)) {
        ++map[character];
    }

    stream.close();
    return map;
}

table_t read_header(istream &stream) {
    table_t map;
    char c;
    deque<uint8_t> attr;
    while (stream.get(c)) {
        attr.push_back(c);

        if (attr.size() == 2) {
            if ((int) attr.at(1) == 0)
                break;
        } else if (attr.size() >= 3) {
            if ((attr.size() - 2) != (((unsigned long) attr.at(1)) / 8)
                + ((unsigned long)(((int) attr.at(1)) % 8) ? 1 : 0))
                continue;
            auto character = (uint8_t) attr.front();
            attr.pop_front();
            auto size = (int) attr.front();
            attr.pop_front();

            dynamic_bitset<> db;

            for (int i = attr.size() - 1; i >= 0; --i) {
                auto byte = attr.back();
                attr.pop_back();

                for (int j = 0; j < 8; ++j)
                    db.push_back((byte >> j) & 0x1);
            }

            db = db >> compense(size);
            db.resize(size);
            map.insert(make_pair(character, db));
        }
    }

    return map;
}
