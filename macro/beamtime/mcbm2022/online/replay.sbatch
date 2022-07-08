#!/bin/bash

#SBATCH -J Replay

# Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# author: Pierre-Alain Loizeau [committer]

if [ $# -ge 2 ]; then
  _run_id=$1
  _port=$2
else
  echo 'Missing parameters. Only following pattern allowed:'
  echo 'replay.sbatch <Run Id> <port>'

  return -1
fi

Filename1=/storage/1/data/${_run_id}_*_1_*.tsa
Filename2=/storage/2/data/${_run_id}_*_2_*.tsa
Filename3=/storage/3/data/${_run_id}_*_3_*.tsa
Filename4=/storage/4/data/${_run_id}_*_4_*.tsa
Filename5=/storage/5/data/${_run_id}_*_5_*.tsa
Filename6=/storage/7/data/${_run_id}_*_6_*.tsa
Filename7=/storage/8/data/${_run_id}_*_7_*.tsa
Filename8=/storage/6/data/${_run_id}_*_8_*.tsa
Filename9=/storage/6/data/${_run_id}_*_9_*.tsa

HOSTNAME=`hostname`
HOST=${HOSTNAME:4:2}
# Force cast to base 10 to avoid wrong octal base assumption
HOSTCLEAN=$((10#${HOST}))

## => Hostname and port for replay toward virgo
#hostnameIB="cbmfles"$HOSTNAME
#ipaddrIB=`dig $hostnameIB.gsi.de +short`
#Port=$((5550 + $HOSTCLEAN))

## => Hostname and port for replay withing mFLES (could also be set to "*")
ipaddrIB=*

LogFile=/home/loizeau/rep_${HOSTNAME}_${_run_id}.log

echo "${ipaddrIB}" $Port "${Filename1};${Filename2};${Filename3};${Filename4};${Filename5};${Filename6};${Filename7};${Filename8};${Filename9}"

tsclient -i file:"${Filename1};${Filename2};${Filename3};${Filename4};${Filename5};${Filename6};${Filename7};${Filename8};${Filename9}"? -P "tcp://${ipaddrIB}:${_port}" --publish-hwm 100 --rate-limit 2 &> $LogFile # ~real readout
