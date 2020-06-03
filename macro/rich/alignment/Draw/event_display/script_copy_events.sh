#!/bin/bash

BATCH=Batch_3
# Batch_3: e- only ; Batch_4: e+ only ; Batch_5: e- and e+
NBTASK=0012

INDIR1=/lustre/nyx/cbm/users/jbendar/Sim_Outputs/Ring_Track_VS_Position/${BATCH}
INDIR2=Misaligned_3mrad_
OUTDIR1=/u/jbendar/Files/${BATCH}/event_display

mkdir ${OUTDIR1}/Aligned ${OUTDIR1}/3mrad_minusX ${OUTDIR1}/3mrad_minusX_minusY ${OUTDIR1}/3mrad_minusX_plusY
mkdir ${OUTDIR1}/3mrad_minusY ${OUTDIR1}/3mrad_plusX ${OUTDIR1}/3mrad_plusX_minusY ${OUTDIR1}/3mrad_plusX_plusY
mkdir ${OUTDIR1}/3mrad_plusY

if [ ${BATCH} = Batch_3 ] ; then
	nbJobTask=18016926_${NBTASK}	#Batch_3
elif [ ${BATCH} = Batch_4 ] ; then
	nbJobTask=18009632_${NBTASK}	#Batch_4
elif [ ${BATCH} = Batch_5 ] ; then
	nbJobTask=17998965_${NBTASK}	#Batch_5
fi
cp -R ${INDIR1}/Aligned/${nbJobTask}/png/rich_event_display_event_* ${OUTDIR1}/Aligned/.
if [ ${BATCH} = Batch_3 ] ; then
	nbJobTask=18017736_${NBTASK}	#Batch_3
elif [ ${BATCH} = Batch_4 ] ; then
	nbJobTask=18010442_${NBTASK}	#Batch_4
elif [ ${BATCH} = Batch_5 ] ; then
	nbJobTask=18002339_${NBTASK}	#Batch_5
fi
cp -R ${INDIR1}/${INDIR2}minusX_Full/${nbJobTask}/png/rich_event_display_event_* ${OUTDIR1}/3mrad_minusX/.
if [ ${BATCH} = Batch_3 ] ; then
	nbJobTask=18018546_${NBTASK}	#Batch_3
elif [ ${BATCH} = Batch_4 ] ; then
	nbJobTask=18011252_${NBTASK}	#Batch_4
elif [ ${BATCH} = Batch_5 ] ; then
	nbJobTask=18003149_${NBTASK}	#Batch_5
fi
cp -R ${INDIR1}/${INDIR2}minusX_minusY_Full/${nbJobTask}/png/rich_event_display_event_* ${OUTDIR1}/3mrad_minusX_minusY/.
if [ ${BATCH} = Batch_3 ] ; then
	nbJobTask=18019356_${NBTASK}	#Batch_3
elif [ ${BATCH} = Batch_4 ] ; then
	nbJobTask=18012062_${NBTASK}	#Batch_4
elif [ ${BATCH} = Batch_5 ] ; then
	nbJobTask=18003959_${NBTASK}	#Batch_5
fi
cp -R ${INDIR1}/${INDIR2}minusX_plusY_Full/${nbJobTask}/png/rich_event_display_event_* ${OUTDIR1}/3mrad_minusX_plusY/.
if [ ${BATCH} = Batch_3 ] ; then
	nbJobTask=18020166_${NBTASK}	#Batch_3
elif [ ${BATCH} = Batch_4 ] ; then
	nbJobTask=18012872_${NBTASK}	#Batch_4
elif [ ${BATCH} = Batch_5 ] ; then
	nbJobTask=18004769_${NBTASK}	#Batch_5
fi
cp -R ${INDIR1}/${INDIR2}minusY_Full/${nbJobTask}/png/rich_event_display_event_* ${OUTDIR1}/3mrad_minusY/.
if [ ${BATCH} = Batch_3 ] ; then
	nbJobTask=18020976_${NBTASK}	#Batch_3
elif [ ${BATCH} = Batch_4 ] ; then
	nbJobTask=18013682_${NBTASK}	#Batch_4
elif [ ${BATCH} = Batch_5 ] ; then
	nbJobTask=18005579_${NBTASK}	#Batch_5
fi
cp -R ${INDIR1}/${INDIR2}plusX_Full/${nbJobTask}/png/rich_event_display_event_* ${OUTDIR1}/3mrad_plusX/.
if [ ${BATCH} = Batch_3 ] ; then
	nbJobTask=18021786_${NBTASK}	#Batch_3
elif [ ${BATCH} = Batch_4 ] ; then
	nbJobTask=18014492_${NBTASK}	#Batch_4
elif [ ${BATCH} = Batch_5 ] ; then
	nbJobTask=18006389_${NBTASK}	#Batch_5
fi
cp -R ${INDIR1}/${INDIR2}plusX_minusY_Full/${nbJobTask}/png/rich_event_display_event_* ${OUTDIR1}/3mrad_plusX_minusY/.
if [ ${BATCH} = Batch_3 ] ; then
	nbJobTask=18022596_${NBTASK}	#Batch_3
elif [ ${BATCH} = Batch_4 ] ; then
	nbJobTask=18015302_${NBTASK}	#Batch_4
elif [ ${BATCH} = Batch_5 ] ; then
	nbJobTask=18007199_${NBTASK}	#Batch_5
fi
cp -R ${INDIR1}/${INDIR2}plusX_plusY_Full/${nbJobTask}/png/rich_event_display_event_* ${OUTDIR1}/3mrad_plusX_plusY/.
if [ ${BATCH} = Batch_3 ] ; then
	nbJobTask=18023406_${NBTASK}	#Batch_3
elif [ ${BATCH} = Batch_4 ] ; then
	nbJobTask=18016112_${NBTASK}	#Batch_4
elif [ ${BATCH} = Batch_5 ] ; then
	nbJobTask=18008009_${NBTASK}	#Batch_5
fi
cp -R ${INDIR1}/${INDIR2}plusY_Full/${nbJobTask}/png/rich_event_display_event_* ${OUTDIR1}/3mrad_plusY/.
