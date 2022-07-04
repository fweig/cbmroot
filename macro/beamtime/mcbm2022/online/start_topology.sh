#!/bin/bash
# Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# author: Pierre-Alain Loizeau [committer]

if [ $# -eq 4 ]; then
  _run_id=$1
  _nbbranch=$2
  _TriggSet=$3
  _Disk=$4
  if [ ${_nbbranch} -eq 0 ]; then
    echo 'Nb branches cannot be 0! At least one branch is needed!'
    return -1
  fi
  if [ ${_Disk} -lt 0 ] || [ ${_Disk} -gt 3 ]; then
    echo 'Disk index on the en13-16 nodes can only be in [0-3]!'
    return -1
  fi
else
  echo 'Missing parameters. Only following pattern allowed:'
  echo 'start_topology.sh <Run Id> <Nb // branches> <Trigger set> <Storage disk index>'

  return -1
fi

((_nbjobs = 4 + $_nbbranch*2 ))
_log_folder="/local/mcbm2022/online_logs/${_run_id}"
_log_config="-D ${_log_folder} -o ${_run_id}_%A_%a.out.log -e ${_run_id}_%A_%a.err.log"

# Create the log folders
sbatch -w en13 create_log_folder.sbatch ${_run_id}
sbatch -w en14 create_log_folder.sbatch ${_run_id}
sbatch -w en15 create_log_folder.sbatch ${_run_id}
sbatch -w en16 create_log_folder.sbatch ${_run_id}

# Online ports
sbatch -w en13 ${_log_config} mq_processing_node.sbatch ${_run_id} ${_nbbranch} ${_TriggSet} ${_Disk} node8ib2:5560
sbatch -w en14 ${_log_config} mq_processing_node.sbatch ${_run_id} ${_nbbranch} ${_TriggSet} ${_Disk} node8ib2:5561
sbatch -w en15 ${_log_config} mq_processing_node.sbatch ${_run_id} ${_nbbranch} ${_TriggSet} ${_Disk} node9ib2:5560
sbatch -w en16 ${_log_config} mq_processing_node.sbatch ${_run_id} ${_nbbranch} ${_TriggSet} ${_Disk} node9ib2:5561

# Replay ports
#sbatch -w en13 ${_log_config} mq_processing_node.sbatch ${_run_id} ${_nbbranch} ${_TriggSet} ${_Disk} node8ib2:5557
#sbatch -w en14 ${_log_config} mq_processing_node.sbatch ${_run_id} ${_nbbranch} ${_TriggSet} ${_Disk} node8ib2:5557
#sbatch -w en15 ${_log_config} mq_processing_node.sbatch ${_run_id} ${_nbbranch} ${_TriggSet} ${_Disk} node9ib2:5557
#sbatch -w en16 ${_log_config} mq_processing_node.sbatch ${_run_id} ${_nbbranch} ${_TriggSet} ${_Disk} node9ib2:5557
