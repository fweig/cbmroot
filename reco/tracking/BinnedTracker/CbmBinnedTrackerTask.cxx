/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "CbmBinnedTrackerTask.h"
#include "CbmMuchTrack.h"
#include "CbmStsTrack.h"
#include "CbmTrdTrack.h"
#include <Logger.h>
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "GeoReader.h"
#include "global/CbmGlobalTrack.h"

#ifdef __MACH__
#include <mach/mach_time.h>
#include <sys/time.h>
#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME 0
#endif
#ifndef CLOCK_MONOTONIC
#define CLOCK_MONOTONIC 0
#endif
inline int clock_gettime(int /*clk_id*/, struct timespec* t) {
  mach_timebase_info_data_t timebase;
  mach_timebase_info(&timebase);
  uint64_t time;
  time = mach_absolute_time();
  double nseconds =
    ((double) time * (double) timebase.numer) / ((double) timebase.denom);
  double seconds =
    ((double) time * (double) timebase.numer) / ((double) timebase.denom * 1e9);
  t->tv_sec  = seconds;
  t->tv_nsec = nseconds;
  return 0;
}
#else
#include <time.h>
#endif

using std::copy;
using std::fill_n;

CbmBinnedTrackerTask* CbmBinnedTrackerTask::fInstance = 0;

CbmBinnedTrackerTask::CbmBinnedTrackerTask(bool useAllDetectors,
                                           Double_t beamWidthX,
                                           Double_t beamWidthY)
  : fUseAllDetectors(useAllDetectors)
  , fIsOnlyPrimary(false)
  , fChiSqCut(0)
  , fCanSkipHits(-1 /*Negative value means: calculate the value*/)
  , fSettings(0)
  , fBeamDx(beamWidthX)
  , fBeamDy(beamWidthY)
  , fTracker(0)
  , fGlobalTracks(0)
  , fStsTracks(0)
  , fMuchTracks(0)
  , fTrdTracks(0) {
  fInstance = this;
  fill_n(fUseModules, int(ECbmModuleId::kLastModule), fUseAllDetectors);
  fUseModules[ToIntegralType(ECbmModuleId::kRich)] = false;  // Temporary hack
}

CbmBinnedTrackerTask::~CbmBinnedTrackerTask() { delete fTracker; }

InitStatus CbmBinnedTrackerTask::Init() {
  fSettings = CbmBinnedSettings::Instance();
  fSettings->SetConfiguring(false);
  fSettings->SetOnlyPrimary(fIsOnlyPrimary);
  fSettings->SetUse(fUseModules);
  fSettings->SetNofStsStations(0);
  fSettings->SetNofMuchStations(0);
  fSettings->SetNofTrdStations(0);
  fSettings->SetNofStations(0);
  CbmBinnedGeoReader* geoReader = CbmBinnedGeoReader::Instance();

  if (0 == geoReader) LOG(fatal) << "Couldn't instantiate CbmBinnedGeoReader";

  geoReader->Read();
  fTracker = CbmBinnedTracker::Instance();
  fTracker->Init();

  if (fChiSqCut) fTracker->SetChiSqCut(fChiSqCut);

  if (fCanSkipHits >= 0) fTracker->SetCanSkipHits(fCanSkipHits);

  FairRootManager* ioman = FairRootManager::Instance();

  if (0 == ioman) LOG(fatal) << "No FairRootManager";

  fGlobalTracks = new TClonesArray("CbmGlobalTrack", 100);
  ioman->Register("GlobalTrack",
                  "Global",
                  fGlobalTracks,
                  IsOutputBranchPersistent("GlobalTrack"));

  LOG(info) << "Use STS detector: "
            << (fSettings->Use(ECbmModuleId::kSts) ? "true" : "false");
  ;
  LOG(info) << "The number of STS stations: " << fSettings->GetNofStsStations();
  LOG(info) << "Use MuCh detector: "
            << (fSettings->Use(ECbmModuleId::kMuch) ? "true" : "false");
  LOG(info) << "The number of MuCh stations: "
            << fSettings->GetNofMuchStations();
  LOG(info) << "Use TRD detector: "
            << (fSettings->Use(ECbmModuleId::kTrd) ? "true" : "false");
  LOG(info) << "The number of TRD stations: " << fSettings->GetNofTrdStations();
  LOG(info) << "Use ToF detector: "
            << (fSettings->Use(ECbmModuleId::kTof) ? "true" : "false");

  if (fSettings->Use(ECbmModuleId::kSts)) {
    fStsTracks = new TClonesArray("CbmStsTrack", 100);
    ioman->Register(
      "StsTrack", "STS", fStsTracks, IsOutputBranchPersistent("StsTrack"));
  }

  if (fSettings->Use(ECbmModuleId::kMuch)) {
    fMuchTracks = new TClonesArray("CbmMuchTrack", 100);
    ioman->Register(
      "MuchTrack", "Much", fMuchTracks, IsOutputBranchPersistent("MuchTrack"));
  }

  if (fSettings->Use(ECbmModuleId::kTrd)) {
    fTrdTracks = new TClonesArray("CbmTrdTrack", 100);
    ioman->Register(
      "TrdTrack", "Trd", fTrdTracks, IsOutputBranchPersistent("TrdTrack"));
  }

  fSettings->setChanged();
  fSettings->setInputVersion(-2, 1);

  return kSUCCESS;
}

