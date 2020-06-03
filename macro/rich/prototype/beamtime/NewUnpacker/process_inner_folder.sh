#!/bin/bash

echo "Script is processing folder $1"

IN_FILES=$1/*
for IN_FILE in ${IN_FILES}; do
   echo ${IN_FILE}
   hexdump -v -x ${IN_FILE} | grep "825b    3e92"
done
