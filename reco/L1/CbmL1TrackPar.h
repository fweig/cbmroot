/** @file CbmL1TrackPar.h
  * @copyright Copyright (C) 2006-2017 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Denis Bertini [committer] **/

#ifndef CbmL1TrackPar_H
#define CbmL1TrackPar_H

#include "CbmKFTrackInterface.h"

struct CbmL1TrackPar : public CbmKFTrackInterface {
public:
  CbmL1TrackPar() : chi2(0), NDF(0), mass(0), is_electron(0) {}
  double* GetTrack() { return T; }
  double* GetCovMatrix() { return C; }
  double& GetRefChi2() { return chi2; }
  int& GetRefNDF() { return NDF; }
  double GetMass() { return mass; }
  bool IsElectron() { return is_electron; }

  double T[7], C[21], chi2;
  int NDF;
  double mass;  // mass hypothesis
  bool is_electron;
};

#endif
