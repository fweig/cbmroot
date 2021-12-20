/* Copyright (C) 2013-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer], Volker Friese, Pierre-Alain Loizeau */

/** @file CbmTofDigi.cxx
 ** @author Pierre-Alain Loizeau <loizeau@physi.uni-heidelberg.de>
 ** @date 07.06.2013
 **
 ** Code for Data class for expanded digital TOF information
 **/
#include "CbmTofDigi.h"

#include <iomanip>  // for hex, setw, setfill, fixed, setprecission
#include <sstream>  // for operator<<, basic_ostream, char_trait
#include <string>   // for basic_string

CbmTofDigi::CbmTofDigi() : fdTime(0.), fdTot(-1.), fuAddress(0)
//    fMatch(nullptr)
{
}

CbmTofDigi::CbmTofDigi(uint32_t address, double time, double tot) : fdTime(time), fdTot(tot), fuAddress(address)
//    fMatch(nullptr)
{
}

CbmTofDigi::CbmTofDigi(uint32_t Sm, uint32_t Rpc, uint32_t Channel, double time, double tot, uint32_t Side,
                       uint32_t SmType)
  : fdTime(time)
  , fdTot(tot)
  , fuAddress(0)
//    fMatch(nullptr)
{
  fuAddress = CbmTofAddress::GetUniqueAddress(Sm, Rpc, Channel, Side, SmType);
}

CbmTofDigi::~CbmTofDigi()
{
  //  if ( fMatch ) delete fMatch;
}

std::string CbmTofDigi::ToString() const
{
  // Example Output i
  // CbmTofDigi: address = 0x05020026 time = 1017.181900 tot = 1.221741

  std::stringstream ss;
  ss << "CbmTofDigi: address = 0x" << std::uppercase << std::hex << std::setw(8) << std::setfill('0') << fuAddress
     << " time = " << std::fixed << std::setprecision(6) << fdTime << " tot = " << fdTot;
  return ss.str();
}

void CbmTofDigi::SetAddress(uint32_t Sm, uint32_t Rpc, uint32_t Channel, uint32_t Side, uint32_t SmType)
{
  fuAddress = CbmTofAddress::GetUniqueAddress(Sm, Rpc, Channel, Side, SmType);
}

#ifndef NO_ROOT
ClassImp(CbmTofDigi)
#endif
