#!/bin/bash

# param1=1
# param2='test'
# sbatch --array=1-3 -D /lustre/nyx/cbm/users/jbendar/Sim_Outputs/Ring_Track_VS_Position ./test.sh $param1
# sbatch -J position -D /lustre/nyx/cbm/users/jbendar/Sim_Outputs/Ring_Track_VS_Position/Misaligned -o %j_%N.out.log -e %j_%N.err.log --time=0:30:00 --array=1-3 ./test1.sh

# for FLAG in 0 1
# do
# WORK_DIR=/lustre/nyx/cbm/users/jbendar/Sim_Outputs/Ring_Track_VS_Position/Aligned
GEO=5		# select geometry	# (== 0) for Reference ; (== 1) for misaligned w/o corrections 1 mrad ; (== 2) for misaligned w/o corrections 2 mrad ;
		# (== 3) for misaligned w/o corrections 3 mrad ; (== 4) for misaligned w/o corrections 4 mrad ; (== 5) for misaligned w/o corrections 5 mrad ;
		# (== 6) to calculate correction parameters ; (== 7) to run Matching with corrected parameters
NEVTS=1250	# select nb of events to run
COLL_ENERGY=8gev
ARRAY=800

if [ $GEO -eq 0 ] ; then
	sbatch -J matching -D /lustre/nyx/cbm/users/jbendar/Sim_Outputs/Matching/Reference --time=1:00:00 --array=1-$ARRAY --partition=main ./matching.sh $GEO $NEVTS $COLL_ENERGY
elif [ $GEO -eq 1 ] ; then
	sbatch -J matching -D /lustre/nyx/cbm/users/jbendar/Sim_Outputs/Matching/WO_Corrections_1mrad --time=0:40:00 --array=1-$ARRAY ./matching.sh $GEO $NEVTS $COLL_ENERGY
elif [ $GEO -eq 2 ] ; then
	sbatch -J matching -D /lustre/nyx/cbm/users/jbendar/Sim_Outputs/Matching/WO_Corrections_2mrad --time=0:40:00 --array=1-$ARRAY ./matching.sh $GEO $NEVTS $COLL_ENERGY
elif [ $GEO -eq 3 ] ; then
	sbatch -J matching -D /lustre/nyx/cbm/users/jbendar/Sim_Outputs/Matching/WO_Corrections_3mrad --time=0:40:00 --array=1-$ARRAY ./matching.sh $GEO $NEVTS $COLL_ENERGY
##	for NB in 0108 0267 0275 0285 0367 0649 0730
##	do
##		sbatch -J matching -D /lustre/nyx/cbm/users/jbendar/Sim_Outputs/Matching/WO_Corrections_3mrad --time=0:40:00 ./matching.sh $GEO $NEVTS $COLL_ENERGY $NB
##	done
elif [ $GEO -eq 4 ] ; then
	sbatch -J matching -D /lustre/nyx/cbm/users/jbendar/Sim_Outputs/Matching/WO_Corrections_4mrad --time=0:40:00 --array=1-$ARRAY ./matching.sh $GEO $NEVTS $COLL_ENERGY $NB
##	for NB in 0136 0479 0644 0777
##	do
##		sbatch -J matching -D /lustre/nyx/cbm/users/jbendar/Sim_Outputs/Matching/WO_Corrections_4mrad --time=0:40:00 ./matching.sh $GEO $NEVTS $COLL_ENERGY $NB
##	done
elif [ $GEO -eq 5 ] ; then
	sbatch -J matching -D /lustre/nyx/cbm/users/jbendar/Sim_Outputs/Matching/WO_Corrections_5mrad --time=0:40:00 --array=1-$ARRAY ./matching.sh $GEO $NEVTS $COLL_ENERGY
##elif [ $GEO -eq 6 ] ; then
##	sbatch -J matching -D /lustre/nyx/cbm/users/jbendar/Sim_Outputs/Matching/Compute_Corrections --time=0:40:00 --array=1-$ARRAY ./matching.sh $GEO $NEVTS $COLL_ENERGY
##elif [ $GEO -eq 7 ] ; then
##	sbatch -J matching -D /lustre/nyx/cbm/users/jbendar/Sim_Outputs/Matching/W_Corrections --time=0:40:00 --array=1-$ARRAY ./matching.sh $GEO $NEVTS $COLL_ENERGY
fi

##	sbatch -J test_match -D /lustre/nyx/cbm/users/jbendar/Sim_Outputs/test/test_matching --time=1:00:00 --array=1-$ARRAY ./matching.sh $GEO $NEVTS $COLL_ENERGY

# done
