FAIRROOT_FORMAT_BASE=upstream/master
BASE_COMMIT=${FAIRROOT_FORMAT_BASE:-HEAD}
CHANGED_FILES=$(git diff --name-only $BASE_COMMIT | grep -E '.*\.(h|hpp|c|C|cpp|cxx|tpl)$' | grep -viE '.*LinkDef.h$')

if [ -z $GIT_CLANG_FORMAT_BIN ]; then
  echo "GIT_CLANG_FORMAT_BIN not defined, trying to use git-clang-format-10 if present"
  GIT_CLANG_FORMAT_BIN=git-clang-format-10
fi

$GIT_CLANG_FORMAT_BIN --verbose --commit $BASE_COMMIT $CHANGED_FILES --extensions h,hpp,c,C,cpp,cxx,tpl
git add $CHANGED_FILES
git status
git commit -m"Apply clang-format"
