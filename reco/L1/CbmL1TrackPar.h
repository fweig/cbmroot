/* Copyright (C) 2006-2017 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov [committer] */

#ifndef CbmL1TrackPar_H
#define CbmL1TrackPar_H

#include "CbmKFTrackInterface.h"

#include "L1TrackPar.h"

struct CbmL1TrackPar : public CbmKFTrackInterface {
public:
  CbmL1TrackPar() : chi2(0), NDF(0), mass(0), is_electron(0) {}
  double* GetTrack() override { return T; }
  double* GetCovMatrix() override { return C; }
  double& GetRefChi2() override { return chi2; }
  int& GetRefNDF() override { return NDF; }
  double GetMass() const override { return mass; }
  bool IsElectron() const override { return is_electron; }

  double T[L1TrackPar::kNparTr];
  double C[L1TrackPar::kNparCov];
  double chi2;
  int NDF;
  double mass;  // mass hypothesis
  bool is_electron;
};

#endif
