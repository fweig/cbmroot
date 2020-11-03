/// \file   CbmMuchDigitizerQa.cxx
/// \brief  Implementation of the CbmMuchDigitizerQa class
/// \author Sergey Gorbunov <se.gorbunov@gsi.de>
/// \author Eugeny Kryshen
/// \author Vikas Singhal
/// \author Ekata Nandy
/// \author Dominik Smith
/// \date   21.10.2020

#include "CbmMuchDigitizerQa.h"
#include "CbmDefs.h"
#include "CbmDigiManager.h"
#include "CbmLink.h"
#include "CbmMCTrack.h"
#include "CbmMatch.h"
#include "CbmMuchAddress.h"
#include "CbmMuchDigi.h"
#include "CbmMuchGeoScheme.h"
#include "CbmMuchModule.h"
#include "CbmMuchModuleGem.h"
#include "CbmMuchPad.h"
#include "CbmMuchPoint.h"
#include "CbmMuchPointInfo.h"
#include "CbmMuchRecoDefs.h"
#include "CbmMuchStation.h"
#include "CbmQaCanvas.h"
#include "TClonesArray.h"
#include "TDatabasePDG.h"
#include "TF1.h"
#include "TGraph.h"
#include "TH1.h"
#include "TH2.h"
#include "TString.h"
#include "TStyle.h"
#include <FairRootManager.h>
#include <FairSink.h>
#include <FairTask.h>
#include <Logger.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TDirectory.h>
#include <TGenericClassInfo.h>
#include <TMath.h>
#include <TParticlePDG.h>
#include <TROOT.h>
#include <TVector2.h>
#include <TVector3.h>
#include <TVirtualPad.h>
#include <assert.h>
#include <boost/exception/exception.hpp>
#include <boost/type_index/type_index_facade.hpp>
#include <iostream>
#include <limits>
#include <math.h>
#include <stdio.h>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

#define BINNING_CHARGE 100, 0, 300.0
#define BINNING_LENGTH 100, 0, 2.5
#define BINNING_CHARGE_LOG 100, 3, 10
#define BINNING_ENERGY_LOG 100, -0.5, 4.5
#define BINNING_ENERGY_LOG_EL 100, -0.5, 4.5

// -------------------------------------------------------------------------
CbmMuchDigitizerQa::CbmMuchDigitizerQa(const char* name, Int_t verbose)
  : FairTask(name, verbose)
  , fGeoScheme(CbmMuchGeoScheme::Instance())
  , fDigiManager(CbmDigiManager::Instance())
  , fPointInfos(new TClonesArray("CbmMuchPointInfo", 10))
  , fOutFolder("MuchDigiQA", "MuchDigitizerQA")
  , fvUsPadsFiredR()
  , fvUsPadsFiredXY()
  , fvMcPointCharge()
  , fvPadsTotalR()
  , fvPadsFiredR()
  , fvPadOccupancyR() {}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
CbmMuchDigitizerQa::~CbmMuchDigitizerQa() { DeInit(); }
// -------------------------------------------------------------------------

void CbmMuchDigitizerQa::DeInit() {
  for (int i = 0; i < fNstations; i++) {
    delete fvUsPadsFiredR[i];
    delete fvUsPadsFiredXY[i];
    delete fvMcPointCharge[i];
    delete fvPadsTotalR[i];
    delete fvPadsFiredR[i];
    delete fvPadOccupancyR[i];
  }
  fvUsPadsFiredR.clear();
  fvUsPadsFiredXY.clear();
  fvMcPointCharge.clear();
  fvPadsTotalR.clear();
  fvPadsFiredR.clear();
  fvPadOccupancyR.clear();

  fNstations = 0;
  fOutFolder.Clear();
}

// -------------------------------------------------------------------------
InitStatus CbmMuchDigitizerQa::Init() {

  TDirectory* oldDirectory  = gDirectory;
  FairRootManager* fManager = FairRootManager::Instance();
  fMCTracks                 = (TClonesArray*) fManager->GetObject("MCTrack");
  fPoints                   = (TClonesArray*) fManager->GetObject("MuchPoint");
  if (fMCTracks && fPoints) {
    LOG(info) << " CbmMuchDigitizerQa: MC data read.";
  } else {
    LOG(info) << " CbmMuchDigitizerQa: No MC data found.";
  }

  // Reading Much Digis from CbmMuchDigiManager which are stored as vector
  fDigiManager = CbmDigiManager::Instance();
  fDigiManager->Init();

  histFolder = fOutFolder.AddFolder("hist", "Histogramms");
  fNstations = fGeoScheme->GetNStations();
  printf("Init: fNstations = %i\n", fNstations);

  //fVerbose = 3;
  InitCanvases();
  InitChargeHistos();
  InitLengthHistos();
  InitPadHistos();
  InitChannelPadInfo();
  InitFits();
  FillTotalPadsHistos();

  gDirectory = oldDirectory;
  return kSUCCESS;
}

