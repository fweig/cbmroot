/**
 * \file CbmLitTofQa.cxx
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2011
 */
#include "CbmLitTofQa.h"

#include "CbmGlobalTrack.h"
#include "CbmHistManager.h"
#include "CbmKFParticleInterface.h"
#include "CbmKFVertex.h"
#include "CbmLitTofQaReport.h"
#include "CbmMCDataManager.h"
#include "CbmMCTrack.h"
#include "CbmStsTrack.h"
#include "CbmTofHit.h"
#include "CbmTofPoint.h"
#include "CbmTofTrack.h"
#include "CbmTrackMatchNew.h"
#include "CbmVertex.h"
#include "qa/base/CbmLitAcceptanceFunction.h"

#include "TClonesArray.h"
#include "TDatabasePDG.h"
#include "TH2F.h"
#include "TParticlePDG.h"
#include <TFile.h>

#include <boost/assign/list_of.hpp>

#include <utility>
#include <vector>

#include <cmath>

using boost::assign::list_of;
using std::make_pair;
using std::min;
using std::pair;
using std::sqrt;
using std::vector;

CbmLitTofQa::CbmLitTofQa()
  : fIsFixedBounds(true)
  , fOutputDir("./test/")
  , fHM(NULL)
  , fPRangeMin(0.)
  , fPRangeMax(15.)
  , fPRangeBins(400)
  , fGlobalTracks(NULL)
  , fStsTracks(NULL)
  , fStsTrackMatches(NULL)
  , fTofHits(NULL)
  , fTofPoints(NULL)
  , fTofHitsMatches(NULL)
  , fTofTracks(NULL)
  , fMCTracks(NULL)
  , fPrimVertex(NULL)
  , fKFFitter()
  , fTrackCategories()
  , fTrackAcceptanceFunctions()
  , fMCTrackIdForTofHits()
  , fMCTrackIdForTofPoints()
{
  FillTrackCategoriesAndAcceptanceFunctions();
}

CbmLitTofQa::~CbmLitTofQa()
{
  if (fHM) delete fHM;
}

InitStatus CbmLitTofQa::Init()
{
  fHM = new CbmHistManager();
  CreateHistograms();
  ReadDataBranches();
  fKFFitter.Init();
  return kSUCCESS;
}

void CbmLitTofQa::Exec(Option_t* opt)
{
  static Int_t nofEvents = 0;
  nofEvents++;
  std::cout << "CbmLitTofQa::Exec: event=" << nofEvents << std::endl;
  ProcessMC(nofEvents - 1);
  ProcessGlobalTracks();
  ProcessTofHits();
  ProcessTofTracks();
}

void CbmLitTofQa::Finish()
{
  FitHistograms();

  TDirectory* oldir = gDirectory;
  TFile* outFile    = FairRootManager::Instance()->GetOutFile();
  if (outFile != NULL) {
    outFile->cd();
    fHM->WriteToFile();
  }
  gDirectory->cd(oldir->GetPath());

  CbmSimulationReport* report = new CbmLitTofQaReport();
  report->Create(fHM, fOutputDir);
  delete report;
}

void CbmLitTofQa::ReadDataBranches()
{
  FairRootManager* ioman = FairRootManager::Instance();
  assert(ioman != NULL);

  CbmMCDataManager* mcManager = (CbmMCDataManager*) ioman->GetObject("MCDataManager");

  fGlobalTracks    = (TClonesArray*) ioman->GetObject("GlobalTrack");
  fStsTracks       = (TClonesArray*) ioman->GetObject("StsTrack");
  fStsTrackMatches = (TClonesArray*) ioman->GetObject("StsTrackMatch");
  fTofHits         = (TClonesArray*) ioman->GetObject("TofHit");
  fTofHitsMatches  = (TClonesArray*) ioman->GetObject("TofHitMatch");
  fTofPoints       = mcManager->InitBranch("TofPoint");
  fTofTracks       = (TClonesArray*) ioman->GetObject("TofTrack");
  fMCTracks        = mcManager->InitBranch("MCTrack");
  //   fPrimVertex = (CbmVertex*) ioman->GetObject("PrimaryVertex");
  // Get pointer to PrimaryVertex object from IOManager if it exists
  // The old name for the object is "PrimaryVertex" the new one
  // "PrimaryVertex." Check first for the new name
  fPrimVertex = dynamic_cast<CbmVertex*>(ioman->GetObject("PrimaryVertex."));
  if (nullptr == fPrimVertex) { fPrimVertex = dynamic_cast<CbmVertex*>(ioman->GetObject("PrimaryVertex")); }
  if (nullptr == fPrimVertex) {
    //   LOG(fatal) << "No primary vertex";
  }
}

