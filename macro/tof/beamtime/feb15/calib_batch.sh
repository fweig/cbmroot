#$ -wd /hera/cbm/users/nh/CBM/cbmroot/trunk/macro/tof/beamtime/feb15
#$ -j y
# Number of subtasks to start
#$ -t 1

X=$((${SGE_TASK_ID} - 1))
XXX=$(printf "%03d" "$X")

if [[ ${RunId} = "" ]]; then
RunId="$1"
fi

source /hera/cbm/users/nh/CBM/cbmroot/trunk/build/config.sh 

wdir=/hera/cbm/users/nh/CBM/cbmroot/trunk/macro/tof/beamtime/feb15
outdir=/hera/cbm/users/nh/CBM/cbmroot/trunk/macro/tof/beamtime/feb15/${RunId}
mkdir ${outdir}

cd  ${wdir}
source ./init_calib.sh ${RunId}

cd  ${wdir}
source ./iter_calib.sh ${RunId}

cp -v ${SGE_STDOUT_PATH} ${outdir}/${JOB_ID}.${SGE_TASK_ID}.log
  