void CbmMuchDigitizerQa::InitChannelPadInfo() {

  fPadMinLx = std::numeric_limits<Double_t>::max();
  fPadMinLy = std::numeric_limits<Double_t>::max();
  fPadMaxLx = std::numeric_limits<Double_t>::min();
  fPadMaxLy = std::numeric_limits<Double_t>::min();

  if (fVerbose > 2) {
    printf("=========================================================\n");
    printf(" Station Nr.\t| Sectors\t| Channels\t| Pad min size\t\t| Pad max"
           "length\t \n");
    printf("---------------------------------------------------------\n");
  }

  Int_t nTotSectors  = 0;
  Int_t nTotChannels = 0;
  for (Int_t iStation = 0; iStation < fNstations; iStation++) {
    Int_t nChannels   = GetNChannels(iStation);
    Int_t nSectors    = GetNSectors(iStation);
    Double_t padMinLx = GetMinPadSize(iStation).X();
    Double_t padMinLy = GetMinPadSize(iStation).Y();
    Double_t padMaxLx = GetMaxPadSize(iStation).X();
    Double_t padMaxLy = GetMaxPadSize(iStation).Y();
    if (fPadMinLx > padMinLx) fPadMinLx = padMinLx;
    if (fPadMinLy > padMinLy) fPadMinLy = padMinLy;
    if (fPadMaxLx < padMaxLx) fPadMaxLx = padMaxLx;
    if (fPadMaxLy < padMaxLy) fPadMaxLy = padMaxLy;
    nTotSectors += nSectors;
    nTotChannels += nChannels;

    if (fVerbose > 2) {
      printf("%i\t\t| %i\t\t| %i\t| %5.4fx%5.4f\t\t| %5.4fx%5.4f\n",
             iStation + 1,
             nSectors,
             nChannels,
             padMinLx,
             padMinLy,
             padMaxLx,
             padMaxLy);
      printf("%i\t\t| %i\t\t\n", iStation + 1, nChannels);
    }
  }
  printf("-----------------------------------------------------------\n");
  printf(" Total:\t\t| %i\t\t\n", nTotChannels);
  printf("===========================================================\n");
}

void CbmMuchDigitizerQa::InitCanvases() {

  /***** charge canvases ****/
  if (fMCTracks && fPoints) {
    fCanvCharge =
      new CbmQaCanvas("cMcPointCharge", "MC point charge", 2 * 800, 2 * 400);
    fCanvCharge->Divide2D(3);

    fCanvStationCharge = new CbmQaCanvas("cMcPointChargeVsStation",
                                         "MC point charge per station",
                                         2 * 800,
                                         2 * 400);
    fCanvStationCharge->Divide2D(fNstations);

    fCanvChargeVsEnergy = new CbmQaCanvas("cMcPointChargeVsEnergy",
                                          "MC point charge vs particle Energy",
                                          2 * 800,
                                          2 * 400);
    fCanvChargeVsEnergy->Divide2D(4);

    fCanvChargeVsLength = new CbmQaCanvas("cMcPointChargeVsLength",
                                          "MC point charge vs track length",
                                          2 * 800,
                                          2 * 400);
    fCanvChargeVsLength->Divide2D(4);

    fOutFolder.Add(fCanvCharge);
    fOutFolder.Add(fCanvStationCharge);
    fOutFolder.Add(fCanvChargeVsEnergy);
    fOutFolder.Add(fCanvChargeVsLength);
  }

  /***** length canvas ****/
  if (fMCTracks && fPoints) {
    fCanvTrackLength =
      new CbmQaCanvas("cTrackLength", "track length", 2 * 800, 2 * 400);
    fCanvTrackLength->Divide2D(4);
    fOutFolder.Add(fCanvTrackLength);
  }

  /***** pad canvases ****/
  fCanvUsPadsFiredXY = new CbmQaCanvas(
    "cPadsFiredXY", "Number of pads fired vs XY", 2 * 800, 2 * 400);
  fCanvUsPadsFiredXY->Divide2D(fNstations);

  fCanvPadOccupancyR = new CbmQaCanvas(
    "cPadOccupancyR", "Pad occupancy [%] vs radius", 2 * 800, 2 * 400);
  fCanvPadOccupancyR->Divide2D(fNstations);

  fCanvPadsTotalR =
    new CbmQaCanvas("cPadsTotalR", "Total pads vs radius", 2 * 800, 2 * 400);
  fCanvPadsTotalR->Divide2D(fNstations);

  fOutFolder.Add(fCanvUsPadsFiredXY);
  fOutFolder.Add(fCanvPadOccupancyR);
  fOutFolder.Add(fCanvPadsTotalR);

  /***** pad canvas (MC) ****/
  if (fMCTracks && fPoints) {
    fCanvNpadsVsArea =
      new CbmQaCanvas("cNpadsVsArea", "N pads Vs Area", 2 * 800, 2 * 400);
    fOutFolder.Add(fCanvNpadsVsArea);
  }
}

