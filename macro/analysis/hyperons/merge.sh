#!/bin/bash
# Copyright (C) 2010 Petersburg Nuclear Physics Institute named by B.P.Konstantinov of National Research Centre "Kurchatov Institute", Gatchina
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Evgeny Kryshen


hadd -f -T la.ef.root 0000/la.ef.root 0001/la.ef.root
hadd -f -T xi.ef.root 0000/xi.ef.root 0001/xi.ef.root
hadd -f -T om.ef.root 0000/om.ef.root 0001/om.ef.root
hadd -f -T la.histo.root 0000/la.histo.root 0001/la.histo.root
#hadd -f -T xi.histo.root 0000/xi.histo.root 0001/xi.histo.root
hadd -f -T om.histo.root 0000/om.histo.root 0001/om.histo.root
