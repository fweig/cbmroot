#!/bin/bash

# Task name
#SBATCH -J test

# Run time limit
#SBATCH --time=1:00:00

# Working directory on shared storage
#SBATCH -D /lustre/nyx/cbm/users/jbendar

# Standard and error output in different files
#SBATCH -o %j_%N.out.log
#SBATCH -e %j_%N.err.log

# #SBATCH --array=1-3
# #SBATCH --ntasks 2

# Execute application code
hostname ; uptime ; sleep 5 ; uname -a
echo 'task ID: ' $SLURM_ARRAY_TASK_ID
if [ $1 = "1" ] ; then
        echo 'It works: FLAG = ' $1
elif [ $1 != "1" ] ; then
	echo 'Does not work: FLAG = ' $1
fi
# echo 'param2: ' $2 			# second parameter value
