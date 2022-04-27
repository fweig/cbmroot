/* Copyright (C) 2009-2021 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer], Volker Friese */

/** CbmLitFindGlobalTracks.cxx
 * @author Andrey Lebedev <andrey.lebedev@gsi.de>
 * @since 2009
 * @version 1.0
 **/

#include "CbmLitFindGlobalTracks.h"

#include "CbmGlobalTrack.h"
#include "CbmHit.h"
#include "CbmKFParticleInterface.h"
#include "CbmMuchTrack.h"
#include "CbmPixelHit.h"
#include "CbmStripHit.h"
#include "CbmStsHit.h"
#include "CbmStsTrack.h"
#include "CbmTofTrack.h"
#include "CbmTrdTrack.h"
#include "base/CbmLitToolFactory.h"
#include "base/CbmLitTrackingGeometryConstructor.h"
#include "data/CbmLitHit.h"
#include "data/CbmLitPixelHit.h"
#include "data/CbmLitStripHit.h"
#include "data/CbmLitTrack.h"
#include "propagation/CbmLitTGeoTrackPropagator.h"
#include "utils/CbmLitConverter.h"
#include "utils/CbmLitMemoryManagment.h"

#include "FairRootManager.h"

#include "TClonesArray.h"

#include <iomanip>
#include <iostream>

#include <cmath>

using std::cout;
using std::endl;
using std::fixed;
using std::left;
using std::pair;
using std::right;
using std::setprecision;
using std::setw;
using std::stringstream;

CbmLitFindGlobalTracks::CbmLitFindGlobalTracks()
  : FairTask("LitFindGlobalTracks")
  , fDet()
  ,

  fStsTracks(NULL)
  , fMvdHits(NULL)
  , fStsHits(NULL)
  , fMuchPixelHits(NULL)
  , fMuchTracks(NULL)
  , fTrdHits(NULL)
  , fTrdTracks(NULL)
  , fTofHits(NULL)
  , fEvents(NULL)
  , fTofTracks(NULL)
  , fGlobalTracks(NULL)
  , fPrimVertex(NULL)
  ,

  fLitStsTracks()
  , fLitHits()
  , fLitTofHits()
  , fLitOutputTracks()
  , fLitOutputTofTracks()
  ,

  fFinder()
  , fMerger()
  , fFitter()
  , fPropagator()
  ,

  fTrackingType("branch")
  , fMergerType("nearest_hit")
  , fFitterType("lit_kalman")
  ,

  fTrackingWatch()
  , fMergerWatch()
  ,

  fEventNo(0)
{
}

CbmLitFindGlobalTracks::~CbmLitFindGlobalTracks() {}

InitStatus CbmLitFindGlobalTracks::Init()
{
  std::cout << std::endl;
  LOG(info) << "=========================================";
  LOG(info) << GetName() << ": Initializing ";
  fDet.DetermineSetup();
  LOG(info) << fDet.ToString();

  ReadInputBranches();
  RegisterOutputBranches();

  InitTrackReconstruction();

  fTrackingWatch.Reset();
  fMergerWatch.Reset();

  LOG(info) << "=========================================";
  std::cout << std::endl;
  return kSUCCESS;
}

