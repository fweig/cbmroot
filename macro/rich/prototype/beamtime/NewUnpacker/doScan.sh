#!/bin/bash

cd ~/cbmroot_may2015_devel/build/
make
make_return_code=$?
cd -

# Break execution if CbmRoot build was not succesfull
if [[ $make_return_code != 0 ]]; then exit; fi

echo "Clear log in log_dir"
rm -f log_dir/*

for TDC in {0..3..1}
do
	for i in {1..31..2}
	do
		echo "TDC 0x001${TDC}, ch ${i}"
		rm -f "calibration_wu.root"
		rm -f output/*
		root -l -b -q "run_analysis_wu_2_ip.C(kTRUE, ${TDC}, ${i})" >> log_dir/std.out 2>> log_dir/std.err
		rm -f output/*
		root -l -b -q "run_analysis_wu_2_ip.C(kFALSE, ${TDC}, ${i})" >> log_dir/std.out 2>> log_dir/std.err
		root -l -b -q "doDraw.C(${TDC}, ${i})" >> log_dir/table.txt
	done
done
