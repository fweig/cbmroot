#!/bin/bash
VGEO=$1

if [[ '' == ${VGEO} ]]; then
  echo "This script needs the geometry version as parameter. E.g.:"
  echo "source create_geopar_allsetups.sh v16c"
  return
fi

# Generate the 1h geo file
root -l -q Create_TOF_Geometry_${VGEO}.C

# Generate the 1e geo file
sed -i.bak 's|const TString geoVersion = "tof_'${VGEO}'_1h";|//const TString geoVersion = "tof_'${VGEO}'_1h";|' Create_TOF_Geometry_${VGEO}.C
sed -i.tmp 's|//const TString geoVersion = "tof_'${VGEO}'_1e";|const TString geoVersion = "tof_'${VGEO}'_1e";|' Create_TOF_Geometry_${VGEO}.C
root -l -q Create_TOF_Geometry_${VGEO}.C

# Generate the 1m geo file
sed -i.tmp 's|const TString geoVersion = "tof_'${VGEO}'_1e";|//const TString geoVersion = "tof_'${VGEO}'_1e";|' Create_TOF_Geometry_${VGEO}.C
sed -i.tmp 's|//const TString geoVersion = "tof_'${VGEO}'_1m";|const TString geoVersion = "tof_'${VGEO}'_1m";|' Create_TOF_Geometry_${VGEO}.C
root -l -q Create_TOF_Geometry_${VGEO}.C

# Generate the 3e geo file
sed -i.tmp 's|const TString geoVersion = "tof_'${VGEO}'_1m";|//const TString geoVersion = "tof_'${VGEO}'_1m";|' Create_TOF_Geometry_${VGEO}.C
sed -i.tmp 's|//const TString geoVersion = "tof_'${VGEO}'_3e";|const TString geoVersion = "tof_'${VGEO}'_3e";|' Create_TOF_Geometry_${VGEO}.C
root -l -q Create_TOF_Geometry_${VGEO}.C

# Generate the 3m geo file
sed -i.tmp 's|const TString geoVersion = "tof_'${VGEO}'_3e";|//const TString geoVersion = "tof_'${VGEO}'_3e";|' Create_TOF_Geometry_${VGEO}.C
sed -i.tmp 's|//const TString geoVersion = "tof_'${VGEO}'_3m";|const TString geoVersion = "tof_'${VGEO}'_3m";|' Create_TOF_Geometry_${VGEO}.C
root -l -q Create_TOF_Geometry_${VGEO}.C

# Restore the macro file to 1h
mv Create_TOF_Geometry_${VGEO}.C.bak Create_TOF_Geometry_${VGEO}.C

# Copy all geo files
cp tof_${VGEO}_*.geo.root ../../../geometry/tof/

# Generate the geofiles
root -l -q 'make_geofile.C("tof_'${VGEO}'_1h")'
root -l -q 'make_geofile.C("tof_'${VGEO}'_1e")'
root -l -q 'make_geofile.C("tof_'${VGEO}'_1m")'
root -l -q 'make_geofile.C("tof_'${VGEO}'_3e")'
root -l -q 'make_geofile.C("tof_'${VGEO}'_3m")'

# Generate the digi parameter files
root -l -q 'create_digipar.C("tof_'${VGEO}'_1h")'
root -l -q 'create_digipar.C("tof_'${VGEO}'_1e")'
root -l -q 'create_digipar.C("tof_'${VGEO}'_1m")'
root -l -q 'create_digipar.C("tof_'${VGEO}'_3e")'
root -l -q 'create_digipar.C("tof_'${VGEO}'_3m")'

cp tof_${VGEO}_*.digi.par ../../../parameters/tof/

# cleanup extra files
rm auaumbias.tof_${VGEO}_*.mc.root auaumbias.tof_${VGEO}_*.params.root test.esd.tof_${VGEO}_*.root gphysi.dat
rm tof_${VGEO}_*.geo.root tof_${VGEO}_*_geo.root tof_${VGEO}_*.geo.info geofile_tof_${VGEO}_*.root tof_${VGEO}_*.digi.par
rm Create_TOF_Geometry_${VGEO}.C.tmp
