#include <cassert>
#include <fstream>
#include <stdexcept>
#include <iterator>
#include "reader.h"

using namespace std;

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
        throw runtime_error("File do not exist");

    map<uint8_t, unsigned int> map;

    for (auto character : Reader(stream)) {
        ++map[character];
    }

    stream.close();
    return map;
}
