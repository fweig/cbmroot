/* Copyright (C) 2012-2021 UGiessen, JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev [committer], Andrey Lebedev, Semen Lebedev [committer] */

/**
 * \file CbmRichReconstruction.cxx
 *
 * \author Semen Lebedev
 * \date 2012
 **/

#include "CbmRichReconstruction.h"

#include "CbmRichProjectionProducerAnalytical.h"
#include "CbmRichProjectionProducerTGeo.h"
#include "CbmRichRing.h"
//#include "prototype/CbmRichProtProjectionProducer.h"

#include "CbmL1RichENNRingFinder.h"
#include "CbmRichRingFinderHough.h"
#include "CbmRichRingFinderIdeal.h"
#include "CbmRichTrackExtrapolationBase.h"
#include "CbmRichTrackExtrapolationIdeal.h"
#include "CbmRichTrackExtrapolationKF.h"
#include "CbmRichTrackExtrapolationLittrack.h"
#include "CbmRichTrackExtrapolationMirrorIdeal.h"
//#include "CbmL1RichENNRingFinderParallel.h"
//#include "prototype/CbmRichProtRingFinderHough.h"

#include "CbmEvent.h"
#include "CbmGlobalTrack.h"
#include "CbmRichConverter.h"
#include "CbmRichRingFitterCOP.h"
#include "CbmRichRingFitterCircle.h"
#include "CbmRichRingFitterEllipseMinuit.h"
#include "CbmRichRingFitterEllipseTau.h"
#include "CbmRichRingFitterRobustCOP.h"
#include "CbmRichRingFitterTAU.h"
#include "CbmRichRingTrackAssignClosestD.h"

#include "FairHit.h"
#include "FairRootManager.h"
#include <Logger.h>

#include "TClonesArray.h"

#include <iomanip>
#include <iostream>

using std::cout;
using std::endl;

CbmRichReconstruction::CbmRichReconstruction() : FairTask("CbmRichReconstruction") {}

CbmRichReconstruction::~CbmRichReconstruction()
{
  if (nullptr != fRingFinder) delete fRingFinder;
  if (nullptr != fRingFitter) delete fRingFitter;
  if (nullptr != fTrackExtrapolation) delete fTrackExtrapolation;
  if (nullptr != fProjectionProducer) delete fProjectionProducer;
  if (nullptr != fRingTrackAssign) delete fRingTrackAssign;
}

InitStatus CbmRichReconstruction::Init()
{
  FairRootManager* manager = FairRootManager::Instance();
  if (nullptr == manager) LOG(fatal) << "CbmRichReconstruction::Init(): FairRootManager is nullptr.";

  fCbmEvents = dynamic_cast<TClonesArray*>(manager->GetObject("CbmEvent"));
  if (fCbmEvents == nullptr) { LOG(info) << ": CbmEvent NOT found \n \n \n"; }
  else {
    LOG(info) << ": CbmEvent found \n \n \n";
  }

  if (fRunExtrapolation) {
    fRichTrackParamZ = new TClonesArray("FairTrackParam", 100);
    manager->Register("RichTrackParamZ", "RICH", fRichTrackParamZ, IsOutputBranchPersistent("RichTrackParamZ"));

    fGlobalTracks = (TClonesArray*) manager->GetObject("GlobalTrack");
    if (fGlobalTracks == nullptr) LOG(fatal) << "CbmRichReconstruction::Init(): No GlobalTrack array.";
  }

  if (fRunProjection) {
    if (!fRunExtrapolation) LOG(fatal) << "CbmRichReconstruction::Init(): fRunExtrapolation must be true.";
    fRichProjections = new TClonesArray("FairTrackParam");
    manager->Register("RichProjection", "RICH", fRichProjections, IsOutputBranchPersistent("RichProjection"));
  }

  fRichHits = (TClonesArray*) manager->GetObject("RichHit");
  if (fRichHits == nullptr) LOG(fatal) << "CbmRichReconstruction::Init(): No RichHit array.";

  fRichRings = new TClonesArray("CbmRichRing", 100);
  manager->Register("RichRing", "RICH", fRichRings, IsOutputBranchPersistent("RichRing"));

  if (fRunExtrapolation) InitExtrapolation();
  if (fRunProjection) InitProjection();
  if (fRunFinder) InitFinder();
  if (fRunFitter) InitFitter();
  if (fRunTrackAssign) InitTrackAssign();

  return kSUCCESS;
}

