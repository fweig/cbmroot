/* Copyright (C) 2007-2021 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer], Florian Uhlig */

/**
 * \file CbmLitTrackingQa.cxx
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2007
 */

#include "CbmLitTrackingQa.h"

#include "CbmGlobalTrack.h"
#include "CbmHistManager.h"
#include "CbmHit.h"
#include "CbmLitTrackingQaReport.h"
#include "CbmLitTrackingQaStudyReport.h"
#include "CbmMCDataArray.h"
#include "CbmMCDataManager.h"
#include "CbmMCTrack.h"
#include "CbmMatch.h"
#include "CbmRichRing.h"
#include "CbmRichUtil.h"
#include "CbmStsSetup.h"
#include "CbmStsTrack.h"
#include "CbmTrackMatchNew.h"
#include "CbmUtils.h"
#include "elid/CbmLitGlobalElectronId.h"
#include "qa/base/CbmLitAcceptanceFunction.h"
#include "qa/mc/CbmLitMCTrackCreator.h"

#include "FairMCPoint.h"

#include "TClonesArray.h"
#include "TH1.h"
#include "TH2F.h"
#include <TFile.h>

#include <boost/assign/list_of.hpp>

#include <fstream>
#include <iostream>

using boost::assign::list_of;
using Cbm::FindAndReplace;
using Cbm::Split;
using std::cout;
using std::list;
using std::make_pair;
using std::pair;

CbmLitTrackingQa::CbmLitTrackingQa()
  : FairTask("LitTrackingQA", 1)
  , fHM(NULL)
  , fOutputDir("")
  , fDet()
  , fMCTrackCreator(NULL)
  , fMinNofPointsSts(4)
  , fMinNofPointsTrd(2)
  ,  // SIS100
  fMinNofPointsMuch(10)
  , fMinNofPointsTof(1)
  , fQuota(0.7)
  , fUseConsecutivePointsInSts(true)
  , fMinNofHitsRich(7)
  , fQuotaRich(0.6)
  , fMinNofHitsTrd(2)
  ,  // SIS100
  fMinNofHitsMuch(10)
  , fPRangeMin(0.)
  , fPRangeMax(10.)
  , fPRangeBins(20.)
  , fPtRangeMin(0.)
  , fPtRangeMax(3.)
  , fPtRangeBins(12.)
  , fYRangeMin(0.)
  , fYRangeMax(4.)
  , fYRangeBins(16)
  , fAngleRangeMin(0.)
  , fAngleRangeMax(25.)
  , fAngleRangeBins(10)
  , fMCTracks(NULL)
  , fGlobalTracks(NULL)
  , fMvdPoints(NULL)
  , fMvdHitMatches(NULL)
  , fStsTracks(NULL)
  , fStsMatches(NULL)
  , fRichRings(NULL)
  , fRichProjections(NULL)
  , fRichRingMatches(NULL)
  , fMuchMatches(NULL)
  , fTrdMatches(NULL)
  , fTofPoints(NULL)
  , fTofMatches(NULL)
  , fRichAnnCut(0.0)  // values loose | strict: -0.4 | 0.0
  , fTrdAnnCut(0.8)   // at the moment, TrdAnn is likelihood value	// values loose | strict: 0.2 | 0.8
{
}

CbmLitTrackingQa::~CbmLitTrackingQa()
{
  if (fHM) delete fHM;
}

InitStatus CbmLitTrackingQa::Init()
{
  // Create histogram manager which is used throughout the program
  fHM = new CbmHistManager();

  fDet.DetermineSetup();
  if (fTrackCategories.empty()) FillDefaultTrackCategories();
  if (fRingCategories.empty()) FillDefaultRingCategories();

  FillDefaultTrackPIDCategories();
  FillDefaultRingPIDCategories();
  FillDefaultPiSuppCategories();
  FillTrackAndRingAcceptanceFunctions();

  CreateHistograms();

  ReadDataBranches();

  // --- Get STS setup
  CbmStsSetup* stsSetup = CbmStsSetup::Instance();

  if (!stsSetup->IsInit()) stsSetup->Init();

  fMCTrackCreator = CbmLitMCTrackCreator::Instance();

  fMcToRecoMap.clear();
  vector<string> trackVariants = GlobalTrackVariants();
  for (UInt_t i = 0; i < trackVariants.size(); i++) {
    fMcToRecoMap.insert(make_pair(trackVariants[i], multimap<pair<Int_t, Int_t>, Int_t>()));
  }

  CbmLitGlobalElectronId::GetInstance().SetTrdAnnCut(fTrdAnnCut);
  CbmLitGlobalElectronId::GetInstance().SetRichAnnCut(fRichAnnCut);

  return kSUCCESS;
}

void CbmLitTrackingQa::Exec(Option_t* /*opt*/)
{
  // Increase event counter
  fHM->H1("hen_EventNo_TrackingQa")->Fill(0.5);
  Int_t eventNum = fHM->H1("hen_EventNo_TrackingQa")->GetEntries() - 1;
  std::cout << "CbmLitTrackingQa::Exec: event=" << eventNum << std::endl;

  fMCTrackCreator->Create(eventNum);

  ProcessGlobalTracks();
  ProcessMcTracks(eventNum);
  PionSuppression();
  IncreaseCounters();
}

void CbmLitTrackingQa::Finish()
{

  TDirectory* oldir = gDirectory;
  TFile* outFile    = FairRootManager::Instance()->GetOutFile();
  if (outFile != NULL) {
    outFile->cd();
    fHM->WriteToFile();
  }
  gDirectory->cd(oldir->GetPath());

  if (fOutputDir != "") {
    CbmSimulationReport* report = new CbmLitTrackingQaReport();
    report->Create(fHM, fOutputDir);
    delete report;
  }
}

void CbmLitTrackingQa::ReadDataBranches()
{
  FairRootManager* ioman = FairRootManager::Instance();
  if (NULL == ioman) { Fatal("Init", "CbmRootManager is not instantiated"); }

  CbmMCDataManager* mcManager = (CbmMCDataManager*) ioman->GetObject("MCDataManager");
  fMCTracks                   = mcManager->InitBranch("MCTrack");
  if (NULL == fMCTracks) { Fatal("Init", "No MCTrack array!"); }

  fGlobalTracks = (TClonesArray*) ioman->GetObject("GlobalTrack");
  if (NULL == fGlobalTracks) { Fatal("Init", "No GlobalTrack array!"); }

  if (fDet.GetDet(ECbmModuleId::kMvd)) {
    fMvdPoints = mcManager->InitBranch("MvdPoint");
    if (NULL == fMvdPoints) { Fatal("Init", ": No MvdPoint array!"); }
    fMvdHitMatches = (TClonesArray*) ioman->GetObject("MvdHitMatch");
    if (NULL == fMvdHitMatches) { Fatal("Init", ": No MvdHitMatch array!"); }
  }

  if (fDet.GetDet(ECbmModuleId::kSts)) {
    fStsTracks = (TClonesArray*) ioman->GetObject("StsTrack");
    if (NULL == fStsTracks) { Fatal("Init", ": No StsTrack array!"); }
    fStsMatches = (TClonesArray*) ioman->GetObject("StsTrackMatch");
    if (NULL == fStsMatches) { Fatal("Init", ": No StsTrackMatch array!"); }
  }

  if (fDet.GetDet(ECbmModuleId::kRich)) {
    fRichRings = (TClonesArray*) ioman->GetObject("RichRing");
    if (NULL == fRichRings) { Fatal("Init", "No RichRing array!"); }
    fRichProjections = (TClonesArray*) ioman->GetObject("RichProjection");
    if (NULL == fRichProjections) { Fatal("Init", "No RichProjection array!"); }
    fRichRingMatches = (TClonesArray*) ioman->GetObject("RichRingMatch");
    if (NULL == fRichRingMatches) { Fatal("Init", "No RichRingMatch array!"); }
  }

  if (fDet.GetDet(ECbmModuleId::kMuch)) {
    fMuchMatches = (TClonesArray*) ioman->GetObject("MuchTrackMatch");
    if (NULL == fMuchMatches) { Fatal("Init", "No MuchTrackMatch array!"); }
  }

  if (fDet.GetDet(ECbmModuleId::kTrd)) {
    fTrdMatches = (TClonesArray*) ioman->GetObject("TrdTrackMatch");
    if (NULL == fTrdMatches) { Fatal("Init", "No TrdTrackMatch array!"); }
  }

  if (fDet.GetDet(ECbmModuleId::kTof)) {
    fTofPoints = mcManager->InitBranch("TofPoint");
    if (NULL == fTofPoints) { Fatal("Init", "No TofPoint array!"); }
    fTofMatches = (TClonesArray*) ioman->GetObject("TofHitMatch");
    if (NULL == fTofMatches) { Fatal("Init", "No TofHitMatch array!"); }
  }
}

