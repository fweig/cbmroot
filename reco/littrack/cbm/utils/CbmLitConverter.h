#ifndef CBMLITCONVERTER_H_
#define CBMLITCONVERTER_H_

#include "base/CbmLitEnums.h"
#include "base/CbmLitTypes.h"

#include "data/CbmLitFitNode.h"
#include "data/CbmLitHit.h"
#include "data/CbmLitPixelHit.h"
#include "data/CbmLitStripHit.h"
#include "data/CbmLitTofTrack.h"
#include "data/CbmLitTrack.h"
#include "data/CbmLitTrackParam.h"

#include "CbmLitConverterFairTrackParam.h"

#include "CbmEvent.h"
#include "CbmHit.h"
#include "CbmMuchGeoScheme.h"
#include "CbmMuchTrack.h"
#include "CbmMvdHit.h"
#include "CbmPixelHit.h"
#include "CbmStripHit.h"
#include "CbmStsTrack.h"
#include "CbmTofTrack.h"
#include "CbmTrack.h"
#include "CbmTrdTrack.h"
#include "FairTrackParam.h"

#include "CbmGlobalTrack.h"
#include "CbmTrdAddress.h"
#include "CbmVertex.h"

#include "TClonesArray.h"

#include "FairRootManager.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <set>


class CbmLitConverter {
public:
  static void CbmPixelHitToCbmLitPixelHit(const CbmPixelHit* hit,
                                          Int_t index,
                                          CbmLitPixelHit* litHit) {
    assert(hit->GetType() == kTRDHIT || hit->GetType() == kMUCHPIXELHIT
           || hit->GetType() == kTOFHIT || hit->GetType() == kMVDHIT
           || hit->GetType() == kSTSHIT || hit->GetType() == kPIXELHIT);

    litHit->SetX(hit->GetX());
    litHit->SetY(hit->GetY());
    litHit->SetZ(hit->GetZ());

    litHit->SetT(
      hit
        ->GetTime());  ///correction removed from here. The time for CbmMuchPixelHit is corrected now in CbmMuchFindHitsGem class

    litHit->SetDx(hit->GetDx());
    litHit->SetDy(hit->GetDy());
    litHit->SetDz(hit->GetDz());
    litHit->SetDxy(hit->GetDxy());

    if (hit->GetTimeError() > 0)
      litHit->SetDt(hit->GetTimeError());
    else
      litHit->SetDt(100);

    litHit->SetRefId(index);

    if (hit->GetType() == kTRDHIT) {
      litHit->SetDetectorId(kLITTRD, hit->GetPlaneId());
    } else if (hit->GetType() == kMUCHPIXELHIT) {
      litHit->SetDetectorId(kLITMUCH, (hit->GetPlaneId() - 1) / 2);
    } else if (hit->GetType() == kTOFHIT) {
      litHit->SetDetectorId(kLITTOF, 0);
    }
  }

  static void CbmMvdHitToCbmLitPixelHit(const CbmMvdHit* hit,
                                        Int_t index,
                                        CbmLitPixelHit* litHit) {
    litHit->SetX(hit->GetX());
    litHit->SetY(hit->GetY());
    litHit->SetZ(hit->GetZ());
    litHit->SetT(hit->GetTime());
    litHit->SetDx(hit->GetDx());
    litHit->SetDy(hit->GetDy());
    litHit->SetDz(hit->GetDz());
    litHit->SetDt(hit->GetTimeError());
    litHit->SetDxy(0.);
    litHit->SetRefId(index);

    litHit->SetDetectorId(kLITMVD, hit->GetStationNr());
  }

  static void CbmStsTrackToCbmLitTrack(const CbmStsTrack* stsTrack,
                                       CbmLitTrack* litTrack) {
    // TODO note that hits are not copied now

    litTrack->SetQuality(kLITGOOD);
    litTrack->SetChi2(stsTrack->GetChiSq());
    litTrack->SetNDF(stsTrack->GetNDF());
    litTrack->SetPreviousTrackId(-1);
    CbmLitTrackParam paramFirst, paramLast;
    //TODO remove this const typecasting
    CbmTrackParam cbmParamFirst;
    cbmParamFirst.Set(*stsTrack->GetParamFirst(),
                      stsTrack->GetTime(),
                      stsTrack->GetTimeError());
    CbmLitConverterFairTrackParam::FairTrackParamToCbmLitTrackParam(
      &cbmParamFirst, &paramFirst);
    CbmTrackParam cbmParamLast;
    cbmParamLast.Set(
      *stsTrack->GetParamLast(), stsTrack->GetTime(), stsTrack->GetTimeError());
    CbmLitConverterFairTrackParam::FairTrackParamToCbmLitTrackParam(
      &cbmParamLast, &paramLast);
    Double_t firstTime;
    Double_t lastTime;
    GetStsTrackTimes(stsTrack, firstTime, lastTime);
    paramFirst.SetTime(firstTime);
    paramLast.SetTime(lastTime);
    litTrack->SetParamFirst(&paramFirst);
    litTrack->SetParamLast(&paramLast);
  }