void CbmRichReconstruction::Exec(Option_t* /*opt*/)
{
  TStopwatch timer;
  timer.Start();
  if (fRichTrackParamZ != nullptr) fRichTrackParamZ->Delete();
  if (fRichProjections != nullptr) fRichProjections->Delete();
  if (fRichRings != nullptr) fRichRings->Delete();

  fNofTs++;

  if (fCbmEvents == nullptr) {
    fNofEvents++;
    ProcessData(nullptr);
  }
  else {
    int nEvents = fCbmEvents->GetEntriesFast();
    fNofEvents += nEvents;
    for (Int_t iEvent = 0; iEvent < nEvents; iEvent++) {
      CbmEvent* event = static_cast<CbmEvent*>(fCbmEvents->At(iEvent));
      ProcessData(event);
    }
  }
  timer.Stop();
  fCalcTime += timer.RealTime();
  fTotalNofHits += fRichHits->GetEntriesFast();
  fTotalNofRings += fRichRings->GetEntriesFast();
  fTotalNofTrackProj += (fRichProjections ? fRichProjections->GetEntriesFast() : 0);

  LOG(info) << setw(20) << left << GetName() << "[" << fixed << setw(8) << setprecision(1) << right
            << timer.RealTime() * 1000. << " ms] "
            << "TS " << fNofTs << ", hits " << fRichHits->GetEntriesFast() << ", rings " << fRichRings->GetEntriesFast()
            << ", trackProj " << (fRichProjections ? fRichProjections->GetEntriesFast() : 0);
}

void CbmRichReconstruction::ProcessData(CbmEvent* event)
{
  if (fRunExtrapolation) RunExtrapolation(event);
  if (fRunProjection) RunProjection(event);
  if (fRunFinder) RunFinder(event);
  if (fRunFitter) RunFitter(event);
  if (fRunTrackAssign) RunTrackAssign(event);
}

void CbmRichReconstruction::InitExtrapolation()
{
  if (fExtrapolationName == "ideal") { fTrackExtrapolation = new CbmRichTrackExtrapolationIdeal(); }
  else if (fExtrapolationName == "mirror_ideal") {
    fTrackExtrapolation = new CbmRichTrackExtrapolationMirrorIdeal();
  }
  else if (fExtrapolationName == "kf" || fExtrapolationName == "KF") {
    fTrackExtrapolation = new CbmRichTrackExtrapolationKF();
  }
  else if (fExtrapolationName == "lit" || fExtrapolationName == "littrack") {
    fTrackExtrapolation = new CbmRichTrackExtrapolationLittrack();
  }
  else {
    LOG(fatal) << fExtrapolationName << " is not correct name for extrapolation algorithm.";
  }
  fTrackExtrapolation->Init();
}

void CbmRichReconstruction::InitProjection()
{
  if (fProjectionName == "analytical") { fProjectionProducer = new CbmRichProjectionProducerAnalytical(); }
  else if (fProjectionName == "TGeo" || fProjectionName == "tgeo") {
    fProjectionProducer = new CbmRichProjectionProducerTGeo();
  }
  else {
    LOG(fatal) << fFinderName << " is not correct name for projection producer algorithm.";
  }
  fProjectionProducer->Init();
}

void CbmRichReconstruction::InitFinder()
{
  if (fFinderName == "hough") {
    fRingFinder = new CbmRichRingFinderHough();
    static_cast<CbmRichRingFinderHough*>(fRingFinder)->SetUseAnnSelect(fUseHTAnnSelect);
  }
  else if (fFinderName == "ideal") {
    fRingFinder = new CbmRichRingFinderIdeal();
  }
  else if (fFinderName == "enn") {
    fRingFinder = new CbmL1RichENNRingFinder(0);
  }
  else if ((fFinderName == "enn_parallel")) {
    // fRingFinder = new CbmL1RichENNRingFinderParallel(0);
    //    } else if (fFinderName == "hough_prototype") {
    //        fRingFinder = new CbmRichProtRingFinderHough();
  }
  else {
    LOG(fatal) << fFinderName << " is not correct name for ring finder algorithm.";
  }

  fRingFinder->Init();
}

