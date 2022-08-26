/* Copyright (C) 2006-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Denis Bertini [committer], Volker Friese, Florian Uhlig */

// -------------------------------------------------------------------------
// -----                  CbmTrackingTrdQa source file               -----
// -----                  Created 11/01/06  by V. Friese               -----
// -------------------------------------------------------------------------

// Includes class header
#include "CbmTrackingTrdQa.h"

// Includes from C++
#include <cassert>
#include <iomanip>

// Includes from ROOT
#include "TClonesArray.h"
#include "TGeoManager.h"
#include "TH1F.h"
#include "TH2F.h"
#include <TPDGCode.h>

// Includes from FairRoot
#include "FairEventHeader.h"
#include "FairRun.h"
#include <Logger.h>

// Includes from CbmRoot
#include "CbmGlobalTrack.h"
#include "CbmMCDataArray.h"
#include "CbmMCDataManager.h"
#include "CbmMCTrack.h"
#include "CbmTimeSlice.h"
#include "CbmTrackMatchNew.h"
#include "CbmTrdHit.h"
#include "CbmTrdPoint.h"
#include "CbmTrdTrack.h"
#include "CbmTrdTrackingInterface.h"

#include "FairRunAna.h"

using std::fixed;
using std::right;
using std::setprecision;
using std::setw;

const char* CbmTrackingTrdQa::fgkIdxName[fgkNpdg] = {"e", "#mu", "#pi", "K", "#bf{p}", "any"};
const char* CbmTrackingTrdQa::fgkIdxSymb[fgkNpdg] = {"e", "mu", "pi", "K", "p", "x"};

// -------------------------------------------------------------------------
int CbmTrackingTrdQa::Pdg2Idx(int pdg)
{
  int idx(5);
  switch (pdg) {
    case kElectron:
    case kPositron: idx = 0; break;
    case kMuonMinus:
    case kMuonPlus: idx = 1; break;
    case kPiMinus:
    case kPiPlus: idx = 2; break;
    case kKMinus:
    case kKPlus: idx = 3; break;
    case kProton:
    case kProtonBar: idx = 4; break;
    default: idx = 5; break;
  }
  return idx;
}
int CbmTrackingTrdQa::Idx2Pdg(int idx)
{
  if (idx < 0 || idx >= 5) return 0;
  int pdg = 0;
  switch (idx) {
    case 0: pdg = kElectron; break;
    case 1: pdg = kMuonMinus; break;
    case 2: pdg = kPiPlus; break;
    case 3: pdg = kKPlus; break;
    case 4: pdg = kProton; break;
  }
  return pdg;
}
const char* CbmTrackingTrdQa::Idx2Name(int idx)
{
  if (idx < 0 || idx >= 5) return "non";
  return fgkIdxName[idx];
}
const char* CbmTrackingTrdQa::Idx2Symb(int idx)
{
  if (idx < 0 || idx >= 5) return "o";
  return fgkIdxSymb[idx];
}


// -----   Default constructor   -------------------------------------------
CbmTrackingTrdQa::CbmTrackingTrdQa(Int_t iVerbose) : FairTask("CbmTrackingTrdQa", iVerbose) {}

// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
CbmTrackingTrdQa::CbmTrackingTrdQa(Int_t minStations, Double_t quota, Int_t iVerbose)
  : FairTask("CbmTrackingTrdQa", iVerbose)
  , fMinStations(minStations)
  , fQuota(quota)
{
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmTrackingTrdQa::~CbmTrackingTrdQa()
{
  fHistoList->Delete();
  delete fHistoList;
}
// -------------------------------------------------------------------------


// -----   Task execution   ------------------------------------------------
void CbmTrackingTrdQa::Exec(Option_t* /*opt*/)
{

  LOG(debug) << GetName() << ": Process event ";

  // Timer
  fTimer.Start();

  // Eventwise counters
  //  Int_t nMCTracks = 0;
  Int_t nTracks      = 0;
  Int_t nGhosts      = 0;
  Int_t nClones      = 0;
  Int_t nAll         = 0;
  Int_t nAcc         = 0;
  Int_t nRecAll      = 0;
  Int_t nPrim        = 0;
  Int_t nRecPrim     = 0;
  Int_t nFast        = 0;
  Int_t nRecFast     = 0;
  Int_t nFastLong    = 0;
  Int_t nRecFastLong = 0;
  Int_t nSec         = 0;
  Int_t nRecSec      = 0;
  TVector3 momentum;
  TVector3 vertex;

  // check consistency
  //assert(fGlobalTracks->GetEntriesFast() == fGlobalTrackMatches->GetEntriesFast());
  assert(fTrdTracks->GetEntriesFast() == fTrdTrackMatches->GetEntriesFast());

  std::vector<CbmLink> events = fTimeSlice->GetMatch().GetLinks();
  std::sort(events.begin(), events.end());

  {
    fMcTrackInfoMap.clear();
    McTrackInfo info;
    info.fIsAccepted          = false;
    info.fNtimesReconstructed = 0;
    info.fIsPrimary           = false;
    info.fIsFast              = false;
    info.fIsLong              = false;

    std::cout << "MC events in slice : " << events.size() << std::endl;

    for (uint iLink = 0; iLink < events.size(); iLink++) {
      CbmLink link    = events[iLink];
      Int_t nMCTracks = fMCTracks->Size(link);
      std::cout << "MC event " << iLink << " n mc tracks " << nMCTracks << std::endl;
      for (Int_t iTr = 0; iTr < nMCTracks; iTr++) {
        link.SetIndex(iTr);
        fMcTrackInfoMap.insert({link, info});
      }
    }
  }

  // Fill hit and track maps
  FillHitMap();
  FillTrackMatchMap(nTracks, nGhosts, nClones);

  int nMcTracks = fMcTrackInfoMap.size();

  // Loop over MCTracks
  int iMcTrack = 0;
  for (auto itTrack = fMcTrackInfoMap.begin(); itTrack != fMcTrackInfoMap.end(); ++itTrack, ++iMcTrack) {
    const CbmLink& link = itTrack->first;
    McTrackInfo& info   = itTrack->second;
    CbmMCTrack* mcTrack = dynamic_cast<CbmMCTrack*>(fMCTracks->Get(link));
    assert(mcTrack);

    nAll++;

    // Check origin of MCTrack
    // TODO: Track origin should rather be compared to MC event vertex
    // But that is not available from MCDataManager
    mcTrack->GetStartVertex(vertex);
    if (TMath::Abs(vertex.Z() - fTargetPos.Z()) < 1.) { info.fIsPrimary = true; }

    // Get momentum
    mcTrack->GetMomentum(momentum);
    info.fPt     = momentum.Pt();
    info.fPdg    = mcTrack->GetPdgCode();
    info.fY      = mcTrack->GetRapidity() - fYCM;
    info.fIsFast = (info.fPt > 0.1);

    // Continue only for reconstructible tracks

    Int_t nStations = info.fHitMap.size();

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

    info.fIsLong = (nContStations >= fTrdNstations);

    if (nStations < fMinStations) continue;      // Too few stations
    if (nContStations < fMinStations) continue;  // Too few stations

    info.fIsAccepted = true;
    nAcc++;

    if (info.fIsPrimary) { nPrim++; }
    else {
      nSec++;
    }

    if (info.fIsFast) { nFast++; }

    Bool_t isFastLong = (info.fIsFast && info.fIsLong);
    if (isFastLong) { nFastLong++; }

    // Fill histograms for reconstructible tracks

    fhPtAccAll->Fill(info.fPt);
    fhNpAccAll->Fill(Double_t(nStations));
    if (info.fIsPrimary) {
      fhPtAccPrim->Fill(info.fPt);
      fhPidPtY["prmY"][Pdg2Idx(info.fPdg)]->Fill(info.fY, info.fPt);
      fhNpAccPrim->Fill(Double_t(nStations));
    }
    else {
      fhPtAccSec->Fill(info.fPt);
      fhPidPtY["secY"][Pdg2Idx(info.fPdg)]->Fill(info.fY, info.fPt);
      fhNpAccSec->Fill(Double_t(nStations));
      fhZAccSec->Fill(vertex.Z());
    }

    // Get matched GlobalTrack
    Int_t globalTrackId = info.fGlobalTrackMatch;
    Int_t trdTrackId    = info.fTrdTrackMatch;
    Double_t quali      = info.fQuali;
    //    Bool_t   isRec    = kFALSE;
    if (globalTrackId < 0) continue;
    if (trdTrackId < 0) continue;
    CbmTrdTrack* trdTrack = (CbmTrdTrack*) fTrdTracks->At(trdTrackId);
    assert(trdTrack);
    assert(quali >= fQuota);
    CbmTrackMatchNew* match = (CbmTrackMatchNew*) fTrdTrackMatches->At(trdTrackId);
    assert(match);
    Int_t nTrue  = match->GetNofTrueHits();
    Int_t nWrong = match->GetNofWrongHits();
    //Int_t nFake  = match->GetNofFakeHits();
    Int_t nFake    = 0;
    Int_t nAllHits = trdTrack->GetNofHits();
    assert(nTrue + nWrong + nFake == nAllHits);
    // Verbose output
    LOG(debug1) << GetName() << ": MCTrack " << iMcTrack << ", stations " << nStations << ", hits " << nAllHits
                << ", true hits " << nTrue;

    // Fill histograms for reconstructed tracks
    nRecAll++;
    fhPtRecAll->Fill(info.fPt);
    fhNpRecAll->Fill(Double_t(nAllHits));
    //fhPidXY[Pdg2Idx[info.fPdg]]->Fill(info.fY, info.fPt);
    if (info.fIsPrimary) {
      nRecPrim++;
      fhPtRecPrim->Fill(info.fPt);
      fhPidPtY["prmE"][Pdg2Idx(info.fPdg)]->Fill(info.fY, info.fPt);
      fhNpRecPrim->Fill(Double_t(nAllHits));
    }
    else {
      nRecSec++;
      fhPtRecSec->Fill(info.fPt);
      fhPidPtY["secE"][Pdg2Idx(info.fPdg)]->Fill(info.fY, info.fPt);
      fhNpRecSec->Fill(Double_t(nAllHits));
      fhZRecSec->Fill(vertex.Z());
    }
    if (info.fIsFast) nRecFast++;
    if (isFastLong) nRecFastLong++;

  }  // Loop over MCTracks

  // Loop over MC points

  for (uint iLink = 0; iLink < events.size(); iLink++) {
    CbmLink link    = events[iLink];
    Int_t nMcPoints = fTrdPoints->Size(link);
    std::cout << "MC event " << iLink << " n mc points " << nMcPoints << std::endl;
    for (Int_t ip = 0; ip < nMcPoints; ip++) {
      link.SetIndex(ip);
      CbmTrdPoint* pt = (CbmTrdPoint*) fTrdPoints->Get(link);
      assert(pt);
      Int_t station = CbmTrdTrackingInterface::Instance()->GetTrackingStationIndex(pt->GetDetectorID());
      link.SetIndex(pt->GetTrackID());
      McTrackInfo& info = getMcTrackInfo(link);
      if (info.fIsAccepted) {
        fhStationEffXY[station].Fill(pt->GetX(), pt->GetY(), (info.fNtimesReconstructed > 0) ? 100. : 0.);
      }
    }
  }

  // Calculate efficiencies
  Double_t effAll      = Double_t(nRecAll) / Double_t(nAcc);
  Double_t effPrim     = Double_t(nRecPrim) / Double_t(nPrim);
  Double_t effFast     = Double_t(nRecFast) / Double_t(nFast);
  Double_t effFastLong = Double_t(nRecFastLong) / Double_t(nFastLong);
  Double_t effSec      = Double_t(nRecSec) / Double_t(nSec);

  fTimer.Stop();


  // Event summary
  LOG(info) << "+ " << setw(20) << GetName() << ": Event " << setw(6) << right << fNEvents << ", real time " << fixed
            << setprecision(6) << fTimer.RealTime() << " s, MC tracks: all " << nMcTracks << ", acc. " << nAcc
            << ", rec. " << nRecAll << ", eff. " << setprecision(2) << 100. * effAll << " %";
  if (fair::Logger::Logging(fair::Severity::debug)) {
    LOG(debug) << "----------   CbmTrackingTrdQa : Event summary   ------------";
    LOG(debug) << "MCTracks   : " << nAll << ", reconstructible: " << nAcc << ", reconstructed: " << nRecAll;
    LOG(debug) << "Vertex     : reconstructible: " << nPrim << ", reconstructed: " << nRecPrim << ", efficiency "
               << effPrim * 100. << "%";
    LOG(debug) << "Fast  : reconstructible: " << nFast << ", reconstructed: " << nRecFast << ", efficiency "
               << effFast * 100. << "%";
    LOG(debug) << "Fast long : reconstructible: " << nFastLong << ", reconstructed: " << nRecFastLong << ", efficiency "
               << effFastLong * 100. << "%";
    LOG(debug) << "Non-vertex : reconstructible: " << nSec << ", reconstructed: " << nRecSec << ", efficiency "
               << effSec * 100. << "%";
    LOG(debug) << "TrdTracks " << nTracks << ", ghosts " << nGhosts << ", clones " << nClones;
    LOG(debug) << "-----------------------------------------------------------\n";
  }


  // Increase counters
  fNAll += nAll;
  fNAccAll += nAcc;
  fNAccPrim += nPrim;
  fNAccFast += nFast;
  fNAccFastLong += nFastLong;
  fNAccSec += nSec;
  fNRecAll += nRecAll;
  fNRecPrim += nRecPrim;
  fNRecFast += nRecFast;
  fNRecFastLong += nRecFastLong;
  fNRecSec += nRecSec;
  fNGhosts += nGhosts;
  fNClones += nClones;
  fNEvents++;
  fTime += fTimer.RealTime();
}
// -------------------------------------------------------------------------


// -----   Public method SetParContainers   --------------------------------
void CbmTrackingTrdQa::SetParContainers() {}
// -------------------------------------------------------------------------


// -----   Public method Init   --------------------------------------------
InitStatus CbmTrackingTrdQa::Init()
{

  LOG(info) << "\n\n====================================================";
  LOG(info) << GetName() << ": Initialising...";

  fManager = FairRootManager::Instance();
  assert(fManager);

  fMcManager = dynamic_cast<CbmMCDataManager*>(fManager->GetObject("MCDataManager"));

  assert(fMcManager);

  fTimeSlice = static_cast<CbmTimeSlice*>(fManager->GetObject("TimeSlice."));

  if (fTimeSlice == nullptr) { LOG(fatal) << "CbmTrackingTrdQa: No time slice object"; }

  if (fMcManager) {
    fMCTracks  = fMcManager->InitBranch("MCTrack");
    fTrdPoints = fMcManager->InitBranch("TrdPoint");
  }

  assert(fMCTracks);
  assert(fTrdPoints);

  // Get the geometry
  InitStatus geoStatus = GetGeometry();
  if (geoStatus != kSUCCESS) {
    LOG(error) << GetName() << "::Init: Error in reading geometry!";
    return geoStatus;
  }


  // TRD

  // Get TrdHit array
  fTrdHits = (TClonesArray*) fManager->GetObject("TrdHit");
  assert(fTrdHits);

  // Get TrdHitMatch array
  fTrdHitMatch = (TClonesArray*) fManager->GetObject("TrdHitMatch");
  assert(fTrdHitMatch);

  // Get GlobalTrack array
  fGlobalTracks = (TClonesArray*) fManager->GetObject("GlobalTrack");
  assert(fGlobalTracks);

  // Get GlobalTrackMatch array
  //fGlobalTrackMatches = (TClonesArray*) fManager->GetObject("GlobalTrackMatch");
  //assert(fGlobalTrackMatches);

  // Get TrdTrack array
  fTrdTracks = (TClonesArray*) fManager->GetObject("TrdTrack");
  assert(fTrdTracks);

  // Get TrdTrackMatch array
  fTrdTrackMatches = (TClonesArray*) fManager->GetObject("TrdTrackMatch");
  assert(fTrdTrackMatches);


  // Create histograms
  CreateHistos();
  Reset();

  // Output
  LOG(info) << "   Number of Trd stations : " << fTrdNstations;
  LOG(info) << "   Target position ( " << fTargetPos.X() << ", " << fTargetPos.Y() << ", " << fTargetPos.Z() << ") cm";
  LOG(info) << "   Minimum number of Trd stations   : " << fMinStations;
  LOG(info) << "   Matching quota               : " << fQuota;
  LOG(info) << "====================================================";

  return geoStatus;
}
// -------------------------------------------------------------------------


// -----   Public method ReInit   ------------------------------------------
InitStatus CbmTrackingTrdQa::ReInit()
{

  LOG(info) << "\n\n====================================================";
  LOG(info) << GetName() << ": Re-initialising...";

  // Get the geometry of target and Trd
  InitStatus geoStatus = GetGeometry();
  if (geoStatus != kSUCCESS) {
    LOG(error) << GetName() << "::Init: Error in reading geometry!";
    return geoStatus;
  }

  // --- Screen log
  LOG(info) << "   Number of TRD stations : " << fTrdNstations;
  LOG(info) << "   Target position ( " << fTargetPos.X() << ", " << fTargetPos.Y() << ", " << fTargetPos.Z() << ") cm";
  LOG(info) << "   Minimum number of TRD stations   : " << fMinStations;
  LOG(info) << "   Matching quota               : " << fQuota;
  LOG(info) << "====================================================";

  return geoStatus;
}
// -------------------------------------------------------------------------


// -----   Private method Finish   -----------------------------------------
void CbmTrackingTrdQa::Finish()
{

  // Divide histograms for efficiency calculation
  DivideHistos(fhPtRecAll, fhPtAccAll, fhPtEffAll);
  DivideHistos(fhPtRecPrim, fhPtAccPrim, fhPtEffPrim);
  DivideHistos(fhPtRecSec, fhPtAccSec, fhPtEffSec);
  DivideHistos(fhNpRecAll, fhNpAccAll, fhNpEffAll);
  DivideHistos(fhNpRecPrim, fhNpAccPrim, fhNpEffPrim);
  DivideHistos(fhNpRecSec, fhNpAccSec, fhNpEffSec);
  DivideHistos(fhZRecSec, fhZAccSec, fhZEffSec);
  for (int idx(0); idx < fgkNpdg; idx++) {
    fhPidPtY["allY"][idx]->Add(fhPidPtY["prmY"][idx]);
    fhPidPtY["allY"][idx]->Add(fhPidPtY["secY"][idx]);
    fhPidPtY["allE"][idx]->Add(fhPidPtY["prmE"][idx]);
    fhPidPtY["allE"][idx]->Add(fhPidPtY["secE"][idx]);
    DivideHistos(fhPidPtY["prmE"][idx], fhPidPtY["prmY"][idx], fhPidPtY["prmE"][idx], "2D");
    DivideHistos(fhPidPtY["secE"][idx], fhPidPtY["secY"][idx], fhPidPtY["secE"][idx], "2D");
    DivideHistos(fhPidPtY["allE"][idx], fhPidPtY["allY"][idx], fhPidPtY["allE"][idx], "2D");
  }
  // Normalise histos for clones and ghosts to one event
  if (fNEvents) {
    fhNhClones->Scale(1. / Double_t(fNEvents));
    fhNhGhosts->Scale(1. / Double_t(fNEvents));
  }

  // Calculate integrated efficiencies and rates
  Double_t effAll      = Double_t(fNRecAll) / Double_t(fNAccAll);
  Double_t effPrim     = Double_t(fNRecPrim) / Double_t(fNAccPrim);
  Double_t effFast     = Double_t(fNRecFast) / Double_t(fNAccFast);
  Double_t effFastLong = Double_t(fNRecFastLong) / Double_t(fNAccFastLong);
  Double_t effSec      = Double_t(fNRecSec) / Double_t(fNAccSec);
  Double_t rateGhosts  = Double_t(fNGhosts) / Double_t(fNRecAll);
  Double_t rateClones  = Double_t(fNClones) / Double_t(fNRecAll);

  // Run summary to screen
  std::cout << std::endl;
  LOG(info) << "=====================================";
  LOG(info) << fName << ": Run summary ";
  LOG(info) << "Events processed      : " << fNEvents << setprecision(2);
  LOG(info) << "Eff. all tracks       : " << effAll * 100 << " % (" << fNRecAll << "/" << fNAccAll << ")";
  LOG(info) << "Eff. vertex tracks    : " << effPrim * 100 << " % (" << fNRecPrim << "/" << fNAccPrim << ")";
  LOG(info) << "Eff. fast tracks : " << effFast * 100 << " % (" << fNRecFast << "/" << fNAccFast << ")";
  LOG(info) << "Eff. fast long tracks : " << effFastLong * 100 << " % (" << fNRecFastLong << "/" << fNAccFastLong
            << ")";
  LOG(info) << "Eff. secondary tracks : " << effSec * 100 << " % (" << fNRecSec << "/" << fNAccSec << ")";
  LOG(info) << "Ghost rate            : " << rateGhosts * 100 << " % (" << fNGhosts << "/" << fNRecAll << ")";
  LOG(info) << "Clone rate            : " << rateClones * 100 << " % (" << fNClones << "/" << fNRecAll << ")";
  LOG(info) << "mc tracks/event " << fNAll / fNEvents << " accepted " << fNRecAll / fNEvents;
  LOG(info) << "Time per event        : " << setprecision(6) << fTime / Double_t(fNEvents) << " s";


  LOG(info) << "=====================================";

  // Write histos to output
  /*
  gDirectory->mkdir("CbmTrackingTrdQa");
  gDirectory->cd("CbmTrackingTrdQa");
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
InitStatus CbmTrackingTrdQa::GetGeometry()
{
  // Get target geometry
  GetTargetPosition();

  // Get TRD setup
  auto trdInterface = CbmTrdTrackingInterface::Instance();
  assert(trdInterface);
  fTrdNstations = trdInterface->GetNtrackingStations();
  return kSUCCESS;
}
// -------------------------------------------------------------------------


// -----   Get target node   -----------------------------------------------
void CbmTrackingTrdQa::GetTargetPosition()
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
void CbmTrackingTrdQa::CreateHistos()
{

  fOutFolder.Clear();

  // Histogram list
  fHistoList = new TList();

  // Momentum distributions
  Double_t minPt = 0.;
  Double_t maxPt = 2.;
  Int_t nBinsPt  = 40;

  fhStationEffXY.clear();

  for (int i = 0; i < fTrdNstations; i++) {
    //auto trdInterface = CbmTrdTrackingInterface::Instance();
    double dx = 150;  //trdInterface->GetXmax(i);
    double dy = 150;  //trdInterface->GetYmax(i);
    fhStationEffXY.emplace_back(Form("fhStationEffXY%i", i), Form("Efficiency XY: Station %i;X [cm];Y [cm]", i), 300,
                                -dx, dx, 300, -dy, dy);
    fhStationEffXY[i].SetDirectory(0);
    fhStationEffXY[i].SetOptStat(10);
    fhStationEffXY[i].GetYaxis()->SetTitleOffset(1.4);
  }

  for (int i = 0; i < fTrdNstations; i++) {
    fHistoList->Add(&fhStationEffXY[i]);
  }

  fhPtAccAll  = new TH1F("hPtAccAll", "all reconstructable tracks", nBinsPt, minPt, maxPt);
  fhPtRecAll  = new TH1F("hPtRecAll", "all reconstructed tracks", nBinsPt, minPt, maxPt);
  fhPtEffAll  = new TH1F("hPtEffAll", "efficiency all tracks", nBinsPt, minPt, maxPt);
  fhPtAccPrim = new TH1F("hPtAccPrim", "reconstructable vertex tracks", nBinsPt, minPt, maxPt);
  fhPtRecPrim = new TH1F("hPtRecPrim", "reconstructed vertex tracks", nBinsPt, minPt, maxPt);
  fhPtEffPrim = new TH1F("hPtEffPrim", "efficiency vertex tracks", nBinsPt, minPt, maxPt);
  fhPtAccSec  = new TH1F("hPtAccSec", "reconstructable non-vertex tracks", nBinsPt, minPt, maxPt);
  fhPtRecSec  = new TH1F("hPtRecSec", "reconstructed non-vertex tracks", nBinsPt, minPt, maxPt);
  fhPtEffSec  = new TH1F("hPtEffSec", "efficiency non-vertex tracks", nBinsPt, minPt, maxPt);
  fHistoList->Add(fhPtAccAll);
  fHistoList->Add(fhPtRecAll);
  fHistoList->Add(fhPtEffAll);
  fHistoList->Add(fhPtAccPrim);
  fHistoList->Add(fhPtRecPrim);
  fHistoList->Add(fhPtEffPrim);
  fHistoList->Add(fhPtAccSec);
  fHistoList->Add(fhPtRecSec);
  fHistoList->Add(fhPtEffSec);

  const char* pltTitle[] = {"Yield", "Yield", "Yield", "Efficiency", "Efficiency", "Efficiency"};
  const char* pltLab[]   = {"yield", "yield", "yield", "#epsilon (%)", "#epsilon (%)", "#epsilon (%)"};
  const char* vxTyp[]    = {"allY", "prmY", "secY", "allE", "prmE", "secE"};
  for (int ivx(0); ivx < 6; ivx++) {
    for (int ipid(0); ipid < fgkNpdg; ipid++) {
      fhPidPtY[vxTyp[ivx]][ipid] = new TH2F(
        Form("hPtY_%s%s", vxTyp[ivx], Idx2Symb(ipid)),
        Form("%s %s(%s); y - y_{cm}; p_{T} (GeV/c); %s", pltTitle[ivx], Idx2Name(ipid), vxTyp[ivx], pltLab[ivx]), 50,
        -2.5, 2.5, nBinsPt, minPt, maxPt);
      fHistoList->Add(fhPidPtY[vxTyp[ivx]][ipid]);
    }
  }
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
void CbmTrackingTrdQa::Reset()
{

  TIter next(fHistoList);
  while (TH1* histo = ((TH1*) next()))
    histo->Reset();

  fNAccAll = fNAccPrim = fNAccFast = fNAccFastLong = fNAccSec = 0;
  fNRecAll = fNRecPrim = fNRecFast = fNRecFastLong = fNRecSec = 0;
  fNGhosts = fNClones = fNEvents = 0;
}
// -------------------------------------------------------------------------


// -----   Private method FillHitMap   -------------------------------------
void CbmTrackingTrdQa::FillHitMap()
{

  // --- Fill hit map ( mcTrack -> ( station -> number of hits ) )

  // pocess Trd hits

  for (Int_t iHit = 0; iHit < fTrdHits->GetEntriesFast(); iHit++) {
    CbmTrdHit* hit = (CbmTrdHit*) fTrdHits->At(iHit);
    assert(hit);

    if ((int) hit->GetClassType() != 1) {
      // skip TRD-1D hit
      //continue;
    }

    Int_t station = CbmTrdTrackingInterface::Instance()->GetTrackingStationIndex(hit);

    // carefully check the hit match by looking at the strips from both sides

    const CbmMatch* match = dynamic_cast<const CbmMatch*>(fTrdHitMatch->At(iHit));
    assert(match);
    if (match->GetNofLinks() <= 0) continue;

    CbmLink link = match->GetMatchedLink();

    CbmTrdPoint* pt = (CbmTrdPoint*) fTrdPoints->Get(link);
    assert(pt);
    assert(CbmTrdTrackingInterface::Instance()->GetTrackingStationIndex(pt->GetDetectorID()) == station);

    link.SetIndex(pt->GetTrackID());
    McTrackInfo& info = getMcTrackInfo(link);
    info.fHitMap[station]++;
  }
  LOG(debug) << GetName() << ": Filled hit map from " << fTrdHits->GetEntriesFast() << " Trd hits";
}
// -------------------------------------------------------------------------


// ------   Private method FillMatchMap   ----------------------------------
void CbmTrackingTrdQa::FillTrackMatchMap(Int_t& nRec, Int_t& nGhosts, Int_t& nClones)
{
  // Clear matching maps
  for (auto it = fMcTrackInfoMap.begin(); it != fMcTrackInfoMap.end(); ++it) {
    McTrackInfo& info         = it->second;
    info.fGlobalTrackMatch    = -1;
    info.fQuali               = 0.;
    info.fMatchedNHitsAll     = 0;
    info.fMatchedNHitsTrue    = 0;
    info.fNtimesReconstructed = 0;
  }

  // Loop over GlobalTracks. Check matched MCtrack and fill maps.
  nGhosts = 0;
  nClones = 0;
  nRec    = 0;

  //assert(fGlobalTrackMatches);

  for (Int_t iGlobalTrack = 0; iGlobalTrack < fGlobalTracks->GetEntriesFast(); iGlobalTrack++) {

    // --- GlobalTrack
    CbmGlobalTrack* globalTrack = (CbmGlobalTrack*) fGlobalTracks->At(iGlobalTrack);
    assert(globalTrack);

    // --- TrackMatch

    //assert(iGlobalTrack >= 0 && iGlobalTrack < fGlobalTrackMatches->GetEntriesFast());
    //CbmTrackMatchNew* globalMatch = (CbmTrackMatchNew*) fGlobalTrackMatches->At(iGlobalTrack);
    //assert(globalMatch);

    int iTrdTrack = globalTrack->GetTrdTrackIndex();

    if (iTrdTrack < 0) continue;
    CbmTrdTrack* trdTrack = (CbmTrdTrack*) fTrdTracks->At(iTrdTrack);
    assert(trdTrack);
    nRec++;

    // --- TrackMatch

    assert(iTrdTrack >= 0 && iTrdTrack < fTrdTrackMatches->GetEntriesFast());
    CbmTrackMatchNew* trdMatch = (CbmTrackMatchNew*) fTrdTrackMatches->At(iTrdTrack);
    assert(trdMatch);

    Int_t nHits = trdTrack->GetNofHits();
    Int_t nTrue = trdMatch->GetNofTrueHits();

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
    assert(trdMatch->GetNofLinks() > 0);
    CbmLink link = trdMatch->GetMatchedLink();
    assert(link.GetIndex() >= 0);
    link.SetFile(0);  //??

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
    info.fGlobalTrackMatch = iGlobalTrack;
    info.fTrdTrackMatch    = iTrdTrack;
    info.fQuali            = quali;
    info.fMatchedNHitsAll  = nHits;
    info.fMatchedNHitsTrue = nTrue;
    info.fNtimesReconstructed++;
  }  // Loop over GlobalTracks

  LOG(debug) << GetName() << ": Filled match map for " << nRec << " Trd tracks. Ghosts " << nGhosts << " Clones "
             << nClones;
}
// -------------------------------------------------------------------------


// -----   Private method DivideHistos   -----------------------------------
void CbmTrackingTrdQa::DivideHistos(TH1* histo1, TH1* histo2, TH1* histo3, Option_t* opt)
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
  if (strcmp(opt, "2D") == 0) {
    Int_t nBinsY = histo1->GetNbinsY();
    if (histo2->GetNbinsY() != nBinsY || histo3->GetNbinsY() != nBinsY) {
      LOG(error) << GetName() << "::DivideHistos: "
                 << "Different bin numbers in histos";
      LOG(error) << histo1->GetName() << " " << histo1->GetNbinsY();
      LOG(error) << histo2->GetName() << " " << histo2->GetNbinsY();
      LOG(error) << histo3->GetName() << " " << histo3->GetNbinsY();
      return;
    }
    nBins *= nBinsY;
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
    histo3->SetBinContent(iBin, 1.e2 * c3);
    histo3->SetBinError(iBin, 1.e2 * ce);
  }
}
// -------------------------------------------------------------------------


ClassImp(CbmTrackingTrdQa)
