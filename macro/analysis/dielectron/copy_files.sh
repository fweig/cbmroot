#####!/bin/sh

#mainDirFrom=/lustre/nyx/cbm/users/slebedev/cbm/data/lmvm/agagtest/3.5gev/
#mainDirTo=/u/slebedev/Baykal/lmvm/agagtest/3.5gev/
mainDirFrom=/lustre/nyx/cbm/users/gpitsch/CbmRoot/data/lmvm/august18_1kk_field50/8gev/
mainDirTo=/u/gpitsch/lmvm/august18_1kk_field50/8gev/
#fileNameAna=analysis.agag.4.5gev.mbias.all.root
#fileNameLitqa=litqa.agag.4.5gev.mbias.all.root
fileNameAna=analysis.auau.8gev.centr.all.root
fileNameLitqa=litqa.auau.8gev.centr.all.root

copy_func() {
   dirFrom=${1}
   dirTo=${2}
   fileName=${3}
   plutoParticle=${4}
   mkdir -p ${dirTo}/${plutoParticle}/
   cp ${dirFrom}/${plutoParticle}/${fileName} ${dirTo}/${plutoParticle}/
}

copy_all_pluto_particles() {
  #copy_func ${1} ${2} ${3} rho0
  copy_func ${1} ${2} ${3} omegaepem
  copy_func ${1} ${2} ${3} omegadalitz
  copy_func ${1} ${2} ${3} phi 
  copy_func ${1} ${2} ${3} inmed
  copy_func ${1} ${2} ${3} qgp
#  copy_func ${1} ${2} ${3} urqmd
}


rm -rf ${mainDirTo}
copy_all_pluto_particles ${mainDirFrom}/ ${mainDirTo}/ ${fileNameAna}
copy_all_pluto_particles ${mainDirFrom}/ ${mainDirTo}/ ${fileNameLitqa}


