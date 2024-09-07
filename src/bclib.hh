#pragma once

#include <stdint.h>
#include <stddef.h>

using I64 = int64_t;
using I32 = int32_t;
using I8 = int8_t;
using Byte = char;

using F32 = float;
using F64 = double;
using F128 = long double;

using U64 = uint64_t;
using U32 = uint32_t;
using U8 = uint8_t;

using Size = ptrdiff_t;
using Usize = size_t;

using B32 = int32_t;

#define assert(c) do {if (!(c) __debug_break()} while(0)

typedef<typename T>
struct Str
{
    T * v;
    Size len;
};

struct StrPair
{
    Str * beg;
    Str * end;
};

struct StrList
{
    Str * v;
    StrList * next;
};

struct Arena
{
    Byte * beg;
    Byte * end;

    inline Arena(Byte * start, Size capacity):
        beg(start),
        end(start+capacity) {}
};

template <typename T>
T * make(Arena * arena, U32 count = 1)
{
    Size size = sizeof T;
    Size align = alignof T;
    Size pad = (size - 1) & arena.end;
    
    assert((arena.end - arena.beg - pad) >= size * count);
    arena.end -= size * count + pad;
    return (T *)arena.end;
}

