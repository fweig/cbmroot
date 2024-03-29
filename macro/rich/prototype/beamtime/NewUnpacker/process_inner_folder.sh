#!/bin/bash
# Copyright (C) 2015 Justus-Liebig-Universitaet Giessen, Giessen
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Egor Ovcharenko


echo "Script is processing folder $1"

IN_FILES=$1/*
for IN_FILE in ${IN_FILES}; do
   echo ${IN_FILE}
   hexdump -v -x ${IN_FILE} | grep "825b    3e92"
done