  static void CbmTrackToCbmLitTrack(const CbmTrack* track,
                                    const HitPtrVector& lhits,
                                    CbmLitTrack* ltrack) {
    for (Int_t iHit = 0; iHit < track->GetNofHits(); iHit++) {
      // Now we convert only pixel hits
      if (track->GetHitType(iHit) != kPIXELHIT
          && track->GetHitType(iHit) != kTRDHIT
          && track->GetHitType(iHit) != kMUCHPIXELHIT)
        continue;
      Int_t hitId = track->GetHitIndex(iHit);
      ltrack->AddHit(lhits[hitId]);
    }

    ltrack->SetQuality(kLITGOOD);
    ltrack->SetChi2(track->GetChiSq());
    ltrack->SetNDF(track->GetNDF());
    ltrack->SetPreviousTrackId(track->GetPreviousTrackId());
    ltrack->SetLastStationId(track->GetFlag());
    ltrack->SetPDG(track->GetPidHypo());
    CbmLitTrackParam paramFirst, paramLast;
    CbmTrackParam cbmParamFirst;
    cbmParamFirst.Set(
      *track->GetParamFirst(), track->GetTime(), track->GetTimeError());
    CbmLitConverterFairTrackParam::FairTrackParamToCbmLitTrackParam(
      &cbmParamFirst, &paramFirst);
    CbmTrackParam cbmParamLast;
    cbmParamLast.Set(
      *track->GetParamLast(), track->GetTime(), track->GetTimeError());
    CbmLitConverterFairTrackParam::FairTrackParamToCbmLitTrackParam(
      &cbmParamLast, &paramLast);
    ltrack->SetParamFirst(&paramFirst);
    ltrack->SetParamLast(&paramLast);
  }

  static void CbmTrackArrayToCbmLitTrackArray(const TClonesArray* tracks,
                                              const HitPtrVector& lhits,
                                              TrackPtrVector& ltracks) {
    Int_t nofTracks = tracks->GetEntriesFast();
    for (Int_t iTrack = 0; iTrack < nofTracks; iTrack++) {
      const CbmTrack* track = static_cast<const CbmTrack*>(tracks->At(iTrack));
      CbmLitTrack* ltrack   = new CbmLitTrack();
      CbmTrackToCbmLitTrack(track, lhits, ltrack);
      ltracks.push_back(ltrack);
    }
  }

  static void CbmLitTrackToCbmTrack(const CbmLitTrack* litTrack,
                                    CbmTrack* track,
                                    LitSystemId systemId) {
    Double_t chiSq = 0.;
    Int_t ndf      = 0;
    Int_t firstHit = -1;
    Int_t lastHit  = -1;
    for (Int_t iHit = 0; iHit < litTrack->GetNofHits(); iHit++) {
      const CbmLitHit* hit = litTrack->GetHit(iHit);
      LitHitType type      = hit->GetType();
      LitSystemId det      = hit->GetSystem();
      if (det != systemId) continue;
      if (firstHit < 0) firstHit = iHit;
      lastHit = iHit;
      if (det == kLITMUCH && type == kLITPIXELHIT) {
        ndf += 2;
        track->AddHit(hit->GetRefId(), kMUCHPIXELHIT);
      } else if (det == kLITTRD) {
        ndf += 2;
        track->AddHit(hit->GetRefId(), kTRDHIT);
      }
      chiSq += litTrack->GetFitNode(iHit)->GetChiSqFiltered();
    }
    ndf -= 5;
    if (ndf <= 0) ndf = 1;

    track->SetChiSq(chiSq);
    track->SetNDF(ndf);
    track->SetPreviousTrackId(litTrack->GetPreviousTrackId());
    track->SetFlag(litTrack->GetQuality());
    FairTrackParam parLast, parFirst;
    CbmLitConverterFairTrackParam::CbmLitTrackParamToFairTrackParam(
      litTrack->GetFitNode(lastHit)->GetUpdatedParam(), &parLast);
    CbmLitConverterFairTrackParam::CbmLitTrackParamToFairTrackParam(
      litTrack->GetFitNode(firstHit)->GetUpdatedParam(), &parFirst);
    track->SetParamLast(&parLast);
    track->SetParamFirst(&parFirst);
  }

