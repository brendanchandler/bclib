#include <bclib.hh>
#include <stdlib.h>
#include <trace.hh>

struct TestStats
{
    I32 failures;
};

void bc_memcpy(U8 * dest, U8 * src, Size len)
{
    for(int i = 0; i < len; i++) {
        dest[i] = src[i];
    }
}

struct TestStruct
{
    U8 a;
    U8 b;
    U32 c;
    U32 d;
    U32 e;
    F64 f;
    F64 g;
    F64 h;
    U8 i[10];
};
    


static void test_malloc(TestStats & ts)
{
    ZoneScoped;
    print(S8("Testing malloc...\n"));
    for (Size i = 0; i < 10000000; i++) {
        U8 * msg = (U8 *)malloc(100);
        S8 const_str("Hello World\n");
        bc_memcpy(msg, const_str.val, const_str.len);
        free(msg);

        TestStruct * ts = (TestStruct *)malloc(sizeof(TestStruct));
        ts->a = 5;
        ts->b = 10;
        ts->e = 100;
        ts->g = 5.5;
        ts->h = 500.555;

        free(ts);
    }
    FrameMark;
}

static void test_arena(TestStats & ts)
{
    ZoneScoped;
    print(S8("Testing arena allocs...\n"));
    Size const BUF_CAP = 1<<30;
    Byte * buf = new Byte[BUF_CAP];

    for (Size i = 0; i < 1000000; i++) {
        Arena a(buf, BUF_CAP);

        for (Size j = 0; j < 10; j++) {
            U8 * msg = make<U8>(&a, 100);
            S8 const_str("Hello World\n");
            bc_memcpy(msg, const_str.val, const_str.len);

            TestStruct * ts = make<TestStruct>(&a);
            ts->a = 5;
            ts->b = 10;
            ts->e = 100;
            ts->g = 5.5;
            ts->h = 500.555;
        }

    }

    delete[] buf;
    FrameMark;
}

static void test_new(TestStats & ts)
{
    ZoneScoped;
    print(S8("Testing new...\n"));
    for (Size i = 0; i < 10000000; i++) {
        U8 * msg = new U8[100];
        S8 const_str("Hello World\n");
        bc_memcpy(msg, const_str.val, const_str.len);
        delete[] msg;

        TestStruct * ts = new TestStruct;
        ts->a = 5;
        ts->b = 10;
        ts->e = 100;
        ts->g = 5.5;
        ts->h = 500.555;

        delete ts;
    }
    FrameMark;
}


int main(void)
{
    ZoneScoped;
    TestStats ts;
    
    test_malloc(ts);
    test_arena(ts);
    test_new(ts);
}
