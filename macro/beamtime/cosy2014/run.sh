#!/bin/bash

for f in 106
do

for i in 0 #1 2 3 4 5 6
do
root -l -n << EOF
.L readLmd_calibr.C
readLmd_calibr($f,$i)
.q
EOF

done
done