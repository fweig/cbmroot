#!/bin/bash

# Batch_3: e- only ; Batch_4: e+ only ; Batch_5: e- and e+
BATCH=Batch_5

INDIR1=/lustre/nyx/cbm/users/jbendar/Sim_Outputs/Ring_Track_VS_Position/${BATCH}
INDIR2=Misaligned_3mrad_
OUTDIR1=/u/jbendar/Files/${BATCH}/draw_qa

mkdir ${OUTDIR1}/Aligned ${OUTDIR1}/3mrad_minusX ${OUTDIR1}/3mrad_minusX_minusY ${OUTDIR1}/3mrad_minusX_plusY
mkdir ${OUTDIR1}/3mrad_minusY ${OUTDIR1}/3mrad_plusX ${OUTDIR1}/3mrad_plusX_minusY ${OUTDIR1}/3mrad_plusX_plusY
mkdir ${OUTDIR1}/3mrad_plusY

cp -R ${INDIR1}/Aligned/results_* ${OUTDIR1}/Aligned/.
cp -R ${INDIR1}/${INDIR2}minusX_Full/results_* ${OUTDIR1}/3mrad_minusX/.
cp -R ${INDIR1}/${INDIR2}minusX_minusY_Full/results_* ${OUTDIR1}/3mrad_minusX_minusY/.
cp -R ${INDIR1}/${INDIR2}minusX_plusY_Full/results_* ${OUTDIR1}/3mrad_minusX_plusY/.
cp -R ${INDIR1}/${INDIR2}minusY_Full/results_* ${OUTDIR1}/3mrad_minusY/.
cp -R ${INDIR1}/${INDIR2}plusX_Full/results_* ${OUTDIR1}/3mrad_plusX/.
cp -R ${INDIR1}/${INDIR2}plusX_minusY_Full/results_* ${OUTDIR1}/3mrad_plusX_minusY/.
cp -R ${INDIR1}/${INDIR2}plusX_plusY_Full/results_* ${OUTDIR1}/3mrad_plusX_plusY/.
cp -R ${INDIR1}/${INDIR2}plusY_Full/results_* ${OUTDIR1}/3mrad_plusY/.
