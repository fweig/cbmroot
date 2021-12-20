/* Copyright (C) 2012-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Nikolay Karpushkin, Volker Friese [committer], Florian Uhlig */

/** @file CbmPsdDigi.cxx
 ** @author Nikolay Karpushkin <karpushkin@inr.ru>
 ** @date 09.10.2019
 **
 ** Code for Data class for PSD digital information
 **/

#include "CbmPsdDigi.h"

#include <iomanip>  // for hex, setw, setfill, fixed, setprecission
#include <sstream>  // for operator<<, basic_ostream, char_trait
#include <string>   // for basic_string

// --- Copy constructor
CbmPsdDigi::CbmPsdDigi(const CbmPsdDigi& other) : fuAddress(other.fuAddress), fdTime(other.fdTime), fdEdep(other.fdEdep)
{
}


// --- Move constructor
CbmPsdDigi::CbmPsdDigi(CbmPsdDigi&& other) : fuAddress(other.fuAddress), fdTime(other.fdTime), fdEdep(other.fdEdep) {}


// --- Set address from module and section number
void CbmPsdDigi::SetAddress(uint32_t moduleId, uint32_t sectionId)
{
  fuAddress = CbmPsdAddress::GetAddress(moduleId, sectionId);
}


// --- Info to string
std::string CbmPsdDigi::ToString() const
{
  // Example output
  // CbmPsdDigi: address = 0x00001018 Charge = 0.011590 Time = 1006.438294

  std::stringstream ss;
  ss << "CbmPsdDigi: address = 0x" << std::uppercase << std::hex << std::setw(8) << std::setfill('0') << fuAddress
     << " Charge = " << std::fixed << std::setprecision(6) << fdEdep << " Time = " << fdTime;
  return ss.str();
}

#ifndef NO_ROOT
ClassImp(CbmPsdDigi)
#endif
