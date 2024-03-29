#!/bin/bash
# Copyright (C) 2016 PI-UHd,GSI
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Norbert Herrmann

# *************************************************************************** #
# 							Setting up Geometry of Cosmic Setup				  				#
# *************************************************************************** #
#source ${HOME}/Software/cbmroot/build/config.sh

cbmlogin
GEODIR=${HOME}/Software/cbmroot/src/macro/tof/geometry
cd ${GEODIR}

root -l -q Create_TOF_Geometry_v15d.C  
cp tof_v14a.root ../../../geometry/tof/tof_v15d.root
cp tof_v14a.root tof_v15d.root
#root -l -q 'make_geofile.C("tof_v15d")'
root -l -q 'make_geofile_noBfield.C("tof_v15d")'
cp geofile_tof_v15d.root ../../../geometry/tof/
root -l -q 'create_digipar.C("tof_v15d")'
cp tof_v15d.digi.par ../../../parameters/tof/
