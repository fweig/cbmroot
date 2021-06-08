/* Copyright (C) 2018-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Ievgenii Kres, Florian Uhlig [committer] */

/**
 *    file CbmKresEtaMCAnalysis.cxx
 *
 *    author Ievgenii Kres
 *    date 27.03.2018
 *    modified 30.01.2020
 *
 *    Class dedicated to estimate from simulation how many eta can be reconstructed using different channels (similar to "CbmKresConversionReconstruction.cxx")
 *    Loop over all global tracks -> check with MC data if particles are leptons -> combine particles in 4 pairs. If 4 particles stem from one pi^0 or eta -> store in histogram
 *    Considered are channels: gamma + gamma, pi^- pi^+ gamma, pi^- pi^+ pi^0.
 *    It is kind pf "perfect PID" + "perfect matching in mother/grandmother particles"
 *    One can also see here the difference for the final reconstruction if one would use MCtrue momenta or reconstructed momenta.
 *
 **/

#include "CbmKresEtaMCAnalysis.h"

#include "CbmGlobalTrack.h"
#include "CbmKresFunctions.h"
#include "CbmMCTrack.h"
#include "CbmStsTrack.h"
#include "CbmTrackMatchNew.h"

#include "FairRootManager.h"

#include "TDirectory.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TMath.h"

#include <iostream>


using namespace std;

CbmKresEtaMCAnalysis::CbmKresEtaMCAnalysis()
  : fMcTracks(nullptr)
  , fGlobalTracks(nullptr)
  , fStsTracks(nullptr)
  , fStsTrackMatches(nullptr)
  , El_Photon_Eta_refmomentum()
  , El_Photon_Eta_MCtrack()
  , El_Photon_Eta_Id()
  , El_Photon_Pion_Eta_refmomentum()
  , El_Photon_Pion_Eta_MCtrack()
  , El_Photon_Pion_Eta_Id()
  , Pion_Eta_refmomentum()
  , Pion_Eta_MCtrack()
  , Pion_Eta_Id()
  , All_El_refmomentum()
  , All_El_MCtrack()
  , All_El_Id()
  , All_Pion_refmomentum()
  , All_Pion_MCtrack()
  , All_Pion_Id()
  , frefmomenta()
  , fMCtracks()
  , fMCId()
  , EDGA_RefMom()
  , EDGA_MC()
  , EDGA_Id()
  , ECPGA_leptons_RefMom()
  , ECPGA_leptons_MC()
  , ECPGA_leptons_Id()
  , ECPGA_pions_RefMom()
  , ECPGA_pions_MC()
  , fHistoList_eta_gg()
  , InvMass_eta_gg_mc(nullptr)
  , InvMass_eta_gg_reffited(nullptr)
  , InvMassPhoton_eta_gg_mc(nullptr)
  , InvMassPhoton_eta_gg_reffited(nullptr)
  , OpeningAnglePhoton_eta_gg_mc(nullptr)
  , OpeningAnglePhoton_eta_gg_reffited(nullptr)
  , OpeningAngle_eta_gg_between_gg_mc(nullptr)
  , OpeningAngle_eta_gg_between_gg_reffited(nullptr)
  , InvMass_eta_gg_allcombinations_mc(nullptr)
  , InvMass_eta_gg_allcombinations_reffited(nullptr)
  , EMT_eta_gg(nullptr)
  , InvMass_eta_gg_reco_aftercuts(nullptr)
  , rap_vs_pt_eta_gg_reco_aftercuts(nullptr)
  , rap_vs_pt_NOTeta_gg_reco_aftercuts(nullptr)
  , fHistoList_eta_ppg()
  , InvMass_eta_ppg_mc(nullptr)
  , InvMass_eta_ppg_reffited(nullptr)
  , InvMassPhoton_eta_ppg_mc(nullptr)
  , InvMassPhoton_eta_ppg_reffited(nullptr)
  , OpeningAnglePhoton_eta_ppg_mc(nullptr)
  , OpeningAnglePhoton_eta_ppg_reffited(nullptr)
  , InvMass_eta_ppg_allcombinations_mc(nullptr)
  , InvMass_eta_ppg_allcombinations_reffited(nullptr)
  , Pion_P_fromEta_reco(nullptr)
  , Pion_P_elsewhere_reco(nullptr)
  , Pion_Pt_fromEta_reco(nullptr)
  , Pion_Pt_elsewhere_reco(nullptr)
  , OA_betweenPions_fromEta_mc(nullptr)
  , OA_betweenPions_fromEta_reco(nullptr)
  , OA_betweenPions_fromEta_reco_wrongcombinations(nullptr)
  , EMT_eta_ppg(nullptr)
  , EMT_eta_three_body(nullptr)
  , InvMass_eta_ppg_reco_aftercuts(nullptr)
  , rap_vs_pt_eta_ppg_reco_aftercuts(nullptr)
  , rap_vs_pt_NOTeta_ppg_reco_aftercuts(nullptr)
  , fHistoList_eta_ppp()
  , InvMass_eta_ppp_mc(nullptr)
  , InvMass_eta_ppp_reffited(nullptr)
  , InvMass_eta_Npion_mc(nullptr)
  , InvMass_eta_Npion_reffited(nullptr)
  , InvMass_eta_ppp_allcombinations_mc(nullptr)
  , InvMass_eta_ppp_allcombinations_reffited(nullptr)
  , EMT_gg_Event()
  , EMT_gg_pair_momenta()
  , EMT_ppg_ee_Event()
  , EMT_ppg_ee_pair_momenta()
  , EMT_ppg_pp_Event()
  , EMT_ppg_pp_pair_momenta()
  , EMT_ppg_positive_pion_Event()
  , EMT_ppg_positive_pion_momenta()
  , EMT_ppg_negative_pion_Event()
  , EMT_ppg_negative_pion_momenta()
{
}

CbmKresEtaMCAnalysis::~CbmKresEtaMCAnalysis() {}

void CbmKresEtaMCAnalysis::Init()
{
  InitHistograms();

  FairRootManager* ioman = FairRootManager::Instance();
  if (nullptr == ioman) { Fatal("CbmKresEtaMCAnalysis::Init", "RootManager not instantised!"); }

  fMcTracks = (TClonesArray*) ioman->GetObject("MCTrack");
  if (nullptr == fMcTracks) { Fatal("CbmKresEtaMCAnalysis::Init", "No MCTrack array!"); }

  fGlobalTracks = (TClonesArray*) ioman->GetObject("GlobalTrack");
  if (nullptr == fGlobalTracks) { Fatal("CbmKresEtaMCAnalysis::Init", "No GlobalTrack array!"); }

  fStsTracks = (TClonesArray*) ioman->GetObject("StsTrack");
  if (nullptr == fStsTracks) { Fatal("CbmKresEtaMCAnalysis::Init", "No StsTrack array!"); }

  fStsTrackMatches = (TClonesArray*) ioman->GetObject("StsTrackMatch");
  if (nullptr == fStsTrackMatches) { Fatal("CbmKresEtaMCAnalysis::Init", "No StsTrackMatch array!"); }
}


