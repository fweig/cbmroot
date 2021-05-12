# Copyright (C) 2015 Facility for Antiproton and Ion Research in Europe, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Pierre-Alain Loizeau


rm ./TofTdcCalibHistos_auto_calib_tdcref.root
root -l -q "tdcref_monitoring_auto.C(100000, kTRUE)"
root -l "tdcref_monitoring_auto.C(21000)"