  static void
  LitTrackVectorToGlobalTrackArray(CbmEvent* event,
                                   const TrackPtrVector& litTracks,
                                   const TofTrackPtrVector& litTofTracks,
                                   TClonesArray* globalTracks,
                                   TClonesArray* stsTracks,
                                   TClonesArray* trdTracks,
                                   TClonesArray* muchTracks,
                                   TClonesArray* tofTracks) {
    // Loop over STS tracks and create GlobalTrack for each StsTrack
    Int_t globalTrackNo = globalTracks->GetEntriesFast();
    Int_t nofStsTracks  = event ? event->GetNofData(ECbmDataType::kStsTrack)
                               : stsTracks->GetEntriesFast();
    for (Int_t i = 0; i < nofStsTracks; i++) {
      Int_t iTrack = event ? event->GetIndex(ECbmDataType::kStsTrack, i) : i;
      CbmGlobalTrack* globalTrack =
        new ((*globalTracks)[globalTrackNo++]) CbmGlobalTrack();
      globalTrack->SetStsTrackIndex(i);

      if (event) event->AddData(ECbmDataType::kGlobalTrack, iTrack);
    }

    Int_t trdTrackNo  = (trdTracks != NULL) ? trdTracks->GetEntriesFast() : 0;
    Int_t muchTrackNo = (muchTracks != NULL) ? muchTracks->GetEntriesFast() : 0;
    Int_t tofTrackNo  = (tofTracks != NULL) ? tofTracks->GetEntriesFast() : 0;

    for (size_t iTrack = 0; iTrack < litTracks.size(); iTrack++) {
      const CbmLitTrack* litTrack = litTracks[iTrack];

      if (litTrack->GetQuality() == kLITBAD) { continue; }
      if (litTrack->GetNofHits() < 1) { continue; }
      if (static_cast<size_t>(litTrack->GetNofHits())
          != litTrack->GetFitNodes().size()) {
        LOG(ERROR) << "CbmLitConverter::LitTrackVectorToGlobalTrackArray: "
                      "unequal number of hits and fit nodes"
                   << std::endl
                   << litTrack->ToString();
        continue;
      }

      CbmGlobalTrack* globalTrack = static_cast<CbmGlobalTrack*>(
        globalTracks->At(litTrack->GetPreviousTrackId()));

      // Set last parameter of the CbmGlobal track to be last parameter of CbmLitTrack
      FairTrackParam parLast;
      CbmLitConverterFairTrackParam::CbmLitTrackParamToFairTrackParam(
        litTrack->GetParamLast(), &parLast);
      globalTrack->SetParamLast(&parLast);

      Bool_t isCreateMuchTrack = false, isCreateTrdTrack = false;
      for (Int_t iHit = 0; iHit < litTrack->GetNofHits(); iHit++) {
        const CbmLitHit* thisHit = litTrack->GetHit(iHit);
        LitSystemId thisDetId    = thisHit->GetSystem();
        if (thisDetId == kLITMUCH && muchTracks != NULL) {
          isCreateMuchTrack = true;
        }
        if (thisDetId == kLITTRD && trdTracks != NULL) {
          isCreateTrdTrack = true;
        }
      }
      if (isCreateTrdTrack) {
        CbmTrdTrack* track = new ((*trdTracks)[trdTrackNo]) CbmTrdTrack();
        CbmLitTrackToCbmTrack(litTrack, track, kLITTRD);
        globalTrack->SetTrdTrackIndex(trdTrackNo);

        if (event) event->AddData(ECbmDataType::kTrdTrack, trdTrackNo);

        ++trdTrackNo;
      }
      if (isCreateMuchTrack) {
        CbmMuchTrack* track = new ((*muchTracks)[muchTrackNo]) CbmMuchTrack();
        CbmLitTrackToCbmTrack(litTrack, track, kLITMUCH);
        globalTrack->SetMuchTrackIndex(muchTrackNo);

        if (event) event->AddData(ECbmDataType::kMuchTrack, muchTrackNo);

        ++muchTrackNo;
      }
    }

    for (size_t iTrack = 0; iTrack < litTofTracks.size(); iTrack++) {
      const CbmLitTofTrack* litTofTrack = litTofTracks[iTrack];
      CbmTofTrack* track  = new ((*tofTracks)[tofTrackNo++]) CbmTofTrack();
      Int_t globalTrackId = litTofTrack->GetTrack()->GetPreviousTrackId();
      Int_t tofHitId      = litTofTrack->GetHit()->GetRefId();
      track->SetTofHitIndex(tofHitId);
      track->SetTrackIndex(globalTrackId);
      track->SetDistance(litTofTrack->GetDistance());
      FairTrackParam par;
      CbmLitConverterFairTrackParam::CbmLitTrackParamToFairTrackParam(
        litTofTrack->GetTrackParam(), &par);
      track->SetTrackParameter(&par);

      CbmGlobalTrack* globalTrack =
        static_cast<CbmGlobalTrack*>(globalTracks->At(globalTrackId));
      globalTrack->SetTofHitIndex(tofHitId);

      if (event) event->AddData(ECbmDataType::kTofHit, tofHitId);
    }
  }