void CbmRichReconstruction::InitFitter()
{
  if (fFitterName == "circle_cop") { fRingFitter = new CbmRichRingFitterCOP(); }
  else if (fFitterName == "circle_simple") {
    fRingFitter = new CbmRichRingFitterCircle();
  }
  else if (fFitterName == "circle_tau") {
    fRingFitter = new CbmRichRingFitterTAU();
  }
  else if (fFitterName == "circle_robust_cop") {
    fRingFitter = new CbmRichRingFitterRobustCOP();
  }
  else if (fFitterName == "ellipse_tau") {
    fRingFitter = new CbmRichRingFitterEllipseTau();
  }
  else if (fFitterName == "ellipse_minuit") {
    fRingFitter = new CbmRichRingFitterEllipseMinuit();
  }
  else {
    LOG(fatal) << fFitterName << " is not correct name for ring fitter algorithm.";
  }
  CbmRichConverter::Init();
}

void CbmRichReconstruction::InitTrackAssign()
{
  if (fTrackAssignName == "closest_distance") { fRingTrackAssign = new CbmRichRingTrackAssignClosestD(); }
  else {
    LOG(fatal) << fTrackAssignName << " is not correct name for ring-track assignment algorithm.";
  }
  fRingTrackAssign->Init();
}

void CbmRichReconstruction::RunExtrapolation(CbmEvent* event)
{
  if (fRichTrackParamZ == nullptr) LOG(info) << "fRichTrackParamZ == nullptr";
  fTrackExtrapolation->DoExtrapolation(event, fGlobalTracks, fRichTrackParamZ, fZTrackExtrapolation);
}

void CbmRichReconstruction::RunProjection(CbmEvent* event)
{
  fProjectionProducer->DoProjection(event, fRichProjections);
}

void CbmRichReconstruction::RunFinder(CbmEvent* event)
{
  fRingFinder->DoFind(event, fRichHits, fRichProjections, fRichRings);
}

void CbmRichReconstruction::RunFitter(CbmEvent* event)
{
  const Int_t nofRings = event ? event->GetNofData(ECbmDataType::kRichRing) : fRichRings->GetEntriesFast();
  if (nofRings <= 0) return;
  for (Int_t iR0 = 0; iR0 < nofRings; iR0++) {
    Int_t iR          = event ? event->GetIndex(ECbmDataType::kRichRing, iR0) : iR0;
    CbmRichRing* ring = static_cast<CbmRichRing*>(fRichRings->At(iR));
    if (nullptr == ring) continue;
    CbmRichRingLight ringL;

    CbmRichConverter::CopyHitsToRingLight(ring, &ringL);
    fRingFitter->DoFit(&ringL);
    CbmRichConverter::CopyParamsToRing(&ringL, ring);
  }
}

void CbmRichReconstruction::RunTrackAssign(CbmEvent* event)
{
  fRingTrackAssign->DoAssign(event, fRichRings, fRichProjections);
}

void CbmRichReconstruction::Finish()
{

  std::cout << std::endl;
  LOG(info) << "=====================================";
  LOG(info) << GetName() << ": Run summary";
  LOG(info) << "Time slices     : " << fNofTs;
  LOG(info) << "Events          : " << fNofEvents;
  LOG(info) << "Events / TS     : " << fixed << setprecision(2) << fNofEvents / (Double_t) fNofTs;
  LOG(info) << "Hits / TS       : " << fixed << setprecision(2) << fTotalNofHits / (Double_t) fNofTs;
  LOG(info) << "Ring / TS       : " << fixed << setprecision(2) << fTotalNofRings / (Double_t) fNofTs;
  LOG(info) << "TrackProj / TS  : " << fixed << setprecision(2) << fTotalNofTrackProj / (Double_t) fNofTs;
  LOG(info) << "Time / TS       : " << fixed << setprecision(2) << 1000. * fCalcTime / Double_t(fNofTs) << " ms";
  LOG(info) << "Hits / ev       : " << fixed << setprecision(2) << fTotalNofHits / (Double_t) fNofEvents;
  LOG(info) << "Ring / ev       : " << fixed << setprecision(2) << fTotalNofRings / (Double_t) fNofEvents;
  LOG(info) << "TrackProj / ev  : " << fixed << setprecision(2) << fTotalNofTrackProj / (Double_t) fNofEvents;
  LOG(info) << "Time / ev       : " << fixed << setprecision(2) << 1000. * fCalcTime / Double_t(fNofEvents) << " ms";
  LOG(info) << "=====================================\n";
}

ClassImp(CbmRichReconstruction)