void CbmLitTrackingQa::FillDefaultTrackCategories()
{
  string elMu = fDet.GetElectronSetup() ? "Electron" : "Muon";
  vector<string> tmp {"All",    "Primary",  "Secondary", "Reference", elMu,
                      "Proton", "PionPlus", "PionMinus", "KaonPlus",  "KaonMinus"};
  fTrackCategories = tmp;
}

void CbmLitTrackingQa::FillDefaultRingCategories()
{
  vector<string> tmp {"All", "AllReference", "Electron", "ElectronReference", "Pion", "PionReference"};
  fRingCategories = tmp;
}

void CbmLitTrackingQa::FillDefaultTrackPIDCategories()
{
  if (fDet.GetElectronSetup()) {
    vector<string> tmp  = list_of("Electron");
    fTrackCategoriesPID = tmp;
  }
}

void CbmLitTrackingQa::FillDefaultRingPIDCategories()
{
  if (fDet.GetElectronSetup()) {
    vector<string> tmp = list_of("Electron");
    fRingCategoriesPID = tmp;
  }
}

void CbmLitTrackingQa::FillDefaultPiSuppCategories()
{
  if (fDet.GetElectronSetup()) {
    vector<string> tmp = list_of("All")("TrueMatch")("WrongMatch");
    fPiSuppCategories  = tmp;
  }
}

void CbmLitTrackingQa::FillTrackAndRingAcceptanceFunctions()
{
  // List of all supported track categories
  fTrackAcceptanceFunctions["All"]       = CbmLitAcceptanceFunction::AllTrackAcceptanceFunction;
  fTrackAcceptanceFunctions["Primary"]   = CbmLitAcceptanceFunction::PrimaryTrackAcceptanceFunction;
  fTrackAcceptanceFunctions["Secondary"] = CbmLitAcceptanceFunction::SecondaryTrackAcceptanceFunction;
  fTrackAcceptanceFunctions["Reference"] = CbmLitAcceptanceFunction::ReferenceTrackAcceptanceFunction;
  fTrackAcceptanceFunctions["Electron"]  = CbmLitAcceptanceFunction::PrimaryElectronTrackAcceptanceFunction;
  fTrackAcceptanceFunctions["Muon"]      = CbmLitAcceptanceFunction::PrimaryMuonTrackAcceptanceFunction;
  fTrackAcceptanceFunctions["Proton"]    = CbmLitAcceptanceFunction::ProtonTrackAcceptanceFunction;
  fTrackAcceptanceFunctions["Pion"]      = CbmLitAcceptanceFunction::PionTrackAcceptanceFunction;
  fTrackAcceptanceFunctions["PionPlus"]  = CbmLitAcceptanceFunction::PionPlusTrackAcceptanceFunction;
  fTrackAcceptanceFunctions["PionMinus"] = CbmLitAcceptanceFunction::PionMinusTrackAcceptanceFunction;
  fTrackAcceptanceFunctions["Kaon"]      = CbmLitAcceptanceFunction::KaonTrackAcceptanceFunction;
  fTrackAcceptanceFunctions["KaonPlus"]  = CbmLitAcceptanceFunction::KaonPlusTrackAcceptanceFunction;
  fTrackAcceptanceFunctions["KaonMinus"] = CbmLitAcceptanceFunction::KaonMinusTrackAcceptanceFunction;

  // List of all supported ring categories
  fRingAcceptanceFunctions["All"]          = CbmLitAcceptanceFunction::AllRingAcceptanceFunction;
  fRingAcceptanceFunctions["AllReference"] = CbmLitAcceptanceFunction::AllReferenceRingAcceptanceFunction;
  fRingAcceptanceFunctions["Electron"]     = CbmLitAcceptanceFunction::PrimaryElectronRingAcceptanceFunction;
  fRingAcceptanceFunctions["ElectronReference"] =
    CbmLitAcceptanceFunction::PrimaryElectronReferenceRingAcceptanceFunction;
  fRingAcceptanceFunctions["Pion"]          = CbmLitAcceptanceFunction::PionRingAcceptanceFunction;
  fRingAcceptanceFunctions["PionReference"] = CbmLitAcceptanceFunction::PionReferenceRingAcceptanceFunction;

  // list of pion suppression categories
  fPiSuppAcceptanceFunctions["All"]        = CbmLitAcceptanceFunction::AllPiSuppAcceptanceFunction;
  fPiSuppAcceptanceFunctions["TrueMatch"]  = CbmLitAcceptanceFunction::TrueMatchPiSuppAcceptanceFunction;
  fPiSuppAcceptanceFunctions["WrongMatch"] = CbmLitAcceptanceFunction::WrongMatchPiSuppAcceptanceFunction;
}

void CbmLitTrackingQa::CreateH1Efficiency(const string& name, const string& parameter, const string& xTitle,
                                          Int_t nofBins, Double_t minBin, Double_t maxBin, const string& opt)
{
  assert(opt == "track" || opt == "ring" || opt == "track_pid" || opt == "ring_pid");
  vector<string> types = list_of("Acc")("Rec")("Eff");
  vector<string> cat   = (opt == "track")       ? fTrackCategories
                         : (opt == "ring")      ? fRingCategories
                         : (opt == "track_pid") ? fTrackCategoriesPID
                                                : fRingCategoriesPID;

  for (UInt_t iCat = 0; iCat < cat.size(); iCat++) {
    for (Int_t iType = 0; iType < 3; iType++) {
      string yTitle    = (types[iType] == "Eff") ? "Efficiency [%]" : "Counter";
      string histName  = name + "_" + cat[iCat] + "_" + types[iType] + "_" + parameter;
      string histTitle = histName + ";" + xTitle + ";" + yTitle;
      fHM->Add(histName, new TH1F(histName.c_str(), histTitle.c_str(), nofBins, minBin, maxBin));
    }
  }
}

void CbmLitTrackingQa::CreateH2Efficiency(const string& name, const string& parameter, const string& xTitle,
                                          const string& yTitle, Int_t nofBinsX, Double_t minBinX, Double_t maxBinX,
                                          Int_t nofBinsY, Double_t minBinY, Double_t maxBinY, const string& opt)
{
  assert(opt == "track" || opt == "ring" || opt == "track_pid" || opt == "ring_pid");
  vector<string> types = list_of("Acc")("Rec")("Eff");
  vector<string> cat   = (opt == "track")       ? fTrackCategories
                         : (opt == "ring")      ? fRingCategories
                         : (opt == "track_pid") ? fTrackCategoriesPID
                                                : fRingCategoriesPID;

  for (UInt_t iCat = 0; iCat < cat.size(); iCat++) {
    for (Int_t iType = 0; iType < 3; iType++) {
      string zTitle    = (types[iType] == "Eff") ? "Efficiency [%]" : "Counter";
      string histName  = name + "_" + cat[iCat] + "_" + types[iType] + "_" + parameter;
      string histTitle = histName + ";" + xTitle + ";" + yTitle + ";" + zTitle;
      fHM->Add(histName,
               new TH2F(histName.c_str(), histTitle.c_str(), nofBinsX, minBinX, maxBinX, nofBinsY, minBinY, maxBinY));
    }
  }
}

void CbmLitTrackingQa::CreateH1PionSuppression(const string& name, const string& parameter, const string& xTitle,
                                               Int_t nofBins, Double_t minBin, Double_t maxBin)
{
  vector<string> types = list_of("RecPions")("Rec")("PionSup");
  for (UInt_t iCat = 0; iCat < fPiSuppCategories.size(); iCat++) {
    for (Int_t iType = 0; iType < 3; iType++) {
      string yTitle    = (types[iType] == "PionSup") ? "Pion suppression" : "Counter";
      string histName  = name + "_" + fPiSuppCategories[iCat] + "_" + types[iType] + "_" + parameter;
      string histTitle = histName + ";" + xTitle + ";" + yTitle;
      fHM->Add(histName, new TH1F(histName.c_str(), histTitle.c_str(), nofBins, minBin, maxBin));
    }
  }
}

