#!/bin/bash

upstream_repo=$1

bla=$(git remote -v | grep upstream)
if [ $? -eq 0 ]; then
  echo "Remote link upstream already exist"
  bla=$(git remote -v | grep upstream | grep $upstream_repo)
  if [ $? -eq 0 ]; then
    echo "Remote link upstream already exist and points to the correct repo"
    # dont do anything
  else
    echo "Remote link upstream already exist and points to the wrong repo"
    git remote rm upstream
    git remote add upstream $upstream_repo
  fi
else
  echo "Remote repo has to be connected"
  git remote add upstream $upstream_repo
fi
