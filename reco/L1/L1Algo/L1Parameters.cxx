/* Copyright (C) 2016-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/************************************************************************************************************
 * @file L1Parameters.cxx
 * @brief Parameter container for the L1Algo library
 * @since 10.02.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***********************************************************************************************************/
#include "L1Parameters.h"

#include <FairLogger.h>

//------------------------------------------------------------------------------------------------------------------------------------
//
L1Parameters::L1Parameters() {}

//------------------------------------------------------------------------------------------------------------------------------------
//
L1Parameters::~L1Parameters() noexcept {}

//------------------------------------------------------------------------------------------------------------------------------------
//
L1Parameters::L1Parameters(const L1Parameters& other) noexcept
: fMaxDoubletsPerSinglet(other.fMaxDoubletsPerSinglet),
  fMaxTripletPerDoublets(other.fMaxTripletPerDoublets),
  fCAIterations(other.fCAIterations),
  fTargetPos(other.fTargetPos),
  fVertexFieldValue(other.fVertexFieldValue),
  fVertexFieldRegion(other.fVertexFieldRegion),
  fStations(other.fStations),
  fThickMap(other.fThickMap),
  fNstationsActive(other.fNstationsActive),
  fNstationsGeometry(other.fNstationsGeometry),
  fActiveStationGlobalIDs(other.fActiveStationGlobalIDs)
{}

//------------------------------------------------------------------------------------------------------------------------------------
//
L1Parameters& L1Parameters::operator=(const L1Parameters& other) noexcept
{
  if (this != &other) { L1Parameters(other).Swap(*this); }
  return *this;
}

//------------------------------------------------------------------------------------------------------------------------------------
//
L1Parameters::L1Parameters(L1Parameters&& other) noexcept
{
  this->Swap(other);
}

//------------------------------------------------------------------------------------------------------------------------------------
//
L1Parameters& L1Parameters::operator=(L1Parameters&& other) noexcept
{
  if (this != &other) {
    L1Parameters tmp(std::move(other));
    this->Swap(tmp);
  }
  return *this;
}

//------------------------------------------------------------------------------------------------------------------------------------
//
void L1Parameters::Swap(L1Parameters& other) noexcept
{
  std::swap(fMaxDoubletsPerSinglet, other.fMaxDoubletsPerSinglet);
  std::swap(fMaxTripletPerDoublets, other.fMaxTripletPerDoublets);
  std::swap(fCAIterations, other.fCAIterations);
  std::swap(fTargetPos, other.fTargetPos);
  std::swap(fVertexFieldValue, other.fVertexFieldValue);
  std::swap(fVertexFieldRegion, other.fVertexFieldRegion);
  std::swap(fStations, other.fStations);
  std::swap(fThickMap, other.fThickMap);
  std::swap(fNstationsActive, other.fNstationsActive);
  std::swap(fNstationsGeometry, other.fNstationsGeometry);
  std::swap(fActiveStationGlobalIDs, other.fActiveStationGlobalIDs);

  //for (int i = 0; i < int(L1Constants::size::kMaxNstations); ++i) {
  //  int tmp = fActiveStationGlobalIDs[i];
  //  fActiveStationGlobalIDs[i] = other.fActiveStationGlobalIDs[i];
  //  other.fActiveStationGlobalIDs[i] = tmp;
  //}

  //for (int i = 0; i < int(L1Constants::size::kMaxNdetectors + 1); ++i) {
  //  int tmp = fNstationsGeometry[i];
  //  fNstationsGeometry[i] = other.fNstationsGeometry[i];
  //  other.fNstationsGeometry[i] = tmp;
  //}

  //for (int i = 0; i < int(L1Constants::size::kMaxNdetectors + 1); ++i) {
  //  int tmp = fNstationsActive[i];
  //  fNstationsActive[i] = other.fNstationsActive[i];
  //  other.fNstationsActive[i] = tmp;
  //}
}

//------------------------------------------------------------------------------------------------------------------------------------
//
void L1Parameters::Print(int /*verbosityLevel*/) const
{
  LOG(info) << "--------------  L1Algo parameters ---------------";
  LOG(info) << ToString();
}

//------------------------------------------------------------------------------------------------------------------------------------
//
std::string L1Parameters::ToString(int verbosity, int indentLevel) const
{
  std::stringstream aStream {};
  constexpr char indentChar = '\t';
  std::string indent(indentLevel, indentChar);
  aStream << '\n';
  aStream << indent << "COMPILE TIME CONSTANTS:\n";
  aStream << indent << indentChar << "Bits to code one station:           " <<L1Constants::size::kStationBits << '\n';
  aStream << indent << indentChar << "Bits to code one thread:            " <<L1Constants::size::kThreadBits << '\n';
  aStream << indent << indentChar << "Bits to code one triplet:           " <<L1Constants::size::kTripletBits << '\n';
  aStream << indent << indentChar << "Max number of stations:             " <<L1Constants::size::kMaxNstations << '\n';
  aStream << indent << indentChar << "Max number of threads:              " <<L1Constants::size::kMaxNthreads << '\n';
  aStream << indent << indentChar << "Max number of triplets:             " <<L1Constants::size::kMaxNtriplets << '\n';
  aStream << indent << "RUNTIME CONSTANTS:\n";
  aStream << indent << indentChar << "Max number of doublets per singlet: " << fMaxDoubletsPerSinglet << '\n';
  aStream << indent << indentChar << "Max number of triplets per doublet: " << fMaxTripletPerDoublets << '\n';
  aStream << indent << "CA TRACK FINDER ITERATIONS:\n";
  for (int idx = 0; idx < static_cast<int>(fCAIterations.size()); ++idx) {
    aStream << idx << ") " << fCAIterations[idx].ToString(indentLevel + 1) << '\n';
  }
  aStream << indent << "GEOMETRY:\n";
  
  aStream << indent << indentChar << "TARGET:\n";
  aStream << indent << indentChar << indentChar << "Position (x [cm], y [cm], z[cm]): ";
  for (int dim = 0; dim < 3 /*nDimensions*/; ++dim ) {
    aStream << std::setw(12) << std::setfill(' ') << fTargetPos[dim][0] << ' ';
  }
  aStream << '\n';
  
  aStream << indent << indentChar << "NUMBER OF STATIONS:\n";
  aStream << indent << indentChar << indentChar << "Number of stations (Geometry): ";
  for (int idx = 0; idx < int(fNstationsGeometry.size()); ++idx ) {
    if (int(fNstationsGeometry.size() - 2) == idx) { aStream << " | total = "; }
    aStream << std::setw(2) << std::setfill(' ') << fNstationsGeometry[idx] << ' ';
  }
  aStream << '\n';
  aStream << indent << indentChar << indentChar << "Number of stations (Active): ";
  for (int idx = 0; idx < int(fNstationsActive.size()); ++idx ) {
    if (int(fNstationsActive.size() - 2) == idx) { aStream << " | total = "; }
    aStream << std::setw(2) << std::setfill(' ') << fNstationsActive[idx] << ' ';
  }
  aStream << '\n';
  aStream << indent << indentChar << indentChar << "Active station index: ";
  for (int idx = 0; idx < *(fNstationsActive.end() - 1); ++idx ) {
    aStream << std::setw(3) << std::setfill(' ') << fActiveStationGlobalIDs[idx] << ' ';
  }
  aStream << '\n';
  
  aStream << indent << indentChar << "STATIONS:\n ";
  for (int idx = 0; idx < *(fNstationsActive.end() - 1); ++idx) {
    aStream << indent << indentChar << indentChar << fStations[idx].ToString(verbosity) << '\n';
  }

  return aStream.str();
}