void CbmLitTrackingQa::CreateH1(const string& name, const string& xTitle, const string& yTitle, Int_t nofBins,
                                Double_t minBin, Double_t maxBin)
{
  TH1F* h = new TH1F(name.c_str(), string(name + ";" + xTitle + ";" + yTitle).c_str(), nofBins, minBin, maxBin);
  fHM->Add(name, h);
}

void CbmLitTrackingQa::CreateH2(const string& name, const string& xTitle, const string& yTitle, const string& zTitle,
                                Int_t nofBinsX, Double_t minBinX, Double_t maxBinX, Int_t nofBinsY, Double_t minBinY,
                                Double_t maxBinY)
{
  TH2F* h = new TH2F(name.c_str(), (name + ";" + xTitle + ";" + yTitle + ";" + zTitle).c_str(), nofBinsX, minBinX,
                     maxBinX, nofBinsY, minBinY, maxBinY);
  fHM->Add(name, h);
}

void CbmLitTrackingQa::CreateTrackHitsHistogram(const string& detName)
{
  string type[]  = {"All", "True", "Fake", "TrueOverAll", "FakeOverAll"};
  Double_t min[] = {-0.5, -0.5, -0.5, -0.1, -0.1};
  Double_t max[] = {99.5, 99.5, 99.5, 1.1, 1.1};
  Int_t bins[]   = {100, 100, 100, 12, 12};
  for (Int_t i = 0; i < 5; i++) {
    string xTitle   = (i == 3 || i == 4) ? "Ratio" : "Number of hits";
    string histName = "hth_" + detName + "_TrackHits_" + type[i];
    CreateH1(histName.c_str(), xTitle, "Yeild", bins[i], min[i], max[i]);
  }
}

vector<string> CbmLitTrackingQa::CreateGlobalTrackingHistogramNames(const vector<string>& detectors)
{
  vector<string> histos;
  Int_t nofDetectors = detectors.size();
  for (Int_t iDet = 0; iDet < nofDetectors; iDet++) {
    string histEff;
    for (Int_t i = 0; i <= iDet; i++) {
      histEff += detectors[i];
    }
    string histNorm = histEff;
    histos.push_back("hte_" + histEff + "_" + histNorm);
    for (Int_t i = iDet + 1; i < nofDetectors; i++) {
      histNorm += detectors[i];
      histos.push_back("hte_" + histEff + "_" + histNorm);
    }
  }
  return histos;
}

vector<string> CbmLitTrackingQa::CreateGlobalTrackingHistogramNames()
{
  // Histograms w/o RICH detector
  vector<string> detectors;
  if (fDet.GetDet(ECbmModuleId::kSts)) detectors.push_back("Sts");
  if (fDet.GetDet(ECbmModuleId::kMuch)) detectors.push_back("Much");
  if (fDet.GetDet(ECbmModuleId::kTrd)) detectors.push_back("Trd");
  if (fDet.GetDet(ECbmModuleId::kTof)) detectors.push_back("Tof");
  vector<string> names1 = CreateGlobalTrackingHistogramNames(detectors);

  // Histograms with RICH detector
  vector<string> names2;
  if (fDet.GetDet(ECbmModuleId::kRich)) {
    detectors.clear();
    if (fDet.GetDet(ECbmModuleId::kSts)) detectors.push_back("Sts");
    if (fDet.GetDet(ECbmModuleId::kRich)) detectors.push_back("Rich");
    if (fDet.GetDet(ECbmModuleId::kTrd)) detectors.push_back("Trd");
    if (fDet.GetDet(ECbmModuleId::kTof)) detectors.push_back("Tof");
    names2 = CreateGlobalTrackingHistogramNames(detectors);
  }

  set<string> names;
  names.insert(names1.begin(), names1.end());
  names.insert(names2.begin(), names2.end());
  vector<string> nameVector(names.begin(), names.end());
  return nameVector;
}

vector<string> CbmLitTrackingQa::GlobalTrackVariants()
{
  set<string> trackVariants;
  // Histograms w/o RICH detector
  vector<string> detectors;
  if (fDet.GetDet(ECbmModuleId::kSts)) detectors.push_back("Sts");
  if (fDet.GetDet(ECbmModuleId::kMuch)) detectors.push_back("Much");
  if (fDet.GetDet(ECbmModuleId::kTrd)) detectors.push_back("Trd");
  if (fDet.GetDet(ECbmModuleId::kTof)) detectors.push_back("Tof");
  string name("");
  for (UInt_t i = 0; i < detectors.size(); i++) {
    name += detectors[i];
    trackVariants.insert(name);
  }

  // Histograms with RICH detector
  if (fDet.GetDet(ECbmModuleId::kRich)) {
    detectors.clear();
    if (fDet.GetDet(ECbmModuleId::kSts)) detectors.push_back("Sts");
    if (fDet.GetDet(ECbmModuleId::kRich)) detectors.push_back("Rich");
    if (fDet.GetDet(ECbmModuleId::kTrd)) detectors.push_back("Trd");
    if (fDet.GetDet(ECbmModuleId::kTof)) detectors.push_back("Tof");
    name = "";
    for (UInt_t i = 0; i < detectors.size(); i++) {
      name += detectors[i];
      trackVariants.insert(name);
    }
  }
  vector<string> trackVariantsVector(trackVariants.begin(), trackVariants.end());

  trackVariantsVector.push_back("Rich");

  return trackVariantsVector;
}

vector<string> CbmLitTrackingQa::PionSuppressionVariants()
{
  set<string> variants;
  // Histograms w/o RICH detector
  vector<string> detectors;
  if (fDet.GetDet(ECbmModuleId::kTrd)) detectors.push_back("Trd");
  if (fDet.GetDet(ECbmModuleId::kTof)) detectors.push_back("Tof");
  string name("");
  for (UInt_t i = 0; i < detectors.size(); i++) {
    name += detectors[i];
    variants.insert(name);
  }

  // Histograms with RICH detector
  if (fDet.GetDet(ECbmModuleId::kRich)) {
    detectors.clear();
    if (fDet.GetDet(ECbmModuleId::kRich)) detectors.push_back("Rich");
    if (fDet.GetDet(ECbmModuleId::kTrd)) detectors.push_back("Trd");
    if (fDet.GetDet(ECbmModuleId::kTof)) detectors.push_back("Tof");
    name = "";
    for (UInt_t i = 0; i < detectors.size(); i++) {
      name += detectors[i];
      variants.insert(name);
    }
  }
  vector<string> variantsVector(variants.begin(), variants.end());
  return variantsVector;
}

string CbmLitTrackingQa::LocalEfficiencyNormalization(const string& detName)
{
  set<string> trackVariants;
  // Histograms w/o RICH detector
  vector<string> detectors;
  if (fDet.GetDet(ECbmModuleId::kSts)) detectors.push_back("Sts");
  if (fDet.GetDet(ECbmModuleId::kMuch)) detectors.push_back("Much");
  if (fDet.GetDet(ECbmModuleId::kTrd)) detectors.push_back("Trd");
  if (fDet.GetDet(ECbmModuleId::kTof)) detectors.push_back("Tof");
  string name("");
  for (UInt_t i = 0; i < detectors.size(); i++) {
    name += detectors[i];
    if (detectors[i] == detName) break;
  }
  return name;
}