void CbmMuchDigitizerQa::InitChargeHistos() {

  if (!fMCTracks || !fPoints) { return; }

  fvMcPointCharge.resize(fNstations);
  for (Int_t i = 0; i < fNstations; i++) {
    fvMcPointCharge[i] = new TH1F(
      Form("hMcPointCharge%i", i + 1),
      Form("MC point charge : Station %i; Charge [10^4 e]; Count", i + 1),
      BINNING_CHARGE);
    histFolder->Add(fvMcPointCharge[i]);
  }

  fhMcPointCharge =
    new TH1F("hCharge", "Charge distribution from tracks", BINNING_CHARGE);
  fhMcPointCharge->GetXaxis()->SetTitle("Charge [10^{4} electrons]");

  fhMcPointChargeLog = new TH1F(
    "hChargeLog", "Charge (log.) distribution from tracks", BINNING_CHARGE_LOG);
  fhMcPointChargeLog->GetXaxis()->SetTitle("Charge [Lg(Number of electrons)]");

  fhMcPointChargePr_1GeV_3mm =
    new TH1F("hChargePr_1GeV_3mm", "Charge for 1 GeV protons", BINNING_CHARGE);
  fhMcPointChargePr_1GeV_3mm->GetXaxis()->SetTitle("Charge [10^{4} electrons]");

  fhMcPointChargeVsTrackEnergyLog =
    new TH2F("hChargeEnergyLog",
             "Charge vs energy (log.) for all tracks",
             BINNING_ENERGY_LOG,
             BINNING_CHARGE);

  fhMcPointChargeVsTrackEnergyLogPi =
    new TH2F("hChargeEnergyLogPi",
             "Charge vs energy (log.) for pions",
             BINNING_ENERGY_LOG,
             BINNING_CHARGE);

  fhMcPointChargeVsTrackEnergyLogPr =
    new TH2F("hChargeEnergyLogPr",
             "Charge vs energy (log.) for protons",
             BINNING_ENERGY_LOG,
             BINNING_CHARGE);

  fhMcPointChargeVsTrackEnergyLogEl =
    new TH2F("hChargeEnergyLogEl",
             "Charge vs energy (log.) for electrons",
             BINNING_ENERGY_LOG_EL,
             BINNING_CHARGE);

  fhMcPointChargeVsTrackLength = new TH2F("hChargeTrackLength",
                                          "Charge vs length for all tracks",
                                          BINNING_LENGTH,
                                          BINNING_CHARGE);

  fhMcPointChargeVsTrackLengthPi = new TH2F("hChargeTrackLengthPi",
                                            "Charge vs length for pions",
                                            BINNING_LENGTH,
                                            BINNING_CHARGE);

  fhMcPointChargeVsTrackLengthPr = new TH2F("hChargeTrackLengthPr",
                                            "Charge vs length for proton",
                                            BINNING_LENGTH,
                                            BINNING_CHARGE);

  fhMcPointChargeVsTrackLengthEl = new TH2F("hChargeTrackLengthEl",
                                            "Charge vs length for electrons",
                                            BINNING_LENGTH,
                                            BINNING_CHARGE);
  std::vector<TH2F*> vChargeHistos;
  vChargeHistos.push_back(fhMcPointChargeVsTrackEnergyLog);
  vChargeHistos.push_back(fhMcPointChargeVsTrackEnergyLogPi);
  vChargeHistos.push_back(fhMcPointChargeVsTrackEnergyLogPr);
  vChargeHistos.push_back(fhMcPointChargeVsTrackEnergyLogEl);
  vChargeHistos.push_back(fhMcPointChargeVsTrackLength);
  vChargeHistos.push_back(fhMcPointChargeVsTrackLengthPi);
  vChargeHistos.push_back(fhMcPointChargeVsTrackLengthPr);
  vChargeHistos.push_back(fhMcPointChargeVsTrackLengthEl);

  for (UInt_t i = 0; i < vChargeHistos.size(); i++) {
    TH2F* histo = vChargeHistos[i];
    histo->SetStats(0);
    histo->GetYaxis()->SetDecimals(kTRUE);
    histo->GetYaxis()->SetTitleOffset(1.4);
    histo->GetYaxis()->CenterTitle();
    histo->GetYaxis()->SetTitle("Charge [10^{4} electrons]");
    if (i < 4) {
      histo->GetXaxis()->SetTitle("Lg(E_{kin} [MeV])");
    } else {
      histo->GetXaxis()->SetTitle("Track length [cm]");
    }
    histFolder->Add(histo);
  }
}

void CbmMuchDigitizerQa::InitLengthHistos() {

  if (!fMCTracks || !fPoints) { return; }

  fhTrackLength = new TH1F("hTrackLength", "Track length", BINNING_LENGTH);

  fhTrackLengthPi =
    new TH1F("hTrackLengthPi", "Track length for pions", BINNING_LENGTH);

  fhTrackLengthPr =
    new TH1F("hTrackLengthPr", "Track length for protons", BINNING_LENGTH);

  fhTrackLengthEl =
    new TH1F("hTrackLengthEl", "Track length for electrons", BINNING_LENGTH);

  std::vector<TH1F*> vLengthHistos;
  vLengthHistos.push_back(fhTrackLength);
  vLengthHistos.push_back(fhTrackLengthPi);
  vLengthHistos.push_back(fhTrackLengthPr);
  vLengthHistos.push_back(fhTrackLengthEl);

  for (UInt_t i = 0; i < vLengthHistos.size(); i++) {
    TH1F* histo = vLengthHistos[i];
    histo->GetXaxis()->SetTitle("Track length [cm]");
    histFolder->Add(histo);
  }
}

