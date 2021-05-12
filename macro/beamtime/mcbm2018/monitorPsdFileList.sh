#!/bin/bash
# Copyright (C) 2019 CBM Collaboration, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by karpushkin@inr.ru

SOURCEPATH="/scratch/mcbm_data/data_mpsd/"
find $SOURCEPATH -name "*.tsa" > MonitorPsdFileList.txt

for filename in `cat MonitorPsdFileList.txt`;
 do
   echo ${filename};
   prefix=`basename ${filename%%.*}`;
   root -l -b -q "MonitorPsd.C(\"${filename}\", \"\",8087,100,1,\"${prefix}\",0)"
 done;
