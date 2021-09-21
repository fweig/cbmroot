/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/*
 * CbmRawEvent.cxx
 *
 *  Created on: 27.03.2021
 *      Author: vfriese
 */


#include "CbmRawEvent.h"


// -----   Number of digis per system
size_t CbmRawEvent::GetNofDigis(ECbmModuleId system) const
{
  switch (system) {
    case ECbmModuleId::kMvd: return fDigisMvd.size(); break;
    case ECbmModuleId::kSts: return fDigisSts.size(); break;
    case ECbmModuleId::kRich: return fDigisRich.size(); break;
    case ECbmModuleId::kMuch: return fDigisMuch.size(); break;
    case ECbmModuleId::kTrd: return fDigisTrd.size(); break;
    case ECbmModuleId::kTof: return fDigisTof.size(); break;
    case ECbmModuleId::kPsd: return fDigisPsd.size(); break;
    default: return 0; break;
  }
}


// -----   Total number of digis
size_t CbmRawEvent::GetNofDigis() const
{
  size_t result = fDigisMvd.size();
  result += fDigisSts.size();
  result += fDigisRich.size();
  result += fDigisMuch.size();
  result += fDigisTrd.size();
  result += fDigisTof.size();
  result += fDigisPsd.size();
  return result;
}


// -----   String output
std::string CbmRawEvent::ToString() const
{
  std::stringstream ss;
  ss << "Event " << fNumber << " at t = " << fTime << " ns. Digis: " << GetNofDigis() << " (MVD " << fDigisMvd.size()
     << "  STS " << fDigisSts.size() << "  RICH " << fDigisRich.size() << "  MUCH " << fDigisMuch.size() << "  TRD "
     << fDigisTrd.size() << "  TOF " << fDigisTof.size() << "  PSD " << fDigisPsd.size() << ")";
  return ss.str();
}
// -------------------------------------------------------------------------