void CbmMuchDigitizerQa::InitPadHistos() {
  // non-MC
  fvPadsTotalR.resize(fNstations);
  fvUsPadsFiredR.resize(fNstations);
  fvUsPadsFiredXY.resize(fNstations);
  fvPadsFiredR.resize(fNstations);
  fvPadOccupancyR.resize(fNstations);

  for (Int_t i = 0; i < fNstations; i++) {
    CbmMuchStation* station = fGeoScheme->GetStation(i);
    Double_t rMax           = station->GetRmax();
    Double_t rMin           = station->GetRmin();

    fvPadsTotalR[i] =
      new TH1F(Form("hPadsTotal%i", i + 1),
               Form("Number of  pads vs radius: Station %i;Radius [cm]", i + 1),
               100,
               0.6 * rMin,
               1.2 * rMax);

    fvUsPadsFiredR[i] = new TH1F(
      Form("hUsPadsFired%i", i + 1),
      Form("Number of fired pads vs radius: Station %i;Radius [cm]", i + 1),
      100,
      0.6 * rMin,
      1.2 * rMax);

    fvUsPadsFiredXY[i] =
      new TH2F(Form("hUsPadsFiredXY%i", i + 1),
               Form("Pads fired XY : Station %i; X; Y", i + 1),
               100,
               -1.2 * rMax,
               1.2 * rMax,
               100,
               -1.2 * rMax,
               1.2 * rMax);

    fvPadsFiredR[i] = new TH1F(
      Form("hPadsFired%i", i + 1),
      Form("Number of fired pads vs radius: Station %i;Radius [cm]", i + 1),
      100,
      0.6 * rMin,
      1.2 * rMax);

    fvPadOccupancyR[i] = new TH1F(
      Form("hOccupancy%i", i + 1),
      Form("Pad occupancy vs radius: Station %i;Radius [cm];Occupancy, %%",
           i + 1),
      100,
      0.6 * rMin,
      1.2 * rMax);

    histFolder->Add(fvPadsTotalR[i]);
    histFolder->Add(fvUsPadsFiredXY[i]);
    histFolder->Add(fvPadsFiredR[i]);
    histFolder->Add(fvPadOccupancyR[i]);
  }

  // MC below
  if (fMCTracks && fPoints) {
    fhNpadsVsS = new TH2F("hNpadsVsS",
                          "Number of fired pads vs pad area:area:n pads",
                          10,
                          -5,
                          0,
                          10,
                          0.5,
                          10.5);
    histFolder->Add(fhNpadsVsS);
  }
}

void CbmMuchDigitizerQa::FillTotalPadsHistos() {

  vector<CbmMuchModule*> modules = fGeoScheme->GetModules();
  for (vector<CbmMuchModule*>::iterator im = modules.begin();
       im != modules.end();
       im++) {
    CbmMuchModule* mod = (*im);
    if (mod->GetDetectorType() == 4
        || mod->GetDetectorType() == 3) {  // modified for rpc
      CbmMuchModuleGem* module = (CbmMuchModuleGem*) mod;
      vector<CbmMuchPad*> pads = module->GetPads();
      for (UInt_t ip = 0; ip < pads.size(); ip++) {
        CbmMuchPad* pad = pads[ip];
        Int_t stationId = CbmMuchAddress::GetStationIndex(pad->GetAddress());
        Double_t x0     = pad->GetX();
        Double_t y0     = pad->GetY();
        Double_t r0     = TMath::Sqrt(x0 * x0 + y0 * y0);
        fvPadsTotalR[stationId]->Fill(r0);
      }
    }
  }

  Double_t errors[100];
  for (Int_t i = 0; i < 100; i++) {
    errors[i] = 0;
  }
  for (Int_t i = 0; i < fNstations; i++) {
    //fvPadsTotalR[i]->Sumw2();
    fvPadsTotalR[i]->SetError(errors);
  }
}

void CbmMuchDigitizerQa::InitFits() {

  fFitEl = new TF1("fit_el", LandauMPV, -0.5, 4.5, 1);
  fFitEl->SetParameter(0, 0.511);
  fFitEl->SetLineWidth(2);
  fFitEl->SetLineColor(kBlack);

  fFitPi = new TF1("fit_pi", LandauMPV, -0.5, 4.5, 1);
  fFitPi->SetParameter(0, 139.57);
  fFitPi->SetLineWidth(2);
  fFitPi->SetLineColor(kBlack);

  fFitPr = new TF1("fit_pr", LandauMPV, -0.5, 4.5, 1);
  fFitPr->SetParameter(0, 938.27);
  fFitPr->SetLineWidth(2);
  fFitPr->SetLineColor(kBlack);
}

// -------------------------------------------------------------------------
void CbmMuchDigitizerQa::SetParContainers() {
  // Get run and runtime database

  // The code currently does not work,
  // CbmMuchGeoScheme::Instance() must be initialised outside.
  // - Sergey

  // FairRuntimeDb* db = FairRuntimeDb::instance();
  // if ( ! db ) Fatal("SetParContainers", "No runtime database");
  // Get MUCH geometry parameter container
  // CbmGeoMuchPar *fGeoPar = (CbmGeoMuchPar*)
  // db->getContainer("CbmGeoMuchPar");  TObjArray *stations =
  // fGeoPar->GetStations();  cout<<"\n\n SG: stations:
  // "<<stations->GetEntriesFast()<<endl;
  //  TString geoTag;
  // CbmSetup::Instance()->GetGeoTag(ECbmModuleId::kMuch, geoTag);
  // bool mcbmFlag = geoTag.Contains("mcbm", TString::kIgnoreCase);
  // CbmMuchGeoScheme::Instance()->Init(stations, mcbmFlag);
}

// -------------------------------------------------------------------------x
void CbmMuchDigitizerQa::Exec(Option_t*) {

  fNevents++;
  LOG(info) << "Event: " << fNevents;
  TDirectory* oldDirectory = gDirectory;

  OccupancyQa();
  DigitizerQa();
  FillChargePerPoint();
  FillDigitizerPerformancePlots();

  if (fVerbose > 1) {
    PrintFrontLayerPoints();
    PrintFrontLayerDigis();
  }
  gDirectory = oldDirectory;
  return;
}

