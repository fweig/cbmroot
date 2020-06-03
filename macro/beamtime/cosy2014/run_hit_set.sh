#!/bin/bash

for f in 106
do

for i in 0
do
root -l -n << EOF
.L hit_set.C
hit_set($f,$i)
.q
EOF

done
done