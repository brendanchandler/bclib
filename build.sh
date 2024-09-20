#!/bin/bash

set -x
set -e

release_type="$1"
mkdir -p build

CXXFLAGS="-Wall -Werror -Wextra"
CXXFLAGS+=" -Wno-unused-parameter -Wno-unused-function"
#CXXFLAGS+=" -fsanitize=undefined -fsanitize-trap"
CXXFLAGS+=" -I src/"
CXXFLAGS+=" -g3 -O0"

SRCS="src/bclib.cc"
SRCS+=" src/platform.cc"

TEST_SRCS="test/test_bclib.cc"

g++ ${CXXFLAGS} -o build/test-bclib ${SRCS} ${TEST_SRCS}

cmake -S tracy -B tracy/build/
cmake --build tracy/build/

PERF_SRCS="test/test_bclib_perf.cc"
g++ ${CXXFLAGS} -DTRACY_CALLSTACK=60 -DTRACY_NO_EXIT -DTRACY_ENABLE -I tracy/public/ -o build/test-bclib-perf ${SRCS} ${PERF_SRCS} -Ltracy/build -lTracyClient -lpthread -ldl