void CbmKresEtaMCAnalysis::Exec(int Event, double OpeningAngleCut, double GammaInvMassCut)
{
  // cout << "CbmKresEtaMCAnalysis, event No. " <<  Event << endl;

  int counter       = 0;
  Int_t nofMcTracks = fMcTracks->GetEntriesFast();
  for (int i = 0; i < nofMcTracks; i++) {
    CbmMCTrack* mctrack = (CbmMCTrack*) fMcTracks->At(i);
    if (mctrack == nullptr) continue;
    if (mctrack->GetPdgCode() == 221) counter++;

    // if (mctrack->GetMotherId() == -1) continue;
    // 	CbmMCTrack* mcMotherTrack_1 = (CbmMCTrack*) fMcTracks->At(mctrack->GetMotherId());
    // 	if (mcMotherTrack_1 == nullptr) continue;
    // 	if (TMath::Abs(mctrack->GetPdgCode()) == 11 && mcMotherTrack_1->GetPdgCode() == 22){
    // 	if(mcMotherTrack_1->GetMotherId() == -1) continue;
    // 	CbmMCTrack* mcGrTrack = (CbmMCTrack*) fMcTracks->At(mcMotherTrack_1->GetMotherId());
    // 	if (mcGrTrack == nullptr) continue;
    // 	if (mcGrTrack->GetPdgCode() == 221){
    // 		cout<< "Electron: id = " << i << "; motherid = " << mctrack->GetMotherId() << "; GrId = " << mcMotherTrack_1->GetMotherId() << endl;
    // 	}
    // }
  }
  // cout << "number of etas in event = " << counter << endl;

  El_Photon_Eta_refmomentum.clear();
  El_Photon_Eta_MCtrack.clear();
  El_Photon_Eta_Id.clear();

  El_Photon_Pion_Eta_refmomentum.clear();
  El_Photon_Pion_Eta_MCtrack.clear();
  El_Photon_Pion_Eta_Id.clear();

  Pion_Eta_refmomentum.clear();
  Pion_Eta_MCtrack.clear();
  Pion_Eta_Id.clear();

  All_El_refmomentum.clear();
  All_El_MCtrack.clear();
  All_El_Id.clear();

  All_Pion_refmomentum.clear();
  All_Pion_MCtrack.clear();
  All_Pion_Id.clear();


  ///////   Global tracks analysis
  // ========================================================================================
  Int_t ngTracks = fGlobalTracks->GetEntriesFast();
  for (Int_t iTr = 0; iTr < ngTracks; iTr++) {
    CbmGlobalTrack* gTrack = (CbmGlobalTrack*) fGlobalTracks->At(iTr);
    if (nullptr == gTrack) continue;
    int stsInd = gTrack->GetStsTrackIndex();

    // ========================================================================================
    ///////   STS
    if (stsInd < 0) continue;
    CbmStsTrack* stsTrack = (CbmStsTrack*) fStsTracks->At(stsInd);
    if (stsTrack == nullptr) continue;
    CbmTrackMatchNew* stsMatch = (CbmTrackMatchNew*) fStsTrackMatches->At(stsInd);
    if (stsMatch == nullptr) continue;
    if (stsMatch->GetNofLinks() <= 0) continue;
    int McTrackId = stsMatch->GetMatchedLink().GetIndex();
    if (McTrackId < 0) continue;
    CbmMCTrack* mcTrack = (CbmMCTrack*) fMcTracks->At(McTrackId);
    if (mcTrack == nullptr) continue;
    int pdg      = mcTrack->GetPdgCode();
    int motherId = mcTrack->GetMotherId();
    if (TMath::Abs(pdg) != 11 && TMath::Abs(pdg) != 211) continue;
    ///////   STS (END)
    // ========================================================================================

    double chi2          = 0;
    TVector3 refmomentum = CbmKresFunctions::FitToVertexAndGetChi(stsTrack, mcTrack->GetStartX(), mcTrack->GetStartY(),
                                                                  mcTrack->GetStartZ(), chi2);

    // cout << "" << endl;
    // cout << " pdg = " << mcTrack->GetPdgCode() << endl;
    // cout << "mc:   P = " << mcTrack->GetP() <<  ";\t Pt = " << mcTrack->GetPt() << "; \t E = " << mcTrack->GetEnergy() << endl;
    // cout << "reco: P = " << refmomentum.Mag() <<  ";\t Pt = " << refmomentum.Perp() << "; \t E = " << TMath::Sqrt(refmomentum.Mag2() + mcTrack->GetMass()*mcTrack->GetMass()) << endl;
    // cout << "chi2 = " << chi2 << endl;
    // cout << "=========================" << endl;

    // if (chi2 > 3) continue;

    if (TMath::Abs(pdg) == 11) {
      All_El_refmomentum.push_back(refmomentum);
      All_El_MCtrack.push_back(mcTrack);
      All_El_Id.push_back(McTrackId);
    }
    if (TMath::Abs(pdg) == 211) {
      All_Pion_refmomentum.push_back(refmomentum);
      All_Pion_MCtrack.push_back(mcTrack);
      All_Pion_Id.push_back(McTrackId);
      Pion_P_elsewhere_reco->Fill(refmomentum.Mag());
      Pion_Pt_elsewhere_reco->Fill(refmomentum.Perp());
    }

    if (motherId == -1) continue;
    CbmMCTrack* mcMotherTrack = (CbmMCTrack*) fMcTracks->At(motherId);
    if (mcMotherTrack == nullptr) continue;

    if (TMath::Abs(pdg) == 11 && mcMotherTrack->GetPdgCode() == 22) {
      if (mcMotherTrack->GetMotherId() == -1) continue;
      CbmMCTrack* mcGrTrack = (CbmMCTrack*) fMcTracks->At(mcMotherTrack->GetMotherId());
      if (mcGrTrack == nullptr) continue;
      if (mcGrTrack->GetPdgCode() == 221) {
        El_Photon_Eta_refmomentum.push_back(refmomentum);
        El_Photon_Eta_MCtrack.push_back(mcTrack);
        El_Photon_Eta_Id.push_back(McTrackId);
      }
      if (mcGrTrack->GetPdgCode() == 111) {
        if (mcGrTrack->GetMotherId() == -1) continue;
        CbmMCTrack* mcTopTrack = (CbmMCTrack*) fMcTracks->At(mcGrTrack->GetMotherId());
        if (mcTopTrack->GetPdgCode() == 221) {
          El_Photon_Pion_Eta_refmomentum.push_back(refmomentum);
          El_Photon_Pion_Eta_MCtrack.push_back(mcTrack);
          El_Photon_Pion_Eta_Id.push_back(McTrackId);
        }
      }
    }

    if (TMath::Abs(pdg) == 211 && mcMotherTrack->GetPdgCode() == 221) {
      Pion_Eta_refmomentum.push_back(refmomentum);
      Pion_Eta_MCtrack.push_back(mcTrack);
      Pion_Eta_Id.push_back(McTrackId);
      Pion_P_fromEta_reco->Fill(refmomentum.Mag());
      Pion_Pt_fromEta_reco->Fill(refmomentum.Perp());
    }
  }
  ///////   Global tracks analysis (END)
  // ========================================================================================


  EtaDoubleGammaAnalysis(El_Photon_Eta_refmomentum, El_Photon_Eta_MCtrack, El_Photon_Eta_Id, fHistoList_eta_gg);
  EtaChargedPionsGammaAnalysis(Pion_Eta_refmomentum, Pion_Eta_MCtrack, Pion_Eta_Id, El_Photon_Eta_refmomentum,
                               El_Photon_Eta_MCtrack, El_Photon_Eta_Id, fHistoList_eta_ppg);
  EtaPosNegNeutralPionsAnalysis(El_Photon_Pion_Eta_refmomentum, El_Photon_Pion_Eta_MCtrack, El_Photon_Pion_Eta_Id,
                                Pion_Eta_refmomentum, Pion_Eta_MCtrack, Pion_Eta_Id, fHistoList_eta_ppp);


  EtaDoubleGammaAnalysis_plusBG(OpeningAngleCut, GammaInvMassCut, Event, All_El_refmomentum, All_El_MCtrack, All_El_Id,
                                fHistoList_eta_gg);
  EtaChargedPionsGammaAnalysis_plusBG(OpeningAngleCut, GammaInvMassCut, Event, All_Pion_refmomentum, All_Pion_MCtrack,
                                      All_Pion_Id, All_El_refmomentum, All_El_MCtrack, All_El_Id, fHistoList_eta_ppg,
                                      fHistoList_eta_ppp);

  if (Event % 1000 == 0) {
    // Mixing_gg();
    EMT_gg_Event.clear();
    EMT_gg_pair_momenta.clear();
  }

  if (Event % 10 == 0) {
    // Mixing_three_body();
    EMT_ppg_negative_pion_Event.clear();
    EMT_ppg_negative_pion_momenta.clear();
    EMT_ppg_positive_pion_Event.clear();
    EMT_ppg_positive_pion_momenta.clear();

    // Mixing_ppg();
    EMT_ppg_ee_Event.clear();
    EMT_ppg_ee_pair_momenta.clear();
    EMT_ppg_pp_Event.clear();
    EMT_ppg_pp_pair_momenta.clear();
  }
}

Double_t CbmKresEtaMCAnalysis::CalculateOpeningAngleBetweenGammas_MC(CbmMCTrack* mctrack1, CbmMCTrack* mctrack2,
                                                                     CbmMCTrack* mctrack3, CbmMCTrack* mctrack4)
{
  Double_t openingAngle;
  TLorentzVector gamma1;
  TLorentzVector gamma2;

  if (mctrack1->GetMotherId() == mctrack2->GetMotherId() && mctrack3->GetMotherId() == mctrack4->GetMotherId()) {
    CbmMCTrack* mother1 = (CbmMCTrack*) fMcTracks->At(mctrack1->GetMotherId());
    mother1->Get4Momentum(gamma1);
    CbmMCTrack* mother2 = (CbmMCTrack*) fMcTracks->At(mctrack3->GetMotherId());
    mother2->Get4Momentum(gamma2);
  }
  if (mctrack1->GetMotherId() == mctrack3->GetMotherId() && mctrack2->GetMotherId() == mctrack4->GetMotherId()) {
    CbmMCTrack* mother1 = (CbmMCTrack*) fMcTracks->At(mctrack1->GetMotherId());
    mother1->Get4Momentum(gamma1);
    CbmMCTrack* mother2 = (CbmMCTrack*) fMcTracks->At(mctrack2->GetMotherId());
    mother2->Get4Momentum(gamma2);
  }
  if (mctrack1->GetMotherId() == mctrack4->GetMotherId() && mctrack2->GetMotherId() == mctrack3->GetMotherId()) {
    CbmMCTrack* mother1 = (CbmMCTrack*) fMcTracks->At(mctrack1->GetMotherId());
    mother1->Get4Momentum(gamma1);
    CbmMCTrack* mother2 = (CbmMCTrack*) fMcTracks->At(mctrack2->GetMotherId());
    mother2->Get4Momentum(gamma2);
  }

  Double_t angle = gamma1.Angle(gamma2.Vect());
  openingAngle   = 180. * angle / TMath::Pi();

  return openingAngle;
}

Double_t CbmKresEtaMCAnalysis::CalculateOpeningAngleBetweenGammas_Reco(TVector3 electron1, TVector3 electron2,
                                                                       TVector3 electron3, TVector3 electron4)
{
  double M2El      = 2.6112004954086e-7;
  Double_t energy1 = TMath::Sqrt(electron1.Mag2() + M2El);
  TLorentzVector lorVec1(electron1, energy1);

  Double_t energy2 = TMath::Sqrt(electron2.Mag2() + M2El);
  TLorentzVector lorVec2(electron2, energy2);

  Double_t energy3 = TMath::Sqrt(electron3.Mag2() + M2El);
  TLorentzVector lorVec3(electron3, energy3);

  Double_t energy4 = TMath::Sqrt(electron4.Mag2() + M2El);
  TLorentzVector lorVec4(electron4, energy4);

  TLorentzVector lorPhoton1 = lorVec1 + lorVec2;
  TLorentzVector lorPhoton2 = lorVec3 + lorVec4;

  Double_t angleBetweenPhotons = lorPhoton1.Angle(lorPhoton2.Vect());
  Double_t theta               = 180. * angleBetweenPhotons / TMath::Pi();

  return theta;
}


