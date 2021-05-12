#!/bin/bash
# Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by David Emschermann

. /data6/cbm/software/cbmroot_trunk_20140702/build/config.sh > /dev/null
nEvents=$NEVENTS
urqmd=$NURQMD
echo "root -l -b -q run_reco_maf.C'('$nEvents,$urqmd')'"
#echo "nice root -l -b -q run_reco_maf.C'('$nEvents,$urqmd')' &"  > myreco
echo "root -l -b -q run_reco_maf.C'('$nEvents,$urqmd')'"  > mymafreco
. mymafreco
