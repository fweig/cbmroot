/** @file CbmTrackMatchNew.cxx
  * @copyright Copyright (C) 2013-2020 GSI/JINR-LIT
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Andrey Lebedev [committer], Florian Uhlig **/

/**
 * \file CbmTrackMatchNew.cxx
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2013
 **/
#include "CbmTrackMatchNew.h"

#include "CbmLink.h"  // for CbmLink

#include <sstream>  // for operator<<, basic_ostream, stringstream
#include <string>   // for char_traits
#include <utility>  // for make_pair
#include <vector>   // for vector

using std::make_pair;
using std::stringstream;

CbmTrackMatchNew::CbmTrackMatchNew() : CbmMatch(), fNofTrueHits(0), fNofWrongHits(0) {}

CbmTrackMatchNew::~CbmTrackMatchNew() {}

std::string CbmTrackMatchNew::ToString() const
{
  stringstream ss;
  ss << "CbmMatch: ";
  Int_t nofLinks = GetNofLinks();
  ss << "nofLinks=" << nofLinks << "\n";
  for (Int_t i = 0; i < nofLinks; i++) {
    const CbmLink& link = fLinks[i];
    ss << link.ToString();
  }
  ss << "| totalWeight=" << fTotalWeight << ", matchedIndex=" << fMatchedIndex << " nofTrueHits=" << fNofTrueHits
     << " nofWrongHits=" << fNofWrongHits << std::endl;
  return ss.str();
}

ClassImp(CbmTrackMatchNew);