void CbmLitTofQa::FillTrackCategoriesAndAcceptanceFunctions()
{
  vector<string> tmp = list_of("All")("Positive")("Negative")("Primary")("Secondary")("Electron")("Muon")("Proton")(
    "AntiProton")("Pion")("PionPlus")("PionMinus")("Kaon")("KaonPlus")("KaonMinus");
  fTrackCategories = tmp;
  // List of all supported track categories
  fTrackAcceptanceFunctions["All"]        = CbmLitAcceptanceFunction::AllTrackAcceptanceFunction;
  fTrackAcceptanceFunctions["Positive"]   = CbmLitAcceptanceFunction::PositiveTrackAcceptanceFunction;
  fTrackAcceptanceFunctions["Negative"]   = CbmLitAcceptanceFunction::NegativeTrackAcceptanceFunction;
  fTrackAcceptanceFunctions["Primary"]    = CbmLitAcceptanceFunction::PrimaryTrackAcceptanceFunction;
  fTrackAcceptanceFunctions["Secondary"]  = CbmLitAcceptanceFunction::SecondaryTrackAcceptanceFunction;
  fTrackAcceptanceFunctions["Electron"]   = CbmLitAcceptanceFunction::ElectronTrackAcceptanceFunction;
  fTrackAcceptanceFunctions["Muon"]       = CbmLitAcceptanceFunction::MuonTrackAcceptanceFunction;
  fTrackAcceptanceFunctions["Proton"]     = CbmLitAcceptanceFunction::ProtonTrackAcceptanceFunction;
  fTrackAcceptanceFunctions["AntiProton"] = CbmLitAcceptanceFunction::AntiProtonTrackAcceptanceFunction;
  fTrackAcceptanceFunctions["Pion"]       = CbmLitAcceptanceFunction::PionTrackAcceptanceFunction;
  fTrackAcceptanceFunctions["PionPlus"]   = CbmLitAcceptanceFunction::PionPlusTrackAcceptanceFunction;
  fTrackAcceptanceFunctions["PionMinus"]  = CbmLitAcceptanceFunction::PionMinusTrackAcceptanceFunction;
  fTrackAcceptanceFunctions["Kaon"]       = CbmLitAcceptanceFunction::KaonTrackAcceptanceFunction;
  fTrackAcceptanceFunctions["KaonPlus"]   = CbmLitAcceptanceFunction::KaonPlusTrackAcceptanceFunction;
  fTrackAcceptanceFunctions["KaonMinus"]  = CbmLitAcceptanceFunction::KaonMinusTrackAcceptanceFunction;
}