void CbmLitFindGlobalTracks::Exec(Option_t* opt)
{

  UInt_t nEvents    = 0;
  UInt_t nStsTracks = 0;
  UInt_t nMuchHits  = 0;
  UInt_t nTrdHits   = 0;
  UInt_t nTofHits   = 0;
  UInt_t nGlbTracks = 0;

  if (fStsTracks) nStsTracks = fStsTracks->GetEntriesFast();
  if (fMuchPixelHits) nMuchHits = fMuchPixelHits->GetEntriesFast();
  if (fTrdHits) nTrdHits = fTrdHits->GetEntriesFast();
  if (fTofHits) nTofHits = fTofHits->GetEntriesFast();

  TStopwatch timer;
  timer.Start();

  if (fTrdTracks != NULL) fTrdTracks->Delete();
  if (fMuchTracks != NULL) fMuchTracks->Delete();
  if (fTofTracks != NULL) fTofTracks->Delete();
  fGlobalTracks->Clear();

  if (fEvents) {
    nEvents = fEvents->GetEntriesFast();
    LOG(debug) << GetName() << ": reading time slice with " << nEvents << " events ";

    for (Int_t iEvent = 0; iEvent < nEvents; iEvent++) {
      CbmEvent* event = static_cast<CbmEvent*>(fEvents->At(iEvent));
      ConvertInputData(event);
      RunTrackReconstruction();
      ConvertOutputData(event);
      CalculateLength(event);
      CalculatePrimaryVertexParameters(event);
      ClearArrays();
      LOG(debug) << "CbmLitFindGlobalTracks::Exec event: " << event->GetNumber();
    }     //# events
  }       //? event branch present
  else {  // Old event-by-event simulation without event branch
    ConvertInputData(0);
    RunTrackReconstruction();
    ConvertOutputData(0);
    CalculateLength(0);
    CalculatePrimaryVertexParameters(0);
    ClearArrays();
    LOG(debug) << "CbmLitFindGlobalTracks::Exec timeslice: " << fEventNo++;
  }

  // --- Timeslice log and statistics
  timer.Stop();
  nGlbTracks = fGlobalTracks->GetEntriesFast();
  timer.Stop();
  stringstream logOut;
  logOut << setw(20) << left << GetName() << " [";
  logOut << fixed << setw(8) << setprecision(1) << right << timer.RealTime() * 1000. << " ms] ";
  logOut << "TS " << fNofTs;
  if (fEvents) logOut << ", events " << nEvents;
  logOut << ", STS tracks " << nStsTracks << ", Hits MUCH " << nMuchHits << " TRD " << nTrdHits;
  logOut << " TOF " << nTofHits << ", global tracks " << nGlbTracks;
  LOG(info) << logOut.str();
  fNofTs++;
  fNofEvents += nEvents;
  fNofStsTracks += nStsTracks;
  fNofMuchHits += nMuchHits;
  fNofTrdHits += nTrdHits;
  fNofTofHits += nTofHits;
  fNofGlbTracks += nGlbTracks;
  fTime += timer.RealTime();
}


void CbmLitFindGlobalTracks::SetParContainers() {}

void CbmLitFindGlobalTracks::Finish()
{

  std::cout << std::endl;
  LOG(info) << "=====================================";
  LOG(info) << GetName() << ": Run summary";
  LOG(info) << "Time slices         : " << fNofTs;
  LOG(info) << "STS tracks   / TS   : " << fixed << setprecision(2) << Double_t(fNofStsTracks) / Double_t(fNofTs);
  LOG(info) << "MUCH hits / TS      : " << fixed << setprecision(2) << Double_t(fNofMuchHits) / Double_t(fNofTs);
  LOG(info) << "TRD hits / TS       : " << fixed << setprecision(2) << Double_t(fNofTrdHits) / Double_t(fNofTs);
  LOG(info) << "TOF hits / TS       : " << fixed << setprecision(2) << Double_t(fNofTofHits) / Double_t(fNofTs);
  LOG(info) << "Global tracks / TS  : " << fixed << setprecision(2) << Double_t(fNofGlbTracks) / Double_t(fNofTs);
  ;
  LOG(info) << "Time per TS         : " << 1000. * fTime / Double_t(fNofTs) << " ms ";
  if (fEvents) {
    LOG(info) << "Events processed    : " << fNofEvents;
    LOG(info) << "Global tracks / evt : " << Double_t(fNofGlbTracks) / Double_t(fNofEvents);
  }
  PrintStopwatchStatistics();
  LOG(info) << "=====================================";
}


