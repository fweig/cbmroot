#$ -wd /hera/cbm/users/nh/CBM/cbmroot/trunk/macro/tof/beamtime/nov15
#$ -j y
# Number of subtasks to start
#$ -t 1

X=$((${SGE_TASK_ID} - 1))
XXX=$(printf "%03d" "$X")

source /hera/cbm/users/nh/CBM/cbmroot/trunk/build/config.sh 

wdir=/hera/cbm/users/nh/CBM/cbmroot/trunk/macro/tof/beamtime/nov15
outdir=/hera/cbm/users/nh/CBM/cbmroot/trunk/macro/tof/beamtime/nov15/${RunId}
mkdir ${outdir}

cd  ${wdir}
rm  ./TofTdcCalibHistos_batch.root
root -b -q 'unpackCalib.C("'${RunId}'")'

mv  ./TofTdcCalibHistos_Tof_calib_batch.root ./TofTdcCalibHistos_${RunId}.root
ln -s  ./TofTdcCalibHistos_${RunId}.root ./TofTdcCalibHistos_batch.root

root -b -q 'unpackRun.C("'${RunId}'")'
rm ./TofTdcCalibHistos_Tof_calib_batch.root
cp ./tofMbsCal.hst.root ./tofMbsCal_${RunId}.hst.root

cp -v ${SGE_STDOUT_PATH} ${outdir}/${JOB_ID}.${SGE_TASK_ID}.log
  
