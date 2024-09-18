#!/bin/bash

release_type="$1"
mkdir -p build

CXXFLAGS="-Wall -Werror -Wextra"
CXXFLAGS+=" -Wno-unused-parameter -Wno-unused-function"
CXXFLAGS+=" -fsanitize=undefined -fsanitize-trap"
CXXFLAGS+=" -I src/"
CXXFLAGS+=" -g3 -O0"

SRCS="src/bclib.cc"
SRCS+=" src/platform.cc"

TEST_SRCS="test/test_bclib.cc"

g++ ${CXXFLAGS} -o build/test-bclib ${SRCS} ${TEST_SRCS}

TEST_SRCS="test/test_bclib_perf.cc"

g++ ${CXXFLAGS} -o build/test-bclib-perf ${SRCS} ${PERF_SRCS}
