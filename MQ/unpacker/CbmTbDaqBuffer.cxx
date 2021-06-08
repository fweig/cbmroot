/* Copyright (C) 2013-2020 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese, Norbert Herrmann [committer] */

/** @file CbmTbDaqBuffer.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 13 December 2013
 **/

#include "CbmTbDaqBuffer.h"

#include <FairLogger.h>  // for Logger, LOG

#include <boost/any.hpp>

#include <iomanip>  // for setprecision, __iom_t5
#include <sstream>  // for basic_stringstream<>::string_type

#include <stddef.h>  // for NULL

// -----   Initialisation of static variables   ------------------------------
CbmTbDaqBuffer* CbmTbDaqBuffer::fgInstance = nullptr;
// ---------------------------------------------------------------------------


// -----   Constructor   -----------------------------------------------------
CbmTbDaqBuffer::CbmTbDaqBuffer() : fData() {}
// ---------------------------------------------------------------------------


// -----   Destructor   ------------------------------------------------------
CbmTbDaqBuffer::~CbmTbDaqBuffer() {}
// ---------------------------------------------------------------------------

Double_t CbmTbDaqBuffer::GetTimeFirst() const
{
  if (!GetSize()) return -1.;
  // Return the key from the first element in the map
  // The key of the map is the time of digi
  return fData.begin()->first;
}

Double_t CbmTbDaqBuffer::GetTimeLast() const
{
  if (!GetSize()) return -1.;
  // Return the key from the last element in the map
  // The key of the map is the time of digi
  return (--fData.end())->first;
}

// -----   Access to next data   ---------------------------------------------
CbmTbDaqBuffer::Data CbmTbDaqBuffer::GetNextData(Double_t time)
{

  // --- Check for empty buffer
  if (!fData.size()) return std::make_pair(boost::any(), ECbmModuleId::kNotExist);

  // --- Get data from buffer
  std::multimap<Double_t, std::pair<boost::any, ECbmModuleId>>::iterator it = fData.begin();
  Double_t digi_time                                                        = it->first;

  if (digi_time < time) {
    boost::any digi    = it->second.first;
    ECbmModuleId sysID = it->second.second;
    fData.erase(it);
    return std::make_pair(digi, sysID);
  }
  return std::make_pair(boost::any(), ECbmModuleId::kNotExist);
}
// ---------------------------------------------------------------------------

// -----   Instance   --------------------------------------------------------
CbmTbDaqBuffer* CbmTbDaqBuffer::Instance()
{
  if (!fgInstance) fgInstance = new CbmTbDaqBuffer();
  return fgInstance;
}
// ---------------------------------------------------------------------------

// -----   Print status   ----------------------------------------------------
void CbmTbDaqBuffer::PrintStatus() const
{
  Int_t size = GetSize();
  std::stringstream ss;
  ss << "CbmTbDaqBuffer: Status ";
  if (!size) {
    LOG(info) << ss.str() << "empty";
    return;
  }
  LOG(info) << ss.str() << GetSize() << " digis from " << std::fixed << std::setprecision(9) << GetTimeFirst() * 1.e-9
            << " s to " << GetTimeLast() * 1.e-9 << " s";
}
// ---------------------------------------------------------------------------
