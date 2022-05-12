/* Copyright (C) 2016-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/***********************************************************************************************//**
 * @file   L1CAIteration.cxx
 * @brief  Definition of the L1CAIteration class methods
 * @since  05.02.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***************************************************************************************************/

#include "L1CAIteration.h"

#include <FairLogger.h>

#include <sstream>
//
//----------------------------------------------------------------------------------------------------------------------
//
L1CAIteration::L1CAIteration() noexcept { LOG(debug) << "L1CAIteration: Default constructor called for " << this; }
//
//----------------------------------------------------------------------------------------------------------------------
//
L1CAIteration::L1CAIteration(const L1CAIteration& other) noexcept
  // Basic fields
  : fName(other.fName)
  , fControlFlags(other.fControlFlags)
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
{
  LOG(debug) << "L1CAIteration: Copy constructor called: " << &other << " was copied into " << this;
}
//
//----------------------------------------------------------------------------------------------------------------------
//
L1CAIteration::L1CAIteration(L1CAIteration&& other) noexcept
{
  this->Swap(other);
  LOG(debug) << "L1CAIteration: Move constructor called: " << &other << " was moved into " << this;
}
//
//----------------------------------------------------------------------------------------------------------------------
//
L1CAIteration::L1CAIteration(const std::string& name) noexcept : fName(name)
{
  LOG(debug) << "L1CAIteration: Constructor from name called for " << this;
}
//
//----------------------------------------------------------------------------------------------------------------------
//
L1CAIteration::~L1CAIteration() noexcept { LOG(debug) << "L1CAIteration: Destructor called for " << this; }
//
//----------------------------------------------------------------------------------------------------------------------
//
L1CAIteration& L1CAIteration::operator=(const L1CAIteration& other) noexcept
{
  if (this != &other) { L1CAIteration(other).Swap(*this); }
  LOG(debug) << "L1CAIteration: Copy operator= called: " << &other << " was copied into " << this;
  return *this;
}
//
//----------------------------------------------------------------------------------------------------------------------
//
L1CAIteration& L1CAIteration::operator=(L1CAIteration&& other) noexcept
{
  if (this != &other) {
    L1CAIteration tmp(std::move(other));
    this->Swap(tmp);
  }
  LOG(debug) << "L1CAIteration: Move operator= called: " << &other << " was moved into " << this;
  return *this;
}
//
//----------------------------------------------------------------------------------------------------------------------
//
void L1CAIteration::Print(int verbosityLevel) const
{
  if (verbosityLevel == 0) { LOG(info) << "  - " << fName; }
  if (verbosityLevel > 0) { LOG(info) << ToString(0); }
}
//
//----------------------------------------------------------------------------------------------------------------------
//
void L1CAIteration::SetTargetPosSigmaXY(float sigmaX, float sigmaY)
{
  fTargetPosSigmaX = sigmaX;
  fTargetPosSigmaY = sigmaY;
}
//
//----------------------------------------------------------------------------------------------------------------------
//
void L1CAIteration::Swap(L1CAIteration& other) noexcept
{
  // Basic fields
  std::swap(fName, other.fName);
  std::swap(fControlFlags, other.fControlFlags);
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
}
//
//----------------------------------------------------------------------------------------------------------------------
//
std::string L1CAIteration::ToString(int indentLevel) const
{
  std::stringstream aStream {};
  constexpr char indentChar = '\t';
  std::string indent(indentLevel, indentChar);
  aStream << indent << "L1CAIteration: " << fName << '\n';
  aStream << indent << indentChar
          << "Is primary:                   " << fControlFlags[static_cast<int>(ControlFlag::kePrimary)] << '\n';
  aStream << indent << indentChar << "Track chi2 cut:               " << fTrackChi2Cut << '\n';
  aStream << indent << indentChar << "Triplet chi2 cut:             " << fTripletChi2Cut << '\n';
  aStream << indent << indentChar << "Doublet chi2 cut:             " << fDoubletChi2Cut << '\n';
  aStream << indent << indentChar << "Pick gather:                  " << fPickGather << '\n';
  aStream << indent << indentChar << "Pick neighbour:               " << fPickNeighbour << '\n';
  aStream << indent << indentChar << "Max invariant momentum:       " << fMaxInvMom << '\n';
  aStream << indent << indentChar << "Max slope at primary vertex:  " << fMaxSlopePV << '\n';
  aStream << indent << indentChar << "Max slope:                    " << fMaxSlope << '\n';
  aStream << indent << indentChar << "Max DZ:                       " << fMaxDZ << '\n';
  aStream << indent << indentChar << "Target position sigma X [cm]: " << fTargetPosSigmaX << '\n';
  aStream << indent << indentChar << "Target position sigma Y [cm]: " << fTargetPosSigmaY << '\n';
  aStream << indent << indentChar << "Min level for triplet start:  " << fMinLevelTripletStart;

  return aStream.str();
}
