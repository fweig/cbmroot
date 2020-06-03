#!/bin/bash
# Shell script for unpacking ...

#cRun=CbmTofPiHd_09Aug1533
#cRun=CbmTofPiHd_10Aug1728
#cRun=CbmTofPiHd_11Aug1224
#cRun=CbmTofPiHd_13Aug0855
#cRun=CbmTofPiHd_15Aug0818
#cRun=CbmTofPiHd_16Aug1016
#cRun=CbmTofPiHd_17Aug1724
#cRun=CbmTofPiHd_22Aug1616
cRun=CbmTofPiHd_29Aug1401
#cRun=CbmTofPiHd_07Sep1333
#cRun=CbmTofPiHd_12Sep1601
# -------------------------------- Cleaning Up --------------------------------

# Removing files: Fresh unpacking
if((0)); then
rm -rf $cRun/
rm all_*.par core_dump_* *.pdf
rm tofMbsUnp.hst.root
rm tofMbsCal.hst.root
rm tofMbsMap.hst.root
rm tofTrbUnp.hst.root
rm TofTdcCalibHistos_${cRun}.root
rm tofFindTracks.hst.root
rm unpack_${cRun}.buffer.root
rm unpack_${cRun}.calib.root
rm unpack_${cRun}.out.root 
rm unpack_${cRun}.params.root
fi

# ------------------------ Using New Calibration Class ------------------------
if((1)); then

# Step 1: Create Calib
if((1)); then
echo "create_calib.C is called..."
root -l<<EOF
Int_t nEvents  =100000000
char* cFileId  ="$cRun"
FairRunOnline* run = new FairRunOnline();
.x create_calib.C(nEvents, cFileId);
EOF
fi

# Step 2: Apply Calib
if((1)); then
echo "apply_calib.C is called..."
root -l<<EOF
Int_t nEvents  =100000000
char* cFileId  ="$cRun"
FairRunOnline* run = new FairRunOnline();
.x apply_calib.C(nEvents, cFileId);
EOF
fi

fi
