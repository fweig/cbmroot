/* Copyright (C) 2008-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Christina Dritsa [committer], Philipp Sitzmann, Florian Uhlig, Volker Friese */

// -------------------------------------------------------------------------
// -----                     CbmMvdDigi header file                    -----
// -----                    Created 02/04/08  by C.Dritsa              -----
// -------------------------------------------------------------------------

// TODO: Include GetAddress,  GetLinks, GetTime

#ifndef CBMMVDDIGI_H
#define CBMMVDDIGI_H 1

#include "CbmDefs.h"           // for ECbmModuleId::kMvd
#include "CbmMvdDetectorId.h"  // for CbmMvdDetectorId

#include <Rtypes.h>      // for ClassDef
#include <TObject.h>     // for TObject

#include <cstdint>
#include <string>  // for string


class CbmMvdDigi : public TObject, public CbmMvdDetectorId {

public:
  /** Default constructor **/
  CbmMvdDigi();

  /** Constructor with all variables **/

  CbmMvdDigi(int32_t iStation, int32_t iChannelNrX, int32_t iChannelNrY, float charge, float pixelSizeX,
             float pixelSizeY, float time = 0.0, int32_t frame = 0);
  /**
     charge     : of each fired pixel in electrons
     PixelSize  : in cm
    */

  /** Destructor **/
  ~CbmMvdDigi();

  static const char* GetClassName() { return "CbmMvdDigi"; }
  static ECbmModuleId GetSystem() { return ECbmModuleId::kMvd; }

  /** Accessors **/
  double GetCharge() const { return fCharge; };
  int32_t GetPixelX();
  int32_t GetPixelY();
  double GetPixelSizeX() { return fPixelSizeX; };
  double GetPixelSizeY() { return fPixelSizeY; };
  int32_t GetAdcCharge(int32_t adcDynamic, int32_t adcOffset, int32_t adcBits);
  int32_t GetFlag() { return fDigiFlag; };
  int32_t GetStationNr() { return StationNr(fDetectorId); };
  int32_t GetDetectorId() { return fDetectorId; };
  int32_t GetAddress() const; /** Unique channel address  **/
  double GetTime() const;     /** Absolute time [ns]  **/
  int32_t GetFrameNumber() { return fFrameNumber; };


  int32_t GetRefId() const { return fRefId; };

  std::string ToString() const { return ""; }

  //these functions are only for littrack
  int32_t GetDominatorX() { return 0; };
  int32_t GetDominatorY() { return 0; };
  int32_t GetTrackID() { return 0; };
  int32_t GetContributors() { return 0; };
  int32_t GetPointID() { return 0; };
  //


  /** Modifiers **/
  void SetCharge(float charge) { fCharge = charge; };
  void SetPixelX(int32_t xIndex) { fChannelNrX = xIndex; };
  void SetPixelY(int32_t yIndex) { fChannelNrY = yIndex; };
  void SetPixelIndex(int32_t pixelIndex) { fChannelNr = pixelIndex; };
  void SetDetectorId(int32_t detId) { fDetectorId = detId; };
  void SetFlag(int32_t flag) { fDigiFlag = flag; }
  void SetFrameNr(int32_t frame) { fFrameNumber = frame; };
  void SetRefId(int32_t refId) { fRefId = refId; }
  void SetTime(double time) { fDigiTime = time; }

private:
  float fCharge;
  int32_t fChannelNrX;
  int32_t fChannelNrY;
  int32_t fTrackID;
  int32_t fPointID;
  float fPixelSizeX;
  float fPixelSizeY;
  int32_t fDetectorId;
  int32_t fChannelNr;
  double fDigiTime;
  int32_t fFrameNumber;
  int32_t fRefId;


  int32_t fDigiFlag;  // Debugging and analysis information

  ClassDef(CbmMvdDigi, 2);
};


#endif
