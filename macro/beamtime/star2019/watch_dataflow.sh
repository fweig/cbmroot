#!/bin/bash
#script to tune the gtbx sync behaviour

source /home/cbm/starsoft/ipbuslogin.sh
source /home/cbm/starsoft/cbmroot_trunk/build/config.sh
## Go to the macro folder
cd /home/cbm/starsoft/cbmroot_trunk/macro/beamtime/star2019/

iloop=0
touch  /tmp/WatchData
touch /home/cbm/starsoft/dpbcontrols/LOG/InspectMonitor.log
while [ -e /tmp/WatchData ]; do
    echo execute loop $iloop
    rm all_*.par
    root -l -b -q MonitorTofSync.C  &> /home/cbm/starsoft/dpbcontrols/LOG/DataMonitor.log
    GDPB=`grep 'GET4 synchronization pulse missing' /home/cbm/starsoft/dpbcontrols/LOG/DataMonitor.log | awk '{print $8}'`
    for AFCK in $GDPB
    do
	if [ ! -e /tmp/MissingSync$AFCK ]; then 
	    touch /tmp/MissingSync$AFCK   # request FW reload asap
	    rm    /tmp/GBTX-OK            # will need retuning of GBTX links
	    rm    /tmp/GBTX-OK$AFCK            # will need retuning of GBTX links
	    nohup /home/cbm/software2020/startup_scripts/request_tune.sh &> /home/cbm/logs/2020/RequestTune.log &
	fi
    done 
#   /home/cbm/starsoft/startup_scripts/start_sync_monitor.sh &> /home/cbm/logs/2019/cbmroot_SyncMonitor.log
    root -l -b -q inspect_shift_histo.C  &>> /home/cbm/starsoft/dpbcontrols/LOG/InspectMonitor.log
    
    /home/cbm/starsoft/dpbcontrols/all_demask_input.sh &> /home/cbm/starsoft/dpbcontrols/LOG/ALL_DEMASK_INPUT
    sleep 10
    (( iloop += 1 ))
done
