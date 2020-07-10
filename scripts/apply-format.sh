#!/bin/bash

CLANG_FORMAT_BIN=${FAIRROOT_CLANG_FORMAT_BIN:-clang-format}

FILES=$(find . -type f \
        \( -iname "*.h" -o -iname "*.hpp" -o -iname "*.cxx" -o -iname "*.cpp" -o -iname "*.c" -o -iname "*.C" \) \
        -not \( -path "./build/*" -o -path "./cmake/*" -o -path "./external/*" -o -path "./geometry/*" -o -path "./parameters/*" -o -path "./input/*" -prune \))

$CLANG_FORMAT_BIN -i $FILES --verbose
