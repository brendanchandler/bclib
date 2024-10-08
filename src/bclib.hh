#pragma once

#include <cstdint>
#include <cstddef>
#include <new>
#include <trace.hh>

#define bc_assert(c) if (!(c)) __builtin_trap()

using I64 = int64_t;
using I32 = int32_t;
using I8 = int8_t;
using Byte = unsigned char;

using F32 = float;
using F64 = double;
using F128 = long double;

using U64 = uint64_t;
using U32 = uint32_t;
using U8 = uint8_t;

using Size = ptrdiff_t;
using USize = size_t;

using B32 = int32_t;


/********************** Arena ****************************/
struct Arena
{
    Byte * beg;
    Byte * end;

    inline Arena(Byte * start, Size capacity):
        beg(start),
        end(start+capacity)
    {
    }

    inline Arena(Arena & other):
        beg(other.beg),
        end(other.end)
    {
    }
};

template <typename T, typename ...A>
T * make_array(Arena * arena, Size count = 1)
{
    Size const size = sizeof(T);
    Size const align = alignof(T);
    Size const pad = (align - 1) & (Size)arena->end;
    
    //bc_assert(count >= 0);
    //bc_assert((arena->end - arena->beg - pad) >= size * count);
    arena->end -= size * count + pad;
    new((void *)(arena->end)) T[count];
    return arena->end;
}

template <typename T, typename ...A>
T * make(Arena * arena, A ...args)
{
    Size size = sizeof(T);
    Size align = alignof(T);
    Size pad = (align - 1) & (Size)arena->end;
    
    bc_assert((arena->end - arena->beg - pad) >= size);
    arena->end -= size * + pad;
    T * r = (T *)arena->end;
    new((void *)r) T(args...);
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

    S8(U8 * val, Size len):
        val(val),
        len(len)
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
    T * beg = 0;
    Size len = 0;
    Size cap = 0;

    T & operator[](std::size_t pos) {
        bc_assert(pos < len);
        return beg[pos];
    }

    Buffer(Arena * arena, Size size):
        len(0),
        cap(size)
    {
        beg = make<T>(arena, size);
    }

        Buffer(Byte * beg, Size cap):
                beg(beg),
                len(0),
                cap(cap) {}
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
