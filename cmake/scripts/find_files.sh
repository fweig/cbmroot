#!/bin/bash

BASE_COMMIT=${FAIRROOT_FORMAT_BASE:-HEAD}
GIT_CLANG_FORMAT_BIN=${FAIRROOT_GIT_CLANG_FORMAT_BIN:-git-clang-format}

FILES=$(git diff --name-only $BASE_COMMIT | grep -E '.*\.(h|hpp|c|C|cpp|cxx|tpl)$' | grep -viE '.*LinkDef.h$')

echo $FILES
