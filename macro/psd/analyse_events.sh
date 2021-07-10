#!/bin/bash

export FILELIST=$1
export FILEINDX=$2
export MAXTSA=$3
export MAXEVENT=$4

while read F  ; do
	echo "making events from ${F}/unp_mcbm_00$FILEINDX.root"
    root -l -b -q "PsdBuildEvents.cpp(\"${F}/unp_mcbm_00$FILEINDX.root\", \"${F}/EventTree$FILEINDX.root\", $MAXTSA)"

	echo "processing ${F}/EventTree$FILEINDX.root"
	mkdir -p ${F}/results
	root -l -b -q "compare_to_sim.cpp(\"${F}/EventTree$FILEINDX.root\", \"${F}/results/Results\", \"EdepFPGA\", $MAXEVENT, 0, 0)"
	root -l -b -q "compare_to_sim.cpp(\"${F}/EventTree$FILEINDX.root\", \"${F}/results/Results\", \"EdepFPGA\", $MAXEVENT, 0, 1)"
	root -l -b -q "compare_to_sim.cpp(\"${F}/EventTree$FILEINDX.root\", \"${F}/results/Results\", \"EdepFPGA\", $MAXEVENT, 0, 2)"

	root -l -b -q "compare_to_sim.cpp(\"${F}/EventTree$FILEINDX.root\", \"${F}/results/Results\", \"EdepWfm\", $MAXEVENT, 0, 0)"
	root -l -b -q "compare_to_sim.cpp(\"${F}/EventTree$FILEINDX.root\", \"${F}/results/Results\", \"EdepWfm\", $MAXEVENT, 0, 1)"
	root -l -b -q "compare_to_sim.cpp(\"${F}/EventTree$FILEINDX.root\", \"${F}/results/Results\", \"EdepWfm\", $MAXEVENT, 0, 2)"

	root -l -b -q "compare_to_sim.cpp(\"${F}/EventTree$FILEINDX.root\", \"${F}/results/Results\", \"FitEdep\", $MAXEVENT, 0, 0)"
	root -l -b -q "compare_to_sim.cpp(\"${F}/EventTree$FILEINDX.root\", \"${F}/results/Results\", \"FitEdep\", $MAXEVENT, 0, 1)"
	root -l -b -q "compare_to_sim.cpp(\"${F}/EventTree$FILEINDX.root\", \"${F}/results/Results\", \"FitEdep\", $MAXEVENT, 0, 2)"
done <$FILELIST


