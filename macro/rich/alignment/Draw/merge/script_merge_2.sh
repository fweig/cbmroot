#!/bin/bash
# Copyright (C) 2019 Justus-Liebig-Universitaet Giessen, Giessen
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Jordan Bendarouach


echo ${MERGE_ROOT}
echo ${INDIR}

hadd -T ${MERGE_ROOT} ${INDIR}
