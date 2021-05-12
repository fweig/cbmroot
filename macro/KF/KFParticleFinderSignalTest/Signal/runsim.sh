#!/bin/sh
# Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Maxim Zyzak


rm -rf *.root *.log *.dat *.par
root -l -b -q "SignalGenerator.C("$1","$2")"      2>&1 | tee gen.log
root -l -b -q "simSignal.C("$1","$2")"      2>&1 | tee sim.log
root -l -b -q "recoSignal.C("$2")"      2>&1 | tee rec.log

