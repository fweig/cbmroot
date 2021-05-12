#!/bin/bash
# Copyright (C) 2017 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by David Emschermann

VGEO=$1

echo ${VGEO}

root -l -q Create_TOF_Geometry_${VGEO}_mcbm.C
cp tof_${VGEO}_mcbm.geo.root ../../../../geometry/tof/tof_${VGEO}_mcbm.geo.root
cp tof_${VGEO}_mcbm.geo.info ../../../../geometry/tof/tof_${VGEO}_mcbm.geo.info
root -l 'create_tof_digipar.C("tof_'${VGEO}'_mcbm")'
cp tof_${VGEO}_mcbm.digi.par ../../../../parameters/tof/tof_${VGEO}_mcbm.digi.par
rm tof_${VGEO}_mcbm* test.esd.tof_${VGEO}_mcbm.root
