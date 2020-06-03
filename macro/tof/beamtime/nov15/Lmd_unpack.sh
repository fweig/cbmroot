#!/bin/bash
# shell script to unpack files automatically
#Use for TSU strip counter also 
#char* cRun="CbmTofQa_28May1635_mrpc1_000_mrpc2_200_mrpc3_200_nb"
#char* cRun="CbmTofQa_21May1538_mrpc1_000_mrpc2_200_mrpc3_200_nb"
#.x setup_unpack.C(100000000,start_run,end_run,calMode,cRun,iSet);

root -l -b<<EOF
Int_t start_run=0
Int_t end_run=6
Int_t calMode=1
char* cRun="CbmTofQa_29May0954_mrpc1_000_mrpc2_200_mrpc3_200_nb"
Int_t iSet=0

FairRunOnline* run = new FairRunOnline();
.x setup_unpack.C(100000000,start_run,end_run,calMode,cRun,iSet);
run->Run(-1);
run->Finish();
.q
EOF

#sudo shutdown -h 20:00

