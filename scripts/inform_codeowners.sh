#!/bin/bash
# Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Florian Uhlig


main () {
  # get passead parameters
  # 1. git name of the official repo
  if [[ ! $# -eq 1 ]]; then
    echo "Pass the name of the upstream repository as argument"
    exit 1
  fi

  # Since bash only supports dictionaries from version 4
  # the dictionary is implemented as list. The values in
  # the list are the directory (key) and the list of users owning
  # this directory separated by a colon. If there is more than one
  # user owning a directory the list of users is separated by a
  # semicolon.
  # An example of such an entry is "/macro/:f.uhlig;v.friese"
  declare -a dictionary
  declare -a unique_users

  parse_codeowners_file
  #declare -p dictionary

  CHANGED_FILES=""
  generate_file_list "$1"

  ALL_USERS=""
  generate_codeowners_list "$CHANGED_FILES"

  generate_comment
}


# Add a key value pair to the dictionary
function add_hash () {
  dictionary+=("$1":"$2")
}

# extract the directory (key) from the passed string
function get_key () {
  dir="${1%%:*}"
}

# extract the user list (value) from the passed string
function get_value () {
  users="${1##*:}"
}

# Parse the CODEOWNERS file and store the information of owned directories
# in a dictionray with the directory as key and the code owners as value
# read a file line by line
# Parse the remaining lines, split them at the first blank
# First entry of the array is the directory, all other entries
# are the list of people to inform
# We have to check that the CODEOWNERS file is properly formated since the
# used parser is very simply
function parse_codeowners_file () {
  while read line; do
    # exclude commented and empty lines
    if [[ $line =~ ^\# || $line =~ ^$ ]]; then
     dummy=$line
    else
      # split the line at blank characters and store the results in an array
      IFS=' ' read -ra my_array <<< "$line"
      # first entry is the directory
      directory="${my_array[0]}"
      # in case of two entries the second is the user
      if [[ ${#my_array[@]} -eq 2 ]]; then
        users="${my_array[1]}"
      # in case of more entries the entries from the second to the last are a list of users
      # Add the users to a semicolon separated list and store the list in the user array
      else
        arr2=("${my_array[@]:1:${#my_array[@]}}")
        newUserList=""
        for i in "${arr2[@]}"; do
          newUserList="$newUserList;$i"
        done
        newUserList="${newUserList:1}"
        users="$newUserList"
      fi
      add_hash "$directory" "$users"
    fi
  done < CODEOWNERS
}

function generate_codeowners_list () {
  CHANGED_FILES=$1
  all_users=""
  for file in $CHANGED_FILES; do
#    echo $file
    file="/$file"
    inform_users=""
    longest_match=0
    for entry in "${dictionary[@]}"; do
      get_key $entry
      if [[ $file =~ ^$dir ]]; then
        get_value $entry
        # echo "The length of the match is ${#dir}"
        if [[ ${#dir} -gt $longest_match ]]; then
          inform_users=$users
        fi
      fi
    done
#    echo "We have to inform user $inform_users for file $file"
    all_users="$all_users;$inform_users"
  done
  ALL_USERS="${all_users:1}"

  # remove duplications from string
  IFS=';'
  read -ra users <<< "$ALL_USERS"

  for user in "${users[@]}"; do   # access each element of array
    if [[ ! " ${unique_users[@]} " =~ " ${user} " ]]; then
      unique_users+=("$user")
    fi
  done
  OWNERS_LIST=${unique_users[@]}

  echo "We have to inform the following users about the code ownership: $OWNERS_LIST"
}

function generate_file_list() {

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

  # Rebase the MR to extract the changed files properly
  # If upstream/master was changed while the pipline was running one would otherwise get
  # a wrong list of files
  git rebase $UPSTREAM/master
  BASE_COMMIT=$UPSTREAM/master
  CHANGED_FILES=$(git diff --name-only $BASE_COMMIT)
}

function generate_comment() {

  # Get Information about the MR in JSON format
  # Check if the returned string contains the signature that the CodeOwners label is set
  # Only create GitLab comment if there is no comment yet (CodeOwners label not set)
  MRInfo=$(curl -s --request GET --header "PRIVATE-TOKEN: $COMMENT_TOKEN" "https://git.cbm.gsi.de/api/v4/projects/$CI_MERGE_REQUEST_PROJECT_ID/merge_requests/$CI_MERGE_REQUEST_IID" | grep labels.*\\[.*CodeOwners.*\\])
  if [[ -z $MRInfo ]]; then

    comment="Dear "
    for user in "${unique_users[@]}"; do
      comment="$comment $user,"
    done
    comment="$comment

you have been identified as code owner of at least one file which was changed with this merge request.

Please check the changes and approve them or request changes."


    # Add a comment to the merge request which informs code owners about their duty to review
    curl -s --request POST \
       -d "body=$comment" \
       --header "PRIVATE-TOKEN: $COMMENT_TOKEN" \
       "https://git.cbm.gsi.de/api/v4/projects/$CI_MERGE_REQUEST_PROJECT_ID/merge_requests/$CI_MERGE_REQUEST_IID/notes"

    # On first call set a label which can be checked later on to avoid sending the comment over and over again
    curl -s --request PUT \
       --header "PRIVATE-TOKEN: $COMMENT_TOKEN" \
       "https://git.cbm.gsi.de/api/v4/projects/$CI_MERGE_REQUEST_PROJECT_ID/merge_requests/$CI_MERGE_REQUEST_IID?add_labels=CodeOwners"
  else
    echo "Information message already exits, so there is no need to create another one."
  fi
}

main "$@"