void CbmLitFindGlobalTracks::ReadInputBranches()
{
  FairRootManager* ioman = FairRootManager::Instance();
  assert(ioman);

  // --- MVD hits
  if (fDet.GetDet(ECbmModuleId::kMvd)) {
    fMvdHits = dynamic_cast<TClonesArray*>(ioman->GetObject("MvdHit"));
    if (!fMvdHits) {
      LOG(warn) << GetName() << ": No MvdHit branch!";
      fDet.SetDet(ECbmModuleId::kMvd, false);
    }
    else
      LOG(info) << GetName() << ": Found MvdHit branch";
  }  //? MVD in geometry

  // --- STS hits
  fStsHits = dynamic_cast<TClonesArray*>(ioman->GetObject("StsHit"));
  if (!fStsHits) {
    LOG(fatal) << GetName() << ": No StsHit branch!";
    fDet.SetDet(ECbmModuleId::kSts, false);
  }
  else
    LOG(info) << GetName() << ": Found StsHit branch";

  // --- STS tracks
  fStsTracks = dynamic_cast<TClonesArray*>(ioman->GetObject("StsTrack"));
  if (!fStsTracks) {
    LOG(fatal) << GetName() << ": No StsTrack branch!";
    fDet.SetDet(ECbmModuleId::kSts, false);
  }
  else
    LOG(info) << GetName() << ": Found StsTrack branch";

  // --- MUCH hits
  if (fDet.GetDet(ECbmModuleId::kMuch)) {
    fMuchPixelHits = dynamic_cast<TClonesArray*>(ioman->GetObject("MuchPixelHit"));
    if (!fMuchPixelHits) {
      LOG(warning) << GetName() << "No MuchPixelHit branch!";
      fDet.SetDet(ECbmModuleId::kMuch, false);
    }
    else {
      if (fMuchPixelHits) LOG(info) << GetName() << ": Found MuchPixelHit branch";
    }
  }  //? MUCH in geometry

  // --- TRD hits
  if (fDet.GetDet(ECbmModuleId::kTrd)) {
    fTrdHits = dynamic_cast<TClonesArray*>(ioman->GetObject("TrdHit"));
    if (NULL == fTrdHits) {
      LOG(warning) << GetName() << ": No TrdHit branch!";
      fDet.SetDet(ECbmModuleId::kTrd, false);
    }
    LOG(info) << GetName() << ": Found TrdHit branch";
  }  //? TRD in geometry

  // --- TOF hits
  if (fDet.GetDet(ECbmModuleId::kTof)) {
    fTofHits = dynamic_cast<TClonesArray*>(ioman->GetObject("TofHit"));
    if (NULL == fTofHits) {
      LOG(warning) << GetName() << ": No TofHit branch!";
      fDet.SetDet(ECbmModuleId::kTof, false);
    }
    else
      LOG(info) << GetName() << ": Found TofHit branch";
  }  //? TOF in geometry

  // --- Events
  fEvents = dynamic_cast<TClonesArray*>(ioman->GetObject("CbmEvent"));
  if (fEvents) LOG(info) << GetName() << ": Found Event branch";
  else
    LOG(info) << GetName() << ": No Event branch; run in time-based mode";

  // --- Primary vertex
  fPrimVertex = dynamic_cast<CbmVertex*>(ioman->GetObject("PrimaryVertex."));
  if (nullptr == fPrimVertex) { fPrimVertex = dynamic_cast<CbmVertex*>(ioman->GetObject("PrimaryVertex")); }
  if (nullptr == fPrimVertex) LOG(warning) << GetName() << ": No PrimaryVertex branch!";
  else
    LOG(info) << GetName() << ": Found PrimaryVertex branch";
}

void CbmLitFindGlobalTracks::RegisterOutputBranches()
{
  FairRootManager* ioman = FairRootManager::Instance();
  assert(ioman);

  // --- MuchTrack
  if (fDet.GetDet(ECbmModuleId::kMuch)) {
    fMuchTracks = new TClonesArray("CbmMuchTrack", 100);
    ioman->Register("MuchTrack", "Much", fMuchTracks, IsOutputBranchPersistent("MuchTrack"));
    LOG(info) << GetName() << ": Register MuchTrack branch";
  }

  // --- TrdTrack
  if (fDet.GetDet(ECbmModuleId::kTrd)) {
    fTrdTracks = new TClonesArray("CbmTrdTrack", 100);
    ioman->Register("TrdTrack", "Trd", fTrdTracks, IsOutputBranchPersistent("TrdTrack"));
    LOG(info) << GetName() << ": Register TrdTrack branch";
  }

  // --- TofTrack
  if (fDet.GetDet(ECbmModuleId::kTof)) {
    fTofTracks = new TClonesArray("CbmTofTrack", 100);
    ioman->Register("TofTrack", "Tof", fTofTracks, IsOutputBranchPersistent("TofTrack"));
    LOG(info) << GetName() << ": Register TofTrack branch";
  }

  // --- GlobalTrack
  fGlobalTracks = new TClonesArray("CbmGlobalTrack", 100);
  ioman->Register("GlobalTrack", "Global", fGlobalTracks, IsOutputBranchPersistent("GlobalTrack"));
  LOG(info) << GetName() << ": Register GlobalTrack branch";
}