void CbmLitTofQa::CreateHistograms()
{
  Int_t nofTrackCategories = fTrackCategories.size();
  for (Int_t iCat = 0; iCat < nofTrackCategories; iCat++) {
    string name = "hmp_Tof_Reco_" + fTrackCategories[iCat] + "_m2p";
    fHM->Add(name, new TH2F(name.c_str(), string(name + ";P [GeV/c];M^{2} [(GeV/c)^{2}]").c_str(), fPRangeBins,
                            fPRangeMin, fPRangeMax, 400, -0.2, 1.8));
    name = "hmp_Tof_RecoMCID_" + fTrackCategories[iCat] + "_m2p";
    fHM->Add(name, new TH2F(name.c_str(), string(name + ";P [GeV/c];M^{2} [(GeV/c)^{2}]").c_str(), fPRangeBins,
                            fPRangeMin, fPRangeMax, 400, -0.2, 1.8));
    name = "hmp_Tof_RecoAccTof_" + fTrackCategories[iCat] + "_m2p";
    fHM->Add(name, new TH2F(name.c_str(), string(name + ";P [GeV/c];M^{2} [(GeV/c)^{2}]").c_str(), fPRangeBins,
                            fPRangeMin, fPRangeMax, 400, -0.2, 1.8));
    name = "hmp_Tof_RecoMCIDAccTof_" + fTrackCategories[iCat] + "_m2p";
    fHM->Add(name, new TH2F(name.c_str(), string(name + ";P [GeV/c];M^{2} [(GeV/c)^{2}]").c_str(), fPRangeBins,
                            fPRangeMin, fPRangeMax, 400, -0.2, 1.8));

    name = "hmp_TofTrack_" + fTrackCategories[iCat] + "_Distance";
    fHM->Add(name, new TH1F(name.c_str(), string(name + ";Distance [cm]").c_str(), 200, 0., 50.));
    name = "hmp_TofTrack_" + fTrackCategories[iCat] + "_NormDistance";
    fHM->Add(name, new TH1F(name.c_str(), string(name + ";Normalized distance").c_str(), 200, 0., 50.));
    name = "hmp_TofTrack_" + fTrackCategories[iCat] + "_Length";
    fHM->Add(name, new TH1F(name.c_str(), string(name + ";Length [cm]").c_str(), 1200, 0., 1200.));
    name = "hmp_TofTrack_" + fTrackCategories[iCat] + "_NofHitsPerGlobalTrack";
    fHM->Add(name, new TH1F(name.c_str(), string(name + ";number of hits per global track").c_str(), 11, -0.5, 10.5));
  }
  string name = "hmp_Tof_dTime";
  fHM->Add(name, new TH1F(name.c_str(), string(name + ";dt [ps];Counter").c_str(), 1000, -500., 500.));
  name = "hmp_Tof_TimeZero_a";
  fHM->Add(name, new TH1F(name.c_str(), string(name + ";Time [ns];Counter").c_str(), 2000, 0., 36.));
  name = "hmp_Tof_TimeZero_reco";
  fHM->Add(name, new TH1F(name.c_str(), string(name + ";Time [ns];Counter").c_str(), 2000, -5., 15.));
  name = "hmp_Tof_TimeZero_mc";
  fHM->Add(name, new TH1F(name.c_str(), string(name + ";Time [ns];Counter").c_str(), 2000, -5., 15.));
  name = "hmp_Tof_TimeZero_NofTracks";
  fHM->Add(name, new TH1F(name.c_str(), string(name + ";Number of tracks;Counter").c_str(), 100, 0., 100.));
  name = "hmp_Tof_Time_FirstTrack";
  fHM->Add(name, new TH1F(name.c_str(), string(name + ";Time [ns];Counter").c_str(), 2000, 0., 36.));
}

void CbmLitTofQa::ProcessMC(Int_t iEvent)
{
  fMCTrackIdForTofHits.clear();
  fMCTrackIdForTofPoints.clear();

  Int_t nofHits = fTofHits->GetEntriesFast();
  for (Int_t iHit = 0; iHit < nofHits; iHit++) {
    const CbmTofHit* tofHit = static_cast<const CbmTofHit*>(fTofHits->At(iHit));
    CbmMatch* tofHitMatch   = static_cast<CbmMatch*>(fTofHitsMatches->At(iHit));
    if (tofHitMatch == NULL) { continue; }
    Int_t tofPointIndex         = tofHitMatch->GetMatchedLink().GetIndex();
    Int_t tofPointEventNo       = tofHitMatch->GetMatchedLink().GetEntry();
    const CbmTofPoint* tofPoint = static_cast<const CbmTofPoint*>(fTofPoints->Get(0, tofPointEventNo, tofPointIndex));
    fMCTrackIdForTofHits.insert(make_pair(tofPointEventNo, tofPoint->GetTrackID()));
  }

  Int_t nofPoints = fTofPoints->Size(0, iEvent);
  for (Int_t iPoint = 0; iPoint < nofPoints; iPoint++) {
    const CbmTofPoint* tofPoint = static_cast<const CbmTofPoint*>(fTofPoints->Get(0, iEvent, iPoint));
    fMCTrackIdForTofPoints.insert(pair<Int_t, Int_t>(iEvent, tofPoint->GetTrackID()));
  }
}

