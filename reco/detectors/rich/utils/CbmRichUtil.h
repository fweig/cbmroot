/* Copyright (C) 2017-2019 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer], Semen Lebedev */

#ifndef RICH_CbmRichUtil
#define RICH_CbmRichUtil

#include "CbmGlobalTrack.h"

#include "FairRootManager.h"
#include "FairTrackParam.h"
#include <Logger.h>

#include "TCanvas.h"
#include "TClonesArray.h"
#include "TH2.h"
#include "TObject.h"

#include <string>

using namespace std;

class CbmRichUtil {

public:
  static Double_t GetRingTrackDistance(Int_t globalTrackId)
  {
    vector<Double_t> v = GetRingTrackDistanceImpl(globalTrackId);
    return v[0];
  }

  static Double_t GetRingTrackDistanceX(Int_t globalTrackId)
  {
    vector<Double_t> v = GetRingTrackDistanceImpl(globalTrackId);
    return v[1];
  }

  static Double_t GetRingTrackDistanceY(Int_t globalTrackId)
  {
    vector<Double_t> v = GetRingTrackDistanceImpl(globalTrackId);
    return v[2];
  }

  static uint16_t GetDirichId(Int_t Address) { return ((Address >> 16) & 0xFFFF); }

  static uint16_t GetDirichChannel(Int_t Address) { return (Address & 0xFFFF); }

private:
  /**
	 * \brief Return a vector with total distance and x, y components. [0] - total distance, [1] - x component, [2] - y component
	 */
  static vector<Double_t> GetRingTrackDistanceImpl(Int_t globalTrackId)
  {
    vector<Double_t> errorVec = {999., 999., 999.};
    FairRootManager* ioman    = FairRootManager::Instance();
    if (ioman == NULL) return errorVec;
    // Do we really need static here, depends on ioman->GetObject() method
    static TClonesArray* globalTracks    = NULL;
    static TClonesArray* richRings       = NULL;
    static TClonesArray* richProjections = NULL;

    if (globalTracks == NULL || richRings == NULL || richProjections == NULL) {
      //cout << "globalTracks == NULL || richRings == NULL || richProjections == NULL" << endl;
      globalTracks    = (TClonesArray*) ioman->GetObject("GlobalTrack");
      richRings       = (TClonesArray*) ioman->GetObject("RichRing");
      richProjections = (TClonesArray*) ioman->GetObject("RichProjection");
    }
    else {
      //cout << "globalTracks, richRings, richProjections NOT NULL" << endl;
    }

    if (globalTracks == NULL || richRings == NULL || richProjections == NULL) {
      LOG(error) << "CbmRichUtil::GetRingTrackDistance globalTracks, "
                    "richRings, richProjections NOT INITIALIZED"
                 << endl;
      return errorVec;
    }

    const CbmGlobalTrack* globalTrack = static_cast<const CbmGlobalTrack*>(globalTracks->At(globalTrackId));
    if (globalTrack == NULL) return errorVec;

    Int_t stsId = globalTrack->GetStsTrackIndex();
    if (stsId < 0) return errorVec;

    const FairTrackParam* pTrack = static_cast<const FairTrackParam*>(richProjections->At(stsId));
    if (pTrack == NULL) return errorVec;

    if (pTrack->GetX() == 0 && pTrack->GetY() == 0) return errorVec;

    Int_t richId = globalTrack->GetRichRingIndex();
    if (richId < 0) return errorVec;

    const CbmRichRing* richRing = static_cast<const CbmRichRing*>(richRings->At(richId));
    if (richRing == NULL) return errorVec;

    //		Double_t xRing = richRing->GetCenterX();
    //		Double_t yRing = richRing->GetCenterY();
    Double_t dx = richRing->GetCenterX() - pTrack->GetX();
    Double_t dy = richRing->GetCenterY() - pTrack->GetY();

    Double_t dist = TMath::Sqrt(dx * dx + dy * dy);

    vector<Double_t> v = {dist, dx, dy};
    return v;
  }
};

#endif