void CbmLitFindGlobalTracks::InitTrackReconstruction()
{
  if (fDet.GetElectronSetup()) {
    if (fTrackingType == "branch" || fTrackingType == "nn" || fTrackingType == "nn_parallel") {
      std::string st("e_");
      st += fTrackingType;
      fFinder = CbmLitToolFactory::CreateTrackFinder(st);
    }
    else {
      TObject::Fatal("CbmLitFindGlobalTracks", "Tracking type not found");
    }
  }
  else {
    if (fTrackingType == "branch" || fTrackingType == "nn" || fTrackingType == "nn_parallel") {
      std::string st("mu_");
      st += fTrackingType;
      fFinder = CbmLitToolFactory::CreateTrackFinder(st);
    }
    else {
      TObject::Fatal("CbmLitFindGlobalTracks", "Tracking type not found");
    }
  }

  if (fDet.GetDet(ECbmModuleId::kTof)) {
    if (fMergerType == "nearest_hit" || fMergerType == "all_hits") {
      fMerger = CbmLitToolFactory::CreateHitToTrackMerger("tof_" + fMergerType);
    }
    else {
      TObject::Fatal("CbmLitFindGlobalTracks", "Merger type not found");
    }
  }

  if (fFitterType == "lit_kalman") { fFitter = CbmLitToolFactory::CreateTrackFitter("lit_kalman"); }
  else {
    TObject::Fatal("CbmLitFindGlobalTracks", "Fitter type not found");
  }

  fPropagator = CbmLitToolFactory::CreateTrackPropagator("lit");
}

void CbmLitFindGlobalTracks::ConvertInputData(CbmEvent* event)
{
  CbmLitConverter::StsTrackArrayToTrackVector(event, fStsTracks, fLitStsTracks);
  LOG(debug2) << "-I- CbmLitFindGlobalTracks: Number of STS tracks: " << fLitStsTracks.size();

  if (fMuchPixelHits) {
    CbmLitConverter::HitArrayToHitVector(event, ECbmDataType::kMuchPixelHit, fMuchPixelHits, fLitHits);
  }
  if (fTrdHits) {
    CbmLitConverter::HitArrayToHitVector(event, ECbmDataType::kTrdHit, fTrdHits, fLitHits);
    //If MUCH-TRD setup, than shift plane id for the TRD hits
    if (fDet.GetDet(ECbmModuleId::kMuch) && fDet.GetDet(ECbmModuleId::kTrd)) {
      Int_t nofStations = CbmLitTrackingGeometryConstructor::Instance()->GetNofMuchStations();
      for (Int_t i = 0; i < fLitHits.size(); i++) {
        CbmLitHit* hit = fLitHits[i];
        if (hit->GetSystem() == kLITTRD) { hit->SetDetectorId(kLITTRD, hit->GetStation() + nofStations); }
      }
    }
  }
  LOG(debug2) << "-I- CbmLitFindGlobalTracks: Number of hits: " << fLitHits.size();

  if (fTofHits) {
    CbmLitConverter::HitArrayToHitVector(event, ECbmDataType::kTofHit, fTofHits, fLitTofHits);
    LOG(debug2) << "-I- CbmLitFindGlobalTracks: Number of TOF hits: " << fLitTofHits.size();
  }
}

void CbmLitFindGlobalTracks::ConvertOutputData(CbmEvent* event)
{
  CbmLitConverter::LitTrackVectorToGlobalTrackArray(event, fLitOutputTracks, fLitOutputTofTracks, fGlobalTracks,
                                                    fStsTracks, fTrdTracks, fMuchTracks, fTofTracks);
}