void CbmKresEtaMCAnalysis::EtaDoubleGammaAnalysis(vector<TVector3> RefMom, vector<CbmMCTrack*> MC, vector<Int_t> Id,
                                                  vector<TH1*> gg)
{
  //================================== decay eta -> gamma gamma -> e+e- e+e-
  // cout << "MC.size() = " << MC.size() << endl;
  if (MC.size() < 4) return;

  for (size_t i = 0; i < MC.size(); i++) {
    for (size_t j = i + 1; j < MC.size(); j++) {
      for (size_t k = j + 1; k < MC.size(); k++) {
        for (size_t l = k + 1; l < MC.size(); l++) {

          int pdg1 = MC.at(i)->GetPdgCode();
          int pdg2 = MC.at(j)->GetPdgCode();
          int pdg3 = MC.at(k)->GetPdgCode();
          int pdg4 = MC.at(l)->GetPdgCode();

          if (pdg1 + pdg2 + pdg3 + pdg4 != 0) continue;
          if (TMath::Abs(pdg1) != 11 || TMath::Abs(pdg2) != 11 || TMath::Abs(pdg3) != 11 || TMath::Abs(pdg4) != 11)
            continue;

          int motherId1 = MC.at(i)->GetMotherId();
          int motherId2 = MC.at(j)->GetMotherId();
          int motherId3 = MC.at(k)->GetMotherId();
          int motherId4 = MC.at(l)->GetMotherId();

          int mcId1 = Id.at(i);
          int mcId2 = Id.at(j);
          int mcId3 = Id.at(k);
          int mcId4 = Id.at(l);

          if (mcId1 == mcId2 || mcId1 == mcId3 || mcId1 == mcId4 || mcId2 == mcId3 || mcId2 == mcId4 || mcId3 == mcId4)
            continue;  // particle is not used twice
          if (motherId1 == -1 || motherId2 == -1 || motherId3 == -1 || motherId4 == -1) continue;

          CbmMCTrack* mother1 = (CbmMCTrack*) fMcTracks->At(motherId1);
          CbmMCTrack* mother2 = (CbmMCTrack*) fMcTracks->At(motherId2);
          CbmMCTrack* mother3 = (CbmMCTrack*) fMcTracks->At(motherId3);
          CbmMCTrack* mother4 = (CbmMCTrack*) fMcTracks->At(motherId4);

          int mcMotherPdg1 = mother1->GetPdgCode();
          int mcMotherPdg2 = mother2->GetPdgCode();
          int mcMotherPdg3 = mother3->GetPdgCode();
          int mcMotherPdg4 = mother4->GetPdgCode();

          if (mcMotherPdg1 != 22 || mcMotherPdg2 != 22 || mcMotherPdg3 != 22 || mcMotherPdg4 != 22) continue;

          int grandmotherId1 = mother1->GetMotherId();
          int grandmotherId2 = mother2->GetMotherId();
          int grandmotherId3 = mother3->GetMotherId();
          int grandmotherId4 = mother4->GetMotherId();

          if (grandmotherId1 == -1) continue;


          if (grandmotherId1 == grandmotherId2 && grandmotherId1 == grandmotherId3
              && grandmotherId1 == grandmotherId4) {

            CbmMCTrack* GrMother1 = (CbmMCTrack*) fMcTracks->At(grandmotherId1);
            int mcGrandmotherPdg1 = GrMother1->GetPdgCode();
            if (mcGrandmotherPdg1 != 221) continue;

            Double_t InvMass_true = CbmKresFunctions::Invmass_4particles_MC(MC.at(i), MC.at(j), MC.at(k), MC.at(l));
            Double_t InvMass_reco =
              CbmKresFunctions::Invmass_4particles_RECO(RefMom.at(i), RefMom.at(j), RefMom.at(k), RefMom.at(l));
            cout << "Decay eta -> gamma gamma -> e+e- e+e- detected!\t\t mc mass: " << InvMass_true
                 << "\t, reco mass: " << InvMass_reco << endl;
            cout << "motherids: " << motherId1 << "/" << motherId2 << "/" << motherId3 << "/" << motherId4 << endl;
            cout << "grandmotherid: " << grandmotherId1 << "/" << grandmotherId2 << "/" << grandmotherId3 << "/"
                 << grandmotherId4 << endl;
            cout << "pdgs: " << mcGrandmotherPdg1 << "-->" << mcMotherPdg1 << "/" << mcMotherPdg2 << "/" << mcMotherPdg3
                 << "/" << mcMotherPdg4 << "-->" << pdg1 << "/" << pdg2 << "/" << pdg3 << "/" << pdg4 << endl;

            gg[0]->Fill(InvMass_true);
            gg[1]->Fill(InvMass_reco);

            cout << "\t \t mc true info: " << endl;
            cout << "particle 1: \t" << MC.at(i)->GetPdgCode() << ";\t pt = " << MC.at(i)->GetPt()
                 << ";\t X = " << MC.at(i)->GetStartX() << ";\t Y = " << MC.at(i)->GetStartY()
                 << ";\t Z = " << MC.at(i)->GetStartZ() << ";\t E = " << MC.at(i)->GetEnergy() << endl;
            cout << "particle 2: \t" << MC.at(j)->GetPdgCode() << ";\t pt = " << MC.at(j)->GetPt()
                 << ";\t X = " << MC.at(j)->GetStartX() << ";\t Y = " << MC.at(j)->GetStartY()
                 << ";\t Z = " << MC.at(j)->GetStartZ() << ";\t E = " << MC.at(j)->GetEnergy() << endl;
            cout << "particle 3: \t" << MC.at(k)->GetPdgCode() << ";\t pt = " << MC.at(k)->GetPt()
                 << ";\t X = " << MC.at(k)->GetStartX() << ";\t Y = " << MC.at(k)->GetStartY()
                 << ";\t Z = " << MC.at(k)->GetStartZ() << ";\t E = " << MC.at(k)->GetEnergy() << endl;
            cout << "particle 4: \t" << MC.at(l)->GetPdgCode() << ";\t pt = " << MC.at(l)->GetPt()
                 << ";\t X = " << MC.at(l)->GetStartX() << ";\t Y = " << MC.at(l)->GetStartY()
                 << ";\t Z = " << MC.at(l)->GetStartZ() << ";\t E = " << MC.at(l)->GetEnergy() << endl;

            Double_t OpeningAngle1_mc        = 0;
            Double_t OpeningAngle2_mc        = 0;
            Double_t OpeningAngle1_refitted  = 0;
            Double_t OpeningAngle2_refitted  = 0;
            Double_t InvMass_realg1_mc       = 0;
            Double_t InvMass_realg2_mc       = 0;
            Double_t InvMass_realg1_refitted = 0;
            Double_t InvMass_realg2_refitted = 0;

            Double_t openingAngleBetweenGammasReco = 0;

            if (motherId1 == motherId2) {
              OpeningAngle1_mc = CbmKresFunctions::CalculateOpeningAngle_MC(MC.at(i), MC.at(j));
              OpeningAngle2_mc = CbmKresFunctions::CalculateOpeningAngle_MC(MC.at(k), MC.at(l));
              gg[4]->Fill(OpeningAngle1_mc);
              gg[4]->Fill(OpeningAngle2_mc);
              OpeningAngle1_refitted = CbmKresFunctions::CalculateOpeningAngle_Reco(RefMom.at(i), RefMom.at(j));
              OpeningAngle2_refitted = CbmKresFunctions::CalculateOpeningAngle_Reco(RefMom.at(k), RefMom.at(l));
              gg[5]->Fill(OpeningAngle1_refitted);
              gg[5]->Fill(OpeningAngle2_refitted);

              InvMass_realg1_mc = CbmKresFunctions::Invmass_2particles_MC(MC.at(i), MC.at(j));
              InvMass_realg2_mc = CbmKresFunctions::Invmass_2particles_MC(MC.at(k), MC.at(l));
              gg[2]->Fill(InvMass_realg1_mc);
              gg[2]->Fill(InvMass_realg2_mc);
              InvMass_realg1_refitted = CbmKresFunctions::Invmass_2particles_RECO(RefMom.at(i), RefMom.at(j));
              InvMass_realg2_refitted = CbmKresFunctions::Invmass_2particles_RECO(RefMom.at(k), RefMom.at(l));
              gg[3]->Fill(InvMass_realg1_refitted);
              gg[3]->Fill(InvMass_realg2_refitted);
              openingAngleBetweenGammasReco =
                CalculateOpeningAngleBetweenGammas_Reco(RefMom.at(i), RefMom.at(j), RefMom.at(k), RefMom.at(l));
            }
            if (motherId1 == motherId3) {
              OpeningAngle1_mc = CbmKresFunctions::CalculateOpeningAngle_MC(MC.at(i), MC.at(k));
              OpeningAngle2_mc = CbmKresFunctions::CalculateOpeningAngle_MC(MC.at(j), MC.at(l));
              gg[4]->Fill(OpeningAngle1_mc);
              gg[4]->Fill(OpeningAngle2_mc);
              OpeningAngle1_refitted = CbmKresFunctions::CalculateOpeningAngle_Reco(RefMom.at(i), RefMom.at(k));
              OpeningAngle2_refitted = CbmKresFunctions::CalculateOpeningAngle_Reco(RefMom.at(j), RefMom.at(l));
              gg[5]->Fill(OpeningAngle1_refitted);
              gg[5]->Fill(OpeningAngle2_refitted);

              InvMass_realg1_mc = CbmKresFunctions::Invmass_2particles_MC(MC.at(i), MC.at(k));
              InvMass_realg2_mc = CbmKresFunctions::Invmass_2particles_MC(MC.at(j), MC.at(l));
              gg[2]->Fill(InvMass_realg1_mc);
              gg[2]->Fill(InvMass_realg2_mc);
              InvMass_realg1_refitted = CbmKresFunctions::Invmass_2particles_RECO(RefMom.at(i), RefMom.at(k));
              InvMass_realg2_refitted = CbmKresFunctions::Invmass_2particles_RECO(RefMom.at(j), RefMom.at(l));
              gg[3]->Fill(InvMass_realg1_refitted);
              gg[3]->Fill(InvMass_realg2_refitted);
              openingAngleBetweenGammasReco =
                CalculateOpeningAngleBetweenGammas_Reco(RefMom.at(i), RefMom.at(k), RefMom.at(j), RefMom.at(l));
            }
            if (motherId1 == motherId4) {
              OpeningAngle1_mc = CbmKresFunctions::CalculateOpeningAngle_MC(MC.at(i), MC.at(l));
              OpeningAngle2_mc = CbmKresFunctions::CalculateOpeningAngle_MC(MC.at(j), MC.at(k));
              gg[4]->Fill(OpeningAngle1_mc);
              gg[4]->Fill(OpeningAngle2_mc);
              OpeningAngle1_refitted = CbmKresFunctions::CalculateOpeningAngle_Reco(RefMom.at(i), RefMom.at(l));
              OpeningAngle2_refitted = CbmKresFunctions::CalculateOpeningAngle_Reco(RefMom.at(j), RefMom.at(k));
              gg[5]->Fill(OpeningAngle1_refitted);
              gg[5]->Fill(OpeningAngle2_refitted);

              InvMass_realg1_mc = CbmKresFunctions::Invmass_2particles_MC(MC.at(i), MC.at(l));
              InvMass_realg2_mc = CbmKresFunctions::Invmass_2particles_MC(MC.at(j), MC.at(k));
              gg[2]->Fill(InvMass_realg1_mc);
              gg[2]->Fill(InvMass_realg2_mc);
              InvMass_realg1_refitted = CbmKresFunctions::Invmass_2particles_RECO(RefMom.at(i), RefMom.at(l));
              InvMass_realg2_refitted = CbmKresFunctions::Invmass_2particles_RECO(RefMom.at(j), RefMom.at(k));
              gg[3]->Fill(InvMass_realg1_refitted);
              gg[3]->Fill(InvMass_realg2_refitted);
              openingAngleBetweenGammasReco =
                CalculateOpeningAngleBetweenGammas_Reco(RefMom.at(i), RefMom.at(l), RefMom.at(j), RefMom.at(k));
            }
            Double_t openingAngleBetweenGammas =
              CalculateOpeningAngleBetweenGammas_MC(MC.at(i), MC.at(j), MC.at(k), MC.at(l));
            gg[6]->Fill(openingAngleBetweenGammas);
            gg[7]->Fill(openingAngleBetweenGammasReco);
          }
        }
      }
    }
  }
}


