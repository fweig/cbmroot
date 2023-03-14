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

#include <iostream>  // for operator<<, basic_ostream, stringstream

#include <cmath>

using std::endl;
using std::string;
using std::stringstream;
using std::vector;
//____________________________________________________________________
CbmTrdCluster::CbmTrdCluster() : CbmCluster() {}

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
CbmTrdCluster::CbmTrdCluster(const std::vector<int32_t>& indices, int32_t address)
  : CbmCluster(indices, address)
{
}

//____________________________________________________________________
CbmTrdCluster::CbmTrdCluster(int32_t address, int32_t idx, int32_t ch, int32_t row, int32_t time)
  : CbmCluster()
{
  ReInit(address, row, time);
  AddDigi(idx, ch);
}

//____________________________________________________________________
CbmTrdCluster::~CbmTrdCluster() {}

CbmTrdCluster& CbmTrdCluster::operator=(const CbmTrdCluster& ref)
{
  if (this != &ref) {
    CbmCluster::operator=(ref);
    fNCols              = ref.fNCols;
    fNRows              = ref.fNRows;
    fStartCh            = ref.fStartCh;
    fStartTime          = ref.fStartTime;
  }
  return *this;
}

//____________________________________________________________________
bool CbmTrdCluster::AddDigi(int32_t idx, int32_t channel, int32_t terminator, int32_t dt)
{
  /** Extend basic functionality of CbmCluster::AddDigi().
 * If channel>=0 add this info to channel map.
 */

  if (channel < 0) {  // basic functionality for rectangular pads
    CbmCluster::AddDigi(idx);
    return true;
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
    if (HasOpenStop()) return false;
    CbmCluster::AddDigi(idx);
    if (terminator > 0) SetProfileStop();
  }
  else if (channel < fStartCh) {  // digi @ beginning
    if (HasOpenStart()) return false;
    fStartCh          = channel;
    vector<int32_t> vec = GetDigis();
    ClearDigis();
    CbmCluster::AddDigi(idx);
    AddDigis(vec);
    if (terminator < 0) SetProfileStart();
  }
  fNCols++;
  if (dt > 0) fStartTime -= dt;

  return true;
}

//____________________________________________________________________
void CbmTrdCluster::Clear(Option_t*)
{
  CbmCluster::ClearDigis();
  fNCols     = 0;
  fNRows     = 0x1f;
  fStartCh   = 0xffff;
  fStartTime = 0xffffffff;
}

//____________________________________________________________________
void CbmTrdCluster::ReInit(int32_t address, int32_t row, int32_t time)
{
  SetAddress(address);
  fNCols   = 0;
  fStartCh = 0xffff;
  // check truncation
  if (row >= 0x1f) LOG(warn) << GetName() << "::ReInit: pad-row truncated to 5bits.";
  SetNRows(row);
  SetProfileStart(0);
  SetProfileStop(0);
  if (std::abs(time) >= 0x7fffffff) LOG(warn) << GetName() << "::ReInit: buffer time truncated to 4bytes.";
  fStartTime = time;
}

//____________________________________________________________________
int32_t CbmTrdCluster::IsChannelInRange(int32_t ch) const
{
  if (!fNCols) return -2;
  //   if(IsTerminatedLeft() && fAddressCh[0]>ch) return -1;
  //   if(IsTerminatedRight() && fAddressCh[clSize-1]<ch) return 1;

  if (fStartCh > ch + 1) return -1;
  if (fStartCh + fNCols < ch) return 1;
  return 0;
}

//____________________________________________________________________
uint16_t CbmTrdCluster::GetSize() const
{
  uint16_t size(0);
  if (HasFaspDigis()) {
    size = GetNCols() << 1;
    if (HasOpenStart()) size--;
    if (HasOpenStop()) size--;
    if (size <= 0) {
      LOG(warn) << GetName() << "::GetSize: Fasp cluster meta-info corrupt.";
      std::cout << ToString();
      return 0;
    }
  }
  else
    size = GetNCols();
  return size;
}

//____________________________________________________________________
bool CbmTrdCluster::Merge(CbmTrdCluster* second, bool typ)
{
  if (GetRow() != second->GetRow()) return false;
  // time difference condition
  if (fNCols == 1 || second->fNCols == 1) {
    if (abs(int32_t(second->fStartTime - fStartTime)) > 50) return false;
  }
  else if (abs(int32_t(second->fStartTime - fStartTime)) > 20)
    return false;
  // look before current
  if (second->fStartCh + second->fNCols == fStartCh && !second->HasOpenStop() && !HasOpenStart()) {
    //     cout<<"Merge before with "<<second->ToString();
    fStartCh = second->fStartCh;
    fNCols += second->fNCols;
    fStartTime = std::min(fStartTime, second->fStartTime);

    vector<int32_t> vec = GetDigis();
    ClearDigis();
    AddDigis(second->GetDigis());
    AddDigis(vec);
    if (second->HasOpenStart()) SetProfileStart();
    return true;
  }
  // special care for clusters which can be merged also on pairing neighboring on 2D read-out
  if (typ) {
    if ((second->fStartCh + second->fNCols - 1 == fStartCh) && second->HasOpenStop()
        && HasOpenStart()) {  // need to merge digi
      fStartCh = second->fStartCh;
      fNCols += second->fNCols - 1;
      fStartTime = std::min(fStartTime, second->fStartTime);

      vector<int32_t> vec = GetDigis();
      ClearDigis();
      AddDigis(second->GetDigis());
      AddDigis(vec);
      SetProfileStart(false);
      return true;
    }
  }

  // look after current
  if (fStartCh + fNCols == second->fStartCh && !HasOpenStop() && !second->HasOpenStart()) {
    //     cout<<"Merge after  with "<<second->ToString();
    fNCols += second->fNCols;
    fStartTime = std::min(fStartTime, second->fStartTime);
    AddDigis(second->GetDigis());
    if (second->HasOpenStop()) SetProfileStop();
    return true;
  }

  // special care for clusters which can be merged also on pairing neighboring on 2D read-out
  if (typ) {
    if ((fStartCh + fNCols - 1 == second->fStartCh) && HasOpenStop() && second->HasOpenStart()) {  // need to merge digi
      fNCols += second->fNCols - 1;
      fStartTime = std::min(fStartTime, second->fStartTime);
      AddDigis(second->GetDigis());
      SetProfileStop(false);
      return true;
    }
  }

  return false;
}

//____________________________________________________________________
string CbmTrdCluster::ToString() const
{
  stringstream ss;
  ss << CbmCluster::ToString();
  ss << "CbmTrdCluster: mod=" << GetAddress() << " row=" << (int32_t) GetRow() << " "
     << (HasFaspDigis() ? "Fasp_" : "Spadic_") << "Chs=";
  ss << (HasOpenStart() ? "/" : "|");
  for (int32_t i(0); i < fNCols; i++)
    ss << fStartCh + i << " ";
  ss << (HasOpenStop() ? "\\" : "|");
  ss << endl;
  return ss.str();
}

ClassImp(CbmTrdCluster)
