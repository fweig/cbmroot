#!/bin/bash

cbmroot_config_path=/lustre/nyx/cbm/users/jbendar/CBMINSTALL_Root5/bin/CbmRootConfig.sh
source ${cbmroot_config_path}

export SCRIPT=yes

macro_dir=/lustre/nyx/cbm/users/jbendar/CBMINSTALL_Root5/share/cbmroot/macro/rich/Draw/event_display/

DIR1=/lustre/nyx/cbm/users/jbendar/Sim_Outputs/Ring_Track_VS_Position
# Batch_3: e- only ; Batch_4: e+ only ; Batch_5: e- and e+
DIR2=Batch_8
NBTASK=0012

NAME_AL=setup_align_
NAME_MIS=setup_misalign_3mrad_

Submitted batch job 18496041
Submitted batch job 18496851
Submitted batch job 18497661
Submitted batch job 18498471
Submitted batch job 18499281

for i in {0..8}
do

if [ $i -eq 0 ] ; then
	if [ ${DIR2} = Batch_6 ] ; then
		nbJobTask=18093899_${NBTASK}
	fi
	outdir=${DIR1}/${DIR2}/Aligned/
	export OUT_DIR=${outdir}
	export NAME=${NAME_AL}
	export NB_JOB_TASK=${nbJobTask}
	root -l -b -q "${macro_dir}/run_rich_event_display.C()"
elif [ $i -eq 1 ] ; then
	if [ ${DIR2} = Batch_6 ] ; then
		nbJobTask=18094709_${NBTASK}
	fi
	outdir=${DIR1}/${DIR2}/Misaligned_3mrad_minusX_Full/
	export OUT_DIR=${outdir}
	export NAME=${NAME_MIS}minusX_
	export NB_JOB_TASK=${nbJobTask}
	root -l -b -q "${macro_dir}/run_rich_event_display.C()"
elif [ $i -eq 2 ] ; then
	if [ ${DIR2} = Batch_6 ] ; then
		nbJobTask=18095519_${NBTASK}
	fi
	outdir=${DIR1}/${DIR2}/Misaligned_3mrad_minusY_Full/
	export OUT_DIR=${outdir}
	export NAME=${NAME_MIS}minusY_
	export NB_JOB_TASK=${nbJobTask}
	root -l -b -q "${macro_dir}/run_rich_event_display.C()"
elif [ $i -eq 3 ] ; then
	if [ ${DIR2} = Batch_6 ] ; then
		nbJobTask=18096330_${NBTASK}
	fi
	outdir=${DIR1}/${DIR2}/Misaligned_3mrad_plusX_Full/
	export OUT_DIR=${outdir}
	export NAME=${NAME_MIS}plusX_
	export NB_JOB_TASK=${nbJobTask}
	root -l -b -q "${macro_dir}/run_rich_event_display.C()"
elif [ $i -eq 4 ] ; then
	if [ ${DIR2} = Batch_6 ] ; then
		nbJobTask=18097140_${NBTASK}
	fi
	outdir=${DIR1}/${DIR2}/Misaligned_3mrad_plusY_Full/
	export OUT_DIR=${outdir}
	export NAME=${NAME_MIS}plusY_
	export NB_JOB_TASK=${nbJobTask}
	root -l -b -q "${macro_dir}/run_rich_event_display.C()"
fi

done
