/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "CbmGlobalTrackingTask.h"
#include "CbmGlobalTrack.h"
#include "CbmKFParticleInterface.h"
#include "CbmKFStsHit.h"
#include "CbmMvdHit.h"
#include "CbmStsCluster.h"
#include "CbmStsHit.h"
#include "CbmStsTrack.h"
#include "FairLogger.h"
#include "TH1F.h"

#include <iostream>

#define CBM_GLOBALTB_PRINT_PERF

#ifdef CBM_GLOBALTB_PRINT_PERF
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
#endif  //CBM_GLOBALTB_PRINT_PERF

using std::cout;
using std::endl;
using std::list;
using std::map;
using std::set;
using std::vector;

CbmGlobalTrackingTask::CbmGlobalTrackingTask()
  : fTofGeometry()
  , fTimeSlice(0)
  , fTofHits(0)
  , fStsTracks(0)
  , fStsHits(0)
  , fMvdHits(0)
  , fGlobalTracks(0)
  , fKFTrack()
  , fPrimVertex(0) {}

InitStatus CbmGlobalTrackingTask::Init() {
  fKFTrack.SetPID(211);

  if (!fTofGeometry.Read())
    LOG(fatal) << "Could not read the ToF geometry information";

  FairRootManager* ioman = FairRootManager::Instance();

  if (0 == ioman) LOG(fatal) << "No FairRootManager";

  fTimeSlice = static_cast<CbmTimeSlice*>(ioman->GetObject("TimeSlice."));

  if (0 == fTimeSlice) LOG(fatal) << "No time slice";

  fTofHits = static_cast<TClonesArray*>(ioman->GetObject("TofHit"));

  if (0 == fTofHits) LOG(fatal) << "No ToF hits";

  fTofGeometry.SetTofHits(fTofHits);

  fStsTracks = static_cast<TClonesArray*>(ioman->GetObject("StsTrack"));

  if (0 == fStsTracks) LOG(fatal) << "No STS tracks";

  fStsHits = static_cast<TClonesArray*>(ioman->GetObject("StsHit"));

  if (0 == fStsHits) LOG(fatal) << "No STS hits";

  fMvdHits = static_cast<TClonesArray*>(ioman->GetObject("MvdHit"));
  //fPrimVertex = static_cast<CbmVertex*> (ioman->GetObject("PrimaryVertex"));
  // Get pointer to PrimaryVertex object from IOManager if it exists
  // The old name for the object is "PrimaryVertex" the new one
  // "PrimaryVertex." Check first for the new name
  fPrimVertex = dynamic_cast<CbmVertex*>(ioman->GetObject("PrimaryVertex."));
  if (nullptr == fPrimVertex) {
    fPrimVertex = dynamic_cast<CbmVertex*>(ioman->GetObject("PrimaryVertex"));
  }
  if (nullptr == fPrimVertex) { LOG(fatal) << "No primary vertex"; }

  fGlobalTracks = new TClonesArray("CbmGlobalTrack", 100);
  ioman->Register("GlobalTrack",
                  "Global",
                  fGlobalTracks,
                  IsOutputBranchPersistent("GlobalTrack"));

  return kSUCCESS;
}

#ifdef CBM_GLOBALTB_PRINT_PERF
static long fullDuration = 0;
#endif  //CBM_GLOBALTB_PRINT_PERF

void CbmGlobalTrackingTask::Exec(Option_t* /*opt*/) {
  fGlobalTracks->Clear();
  Double_t startTime = fTimeSlice->GetStartTime();
  fTofGeometry.Prepare(startTime);
  int nofStsTracks = fStsTracks->GetEntriesFast();

  for (int i = 0; i < nofStsTracks; ++i) {
    const CbmStsTrack* track =
      static_cast<const CbmStsTrack*>(fStsTracks->At(i));
    CbmGlobalTrack* globalTrack = new ((*fGlobalTracks)[i]) CbmGlobalTrack();
    globalTrack->SetStsTrackIndex(i);
    Int_t tofHitInd;
    Double_t stsTrackLength = CalcStsTrackLength(track);
    Double_t trackLength    = stsTrackLength;
    FairTrackParam param    = *track->GetParamLast();
#ifdef CBM_GLOBALTB_PRINT_PERF
    timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    long beginTime = ts.tv_sec * 1000000000 + ts.tv_nsec;
#endif  //CBM_GLOBALTB_PRINT_PERF
    //fTofGeometry.Find(param->GetX(), param->GetCovariance(0, 0), param->GetY(), param->GetCovariance(1, 1), param->GetZ(),
    //track->GetTime(), track->GetTimeError(), param->GetTx(), TMath::Sqrt(param->GetCovariance(2, 2)), param->GetTy(), TMath::Sqrt(param->GetCovariance(3, 3)), tofHitInd);
    fTofGeometry.Find(
      param, track->GetTime(), track->GetTimeError(), tofHitInd, trackLength);
    globalTrack->SetTofHitIndex(tofHitInd);
    globalTrack->SetLength(stsTrackLength + trackLength);
    globalTrack->SetParamFirst(track->GetParamFirst());
    globalTrack->SetParamLast(&param);

    if (fPrimVertex) {
      FairTrackParam vtxTrackParam;
      float chiSqPrimary = 0;
      CbmKFParticleInterface::ExtrapolateTrackToPV(
        track, fPrimVertex, &vtxTrackParam, chiSqPrimary);
      globalTrack->SetParamPrimaryVertex(&vtxTrackParam);
    }
#ifdef CBM_GLOBALTB_PRINT_PERF
    clock_gettime(CLOCK_REALTIME, &ts);
    long endTime = ts.tv_sec * 1000000000 + ts.tv_nsec;
    fullDuration += endTime - beginTime;
#endif  //CBM_GLOBALTB_PRINT_PERF
  }     //for (int i = 0; i < nofStsTracks; ++i)
}

