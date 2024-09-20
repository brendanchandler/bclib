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

        TestStruct():
                a(5),
                b(10),
                e(100),
                g(5.5),
                h(500.555) {}
};
    


static void test_malloc(TestStats & ts)
{
    ZoneScoped;
    print(S8("Testing malloc...\n"));
    for (Size i = 0; i < 10000000; i++) {
        U8 * msg = (U8 *)malloc(100);
        S8 const_str("Hello World\n");
        bc_memcpy(msg, const_str.val, const_str.len);

        TestStruct * ts = (TestStruct *)malloc(sizeof(TestStruct));
        new(ts) TestStruct;
        bc_assert(ts->a == 5);

        free(ts);
        free(msg);
    }
    FrameMark;
}

static void test_arena(TestStats & ts)
{
        ZoneScoped;
        print(S8("Testing C style arena allocs...\n"));
        USize const BUF_CAP = 1u << 31;
        Byte * buf = (Byte *)malloc(sizeof(Byte) * BUF_CAP);
        Arena a(buf, BUF_CAP);

        for (Size i = 0; i < 10`000`000; i++) {
                U8 * msg = make_array<U8>(&a, 100);
                memset(msg, 0, 100);
                
                S8 const_str("Hello World\n");
                bc_memcpy(msg, const_str.val, const_str.len);

                TestStruct * ts = (TestStruct *)make<TestStruct>(&a);
                bc_assert(ts->a == 5);
        }

        free(buf);
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
        bc_assert(ts->a == 5);

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
