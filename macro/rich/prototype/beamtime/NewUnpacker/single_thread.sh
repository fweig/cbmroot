#!/bin/bash

root -l -b -q "run_analysis_thr_scan.C(kTRUE, \"$1\", \"$5\", \"$3\", \"$4\")" > $6
root -l -b -q "run_analysis_thr_scan.C(kFALSE, \"$1\", \"$2\", \"$3\", \"$4\")" > $6