void CbmKresEtaMCAnalysis::EtaChargedPionsGammaAnalysis(vector<TVector3> RefMomPion, vector<CbmMCTrack*> MCPion,
                                                        vector<Int_t> /*IdPion*/, vector<TVector3> RefMomEl,
                                                        vector<CbmMCTrack*> MCEl, vector<Int_t> /*IdEl*/,
                                                        vector<TH1*> ppg)
{
  //================================== decay eta -> p+p- gamma -> p+p- e+e-
  if (MCPion.size() < 2 || MCEl.size() < 2) return;

  //	int fDecayedParticlePdg = 221;

  for (size_t i = 0; i < MCEl.size(); i++) {
    for (size_t j = i + 1; j < MCEl.size(); j++) {
      int pdg1 = MCEl.at(i)->GetPdgCode();
      int pdg2 = MCEl.at(j)->GetPdgCode();

      if (pdg1 + pdg2 != 0) continue;
      if (TMath::Abs(pdg1) != 11 || TMath::Abs(pdg2) != 11) continue;

      int motherId1 = MCEl.at(i)->GetMotherId();
      int motherId2 = MCEl.at(j)->GetMotherId();

      if (motherId1 != motherId2) continue;
      if (motherId1 == -1 || motherId2 == -1) continue;

      CbmMCTrack* mother1 = (CbmMCTrack*) fMcTracks->At(motherId1);

      if (mother1->GetPdgCode() != 22) continue;

      int grandmotherId1 = mother1->GetMotherId();

      if (grandmotherId1 == -1) continue;

      CbmMCTrack* GrMother1 = (CbmMCTrack*) fMcTracks->At(grandmotherId1);
      int mcGrandmotherPdg1 = GrMother1->GetPdgCode();
      if (mcGrandmotherPdg1 != 221) continue;


      for (size_t k = 0; k < MCPion.size(); k++) {
        for (size_t l = k + 1; l < MCPion.size(); l++) {
          int pdg3 = MCPion.at(k)->GetPdgCode();
          int pdg4 = MCPion.at(l)->GetPdgCode();

          if (pdg3 + pdg4 != 0) continue;
          if (TMath::Abs(pdg3) != 211 || TMath::Abs(pdg4) != 211) continue;

          int motherId3 = MCPion.at(k)->GetMotherId();
          int motherId4 = MCPion.at(l)->GetMotherId();

          if (motherId3 != motherId4) continue;
          if (motherId3 == -1 || motherId4 == -1) continue;
          if (motherId3 != grandmotherId1) continue;

          Double_t InvMass_true =
            CbmKresFunctions::Invmass_4particles_MC(MCEl.at(i), MCEl.at(j), MCPion.at(k), MCPion.at(l));
          Double_t InvMass_reco = CbmKresFunctions::Invmass_2el_2pions_RECO(RefMomEl.at(i), RefMomEl.at(j),
                                                                            RefMomPion.at(k), RefMomPion.at(l));
          cout << "Decay eta -> p+p- gamma -> p+p- e+e- detected!\t\t mc mass: " << InvMass_true
               << "\t, reco mass: " << InvMass_reco << endl;
          cout << "motherids: " << motherId1 << "/" << motherId2 << "/" << motherId3 << "/" << motherId4 << endl;
          cout << "grandmotherid: " << grandmotherId1 << endl;
          cout << "pdgs: " << mcGrandmotherPdg1 << "-->" << mother1->GetPdgCode() << "/" << pdg3 << "/" << pdg4 << "-->"
               << pdg1 << "/" << pdg2 << "/" << pdg3 << "/" << pdg4 << endl;

          ppg[0]->Fill(InvMass_true);
          ppg[1]->Fill(InvMass_reco);

          cout << "\t \t mc true info: " << endl;
          cout << "particle 1: \t" << MCEl.at(i)->GetPdgCode() << ";\t pt = " << MCEl.at(i)->GetPt()
               << ";\t X = " << MCEl.at(i)->GetStartX() << ";\t Y = " << MCEl.at(i)->GetStartY()
               << ";\t Z = " << MCEl.at(i)->GetStartZ() << ";\t E = " << MCEl.at(i)->GetEnergy() << endl;
          cout << "particle 2: \t" << MCEl.at(j)->GetPdgCode() << ";\t pt = " << MCEl.at(j)->GetPt()
               << ";\t X = " << MCEl.at(j)->GetStartX() << ";\t Y = " << MCEl.at(j)->GetStartY()
               << ";\t Z = " << MCEl.at(j)->GetStartZ() << ";\t E = " << MCEl.at(j)->GetEnergy() << endl;
          cout << "particle 3: \t" << MCPion.at(k)->GetPdgCode() << ";\t pt = " << MCPion.at(k)->GetPt()
               << ";\t X = " << MCPion.at(k)->GetStartX() << ";\t Y = " << MCPion.at(k)->GetStartY()
               << ";\t Z = " << MCPion.at(k)->GetStartZ() << ";\t E = " << MCPion.at(k)->GetEnergy() << endl;
          cout << "particle 4: \t" << MCPion.at(l)->GetPdgCode() << ";\t pt = " << MCPion.at(l)->GetPt()
               << ";\t X = " << MCPion.at(l)->GetStartX() << ";\t Y = " << MCPion.at(l)->GetStartY()
               << ";\t Z = " << MCPion.at(l)->GetStartZ() << ";\t E = " << MCPion.at(l)->GetEnergy() << endl;

          double InvMass_g_mc            = CbmKresFunctions::Invmass_2particles_MC(MCEl.at(i), MCEl.at(j));
          double InvMass_g_reffited      = CbmKresFunctions::Invmass_2particles_RECO(RefMomEl.at(i), RefMomEl.at(j));
          double OpeningAngle_g_mc       = CbmKresFunctions::CalculateOpeningAngle_MC(MCEl.at(i), MCEl.at(j));
          double OpeningAngle_g_refitted = CbmKresFunctions::CalculateOpeningAngle_Reco(RefMomEl.at(i), RefMomEl.at(j));

          ppg[2]->Fill(InvMass_g_mc);
          ppg[3]->Fill(InvMass_g_reffited);
          ppg[4]->Fill(OpeningAngle_g_mc);
          ppg[5]->Fill(OpeningAngle_g_refitted);
        }
      }
    }
  }
}


