#!/bin/bash
PREFIX="/store/cbm_rich_data_2014/WLS/WLS_on/nonstretched/"
SUFFIX="offset00250/"

FOLDERS[1]=$PREFIX"ringD1/"$SUFFIX
FOLDERS[2]=$PREFIX"ringDG1/"$SUFFIX
FOLDERS[3]=$PREFIX"ringG1/"$SUFFIX
FOLDERS[4]=$PREFIX"ringGH1/"$SUFFIX
FOLDERS[5]=$PREFIX"ringH1/"$SUFFIX
FOLDERS[6]=$PREFIX"ringH2/"$SUFFIX

OUTPUT_FOLDER=output/
CALIB_FOLDER=calib/

PIDs=()

for i in {1..6}
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
