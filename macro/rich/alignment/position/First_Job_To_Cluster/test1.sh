#!/bin/bash
# Copyright (C) 2019 Justus-Liebig-Universitaet Giessen, Giessen
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Jordan Bendarouach


# Execute application code
hostname ; uptime ; sleep 5 ; uname -a
echo 'task ID: ' $SLURM_ARRAY_TASK_ID
