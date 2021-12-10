/* Copyright (C) 2016-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#include "CbmDefs.h"

#include <Rtypes.h>

#include <gtest/gtest-spi.h>
#include <gtest/gtest.h>

TEST(_GTestCbmConstants, CheckParticleMasses)
{
  double protonMass   = 0.938272046;
  double neutronMass  = 0.939565379;
  double electronMass = 0.000510998928;

  EXPECT_EQ(protonMass, CbmProtonMass());
  EXPECT_EQ(neutronMass, CbmNeutronMass());
  EXPECT_EQ(electronMass, CbmElectronMass());
}
