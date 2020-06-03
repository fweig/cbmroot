#$ -wd /hera/cbm/users/nh/CBM/cbmroot/trunk/macro/tof/beamtime/nov15
#$ -j y
# Number of subtasks to start
#$ -t 1

X=$((${SGE_TASK_ID} - 1))
XXX=$(printf "%03d" "$X")

if [[ ${RunId} = "" ]]; then
RunId="$1"
fi

source /hera/cbm/users/nh/CBM/cbmroot/trunk/build/config.sh 
 
wdir=/hera/cbm/users/nh/CBM/cbmroot/trunk/macro/tof/beamtime/nov15
outdir=/hera/cbm/users/nh/CBM/cbmroot/trunk/macro/tof/beamtime/nov15/${RunId}
mkdir ${outdir}

cd  ${wdir}
if (( 0 )) ; then
rm  -v ./TofTdcCalibHistos_batch.root
root -b -q 'unpackCalib.C("'${RunId}'")'
mv  -v  ./TofTdcCalibHistos_Tof_calib_batch.root ./TofTdcCalibHistos_${RunId}.root
ln  -s  ./TofTdcCalibHistos_${RunId}.root ./TofTdcCalibHistos_batch.root

root -b -q 'unpackRun.C("'${RunId}'")'
rm ./TofTdcCalibHistos_Tof_calib_batch.root
cp ./tofMbsCal.hst.root ./tofMbsCal_${RunId}.hst.root
else
root -b -q 'create_calib.C(10000000000,"'${RunId}'")'
root -b -q 'apply_calib.C(10000000000,"'${RunId}'")'
fi

mv -v ${SGE_STDOUT_PATH} ${outdir}/unpack_${RunId}_${SGE_TASK_ID}.log
  
