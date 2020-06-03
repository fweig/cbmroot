#$ -wd /hera/cbm/users/nh/CBM/cbmroot/trunk/macro/tof/beamtime/nov15
#$ -j y
# Number of subtasks to start
#$ -t 1

X=$((${SGE_TASK_ID} - 1))
XXX=$(printf "%03d" "$X")

if [[ ${RunId} = "" ]]; then
RunId="$1"
fi

if [[ ${CalSet} = "" ]]; then
CalSet="$2"
fi

if [[ ${Sel2} = "" ]]; then
Sel2="$3"
fi

source /hera/cbm/users/nh/CBM/cbmroot/trunk/build/config.sh 

wdir=/hera/cbm/users/nh/CBM/cbmroot/trunk/macro/tof/beamtime/nov15
outdir=/hera/cbm/users/nh/CBM/cbmroot/trunk/macro/tof/beamtime/nov15/${RunId}
mkdir ${outdir}

cd  ${wdir}
#source ./init_calib.sh ${RunId} ${CalSet}

cd  ${wdir}
source ./iter_calib.sh ${RunId} ${CalSet} ${Sel2}

cd  ${wdir}
source ./gen_digi.sh ${RunId} ${CalSet} ${Sel2}

mv -v ${SGE_STDOUT_PATH} ${outdir}/Calib_${RunId}_${CalSet}_${Sel2}.log