void CbmKresEtaMCAnalysis::EtaPosNegNeutralPionsAnalysis(vector<TVector3> RefMomNeutral, vector<CbmMCTrack*> MCNeutral,
                                                         vector<Int_t> IdNeutral, vector<TVector3> RefMomPion,
                                                         vector<CbmMCTrack*> MCPion, vector<Int_t> /*IdPion*/,
                                                         vector<TH1*> ppp)
{
  if (MCNeutral.size() < 4 || MCPion.size() < 2) return;

  int fDecayedParticlePdg = 221;

  // cout << "!!!!!!!!!! MCNeutral.size() = " << MCNeutral.size() << endl;

  for (size_t i = 0; i < MCNeutral.size(); i++) {
    for (size_t j = i + 1; j < MCNeutral.size(); j++) {
      for (size_t k = j + 1; k < MCNeutral.size(); k++) {
        for (size_t l = k + 1; l < MCNeutral.size(); l++) {

          int pdg1 = MCNeutral.at(i)->GetPdgCode();
          int pdg2 = MCNeutral.at(j)->GetPdgCode();
          int pdg3 = MCNeutral.at(k)->GetPdgCode();
          int pdg4 = MCNeutral.at(l)->GetPdgCode();

          if (pdg1 + pdg2 + pdg3 + pdg4 != 0) continue;
          if (TMath::Abs(pdg1) != 11 || TMath::Abs(pdg2) != 11 || TMath::Abs(pdg3) != 11 || TMath::Abs(pdg4) != 11)
            continue;

          int motherId1 = MCNeutral.at(i)->GetMotherId();
          int motherId2 = MCNeutral.at(j)->GetMotherId();
          int motherId3 = MCNeutral.at(k)->GetMotherId();
          int motherId4 = MCNeutral.at(l)->GetMotherId();

          int mcId1 = IdNeutral.at(i);
          int mcId2 = IdNeutral.at(j);
          int mcId3 = IdNeutral.at(k);
          int mcId4 = IdNeutral.at(l);

          if (mcId1 == mcId2 || mcId1 == mcId3 || mcId1 == mcId4 || mcId2 == mcId3 || mcId2 == mcId4 || mcId3 == mcId4)
            continue;  // particle is not used twice
          if (motherId1 == -1 || motherId2 == -1 || motherId3 == -1 || motherId4 == -1) continue;

          CbmMCTrack* mother1 = (CbmMCTrack*) fMcTracks->At(motherId1);
          CbmMCTrack* mother2 = (CbmMCTrack*) fMcTracks->At(motherId2);
          CbmMCTrack* mother3 = (CbmMCTrack*) fMcTracks->At(motherId3);
          CbmMCTrack* mother4 = (CbmMCTrack*) fMcTracks->At(motherId4);

          int mcMotherPdg1 = mother1->GetPdgCode();
          int mcMotherPdg2 = mother2->GetPdgCode();
          int mcMotherPdg3 = mother3->GetPdgCode();
          int mcMotherPdg4 = mother4->GetPdgCode();

          if (mcMotherPdg1 != 22 || mcMotherPdg2 != 22 || mcMotherPdg3 != 22 || mcMotherPdg4 != 22) continue;

          int grandmotherId1 = mother1->GetMotherId();
          int grandmotherId2 = mother2->GetMotherId();
          int grandmotherId3 = mother3->GetMotherId();
          int grandmotherId4 = mother4->GetMotherId();

          if (grandmotherId1 == -1) continue;

          if (grandmotherId1 == grandmotherId2 && grandmotherId1 == grandmotherId3
              && grandmotherId1 == grandmotherId4) {
            CbmMCTrack* GrMother1 = (CbmMCTrack*) fMcTracks->At(grandmotherId1);
            int mcGrandmotherPdg1 = GrMother1->GetPdgCode();
            if (mcGrandmotherPdg1 != 111) continue;
            // cout << "*****************   111      ****************" << endl;
            if (GrMother1->GetMotherId() == -1) continue;
            CbmMCTrack* TopTrack = (CbmMCTrack*) fMcTracks->At(GrMother1->GetMotherId());
            if (TopTrack->GetPdgCode() != fDecayedParticlePdg) continue;
            // cout << "*****************   from eta !!!!!   ****************" << endl;

            for (size_t m = 0; m < MCPion.size(); m++) {
              for (size_t s = m + 1; s < MCPion.size(); s++) {
                int pdg5 = MCPion.at(m)->GetPdgCode();
                int pdg6 = MCPion.at(s)->GetPdgCode();

                if (pdg5 + pdg6 != 0) continue;
                if (TMath::Abs(pdg5) != 211 || TMath::Abs(pdg6) != 211) continue;

                int motherId5 = MCPion.at(m)->GetMotherId();
                int motherId6 = MCPion.at(s)->GetMotherId();

                if (motherId5 != motherId6) continue;
                if (motherId5 == -1 || motherId6 == -1) continue;
                if (motherId5 != GrMother1->GetMotherId()) continue;

                Double_t InvMass_six_true = CbmKresFunctions::Invmass_6particles_MC(
                  MCNeutral.at(i), MCNeutral.at(j), MCNeutral.at(k), MCNeutral.at(l), MCPion.at(m), MCPion.at(s));
                Double_t InvMass_six_reco = CbmKresFunctions::Invmass_4el_2pions_RECO(
                  RefMomNeutral.at(i), RefMomNeutral.at(j), RefMomNeutral.at(k), RefMomNeutral.at(l), RefMomPion.at(m),
                  RefMomPion.at(s));
                cout << "Decay eta -> p+p- p0 -> p+p- e+e-e+e- detected!\t\t "
                        "mc mass: "
                     << InvMass_six_true << "\t, reco mass: " << InvMass_six_reco << endl;
                cout << "motherids for neutral: " << motherId1 << "/" << motherId2 << "/" << motherId3 << "/"
                     << motherId4 << endl;
                cout << "grandmotherid: " << GrMother1->GetMotherId() << "/" << motherId5 << "/" << motherId6 << endl;
                cout << "pdgs: " << TopTrack->GetPdgCode() << "-->" << GrMother1->GetPdgCode() << "/" << pdg5 << "/"
                     << pdg6 << endl;

                ppp[0]->Fill(InvMass_six_true);
                ppp[1]->Fill(InvMass_six_reco);

                cout << "\t \t mc true info: " << endl;
                cout << "particle 1: \t" << MCNeutral.at(i)->GetPdgCode() << ";\t pt = " << MCNeutral.at(i)->GetPt()
                     << ";\t X = " << MCNeutral.at(i)->GetStartX() << ";\t Y = " << MCNeutral.at(i)->GetStartY()
                     << ";\t Z = " << MCNeutral.at(i)->GetStartZ() << ";\t E = " << MCNeutral.at(i)->GetEnergy()
                     << endl;
                cout << "particle 2: \t" << MCNeutral.at(j)->GetPdgCode() << ";\t pt = " << MCNeutral.at(j)->GetPt()
                     << ";\t X = " << MCNeutral.at(j)->GetStartX() << ";\t Y = " << MCNeutral.at(j)->GetStartY()
                     << ";\t Z = " << MCNeutral.at(j)->GetStartZ() << ";\t E = " << MCNeutral.at(j)->GetEnergy()
                     << endl;
                cout << "particle 3: \t" << MCNeutral.at(k)->GetPdgCode() << ";\t pt = " << MCNeutral.at(k)->GetPt()
                     << ";\t X = " << MCNeutral.at(k)->GetStartX() << ";\t Y = " << MCNeutral.at(k)->GetStartY()
                     << ";\t Z = " << MCNeutral.at(k)->GetStartZ() << ";\t E = " << MCNeutral.at(k)->GetEnergy()
                     << endl;
                cout << "particle 4: \t" << MCNeutral.at(l)->GetPdgCode() << ";\t pt = " << MCNeutral.at(l)->GetPt()
                     << ";\t X = " << MCNeutral.at(l)->GetStartX() << ";\t Y = " << MCNeutral.at(l)->GetStartY()
                     << ";\t Z = " << MCNeutral.at(l)->GetStartZ() << ";\t E = " << MCNeutral.at(l)->GetEnergy()
                     << endl;
                cout << "particle 5: \t" << MCPion.at(m)->GetPdgCode() << ";\t pt = " << MCPion.at(m)->GetPt()
                     << ";\t X = " << MCPion.at(m)->GetStartX() << ";\t Y = " << MCPion.at(m)->GetStartY()
                     << ";\t Z = " << MCPion.at(m)->GetStartZ() << ";\t E = " << MCPion.at(m)->GetEnergy() << endl;
                cout << "particle 6: \t" << MCPion.at(s)->GetPdgCode() << ";\t pt = " << MCPion.at(s)->GetPt()
                     << ";\t X = " << MCPion.at(s)->GetStartX() << ";\t Y = " << MCPion.at(s)->GetStartY()
                     << ";\t Z = " << MCPion.at(s)->GetStartZ() << ";\t E = " << MCPion.at(s)->GetEnergy() << endl;

                Double_t InvMass_true = CbmKresFunctions::Invmass_4particles_MC(MCNeutral.at(i), MCNeutral.at(j),
                                                                                MCNeutral.at(k), MCNeutral.at(l));
                Double_t InvMass_reco = CbmKresFunctions::Invmass_4particles_RECO(
                  RefMomNeutral.at(i), RefMomNeutral.at(j), RefMomNeutral.at(k), RefMomNeutral.at(l));
                ppp[2]->Fill(InvMass_true);
                ppp[3]->Fill(InvMass_reco);
              }
            }
          }
        }
      }
    }
  }
}


void CbmKresEtaMCAnalysis::EtaDoubleGammaAnalysis_plusBG(double OpeningAngleCut, double GammaInvMassCut, int Event,
                                                         vector<TVector3> RefMom, vector<CbmMCTrack*> MC,
                                                         vector<Int_t> Id, vector<TH1*> gg)
{
  if (MC.size() < 4) return;

  EDGA_RefMom.clear();
  EDGA_MC.clear();
  EDGA_Id.clear();

  for (size_t i = 0; i < MC.size(); i++) {
    for (size_t j = i + 1; j < MC.size(); j++) {
      int pdg1 = MC.at(i)->GetPdgCode();
      int pdg2 = MC.at(j)->GetPdgCode();
      if (pdg1 + pdg2 != 0) continue;
      if (TMath::Abs(pdg1) != 11 || TMath::Abs(pdg2) != 11) continue;

      double OpeningAngle_g_refitted = CbmKresFunctions::CalculateOpeningAngle_Reco(RefMom.at(i), RefMom.at(j));
      double InvMass_g_refitted      = CbmKresFunctions::Invmass_2particles_RECO(RefMom.at(i), RefMom.at(j));

      if (OpeningAngle_g_refitted > OpeningAngleCut || InvMass_g_refitted > GammaInvMassCut) continue;

      frefmomenta.clear();
      frefmomenta.push_back(RefMom.at(i));
      frefmomenta.push_back(RefMom.at(j));
      fMCtracks.clear();
      fMCtracks.push_back(MC.at(i));
      fMCtracks.push_back(MC.at(j));
      fMCId.clear();
      fMCId.push_back(Id.at(i));
      fMCId.push_back(Id.at(j));

      EDGA_RefMom.push_back(frefmomenta);
      EDGA_MC.push_back(fMCtracks);
      EDGA_Id.push_back(fMCId);

      // for event mixing gg channel
      EMT_gg_Event.push_back(Event);
      EMT_gg_pair_momenta.push_back(frefmomenta);
    }
  }

  if (EDGA_MC.size() < 2) return;  // min 2 gammas to form eta are required
  for (size_t gamma1 = 0; gamma1 < EDGA_RefMom.size(); gamma1++) {
    for (size_t gamma2 = gamma1 + 1; gamma2 < EDGA_RefMom.size(); gamma2++) {
      TVector3 e11 = EDGA_RefMom[gamma1][0];
      TVector3 e12 = EDGA_RefMom[gamma1][1];
      TVector3 e21 = EDGA_RefMom[gamma2][0];
      TVector3 e22 = EDGA_RefMom[gamma2][1];

      // MC true data for this particles
      CbmMCTrack* mcTrack1 = EDGA_MC[gamma1][0];
      CbmMCTrack* mcTrack2 = EDGA_MC[gamma1][1];
      CbmMCTrack* mcTrack3 = EDGA_MC[gamma2][0];
      CbmMCTrack* mcTrack4 = EDGA_MC[gamma2][1];

      int mcId1 = EDGA_Id[gamma1][0];
      int mcId2 = EDGA_Id[gamma1][1];
      int mcId3 = EDGA_Id[gamma2][0];
      int mcId4 = EDGA_Id[gamma2][1];

      if (mcId1 == mcId2 || mcId1 == mcId3 || mcId1 == mcId4 || mcId2 == mcId3 || mcId2 == mcId4 || mcId3 == mcId4)
        continue;  // particle is not used twice

      double openingAngleBetweenGammasReco = CalculateOpeningAngleBetweenGammas_Reco(e11, e12, e21, e22);
      if (openingAngleBetweenGammasReco < 10 || openingAngleBetweenGammasReco > 40) continue;

      double InvMass_true           = CbmKresFunctions::Invmass_4particles_MC(mcTrack1, mcTrack2, mcTrack3, mcTrack4);
      double InvMass_reco           = CbmKresFunctions::Invmass_4particles_RECO(e11, e12, e21, e22);
      CbmLmvmKinematicParams params = CbmKresFunctions::CalculateKinematicParams_4particles(e11, e12, e21, e22);

      gg[8]->Fill(InvMass_true);
      gg[9]->Fill(InvMass_reco);

      int CorrectEta = 0;
      if ((mcTrack1->GetMotherId() == mcTrack2->GetMotherId() && mcTrack3->GetMotherId() == mcTrack4->GetMotherId())
          || (mcTrack1->GetMotherId() == mcTrack3->GetMotherId() && mcTrack2->GetMotherId() == mcTrack4->GetMotherId())
          || (mcTrack1->GetMotherId() == mcTrack4->GetMotherId()
              && mcTrack2->GetMotherId() == mcTrack3->GetMotherId())) {
        CbmMCTrack* MothTrack1 = (CbmMCTrack*) fMcTracks->At(mcTrack1->GetMotherId());
        CbmMCTrack* MothTrack2 = (CbmMCTrack*) fMcTracks->At(mcTrack2->GetMotherId());
        CbmMCTrack* MothTrack3 = (CbmMCTrack*) fMcTracks->At(mcTrack3->GetMotherId());
        CbmMCTrack* MothTrack4 = (CbmMCTrack*) fMcTracks->At(mcTrack4->GetMotherId());
        if (MothTrack1->GetPdgCode() == 22 && MothTrack2->GetPdgCode() == 22 && MothTrack3->GetPdgCode() == 22
            && MothTrack4->GetPdgCode() == 22 && MothTrack1->GetMotherId() != -1
            && MothTrack1->GetMotherId() == MothTrack2->GetMotherId()
            && MothTrack1->GetMotherId() == MothTrack3->GetMotherId()
            && MothTrack1->GetMotherId() == MothTrack4->GetMotherId()) {
          CbmMCTrack* GrandMothTrack1 = (CbmMCTrack*) fMcTracks->At(MothTrack1->GetMotherId());
          if (GrandMothTrack1->GetPdgCode() == 221) CorrectEta = 1;
        }
      }

      if (CorrectEta == 1) {
        InvMass_eta_gg_reco_aftercuts->Fill(InvMass_reco);
        rap_vs_pt_eta_gg_reco_aftercuts->Fill(params.fRapidity, params.fPt);
      }
      else {
        rap_vs_pt_NOTeta_gg_reco_aftercuts->Fill(params.fRapidity, params.fPt);
      }
    }
  }
}

