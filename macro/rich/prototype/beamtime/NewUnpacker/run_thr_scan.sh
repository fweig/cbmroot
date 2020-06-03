#!/bin/bash

INPUT_ROOT_FOLDER=/store/Wuppertal_data_jul2015/threshold_scan_1/

OUTPUT_FOLDER=output/
CALIB_FOLDER=calib/

for IN_FILE in ${INPUT_ROOT_FOLDER}te*; do

	FILENAME=`basename ${IN_FILE}`
	OUTPUT_FILE=${OUTPUT_FOLDER}${FILENAME}.root
	TMP_OUTPUT_FILE=${OUTPUT_FOLDER}tmp_${FILENAME}.root
	CALIB_FILE=${CALIB_FOLDER}calib_${FILENAME}.root
	HISTO_FILE=${OUTPUT_FOLDER}histo_${FILENAME}.root
	TEXT_OUT_FILE=${OUTPUT_FOLDER}out_${FILENAME}.txt

	xterm -geometry 100x40 -e "./single_thread.sh ${IN_FILE} ${OUTPUT_FILE} ${CALIB_FILE} ${HISTO_FILE} ${TMP_OUTPUT_FILE} ${TEXT_OUT_FILE}"&

#	exit

done
