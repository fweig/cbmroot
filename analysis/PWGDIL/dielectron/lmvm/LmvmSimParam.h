/* Copyright (C) 2021 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev [committer] */

#ifndef LMVM_SIM_PARAM_H
#define LMVM_SIM_PARAM_H

#include "Logger.h"

#include "TObject.h"

#include <string>

class LmvmSimParam {
public:
  static Double_t GetWeight(const std::string& energy, const std::string& particle)
  {
    // TODO: check all weights for all energies and particles
    // Weight = Multiplicity * Branching Ratio
    if (energy == "8gev" || energy == "12gev") {
      // TODO: add weights for 12geV
      if (particle == "omegaepem" || particle == "w") return 2.28721 * 7.36e-5;
      if (particle == "omegadalitz" || particle == "wdalitz") return 2.28721 * 7.7e-4;
      if (particle == "phi") return 0.311619 * 2.97e-4;
      if (particle == "inmed" || particle == "rho0") return 0.0304706 * 4.72e-5;
      if (particle == "qgp" || particle == "qgp_epem") return 4.52941e-4 * 1.15e-2;  // TODO: check BR
    }
    else if (energy == "25gev") {
      if (particle == "rho0") return 23 * 4.7e-5;
      if (particle == "omegaepem" || particle == "w") return 38 * 7.28e-5;
      if (particle == "omegadalitz" || particle == "wdalitz") return 38 * 7.7e-4;
      if (particle == "phi") return 1.28 * 2.97e-4;
      if (particle == "inmed") return 4.45e-2;
      if (particle == "qgp" || particle == "qgp_epem") return 1.15e-2;
    }
    else if (energy == "3.5gev") {
      if (particle == "rho0") return 1.0 * 4.7e-5;
      if (particle == "omegaepem" || particle == "w") return 1.2 * 7.28e-5;
      if (particle == "omegadalitz" || particle == "wdalitz") return 1.2 * 7.7e-5;
      if (particle == "phi") return 0.1 * 2.97e-4;
    }
    else if (energy == "4.5gev") {
      if (particle == "omegadalitz" || particle == "wdalitz") return 1.2 * 7.7e-5;
      if (particle == "omegaepem" || particle == "w") return 1.2 * 7.28e-6;
      if (particle == "phi") return 1.2 * 2.97e-6;
      if (particle == "inmed") return 2.4 * 10e-3;
    }
    else {
      LOG(fatal) << "LmvmSimParam::SetEnergyAndParticle energy or particle is not correct, energy:" << energy
                 << " particle:" << particle;
      return 0.;
    }
    return 0.;
  }
};


#endif
