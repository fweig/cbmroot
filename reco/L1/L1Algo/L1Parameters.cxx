/* Copyright (C) 2016-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/************************************************************************************************************
 * @file L1Parameters.cxx
 * @brief Parameter container for the L1Algo library
 * @since 10.02.2022
 ***********************************************************************************************************/
#include "L1Parameters.h"

#include <FairLogger.h>

void L1Parameters::Print(int verbosityLevel) const
{
  LOG(info) << "== L1Algo parameters ==============================================================";
  LOG(info) << "";
  LOG(info) << "  COMPILE TIME CONSTANTS";
  LOG(info) << "    Bits to code one station:          " << kStationBits;
  LOG(info) << "    Bits to code one thread:           " << kThreadBits;
  LOG(info) << "    Bits to code one triplet:          " << kTripletBits;
  LOG(info) << "    Max number of stations:            " << kMaxNstations;
  LOG(info) << "    Max number of threads:             " << kMaxNthreads;
  LOG(info) << "    Max number of triplets:            " << kMaxNtriplets;
  LOG(info) << "";
  LOG(info) << "  RUNTIME CONSTANTS (CUTS)";
  LOG(info) << "    Max number of doublets per singlet: " << fMaxDoubletsPerSinglet;
  LOG(info) << "    Max number of triplets per doublet: " << fMaxTripletPerDoublets;
  LOG(info) << "";
  LOG(info) << "  TRACK FINDER ITERATIONS";
  for (const auto& iteration : fCAIterationsContainer) {
    iteration.Print(verbosityLevel);
  }
  LOG(info) << "===================================================================================";
}
