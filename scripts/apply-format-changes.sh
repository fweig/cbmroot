#!/bin/bash

if [[ -z $GIT_CLANG_FORMAT_BIN || -z $CLANG_FORMAT_BIN ]]; then
  echo "Error: GIT_CLANG_FORMAT_BIN or CLANG_FORMAT_BIN not defined"
  echo "=> Please follow the instruction at https://redmine.cbm.gsi.de/projects/cbmroot/wiki/Clang-format"
  exit -1
fi

# Check clang-format version
VERSION=$($CLANG_FORMAT_BIN --version)
if [[ "$VERSION" != *"11.0.0"* ]]; then
  echo "Error: CLANG_FORMAT_BIN version not matching the standard cbmroot one (used in the MR test chain)"
  echo "11.0.0 vs " $VERSION
  echo "=> Please follow the instruction at https://redmine.cbm.gsi.de/projects/cbmroot/wiki/Clang-format"
  exit -1
fi

FAIRROOT_FORMAT_BASE=upstream/master
BASE_COMMIT=${FAIRROOT_FORMAT_BASE:-HEAD}
CHANGED_FILES=$(git diff --name-only $BASE_COMMIT | grep -E '.*\.(h|hpp|c|C|cpp|cxx|tpl)$' | grep -viE '.*LinkDef.h$')


$GIT_CLANG_FORMAT_BIN --verbose --commit $BASE_COMMIT $CHANGED_FILES --extensions h,hpp,c,C,cpp,cxx,tpl
git add $CHANGED_FILES
echo ""
git status
git commit -m"Apply clang-format"
