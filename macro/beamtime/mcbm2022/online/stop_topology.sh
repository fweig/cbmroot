#!/bin/bash
# Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# author: Pierre-Alain Loizeau [committer]

if [ $# -eq 1 ]; then
  _run_id=$1
else
  echo 'Missing parameters. Only following pattern allowed:'
  echo 'stop_topology.sh <Run Id> '

  return -1
fi

((_nbjobs = 4 + $_nbbranch*2 ))
_log_folder="/local/mcbm2022/online_logs/${_run_id}"
_log_config="-D ${_log_folder} -o ${_run_id}_%A_%a.out.log -e ${_run_id}_%A_%a.err.log"


# Online ports
sbatch -w en13 ${_log_config} mq_shutdown.sbatch ${_run_id}
sbatch -w en14 ${_log_config} mq_shutdown.sbatch ${_run_id}
sbatch -w en15 ${_log_config} mq_shutdown.sbatch ${_run_id}
sbatch -w en16 ${_log_config} mq_shutdown.sbatch ${_run_id}