static long fullDuration = 0;

void CbmBinnedTrackerTask::Exec(Option_t*) {
  timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  long beginTime = ts.tv_sec * 1000000000 + ts.tv_nsec;
  fTracker->Reconstruct(-100);

  if (fSettings->Use(ECbmModuleId::kSts)) fStsTracks->Clear();

  if (fSettings->Use(ECbmModuleId::kMuch)) fMuchTracks->Clear();

  if (fSettings->Use(ECbmModuleId::kTrd)) fTrdTracks->Clear();

  fGlobalTracks->Clear();
  int globalTrackNumber = 0;
  int stsTrackNumber    = 0;
  int muchTrackNumber   = 0;
  int trdTrackNumber    = 0;
  std::list<CbmBinnedTracker::Track*>::const_iterator tracksEnd =
    fTracker->GetTracksEnd();

  for (std::list<CbmBinnedTracker::Track*>::const_iterator trackIter =
         fTracker->GetTracksBegin();
       trackIter != tracksEnd;
       ++trackIter) {
    const CbmBinnedTracker::Track* recoTrack = *trackIter;

    if (recoTrack->fIsClone) continue;

    //int previousGlobalTrackId = -1; (VF) unused
    //int previousStsTrackId = -1; (VF) unused
    //int previousMuchTrackId = -1; (VF) unused
    //int previousTrdTrackId = -1; (VF) unused
    int stsStationNumber  = 0;
    int muchStationNumber = 0;
    int trdStationNumber  = 0;
    int tofStationNumber  = 0;
    Double_t cov[]        = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    //Double_t parCov[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    FairTrackParam trackParam;
    CbmGlobalTrack* globalTrack =
      new ((*fGlobalTracks)[globalTrackNumber++]) CbmGlobalTrack();
    CbmStsTrack* stsTrack   = 0;
    CbmMuchTrack* muchTrack = 0;
    CbmTrdTrack* trdTrack   = 0;

    for (int hitNo = 0; hitNo < recoTrack->fLength; ++hitNo) {
      CbmTBin::HitHolder* hitHolder = recoTrack->fHits[hitNo];

      switch (hitHolder->type) {
        case ECbmModuleId::kSts:
          if (0 == stsTrack) {
            stsTrack = new ((*fStsTracks)[stsTrackNumber]) CbmStsTrack();
            globalTrack->SetStsTrackIndex(stsTrackNumber++);
          }

          ++stsStationNumber;
          stsTrack->AddStsHit(hitHolder->index);
          break;

        case ECbmModuleId::kMuch:
          if (0 == muchTrack) {
            muchTrack = new ((*fMuchTracks)[muchTrackNumber]) CbmMuchTrack();
            globalTrack->SetMuchTrackIndex(muchTrackNumber++);
          }

          ++muchStationNumber;
          muchTrack->AddHit(hitHolder->index, kMUCHPIXELHIT);
          break;

        case ECbmModuleId::kTrd:
          if (0 == trdTrack) {
            trdTrack = new ((*fTrdTracks)[trdTrackNumber]) CbmTrdTrack();
            globalTrack->SetTrdTrackIndex(trdTrackNumber++);
          }

          ++trdStationNumber;
          trdTrack->AddHit(hitHolder->index, kTRDHIT);
          break;

        case ECbmModuleId::kTof:
          globalTrack->SetTofHitIndex(hitHolder->index);
          ++tofStationNumber;
          break;
        default:
          LOG(fatal) << "This hit type is not accepted. Only sts, much, trd "
                        "and tof hits are accepted.";
      }
    }

    int lastStationNumber = stsStationNumber + muchStationNumber
                            + trdStationNumber + tofStationNumber - 1;

    trackParam.SetX(recoTrack->fParams[0].GetX());
    trackParam.SetY(recoTrack->fParams[0].GetY());
    trackParam.SetZ(recoTrack->fHits[0]->hit->GetZ());
    trackParam.SetTx(recoTrack->fParams[0].GetTx());
    trackParam.SetTy(recoTrack->fParams[0].GetTy());
    cov[0] = recoTrack->fParams[0].GetCovXX();  //parCov[0];
    cov[5] = recoTrack->fParams[0].GetCovYY();  //parCov[6];
    trackParam.SetCovMatrix(cov);
    globalTrack->SetParamFirst(&trackParam);

    trackParam.SetX(recoTrack->fParams[lastStationNumber].GetX());
    trackParam.SetY(recoTrack->fParams[lastStationNumber].GetY());
    trackParam.SetZ(recoTrack->fHits[lastStationNumber]->hit->GetZ());
    trackParam.SetTx(recoTrack->fParams[lastStationNumber].GetTx());
    trackParam.SetTy(recoTrack->fParams[lastStationNumber].GetTy());
    cov[0] = recoTrack->fParams[lastStationNumber].GetCovXX();  //parCov[0];
    cov[5] = recoTrack->fParams[lastStationNumber].GetCovYY();  //parCov[6];
    trackParam.SetCovMatrix(cov);
    globalTrack->SetParamLast(&trackParam);

    globalTrack->SetNDF((lastStationNumber + 1) * 2);
    globalTrack->SetChi2(recoTrack->fChiSq);

    /*if (fSettings->Use(kSts))
      {
         globalTrack->SetStsTrackIndex(trackNumber);
         CbmStsTrack* stsTrack = new ((*fStsTracks)[trackNumber]) CbmStsTrack();
         stsTrack->SetNDF(fSettings->GetNofStsStations() * 2);
         stsTrack->SetChiSq(1);
         stsTrack->SetPreviousTrackId(previousTrackId);
         previousTrackId = trackNumber;
         nofStations += fSettings->GetNofStsStations();
         
         trackParam.SetX(recoTrack->fParams[stationNumber].GetX());
         trackParam.SetY(recoTrack->fParams[stationNumber].GetY());
         trackParam.SetZ(recoTrack->fHits[stationNumber]->hit->GetZ());
         trackParam.SetTx(recoTrack->fParams[stationNumber].GetTx());
         trackParam.SetTy(recoTrack->fParams[stationNumber].GetTy());
         //recoTrack->fParams[stationNumber].CovMatrix(parCov);
         cov[0] = recoTrack->fParams[stationNumber].GetCovXX();//parCov[0];
         cov[5] = recoTrack->fParams[stationNumber].GetCovYY();//parCov[6];
         trackParam.SetCovMatrix(cov);
         stsTrack->SetParamFirst(&trackParam);

         trackParam.SetX(recoTrack->fParams[nofStations - 1].GetX());
         trackParam.SetY(recoTrack->fParams[nofStations - 1].GetY());
         trackParam.SetZ(recoTrack->fHits[nofStations - 1]->hit->GetZ());
         trackParam.SetTx(recoTrack->fParams[nofStations - 1].GetTx());
         trackParam.SetTy(recoTrack->fParams[nofStations - 1].GetTy());
         //recoTrack->fParams[stationNumber - 1].CovMatrix(parCov);
         cov[0] = recoTrack->fParams[nofStations - 1].GetCovXX();//parCov[0];
         cov[5] = recoTrack->fParams[nofStations - 1].GetCovYY();//parCov[6];
         stsTrack->SetParamLast(&trackParam);
         
         for (; stationNumber < nofStations; ++stationNumber)
         {
            CbmTBin::HitHolder* hh = recoTrack->fHits[stationNumber];
            stsTrack->AddStsHit(hh->index);
         }
      }
      
      if (fSettings->Use(kMuch))
      {
         globalTrack->SetMuchTrackIndex(trackNumber);
         CbmMuchTrack* muchTrack = new ((*fMuchTracks)[trackNumber]) CbmMuchTrack();
         muchTrack->SetNDF(fSettings->GetNofMuchStations() * 2);
         muchTrack->SetChiSq(1);
         muchTrack->SetPreviousTrackId(previousTrackId);
         previousTrackId = trackNumber;
         nofStations += fSettings->GetNofMuchStations();
         
         trackParam.SetX(recoTrack->fParams[stationNumber].GetX());
         trackParam.SetY(recoTrack->fParams[stationNumber].GetY());
         trackParam.SetZ(recoTrack->fHits[stationNumber]->hit->GetZ());
         trackParam.SetTx(recoTrack->fParams[stationNumber].GetTx());
         trackParam.SetTy(recoTrack->fParams[stationNumber].GetTy());
         //recoTrack->fParams[stationNumber].CovMatrix(parCov);
         cov[0] = recoTrack->fParams[stationNumber].GetCovXX();//parCov[0];
         cov[5] = recoTrack->fParams[stationNumber].GetCovYY();//parCov[6];
         trackParam.SetCovMatrix(cov);
         muchTrack->SetParamFirst(&trackParam);

         trackParam.SetX(recoTrack->fParams[nofStations - 1].GetX());
         trackParam.SetY(recoTrack->fParams[nofStations - 1].GetY());
         trackParam.SetZ(recoTrack->fHits[nofStations - 1]->hit->GetZ());
         trackParam.SetTx(recoTrack->fParams[nofStations - 1].GetTx());
         trackParam.SetTy(recoTrack->fParams[nofStations - 1].GetTy());
         //recoTrack->fParams[stationNumber - 1].CovMatrix(parCov);
         cov[0] = recoTrack->fParams[nofStations - 1].GetCovXX();//parCov[0];
         cov[5] = recoTrack->fParams[nofStations - 1].GetCovYY();//parCov[6];
         trackParam.SetCovMatrix(cov);
         muchTrack->SetParamLast(&trackParam);
      
         for (; stationNumber < nofStations; ++stationNumber)
         {
            CbmTBin::HitHolder* hh = recoTrack->fHits[stationNumber];
            muchTrack->AddHit(hh->index, kMUCHPIXELHIT);
         }
      }
      
      if (fSettings->Use(kTrd))
      {
         globalTrack->SetTrdTrackIndex(trackNumber);
         CbmTrdTrack* trdTrack = new ((*fTrdTracks)[trackNumber]) CbmTrdTrack();
         trdTrack->SetNDF(fSettings->GetNofTrdStations() * 2);
         trdTrack->SetChiSq(1);
         trdTrack->SetPreviousTrackId(previousTrackId);
         previousTrackId = trackNumber;
         nofStations += fSettings->GetNofTrdStations();
         
         trackParam.SetX(recoTrack->fParams[stationNumber].GetX());
         trackParam.SetY(recoTrack->fParams[stationNumber].GetY());
         trackParam.SetZ(recoTrack->fHits[stationNumber]->hit->GetZ());
         trackParam.SetTx(recoTrack->fParams[stationNumber].GetTx());
         trackParam.SetTy(recoTrack->fParams[stationNumber].GetTy());
         //recoTrack->fParams[stationNumber].CovMatrix(parCov);
         cov[0] = recoTrack->fParams[stationNumber].GetCovXX();//parCov[0];
         cov[5] = recoTrack->fParams[stationNumber].GetCovYY();//parCov[6];
         trackParam.SetCovMatrix(cov);
         trdTrack->SetParamFirst(&trackParam);

         trackParam.SetX(recoTrack->fParams[nofStations - 1].GetX());
         trackParam.SetY(recoTrack->fParams[nofStations - 1].GetY());
         trackParam.SetZ(recoTrack->fHits[nofStations - 1]->hit->GetZ());
         trackParam.SetTx(recoTrack->fParams[nofStations - 1].GetTx());
         trackParam.SetTy(recoTrack->fParams[nofStations - 1].GetTy());
         //recoTrack->fParams[stationNumber - 1].CovMatrix(parCov);
         cov[0] = recoTrack->fParams[nofStations - 1].GetCovXX();//parCov[0];
         cov[5] = recoTrack->fParams[nofStations - 1].GetCovYY();//parCov[6];
         trackParam.SetCovMatrix(cov);
         trdTrack->SetParamLast(&trackParam);
      
         for (; stationNumber < nofStations; ++stationNumber)
         {
            CbmTBin::HitHolder* hh = recoTrack->fHits[stationNumber];
            trdTrack->AddHit(hh->index, kTRDHIT);
         }
      }
      
      if (fSettings->Use(kTof))
         globalTrack->SetTofHitIndex(recoTrack->fHits[nofStations++]->index);
      
      trackParam.SetX(recoTrack->fParams[0].GetX());
      trackParam.SetY(recoTrack->fParams[0].GetY());
      trackParam.SetZ(recoTrack->fHits[0]->hit->GetZ());
      trackParam.SetTx(recoTrack->fParams[0].GetTx());
      trackParam.SetTy(recoTrack->fParams[0].GetTy());
      //recoTrack->fParams[stationNumber].CovMatrix(parCov);
      cov[0] = recoTrack->fParams[0].GetCovXX();//parCov[0];
      cov[5] = recoTrack->fParams[0].GetCovYY();//parCov[6];
      trackParam.SetCovMatrix(cov);
      globalTrack->SetParamFirst(&trackParam);
      
      trackParam.SetX(recoTrack->fParams[nofStations - 1].GetX());
      trackParam.SetY(recoTrack->fParams[nofStations - 1].GetY());
      trackParam.SetZ(recoTrack->fHits[nofStations - 1]->hit->GetZ());
      trackParam.SetTx(recoTrack->fParams[nofStations - 1].GetTx());
      trackParam.SetTy(recoTrack->fParams[nofStations - 1].GetTy());
      //recoTrack->fParams[stationNumber - 1].CovMatrix(parCov);
      cov[0] = recoTrack->fParams[nofStations - 1].GetCovXX();//parCov[0];
      cov[5] = recoTrack->fParams[nofStations - 1].GetCovYY();//parCov[6];
      trackParam.SetCovMatrix(cov);
      globalTrack->SetParamLast(&trackParam);
      
      globalTrack->SetNDF(nofStations * 2);
      globalTrack->SetChi2(recoTrack->fChiSq);*/
  }

  clock_gettime(CLOCK_REALTIME, &ts);
  long endTime = ts.tv_sec * 1000000000 + ts.tv_nsec;
  fullDuration += endTime - beginTime;
}

void CbmBinnedTrackerTask::Finish() {
  CbmBinnedHitReader::Instance()->Finish();
  double segTrue = 100 * fTracker->fNofTrueSegments;
  segTrue /= fTracker->fNofTrueSegments + fTracker->fNofWrongSegments;
  double segWrong = 100 * fTracker->fNofWrongSegments;
  segWrong /= fTracker->fNofTrueSegments + fTracker->fNofWrongSegments;
  cout << "True segments = " << segTrue << " [" << fTracker->fNofTrueSegments
       << "/" << fTracker->fNofTrueSegments + fTracker->fNofWrongSegments << "]"
       << endl;
  cout << "Wrong segments = " << segWrong << " [" << fTracker->fNofWrongSegments
       << "/" << fTracker->fNofTrueSegments + fTracker->fNofWrongSegments << "]"
       << endl;
  cout << "Full reconstruction duration: " << fullDuration << " nanoseconds"
       << endl;
}

void CbmBinnedTrackerTask::SetParContainers() {
  fSettings = static_cast<CbmBinnedSettings*>(
    FairRunAna::Instance()->GetRuntimeDb()->getContainer("CbmBinnedSettings"));
}

ClassImp(CbmBinnedTrackerTask)
