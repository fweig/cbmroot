#!/bin/bash

if [ -z "$1" ]; then
  echo 'no geometry version specified'
  return 1
else
  cVersion=$1
fi

if [ -z "$VMCWORKDIR" ]; then
  echo 'no ROOT environment available'
  return 1
fi

currdir=`pwd`
workdir=${VMCWORKDIR}/macro/tof/geometry

cd ${workdir}

root -b -q ''${workdir}'/Create_TOF_Geometry_'${cVersion}'.C'

cp -v ${workdir}/tof_v14a.root ${VMCWORKDIR}/geometry/tof/tof_${cVersion}.root
mv -v ${workdir}/tof_v14a.root ${VMCWORKDIR}/geometry/tof/tof_${cVersion}.geo.root

root -b -q ''${workdir}'/make_geofile_noBfield.C("tof_'${cVersion}'")'

cp -v ${workdir}/geofile_tof_${cVersion}.root ${VMCWORKDIR}/geometry/tof/

root -b -q ''${workdir}'/create_digipar.C("tof_'${cVersion}'")'

mv -v ${workdir}/tof_${cVersion}.digi.par ${VMCWORKDIR}/parameters/tof/

rm -v ${workdir}/auaumbias.tof_${cVersion}.mc.root
#rm -v ${workdir}/auaumbias.tof_${cVersion}.params.root
rm -v ${workdir}/tof_${cVersion}.par.root
rm -v ${workdir}/geofile_tof_${cVersion}.root
rm -v ${workdir}/gphysi.dat
rm -v ${workdir}/test.esd.tof_${cVersion}.root

cd ${currdir}