void CbmLitFindGlobalTracks::CalculateLength(CbmEvent* event)
{
  if (fTofTracks == NULL || fGlobalTracks == NULL) return;

  CbmVertex* primVertex = event ? event->GetVertex() : fPrimVertex;

  /* Calculate the length of the global track
    * starting with (0, 0, 0) and adding all
    * distances between hits
    */
  Int_t nofTofTracks = event ? event->GetNofData(ECbmDataType::kTofTrack) : fTofTracks->GetEntriesFast();
  for (Int_t i = 0; i < nofTofTracks; ++i) {
    Int_t itt                   = event ? event->GetIndex(ECbmDataType::kTofTrack, i) : i;
    CbmTofTrack* tofTrack       = static_cast<CbmTofTrack*>(fTofTracks->At(itt));
    CbmGlobalTrack* globalTrack = static_cast<CbmGlobalTrack*>(fGlobalTracks->At(tofTrack->GetTrackIndex()));
    if (globalTrack == NULL) { continue; }

    std::vector<Double_t> X, Y, Z;
    if (primVertex == NULL) {
      X.push_back(0.);
      Y.push_back(0.);
      Z.push_back(0.);
    }
    else {
      X.push_back(primVertex->GetX());
      Y.push_back(primVertex->GetY());
      Z.push_back(primVertex->GetZ());
    }

    // get track segments indices
    Int_t stsId  = globalTrack->GetStsTrackIndex();
    Int_t trdId  = globalTrack->GetTrdTrackIndex();
    Int_t muchId = globalTrack->GetMuchTrackIndex();
    Int_t tofId  = tofTrack->GetTofHitIndex();  //globalTrack->GetTofHitIndex();

    if (stsId > -1) {
      const CbmStsTrack* stsTrack = static_cast<const CbmStsTrack*>(fStsTracks->At(stsId));
      Int_t nofStsHits            = stsTrack->GetNofStsHits();
      for (Int_t ih = 0; ih < nofStsHits; ih++) {
        CbmStsHit* hit = (CbmStsHit*) fStsHits->At(stsTrack->GetHitIndex(ih));
        X.push_back(hit->GetX());
        Y.push_back(hit->GetY());
        Z.push_back(hit->GetZ());
      }
    }

    if (muchId > -1) {
      const CbmTrack* muchTrack = static_cast<const CbmTrack*>(fMuchTracks->At(muchId));
      Int_t nofMuchHits         = muchTrack->GetNofHits();
      for (Int_t ih = 0; ih < nofMuchHits; ih++) {
        HitType hitType = muchTrack->GetHitType(ih);
        if (hitType == ToIntegralType(ECbmDataType::kMuchPixelHit)) {
          CbmPixelHit* hit = (CbmPixelHit*) fMuchPixelHits->At(muchTrack->GetHitIndex(ih));
          X.push_back(hit->GetX());
          Y.push_back(hit->GetY());
          Z.push_back(hit->GetZ());
        }
      }
    }

    if (trdId > -1) {
      const CbmTrack* trdTrack = static_cast<const CbmTrack*>(fTrdTracks->At(trdId));
      Int_t nofTrdHits         = trdTrack->GetNofHits();
      for (Int_t ih = 0; ih < nofTrdHits; ih++) {
        CbmPixelHit* hit = (CbmPixelHit*) fTrdHits->At(trdTrack->GetHitIndex(ih));
        X.push_back(hit->GetX());
        Y.push_back(hit->GetY());
        Z.push_back(hit->GetZ());
      }
    }

    if (tofId > -1) {
      const CbmPixelHit* hit = static_cast<const CbmPixelHit*>(fTofHits->At(tofId));
      X.push_back(hit->GetX());
      Y.push_back(hit->GetY());
      Z.push_back(hit->GetZ());
    }

    // Calculate distances between hits
    Double_t length = 0.;
    for (Int_t j = 0; j < X.size() - 1; ++j) {
      Double_t dX = X[j] - X[j + 1];
      Double_t dY = Y[j] - Y[j + 1];
      Double_t dZ = Z[j] - Z[j + 1];
      length += std::sqrt(dX * dX + dY * dY + dZ * dZ);
    }

    if (globalTrack->GetTofHitIndex() == tofTrack->GetTofHitIndex()) globalTrack->SetLength(length);
    tofTrack->SetTrackLength(length);
  }
}

