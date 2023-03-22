/* Copyright (C) 2020-2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith [committer], Sergey Gorbunov */

/// \file   CbmMuchDigitizerQa.cxx
/// \brief  Implementation of the CbmMuchDigitizerQa class
/// \author Sergey Gorbunov <se.gorbunov@gsi.de>
/// \author Eugeny Kryshen
/// \author Vikas Singhal
/// \author Ekata Nandy
/// \author Dominik Smith
/// \date   21.10.2020

#include "CbmMuchDigitizerQa.h"

#include "CbmDigiManager.h"
#include "CbmLink.h"
#include "CbmMCDataArray.h"
#include "CbmMCDataManager.h"
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
#include "CbmTimeSlice.h"

#include <FairRootManager.h>
#include <FairSink.h>
#include <FairTask.h>
#include <Logger.h>

#include "TClonesArray.h"
#include "TDatabasePDG.h"
#include "TF1.h"
#include "TGraph.h"
#include "TH1.h"
#include "TH2.h"
#include "TParameter.h"
#include "TString.h"
#include "TStyle.h"
#include <TAxis.h>
#include <TCanvas.h>
#include <TDirectory.h>
#include <TMath.h>
#include <TParticlePDG.h>
#include <TVector2.h>

#include <iostream>
#include <limits>
#include <vector>

#include <math.h>

using std::endl;
using std::vector;

#define BINNING_CHARGE 100, 0, 300.0
#define BINNING_LENGTH 100, 0, 2.5
#define BINNING_CHARGE_LOG 100, 3, 10
#define BINNING_ENERGY_LOG 100, -0.5, 4.5
#define BINNING_ENERGY_LOG_EL 100, -0.5, 4.5

ClassImp(CbmMuchDigitizerQa);

// -------------------------------------------------------------------------
CbmMuchDigitizerQa::CbmMuchDigitizerQa(const char* name, Int_t verbose)
  : FairTask(name, verbose)
  , fOutFolder("MuchDigiQA", "MuchDigitizerQA")
  , fNevents("nEvents", 0)
  , fvUsPadsFiredR()
  , fvUsPadsFiredXY()
  , fvTrackCharge()
  , fvPadsTotalR()
  , fvPadsFiredR()
  , fvPadOccupancyR()
{
}

// -------------------------------------------------------------------------
CbmMuchDigitizerQa::~CbmMuchDigitizerQa() { DeInit(); }

// -------------------------------------------------------------------------
void CbmMuchDigitizerQa::DeInit()
{

  histFolder   = nullptr;
  fGeoScheme   = nullptr;
  fDigiManager = nullptr;
  fPoints      = nullptr;
  fDigis       = nullptr;
  fDigiMatches = nullptr;
  fMCTracks    = nullptr;
  fOutFolder.Clear();
  fMcPointInfoMap.clear();

  for (uint i = 0; i < fvUsPadsFiredR.size(); i++) {
    SafeDelete(fvUsPadsFiredR[i]);
  }
  for (uint i = 0; i < fvUsPadsFiredXY.size(); i++) {
    SafeDelete(fvUsPadsFiredXY[i]);
  }
  for (uint i = 0; i < fvTrackCharge.size(); i++) {
    SafeDelete(fvTrackCharge[i]);
  }
  for (uint i = 0; i < fvPadsTotalR.size(); i++) {
    SafeDelete(fvPadsTotalR[i]);
  }
  for (uint i = 0; i < fvPadsFiredR.size(); i++) {
    SafeDelete(fvPadsFiredR[i]);
  }
  for (uint i = 0; i < fvPadOccupancyR.size(); i++) {
    SafeDelete(fvPadOccupancyR[i]);
  }
  fvUsPadsFiredR.clear();
  fvUsPadsFiredXY.clear();
  fvTrackCharge.clear();
  fvPadsTotalR.clear();
  fvPadsFiredR.clear();
  fvPadOccupancyR.clear();

  SafeDelete(fhTrackLength);
  SafeDelete(fhTrackLengthPi);
  SafeDelete(fhTrackLengthPr);
  SafeDelete(fhTrackLengthEl);
  SafeDelete(fhTrackChargeVsTrackEnergyLog);
  SafeDelete(fhTrackChargeVsTrackEnergyLogPi);
  SafeDelete(fhTrackChargeVsTrackEnergyLogPr);
  SafeDelete(fhTrackChargeVsTrackEnergyLogEl);
  SafeDelete(fhTrackChargeVsTrackLength);
  SafeDelete(fhTrackChargeVsTrackLengthPi);
  SafeDelete(fhTrackChargeVsTrackLengthPr);
  SafeDelete(fhTrackChargeVsTrackLengthEl);
  SafeDelete(fhNpadsVsS);
  SafeDelete(fCanvCharge);
  SafeDelete(fCanvStationCharge);
  SafeDelete(fCanvChargeVsEnergy);
  SafeDelete(fCanvChargeVsLength);
  SafeDelete(fCanvTrackLength);
  SafeDelete(fCanvNpadsVsArea);
  SafeDelete(fCanvUsPadsFiredXY);
  SafeDelete(fCanvPadOccupancyR);
  SafeDelete(fCanvPadsTotalR);
  SafeDelete(fFitEl);
  SafeDelete(fFitPi);
  SafeDelete(fFitPr);

  fNevents.SetVal(0);
  fNstations  = 0;
  fnPadSizesX = 0;
  fnPadSizesY = 0;
  fPadMinLx   = 0.;
  fPadMinLy   = 0.;
  fPadMaxLx   = 0.;
  fPadMaxLy   = 0.;
}

