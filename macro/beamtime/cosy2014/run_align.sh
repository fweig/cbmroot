#!/bin/bash
# Copyright (C) 2015 Facility for Antiproton and Ion Research in Europe, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Pierre-Alain Loizeau


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
