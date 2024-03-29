#!/bin/bash
# Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by David Emschermann

. /data6/cbm/software/cbmroot_trunk_20140702/build/config.sh > /dev/null
nEvents=$NEVENTS
urqmd=$NURQMD
centrality=mbias
echo "root -l -b -q run_sim_maf.C'('$nEvents,$urqmd,'\"'$centrality'\"'')'"
#echo "nice root -l -b -q run_sim_maf.C'('$nEvents,$urqmd,'\"'$centrality'\"'')' &"  > mymafsim
#
#echo "root -l -b -q run_sim_maf.C'('$nEvents,$urqmd,'\"'$centrality'\"'')'"  > mymafsim
echo "root -l -b -q run_sim_maf_delta.C'('$nEvents,$urqmd,'\"'$centrality'\"'')'"  > mymafsim
. mymafsim
