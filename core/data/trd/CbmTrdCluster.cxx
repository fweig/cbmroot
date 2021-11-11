/* Copyright (C) 2010-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

/**
 * \file CbmTrdCluster.cxx
 * \author Florian Uhlig <f.uhlig@gsi.de>
 * \brief Data Container for TRD clusters.
 */
#include "CbmTrdCluster.h"

#include <Logger.h>  // for LOG, Logger

#include <TMathBase.h>  // for Min, Abs
#
#include <iostream>  // for operator<<, basic_ostream, stringstream

using std::endl;
using std::string;
using std::stringstream;
using std::vector;
//____________________________________________________________________
CbmTrdCluster::CbmTrdCluster() : CbmCluster(), fNCols(0), fNRows(0xff), fStartCh(0xffff), fStartTime(0xffff) {}

//____________________________________________________________________
CbmTrdCluster::CbmTrdCluster(const CbmTrdCluster& ref)
  : CbmCluster(ref.GetDigis(), ref.GetAddress())
  , fNCols(ref.fNCols)
  , fNRows(ref.fNRows)
  , fStartCh(ref.fStartCh)
  , fStartTime(ref.fStartTime)
{
}

//____________________________________________________________________
CbmTrdCluster::CbmTrdCluster(const std::vector<Int_t>& indices, Int_t address)
  : CbmCluster(indices, address)
  , fNCols(0)
  , fNRows(0xff)
  , fStartCh(0xffff)
  , fStartTime(0xffff)
{
}

//____________________________________________________________________
CbmTrdCluster::CbmTrdCluster(Int_t address, Int_t idx, Int_t ch, Int_t row, Int_t time)
  : CbmCluster()
  , fNCols(0)
  , fNRows(0xff)
  , fStartCh(0xffff)
  , fStartTime(0xffff)
{
  ReInit(address, row, time);
  AddDigi(idx, ch);
}

//____________________________________________________________________
CbmTrdCluster::~CbmTrdCluster() {}

//____________________________________________________________________
Bool_t CbmTrdCluster::AddDigi(Int_t idx, Int_t channel, Int_t terminator, Int_t dt)
{
  /** Extend basic functionality of CbmCluster::AddDigi(). 
 * If channel>=0 add this info to channel map.
 */

  if (channel < 0) {  // basic functionality for rectangular pads
    CbmCluster::AddDigi(idx);
    return kTRUE;
  }

  if (channel >= 0xffff) LOG(warn) << GetName() << "::AddDigi: pad-channel truncated to 2bytes.";

  // assume triangular pads only
  if (!fNCols) {  // first digi
    fStartCh = channel;
    CbmCluster::AddDigi(idx);
    if (terminator < 0) SetProfileStart();
    else if (terminator > 0)
      SetProfileStop();
  }
  else if (channel > GetEndCh()) {  // digi @ end
    if (HasOpenStop()) return kFALSE;
    CbmCluster::AddDigi(idx);
    if (terminator > 0) SetProfileStop();
  }
  else if (channel < fStartCh) {  // digi @ beginning
    if (HasOpenStart()) return kFALSE;
    fStartCh          = channel;
    vector<Int_t> vec = GetDigis();
    ClearDigis();
    CbmCluster::AddDigi(idx);
    AddDigis(vec);
    if (terminator < 0) SetProfileStart();
  }
  fNCols++;
  if (dt > 0) fStartTime -= dt;

  return kTRUE;
}

//____________________________________________________________________
void CbmTrdCluster::Clear(Option_t*)
{
  CbmCluster::ClearDigis();
  fNCols     = 0;
  fNRows     = 0xff;
  fStartCh   = 0xffff;
  fStartTime = 0xffff;
}

//____________________________________________________________________
void CbmTrdCluster::ReInit(Int_t address, Int_t row, Int_t time)
{
  SetAddress(address);
  fNCols   = 0;
  fStartCh = 0xffff;
  // check truncation
  if (row >= 0x1f) LOG(warn) << GetName() << "::ReInit: pad-row truncated to 5bits.";
  SetNRows(row);
  SetProfileStart(0);
  SetProfileStop(0);
  if (time >= 0xffff) LOG(warn) << GetName() << "::ReInit: buffer time truncated to 2bytes.";
  fStartTime = time;
}

