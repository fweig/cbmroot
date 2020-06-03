#!/bin/bash

INDIR=/lustre/nyx/cbm/users/jbendar/Sim_Outputs/Matching
OUTDIR1=/u/jbendar/Files
OUTDIR2=Matching_1

mkdir ${OUTDIR1}/${OUTDIR2}/WO_Corr_1mrad/ ${OUTDIR1}/${OUTDIR2}/WO_Corr_2mrad/ ${OUTDIR1}/${OUTDIR2}/WO_Corr_3mrad/ ${OUTDIR1}/${OUTDIR2}/WO_Corr_4mrad/ ${OUTDIR1}/${OUTDIR2}/WO_Corr_5mrad/

#cp -R ${INDIR}/Reference/results_* ${OUTDIR1}/${OUTDIR2}/Reference/.
cp -R ${INDIR}/WO_Corrections_1mrad/results_* ${OUTDIR1}/${OUTDIR2}/WO_Corr_1mrad/.
cp -R ${INDIR}/WO_Corrections_2mrad/results_* ${OUTDIR1}/${OUTDIR2}/WO_Corr_2mrad/.
cp -R ${INDIR}/WO_Corrections_3mrad/results_* ${OUTDIR1}/${OUTDIR2}/WO_Corr_3mrad/.
cp -R ${INDIR}/WO_Corrections_4mrad/results_* ${OUTDIR1}/${OUTDIR2}/WO_Corr_4mrad/.
cp -R ${INDIR}/WO_Corrections_5mrad/results_* ${OUTDIR1}/${OUTDIR2}/WO_Corr_5mrad/.
