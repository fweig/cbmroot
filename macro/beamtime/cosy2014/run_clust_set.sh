#!/bin/bash

for f in 106
do

for i in 0
do
root -l -n << EOF
.L cluster_set.C
cluster_set($f,$i)
.q
EOF

done
done