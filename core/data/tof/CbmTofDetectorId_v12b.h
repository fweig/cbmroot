/* Copyright (C) 2013-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

// -------------------------------------------------------------------------
// -----               CbmTofDetectorId_v12b header file               -----
// -----                 Created 20/11/12  by F. Uhlig                 -----
// -------------------------------------------------------------------------


/** CbmTofDetectorId.h
 ** Defines unique detector identifier for all TOF modules.
 ** This class is the implementation for tof geometry version v12b
 ** and v12a
 ** All classes which uses this scheme should have a data member
 ** of this class 
 ** @author F. Uhlig <f.uhlig@gsi.de>
 **/

//                                  3         2         1          shift length
/** Current definition:            10987654321098765432109876543210
 ** System ID (kTOF=6) on bits 0-4 00000000000000000000000000011111       31
 ** SuperModule type on bits 5-8   00000000000000000000000111100000 <<5   15
 ** SuperModule NR on bits 9-16    00000000000000011111111000000000 <<9   255
 ** Module NR on bits 17-19        00000000000011100000000000000000 <<17  7
 ** Gap NR on bits 20-23           00000000111100000000000000000000 <<20  15
 ** Cell NR on bits 24-31          11111111000000000000000000000000 <<24  255
 **/


#ifndef CBMTOFDETECTORID_V12B_H
#define CBMTOFDETECTORID_V12B_H 1

#include "CbmTofDetectorId.h"  // for CbmTofDetectorId, CbmTofDetectorInfo

#include <cstdint>

class CbmTofDetectorId_v12b : public CbmTofDetectorId {

public:
  /** Constructor **/
  CbmTofDetectorId_v12b();

  /** Destructor **/
  ~CbmTofDetectorId_v12b() {};

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

  /** Get module number from detector ID **/
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
  static const int32_t array_length = 6;
  static const int32_t shiftarray[];
  static const int32_t bitarray[];
  int32_t result_array[array_length];
  int32_t maskarray[array_length];
  int32_t modulemask;
};

#endif
