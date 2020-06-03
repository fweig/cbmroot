#!/bin/bash

cbmroot_config_path=/lustre/nyx/cbm/users/jbendar/CBMINSTALL_Root5/bin/CbmRootConfig.sh
source ${cbmroot_config_path}

export SCRIPT=yes

macro_dir=/lustre/nyx/cbm/users/jbendar/CBMINSTALL_Root5/share/cbmroot/macro/rich/Draw/event_display/

DIR1=/lustre/nyx/cbm/users/jbendar/Sim_Outputs/Ring_Track_VS_Position
# Batch_3: e- only ; Batch_4: e+ only ; Batch_5: e- and e+
DIR2=Batch_7
NBTASK=0012

Submitted batch job 18493611
Submitted batch job 18494421
Submitted batch job 18495231

NAME_AL=setup_align_
NAME_MIS=setup_misalign_3mrad_

for i in {0..8}
do

if [ $i -eq 0 ] ; then
	if [ ${DIR2} = Batch_3 ] ; then
		nbJobTask=18016926_${NBTASK}
	elif [ ${DIR2} = Batch_4 ] ; then
		nbJobTask=18009632_${NBTASK}
	elif [ ${DIR2} = Batch_5 ] ; then
		nbJobTask=17998965_${NBTASK}
	fi
	outdir=${DIR1}/${DIR2}/Aligned/
	export OUT_DIR=${outdir}
	export NAME=${NAME_AL}
	export NB_JOB_TASK=${nbJobTask}
	root -l -b -q "${macro_dir}/run_rich_event_display.C()"
elif [ $i -eq 1 ] ; then
	if [ ${DIR2} = Batch_3 ] ; then
		nbJobTask=18017736_${NBTASK}
	elif [ ${DIR2} = Batch_4 ] ; then
		nbJobTask=18010442_${NBTASK}
	elif [ ${DIR2} = Batch_5 ] ; then
		nbJobTask=18002339_${NBTASK}
	fi
	outdir=${DIR1}/${DIR2}/Misaligned_3mrad_minusX_Full/
	export OUT_DIR=${outdir}
	export NAME=${NAME_MIS}minusX_
	export NB_JOB_TASK=${nbJobTask}
	root -l -b -q "${macro_dir}/run_rich_event_display.C()"
elif [ $i -eq 2 ] ; then
	if [ ${DIR2} = Batch_3 ] ; then
		nbJobTask=18018546_${NBTASK}
	elif [ ${DIR2} = Batch_4 ] ; then
		nbJobTask=18011252_${NBTASK}
	elif [ ${DIR2} = Batch_5 ] ; then
		nbJobTask=18003149_${NBTASK}
	fi
	outdir=${DIR1}/${DIR2}/Misaligned_3mrad_minusX_minusY_Full/
	export OUT_DIR=${outdir}
	export NAME=${NAME_MIS}minusX_minusY_
	export NB_JOB_TASK=${nbJobTask}
	root -l -b -q "${macro_dir}/run_rich_event_display.C()"
elif [ $i -eq 3 ] ; then
	if [ ${DIR2} = Batch_3 ] ; then
		nbJobTask=18019356_${NBTASK}
	elif [ ${DIR2} = Batch_4 ] ; then
		nbJobTask=18012062_${NBTASK}
	elif [ ${DIR2} = Batch_5 ] ; then
		nbJobTask=18003959_${NBTASK}
	fi
	outdir=${DIR1}/${DIR2}/Misaligned_3mrad_minusX_plusY_Full/
	export OUT_DIR=${outdir}
	export NAME=${NAME_MIS}minusX_plusY_
	export NB_JOB_TASK=${nbJobTask}
	root -l -b -q "${macro_dir}/run_rich_event_display.C()"
elif [ $i -eq 4 ] ; then
	if [ ${DIR2} = Batch_3 ] ; then
		nbJobTask=18020166_${NBTASK}
	elif [ ${DIR2} = Batch_4 ] ; then
		nbJobTask=18012872_${NBTASK}
	elif [ ${DIR2} = Batch_5 ] ; then
		nbJobTask=18004769_${NBTASK}
	fi
	outdir=${DIR1}/${DIR2}/Misaligned_3mrad_minusY_Full/
	export OUT_DIR=${outdir}
	export NAME=${NAME_MIS}minusY_
	export NB_JOB_TASK=${nbJobTask}
	root -l -b -q "${macro_dir}/run_rich_event_display.C()"
elif [ $i -eq 5 ] ; then
	if [ ${DIR2} = Batch_3 ] ; then
		nbJobTask=18020976_${NBTASK}
	elif [ ${DIR2} = Batch_4 ] ; then
		nbJobTask=18013682_${NBTASK}
	elif [ ${DIR2} = Batch_5 ] ; then
		nbJobTask=18005579_${NBTASK}
	fi
	outdir=${DIR1}/${DIR2}/Misaligned_3mrad_plusX_Full/
	export OUT_DIR=${outdir}
	export NAME=${NAME_MIS}plusX_
	export NB_JOB_TASK=${nbJobTask}
	root -l -b -q "${macro_dir}/run_rich_event_display.C()"
elif [ $i -eq 6 ] ; then
	if [ ${DIR2} = Batch_3 ] ; then
		nbJobTask=18021786_${NBTASK}
	elif [ ${DIR2} = Batch_4 ] ; then
		nbJobTask=18014492_${NBTASK}
	elif [ ${DIR2} = Batch_5 ] ; then
		nbJobTask=18006389_${NBTASK}
	fi
	outdir=${DIR1}/${DIR2}/Misaligned_3mrad_plusX_minusY_Full/
	export OUT_DIR=${outdir}
	export NAME=${NAME_MIS}plusX_minusY_
	export NB_JOB_TASK=${nbJobTask}
	root -l -b -q "${macro_dir}/run_rich_event_display.C()"
elif [ $i -eq 7 ] ; then
	if [ ${DIR2} = Batch_3 ] ; then
		nbJobTask=18022596_${NBTASK}
	elif [ ${DIR2} = Batch_4 ] ; then
		nbJobTask=18015302_${NBTASK}
	elif [ ${DIR2} = Batch_5 ] ; then
		nbJobTask=18007199_${NBTASK}
	fi
	outdir=${DIR1}/${DIR2}/Misaligned_3mrad_plusX_plusY_Full/
	export OUT_DIR=${outdir}
	export NAME=${NAME_MIS}plusX_plusY_
	export NB_JOB_TASK=${nbJobTask}
	root -l -b -q "${macro_dir}/run_rich_event_display.C()"
elif [ $i -eq 8 ] ; then
	if [ ${DIR2} = Batch_3 ] ; then
		nbJobTask=18023406_${NBTASK}
	elif [ ${DIR2} = Batch_4 ] ; then
		nbJobTask=18016112_${NBTASK}
	elif [ ${DIR2} = Batch_5 ] ; then
		nbJobTask=18008009_${NBTASK}
	fi
	outdir=${DIR1}/${DIR2}/Misaligned_3mrad_plusY_Full/
	export OUT_DIR=${outdir}
	export NAME=${NAME_MIS}plusY_
	export NB_JOB_TASK=${nbJobTask}
	root -l -b -q "${macro_dir}/run_rich_event_display.C()"
fi

done
