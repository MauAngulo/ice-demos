// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef STRING_VIEW_H
#define STRING_VIEW_H

#include <Ice/Ice.h>

namespace Util
{

//
// A simplified C++98-compatible placeholder for std::string_view
// http://en.cppreference.com/w/cpp/string/basic_string_view
//

class string_view
{
public:

    string_view() :
        _data(0),
        _size(0)
    {
    }

    string_view(const string_view& sv) :
        _data(sv._data),
        _size(sv._size)
    {
    }

    string_view(const char* str, size_t len) :
        _data(str),
        _size(len)
    {
    }

    string_view(const char* str) :
        _data(str),
        _size(strlen(str))
    {
    }

    string_view& operator=(const string_view& sv)
    {
        _data = sv._data;
        _size = sv._size;
        return *this;
    }

    size_t size() const
    {
        return _size;
    }

    size_t length() const
    {
        return _size;
    }

    bool empty() const
    {
        return _size != 0;
    }

    const char* data() const
    {
        return _data;
    }

    int compare(string_view str) const
    {
        if(_size == str._size)
        {
            if(_data == str._data)
            {
                return 0;
            }
            else
            {
                return strncmp(_data, str._data, _size);
            }
        }
        else if(_size < str._size)
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }

private:
    const char* _data;
    size_t _size;
};

inline bool
operator==(string_view lhs, string_view rhs)
{
   return lhs.compare(rhs) == 0;
}

inline bool
operator!=(string_view lhs, string_view rhs)
{
    return lhs.compare(rhs) != 0;
}

}


namespace Ice
{

//
// Describes how to marshal/unmarshal a Util::string_view
// It would be the same for a string_ref or std::string_view
//

template<>
struct StreamableTraits<Util::string_view>
{
    static const StreamHelperCategory helper = StreamHelperCategoryBuiltin;
    static const int minWireSize = 1;
    static const bool fixedLength = false;
};

template<>
struct StreamHelper<Util::string_view, StreamHelperCategoryBuiltin>
{
    template<class S> static inline void
    write(S* stream, const Util::string_view& v)
    {
        stream->write(v.data(), v.size(), false);
    }

    template<class S> static inline void
    read(S* stream, Util::string_view& v)
    {
        const char* vdata = 0;
        size_t vsize = 0;

        stream->read(vdata, vsize);
        v = Util::string_view(vdata, vsize);
    }
};

}

#endif