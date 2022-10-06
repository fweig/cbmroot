/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/***************************************************************************************************
 * @file   L1CAIteration.cxx
 * @brief  Definition of the L1CAIteration class methods
 * @since  05.02.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***************************************************************************************************/

#include "L1CAIteration.h"

#include <sstream>

#include "L1Constants.h"

using namespace L1Constants::size;


// ---------------------------------------------------------------------------------------------------------------------
//
L1CAIteration::L1CAIteration() noexcept { LOG(debug) << "L1CAIteration: Default constructor called for " << this; }

// ---------------------------------------------------------------------------------------------------------------------
//
L1CAIteration::L1CAIteration(const L1CAIteration& other) noexcept
  // Basic fields
  : fName(other.fName)
  // Cuts
  , fTrackChi2Cut(other.fTrackChi2Cut)
  , fTripletChi2Cut(other.fTripletChi2Cut)
  , fDoubletChi2Cut(other.fDoubletChi2Cut)
  , fPickGather(other.fPickGather)
  , fPickNeighbour(other.fPickNeighbour)
  , fMaxInvMom(other.fMaxInvMom)
  , fMaxSlopePV(other.fMaxSlopePV)
  , fMaxSlope(other.fMaxSlope)
  , fMaxDZ(other.fMaxDZ)
  , fTargetPosSigmaX(other.fTargetPosSigmaX)
  , fTargetPosSigmaY(other.fTargetPosSigmaY)
  , fMinLevelTripletStart(other.fMinLevelTripletStart)
  , fFirstStationIndex(other.fFirstStationIndex)
  , fIsPrimary(other.fIsPrimary)
  , fIsElectron(other.fIsElectron)
  , fIsTrackFromTriplets(other.fIsTrackFromTriplets)
  , fIfExtendTracks(other.fIfExtendTracks)
  , fIfJumped(other.fIfJumped)
  , fIfSuppressGhost(other.fIfSuppressGhost)
{
}

// ---------------------------------------------------------------------------------------------------------------------
//
L1CAIteration::L1CAIteration(L1CAIteration&& other) noexcept
{
  this->Swap(other);
}

// ---------------------------------------------------------------------------------------------------------------------
//
L1CAIteration::L1CAIteration(const std::string& name) noexcept : fName(name) {}

// ---------------------------------------------------------------------------------------------------------------------
//
L1CAIteration::~L1CAIteration() noexcept {}

// ---------------------------------------------------------------------------------------------------------------------
//
L1CAIteration& L1CAIteration::operator=(const L1CAIteration& other) noexcept
{
  if (this != &other) { L1CAIteration(other).Swap(*this); }
  return *this;
}

// ---------------------------------------------------------------------------------------------------------------------
//
L1CAIteration& L1CAIteration::operator=(L1CAIteration&& other) noexcept
{
  if (this != &other) {
    L1CAIteration tmp(std::move(other));
    this->Swap(tmp);
  }
  return *this;
}