void CbmLitTofQa::ProcessGlobalTracks()
{
  Double_t timeZeroReco      = 0.0;
  Double_t timeZeroMC        = 0.0;
  Double_t timeFirstTrack    = 100.;  // ns
  Double_t timeZeroA         = 0.;    // ns
  Int_t nofTracksForTimeZero = 0;

  Int_t nofGlobalTracks = fGlobalTracks->GetEntriesFast();
  for (Int_t iTrack = 0; iTrack < nofGlobalTracks; iTrack++) {
    const CbmGlobalTrack* globalTrack = static_cast<const CbmGlobalTrack*>(fGlobalTracks->At(iTrack));
    Int_t stsId                       = globalTrack->GetStsTrackIndex();
    Int_t tofId                       = globalTrack->GetTofHitIndex();
    if (stsId < 0 || tofId < 0) continue;  // We need both STS track and TOF hit

    CbmStsTrack* stsTrack            = static_cast<CbmStsTrack*>(fStsTracks->At(stsId));
    const CbmTrackMatchNew* stsMatch = static_cast<const CbmTrackMatchNew*>(fStsTrackMatches->At(stsId));
    Int_t stsMCTrackId               = stsMatch->GetMatchedLink().GetIndex();

    const CbmTofHit* tofHit = static_cast<const CbmTofHit*>(fTofHits->At(tofId));
    CbmMatch* tofHitMatch   = static_cast<CbmMatch*>(fTofHitsMatches->At(tofId));
    if (tofHitMatch == NULL) { continue; }
    Int_t tofMCPointId          = tofHitMatch->GetMatchedLink().GetIndex();
    Int_t tofMCEventId          = tofHitMatch->GetMatchedLink().GetEntry();
    const CbmTofPoint* tofPoint = static_cast<const CbmTofPoint*>(fTofPoints->Get(0, tofMCEventId, tofMCPointId));
    Int_t tofMCTrackId          = tofPoint->GetTrackID();


    FairTrackParam vtxTrack;
    //       fKFFitter.FitToVertex(stsTrack, fPrimVertex, &vtxTrack);
    //       Double_t chiSqPrimary = fKFFitter.GetChiToVertex(stsTrack, fPrimVertex);
    float chiSqPrimary = 0.f;
    CbmKFParticleInterface::ExtrapolateTrackToPV(stsTrack, fPrimVertex, &vtxTrack, chiSqPrimary);

    Bool_t chiSqPrimaryOk = chiSqPrimary < 3.;

    //      Double_t ctCorrection = -0.007;
    Double_t ctCorrection = 0.0;
    Double_t ctReco       = 0.299792458 * tofHit->GetTime() + ctCorrection;  // ToF time in ns -> transfrom to ct in m
    Double_t ctMC         = 0.299792458 * tofPoint->GetTime();               // mc time in ns -> transfrom to ct in m
    Double_t trackLengthReco = globalTrack->GetLength() / 100.;              //global length
    // Double_t trackLengthMC = tofPoint->GetLength() / 100.; //mc length
    Double_t preco     = (vtxTrack.GetQp() != 0) ? std::abs(1. / vtxTrack.GetQp()) : 0;
    Double_t t         = (trackLengthReco != 0) ? (ctReco / trackLengthReco) : 0;
    Double_t m2reco    = preco * preco * (t * t - 1);
    Double_t radialPos = sqrt(tofHit->GetX() * tofHit->GetX() + tofHit->GetY() * tofHit->GetY());

    if (chiSqPrimaryOk && radialPos < 50.) {
      nofTracksForTimeZero++;
      Double_t beta = 1.;
      if (m2reco > 0.6) beta = preco / sqrt(preco * preco + 0.93827231 * 0.93827231);
      if (m2reco <= 0.6) beta = preco / sqrt(preco * preco + 0.1395679 * 0.1395679);
      timeZeroReco += tofHit->GetTime() - trackLengthReco / (beta * 0.299792458);
      timeZeroMC += tofPoint->GetTime() - trackLengthReco / (beta * 0.299792458);
      timeZeroA += trackLengthReco / 0.299792458;
      timeFirstTrack = std::min(timeFirstTrack, trackLengthReco / 0.299792458);
    }

    Int_t nofTrackCategories = fTrackCategories.size();
    for (Int_t iCat = 0; iCat < nofTrackCategories; iCat++) {
      string category                     = fTrackCategories[iCat];
      LitTrackAcceptanceFunction function = fTrackAcceptanceFunctions.find(category)->second;
      Bool_t categoryOk                   = function(fMCTracks, tofMCEventId, stsMCTrackId);
      //Bool_t accTofOk = fMCTrackIdForTofPoints.find(make_pair(tofMCEventId, stsMCTrackId)) != fMCTrackIdForTofPoints.end();
      Bool_t accTofOk = fMCTrackIdForTofHits.find(make_pair(tofMCEventId, stsMCTrackId)) != fMCTrackIdForTofHits.end();

      if (categoryOk && chiSqPrimaryOk) {
        fHM->H1("hmp_Tof_Reco_" + category + "_m2p")->Fill(preco, m2reco);
        if (accTofOk) fHM->H1("hmp_Tof_RecoAccTof_" + category + "_m2p")->Fill(preco, m2reco);

        if (stsMCTrackId == tofMCTrackId) {
          fHM->H1("hmp_Tof_RecoMCID_" + category + "_m2p")->Fill(preco, m2reco);
          if (accTofOk) fHM->H1("hmp_Tof_RecoMCIDAccTof_" + category + "_m2p")->Fill(preco, m2reco);
        }
      }
    }
  }
  if (nofTracksForTimeZero > 0) {
    timeZeroReco /= nofTracksForTimeZero;
    timeZeroMC /= nofTracksForTimeZero;
    timeZeroA /= nofTracksForTimeZero;
  }
  fHM->H1("hmp_Tof_TimeZero_reco")->Fill(timeZeroReco);
  fHM->H1("hmp_Tof_TimeZero_mc")->Fill(timeZeroMC);
  fHM->H1("hmp_Tof_TimeZero_a")->Fill(timeZeroA);
  fHM->H1("hmp_Tof_TimeZero_NofTracks")->Fill(nofTracksForTimeZero);
  fHM->H1("hmp_Tof_Time_FirstTrack")->Fill(timeFirstTrack);
}

