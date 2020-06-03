#!/bin/bash

# param1=1
# param2='test'
# sbatch --array=1-3 -D /lustre/nyx/cbm/users/jbendar/Sim_Outputs/Ring_Track_VS_Position ./test.sh $param1
# sbatch -J position -D /lustre/nyx/cbm/users/jbendar/Sim_Outputs/Ring_Track_VS_Position/Misaligned -o %j_%N.out.log -e %j_%N.err.log --time=0:30:00 --array=1-3 ./test1.sh

# for FLAG in 0 1
# do
# WORK_DIR=/lustre/nyx/cbm/users/jbendar/Sim_Outputs/Ring_Track_VS_Position/Aligned
GEO=30		# select geometry	# (== 0) for aligned case ; (== 1) for misaligned 1 mrad case ; (== 2) for misaligned 2 mrad case ; (== 30) for misaligned 3 mrad XY case ;
		# (== 31) for misaligned 3 mrad -X case ; (== 32) for misaligned 3 mrad -Y case ; (== 33) for misaligned 3 mrad -XY case ; (== 4) for misaligned 4 mrad case ;
		# (== 5) for misaligned 5 mrad case ; (== 6) for misaligned OFF case
NEVTS=1250	# select nb of events to run
COLL_ENERGY=8gev
ARRAY=800
DIR1=/lustre/nyx/cbm/users/jbendar/Sim_Outputs/Ring_Track_VS_Position
DIR2=Batch_4

#for GEO in 0 1 2 30 4 5 6
#do

if [ $GEO -eq 0 ] ; then
	sbatch -J position -D ${DIR1}/${DIR2}/Aligned --time=1:00:00 --array=1-$ARRAY ./position.sh $GEO $NEVTS $COLL_ENERGY
elif [ $GEO -eq 1 ] ; then
	sbatch -J position -D ${DIR1}/${DIR2}/Misaligned_1mrad_Full --time=1:00:00 --array=1-$ARRAY ./position.sh $GEO $NEVTS $COLL_ENERGY
elif [ $GEO -eq 2 ] ; then
	sbatch -J position -D ${DIR1}/${DIR2}/Misaligned_2mrad_Full --time=1:00:00 --array=1-$ARRAY ./position.sh $GEO $NEVTS $COLL_ENERGY
elif [ $GEO -eq 30 ] ; then
	sbatch -J position -D ${DIR1}/${DIR2}/Misaligned_3mrad_XY_Full --time=1:00:00 --array=1-$ARRAY ./position.sh $GEO $NEVTS $COLL_ENERGY
elif [ $GEO -eq 31 ] ; then
	sbatch -J position -D ${DIR1}/${DIR2}/Misaligned_3mrad_minX_Full --time=1:00:00 --array=1-$ARRAY ./position.sh $GEO $NEVTS $COLL_ENERGY
elif [ $GEO -eq 32 ] ; then
	sbatch -J position -D ${DIR1}/${DIR2}/Misaligned_3mrad_minY_Full --time=1:00:00 --array=1-$ARRAY ./position.sh $GEO $NEVTS $COLL_ENERGY
elif [ $GEO -eq 33 ] ; then
	sbatch -J position -D ${DIR1}/${DIR2}/Misaligned_3mrad_minXY_Full --time=1:00:00 --array=1-$ARRAY ./position.sh $GEO $NEVTS $COLL_ENERGY
elif [ $GEO -eq 4 ] ; then
	sbatch -J position -D ${DIR1}/${DIR2}/Misaligned_4mrad_Full --time=1:00:00 --array=1-$ARRAY ./position.sh $GEO $NEVTS $COLL_ENERGY
elif [ $GEO -eq 5 ] ; then
	sbatch -J position -D ${DIR1}/${DIR2}/Misaligned_5mrad_Full --time=1:00:00 --array=1-$ARRAY ./position.sh $GEO $NEVTS $COLL_ENERGY
elif [ $GEO -eq 6 ] ; then
	sbatch -J position -D ${DIR1}/${DIR2}/Misaligned_OFF --time=1:00:00 --array=1-$ARRAY ./position.sh $GEO $NEVTS $COLL_ENERGY
fi

#done

##	sbatch -J test_posi -D /lustre/nyx/cbm/users/jbendar/Sim_Outputs/Ring_Track_VS_Position/Aligned -o %j_%N.out.log -e %j_%N.err.log --time=2:00:00 --array=1-200 ./position.sh $GEO1 $NEVTS $COLL_ENERGY
##	sbatch -J test_posi -D /lustre/nyx/cbm/users/jbendar/Sim_Outputs/test_position --time=2:00:00 --array=1-200 ./position.sh $GEO1 $NEVTS $COLL_ENERGY