// -------------------------------------------------------------------------
void CbmMuchDigitizerQa::OccupancyQa() {
  // Filling occupancy plots
  for (Int_t i = 0; i < fDigiManager->GetNofDigis(ECbmModuleId::kMuch); i++) {
    CbmMuchDigi* digi = (CbmMuchDigi*) fDigiManager->Get<CbmMuchDigi>(i);
    assert(digi);
    UInt_t address        = digi->GetAddress();
    CbmMuchModule* module = fGeoScheme->GetModuleByDetId(address);
    if (!module) continue;
    Double_t r0 = 0;
    if (module->GetDetectorType() != 4 && module->GetDetectorType() != 3)
      continue;
    CbmMuchModuleGem* module1 = (CbmMuchModuleGem*) module;
    CbmMuchPad* pad           = module1->GetPad(
      address);  // fGeoScheme->GetPadByDetId(detectorId, channelId);
    assert(pad);
    Double_t x0 = pad->GetX();
    Double_t y0 = pad->GetY();
    r0          = TMath::Sqrt(x0 * x0 + y0 * y0);
    fvUsPadsFiredR[CbmMuchAddress::GetStationIndex(address)]->Fill(r0);
    fvUsPadsFiredXY[CbmMuchAddress::GetStationIndex(address)]->Fill(x0, y0);
  }
}

// -------------------------------------------------------------------------
void CbmMuchDigitizerQa::DigitizerQa() {

  if (!fMCTracks || !fPoints) {
    LOG(debug)
      << " CbmMuchDigitizerQa::DigitizerQa(): Found no MC data. Skipping.";
    return;
  }
  TVector3 vIn;   // in  position of the track
  TVector3 vOut;  // out position of the track
  TVector3 p;     // track momentum
  fPointInfos->Clear();

  for (Int_t i = 0; i < fPoints->GetEntriesFast(); i++) {
    CbmMuchPoint* point = (CbmMuchPoint*) fPoints->At(i);
    Int_t stId = CbmMuchAddress::GetStationIndex(point->GetDetectorID());

    // Check if the point corresponds to a certain  MC Track
    Int_t trackId = point->GetTrackID();
    if (trackId < 0) {
      new ((*fPointInfos)[i]) CbmMuchPointInfo(0, 0, 0, 0, 0);
      continue;
    }
    CbmMCTrack* mcTrack = (CbmMCTrack*) fMCTracks->At(trackId);
    if (!mcTrack) {
      new ((*fPointInfos)[i]) CbmMuchPointInfo(0, 0, 0, 0, 0);
      continue;
    }

    Int_t motherId         = mcTrack->GetMotherId();
    Int_t pdgCode          = mcTrack->GetPdgCode();
    TParticlePDG* particle = TDatabasePDG::Instance()->GetParticle(pdgCode);
    if (!particle || pdgCode == 22 ||  // photons
        pdgCode == 2112)               // neutrons
    {
      new ((*fPointInfos)[i]) CbmMuchPointInfo(0, 0, 0, 0, 0);
      continue;
    }

    Double_t mass = particle->Mass();
    point->PositionIn(vIn);
    point->PositionOut(vOut);
    point->Momentum(p);
    Double_t length = (vOut - vIn).Mag();                   // Track length
    Double_t kine   = sqrt(p.Mag2() + mass * mass) - mass;  // Kinetic energy
    // Get mother pdg code
    Int_t motherPdg         = 0;
    CbmMCTrack* motherTrack = NULL;
    if (motherId != -1) motherTrack = (CbmMCTrack*) fMCTracks->At(motherId);
    if (motherTrack) motherPdg = motherTrack->GetPdgCode();
    new ((*fPointInfos)[i])
      CbmMuchPointInfo(pdgCode, motherPdg, kine, length, stId);
  }
}

// -------------------------------------------------------------------------
void CbmMuchDigitizerQa::FillChargePerPoint() {

  if (!fMCTracks || !fPoints) {
    LOG(debug) << " CbmMuchDigitizerQa::FillChargePerPoint()): Found no MC "
                  "data. Skipping.";
    return;
  }
  for (Int_t i = 0; i < fDigiManager->GetNofDigis(ECbmModuleId::kMuch); i++) {
    CbmMuchDigi* digi = (CbmMuchDigi*) fDigiManager->Get<CbmMuchDigi>(i);
    assert(digi);
    CbmMatch* match =
      (CbmMatch*) fDigiManager->GetMatch(ECbmModuleId::kMuch, i);
    assert(match);
    CbmMuchModule* module = fGeoScheme->GetModuleByDetId(digi->GetAddress());
    assert(module);
    if (!module) continue;
    Double_t area = 0;
    if (module->GetDetectorType() != 4 && module->GetDetectorType() != 3)
      continue;  /// rpc
    LOG(debug) << GetName() << " Processing MuchDigi " << i << " at address "
               << digi->GetAddress() << " Module number "
               << module->GetDetectorType();

    CbmMuchModuleGem* module1 = (CbmMuchModuleGem*) module;
    assert(module1);
    CbmMuchPad* pad = module1->GetPad(digi->GetAddress());
    assert(pad);
    area           = pad->GetDx() * pad->GetDy();
    Int_t nofLinks = match->GetNofLinks();
    for (Int_t pt = 0; pt < nofLinks; pt++) {
      Int_t pointId          = match->GetLink(pt).GetIndex();
      Int_t charge           = match->GetLink(pt).GetWeight();
      CbmMuchPointInfo* info = (CbmMuchPointInfo*) fPointInfos->At(pointId);
      if (info->GetPdgCode() == 0) continue;
      info->AddCharge(charge);
      info->AddArea(area);
    }
  }
}

