#!/bin/bash
# Copyright (C) 2015 Justus-Liebig-Universitaet Giessen, Giessen
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Egor Ovcharenko

PREFIX="/store/cbm_rich_data_2014/WLS/WLS_off/nonstretched/"
SUFFIX="offset00100/"

FOLDERS[1]=$PREFIX"ringD1/"$SUFFIX
FOLDERS[2]=$PREFIX"ringDG1/"$SUFFIX
FOLDERS[3]=$PREFIX"ringDG2/"$SUFFIX
FOLDERS[4]=$PREFIX"ringG1/"$SUFFIX
FOLDERS[5]=$PREFIX"ringGH1/"$SUFFIX
FOLDERS[6]=$PREFIX"ringGH2/"$SUFFIX
FOLDERS[7]=$PREFIX"ringGH_redo/"$SUFFIX
FOLDERS[8]=$PREFIX"ringG_redo/"$SUFFIX
FOLDERS[9]=$PREFIX"ringH1/"$SUFFIX
FOLDERS[10]=$PREFIX"ringH2/"$SUFFIX
FOLDERS[11]=$PREFIX"ringH_redo/"$SUFFIX

OUTPUT_FOLDER=output/
CALIB_FOLDER=calib/

PIDs=()

for i in {1..11}
do
	for IN_FILE in ${FOLDERS[$i]}*
	do
		FILENAME=`basename ${IN_FILE}`

		OUTPUT_FILE=${OUTPUT_FOLDER}${FILENAME}.root
		TMP_OUTPUT_FILE=${OUTPUT_FOLDER}tmp_${FILENAME}.root
		CALIB_FILE=${CALIB_FOLDER}calib_${FILENAME}.root
		HISTO_FILE=${OUTPUT_FOLDER}histo_${FILENAME}.root
		HISTO2_FILE=${OUTPUT_FOLDER}histo2_${FILENAME}.root
		TEXT_OUT_FILE=${OUTPUT_FOLDER}out_${FILENAME}.txt

#		echo ${FILENAME} ${IN_FILE} ${OUTPUT_FILE}
#		echo "           "${TMP_OUTPUT_FILE} ${CALIB_FILE} ${HISTO_FILE} ${TEXT_OUT_FILE}

		xterm -geometry 100x40 -e "./single_thread.sh ${IN_FILE} ${OUTPUT_FILE} ${CALIB_FILE} ${HISTO_FILE} ${HISTO2_FILE} ${TMP_OUTPUT_FILE} ${TEXT_OUT_FILE}"&

		PIDs+=($!)
	done

	for curPID in ${PIDs[@]}; do
		wait $curPID
	done
	unset PIDs

done
