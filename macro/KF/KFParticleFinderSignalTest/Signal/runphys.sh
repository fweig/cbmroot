#!/bin/sh
# Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Maxim Zyzak


rm -rf Efficiency.txt
root -l -b -q "physSignal.C("$1")"      2>&1 | tee phys.log

