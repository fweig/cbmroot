#!/bin/bash

if [[ $# -ne 1 ]]; then
  echo "Missing argument! please call this script with either the check or apply argument:"
  echo "./scripts/check-apply-format-changes.sh check"
  echo "./scripts/check-apply-format-changes.sh apply"
  exit -1
fi

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

if [ -z $UPSTREAM ]; then
  UPSTREAM=$(git remote -v | grep git.cbm.gsi.de[:/]computing/cbmroot | cut -f1 | uniq)
  if [ -z $UPSTREAM ]; then
    echo "Error: Name of upstream repository not provided and not found by automatic means"
    echo 'Please provide if by checking your remotes with "git remote -v" and exporting UPSTREAM'
    exit -1
  fi
fi
echo "Upstream name is :" $UPSTREAM

BASE_COMMIT=$UPSTREAM/master
CHANGED_FILES=$(git diff --name-only $BASE_COMMIT | grep -E '.*\.(h|hpp|c|C|cpp|cxx|tpl)$' | grep -viE '.*LinkDef.h$')

case $1 in
  check)
    echo "Checking if there are format changes required"
    git fetch $UPSTREAM
    $GIT_CLANG_FORMAT_BIN --binary $CLANG_FORMAT_BIN --commit $BASE_COMMIT --diff $CHANGED_FILES --extensions h,hpp,c,C,cpp,cxx,tpl
    ;;

  apply)
    echo "Applying required format changes"
    $GIT_CLANG_FORMAT_BIN --binary $CLANG_FORMAT_BIN --verbose --commit $BASE_COMMIT $CHANGED_FILES --extensions h,hpp,c,C,cpp,cxx,tpl
    git status
    echo "Next step: git add "$CHANGED_FILES
    echo 'Then     : git commit -m"Apply clang-format"'
    ;;

  *)
    echo "Invalid argument! please call this script with either the check or apply argument:"
    echo "./scripts/check-apply-format-changes.sh check"
    echo "./scripts/check-apply-format-changes.sh apply"
    exit -1
    ;;
esac
