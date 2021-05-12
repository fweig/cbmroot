#!/bin/bash
# Copyright (C) 2012 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by David Emschermann

#
# create for a list of geometries in root format
# the corresponding trd digitization parameters
#

# as defined in create_geometry_file_root.C,
# trd_v13q.root file is expected in:
directory=geometry/trd
#directory=../mcbm/geometry/trd
geoTag=${1-"v18o_mcbm"}

if [ $# -eq 0 ]; then
  echo "Usage : $(basename $0) [geoTag]"
  echo -e "        Creating trd parameters for geometry tag $geoTag\n"
fi

macroDir=$VMCWORKDIR/macro/trd

for file in $VMCWORKDIR/$directory/trd_$geoTag.geo.root; do

  echo "  Processing \"$file\" ..."
  fileName=$(basename $file)
#echo "fileName : $fileName"
  fileNameNoExt=${fileName%.*.*}
#echo "fileNoExt: $fileNameNoExt"

# prepare geometry file
  root -l -b -q $macroDir/create_geometry_file_root.C\(\"$fileNameNoExt\"\)
# extract digi parameters
  root -l -b -q $macroDir/create_digipar_root.C\(\"$fileNameNoExt\"\)

#split file
# extract asic.par
  cat ${fileNameNoExt}.par | awk -v tag=$geoTag 'BEGIN{last=""; done=0; out="trd_"tag".asic.par"} {if(last==$0 && match(last, "###") && ! done){ out="trd_"tag".1.par"; done=1;} else if(last!=""){ print last > out;} last=$0} END{print last>out}'
  echo "##############################################################################" >> trd_${geoTag}.asic.par
# extract digi.par
  cat trd_${geoTag}.1.par | awk -v tag=$geoTag 'BEGIN{last=""; done=0; out="trd_"tag".digi.par"} {if(last==$0  && match(last, "###") && ! done){ out="trd_"tag".2.par"; done=1;} else if(last!=""){ print last > out;} last=$0} END{print last>out}'   
  echo "##############################################################################" >> trd_${geoTag}.digi.par
  rm trd_${geoTag}.1.par
# extract gas.par and gain.par
  cat trd_${geoTag}.2.par | awk -v tag=$geoTag 'BEGIN{last=""; done=0; out="trd_"tag".gas.par"} {if(last==$0 && match(last, "###") && ! done){ out="trd_"tag".gain.par"; done=1;} else if(last!=""){ print last > out;} last=$0} END{print last>out}'   
  echo "##############################################################################" >> trd_${geoTag}.gas.par
  echo "##############################################################################" >> trd_${geoTag}.gain.par
  rm trd_${geoTag}.2.par

# # compile, if needed
#  if [ ! -x ./cut_digipar_lf ] ; then 
#   echo "compiling cut_digipar_lf.c"
#   echo g++ -o cut_digipar_lf cut_digipar_lf.c
#   g++ -o cut_digipar_lf cut_digipar_lf.c
#   echo done
#  fi
# 
# # cut trailing 0x0a
#  if [ -x ./cut_digipar_lf ] ; then 
#   echo ""
#   echo ">> Please remember to delete the last line in all the created parameter files. <<"
#   echo "./cut_digipar_lf $fileNameNoExt.digi.par.long $fileNameNoExt.digi.par"
#   echo ""
#   ./cut_digipar_lf $fileNameNoExt.digi.par.long $fileNameNoExt.digi.par
# #cp $fileNameNoExt.digi.par $directory
#  fi

done

#[ -x digipar_cut_lf ] echo digipar_cut_lf digipar_cut_lf.cut
# # fileName=$(basename $file)
# # fileNameNoExt=${fileName%.*}
# # echo mv $fileNameNoExt.digi.par $directory
# # echo mv geofile_$fileNameNoExt.root $directory

# echo ""
# echo ">> Please remember to delete the last line in all the created parameter files. <<"
# echo "cut_digipar_lf file.digi.par.long file.digi.par"
# echo ""
