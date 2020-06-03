#!/bin/bash

nEvs=5000;

for TILE in 1_4 2_8
do

	for AXIS in x y xy
	do

		for (( j=0; j<2; j++ ))
		do

			for (( i=1; i<10; i++ ))
			do

		#		f=$j.$i;
		#		echo $f;

				root -b -q -l "run_mirror_sim_test.C($nEvs, \"$TILE\", \"$AXIS\", \"$j.$i\")"
				root -b -q -l "run_mirror_reco_alignment_test.C($nEvs, \"$TILE\", \"$AXIS\", \"$j.$i\")"
				root -b -q -l "run_mirror_reco_correction_test.C($nEvs, \"$TILE\", \"$AXIS\", \"$j.$i\")"

			done

		done

		for (( j=1; j<6; j++ ))
		do

			root -b -q -l "run_mirror_sim_test.C($nEvs, \"$TILE\", \"$AXIS\", \"$j\")"
			root -b -q -l "run_mirror_reco_alignment_test.C($nEvs, \"$TILE\", \"$AXIS\", \"$j\")"
			root -b -q -l "run_mirror_reco_correction_test.C($nEvs, \"$TILE\", \"$AXIS\", \"$j\")"

		done

	done

done
