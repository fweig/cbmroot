#!/bin/bash
# Copyright (C) 2016 Justus-Liebig-Universitaet Giessen, Giessen
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Julian Book


LOCATION=/lustre/nyx

## choose cbm root installation
. $LOCATION/cbm/users/$USER/CBMsoft/cbm-env.sh -n

## job content
dir=$(dirname $1)
file=$(basename $1)

root -l -b -q './merge.C("'$1'","'$dir'/merged_'$file'.root")'
