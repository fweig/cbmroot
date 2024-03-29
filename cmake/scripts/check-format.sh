#!/bin/bash
# Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Florian Uhlig


BASE_COMMIT=${FAIRROOT_FORMAT_BASE:-HEAD}
GIT_CLANG_FORMAT_BIN=${FAIRROOT_GIT_CLANG_FORMAT_BIN:-git-clang-format}

FILES=$(git diff --name-only $BASE_COMMIT | grep -E '.*\.(h|hpp|c|C|cpp|cxx|tpl)$' | grep -viE '.*LinkDef.h$')
RESULT=$($GIT_CLANG_FORMAT_BIN --commit $BASE_COMMIT --diff $FILES --extensions h,hpp,c,C,cpp,cxx,tpl)

if [ "$RESULT" == "no modified files to format" ] || [ "$RESULT" == "clang-format did not modify any files" ]; then
    echo "format check passed."
    exit 0
else
    echo "ERROR: format check failed. Suggested changes:"
    echo "$RESULT"
    exit 1
fi
