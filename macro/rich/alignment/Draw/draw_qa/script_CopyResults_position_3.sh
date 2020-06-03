#!/bin/bash

BATCH=Batch_8

INDIR1=/lustre/nyx/cbm/users/jbendar/Sim_Outputs/Ring_Track_VS_Position/${BATCH}
INDIR2=Misaligned_3mrad_
OUTDIR1=/u/jbendar/Files/${BATCH}/draw_qa

mkdir ${OUTDIR1}/Aligned ${OUTDIR1}/3mrad_minusX ${OUTDIR1}/3mrad_minusY ${OUTDIR1}/3mrad_plusX ${OUTDIR1}/3mrad_plusY

cp -R ${INDIR1}/Aligned/results_* ${OUTDIR1}/Aligned/.
cp -R ${INDIR1}/${INDIR2}minusX_Full/results_* ${OUTDIR1}/3mrad_minusX/.
#cp -R ${INDIR1}/${INDIR2}minusY_Full/results_* ${OUTDIR1}/3mrad_minusY/.
cp -R ${INDIR1}/${INDIR2}plusX_Full/results_* ${OUTDIR1}/3mrad_plusX/.
#cp -R ${INDIR1}/${INDIR2}plusY_Full/results_* ${OUTDIR1}/3mrad_plusY/.
