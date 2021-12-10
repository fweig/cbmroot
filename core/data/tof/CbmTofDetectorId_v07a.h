/* Copyright (C) 2013-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

// -------------------------------------------------------------------------
// -----               CbmTofDetectorId_v07a header file               -----
// -----                 Created 20/11/12  by F. Uhlig                 -----
// -------------------------------------------------------------------------


/** CbmTofDetectorId.h
 ** Defines unique detector identifier for all TOF modules.
 ** This class is the implementation for tof geometry version v07a
 ** and v12a
 ** All classes which uses this scheme should have a data member
 ** of this class 
 ** @author F. Uhlig <f.uhlig@gsi.de>
 **/

//                                  3         2         1          shift length
/** Current definition:            10987654321098765432109876543210
 ** System ID (kTOF=6) on bits 0-4 00000000000000000000000000011111        31
 ** Super Module type on bits 5-8  00000000000000000000000111100000 <<5    15
 ** Module NR on bits 9-18         00000000000001111111111000000000 <<9  1023
 ** Gap NR on bits 19-22           00000000011110000000000000000000 <<19   15
 ** Cell NR on bits 23-32          11111111100000000000000000000000 <<23  511
 **/


#ifndef CBMTOFDETECTORID_V07A_H
#define CBMTOFDETECTORID_V07A_H 1

#include "CbmTofDetectorId.h"  // for CbmTofDetectorId, CbmTofDetectorInfo

#include <cstdint>

class CbmTofDetectorId_v07a : public CbmTofDetectorId {

public:
  /** Constructor **/
  CbmTofDetectorId_v07a();

  /** Destructor **/
  ~CbmTofDetectorId_v07a() {};

  /** Get complete system info from detector ID
   ** This will return a pointer to an integer
   ** array of length array_length
   **/
  CbmTofDetectorInfo GetDetectorInfo(const int32_t detectorId);

  /** Get the global sytem ID **/
  int32_t GetSystemId(int32_t detectorId);

  /** Get SMType number from detector ID **/
  int32_t GetSMType(const int32_t detectorId);

  /** Get smodule number from detector ID **/
  int32_t GetSModule(const int32_t detectorId);

  /** Get counter number from detector ID **/
  int32_t GetCounter(const int32_t detectorId);

  /** Get sector number from detector ID **/
  int32_t GetGap(const int32_t detectorId);

  /** Get cell number from detector ID **/
  int32_t GetCell(const int32_t detectorId);

  /** Get region number from detector ID **/
  int32_t GetRegion(const int32_t detectorId);

  /** Get cell number from detector ID.
   ** This is masking the the gap number
   ** if this is set.
   **/
  int32_t GetCellId(const int32_t detectorId);

  /** Calculate the unique detector ID
   ** This will return a pointer to an integer
   ** array of length array_length
   **/
  int32_t SetDetectorInfo(const CbmTofDetectorInfo detectorInfo);

private:
  static const int32_t array_length = 5;
  static const int32_t shiftarray[];
  static const int32_t bitarray[];
  int32_t result_array[array_length];
  int32_t maskarray[array_length];
  int32_t modulemask;
};

#endif
