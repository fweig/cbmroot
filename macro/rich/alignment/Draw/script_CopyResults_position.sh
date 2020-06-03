#!/bin/bash

INDIR=/lustre/nyx/cbm/users/jbendar/Sim_Outputs/Ring_Track_VS_Position/Batch_4
OUTDIR1=/u/jbendar/Files/Batch_4
#OUTDIR2=Position_

mkdir ${OUTDIR1}/Aligned ${OUTDIR1}/Misaligned_1mrad ${OUTDIR1}/Misaligned_2mrad ${OUTDIR1}/Misaligned_3mrad ${OUTDIR1}/Misaligned_4mrad
mkdir ${OUTDIR1}/Misaligned_5mrad ${OUTDIR1}/Misaligned_OFF

cp -R ${INDIR}/Aligned/results_* ${OUTDIR1}/${OUTDIR2}/Aligned/.
cp -R ${INDIR}/Misaligned_1mrad_Full/results_* ${OUTDIR1}/${OUTDIR2}/Misaligned_1mrad/.
cp -R ${INDIR}/Misaligned_2mrad_Full/results_* ${OUTDIR1}/${OUTDIR2}/Misaligned_2mrad/.
cp -R ${INDIR}/Misaligned_3mrad_XY_Full/results_* ${OUTDIR1}/${OUTDIR2}/Misaligned_3mrad/.
cp -R ${INDIR}/Misaligned_4mrad_Full/results_* ${OUTDIR1}/${OUTDIR2}/Misaligned_4mrad/.
cp -R ${INDIR}/Misaligned_5mrad_Full/results_* ${OUTDIR1}/${OUTDIR2}/Misaligned_5mrad/.
cp -R ${INDIR}/Misaligned_OFF/results_* ${OUTDIR1}/Misaligned_OFF/.
