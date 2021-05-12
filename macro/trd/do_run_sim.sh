#!/bin/bash
# Copyright (C) 2014 Institut fuer Kernphysik, Westfaelische Wilhelms-Universitaet Muenster, Muenster
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Cyrano Bergmann

nThreads=0
maxThreads=0
nEvents=1000
centrality=mbias
cat /proc/cpuinfo | grep processor | tail -n1 | cut -c 13- > temp
while read line 
do
    maxThreads=$line
    echo "${maxThreads} threads are started in parallel"
done
rm temp  
 maxThreads=3
for urqmd in {0..399}
do
	nThreads=$(($nThreads+1))
	echo "${nThreads} root -l -b -q run_sim_maf.C'('$nEvents,$urqmd,'\"'$centrality'\"'')'"
	echo "nice root -l -b -q run_sim_maf.C'('$nEvents,$urqmd,'\"'$centrality'\"'')' &"  > mysim
	. mysim  &> /dev/null
#&>> logsimfile.txt
	sleep 10
	if [ "$nThreads" -ge "$maxThreads" ]; then
	    wait ${!}
	    nThreads=0
	fi
done
