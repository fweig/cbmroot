fileSizeLimit=50000
nofEventsInFile=1000

#for plutoParticle in omegadalitz; do
for plutoParticle in inmed qgp omegaepem omegadalitz phi; do
	
	dir=/lustre/nyx/cbm/users/gpitsch/CbmRoot/data/lmvm/august18_1kk_field50/8gev/${plutoParticle}/
	#filePatternAna=${dir}/analysis.agag.4.5gev.mbias.*.root
	filePatternAna=${dir}/analysis.auau.8gev.centr.*.root
	outputFileAna=${dir}/analysis.auau.8gev.centr.all.root
	#outputFileAna=${dir}/analysis.agag.4.5gev.mbias.all.root
        rm -rf ${outputFileAna}
	xterm -hold -e "root -l -b -q 'hadd.C(\"${filePatternAna}\", \"${outputFileAna}\", ${fileSizeLimit}, ${nofEventsInFile})'"&
	
        #filePatternLitqa=litqa.agag.4.5gev.mbias.*.root
	#outputFileLitqa=${dir}/litqa.agag.4.5gev.mbias.all.root
        filePatternLitqa=litqa.auau.8gev.centr.*.root
	outputFileLitqa=${dir}/litqa.auau.8gev.centr.all.root
	rm -rf ${outputFileLitqa}
	xterm -hold -e "root -l -b -q 'hadd.C(\"${dir}/${filePatternLitqa}\", \"${outputFileLitqa}\", ${fileSizeLimit}, ${nofEventsInFile})'"&
done
