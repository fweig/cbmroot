#!/bin/sh

export SCRIPT=yes

filesArray=( ${1}/*.hld )

export OUTPUT_DIR=/home/evovch/cbmroot/trunk/macro/rich/prototype/beamtime/NewUnpacker/output250_/

# Change this line according to your needs
export RUN_TITLE=WLS_off_nonstr_A1_250

PIDs=()

for curFile in ${filesArray[@]}; do

	export INPUT_HLD_FILE=${curFile}
	filename=$(basename "$curFile")
	export OUTPUT_ROOT_FILE=${OUTPUT_DIR}/${filename}.root
	export OUTPUT_HISTO_FILE=${OUTPUT_DIR}/${filename}.histo.root

#	echo $INPUT_HLD_FILE
#	echo $OUTPUT_ROOT_FILE
#	echo $OUTPUT_HISTO_FILE

	xterm -e "root -l -b -q run_analysis2.C"&

	# Store the PID of the process launched in the background
	PIDs+=($!)
done

# Barrier - wait for all the analysis processes to sum the histograms after
for curPID in ${PIDs[@]}; do
	wait $curPID	
done

hadd ${OUTPUT_DIR}/total_histos.root ${OUTPUT_DIR}/*.histo.root
echo "Output total histograms in ${OUTPUT_DIR}/total_histos.root"

export SCRIPT=no
