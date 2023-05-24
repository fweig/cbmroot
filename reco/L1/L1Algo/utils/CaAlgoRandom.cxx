/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   CaAlgoRandom.cxx
/// @brief  Random generator utility class for CA tracking (implementation)
/// @since  16.05.2023
/// @author Sergei Zharko <s.zharko@gsi.de>

#include "CaAlgoRandom.h"

using ca::algo::Random;


// ---------------------------------------------------------------------------------------------------------------------
//
Random::Random() : Random(1) {}

// ---------------------------------------------------------------------------------------------------------------------
//
Random::Random(int seed) { this->SetSeed(seed); }

// ---------------------------------------------------------------------------------------------------------------------
//
void Random::SetSeed(int seed)
{
  if (seed) { fSeed = seed; }
  else {
    std::random_device rd;
    unsigned int randomDeviceSeed = rd();
    fSeed                         = randomDeviceSeed;
  }
  fGenerator.seed(fSeed);
}
