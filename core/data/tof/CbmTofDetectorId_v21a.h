/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig, Norbert Herrmann [committer] */

// -------------------------------------------------------------------------
// -----               CbmTofDetectorId_v12b header file               -----
// -----                 Created 20/11/12  by F. Uhlig                 -----
// -------------------------------------------------------------------------


/** CbmTofDetectorId.h
 ** Defines unique detector identifier for all TOF modules.
 ** This class is the implementation for tof geometry version v21a 
 ** nh, 28.07.2021 add counter type
 ** nh, 11.03.2014
 ** PAL, 23.09.2015: make the class common to both v14 and v15 geometries
 **                  Field 4 used as Side index (or fake Gap index in digitizer)
 **                  Naming as in TDR: SM -> Module, Module -> Counter, Cell -> Strip 
 ** All classes which uses this scheme should have a data member
 ** of this class 
 ** @author F. Uhlig <f.uhlig@gsi.de>
 **/

//                                  3         2         1          shift length
/** Current definition:            10987654321098765432109876543210
 ** System ID (kTOF=6) on bits 0-3 00000000000000000000000000001111       15
 ** Module ID on bits 4-10         00000000000000000000011111110000 <<4   127
 ** Module Type on bits 11-14      00000000000000000111100000000000 <<11  15
 ** Counter ID on bits 15-20       00000000000111111000000000000000 <<15  63
 ** Side/Gap NR on bits 21-21      00000000001000000000000000000000 <<21  1
 ** Strip ID on bits 22-27         00001111110000000000000000000000 <<22  255
 *  Counter Type on bits 28 - 31   11110000000000000000000000000000 <<28  15
 **/


#ifndef CBMTOFDETECTORID_V21A_H
#define CBMTOFDETECTORID_V21A_H 1

#include "CbmTofDetectorId.h"

class CbmTofDetectorId_v21a : public CbmTofDetectorId {

public:
  /** Constructor **/
  CbmTofDetectorId_v21a();

  /** Destructor **/
  ~CbmTofDetectorId_v21a() {};

  /** Get complete system info from detector ID
   ** This will return a pointer to an integer
   ** array of length array_length
   **/
  CbmTofDetectorInfo GetDetectorInfo(const int32_t detectorId);

  /** Get the global sytem ID **/
  int32_t GetSystemId(int32_t detectorId);

  /** Get Module Type from detector ID **/
  int32_t GetSMType(const int32_t detectorId);
  int32_t GetModuleType(const int32_t detectorId);
  int32_t GetCounterType(const int32_t detectorId);

  /** Get Module ID from detector ID **/
  int32_t GetSModule(const int32_t detectorId);
  int32_t GetModuleId(const int32_t detectorId);

  /** Get counter ID from detector ID **/
  int32_t GetCounter(const int32_t detectorId);

  /** Get sector number from detector ID **/
  int32_t GetGap(const int32_t detectorId);

  /** Get sector number from detector ID **/
  int32_t GetSide(const int32_t detectorId);

  /** Get cell number from detector ID **/
  int32_t GetCell(const int32_t detectorId);
  /** Get Strip ID from detector ID **/
  int32_t GetStrip(const int32_t detectorId);

  /** Get region number from detector ID **/
  int32_t GetRegion(const int32_t detectorId);

  /** Get full cell number from detector ID.
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
  static const int32_t array_length = 7;
  static const int32_t shiftarray[];
  static const int32_t bitarray[];
  int32_t result_array[array_length];
  int32_t maskarray[array_length];
  int32_t modulemask;
};

#endif
