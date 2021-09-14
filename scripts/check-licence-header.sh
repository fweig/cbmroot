#!/bin/bash
# Copyright (C) 2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Eoin Clerkin

RETURN_CODE="0"

licenceHeaderCheck () {

	FILE_CODE="0";
	sed -n '2p' $1 | grep -q '   SPDX-License-Identifier: GPL-3.0-only'
	if [ $? -ne 0 ]; then
		echo "[ERROR] $1, line 2 missing spdx licence header declaration."
		((RETURN_CODE++))
		FILE_CODE="1"
	fi

	head -n 1 $1 | grep -q '\/\* Copyright (C) [0-9-]* [a-zA-Z -]*, [a-zA-Z]*'
	if [ $? -ne 0 ]; then
		echo "[ERROR] $1; line 1 has syntax errors in its licence header."
		((RETURN_CODE++))
		FILE_CODE="1"
	fi

	sed -n '3p' $1 | grep -q '   Authors: .[^\*\/]*\*/'
	if [ $? -ne 0 ]; then
		echo "[ERROR] $1; line 3 has syntax errors in its licence header."
		((RETURN_CODE++))
		FILE_CODE="1"
	fi

	sed -n '3p' $1 | grep -q '\[committer\]'
	if [ $? -ne 0 ]; then
		echo "[ERROR] $1; line 3 has missing first commiter information."
		((RETURN_CODE++))
		FILE_CODE="1"
	fi

	if [[ ${FILE_CODE} -eq 0 ]]; then
		echo "[OK] File: $1 passes licence header check."
	fi
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



if [ -f $1 ]; then 
CHANGED_FILES="$1"
echo "LICENCE HEADER CHECK FOR FILE: $1"
else
echo "Upstream name is :" $UPSTREAM
BASE_COMMIT=$UPSTREAM/master
CHANGED_FILES=$(git diff --name-only $BASE_COMMIT | egrep '\.cxx$|\.h|\.C$')
fi


for FILE in $CHANGED_FILES; do
	licenceHeaderCheck $FILE
done

if [ $RETURN_CODE -eq "0" ]; then
	echo "[OK] Licence header passes automatic checks."
else
	echo "[FAIL] Visit https://redmine.cbm.gsi.de/projects/cbmroot/wiki/Licence for information on correct syntax for licence header."
fi



exit $RETURN_CODE;
