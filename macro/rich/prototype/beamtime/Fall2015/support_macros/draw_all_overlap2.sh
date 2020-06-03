#!/bin/bash

FILE_WLS_OFF=output/sum_WLS_off.root
FILE_WLS_ON=output/sum_WLS_on.root

tdcs=(10 11 12 13 20 21 22 23 50 51 52 53 60 61 62 63)


for (( i=0; i<16; i++ )); do

	j=i
	for (( ch1=0; ch1<16; ch1++ )); do
		for (( ch2=ch1; ch2<16; ch2++ )); do
			#echo TDC_${tdcs[$i]}_ch${ch1}_TDC${tdcs[$j]}_ch${ch2}
			root -l -b -q "draw_overlap.C(\"${FILE_WLS_OFF}\", \"${FILE_WLS_ON}\", \"${tdcs[$i]}\", \"${tdcs[$j]}\", ${ch1}, ${ch2})"
		done
	done

	for (( j=i+1; j<16; j++ )); do
		for (( ch1=0; ch1<16; ch1++ )); do
			for (( ch2=0; ch2<16; ch2++ )); do
				#echo TDC_${tdcs[$i]}_ch${ch1}_TDC${tdcs[$j]}_ch${ch2}
   				root -l -b -q "draw_overlap.C(\"${FILE_WLS_OFF}\", \"${FILE_WLS_ON}\", \"${tdcs[$i]}\", \"${tdcs[$j]}\", ${ch1}, ${ch2})"
			done
		done
	done

done
