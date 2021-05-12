#!/bin/bash
# Copyright (C) 2019 Justus-Liebig-Universitaet Giessen, Giessen
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Jordan Bendarouach


# param1=1
# param2='test'
# sbatch --array=1-3 -D /lustre/nyx/cbm/users/jbendar/Sim_Outputs/Ring_Track_VS_Position ./test.sh $param1
# sbatch -J position -D /lustre/nyx/cbm/users/jbendar/Sim_Outputs/Ring_Track_VS_Position/Misaligned -o %j_%N.out.log -e %j_%N.err.log --time=0:30:00 --array=1-3 ./test1.sh

FLAAG=0
NEVTS=2
COLL_ENERGY=8gev
	sbatch -J matching -D /lustre/nyx/cbm/users/jbendar/Sim_Outputs/Matching/Reference/ -o %j_%N.out.log -e %j_%N.err.log --time=0:10:00 --array=1-2 ./matching.sh $FLAG $NEVTS $COLL_ENERGY

# for FLAG in 0 1
# do
# WORK_DIR=/lustre/nyx/cbm/users/jbendar/Sim_Outputs/Ring_Track_VS_Position/Aligned
#FLAG=0		# (== 0) for aligned case ; (== 1) for misaligned case ; (== 2) for standard v16a_1e geo
#NEVTS=5000	# nb of events to run
#COLL_ENERGY=8gev
##	sbatch -J position -D /lustre/nyx/cbm/users/jbendar/Sim_Outputs/Ring_Track_VS_Position/Aligned -o %j_%N.out.log -e %j_%N.err.log --time=2:00:00 --array=1-200 ./position.sh $FLAG $NEVTS $COLL_ENERGY
##	sbatch -J test_posi -D /lustre/nyx/cbm/users/jbendar/Sim_Outputs/Ring_Track_VS_Position/Aligned -o %j_%N.out.log -e %j_%N.err.log --time=0:10:00 --array=1-2 ./matching.sh $FLAG $NEVTS $COLL_ENERGY
##	sbatch -J position -D /lustre/nyx/cbm/users/jbendar/Sim_Outputs/Ring_Track_VS_Position/Misaligned -o %j_%N.out.log -e %j_%N.err.log --time=2:00:00 --array=1-200 ./position.sh $FLAG $NEVTS $COLL_ENERGY
##	sbatch -J position -D /lustre/nyx/cbm/users/jbendar/Sim_Outputs/Ring_Track_VS_Position/Standard -o %j_%N.out.log -e %j_%N.err.log --time=2:00:00 --array=1-200 ./position.sh $FLAG $NEVTS $COLL_ENERGY
# done
