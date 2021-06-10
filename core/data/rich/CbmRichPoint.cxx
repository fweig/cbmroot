/* Copyright (C) 2004-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Boris Polichtchouk, Denis Bertini [committer], Florian Uhlig */

// -------------------------------------------------------------------------
// -----                      CbmRichPoint source file                 -----
// -----               Created 28/04/04  by B. Polichtchouk            -----
// -------------------------------------------------------------------------

#include "CbmRichPoint.h"

#include <FairMCPoint.h>  // for FairMCPoint
#include <Logger.h>       // for Logger, LOG

#include <TVector3.h>  // for TVector3


// -----   Default constructor   -------------------------------------------
CbmRichPoint::CbmRichPoint() : FairMCPoint() {}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
CbmRichPoint::CbmRichPoint(Int_t trackID, Int_t detID, TVector3 pos, TVector3 mom, Double_t tof, Double_t length,
                           Double_t eLoss)
  : FairMCPoint(trackID, detID, pos, mom, tof, length, eLoss)
{
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmRichPoint::~CbmRichPoint() {}
// -------------------------------------------------------------------------


// -----   Public method Print   -------------------------------------------
void CbmRichPoint::Print(const Option_t* /*opt*/) const
{
  LOG(info) << "-I- CbmRichPoint: RICH Point for track " << fTrackID << " in detector " << fDetectorID;
  LOG(info) << "    Position (" << fX << ", " << fY << ", " << fZ << ") cm";
  LOG(info) << "    Momentum (" << fPx << ", " << fPy << ", " << fPz << ") GeV";
  LOG(info) << "    Time " << fTime << " ns,  Length " << fLength << " cm,  Energy loss " << fELoss * 1.0e06 << " keV";
}
// -------------------------------------------------------------------------


ClassImp(CbmRichPoint)
