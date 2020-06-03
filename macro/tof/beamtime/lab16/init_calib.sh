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

#cRun=$1
#cRun='CbmTofPiHd_09Aug1533'
#cRun='CbmTofPiHd_10Aug1728'
#cRun='CbmTofPiHd_11Aug1224'
#cRun='CbmTofPiHd_12Aug1744'
#cRun='CbmTofPiHd_13Aug0855'
#cRun='CbmTofPiHd_15Aug0818'
#cRun='CbmTofPiHd_16Aug1016'
#cRun='CbmTofPiHd_17Aug1724'
#cRun='CbmTofPiHd_22Aug1616'
cRun='CbmTofPiHd_29Aug1401'
#cRun='CbmTofPiHd_07Sep1333'
#cRun=CbmTofPiHd_12Sep1601'

nIter=1
echo "Initialize clusterizer calibration for run $cRun, execute $nIter iterations"

#iDut=901; iMRef=900; iBRef=921;
#((iSet=$iDut*1000+$iMRef))
#((iSet=$iDut*1000000+$iMRef*1000+$iBRef))

#iCalSet=$2
#iCalSet=iDutiMRefiBRef
#iCalSet=901900921
#iCalSet=901900910
iCalSet=601600921

((iTmp  = $iCalSet ))
((iBRef = $iTmp % 1000))
((iTmp  = $iTmp - $iBRef))
((iSet  = $iTmp / 1000))
((iMRef = $iTmp % 1000000))
((iMRef = $iMRef / 1000))
((iTmp  = $iTmp - $iMRef))
((iDut  = $iTmp / 1000000))

# ----------------------- Clean up before fresh Re-run ------------------------
if((0)); then
rm all_*.par core_dump_* *.pdf
rm -rf $cRun/
rm ${cRun}_set${iCalSet}_*_0tofTestBeamClust.hst.root
rm ${cRun}_set${iCalSet}_*_1tofTestBeamClust.hst.root
fi
# ------------------------------- End Clean up --------------------------------
echo "Calib setup is ${iCalSet}, iSet=$iSet, iDut=$iDut, iMRef=$iMRef, iBRef=$iBRef"

#return
#iSet=0
#lastOpt=''

nEvi=100000
nEv4=100000
nEvii=100000
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

# ------------------ Starting 1st for Loop ------------------ #
if((1)); then
# echo "starting first loop with $nIter iterations"
# inOpt = 'nEvents, calMode, calSel, calSm, RefSel'
	
	for inOpt in ''$nEvii',0,0,0,50' ''$nEvii',03,0,0,50' ''$nEvii',13,0,0,50' ''$nEvii',23,0,0,50' ''$nEvii',23,0,0,50' ''$nEvii',33,0,0,50' ''$nEvii',33,0,0,50' ''$nEvii',33,0,0,50' ''$nEvii',43,0,0,50' ''$nEvii',43,0,0,50' ''$nEvii',43,0,0,50' ''$nEvii',53,0,0,50' ''$nEvii',53,0,0,50' ''$nEvii',53,0,0,50' ''$nEvii',63,0,0,50' ''$nEvii',63,0,0,50'

	do
	((iStepLast = ${iStep}))
	((iStep += 1))
	mkdir Init${iStep}
	cp rootlogon.C Init${iStep}
	cp .rootrc Init${iStep}
	cd Init${iStep}

	# Substring Extraction: ${string:position}
	# The if-condition will skip the 0th iteration
	if [[ ${lastOpt:+1} ]] ; then
		# echo "last round was done with $lastOpt, extract 2. and 3. word"
		i1=`expr index $inOpt , `
		i2=($i1+3)
		#echo `expr index $inOpt , ` = $i1
		cMode=${inOpt:$i1:2}
		cSel=${inOpt:$i2:1}
		echo "Next iteration: cMode=$cMode, cSel=$cSel"
		if [[ ${cSel} = "-" ]];then 
			cSel=${inOpt:$i2:2}
			echo "cSel=$cSel"
			cSel="0"
		fi
		
		# Copy calibration file from last "Init${iStepLast}" to current "Init${iStep}"
		cp -v ../Init${iStepLast}/tofTestBeamClust_${cRun}_set${iCalSet}.hst.root ${cRun}_set${iCalSet}_${cMode}_${cSel}tofTestBeamClust.hst.root
	fi 

	# generate new calibration file
	# ana_digi_ini(nEvents,calMode,calSel,calSm,RefSel,cFileId,iSet,iBRef)
	echo "executing ana_digi_ini.C with iStepLast = ${iStepLast} and iStep = $iStep"
	root -b -q '../../ana_digi_ini.C('$inOpt',"'${cRun}'",'${iCalSet}','${iBRef}') '
	lastOpt=$inOpt
	#./screenshot.sh
	cp *pdf ../
	rm all_*
	cd ..
	done # end for-loop

	echo "end of 1st for loop with iStepLast = ${iStepLast} and iStep = $iStep"	
	cp -v  ./Init${iStep}/${cRun}_set${iCalSet}_${cMode}_${cSel}tofTestBeamClust.hst.root ${cRun}_set${iCalSet}_03_0tofTestBeamClust.hst.root

fi # end if((1))

