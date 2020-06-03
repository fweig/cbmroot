#!/bin/bash
# Shell script to initialize clusterizer calibrations...

# **************************************************************************** #
# Macro definitions are;
# void ana_digi_ini(nEvents,calMode,calSel,calSm,RefSel,cFileId,iSet,iBRef)
# void ana_digi_cal(nEvents,calMode,calSel,calSm,RefSel,cFileId,iSet,iBRef)
#					--------------------------------
# nEvent : Number of events used for particular iteration
# calMode: is a 2-digit number, 1st digit shows cuts applied and 2nd digit shows 
#          type of correction applied.
#		   1 : Walk Correction
#		   2 : Position alignment and gain correction at detector level
#		   3 : Position alignment and gain correction at strip level
# 		   4 : Velocity corrections
# calSel : is event selector for calibration
# 		   0 : Use all triggers, consider only events when a digi is present
#			   in the current iDut, iMRef and iBRef
# 		   1 : Use all triggers, consider only events when a digi is present 
#              in the current iMRef and iBRef
# 		   -1: Take corrections from untriggered distributions
# 		   -2: Calculate corrections from deviations within clusters only
# 		   -3: Take corrections from deviations to matched trigger digis
# calSm  : select detector for calibration update
# refSel : reference trigger for offset calculation 
# cRun   : cFileId
# iSet	 : counter setup selector ???
# iBRef  : Beam Reference Counter
#					--------------------------------
#
# nEvents = 1000000, for "ana_digi_ini.C" but for "ana_digi_cal.C" nEvents can
# used differently in loops.
#					--------------------------------
# Loop on ana_digi_ini.C is on different sets of 'calMode'
# inOpt = 'nEvents,calMode,calSel,calSm, RefSel' 
# inOpt = ''$nEvi',0      ,0     ,0    , 50'
#					--------------------------------
# Loop on ana_digi_cal.C is on two consective sets i.e. calSel & calSm varying
#
# inOpt = 'nEvents,calMode,calSel,calSm      , RefSel'
# inOpt = ''$nEvi',93     ,0     ,'${iDut}'  , 0' 
# inOpt = ''$nEvi',93     ,1     ,'${iMRef}' , 0'
#
# **************************************************************************** #

#cRun='Get4_0807'
#cRun='USTCDS_Get4_2607'
#cRun='BUCDS_Get4_2807'
#cRun='BUCDS_Get4_2907_5500V'
#cRun='BUCSS_Get4_0308_8500V'
#cRun='BUCSS_Get4_0508_8500V'
#cRun='Pulser_0808'
#cRun='P5PlaCut_1908'
#cRun='P5PlaCut_2008'
#cRun='P5PlaCut_3008'
#cRun='P5Pla_3008'
#cRun='THUDS_1909'
#cRun='THUDS_unsorted_1909'
#cRun='USTC_dsm_1110_0x190_0x160'
cRun='USTC_dsu_1110_0x160_6300V'

# Clean up before fresh re-run of init_calib.sh
if((0)); then
	rm -r $cRun
	rm $cRun_*tofTestBeamClust.hst.root
fi

nIter=1
echo "Initialize clusterizer calibration for run $cRun, execute $nIter iterations"
#iDut=3; iMRef=9; iBRef=7;
#iDut=9; iMRef=4; iBRef=3;
#iDut=9; iMRef=7; iBRef=3;
#iDut=9; iMRef=3; iBRef=5; 
#iDut=3; iMRef=9; iBRef=5;
#iDut=9; iMRef=9; iBRef=5;
#iDut=3; iMRef=4; iBRef=5;
#iDut=921; iMRef=920; iBRef=300;
#iDut=400; iMRef=300; iBRef=500;
#iDut=921; iMRef=300; iBRef=500;
#iDut=921; iMRef=920; iBRef=500;

#iDut=6; iMRef=1; iBRef=8; 
#iDut=6; iMRef=1; iBRef=5; 
#iDut=1; iMRef=6; iBRef=5; 

#((iSet=$iDut*1000+$iMRef))
#((iSet=$iDut*1000000+$iMRef*1000+$iBRef))

#iCalSet=$2
#iCalSet=iDutiMRefiBRef
#iCalSet=910910910
iCalSet=911911911
#iCalSet=921921921
#iCalSet=400400400
#iCalSet=400210200
#iCalSet=601601601
#iCalSet=600600600

((iTmp  = $iCalSet ))
((iBRef = $iTmp % 1000))
((iTmp  = $iTmp - $iBRef))
((iSet  = $iTmp / 1000))
((iMRef = $iTmp % 1000000))
((iMRef = $iMRef / 1000))
((iTmp  = $iTmp - $iMRef))
((iDut  = $iTmp / 1000000))

echo "Calib setup is ${iCalSet}, iSet=$iSet, iDut=$iDut, iMRef=$iMRef, iBRef=$iBRef"

#return
#iSet=0
#lastOpt=''

