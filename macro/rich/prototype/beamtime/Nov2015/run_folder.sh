#!/bin/bash

DATAFOLDER=/home/evovch/DATA/threshold_scan_2/part9/
PICTURESFOLDER=pictures/
OUTPUTFOLDER=output/
CALIBFOLDER=calib_save/

runSingleFile () {
#   echo ${1} "---" ${2} "---" ${3} "---" ${4} "---" ${5} "---" ${6} "---" ${7} "---" ${8} "---" ${9} "---" ${10}
#   root -l -b -q "run_analysis_laser.C(kTRUE, \"${1}\", \"${2}\", \"${3}\", \"${4}\", \"${5}\", \"${6}\")" > ${7} 2> ${8}
   root -l -b -q "run_analysis_laser.C(kFALSE, \"${1}\", \"${2}\", \"${3}\", \"${4}\", \"${5}\", \"${6}\")"  > ${9} 2> ${10}
}

for DATAFILE in ${DATAFOLDER}*.hld
do

   DATAFILENAME=$(basename "$DATAFILE")
   OUTROOTFILE=${DATAFILENAME}.root
   CALIBFILENAME=calib_${DATAFILENAME}.root
   HISTO1FILE=histo1_${DATAFILENAME}.root
   HISTO2FILE=histo2_${DATAFILENAME}.root
   OUTFILENAME1=out1_${DATAFILENAME}.txt
   ERRFILENAME1=err1_${DATAFILENAME}.txt
   OUTFILENAME2=out2_${DATAFILENAME}.txt
   ERRFILENAME2=err2_${DATAFILENAME}.txt

   runSingleFile ${DATAFOLDER}${DATAFILENAME} ${OUTPUTFOLDER}${OUTROOTFILE} ${CALIBFOLDER}${CALIBFILENAME} ${OUTPUTFOLDER}${HISTO1FILE} ${OUTPUTFOLDER}${HISTO2FILE} ${DATAFILENAME} ${OUTPUTFOLDER}${OUTFILENAME1} ${OUTPUTFOLDER}${ERRFILENAME1} ${OUTPUTFOLDER}${OUTFILENAME2} ${OUTPUTFOLDER}${ERRFILENAME2} &

done

wait

#hadd ${OUTPUTFOLDER}sum_histo1.root ${OUTPUTFOLDER}histo1*
#hadd ${OUTPUTFOLDER}sum_histo2.root ${OUTPUTFOLDER}histo2*

echo "DONE"
