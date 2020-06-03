#!/bin/sh
ComputingPlace=1

OnBatch=0
if [ $ComputingPlace == 2 ];then OnBatch=1; fi

if [ $OnBatch == 1 ];then 
    cbmroot_config_path=/hera/cbm/users/tariq/cbmroot/buildcbm/config.sh
    source ${cbmroot_config_path}
fi

nEvs=100
RX=20; RY=10 ; TrY=0; TrZ=0
###########################################
extendedmir=1; GeoCase=2; DimCase=2
OldCode=1
###########################################
#DimCase --> 0: enlarged rich (in x,y and z) and PMT, 1: Default RICH dim, 2: enlarged PMT and RICH in x and y, 3: optimised PMT dims (_PMTW1002_H604.)
EnlargedPMTWidth=60; EnlargedPMTHight=36

MinTheta=250; MaxTheta=2500
#2500/100 = 25 (in cpp)
MinPhi=90; MaxPhi=180


PtNotP=1
MomMin=0; MomMax=400
#400/100 =4 gev in cpp
rotmir=-10;

extratext=_MirrUpdated.
echo " eeeeeeeeeeeeeeeeeeeeeeeet  $extratext  eeeeeeeeeeeeeeeeeeeeeeeeet"

DefaultDims=0;DefaultDimsLargePMT=0 #for old code
if [ $OldCode == 1 ];then extendedmir=0; extratext=.; fi

if [ $OldCode == 1 -a $DimCase == 3 ];then 
    echo " Want to work with old code but setting DimCase to 3 !!"
    echo " DimCase 3 goes only with new code!!"
    echo " Setting DimCase to 2!!"
    DimCase=2
    extendedmir=0
fi
if [ $DimCase -lt 3 -a $OldCode == 0 ];then 
    echo " Want to work with older geometries (DimCase=$DimCase) but with new code !!"
    echo " Setting DimCase to 3 --> prepare geo for new code!!"
    DimCase=3
fi
if [ $DimCase == 3 ];then  extendedmir=1; fi
#########################
UpperTransY=0; UpperTransZ=0;
if [ $DimCase == 0 ];then UpperTransY=50; UpperTransZ=50; DefaultDims=0; DefaultDimsLargePMT=0; fi
if [ $DimCase == 1 ];then UpperTransY=31; UpperTransZ=23; DefaultDims=1; DefaultDimsLargePMT=0; fi
if [ $DimCase == 2 ];then UpperTransY=23; UpperTransZ=13; DefaultDims=1; DefaultDimsLargePMT=1; fi
if [ $DimCase == 3 ];then UpperTransY=23; UpperTransZ=13; DefaultDims=1; DefaultDimsLargePMT=1; fi 


#for ((extt=0; extt<=2; extt++)); do
#	if [ $extt == 0 ];then extratext=_MirrUpdated.; fi
#	if [ $extt == 1 ];then extratext=_MirrUpdated_Electrons.; fi
#	if [ $extt == 2 ];then extratext=_MirrUpdated_Positrons.; fi

#for ((phi=0; phi<=1; phi++)); do
#	if [ $phi == 0 ];then MinPhi=90; MaxPhi=180; fi
#	if [ $phi == 1 ];then MinPhi=0; MaxPhi=360; fi

for ((Rm=1; Rm<=1; Rm++)); do
    if [ $Rm == 0 ];then rotmir=1; fi
    if [ $Rm == 1 ];then rotmir=-10; fi
    
    for ((Th=0; Th<=0; Th++)); do
	if [ $Th == 0 ];then MaxTheta=2500; fi
	if [ $Th == 1 ];then MaxTheta=3500; fi
	
	#   for ((RX=0; RX<=$UpperRotX; RX++)); do
	#	for ((RY=0; RY<=$UpperRotY; RY++)); do
	#	    echo $RX $RY
	
	for ((trY=-2; trY<=-2; trY++)); do
	    TrY=$(( 20 * ${trY}))
	    for ((trZ=4; trZ<=4; trZ++)); do
		TrZ=$(( 20 * ${trZ}))
		echo $TrY   $TrZ
		
		if [ $OldCode == 1 ];then 
		    echo " working with old code CreateGeo/CreateGDMLfile_April2015_ModAug2015.C()"
		    root -b -q "CreateGeo/CreateGDMLfile_April2015_ModAug2015.C($TrY, $TrZ, $rotmir, $RX, $RY, $DefaultDims, $DefaultDimsLargePMT)"
		else
		    echo " working with new code CreateGeo/CreateGDMLfile_ExtMirrOct2015.C()"
		    root -b -q "CreateGeo/CreateGDMLfile_ExtMirrOct2015.C($TrY, $TrZ, $rotmir, $RX, $RY, $EnlargedPMTWidth, $EnlargedPMTHight)"
		fi
		
 		
		if [ $OnBatch == 1 ];then 
		    qsub -l mem=5G,ct=5:00:00 ./RunSimOnBatch.sh -extt $extratext -batch $OnBatch -nev $nEvs -rx $RX -ry $RY -dy $TrY -dz $TrZ -mint $MinTheta -maxt $MaxTheta -minp $MinPhi -maxp $MaxPhi -geo $GeoCase -dim $DimCase -pmtw $EnlargedPMTWidth -pmth $EnlargedPMTHight -ptp $PtNotP -minm $MomMin -maxm $MomMax -mir $rotmir -extm $extendedmir -oldc $OldCode -ddim $DefaultDims -pmtd $DefaultDimsLargePMT
		else
		    
		    . ./RunSimOnBatch.sh -extt $extratext -batch $OnBatch -nev $nEvs -rx $RX -ry $RY -dy $TrY -dz $TrZ -mint $MinTheta -maxt $MaxTheta -minp $MinPhi -maxp $MaxPhi -geo $GeoCase -dim $DimCase -pmtw $EnlargedPMTWidth -pmth $EnlargedPMTHight -ptp $PtNotP -minm $MomMin -maxm $MomMax -mir $rotmir -extm $extendedmir -oldc $OldCode -ddim $DefaultDims -pmtd $DefaultDimsLargePMT
		fi
	    done
	done
    done
done
#	done
#    done
#done
