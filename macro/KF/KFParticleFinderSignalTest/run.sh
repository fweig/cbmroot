# Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Maxim Zyzak

# #!
# run KF Particle Finder

MAINDIR=`pwd`

if [ -z $1 ]
then
  NEVENTS=1000
else
  NEVENTS=$1
fi

if [ -z $2 ]
then
  WORKDIR=$MAINDIR"/data"
else
  WORKDIR=$2
fi

bash runSimAll.sh  $NEVENTS $WORKDIR
bash runPhysAll.sh $NEVENTS $WORKDIR

bash CollectHisto.sh $WORKDIR