void CbmLitTrackingQa::CreateHistograms()
{
  fDet.DetermineSetup();

  // Number of points distributions
  Double_t minNofPoints = 0.;
  Double_t maxNofPoints = 100.;
  Int_t nofBinsPoints   = 100;

  // Reconstruction efficiency histograms
  // Local efficiency histograms
  // STS
  //  CreateEffHist3D("hSts3D", "track");
  CreateH1Efficiency("hte_Sts_Sts", "Np", "Number of points", nofBinsPoints, minNofPoints, maxNofPoints, "track");
  CreateH1Efficiency("hte_Sts_Sts", "Angle", "Polar angle [deg]", fAngleRangeBins, fAngleRangeMin, fAngleRangeMax,
                     "track");
  // MUCH
  if (fDet.GetDet(ECbmModuleId::kMuch)) {
    string norm     = LocalEfficiencyNormalization("Much");
    string histName = "hte_Much_" + norm;
    CreateH1Efficiency(histName, "p", "P [GeV/c]", fPRangeBins, fPRangeMin, fPRangeMax, "track");
    CreateH1Efficiency(histName, "y", "Rapidity", fYRangeBins, fYRangeMin, fYRangeMax, "track");
    CreateH1Efficiency(histName, "pt", "P_{t} [GeV/c]", fPtRangeBins, fPtRangeMin, fPtRangeMax, "track");
    CreateH1Efficiency(histName, "Np", "Number of points", nofBinsPoints, minNofPoints, maxNofPoints, "track");
    CreateH1Efficiency(histName, "Angle", "Polar angle [deg]", fAngleRangeBins, fAngleRangeMin, fAngleRangeMax,
                       "track");
    CreateH2Efficiency(histName, "YPt", "Rapidity", "P_{t} [GeV/c]", fYRangeBins, fYRangeMin, fYRangeMax, fPtRangeBins,
                       fPtRangeMin, fPtRangeMax, "track");
  }
  // TRD
  if (fDet.GetDet(ECbmModuleId::kTrd)) {
    string norm     = LocalEfficiencyNormalization("Trd");
    string histName = "hte_Trd_" + norm;
    CreateH1Efficiency(histName, "p", "P [GeV/c]", fPRangeBins, fPRangeMin, fPRangeMax, "track");
    CreateH1Efficiency(histName, "y", "Rapidity", fYRangeBins, fYRangeMin, fYRangeMax, "track");
    CreateH1Efficiency(histName, "pt", "P_{t} [GeV/c]", fPtRangeBins, fPtRangeMin, fPtRangeMax, "track");
    CreateH1Efficiency(histName, "Np", "Number of points", nofBinsPoints, minNofPoints, maxNofPoints, "track");
    CreateH1Efficiency(histName, "Angle", "Polar angle [deg]", fAngleRangeBins, fAngleRangeMin, fAngleRangeMax,
                       "track");
    CreateH2Efficiency(histName, "YPt", "Rapidity", "P_{t} [GeV/c]", fYRangeBins, fYRangeMin, fYRangeMax, fPtRangeBins,
                       fPtRangeMin, fPtRangeMax, "track");
  }
  // TOF
  if (fDet.GetDet(ECbmModuleId::kTof)) {
    string norm     = LocalEfficiencyNormalization("Tof");
    string histName = "hte_Tof_" + norm;
    CreateH1Efficiency(histName, "p", "P [GeV/c]", fPRangeBins, fPRangeMin, fPRangeMax, "track");
    CreateH1Efficiency(histName, "y", "Rapidity", fYRangeBins, fYRangeMin, fYRangeMax, "track");
    CreateH1Efficiency(histName, "pt", "P_{t} [GeV/c]", fPtRangeBins, fPtRangeMin, fPtRangeMax, "track");
    //    CreateEfficiencyHistogram(histName, "Np", "Number of points", nofBinsPoints, minNofPoints, maxNofPoints, "track");
    CreateH1Efficiency(histName, "Angle", "Polar angle [deg]", fAngleRangeBins, fAngleRangeMin, fAngleRangeMax,
                       "track");
    CreateH2Efficiency(histName, "YPt", "Rapidity", "P_{t} [GeV/c]", fYRangeBins, fYRangeMin, fYRangeMax, fPtRangeBins,
                       fPtRangeMin, fPtRangeMax, "track");
  }

  // RICH
  if (fDet.GetDet(ECbmModuleId::kRich)) {
    CreateH1Efficiency("hte_Rich_Rich", "p", "P [GeV/c]", fPRangeBins, fPRangeMin, fPRangeMax, "ring");
    CreateH1Efficiency("hte_Rich_Rich", "y", "Rapidity", fYRangeBins, fYRangeMin, fYRangeMax, "ring");
    CreateH1Efficiency("hte_Rich_Rich", "pt", "P_{t} [GeV/c]", fPtRangeBins, fPtRangeMin, fPtRangeMax, "ring");
    CreateH1Efficiency("hte_Rich_Rich", "RingNh", "Number of hits", nofBinsPoints, minNofPoints, maxNofPoints, "ring");
    CreateH1Efficiency("hte_Rich_Rich", "BoA", "B/A", 50, 0.0, 1.0, "ring");
    CreateH1Efficiency("hte_Rich_Rich", "RingXc", "X [cm]", 60, -120, 120, "ring");
    CreateH1Efficiency("hte_Rich_Rich", "RingYc", "|Y| [cm]", 25, 100, 200, "ring");
    // CreateH1Efficiency("hte_Rich_Rich", "RadPos", "Radial position [cm]", 50, 0., 150., "ring");
    CreateH2Efficiency("hte_Rich_Rich", "RingXcYc", "X [cm]", "Y [cm]", 14, -110., 110, 60, -300, 300, "ring");
    CreateH2Efficiency("hte_Rich_Rich", "YPt", "Rapidity", "P_{t} [GeV/c]", fYRangeBins, fYRangeMin, fYRangeMax,
                       fPtRangeBins, fPtRangeMin, fPtRangeMax, "ring");
  }

  // Global efficiency histograms
  vector<string> histoNames = CreateGlobalTrackingHistogramNames();
  for (UInt_t iHist = 0; iHist < histoNames.size(); iHist++) {
    string name = histoNames[iHist];
    string opt  = (name.find("Rich") == string::npos) ? "track" : "ring";
    // Tracking efficiency
    CreateH1Efficiency(name, "p", "P [GeV/c]", fPRangeBins, fPRangeMin, fPRangeMax, opt);
    CreateH1Efficiency(name, "y", "Rapidity", fYRangeBins, fYRangeMin, fYRangeMax, opt);
    CreateH1Efficiency(name, "pt", "P_{t} [GeV/c]", fPtRangeBins, fPtRangeMin, fPtRangeMax, opt);
    CreateH2Efficiency(name, "YPt", "Rapidity", "P_{t} [GeV/c]", fYRangeBins, fYRangeMin, fYRangeMax, fPtRangeBins,
                       fPtRangeMin, fPtRangeMax, opt);

    //Global efficiency vs. RingXc or RingYc
    if (name.find("Rich") != string::npos) {
      CreateH1Efficiency(name, "RingXc", "Ring Xc [cm]", 60, -120, 120, opt);
      CreateH1Efficiency(name, "RingYc", "|Ring Yc| [cm]", 25, 100, 200, opt);
    }
    // PID
    opt += "_pid";
    CreateH1Efficiency(FindAndReplace(name, "hte_", "hpe_"), "p", "P [GeV/c]", fPRangeBins, fPRangeMin, fPRangeMax,
                       opt);
    //      CreateH1Efficiency(name, "y", "Rapidity", fYRangeBins, fYRangeMin, fYRangeMax, opt);
    //      CreateH1Efficiency(name, "pt", "P_{t} [GeV/c]", fPtRangeBins, fPtRangeMin, fPtRangeMax, opt);
    //      CreateH2Efficiency(name, "YPt", "Rapidity", "P_{t} [GeV/c]", fYRangeBins, fYRangeMin, fYRangeMax, fPtRangeBins, fPtRangeMin, fPtRangeMax, opt);
  }


  // Create histograms for pion suppression calculation
  vector<string> psVariants = PionSuppressionVariants();
  for (UInt_t iHist = 0; iHist < psVariants.size(); iHist++) {
    string name = "hps_" + psVariants[iHist];
    CreateH1PionSuppression(name, "p", "P [GeV/c]", fPRangeBins, fPRangeMin, fPRangeMax);
    CreateH1PionSuppression(name, "p", "P [GeV/c]", fPRangeBins, fPRangeMin, fPRangeMax);
    CreateH1PionSuppression(name, "p", "P [GeV/c]", fPRangeBins, fPRangeMin, fPRangeMax);
  }

  //   // Create efficiency histograms with normalization to INPUT
  //   vector<TH1*> effHistos = fHM->H1Vector("hte_+*_Eff_+*");
  //   Int_t nofEffHistos = effHistos.size();
  //   set<string> effNamesSet;
  //   vector<string> effNormNames;
  //   for (Int_t iHist = 0; iHist < nofEffHistos; iHist++) {
  //      TH1* hist = effHistos[iHist];
  //      vector<string> split = Split(hist->GetName(), '_');
  //      string effName = split[1];
  //      string normName = split[2];
  //      if (effNamesSet.find(effName) == effNamesSet.end()) {
  //         effNamesSet.insert(effName);
  //         effNormNames.push_back(effName + "_" + normName);
  //      }
  //   }
  //   for (Int_t i = 0; i < effNormNames.size(); i++) {
  //      std::cout << i << " " << effNormNames[i] << std::endl;
  //   }
  //   Int_t nofEffNormNames = effNormNames.size();
  //   for (Int_t iHist = 0; iHist < effNormNames.size(); iHist++) {
  //
  //   }
  //   //vector<string> effNormNames(effNormNamesSet.begin(), effNormNames.end());
  //   //

  // Create histograms for ghost tracks
  if (fDet.GetDet(ECbmModuleId::kSts))
    CreateH1("hng_NofGhosts_Sts_Nh", "Number of hits", "Yield", nofBinsPoints, minNofPoints, maxNofPoints);
  if (fDet.GetDet(ECbmModuleId::kTrd))
    CreateH1("hng_NofGhosts_Trd_Nh", "Number of hits", "Yield", nofBinsPoints, minNofPoints, maxNofPoints);
  if (fDet.GetDet(ECbmModuleId::kMuch))
    CreateH1("hng_NofGhosts_Much_Nh", "Number of hits", "Yield", nofBinsPoints, minNofPoints, maxNofPoints);
  if (fDet.GetDet(ECbmModuleId::kRich)) {
    CreateH1("hng_NofGhosts_Rich_Nh", "Number of hits", "Yield", nofBinsPoints, minNofPoints, maxNofPoints);
    CreateH2("hng_NofGhosts_Rich_RingXcYc", "X [cm]", "Y [cm]", "Ghosts per event", 28, -110., 110, 40, -200, 200);
    CreateH1("hng_NofGhosts_RichStsMatching_Nh", "Number of hits", "Yield", nofBinsPoints, minNofPoints, maxNofPoints);
    CreateH1("hng_NofGhosts_RichElId_Nh", "Number of hits", "Yield", nofBinsPoints, minNofPoints, maxNofPoints);
    CreateH1("hng_NofGhosts_StsRichMatching_Nh", "Number of hits", "Yield", nofBinsPoints, minNofPoints, maxNofPoints);
  }

  // Create track hits histograms
  if (fDet.GetDet(ECbmModuleId::kMvd)) CreateTrackHitsHistogram("Mvd");
  if (fDet.GetDet(ECbmModuleId::kSts)) CreateTrackHitsHistogram("Sts");
  if (fDet.GetDet(ECbmModuleId::kTrd)) CreateTrackHitsHistogram("Trd");
  if (fDet.GetDet(ECbmModuleId::kMuch)) CreateTrackHitsHistogram("Much");
  if (fDet.GetDet(ECbmModuleId::kRich)) CreateTrackHitsHistogram("Rich");


  // Create number of object histograms
  Int_t nofBinsC = 100000;
  Double_t maxXC = 100000.;
  CreateH1("hno_NofObjects_GlobalTracks", "Tracks per event", "Yield", nofBinsC, 1., maxXC);
  if (fDet.GetDet(ECbmModuleId::kSts))
    CreateH1("hno_NofObjects_StsTracks", "Tracks per event", "Yield", nofBinsC, 1., maxXC);
  if (fDet.GetDet(ECbmModuleId::kTrd))
    CreateH1("hno_NofObjects_TrdTracks", "Tracks per event", "Yield", nofBinsC, 1., maxXC);
  if (fDet.GetDet(ECbmModuleId::kMuch))
    CreateH1("hno_NofObjects_MuchTracks", "Tracks per event", "Yield", nofBinsC, 1., maxXC);
  if (fDet.GetDet(ECbmModuleId::kRich)) {
    CreateH1("hno_NofObjects_RichRings", "Rings per event", "Yield", nofBinsC, 1., maxXC);
    CreateH1("hno_NofObjects_RichProjections", "Projections per event", "Yield", nofBinsC, 1., maxXC);
  }

  // Histogram stores number of events
  CreateH1("hen_EventNo_TrackingQa", "", "", 1, 0, 1.);

  cout << fHM->ToString();
}


