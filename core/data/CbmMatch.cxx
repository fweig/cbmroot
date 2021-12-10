/* Copyright (C) 2013-2020 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer] */

/**
 * \file CbmMatch.cxx
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2013
 **/
#include "CbmMatch.h"

#include <sstream>  // for operator<<, basic_ostream, stringstream
#include <string>   // for char_traits
#include <utility>  // for make_pair

using std::make_pair;
using std::string;
using std::stringstream;
using std::vector;

CbmMatch::CbmMatch() : fLinks(), fTotalWeight(0.), fMatchedIndex(-1) {}

CbmMatch::~CbmMatch() {}

string CbmMatch::ToString() const
{
  stringstream ss;
  ss << "CbmMatch: ";
  int32_t nofLinks = GetNofLinks();
  ss << "nofLinks=" << nofLinks << "\n";
  for (int32_t i = 0; i < nofLinks; i++) {
    const CbmLink& link = fLinks[i];
    ss << link.ToString();
  }
  ss << " totalWeight=" << fTotalWeight << ", matchedIndex=" << fMatchedIndex << std::endl;
  return ss.str();
}

void CbmMatch::AddLinks(const CbmMatch& match)
{
  int32_t nofLinks = match.GetNofLinks();
  for (int32_t i = 0; i < nofLinks; i++) {
    AddLink(match.GetLink(i));
  }
}

void CbmMatch::AddLink(const CbmLink& newLink)
{
  int32_t addedIndex = -1;
  int32_t nofLinks   = GetNofLinks();
  for (int32_t i = 0; i < nofLinks; i++) {
    CbmLink& link = fLinks[i];
    if (link == newLink) {
      link.AddWeight(newLink.GetWeight());
      addedIndex = i;
      break;
    }
  }
  if (addedIndex < 0) {
    fLinks.push_back(newLink);
    addedIndex = fLinks.size() - 1;
  }

  fTotalWeight += newLink.GetWeight();
  if (fMatchedIndex < 0) { fMatchedIndex = addedIndex; }
  else {
    if (fLinks[addedIndex].GetWeight() > fLinks[fMatchedIndex].GetWeight()) { fMatchedIndex = addedIndex; }
  }
}

void CbmMatch::AddLink(double weight, int32_t index, int32_t entry, int32_t file)
{
  CbmLink link(weight, index, entry, file);
  AddLink(link);
}


void CbmMatch::ClearLinks()
{
  fLinks.clear();
  fTotalWeight  = 0.;
  fMatchedIndex = -1;
}

ClassImp(CbmMatch);
