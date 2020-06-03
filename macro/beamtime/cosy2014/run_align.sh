#!/bin/bash

for f in 106
do

for i in 0
do
root -l -n << EOF
.L alignment.C
alignment($f,$i)
.q
EOF

done
done