// -------------------------------------------------------------------------
InitStatus CbmMuchDigitizerQa::Init()
{
  DeInit();
  fMcPointInfoMap.clear();
  TDirectory* oldDirectory = gDirectory;

  fManager = FairRootManager::Instance();
  if (!fManager) {
    LOG(fatal) << "Can not find FairRootManager";
    return kFATAL;
  }

  fGeoScheme = CbmMuchGeoScheme::Instance();
  if (!fGeoScheme) {
    LOG(fatal) << "Can not find Much geo scheme";
    return kFATAL;
  }

  fNstations = fGeoScheme->GetNStations();
  LOG(info) << "CbmMuchDigitizerQa: N Stations = " << fNstations;

  fDigiManager = CbmDigiManager::Instance();
  if (!fDigiManager) {
    LOG(fatal) << "Can not find Much digi manager";
    return kFATAL;
  }
  fDigiManager->Init();

  fMCTracks = nullptr;
  fPoints   = nullptr;

  fMcManager = dynamic_cast<CbmMCDataManager*>(fManager->GetObject("MCDataManager"));

  fTimeSlice = (CbmTimeSlice*) fManager->GetObject("TimeSlice.");
  if (!fTimeSlice) { LOG(error) << GetName() << ": No time slice found"; }

  if (fMcManager) {
    // Get MCTrack array
    fMCTracks = fMcManager->InitBranch("MCTrack");

    // Get StsPoint array
    fPoints = fMcManager->InitBranch("MuchPoint");
  }

  if (fMCTracks && fPoints && fDigiManager->IsMatchPresent(ECbmModuleId::kMuch)) {
    LOG(info) << " CbmMuchDigitizerQa: MC data read.";
  }
  else {
    LOG(info) << " CbmMuchDigitizerQa: No MC data found.";
    fMCTracks = nullptr;
    fPoints   = nullptr;
  }
  histFolder = fOutFolder.AddFolder("hist", "Histogramms");
  fNevents.SetVal(0);
  histFolder->Add(&fNevents);

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

// -------------------------------------------------------------------------
int CbmMuchDigitizerQa::InitChannelPadInfo()
{

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

  //  Int_t nTotSectors  = 0; // not used FU 23.03.23
  Int_t nTotChannels = 0;
  for (Int_t iStation = 0; iStation < fNstations; iStation++) {
    Int_t nChannels                = 0;
    Int_t nSectors                 = 0;
    Double_t padMinLx              = std::numeric_limits<Double_t>::max();
    Double_t padMinLy              = std::numeric_limits<Double_t>::max();
    Double_t padMaxLx              = std::numeric_limits<Double_t>::min();
    Double_t padMaxLy              = std::numeric_limits<Double_t>::min();
    vector<CbmMuchModule*> modules = fGeoScheme->GetModules(iStation);
    for (UInt_t im = 0; im < modules.size(); im++) {
      CbmMuchModule* mod = modules[im];
      if (!mod) {
        LOG(fatal) << "module pointer is 0";
        return -1;
      }
      if (mod->GetDetectorType() != 4 && mod->GetDetectorType() != 3) {
        LOG(fatal) << "unknown detector type " << mod->GetDetectorType();
        return -1;
      }
      CbmMuchModuleGem* module = dynamic_cast<CbmMuchModuleGem*>(mod);
      if (!module) {
        LOG(fatal) << "module is not a Gem module";
        return -1;
      }
      nChannels += module->GetNPads();
      nSectors += module->GetNSectors();
      vector<CbmMuchPad*> pads = module->GetPads();
      for (UInt_t ip = 0; ip < pads.size(); ip++) {
        CbmMuchPad* pad = pads[ip];
        if (!pad) {
          LOG(fatal) << "pad pointer is 0";
          return -1;
        }
        if (pad->GetDx() < padMinLx) padMinLx = pad->GetDx();
        if (pad->GetDy() < padMinLy) padMinLy = pad->GetDy();
        if (pad->GetDx() > padMaxLx) padMaxLx = pad->GetDx();
        if (pad->GetDy() > padMaxLy) padMaxLy = pad->GetDy();
      }
    }

    if (fPadMinLx > padMinLx) fPadMinLx = padMinLx;
    if (fPadMinLy > padMinLy) fPadMinLy = padMinLy;
    if (fPadMaxLx < padMaxLx) fPadMaxLx = padMaxLx;
    if (fPadMaxLy < padMaxLy) fPadMaxLy = padMaxLy;
    //  nTotSectors += nSectors; // not used FU 23.03.23
    nTotChannels += nChannels;

    if (fVerbose > 2) {
      printf("%i\t\t| %i\t\t| %i\t| %5.4fx%5.4f\t\t| %5.4fx%5.4f\n", iStation + 1, nSectors, nChannels, padMinLx,
             padMinLy, padMaxLx, padMaxLy);
      printf("%i\t\t| %i\t\t\n", iStation + 1, nChannels);
    }
  }
  if (fVerbose > 2) {
    printf("-----------------------------------------------------------\n");
    printf(" Much total channels:\t\t| %i\t\t\n", nTotChannels);
    printf("===========================================================\n");
  }
  return 0;
}

// -------------------------------------------------------------------------
void CbmMuchDigitizerQa::InitCanvases()
{

  /***** charge canvases ****/
  if (fMCTracks && fPoints) {
    fCanvCharge = new CbmQaCanvas("cTrackCharge", "Charge collected per track", 2 * 800, 2 * 400);
    fCanvCharge->Divide2D(3);

    fCanvStationCharge = new CbmQaCanvas("cTrackChargeVsStation", "Track charge per station", 2 * 800, 2 * 400);
    fCanvStationCharge->Divide2D(fNstations);

    fCanvChargeVsEnergy = new CbmQaCanvas("cTrackChargeVsEnergy", "Track charge vs particle Energy", 2 * 800, 2 * 400);
    fCanvChargeVsEnergy->Divide2D(4);

    fCanvChargeVsLength = new CbmQaCanvas("cTrackChargeVsLength", "Track charge vs track length", 2 * 800, 2 * 400);
    fCanvChargeVsLength->Divide2D(4);

    fOutFolder.Add(fCanvCharge);
    fOutFolder.Add(fCanvStationCharge);
    fOutFolder.Add(fCanvChargeVsEnergy);
    fOutFolder.Add(fCanvChargeVsLength);
  }

  /***** length canvas ****/
  if (fMCTracks && fPoints) {
    fCanvTrackLength = new CbmQaCanvas("cTrackLength", "track length", 2 * 800, 2 * 400);
    fCanvTrackLength->Divide2D(4);
    fOutFolder.Add(fCanvTrackLength);
  }

  /***** pad canvases ****/
  fCanvUsPadsFiredXY = new CbmQaCanvas("cPadsFiredXY", "Number of pads fired vs XY", 2 * 400, 2 * 400);
  fCanvUsPadsFiredXY->Divide2D(fNstations);

  fCanvPadOccupancyR = new CbmQaCanvas("cPadOccupancyR", "Pad occupancy [%] vs radius", 2 * 800, 2 * 400);
  fCanvPadOccupancyR->Divide2D(fNstations);

  fCanvPadsTotalR = new CbmQaCanvas("cPadsTotalR", "Total pads vs radius", 2 * 800, 2 * 400);
  fCanvPadsTotalR->Divide2D(fNstations);

  fOutFolder.Add(fCanvUsPadsFiredXY);
  fOutFolder.Add(fCanvPadOccupancyR);
  fOutFolder.Add(fCanvPadsTotalR);

  /***** pad canvas (MC) ****/
  if (fMCTracks && fPoints) {
    fCanvNpadsVsArea = new CbmQaCanvas("cNpadsVsArea", "N pads Vs Area", 2 * 800, 2 * 400);
    fOutFolder.Add(fCanvNpadsVsArea);
  }
}

// -------------------------------------------------------------------------
void CbmMuchDigitizerQa::InitChargeHistos()
{

  if (!fMCTracks || !fPoints) { return; }

  fvTrackCharge.resize(fNstations);
  for (Int_t i = 0; i < fNstations; i++) {
    fvTrackCharge[i] = new TH1F(Form("hTrackCharge%i", i + 1),
                                Form("Track charge : Station %i; Charge [10^4 e]; Count", i + 1), BINNING_CHARGE);
    histFolder->Add(fvTrackCharge[i]);
  }

  fhTrackCharge = new TH1F("hCharge", "Charge distribution from tracks", BINNING_CHARGE);
  fhTrackCharge->GetXaxis()->SetTitle("Charge [10^{4} electrons]");

  fhTrackChargeLog = new TH1F("hChargeLog", "Charge (log.) distribution from tracks", BINNING_CHARGE_LOG);
  fhTrackChargeLog->GetXaxis()->SetTitle("Charge [Lg(Number of electrons)]");

  fhTrackChargePr_1GeV_3mm = new TH1F("hChargePr_1GeV_3mm", "Charge for 1 GeV protons", BINNING_CHARGE);
  fhTrackChargePr_1GeV_3mm->GetXaxis()->SetTitle("Charge [10^{4} electrons]");

  fhTrackChargeVsTrackEnergyLog =
    new TH2F("hChargeEnergyLog", "Charge vs energy (log.) for all tracks", BINNING_ENERGY_LOG, BINNING_CHARGE);

  fhTrackChargeVsTrackEnergyLogPi =
    new TH2F("hChargeEnergyLogPi", "Charge vs energy (log.) for pions", BINNING_ENERGY_LOG, BINNING_CHARGE);

  fhTrackChargeVsTrackEnergyLogPr =
    new TH2F("hChargeEnergyLogPr", "Charge vs energy (log.) for protons", BINNING_ENERGY_LOG, BINNING_CHARGE);

  fhTrackChargeVsTrackEnergyLogEl =
    new TH2F("hChargeEnergyLogEl", "Charge vs energy (log.) for electrons", BINNING_ENERGY_LOG_EL, BINNING_CHARGE);

  fhTrackChargeVsTrackLength =
    new TH2F("hChargeTrackLength", "Charge vs length for all tracks", BINNING_LENGTH, BINNING_CHARGE);

  fhTrackChargeVsTrackLengthPi =
    new TH2F("hChargeTrackLengthPi", "Charge vs length for pions", BINNING_LENGTH, BINNING_CHARGE);

  fhTrackChargeVsTrackLengthPr =
    new TH2F("hChargeTrackLengthPr", "Charge vs length for proton", BINNING_LENGTH, BINNING_CHARGE);

  fhTrackChargeVsTrackLengthEl =
    new TH2F("hChargeTrackLengthEl", "Charge vs length for electrons", BINNING_LENGTH, BINNING_CHARGE);
  std::vector<TH2F*> vChargeHistos;
  vChargeHistos.push_back(fhTrackChargeVsTrackEnergyLog);
  vChargeHistos.push_back(fhTrackChargeVsTrackEnergyLogPi);
  vChargeHistos.push_back(fhTrackChargeVsTrackEnergyLogPr);
  vChargeHistos.push_back(fhTrackChargeVsTrackEnergyLogEl);
  vChargeHistos.push_back(fhTrackChargeVsTrackLength);
  vChargeHistos.push_back(fhTrackChargeVsTrackLengthPi);
  vChargeHistos.push_back(fhTrackChargeVsTrackLengthPr);
  vChargeHistos.push_back(fhTrackChargeVsTrackLengthEl);

  for (UInt_t i = 0; i < vChargeHistos.size(); i++) {
    TH2F* histo = vChargeHistos[i];
    histo->SetStats(0);
    histo->GetYaxis()->SetDecimals(kTRUE);
    histo->GetYaxis()->SetTitleOffset(1.4);
    histo->GetYaxis()->CenterTitle();
    histo->GetYaxis()->SetTitle("Charge [10^{4} electrons]");
    if (i < 4) { histo->GetXaxis()->SetTitle("Lg(E_{kin} [MeV])"); }
    else {
      histo->GetXaxis()->SetTitle("Track length [cm]");
    }
    histFolder->Add(histo);
  }
}

// -------------------------------------------------------------------------
void CbmMuchDigitizerQa::InitLengthHistos()
{

  if (!fMCTracks || !fPoints) { return; }

  fhTrackLength = new TH1F("hTrackLength", "Track length", BINNING_LENGTH);

  fhTrackLengthPi = new TH1F("hTrackLengthPi", "Track length for pions", BINNING_LENGTH);

  fhTrackLengthPr = new TH1F("hTrackLengthPr", "Track length for protons", BINNING_LENGTH);

  fhTrackLengthEl = new TH1F("hTrackLengthEl", "Track length for electrons", BINNING_LENGTH);

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

// -------------------------------------------------------------------------
void CbmMuchDigitizerQa::InitPadHistos()
{
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
      new TH1F(Form("hPadsTotal%i", i + 1), Form("Number of  pads vs radius: Station %i;Radius [cm]", i + 1), 100,
               0.6 * rMin, 1.2 * rMax);

    fvUsPadsFiredR[i] =
      new TH1F(Form("hUsPadsFired%i", i + 1), Form("Number of fired pads vs radius: Station %i;Radius [cm]", i + 1),
               100, 0.6 * rMin, 1.2 * rMax);

    fvUsPadsFiredXY[i] = new TH2F(Form("hUsPadsFiredXY%i", i + 1), Form("Pads fired XY : Station %i; X; Y", i + 1), 100,
                                  -1.2 * rMax, 1.2 * rMax, 100, -1.2 * rMax, 1.2 * rMax);

    fvPadsFiredR[i] =
      new TH1F(Form("hPadsFired%i", i + 1), Form("Number of fired pads vs radius: Station %i;Radius [cm]", i + 1), 100,
               0.6 * rMin, 1.2 * rMax);

    fvPadOccupancyR[i] = new TH1F(Form("hOccupancy%i", i + 1),
                                  Form("Pad occupancy vs radius: Station %i;Radius [cm];Occupancy, %%", i + 1), 100,
                                  0.6 * rMin, 1.2 * rMax);

    histFolder->Add(fvPadsTotalR[i]);
    histFolder->Add(fvUsPadsFiredXY[i]);
    histFolder->Add(fvPadsFiredR[i]);
    histFolder->Add(fvPadOccupancyR[i]);
  }

  // MC below
  if (fMCTracks && fPoints) {
    fhNpadsVsS =
      new TH2F("hNpadsVsS", "Number of fired pads per particle vs average pad area", 50, 0, 5, 15, 0.5, 15.5);
    fhNpadsVsS->SetYTitle("N fired pads");
    fhNpadsVsS->SetXTitle("pad area [cm^2]");
    histFolder->Add(fhNpadsVsS);
  }
}

// -------------------------------------------------------------------------
void CbmMuchDigitizerQa::FillTotalPadsHistos()
{

  vector<CbmMuchModule*> modules = fGeoScheme->GetModules();
  for (vector<CbmMuchModule*>::iterator im = modules.begin(); im != modules.end(); im++) {
    CbmMuchModule* mod = (*im);
    if (mod->GetDetectorType() == 4 || mod->GetDetectorType() == 3) {  // modified for rpc
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

// -------------------------------------------------------------------------
void CbmMuchDigitizerQa::InitFits()
{

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
void CbmMuchDigitizerQa::SetParContainers()
{
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
void CbmMuchDigitizerQa::Exec(Option_t*)
{
  fNevents.SetVal(fNevents.GetVal() + 1);
  LOG(debug) << "Event: " << fNevents.GetVal();

  if (CheckConsistency() != 0) { return; }

  TDirectory* oldDirectory = gDirectory;

  OccupancyQa();

  ProcessMCPoints();
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
int CbmMuchDigitizerQa::CheckConsistency()
{
  // check consistency of geometry & data
  if (!fDigiManager) {
    LOG(error) << "Can not find Much digi manager";
    return -1;
  }
  if (!fGeoScheme) {
    LOG(error) << "Can not find Much geo scheme";
    return -1;
  }

  for (Int_t i = 0; i < fDigiManager->GetNofDigis(ECbmModuleId::kMuch); i++) {
    CbmMuchDigi* digi = (CbmMuchDigi*) fDigiManager->Get<CbmMuchDigi>(i);
    if (!digi) {
      LOG(error) << " Much digi " << i << " out of " << fDigiManager->GetNofDigis(ECbmModuleId::kMuch)
                 << " doesn't exist";
      return -1;
    }
    UInt_t address = digi->GetAddress();

    int ista = CbmMuchAddress::GetStationIndex(address);
    if (ista < 0 || ista >= fNstations) {
      LOG(error) << " Much station " << ista << " for adress " << address << " is out of the range";
      return -1;
    }

    CbmMuchModule* module = fGeoScheme->GetModuleByDetId(address);
    if (!module) {
      LOG(error) << " Much module " << address << " doesn't exist";
      return -1;
    }
    if (module->GetDetectorType() != 4 && module->GetDetectorType() != 3) {
      LOG(error) << " Much module: unknown detector type  " << module->GetDetectorType();
      return -1;
    }
    CbmMuchModuleGem* moduleGem = dynamic_cast<CbmMuchModuleGem*>(module);
    if (!moduleGem) {
      LOG(error) << " Unexpected Much module type: module " << address << " is not a Gem module";
      return -1;
    }
    CbmMuchPad* pad = moduleGem->GetPad(address);
    if (!pad) {
      LOG(error) << " Much pad for adress " << address << " doesn't exist";
      return -1;
    }

    if (fDigiManager->IsMatchPresent(ECbmModuleId::kMuch)) {
      CbmMatch* match = (CbmMatch*) fDigiManager->GetMatch(ECbmModuleId::kMuch, i);
      if (!match) {
        LOG(error) << " Much MC match for digi " << i << " out of " << fDigiManager->GetNofDigis(ECbmModuleId::kMuch)
                   << "doesn't exist";
        return -1;
      }
    }
  }
  return 0;
}

// -------------------------------------------------------------------------
const CbmMuchPad* CbmMuchDigitizerQa::GetPad(UInt_t address) const
{
  // get Much pad from the digi address
  CbmMuchModuleGem* moduleGem = dynamic_cast<CbmMuchModuleGem*>(fGeoScheme->GetModuleByDetId(address));
  CbmMuchPad* pad             = moduleGem->GetPad(address);
  return pad;
}


// -------------------------------------------------------------------------
void CbmMuchDigitizerQa::OccupancyQa()
{
  // Filling occupancy plots
  for (Int_t i = 0; i < fDigiManager->GetNofDigis(ECbmModuleId::kMuch); i++) {
    CbmMuchDigi* digi     = (CbmMuchDigi*) fDigiManager->Get<CbmMuchDigi>(i);
    UInt_t address        = digi->GetAddress();
    int ista              = CbmMuchAddress::GetStationIndex(address);
    const CbmMuchPad* pad = GetPad(address);
    Double_t x0           = pad->GetX();
    Double_t y0           = pad->GetY();
    double r0             = TMath::Sqrt(x0 * x0 + y0 * y0);
    fvUsPadsFiredR[ista]->Fill(r0);
    fvUsPadsFiredXY[ista]->Fill(x0, y0);
  }
}

// -------------------------------------------------------------------------
int CbmMuchDigitizerQa::ProcessMCPoints()
{

  if (!fMCTracks || !fPoints || !fTimeSlice) {
    LOG(debug) << " CbmMuchDigitizerQa::DigitizerQa(): Found no MC data. Skipping.";
    return 0;
  }
  TVector3 vIn;   // in  position of the track
  TVector3 vOut;  // out position of the track
  TVector3 p;     // track momentum

  fMcPointInfoMap.clear();

  std::vector<CbmLink> events = fTimeSlice->GetMatch().GetLinks();
  std::sort(events.begin(), events.end());

  for (uint iLink = 0; iLink < events.size(); iLink++) {
    CbmLink link    = events[iLink];
    int nMuchPoints = fPoints->Size(link);
    int nMcTracks   = fMCTracks->Size(link);
    for (Int_t ip = 0; ip < nMuchPoints; ip++) {
      link.SetIndex(ip);
      CbmMuchPoint* point = (CbmMuchPoint*) fPoints->Get(link);
      if (!point) {
        LOG(error) << " Much MC point " << ip << " out of " << nMuchPoints << " doesn't exist";
        return -1;
      }

      Int_t stId = CbmMuchAddress::GetStationIndex(point->GetDetectorID());

      if (stId < 0 || stId > fNstations) {
        LOG(error) << " Much MC point station " << stId << " is out of the range";
        return -1;
      }

      // Check if the point corresponds to a certain  MC Track
      Int_t trackId = point->GetTrackID();
      if (trackId < 0 || trackId >= nMcTracks) {
        LOG(error) << " Much MC point track Id " << trackId << " is out of the range";
        return -1;
      }

      CbmMCTrack* mcTrack = (CbmMCTrack*) fMCTracks->Get(link.GetFile(), link.GetEntry(), trackId);
      if (!mcTrack) {
        LOG(error) << " MC track" << trackId << " is not found";
        return -1;
      }

      Int_t pdgCode          = mcTrack->GetPdgCode();
      TParticlePDG* particle = TDatabasePDG::Instance()->GetParticle(pdgCode);

      point->PositionIn(vIn);
      point->PositionOut(vOut);
      point->Momentum(p);
      Double_t length = (vOut - vIn).Mag();  // Track length
      Double_t kine   = 0;                   // Kinetic energy
      if (particle) {
        Double_t mass = particle->Mass();
        kine          = sqrt(p.Mag2() + mass * mass) - mass;
      }

      // Get mother pdg code
      Int_t motherPdg = 0;
      Int_t motherId  = mcTrack->GetMotherId();
      if (motherId >= nMcTracks) {
        LOG(error) << " MC track mother Id" << trackId << " is out of the range";
        return -1;
      }
      if (motherId >= 0) {
        CbmMCTrack* motherTrack = (CbmMCTrack*) fMCTracks->Get(link.GetFile(), link.GetEntry(), motherId);
        if (!motherTrack) {
          LOG(error) << " MC track" << trackId << " is not found";
          return -1;
        }
        motherPdg = motherTrack->GetPdgCode();
      }
      fMcPointInfoMap.insert({link, CbmMuchPointInfo(pdgCode, motherPdg, kine, length, stId)});
    }  // points
  }    // events
  return 0;
}

// -------------------------------------------------------------------------
void CbmMuchDigitizerQa::FillChargePerPoint()
{

  if (!fMCTracks || !fPoints) {
    LOG(debug) << " CbmMuchDigitizerQa::FillChargePerPoint()): Found no MC "
                  "data. Skipping.";
    return;
  }
  for (Int_t i = 0; i < fDigiManager->GetNofDigis(ECbmModuleId::kMuch); i++) {
    CbmMuchDigi* digi     = (CbmMuchDigi*) fDigiManager->Get<CbmMuchDigi>(i);
    const CbmMatch* match = fDigiManager->GetMatch(ECbmModuleId::kMuch, i);
    if (!match) {
      LOG(error) << "digi has no match";
      return;
    }
    const CbmMuchPad* pad = GetPad(digi->GetAddress());
    Double_t area         = pad->GetDx() * pad->GetDy();
    Int_t nofLinks        = match->GetNofLinks();
    for (Int_t pt = 0; pt < nofLinks; pt++) {
      const CbmLink& link    = match->GetLink(pt);
      Int_t charge           = link.GetWeight();
      CbmMuchPointInfo& info = getPointInfo(link);
      info.AddCharge(charge);
      info.AddArea(area);
    }
  }
}

// -------------------------------------------------------------------------
void CbmMuchDigitizerQa::FillDigitizerPerformancePlots()
{

  if (!fMCTracks || !fPoints) {
    LOG(debug) << " CbmMuchDigitizerQa::FillDigitizerPerformancePlots(): Found "
                  "no MC data. Skipping.";
    return;
  }
  Bool_t verbose = (fVerbose > 2);
  int iPoint     = 0;
  for (auto it = fMcPointInfoMap.begin(); it != fMcPointInfoMap.end(); it++, ++iPoint) {
    const CbmMuchPointInfo& info = it->second;
    if (verbose) {
      printf("%i", iPoint);
      info.Print();
    }
    Double_t length = info.GetLength();
    Double_t kine   = 1000 * (info.GetKine());
    Double_t charge = info.GetCharge();
    Int_t pdg       = info.GetPdgCode();
    if (charge <= 0) continue;

    if (pdg == 22 ||  // photons
        pdg == 2112)  // neutrons
    {
      LOG(error) << "Particle with pdg code " << pdg << " produces signal in Much";
    }

    // special entry at -0.2 for the particles that are not known for TDataBasePDG
    Double_t log_kine   = (kine > 0) ? TMath::Log10(kine) : -0.2;
    Double_t log_charge = TMath::Log10(charge);
    charge              = charge / 1.e+4;  // measure charge in 10^4 electrons

    Int_t nPads   = info.GetNPads();
    Double_t area = info.GetArea() / nPads;
    // printf("%f %i\n",TMath::Log2(area),nPads);
    //fhNpadsVsS->Fill(TMath::Log2(area), nPads);
    fhNpadsVsS->Fill(area, nPads);
    fhTrackCharge->Fill(charge);
    fvTrackCharge[info.GetStationId()]->Fill(charge);
    fhTrackChargeLog->Fill(log_charge);
    fhTrackChargeVsTrackEnergyLog->Fill(log_kine, charge);
    fhTrackChargeVsTrackLength->Fill(length, charge);
    fhTrackLength->Fill(length);

    if (pdg == 2212) {
      fhTrackChargeVsTrackEnergyLogPr->Fill(log_kine, charge);
      fhTrackChargeVsTrackLengthPr->Fill(length, charge);
      fhTrackLengthPr->Fill(length);
    }
    else if (pdg == 211 || pdg == -211) {
      fhTrackChargeVsTrackEnergyLogPi->Fill(log_kine, charge);
      fhTrackChargeVsTrackLengthPi->Fill(length, charge);
      fhTrackLengthPi->Fill(length);
    }
    else if (pdg == 11 || pdg == -11) {
      fhTrackChargeVsTrackEnergyLogEl->Fill(log_kine, charge);
      fhTrackChargeVsTrackLengthEl->Fill(length, charge);
      fhTrackLengthEl->Fill(length);
    }
    if (pdg == 2212 && length > 0.3 && length < 0.32 && kine > 1000 && kine < 1020)
      fhTrackChargePr_1GeV_3mm->Fill(charge);
  }
}


// -------------------------------------------------------------------------
void CbmMuchDigitizerQa::DrawChargeCanvases()
{

  if (!fMCTracks || !fPoints) { return; }

  for (Int_t i = 0; i < fNstations; i++) {
    fCanvStationCharge->cd(i + 1);
    fvTrackCharge[i]->DrawCopy("", "");
  }
  fCanvCharge->cd(1);
  fhTrackCharge->DrawCopy("", "");
  fCanvCharge->cd(2);
  fhTrackChargeLog->DrawCopy("", "");
  fCanvCharge->cd(3);
  fhTrackChargePr_1GeV_3mm->DrawCopy("", "");

  for (Int_t i = 0; i < 4; i++) {
    fCanvChargeVsEnergy->cd(i + 1);
    gPad->Range(0, 0, 200, 200);
    gPad->SetBottomMargin(0.11);
    gPad->SetRightMargin(0.14);
    gPad->SetLeftMargin(0.12);
    gPad->SetLogz();
  }
  fCanvChargeVsEnergy->cd(1);
  fhTrackChargeVsTrackEnergyLog->DrawCopy("colz", "");
  fCanvChargeVsEnergy->cd(2);
  fhTrackChargeVsTrackEnergyLogPi->DrawCopy("colz", "");
  fFitPi->DrawCopy("same");
  fCanvChargeVsEnergy->cd(3);
  fhTrackChargeVsTrackEnergyLogPr->DrawCopy("colz", "");
  fFitPr->DrawCopy("same");
  fCanvChargeVsEnergy->cd(4);
  fhTrackChargeVsTrackEnergyLogEl->DrawCopy("colz", "");
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
  fhTrackChargeVsTrackLength->DrawCopy("colz", "");
  fCanvChargeVsLength->cd(2);
  fhTrackChargeVsTrackLengthPi->DrawCopy("colz", "");
  fCanvChargeVsLength->cd(3);
  fhTrackChargeVsTrackLengthPr->DrawCopy("colz", "");
  fCanvChargeVsLength->cd(4);
  fhTrackChargeVsTrackLengthEl->DrawCopy("colz", "");
}

// -------------------------------------------------------------------------
void CbmMuchDigitizerQa::DrawPadCanvases()
{
  //non-MC
  for (Int_t i = 0; i < fNstations; i++) {
    *fvPadsFiredR[i] = *fvUsPadsFiredR[i];
    //fvPadsFiredR[i]->Sumw2();
    fvPadsFiredR[i]->Scale(1. / fNevents.GetVal());
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

// -------------------------------------------------------------------------
void CbmMuchDigitizerQa::DrawLengthCanvases()
{

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

// -------------------------------------------------------------------------
void CbmMuchDigitizerQa::PrintFrontLayerPoints()
{

  if (!fMCTracks || !fTimeSlice) { return; }

  const CbmMatch& match = fTimeSlice->GetMatch();
  for (int iLink = 0; iLink < match.GetNofLinks(); iLink++) {
    CbmLink link    = match.GetLink(iLink);
    int nMuchPoints = fPoints->Size(link);
    for (Int_t ip = 0; ip < nMuchPoints; ip++) {
      link.SetIndex(ip);
      CbmMuchPoint* point = (CbmMuchPoint*) fPoints->Get(link);
      if (!point) {
        LOG(error) << " Much MC point " << ip << " out of " << nMuchPoints << " doesn't exist";
        return;
      }
      Int_t stId = CbmMuchAddress::GetStationIndex(point->GetDetectorID());
      if (stId != 0) continue;
      Int_t layerId = CbmMuchAddress::GetLayerIndex(point->GetDetectorID());
      if (layerId != 0) continue;
      printf("point %4i xin=%6.2f yin=%6.2f xout=%6.2f yout=%6.2f zin=%6.2f\n", ip, point->GetXIn(), point->GetYIn(),
             point->GetXOut(), point->GetYOut(), point->GetZIn());
    }
  }
}

// -------------------------------------------------------------------------
void CbmMuchDigitizerQa::PrintFrontLayerDigis()
{
  for (Int_t i = 0; i < fDigiManager->GetNofDigis(ECbmModuleId::kMuch); i++) {
    CbmMuchDigi* digi = (CbmMuchDigi*) fDigiManager->Get<CbmMuchDigi>(i);
    UInt_t address    = digi->GetAddress();
    Int_t stId        = CbmMuchAddress::GetStationIndex(address);
    if (stId != 0) continue;
    Int_t layerId = CbmMuchAddress::GetLayerIndex(address);
    if (layerId != 0) continue;
    CbmMuchModuleGem* module = (CbmMuchModuleGem*) fGeoScheme->GetModuleByDetId(address);
    if (!module) continue;
    CbmMuchPad* pad = module->GetPad(address);
    Double_t x0     = pad->GetX();
    Double_t y0     = pad->GetY();
    UInt_t charge   = digi->GetAdc();
    printf("digi %4i x0=%5.1f y0=%5.1f charge=%3i\n", i, x0, y0, charge);
  }
}

// -------------------------------------------------------------------------
TFolder& CbmMuchDigitizerQa::GetQa()
{
  TDirectory* oldDirectory = gDirectory;
  DrawChargeCanvases();
  DrawPadCanvases();
  DrawLengthCanvases();
  gDirectory = oldDirectory;
  return fOutFolder;
}

// -------------------------------------------------------------------------
void CbmMuchDigitizerQa::Finish()
{

  if (!FairRootManager::Instance() || !FairRootManager::Instance()->GetSink()) {
    LOG(error) << "No sink found";
    return;
  }
  FairSink* sink = FairRootManager::Instance()->GetSink();
  sink->WriteObject(&GetQa(), nullptr);
}

// -------------------------------------------------------------------------
void CbmMuchDigitizerQa::OutputNvsS()
{
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
Double_t CbmMuchDigitizerQa::LandauMPV(Double_t* lg_x, Double_t* par)
{
  Double_t gaz_gain_mean = 1.7e+4;
  Double_t scale         = 1.e+6;
  gaz_gain_mean /= scale;
  Double_t mass = par[0];  // mass in MeV
  Double_t x    = TMath::Power(10, lg_x[0]);
  return gaz_gain_mean * MPV_n_e(x, mass);
}

// -------------------------------------------------------------------------
Double_t CbmMuchDigitizerQa::MPV_n_e(Double_t Tkin, Double_t mass)
{
  Double_t logT;
  TF1 fPol6("fPol6", "pol6", -5, 10);
  if (mass < 0.1) {
    logT = log(Tkin * 0.511 / mass);
    if (logT > 9.21034) logT = 9.21034;
    if (logT < min_logT_e) logT = min_logT_e;
    return fPol6.EvalPar(&logT, mpv_e);
  }
  else if (mass >= 0.1 && mass < 0.2) {
    logT = log(Tkin * 105.658 / mass);
    if (logT > 9.21034) logT = 9.21034;
    if (logT < min_logT_mu) logT = min_logT_mu;
    return fPol6.EvalPar(&logT, mpv_mu);
  }
  else {
    logT = log(Tkin * 938.272 / mass);
    if (logT > 9.21034) logT = 9.21034;
    if (logT < min_logT_p) logT = min_logT_p;
    return fPol6.EvalPar(&logT, mpv_p);
  }
}
