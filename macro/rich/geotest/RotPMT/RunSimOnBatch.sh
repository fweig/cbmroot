#!/bin/sh
#$ -wd /tmp 
OnBatch=0
nEvs=1
rotx=0
roty=0
transy=0
transz=0

ThetaMin=250 #devide by 10 in cpp
ThetaMax=2500 #devide by 10 in cpp
PhiMin=90
PhiMax=180

GeoCase=2
DimCase=2
EnlargedPMTWidth=2
EnlargedPMTHight=4


PtNotP=0
MomMin=0 #devide by 10 in cpp
MomMax=40 #devide by 10 in cpp

rotmir=-10
extendedmir=0

OldCode=0
DefaultDims=0 #for old code					
DefaultDimsLargePMT=0 #for old code
extratext=.

while [ $# -gt 0 ]; do
    case "$1" in
	-extt) shift; extratext=$1 ;;
	-batch) shift; OnBatch=$1 ;;
	-nev) shift; nEvs=$1 ;;
	-rx) shift; rotx=$1 ;;
	-ry) shift; roty=$1 ;;
	-dy) shift; transy=$1 ;;
	-dz) shift; transz=$1 ;;
	
	-mint) shift; ThetaMin=$1 ;;
	-maxt) shift; ThetaMax=$1 ;;
	-minp) shift; PhiMin=$1 ;;
	-maxp) shift; PhiMax=$1 ;;

	-geo) shift; GeoCase=$1 ;;
	-dim) shift; DimCase=$1 ;;
	-pmtw) shift; EnlargedPMTWidth=$1 ;;
	-pmth) shift; EnlargedPMTHight=$1 ;;

	-ptp) shift; PtNotP=$1 ;;
	-minm) shift; MomMin=$1 ;;
	-maxm) shift; MomMax=$1 ;;

	-mir) shift; rotmir=$1 ;;
	-extm) shift; extendedmir=$1 ;;

	-oldc) shift; OldCode=$1 ;;
	-ddim) shift; DefaultDims=$1 ;;
	-pmtd) shift; DefaultDimsLargePMT=$1 ;;
	
#	--)        shift ; break ;;
#	*)         echo "unknown option: $1" ;
    esac
    shift
done


#echo "nev=$nev"
#echo "rotx=$rotx"
#echo "roty=$roty"
#echo "transy=$transy"
#echo "transz=$transz"

#echo "ThetaMin=$ThetaMin"
#echo "ThetaMax=$ThetaMax"
#echo "PhiMin=$PhiMin"
#echo "PhiMax=$PhiMax"

#echo "GeoCase=$GeoCase"
#echo "DimCase=$DimCase"
#echo "EnlargedPMTWidth=$EnlargedPMTWidth"
#echo "EnlargedPMTHight=$EnlargedPMTHight"
echo "######################### PtNotP=$PtNotP"
echo "MomMin=$MomMin"
echo "MomMax=$MomMax"

#echo "rotmir=$rotmir"
#echo "extendedmir=$extendedmir"

#echo "OldCode=$OldCode"
#echo "DefaultDims=$DefaultDims"
#echo "DefaultDimsLargePMT=$DefaultDimsLargePMT"

outdir=/hera/cbm/users/tariq/GeoOptRootFiles/
cbmroot_config_path=/hera/cbm/users/tariq/cbmroot/buildcbm/config.sh
macro_dir=/hera/cbm/users/tariq/cbmroot/macro/rich/geotest/RotPMT
# Needed to run macro via script
export EXTRATEXT=$extratext
export SCRIPT=yes
export N_EVS=$nEvs
export PMT_ROTX=$rotx
export PMT_ROTY=$roty
export PMT_TRAY=$transy
export PMT_TRAZ=$transz

export THETAMIN=$ThetaMin
export THETAMAX=$ThetaMax
export PHIMIN=$PhiMin
export PHIMAX=$PhiMax

export GEO_CASE=$GeoCase
export DIM_CASE=$DimCase
export ENL_PMTWIDTH=$EnlargedPMTWidth
export ENL_PMTHIGHT=$EnlargedPMTHight

export PT_NOT_P=$PtNotP
export MOM_MIN=$MomMin
export MOM_MAX=$MomMax

export ROTMIR=$rotmir
export EXTENDEDMIR=$extendedmir
export OLDCODE=$OldCode
export DEFAULDIMS=$DefaultDims
export DEFAULDIMSLPMT=$DefaultDimsLargePMT

if [ $OnBatch == 0 ];then 
    root -b -l -q "Run_Sim_GeoOpt_Batch.C()"
    root -b -l -q "Run_Reco_GeoOpt_Batch.C()"
    root -b -l -q "Run_Ana_GeoOpt_Batch.C()"
    return
fi

# setup the run environment
source ${cbmroot_config_path}
 
# This line is needed, otherwise root will crash
export DISPLAY=localhost:0.0

#create needed directories
mkdir -p ${outdir}/log

# create special and enter special workdir for job
username=$(whoami)
#workdir=/tmp/$username/$rotmir.$rotx.$roty.$GeoCase.$PtNotP.
workdir=/tmp/$username/$JOB_ID.$SGE_TASK_ID
mkdir -p $workdir
cd $workdir

root -b -l -q "${macro_dir}/Run_Sim_GeoOpt_Batch.C()"
root -b -l -q "${macro_dir}/Run_Reco_GeoOpt_Batch.C()"
root -b -l -q "${macro_dir}/Run_Ana_GeoOpt_Batch.C()"

cp -v ${SGE_STDOUT_PATH} ${outdir}/log/"RX.${rotx}.RY.${roty}.TransY.${transy}.TransZ.${transz}.DeDi.${DefaultDims}.LPMT.${DefaultDimsLargePMT}.${JOB_ID}.${SGE_TASK_ID}.log"
#cp -v ${SGE_STDOUT_PATH} ${outdir}/log/${JOB_ID}.${SGE_TASK_ID}.log

export SCRIPT=no
