/* Copyright (C) 2009-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer], Florian Uhlig */

// -------------------------------------------------------------------------
// -----                   CbmMvdDetectorId header file                -----
// -----                 Created 22/04/09  by V. Friese                -----
// -------------------------------------------------------------------------


/** CbmMvdDetectorId.h
 ** Defines unique detector identifier for all MVD components. 
 ** Classes using this ID scheme should derive from this class.
 ** @author V.Friese <v.friese@gsi.de>
 **/


/** Current definition:
 ** System ID (kMVD=1) on bits 0-4
 ** Station number on bits 5-31
 **/


#ifndef CBMMVDDETECTORID_H
#define CBMMVDDETECTORID_H 1


#include "CbmDefs.h"  // for ECbmModuleId::kMvd

#include <Logger.h>  // for LOG

#include <Rtypes.h>      // for ClassDef

#include <cstdint>


class CbmMvdDetectorId {

public:
  /** Constructor **/
  CbmMvdDetectorId();


  /** Destructor **/
  virtual ~CbmMvdDetectorId() {}


  /** Create unique detector ID from station number **/
  int32_t DetectorId(int32_t iStation) const { return (ToIntegralType(ECbmModuleId::kMvd) | (iStation << 5)); }


  /** Get System identifier from detector ID **/
  int32_t SystemId(int32_t detectorId) const
  {
    int32_t iSystem = detectorId & 31;
    if (iSystem != ToIntegralType(ECbmModuleId::kMvd)) {
      LOG(error) << "wrong system ID " << iSystem;
      return -1;
    }
    return iSystem;
  }


  /** Get station number from detector ID **/
  int32_t StationNr(int32_t detectorId) const { return ((detectorId & (~31)) >> 5); }


  ClassDef(CbmMvdDetectorId, 1);
};


#endif
