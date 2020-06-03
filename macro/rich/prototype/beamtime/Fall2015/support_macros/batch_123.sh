#!/bin/bash

#./run_rings_test.sh "off" "G1"
./run_rings_test2.sh "on" "G1"

exit

./run_rings_test.sh "off" "DG1"
./run_rings_test.sh "on" "DG1"

./run_rings_test.sh "off" "GH1"
./run_rings_test.sh "on" "GH1"

./run_rings_test.sh "off" "H1"
./run_rings_test.sh "on" "H1"

./run_rings_test.sh "off" "D1"
./run_rings_test.sh "on" "D1"

./run_rings_test.sh "off" "H2"
./run_rings_test.sh "on" "H2"

echo "BATCH FINISHED!"
