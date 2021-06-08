/* Copyright (C) 2006-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Denis Bertini [committer], Volker Friese, Florian Uhlig */

// -------------------------------------------------------------------------
// -----                  CbmStsFindTracksQa source file               -----
// -----                  Created 11/01/06  by V. Friese               -----
// -------------------------------------------------------------------------

// Includes class header
#include "CbmStsFindTracksQa.h"

// Includes from C++
#include <cassert>
#include <iomanip>

// Includes from ROOT
#include "TClonesArray.h"
#include "TGeoManager.h"
#include "TH1F.h"

// Includes from FairRoot
#include "FairEventHeader.h"
#include "FairRun.h"
#include <Logger.h>

// Includes from CbmRoot
#include "CbmMCDataArray.h"
#include "CbmMCDataManager.h"
#include "CbmMCTrack.h"
#include "CbmMvdDetector.h"
#include "CbmMvdHit.h"
#include "CbmMvdPoint.h"
#include "CbmMvdStationPar.h"
#include "CbmStsHit.h"
#include "CbmStsPoint.h"
#include "CbmStsSetup.h"
#include "CbmStsTrack.h"
#include "CbmTimeSlice.h"
#include "CbmTrackMatchNew.h"

#include "FairRunAna.h"

using std::fixed;
using std::right;
using std::setprecision;
using std::setw;


// -----   Default constructor   -------------------------------------------
CbmStsFindTracksQa::CbmStsFindTracksQa(Int_t iVerbose) : FairTask("STSFindTracksQA", iVerbose) {}

// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
CbmStsFindTracksQa::CbmStsFindTracksQa(Int_t minStations, Double_t quota, Int_t iVerbose)
  : FairTask("STSFindTracksQA", iVerbose)
  , fMinStations(minStations)
  , fQuota(quota)
{
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmStsFindTracksQa::~CbmStsFindTracksQa()
{

  fHistoList->Delete();
  delete fHistoList;
}
// -------------------------------------------------------------------------


// -----   Task execution   ------------------------------------------------
void CbmStsFindTracksQa::Exec(Option_t* /*opt*/)
{

  LOG(debug) << GetName() << ": Process event ";

  // Timer
  fTimer.Start();

  // Eventwise counters
  //  Int_t nMCTracks = 0;
  Int_t nTracks     = 0;
  Int_t nGhosts     = 0;
  Int_t nClones     = 0;
  Int_t nAll        = 0;
  Int_t nAcc        = 0;
  Int_t nRecAll     = 0;
  Int_t nPrim       = 0;
  Int_t nRecPrim    = 0;
  Int_t nRef        = 0;
  Int_t nRecRef     = 0;
  Int_t nRefLong    = 0;
  Int_t nRecRefLong = 0;
  Int_t nSec        = 0;
  Int_t nRecSec     = 0;
  TVector3 momentum;
  TVector3 vertex;

  // check consistency
  assert(fStsTracks->GetEntriesFast() == fStsTrackMatches->GetEntriesFast());

  {
    fMcTrackInfoMap.clear();
    std::vector<CbmLink> events = fTimeSlice->GetMatch().GetLinks();
    std::sort(events.begin(), events.end());
    McTrackInfo info;
    for (uint iLink = 0; iLink < events.size(); iLink++) {
      CbmLink link    = events[iLink];
      Int_t nMCTracks = fMCTracks->Size(link);
      for (Int_t iTr = 0; iTr < nMCTracks; iTr++) {
        link.SetIndex(iTr);
        fMcTrackInfoMap.insert({link, info});
      }
    }
  }

  // Fill hit and track maps
  FillHitMap();
  FillMatchMap(nTracks, nGhosts, nClones);

  int nMcTracks = fMcTrackInfoMap.size();

  // Loop over MCTracks
  int iMcTrack = 0;
  for (auto itTrack = fMcTrackInfoMap.begin(); itTrack != fMcTrackInfoMap.end(); ++itTrack, ++iMcTrack) {
    const CbmLink& link = itTrack->first;
    McTrackInfo& info   = itTrack->second;
    CbmMCTrack* mcTrack = dynamic_cast<CbmMCTrack*>(fMCTracks->Get(link));
    assert(mcTrack);

    // Continue only for reconstructible tracks
    nAll++;
    Int_t nStations = info.fHitMap.size();
    if (nStations < fMinStations) continue;  // Too few stations

    int nContStations = 0;  // Number of continious stations
    {
      int istaprev = -1;
      int len      = 0;
      for (auto itSta = info.fHitMap.begin(); itSta != info.fHitMap.end(); itSta++) {
        if (len == 0 || itSta->first == istaprev + 1) { len++; }
        else {
          len = 1;
        }
        if (nContStations < len) { nContStations = len; }
        istaprev = itSta->first;
      }
    }
    if (nContStations < fMinStations) continue;  // Too few stations

    nAcc++;

    // Check origin of MCTrack
    // TODO: Track origin should rather be compared to MC event vertex
    // But that is not available from MCDataManager
    mcTrack->GetStartVertex(vertex);
    Bool_t isPrim = kFALSE;
    if (TMath::Abs(vertex.Z() - fTargetPos.Z()) < 1.) {
      isPrim = kTRUE;
      nPrim++;
    }
    else
      nSec++;

    // Get momentum
    mcTrack->GetMomentum(momentum);
    Double_t mom = momentum.Mag();
    Bool_t isRef = kFALSE;
    if (mom > 1. && isPrim) {
      isRef = kTRUE;
      nRef++;
    }

    Bool_t isRefLong = kFALSE;
    if (isRef && nContStations >= fStsNstations) {
      isRefLong = kTRUE;
      nRefLong++;
    }

    // Fill histograms for reconstructible tracks
    fhMomAccAll->Fill(mom);
    fhNpAccAll->Fill(Double_t(nStations));
    if (isPrim) {
      fhMomAccPrim->Fill(mom);
      fhNpAccPrim->Fill(Double_t(nStations));
    }
    else {
      fhMomAccSec->Fill(mom);
      fhNpAccSec->Fill(Double_t(nStations));
      fhZAccSec->Fill(vertex.Z());
    }

    // Get matched StsTrack
    Int_t trackId  = info.fStsTrackMatch;
    Double_t quali = info.fQuali;
    //    Bool_t   isRec    = kFALSE;
    if (trackId >= 0) {
      //      isRec = kTRUE;
      CbmStsTrack* stsTrack = (CbmStsTrack*) fStsTracks->At(trackId);
      assert(stsTrack);
      assert(quali >= fQuota);
      CbmTrackMatchNew* match = (CbmTrackMatchNew*) fStsTrackMatches->At(trackId);
      assert(match);
      Int_t nTrue  = match->GetNofTrueHits();
      Int_t nWrong = match->GetNofWrongHits();
      //Int_t nFake  = match->GetNofFakeHits();
      Int_t nFake    = 0;
      Int_t nAllHits = stsTrack->GetNofStsHits() + stsTrack->GetNofMvdHits();
      if (!fIsMvdActive) { assert(stsTrack->GetNofMvdHits() == 0); }
      assert(nTrue + nWrong + nFake == nAllHits);
      // Verbose output
      LOG(debug1) << GetName() << ": MCTrack " << iMcTrack << ", stations " << nStations << ", hits " << nAllHits
                  << ", true hits " << nTrue;

      // Fill histograms for reconstructed tracks
      nRecAll++;
      fhMomRecAll->Fill(mom);
      fhNpRecAll->Fill(Double_t(nAllHits));
      if (isPrim) {
        nRecPrim++;
        fhMomRecPrim->Fill(mom);
        fhNpRecPrim->Fill(Double_t(nAllHits));
      }
      else {
        nRecSec++;
        fhMomRecSec->Fill(mom);
        fhNpRecSec->Fill(Double_t(nAllHits));
        fhZRecSec->Fill(vertex.Z());
      }
      if (isRef) nRecRef++;
      if (isRefLong) nRecRefLong++;

    }  // Match found in map?

  }  // Loop over MCTracks

  // Calculate efficiencies
  Double_t effAll     = Double_t(nRecAll) / Double_t(nAcc);
  Double_t effPrim    = Double_t(nRecPrim) / Double_t(nPrim);
  Double_t effRef     = Double_t(nRecRef) / Double_t(nRef);
  Double_t effRefLong = Double_t(nRecRefLong) / Double_t(nRefLong);
  Double_t effSec     = Double_t(nRecSec) / Double_t(nSec);

  fTimer.Stop();


  // Event summary
  LOG(info) << "+ " << setw(20) << GetName() << ": Event " << setw(6) << right << fNEvents << ", real time " << fixed
            << setprecision(6) << fTimer.RealTime() << " s, MC tracks: all " << nMcTracks << ", acc. " << nAcc
            << ", rec. " << nRecAll << ", eff. " << setprecision(2) << 100. * effAll << " %";
  if (fair::Logger::Logging(fair::Severity::debug)) {
    LOG(debug) << "----------   StsFindTracksQa : Event summary   ------------";
    LOG(debug) << "MCTracks   : " << nAll << ", reconstructible: " << nAcc << ", reconstructed: " << nRecAll;
    LOG(debug) << "Vertex     : reconstructible: " << nPrim << ", reconstructed: " << nRecPrim << ", efficiency "
               << effPrim * 100. << "%";
    LOG(debug) << "Reference  : reconstructible: " << nRef << ", reconstructed: " << nRecRef << ", efficiency "
               << effRef * 100. << "%";
    LOG(debug) << "Reference long : reconstructible: " << nRefLong << ", reconstructed: " << nRecRefLong
               << ", efficiency " << effRefLong * 100. << "%";
    LOG(debug) << "Non-vertex : reconstructible: " << nSec << ", reconstructed: " << nRecSec << ", efficiency "
               << effSec * 100. << "%";
    LOG(debug) << "STSTracks " << nTracks << ", ghosts " << nGhosts << ", clones " << nClones;
    LOG(debug) << "-----------------------------------------------------------\n";
  }


  // Increase counters
  fNAll += nAll;
  fNAccAll += nAcc;
  fNAccPrim += nPrim;
  fNAccRef += nRef;
  fNAccRefLong += nRefLong;
  fNAccSec += nSec;
  fNRecAll += nRecAll;
  fNRecPrim += nRecPrim;
  fNRecRef += nRecRef;
  fNRecRefLong += nRecRefLong;
  fNRecSec += nRecSec;
  fNGhosts += nGhosts;
  fNClones += nClones;
  fNEvents++;
  fTime += fTimer.RealTime();
}
// -------------------------------------------------------------------------


// -----   Public method SetParContainers   --------------------------------
void CbmStsFindTracksQa::SetParContainers() {}
// -------------------------------------------------------------------------


// -----   Public method Init   --------------------------------------------
InitStatus CbmStsFindTracksQa::Init()
{

  LOG(info) << "\n\n====================================================";
  LOG(info) << GetName() << ": Initialising...";

  // Get STS setup
  fStsSetup = CbmStsSetup::Instance();
  assert(fStsSetup);
  if (!fStsSetup->IsInit()) { fStsSetup->Init(); }

  fManager = FairRootManager::Instance();
  assert(fManager);

  fMcManager = dynamic_cast<CbmMCDataManager*>(fManager->GetObject("MCDataManager"));

  assert(fMcManager);

  fTimeSlice = static_cast<CbmTimeSlice*>(fManager->GetObject("TimeSlice."));

  if (fTimeSlice == nullptr) { LOG(fatal) << "CbmStsFindTracksQa: No time slice object"; }

  if (fMcManager) {
    fMCTracks  = fMcManager->InitBranch("MCTrack");
    fStsPoints = fMcManager->InitBranch("StsPoint");
  }

  assert(fMCTracks);
  assert(fStsPoints);

  // Get the geometry
  InitStatus geoStatus = GetGeometry();
  if (geoStatus != kSUCCESS) {
    LOG(error) << GetName() << "::Init: Error in reading geometry!";
    return geoStatus;
  }

  // MVD

  fMvdPoints   = fMcManager->InitBranch("MvdPoint");
  fMvdCluster  = (TClonesArray*) (fManager->GetObject("MvdCluster"));
  fMvdHits     = (TClonesArray*) (fManager->GetObject("MvdHit"));
  fMvdHitMatch = (TClonesArray*) fManager->GetObject("MvdHitMatch");

  // Currently in the time-based mode MVD is present but not reconstructed
  // TODO: remove the check once the reconstruction works
  if (fIsMvdActive && !fMvdHits) {
    LOG(warning) << "CbmStsFindTracksQa: MVD hits are missing, MVD will not be "
                    "included to the STS track match";
    fIsMvdActive = false;
  }

  if (fIsMvdActive) {
    assert(fMvdPoints);
    assert(fMvdCluster);
    assert(fMvdHits);
    assert(fMvdHitMatch);
  }

  // STS

  // Get StsHit array
  fStsHits = (TClonesArray*) fManager->GetObject("StsHit");
  assert(fStsHits);

  // Get StsHitMatch array
  fStsHitMatch = (TClonesArray*) fManager->GetObject("StsHitMatch");
  assert(fStsHitMatch);

  // Get StsHitMatch array
  fStsClusterMatch = (TClonesArray*) fManager->GetObject("StsClusterMatch");
  assert(fStsClusterMatch);

  // Get StsTrack array
  fStsTracks = (TClonesArray*) fManager->GetObject("StsTrack");
  assert(fStsTracks);

  // Get StsTrackMatch array
  fStsTrackMatches = (TClonesArray*) fManager->GetObject("StsTrackMatch");
  assert(fStsTrackMatches);


  // Create histograms
  CreateHistos();
  Reset();

  // Output
  LOG(info) << "   Number of STS stations : " << fStsNstations;
  LOG(info) << "   Target position ( " << fTargetPos.X() << ", " << fTargetPos.Y() << ", " << fTargetPos.Z() << ") cm";
  LOG(info) << "   Minimum number of STS stations   : " << fMinStations;
  LOG(info) << "   Matching quota               : " << fQuota;
  LOG(info) << "====================================================";

  return geoStatus;
}
// -------------------------------------------------------------------------


// -----   Public method ReInit   ------------------------------------------
InitStatus CbmStsFindTracksQa::ReInit()
{

  LOG(info) << "\n\n====================================================";
  LOG(info) << GetName() << ": Re-initialising...";

  // Get the geometry of target and STS
  InitStatus geoStatus = GetGeometry();
  if (geoStatus != kSUCCESS) {
    LOG(error) << GetName() << "::Init: Error in reading geometry!";
    return geoStatus;
  }

  // --- Screen log
  LOG(info) << "   Number of STS stations : " << fStsNstations;
  LOG(info) << "   Target position ( " << fTargetPos.X() << ", " << fTargetPos.Y() << ", " << fTargetPos.Z() << ") cm";
  LOG(info) << "   Minimum number of STS stations   : " << fMinStations;
  LOG(info) << "   Matching quota               : " << fQuota;
  LOG(info) << "====================================================";

  return geoStatus;
}
// -------------------------------------------------------------------------


// -----   Private method Finish   -----------------------------------------
void CbmStsFindTracksQa::Finish()
{

  // Divide histograms for efficiency calculation
  DivideHistos(fhMomRecAll, fhMomAccAll, fhMomEffAll);
  DivideHistos(fhMomRecPrim, fhMomAccPrim, fhMomEffPrim);
  DivideHistos(fhMomRecSec, fhMomAccSec, fhMomEffSec);
  DivideHistos(fhNpRecAll, fhNpAccAll, fhNpEffAll);
  DivideHistos(fhNpRecPrim, fhNpAccPrim, fhNpEffPrim);
  DivideHistos(fhNpRecSec, fhNpAccSec, fhNpEffSec);
  DivideHistos(fhZRecSec, fhZAccSec, fhZEffSec);

  // Normalise histos for clones and ghosts to one event
  if (fNEvents) {
    fhNhClones->Scale(1. / Double_t(fNEvents));
    fhNhGhosts->Scale(1. / Double_t(fNEvents));
  }

  // Calculate integrated efficiencies and rates
  Double_t effAll     = Double_t(fNRecAll) / Double_t(fNAccAll);
  Double_t effPrim    = Double_t(fNRecPrim) / Double_t(fNAccPrim);
  Double_t effRef     = Double_t(fNRecRef) / Double_t(fNAccRef);
  Double_t effRefLong = Double_t(fNRecRefLong) / Double_t(fNAccRefLong);
  Double_t effSec     = Double_t(fNRecSec) / Double_t(fNAccSec);
  Double_t rateGhosts = Double_t(fNGhosts) / Double_t(fNRecAll);
  Double_t rateClones = Double_t(fNClones) / Double_t(fNRecAll);

  // Run summary to screen
  std::cout << std::endl;
  LOG(info) << "=====================================";
  LOG(info) << fName << ": Run summary ";
  LOG(info) << "Events processed      : " << fNEvents << setprecision(2);
  LOG(info) << "Eff. all tracks       : " << effAll * 100 << " % (" << fNRecAll << "/" << fNAccAll << ")";
  LOG(info) << "Eff. vertex tracks    : " << effPrim * 100 << " % (" << fNRecPrim << "/" << fNAccPrim << ")";
  LOG(info) << "Eff. reference tracks : " << effRef * 100 << " % (" << fNRecRef << "/" << fNAccRef << ")";
  LOG(info) << "Eff. reference long tracks : " << effRefLong * 100 << " % (" << fNRecRefLong << "/" << fNAccRefLong
            << ")";
  LOG(info) << "Eff. secondary tracks : " << effSec * 100 << " % (" << fNRecSec << "/" << fNAccSec << ")";
  LOG(info) << "Ghost rate            : " << rateGhosts * 100 << " % (" << fNGhosts << "/" << fNRecAll << ")";
  LOG(info) << "Clone rate            : " << rateClones * 100 << " % (" << fNClones << "/" << fNRecAll << ")";
  LOG(info) << "mc tracks/event " << fNAll / fNEvents << " accepted " << fNRecAll / fNEvents;
  LOG(info) << "Time per event        : " << setprecision(6) << fTime / Double_t(fNEvents) << " s";

  if (fMvdNstations > 0 && !fIsMvdActive) {
    LOG(warning) << "CbmStsFindTracksQa: MVD hits are missing, MVD is not "
                    "included to the STS track match";
  }

  LOG(info) << "=====================================";

  // Write histos to output
  /*
  gDirectory->mkdir("STSFindTracksQA");
  gDirectory->cd("STSFindTracksQA");
  TIter next(fHistoList);
  while (TH1* histo = ((TH1*) next()))
    histo->Write();
  gDirectory->cd("..");
*/
  FairSink* sink = FairRootManager::Instance()->GetSink();
  sink->WriteObject(&fOutFolder, nullptr);
}
// -------------------------------------------------------------------------


// -----   Private method GetGeometry   ------------------------------------
InitStatus CbmStsFindTracksQa::GetGeometry()
{
  // Get target geometry
  GetTargetPosition();
  fMvdNstations = 0;
  {
    CbmMvdDetector* mvdDetector = CbmMvdDetector::Instance();
    if (mvdDetector) {
      CbmMvdStationPar* mvdStationPar = mvdDetector->GetParameterFile();
      assert(mvdStationPar);
      fMvdNstations = mvdStationPar->GetStationCount();
    }
  }
  fIsMvdActive  = (fMvdNstations > 0);
  fStsNstations = CbmStsSetup::Instance()->GetNofStations();
  return kSUCCESS;
}
// -------------------------------------------------------------------------


// -----   Get target node   -----------------------------------------------
void CbmStsFindTracksQa::GetTargetPosition()
{

  TGeoNode* target = NULL;

  gGeoManager->CdTop();
  TGeoNode* cave = gGeoManager->GetCurrentNode();
  for (Int_t iNode1 = 0; iNode1 < cave->GetNdaughters(); iNode1++) {
    TString name = cave->GetDaughter(iNode1)->GetName();
    if (name.Contains("pipe", TString::kIgnoreCase)) {
      LOG(debug) << "Found pipe node " << name;
      gGeoManager->CdDown(iNode1);
      break;
    }
  }
  for (Int_t iNode2 = 0; iNode2 < gGeoManager->GetCurrentNode()->GetNdaughters(); iNode2++) {
    TString name = gGeoManager->GetCurrentNode()->GetDaughter(iNode2)->GetName();
    if (name.Contains("pipevac1", TString::kIgnoreCase)) {
      LOG(debug) << "Found vacuum node " << name;
      gGeoManager->CdDown(iNode2);
      break;
    }
  }
  for (Int_t iNode3 = 0; iNode3 < gGeoManager->GetCurrentNode()->GetNdaughters(); iNode3++) {
    TString name = gGeoManager->GetCurrentNode()->GetDaughter(iNode3)->GetName();
    if (name.Contains("target", TString::kIgnoreCase)) {
      LOG(debug) << "Found target node " << name;
      gGeoManager->CdDown(iNode3);
      target = gGeoManager->GetCurrentNode();
      break;
    }
  }
  if (!target) {
    fTargetPos[0] = 0.;
    fTargetPos[1] = 0.;
    fTargetPos[2] = 0.;
  }
  else {
    TGeoHMatrix* glbMatrix = gGeoManager->GetCurrentMatrix();
    Double_t* pos          = glbMatrix->GetTranslation();
    fTargetPos[0]          = pos[0];
    fTargetPos[1]          = pos[1];
    fTargetPos[2]          = pos[2];
  }

  gGeoManager->CdTop();
}
// -------------------------------------------------------------------------


// -----   Private method CreateHistos   -----------------------------------
void CbmStsFindTracksQa::CreateHistos()
{

  fOutFolder.Clear();

  // Histogram list
  fHistoList = new TList();

  // Momentum distributions
  Double_t minMom = 0.;
  Double_t maxMom = 10.;
  Int_t nBinsMom  = 40;
  fhMomAccAll     = new TH1F("hMomAccAll", "all reconstructable tracks", nBinsMom, minMom, maxMom);
  fhMomRecAll     = new TH1F("hMomRecAll", "all reconstructed tracks", nBinsMom, minMom, maxMom);
  fhMomEffAll     = new TH1F("hMomEffAll", "efficiency all tracks", nBinsMom, minMom, maxMom);
  fhMomAccPrim    = new TH1F("hMomAccPrim", "reconstructable vertex tracks", nBinsMom, minMom, maxMom);
  fhMomRecPrim    = new TH1F("hMomRecPrim", "reconstructed vertex tracks", nBinsMom, minMom, maxMom);
  fhMomEffPrim    = new TH1F("hMomEffPrim", "efficiency vertex tracks", nBinsMom, minMom, maxMom);
  fhMomAccSec     = new TH1F("hMomAccSec", "reconstructable non-vertex tracks", nBinsMom, minMom, maxMom);
  fhMomRecSec     = new TH1F("hMomRecSec", "reconstructed non-vertex tracks", nBinsMom, minMom, maxMom);
  fhMomEffSec     = new TH1F("hMomEffSec", "efficiency non-vertex tracks", nBinsMom, minMom, maxMom);
  fHistoList->Add(fhMomAccAll);
  fHistoList->Add(fhMomRecAll);
  fHistoList->Add(fhMomEffAll);
  fHistoList->Add(fhMomAccPrim);
  fHistoList->Add(fhMomRecPrim);
  fHistoList->Add(fhMomEffPrim);
  fHistoList->Add(fhMomAccSec);
  fHistoList->Add(fhMomRecSec);
  fHistoList->Add(fhMomEffSec);

  // Number-of-points distributions
  Double_t minNp = -0.5;
  Double_t maxNp = 15.5;
  Int_t nBinsNp  = 16;
  fhNpAccAll     = new TH1F("hNpAccAll", "all reconstructable tracks", nBinsNp, minNp, maxNp);
  fhNpRecAll     = new TH1F("hNpRecAll", "all reconstructed tracks", nBinsNp, minNp, maxNp);
  fhNpEffAll     = new TH1F("hNpEffAll", "efficiency all tracks", nBinsNp, minNp, maxNp);
  fhNpAccPrim    = new TH1F("hNpAccPrim", "reconstructable vertex tracks", nBinsNp, minNp, maxNp);
  fhNpRecPrim    = new TH1F("hNpRecPrim", "reconstructed vertex tracks", nBinsNp, minNp, maxNp);
  fhNpEffPrim    = new TH1F("hNpEffPrim", "efficiency vertex tracks", nBinsNp, minNp, maxNp);
  fhNpAccSec     = new TH1F("hNpAccSec", "reconstructable non-vertex tracks", nBinsNp, minNp, maxNp);
  fhNpRecSec     = new TH1F("hNpRecSec", "reconstructed non-vertex tracks", nBinsNp, minNp, maxNp);
  fhNpEffSec     = new TH1F("hNpEffSec", "efficiency non-vertex tracks", nBinsNp, minNp, maxNp);
  fHistoList->Add(fhNpAccAll);
  fHistoList->Add(fhNpRecAll);
  fHistoList->Add(fhNpEffAll);
  fHistoList->Add(fhNpAccPrim);
  fHistoList->Add(fhNpRecPrim);
  fHistoList->Add(fhNpEffPrim);
  fHistoList->Add(fhNpAccSec);
  fHistoList->Add(fhNpRecSec);
  fHistoList->Add(fhNpEffSec);

  // z(vertex) distributions
  Double_t minZ = 0.;
  Double_t maxZ = 50.;
  Int_t nBinsZ  = 50;
  fhZAccSec     = new TH1F("hZAccSec", "reconstructable non-vertex tracks", nBinsZ, minZ, maxZ);
  fhZRecSec     = new TH1F("hZRecSecl", "reconstructed non-vertex tracks", nBinsZ, minZ, maxZ);
  fhZEffSec     = new TH1F("hZEffRec", "efficiency non-vertex tracks", nBinsZ, minZ, maxZ);
  fHistoList->Add(fhZAccSec);
  fHistoList->Add(fhZRecSec);
  fHistoList->Add(fhZEffSec);

  // Number-of-hit distributions
  fhNhClones = new TH1F("hNhClones", "number of hits for clones", nBinsNp, minNp, maxNp);
  fhNhGhosts = new TH1F("hNhGhosts", "number of hits for ghosts", nBinsNp, minNp, maxNp);
  fHistoList->Add(fhNhClones);
  fHistoList->Add(fhNhGhosts);

  TIter next(fHistoList);
  while (TH1* histo = ((TH1*) next())) {
    fOutFolder.Add(histo);
  }
}
// -------------------------------------------------------------------------


// -----   Private method Reset   ------------------------------------------
void CbmStsFindTracksQa::Reset()
{

  TIter next(fHistoList);
  while (TH1* histo = ((TH1*) next()))
    histo->Reset();

  fNAccAll = fNAccPrim = fNAccRef = fNAccRefLong = fNAccSec = 0;
  fNRecAll = fNRecPrim = fNRecRef = fNRecRefLong = fNRecSec = 0;
  fNGhosts = fNClones = fNEvents = 0;
}
// -------------------------------------------------------------------------


// -----   Private method FillHitMap   -------------------------------------
void CbmStsFindTracksQa::FillHitMap()
{

  // --- Fill hit map ( mcTrack -> ( station -> number of hits ) )

  // pocess MVD hits

  if (fIsMvdActive) {
    assert(fMvdHits);
    assert(fMvdHitMatch);
    assert(fMvdPoints);
    for (Int_t iHit = 0; iHit < fMvdHits->GetEntriesFast(); iHit++) {
      CbmMvdHit* hit = (CbmMvdHit*) fMvdHits->At(iHit);
      assert(hit);
      Int_t station = hit->GetStationNr();
      assert(station >= 0 && station < fMvdNstations);
      const CbmMatch* match = (const CbmMatch*) fMvdHitMatch->At(iHit);
      assert(match);
      if (match->GetNofLinks() <= 0) continue;
      CbmLink link    = match->GetMatchedLink();
      CbmMvdPoint* pt = (CbmMvdPoint*) fMvdPoints->Get(link);
      assert(pt);
      link.SetIndex(pt->GetTrackID());
      McTrackInfo& info = getMcTrackInfo(link);
      info.fHitMap[station]++;
    }
  }

  // pocess STS hits

  for (Int_t iHit = 0; iHit < fStsHits->GetEntriesFast(); iHit++) {
    CbmStsHit* hit = (CbmStsHit*) fStsHits->At(iHit);
    assert(hit);

    Int_t station = fStsSetup->GetStationNumber(hit->GetAddress());

    // carefully check the hit match by looking at the strips from both sides

    const CbmMatch* frontMatch = dynamic_cast<const CbmMatch*>(fStsClusterMatch->At(hit->GetFrontClusterId()));
    assert(frontMatch);
    if (frontMatch->GetNofLinks() <= 0) continue;

    const CbmMatch* backMatch = dynamic_cast<const CbmMatch*>(fStsClusterMatch->At(hit->GetBackClusterId()));
    assert(backMatch);
    if (backMatch->GetNofLinks() <= 0) continue;

    if (frontMatch->GetMatchedLink() == backMatch->GetMatchedLink()) {
      CbmLink link    = frontMatch->GetMatchedLink();
      CbmStsPoint* pt = (CbmStsPoint*) fStsPoints->Get(link);
      assert(pt);
      link.SetIndex(pt->GetTrackID());
      McTrackInfo& info = getMcTrackInfo(link);
      info.fHitMap[fMvdNstations + station]++;
    }
  }
  LOG(debug) << GetName() << ": Filled hit map from " << fStsHits->GetEntriesFast() << " STS hits";
}
// -------------------------------------------------------------------------


// ------   Private method FillMatchMap   ----------------------------------
void CbmStsFindTracksQa::FillMatchMap(Int_t& nRec, Int_t& nGhosts, Int_t& nClones)
{

  // Clear matching maps
  for (auto it = fMcTrackInfoMap.begin(); it != fMcTrackInfoMap.end(); ++it) {
    McTrackInfo& info      = it->second;
    info.fStsTrackMatch    = -1;
    info.fQuali            = 0.;
    info.fMatchedNHitsAll  = 0;
    info.fMatchedNHitsTrue = 0;
  }

  // Loop over StsTracks. Check matched MCtrack and fill maps.
  nGhosts = 0;
  nClones = 0;

  Int_t nTracks = fStsTracks->GetEntriesFast();
  for (Int_t iTrack = 0; iTrack < nTracks; iTrack++) {

    // --- StsTrack
    CbmStsTrack* stsTrack = (CbmStsTrack*) fStsTracks->At(iTrack);
    assert(stsTrack);
    Int_t nHits = stsTrack->GetNofStsHits();

    // --- TrackMatch

    assert(iTrack >= 0 && iTrack < fStsTrackMatches->GetEntriesFast());
    CbmTrackMatchNew* match = (CbmTrackMatchNew*) fStsTrackMatches->At(iTrack);
    assert(match);
    Int_t nTrue = match->GetNofTrueHits();

    // Check matching criterion (quota)
    Double_t quali = Double_t(nTrue) / Double_t(nHits);

    // Quality isn't good, it's a ghost

    if (quali < fQuota) {
      fhNhGhosts->Fill(nHits);
      nGhosts++;
      continue;
    }

    // Quality is good

    // --- Matched MCTrack
    assert(match->GetNofLinks() > 0);
    const CbmLink& link = match->GetMatchedLink();
    assert(link.GetIndex() >= 0);
    McTrackInfo& info = getMcTrackInfo(link);

    // previous match is better, this track is a clone
    if ((quali < info.fQuali) || ((quali == info.fQuali) && (nTrue < info.fMatchedNHitsTrue))) {
      fhNhClones->Fill(nHits);
      nClones++;
      continue;
    }

    // this track is better than the old one
    if (info.fMatchedNHitsAll > 0) {
      fhNhClones->Fill(info.fMatchedNHitsAll);
      nClones++;
    }
    info.fStsTrackMatch    = iTrack;
    info.fQuali            = quali;
    info.fMatchedNHitsAll  = nHits;
    info.fMatchedNHitsTrue = nTrue;

  }  // Loop over StsTracks

  nRec = nTracks;
  LOG(debug) << GetName() << ": Filled match map for " << nRec << " STS tracks. Ghosts " << nGhosts << " Clones "
             << nClones;
}
// -------------------------------------------------------------------------


// -----   Private method DivideHistos   -----------------------------------
void CbmStsFindTracksQa::DivideHistos(TH1* histo1, TH1* histo2, TH1* histo3)
{

  if (!histo1 || !histo2 || !histo3) {
    LOG(fatal) << GetName() << "::DivideHistos: "
               << "NULL histogram pointer";
  }

  Int_t nBins = histo1->GetNbinsX();
  if (histo2->GetNbinsX() != nBins || histo3->GetNbinsX() != nBins) {
    LOG(error) << GetName() << "::DivideHistos: "
               << "Different bin numbers in histos";
    LOG(error) << histo1->GetName() << " " << histo1->GetNbinsX();
    LOG(error) << histo2->GetName() << " " << histo2->GetNbinsX();
    LOG(error) << histo3->GetName() << " " << histo3->GetNbinsX();
    return;
  }

  Double_t c1, c2, c3, ce;
  for (Int_t iBin = 0; iBin < nBins; iBin++) {
    c1 = histo1->GetBinContent(iBin);
    c2 = histo2->GetBinContent(iBin);
    if (c2 != 0.) {
      c3          = c1 / c2;
      Double_t c4 = (c3 * (1. - c3) / c2);
      if (c4 >= 0.) { ce = TMath::Sqrt(c3 * (1. - c3) / c2); }
      else {
        ce = 0;
      }
    }
    else {
      c3 = 0.;
      ce = 0.;
    }
    histo3->SetBinContent(iBin, c3);
    histo3->SetBinError(iBin, ce);
  }
}
// -------------------------------------------------------------------------


ClassImp(CbmStsFindTracksQa)
