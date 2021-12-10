/* Copyright (C) 2012-2015 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig, Norbert Herrmann [committer], Pierre-Alain Loizeau */

// -------------------------------------------------------------------------
// -----                   CbmTofDetectorId source file                -----
// -----                  Created 20/11/12  by F. Uhlig                -----
// -------------------------------------------------------------------------


#include "CbmTofDetectorId_v14a.h"

const int32_t CbmTofDetectorId_v14a::shiftarray[] = {0, 4, 12, 16, 23, 24};
const int32_t CbmTofDetectorId_v14a::bitarray[]   = {4, 8, 4, 7, 1, 8};


CbmTofDetectorId_v14a::CbmTofDetectorId_v14a() : CbmTofDetectorId(), result_array(), maskarray(), modulemask(0)
{
  for (int32_t i = 0; i < array_length; i++) {
    maskarray[i] = (1 << bitarray[i]) - 1;
  }

  modulemask = ((maskarray[0] << shiftarray[0]) | (maskarray[1] << shiftarray[1]) | (maskarray[2] << shiftarray[2])
                | (maskarray[3] << shiftarray[3]) | (0 << shiftarray[4]) | (maskarray[5] << shiftarray[5]));
}

CbmTofDetectorInfo CbmTofDetectorId_v14a::GetDetectorInfo(const int32_t detectorId)
{
  for (int32_t i = 0; i < array_length; i++) {
    result_array[i] = ((detectorId >> shiftarray[i]) & maskarray[i]);
  }

  return CbmTofDetectorInfo(result_array[0], result_array[2], result_array[1], result_array[3], result_array[4],
                            result_array[5]);
}

int32_t CbmTofDetectorId_v14a::GetSystemId(int32_t detectorId) { return (detectorId & maskarray[0]); }

//-----------------------------------------------------------

int32_t CbmTofDetectorId_v14a::GetSMType(const int32_t detectorId)
{
  return ((detectorId >> shiftarray[2]) & maskarray[2]);
}
int32_t CbmTofDetectorId_v14a::GetModuleType(const int32_t detectorId) { return GetSMType(detectorId); }

//-----------------------------------------------------------

int32_t CbmTofDetectorId_v14a::GetSModule(const int32_t detectorId)
{
  return ((detectorId >> shiftarray[1]) & maskarray[1]);
}
int32_t CbmTofDetectorId_v14a::GetModuleId(const int32_t detectorId) { return GetSModule(detectorId); }

//-----------------------------------------------------------

int32_t CbmTofDetectorId_v14a::GetCounter(const int32_t detectorId)
{
  return ((detectorId >> shiftarray[3]) & maskarray[3]);
}

//-----------------------------------------------------------

int32_t CbmTofDetectorId_v14a::GetSide(const int32_t detectorId)
{
  return ((detectorId >> shiftarray[4]) & maskarray[4]);
}
int32_t CbmTofDetectorId_v14a::GetGap(const int32_t detectorId) { return GetSide(detectorId); }

//-----------------------------------------------------------

int32_t CbmTofDetectorId_v14a::GetCell(const int32_t detectorId)
{
  return ((detectorId >> shiftarray[5]) & maskarray[5]);
}

int32_t CbmTofDetectorId_v14a::GetStrip(const int32_t detectorId) { return GetCell(detectorId); }

//-----------------------------------------------------------

int32_t CbmTofDetectorId_v14a::GetRegion(const int32_t /*detectorId*/) { return -1; }

int32_t CbmTofDetectorId_v14a::GetCellId(const int32_t detectorId) { return (detectorId & modulemask); }

//-----------------------------------------------------------

int32_t CbmTofDetectorId_v14a ::SetDetectorInfo(const CbmTofDetectorInfo detInfo)
{
  return ((((detInfo.fDetectorSystem) & maskarray[0]) << shiftarray[0])
          | (((detInfo.fSMtype) & maskarray[2]) << shiftarray[2])
          | (((detInfo.fSModule) & maskarray[1]) << shiftarray[1])
          | (((detInfo.fCounter) & maskarray[3]) << shiftarray[3]) | (((detInfo.fGap) & maskarray[4]) << shiftarray[4])
          | (((detInfo.fCell) & maskarray[5]) << shiftarray[5]));
}
