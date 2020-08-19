#!/bin/bash

infile=$1
outfile=$2

BASE_COMMIT=${FAIRROOT_FORMAT_BASE:-HEAD}
GIT_CLANG_FORMAT_BIN=${FAIRROOT_GIT_CLANG_FORMAT_BIN:-git-clang-format}

RESULT=$($GIT_CLANG_FORMAT_BIN --commit $BASE_COMMIT --diff $infile --extensions h,hpp,c,C,cpp,cxx,tpl)

if [ "$RESULT" == "no modified files to format" ] || [ "$RESULT" == "clang-format did not modify any files" ]; then
  exit 0
else
  echo "ERROR: format check failed for file $infile. Suggested changes:" > $outfile
  echo  >> $outfile
  echo "$RESULT" >> $outfile
  echo "ERROR: format check failed for file $infile. Suggested changes:"
  echo
  echo "$RESULT"
  exit 1
fi
