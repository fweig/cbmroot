#
##   create mcbm_sim.C from patch file
#
# diff -c ../run/run_sim.C mcbm_sim.C > run_sim_to_mcbm_sim.patch
#
# cp ../run/run_sim.C mcbm_sim.C
# patch mcbm_sim.C run_sim_to_mcbm_sim.patch
# root -l mcbm_sim.C
#
SOURCE=../run/run_sim.C
TARGET=mcbm_sim.C
PATCH=run_sim_to_mcbm_sim.patch
#
echo 
echo cp $SOURCE $TARGET
echo patch $TARGET $PATCH
echo root -l $TARGET
echo 
#
cp $SOURCE $TARGET
patch $TARGET $PATCH
root -l $TARGET
