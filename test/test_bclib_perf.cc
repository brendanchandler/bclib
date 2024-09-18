#include <bclib.hh>

static void test_malloc(TestStats * ts)
{
    Byte buf[1024];
    
    for (Size i = 0; i < 10000000; i++) {
        U8 * msg = (U8 *)malloc(100);
        U8 * msg = make<U8>(&a, 100);
        S8 const_str("Hello World\n");
        bc_memcpy(msg, const_str.val, const_str.len);
        free(msg);
    }
}

static void test_arena(TestStats * ts)
{
    Byte buf[1024];
    
    for (Size i = 0; i < 10000000; i++) {
        Arena a(buf, 1024);
        U8 * msg = make<U8>(&a, 100);
        S8 const_str("Hello World\n");
        bc_memcpy(msg, const_str.val, const_str.len);
    }
}

static void test_new(TestStats * ts)
{
    Byte buf[1024];
    
    for (Size i = 0; i < 10000000; i++) {
        U8 * msg = new U8[100];
        S8 const_str("Hello World\n");
        bc_memcpy(msg, const_str.val, const_str.len);
        delete msg;
    }
}


int main(void)
{
    test_malloc();
    test_arena();
    test_new();
}
