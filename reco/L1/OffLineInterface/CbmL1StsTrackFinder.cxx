/* Copyright (C) 2006-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Ivan Kisel, Sergey Gorbunov, Volker Friese, Denis Bertini [committer] */

/*
 *====================================================================
 *
 *  CBM Level 1 Reconstruction 
 *  
 *  Authors: I.Kisel,  S.Gorbunov
 *
 *  e-mail : ikisel@kip.uni-heidelberg.de 
 *
 *====================================================================
 *
 *  CbmL1StsTrackFinder source file
 *
 *====================================================================
 */
#include "CbmL1StsTrackFinder.h"

#include "CbmEvent.h"
#include "CbmKFMath.h"
#include "CbmStsHit.h"
#include "CbmStsTrack.h"

#include "FairHit.h"
#include "FairMCPoint.h"
#include "FairRootManager.h"

#include "TClonesArray.h"
#include "TDatabasePDG.h"

#include <iostream>
#include <vector>

#include "L1Algo/L1Algo.h"

using std::cout;
using std::endl;
using std::vector;

ClassImp(CbmL1StsTrackFinder)

  // -----   Default constructor   -------------------------------------------
  CbmL1StsTrackFinder::CbmL1StsTrackFinder()
{
  fName = "STS Track Finder L1";
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmL1StsTrackFinder::~CbmL1StsTrackFinder() {}
// -------------------------------------------------------------------------


// -----   Public method Init   --------------------------------------------
void CbmL1StsTrackFinder::Init() {}
// -------------------------------------------------------------------------


// -----   Copy tracks to output array   -----------------------------------
Int_t CbmL1StsTrackFinder::CopyL1Tracks(CbmEvent* event)
{
  CbmL1* L1 = CbmL1::Instance();
  if (!L1) return 0;

  Int_t trackIndex = fTracks->GetEntriesFast();
  Int_t nTracks    = 0;
  LOG(debug) << "Copy L1 tracks : " << L1->fvRecoTracks.size() << " tracks in L1";
  for (vector<CbmL1Track>::iterator it = L1->fvRecoTracks.begin(); it != L1->fvRecoTracks.end(); ++it) {
    CbmL1Track& T = *it;
    new ((*fTracks)[trackIndex]) CbmStsTrack();
    nTracks++;
    if (event) event->AddData(ECbmDataType::kStsTrack, trackIndex);
    CbmStsTrack* t = L1_DYNAMIC_CAST<CbmStsTrack*>(fTracks->At(trackIndex++));
    t->SetFlag(0);
    FairTrackParam fpar(*t->GetParamFirst()), lpar(*t->GetParamLast());
    CbmKFMath::CopyTC2TrackParam(&fpar, T.T, T.C);
    CbmKFMath::CopyTC2TrackParam(&lpar, T.TLast, T.CLast);
    t->SetParamFirst(&fpar);
    t->SetParamLast(&lpar);
    t->SetChiSq(T.chi2);
    t->SetNDF(T.NDF);
    t->SetPidHypo(T.T[4] >= 0 ? 211 : -211);
    t->SetStartTime(T.Tpv[6]);
    t->SetStartTimeError(T.Cpv[20]);
    t->SetFirstHitTime(T.T[6]);
    t->SetFirstHitTimeError(T.C[20]);
    t->SetLastHitTime(T.TLast[6]);
    t->SetLastHitTimeError(T.CLast[20]);

    for (vector<int>::iterator ih = it->Hits.begin(); ih != it->Hits.end(); ++ih) {
      CbmL1HitStore& h = L1->fvHitStore[*ih];
      // 	  double zref = L1->fpAlgo->vStations[h.iStation].z[0];
      if (h.Det > 1) {  // not MVD or STS hit
        continue;
      }
      if (h.ExtIndex < 0) {
        // CbmMvdHit tmp;
        // tmp.SetZ(zref);
        t->AddMvdHit(-h.ExtIndex - 1);  //, &tmp );
      }
      else {
        //CbmStsHit tmp;
        //tmp.SetZ(zref);
        t->AddHit(h.ExtIndex, kSTSHIT);  //, &tmp );
      }
    }
  }

  return nTracks;
}
// -------------------------------------------------------------------------


// -----   Public method DoFind   ------------------------------------------
Int_t CbmL1StsTrackFinder::DoFind()
{

  if (!fTracks) {
    LOG(error) << "-E- CbmL1StsTrackFinder::DoFind: "
               << "Track array missing! ";
    return -1;
  }

  CbmL1* L1 = CbmL1::Instance();
  if (!L1) return 0;

  L1->Reconstruct();
  int ntracks = CopyL1Tracks();

  return ntracks;
}
// -------------------------------------------------------------------------


// -----   Track finding in one event   ------------------------------------
Int_t CbmL1StsTrackFinder::FindTracks(CbmEvent* event)
{

  CbmL1* l1 = CbmL1::Instance();
  if (!l1) return 0;

  l1->Reconstruct(event);
  int nTracks = CopyL1Tracks(event);

  return nTracks;
}
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
void SetDefaultParticlePDG(int pdg = 211)
{
  /// set a default particle mass for the track fit
  /// it is used during reconstruction for the multiple scattering estimation
  CbmL1* l1 = CbmL1::Instance();
  if (!l1 || !l1->fpAlgo) {
    LOG(fatal) << "L1 instance doesn't exist or is not initialised";
    return;
  }
  auto* p = TDatabasePDG::Instance()->GetParticle(pdg);
  if (!p) {
    LOG(fatal) << "Particle with pdg " << pdg << " doesn't exist";
    return;
  }
  l1->fpAlgo->SetDefaultParticleMass(p->Mass());
}
// -------------------------------------------------------------------------
