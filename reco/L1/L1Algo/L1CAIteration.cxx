/* Copyright (C) 2016-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

#include "L1CAIteration.h"

#include <FairLogger.h>

//----------------------------------------------------------------------------------------------------------------------
//
L1CAIteration::L1CAIteration() noexcept { LOG(debug) << "L1CAIteration: Default constructor called for " << this; }

//----------------------------------------------------------------------------------------------------------------------
//
L1CAIteration::~L1CAIteration() noexcept { LOG(debug) << "L1CAIteration: Destructor called for " << this; }

//----------------------------------------------------------------------------------------------------------------------
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
{
  LOG(debug) << "L1CAIteration: Copy constructor called: " << &other << " was copied into " << this;
}

//----------------------------------------------------------------------------------------------------------------------
//
L1CAIteration::L1CAIteration(L1CAIteration&& other) noexcept
{
  this->Swap(other);
  LOG(debug) << "L1CAIteration: Move constructor called: " << &other << " was moved into " << this;
}

//----------------------------------------------------------------------------------------------------------------------
//
L1CAIteration& L1CAIteration::operator=(const L1CAIteration& other) noexcept
{
  if (this != &other) { L1CAIteration(other).Swap(*this); }
  LOG(debug) << "L1CAIteration: Copy operator= called: " << &other << " was copied into " << this;
  return *this;
}

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

//----------------------------------------------------------------------------------------------------------------------
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
}

//----------------------------------------------------------------------------------------------------------------------
//
L1CAIteration::L1CAIteration(const std::string& name) noexcept : fName(name)
{
  LOG(debug) << "L1CAIteration: Constructor from name called for " << this;
}

//----------------------------------------------------------------------------------------------------------------------
//
void L1CAIteration::Print(int verbosityLevel) const
{
  LOG(info) << "CA Track Finder Iteration: " << fName;
  if (verbosityLevel > 0) {
    LOG(info) << "\tTrack chi2 cut: " << fTrackChi2Cut;
    LOG(info) << "\tTriplet chi2 cut: " << fTripletChi2Cut;
    LOG(info) << "\tDoublet chi2 cut: " << fDoubletChi2Cut;
    LOG(info) << "\tPick gather: " << fPickGather;
    LOG(info) << "\tPick neighbour: " << fPickNeighbour;
    LOG(info) << "\tMax invariant momentum: " << fMaxInvMom;
    LOG(info) << "\tMax slope at primary vertex: " << fMaxSlopePV;
    LOG(info) << "\tMax slope: " << fMaxSlope;
  }
}
