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
  LOG(info) << "--------------  L1Algo parameters ---------------";
  LOG(info) << ToString();
}

//----------------------------------------------------------------------------------------------------------------------
//
std::string L1Parameters::ToString(int indentLevel) const
{
  std::stringstream aStream {};
  constexpr char indentChar = '\t';
  std::string indent(indentLevel, indentChar);
  aStream << '\n';
  aStream << indent << "COMPILE TIME CONSTANTS:\n";
  aStream << indent << indentChar << "Bits to code one station:           " << kStationBits << '\n';
  aStream << indent << indentChar << "Bits to code one thread:            " << kThreadBits << '\n';
  aStream << indent << indentChar << "Bits to code one triplet:           " << kTripletBits << '\n';
  aStream << indent << indentChar << "Max number of stations:             " << kMaxNstations << '\n';
  aStream << indent << indentChar << "Max number of threads:              " << kMaxNthreads << '\n';
  aStream << indent << indentChar << "Max number of triplets:             " << kMaxNtriplets << '\n';
  aStream << indent << "RUNTIME CONSTANTS:\n";
  aStream << indent << indentChar << "Max number of doublets per singlet: " << fMaxDoubletsPerSinglet << '\n';
  aStream << indent << indentChar << "Max number of triplets per doublet: " << fMaxTripletPerDoublets << '\n';
  aStream << indent << "CA TRACK FINDER ITERATIONS:\n";
  for (int idx = 0; idx < static_cast<int>(fCAIterationsContainer.size()); ++idx) {
    aStream << idx << ") " << fCAIterationsContainer[idx].ToString(indentLevel + 1) << '\n';
  }
  return aStream.str();
}
