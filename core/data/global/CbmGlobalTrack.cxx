/* Copyright (C) 2005-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese, Florian Uhlig, Denis Bertini [committer] */

// -------------------------------------------------------------------------
// -----                    CbmGlobalTrack source file                 -----
// -----                  Created 01/12/05  by V. Friese               -----
// -----                  Modified 04/06/09  by A. Lebedev             -----
// -------------------------------------------------------------------------
#include "CbmGlobalTrack.h"

#include <Logger.h>  // for Logger, LOG

#include <TObject.h>  // for TObject

// -----   Default constructor   -------------------------------------------
CbmGlobalTrack::CbmGlobalTrack() {}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmGlobalTrack::~CbmGlobalTrack() {}
// -------------------------------------------------------------------------


// -----   Public method Print   -------------------------------------------
void CbmGlobalTrack::Print(Option_t*) const
{
  LOG(info) << "StsTrack " << fStsTrack << ", TrdTrack " << fTrdTrack << ", MuchTrack " << fMuchTrack << ", RichRing "
            << fRichRing << ", TofHit " << fTofHit << ", TofTrack " << fTofTrack;
  //  LOG(info) << "Parameters at first plane: ";
  //  fParamFirst.Print();
  //  LOG(info) << "Parameters at last plane: ";
  //  fParamLast.Print();
  LOG(info) << "chi2 = " << fChi2 << ", NDF = " << fNDF << ", Quality flag " << fFlag;
  LOG(info) << "length = " << fLength;
}
// -------------------------------------------------------------------------


ClassImp(CbmGlobalTrack)
