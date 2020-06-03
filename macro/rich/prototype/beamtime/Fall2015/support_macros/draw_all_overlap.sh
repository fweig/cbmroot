#!/bin/bash

FILE_WLS_OFF=output/sum_WLS_off.root
FILE_WLS_ON=output/sum_WLS_on.root

tdcJs=(1 2 3 4 5 6 7 8 9 a b c d e 10)

for tdcJ in ${tdcJs[*]}; do
	for tdcI in {0..3}; do
		for (( i=0; i<=15; i++ )); do
			for (( j=$i; j<=15; j++ )); do
				#echo TDC${tdcJ}${tdcI}_${i}_${j}
				root -l -b -q "draw_overlap.C(\"${FILE_WLS_OFF}\", \"${FILE_WLS_ON}\", \"${tdcJ}${tdcI}\", ${i}, ${j})"
			done
		done
	done
done
