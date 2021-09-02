#!/bin/bash
# Copyright (C) 2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Eoin Clerkin

licenceHeaderCheck () {
	sed -n '2p' $1 | grep -L '   SPDX-License-Identifier: GPL-3.0-only'
	if [ $? -ne 0 ]; then
		echo "[ERROR] Missing or Malformated licence header for file $FILE"
		exit 11
	fi

	head -n 1 $1 | grep -L '\/\* Copyright (C) [0-9-]* .[a-zA-Z ]*, [a-zA-Z]*'
	if [ $? -ne 0 ]; then
		echo "[ERROR] $FILE; line 2 has syntax errors in its licence header."
		exit 12
	fi

	sed -n '3p' $1 | grep -L '   Authors: .[^\*\/]*\*/'
	if [ $? -ne 0 ]; then
		echo "[ERROR] $FILE; line 3 has syntax errors in its licence header."
		exit 13
	fi

	sed -n '3p' $FILE | grep -L '\[committer\]'
	if [ $? -ne 0 ]; then
		echo "[ERROR] $FILE; line 3 has missing first commiter information."
	exit 14
	fi
	echo "[OK] File: $1 passes licence header check.";
	}


if [ $# -eq 1 ]; then
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

echo "See wikipage https://redmine.cbm.gsi.de/projects/cbmroot/wiki/Licence for information regarding the correct syntax used in the licence header."

echo "Upstream name is :" $UPSTREAM

BASE_COMMIT=$UPSTREAM/master
CHANGED_FILES=$(git diff --name-only $BASE_COMMIT | egrep '\.cxx$|\.h|\.C$')
for FILE in $CHANGED_FILES; do
licenceHeaderCheck $FILE
done

echo "[OK] Licence header passes automatic checks."
exit 0;