// -------------------------------------------------------------------------
void CbmMuchDigitizerQa::FillDigitizerPerformancePlots() {

  if (!fMCTracks || !fPoints) {
    LOG(debug) << " CbmMuchDigitizerQa::FillDigitizerPerformancePlots(): Found "
                  "no MC data. Skipping.";
    return;
  }
  Bool_t verbose = (fVerbose > 2);
  for (Int_t i = 0; i < fPointInfos->GetEntriesFast(); i++) {
    CbmMuchPointInfo* info = (CbmMuchPointInfo*) fPointInfos->At(i);
    if (verbose) {
      printf("%i", i);
      info->Print();
    }
    Double_t length = info->GetLength();
    Double_t kine   = 1000 * (info->GetKine());
    Double_t charge = info->GetCharge();
    Int_t pdg       = info->GetPdgCode();
    if (pdg == 0) continue;
    if (charge <= 0) continue;
    Double_t log_kine   = TMath::Log10(kine);
    Double_t log_charge = TMath::Log10(charge);
    charge              = charge / 1.e+4;  // measure charge in 10^4 electrons

    Int_t nPads   = info->GetNPads();
    Double_t area = info->GetArea() / nPads;
    // printf("%f %i\n",TMath::Log2(area),nPads);
    fhNpadsVsS->Fill(TMath::Log2(area), nPads);
    fhMcPointCharge->Fill(charge);
    fvMcPointCharge[info->GetStationId()]->Fill(charge);
    fhMcPointChargeLog->Fill(log_charge);
    fhMcPointChargeVsTrackEnergyLog->Fill(log_kine, charge);
    fhMcPointChargeVsTrackLength->Fill(length, charge);
    fhTrackLength->Fill(length);

    if (pdg == 2212) {
      fhMcPointChargeVsTrackEnergyLogPr->Fill(log_kine, charge);
      fhMcPointChargeVsTrackLengthPr->Fill(length, charge);
      fhTrackLengthPr->Fill(length);
    } else if (pdg == 211 || pdg == -211) {
      fhMcPointChargeVsTrackEnergyLogPi->Fill(log_kine, charge);
      fhMcPointChargeVsTrackLengthPi->Fill(length, charge);
      fhTrackLengthPi->Fill(length);
    } else if (pdg == 11 || pdg == -11) {
      fhMcPointChargeVsTrackEnergyLogEl->Fill(log_kine, charge);
      fhMcPointChargeVsTrackLengthEl->Fill(length, charge);
      fhTrackLengthEl->Fill(length);
    }
    if (pdg == 2212 && length > 0.3 && length < 0.32 && kine > 1000
        && kine < 1020)
      fhMcPointChargePr_1GeV_3mm->Fill(charge);
  }
}


// -------------------------------------------------------------------------
void CbmMuchDigitizerQa::DrawChargeCanvases() {

  if (!fMCTracks || !fPoints) { return; }

  for (Int_t i = 0; i < fNstations; i++) {
    fCanvStationCharge->cd(i + 1);
    fvMcPointCharge[i]->DrawCopy("", "");
  }
  fCanvCharge->cd(1);
  fhMcPointCharge->DrawCopy("", "");
  fCanvCharge->cd(2);
  fhMcPointChargeLog->DrawCopy("", "");
  fCanvCharge->cd(3);
  fhMcPointChargePr_1GeV_3mm->DrawCopy("", "");

  for (Int_t i = 0; i < 4; i++) {
    fCanvChargeVsEnergy->cd(i + 1);
    gPad->Range(0, 0, 200, 200);
    gPad->SetBottomMargin(0.11);
    gPad->SetRightMargin(0.14);
    gPad->SetLeftMargin(0.12);
    gPad->SetLogz();
  }
  fCanvChargeVsEnergy->cd(1);
  fhMcPointChargeVsTrackEnergyLog->DrawCopy("colz", "");
  fCanvChargeVsEnergy->cd(2);
  fhMcPointChargeVsTrackEnergyLogPi->DrawCopy("colz", "");
  fFitPi->DrawCopy("same");
  fCanvChargeVsEnergy->cd(3);
  fhMcPointChargeVsTrackEnergyLogPr->DrawCopy("colz", "");
  fFitPr->DrawCopy("same");
  fCanvChargeVsEnergy->cd(4);
  fhMcPointChargeVsTrackEnergyLogEl->DrawCopy("colz", "");
  fFitEl->DrawCopy("same");

  for (Int_t i = 0; i < 4; i++) {
    fCanvChargeVsLength->cd(i + 1);
    gPad->Range(0, 0, 200, 200);
    gPad->SetBottomMargin(0.11);
    gPad->SetRightMargin(0.14);
    gPad->SetLeftMargin(0.12);
    gPad->SetLogz();
  }
  fCanvChargeVsLength->cd(1);
  fhMcPointChargeVsTrackLength->DrawCopy("colz", "");
  fCanvChargeVsLength->cd(2);
  fhMcPointChargeVsTrackLengthPi->DrawCopy("colz", "");
  fCanvChargeVsLength->cd(3);
  fhMcPointChargeVsTrackLengthPr->DrawCopy("colz", "");
  fCanvChargeVsLength->cd(4);
  fhMcPointChargeVsTrackLengthEl->DrawCopy("colz", "");
}

