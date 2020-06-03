#!/bin/bash
VGEO=$1

root -l -q Create_TOF_Geometry_${VGEO}.C
#cp tof_v14a.root tof_${VGEO}.root
cp tof_${VGEO}.root ../../../geometry/tof/tof_${VGEO}.root
#ln -s  ../../../geometry/tof/tof_${VGEO}.root ../../../geometry/tof/tof_${VGEO}.geo.root
root -l -q 'make_geofile_noBfield.C("tof_'${VGEO}'")'
cp geofile_tof_${VGEO}.root ../../../geometry/tof/
root -l 'create_digipar.C("tof_'${VGEO}'")'
cp tof_${VGEO}.digi.par ../../../parameters/tof/
