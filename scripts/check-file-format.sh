#!/bin/bash
# Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Florian Uhlig


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

BASE_COMMIT=$UPSTREAM/master
CHANGED_FILES=$(git diff --name-only $BASE_COMMIT)
for file in $CHANGED_FILES; do
  result=$(file $file | grep CRLF)
  if [[ "$result" != "" ]]; then
    echo ""
    echo "File $file has wrong file format"
    echo "$result"
    echo ""
    okay=false
  fi
done
if [[ "$okay" = "false" ]]; then
  echo ""
  echo "Not all files have the correct file format"
  echo "Test failed"
  exit 1
else
  exit 0
fi