void CbmMuchDigitizerQa::DrawPadCanvases() {
  //non-MC
  for (Int_t i = 0; i < fNstations; i++) {
    *fvPadsFiredR[i] = *fvUsPadsFiredR[i];
    //fvPadsFiredR[i]->Sumw2();
    fvPadsFiredR[i]->Scale(1. / fNevents);
    fvPadOccupancyR[i]->Divide(fvPadsFiredR[i], fvPadsTotalR[i]);
    fvPadOccupancyR[i]->Scale(100.);

    fCanvPadOccupancyR->cd(i + 1);
    fvPadOccupancyR[i]->DrawCopy("", "");
    fCanvPadsTotalR->cd(i + 1);
    fvPadsTotalR[i]->DrawCopy("", "");
    fCanvUsPadsFiredXY->cd(i + 1);
    fvUsPadsFiredXY[i]->DrawCopy("colz", "");
  }
  //MC below
  if (fMCTracks && fPoints) {
    fCanvNpadsVsArea->cd();
    fhNpadsVsS->DrawCopy("colz", "");
  }
}

void CbmMuchDigitizerQa::DrawLengthCanvases() {

  if (!fMCTracks || !fPoints) { return; }

  for (Int_t i = 0; i < 4; i++) {
    fCanvTrackLength->cd(i + 1);
    gPad->SetLogy();
    gStyle->SetOptStat(1110);
  }
  fCanvTrackLength->cd(1);
  fhTrackLength->DrawCopy("", "");
  fCanvTrackLength->cd(2);
  fhTrackLengthPi->DrawCopy("", "");
  fCanvTrackLength->cd(3);
  fhTrackLengthPr->DrawCopy("", "");
  fCanvTrackLength->cd(4);
  fhTrackLengthEl->DrawCopy("", "");
}

void CbmMuchDigitizerQa::PrintFrontLayerPoints() {

  if (!fMCTracks || !fPoints) { return; }

  for (int i = 0; i < fPoints->GetEntriesFast(); i++) {
    CbmMuchPoint* point = (CbmMuchPoint*) fPoints->At(i);
    Int_t stId = CbmMuchAddress::GetStationIndex(point->GetDetectorID());
    if (stId != 0) continue;
    Int_t layerId = CbmMuchAddress::GetLayerIndex(point->GetDetectorID());
    if (layerId != 0) continue;
    printf("point %4i xin=%6.2f yin=%6.2f xout=%6.2f yout=%6.2f zin=%6.2f\n",
           i,
           point->GetXIn(),
           point->GetYIn(),
           point->GetXOut(),
           point->GetYOut(),
           point->GetZIn());
  }
}

// -------------------------------------------------------------------------
void CbmMuchDigitizerQa::PrintFrontLayerDigis() {
  for (Int_t i = 0; i < fDigiManager->GetNofDigis(ECbmModuleId::kMuch); i++) {
    CbmMuchDigi* digi = (CbmMuchDigi*) fDigiManager->Get<CbmMuchDigi>(i);
    UInt_t address    = digi->GetAddress();
    Int_t stId        = CbmMuchAddress::GetStationIndex(address);
    if (stId != 0) continue;
    Int_t layerId = CbmMuchAddress::GetLayerIndex(address);
    if (layerId != 0) continue;
    CbmMuchModuleGem* module =
      (CbmMuchModuleGem*) fGeoScheme->GetModuleByDetId(address);
    if (!module) continue;
    CbmMuchPad* pad = module->GetPad(address);
    Double_t x0     = pad->GetX();
    Double_t y0     = pad->GetY();
    UInt_t charge   = digi->GetAdc();
    printf("digi %4i x0=%5.1f y0=%5.1f charge=%3i\n", i, x0, y0, charge);
  }
}

// -------------------------------------------------------------------------
void CbmMuchDigitizerQa::FinishTask() {

  printf("FinishTask\n");
  cout << "\n\n SG: Finish task!" << endl;

  DrawChargeCanvases();
  DrawPadCanvases();
  DrawLengthCanvases();

  TDirectory* oldDirectory = gDirectory;
  bool oldBatchMode        = gROOT->IsBatch();

  if (!FairRootManager::Instance() || !FairRootManager::Instance()->GetSink()) {
    cout << "No sink found" << endl;
    return;
  }
  // gROOT->SetBatch(kTRUE);
  gROOT->SetBatch(kFALSE);
  gStyle->SetPaperSize(20, 20);

  if (fVerbose > 1) { OutputNvsS(); }
  FairSink* sink = FairRootManager::Instance()->GetSink();
  sink->WriteObject(&fOutFolder, nullptr);
  gDirectory = oldDirectory;
  gROOT->SetBatch(oldBatchMode);
}
// -------------------------------------------------------------------------

void CbmMuchDigitizerQa::OutputNvsS() {
  TCanvas* c = new TCanvas("nMeanVsS", "nMeanVsS", 2 * 800, 2 * 400);
  printf("===================================\n");
  printf("DigitizerQa:\n");

  Double_t nMean[11];
  Double_t s[11];
  for (Int_t iS = 1; iS <= 10; iS++) {
    nMean[iS]      = 0;
    s[iS]          = -5.25 + 0.5 * iS;
    Double_t total = 0;
    for (Int_t iN = 1; iN <= 10; iN++) {
      nMean[iS] += iN * fhNpadsVsS->GetBinContent(iS, iN);
      total += fhNpadsVsS->GetBinContent(iS, iN);
    }
    if (total > 0) nMean[iS] /= total;
    printf("%f %f\n", s[iS], nMean[iS]);
  }
  c->cd();

  TGraph* gNvsS = new TGraph(11, s, nMean);
  //gNvsS->SetLineColor(2);
  //gNvsS->SetLineWidth(4);
  gNvsS->SetMarkerColor(4);
  gNvsS->SetMarkerSize(1.5);
  gNvsS->SetMarkerStyle(21);
  gNvsS->SetTitle("nMeanVsS");
  gNvsS->GetYaxis()->SetTitle("nMean");
  gNvsS->GetYaxis()->SetTitle("nMean");
  //gNvsS->DrawClone("ALP");
  gNvsS->DrawClone("AP");
  fOutFolder.Add(c);
}

