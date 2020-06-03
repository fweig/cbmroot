#!/bin/bash

cd ~/cbmroot_may2015_devel/build/
make Rich
make_return_code=$?
cd -

# Break execution if CbmRoot build was not succesfull
if [[ $make_return_code != 0 ]]; then exit; fi

echo "Clear log in log_dir"
rm log_dir/*

for i in {1..31..2}
do
	rm output/*
	root -l -b -q "run_analysis_wu.C(kTRUE, ${i})" >> log_dir/std.out 2>> log_dir/std.err
	rm output/*
	root -l -b -q "run_analysis_wu.C(kFALSE, ${i})" >> log_dir/std.out 2>> log_dir/std.err
	root -l -b -q "doDraw.C(${i})"
done

#rm output/*
#root -l -b -q "run_analysis_wu.C(kTRUE)"
#rm output/*
#root -l -b -q "run_analysis_wu.C(kFALSE)"
#root -l output/Wuppertal_analysis.histo.root
