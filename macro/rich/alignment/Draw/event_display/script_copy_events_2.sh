#!/bin/bash

BATCH=Batch_6		# Batch_3: e- only ; Batch_4: e+ only ; Batch_5: e- and e+
NBTASK=0012

INDIR1=/lustre/nyx/cbm/users/jbendar/Sim_Outputs/Ring_Track_VS_Position/${BATCH}
INDIR2=Misaligned_3mrad_
OUTDIR1=/u/jbendar/Files/${BATCH}/event_display

mkdir ${OUTDIR1}/Aligned ${OUTDIR1}/3mrad_minusX ${OUTDIR1}/3mrad_minusX_minusY ${OUTDIR1}/3mrad_minusX_plusY
mkdir ${OUTDIR1}/3mrad_minusY ${OUTDIR1}/3mrad_plusX ${OUTDIR1}/3mrad_plusX_minusY ${OUTDIR1}/3mrad_plusX_plusY
mkdir ${OUTDIR1}/3mrad_plusY

if [ ${BATCH} = Batch_6 ] ; then
	nbJobTask=18093899_${NBTASK}	#Batch_6
fi
cp -R ${INDIR1}/Aligned/${nbJobTask}/png/rich_event_display_event_* ${OUTDIR1}/Aligned/.
if [ ${BATCH} = Batch_6 ] ; then
	nbJobTask=18094709_${NBTASK}	#Batch_6
fi
cp -R ${INDIR1}/${INDIR2}minusX_Full/${nbJobTask}/png/rich_event_display_event_* ${OUTDIR1}/3mrad_minusX/.
if [ ${BATCH} = Batch_6 ] ; then
	nbJobTask=18095519_${NBTASK}	#Batch_6
fi
cp -R ${INDIR1}/${INDIR2}minusY_Full/${nbJobTask}/png/rich_event_display_event_* ${OUTDIR1}/3mrad_minusY/.
if [ ${BATCH} = Batch_6 ] ; then
	nbJobTask=18096330_${NBTASK}	#Batch_6
fi
cp -R ${INDIR1}/${INDIR2}plusX_Full/${nbJobTask}/png/rich_event_display_event_* ${OUTDIR1}/3mrad_plusX/.
if [ ${BATCH} = Batch_6 ] ; then
	nbJobTask=18097140_${NBTASK}	#Batch_6
fi
cp -R ${INDIR1}/${INDIR2}plusY_Full/${nbJobTask}/png/rich_event_display_event_* ${OUTDIR1}/3mrad_plusY/.
