/* Copyright (C) 2012-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese, Florian Uhlig [committer], Andrey Lebedev */

/**
 * \file CbmCluster.cxx
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2012
 */
#include "CbmCluster.h"

#include "CbmMatch.h"  // for CbmMatch

#include <TObject.h>    // for TObject
#include <type_traits>  // for move

#include <sstream>  // for operator<<, basic_ostream, stringstream
#include <string>   // for char_traits

using namespace std;

CbmCluster::CbmCluster() : TObject(), fDigis(), fAddress(0), fMatch(nullptr) {}
CbmCluster::CbmCluster(const std::vector<int32_t>& indices, int32_t address)
  : TObject()
  , fDigis()
  , fAddress(address)
  , fMatch(nullptr)
{
  fDigis.assign(indices.begin(), indices.end());
}

CbmCluster::CbmCluster(const CbmCluster& other)
  : TObject(other)
  , fDigis(other.fDigis)
  , fAddress(other.fAddress)
  , fMatch(nullptr)
{
  if (other.fMatch) {
    fMatch = new CbmMatch();
    fMatch->AddLinks(*(other.fMatch));
  }
}

CbmCluster::CbmCluster(CbmCluster&& other) noexcept
  : TObject(other)
  , fDigis(std::move(other.fDigis))
  , fAddress(std::move(other.fAddress))
  , fMatch(other.fMatch)
{
  other.fMatch = nullptr;
}

CbmCluster& CbmCluster::operator=(const CbmCluster& other)
{
  if (this != &other) {
    fDigis   = other.fDigis;
    fAddress = other.fAddress;
    fMatch   = nullptr;
    if (other.fMatch) {
      fMatch = new CbmMatch();
      fMatch->AddLinks(*(other.fMatch));
    }
  }
  return *this;
}

CbmCluster& CbmCluster::operator=(CbmCluster&& other) noexcept
{
  if (this != &other) {
    fDigis       = std::move(other.fDigis);
    fAddress     = std::move(other.fAddress);
    fMatch       = other.fMatch;
    other.fMatch = nullptr;
  }
  return *this;
}

CbmCluster::~CbmCluster() {}

void CbmCluster::SetMatch(CbmMatch* match)
{
  if (fMatch) delete fMatch;
  fMatch = match;
}


string CbmCluster::ToString() const
{
  stringstream ss;
  ss << "CbmCluster: ";
  int32_t nofDigis = GetNofDigis();
  ss << "nofDigis=" << nofDigis << " | ";
  for (int32_t i = 0; i < nofDigis; i++) {
    ss << fDigis[i] << " ";
  }
  ss << " | address=" << fAddress << endl;
  return ss.str();
}

ClassImp(CbmCluster);
