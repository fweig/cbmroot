#!/bin/bash

root -l -b -q "FilterMultiPeak.C(\"sum_rings_WLS_off_G1_corr.root\", \"pictures_rings_WLS_off_corr\")" > filtering_rings_WLS_off_corr_.txt
grep "GOOD" filtering_rings_WLS_off_corr_.txt > filtering_rings_WLS_off_corr.txt
root -l -b -q "buildDeltaTable.C(\"filtering_rings_WLS_off_corr.txt\", \"alphas_rings_WLS_off_corr.root\", \"pictures_rings_WLS_off_corr\")" > table_rings_WLS_off_corr.txt

exit

root -l -b -q "FilterMultiPeak.C(\"sum_laser_WLS_off_G1.root\", \"pictures_laser_WLS_off\")" > filtering_laser_WLS_off_.txt
root -l -b -q "FilterMultiPeak.C(\"sum_laser_WLS_on_G1.root\", \"pictures_laser_WLS_on\")" > filtering_laser_WLS_on_.txt

root -l -b -q "FilterMultiPeak.C(\"sum_laser_WLS_off_G1_corr.root\", \"pictures_laser_WLS_off_corr\")" > filtering_laser_WLS_off_corr_.txt
root -l -b -q "FilterMultiPeak.C(\"sum_laser_WLS_on_G1_corr.root\", \"pictures_laser_WLS_on_corr\")" > filtering_laser_WLS_on_corr_.txt

root -l -b -q "FilterMultiPeak.C(\"sum_rings_WLS_off_G1_corr.root\", \"pictures_rings_WLS_off_corr\")" > filtering_rings_WLS_off_corr_.txt
root -l -b -q "FilterMultiPeak.C(\"sum_rings_WLS_on_G1_corr.root\", \"pictures_rings_WLS_on_corr\")" > filtering_rings_WLS_on_corr_.txt

grep "GOOD" filtering_laser_WLS_off_.txt > filtering_laser_WLS_off.txt
grep "GOOD" filtering_laser_WLS_on_.txt > filtering_laser_WLS_on.txt
grep "GOOD" filtering_laser_WLS_off_corr_.txt > filtering_laser_WLS_off_corr.txt
grep "GOOD" filtering_laser_WLS_on_corr_.txt > filtering_laser_WLS_on_corr.txt
grep "GOOD" filtering_rings_WLS_off_corr_.txt > filtering_rings_WLS_off_corr.txt
grep "GOOD" filtering_rings_WLS_on_corr_.txt > filtering_rings_WLS_on_corr.txt
