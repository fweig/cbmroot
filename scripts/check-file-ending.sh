#!/bin/bash

if [[ $# -eq 1 ]]; then
  UPSTREAM=$1
else
  if [ -z $UPSTREAM ]; then
    UPSTREAM=$(git remote -v | grep git.cbm.gsi.de[:/]computing/cbmroot | cut -f1 | uniq)
    if [ -z $UPSTREAM ]; then
      echo "Error: Name of upstream repository not provided and not found by automatic means"
      echo 'Please provide if by checking your remotes with "git remote -v" and exporting UPSTREAM'
      echo "or passing as an argument"
      exit -1
    fi
  fi
fi  
echo "Upstream name is :" $UPSTREAM


# If one wants to find all files in the CbmRoot and not only the changed ones
# uncomment the follwing line and comment the next two
#CHANGED_FILES=$(find . -type f -not \( -path "./.git/*" -o -path "./geometry/*" -o -path "./input/*" -o -path "./external/*" -o -path "./parameters/*" -prune \))

BASE_COMMIT=$UPSTREAM/master
CHANGED_FILES=$(git diff --name-only $BASE_COMMIT)

echo ""
for file in $CHANGED_FILES; do

  # First check for text files and only do the further test on line endings
  # for text files
  result=$(file $file | grep -v text)
  if [[ -z $result ]]; then
    if [[ $(tail -c 1 $file) ]]; then
      echo "File $file does not finish with end of line"
      okay=false
    fi
  fi    
done

if [[ "$okay" = "false" ]]; then
  echo ""
  echo "Not all files have the correct file ending"
  echo "Test failed"
  echo ""
  exit 1
else
  exit 0
fi

