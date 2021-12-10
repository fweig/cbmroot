/* Copyright (C) 2012-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig, Norbert Herrmann [committer] */

// -------------------------------------------------------------------------
// -----                   CbmTofDetectorId source file                -----
// -----                  Created 20/11/12  by F. Uhlig                -----
// -------------------------------------------------------------------------


#include "CbmTofDetectorId_v21a.h"

#include <iomanip>
#include <iostream>

using namespace std;

const int32_t CbmTofDetectorId_v21a::shiftarray[] = {0, 4, 11, 15, 21, 22, 28};
const int32_t CbmTofDetectorId_v21a::bitarray[]   = {4, 7, 4, 6, 1, 6, 4};


CbmTofDetectorId_v21a::CbmTofDetectorId_v21a() : CbmTofDetectorId(), result_array(), maskarray(), modulemask(0)
{
  for (int32_t i = 0; i < array_length; i++) {
    maskarray[i] = (1 << bitarray[i]) - 1;
  }

  for (int32_t i = 0; i < array_length - 1; i++) {
    if (i == 4) continue;  // ignore side bit
    modulemask |= (maskarray[i] << shiftarray[i]);
  }
  char prev = std::cout.fill();
  std::cout << "<I> V21a module mask 0x" << std::setfill('0') << std::setw(8) << std::right << std::hex << modulemask
            << std::setfill(prev) << std::dec << std::endl;
}

CbmTofDetectorInfo CbmTofDetectorId_v21a::GetDetectorInfo(const int32_t detectorId)
{
  for (int32_t i = 0; i < array_length; i++) {
    result_array[i] = ((detectorId >> shiftarray[i]) & maskarray[i]);
  }

  return CbmTofDetectorInfo((ECbmModuleId) result_array[0], result_array[2], result_array[1], result_array[3],
                            result_array[4], result_array[5], result_array[6]);
}

int32_t CbmTofDetectorId_v21a::GetSystemId(int32_t detectorId) { return (detectorId & maskarray[0]); }

//-----------------------------------------------------------

int32_t CbmTofDetectorId_v21a::GetSMType(const int32_t detectorId)
{
  return ((detectorId >> shiftarray[2]) & maskarray[2]);
}
int32_t CbmTofDetectorId_v21a::GetModuleType(const int32_t detectorId) { return GetSMType(detectorId); }
int32_t CbmTofDetectorId_v21a::GetCounterType(const int32_t detectorId)
{
  return ((detectorId >> shiftarray[6]) & maskarray[6]);
}

//-----------------------------------------------------------

int32_t CbmTofDetectorId_v21a::GetSModule(const int32_t detectorId)
{
  return ((detectorId >> shiftarray[1]) & maskarray[1]);
}
int32_t CbmTofDetectorId_v21a::GetModuleId(const int32_t detectorId) { return GetSModule(detectorId); }

//-----------------------------------------------------------

int32_t CbmTofDetectorId_v21a::GetCounter(const int32_t detectorId)
{
  return ((detectorId >> shiftarray[3]) & maskarray[3]);
}

//-----------------------------------------------------------

int32_t CbmTofDetectorId_v21a::GetSide(const int32_t detectorId)
{
  return ((detectorId >> shiftarray[4]) & maskarray[4]);
}
int32_t CbmTofDetectorId_v21a::GetGap(const int32_t detectorId) { return GetSide(detectorId); }

//-----------------------------------------------------------

int32_t CbmTofDetectorId_v21a::GetCell(const int32_t detectorId)
{
  return ((detectorId >> shiftarray[5]) & maskarray[5]);
}

int32_t CbmTofDetectorId_v21a::GetStrip(const int32_t detectorId) { return GetCell(detectorId); }

//-----------------------------------------------------------

int32_t CbmTofDetectorId_v21a::GetRegion(const int32_t /*detectorId*/) { return -1; }

int32_t CbmTofDetectorId_v21a::GetCellId(const int32_t detectorId) { return (detectorId & modulemask); }

//-----------------------------------------------------------

int32_t CbmTofDetectorId_v21a ::SetDetectorInfo(const CbmTofDetectorInfo detInfo)
{
  /*
  std::cout << "SetDetectorInfo for "
		  << "Mtype "   << detInfo.fSMtype
		  << ", MId "   << detInfo.fSModule << " "
		  << ", CType " << detInfo.fCounterType <<" "
		  << ", CId "   << detInfo.fCounter
		  << ", Side "  << detInfo.fGap
		  << ", Strip " << detInfo.fCell
		  << std::endl;
*/
  return (
    (((detInfo.fDetectorSystem) & maskarray[0]) << shiftarray[0])
    | (((detInfo.fSMtype) & maskarray[2]) << shiftarray[2]) | (((detInfo.fSModule) & maskarray[1]) << shiftarray[1])
    | (((detInfo.fCounter) & maskarray[3]) << shiftarray[3]) | (((detInfo.fGap) & maskarray[4]) << shiftarray[4])
    | (((detInfo.fCell) & maskarray[5]) << shiftarray[5]) | (((detInfo.fCounterType) & maskarray[6]) << shiftarray[6]));
}
