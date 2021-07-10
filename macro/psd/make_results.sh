#!/bin/bash

while read F  ; do
	echo "processing ${F}/EventTree.root"
	mkdir -p ${F}/results
	root -l -b -q "compare_to_sim.cpp(\"${F}/EventTree.root\", \"${F}/results/Results\", \"EdepFPGA\", -1, 0, 0)"
	root -l -b -q "compare_to_sim.cpp(\"${F}/EventTree.root\", \"${F}/results/Results\", \"EdepFPGA\", -1, 0, 1)"
	root -l -b -q "compare_to_sim.cpp(\"${F}/EventTree.root\", \"${F}/results/Results\", \"EdepFPGA\", -1, 0, 2)"

	root -l -b -q "compare_to_sim.cpp(\"${F}/EventTree.root\", \"${F}/results/Results\", \"EdepWfm\", -1, 0, 0)"
	root -l -b -q "compare_to_sim.cpp(\"${F}/EventTree.root\", \"${F}/results/Results\", \"EdepWfm\", -1, 0, 1)"
	root -l -b -q "compare_to_sim.cpp(\"${F}/EventTree.root\", \"${F}/results/Results\", \"EdepWfm\", -1, 0, 2)"

	root -l -b -q "compare_to_sim.cpp(\"${F}/EventTree.root\", \"${F}/results/Results\", \"FitEdep\", -1, 0, 0)"
	root -l -b -q "compare_to_sim.cpp(\"${F}/EventTree.root\", \"${F}/results/Results\", \"FitEdep\", -1, 0, 1)"
	root -l -b -q "compare_to_sim.cpp(\"${F}/EventTree.root\", \"${F}/results/Results\", \"FitEdep\", -1, 0, 2)"
done <filelist.txt


