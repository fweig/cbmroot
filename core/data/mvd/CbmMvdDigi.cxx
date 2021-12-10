/* Copyright (C) 2008-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Christina Dritsa [committer], Florian Uhlig, Philipp Sitzmann */

// -----------------------------------------------------------------------
// -----               CbmMvdDigi source file                        -----
// -----              Created 17/04/08  by C. Dritsa                 -----
// -----------------------------------------------------------------------

#include "CbmMvdDigi.h"

#include <Logger.h>  // for LOG, Logger

#include <TObject.h>  // for TObject

#include <cmath>

// -----   Default constructor   -------------------------------------------
CbmMvdDigi::CbmMvdDigi()
  : TObject()
  , CbmMvdDetectorId()
  , fCharge(0.)
  , fChannelNrX(0)
  , fChannelNrY(0)
  , fTrackID(-1)
  , fPointID(0)
  , fPixelSizeX(0.)
  , fPixelSizeY(0.)
  , fDetectorId(0)
  , fChannelNr(0)
  , fDigiTime(0.)
  , fFrameNumber(0)
  , fRefId(-1)
  , fDigiFlag(-1)
{
}
// -------------------------------------------------------------------------


/*
// -----   Constructor with parameters   -----------------------------------
CbmMvdDigi::CbmMvdDigi(int32_t iStation, int32_t iChannelNrX, int32_t iChannelNrY, float charge,
                       float pixelSizeX, float pixelSizeY)
  : CbmDigi(kMVD, 0),
    CbmMvdDetectorId(), 	
    fCharge(charge),
    fChannelNrX(iChannelNrX),
    fChannelNrY(iChannelNrY),
    fTrackID(-1),
    fPointID(0),
    fPixelSizeX(pixelSizeX),
    fPixelSizeY(pixelSizeY),
    fDetectorId(0),
    fChannelNr(0),
    fDigiTime(0.),
    fFrameNumber(0),
    fRefId(-1),
    fDigiFlag(-1)
{
    // Check range for station
    if ( ! ( iStation >= 0 && iStation <= 255 ) ) {
	LOG(fatal) << "Illegal station number " << iStation;
    }

    fDetectorId = DetectorId(iStation);

    fChannelNrY = iChannelNrY;
    fCharge  = charge;
    fChannelNrX=iChannelNrX;
    fChannelNrY=iChannelNrY;
    fPixelSizeX=pixelSizeX;
    fPixelSizeY=pixelSizeY;
    fDigiFlag=-1;
    
}
// -------------------------------------------------------------------------
*/

// -----   Constructor with parameters  --> used only due to error TODO include correct version -----------------------------------
CbmMvdDigi::CbmMvdDigi(int32_t iStation, int32_t iChannelNrX, int32_t iChannelNrY, float charge, float pixelSizeX,
                       float pixelSizeY, float time, int32_t frame)
  : TObject()
  , CbmMvdDetectorId()
  , fCharge(charge)
  , fChannelNrX(iChannelNrX)
  , fChannelNrY(iChannelNrY)
  , fTrackID(0)
  , fPointID(0)
  , fPixelSizeX(pixelSizeX)
  , fPixelSizeY(pixelSizeY)
  , fDetectorId(DetectorId(iStation))
  , fChannelNr(0)
  , fDigiTime(time)
  , fFrameNumber(frame)
  , fRefId(-1)
  , fDigiFlag(-1)
{
  // Check range for station
  if (!(iStation >= 0 && iStation <= 600)) { LOG(fatal) << "Illegal station number " << iStation; }
}
// -------------------------------------------------------------------------

int32_t CbmMvdDigi::GetAdcCharge(int32_t adcDynamic, int32_t adcOffset, int32_t adcBits)
{
  /**
     adcOffset  is the minimum value of the analogue signal
     adcDynamic is the difference between the max and min values of the full scale measurement range
     adcBits    is the number of bits used to encode the analogue signal

     Exemple:
     * If full scale measurement range is from 15 to 20 Volts
     * adcDynamic is the difference 20 - 15 = 5
     * adcOffset  = 15

     */

  int32_t adcCharge;

  if (fCharge < adcOffset) { return 0; };


  double stepSize;
  //    int32_t adcMax = adcOffset + adcDynamic;

  stepSize  = adcDynamic / pow(2, adcBits);
  adcCharge = int((fCharge - adcOffset) / stepSize);


  if (adcCharge > int(pow(2, adcBits) - 1)) { adcCharge = (int) pow(2, adcBits) - 1; }

  if (gDebug > 0) { LOG(debug) << "CbmMvdDigi::GetAdcCharge() " << adcCharge; }

  return adcCharge;
}


// -------------------------------------------------------------------------
int32_t CbmMvdDigi::GetPixelX() { return fChannelNrX; }
// -------------------------------------------------------------------------
int32_t CbmMvdDigi::GetPixelY() { return fChannelNrY; }
// -------------------------------------------------------------------------

/** Unique channel address  **/
int32_t CbmMvdDigi::GetAddress() const { return 0; }

// -------------------------------------------------------------------------

/** Absolute time [ns]  **/
double CbmMvdDigi::GetTime() const { return fDigiTime; }


// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmMvdDigi::~CbmMvdDigi() {}
// -------------------------------------------------------------------------

ClassImp(CbmMvdDigi)
