#!/bin/bash
# Copyright (C) 2019 Justus-Liebig-Universitaet Giessen, Giessen
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Jordan Bendarouach


nEvts=5000

for i in `seq 2 4`;
do
	./simulation.sh $i $nEvts
done