//____________________________________________________________________
Int_t CbmTrdCluster::IsChannelInRange(Int_t ch) const
{
  if (!fNCols) return -2;
  //   if(IsTerminatedLeft() && fAddressCh[0]>ch) return -1;
  //   if(IsTerminatedRight() && fAddressCh[clSize-1]<ch) return 1;

  if (fStartCh > ch + 1) return -1;
  if (fStartCh + fNCols < ch) return 1;
  return 0;
}

//____________________________________________________________________
Bool_t CbmTrdCluster::Merge(CbmTrdCluster* second)
{
  if (GetRow() != second->GetRow()) return kFALSE;
  // time difference condition
  if (fNCols == 1 || second->fNCols == 1) {
    if (TMath::Abs(second->fStartTime - fStartTime) > 50) return kFALSE;
  }
  else if (TMath::Abs(second->fStartTime - fStartTime) > 20)
    return kFALSE;
  // look before current
  if (second->fStartCh + second->fNCols == fStartCh && !second->HasOpenStop() && !HasOpenStart()) {
    //     cout<<"Merge before with "<<second->ToString();
    fStartCh = second->fStartCh;
    fNCols += second->fNCols;
    fStartTime = TMath::Min(fStartTime, second->fStartTime);

    vector<Int_t> vec = GetDigis();
    ClearDigis();
    AddDigis(second->GetDigis());
    AddDigis(vec);
    if (second->HasOpenStart()) SetProfileStart();
    return kTRUE;
  }
  // special care for FASP clusters which can be merged also on pairing neighboring
  if (HasTrianglePads()) {
    if ((second->fStartCh + second->fNCols - 1 == fStartCh) && second->HasOpenStop()
        && HasOpenStart()) {  // need to merge digi
      fStartCh = second->fStartCh;
      fNCols += second->fNCols - 1;
      fStartTime = TMath::Min(fStartTime, second->fStartTime);

      vector<Int_t> vec = GetDigis();
      ClearDigis();
      AddDigis(second->GetDigis());
      AddDigis(vec);
      SetProfileStart(kFALSE);
      return kTRUE;
    }
  }

  // look after current
  if (fStartCh + fNCols == second->fStartCh && !HasOpenStop() && !second->HasOpenStart()) {
    //     cout<<"Merge after  with "<<second->ToString();
    fNCols += second->fNCols;
    fStartTime = TMath::Min(fStartTime, second->fStartTime);
    AddDigis(second->GetDigis());
    if (second->HasOpenStop()) SetProfileStop();
    return kTRUE;
  }

  // special care for FASP clusters which can be merged also on pairing neighboring
  if (HasTrianglePads()) {
    if ((fStartCh + fNCols - 1 == second->fStartCh) && HasOpenStop() && second->HasOpenStart()) {  // need to merge digi
      fNCols += second->fNCols - 1;
      fStartTime = TMath::Min(fStartTime, second->fStartTime);
      AddDigis(second->GetDigis());
      SetProfileStop(kFALSE);
      return kTRUE;
    }
  }

  return kFALSE;
}

//____________________________________________________________________
string CbmTrdCluster::ToString() const
{
  stringstream ss;
  ss << CbmCluster::ToString();
  ss << "CbmTrdCluster: mod=" << GetAddress() << " t0=" << fStartTime << " R=" << (Int_t) GetRow() << " "
     << (HasTrianglePads() ? "T" : "R") << "Chs=";
  ss << (HasOpenStart() ? "/" : "|");
  for (Int_t i(0); i < fNCols; i++)
    ss << fStartCh + i << " ";
  ss << (HasOpenStop() ? "\\" : "|");
  ss << endl;
  return ss.str();
}

ClassImp(CbmTrdCluster)
