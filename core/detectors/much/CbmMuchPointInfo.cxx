/* Copyright (C) 2007-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Evgeny Kryshen, Florian Uhlig [committer] */

// -------------------------------------------------------------------------
// -----                    CbmMuchPointInfo source file               -----
// -----                   Created 21/11/07 by E. Kryshen              -----
// -------------------------------------------------------------------------


#include "CbmMuchPointInfo.h"


ClassImp(CbmMuchPointInfo)

  // -------------------------------------------------------------------------
  CbmMuchPointInfo::CbmMuchPointInfo()
  : TObject()
  , fKine(0.)
  , fLength(0.)
  , fPdgCode(0)
  , fMotherPdg(0)
  , fCharge(0)
  , fStationId(0)
  , fS(0.)
  , fNPads(0)
{
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
CbmMuchPointInfo::CbmMuchPointInfo(Int_t pdgCode, Int_t motherPdg, Double_t kine, Double_t length, Int_t stationId)
  : TObject()
  , fKine(kine)
  , fLength(length)
  , fPdgCode(pdgCode)
  , fMotherPdg(motherPdg)
  , fCharge(0)
  , fStationId(stationId)
  , fS(0)
  , fNPads(0)
{
}
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
void CbmMuchPointInfo::Print(Option_t*) const
{
  printf("PointInfo:pdg=%i,kine=%6.4f,length=%4.2f,st_id=%i,charge=%i\n", fPdgCode, fKine, fLength, fStationId,
         fCharge);
}
// -------------------------------------------------------------------------