void CbmLitTrackingQa::ProcessGlobalTracks()
{
  // Clear all maps for MC to reco matching
  for (auto it = fMcToRecoMap.begin(); it != fMcToRecoMap.end(); it++) {
    multimap<pair<Int_t, Int_t>, Int_t>& mcRecoMap = (*it).second;
    mcRecoMap.clear();
    //(*it).second.clear();
  }

  ProcessRichRings();

  if (fGlobalTracks == nullptr) return;

  Int_t nofGlobalTracks = fGlobalTracks->GetEntriesFast();
  for (Int_t iTrack = 0; iTrack < nofGlobalTracks; iTrack++) {
    const CbmGlobalTrack* globalTrack = static_cast<const CbmGlobalTrack*>(fGlobalTracks->At(iTrack));

    // get track segments indices
    Int_t stsId  = globalTrack->GetStsTrackIndex();
    Int_t trdId  = globalTrack->GetTrdTrackIndex();
    Int_t muchId = globalTrack->GetMuchTrackIndex();
    Int_t tofId  = globalTrack->GetTofHitIndex();
    Int_t richId = globalTrack->GetRichRingIndex();

    // check track segments
    Bool_t isStsOk  = stsId > -1 && fDet.GetDet(ECbmModuleId::kSts);
    Bool_t isTrdOk  = trdId > -1 && fDet.GetDet(ECbmModuleId::kTrd);
    Bool_t isMuchOk = muchId > -1 && fDet.GetDet(ECbmModuleId::kMuch);
    Bool_t isTofOk  = tofId > -1 && fDet.GetDet(ECbmModuleId::kTof);
    Bool_t isRichOk = richId > -1 && fDet.GetDet(ECbmModuleId::kRich);

    Double_t rtDistance = 10;

    if (isRichOk) rtDistance = CbmRichUtil::GetRingTrackDistance(iTrack);

    // check the quality of track segments
    const CbmTrackMatchNew* stsTrackMatch = nullptr;
    if (isStsOk) {
      stsTrackMatch = static_cast<const CbmTrackMatchNew*>(fStsMatches->At(stsId));
      isStsOk       = stsTrackMatch->GetTrueOverAllHitsRatio() >= fQuota;
      FillTrackQualityHistograms(stsTrackMatch, ECbmModuleId::kSts);
      if (!isStsOk) {  // ghost track
        Int_t nofHits = stsTrackMatch->GetNofHits();
        fHM->H1("hng_NofGhosts_Sts_Nh")->Fill(nofHits);

        // calculate number of ghost after RICH matching
        if (isRichOk) {
          const CbmRichRing* ring = static_cast<const CbmRichRing*>(fRichRings->At(richId));
          if (NULL != ring) {
            if (CbmRichUtil::GetRingTrackDistance(iTrack) < 1.)
              fHM->H1("hng_NofGhosts_StsRichMatching_Nh")->Fill(nofHits);
          }
        }
      }
      else {
        ProcessMvd(stsId);
      }
    }
    const CbmTrackMatchNew* trdTrackMatch = nullptr;
    if (isTrdOk) {
      trdTrackMatch = static_cast<const CbmTrackMatchNew*>(fTrdMatches->At(trdId));
      Int_t nofHits = trdTrackMatch->GetNofHits();
      if (nofHits >= fMinNofHitsTrd) {
        isTrdOk = trdTrackMatch->GetTrueOverAllHitsRatio() >= fQuota;
        FillTrackQualityHistograms(trdTrackMatch, ECbmModuleId::kTrd);
        if (!isTrdOk) {  // ghost track
          fHM->H1("hng_NofGhosts_Trd_Nh")->Fill(nofHits);
        }
      }
      else {
        isTrdOk = false;
      }
    }
    const CbmTrackMatchNew* muchTrackMatch;
    if (isMuchOk) {
      muchTrackMatch = static_cast<const CbmTrackMatchNew*>(fMuchMatches->At(muchId));
      Int_t nofHits  = muchTrackMatch->GetNofHits();
      if (nofHits >= fMinNofHitsMuch) {
        isMuchOk = muchTrackMatch->GetTrueOverAllHitsRatio() >= fQuota;
        FillTrackQualityHistograms(muchTrackMatch, ECbmModuleId::kMuch);
        if (!isMuchOk) {  // ghost track
          fHM->H1("hng_NofGhosts_Much_Nh")->Fill(nofHits);
        }
      }
      else {
        isMuchOk = false;
      }
    }
    const CbmTrackMatchNew* richRingMatch = nullptr;
    if (isRichOk) {
      richRingMatch = static_cast<const CbmTrackMatchNew*>(fRichRingMatches->At(richId));
      Int_t nofHits = richRingMatch->GetNofHits();
      isRichOk      = richRingMatch->GetTrueOverAllHitsRatio() >= fQuotaRich;
      FillTrackQualityHistograms(richRingMatch, ECbmModuleId::kRich);
      if (!isRichOk) {  // ghost ring
        fHM->H1("hng_NofGhosts_Rich_Nh")->Fill(nofHits);

        const CbmRichRing* ring = static_cast<const CbmRichRing*>(fRichRings->At(richId));
        if (NULL != ring) {
          // ghost ring distribution vs position on photodetector plane
          fHM->H1("hng_NofGhosts_Rich_RingXcYc")->Fill(ring->GetCenterX(), ring->GetCenterY());

          // calculate number of ghost after STS matching and electron identification
          if (rtDistance < 1.) fHM->H1("hng_NofGhosts_RichStsMatching_Nh")->Fill(nofHits);

          Double_t momentumMc = 0.;
          if (stsTrackMatch != NULL) {
            const CbmMCTrack* mcTrack = static_cast<const CbmMCTrack*>(fMCTracks->Get(stsTrackMatch->GetMatchedLink()));
            if (mcTrack != NULL) momentumMc = mcTrack->GetP();
          }
          if (rtDistance < 1. && CbmLitGlobalElectronId::GetInstance().IsRichElectron(iTrack, momentumMc)) {
            fHM->H1("hng_NofGhosts_RichElId_Nh")->Fill(nofHits);
          }
        }
      }
    }

    // Get MC indices of track segments
    pair<Int_t, Int_t> stsMCId = {-1, -1}, trdMCId = {-1, -1}, muchMCId = {-1, -1}, richMCId = {-1, -1};
    list<pair<Int_t, Int_t>> tofMCIds;
    if (isStsOk) { stsMCId = {stsTrackMatch->GetMatchedLink().GetEntry(), stsTrackMatch->GetMatchedLink().GetIndex()}; }
    if (isTrdOk) { trdMCId = {trdTrackMatch->GetMatchedLink().GetEntry(), trdTrackMatch->GetMatchedLink().GetIndex()}; }
    if (isMuchOk) {
      muchMCId = {muchTrackMatch->GetMatchedLink().GetEntry(), muchTrackMatch->GetMatchedLink().GetIndex()};
    }
    if (isTofOk) {
      const CbmMatch* tofMatch          = static_cast<const CbmMatch*>(fTofMatches->At(tofId));
      const vector<CbmLink>& tofMCLinks = tofMatch->GetLinks();

      for (vector<CbmLink>::const_iterator tofMCIt = tofMCLinks.begin(); tofMCIt != tofMCLinks.end(); ++tofMCIt) {
        const FairMCPoint* tofPoint = static_cast<const FairMCPoint*>(fTofPoints->Get(*tofMCIt));

        if (tofPoint != NULL) tofMCIds.push_back(pair<Int_t, Int_t>(tofMCIt->GetEntry(), tofPoint->GetTrackID()));
      }
    }
    if (isRichOk) {
      richMCId = {richRingMatch->GetMatchedLink().GetEntry(), richRingMatch->GetMatchedLink().GetIndex()};
    }

    for (auto it = fMcToRecoMap.begin(); it != fMcToRecoMap.end(); it++) {
      string name                                    = (*it).first;
      multimap<pair<Int_t, Int_t>, Int_t>& mcRecoMap = (*it).second;
      Bool_t sts  = (name.find("Sts") != string::npos) ? isStsOk && stsMCId.second != -1 : true;
      Bool_t trd  = (name.find("Trd") != string::npos) ? isTrdOk && stsMCId == trdMCId : true;
      Bool_t much = (name.find("Much") != string::npos) ? isMuchOk && stsMCId == muchMCId : true;
      Bool_t tof  = (name.find("Tof") != string::npos)
                      ? isTofOk && find(tofMCIds.begin(), tofMCIds.end(), stsMCId) != tofMCIds.end()
                      : true;
      Bool_t rich = (name.find("Rich") != string::npos) ? isRichOk && stsMCId == richMCId : true;

      if (sts && trd && much && tof && rich) {
        pair<pair<Int_t, Int_t>, Int_t> tmp = make_pair(stsMCId, iTrack);
        mcRecoMap.insert(tmp);
      }
    }
  }
}

