#!/bin/bash
# Copyright (C) 2015 Institut fuer Kernphysik, Westfaelische Wilhelms-Universitaet Muenster, Muenster
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Philipp Kähler


FILE=1

while [ $FILE -le 141 ]
do
sed -n ${FILE}p /mount/scr1/p_kaeh01/hv/allnames.config > /mount/scr1/p_kaeh01/hv/tempname.config
root -b -q monHVlong.C++
let FILE=$FILE+1
done