extern int nofMRPCIntersections;
extern int nofToFIntersections;
extern int nofMRPCIntersectionsT;
extern int globalNofHits;
extern int globalNofHitsT;
extern int globalNofHitsM;

void CbmGlobalTrackingTask::Finish() {
  cout << "nofMRPCIntersections: " << nofMRPCIntersections << endl;
  cout << "nofToFIntersections: " << nofToFIntersections << endl;
  cout << "nofMRPCIntersectionsT: " << nofMRPCIntersectionsT << endl;
  cout << "globalNofHits: " << globalNofHits << endl;
  cout << "globalNofHitsT: " << globalNofHitsT << endl;
  cout << "globalNofHitsM: " << globalNofHitsM << endl << endl << endl;
  cout << "Full merging duration was: " << fullDuration << endl << endl << endl;
}

Double_t CbmGlobalTrackingTask::CalcStsTrackLength(const CbmStsTrack* track) {
  Double_t length                  = 0;
  Double_t* T                      = fKFTrack.GetTrack();
  Double_t zVert                   = fPrimVertex ? fPrimVertex->GetZ() : 0;
  const FairTrackParam* paramFirst = track->GetParamFirst();
  Double_t x                       = paramFirst->GetX();
  Double_t y                       = paramFirst->GetY();
  Double_t z                       = paramFirst->GetZ();
  Double_t p = paramFirst->GetQp() ? TMath::Abs(1 / paramFirst->GetQp()) : 1;
  fKFTrack.SetTrackParam(*paramFirst);

  while (z > zVert) {
    Double_t xOld = x;
    Double_t yOld = y;
    Double_t zOld = z;

    if (z - p > zVert)
      z -= p;
    else
      z = zVert;

    fKFTrack.Extrapolate(z);
    x = T[0];
    y = T[1];
    length += TMath::Sqrt(TMath::Power(x - xOld, 2) + TMath::Power(y - yOld, 2)
                          + TMath::Power(z - zOld, 2));
  }

  x = paramFirst->GetX();
  y = paramFirst->GetY();
  z = paramFirst->GetZ();

  const FairTrackParam* paramLast = track->GetParamLast();
  int nofHits                     = track->GetNofHits();

  for (int i = 1; i < nofHits; ++i) {
    Int_t hitInd     = track->GetHitIndex(i);
    HitType hitType  = track->GetHitType(i);
    CbmPixelHit* hit = static_cast<CbmPixelHit*>(
      kMVDHIT == hitType ? fMvdHits->At(hitInd) : fStsHits->At(hitInd));
    // Double_t nextX;  (VF) not used
    // Double_t nextY;  (VF) not used
    Double_t nextZ;

    if (i == nofHits - 1) {
      // nextX = paramLast->GetX(); (VF) not used
      // nextY = paramLast->GetY(); (VF) not used
      nextZ = paramLast->GetZ();
    } else {
      // nextX = hit->GetX(); (VF) not used
      // nextY = hit->GetY(); (VF) not used
      nextZ = hit->GetZ();
    }

    while (z < nextZ) {
      Double_t xOld = x;
      Double_t yOld = y;
      Double_t zOld = z;

      if (z + p < nextZ)
        z += p;
      else
        z = nextZ;

      fKFTrack.Extrapolate(z);

      if (z == nextZ) {
        CbmKFStsHit stsKFHit;

        if (kMVDHIT == hitType)
          stsKFHit.Create(static_cast<CbmMvdHit*>(hit));
        else
          stsKFHit.Create(static_cast<CbmStsHit*>(hit));

        Double_t qp0 = T[4];
        stsKFHit.Filter(fKFTrack, kTRUE, qp0);
      }

      x = T[0];
      y = T[1];
      length +=
        TMath::Sqrt(TMath::Power(x - xOld, 2) + TMath::Power(y - yOld, 2)
                    + TMath::Power(z - zOld, 2));
    }
  }

  return length;
}

ClassImp(CbmGlobalTrackingTask)
