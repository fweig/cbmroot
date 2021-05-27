/**
 *    file CbmKresTemperature.cxx
 *
 *    author Ievgenii Kres
 *    date 14.06.2017
 *    modified 30.01.2020
 *
 *
 *    Class is based only on MCtrue information. 
 *    Loop over all MCtrue tracks and plot the temperature spectrum distribution of all produced particles of interest (pions, kaons, photons, protons).
 *    These spectra show the expectations of temperature from temperature analysis. -> one needs to fit them at the end
 *
 **/

#include "CbmKresTemperature.h"

#include "CbmGlobalTrack.h"
#include "CbmMCTrack.h"
#include "CbmStsTrack.h"

#include "FairRootManager.h"

#include "TH1.h"
#include "TH1D.h"


using namespace std;

CbmKresTemperature::CbmKresTemperature()
  : fMcTracks(nullptr)
  , fGlobalTracks(nullptr)
  , fStsTracks(nullptr)
  , fHistoList_MC()
  , MC_pi_minus_Tempr(nullptr)
  , MC_pi_plus_Tempr(nullptr)
  , MC_pi_zero_Tempr(nullptr)
  , MC_proton_Tempr(nullptr)
  , MC_kaon_zero_Tempr(nullptr)
  , MC_kaon_plus_Tempr(nullptr)
  , MC_kaon_minus_Tempr(nullptr)
  , MC_direct_photons_Tempr(nullptr)
{
}

CbmKresTemperature::~CbmKresTemperature() {}

void CbmKresTemperature::Init()
{
  InitHistograms();

  FairRootManager* ioman = FairRootManager::Instance();
  if (nullptr == ioman) { Fatal("CbmKresTemperature::Init", "RootManager not instantised!"); }

  fMcTracks = (TClonesArray*) ioman->GetObject("MCTrack");
  if (nullptr == fMcTracks) { Fatal("CbmKresTemperature::Init", "No MCTrack array!"); }

  fGlobalTracks = (TClonesArray*) ioman->GetObject("GlobalTrack");
  if (nullptr == fGlobalTracks) { Fatal("CbmKresTemperature::Init", "No GlobalTrack array!"); }

  fStsTracks = (TClonesArray*) ioman->GetObject("StsTrack");
  if (nullptr == fStsTracks) { Fatal("CbmKresTemperature::Init", "No StsTrack array!"); }
}

void CbmKresTemperature::Exec(int fEventNumTempr)
{
  cout << "CbmKresTemperature, event No. " << fEventNumTempr << endl;

  // ========================================================================================
  ///////   START - Analyse MC tracks
  Int_t nofMcTracks = fMcTracks->GetEntriesFast();
  for (int i = 0; i < nofMcTracks; i++) {
    CbmMCTrack* mctrack = (CbmMCTrack*) fMcTracks->At(i);
    if (mctrack == nullptr) continue;


    TVector3 v, momentum;

    if (mctrack->GetMotherId() == -1 && mctrack->GetRapidity() > 1.2 && mctrack->GetRapidity() < 4.0
        && mctrack->GetPt() < 2.0) {

      //***** pi0 analysis
      //if (TMath::Abs(mctrack->GetPdgCode()) == 111  && mctrack->GetMotherId() == -1) {
      if (mctrack->GetPdgCode() == 111) {
        mctrack->GetStartVertex(v);
        mctrack->GetMomentum(momentum);
        double m0 = 0.1349766;  // Gev/c^2
        double pt = mctrack->GetPt();
        double mt = TMath::Sqrt(pt * pt + m0 * m0);
        MC_pi_zero_Tempr->Fill(mt - m0);
      }

      //***** direct photons analysis
      if (mctrack->GetPdgCode() == 22) {
        mctrack->GetStartVertex(v);
        mctrack->GetMomentum(momentum);
        double pt = mctrack->GetPt();
        MC_direct_photons_Tempr->Fill(pt);
      }

      //***** pi+ analysis
      //if (mctrack->GetPdgCode() == 211 && mctrack->GetMotherId() == -1) {
      if (mctrack->GetPdgCode() == 211) {
        mctrack->GetStartVertex(v);
        mctrack->GetMomentum(momentum);
        double m0 = 0.13957018;  // Gev/c^2
        double pt = mctrack->GetPt();
        double mt = TMath::Sqrt(pt * pt + m0 * m0);
        MC_pi_plus_Tempr->Fill(mt - m0);
      }

      //***** pi- analysis
      //if (mctrack->GetPdgCode() == -211 && mctrack->GetMotherId() == -1) {
      if (mctrack->GetPdgCode() == -211) {
        mctrack->GetStartVertex(v);
        mctrack->GetMomentum(momentum);
        double m0 = 0.13957018;  // Gev/c^2
        double pt = mctrack->GetPt();
        double mt = TMath::Sqrt(pt * pt + m0 * m0);
        MC_pi_minus_Tempr->Fill(mt - m0);
      }

      //***** proton analysis
      //if (mctrack->GetPdgCode() == 2212 && mctrack->GetMotherId() == -1) {
      if (mctrack->GetPdgCode() == 2212) {
        mctrack->GetStartVertex(v);
        mctrack->GetMomentum(momentum);
        double m0 = 0.9382720813;  // Gev/c^2
        double pt = mctrack->GetPt();
        double mt = TMath::Sqrt(pt * pt + m0 * m0);
        MC_proton_Tempr->Fill(mt - m0);
      }

      //***** kaon0_S analysis
      //if (mctrack->GetPdgCode() == 310 && mctrack->GetMotherId() == -1) {
      if (mctrack->GetPdgCode() == 310) {
        mctrack->GetStartVertex(v);
        mctrack->GetMomentum(momentum);
        double m0 = 0.497614;  // Gev/c^2
        double pt = mctrack->GetPt();
        double mt = TMath::Sqrt(pt * pt + m0 * m0);
        MC_kaon_zero_Tempr->Fill(mt - m0);
      }

      //***** kaon+ analysis
      //if (mctrack->GetPdgCode() == 321 && mctrack->GetMotherId() == -1) {
      if (mctrack->GetPdgCode() == 321) {
        mctrack->GetStartVertex(v);
        mctrack->GetMomentum(momentum);
        double m0 = 0.493677;  // Gev/c^2
        double pt = mctrack->GetPt();
        double mt = TMath::Sqrt(pt * pt + m0 * m0);
        MC_kaon_plus_Tempr->Fill(mt - m0);
      }

      //***** kaon- analysis
      //if (mctrack->GetPdgCode() == -321 && mctrack->GetMotherId() == -1) {
      if (mctrack->GetPdgCode() == -321) {
        mctrack->GetStartVertex(v);
        mctrack->GetMomentum(momentum);
        double m0 = 0.493677;  // Gev/c^2
        double pt = mctrack->GetPt();
        double mt = TMath::Sqrt(pt * pt + m0 * m0);
        MC_kaon_minus_Tempr->Fill(mt - m0);
      }
    }
  }
}

