/* Copyright (C) 2020-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

/**

	TODO: If the TBR addess is not provided in the par file but found in the data file,
	The std::map::at will throw out an exception which is not currectly caught.

*/

#include "CbmMcbm2018RichPar.h"

// FairRoot
#include "FairParamList.h"
#include <Logger.h>

// C/C++
#include <iomanip>

//#include <Logger.h> //TODO delete

CbmMcbm2018RichPar::CbmMcbm2018RichPar(const char* name, const char* title, const char* context)
  : FairParGenericSet(name, title, context)
{
  detName = "RICH";
}

CbmMcbm2018RichPar::~CbmMcbm2018RichPar() {}

void CbmMcbm2018RichPar::putParams(FairParamList* l)
{
  if (!l) return;

  l->add("TRBaddresses", fTRBaddresses);
  l->add("ToTshifts", fToTshifts);
}

Bool_t CbmMcbm2018RichPar::getParams(FairParamList* l)
{
  if (!l) return kFALSE;

  if (!l->fill("TRBaddresses", &fTRBaddresses)) return kFALSE;
  if (!l->fill("ToTshifts", &fToTshifts)) return kFALSE;

  LoadInternalContainers();

  return kTRUE;
}

void CbmMcbm2018RichPar::LoadInternalContainers()
{
  // Create a map from the list imported from the par file
  Int_t siz = fTRBaddresses.GetSize();
  for (Int_t i = 0; i < siz; i++) {
    fTRBaddrMap.insert(std::pair<Int_t, Int_t>(fTRBaddresses[i], i));
    LOG(debug) << "Inserting in RICH TRB map: 0x" << std::hex << std::setw(4) << fTRBaddresses[i] << std::dec << " "
               << i;
  }

  // Create a vector from the lists imported from the par file
  // Continuous indices => no need for map with deep calls
  for (Int_t i = 0; i < siz; i++) {
    for (Int_t ch = 0; ch <= 32; ch++) {
      fToTshiftMap.push_back(fToTshifts[i * 33 + ch]);
    }
  }
}

Int_t CbmMcbm2018RichPar::GetAddressIdx(Int_t addr) const
{
  auto it = fTRBaddrMap.find(addr);
  if (fTRBaddrMap.end() == it) {
    LOG(warning) << "CbmMcbm2018RichPar::GetAddressIdx => Unknown TRB address 0x" << std::hex << std::setw(4) << addr
                 << std::dec << ", probably corrupted data!";
    LOG(warning) << "Nb available TRB addresses: " << GetNaddresses();

    Print();

    return -1;
  }
  else {
    return it->second;
  }
}

Int_t CbmMcbm2018RichPar::GetAddress(Int_t ind) const
{
  if (ind < 0 || ind >= fTRBaddresses.GetSize()) return -1;
  return fTRBaddresses[ind];
}

Double_t CbmMcbm2018RichPar::GetToTshift2(Int_t tdcIdx, Int_t ch) const
{
  if (-1 == tdcIdx) {
    LOG(fatal) << "CbmMcbm2018RichPar::GetToTshift2 => Invalid TDC index, "
               << "check your data or your parameters!";
  }
  return fToTshiftMap[tdcIdx * 33 + ch];
}

void CbmMcbm2018RichPar::Print() const
{
  LOG(info) << "Nb available TRB addresses: " << GetNaddresses();

  TString sPrintout = "";
  for (Int_t iTrb = 0; iTrb < GetNaddresses(); ++iTrb) {
    if (0 == iTrb % 8) sPrintout += "\n";
    sPrintout += Form(" 0x%04x", GetAddress(iTrb));
  }  // for( Int_t iTrb = 0; iTrb < GetNaddresses; ++iTrb)
  LOG(info) << "Available TRB addresses: " << sPrintout;

  sPrintout = "";
  for (auto it = fTRBaddrMap.begin(); it != fTRBaddrMap.end(); ++it) {
    sPrintout += Form(" 0x%04x", it->first);
  }  // for( UInt_t i = 0; i < uNrOfChannels; ++i)
  LOG(info) << "TRB addresses in map: " << std::endl << sPrintout;
}

ClassImp(CbmMcbm2018RichPar)
