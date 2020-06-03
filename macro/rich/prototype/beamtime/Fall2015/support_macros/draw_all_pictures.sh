#!/bin/bash

INFILE=$1
PICFOLDER=$2
LOGFILE=$3

root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"10\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"11\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"12\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"13\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"20\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"21\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"22\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"23\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"30\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"31\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"32\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"33\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"40\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"41\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"42\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"43\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE

root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"50\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"51\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"52\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"53\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"60\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"61\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"62\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"63\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"70\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"71\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"72\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"73\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"80\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"81\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"82\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"83\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE

root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"90\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"91\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"92\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"93\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"a0\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"a1\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"a2\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"a3\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"b0\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"b1\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"b2\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"b3\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"c0\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"c1\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"c2\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"c3\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE

root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"d0\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"d1\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"d2\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"d3\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"e0\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"e1\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"e2\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"e3\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"f0\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"f1\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"f2\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"f3\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"100\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"101\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"102\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
root -l -b -q "drawAllHisto3.C(\"$INFILE\", \"103\", \"$PICFOLDER\")" >> $LOGFILE 2>> $LOGFILE