  static void HitArrayToHitVector(CbmEvent* event,
                                  ECbmDataType hitDataType,
                                  const TClonesArray* hits,
                                  HitPtrVector& litHits) {
    Int_t nofHits =
      event ? event->GetNofData(hitDataType) : hits->GetEntriesFast();
    for (Int_t i = 0; i < nofHits; ++i) {
      Int_t iHit  = event ? event->GetIndex(hitDataType, i) : i;
      CbmHit* hit = (CbmHit*) hits->At(iHit);
      if (NULL == hit) { continue; }
      CbmLitPixelHit* litHit = new CbmLitPixelHit();
      CbmPixelHit* pixelHit  = static_cast<CbmPixelHit*>(hit);
      CbmPixelHitToCbmLitPixelHit(pixelHit, iHit, litHit);
      litHits.push_back(litHit);
    }
  }

  static void MvdHitArrayToHitVector(const TClonesArray* hits,
                                     HitPtrVector& litHits) {
    Int_t nofHits = hits->GetEntriesFast();
    for (Int_t iHit = 0; iHit < nofHits; iHit++) {
      CbmMvdHit* hit = static_cast<CbmMvdHit*>(hits->At(iHit));
      if (NULL == hit) { continue; }
      CbmLitPixelHit* litHit = new CbmLitPixelHit();
      CbmMvdHitToCbmLitPixelHit(hit, iHit, litHit);
      litHits.push_back(litHit);
    }
  }

  static void StsTrackArrayToTrackVector(CbmEvent* event,
                                         const TClonesArray* tracks,
                                         TrackPtrVector& litTracks) {
    Int_t nofTracks = event ? event->GetNofData(ECbmDataType::kStsTrack)
                            : tracks->GetEntriesFast();
    for (Int_t i = 0; i < nofTracks; ++i) {
      Int_t iTrack = event ? event->GetIndex(ECbmDataType::kStsTrack, i) : i;
      CbmStsTrack* track = (CbmStsTrack*) tracks->At(iTrack);
      if (track == NULL) { continue; }
      if (track->GetParamLast()->GetQp() == 0) { continue; }
      CbmLitTrack* litTrack = new CbmLitTrack;
      CbmStsTrackToCbmLitTrack(track, litTrack);
      litTrack->SetPreviousTrackId(iTrack);
      litTrack->SetRefId(iTrack);
      litTracks.push_back(litTrack);
    }
  }

