#pragma once

#include <stdint.h>
#include <stddef.h>
#include <new>

#define assert(c) if (!(c)) __builtin_trap()

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


/********************** Arena ****************************/
struct Arena
{
    Byte * beg;
    Byte * end;

    inline Arena(Byte * start, Size capacity):
        beg(start),
        end(start+capacity) {}
};

template <typename T, typename ...A>
T * make(Arena * arena, Size count = 1, A ...args)
{
    Size size = sizeof(T);
    Size align = alignof(T);
    Size pad = (align - 1) & (Size)arena->end;
    
    assert(count >= 0);
    assert((arena->end - arena->beg - pad) >= size * count);
    arena->end -= size * count + pad;
    T * r = (T *)arena->end;
    for (int i = 0; i < count; ++i) {
        new((void *)&r[i]) T(args...);
    }
    return r;
}

/********************* Strings ***************************/
struct S8
{
    U8 * val;
    Size len;

    S8() = default;

    template <Size N>
    S8(char const (&s)[N]):
        val((U8 *)s),
        len(N-1)
    {
    }
};


struct StrPair
{
    S8 * beg;
    S8 * end;

    StrPair(S8 * beg, S8 * end):
        beg(beg),
        end(end)
    {
    }
};

struct StrList
{
    S8 val = "";
    StrList * next = nullptr;
};


void strlist_append(StrList * list, Arena * arena, S8 str);

/**************** Buffers ***************************/
template <typename T>
struct Buffer
{
    T * val = 0;
    Size len = 0;
    Size cap = 0;

    T & operator[](std::size_t pos) {
        assert(pos < len);
        return val[pos];
    }

    Buffer(Arena * arena, Size size):
        len(0),
        cap(size)
    {
        val = make<T>(arena, size);
    }
};

template <typename T>
void buffer_append(Buffer<T> * buf, Arena * arena, T * value)
{
    //TODO handle full buffer
    buf[buf.len++] = value;
}

/**************** Platform Code *********************/
struct PlatformFD {
    int fd;
};

PlatformFD platform_stdout();
PlatformFD platform_stderr();

struct PlatformWritten
{
    static int const ERR_MSG_CAP = 256;

    Size bytes_written = 0;
    B32 ok = false;
};

PlatformWritten platform_write(PlatformFD * pfd, S8 msg);

int print(S8 msg, PlatformFD fd = platform_stdout());
int print(StrList strlist, PlatformFD fd = platform_stdout());
