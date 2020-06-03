#!/bin/bash

root -l -b -q "buildDeltaTable.C(\"filtering_laser_WLS_off.txt\", \"alphas_laser_WLS_off.root\", \"pictures_laser_WLS_off\")" > table_laser_WLS_off.txt
root -l -b -q "buildDeltaTable.C(\"filtering_laser_WLS_on.txt\", \"alphas_laser_WLS_on.root\", \"pictures_laser_WLS_on\")" > table_laser_WLS_on.txt

root -l -b -q "buildDeltaTable.C(\"filtering_laser_WLS_off_corr.txt\", \"alphas_laser_WLS_off_corr.root\", \"pictures_laser_WLS_off_corr\")" > table_laser_WLS_off_corr.txt
root -l -b -q "buildDeltaTable.C(\"filtering_laser_WLS_on_corr.txt\", \"alphas_laser_WLS_on_corr.root\", \"pictures_laser_WLS_on_corr\")" > table_laser_WLS_on_corr.txt

root -l -b -q "buildDeltaTable.C(\"filtering_rings_WLS_off_corr.txt\", \"alphas_rings_WLS_off_corr.root\", \"pictures_rings_WLS_off_corr\")" > table_rings_WLS_off_corr.txt
root -l -b -q "buildDeltaTable.C(\"filtering_rings_WLS_on_corr.txt\", \"alphas_rings_WLS_on_corr.root\", \"pictures_rings_WLS_on_corr\")" > table_rings_WLS_on_corr.txt
