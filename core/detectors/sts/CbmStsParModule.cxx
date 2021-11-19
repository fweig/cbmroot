/* Copyright (C) 2014-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */


#include "CbmStsParModule.h"

#include <cassert>   // for assert
#include <iostream>  // for operator<<, basic_ostream, stringstream
#include <sstream>   // for stringstream
#include <string>    // for char_traits

using std::string;
using std::stringstream;


// -----   Constructor   ---------------------------------------------------
CbmStsParModule::CbmStsParModule(uint32_t nChannels, uint32_t nAsicChannels)
  : fNofChannels(nChannels)
  , fNofAsicChannels(nAsicChannels)
{
  uint32_t nAsics = (nChannels % nAsicChannels ? nChannels / nAsicChannels + 1 : nChannels / nAsicChannels);
  fAsicPars.resize(nAsics);
}
// -------------------------------------------------------------------------


// -----   Randomly deactivate channels   ----------------------------------
uint32_t CbmStsParModule::DeactivateRandomChannels(Double_t fraction)
{
  if (fraction <= 0.) return 0;
  uint32_t nDeactivated = 0;
  for (auto& asic : fAsicPars) {
    nDeactivated += asic.DeactivateRandomChannels(fraction);
  }
  return nDeactivated;
}
// -------------------------------------------------------------------------


// -----   Get ASIC parameters   -------------------------------------------
const CbmStsParAsic& CbmStsParModule::GetParAsic(uint32_t channel) const
{
  assert(!fAsicPars.empty());
  assert(channel < fNofChannels);
  uint32_t nAsic = channel / fNofAsicChannels;
  assert(nAsic < GetNofAsics());
  return fAsicPars[nAsic];
}
// -------------------------------------------------------------------------


// -----   Check for a channel being active   ------------------------------
Bool_t CbmStsParModule::IsChannelActive(uint32_t channel) const
{
  const CbmStsParAsic& parAsic = GetParAsic(channel);
  uint32_t asicChannel         = channel % fNofAsicChannels;
  return parAsic.IsChannelActive(asicChannel);
}
// -------------------------------------------------------------------------


// -----   Set parameters for all ASICs   ----------------------------------
void CbmStsParModule::SetAllAsics(const CbmStsParAsic& asicPar)
{
  assert(asicPar.GetNofChannels() == fNofAsicChannels);
  for (auto& par : fAsicPars) {
    par = asicPar;
    par.Init();
  }
}
// -------------------------------------------------------------------------


// -----   Set parameters for one ASIC   -----------------------------------
void CbmStsParModule::SetAsic(uint32_t asicNr, const CbmStsParAsic& asicPar)
{
  assert(asicNr < fAsicPars.size());
  fAsicPars[asicNr] = asicPar;
}
// -------------------------------------------------------------------------


// -----   String output   -------------------------------------------------
string CbmStsParModule::ToString() const
{
  stringstream ss;
  ss << "Channels " << fNofChannels << " | ASICS " << GetNofAsics() << " | Channels per ASIC " << fNofAsicChannels;
  return ss.str();
}
// -------------------------------------------------------------------------


ClassImp(CbmStsParModule)
