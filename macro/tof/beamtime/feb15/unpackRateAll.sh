# Copyright (C) 2015 Facility for Antiproton and Ion Research in Europe, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Pierre-Alain Loizeau

rm all.*.par out/*.out.root out/*.params.root  out/*.hst.root

root -b -q 'rates_unpacking.C("/home/nh7/trbnas/cern-feb15/ratesMbs/", "rates_2015_02_27_2311")'
root -b -q 'rates_unpacking.C("/home/nh7/trbnas/cern-feb15/ratesMbs/", "rates_2015_02_28_0928")'
root -b -q 'rates_unpacking.C("/home/nh7/trbnas/cern-feb15/ratesMbs/", "2015_02_28_21_34")'
root -b -q 'rates_unpacking.C("/home/nh7/trbnas/cern-feb15/ratesMbs/", "2015_03_01_09_47")'
root -b -q 'rates_unpacking.C("/home/nh7/trbnas/cern-feb15/ratesMbs/", "2015_03_01_13_40")'
root -b -q 'rates_unpacking.C("/home/nh7/trbnas/cern-feb15/ratesMbs/", "2015_03_01_04_09")'
root -b -q 'rates_unpacking.C("/home/nh7/trbnas/cern-feb15/ratesMbs/", "2015_03_01_16_14")'
root -b -q 'rates_unpacking.C("/home/nh7/trbnas/cern-feb15/ratesMbs/", "2015_03_01_19_06")'
root -b -q 'rates_unpacking.C("/home/nh7/trbnas/cern-feb15/ratesMbs/", "2015_03_02_11_02")'
root -b -q 'rates_unpacking.C("/home/nh7/trbnas/cern-feb15/ratesMbs/", "2015_03_02_14_17")'
root -b -q 'rates_unpacking.C("/home/nh7/trbnas/cern-feb15/ratesMbs/", "2015_03_02_14_57")'
root -b -q 'rates_unpacking.C("/home/nh7/trbnas/cern-feb15/ratesMbs/", "2015_03_02_17_37_freeStr")'
root -b -q 'rates_unpacking.C("/home/nh7/trbnas/cern-feb15/ratesMbs/", "2015_03_02_23_16_freeStr_noiserun")'
root -b -q 'rates_unpacking.C("/home/nh7/trbnas/cern-feb15/ratesMbs/", "2015_03_02_23_41_freeStr")'
root -b -q 'rates_unpacking.C("/home/nh7/trbnas/cern-feb15/ratesMbs/", "2015_03_03_01_53")'
root -b -q 'rates_unpacking.C("/home/nh7/trbnas/cern-feb15/ratesMbs/", "2015_03_03_14_23")'
root -b -q 'rates_unpacking.C("/home/nh7/trbnas/cern-feb15/ratesMbs/", "2015_03_03_17_25")'
root -b -q 'rates_unpacking.C("/home/nh7/trbnas/cern-feb15/ratesMbs/", "2015_03_03_22_15")'
root -b -q 'rates_unpacking.C("/home/nh7/trbnas/cern-feb15/ratesMbs/", "2015_03_04_02_51")'
root -b -q 'rates_unpacking.C("/home/nh7/trbnas/cern-feb15/ratesMbs/", "2015_03_04_17_17")'