void CbmLitTrackingQa::ProcessRichRings()
{
  if (!fDet.GetDet(ECbmModuleId::kRich)) return;
  Int_t nofRings = fRichRings->GetEntriesFast();
  for (Int_t iRing = 0; iRing < nofRings; iRing++) {
    const CbmTrackMatchNew* richRingMatch = static_cast<const CbmTrackMatchNew*>(fRichRingMatches->At(iRing));
    Bool_t isRichOk                       = richRingMatch->GetTrueOverAllHitsRatio() >= fQuotaRich;
    pair<Int_t, Int_t> richMCId           = {isRichOk ? richRingMatch->GetMatchedLink().GetEntry() : -1,
                                   isRichOk ? richRingMatch->GetMatchedLink().GetIndex() : -1};  //GetMCTrackId() : -1;
    if (isRichOk && -1 != richMCId.second) {
      pair<pair<Int_t, Int_t>, Int_t> tmp = make_pair(richMCId, iRing);
      fMcToRecoMap["Rich"].insert(tmp);
    }
  }
}

void CbmLitTrackingQa::ProcessMvd(Int_t stsId)
{
  if (!fDet.GetDet(ECbmModuleId::kMvd)) return;
  const CbmStsTrack* track = static_cast<const CbmStsTrack*>(fStsTracks->At(stsId));
  Int_t nofHits            = track->GetNofMvdHits();
  fHM->H1("hth_Mvd_TrackHits_All")->Fill(nofHits);

  const CbmTrackMatchNew* stsTrackMatch = static_cast<const CbmTrackMatchNew*>(fStsMatches->At(stsId));
  Int_t stsMcTrackId                    = stsTrackMatch->GetMatchedLink().GetIndex();

  Int_t nofTrueHits = 0, nofFakeHits = 0;
  for (Int_t iHit = 0; iHit < nofHits; iHit++) {
    Int_t hitId              = track->GetMvdHitIndex(iHit);
    const CbmMatch* hitMatch = static_cast<const CbmMatch*>(fMvdHitMatches->At(hitId));
    if (NULL == hitMatch) continue;
    Int_t pointId            = hitMatch->GetMatchedLink().GetIndex();
    Int_t eventId            = hitMatch->GetMatchedLink().GetEntry();
    const FairMCPoint* point = static_cast<const FairMCPoint*>(fMvdPoints->Get(0, eventId, pointId));
    if (NULL == point) continue;
    Int_t mcTrackId = point->GetTrackID();
    if (mcTrackId == stsMcTrackId) {  // true hit
      nofTrueHits++;
    }
    else {  // fake hit
      nofFakeHits++;
    }
  }
  fHM->H1("hth_Mvd_TrackHits_True")->Fill(nofTrueHits);
  fHM->H1("hth_Mvd_TrackHits_Fake")->Fill(nofFakeHits);
  if (nofHits != 0) {
    fHM->H1("hth_Mvd_TrackHits_TrueOverAll")->Fill(Double_t(nofTrueHits) / Double_t(nofHits));
    fHM->H1("hth_Mvd_TrackHits_FakeOverAll")->Fill(Double_t(nofFakeHits) / Double_t(nofHits));
  }
}

void CbmLitTrackingQa::FillTrackQualityHistograms(const CbmTrackMatchNew* trackMatch, ECbmModuleId detId)
{
  string detName = (detId == ECbmModuleId::kSts)    ? "Sts"
                   : (detId == ECbmModuleId::kTrd)  ? "Trd"
                   : (detId == ECbmModuleId::kMuch) ? "Much"
                   : (detId == ECbmModuleId::kRich) ? "Rich"
                                                    : "";
  assert(detName != "");
  string histName = "hth_" + detName + "_TrackHits";
  fHM->H1(histName + "_All")->Fill(trackMatch->GetNofHits());
  fHM->H1(histName + "_True")->Fill(trackMatch->GetNofTrueHits());
  fHM->H1(histName + "_Fake")->Fill(trackMatch->GetNofWrongHits());
  fHM->H1(histName + "_TrueOverAll")->Fill(trackMatch->GetTrueOverAllHitsRatio());
  fHM->H1(histName + "_FakeOverAll")->Fill(trackMatch->GetWrongOverAllHitsRatio());
}

