/* Copyright (C) 2006-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Matus Kalisky [committer], Florian Uhlig, Andrey Lebedev */

/**
 * \file CbmTrdHit.cxx
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2009
 **/
#include "CbmTrdHit.h"

#include "CbmHit.h"  // for kTRDHIT

#include <TVector3.h>  // for TVector3

#include <sstream>  // for operator<<, basic_ostream, stringstream

using std::endl;
using std::stringstream;

CbmTrdHit::CbmTrdHit() : CbmPixelHit(), fDefine(0), fELoss(-1.)
{
  SetType(kTRDHIT);
  SetTime(-1);
  SetTimeError(-1);
}

CbmTrdHit::CbmTrdHit(Int_t address, const TVector3& pos, const TVector3& dpos, Double_t dxy, Int_t refId,
                     Double_t eLoss, Double_t time, Double_t timeError)
  : CbmPixelHit(address, pos, dpos, dxy, refId)
  , fDefine(0)
  , fELoss(eLoss)
{
  SetType(kTRDHIT);
  SetTime(time);
  SetTimeError(timeError);
}

CbmTrdHit::~CbmTrdHit() {}

std::string CbmTrdHit::ToString() const
{
  stringstream ss;
  ss << CbmPixelHit::ToString();
  ss << "CbmTrdHit: time[ns]=" << GetTime() << "+-" << GetTimeError() << " eloss=" << GetELoss();
  if (GetClassType()) ss << " typ[T]" << (GetMaxType() ? "Max{T}" : "Max{R}");
  else
    ss << " typ[R]";
  ss << " RC=" << (IsRowCross() ? 'y' : 'n') << " Ovf=" << (HasOverFlow() ? 'y' : 'n') << endl;
  return ss.str();
}

ClassImp(CbmTrdHit);
