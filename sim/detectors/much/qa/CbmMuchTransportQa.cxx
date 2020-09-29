/// \file   CbmMuchTransportQa.cxx
/// \brief  Implementation of the CbmMuchTransportQa class
/// \author Sergey Gorbunov <se.gorbunov@gsi.de>
/// \author Eugeny Kryshen
/// \author Vikas Singhal
/// \author Ekata Nandy
/// \date   25.09.2020

#include "CbmMuchTransportQa.h"

#include "FairLogger.h"
#include "FairRootFileSink.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"

#include "CbmGeoMuchPar.h"
#include "CbmMuchGeoScheme.h"
#include "CbmMuchStation.h"

#include "CbmMuchPoint.h"

#include "CbmMCTrack.h"
#include "TDatabasePDG.h"
#include "TParticlePDG.h"


#include "TString.h"

#include "TClonesArray.h"

#include "CbmQaCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TStyle.h"

#include <cassert>
#include <vector>

ClassImp(CbmMuchTransportQa);

// -------------------------------------------------------------------------
CbmMuchTransportQa::CbmMuchTransportQa(const char* name, Int_t verbose)
  : FairTask(name, verbose)
  , fOutFolder("MuchTransportQA", "Much Transport QA")
  , fhNevents("nEvents", 0)
  , fvUsNtra()
  , fvMcPointXY()
  , fvMcPointPhiZ()
  , fvMcPointRZ()
  , fvFraction() {}
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
CbmMuchTransportQa::~CbmMuchTransportQa() { DeInit(); }
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
void CbmMuchTransportQa::DeInit() {

  fPoints   = nullptr;
  fMcTracks = nullptr;

  fOutFolder.Clear();
  fhNevents.SetVal(0);

  SafeDelete(fhUsNtraAll);
  SafeDelete(fhUsNtraPrim);
  SafeDelete(fhUsNtraSec);
  SafeDelete(fhUsNtraPr);
  SafeDelete(fhUsNtraPi);
  SafeDelete(fhUsNtraEl);
  SafeDelete(fhUsNtraMu);
  SafeDelete(fhUsNtraKa);

  for (uint i = 0; i < fvMcPointXY.size(); i++) {
    SafeDelete(fvMcPointXY[i]);
  }
  fvMcPointXY.clear();
  for (uint i = 0; i < fvMcPointPhiZ.size(); i++) {
    SafeDelete(fvMcPointPhiZ[i]);
  }
  fvMcPointPhiZ.clear();
  for (uint i = 0; i < fvMcPointRZ.size(); i++) {
    SafeDelete(fvMcPointRZ[i]);
  }
  fvMcPointRZ.clear();

  SafeDelete(fhNtracks);
  SafeDelete(fhFractionPrim);
  SafeDelete(fhFractionSec);
  SafeDelete(fhFractionPr);
  SafeDelete(fhFractionPi);
  SafeDelete(fhFractionEl);
  SafeDelete(fhFractionMu);
  SafeDelete(fhFractionKa);

  fvUsNtra.clear();
  fvFraction.clear();

  SafeDelete(fCanvStationXY);
  SafeDelete(fCanvStationPhiZ);
  SafeDelete(fCanvStationRZ);

  fNstations = 0;
  fOutFolder.Clear();
}
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
InitStatus CbmMuchTransportQa::Init() {

  TDirectory* oldDirectory = gDirectory;

  FairRootManager* manager = FairRootManager::Instance();
  if (!manager) {
    LOG(error) << "No FairRootManager found";
    return kERROR;
  }

  fMcTracks = (TClonesArray*) manager->GetObject("MCTrack");
  if (!fMcTracks) {
    LOG(error) << "No MC tracks found";
    return kERROR;
  }

  fPoints = (TClonesArray*) manager->GetObject("MuchPoint");
  if (!fPoints) {
    LOG(error) << "No MC points found";
    return kERROR;
  }

  if (!CbmMuchGeoScheme::Instance()) {
    LOG(fatal) << "No CbmMuchGeoScheme found";
    return kFATAL;
  }

  fNstations = CbmMuchGeoScheme::Instance()->GetNStations();

  if (fNstations == 0) {
    LOG(error) << "CbmMuchGeoScheme is not initialized";
    return kERROR;
  }

  TFolder* histFolder = fOutFolder.AddFolder("hist", "Histogramms");

  fhNevents.SetVal(0);
  histFolder->Add(&fhNevents);

#define BINS_STA fNstations, 0, fNstations

  {
    fvUsNtra.clear();
    std::vector<TH1F*>& v = fvUsNtra;
    v.push_back(fhUsNtraAll = new TH1F("hUsNtraAll", "N tracks", BINS_STA));
    v.push_back(fhUsNtraPrim =
                  new TH1F("hUsNtraPrim", "N primary tracks", BINS_STA));
    v.push_back(fhUsNtraSec =
                  new TH1F("hUsNtraSec", "N secondary tracks", BINS_STA));
    v.push_back(fhUsNtraPr = new TH1F("hUsNtraPr", "N protons", BINS_STA));
    v.push_back(fhUsNtraPi = new TH1F("hUsNtraPi", "N pions", BINS_STA));
    v.push_back(fhUsNtraEl = new TH1F("hUsNtraEl", "N electrons", BINS_STA));
    v.push_back(fhUsNtraMu = new TH1F("hUsNtraMu", "N muons", BINS_STA));
    v.push_back(fhUsNtraKa = new TH1F("hUsNtraKa", "N kaons", BINS_STA));
    for (uint i = 0; i < fvUsNtra.size(); i++) {
      TH1F* h = fvUsNtra[i];
      h->SetStats(0);
      h->GetXaxis()->SetTitle("Station");
      histFolder->Add(h);
    }
  }

  {
    fvFraction.clear();
    std::vector<TH1F*>& v = fvFraction;
    v.push_back(fhNtracks =
                  new TH1F("hNtracks", "N tracks per event", BINS_STA));
    v.push_back(fhFractionPrim = new TH1F(
                  "hFractionPrim", "Fraction of primary tracks", BINS_STA));
    v.push_back(fhFractionSec = new TH1F(
                  "hFractionSec", "Fraction of secondary tracks", BINS_STA));
    v.push_back(fhFractionPr =
                  new TH1F("hFractionPr", "Fraction of protons", BINS_STA));
    v.push_back(fhFractionPi =
                  new TH1F("hFractionPi", "Fraction of pions", BINS_STA));
    v.push_back(fhFractionEl =
                  new TH1F("hFractionEl", "Fraction of electrons", BINS_STA));
    v.push_back(fhFractionMu =
                  new TH1F("hFractionMu", "Fraction of muons", BINS_STA));
    v.push_back(fhFractionKa =
                  new TH1F("hFractionKa", "Fraction of kaons", BINS_STA));

    for (uint i = 0; i < fvFraction.size(); i++) {
      TH1F* h = fvFraction[i];
      h->SetStats(0);
      h->GetXaxis()->SetTitle("Station");
      h->GetYaxis()->SetTitle("%");
      histFolder->Add(h);
    }
  }

  fvMcPointXY.resize(fNstations);
  fvMcPointPhiZ.resize(fNstations);
  fvMcPointRZ.resize(fNstations);

  for (Int_t i = 0; i < fNstations; i++) {
    CbmMuchStation* station = CbmMuchGeoScheme::Instance()->GetStation(i);
    if (!station) {
      LOG(fatal) << "Much station " << i << " doesn't exist";
      return kFATAL;
    }
    Double_t rMax = station->GetRmax();
    Double_t rMin = station->GetRmin();

    fvMcPointXY[i] = new TH2F(Form("hMcPointXY%i", i + 1),
                              Form("MC point XY : Station %i; X; Y", i + 1),
                              100,
                              -1.2 * rMax,
                              1.2 * rMax,
                              100,
                              -1.2 * rMax,
                              1.2 * rMax);
    histFolder->Add(fvMcPointXY[i]);

    fvMcPointPhiZ[i] =
      new TH2F(Form("hMcPointPhiZ%i", i + 1),
               Form("MC point Phi vs Z : Station %i; Z; Phi", i + 1),
               100,
               station->GetZ() - station->GetTubeDz() - 5.,
               station->GetZ() + station->GetTubeDz() + 5.,
               100,
               -200.,
               200.);
    histFolder->Add(fvMcPointPhiZ[i]);

    float dR       = rMax - rMin;
    fvMcPointRZ[i] = new TH2F(Form("hMcPointRZ%i", i + 1),
                              Form("MC point R vs Z : Station %i; Z; R", i + 1),
                              100,
                              station->GetZ() - station->GetTubeDz() - 5.,
                              station->GetZ() + station->GetTubeDz() + 5.,
                              100,
                              rMin - 0.1 * dR,
                              rMax + 0.1 * dR);
    histFolder->Add(fvMcPointRZ[i]);
  }

  fCanvStationXY =
    new CbmQaCanvas("cMcPointXY", "Much: MC point XY", 2 * 400, 2 * 400);
  fCanvStationXY->Divide2D(fNstations);
  fOutFolder.Add(fCanvStationXY);

  fCanvStationPhiZ = new CbmQaCanvas(
    "cMcPointPhiZ", "Much: MC point Phi vs Z", 2 * 800, 2 * 400);
  fCanvStationPhiZ->Divide2D(fNstations);
  fOutFolder.Add(fCanvStationPhiZ);

  fCanvStationRZ =
    new CbmQaCanvas("cMcPointRZ", "Much: MC point R vs Z", 2 * 800, 2 * 400);
  fCanvStationRZ->Divide2D(fNstations);
  fOutFolder.Add(fCanvStationRZ);

  gDirectory = oldDirectory;

  return kSUCCESS;
}
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
InitStatus CbmMuchTransportQa::ReInit() {
  DeInit();
  return Init();
}
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
void CbmMuchTransportQa::SetParContainers() {
  // Get run and runtime database

  // The code currently does not work,
  // CbmMuchGeoScheme::Instance() must be initialised outside.
  // - Sergey

  // FairRuntimeDb* db = FairRuntimeDb::instance();
  // if ( ! db ) Fatal("SetParContainers", "No runtime database");
  // Get MUCH geometry parameter container
  // CbmGeoMuchPar *fGeoPar = (CbmGeoMuchPar*)
  // db->getContainer("CbmGeoMuchPar");  TObjArray *stations =
  // fGeoPar->GetStations();
  //  TString geoTag;
  // CbmSetup::Instance()->GetGeoTag(ECbmModuleId::kMuch, geoTag);
  // bool mcbmFlag = geoTag.Contains("mcbm", TString::kIgnoreCase);
  // CbmMuchGeoScheme::Instance()->Init(stations, mcbmFlag);
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMuchTransportQa::Exec(Option_t*) {

  LOG(info) << "Event: " << fhNevents.GetVal();

  fhNevents.SetVal(fhNevents.GetVal() + 1);

  // bitmask tells which stations were crossed by mc track
  std::vector<UInt_t> trackStaCross(fMcTracks->GetEntriesFast(), 0);

  for (Int_t i = 0; i < fPoints->GetEntriesFast(); i++) {

    CbmMuchPoint* point = (CbmMuchPoint*) fPoints->At(i);

    if (!point) {
      LOG(fatal) << "Much point " << i << " doesn't exist";
      break;
    }

    Int_t stId    = CbmMuchAddress::GetStationIndex(point->GetDetectorID());
    UInt_t stMask = (1 << stId);

    // Check if the point corresponds to a certain  MC Track
    Int_t trackId = point->GetTrackID();
    if (trackId < 0 || trackId >= fMcTracks->GetEntriesFast()) {
      LOG(fatal) << "Much point " << i << ": trackId " << trackId
                 << " doesn't belong to [0," << fMcTracks->GetEntriesFast() - 1
                 << "]";
      break;
    }

    CbmMCTrack* mcTrack = (CbmMCTrack*) fMcTracks->At(trackId);
    if (!mcTrack) {
      LOG(fatal) << "MC track " << trackId << " doesn't exist";
      break;
    }

    Int_t motherId = mcTrack->GetMotherId();

    // Get mass
    Int_t pdgCode = mcTrack->GetPdgCode();

    //if (pdgCode == 0) continue;

    TParticlePDG* particle = TDatabasePDG::Instance()->GetParticle(pdgCode);
    if (!particle) {
      LOG(warning) << "Particle with pdg code " << pdgCode << " doesn't exist";
      //continue;
    }
    if (pdgCode == 0 || pdgCode == 22 ||  // photons
        pdgCode == 2112)                  // neutrons
    {
      LOG(warning) << "Particle with pdg code " << pdgCode
                   << " left an mc point";
      //continue;
    }

    if (!(trackStaCross[trackId] & stMask)) {
      fhUsNtraAll->Fill(stId);
      if (motherId == -1) {
        fhUsNtraPrim->Fill(stId);
      } else {
        fhUsNtraSec->Fill(stId);
      }
      switch (abs(pdgCode)) {
        case 2212:  // proton
          fhUsNtraPr->Fill(stId);
          break;
        case 211:  // pion
          fhUsNtraPi->Fill(stId);
          break;
        case 11:  // electron
          fhUsNtraEl->Fill(stId);
          break;
        case 13:  // muon
          fhUsNtraMu->Fill(stId);
          break;
        case 321:  // kaon
          fhUsNtraKa->Fill(stId);
          break;
      }
    }

    trackStaCross[trackId] |= stMask;

    TVector3 v1;  // in  position of the track
    TVector3 v2;  // out position of the track

    point->PositionIn(v1);
    point->PositionOut(v2);

    fvMcPointXY[stId]->Fill(v1.X(), v1.Y());
    fvMcPointXY[stId]->Fill(v2.X(), v2.Y());

    fvMcPointPhiZ[stId]->Fill(v1.Z(), v1.Phi() * TMath::RadToDeg());
    fvMcPointPhiZ[stId]->Fill(v2.Z(), v2.Phi() * TMath::RadToDeg());

    fvMcPointRZ[stId]->Fill(v1.Z(), v1.Perp());
    fvMcPointRZ[stId]->Fill(v2.Z(), v2.Perp());
  }
}
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
TFolder& CbmMuchTransportQa::GetQa() {

  TDirectory* oldDirectory = gDirectory;

  fhNtracks->Reset();
  fhNtracks->Add(fhUsNtraAll, 1. / fhNevents.GetVal());

  {
    std::vector<Double_t> errors(fNstations, 0.);
    fhUsNtraAll->SetError(errors.data());
  }

  for (uint i = 1; i < fvFraction.size(); i++) {
    fvFraction[i]->Divide(fvUsNtra[i], fhUsNtraAll);
    fvFraction[i]->Scale(100.);
  }

  for (Int_t i = 0; i < fNstations; i++) {

    fCanvStationXY->cd(i + 1);
    gStyle->SetOptStat(0);
    fvMcPointXY[i]->DrawCopy("colz", "");

    fCanvStationPhiZ->cd(i + 1);
    gStyle->SetOptStat(0);
    fvMcPointPhiZ[i]->DrawCopy("colz", "");

    fCanvStationRZ->cd(i + 1);
    gStyle->SetOptStat(0);
    fvMcPointRZ[i]->DrawCopy("colz", "");

    gStyle->SetOptStat(1110);
  }

  gDirectory = oldDirectory;
  return fOutFolder;
}
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
void CbmMuchTransportQa::Finish() {

  if (!FairRootManager::Instance() || !FairRootManager::Instance()->GetSink()) {
    LOG(error) << "No sink found";
    return;
  }
  FairSink* sink = FairRootManager::Instance()->GetSink();
  sink->WriteObject(&GetQa(), nullptr);
}
// -------------------------------------------------------------------------
