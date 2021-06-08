/* Copyright (C) 2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

/** @file CbmErrorMessage.cxx
 ** @author Pierre-Alain Loizeau <p.-a.loizeau@gsi.de>
 ** @date 19.02.2020
 **/
#include "CbmErrorMessage.h"

#include <iomanip>  // for operator<<, setw
#include <sstream>  // for basic_ostream, operator<<, stringstream
#include <string>   // for char_traits

// -----   Standard constructor   ------------------------------------------
CbmErrorMessage::CbmErrorMessage(ECbmModuleId sysId, Double_t dTime, UInt_t uAddress, UInt_t uFlags, UInt_t uPayload)
  : fModuleId(sysId)
  , fdTime(dTime)
  , fuAddress(uAddress)
  , fuFlags(uFlags)
  , fuPayload(uPayload)
{
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmErrorMessage::~CbmErrorMessage() {}
// -------------------------------------------------------------------------


// -----   Info to string   ------------------------------------------------
std::string CbmErrorMessage::ToString() const
{
  std::stringstream ss;
  ss << "Error message: System " << GetSystemId() << " | time " << GetTime() << " | address " << GetAddress();
  char cPrev = ss.fill('0');
  ss << " | flags 0x" << std::hex << std::setw(8) << GetFlags() << std::dec << " | fulPayload 0x" << std::hex
     << std::setw(8) << GetPayload() << std::dec;
  ss.fill(cPrev);
  return ss.str();
}
// -------------------------------------------------------------------------


ClassImp(CbmErrorMessage)