void CbmKresTemperature::Finish()
{
  MC_pi_minus_Tempr->SetOption("P");
  MC_pi_minus_Tempr->SetMarkerStyle(20);
  MC_pi_plus_Tempr->SetOption("P");
  MC_pi_plus_Tempr->SetMarkerStyle(20);
  MC_pi_zero_Tempr->SetOption("P");
  MC_pi_zero_Tempr->SetMarkerStyle(20);
  MC_direct_photons_Tempr->SetOption("P");
  MC_direct_photons_Tempr->SetMarkerStyle(20);

  MC_proton_Tempr->SetOption("P");
  MC_proton_Tempr->SetMarkerStyle(20);
  MC_kaon_zero_Tempr->SetOption("P");
  MC_kaon_zero_Tempr->SetMarkerStyle(20);
  MC_kaon_plus_Tempr->SetOption("P");
  MC_kaon_plus_Tempr->SetMarkerStyle(20);
  MC_kaon_minus_Tempr->SetOption("P");
  MC_kaon_minus_Tempr->SetMarkerStyle(20);

  gDirectory->mkdir("temperature");
  gDirectory->cd("temperature");

  gDirectory->mkdir("MC");
  gDirectory->cd("MC");
  for (UInt_t i = 0; i < fHistoList_MC.size(); i++) {
    fHistoList_MC[i]->Write();
  }
  gDirectory->cd("..");

  gDirectory->cd("..");
}

void CbmKresTemperature::InitHistograms()
{
  MC_pi_minus_Tempr =
    new TH1D("MC_pi_minus_Tempr", "Monte Carlo,   primary #pi^{-}; m_{t}-m_{0} distribution", 100, 0, 2);
  fHistoList_MC.push_back(MC_pi_minus_Tempr);

  MC_pi_plus_Tempr =
    new TH1D("MC_pi_plus_Tempr", "Monte Carlo,   primary #pi^{+}; m_{t}-m_{0} distribution", 100, 0, 2);
  fHistoList_MC.push_back(MC_pi_plus_Tempr);

  MC_pi_zero_Tempr =
    new TH1D("MC_pi_zero_Tempr", "Monte Carlo,   primary #pi^{0}; m_{t}-m_{0} distribution", 100, 0, 2);
  fHistoList_MC.push_back(MC_pi_zero_Tempr);

  MC_direct_photons_Tempr =
    new TH1D("MC_direct_photons_Tempr", "Monte Carlo,   direct #gamma; m_{t}-m_{0} distribution", 100, 0, 2);
  fHistoList_MC.push_back(MC_direct_photons_Tempr);

  MC_proton_Tempr = new TH1D("MC_proton_Tempr", "Monte Carlo,   primary #p; m_{t}-m_{0} distribution", 100, 0, 2);
  fHistoList_MC.push_back(MC_proton_Tempr);

  MC_kaon_zero_Tempr =
    new TH1D("MC_kaon_zero_Tempr", "Monte Carlo,   primary #kaon^{S0}; m_{t}-m_{0} distribution", 100, 0, 2);
  fHistoList_MC.push_back(MC_kaon_zero_Tempr);

  MC_kaon_plus_Tempr =
    new TH1D("MC_kaon_plus_Tempr", "Monte Carlo,   primary #kaon^{+}; m_{t}-m_{0} distribution", 100, 0, 2);
  fHistoList_MC.push_back(MC_kaon_plus_Tempr);

  MC_kaon_minus_Tempr =
    new TH1D("MC_kaon_minus_Tempr", "Monte Carlo,   primary #kaon^{-}; m_{t}-m_{0} distribution", 100, 0, 2);
  fHistoList_MC.push_back(MC_kaon_minus_Tempr);
}
