#include "CbmDefs.h"

#include <Rtypes.h>

#include <gtest/gtest-spi.h>
#include <gtest/gtest.h>

TEST(_GTestCbmConstants, CheckParticleMasses) {
  Double_t protonMass   = 0.938272046;
  Double_t neutronMass  = 0.939565379;
  Double_t electronMass = 0.000510998928;

  EXPECT_EQ(protonMass, CbmProtonMass());
  EXPECT_EQ(neutronMass, CbmNeutronMass());
  EXPECT_EQ(electronMass, CbmElectronMass());
}
