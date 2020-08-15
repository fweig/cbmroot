#!/bin/bash
VGEO=$1
Setup=$2

if [[ $Setup = "" ]]; then
 Setup=mcbm_beam_2020_03
fi 

root -l -q Create_TOF_Geometry_${VGEO}.C
#cp tof_v14a.root tof_${VGEO}.root
cp -v tof_${VGEO}.geo.root ../../../../geometry/tof/

cp -v tof_${VGEO}_geo.root ../../../../geometry/tof/geofile_tof_${VGEO}.root

exit 0

CURDIR=`pwd`
cd ../..
rm -v test*.root
rm -v ${Setup}*.root
root -l -q './mcbm_transport_beam.C(0,"'$Setup'")'
cp -v ./${Setup}.geo.root     ./geometry/tof/geofile_tof_${VGEO}.root
cp -v ./${Setup}.geo.root ../../geometry/tof/geofile_tof_${VGEO}.root
cp -v ./${Setup}.par.root     ./geometry/tof/tof_${Setup}.par.root
cd $CURDIR

root -l 'create_digipar.C("tof_'${VGEO}'")'
cp tof_${VGEO}.digi.par      ../../../../parameters/tof/
cp geofile_tof_${VGEO}.root  ../../../../geometry/tof/