// ---------------------------------------------------------------------------------------------------------------------
//
bool L1CAIteration::Check() const
{
  bool res = true;
  // TODO: SZh 06.10.2022: These values should be tuned
  res = this->CheckValueLimits<float>("track_chi2_cut", fTrackChi2Cut, 0.f, 12.f) && res;
  res = this->CheckValueLimits<float>("triplet_chi2_cut", fTripletChi2Cut, 0.f, 30.f) && res;
  res = this->CheckValueLimits<float>("doublet_chi2_cut", fDoubletChi2Cut, 0.f, 15.f) && res;
  res = this->CheckValueLimits<float>("pick_gather", fPickGather, 2.f, 5.f) && res;
  res = this->CheckValueLimits<float>("pick_neighbour", fPickNeighbour, 2.f, 6.f) && res;
  res = this->CheckValueLimits<float>("min_momentum", fMaxInvMom, 1.f / 1.f, 1.f / 0.05f) && res;
  res = this->CheckValueLimits<float>("max_slope_pv", fMaxSlopePV, 0.1f, 2.f) && res;
  res = this->CheckValueLimits<float>("max_slope", fMaxSlope, 1.f, 3.f) && res;
  res = this->CheckValueLimits<float>("max_dz", fMaxDZ, 0.f, 1.0f) && res;
  res = this->CheckValueLimits<int>("min_start_triplet_lvl", fMinLevelTripletStart, 0, 5) && res;
  res = this->CheckValueLimits<int>("first_station_index", fFirstStationIndex, 0, kMaxNstations) && res;
  res = this->CheckValueLimits<float>("target_pos_sigma_x", fTargetPosSigmaX, 0.01f, 15.f) && res;
  res = this->CheckValueLimits<float>("target_pos_sigma_y", fTargetPosSigmaY, 0.01f, 15.f) && res;
  return res;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void L1CAIteration::Print(int verbosityLevel) const
{
  if (verbosityLevel == 0) { LOG(info) << "  - " << fName; }
  if (verbosityLevel > 0) { LOG(info) << ToString(0); }
}

// ---------------------------------------------------------------------------------------------------------------------
//
void L1CAIteration::SetTargetPosSigmaXY(float sigmaX, float sigmaY)
{
  fTargetPosSigmaX = sigmaX;
  fTargetPosSigmaY = sigmaY;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void L1CAIteration::Swap(L1CAIteration& other) noexcept
{
  // Basic fields
  std::swap(fName, other.fName);
  // Cuts
  std::swap(fTrackChi2Cut, other.fTrackChi2Cut);
  std::swap(fTripletChi2Cut, other.fTripletChi2Cut);
  std::swap(fDoubletChi2Cut, other.fDoubletChi2Cut);
  std::swap(fPickGather, other.fPickGather);
  std::swap(fPickNeighbour, other.fPickNeighbour);
  std::swap(fMaxInvMom, other.fMaxInvMom);
  std::swap(fMaxSlopePV, other.fMaxSlopePV);
  std::swap(fMaxSlope, other.fMaxSlope);
  std::swap(fMaxDZ, other.fMaxDZ);
  std::swap(fTargetPosSigmaX, other.fTargetPosSigmaX);
  std::swap(fTargetPosSigmaY, other.fTargetPosSigmaY);
  std::swap(fMinLevelTripletStart, other.fMinLevelTripletStart);
  std::swap(fFirstStationIndex, other.fFirstStationIndex);
  std::swap(fIsPrimary, other.fIsPrimary);
  std::swap(fIsElectron, other.fIsElectron);
  std::swap(fIsTrackFromTriplets, other.fIsTrackFromTriplets);
  std::swap(fIfExtendTracks, other.fIfExtendTracks);
  std::swap(fIfJumped, other.fIfJumped);
  std::swap(fIfSuppressGhost, other.fIfSuppressGhost);
}

// ---------------------------------------------------------------------------------------------------------------------
//
std::string L1CAIteration::ToString(int indentLevel) const
{
  std::stringstream aStream {};
  constexpr char indCh = '\t';
  std::string indent(indentLevel, indCh);
  aStream << indent << "L1CAIteration: " << fName << '\n';
  aStream << indent << indCh << "Is primary:                             " << fIsPrimary << '\n';
  aStream << indent << indCh << "Is electron:                            " << fIsElectron << '\n';
  aStream << indent << indCh << "Are tracks created from triplets:       " << fIsTrackFromTriplets << '\n';
  aStream << indent << indCh << "Are tracks extended with unused hits :  " << fIfExtendTracks << '\n';
  aStream << indent << indCh << "Are hits skip in triplets building:     " << fIfJumped << '\n';
  aStream << indent << indCh << "Ghost suppression in tracks cand. selection :" << fIfSuppressGhost << '\n';
  aStream << indent << indCh << "Track chi2 cut:                         " << fTrackChi2Cut << '\n';
  aStream << indent << indCh << "Triplet chi2 cut:                       " << fTripletChi2Cut << '\n';
  aStream << indent << indCh << "Doublet chi2 cut:                       " << fDoubletChi2Cut << '\n';
  aStream << indent << indCh << "Pick gather:                            " << fPickGather << '\n';
  aStream << indent << indCh << "Pick neighbour:                         " << fPickNeighbour << '\n';
  aStream << indent << indCh << "Max inverse momentum:                   " << fMaxInvMom << '\n';
  aStream << indent << indCh << "Max slope at primary vertex:            " << fMaxSlopePV << '\n';
  aStream << indent << indCh << "Max slope:                              " << fMaxSlope << '\n';
  aStream << indent << indCh << "Max DZ:                                 " << fMaxDZ << '\n';
  aStream << indent << indCh << "Target position sigma X [cm]:           " << fTargetPosSigmaX << '\n';
  aStream << indent << indCh << "Target position sigma Y [cm]:           " << fTargetPosSigmaY << '\n';
  aStream << indent << indCh << "Min level for triplet start:            " << fMinLevelTripletStart << '\n';
  aStream << indent << indCh << "First tracking station index:           " << fFirstStationIndex;

  return aStream.str();
}
