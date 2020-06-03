#!/bin/bash
#SBATCH -J calib
#SBATCH -D /lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2018
#SBATCH --time=6-00:00:00
#SBATCH --mem=4000
#SBATCH --partition=long

X=$((${SLURM_ARRAY_TASK_ID} - 0))
XXX=$(printf "%03d" "$X")

CalSet="$1"
if [[ ${CalSet} = "" ]]; then
  echo please specify CalSet!
  return
fi

Sel2="$2"
if [[ ${Sel2} = "" ]]; then
  echo please specify Sel2!
  return
fi

RunId="$3"
if [[ ${RunId} = "" ]]; then
RunList1=' 
20161121_2133_run1_gdpb
'
    I=1
    RunId=""
    for RunId in $RunList1
    do
    #echo check $RunId
     if (($I == $X)); then
       echo found RunId $RunId
       break
     fi
     ((I=$I+1))
    done
fi

CalMode="$4"
if [[ ${CalMode} = "" ]]; then
#CalMode="_DT200_0x00005006"
#CalMode="_DT50_0x00005006"
#CalMode="_DT50_0x00000000"
#CalMode="_DT50_0x00019026"
#CalMode="_DT50_Req5"
#CalMode="_DT50_Req0"
#CalMode="_DT50000_Req-3"
#CalMode="_DT50000_Req-4"
#CalMode="_DT26000_Req-4"
CalMode=".100.3" 
#CalMode="_DT100_Req-4"
#CalMode="_DT50_Req-3"
#CalMode="_DT50_Req-2"
fi

RunIdMode=`echo $RunId$CalMode`
Deadtime=50

echo define root execution environment at I=$I for Run $RunId Mode $RunIdMode

if [ -e /lustre/nyx ]; then
source /lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/build6/config.sh 
wdir=/lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2018
outdir=/lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2018/${RunId}
else 
wdir=`pwd`
outdir=${wdir}/${RunId}
fi
mkdir ${outdir}

cd  ${wdir}
echo execute: ./init_calib.sh ${RunIdMode} ${CalSet} ${Deadtime}
source ./init_calib.sh ${RunIdMode} ${CalSet} ${Deadtime}
#echo execute: ./init_calib_star.sh ${RunIdMode} ${CalSet} ${Deadtime}
#source ./init_calib_star.sh ${RunIdMode} ${CalSet} ${Deadtime}

cd  ${wdir}
echo execute: ./iter_calib.sh ${RunIdMode} ${CalSet} ${Sel2} ${Deadtime}
source ./iter_calib.sh ${RunIdMode} ${CalSet} ${Sel2} ${Deadtime}

cd  ${wdir}
echo execute: ./gen_digi.sh ${RunIdMode} ${CalSet} ${Sel2} ${Deadtime}
source ./gen_digi.sh ${RunIdMode} ${CalSet} ${Sel2} ${Deadtime}

mv -v slurm-${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out ${outdir}/Calib_${RunIdMode}_${CalSet}_${Sel2}.out
