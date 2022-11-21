#!/bin/bash
# Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Pierre-Alain Loizeau

TAG="git"
if [[ $# -eq 1 ]]; then
  TAG=$1
fi

FAILED=0
FAILED_LIST=""

CHECK_LIST=""
if [[ $TAG == "git" ]]; then
  if [ -z $UPSTREAM ]; then
    UPSTREAM=$(git remote -v | grep git.cbm.gsi.de[:/]computing/cbmroot | cut -f1 | uniq)
    if [ -z $UPSTREAM ]; then
      echo "Error: Name of upstream repository not provided and not found by automatic means"
      echo 'Please provide if by checking your remotes with "git remote -v" and exporting UPSTREAM'
      return -1 2>/dev/null || exit -1
    fi
  fi
  echo "Upstream name is :" $UPSTREAM

  BASE_COMMIT=$UPSTREAM/master
  GEO_BASE=`git diff ${BASE_COMMIT} ${VMCWORKDIR}/external/InstallGeometry.cmake | grep '-' | grep GEOMETRY_VERSION`
  if [[ 1 -eq `echo "$GEO_BASE" | wc -l` ]]; then
    GEO_BASE=`echo ${GEO_BASE} | cut -f2 -d ' ' | rev | cut -c2- | rev`
    printf "Geo Base commit is ${GEO_BASE}\n"

    cd ${VMCWORKDIR}/geometry/
    # Get only name of changed files and filter out deleted files
    CHECK_LIST=$(git diff --name-only --diff-filter=d ${GEO_BASE} | grep -E '.*\.geo.root$')
    cd -
  else
    echo "No change to GEO hash"
    return 0 2>/dev/null ||exit 0
  fi
elif [[ $TAG == "all" || $TAG == "main" ]]; then
  cd ${VMCWORKDIR}/geometry/
  CHECK_LIST=`ls */*.geo.root`
  if [[ $TAG == "main" ]]; then
    CHECK_LIST=`printf "${CHECK_LIST}" | grep -v mcbm`
  fi
  cd -
else
  cd ${VMCWORKDIR}/geometry/
  CHECK_LIST=`ls */*${TAG}*.geo.root`
  cd -
fi
printf "Files to be checked: \n${CHECK_LIST}\n"

cd ${VMCWORKDIR}/geometry/
echo "---------------------------------"
for geo in ${CHECK_LIST}; do
  echo $geo;
  ./ci_scripts/check_radlen.sh ${VMCWORKDIR}/geometry/$geo;
  if [[ $? -gt 0 ]]; then
    FAILED=$((FAILED+1));
    FAILED_LIST=${FAILED_LIST}" \n "${geo}
  fi
  echo "---------------------------------"
done
cd -

if [[ ${FAILED} -gt 0 ]]; then
  printf "${FAILED} geometries have wrong radiation length or are incompatible with the macro\n"
  printf "These geometries are: \n ${FAILED_LIST} \n"
else
  echo "All checked geometries have valid radiation length values"
fi

return ${FAILED} 2>/dev/null || exit ${FAILED}
