/* Copyright (C) 2013-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

// -------------------------------------------------------------------------
// -----                   CbmTofDetectorId header file                -----
// -----                 Created 29/05/09  by F. Uhlig                 -----
// -------------------------------------------------------------------------


/** CbmTofDetectorId.h
 ** Abstract base class for the different implementations to calculate
 ** the information about TOF modules from a unique detetcor ID
 ** @author F. Uhlig <f.uhlig@gsi.de>
 **/

#ifndef CBMTOFDETECTORID_H
#define CBMTOFDETECTORID_H 1

#include "CbmDefs.h"

#include <cstdint>

class CbmTofDetectorInfo {

public:
  CbmTofDetectorInfo()
    : fDetectorSystem(0)
    , fSMtype(0)
    , fSModule(0)
    , fCounter(0)
    , fGap(0)
    , fCell(0)
    , fCounterType(0) {};

  CbmTofDetectorInfo(int32_t detsystem, int32_t smtype, int32_t smodule, int32_t counter, int32_t gap, int32_t cell)
    : fDetectorSystem(detsystem)
    , fSMtype(smtype)
    , fSModule(smodule)
    , fCounter(counter)
    , fGap(gap)
    , fCell(cell)
    , fCounterType(0) {};

  CbmTofDetectorInfo(ECbmModuleId detsystem, int32_t smtype, int32_t smodule, int32_t counter, int32_t gap,
                     int32_t cell)
    : fDetectorSystem(ToIntegralType(detsystem))
    , fSMtype(smtype)
    , fSModule(smodule)
    , fCounter(counter)
    , fGap(gap)
    , fCell(cell)
    , fCounterType(0) {};

  CbmTofDetectorInfo(ECbmModuleId detsystem, int32_t smtype, int32_t smodule, int32_t counter, int32_t gap,
                     int32_t cell, int32_t counterType)
    : fDetectorSystem(ToIntegralType(detsystem))
    , fSMtype(smtype)
    , fSModule(smodule)
    , fCounter(counter)
    , fGap(gap)
    , fCell(cell)
    , fCounterType(counterType) {};

  int32_t fDetectorSystem;
  int32_t fSMtype;
  int32_t fSModule;
  int32_t fCounter;
  int32_t fGap;
  int32_t fCell;
  int32_t fCounterType;
};


class CbmTofDetectorId {

public:
  /** Constructor **/
  CbmTofDetectorId() { ; }


  /** Destructor **/
  virtual ~CbmTofDetectorId() { ; }

  /** Get System identifier from detector ID **/
  //  int32_t GetSystemId(const int32_t detectorId);

  /** Get complete system info from detector ID
   ** This will return a pointer to an integer
   ** array of length array_length
   **/
  virtual CbmTofDetectorInfo GetDetectorInfo(const int32_t detectorId) = 0;

  /** Get the global sytem ID **/
  virtual int32_t GetSystemId(int32_t detectorId) = 0;

  /** Get SMType from detector ID **/
  virtual int32_t GetSMType(const int32_t detectorId) = 0;

  /** Get SModule number from detector ID **/
  virtual int32_t GetSModule(const int32_t detectorId) = 0;

  /** Get counter number from detector ID **/
  virtual int32_t GetCounter(const int32_t detectorId) = 0;

  /** Get gap number from detector ID **/
  virtual int32_t GetGap(const int32_t detectorId) = 0;

  /** Get cell number from detector ID **/
  virtual int32_t GetCell(const int32_t detectorId) = 0;

  /** Get region number from detector ID **/
  virtual int32_t GetRegion(const int32_t detectorId) = 0;

  /** Get cell number from detector ID **/
  virtual int32_t GetCellId(const int32_t detectorId) = 0;

  /** Calculate the unique detector ID
   ** This will return a pointer to an integer
   ** array of length array_length
   **/
  virtual int32_t SetDetectorInfo(const CbmTofDetectorInfo detectorInfo) = 0;

  //  char* itoa(int value, char* result, int base);

private:
};

#endif