// -------------------------------------------------------------------------
Int_t CbmMuchDigitizerQa::GetNChannels(Int_t iStation) {
  Int_t nChannels                = 0;
  vector<CbmMuchModule*> modules = fGeoScheme->GetModules(iStation);
  for (UInt_t im = 0; im < modules.size(); im++) {
    CbmMuchModule* mod = modules[im];
    if (mod->GetDetectorType() != 4 && mod->GetDetectorType() != 3) continue;
    CbmMuchModuleGem* module = (CbmMuchModuleGem*) mod;
    if (!module) continue;
    nChannels += module->GetNPads();
  }
  return nChannels;
}

// -------------------------------------------------------------------------
Int_t CbmMuchDigitizerQa::GetNSectors(Int_t iStation) {
  Int_t nSectors                 = 0;
  vector<CbmMuchModule*> modules = fGeoScheme->GetModules(iStation);
  for (UInt_t im = 0; im < modules.size(); im++) {
    CbmMuchModule* mod = modules[im];
    if (mod->GetDetectorType() != 4 && mod->GetDetectorType() != 3) continue;
    CbmMuchModuleGem* module = (CbmMuchModuleGem*) mod;
    if (!module) continue;
    nSectors += module->GetNSectors();
  }
  return nSectors;
}

// -------------------------------------------------------------------------
TVector2 CbmMuchDigitizerQa::GetMinPadSize(Int_t iStation) {
  Double_t padMinLx              = std::numeric_limits<Double_t>::max();
  Double_t padMinLy              = std::numeric_limits<Double_t>::max();
  vector<CbmMuchModule*> modules = fGeoScheme->GetModules(iStation);
  for (UInt_t im = 0; im < modules.size(); im++) {
    CbmMuchModule* mod = modules[im];
    if (mod->GetDetectorType() != 1) continue;
    CbmMuchModuleGem* module = (CbmMuchModuleGem*) mod;
    vector<CbmMuchPad*> pads = module->GetPads();
    for (UInt_t ip = 0; ip < pads.size(); ip++) {
      CbmMuchPad* pad = pads[ip];
      if (pad->GetDx() < padMinLx) padMinLx = pad->GetDx();
      if (pad->GetDy() < padMinLy) padMinLy = pad->GetDy();
    }
  }
  return TVector2(padMinLx, padMinLy);
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
TVector2 CbmMuchDigitizerQa::GetMaxPadSize(Int_t iStation) {
  Double_t padMaxLx              = std::numeric_limits<Double_t>::min();
  Double_t padMaxLy              = std::numeric_limits<Double_t>::min();
  vector<CbmMuchModule*> modules = fGeoScheme->GetModules(iStation);
  for (UInt_t im = 0; im < modules.size(); im++) {
    CbmMuchModule* mod = modules[im];
    if (mod->GetDetectorType() != 1) continue;
    CbmMuchModuleGem* module = (CbmMuchModuleGem*) mod;
    vector<CbmMuchPad*> pads = module->GetPads();
    for (UInt_t ip = 0; ip < pads.size(); ip++) {
      CbmMuchPad* pad = pads[ip];
      if (pad->GetDx() > padMaxLx) padMaxLx = pad->GetDx();
      if (pad->GetDy() > padMaxLy) padMaxLy = pad->GetDy();
    }
  }
  return TVector2(padMaxLx, padMaxLy);
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
Double_t CbmMuchDigitizerQa::LandauMPV(Double_t* lg_x, Double_t* par) {
  Double_t gaz_gain_mean = 1.7e+4;
  Double_t scale         = 1.e+6;
  gaz_gain_mean /= scale;
  Double_t mass = par[0];  // mass in MeV
  Double_t x    = TMath::Power(10, lg_x[0]);
  return gaz_gain_mean * MPV_n_e(x, mass);
}

// -------------------------------------------------------------------------
Double_t CbmMuchDigitizerQa::MPV_n_e(Double_t Tkin, Double_t mass) {
  Double_t logT;
  TF1 fPol6("fPol6", "pol6", -5, 10);
  if (mass < 0.1) {
    logT = log(Tkin * 0.511 / mass);
    if (logT > 9.21034) logT = 9.21034;
    if (logT < min_logT_e) logT = min_logT_e;
    return fPol6.EvalPar(&logT, mpv_e);
  } else if (mass >= 0.1 && mass < 0.2) {
    logT = log(Tkin * 105.658 / mass);
    if (logT > 9.21034) logT = 9.21034;
    if (logT < min_logT_mu) logT = min_logT_mu;
    return fPol6.EvalPar(&logT, mpv_mu);
  } else {
    logT = log(Tkin * 938.272 / mass);
    if (logT > 9.21034) logT = 9.21034;
    if (logT < min_logT_p) logT = min_logT_p;
    return fPol6.EvalPar(&logT, mpv_p);
  }
}
// -------------------------------------------------------------------------

ClassImp(CbmMuchDigitizerQa)