void CbmKresEtaMCAnalysis::EtaChargedPionsGammaAnalysis_plusBG(double OpeningAngleCut, double GammaInvMassCut,
                                                               int Event, vector<TVector3> RefMomPion,
                                                               vector<CbmMCTrack*> MCPion, vector<Int_t> /*IdPion*/,
                                                               vector<TVector3> RefMomEl, vector<CbmMCTrack*> MCEl,
                                                               vector<Int_t> IdEl, vector<TH1*> ppg, vector<TH1*> ppp)
{
  if (MCPion.size() < 2 || MCEl.size() < 2) return;

  ECPGA_leptons_RefMom.clear();
  ECPGA_leptons_MC.clear();
  ECPGA_leptons_Id.clear();

  for (size_t i = 0; i < MCEl.size(); i++) {
    for (size_t j = i + 1; j < MCEl.size(); j++) {
      int pdg1 = MCEl.at(i)->GetPdgCode();
      int pdg2 = MCEl.at(j)->GetPdgCode();
      if (pdg1 + pdg2 != 0) continue;
      if (TMath::Abs(pdg1) != 11 || TMath::Abs(pdg2) != 11) continue;

      double OpeningAngle_g_refitted = CbmKresFunctions::CalculateOpeningAngle_Reco(RefMomEl.at(i), RefMomEl.at(j));
      double InvMass_g_refitted      = CbmKresFunctions::Invmass_2particles_RECO(RefMomEl.at(i), RefMomEl.at(j));

      if (OpeningAngle_g_refitted > OpeningAngleCut || InvMass_g_refitted > GammaInvMassCut) continue;

      frefmomenta.clear();
      frefmomenta.push_back(RefMomEl.at(i));
      frefmomenta.push_back(RefMomEl.at(j));
      fMCtracks.clear();
      fMCtracks.push_back(MCEl.at(i));
      fMCtracks.push_back(MCEl.at(j));
      fMCId.clear();
      fMCId.push_back(IdEl.at(i));
      fMCId.push_back(IdEl.at(j));

      ECPGA_leptons_RefMom.push_back(frefmomenta);
      ECPGA_leptons_MC.push_back(fMCtracks);
      ECPGA_leptons_Id.push_back(fMCId);

      // for event mixing ppg channel
      EMT_ppg_ee_Event.push_back(Event);
      EMT_ppg_ee_pair_momenta.push_back(frefmomenta);
    }
  }

  ECPGA_pions_RefMom.clear();
  ECPGA_pions_MC.clear();
  for (size_t k = 0; k < MCPion.size(); k++) {
    if (MCPion.at(k)->GetPdgCode() == 211) {
      EMT_ppg_positive_pion_Event.push_back(Event);
      EMT_ppg_positive_pion_momenta.push_back(RefMomPion.at(k));
    }
    else {
      EMT_ppg_negative_pion_Event.push_back(Event);
      EMT_ppg_negative_pion_momenta.push_back(RefMomPion.at(k));
    }
    for (size_t l = k + 1; l < MCPion.size(); l++) {
      int pdg3 = MCPion.at(k)->GetPdgCode();
      int pdg4 = MCPion.at(l)->GetPdgCode();
      if (pdg3 + pdg4 != 0) continue;
      if (TMath::Abs(pdg3) != 211 || TMath::Abs(pdg4) != 211) continue;

      double Pions_angle_true = CbmKresFunctions::CalculateOpeningAngleBetweenPions_MC(MCPion.at(k), MCPion.at(l));
      double Pions_angle_reco =
        CbmKresFunctions::CalculateOpeningAngleBetweenPions_Reco(RefMomPion.at(k), RefMomPion.at(l));

      if (Pions_angle_reco > 20) continue;

      CbmMCTrack* mcTrack3 = MCPion.at(k);
      CbmMCTrack* mcTrack4 = MCPion.at(l);

      frefmomenta.clear();
      frefmomenta.push_back(RefMomPion.at(k));
      frefmomenta.push_back(RefMomPion.at(l));

      fMCtracks.clear();
      fMCtracks.push_back(MCPion.at(k));
      fMCtracks.push_back(MCPion.at(l));

      ECPGA_pions_RefMom.push_back(frefmomenta);
      ECPGA_pions_MC.push_back(fMCtracks);

      // for event mixing ppg channel
      EMT_ppg_pp_Event.push_back(Event);
      EMT_ppg_pp_pair_momenta.push_back(frefmomenta);

      // if (pdg3 == 211){
      // 	EMT_ppg_positive_pion_Event.push_back(Event);
      // 	EMT_ppg_positive_pion_momenta.push_back(RefMomPion.at(k));
      // 	EMT_ppg_negative_pion_Event.push_back(Event);
      // 	EMT_ppg_negative_pion_momenta.push_back(RefMomPion.at(l));
      // }
      // if (pdg4 == 211){
      // 	EMT_ppg_negative_pion_Event.push_back(Event);
      // 	EMT_ppg_negative_pion_momenta.push_back(RefMomPion.at(k));
      // 	EMT_ppg_positive_pion_Event.push_back(Event);
      // 	EMT_ppg_positive_pion_momenta.push_back(RefMomPion.at(l));
      // }

      int correctPionspair = 0;
      if (mcTrack3->GetMotherId() == mcTrack4->GetMotherId() && mcTrack4->GetMotherId() != -1) {
        CbmMCTrack* EtaTrack = (CbmMCTrack*) fMcTracks->At(mcTrack4->GetMotherId());
        if (EtaTrack->GetPdgCode() == 221) correctPionspair = 1;
      }

      if (correctPionspair == 1) {
        OA_betweenPions_fromEta_mc->Fill(Pions_angle_true);
        OA_betweenPions_fromEta_reco->Fill(Pions_angle_reco);
      }
      else {
        OA_betweenPions_fromEta_reco_wrongcombinations->Fill(Pions_angle_reco);
      }
    }
  }


  if (ECPGA_leptons_MC.size() < 1) return;  // min 1 gamma to form eta is required
  for (size_t gamma = 0; gamma < ECPGA_leptons_RefMom.size(); gamma++) {
    if (ECPGA_pions_MC.size() < 1) continue;  // min 1 p+p- pair to form eta is required
    TVector3 e11 = ECPGA_leptons_RefMom[gamma][0];
    TVector3 e12 = ECPGA_leptons_RefMom[gamma][1];

    CbmMCTrack* mcTrack1 = ECPGA_leptons_MC[gamma][0];
    CbmMCTrack* mcTrack2 = ECPGA_leptons_MC[gamma][1];

    for (size_t pionpair = 0; pionpair < ECPGA_pions_RefMom.size(); pionpair++) {

      CbmMCTrack* mcTrack3 = ECPGA_pions_MC[pionpair][0];
      CbmMCTrack* mcTrack4 = ECPGA_pions_MC[pionpair][1];

      TVector3 e21 = ECPGA_pions_RefMom[pionpair][0];
      TVector3 e22 = ECPGA_pions_RefMom[pionpair][1];

      Double_t InvMass_true         = CbmKresFunctions::Invmass_4particles_MC(mcTrack1, mcTrack2, mcTrack3, mcTrack4);
      Double_t InvMass_reco         = CbmKresFunctions::Invmass_2el_2pions_RECO(e11, e12, e21, e22);
      CbmLmvmKinematicParams params = CbmKresFunctions::CalculateKinematicParams_2el_2pions(e11, e12, e21, e22);

      ppg[6]->Fill(InvMass_true);
      ppg[7]->Fill(InvMass_reco);

      int CorrectEta = 0;
      if (mcTrack1->GetMotherId() == mcTrack2->GetMotherId() && mcTrack1->GetMotherId() != -1) {
        CbmMCTrack* GammaTrack = (CbmMCTrack*) fMcTracks->At(mcTrack1->GetMotherId());
        if (GammaTrack->GetPdgCode() == 22 && GammaTrack->GetMotherId() != -1
            && mcTrack3->GetMotherId() == mcTrack4->GetMotherId()
            && mcTrack3->GetMotherId() == GammaTrack->GetMotherId()) {
          CbmMCTrack* EtaTrack = (CbmMCTrack*) fMcTracks->At(GammaTrack->GetMotherId());
          if (EtaTrack->GetPdgCode() == 221) CorrectEta = 1;
        }
      }

      if (CorrectEta == 1) {
        InvMass_eta_ppg_reco_aftercuts->Fill(InvMass_reco);
        rap_vs_pt_eta_ppg_reco_aftercuts->Fill(params.fRapidity, params.fPt);
      }
      else {
        rap_vs_pt_NOTeta_ppg_reco_aftercuts->Fill(params.fRapidity, params.fPt);
      }
    }
  }


  ////////    case with p+p-p0
  if (ECPGA_leptons_MC.size() < 2)
    return;  // min 2 gammas to form pion is required in EtaPosNegNeutralPionsAnalysis_plusBG
  for (size_t gamma1 = 0; gamma1 < ECPGA_leptons_RefMom.size(); gamma1++) {
    for (size_t gamma2 = gamma1 + 1; gamma2 < ECPGA_leptons_RefMom.size(); gamma2++) {
      TVector3 e11 = ECPGA_leptons_RefMom[gamma1][0];
      TVector3 e12 = ECPGA_leptons_RefMom[gamma1][1];
      TVector3 e21 = ECPGA_leptons_RefMom[gamma2][0];
      TVector3 e22 = ECPGA_leptons_RefMom[gamma2][1];

      CbmMCTrack* mcTrack1 = ECPGA_leptons_MC[gamma1][0];
      CbmMCTrack* mcTrack2 = ECPGA_leptons_MC[gamma1][1];
      CbmMCTrack* mcTrack3 = ECPGA_leptons_MC[gamma2][0];
      CbmMCTrack* mcTrack4 = ECPGA_leptons_MC[gamma2][1];

      int mcId1 = ECPGA_leptons_Id[gamma1][0];
      int mcId2 = ECPGA_leptons_Id[gamma1][1];
      int mcId3 = ECPGA_leptons_Id[gamma2][0];
      int mcId4 = ECPGA_leptons_Id[gamma2][1];

      if (mcId1 == mcId2 || mcId1 == mcId3 || mcId1 == mcId4 || mcId2 == mcId3 || mcId2 == mcId4 || mcId3 == mcId4)
        continue;  // particle is not used twice

      //			Double_t InvMass_pi_true = CbmKresFunctions::Invmass_4particles_MC(mcTrack1, mcTrack2, mcTrack3, mcTrack4);
      CbmKresFunctions::Invmass_4particles_MC(mcTrack1, mcTrack2, mcTrack3, mcTrack4);
      Double_t InvMass_pi_reco = CbmKresFunctions::Invmass_4particles_RECO(e11, e12, e21, e22);

      if (InvMass_pi_reco < 0.12 || InvMass_pi_reco > 0.15) continue;

      for (size_t k = 0; k < MCPion.size(); k++) {
        for (size_t l = k + 1; l < MCPion.size(); l++) {
          int pdg5 = MCPion.at(k)->GetPdgCode();
          int pdg6 = MCPion.at(l)->GetPdgCode();

          if (pdg5 + pdg6 != 0) continue;
          if (TMath::Abs(pdg5) != 211 || TMath::Abs(pdg6) != 211) continue;

          double InvMass_six_true =
            CbmKresFunctions::Invmass_6particles_MC(mcTrack1, mcTrack2, mcTrack3, mcTrack4, MCPion.at(k), MCPion.at(l));
          double InvMass_six_reco =
            CbmKresFunctions::Invmass_4el_2pions_RECO(e11, e12, e21, e22, RefMomPion.at(k), RefMomPion.at(l));
          double Pions_angle_reco =
            CbmKresFunctions::CalculateOpeningAngleBetweenPions_Reco(RefMomPion.at(k), RefMomPion.at(l));

          if (Pions_angle_reco > 20) continue;

          ppp[4]->Fill(InvMass_six_true);
          ppp[5]->Fill(InvMass_six_reco);
        }
      }
    }
  }
}