void CbmLitTrackingQa::ProcessMcTracks(Int_t iEvent)
{
  vector<TH1*> effHistos = fHM->H1Vector("(hte|hpe)_.*_Eff_.*");
  Int_t nofEffHistos     = effHistos.size();

  Int_t nofMcTracks = fMCTracks->Size(0, iEvent);
  //Int_t nofExistsMcTracks = 0;
  for (Int_t iMCTrack = 0; iMCTrack < nofMcTracks; ++iMCTrack) {
    const CbmMCTrack* mcTrack = static_cast<const CbmMCTrack*>(fMCTracks->Get(0, iEvent, iMCTrack));

    // Check accepted tracks cutting on minimal number of MC points
    if (!fMCTrackCreator->TrackExists(iEvent, iMCTrack)) { continue; }

    const CbmLitMCTrack& litMCTrack = fMCTrackCreator->GetTrack(iEvent, iMCTrack);
    Int_t nofPointsSts              = litMCTrack.GetNofPointsInDifferentStations(ECbmModuleId::kSts);
    Int_t nofPointsTrd              = litMCTrack.GetNofPointsInDifferentStations(ECbmModuleId::kTrd);
    Int_t nofPointsMuch             = litMCTrack.GetNofPointsInDifferentStations(ECbmModuleId::kMuch);
    Int_t nofPointsTof              = litMCTrack.GetNofPoints(ECbmModuleId::kTof);
    Int_t nofHitsRich               = litMCTrack.GetNofRichHits();
    Double_t boa                    = litMCTrack.GetRingBaxis() / litMCTrack.GetRingAaxis();
    if (litMCTrack.GetRingBaxis() == -1. || litMCTrack.GetRingAaxis() == -1) boa = -1.;
    Double_t ringX = litMCTrack.GetRingCenterX();
    Double_t ringY = litMCTrack.GetRingCenterY();


    // Check local tracks
    Bool_t stsConsecutive =
      (fUseConsecutivePointsInSts) ? litMCTrack.GetNofConsecutivePoints(ECbmModuleId::kSts) >= fMinNofPointsSts : true;
    Bool_t isStsOk  = nofPointsSts >= fMinNofPointsSts && fDet.GetDet(ECbmModuleId::kSts) && stsConsecutive;
    Bool_t isTrdOk  = nofPointsTrd >= fMinNofPointsTrd && fDet.GetDet(ECbmModuleId::kTrd);
    Bool_t isMuchOk = nofPointsMuch >= fMinNofPointsMuch && fDet.GetDet(ECbmModuleId::kMuch);
    Bool_t isTofOk  = nofPointsTof >= fMinNofPointsTof && fDet.GetDet(ECbmModuleId::kTof);
    Bool_t isRichOk = nofHitsRich >= fMinNofHitsRich && fDet.GetDet(ECbmModuleId::kRich);

    // calculate polar angle
    TVector3 mom;
    mcTrack->GetMomentum(mom);
    Double_t angle = std::abs(mom.Theta() * 180 / TMath::Pi());
    Double_t mcP   = mcTrack->GetP();
    Double_t mcY   = mcTrack->GetRapidity();
    Double_t mcPt  = mcTrack->GetPt();

    // Fill parameter name to value map
    map<string, vector<Double_t>> parMap;

    vector<Double_t> tmp = list_of(mcP);
    parMap["p"]          = tmp;

    vector<Double_t> tmp1 = list_of(mcY);
    parMap["y"]           = tmp1;

    vector<Double_t> tmp2 = list_of(mcPt);
    parMap["pt"]          = tmp2;

    vector<Double_t> tmp3 = list_of(angle);
    parMap["Angle"]       = tmp3;

    vector<Double_t> tmp4 = list_of(0);
    parMap["Np"]          = tmp4;  // FIXME : correct to number of points in concrete detector!
    // Currently as a  temporary solution it is reassigned later

    vector<Double_t> tmp5 = list_of(boa);
    parMap["BoA"]         = tmp5;

    vector<Double_t> tmp5X = list_of(ringX);
    parMap["RingXc"]       = tmp5X;

    vector<Double_t> tmp5Y = list_of(std::abs(ringY));
    parMap["RingYc"]       = tmp5Y;

    vector<Double_t> tmp6 = list_of(ringX)(ringY);
    parMap["RingXcYc"]    = tmp6;

    vector<Double_t> tmp7 = list_of(nofHitsRich);
    parMap["RingNh"]      = tmp7;

    vector<Double_t> tmp8 = list_of(mcY)(mcPt);
    //parMap["RadPos"] = list_of(1);
    parMap["YPt"] = tmp8;

    for (Int_t iHist = 0; iHist < nofEffHistos; iHist++) {
      TH1* hist            = effHistos[iHist];
      string histName      = hist->GetName();
      vector<string> split = Split(histName, '_');
      string effName       = split[1];
      string normName      = split[2];
      string catName       = split[3];
      string histTypeName  = split[0];
      string parName       = split[5];
      assert(parMap.count(parName) != 0);

      vector<Double_t> par = list_of(0);
      if (parName == "Np") {
        vector<Double_t> tmp9 = list_of((effName == "Sts")    ? nofPointsSts
                                        : (effName == "Trd")  ? nofPointsTrd
                                        : (effName == "Much") ? nofPointsMuch
                                        : (effName == "Tof")  ? nofPointsTof
                                                              : 0);
        par                   = tmp9;
      }
      else {
        par = parMap[parName];
      }

      Bool_t sts  = (normName.find("Sts") != string::npos) ? isStsOk : true;
      Bool_t trd  = (normName.find("Trd") != string::npos) ? isTrdOk : true;
      Bool_t much = (normName.find("Much") != string::npos) ? isMuchOk : true;
      Bool_t tof  = (normName.find("Tof") != string::npos) ? isTofOk : true;
      Bool_t rich = (normName.find("Rich") != string::npos) ? isRichOk : true;

      if (effName == "Trd" || effName == "Much" || effName == "Tof") {
        string prevRecName = FindAndReplace(normName, effName, "");
        Bool_t isPrevRec =
          fMcToRecoMap[prevRecName].find(make_pair(iEvent, iMCTrack)) != fMcToRecoMap[prevRecName].end();
        Bool_t accOk = isPrevRec && sts && trd && much && tof && rich;
        if (accOk) {
          FillGlobalReconstructionHistos(iEvent, iMCTrack, fMcToRecoMap[normName], histName, histTypeName, effName,
                                         catName, par);
        }
      }
      else {
        Bool_t accOk = sts && trd && much && tof && rich;
        if (accOk) {
          if (histName.find("Rich") == string::npos) {
            FillGlobalReconstructionHistos(iEvent, iMCTrack, fMcToRecoMap[effName], histName, histTypeName, effName,
                                           catName, par);
          }
          else {
            FillGlobalReconstructionHistosRich(iEvent, iMCTrack, fMcToRecoMap[effName], histName, histTypeName, effName,
                                               catName, par);
          }
        }
      }
    }  // Loop over efficiency histograms
  }    // Loop over MCTracks
}

void CbmLitTrackingQa::FillGlobalReconstructionHistos(Int_t mcEventId, Int_t mcId,
                                                      const multimap<pair<Int_t, Int_t>, Int_t>& mcMap,
                                                      const string& histName, const string& histTypeName,
                                                      const string& effName, const string& catName,
                                                      const vector<Double_t>& par)
{
  string accHistName                  = FindAndReplace(histName, "_Eff_", "_Acc_");
  string recHistName                  = FindAndReplace(histName, "_Eff_", "_Rec_");
  LitTrackAcceptanceFunction function = fTrackAcceptanceFunctions.find(catName)->second;
  Bool_t accOk                        = function(fMCTracks, mcEventId, mcId);
  Bool_t recOk    = (histTypeName == "hte") ? (mcMap.find(pair<Int_t, Int_t>(mcEventId, mcId)) != mcMap.end() && accOk)
                                            : (ElectronId(mcEventId, mcId, mcMap, effName) && accOk);
  Int_t nofParams = par.size();
  assert(nofParams < 3 && nofParams > 0);
  if (nofParams == 1) {
    if (accOk) { fHM->H1(accHistName)->Fill(par[0]); }
    if (recOk) { fHM->H1(recHistName)->Fill(par[0]); }
  }
  else if (nofParams == 2) {
    if (accOk) { fHM->H1(accHistName)->Fill(par[0], par[1]); }
    if (recOk) { fHM->H1(recHistName)->Fill(par[0], par[1]); }
  }
}