void CbmLitTofQa::ProcessTofHits()
{
  Int_t nofTofHits = fTofHits->GetEntriesFast();
  for (Int_t iHit = 0; iHit < nofTofHits; iHit++) {
    const CbmTofHit* tofHit = static_cast<const CbmTofHit*>(fTofHits->At(iHit));
    CbmMatch* tofHitMatch   = static_cast<CbmMatch*>(fTofHitsMatches->At(iHit));
    if (tofHitMatch == NULL) { continue; }
    Int_t tofMCPointId          = tofHitMatch->GetMatchedLink().GetIndex();
    Int_t tofMCEventId          = tofHitMatch->GetMatchedLink().GetEntry();
    const CbmTofPoint* tofPoint = static_cast<const CbmTofPoint*>(fTofPoints->Get(0, tofMCEventId, tofMCPointId));
    Int_t tofMCTrackId          = tofPoint->GetTrackID();

    fHM->H1("hmp_Tof_dTime")->Fill(1000 * (tofPoint->GetTime() - tofHit->GetTime()));
  }
}

void CbmLitTofQa::ProcessTofTracks()
{
  map<Int_t, Int_t> nofTofHitsPerGlobalTrack;
  Int_t nofTofTracks = fTofTracks->GetEntriesFast();
  for (Int_t iTrack = 0; iTrack < nofTofTracks; iTrack++) {
    const CbmTofTrack* tofTrack = static_cast<const CbmTofTrack*>(fTofTracks->At(iTrack));
    nofTofHitsPerGlobalTrack[tofTrack->GetTrackIndex()]++;
  }

  for (Int_t iTrack = 0; iTrack < nofTofTracks; iTrack++) {
    const CbmTofTrack* tofTrack = static_cast<const CbmTofTrack*>(fTofTracks->At(iTrack));
    const CbmTofHit* tofHit     = static_cast<const CbmTofHit*>(fTofHits->At(tofTrack->GetTofHitIndex()));
    CbmMatch* tofHitMatch       = static_cast<CbmMatch*>(fTofHitsMatches->At(tofTrack->GetTofHitIndex()));
    if (tofHitMatch == NULL) { continue; }
    Int_t tofMCPointId          = tofHitMatch->GetMatchedLink().GetIndex();
    Int_t tofMCEventId          = tofHitMatch->GetMatchedLink().GetEntry();
    const FairMCPoint* tofPoint = static_cast<const FairMCPoint*>(fTofPoints->Get(0, tofMCEventId, tofMCPointId));
    Int_t tofMCTrackId          = tofPoint->GetTrackID();

    const FairTrackParam* par = tofTrack->GetTrackParameter();
    Double_t dx               = par->GetX() - tofHit->GetX();
    Double_t dy               = par->GetY() - tofHit->GetY();
    Double_t distance         = sqrt(dx * dx + dy * dy);

    Int_t nofTrackCategories = fTrackCategories.size();
    for (Int_t iCat = 0; iCat < nofTrackCategories; iCat++) {
      string category                     = fTrackCategories[iCat];
      LitTrackAcceptanceFunction function = fTrackAcceptanceFunctions.find(category)->second;
      Bool_t categoryOk                   = function(fMCTracks, tofMCEventId, tofMCTrackId);
      if (categoryOk) {
        fHM->H1("hmp_TofTrack_" + category + "_Distance")->Fill(distance);
        fHM->H1("hmp_TofTrack_" + category + "_NormDistance")->Fill(tofTrack->GetDistance());
        fHM->H1("hmp_TofTrack_" + category + "_Length")->Fill(tofTrack->GetTrackLength());
        fHM->H1("hmp_TofTrack_" + category + "_NofHitsPerGlobalTrack")
          ->Fill(nofTofHitsPerGlobalTrack[tofTrack->GetTrackIndex()]);
      }
    }
  }
}

void CbmLitTofQa::FitHistograms()
{
  fHM->H2("hmp_Tof_RecoMCID_Pion_m2p")->FitSlicesY();
  TH1* meanHist  = (TH1*) gDirectory->Get("hmp_Tof_RecoMCID_Pion_m2p_1");  // mean
  TH1* sigmaHist = (TH1*) gDirectory->Get("hmp_Tof_RecoMCID_Pion_m2p_2");  // sigma
  Int_t nofBins  = meanHist->GetNbinsX();
  for (Int_t iBin = 0; iBin <= nofBins; iBin++) {
    Double_t mean  = meanHist->GetBinContent(iBin);
    Double_t sigma = sigmaHist->GetBinContent(iBin);
    std::cout << "mean=" << mean << " sigma=" << sigma << std::endl;
  }
}

ClassImp(CbmLitTofQa)
