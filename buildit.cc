#include <stdlib.h>

int main(int argc, char * argv[])
{
    system("mkdir -p build");
    system("g++ --std=c++20 -o build/bclib-tests src/bclib_unity.cc tests/bclib_tests_unity.cc");
    return 0;
}
