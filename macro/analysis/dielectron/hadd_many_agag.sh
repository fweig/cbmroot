# Copyright (C) 2018 Justus-Liebig-Universitaet Giessen, Giessen
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Gregor Pitsch

fileSizeLimit=50000
nofEventsInFile=10000

#for plutoParticle in omegaepem; do
for plutoParticle in inmed omegaepem omegadalitz phi; do
	
	dir=/lustre/nyx/cbm/users/gpitsch/CbmRoot/data/lmvm/agag/august18_2kk_field60/4.5gev/${plutoParticle}/
	filePatternAna=${dir}/analysis.agag.4.5gev.mbias.*.root
	#filePatternAna=${dir}/analysis.auau.8gev.centr.*.root
	#outputFileAna=${dir}/analysis.auau.8gev.centr.all.root
	outputFileAna=${dir}/analysis.agag.4.5gev.mbias.all.root
        rm -rf ${outputFileAna}
	xterm -hold -e "root -l -b -q 'hadd.C(\"${filePatternAna}\", \"${outputFileAna}\", ${fileSizeLimit}, ${nofEventsInFile})'"&
	
        filePatternLitqa=litqa.agag.4.5gev.mbias.*.root
	outputFileLitqa=${dir}/litqa.agag.4.5gev.mbias.all.root
        #filePatternLitqa=litqa.auau.8gev.centr.*.root
	#outputFileLitqa=${dir}/litqa.auau.8gev.centr.all.root
	rm -rf ${outputFileLitqa}
	xterm -hold -e "root -l -b -q 'hadd.C(\"${dir}/${filePatternLitqa}\", \"${outputFileLitqa}\", ${fileSizeLimit}, ${nofEventsInFile})'"&
done