void CbmLitTrackingQa::FillGlobalReconstructionHistosRich(Int_t mcEventId, Int_t mcId,
                                                          const multimap<pair<Int_t, Int_t>, Int_t>& mcMap,
                                                          const string& histName, const string& histTypeName,
                                                          const string& effName, const string& catName,
                                                          const vector<Double_t>& par)
{
  Int_t nofHitsInRing                = fMCTrackCreator->GetTrack(mcEventId, mcId).GetNofRichHits();
  string accHistName                 = FindAndReplace(histName, "_Eff_", "_Acc_");
  string recHistName                 = FindAndReplace(histName, "_Eff_", "_Rec_");
  LitRingAcceptanceFunction function = fRingAcceptanceFunctions.find(catName)->second;
  Bool_t accOk                       = function(fMCTracks, mcEventId, mcId, nofHitsInRing);
  Bool_t recOk    = (histTypeName == "hte") ? (mcMap.find(make_pair(mcEventId, mcId)) != mcMap.end() && accOk)
                                            : (ElectronId(mcEventId, mcId, mcMap, effName) && accOk);
  Int_t nofParams = par.size();
  assert(nofParams < 3 && nofParams > 0);
  if (nofParams == 1) {
    if (accOk) { fHM->H1(accHistName)->Fill(par[0]); }
    if (recOk) { fHM->H1(recHistName)->Fill(par[0]); }
  }
  else if (nofParams == 2) {
    if (accOk) { fHM->H1(accHistName)->Fill(par[0], par[1]); }
    if (recOk) { fHM->H1(recHistName)->Fill(par[0], par[1]); }
  }
}

Bool_t CbmLitTrackingQa::ElectronId(Int_t mcEventId, Int_t mcId, const multimap<pair<Int_t, Int_t>, Int_t>& mcMap,
                                    const string& effName)
{
  multimap<pair<Int_t, Int_t>, Int_t>::const_iterator it = mcMap.find(pair<Int_t, Int_t>(mcEventId, mcId));
  if (it == mcMap.end()) return false;
  Int_t globalTrackIndex    = (*it).second;
  const CbmMCTrack* mcTrack = static_cast<const CbmMCTrack*>(fMCTracks->Get(0, mcEventId, mcId));
  TVector3 mom;
  mcTrack->GetMomentum(mom);
  Bool_t isRichElectron = (fDet.GetDet(ECbmModuleId::kRich) && (effName.find("Rich") != string::npos))
                            ? CbmLitGlobalElectronId::GetInstance().IsRichElectron(globalTrackIndex, mom.Mag())
                            : true;
  Bool_t isTrdElectron  = (fDet.GetDet(ECbmModuleId::kTrd) && (effName.find("Trd") != string::npos))
                            ? CbmLitGlobalElectronId::GetInstance().IsTrdElectron(globalTrackIndex, mom.Mag())
                            : true;
  Bool_t isTofElectron  = (fDet.GetDet(ECbmModuleId::kTof) && (effName.find("Tof") != string::npos))
                            ? CbmLitGlobalElectronId::GetInstance().IsTofElectron(globalTrackIndex, mom.Mag())
                            : true;
  return isRichElectron && isTrdElectron && isTofElectron;
}

void CbmLitTrackingQa::PionSuppression()
{
  if (!(fGlobalTracks && fStsMatches && fRichProjections && fMCTracks)) return;
  vector<TH1*> histos = fHM->H1Vector("hps_.*_RecPions_.*");
  Int_t nofHistos     = histos.size();

  Int_t nofGlobalTracks = fGlobalTracks->GetEntriesFast();
  for (Int_t iGT = 0; iGT < nofGlobalTracks; iGT++) {
    const CbmGlobalTrack* globalTrack = static_cast<const CbmGlobalTrack*>(fGlobalTracks->At(iGT));
    Int_t stsIndex                    = globalTrack->GetStsTrackIndex();
    if (stsIndex < 0) continue;
    const CbmTrackMatchNew* trackMatch   = static_cast<const CbmTrackMatchNew*>(fStsMatches->At(stsIndex));
    const FairTrackParam* richProjection = static_cast<const FairTrackParam*>(fRichProjections->At(iGT));
    if (richProjection == NULL || richProjection->GetX() == 0 || richProjection->GetY() == 0) continue;
    Int_t trdIndex = globalTrack->GetTrdTrackIndex();
    if (fDet.GetDet(ECbmModuleId::kTrd) && trdIndex < 0) continue;
    Int_t tofIndex = globalTrack->GetTofHitIndex();
    if (fDet.GetDet(ECbmModuleId::kTof) && tofIndex < 0) continue;

    if (trackMatch->GetNofLinks() <= 0) continue;
    Int_t mcIdSts      = trackMatch->GetMatchedLink().GetIndex();
    Int_t mcIdStsEvent = trackMatch->GetMatchedLink().GetEntry();
    if (mcIdSts < 0) continue;
    const CbmMCTrack* mcTrack = static_cast<const CbmMCTrack*>(fMCTracks->Get(0, mcIdStsEvent, mcIdSts));
    TVector3 mom;
    mcTrack->GetMomentum(mom);
    Double_t p   = mom.Mag();
    Int_t pdgSts = mcTrack->GetPdgCode();
    if (std::abs(pdgSts) == 211) {
      Bool_t isRichElectron =
        (fDet.GetDet(ECbmModuleId::kRich)) ? CbmLitGlobalElectronId::GetInstance().IsRichElectron(iGT, p) : true;
      Bool_t isTrdElectron =
        (fDet.GetDet(ECbmModuleId::kTrd)) ? CbmLitGlobalElectronId::GetInstance().IsTrdElectron(iGT, p) : true;
      Bool_t isTofElectron =
        (fDet.GetDet(ECbmModuleId::kTof)) ? CbmLitGlobalElectronId::GetInstance().IsTofElectron(iGT, p) : true;

      for (Int_t iHist = 0; iHist < nofHistos; iHist++) {
        histos[iHist]->Fill(p);  // Fill RecPions histogramm
        string name          = histos[iHist]->GetName();
        vector<string> split = Split(name, '_');
        string effName       = split[1];
        string category      = split[2];
        //cout << category << endl;
        Bool_t isElectron = ((effName.find("Rich") != string::npos) ? isRichElectron : true)
                            && ((effName.find("Trd") != string::npos) ? isTrdElectron : true)
                            && ((effName.find("Tof") != string::npos) ? isTofElectron : true);
        if (isElectron) {

          LitPiSuppAcceptanceFunction function = fPiSuppAcceptanceFunctions.find(category)->second;
          Bool_t ok                            = function(fGlobalTracks, fStsMatches, fRichRingMatches, iGT);

          if (ok) fHM->H1(FindAndReplace(name, "RecPions", "Rec"))->Fill(p);
        }
      }
    }
  }
}

void CbmLitTrackingQa::IncreaseCounters()
{
  if (fGlobalTracks != nullptr) fHM->H1("hno_NofObjects_GlobalTracks")->Fill(fGlobalTracks->GetEntriesFast());
  if (fDet.GetDet(ECbmModuleId::kSts)) { fHM->H1("hno_NofObjects_StsTracks")->Fill(fStsTracks->GetEntriesFast()); }
  if (fDet.GetDet(ECbmModuleId::kRich)) {
    fHM->H1("hno_NofObjects_RichRings")->Fill(fRichRings->GetEntriesFast());
    Int_t projCount = 0;
    Int_t nProj     = fRichProjections->GetEntriesFast();
    for (Int_t iProj = 0; iProj < nProj; iProj++) {
      FairTrackParam* proj = (FairTrackParam*) fRichProjections->At(iProj);
      if (NULL == proj || proj->GetX() == 0. || proj->GetY() == 0) continue;
      projCount++;
    }
    fHM->H1("hno_NofObjects_RichProjections")->Fill(projCount);
  }
  if (fDet.GetDet(ECbmModuleId::kTrd)) { fHM->H1("hno_NofObjects_TrdTracks")->Fill(fTrdMatches->GetEntriesFast()); }
  if (fDet.GetDet(ECbmModuleId::kMuch)) { fHM->H1("hno_NofObjects_MuchTracks")->Fill(fMuchMatches->GetEntriesFast()); }
}

ClassImp(CbmLitTrackingQa);
