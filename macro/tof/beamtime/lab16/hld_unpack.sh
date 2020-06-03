#!/bin/bash
# Shell script for unpacking ...

#cRun=CbmTofPiHd_01Mar1558
#cRun=CbmTofPiHd_21Mar1734
#cRun=CbmTofPiHd_30Mar1435
#cRun=CbmTofPiHd_01Apr1647
#cRun=CbmTofPiHd_08Apr1811
#cRun=CbmTofPiHd_14Apr1255
#cRun=CbmTofPiHd_All
#cRun=CbmTofPiHd_09Aug1533
#cRun=CbmTofPiHd_10Aug1728
#cRun=CbmTofPiHd_11Aug1224
#cRun=CbmTofPiHd_13Aug0855
cRun=CbmTofPiHd_15Aug0818


# Removing files: Fresh unpacking
if((0)); then
rm all_*.par core_dump_* *.pdf
rm tofMbsUnp.hst.root
rm tofMbsCal.hst.root
rm tofMbsMap.hst.root
rm tofTrbUnp.hst.root
rm TofTdcCalibHistos_$cRun.root
rm tofFindTracks.hst.root
rm unpack_$cRun.buffer.root
rm unpack_$cRun.calib.root
rm unpack_$cRun.out.root 
rm unpack_$cRun.params.root
fi

# Removing files: Without unpacking
if((0)); then
rm -r $cRun
rm *.pdf
rm tofTestBeamClust_$cRun_set*.hst.root
rm $cRun_set*_0tofTestBeamClust.hst.root
rm $cRun_set*_1tofTestBeamClust.hst.root
rm $cRun_*tofAnaTestBeam.hst.root
rm digi_$cRun.out.root
rm digi_$cRun_*.out.root
fi

# -------------------------- Old Calibration Class ----------------------------
echo "setup_unpack.C is called..."

if((1)); then
root -l<<EOF
Int_t nEvents  =10000000
Int_t calMode  =1
char* cFileId  ="$cRun"
Int_t iSet     =0
FairRunOnline* run = new FairRunOnline();
.x setup_unpack.C(calMode, cFileId, iSet);
run->Run(-1);
run->Finish();
EOF
fi





# --------------------------  Analysis Commands  ------------------------------
# calMode = 1																  						#
# cFileId = "CernSps05Mar0041"												  				#
# iSet 	 = 0 (whole), 1 (upper), 2 (lower). iSet=0 is recommended. 		  		#
# nEvent  = passed to macro, nEvents=-1 takes all events available.			  	#
# run->Run(0,nEvents) OR run->Run(-1)
#																				
# --------------------------  Files Genenrated  -------------------------------
#																			  							#
# All Params (FairRuntimeDb Class)	   >> all_27868.par						  		#
# Unpacker (FairRunOnline Class)	   	>> unpack_$cRun.out.root			  		#
# Unpack Params (FairRuntimeDb Class)  >> unpack_$cRun.params.root			  	#
#																			  							#
# Monitoring (TMbsUnpTofMonitor Class) >> tofMbsUnp.hst.root				  		#
# Calibration (TMbsCalibTof Class) 	   >> tofMbsCal.hst.root				  		#
# Mapping (TMbsMappingTof Class) 	   >> tofMbsMap.hst.root 				  		#
# Calibration (TMbsCalibTdcTof Class)  >> TofTdcCalibHistos_$cRun.root 		  	#
# Display (CbmTofOnlineDisplay Class)  >> ???							  	  			#
