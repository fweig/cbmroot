#!/bin/bash

NUM=0
QUEUE=""
PIDs=()

MAX_NPROC=8

WLS=$1
RINGPOS=$2

INPUT_SET_OF_FILE=/store/cbm_rich_data_2014/WLS/WLS_${WLS}/nonstretched/ring${RINGPOS}/offset00250/te*.hld
OUTPUT_FOLDER=output/output_WLS_${WLS}_${RINGPOS}/
OUTPUT_PICS_FOLDER=pictures/pics_WLS_${WLS}_${RINGPOS}/
CALIB_FOLDER=../Fall2015/calib_save/

function AddToTheQueue {
   PIDs+=($PID)
   QUEUE="$QUEUE $1"
   NUM=$(($NUM+1))
   #echo $NUM: $QUEUE
}

function RegenerateQueue {
   OLDREQUEUE=$QUEUE
   QUEUE=""
   NUM=0
   for v_PID in $OLDREQUEUE; do
      if [ -d /proc/$PID ]; then
         QUEUE="$QUEUE $v_PID"
         NUM=$(($NUM+1))
      fi
   done
}

function CheckQueue {
	OLDREQUEUE=$QUEUE
	for v_PID in $OLDREQUEUE; do
		if [ ! -d /proc/$PID ]; then
			RegenerateQueue
			break
		fi
	done
}

# Main program
for IN_FILE in ${INPUT_SET_OF_FILE}
do
   FILENAME=`basename ${IN_FILE}`

   OUTPUT_FILE=${OUTPUT_FOLDER}${FILENAME}.root
   TMP_OUTPUT_FILE=${OUTPUT_FOLDER}tmp_${FILENAME}.root
   CALIB_FILE=${CALIB_FOLDER}calib_${FILENAME}.root
   HISTO_FILE=${OUTPUT_FOLDER}histo_${FILENAME}.root
   HISTO2_FILE=${OUTPUT_FOLDER}histo2_${FILENAME}.root
   TEXT_OUT_FILE=${OUTPUT_FOLDER}out_${FILENAME}.txt
   TEXT_ERR_FILE=${OUTPUT_FOLDER}err_${FILENAME}.txt
   
   root -l -b -q "run_analysis_laser2.C(kFALSE, \"$IN_FILE\", \"$OUTPUT_FILE\", \"$CALIB_FILE\", \"$HISTO_FILE\", \"$HISTO2_FILE\", \"$FILENAME\", -1)" > $TEXT_OUT_FILE 2> $TEXT_ERR_FILE &

   PID=$!
   AddToTheQueue $PID

   while [ $NUM -ge $MAX_NPROC ]; do
      CheckQueue
      sleep 1
   done

done

wait # wait for all processes to finish before exit

#for curPID in ${PIDs[@]}; do
#	wait $curPID
#done

SUM_FILE=sum_WLS_${WLS}_${RINGPOS}.root
hadd ${OUTPUT_FOLDER}${SUM_FILE} ${OUTPUT_FOLDER}histo2_* > ${OUTPUT_FOLDER}hadd_log.txt 2> ${OUTPUT_FOLDER}hadd_log.txt

#./draw_all_pictures.sh ${OUTPUT_FOLDER}${SUM_FILE} ${OUTPUT_PICS_FOLDER} ${OUTPUT_PICS_FOLDER}pics_log.txt
#tar -cvzf ${OUTPUT_PICS_FOLDER}pics_WLS_${WLS}_${RINGPOS}.tar.gz ${OUTPUT_PICS_FOLDER}*.png > ${OUTPUT_PICS_FOLDER}pics_log.txt 2> ${OUTPUT_PICS_FOLDER}pics_log.txt

echo "DONE WLS ${WLS} RING POS ${RINGPOS}"
