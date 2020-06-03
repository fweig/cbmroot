#$ -wd /hera/cbm/users/nh/CBM/cbmroot/trunk/macro/tof/beamtime/nov15
#$ -j y
# Number of subtasks to start
#$ -t 1

X=$((${SGE_TASK_ID} - 1))
XXX=$(printf "%03d" "$X")

if [[ ${RunId} = "" ]]; then
RunId="$1"
fi

if [[ ${Dut} = "" ]]; then
Dut="$2"
fi

if [[ ${Mref} = "" ]]; then
Mref="$3"
fi

if [[ ${Sel2} = "" ]]; then
Sel2="$4"
fi

if [[ ${InSet} = "" ]]; then
InSet="$5"
fi

if [[ ${ScalFac} = "" ]]; then
ScalFac="$6"
fi

source /hera/cbm/users/nh/CBM/cbmroot/trunk/build/config.sh 

wdir=/hera/cbm/users/nh/CBM/cbmroot/trunk/macro/tof/beamtime/nov15
outdir=/hera/cbm/users/nh/CBM/cbmroot/trunk/macro/tof/beamtime/nov15/hst

cd  ${wdir}
source ./iter_hits.sh ${RunId} ${Dut} ${Mref} ${Sel2} ${InSet} ${ScalFac}
  
mv -v ${SGE_STDOUT_PATH} ${outdir}/anahit_${RunId}_${Dut}${Mref}${Sel2}_${InSet}_${ScalFac}.out
