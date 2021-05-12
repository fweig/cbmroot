#!/bin/bash
# Copyright (C) 2016 PI-UHd,GSI
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Norbert Herrmann

#SBATCH -J unpack
#SBATCH -D /lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/cern2016
#SBATCH --time=8:00:00
##SBATCH --mem=2000
##SBATCH --partition=long

X=$((${SLURM_ARRAY_TASK_ID} - 0))
XXX=$(printf "%03d" "$X")

DeltaT=$1
if [[ ${DeltaT} = "" ]]; then
DeltaT=200.
fi

ReqDet=$2
if [[ ${ReqDet} = "" ]]; then
ReqDet=0
fi

EpSupp=$3
if [[ ${EpSupp} = "" ]]; then
EpSupp=1
fi

RunId="$4"
if [[ ${RunId} = "" ]]; then
    RunList1=' 
20161121_2133_run1_gdpb
20161121_2248_run2_all_beam
20161121_2333_run3_all_beam
20161121_2356_run4_all_beam
20161122_0018_run5_all_beam
20161122_0039_run6_all_beam
20161122_0105_run7_all_beam
20161122_1023_run8_gdpb_beam
XXXX
'
RunList2=' 
r0002_20161122_0954
r0003_20161122_1036
r0004_20161122_1112
r0005_20161122_1519
r0006_20161122_1529
r0007_20161122_1608
r0008_20161122_1642
r0009_20161122_1749
r0010_20161122_1815
r0011_20161122_1844
r0012_20161122_1917
r0013_20161122_2112
r0014_20161122_2136
r0015_20161122_2212
r0016_20161122_2249
r0017_20161122_2326
r0018_20161123_0008
r0019_20161123_0045
r0020_20161123_0122
r0021_20161123_0152
r0022_20161123_0227
r0023_20161123_0300
r0024_20161123_0334
r0025_20161123_0346
r0026_20161123_0350
r0027_20161123_0353
r0028_20161123_0356
r0029_20161123_0406
r0030_20161123_0444
r0031_20161123_0516
r0032_20161123_0538
r0033_20161123_0600
r0034_20161123_0606
r0035_20161123_0610
XXXX
'
RunList3=' 
r0001_20161129_2253
r0002_20161129_2254
r0003_20161129_2300
r0004_20161130_1743
r0005_20161130_2115
r0006_20161130_2142
r0007_20161130_2157
r0008_20161201_0056
r0009_20161201_0150
r0010_20161201_0213
r0011_20161201_0241
r0012_20161201_0312
r0013_20161201_0336
r0014_20161201_0402
r0015_20161201_0446
r0016_20161201_0512
r0017_20161201_0533
r0018_20161201_0600
r0019_20161201_0632
r0020_20161201_0700
r0021_20161201_0953
r0022_20161201_1027
r0023_20161201_1045
r0024_20161201_1121
r0025_20161201_1256
r0026_20161201_1309
r0027_20161201_1339
r0028_20161201_1438
r0029_20161201_1440
r0030_20161201_1443
r0040_20161201_1839
r0041_20161201_2008
r0042_20161201_2053
r0043_20161201_2335
r0044_20161202_0017
r0045_20161202_0051
r0046_20161202_0130
r0047_20161202_0211
r0048_20161202_0253
r0049_20161202_0323
r0050_20161202_0336
r0051_20161202_0409
r0052_20161202_0437
r0053_20161202_0503
r0054_20161202_0535
r0055_20161202_0538
r0056_20161202_0629
r0057_20161202_0728
r0058_20161202_0749
r0059_20161202_0756
r0060_20161202_0828
r0061_20161202_0832
r0062_20161202_0849
r0063_20161202_0903
r0064_20161202_0922
r0065_20161202_0928
r0066_20161202_0933
r0067_20161202_0945
r0068_20161202_0948
r0069_20161202_0950
r0070_20161202_0953
r0071_20161202_0954
r0072_20161202_0957
r0073_20161202_0958
r0074_20161202_1000
r0075_20161202_1002
r0076_20161202_1004
r0077_20161202_1006
r0078_20161202_1011
r0079_20161202_1013
r0080_20161202_1015
r0081_20161202_1018
r0082_20161202_1021
r0083_20161202_1023
r0084_20161202_1025
r0085_20161202_1027
r0086_20161202_1031
r0087_20161202_1033
XXXX
'
    I=1
    RunId=""
    for RunId in $RunList3
    do
    #echo check $RunId
     if (($I == $X)); then
       echo found RunId $RunId
       break
     fi
     ((I=$I+1))
    done
fi

if [ -e /lustre/nyx ]; then
echo define root execution environment at I=$I for Run $RunId

source /lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/build6/config.sh 
wdir=/lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/cern2016
outdir=/lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/cern2016/${RunId}
else 
wdir=`pwd`
outdir=${wdir}/${RunId}
fi
mkdir ${outdir}

cd  ${wdir}
root -b -q './unpack_tsa.C(-1,'$DeltaT','$ReqDet','$EpSupp',"'$RunId'") '

mv -v slurm-${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out ${outdir}/Unpack_$DeltaT_$ReqDet_${RunId}.out
