#!/bin/bash

input_dir_1=/lustre/nyx/cbm/users/jbendar/Sim_Outputs/Ring_Track_VS_Position/Batch_5
input_dir_2=Misaligned_3mrad_
reco_name=setup_misalign_3mrad_

echo ${input_dir_1}

#cd ${INDIR1}/Aligned/results_*
hadd -T ${input_dir_1}/Aligned/merge_align.root ${input_dir_1}/Aligned/setup_align_reco.*

#cd ${INDIR1}/${INDIR2}minusX_Full/results_*
hadd -T ${input_dir_1}/${input_dir_2}minusX_Full/merge_3mrad_minusX.root ${input_dir_1}/${input_dir_2}minusX_Full/${reco_name}minusX_reco.*

#cd ${INDIR1}/${INDIR2}minusX_minusY_Full/results_*
hadd -T ${input_dir_1}/${input_dir_2}minusX_minusY_Full/merge_3mrad_minusX_minusY.root ${input_dir_1}/${input_dir_2}minusX_minusY_Full/${reco_name}minusX_minusY_reco.*

#cd ${INDIR1}/${INDIR2}minusX_plusY_Full/results_*
hadd -T ${input_dir_1}/${input_dir_2}minusX_plusY_Full/merge_3mrad_minusX_plusY.root ${input_dir_1}/${input_dir_2}minusX_plusY_Full/${reco_name}minusX_plusY_reco.*

#cd ${INDIR1}/${INDIR2}minusY_Full/results_*
hadd -T ${input_dir_1}/${input_dir_2}minusY_Full/merge_3mrad_minusY.root ${input_dir_1}/${input_dir_2}minusY_Full/${reco_name}minusY_reco.*

#cd ${INDIR1}/${INDIR2}plusX_Full/results_*
hadd -T ${input_dir_1}/${input_dir_2}plusX_Full/merge_3mrad_plusX.root ${input_dir_1}/${input_dir_2}plusX_Full/${reco_name}plusX_reco.*

#cd ${INDIR1}/${INDIR2}plusX_minusY_Full/results_*
hadd -T ${input_dir_1}/${input_dir_2}plusX_minusY_Full/merge_3mrad_plusX_minusY.root ${input_dir_1}/${input_dir_2}plusX_minusY_Full/${reco_name}plusX_minusY_reco.*

#cd ${INDIR1}/${INDIR2}plusX_plusY_Full/results_*
hadd -T ${input_dir_1}/${input_dir_2}plusX_plusY_Full/merge_3mrad_plusX_plusY.root ${input_dir_1}/${input_dir_2}plusX_plusY_Full/${reco_name}plusX_plusY_reco.*

#cd ${INDIR1}/${INDIR2}plusY_Full/results_*
hadd -T ${input_dir_1}/${input_dir_2}plusY_Full/merge_3mrad_plusY.root ${input_dir_1}/${input_dir_2}plusY_Full/${reco_name}plusY_reco.*
