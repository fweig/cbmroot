/* Copyright (C) 2013-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

// -------------------------------------------------------------------------
// -----                   CbmTofDetectorId source file                -----
// -----                  Created 20/11/12  by F. Uhlig                -----
// -------------------------------------------------------------------------


#include "CbmTofDetectorId_v07a.h"

#include <Logger.h>  // for LOG, Logger

const int32_t CbmTofDetectorId_v07a::shiftarray[] = {0, 5, 9, 19, 23};
const int32_t CbmTofDetectorId_v07a::bitarray[]   = {5, 4, 10, 4, 9};

//                                  3         2         1          shift length
/** Current definition:            10987654321098765432109876543210
 ** System ID (kTOF=6) on bits 0-4 00000000000000000000000000011111        31
 ** Super Module type on bits 5-8  00000000000000000000000111100000 <<5    15
 ** Module NR on bits 9-18         00000000000001111111111000000000 <<9  1023
 ** Gap NR on bits 19-22           00000000011110000000000000000000 <<19   15
 ** Cell NR on bits 23-32          11111111100000000000000000000000 <<23  511
 **/

CbmTofDetectorId_v07a::CbmTofDetectorId_v07a() : CbmTofDetectorId(), result_array(), maskarray(), modulemask(0)
{
  for (int32_t i = 0; i < array_length; i++) {
    maskarray[i] = (1 << bitarray[i]) - 1;
  }

  modulemask = ((maskarray[0] << shiftarray[0]) | (maskarray[1] << shiftarray[1]) | (maskarray[2] << shiftarray[2])
                | (0 << shiftarray[3]) | (maskarray[4] << shiftarray[4]));
}

CbmTofDetectorInfo CbmTofDetectorId_v07a::GetDetectorInfo(const int32_t detectorId)
{
  for (int32_t i = 0; i < array_length; i++) {
    result_array[i] = ((detectorId >> shiftarray[i]) & maskarray[i]);
  }
  return CbmTofDetectorInfo(result_array[0], result_array[1], 0, result_array[2], result_array[3], result_array[4]);
}

int32_t CbmTofDetectorId_v07a::GetSystemId(int32_t detectorId) { return (detectorId & maskarray[0]); }

//-----------------------------------------------------------

int32_t CbmTofDetectorId_v07a::GetSMType(const int32_t detectorId)
{
  return ((detectorId >> shiftarray[1]) & maskarray[1]);
}

//-----------------------------------------------------------

int32_t CbmTofDetectorId_v07a::GetSModule(const int32_t detectorId)
{
  return ((detectorId >> shiftarray[2]) & maskarray[2]);
}

//-----------------------------------------------------------

int32_t CbmTofDetectorId_v07a::GetCounter(const int32_t /*detectorId*/) { return -1; }

//-----------------------------------------------------------

int32_t CbmTofDetectorId_v07a::GetGap(const int32_t detectorId)
{
  return ((detectorId >> shiftarray[3]) & maskarray[3]);
}

//-----------------------------------------------------------

int32_t CbmTofDetectorId_v07a::GetCell(const int32_t detectorId)
{
  return ((detectorId >> shiftarray[4]) & maskarray[4]);
}

//-----------------------------------------------------------

int32_t CbmTofDetectorId_v07a::GetRegion(const int32_t /*detectorId*/) { return -1; }

int32_t CbmTofDetectorId_v07a::GetCellId(const int32_t detectorId) { return (detectorId & modulemask); }

//-----------------------------------------------------------

int32_t CbmTofDetectorId_v07a::SetDetectorInfo(const CbmTofDetectorInfo detInfo)
{

  LOG(debug2) << "Det System: " << detInfo.fDetectorSystem;
  LOG(debug2) << "SMtype: " << detInfo.fSMtype;
  LOG(debug2) << "SModule: " << detInfo.fSModule;
  LOG(debug2) << "Counter: " << detInfo.fCounter;
  LOG(debug2) << "Gap: " << detInfo.fGap;
  LOG(debug2) << "Cell: " << detInfo.fCell;

  return ((detInfo.fDetectorSystem << shiftarray[0]) | (detInfo.fSMtype << shiftarray[1])
          | (detInfo.fCounter << shiftarray[2]) | (detInfo.fGap << shiftarray[3]) | (detInfo.fCell << shiftarray[4]));
}
