#!/bin/bash
# Copyright (C) 2010 Petersburg Nuclear Physics Institute named by B.P.Konstantinov of National Research Centre "Kurchatov Institute", Gatchina
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Evgeny Kryshen


root -l -b -q 'eff_draw.C("la.ef.root")'
root -l -b -q 'eff_draw.C("xi.ef.root")'
root -l -b -q 'eff_draw.C("om.ef.root")'
