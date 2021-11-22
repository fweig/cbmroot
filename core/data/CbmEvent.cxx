/* Copyright (C) 2016-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** CbmEvent.cxx
 ** @author V.Friese <v.friese@gsi.de>
 ** @since 17.09.2016
 ** @version 1.0
 **/
#include "CbmEvent.h"

#include <iostream>  // for operator<<, basic_ostream
#include <sstream>   // for stringstream
#include <string>    // for char_traits
#include <utility>   // for pair

// -----   Add data to event   ---------------------------------------------
CbmEvent::CbmEvent(const CbmEvent& rhs)
  : TObject(rhs)
  , fNumber(rhs.fNumber)
  , fTimeStart(rhs.fTimeStart)
  , fTimeEnd(rhs.fTimeEnd)
  , fNofData(rhs.fNofData)
  , fVertex(rhs.fVertex)
  , fMatch(nullptr)
  , fIndexMap(rhs.fIndexMap)
{
  if (fMatch) fMatch = new CbmMatch(*(rhs.fMatch));
}
// -------------------------------------------------------------------------

// -----   Add data to event   ---------------------------------------------
void CbmEvent::AddData(ECbmDataType type, uint32_t index)
{

  fIndexMap[type].push_back(index);
  fNofData++;
}
// -------------------------------------------------------------------------


// -----   Get a data index   ----------------------------------------------
uint32_t CbmEvent::GetIndex(ECbmDataType type, uint32_t iData)
{

  if (fIndexMap.find(type) == fIndexMap.end()) return -1;
  if (fIndexMap[type].size() <= iData) return -2;
  return fIndexMap.at(type)[iData];
}
// -------------------------------------------------------------------------


// -----   Get number of data of a type in this event   --------------------
int32_t CbmEvent::GetNofData(ECbmDataType type) const
{

  if (fIndexMap.find(type) == fIndexMap.end()) return -1;
  else
    return fIndexMap.at(type).size();
}
// -------------------------------------------------------------------------


// -----   Set the vertex parameters   -------------------------------------
void CbmEvent::SetVertex(double x, double y, double z, double chi2, int32_t ndf, int32_t nTracks,
                         const TMatrixFSym& covMat)
{
  fVertex.SetVertex(x, y, z, chi2, ndf, nTracks, covMat);
}
// -------------------------------------------------------------------------

// -----    Swap two events
void CbmEvent::Swap(CbmEvent& e)
{
  std::swap(fNumber, e.fNumber);
  std::swap(fTimeStart, e.fTimeStart);
  std::swap(fTimeEnd, e.fTimeEnd);
  std::swap(fNofData, e.fNofData);
  std::swap(fVertex, e.fVertex);
  std::swap(fMatch, e.fMatch);
  std::swap(fIndexMap, e.fIndexMap);
}


// -----   String output   -------------------------------------------------
std::string CbmEvent::ToString() const
{
  std::stringstream ss;
  ss << "Event " << fNumber << " at t = " << fTimeStart << " ns. Registered data types: " << fIndexMap.size()
     << ", data objects: " << fNofData << (nullptr != fMatch ? ", with matches" : ", without matches") << "\n";
  for (auto it = fIndexMap.begin(); it != fIndexMap.end(); it++) {
    ss << "          -- Data type " << it->first << ", number of data " << it->second.size() << "\n";
  }
  return ss.str();
}
// -------------------------------------------------------------------------


ClassImp(CbmEvent)
