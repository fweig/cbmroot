/* Copyright (C) 2016-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

#include "L1CAIteration.h"

#include <FairLogger.h>

L1CAIteration::L1CAIteration(const std::string& name, L1CAIterationType type) : fName(name), fType(type)
{
  this->Print();
}

void L1CAIteration::Print() const
{
  LOG(INFO) << "CA Track Finder Iteration: " << fName;
  LOG(INFO) << "\t\t-type" << static_cast<int>(fType);
  LOG(INFO) << "\tTrack cuts:";
  LOG(INFO) << "\t\t- chi2 cut" << fTrackChi2Cut;
  LOG(INFO) << "\tTriplet cuts:";
  LOG(INFO) << "\t\t- chi2 cut" << fTripletChi2Cut;
  LOG(INFO) << "\tDoublet cuts:";
  LOG(INFO) << "\t\t- chi2 cut" << fDoubletChi2Cut;
}
