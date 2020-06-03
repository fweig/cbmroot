export SCRIPT=yes

draw_litqa() 
{
	export LMVM_LITQA_FILE_NAME=${2}
	export LMVM_MAIN_DIR=${1}/${3}
	root -b -l -q draw_litqa.C
}

draw_ana()
{
	export LMVM_ANA_FILE_NAME=${2}
	export LMVM_MAIN_DIR=${1}/${3}
	root -b -l -q draw_analysis.C
}

draw_ana_all()
{
	export LMVM_ANA_FILE_NAME=${2}
	export LMVM_MAIN_DIR=${1}
	root -b -l -q draw_analysis_all.C
}

draw_all()
{
	main_dir=${1}
	ana_file=analysis${2}
	litqa_file=litqa${2}
	echo "main_dir:" ${main_dir}
	echo "ana_file:" ${ana_file}
	echo "litqa_file:" ${litqa_file}
	
	echo "Draw LITQA"
	draw_litqa ${main_dir} ${litqa_file} rho0/
	#draw_litqa ${main_dir} ${litqa_file} omegaepem/
	#draw_litqa ${main_dir} ${litqa_file} phi/
	#draw_litqa ${main_dir} ${litqa_file} omegadalitz/
	
	echo "Draw ANALYSIS"
	draw_ana ${main_dir} ${ana_file} rho0/
	draw_ana ${main_dir} ${ana_file} omegaepem/
	draw_ana ${main_dir} ${ana_file} phi/
	draw_ana ${main_dir} ${ana_file} omegadalitz/
	
	echo "Draw ANALYSIS ALL"
	draw_ana_all ${main_dir} ${ana_file}
}


draw_all /Users/slebedev/Development/cbm/data/lmvm/test6/8gev/ .auau.8gev.centr.all.root


export SCRIPT=no