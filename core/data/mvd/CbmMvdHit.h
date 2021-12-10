/* Copyright (C) 2006-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer], Philipp Sitzmann, Florian Uhlig */

#// -------------------------------------------------------------------------
// -----                      CbmMvdHit header file                    -----
// -----                 Created 07/11/06  by V. Friese                -----
// -------------------------------------------------------------------------


/** CbmMvdHit.h
 *@author V.Friese <v.friese@gsi.de>
 **
 ** Data class for hits in the CBM-MVD. 
 ** Data level RAW.
 ** Original source (CbmStsMapsHit) by M. Deveaux.
 ** 
 ** Hit flag not used up to now (will come later with real hit finding).
 **/


#ifndef CBMMVDHIT_H
#define CBMMVDHIT_H 1

#include "CbmMvdDetectorId.h"  // for CbmMvdDetectorId
#include "CbmPixelHit.h"       // for CbmPixelHit

#include <Rtypes.h>      // for ClassDef

#include <cstdint>

class TVector3;

class CbmMvdHit : public CbmPixelHit, public CbmMvdDetectorId {

public:
  /** Default constructor **/
  CbmMvdHit();


  /** Standard constructor 
  *@param statNr Station number
  *@param pos    Position coordinates [cm]
  *@param dpos   Errors in position coordinates [cm]
  *@param flag   Hit flag
  **/
  CbmMvdHit(int32_t statNr, TVector3& pos, TVector3& dpos, int32_t indexCentralX, int32_t indexCentralY = 0,
            int32_t fClusterIndex = 0, int32_t flag = 0);


  /** Destructor **/
  virtual ~CbmMvdHit();


  /** Output to screen **/
  virtual void Print(const Option_t* opt = 0) const;


  /** Accessors **/
  int32_t GetSystemId() const { return SystemId(fDetectorID); };
  virtual int32_t GetStationNr() const { return StationNr(fDetectorID); };
  int32_t GetFlag() const { return fFlag; };
  int32_t GetClusterIndex() const { return fClusterIndex; };
  int32_t GetIndexCentralX() const { return fIndexCentralX; };  // returns index of center of gravity
  int32_t GetIndexCentralY() const { return fIndexCentralY; };  // returns index of center of gravity
  //  void GetDigiIndexVector(TClonesArray* cbmMvdClusterArray, std::vector<int32_t>* digiIndexVector);
  int32_t GetRefIndex() { return fFlag; }


protected:
  int32_t fFlag;  // Hit flag; to be used later
  int32_t fClusterIndex;
  int32_t fIndexCentralX;
  int32_t fIndexCentralY;

  int32_t fDetectorID;


  ClassDef(CbmMvdHit, 3);
};


#endif
