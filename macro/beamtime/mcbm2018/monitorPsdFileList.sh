#!/bin/bash
SOURCEPATH="/scratch/mcbm_data/data_mpsd/"
find $SOURCEPATH -name "*.tsa" > MonitorPsdFileList.txt

for filename in `cat MonitorPsdFileList.txt`;
 do
   echo ${filename};
   prefix=`basename ${filename%%.*}`;
   root -l -b -q "MonitorPsd.C(\"${filename}\", \"\",8087,100,1,\"${prefix}\",0)"
 done;
