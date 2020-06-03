#!/bin/bash

# Execute application code
hostname ; uptime ; sleep 5 ; uname -a
echo 'task ID: ' $SLURM_ARRAY_TASK_ID
