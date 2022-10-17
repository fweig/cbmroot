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

#include <limits>
#include <sstream>

#include "L1Constants.h"

using namespace L1Constants::size;

// ---------------------------------------------------------------------------------------------------------------------
//
L1CAIteration::L1CAIteration(const std::string& name) : fName(name) {}

// ---------------------------------------------------------------------------------------------------------------------
//
bool L1CAIteration::Check() const
{
  constexpr float kMaxFloat = std::numeric_limits<float>::max();
  bool res                  = true;
  // TODO: SZh 06.10.2022: These values should be tuned. At the moment the std::numeric_limits<T>::max value is used for
  //                       debug purposes. In future, these values will be strengthened.
  res = this->CheckValueLimits<float>("track_chi2_cut", fTrackChi2Cut, 0.f, kMaxFloat) && res;
  res = this->CheckValueLimits<float>("triplet_chi2_cut", fTripletChi2Cut, 0.f, kMaxFloat) && res;
  res = this->CheckValueLimits<float>("doublet_chi2_cut", fDoubletChi2Cut, 0.f, kMaxFloat) && res;
  res = this->CheckValueLimits<float>("pick_gather", fPickGather, 0.f, kMaxFloat) && res;
  res = this->CheckValueLimits<float>("triplet_link_chi2", fTripletLinkChi2, 0.f, kMaxFloat) && res;
  res = this->CheckValueLimits<float>("min_momentum", fMaxInvMom, 1.f / kMaxFloat, 1.f / 0.001f) && res;
  res = this->CheckValueLimits<float>("max_slope_pv", fMaxSlopePV, 0.f, kMaxFloat) && res;
  res = this->CheckValueLimits<float>("max_slope", fMaxSlope, 0.f, kMaxFloat) && res;
  res = this->CheckValueLimits<float>("max_dz", fMaxDZ, 0.f, kMaxFloat) && res;
  res = this->CheckValueLimits<int>("min_n_hits", fMinNhits, 3, kMaxNstations) && res;
  res = this->CheckValueLimits<int>("min_n_hits_sta_0", fMinNhitsStation0, 3, kMaxNstations) && res;
  res = this->CheckValueLimits<int>("first_station_index", fFirstStationIndex, 0, kMaxNstations) && res;
  res = this->CheckValueLimits<float>("target_pos_sigma_x", fTargetPosSigmaX, 0.f, kMaxFloat) && res;
  res = this->CheckValueLimits<float>("target_pos_sigma_y", fTargetPosSigmaY, 0.f, kMaxFloat) && res;
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
std::string L1CAIteration::ToString(int indentLevel) const
{
  std::stringstream aStream {};
  constexpr char indCh = '\t';
  std::string indent(indentLevel, indCh);
  aStream << indent << "\033[1;30m" << fName << "\033[0m\n";
  aStream << indent << indCh << "Is primary:                             " << fIsPrimary << '\n';
  aStream << indent << indCh << "Is electron:                            " << fIsElectron << '\n';
  aStream << indent << indCh << "Are tracks created from triplets:       " << fIsTrackFromTriplets << '\n';
  aStream << indent << indCh << "Are tracks extended with unused hits :  " << fIfExtendTracks << '\n';
  aStream << indent << indCh << "Are hits skip in triplets building:     " << fIfJumped << '\n';
  aStream << indent << indCh << "Min n hits :                            " << fMinNhits << '\n';
  aStream << indent << indCh << "Min n hits for trscs at station 0:      " << fMinNhitsStation0 << '\n';
  aStream << indent << indCh << "Track chi2 cut:                         " << fTrackChi2Cut << '\n';
  aStream << indent << indCh << "Triplet chi2 cut:                       " << fTripletChi2Cut << '\n';
  aStream << indent << indCh << "Doublet chi2 cut:                       " << fDoubletChi2Cut << '\n';
  aStream << indent << indCh << "Pick gather:                            " << fPickGather << '\n';
  aStream << indent << indCh << "Triplet link chi2:                      " << fTripletLinkChi2 << '\n';
  aStream << indent << indCh << "Max inverse momentum:                   " << fMaxInvMom << '\n';
  aStream << indent << indCh << "Max slope at primary vertex:            " << fMaxSlopePV << '\n';
  aStream << indent << indCh << "Max slope:                              " << fMaxSlope << '\n';
  aStream << indent << indCh << "Max DZ:                                 " << fMaxDZ << '\n';
  aStream << indent << indCh << "Target position sigma X [cm]:           " << fTargetPosSigmaX << '\n';
  aStream << indent << indCh << "Target position sigma Y [cm]:           " << fTargetPosSigmaY << '\n';
  aStream << indent << indCh << "First tracking station index:           " << fFirstStationIndex;

  return aStream.str();
}