//void CbmLitFindGlobalTracks::CalculateLength()
//{
//   /* Calculate the length of the global track
//    * starting with (0, 0, 0) and adding all
//    * distances between hits
//    */
//
//	// Reduce step to calculate track length more accurately
//	CbmLitTGeoTrackPropagator::MAXIMUM_PROPAGATION_STEP_SIZE = 1.0;
//
//	TrackPtrVector litTracks;
//	CbmLitConverter::GlobalTrackArrayToLitTrackVector(fGlobalTracks, fStsTracks, fTrdTracks, fMuchTracks, fMvdHits, fStsHits, fTrdHits, fMuchPixelHits, fTofHits, litTracks);
//	Int_t nofTracks = litTracks.size();
//	for (UInt_t iTrack = 0; iTrack < nofTracks; iTrack++) {
//		CbmGlobalTrack* globalTrack = static_cast<CbmGlobalTrack*>(fGlobalTracks->At(iTrack));
//		if (globalTrack->GetStsTrackIndex() >= 0 && globalTrack->GetTofHitIndex() >= 0) {
//			CbmLitTrack* track = litTracks[iTrack];
//			CbmLitTrackParam par = *track->GetParamFirst();
//			LitStatus propStatus = fPropagator->Propagate(&par, 0.0, 211, NULL);
//			track->SetParamFirst(&par);
//			fFitter->Fit(track);
//			globalTrack->SetLength(track->GetLength());
//		} else {
//			globalTrack->SetLength(-1.);
//		}
//	}
//
//	CbmLitTGeoTrackPropagator::MAXIMUM_PROPAGATION_STEP_SIZE = 10.;
//
//	// free memory
//	for (UInt_t iTrack = 0; iTrack < nofTracks; iTrack++) {
//		CbmLitTrack* track = litTracks[iTrack];
//		Int_t nofHits = track->GetNofHits();
//		for (Int_t iHit = 0; iHit < nofHits; iHit++) {
//			delete track->GetHit(iHit);
//		}
//		track->ClearHits();
//	}
//	for_each(litTracks.begin(), litTracks.end(), DeleteObject());
//}

void CbmLitFindGlobalTracks::CalculatePrimaryVertexParameters(CbmEvent* event)
{
  if (0 == fGlobalTracks) return;

  CbmVertex* primVertex = event ? event->GetVertex() : fPrimVertex;

  if (0 == primVertex) return;

  Int_t nofGlobalTracks = event ? event->GetNofData(ECbmDataType::kGlobalTrack) : fGlobalTracks->GetEntriesFast();

  for (Int_t i0 = 0; i0 < nofGlobalTracks; ++i0) {
    Int_t i                     = event ? event->GetIndex(ECbmDataType::kGlobalTrack, i0) : i0;
    CbmGlobalTrack* globalTrack = static_cast<CbmGlobalTrack*>(fGlobalTracks->At(i));
    Int_t stsId                 = globalTrack->GetStsTrackIndex();
    CbmStsTrack* stsTrack       = static_cast<CbmStsTrack*>(fStsTracks->At(stsId));
    FairTrackParam vtxTrackParam;
    float chiSqPrimary = 0.f;
    CbmKFParticleInterface::ExtrapolateTrackToPV(stsTrack, primVertex, &vtxTrackParam, chiSqPrimary);
    globalTrack->SetParamPrimaryVertex(&vtxTrackParam);
  }
}

void CbmLitFindGlobalTracks::ClearArrays()
{
  // Free memory
  for_each(fLitStsTracks.begin(), fLitStsTracks.end(), DeleteObject());
  for_each(fLitOutputTracks.begin(), fLitOutputTracks.end(), DeleteObject());
  for_each(fLitHits.begin(), fLitHits.end(), DeleteObject());
  for_each(fLitTofHits.begin(), fLitTofHits.end(), DeleteObject());
  for_each(fLitOutputTofTracks.begin(), fLitOutputTofTracks.end(), DeleteObject());
  fLitStsTracks.clear();
  fLitOutputTracks.clear();
  fLitHits.clear();
  fLitTofHits.clear();
  fLitOutputTofTracks.clear();
}

