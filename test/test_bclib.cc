#include <bclib.hh>
#include <stdlib.h>

#define check(ts, cond) check2(ts, cond, #cond)

struct TestStats
{
    I32 failures;
};

template <typename T>
void bc_memcpy(T * dest, T * src, Size len)
{
    for(int i = 0; i < len; i++) {
        dest[i] = src[i];
    }
}

void check2(TestStats * ts, B32 condition, S8 msg)
{
    int const BUF_SIZE=256;
    Byte buf[BUF_SIZE];
    Arena scratch(buf, BUF_SIZE);

    StrList out;
    
    strlist_append(&out, &scratch, "Checking ");
    strlist_append(&out, &scratch, msg);
    strlist_append(&out, &scratch, "... ");

    if(!condition) {
        strlist_append(&out, &scratch, "failed!");
        ts->failures++;
    } else {
        strlist_append(&out, &scratch, "ok!\n");
    }

    print(out);
}

static void test_arena(TestStats * ts)
{
    I32 arena_size = 1024;
    Byte * addr = (Byte *)malloc(arena_size);

    Arena arena(addr, arena_size);

    I32 * a = make<I32>(&arena);
    *a = 5;

    I32 * b = make<I32>(&arena);
    *b = 10;
    
    /*
    I32 ** c = make<I32>(&arena, 5);
    c[0] = 1;
    c[1] = 2;
    c[2] = 3;
    c[3] = 4;
    c[4] = 5;
*/
    check(ts, *a == 5);
    check(ts, *b == 10);
    /*
    check(ts, c[0] = 1);
    check(ts, c[0] = 2);
    check(ts, c[0] = 3);
    check(ts, c[0] = 4);
    check(ts, c[0] = 5);
    */
}

int main()
{
    TestStats ts;
    test_arena(&ts);

    if (ts.failures == 0) {
        print(S8("All tests passed!\n"));
    } else {
        print(S8("Tests failed.\n"));
    }
}
