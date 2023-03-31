/* Copyright (C) 2008-2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Christina Dritsa [committer], Samir Amar-Youcef, Florian Uhlig, Philipp Sitzmann */

#include "CbmMvdPixelCharge.h"

#include <Logger.h>

#include <iostream>


using std::cout;
using std::endl;

// -------------------------------------------------------------------------
Bool_t CbmMvdPixelCharge::TestXY(Int_t channelNrX, Int_t channelNrY)
{

  if ((channelNrX == fChannelNrX) && (channelNrY == fChannelNrY)) { return 1; }
  else {
    return 0;
  };
}

// -----   Constructor with parameters   -----------------------------------
CbmMvdPixelCharge::CbmMvdPixelCharge(Float_t charge, Int_t channelNrX, Int_t channelNrY, Int_t pointId, Int_t trackId,
                                     Float_t pointPosX, Float_t pointPosY, Float_t time, Int_t frame)
  : TObject()
  , fFrame(frame)
  , fCharge(charge)
  , fMaxChargeContribution(charge)
  , fChannelNrX(channelNrX)
  , fChannelNrY(channelNrY)
  , fTrackCharge(charge)
  , fPixelTime(time)
{
  fTrackId.push_back(trackId);
  fPointId.push_back(pointId);
  fPointWeight.push_back(charge);
  fPointX.push_back(pointPosX);
  fPointY.push_back(pointPosY);
  fTime.push_back(time);
  //  fLink.push_back(); // TODO: pass link
  fDominatorIndex = fPointWeight.size();
}

// ------- DigestCharge ----------------------------------------------------#

// Sums up the charge stored in track charge (assuming this is the summed charge from
// all segments of a track). Checks if a new track contributed charge to the pixel
// Checks if the new track is dominant

void CbmMvdPixelCharge::DigestCharge(Float_t pointX, Float_t pointY, Double_t time, Int_t pointId, Int_t trackId)
{
  Float_t chargeContr = fTrackCharge;

  for (auto charge : fPointWeight) {
    chargeContr -= charge;
  }

  if (chargeContr > 0.) {
    fCharge = fTrackCharge;
    fTrackId.push_back(trackId);
    fPointId.push_back(pointId);

    fPointWeight.push_back(chargeContr);
    fPointX.push_back(pointX);
    fPointY.push_back(pointY);
    fTime.push_back(time);

    //  fTime.push_back(time);
    //  fLink.push_back(); // TODO: pass link

    if (chargeContr > fMaxChargeContribution) {
      fDominatorIndex        = fPointWeight.size();
      fMaxChargeContribution = chargeContr;
    }
  }
}

ClassImp(CbmMvdPixelCharge)