void CbmLitFindGlobalTracks::RunTrackReconstruction()
{
  // Track finding in TRD or MUCH
  if (fDet.GetDet(ECbmModuleId::kMuch) || fDet.GetDet(ECbmModuleId::kTrd)) {
    fTrackingWatch.Start(kFALSE);
    fFinder->DoFind(fLitHits, fLitStsTracks, fLitOutputTracks);
    fTrackingWatch.Stop();
  }
  // Merging of TOF hits to global tracks
  if (fDet.GetDet(ECbmModuleId::kTof)) {
    // If there are no TRD or MUCH than merge STS tracks with TOF
    if (!(fDet.GetDet(ECbmModuleId::kMuch) || fDet.GetDet(ECbmModuleId::kTrd))) {
      for (TrackPtrIterator it = fLitStsTracks.begin(); it != fLitStsTracks.end(); it++) {
        CbmLitTrack* track = new CbmLitTrack(*(*it));
        fLitOutputTracks.push_back(track);
      }
    }

    // Selection of tracks to be merged with TOF
    if (fDet.GetDet(ECbmModuleId::kMuch) || fDet.GetDet(ECbmModuleId::kTrd)) { SelectTracksForTofMerging(); }
    else {
      for (TrackPtrIterator it = fLitOutputTracks.begin(); it != fLitOutputTracks.end(); it++) {
        (*it)->SetQuality(kLITGOODMERGE);
      }
    }

    fMergerWatch.Start(kFALSE);
    fMerger->DoMerge(fLitTofHits, fLitOutputTracks, fLitOutputTofTracks);
    fMergerWatch.Stop();
  }

  // Refit found tracks
  for (TrackPtrIterator it = fLitOutputTracks.begin(); it != fLitOutputTracks.end(); it++) {
    CbmLitTrack* track = *it;
    fFitter->Fit(track);
  }
}

void CbmLitFindGlobalTracks::SelectTracksForTofMerging()
{
  // The aim of this procedure is to select only those tracks
  // which have at least one hit in the last station group.
  // Only those tracks will be propagated further and merged
  // with TOF hits.

  Int_t nofStations = CbmLitTrackingGeometryConstructor::Instance()->GetNofMuchTrdStations();
  //   Int_t stationCut = nofStations - 4;
  // TODO: Fix this issue in a better way. This is done only as an ugly fix
  // FU 19.09.13
  Int_t stationCut = nofStations - 3;

  for (TrackPtrIterator it = fLitOutputTracks.begin(); it != fLitOutputTracks.end(); it++) {
    CbmLitTrack* track = *it;
    if (track->GetQuality() == kLITBAD) { continue; }
    const CbmLitHit* hit = track->GetHit(track->GetNofHits() - 1);
    if (hit->GetStation() >= stationCut) {
      // OK select this track for further merging with TOF
      track->SetQuality(kLITGOODMERGE);
    }
  }
}

void CbmLitFindGlobalTracks::PrintStopwatchStatistics()
{
  std::cout << "Stopwatch: " << std::endl;
  std::cout << "tracking: counts=" << fTrackingWatch.Counter()
            << ", real=" << fTrackingWatch.RealTime() / fTrackingWatch.Counter() << "/" << fTrackingWatch.RealTime()
            << " s, cpu=" << fTrackingWatch.CpuTime() / fTrackingWatch.Counter() << "/" << fTrackingWatch.CpuTime()
            << std::endl;
  std::cout << "fitter: real=" << fMergerWatch.Counter()
            << ", real=" << fMergerWatch.RealTime() / fMergerWatch.Counter() << "/" << fMergerWatch.RealTime()
            << " s, cpu=" << fMergerWatch.CpuTime() / fMergerWatch.Counter() << "/" << fMergerWatch.CpuTime()
            << std::endl;
}

ClassImp(CbmLitFindGlobalTracks);
