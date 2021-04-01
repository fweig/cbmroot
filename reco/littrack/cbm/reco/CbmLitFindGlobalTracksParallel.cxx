/**
 * \file CbmLitFindGlobalTracksParallel.cxx
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2013
 **/
#include "CbmLitFindGlobalTracksParallel.h"

#include "CbmGlobalTrack.h"
#include "CbmStsTrack.h"
#include "CbmTrack.h"
#include "cbm/base/CbmLitTrackingGeometryConstructor.h"
#include "cbm/utils/CbmLitConverterParallel.h"

#include <Logger.h>

#include "TClonesArray.h"

#include <boost/assign/list_of.hpp>

#include <algorithm>
#include <iostream>
#include <set>

#include "parallel/LitDetectorLayout.h"
#include "parallel/LitScalPixelHit.h"
#include "parallel/LitScalTrack.h"
#include "parallel/LitTrackFinderNN.h"
#include "std/utils/CbmLitMemoryManagment.h"

using boost::assign::list_of;
using std::cout;
using std::endl;
using std::for_each;
using std::set;

CbmLitFindGlobalTracksParallel::CbmLitFindGlobalTracksParallel()
  : FairTask()
  , fStsTracks(NULL)
  , fTrdHits(NULL)
  , fTrdTracks(NULL)
  , fMuchPixelHits(NULL)
  , fMuchTracks(NULL)
  , fGlobalTracks(NULL)
  , fTrackingType("nn")
  , fMergerType("neares_hit")
  , fFitterType("kalman")
  , fTrackingWatch()
  , fTrackingWithIOWatch() {}

CbmLitFindGlobalTracksParallel::~CbmLitFindGlobalTracksParallel() {}

InitStatus CbmLitFindGlobalTracksParallel::Init() {
  fDet.DetermineSetup();
  cout << fDet.ToString();

  ReadAndCreateDataBranches();

  fTrackingWatch.Reset();
  fTrackingWithIOWatch.Reset();

  return kSUCCESS;
}

void CbmLitFindGlobalTracksParallel::Exec(Option_t* opt) {
  if (fDet.GetDet(ECbmModuleId::kTrd)) { fTrdTracks->Delete(); }
  if (fDet.GetDet(ECbmModuleId::kMuch)) { fMuchTracks->Delete(); }
  fGlobalTracks->Delete();

  DoTracking();

  static Int_t eventNo = 0;
  cout << "CbmLitFindGlobalTracksParallel::Exec eventNo: " << eventNo++ << endl;
}

void CbmLitFindGlobalTracksParallel::Finish() { PrintStopwatchStatistics(); }

void CbmLitFindGlobalTracksParallel::ReadAndCreateDataBranches() {
  FairRootManager* ioman = FairRootManager::Instance();
  if (NULL == ioman)
    LOG(fatal)
      << "CbmLitFindGlobalTracksParallel: FairRootManager is not instantiated";

  // STS
  fStsTracks = (TClonesArray*) ioman->GetObject("StsTrack");
  if (NULL == fStsTracks)
    LOG(fatal) << "CbmLitFindGlobalTracksParallel: No StsTrack array!";

  // TRD
  if (fDet.GetDet(ECbmModuleId::kTrd)) {
    fTrdHits = (TClonesArray*) ioman->GetObject("TrdHit");
    if (NULL == fTrdHits)
      if (NULL == fTrdHits)
        LOG(fatal) << "CbmLitFindGlobalTracksParallel: No TRDHit array!";
  }

  // MUCH
  if (fDet.GetDet(ECbmModuleId::kMuch)) {
    fMuchPixelHits = (TClonesArray*) ioman->GetObject("MuchPixelHit");
    if (NULL == fMuchPixelHits)
      if (NULL == fMuchPixelHits)
        LOG(fatal) << "CbmLitFindGlobalTracksParallel: No MuchPixelHit array!";
  }

  // Create and register track arrays
  fGlobalTracks = new TClonesArray("CbmGlobalTrack", 100);
  ioman->Register("GlobalTrack",
                  "Global",
                  fGlobalTracks,
                  IsOutputBranchPersistent("GlobalTrack"));

  if (fDet.GetDet(ECbmModuleId::kTrd)) {
    fTrdTracks = new TClonesArray("CbmTrdTrack", 100);
    ioman->Register(
      "TrdTrack", "Trd", fTrdTracks, IsOutputBranchPersistent("TrdTrack"));
  }

  if (fDet.GetDet(ECbmModuleId::kMuch)) {
    fMuchTracks = new TClonesArray("CbmMuchTrack", 100);
    ioman->Register(
      "MuchTrack", "Much", fMuchTracks, IsOutputBranchPersistent("MuchTrack"));
  }
}

