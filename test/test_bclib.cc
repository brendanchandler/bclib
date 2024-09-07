#include <bclib.h>

struct TestStats
{
    I32 failures;
};

#define check(ts, cond) check2(ts, cond, #cond)


void check2(teststats * ts, b32 condition, Str * msg)
{
    if(!condition) {
        // TODO: print the condition that failed
        teststats->falures++;
    }
}

static TestStats test_arena(teststats & ts)
{
    I32 arena_size = 1024;
    Byte * addr = malloc(arena_size);

    Arena arena(addr, arena_size);

    I32 * a = make<I32>(arena);
    *a = 5;

    I32 * b = make<I32>(arena);
    *b = 10;
    
    I32 ** c = make<I32>(arena, 5);
    c[0] = 1;
    c[1] = 2;
    c[2] = 3;
    c[3] = 4;
    c[4] = 5;

    TestStats ts;
    
    check(&ts, *a == 5);
    check(&ts, *b == 10);
    check(&ts, c[0] = 1);
    check(&ts, c[0] = 2);
    check(&ts, c[0] = 3);
    check(&ts, c[0] = 4);
    check(&ts, c[0] = 5);

    return ts;
}

int main(int argc, char * argv[])
{
    teststats ts;
    test_arena(ts);
}
