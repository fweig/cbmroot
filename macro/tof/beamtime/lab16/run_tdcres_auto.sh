
rm ./TofTdcCalibHistos_auto_calib_tdcref.root
root -l -q "tdcref_monitoring_auto.C(100000, kTRUE)"
root -l "tdcref_monitoring_auto.C(21000)"