nEvi=100000
#nEvi=1000
mkdir ${cRun}
cp rootlogon.C ${cRun}
cp .rootrc ${cRun}
cd ${cRun}

# Global variables, for for-loops. Runs from 1-15 and 16-79 in two for-loops
iRestart=0
iStep=0
iStepLast=0

# ************************** Starting while Loop ***************************** #
while [[ $nIter > 0 ]]; do 
echo "start from scratch with $nIter iterations"

# ------------------ Starting 2nd for Loop ------------------ #
if((1)); then
# echo "execute main loop at $nIter iteration"
# inOpt = 'nEvents, calMode, calSel, calSm, RefSel'
for inOpt in ''$nEvi',05,0,0,50' ''$nEvi',05,1,'${iMRef}',0' ''$nEvi',15,0,0,50' ''$nEvi',13,-2,-2,0' ''$nEvi',15,0,0,50' ''$nEvi',25,0,0,50' ''$nEvi',23,-2,-2,0' ''$nEvi',25,0,0,50' ''$nEvi',35,0,0,50' ''$nEvi',33,-2,-2,0' ''$nEvi',35,0,0,50' ''$nEvi',45,0,0,50' ''$nEvi',43,-2,-2,0' ''$nEvi',45,0,0,50' ''$nEvi',55,0,0,50' ''$nEvi',53,-2,-2,0' ''$nEvi',55,0,0,50' ''$nEvi',65,0,0,50' ''$nEvi',63,-2,-2,0' ''$nEvi',65,0,0,50' ''$nEvi',75,0,0,50' ''$nEvi',73,-2,-2,0' ''$nEvi',75,0,0,50' ''$nEvi',85,0,0,50' ''$nEvi',83,-2,-2,0' ''$nEvi',85,0,0,50' ''$nEvi',95,0,0,50' ''$nEvi',93,-2,-2,0' ''$nEvi',95,0,0,50' ''$nEvi'00,61,-1,-2,0' ''$nEvi',93,-2,-2,0' ''$nEvi',95,0,0,50' ''$nEvi'00,61,-1,-2,0' ''$nEvi',93,-2,-2,0' ''$nEvi'00,61,-1,-2,0' ''$nEvi',93,-2,-2,0' ''$nEvi'00,61,-1,-2,0' ''$nEvi',93,-2,-2,0' ''$nEvi'00,61,-1,-2,0' ''$nEvi',93,-2,-2,0' ''$nEvi'00,61,-1,-2,0' ''$nEvi',93,-2,-2,0' 

	do
	((iStepLast = ${iStep}))
	((iStep += 1))
	mkdir Init${iStep}
	cp rootlogon.C Init${iStep}
	cp .rootrc Init${iStep}
	cd Init${iStep}
	if [[ ${lastOpt:+1} ]] ; then
		# echo "last round was done with $lastOpt, extract 2. and 3. word"
		i1=`expr index $inOpt , `
		i2=($i1+3)
		#echo `expr index $inOpt , ` = $i1
		cMode=${inOpt:$i1:2}
		cSel=${inOpt:$i2:1}
		echo "Next iteration:  iStep= $iStep cMode=$cMode, cSel=$cSel"
		echo "Next iteration:  iStep=$iStep, cMode=$cMode, cSel=$cSel, inOpt=$inOpt">mode.txt
		if [[ ${cSel} = "-" ]];then 
			cSel=${inOpt:$i2:2}
			echo cSel=$cSel 
			cSel="0"
		fi
		#copy calibration file 
		if (($iStep > $iRestart)) ; then 
		cp -v ../Init${iStepLast}/tofTestBeamClust_${cRun}_set${iSet}.hst.root ${cRun}_set${iSet}_${cMode}_${cSel}tofTestBeamClust.hst.root
		fi
	fi 	
if (($iStep > $iRestart)) ; then 
	# generate new calibration file
	root -b -q '../../ana_digi_cal_self.C('$inOpt',"'${cRun}'",'${iSet}','${iBRef}') '
	lastOpt=$inOpt
	cp -v tofTestBeamClust_${cRun}_set${iSet}.hst.root ../${cRun}_set${iSet}_${cMode}_${cSel}tofTestBeamClust.hst.root
	cp *pdf ../
	#./screenshot.sh
	cd .. 
	rm ../${cRun}_set${iSet}_${cMode}_${cSel}tofTestBeamClust.hst.root
	ln -s ./${cRun}/${cRun}_set${iSet}_${cMode}_${cSel}tofTestBeamClust.hst.root ../${cRun}_set${iSet}_${cMode}_${cSel}tofTestBeamClust.hst.root
	echo Init step $iStep with mode ${cMode}, option $inOpt  finished
	else 
        cd ..
        echo Init step $iStep with mode ${cMode}, option $inOpt  skipped
	fi
	done # end for-loop
fi # end if((1))

#	root -b -q  '../../ana_digi_cal_self.C('$nEvi',93,1,'${iMRef}',0,"'${cRun}'",'${iSet}','${iBRef}',1)' 

(( nIter -= 1))
done # end-while(nIter)
