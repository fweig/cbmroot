#!/bin/sh
# Copyright (C) 2012 GSI/JINR-LIT
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Andrey Lebedev


DIR=/lustre/cbm/user/andrey/events/nu/25gev/

OUTPUTFILE=$DIR/analysis.all.root
INPUTFILES=$DIR/analysis.0*.root

hadd  -T -f $OUTPUTFILE $INPUTFILES