void CbmLitFindGlobalTracksParallel::DoTracking() {
  static Bool_t firstTime = true;
  static lit::parallel::LitDetectorLayoutScal layout;
  static lit::parallel::LitTrackFinderNN finder;
  if (firstTime) {
    if (fDet.GetDet(ECbmModuleId::kTrd)) {
      CbmLitTrackingGeometryConstructor::Instance()->GetTrdLayoutScal(layout);
      finder.SetDetectorLayout(layout);
      finder.SetNofIterations(1);
      finder.SetMaxNofMissingHits(list_of(4));
      finder.SetPDG(list_of(211));
      finder.SetChiSqStripHitCut(list_of(9.));
      finder.SetChiSqPixelHitCut(list_of(25.));
      finder.SetSigmaCoef(list_of(5.));
      firstTime = false;
    }
    if (fDet.GetDet(ECbmModuleId::kMuch)) {
      CbmLitTrackingGeometryConstructor::Instance()->GetMuchLayoutScal(layout);
      finder.SetDetectorLayout(layout);
      finder.SetNofIterations(1);
      finder.SetMaxNofMissingHits(list_of(4));
      finder.SetPDG(list_of(211));
      finder.SetChiSqStripHitCut(list_of(9.));
      finder.SetChiSqPixelHitCut(list_of(25.));
      finder.SetSigmaCoef(list_of(5.));
      firstTime = false;
    }
  }

  fTrackingWithIOWatch.Start(kFALSE);

  // Convert input data
  vector<lit::parallel::LitScalTrack*> lseeds;
  vector<lit::parallel::LitScalPixelHit*> lhits;
  vector<lit::parallel::LitScalTrack*> ltracks;
  if (fDet.GetDet(ECbmModuleId::kTrd)) {
    CbmLitConverterParallel::CbmPixelHitArrayToLitScalPixelHitArray(fTrdHits,
                                                                    lhits);
  }
  if (fDet.GetDet(ECbmModuleId::kMuch)) {
    CbmLitConverterParallel::CbmPixelHitArrayToLitScalPixelHitArray(
      fMuchPixelHits, lhits);
  }

  // Convert track seeds
  Int_t nofSeeds = fStsTracks->GetEntriesFast();
  for (Int_t iSeed = 0; iSeed < nofSeeds; iSeed++) {
    CbmStsTrack* stsTrack = static_cast<CbmStsTrack*>(fStsTracks->At(iSeed));
    lit::parallel::LitScalTrack* lseed = new lit::parallel::LitScalTrack();
    lit::parallel::LitTrackParamScal lpar;
    CbmLitConverterParallel::FairTrackParamToLitTrackParamScal(
      stsTrack->GetParamLast(), &lpar);
    lseed->SetParamFirst(lpar);
    lseeds.push_back(lseed);
    lseed->SetPreviousTrackId(iSeed);
  }

  fTrackingWatch.Start(kFALSE);
  finder.DoFind(lhits, lseeds, ltracks);
  fTrackingWatch.Stop();

  // Convert to CBMROOT data and construct global tracks
  if (fDet.GetDet(ECbmModuleId::kTrd)) {
    CbmLitConverterParallel::LitScalTrackArrayToCbmTrdTrackArray(ltracks,
                                                                 fTrdTracks);
  }
  if (fDet.GetDet(ECbmModuleId::kMuch)) {
    CbmLitConverterParallel::LitScalTrackArrayToCbmMuchTrackArray(ltracks,
                                                                  fMuchTracks);
  }
  ConstructGlobalTracks();

  // Clear memory
  for_each(lseeds.begin(), lseeds.end(), DeleteObject());
  for_each(ltracks.begin(), ltracks.end(), DeleteObject());
  for_each(lhits.begin(), lhits.end(), DeleteObject());
  lseeds.clear();
  ltracks.clear();
  lhits.clear();

  fTrackingWithIOWatch.Stop();
}