# ------------------ Starting 2nd for Loop ------------------ #
if((1)); then
# echo "execute main loop at $nIter iteration"
# inOpt = 'nEvents, calMode, calSel, calSm, RefSel'

	for inOpt in ''$nEvi',0,0,0,50' ''$nEvi',03,0,0,50' ''$nEvi',03,1,'${iMRef}',0' ''$nEvi',03,0,'${iDut}',0' ''$nEvi',03,1,'${iMRef}',0' ''$nEvi',03,0,'${iDut}',0' ''$nEvi',13,0,0,50' ''$nEvi',13,1,'${iMRef}',0'  ''$nEvi',13,0,'${iDut}',0' ''$nEvi',13,0,0,50' ''$nEvi',13,1,'${iMRef}',0'  ''$nEvi',13,0,'${iDut}',0' ''$nEvi',23,0,0,50' ''$nEvi',23,1,'${iMRef}',0'  ''$nEvi',23,0,'${iDut}',0' ''$nEvi',23,0,0,50' ''$nEvi',23,1,'${iMRef}',0'  ''$nEvi',23,0,'${iDut}',0' ''$nEvi',33,0,0,50' ''$nEvi',33,1,'${iMRef}',0'  ''$nEvi',33,0,'${iDut}',0' ''$nEvi',33,0,0,50' ''$nEvi',33,1,'${iMRef}',0' ''$nEvi',33,0,'${iDut}',0' ''$nEvi',43,0,0,50' ''$nEvi',43,1,'${iMRef}',0' ''$nEvi',43,0,'${iDut}',0' ''$nEvi',43,0,0,50' ''$nEvi',43,1,'${iMRef}',0' ''$nEvi',43,0,'${iDut}',0' ''$nEvi',43,1,'${iMRef}',0' ''$nEvi',43,0,0,50' ''$nEvi',43,1,'${iMRef}',0' ''$nEvi',43,0,'${iDut}',0' ''$nEvi',53,0,0,50' ''$nEvi',53,1,'${iMRef}',0' ''$nEvi',53,0,'${iDut}',0' ''$nEvi',53,0,0,50' ''$nEvi',53,1,'${iMRef}',0' ''$nEvi',53,0,'${iDut}',0' ''$nEvi',52,1,0,50' ''$nEvi',52,0,0,50' ''$nEv4',14,0,-'${iDut}',0' ''$nEv4',14,1,-'${iMRef}',0' ''$nEvi',53,0,'${iDut}',0' ''$nEvi',53,1,'${iMRef}',0' ''$nEvi',63,0,'${iDut}',0' ''$nEvi',63,1,'${iMRef}',0' ''$nEvi',73,0,'${iDut}',0' ''$nEvi',73,1,'${iMRef}',0' ''$nEvi',73,0,'${iDut}',0' ''$nEv4',24,0,-'${iDut}',0' ''$nEvi',73,1,'${iMRef}',0' ''$nEv4',24,1,-'${iMRef}',0' ''$nEvi',83,1,'${iMRef}',0' ''$nEvi',83,0,'${iDut}',0' ''$nEv4',34,0,-'${iDut}',0' ''$nEvi',83,1,'${iMRef}',0' ''$nEv4',34,1,-'${iMRef}',0' ''$nEvi',93,0,'${iDut}',0' ''$nEvi',93,1,'${iMRef}',0' ''$nEvi',93,1,0,50' ''$nEvi',93,0,0,50' ''$nEvi',93,0,'${iDut}',0' ''$nEvi',93,1,'${iMRef}',0' ''$nEvi',93,1,0,50' ''$nEvi',93,0,0,50' ''$nEvi',93,0,'${iDut}',0' ''$nEv4',64,0,-'${iDut}',0' ''$nEvi',93,1,'${iMRef}',0' ''$nEv4',64,1,-'${iMRef}',0' ''$nEvi',93,1,'${iMRef}',0'  
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
		
		echo "Next iteration: cMode=$cMode, cSel=$cSel" in directory `pwd`
		if [[ ${cSel} = "-" ]];then 
			cSel=${inOpt:$i2:2}
			echo cSel=$cSel 
			cSel="0"
		fi
		
		#copy calibration file 
		if (($iStep > $iRestart)) ; then 
			cp -v ../Init${iStepLast}/tofTestBeamClust_${cRun}_set${iCalSet}.hst.root ${cRun}_set${iCalSet}_${cMode}_${cSel}tofTestBeamClust.hst.root
		fi
		
	fi #end-if(${lastOpt:+1})

	lastOpt=$inOpt

   if (($iStep > $iRestart)) ; then 
		# generate new calibration file
		# void ana_digi_cal(nEvents,calMode,calSel,calSm,RefSel,cFileId,iSet,iBRef)
		root -b -q '../../ana_digi_cal.C('$inOpt',"'${cRun}'",'${iCalSet}','${iBRef}') '

		cp -v tofTestBeamClust_${cRun}_set${iCalSet}.hst.root ../${cRun}_set${iCalSet}_${cMode}_${cSel}tofTestBeamClust.hst.root
		cp *pdf ../
		rm all_*
		#./screenshot.sh
		cd .. 
		rm ../${cRun}_set${iCalSet}_${cMode}_${cSel}tofTestBeamClust.hst.root
		ln -s ./${cRun}/${cRun}_set${iCalSet}_${cMode}_${cSel}tofTestBeamClust.hst.root ../${cRun}_set${iCalSet}_${cMode}_${cSel}tofTestBeamClust.hst.root
   	echo Init step $iStep with mode ${cMode}, option $inOpt  finished
	
	else 
      cd ..
      echo Init step $iStep with mode ${cMode}, option $inOpt  skipped
	fi #end-if($iStep > $iRestart)
	
	done # end for-loop
fi # end if((1))

(( nIter -= 1))
done # end-while(nIter)
