#!/bin/bash

input_dir_1=/lustre/nyx/cbm/users/jbendar/Sim_Outputs/Ring_Track_VS_Position/Batch_5
input_dir_2=Misaligned_3mrad_
reco_name=setup_misalign_3mrad_

#for i in `seq 0 8`;
for i in 0
do
	if [ $i -eq 0 ] ; then
		export MERGE_ROOT=${input_dir_1}/Aligned/merge_align_2.root
		export INDIR=${input_dir_1}/Aligned/setup_align_reco.*
		xterm -hold -e "./script_merge_2.sh"&
	elif [ $i -eq 1 ] ; then
		export MERGE_ROOT=${input_dir_1}/${input_dir_2}minusX_Full/merge_3mrad_minusX.root
		export INDIR=${input_dir_1}/${input_dir_2}minusX_Full/${reco_name}minusX_reco.*
		xterm -hold -e "./script_merge_2.sh"&
	elif [ $i -eq 2 ] ; then
		export MERGE_ROOT=${input_dir_1}/${input_dir_2}minusX_minusY_Full/merge_3mrad_minusX_minusY.root
		export INDIR=${input_dir_1}/${input_dir_2}minusX_minusY_Full/${reco_name}minusX_minusY_reco.*
		xterm -hold -e "./script_merge_2.sh"&
	elif [ $i -eq 3 ] ; then
		export MERGE_ROOT=${input_dir_1}/${input_dir_2}minusX_plusY_Full/merge_3mrad_minusX_plusY.root
		export INDIR=${input_dir_1}/${input_dir_2}minusX_plusY_Full/${reco_name}minusX_plusY_reco.*
		xterm -hold -e "./script_merge_2.sh"&
	elif [ $i -eq 4 ] ; then
		export MERGE_ROOT=${input_dir_1}/${input_dir_2}minusY_Full/merge_3mrad_minusY.root
		export INDIR=${input_dir_1}/${input_dir_2}minusY_Full/${reco_name}minusY_reco.*
		xterm -hold -e "./script_merge_2.sh"&
	elif [ $i -eq 5 ] ; then
		export MERGE_ROOT=${input_dir_1}/${input_dir_2}plusX_Full/merge_3mrad_plusX.root
		export INDIR=${input_dir_1}/${input_dir_2}plusX_Full/${reco_name}plusX_reco.*
		xterm -hold -e "./script_merge_2.sh"&
	elif [ $i -eq 6 ] ; then
		export MERGE_ROOT=${input_dir_1}/${input_dir_2}plusX_minusY_Full/merge_3mrad_plusX_minusY.root
		export INDIR=${input_dir_1}/${input_dir_2}plusX_minusY_Full/${reco_name}plusX_minusY_reco.*
		xterm -hold -e "./script_merge_2.sh"&
	elif [ $i -eq 7 ] ; then
		export MERGE_ROOT=${input_dir_1}/${input_dir_2}plusX_plusY_Full/merge_3mrad_plusX_plusY.root
		export INDIR=${input_dir_1}/${input_dir_2}plusX_plusY_Full/${reco_name}plusX_plusY_reco.*
		xterm -hold -e "./script_merge_2.sh"&
	elif [ $i -eq 8 ] ; then
		export MERGE_ROOT=${input_dir_1}/${input_dir_2}plusY_Full/merge_3mrad_plusY.root
		export INDIR=${input_dir_1}/${input_dir_2}plusY_Full/${reco_name}plusY_reco.*
		xterm -hold -e "./script_merge_2.sh"&
	fi
done