void CbmLitFindGlobalTracksParallel::ConstructGlobalTracks() {
  set<Int_t> stsTracksSet;
  Int_t globalTrackNo = 0;
  if (fDet.GetDet(ECbmModuleId::kTrd)) {
    Int_t nofTrdTracks = fTrdTracks->GetEntriesFast();
    for (Int_t iTrack = 0; iTrack < nofTrdTracks; iTrack++) {
      const CbmTrack* trdTrack =
        static_cast<const CbmTrack*>(fTrdTracks->At(iTrack));
      CbmGlobalTrack* globalTrack =
        new ((*fGlobalTracks)[globalTrackNo++]) CbmGlobalTrack();
      globalTrack->SetStsTrackIndex(trdTrack->GetPreviousTrackId());
      globalTrack->SetTrdTrackIndex(iTrack);
    }
  }

  if (fDet.GetDet(ECbmModuleId::kMuch)) {
    Int_t nofMuchTracks = fMuchTracks->GetEntriesFast();
    for (Int_t iTrack = 0; iTrack < nofMuchTracks; iTrack++) {
      const CbmTrack* trdTrack =
        static_cast<const CbmTrack*>(fMuchTracks->At(iTrack));
      CbmGlobalTrack* globalTrack =
        new ((*fGlobalTracks)[globalTrackNo++]) CbmGlobalTrack();
      globalTrack->SetStsTrackIndex(trdTrack->GetPreviousTrackId());
      globalTrack->SetMuchTrackIndex(iTrack);
    }
  }

  // Loop over STS tracks in order to create additional CbmGlobalTrack,
  // consisting of only CbmStsTrack
  for (Int_t iTrack = 0; iTrack < fStsTracks->GetEntriesFast(); iTrack++) {
    // if CbmGlobalTrack for STS track was not created, than create one
    if (stsTracksSet.find(iTrack) == stsTracksSet.end()) {
      CbmGlobalTrack* globalTrack =
        new ((*fGlobalTracks)[globalTrackNo++]) CbmGlobalTrack();
      globalTrack->SetStsTrackIndex(iTrack);
    }
  }
}

void CbmLitFindGlobalTracksParallel::PrintStopwatchStatistics() {
  LOG(info) << "CbmLitFindGlobalTracksParallel::PrintStopwatchStatistics:\n "
            << "tracking without IO: counts=" << fTrackingWatch.Counter()
            << ", real=" << fTrackingWatch.RealTime() / fTrackingWatch.Counter()
            << "/" << fTrackingWatch.RealTime()
            << " s, cpu=" << fTrackingWatch.CpuTime() / fTrackingWatch.Counter()
            << "/" << fTrackingWatch.CpuTime() << "\n"
            << "tracking with IO: counts=" << fTrackingWithIOWatch.Counter()
            << ", real="
            << fTrackingWithIOWatch.RealTime() / fTrackingWithIOWatch.Counter()
            << "/" << fTrackingWithIOWatch.RealTime() << " s, cpu="
            << fTrackingWithIOWatch.CpuTime() / fTrackingWithIOWatch.Counter()
            << "/" << fTrackingWithIOWatch.CpuTime();
}

ClassImp(CbmLitFindGlobalTracksParallel);