void CbmKresEtaMCAnalysis::Mixing_gg()
{
  int nof = EMT_gg_Event.size();
  cout << "Mixing for  eta->gg  channel - nof entries " << nof << endl;
  for (Int_t a = 0; a < nof - 1; a++) {
    for (Int_t b = a + 1; b < nof; b++) {
      if (EMT_gg_Event[a] == EMT_gg_Event[b]) continue;  // to make sure that the photons are from two different events
      TVector3 e11 = EMT_gg_pair_momenta[a][0];
      TVector3 e12 = EMT_gg_pair_momenta[a][1];
      TVector3 e21 = EMT_gg_pair_momenta[b][0];
      TVector3 e22 = EMT_gg_pair_momenta[b][1];

      double openingAngleBetweenGammasReco = CalculateOpeningAngleBetweenGammas_Reco(e11, e12, e21, e22);
      if (openingAngleBetweenGammasReco < 10 || openingAngleBetweenGammasReco > 40) continue;

      CbmLmvmKinematicParams params = CbmKresFunctions::CalculateKinematicParams_4particles(e11, e12, e21, e22);
      EMT_eta_gg->Fill(params.fMinv);
    }
  }
}

void CbmKresEtaMCAnalysis::Mixing_ppg()
{
  int nof_leptons = EMT_ppg_ee_Event.size();
  int nof_pions   = EMT_ppg_pp_Event.size();
  cout << "Mixing for  eta->(pp)g  channel - nof lepton pairs = " << nof_leptons << ";  nof pion pairs = " << nof_pions
       << endl;
  for (Int_t a = 0; a < nof_leptons; a++) {
    for (Int_t b = 0; b < nof_pions; b++) {
      if (EMT_ppg_ee_Event[a] == EMT_ppg_pp_Event[b])
        continue;  // to make sure that the photons are from two different events
      TVector3 e11 = EMT_ppg_ee_pair_momenta[a][0];
      TVector3 e12 = EMT_ppg_ee_pair_momenta[a][1];
      TVector3 e21 = EMT_ppg_pp_pair_momenta[b][0];
      TVector3 e22 = EMT_ppg_pp_pair_momenta[b][1];

      CbmLmvmKinematicParams params = CbmKresFunctions::CalculateKinematicParams_2el_2pions(e11, e12, e21, e22);
      EMT_eta_ppg->Fill(params.fMinv);
    }
  }
}


void CbmKresEtaMCAnalysis::Mixing_three_body()
{
  int nof_photons  = EMT_ppg_ee_Event.size();
  int nof_positive = EMT_ppg_positive_pion_Event.size();
  int nof_negative = EMT_ppg_negative_pion_Event.size();
  cout << "Mixing 3 bodies for  eta-> p p g  channel - nof photons = " << nof_photons
       << ";  nof +pions = " << nof_positive << ";  nof -pions = " << nof_negative << endl;

  for (Int_t p = 0; p < nof_positive; p++) {
    for (Int_t m = 0; m < nof_negative; m++) {
      if (EMT_ppg_positive_pion_Event[p] == EMT_ppg_negative_pion_Event[m])
        continue;  // to make sure that the photons are from two different events
      TVector3 e21 = EMT_ppg_positive_pion_momenta[p];
      TVector3 e22 = EMT_ppg_negative_pion_momenta[m];

      double Pions_angle_reco = CbmKresFunctions::CalculateOpeningAngleBetweenPions_Reco(e21, e22);
      if (Pions_angle_reco > 20) continue;

      for (Int_t a = 0; a < nof_photons; a++) {
        if (EMT_ppg_ee_Event[a] == EMT_ppg_positive_pion_Event[p]
            || EMT_ppg_ee_Event[a] == EMT_ppg_negative_pion_Event[m])
          continue;  // to make sure that the photons are from two different events
        TVector3 e11 = EMT_ppg_ee_pair_momenta[a][0];
        TVector3 e12 = EMT_ppg_ee_pair_momenta[a][1];

        CbmLmvmKinematicParams params = CbmKresFunctions::CalculateKinematicParams_2el_2pions(e11, e12, e21, e22);
        EMT_eta_three_body->Fill(params.fMinv);
      }
    }
  }
}


void CbmKresEtaMCAnalysis::Finish()
{
  gDirectory->mkdir("EtaMCAnalysis");
  gDirectory->cd("EtaMCAnalysis");

  gDirectory->mkdir("eta_gg");
  gDirectory->cd("eta_gg");
  for (UInt_t i = 0; i < fHistoList_eta_gg.size(); i++) {
    fHistoList_eta_gg[i]->Write();
  }
  gDirectory->cd("..");

  gDirectory->mkdir("eta_ppg");
  gDirectory->cd("eta_ppg");
  for (UInt_t i = 0; i < fHistoList_eta_ppg.size(); i++) {
    fHistoList_eta_ppg[i]->Write();
  }
  gDirectory->cd("..");

  gDirectory->mkdir("eta_ppp");
  gDirectory->cd("eta_ppp");
  for (UInt_t i = 0; i < fHistoList_eta_ppp.size(); i++) {
    fHistoList_eta_ppp[i]->Write();
  }
  gDirectory->cd("..");


  gDirectory->cd("..");
}