  static void GetStsTrackTimes(const CbmStsTrack* track,
                               Double_t& firstTime,
                               Double_t& lastTime) {
    static FairRootManager* ioman = 0;
    static CbmVertex* primVertex  = 0;
    static TClonesArray* stsHits  = 0;
    static TClonesArray* mvdHits  = 0;
    static TrackPropagatorPtr propagator;
    static TrackUpdatePtr filter;
    static bool init = false;
    static Int_t pdg = 211;

    if (!init) {
      init  = true;
      ioman = FairRootManager::Instance();

      if (0 != ioman) {
        primVertex = static_cast<CbmVertex*>(ioman->GetObject("PrimaryVertex"));
        stsHits    = static_cast<TClonesArray*>(ioman->GetObject("StsHit"));
        mvdHits    = static_cast<TClonesArray*>(ioman->GetObject("MvdHit"));
      }

      propagator = CbmLitToolFactory::CreateTrackPropagator("lit");
      filter     = CbmLitToolFactory::CreateTrackUpdate("kalman");
    }

    Double_t xVert = primVertex ? primVertex->GetX() : 0;
    Double_t yVert = primVertex ? primVertex->GetY() : 0;
    Double_t zVert = primVertex ? primVertex->GetZ() : 0;
    CbmLitTrackParam paramFirst;
    CbmLitTrackParam paramLast;
    CbmTrackParam cbmParamFirst;
    cbmParamFirst.Set(
      *track->GetParamFirst(), track->GetTime(), track->GetTimeError());
    CbmLitConverterFairTrackParam::FairTrackParamToCbmLitTrackParam(
      &cbmParamFirst, &paramFirst);
    CbmTrackParam cbmParamLast;
    cbmParamLast.Set(
      *track->GetParamLast(), track->GetTime(), track->GetTimeError());
    CbmLitConverterFairTrackParam::FairTrackParamToCbmLitTrackParam(
      &cbmParamLast, &paramLast);

    Double_t x = paramFirst.GetX();
    Double_t y = paramFirst.GetY();
    Double_t z = paramFirst.GetZ();
    //Double_t p = paramFirst.GetQp() ? TMath::Abs(1 / paramFirst.GetQp()) : 1; (VF) unused
    CbmLitTrackParam par = paramFirst;
    Double_t deltaTFirst = 0;

    if (propagator->Propagate(&par, zVert, pdg) == kLITERROR)
      deltaTFirst =
        -TMath::Sqrt(TMath::Power(x - xVert, 2) + TMath::Power(y - yVert, 2)
                     + TMath::Power(z - zVert, 2))
        / CbmLitTrackParam::fSpeedOfLight;
    else
      deltaTFirst = par.GetTime() - paramFirst.GetTime();

    if (deltaTFirst > 0) deltaTFirst = -deltaTFirst;

    paramFirst.SetTime(paramFirst.GetTime() - deltaTFirst);
    firstTime = paramFirst.GetTime();
    par       = paramFirst;
    //int nofHits = track->GetNofHits();
    int nofMvdHits      = track->GetNofMvdHits();
    int nofStsHits      = track->GetNofStsHits();
    int nofHits         = nofMvdHits + nofStsHits;
    Double_t deltaTLast = 0;

    for (int i = 1; i < nofHits; ++i) {
      //HitType hitType = track->GetHitType(i);
      HitType hitType = (i < nofMvdHits) ? kMVDHIT : kSTSHIT;
      //Int_t hitInd = track->GetHitIndex(i);
      Int_t hitInd = (i < nofMvdHits) ? track->GetMvdHitIndex(i)
                                      : track->GetStsHitIndex(i - nofMvdHits);
      CbmPixelHit* hit = static_cast<CbmPixelHit*>(
        kMVDHIT == hitType ? mvdHits->At(hitInd) : stsHits->At(hitInd));

      if (i == nofHits - 1)
        z = paramLast.GetZ();
      else
        z = hit->GetZ();

      if (propagator->Propagate(&par, z, pdg) == kLITERROR) {
        deltaTLast =
          TMath::Sqrt(TMath::Power(paramLast.GetX() - paramFirst.GetX(), 2)
                      + TMath::Power(paramLast.GetY() - paramFirst.GetY(), 2)
                      + TMath::Power(paramLast.GetZ() - paramFirst.GetZ(), 2))
          / CbmLitTrackParam::fSpeedOfLight;
        break;
      }

      CbmLitPixelHit litHit;
      CbmPixelHitToCbmLitPixelHit(hit, hitInd, &litHit);
      litfloat chi = 0;
      filter->Update(&par, &litHit, chi);
    }

    if (0 == deltaTLast)
      lastTime = par.GetTime();
    else
      lastTime = firstTime + deltaTLast;
  }
};

#endif /*CBMLITCONVERTER_H_*/
