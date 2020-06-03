#!/bin/bash

INPUT_ROOT_FOLDER=/store/cbm_rich_data_2014/WLS/WLS_off/nonstretched/

# these are folders with names ring**
INPUT_FOLDERS=${INPUT_ROOT_FOLDER}*

for IN_FOLDER in ${INPUT_FOLDERS}; do

   echo "--------------------- current folder: ${IN_FOLDER} -------------------------------"

   # there are folders with names offset*****
   IN_FOLDERS2=${IN_FOLDER}/*

   for IN_FOLDER2 in ${IN_FOLDERS2}; do

      echo "    ================ current inner folder: ${IN_FOLDER2} ======================"

      xterm -hold -geometry 600x500+70+0 -e "./process_inner_folder.sh ${IN_FOLDER2}"&

#      IN_FILES=${IN_FOLDER2}/*
#      for IN_FILE in ${IN_FILES}; do
#         echo "              ++++++++++++++ current file: ${IN_FILE} +++++++++++++"
##         hexdump -v -x ${IN_FILE} | grep "527f    1016"
#      done

   done

done