void CbmKresEtaMCAnalysis::InitHistograms()
{
  ///////         eta -> gamma gamma -> e+e- e+e-
  InvMass_eta_gg_mc = new TH1D("InvMass_eta_gg_mc", "InvMass_eta_gg_mc; invariant mass in GeV/c^{2};#", 50, 0.3, 0.8);
  fHistoList_eta_gg.push_back(InvMass_eta_gg_mc);
  InvMass_eta_gg_reffited =
    new TH1D("InvMass_eta_gg_reffited", "InvMass_eta_gg_reffited; invariant mass in GeV/c^{2};#", 50, 0.3, 0.8);
  fHistoList_eta_gg.push_back(InvMass_eta_gg_reffited);
  InvMassPhoton_eta_gg_mc =
    new TH1D("InvMassPhoton_eta_gg_mc", "InvMassPhoton_eta_gg_mc; invariant mass in GeV/c^{2};#", 60, -0.01, 0.05);
  fHistoList_eta_gg.push_back(InvMassPhoton_eta_gg_mc);
  InvMassPhoton_eta_gg_reffited = new TH1D(
    "InvMassPhoton_eta_gg_reffited", "InvMassPhoton_eta_gg_reffited; invariant mass in GeV/c^{2};#", 60, -0.01, 0.05);
  fHistoList_eta_gg.push_back(InvMassPhoton_eta_gg_reffited);
  OpeningAnglePhoton_eta_gg_mc =
    new TH1D("OpeningAnglePhoton_eta_gg_mc", "OpeningAnglePhoton_eta_gg_mc (between e+e- from #gamma); angle [deg];#",
             100, -0.1, 9.9);
  fHistoList_eta_gg.push_back(OpeningAnglePhoton_eta_gg_mc);
  OpeningAnglePhoton_eta_gg_reffited = new TH1D("OpeningAnglePhoton_eta_gg_reffited",
                                                "OpeningAnglePhoton_eta_gg_reffited (between e+e- from #gamma); "
                                                "angle [deg];#",
                                                100, -0.1, 9.9);
  fHistoList_eta_gg.push_back(OpeningAnglePhoton_eta_gg_reffited);
  OpeningAngle_eta_gg_between_gg_mc = new TH1D("OpeningAngle_eta_gg_between_gg_mc",
                                               "OpeningAngle_eta_gg_between_gg_mc (between #gamma#gamma from "
                                               "#eta); angle [deg];#",
                                               900, -0.1, 89.9);
  fHistoList_eta_gg.push_back(OpeningAngle_eta_gg_between_gg_mc);
  OpeningAngle_eta_gg_between_gg_reffited = new TH1D("OpeningAngle_eta_gg_between_gg_reffited",
                                                     "OpeningAngle_eta_gg_between_gg_reffited (between #gamma#gamma "
                                                     "from #eta); angle [deg];#",
                                                     900, -0.1, 89.9);
  fHistoList_eta_gg.push_back(OpeningAngle_eta_gg_between_gg_reffited);
  InvMass_eta_gg_allcombinations_mc =
    new TH1D("InvMass_eta_gg_allcombinations_mc", "InvMass_eta_gg_allcombinations_mc; invariant mass in GeV/c^{2};#",
             200, 0.0, 2.0);
  fHistoList_eta_gg.push_back(InvMass_eta_gg_allcombinations_mc);
  InvMass_eta_gg_allcombinations_reffited =
    new TH1D("InvMass_eta_gg_allcombinations_reffited",
             "InvMass_eta_gg_allcombinations_reffited; invariant mass in GeV/c^{2};#", 200, 0.0, 2.0);
  fHistoList_eta_gg.push_back(InvMass_eta_gg_allcombinations_reffited);
  EMT_eta_gg = new TH1D("EMT_eta_gg", "EMT_eta_gg; invariant mass in GeV/c^{2};#", 200, 0.0, 2.0);
  fHistoList_eta_gg.push_back(EMT_eta_gg);
  InvMass_eta_gg_reco_aftercuts = new TH1D(
    "InvMass_eta_gg_reco_aftercuts", "InvMass_eta_gg_reco_aftercuts; invariant mass in GeV/c^{2};#", 50, 0.3, 0.8);
  fHistoList_eta_gg.push_back(InvMass_eta_gg_reco_aftercuts);
  rap_vs_pt_eta_gg_reco_aftercuts =
    new TH2D("rap_vs_pt_eta_gg_reco_aftercuts", "rap_vs_pt_eta_gg_reco_aftercuts; rapidity y; p_{t} in GeV/c ", 10, 0.,
             4., 5, 0., 2.);
  fHistoList_eta_gg.push_back(rap_vs_pt_eta_gg_reco_aftercuts);
  rap_vs_pt_NOTeta_gg_reco_aftercuts =
    new TH2D("rap_vs_pt_NOTeta_gg_reco_aftercuts", "rap_vs_pt_NOTeta_gg_reco_aftercuts; rapidity y; p_{t} in GeV/c ",
             10, 0., 4., 5, 0., 2.);
  fHistoList_eta_gg.push_back(rap_vs_pt_NOTeta_gg_reco_aftercuts);


  ///////         eta -> p+ + p- + gamma -> p+p- e+e-
  InvMass_eta_ppg_mc =
    new TH1D("InvMass_eta_ppg_mc", "InvMass_eta_ppg_mc; invariant mass in GeV/c^{2};#", 50, 0.3, 0.8);
  fHistoList_eta_ppg.push_back(InvMass_eta_ppg_mc);
  InvMass_eta_ppg_reffited =
    new TH1D("InvMass_eta_ppg_reffited", "InvMass_eta_ppg_reffited; invariant mass in GeV/c^{2};#", 50, 0.3, 0.8);
  fHistoList_eta_ppg.push_back(InvMass_eta_ppg_reffited);
  InvMassPhoton_eta_ppg_mc =
    new TH1D("InvMassPhoton_eta_ppg_mc", "InvMassPhoton_eta_ppg_mc; invariant mass in GeV/c^{2};#", 60, -0.01, 0.05);
  fHistoList_eta_ppg.push_back(InvMassPhoton_eta_ppg_mc);
  InvMassPhoton_eta_ppg_reffited = new TH1D(
    "InvMassPhoton_eta_ppg_reffited", "InvMassPhoton_eta_ppg_reffited; invariant mass in GeV/c^{2};#", 60, -0.01, 0.05);
  fHistoList_eta_ppg.push_back(InvMassPhoton_eta_ppg_reffited);
  OpeningAnglePhoton_eta_ppg_mc =
    new TH1D("OpeningAnglePhoton_eta_ppg_mc", "OpeningAnglePhoton_eta_ppg_mc (between e+e- from #gamma); angle [deg];#",
             100, -0.1, 9.9);
  fHistoList_eta_ppg.push_back(OpeningAnglePhoton_eta_ppg_mc);
  OpeningAnglePhoton_eta_ppg_reffited = new TH1D("OpeningAnglePhoton_eta_ppg_reffited",
                                                 "OpeningAnglePhoton_eta_ppg_reffited (between e+e- from #gamma); "
                                                 "angle [deg];#",
                                                 100, -0.1, 9.9);
  fHistoList_eta_ppg.push_back(OpeningAnglePhoton_eta_ppg_reffited);
  InvMass_eta_ppg_allcombinations_mc =
    new TH1D("InvMass_eta_ppg_allcombinations_mc", "InvMass_eta_ppg_allcombinations_mc; invariant mass in GeV/c^{2};#",
             200, 0.0, 2.0);
  fHistoList_eta_ppg.push_back(InvMass_eta_ppg_allcombinations_mc);
  InvMass_eta_ppg_allcombinations_reffited =
    new TH1D("InvMass_eta_ppg_allcombinations_reffited",
             "InvMass_eta_ppg_allcombinations_reffited; invariant mass in GeV/c^{2};#", 200, 0.0, 2.0);
  fHistoList_eta_ppg.push_back(InvMass_eta_ppg_allcombinations_reffited);
  Pion_P_fromEta_reco = new TH1D("Pion_P_fromEta_reco", "Pion_P_fromEta_reco; pions P in GeV/c;#", 500, 0.0, 5.0);
  fHistoList_eta_ppg.push_back(Pion_P_fromEta_reco);
  Pion_P_elsewhere_reco = new TH1D("Pion_P_elsewhere_reco", "Pion_P_elsewhere_reco; pions P in GeV/c;#", 500, 0.0, 5.0);
  fHistoList_eta_ppg.push_back(Pion_P_elsewhere_reco);
  Pion_Pt_fromEta_reco =
    new TH1D("Pion_Pt_fromEta_reco", "Pion_Pt_fromEta_reco; pions P_{t} in GeV/c;#", 500, 0.0, 5.0);
  fHistoList_eta_ppg.push_back(Pion_Pt_fromEta_reco);
  Pion_Pt_elsewhere_reco =
    new TH1D("Pion_Pt_elsewhere_reco", "Pion_Pt_elsewhere_reco; pions P_{t} in GeV/c;#", 500, 0.0, 5.0);
  fHistoList_eta_ppg.push_back(Pion_Pt_elsewhere_reco);
  OA_betweenPions_fromEta_mc = new TH1D("OA_betweenPions_fromEta_mc",
                                        "OA_betweenPions_fromEta_mc (between #p^{+}#p^{-} from #eta); "
                                        "angle [deg];#",
                                        900, -0.1, 89.9);
  fHistoList_eta_ppg.push_back(OA_betweenPions_fromEta_mc);
  OA_betweenPions_fromEta_reco = new TH1D("OA_betweenPions_fromEta_reco",
                                          "OA_betweenPions_fromEta_reco (between #p^{+}#p^{-} from #eta); "
                                          "angle [deg];#",
                                          900, -0.1, 89.9);
  fHistoList_eta_ppg.push_back(OA_betweenPions_fromEta_reco);
  OA_betweenPions_fromEta_reco_wrongcombinations = new TH1D("OA_betweenPions_fromEta_reco_wrongcombinations",
                                                            "OA_betweenPions_fromEta_reco_wrongcombinations (between "
                                                            "#p^{+}#p^{-} from #eta); angle [deg];#",
                                                            900, -0.1, 89.9);
  fHistoList_eta_ppg.push_back(OA_betweenPions_fromEta_reco_wrongcombinations);
  EMT_eta_ppg = new TH1D("EMT_eta_ppg", "EMT_eta_ppg; invariant mass in GeV/c^{2};#", 200, 0.0, 2.0);
  fHistoList_eta_ppg.push_back(EMT_eta_ppg);
  EMT_eta_three_body =
    new TH1D("EMT_eta_three_body", "EMT_eta_three_body; invariant mass in GeV/c^{2};#", 200, 0.0, 2.0);
  fHistoList_eta_ppg.push_back(EMT_eta_three_body);
  InvMass_eta_ppg_reco_aftercuts = new TH1D(
    "InvMass_eta_ppg_reco_aftercuts", "InvMass_eta_ppg_reco_aftercuts; invariant mass in GeV/c^{2};#", 50, 0.3, 0.8);
  fHistoList_eta_ppg.push_back(InvMass_eta_ppg_reco_aftercuts);
  rap_vs_pt_eta_ppg_reco_aftercuts =
    new TH2D("rap_vs_pt_eta_ppg_reco_aftercuts", "rap_vs_pt_eta_ppg_reco_aftercuts; rapidity y; p_{t} in GeV/c ", 10,
             0., 4., 5, 0., 2.);
  fHistoList_eta_ppg.push_back(rap_vs_pt_eta_ppg_reco_aftercuts);
  rap_vs_pt_NOTeta_ppg_reco_aftercuts =
    new TH2D("rap_vs_pt_NOTeta_ppg_reco_aftercuts", "rap_vs_pt_NOTeta_ppg_reco_aftercuts; rapidity y; p_{t} in GeV/c ",
             10, 0., 4., 5, 0., 2.);
  fHistoList_eta_ppg.push_back(rap_vs_pt_NOTeta_ppg_reco_aftercuts);


  ///////         eta -> p+ + p- + p0 -> p+p- (g+g) --> p+p- (e+e-) (e+e-)
  InvMass_eta_ppp_mc =
    new TH1D("InvMass_eta_ppp_mc", "InvMass_eta_ppp_mc; invariant mass in GeV/c^{2};#", 50, 0.3, 0.8);
  fHistoList_eta_ppp.push_back(InvMass_eta_ppp_mc);
  InvMass_eta_ppp_reffited =
    new TH1D("InvMass_eta_ppp_reffited", "InvMass_eta_ppp_reffited; invariant mass in GeV/c^{2};#", 50, 0.3, 0.8);
  fHistoList_eta_ppp.push_back(InvMass_eta_ppp_reffited);
  InvMass_eta_Npion_mc =
    new TH1D("InvMass_eta_Npion_mc", "InvMass_eta_Npion_mc; invariant mass in GeV/c^{2};#", 30, 0.0, 0.3);
  fHistoList_eta_ppp.push_back(InvMass_eta_Npion_mc);
  InvMass_eta_Npion_reffited =
    new TH1D("InvMass_eta_Npion_reffited", "InvMass_eta_Npion_reffited; invariant mass in GeV/c^{2};#", 30, 0., 0.3);
  fHistoList_eta_ppp.push_back(InvMass_eta_Npion_reffited);
  InvMass_eta_ppp_allcombinations_mc =
    new TH1D("InvMass_eta_ppp_allcombinations_mc", "InvMass_eta_ppp_allcombinations_mc; invariant mass in GeV/c^{2};#",
             200, 0.0, 2.0);
  fHistoList_eta_ppp.push_back(InvMass_eta_ppp_allcombinations_mc);
  InvMass_eta_ppp_allcombinations_reffited =
    new TH1D("InvMass_eta_ppp_allcombinations_reffited",
             "InvMass_eta_ppp_allcombinations_reffited; invariant mass in GeV/c^{2};#", 200, 0.0, 2.0);
  fHistoList_eta_ppp.push_back(InvMass_eta_ppp_allcombinations_reffited);
}
