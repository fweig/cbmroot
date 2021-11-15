/* Copyright (C) 2014-2020 Fakultaet fuer Mathematik und Naturwissenschaften, Bergische Universitaet Wuppertal, Wuppertal
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sascha Reinecke [committer], Florian Uhlig, Andrey Lebedev */

/**
 * \file CbmAnaConversionReco.cxx
 *
 * 
 * 
 *
 * \author Sascha Reinecke <reinecke@uni-wuppertal.de>
 * \date 2014
 **/

#include "CbmAnaConversionReco.h"

// standard includes
#include <iostream>

// includes from ROOT
#include "TRandom3.h"

// included from CbmRoot
#include "CbmAnaConversionCutSettings.h"
#include "CbmAnaConversionGlobalFunctions.h"
#include "CbmGlobalTrack.h"
#include "CbmMCTrack.h"
#include "CbmRichElectronIdAnn.h"
#include "CbmRichHit.h"
#include "CbmRichRing.h"
#include "CbmRichRingLight.h"
#include "CbmRichUtil.h"

#include "FairRootManager.h"

#define M2E 2.6112004954086e-7
using namespace std;


CbmAnaConversionReco::CbmAnaConversionReco()
  : fMcTracks(NULL)
  , fGlobalTracks(NULL)
  , fRichRings(NULL)
  , fRichHits(NULL)
  , fMCTracklist_all()
  , fRecoTracklistEPEM()
  , fRecoTracklistEPEM_ids()
  , fRecoTracklistEPEM_chi()
  , fRecoTracklistEPEM_gtid()
  , fRecoMomentum()
  , fRecoRefittedMomentum()
  , fHistoList_MC()
  , fHistoList_reco()
  , fHistoList_reco_mom()
  , fHistoList_gg()
  , fHistoList_gee()
  , fHistoList_eeee()
  , fHistoList_all()
  , fHistoList_eta()
  , fhInvariantMass_MC_all(NULL)
  , fhInvariantMass_MC_pi0(NULL)
  , fhInvariantMass_MC_pi0_epem(NULL)
  , fhInvariantMass_MC_pi0_gepem(NULL)
  , fhInvariantMass_MC_pi0_gg(NULL)
  , fhInvariantMass_MC_eta(NULL)
  , fhInvariantMass_MC_etaPrime(NULL)
  , fhMC_electrons_theta(NULL)
  , fhMC_electrons_p(NULL)
  , fhMC_electrons_theta_vs_p(NULL)
  , fhEta_openingAngleGG(NULL)
  , fhMC_grandmotherPDGs(NULL)
  , fhInvariantMassReco_pi0(NULL)
  , fhMCtest(NULL)
  , fhEPEM_invmass_gg_mc(NULL)
  , fhEPEM_invmass_gg_refitted(NULL)
  , fhEPEM_invmass_gee_mc(NULL)
  , fhEPEM_invmass_gee_refitted(NULL)
  , fhEPEM_invmass_eeee_mc(NULL)
  , fhEPEM_invmass_eeee_refitted(NULL)
  , fhEPEM_invmass_all_mc(NULL)
  , fhEPEM_invmass_all_refitted(NULL)
  , fhEPEM_openingAngle_gg_mc(NULL)
  , fhEPEM_openingAngle_gg_refitted(NULL)
  , fhEPEM_openingAngle_gee_mc(NULL)
  , fhEPEM_openingAngle_gee_refitted(NULL)
  , fhEPEM_openingAngle_gee_mc_dalitz(NULL)
  , fhEPEM_openingAngle_gee_refitted_dalitz(NULL)
  , fhEPEM_openingAngle_vs_pt_gg_mc(NULL)
  , fhEPEM_openingAngle_vs_pt_gg_reco(NULL)
  , fhEPEM_openingAngle_betweenGammas_mc(NULL)
  , fhEPEM_openingAngle_betweenGammas_reco(NULL)
  , fhPi0_pt_vs_rap_gg(NULL)
  , fhPi0_pt_vs_rap_gee(NULL)
  , fhPi0_pt_vs_rap_all(NULL)
  , fhPi0_pt_gg(NULL)
  , fhPi0_pt_gee(NULL)
  , fhPi0_pt_all(NULL)
  , fhEPEM_efficiencyCuts(NULL)
  , fhEPEM_efficiencyCuts2(NULL)
  , fhEPEM_pi0_nofLeptons_ann(NULL)
  , fhEPEM_pi0_ANNvalues_noCuts(NULL)
  , fhEPEM_pi0_ANNvalues_angleCut(NULL)
  , fhEPEM_pi0_ANNefficiencies(NULL)
  , fhEPEM_rap_vs_chi(NULL)
  , fhEPEM_rap_vs_invmass(NULL)
  , fhInvMass_EPEM_mc(NULL)
  , fhInvMass_EPEM_stsMomVec(NULL)
  , fhInvMass_EPEM_refitted(NULL)
  , fhInvMass_EPEM_error_stsMomVec(NULL)
  , fhInvMass_EPEM_error_refitted(NULL)
  , fhInvMass_EPEM_openingAngleRef(NULL)
  , fhUsedMomenta_stsMomVec(NULL)
  , fhUsedMomenta_mc(NULL)
  , fhUsedMomenta_error_stsMomVec(NULL)
  , fhUsedMomenta_error_refitted(NULL)
  , fhUsedMomenta_errorX_stsMomVec(NULL)
  , fhUsedMomenta_vsX_stsMomVec(NULL)
  , fhUsedMomenta_errorY_stsMomVec(NULL)
  , fhUsedMomenta_vsY_stsMomVec(NULL)
  , fhUsedMomenta_errorZ_stsMomVec(NULL)
  , fhUsedMomenta_vsZ_stsMomVec(NULL)
  , fhUsedMomenta_errorX_refitted(NULL)
  , fhUsedMomenta_vsX_refitted(NULL)
  , fhUsedMomenta_errorY_refitted(NULL)
  , fhUsedMomenta_vsY_refitted(NULL)
  , fhUsedMomenta_errorZ_refitted(NULL)
  , fhUsedMomenta_vsZ_refitted(NULL)
  , fhInvariantMass_pi0epem(NULL)
  , fhPi0_startvertex(NULL)
  , fhPi0_startvertexElectrons_all(NULL)
  , fhPi0_startvertexElectrons_gg(NULL)
  , fhPi0_startvertexElectrons_gee(NULL)
  , fhPi0_startvertex_vs_chi(NULL)
  , fhPi0_startvertex_vs_momentum(NULL)
  , fhInvMassWithFullRecoCuts(NULL)
  , fhEPEM_InDetector_invmass_gg_mc(NULL)
  , fhEPEM_InDetector_invmass_gg_refitted(NULL)
  , fhEPEM_InDetector_invmass_gee_mc(NULL)
  , fhEPEM_InDetector_invmass_gee_refitted(NULL)
  , fhEPEM_InDetector_invmass_all_mc(NULL)
  , fhEPEM_InDetector_invmass_all_refitted(NULL)
  , fhEPEM_pt_vs_p_all_mc(NULL)
  , fhEPEM_pt_vs_p_all_refitted(NULL)
  , fhEPEM_missingLepton_nofRingHits(NULL)
  , fhEPEM_missingLepton_ringMid(NULL)
  , fhEPEM_missingLepton_ringRadius(NULL)
  , fhEPEM_missingLepton_distance(NULL)
  , fhEPEM_missingLepton_selectionNN(NULL)
  , fhEPEM_missingLepton_rings(NULL)
  , fhEPEM_missingLepton_radius_vs_p(NULL)
  , fhEPEM_missingLepton_ANNvalue(NULL)
  , fhEPEM_identifiedLepton_nofRingHits(NULL)
  , fhEPEM_identifiedLepton_ringMid(NULL)
  , fhEPEM_identifiedLepton_ringRadius(NULL)
  , fhEPEM_identifiedLepton_distance(NULL)
  , fhEPEM_identifiedLepton_selectionNN(NULL)
  , fhEPEM_identifiedLepton_rings(NULL)
  , fhEPEM_identifiedLepton_radius_vs_p(NULL)
  , fhEPEM_identifiedLepton_ANNvalue(NULL)
  , fhEPEM_invmass_eta_mc(NULL)
  , fhEPEM_invmass_eta_refitted(NULL)
  , fhEPEM_efficiencyCuts_eta(NULL)
  , timer()
  , fTime(0.)
{
}

CbmAnaConversionReco::~CbmAnaConversionReco() {}


void CbmAnaConversionReco::Init()
{
  FairRootManager* ioman = FairRootManager::Instance();
  if (NULL == ioman) { Fatal("CbmAnaConversion::Init", "RootManager not instantised!"); }

  fMcTracks = (TClonesArray*) ioman->GetObject("MCTrack");
  if (NULL == fMcTracks) { Fatal("CbmAnaConversion::Init", "No MCTrack array!"); }

  fGlobalTracks = (TClonesArray*) ioman->GetObject("GlobalTrack");
  if (NULL == fGlobalTracks) { Fatal("CbmAnaConversion::Init", "No GlobalTrack array!"); }

  fRichRings = (TClonesArray*) ioman->GetObject("RichRing");
  if (NULL == fRichRings) { Fatal("CbmAnaConversion::Init", "No RichRing array!"); }

  fRichHits = (TClonesArray*) ioman->GetObject("RichHit");
  if (NULL == fRichHits) { Fatal("CbmAnaConversion::Init", "No RichHit array!"); }


  InitHistos();
}


void CbmAnaConversionReco::InitHistos()
{
  fHistoList_MC.clear();
  fHistoList_reco.clear();
  fHistoList_reco_mom.clear();

  fHistoList_gg.clear();
  fHistoList_gee.clear();
  fHistoList_eeee.clear();
  fHistoList_all.clear();

  fHistoList_eta.clear();


  Double_t invmassSpectra_nof   = 800;
  Double_t invmassSpectra_start = -0.00125;
  Double_t invmassSpectra_end   = 1.99875;


  fhInvariantMass_MC_all =
    new TH1D("fhInvariantMass_MC_all", "fhInvariantMass_MC_all;invariant mass in GeV/c^{2};#", 2001, -0.0005, 2.0005);
  fhInvariantMass_MC_pi0 =
    new TH1D("fhInvariantMass_MC_pi0", "fhInvariantMass_MC_pi0;invariant mass in GeV/c^{2};#", 2001, -0.0005, 2.0005);
  fhInvariantMass_MC_pi0_epem = new TH1D(
    "fhInvariantMass_MC_pi0_epem", "fhInvariantMass_MC_pi0_epem;invariant mass in GeV/c^{2};#", 2001, -0.0005, 2.0005);
  fhInvariantMass_MC_pi0_gepem =
    new TH1D("fhInvariantMass_MC_pi0_gepem", "fhInvariantMass_MC_pi0_gepem;invariant mass in GeV/c^{2};#", 2001,
             -0.0005, 2.0005);
  fhInvariantMass_MC_pi0_gg = new TH1D(
    "fhInvariantMass_MC_pi0_gg", "fhInvariantMass_MC_pi0_gg;invariant mass in GeV/c^{2};#", 2001, -0.0005, 2.0005);
  fhInvariantMass_MC_eta =
    new TH1D("fhInvariantMass_MC_eta", "fhInvariantMass_MC_eta;invariant mass in GeV/c^{2};#", 2001, -0.0005, 2.0005);
  fhInvariantMass_MC_etaPrime = new TH1D(
    "fhInvariantMass_MC_etaPrime", "fhInvariantMass_MC_etaPrime;invariant mass in GeV/c^{2};#", 2001, -0.0005, 2.0005);
  fHistoList_MC.push_back(fhInvariantMass_MC_all);
  fHistoList_MC.push_back(fhInvariantMass_MC_pi0);
  fHistoList_MC.push_back(fhInvariantMass_MC_pi0_epem);
  fHistoList_MC.push_back(fhInvariantMass_MC_pi0_gepem);
  fHistoList_MC.push_back(fhInvariantMass_MC_pi0_gg);
  fHistoList_MC.push_back(fhInvariantMass_MC_eta);
  fHistoList_MC.push_back(fhInvariantMass_MC_etaPrime);


  fhMC_electrons_theta = new TH1D("fhMC_electrons_theta", "fhMC_electrons_theta;theta in deg;#", 90, 0., 90.);
  fhMC_electrons_p     = new TH1D("fhMC_electrons_p", "fhMC_electrons_p;momentum p in GeV/c;#", 100, 0., 10.);
  fhMC_electrons_theta_vs_p =
    new TH2D("fhMC_electrons_theta_vs_p", "fhMC_electrons_theta_vs_p;theta in deg;momentum p in GeV/c", 90, 0., 90.,
             100, 0., 10.);
  fHistoList_MC.push_back(fhMC_electrons_theta);
  fHistoList_MC.push_back(fhMC_electrons_p);
  fHistoList_MC.push_back(fhMC_electrons_theta_vs_p);

  fhEta_openingAngleGG = new TH1D("fhEta_openingAngleGG", "fhEta_openingAngleGG;theta in deg;#", 900, 0., 90.);
  fHistoList_MC.push_back(fhEta_openingAngleGG);

  fhMC_grandmotherPDGs = new TH1D("fhMC_grandmotherPDGs", "fhMC_grandmotherPDGs;particle pdg;#", 1000, 0., 1000.);
  fHistoList_MC.push_back(fhMC_grandmotherPDGs);

  fhMCtest = new TH1D("fhMCtest", "fhMCtest;invariant mass in GeV/c^{2};#", 2000, 0., 2.);
  fHistoList_MC.push_back(fhMCtest);


  fhEPEM_invmass_gg_mc = new TH1D("fhEPEM_invmass_gg_mc", "fhEPEM_invmass_gg_mc;invariant mass in GeV/c^{2};#",
                                  invmassSpectra_nof, invmassSpectra_start, invmassSpectra_end);
  fhEPEM_invmass_gg_refitted =
    new TH1D("fhEPEM_invmass_gg_refitted", "fhEPEM_invmass_gg_refitted;invariant mass in GeV/c^{2};#",
             invmassSpectra_nof, invmassSpectra_start, invmassSpectra_end);
  fhEPEM_invmass_gee_mc = new TH1D("fhEPEM_invmass_gee_mc", "fhEPEM_invmass_gee_mc;invariant mass in GeV/c^{2};#",
                                   invmassSpectra_nof, invmassSpectra_start, invmassSpectra_end);
  fhEPEM_invmass_gee_refitted =
    new TH1D("fhEPEM_invmass_gee_refitted", "fhEPEM_invmass_gee_refitted;invariant mass in GeV/c^{2};#",
             invmassSpectra_nof, invmassSpectra_start, invmassSpectra_end);
  fhEPEM_invmass_eeee_mc = new TH1D("fhEPEM_invmass_eeee_mc", "fhEPEM_invmass_eeee_mc;invariant mass in GeV/c^{2};#",
                                    invmassSpectra_nof, invmassSpectra_start, invmassSpectra_end);
  fhEPEM_invmass_eeee_refitted =
    new TH1D("fhEPEM_invmass_eeee_refitted", "fhEPEM_invmass_eeee_refitted;invariant mass in GeV/c^{2};#",
             invmassSpectra_nof, invmassSpectra_start, invmassSpectra_end);
  fhEPEM_invmass_all_mc = new TH1D("fhEPEM_invmass_all_mc", "fhEPEM_invmass_all_mc;invariant mass in GeV/c^{2};#",
                                   invmassSpectra_nof, invmassSpectra_start, invmassSpectra_end);
  fhEPEM_invmass_all_refitted =
    new TH1D("fhEPEM_invmass_all_refitted", "fhEPEM_invmass_all_refitted;invariant mass in GeV/c^{2};#",
             invmassSpectra_nof, invmassSpectra_start, invmassSpectra_end);
  fHistoList_gg.push_back(fhEPEM_invmass_gg_mc);
  fHistoList_gg.push_back(fhEPEM_invmass_gg_refitted);
  fHistoList_gee.push_back(fhEPEM_invmass_gee_mc);
  fHistoList_gee.push_back(fhEPEM_invmass_gee_refitted);
  fHistoList_eeee.push_back(fhEPEM_invmass_eeee_mc);
  fHistoList_eeee.push_back(fhEPEM_invmass_eeee_refitted);
  fHistoList_all.push_back(fhEPEM_invmass_all_mc);
  fHistoList_all.push_back(fhEPEM_invmass_all_refitted);

  fhEPEM_openingAngle_gg_mc = new TH1D(
    "fhEPEM_openingAngle_gg_mc", "fhEPEM_openingAngle_gg_mc (between e+e- from g);angle [deg];#", 1010, -0.1, 100.9);
  fhEPEM_openingAngle_gg_refitted =
    new TH1D("fhEPEM_openingAngle_gg_refitted", "fhEPEM_openingAngle_gg_refitted (between e+e- from g);angle [deg];#",
             1010, -0.1, 100.9);
  fhEPEM_openingAngle_gee_mc = new TH1D(
    "fhEPEM_openingAngle_gee_mc", "fhEPEM_openingAngle_gee_mc (between e+e- from g);angle [deg];#", 1010, -0.1, 100.9);
  fhEPEM_openingAngle_gee_refitted =
    new TH1D("fhEPEM_openingAngle_gee_refitted", "fhEPEM_openingAngle_gee_refitted (between e+e- from g);angle [deg];#",
             1010, -0.1, 100.9);
  fHistoList_gg.push_back(fhEPEM_openingAngle_gg_mc);
  fHistoList_gg.push_back(fhEPEM_openingAngle_gg_refitted);
  fHistoList_gee.push_back(fhEPEM_openingAngle_gee_mc);
  fHistoList_gee.push_back(fhEPEM_openingAngle_gee_refitted);
  fhEPEM_openingAngle_gee_mc_dalitz =
    new TH1D("fhEPEM_openingAngle_gee_mc_dalitz",
             "fhEPEM_openingAngle_gee_mc_dalitz (between e+e- from pi0);angle [deg];#", 1010, -0.1, 100.9);
  fhEPEM_openingAngle_gee_refitted_dalitz = new TH1D("fhEPEM_openingAngle_gee_refitted_dalitz",
                                                     "fhEPEM_openingAngle_gee_refitted_dalitz (between e+e- from "
                                                     "pi0);angle [deg];#",
                                                     1010, -0.1, 100.9);
  fHistoList_gee.push_back(fhEPEM_openingAngle_gee_mc_dalitz);
  fHistoList_gee.push_back(fhEPEM_openingAngle_gee_refitted_dalitz);

  fhEPEM_openingAngle_vs_pt_gg_mc =
    new TH2D("fhEPEM_openingAngle_vs_pt_gg_mc", "fhEPEM_openingAngle_vs_pt_gg_mc;pt [GeV]; opening angle [deg]", 220,
             -1., 10., 100, 0., 10.);
  fHistoList_gg.push_back(fhEPEM_openingAngle_vs_pt_gg_mc);
  fhEPEM_openingAngle_vs_pt_gg_reco =
    new TH2D("fhEPEM_openingAngle_vs_pt_gg_reco", "fhEPEM_openingAngle_vs_pt_gg_reco;pt [GeV]; opening angle [deg]",
             220, -1., 10., 100, 0., 10.);
  fHistoList_gg.push_back(fhEPEM_openingAngle_vs_pt_gg_reco);

  fhEPEM_openingAngle_betweenGammas_mc = new TH1D(
    "fhEPEM_openingAngle_betweenGammas_mc", "fhEPEM_openingAngle_betweenGammas_mc;angle [deg];#", 1010, -0.1, 100.9);
  fhEPEM_openingAngle_betweenGammas_reco =
    new TH1D("fhEPEM_openingAngle_betweenGammas_reco", "fhEPEM_openingAngle_betweenGammas_reco;angle [deg];#", 1010,
             -0.1, 100.9);
  fHistoList_gg.push_back(fhEPEM_openingAngle_betweenGammas_mc);
  fHistoList_gg.push_back(fhEPEM_openingAngle_betweenGammas_reco);


  fhPi0_pt_vs_rap_gg =
    new TH2D("fhPi0_pt_vs_rap_gg", "fhPi0_pt_vs_rap_gg;pt [GeV]; rap [GeV]", 240, -2., 10., 270, -2., 7.);
  fhPi0_pt_vs_rap_gee =
    new TH2D("fhPi0_pt_vs_rap_gee", "fhPi0_pt_vs_rap_gee;pt [GeV]; rap [GeV]", 240, -2., 10., 270, -2., 7.);
  fhPi0_pt_vs_rap_all =
    new TH2D("fhPi0_pt_vs_rap_all", "fhPi0_pt_vs_rap_all;pt [GeV]; rap [GeV]", 240, -2., 10., 270, -2., 7.);
  fHistoList_gg.push_back(fhPi0_pt_vs_rap_gg);
  fHistoList_gee.push_back(fhPi0_pt_vs_rap_gee);
  fHistoList_all.push_back(fhPi0_pt_vs_rap_all);

  fhPi0_pt_gg  = new TH1D("fhPi0_pt_gg", "fhPi0_pt_gg;pt [GeV];#", 200, 0., 10.);
  fhPi0_pt_gee = new TH1D("fhPi0_pt_gee", "fhPi0_pt_gee;pt [GeV];#", 200, 0., 10.);
  fhPi0_pt_all = new TH1D("fhPi0_pt_all", "fhPi0_pt_all;pt [GeV];#", 200, 0., 10.);
  fHistoList_gg.push_back(fhPi0_pt_gg);
  fHistoList_gee.push_back(fhPi0_pt_gee);
  fHistoList_all.push_back(fhPi0_pt_all);

  fhEPEM_efficiencyCuts = new TH1D("fhEPEM_efficiencyCuts", "fhEPEM_efficiencyCuts;;#", 13, 0., 13.);
  fHistoList_all.push_back(fhEPEM_efficiencyCuts);
  fhEPEM_efficiencyCuts->GetXaxis()->SetBinLabel(1, "no cuts");
  fhEPEM_efficiencyCuts->GetXaxis()->SetBinLabel(2, "ANN: 4 rich electrons");
  fhEPEM_efficiencyCuts->GetXaxis()->SetBinLabel(3, "ANN: #chi^{2}-cut");
  fhEPEM_efficiencyCuts->GetXaxis()->SetBinLabel(4, "ANN: #theta of e^{+}e^{-} pairs");
  fhEPEM_efficiencyCuts->GetXaxis()->SetBinLabel(5, "ANN: m_{inv} of e^{+}e^{-} pairs");
  fhEPEM_efficiencyCuts->GetXaxis()->SetBinLabel(6, "Normal: 4 rich electrons");
  fhEPEM_efficiencyCuts->GetXaxis()->SetBinLabel(7, "Normal: #chi^{2}-cut");
  fhEPEM_efficiencyCuts->GetXaxis()->SetBinLabel(8, "Normal: #theta of e^{+}e^{-} pairs");
  fhEPEM_efficiencyCuts->GetXaxis()->SetBinLabel(9, "Normal: m_{inv} of e^{+}e^{-} pairs");
  fhEPEM_efficiencyCuts->GetXaxis()->SetBinLabel(10, "MC: 4 rich electrons");
  fhEPEM_efficiencyCuts->GetXaxis()->SetBinLabel(11, "MC: #chi^{2}-cut");
  fhEPEM_efficiencyCuts->GetXaxis()->SetBinLabel(12, "MC: #theta of e^{+}e^{-} pairs");
  fhEPEM_efficiencyCuts->GetXaxis()->SetBinLabel(13, "MC: m_{inv} of e^{+}e^{-} pairs");

  fhEPEM_efficiencyCuts2 = new TH1D("fhEPEM_efficiencyCuts2", "fhEPEM_efficiencyCuts2;;#", 10, 0., 10.);
  fHistoList_all.push_back(fhEPEM_efficiencyCuts2);

  fhEPEM_pi0_nofLeptons_ann   = new TH1D("fhEPEM_pi0_nofLeptons_ann", "fhEPEM_pi0_nofLeptons_ann;;#", 5, -0.5, 4.5);
  fhEPEM_pi0_ANNvalues_noCuts = new TH1D("fhEPEM_pi0_ANNvalues_noCuts", "fhEPEM_pi0_ANNvalues_noCuts;;#", 400, -2, 2);
  fhEPEM_pi0_ANNvalues_angleCut =
    new TH1D("fhEPEM_pi0_ANNvalues_angleCut", "fhEPEM_pi0_ANNvalues_angleCut;;#", 400, -2, 2);
  fhEPEM_pi0_ANNefficiencies = new TH1D("fhEPEM_pi0_ANNefficiencies", "fhEPEM_pi0_ANNefficiencies;;#", 15, 0, 15);
  fHistoList_all.push_back(fhEPEM_pi0_ANNvalues_noCuts);
  fHistoList_all.push_back(fhEPEM_pi0_ANNvalues_angleCut);
  fHistoList_all.push_back(fhEPEM_pi0_nofLeptons_ann);
  fHistoList_all.push_back(fhEPEM_pi0_ANNefficiencies);

  fhEPEM_pi0_ANNefficiencies->GetXaxis()->SetBinLabel(1, "no cuts");
  fhEPEM_pi0_ANNefficiencies->GetXaxis()->SetBinLabel(2, "-1.0");
  fhEPEM_pi0_ANNefficiencies->GetXaxis()->SetBinLabel(3, "-0.9");
  fhEPEM_pi0_ANNefficiencies->GetXaxis()->SetBinLabel(4, "-0.8");
  fhEPEM_pi0_ANNefficiencies->GetXaxis()->SetBinLabel(5, "-0.7");
  fhEPEM_pi0_ANNefficiencies->GetXaxis()->SetBinLabel(6, "-0.6");
  fhEPEM_pi0_ANNefficiencies->GetXaxis()->SetBinLabel(7, "-0.5");
  fhEPEM_pi0_ANNefficiencies->GetXaxis()->SetBinLabel(8, "-0.4");
  fhEPEM_pi0_ANNefficiencies->GetXaxis()->SetBinLabel(9, "-0.3");
  fhEPEM_pi0_ANNefficiencies->GetXaxis()->SetBinLabel(10, "-0.2");
  fhEPEM_pi0_ANNefficiencies->GetXaxis()->SetBinLabel(11, "-0.1");
  fhEPEM_pi0_ANNefficiencies->GetXaxis()->SetBinLabel(12, "0.0");

  fhEPEM_rap_vs_chi =
    new TH2D("fhEPEM_rap_vs_chi", "fhEPEM_rap_vs_chi; rap [GeV]; chi of electrons", 300, 0., 10., 100, 0., 100.);
  fHistoList_all.push_back(fhEPEM_rap_vs_chi);
  fhEPEM_rap_vs_invmass =
    new TH2D("fhEPEM_rap_vs_invmass", "fhEPEM_rap_vs_invmass; rap [GeV]; invmass", 300, 0., 10., 100, 0., 10.);
  fHistoList_all.push_back(fhEPEM_rap_vs_invmass);


  fhInvMass_EPEM_mc = new TH1D("fhInvMass_EPEM_mc", "fhInvariantMass_recoMomentum1 (mc);mass [GeV/c^2];#",
                               invmassSpectra_nof, invmassSpectra_start, invmassSpectra_end);
  fhInvMass_EPEM_stsMomVec =
    new TH1D("fhInvMass_EPEM_stsMomVec", "fhInvariantMass_recoMomentum2 (stsMomentumVec);mass [GeV/c^2];#",
             invmassSpectra_nof, invmassSpectra_start, invmassSpectra_end);
  fhInvMass_EPEM_refitted =
    new TH1D("fhInvMass_EPEM_refitted", "fhInvariantMass_recoMomentum3 (refitted at primary);mass [GeV/c^2];#",
             invmassSpectra_nof, invmassSpectra_start, invmassSpectra_end);
  fhInvMass_EPEM_error_stsMomVec =
    new TH1D("fhInvMass_EPEM_error_stsMomVec", "fhInvariantMass_recoMomentum4 (error, stsMomentumVec);(mc-reco)/mc;#",
             500, -0.005, 4.995);
  fhInvMass_EPEM_error_refitted =
    new TH1D("fhInvMass_EPEM_error_refitted", "fhInvariantMass_recoMomentum5 (error, refitted);(mc-reco)/mc;#", 500,
             -0.005, 4.995);
  fhInvMass_EPEM_openingAngleRef =
    new TH1D("fhInvMass_EPEM_openingAngleRef", "fhInvariantMass_openingAngleRef;angle [deg];#", 1010, -0.1, 100.9);
  fhUsedMomenta_stsMomVec = new TH1D("fhUsedMomenta_stsMomVec", "fhMomentumtest1;momentum;#", 100, 0., 2.);
  fhUsedMomenta_mc        = new TH1D("fhUsedMomenta_mc", "fhMomentumtest2;momentum;#", 100, 0., 2.);
  fhUsedMomenta_error_stsMomVec =
    new TH1D("fhUsedMomenta_error_stsMomVec", "fhMomentumtest3 (error);(mc-reco)/mc;#", 400, -2.005, 1.995);
  fhUsedMomenta_error_refitted =
    new TH1D("fhUsedMomenta_error_refitted", "fhMomentumtest4 (error);(mc-reco_refitted)/mc;#", 400, -2.005, 1.995);

  fhUsedMomenta_errorX_stsMomVec = new TH1D(
    "fhUsedMomenta_errorX_stsMomVec", "fhMomentumtest5 (error of x-momentum);(mc-reco_reco)/mc;#", 401, -4.01, 4.01);
  fhUsedMomenta_vsX_stsMomVec =
    new TH2D("fhUsedMomenta_vsX_stsMomVec", "fhMomentumtest5vs (error of x-momentum);mc;reco", 101, -1.01, 1.01, 101,
             -1.01, 1.01);
  fhUsedMomenta_errorY_stsMomVec = new TH1D(
    "fhUsedMomenta_errorY_stsMomVec", "fhMomentumtest6 (error of y-momentum);(mc-reco_reco)/mc;#", 401, -4.01, 4.01);
  fhUsedMomenta_vsY_stsMomVec =
    new TH2D("fhUsedMomenta_vsY_stsMomVec", "fhMomentumtest6vs (error of y-momentum);mc;reco", 101, -1.01, 1.01, 101,
             -1.01, 1.01);
  fhUsedMomenta_errorZ_stsMomVec = new TH1D(
    "fhUsedMomenta_errorZ_stsMomVec", "fhMomentumtest7 (error of z-momentum);(mc-reco_reco)/mc;#", 401, -4.01, 4.01);
  fhUsedMomenta_vsZ_stsMomVec =
    new TH2D("fhUsedMomenta_vsZ_stsMomVec", "fhMomentumtest7vs (error of z-momentum);mc;reco", 201, -0.01, 4.01, 201,
             -0.01, 4.01);
  fhUsedMomenta_errorX_refitted = new TH1D(
    "fhUsedMomenta_errorX_refitted", "fhMomentumtest5 (error of x-momentum);(mc-reco_reco)/mc;#", 401, -4.01, 4.01);
  fhUsedMomenta_vsX_refitted = new TH2D("fhUsedMomenta_vsX_refitted", "fhMomentumtest5vs (error of x-momentum);mc;reco",
                                        101, -1.01, 1.01, 101, -1.01, 1.01);
  fhUsedMomenta_errorY_refitted = new TH1D(
    "fhUsedMomenta_errorY_refitted", "fhMomentumtest6 (error of y-momentum);(mc-reco_reco)/mc;#", 401, -4.01, 4.01);
  fhUsedMomenta_vsY_refitted = new TH2D("fhUsedMomenta_vsY_refitted", "fhMomentumtest6vs (error of y-momentum);mc;reco",
                                        101, -1.01, 1.01, 101, -1.01, 1.01);
  fhUsedMomenta_errorZ_refitted = new TH1D(
    "fhUsedMomenta_errorZ_refitted", "fhMomentumtest7 (error of z-momentum);(mc-reco_reco)/mc;#", 401, -4.01, 4.01);
  fhUsedMomenta_vsZ_refitted = new TH2D("fhUsedMomenta_vsZ_refitted", "fhMomentumtest7vs (error of z-momentum);mc;reco",
                                        201, -0.01, 4.01, 201, -0.01, 4.01);
  fHistoList_reco.push_back(fhInvMass_EPEM_mc);
  fHistoList_reco.push_back(fhInvMass_EPEM_stsMomVec);
  fHistoList_reco.push_back(fhInvMass_EPEM_refitted);
  fHistoList_reco.push_back(fhInvMass_EPEM_error_stsMomVec);
  fHistoList_reco.push_back(fhInvMass_EPEM_error_refitted);
  fHistoList_reco.push_back(fhInvMass_EPEM_openingAngleRef);
  fHistoList_reco_mom.push_back(fhUsedMomenta_stsMomVec);
  fHistoList_reco_mom.push_back(fhUsedMomenta_mc);
  fHistoList_reco_mom.push_back(fhUsedMomenta_error_stsMomVec);
  fHistoList_reco_mom.push_back(fhUsedMomenta_error_refitted);
  fHistoList_reco_mom.push_back(fhUsedMomenta_errorX_stsMomVec);  // error of x-component of reconstructed momentum
  fHistoList_reco_mom.push_back(fhUsedMomenta_errorY_stsMomVec);  // error of y-component of reconstructed momentum
  fHistoList_reco_mom.push_back(fhUsedMomenta_errorZ_stsMomVec);  // error of z-component of reconstructed momentum
  fHistoList_reco_mom.push_back(fhUsedMomenta_vsX_stsMomVec);    // x-component of reconstructed momentum vs mc-momentum
  fHistoList_reco_mom.push_back(fhUsedMomenta_vsY_stsMomVec);    // y-component of reconstructed momentum vs mc-momentum
  fHistoList_reco_mom.push_back(fhUsedMomenta_vsZ_stsMomVec);    // z-component of reconstructed momentum vs mc-momentum
  fHistoList_reco_mom.push_back(fhUsedMomenta_errorX_refitted);  // error of x-component of reconstructed momentum
  fHistoList_reco_mom.push_back(fhUsedMomenta_errorY_refitted);  // error of y-component of reconstructed momentum
  fHistoList_reco_mom.push_back(fhUsedMomenta_errorZ_refitted);  // error of z-component of reconstructed momentum
  fHistoList_reco_mom.push_back(fhUsedMomenta_vsX_refitted);     // x-component of reconstructed momentum vs mc-momentum
  fHistoList_reco_mom.push_back(fhUsedMomenta_vsY_refitted);     // y-component of reconstructed momentum vs mc-momentum
  fHistoList_reco_mom.push_back(fhUsedMomenta_vsZ_refitted);     // z-component of reconstructed momentum vs mc-momentum

  fhInvariantMass_pi0epem =
    new TH1D("fhInvariantMass_pi0epem", "fhInvariantMass_pi0epem;mass [GeV/c^2];#", 400, 0., 2.);
  fHistoList_reco.push_back(fhInvariantMass_pi0epem);

  fhPi0_startvertex = new TH1D("fhPi0_startvertex", "fhPi0_startvertex;z in cm;#", 210, -5., 100.);
  fHistoList_reco.push_back(fhPi0_startvertex);

  fhPi0_startvertexElectrons_all =
    new TH1D("fhPi0_startvertexElectrons_all", "fhPi0_startvertexElectrons_all;z in cm;#", 411, -5.25, 200.25);
  fHistoList_reco.push_back(fhPi0_startvertexElectrons_all);

  fhPi0_startvertexElectrons_gg =
    new TH1D("fhPi0_startvertexElectrons_gg", "fhPi0_startvertexElectrons_gg;z in cm;#", 411, -5.25, 200.25);
  fHistoList_reco.push_back(fhPi0_startvertexElectrons_gg);

  fhPi0_startvertexElectrons_gee =
    new TH1D("fhPi0_startvertexElectrons_gee", "fhPi0_startvertexElectrons_gee;z in cm;#", 411, -5.25, 200.25);
  fHistoList_reco.push_back(fhPi0_startvertexElectrons_gee);

  fhPi0_startvertex_vs_chi =
    new TH2D("fhPi0_startvertex_vs_chi", "fhPi0_startvertex_vs_chi;z[cm];chi", 210, -5., 100., 1000, 0., 100.);
  fHistoList_reco.push_back(fhPi0_startvertex_vs_chi);

  fhPi0_startvertex_vs_momentum =
    new TH2D("fhPi0_startvertex_vs_momentum", "fhPi0_startvertex_vs_momentum;z in cm;momentum (MC-true)", 210, -5.,
             100., 1000, 0., 100.);
  fHistoList_reco.push_back(fhPi0_startvertex_vs_momentum);

  fhInvMassWithFullRecoCuts =
    new TH1D("fhInvMassWithFullRecoCuts", "fhInvMassWithFullRecoCuts;mass [GeV/c^2];#", 800, 0., 2.);
  fHistoList_reco.push_back(fhInvMassWithFullRecoCuts);


  fhEPEM_InDetector_invmass_gg_mc =
    new TH1D("fhEPEM_InDetector_invmass_gg_mc", "fhEPEM_InDetector_invmass_gg_mc;mass [GeV/c^2];#", invmassSpectra_nof,
             invmassSpectra_start, invmassSpectra_end);
  fhEPEM_InDetector_invmass_gg_refitted =
    new TH1D("fhEPEM_InDetector_invmass_gg_refitted", "fhEPEM_InDetector_invmass_gg_refitted;mass [GeV/c^2];#",
             invmassSpectra_nof, invmassSpectra_start, invmassSpectra_end);
  fhEPEM_InDetector_invmass_gee_mc =
    new TH1D("fhEPEM_InDetector_invmass_gee_mc", "fhEPEM_InDetector_invmass_gee_mc;mass [GeV/c^2];#",
             invmassSpectra_nof, invmassSpectra_start, invmassSpectra_end);
  fhEPEM_InDetector_invmass_gee_refitted =
    new TH1D("fhEPEM_InDetector_invmass_gee_refitted", "fhEPEM_InDetector_invmass_gee_refitted;mass [GeV/c^2];#",
             invmassSpectra_nof, invmassSpectra_start, invmassSpectra_end);
  fhEPEM_InDetector_invmass_all_mc =
    new TH1D("fhEPEM_InDetector_invmass_all_mc", "fhEPEM_InDetector_invmass_all_mc;mass [GeV/c^2];#",
             invmassSpectra_nof, invmassSpectra_start, invmassSpectra_end);
  fhEPEM_InDetector_invmass_all_refitted =
    new TH1D("fhEPEM_InDetector_invmass_all_refitted", "fhEPEM_InDetector_invmass_all_refitted;mass [GeV/c^2];#",
             invmassSpectra_nof, invmassSpectra_start, invmassSpectra_end);
  fHistoList_gg.push_back(fhEPEM_InDetector_invmass_gg_mc);
  fHistoList_gg.push_back(fhEPEM_InDetector_invmass_gg_refitted);
  fHistoList_gee.push_back(fhEPEM_InDetector_invmass_gee_mc);
  fHistoList_gee.push_back(fhEPEM_InDetector_invmass_gee_refitted);
  fHistoList_all.push_back(fhEPEM_InDetector_invmass_all_mc);
  fHistoList_all.push_back(fhEPEM_InDetector_invmass_all_refitted);


  fhEPEM_pt_vs_p_all_mc =
    new TH2D("fhEPEM_pt_vs_p_all_mc", "fhEPEM_pt_vs_p_all_mc;p_{t} in GeV/c; p in GeV/c", 240, -2., 10., 360, -2., 16.);
  fHistoList_all.push_back(fhEPEM_pt_vs_p_all_mc);
  fhEPEM_pt_vs_p_all_refitted =
    new TH2D("fhEPEM_pt_vs_p_all_refitted", "fhTest2_electrons_pt_vs_p;p_{t} in GeV/c; p in GeV/c", 240, -2., 10., 360,
             -2., 16.);
  fHistoList_all.push_back(fhEPEM_pt_vs_p_all_refitted);

  fhEPEM_missingLepton_nofRingHits =
    new TH1D("fhEPEM_missingLepton_nofRingHits", "fhEPEM_missingLepton_nofRingHits;nofringhits;#", 41, -0.5, 40.5);
  fhEPEM_missingLepton_ringMid =
    new TH2D("fhEPEM_missingLepton_ringMid", "fhEPEM_missingLepton_ringMid;X;Y", 100, -150, 150, 150, -250, 250);
  fhEPEM_missingLepton_ringRadius =
    new TH1D("fhEPEM_missingLepton_ringRadius", "fhEPEM_missingLepton_ringRadius;ringRadius;#", 100, 0, 10);
  fhEPEM_missingLepton_distance =
    new TH1D("fhEPEM_missingLepton_distance", "fhEPEM_missingLepton_distance;distance;#", 100, 0, 20);
  fhEPEM_missingLepton_selectionNN =
    new TH1D("fhEPEM_missingLepton_selectionNN", "fhEPEM_missingLepton_selectionNN;selectionNN;#", 40, -2, 2);
  fhEPEM_missingLepton_rings =
    new TH2D("fhEPEM_missingLepton_rings", "fhEPEM_missingLepton_rings;selectionNN;#", 400, -200, 200, 600, -300, 300);
  fhEPEM_missingLepton_radius_vs_p =
    new TH2D("fhEPEM_missingLepton_radius_vs_p",
             "fhEPEM_missingLepton_radius_vs_p;momentum p in GeV/c;ring radius in cm", 120, 0, 12, 100, 0, 10);
  fhEPEM_missingLepton_ANNvalue =
    new TH1D("fhEPEM_missingLepton_ANNvalue", "fhEPEM_missingLepton_ANNvalue;ANNvalue;#", 40, -2, 2);
  fHistoList_all.push_back(fhEPEM_missingLepton_nofRingHits);
  fHistoList_all.push_back(fhEPEM_missingLepton_ringMid);
  fHistoList_all.push_back(fhEPEM_missingLepton_ringRadius);
  fHistoList_all.push_back(fhEPEM_missingLepton_distance);
  fHistoList_all.push_back(fhEPEM_missingLepton_selectionNN);
  fHistoList_all.push_back(fhEPEM_missingLepton_rings);
  fHistoList_all.push_back(fhEPEM_missingLepton_radius_vs_p);
  fHistoList_all.push_back(fhEPEM_missingLepton_ANNvalue);


  fhEPEM_identifiedLepton_nofRingHits = new TH1D("fhEPEM_identifiedLepton_nofRingHits",
                                                 "fhEPEM_identifiedLepton_nofRingHits;nofringhits;#", 41, -0.5, 40.5);
  fhEPEM_identifiedLepton_ringMid =
    new TH2D("fhEPEM_identifiedLepton_ringMid", "fhEPEM_identifiedLepton_ringMid;X;Y", 100, -150, 150, 150, -250, 250);
  fhEPEM_identifiedLepton_ringRadius =
    new TH1D("fhEPEM_identifiedLepton_ringRadius", "fhEPEM_identifiedLepton_ringRadius;ringRadius;#", 100, 0, 10);
  fhEPEM_identifiedLepton_distance =
    new TH1D("fhEPEM_identifiedLepton_distance", "fhEPEM_identifiedLepton_distance;distance;#", 100, 0, 20);
  fhEPEM_identifiedLepton_selectionNN =
    new TH1D("fhEPEM_identifiedLepton_selectionNN", "fhEPEM_identifiedLepton_selectionNN;selectionNN;#", 40, -2, 2);
  fhEPEM_identifiedLepton_rings = new TH2D(
    "fhEPEM_identifiedLepton_rings", "fhEPEM_identifiedLepton_rings;selectionNN;#", 400, -200, 200, 600, -300, 300);
  fhEPEM_identifiedLepton_radius_vs_p =
    new TH2D("fhEPEM_identifiedLepton_radius_vs_p",
             "fhEPEM_identifiedLepton_radius_vs_p;momentum p in GeV/c;ring radius in cm", 120, 0, 12, 100, 0, 10);
  fhEPEM_identifiedLepton_ANNvalue =
    new TH1D("fhEPEM_identifiedLepton_ANNvalue", "fhEPEM_identifiedLepton_ANNvalue;ANNvalue;#", 40, -2, 2);
  fHistoList_all.push_back(fhEPEM_identifiedLepton_nofRingHits);
  fHistoList_all.push_back(fhEPEM_identifiedLepton_ringMid);
  fHistoList_all.push_back(fhEPEM_identifiedLepton_ringRadius);
  fHistoList_all.push_back(fhEPEM_identifiedLepton_distance);
  fHistoList_all.push_back(fhEPEM_identifiedLepton_selectionNN);
  fHistoList_all.push_back(fhEPEM_identifiedLepton_rings);
  fHistoList_all.push_back(fhEPEM_identifiedLepton_radius_vs_p);
  fHistoList_all.push_back(fhEPEM_identifiedLepton_ANNvalue);


  // histograms for eta analysis
  fhEPEM_invmass_eta_mc       = new TH1D("fhEPEM_invmass_eta_mc", "fhEPEM_Iinvmass_eta_mc;mass [GeV/c^2];#",
                                   invmassSpectra_nof, invmassSpectra_start, invmassSpectra_end);
  fhEPEM_invmass_eta_refitted = new TH1D("fhEPEM_invmass_eta_refitted", "fhEPEM_Iinvmass_eta_refitted;mass [GeV/c^2];#",
                                         invmassSpectra_nof, invmassSpectra_start, invmassSpectra_end);
  fHistoList_eta.push_back(fhEPEM_invmass_eta_mc);
  fHistoList_eta.push_back(fhEPEM_invmass_eta_refitted);

  fhEPEM_efficiencyCuts_eta = new TH1D("fhEPEM_efficiencyCuts_eta", "fhEPEM_efficiencyCuts_eta;;#", 13, 0., 13.);
  fHistoList_eta.push_back(fhEPEM_efficiencyCuts_eta);
  fhEPEM_efficiencyCuts_eta->GetXaxis()->SetBinLabel(1, "no cuts");
  fhEPEM_efficiencyCuts_eta->GetXaxis()->SetBinLabel(2, "ANN: 4 rich electrons");
  fhEPEM_efficiencyCuts_eta->GetXaxis()->SetBinLabel(3, "ANN: #chi^{2}-cut");
  fhEPEM_efficiencyCuts_eta->GetXaxis()->SetBinLabel(4, "ANN: #theta of e^{+}e^{-} pairs");
  fhEPEM_efficiencyCuts_eta->GetXaxis()->SetBinLabel(5, "ANN: m_{inv} of e^{+}e^{-} pairs");
  fhEPEM_efficiencyCuts_eta->GetXaxis()->SetBinLabel(6, "Normal: 4 rich electrons");
  fhEPEM_efficiencyCuts_eta->GetXaxis()->SetBinLabel(7, "Normal: #chi^{2}-cut");
  fhEPEM_efficiencyCuts_eta->GetXaxis()->SetBinLabel(8, "Normal: #theta of e^{+}e^{-} pairs");
  fhEPEM_efficiencyCuts_eta->GetXaxis()->SetBinLabel(9, "Normal: m_{inv} of e^{+}e^{-} pairs");
  fhEPEM_efficiencyCuts_eta->GetXaxis()->SetBinLabel(10, "MC: 4 rich electrons");
  fhEPEM_efficiencyCuts_eta->GetXaxis()->SetBinLabel(11, "MC: #chi^{2}-cut");
  fhEPEM_efficiencyCuts_eta->GetXaxis()->SetBinLabel(12, "MC: #theta of e^{+}e^{-} pairs");
  fhEPEM_efficiencyCuts_eta->GetXaxis()->SetBinLabel(13, "MC: m_{inv} of e^{+}e^{-} pairs");
}


void CbmAnaConversionReco::Finish()
{
  //gDirectory->cd("analysis-conversion");
  gDirectory->mkdir("MCreco");
  gDirectory->cd("MCreco");
  for (UInt_t i = 0; i < fHistoList_MC.size(); i++) {
    fHistoList_MC[i]->Write();
  }
  gDirectory->cd("..");

  gDirectory->mkdir("Reconstruction2");
  gDirectory->cd("Reconstruction2");

  gDirectory->mkdir("pi0 -> gg");
  gDirectory->cd("pi0 -> gg");
  for (UInt_t i = 0; i < fHistoList_gg.size(); i++) {
    fHistoList_gg[i]->Write();
  }
  gDirectory->cd("..");

  gDirectory->mkdir("pi0 -> ge+e-");
  gDirectory->cd("pi0 -> ge+e-");
  for (UInt_t i = 0; i < fHistoList_gee.size(); i++) {
    fHistoList_gee[i]->Write();
  }
  gDirectory->cd("..");

  gDirectory->mkdir("pi0 -> e+e-e+e-");
  gDirectory->cd("pi0 -> e+e-e+e-");
  for (UInt_t i = 0; i < fHistoList_eeee.size(); i++) {
    fHistoList_eeee[i]->Write();
  }
  gDirectory->cd("..");

  gDirectory->mkdir("pi0 -> all");
  gDirectory->cd("pi0 -> all");
  for (UInt_t i = 0; i < fHistoList_all.size(); i++) {
    fHistoList_all[i]->Write();
  }
  gDirectory->cd("..");

  gDirectory->mkdir("eta");
  gDirectory->cd("eta");
  for (UInt_t i = 0; i < fHistoList_eta.size(); i++) {
    fHistoList_eta[i]->Write();
  }
  gDirectory->cd("..");

  for (UInt_t i = 0; i < fHistoList_reco.size(); i++) {
    fHistoList_reco[i]->Write();
  }
  gDirectory->mkdir("Momenta2");
  gDirectory->cd("Momenta2");
  for (UInt_t i = 0; i < fHistoList_reco_mom.size(); i++) {
    fHistoList_reco_mom[i]->Write();
  }
  gDirectory->cd("../..");

  cout << "CbmAnaConversionReco: Realtime - " << fTime << endl;
  //timer.Print();
}


void CbmAnaConversionReco::SetTracklistMC(vector<CbmMCTrack*> MCTracklist) { fMCTracklist_all = MCTracklist; }


void CbmAnaConversionReco::SetTracklistReco(vector<CbmMCTrack*> MCTracklist, vector<TVector3> RecoTracklist1,
                                            vector<TVector3> RecoTracklist2, vector<int> ids, vector<Double_t> chi,
                                            vector<Int_t> GlobalTrackId)
{
  fRecoTracklistEPEM      = MCTracklist;
  fRecoTracklistEPEM_ids  = ids;
  fRecoTracklistEPEM_chi  = chi;
  fRecoTracklistEPEM_gtid = GlobalTrackId;
  fRecoMomentum           = RecoTracklist1;
  fRecoRefittedMomentum   = RecoTracklist2;
}


void CbmAnaConversionReco::InvariantMassMC_all()
// calculation of invariant mass via X -> gamma gamma -> e+ e- e+ e-, only MC data with several cuts (see FillMCTrackslists())
{
  timer.Start();


  cout << "CbmAnaConversionReco: InvariantMassTestMC - Start..." << endl;
  cout << "CbmAnaConversionReco: InvariantMassTestMC - Size of "
          "fTracklistMC_all:\t "
       << fMCTracklist_all.size() << endl;
  if (fMCTracklist_all.size() >= 4) {
    for (unsigned int i = 0; i < fMCTracklist_all.size() - 3; i++) {
      if (i % 10 == 0) cout << "CbmAnaConversionReco: InvariantMassTestMC - iteration i = " << i << endl;
      for (unsigned int j = i + 1; j < fMCTracklist_all.size() - 2; j++) {
        for (unsigned int k = j + 1; k < fMCTracklist_all.size() - 1; k++) {
          for (unsigned int l = k + 1; l < fMCTracklist_all.size(); l++) {

            if (fMCTracklist_all[i]->GetPdgCode() + fMCTracklist_all[j]->GetPdgCode()
                  + fMCTracklist_all[k]->GetPdgCode() + fMCTracklist_all[l]->GetPdgCode()
                != 0)
              continue;

            if (i == j || i == k || i == l || j == k || j == l || k == l) continue;

            int motherId1 = fMCTracklist_all[i]->GetMotherId();
            int motherId2 = fMCTracklist_all[j]->GetMotherId();
            int motherId3 = fMCTracklist_all[k]->GetMotherId();
            int motherId4 = fMCTracklist_all[l]->GetMotherId();

            TVector3 momentum1, momentum2, momentum3, momentum4;
            fMCTracklist_all[i]->GetMomentum(momentum1);
            fMCTracklist_all[j]->GetMomentum(momentum2);
            fMCTracklist_all[k]->GetMomentum(momentum3);
            fMCTracklist_all[l]->GetMomentum(momentum4);


            // decay pi0 -> e+ e- e+ e-
            if (motherId1 == motherId2 && motherId1 == motherId3 && motherId1 == motherId4) {
              cout << "testxyz" << endl;
              Double_t invmass =
                Invmass_4particles(fMCTracklist_all[i], fMCTracklist_all[j], fMCTracklist_all[k], fMCTracklist_all[l]);
              fhMCtest->Fill(invmass);

              if (motherId1 != -1) {
                int mcMotherPdg1    = -1;
                CbmMCTrack* mother1 = (CbmMCTrack*) fMcTracks->At(motherId1);
                if (NULL != mother1) mcMotherPdg1 = mother1->GetPdgCode();
                if (mcMotherPdg1 == 111) {
                  Double_t invmass2 = 0;
                  invmass2          = Invmass_4particles(fMCTracklist_all[i], fMCTracklist_all[j], fMCTracklist_all[k],
                                                fMCTracklist_all[l]);
                  fhMCtest->Fill(invmass2);
                  fhInvariantMass_MC_pi0->Fill(invmass2);
                  fhInvariantMass_MC_pi0_epem->Fill(invmass2);
                  cout << "#######################################  Decay pi0 "
                          "-> e+e-e+e- detected!\t\t"
                       << invmass2 << endl;
                }
              }
              else {
                continue;
              }
            }


            int grandmotherId1 = -1;
            int grandmotherId2 = -1;
            int grandmotherId3 = -1;
            int grandmotherId4 = -1;

            int mcMotherPdg1 = -1;
            int mcMotherPdg2 = -1;
            int mcMotherPdg3 = -1;
            //						int mcMotherPdg4  = -1;
            int mcGrandmotherPdg1 = -1;
            //						int mcGrandmotherPdg2  = -1;
            //						int mcGrandmotherPdg3  = -1;
            //						int mcGrandmotherPdg4  = -1;


            if (motherId1 != -1) {
              CbmMCTrack* mother1 = (CbmMCTrack*) fMcTracks->At(motherId1);
              if (NULL != mother1) mcMotherPdg1 = mother1->GetPdgCode();
              grandmotherId1 = mother1->GetMotherId();
              if (grandmotherId1 != -1) {
                CbmMCTrack* grandmother1 = (CbmMCTrack*) fMcTracks->At(grandmotherId1);
                if (NULL != grandmother1) mcGrandmotherPdg1 = grandmother1->GetPdgCode();
              }
            }
            if (motherId2 != -1) {
              CbmMCTrack* mother2 = (CbmMCTrack*) fMcTracks->At(motherId2);
              if (NULL != mother2) mcMotherPdg2 = mother2->GetPdgCode();
              grandmotherId2 = mother2->GetMotherId();
              if (grandmotherId2 != -1) {
                //								CbmMCTrack* grandmother2 = (CbmMCTrack*) fMcTracks->At(grandmotherId2);
                //								if (NULL != grandmother2) mcGrandmotherPdg2 = grandmother2->GetPdgCode();
              }
            }
            if (motherId3 != -1) {
              CbmMCTrack* mother3 = (CbmMCTrack*) fMcTracks->At(motherId3);
              if (NULL != mother3) mcMotherPdg3 = mother3->GetPdgCode();
              grandmotherId3 = mother3->GetMotherId();
              if (grandmotherId3 != -1) {
                //								CbmMCTrack* grandmother3 = (CbmMCTrack*) fMcTracks->At(grandmotherId3);
                //								if (NULL != grandmother3) mcGrandmotherPdg3 = grandmother3->GetPdgCode();
              }
            }
            if (motherId4 != -1) {
              CbmMCTrack* mother4 = (CbmMCTrack*) fMcTracks->At(motherId4);
              //							if (NULL != mother4) mcMotherPdg4 = mother4->GetPdgCode();
              grandmotherId4 = mother4->GetMotherId();
              if (grandmotherId4 != -1) {
                //								CbmMCTrack* grandmother4 = (CbmMCTrack*) fMcTracks->At(grandmotherId4);
                //								if (NULL != grandmother4) mcGrandmotherPdg4 = grandmother4->GetPdgCode();
              }
            }


            if (motherId1 == motherId2 && motherId1 == motherId3) {}
            if (motherId1 == motherId2 && motherId1 == motherId4) {}
            if (motherId1 == motherId3 && motherId1 == motherId4) {}
            if (motherId2 == motherId3 && motherId2 == motherId4) {}

            //	if(  ((motherId1 == motherId2 && motherId3 == motherId4) && (mcMotherPdg1 == 22 || mcMotherPdg1 == 111) && (mcMotherPdg3 == 22 || mcMotherPdg3 == 111))
            //	  || ((motherId1 == motherId3 && motherId2 == motherId4) && (mcMotherPdg1 == 22 || mcMotherPdg1 == 111) && (mcMotherPdg2 == 22 || mcMotherPdg2 == 111))
            //	  || ((motherId1 == motherId4 && motherId2 == motherId3) && (mcMotherPdg1 == 22 || mcMotherPdg1 == 111) && (mcMotherPdg2 == 22 || mcMotherPdg2 == 111))) {

            // decay X -> gg -> e+ e- e+ e-
            if (((motherId1 == motherId2 && motherId3 == motherId4) && (mcMotherPdg1 == 22) && (mcMotherPdg3 == 22)
                 && grandmotherId1 == grandmotherId3)
                || ((motherId1 == motherId3 && motherId2 == motherId4) && (mcMotherPdg1 == 22) && (mcMotherPdg2 == 22)
                    && grandmotherId1 == grandmotherId2)
                || ((motherId1 == motherId4 && motherId2 == motherId3) && (mcMotherPdg1 == 22) && (mcMotherPdg2 == 22)
                    && grandmotherId1 == grandmotherId2)) {
              Double_t invmass =
                Invmass_4particles(fMCTracklist_all[i], fMCTracklist_all[j], fMCTracklist_all[k], fMCTracklist_all[l]);
              //fhMCtest->Fill(invmass);
              fhInvariantMass_MC_all->Fill(invmass);
              if (mcGrandmotherPdg1 == 111) {
                fhInvariantMass_MC_pi0->Fill(invmass);
                fhInvariantMass_MC_pi0_gg->Fill(invmass);
                fhMC_electrons_theta->Fill(momentum1.Theta() * 180. / TMath::Pi());
                fhMC_electrons_theta->Fill(momentum2.Theta() * 180. / TMath::Pi());
                fhMC_electrons_theta->Fill(momentum3.Theta() * 180. / TMath::Pi());
                fhMC_electrons_theta->Fill(momentum4.Theta() * 180. / TMath::Pi());
                fhMC_electrons_p->Fill(momentum1.Mag());
                fhMC_electrons_p->Fill(momentum2.Mag());
                fhMC_electrons_p->Fill(momentum3.Mag());
                fhMC_electrons_p->Fill(momentum4.Mag());
                fhMC_electrons_theta_vs_p->Fill(momentum1.Theta() * 180. / TMath::Pi(), momentum1.Mag());
                fhMC_electrons_theta_vs_p->Fill(momentum2.Theta() * 180. / TMath::Pi(), momentum2.Mag());
                fhMC_electrons_theta_vs_p->Fill(momentum3.Theta() * 180. / TMath::Pi(), momentum3.Mag());
                fhMC_electrons_theta_vs_p->Fill(momentum4.Theta() * 180. / TMath::Pi(), momentum4.Mag());
              }
              if (mcGrandmotherPdg1 == 221) {
                fhInvariantMass_MC_eta->Fill(invmass);

                Double_t opening_angle_gg = 0;
                if (motherId1 == motherId2) {
                  opening_angle_gg = CbmAnaConversionGlobalFunctions::OpeningAngleBetweenGamma(momentum1, momentum2,
                                                                                               momentum3, momentum4);
                }
                if (motherId1 == motherId3) {
                  opening_angle_gg = CbmAnaConversionGlobalFunctions::OpeningAngleBetweenGamma(momentum1, momentum3,
                                                                                               momentum2, momentum4);
                }
                if (motherId1 == motherId4) {
                  opening_angle_gg = CbmAnaConversionGlobalFunctions::OpeningAngleBetweenGamma(momentum1, momentum4,
                                                                                               momentum2, momentum3);
                }

                fhEta_openingAngleGG->Fill(opening_angle_gg);
              }
              if (TMath::Abs(mcGrandmotherPdg1) == 331) {  // eta prime (958)
                fhInvariantMass_MC_etaPrime->Fill(invmass);
              }
              fhMC_grandmotherPDGs->Fill(TMath::Abs(mcGrandmotherPdg1));
            }


            // decay pi0 -> g e+ e- -> e+ e- e+ e-
            if (((motherId1 == motherId2 && motherId3 == motherId4) && (mcMotherPdg1 == 22) && (mcMotherPdg3 == 111)
                 && grandmotherId1 == motherId3)
                || ((motherId1 == motherId2 && motherId3 == motherId4) && (mcMotherPdg1 == 111) && (mcMotherPdg3 == 22)
                    && grandmotherId3 == motherId1)
                || ((motherId1 == motherId3 && motherId2 == motherId4) && (mcMotherPdg1 == 22) && (mcMotherPdg2 == 111)
                    && grandmotherId1 == motherId2)
                || ((motherId1 == motherId3 && motherId2 == motherId4) && (mcMotherPdg1 == 111) && (mcMotherPdg2 == 22)
                    && grandmotherId2 == motherId1)
                || ((motherId1 == motherId4 && motherId2 == motherId3) && (mcMotherPdg1 == 22) && (mcMotherPdg2 == 111)
                    && grandmotherId1 == motherId2)
                || ((motherId1 == motherId4 && motherId2 == motherId3) && (mcMotherPdg1 == 111) && (mcMotherPdg2 == 22)
                    && grandmotherId2 == motherId1)) {
              Double_t invmass =
                Invmass_4particles(fMCTracklist_all[i], fMCTracklist_all[j], fMCTracklist_all[k], fMCTracklist_all[l]);
              fhInvariantMass_MC_pi0_gepem->Fill(invmass);
              fhInvariantMass_MC_pi0->Fill(invmass);

              fhMC_electrons_theta->Fill(momentum1.Theta() * 180. / TMath::Pi());
              fhMC_electrons_theta->Fill(momentum2.Theta() * 180. / TMath::Pi());
              fhMC_electrons_theta->Fill(momentum3.Theta() * 180. / TMath::Pi());
              fhMC_electrons_theta->Fill(momentum4.Theta() * 180. / TMath::Pi());
              fhMC_electrons_p->Fill(momentum1.Mag());
              fhMC_electrons_p->Fill(momentum2.Mag());
              fhMC_electrons_p->Fill(momentum3.Mag());
              fhMC_electrons_p->Fill(momentum4.Mag());
              fhMC_electrons_theta_vs_p->Fill(momentum1.Theta() * 180. / TMath::Pi(), momentum1.Mag());
              fhMC_electrons_theta_vs_p->Fill(momentum2.Theta() * 180. / TMath::Pi(), momentum2.Mag());
              fhMC_electrons_theta_vs_p->Fill(momentum3.Theta() * 180. / TMath::Pi(), momentum3.Mag());
              fhMC_electrons_theta_vs_p->Fill(momentum4.Theta() * 180. / TMath::Pi(), momentum4.Mag());
            }
            // decay eta -> g e+ e- -> e+ e- e+ e-
            if (((motherId1 == motherId2 && motherId3 == motherId4) && (mcMotherPdg1 == 22) && (mcMotherPdg3 == 221)
                 && grandmotherId1 == motherId3)
                || ((motherId1 == motherId2 && motherId3 == motherId4) && (mcMotherPdg1 == 221) && (mcMotherPdg3 == 22)
                    && grandmotherId3 == motherId1)
                || ((motherId1 == motherId3 && motherId2 == motherId4) && (mcMotherPdg1 == 22) && (mcMotherPdg2 == 221)
                    && grandmotherId1 == motherId2)
                || ((motherId1 == motherId3 && motherId2 == motherId4) && (mcMotherPdg1 == 221) && (mcMotherPdg2 == 22)
                    && grandmotherId2 == motherId1)
                || ((motherId1 == motherId4 && motherId2 == motherId3) && (mcMotherPdg1 == 22) && (mcMotherPdg2 == 221)
                    && grandmotherId1 == motherId2)
                || ((motherId1 == motherId4 && motherId2 == motherId3) && (mcMotherPdg1 == 221) && (mcMotherPdg2 == 22)
                    && grandmotherId2 == motherId1)) {
              Double_t invmass =
                Invmass_4particles(fMCTracklist_all[i], fMCTracklist_all[j], fMCTracklist_all[k], fMCTracklist_all[l]);
              fhInvariantMass_MC_eta->Fill(invmass);
            }
          }
        }
      }
    }
  }
  cout << "CbmAnaConversionReco: InvariantMassTestMC - End!" << endl;

  timer.Stop();
  fTime += timer.RealTime();
}


Double_t CbmAnaConversionReco::Invmass_4particles(const CbmMCTrack* mctrack1, const CbmMCTrack* mctrack2,
                                                  const CbmMCTrack* mctrack3, const CbmMCTrack* mctrack4)
// calculation of invariant mass from four electrons/positrons
{
  /*    TVector3 mom1;
    mctrack1->GetMomentum(mom1);
    TVector3 tempmom1;
    tempmom1.SetX(SmearValue(mom1.X()));
    tempmom1.SetY(SmearValue(mom1.Y()));
    tempmom1.SetZ(SmearValue(mom1.Z()));
    Double_t energy1 = TMath::Sqrt(tempmom1.Mag2() + M2E);
    TLorentzVector lorVec1(tempmom1, energy1);

    TVector3 mom2;
    mctrack2->GetMomentum(mom2);
    TVector3 tempmom2;
    tempmom2.SetX(SmearValue(mom2.X()));
    tempmom2.SetY(SmearValue(mom2.Y()));
    tempmom2.SetZ(SmearValue(mom2.Z()));
    Double_t energy2 = TMath::Sqrt(tempmom2.Mag2() + M2E);
    TLorentzVector lorVec2(tempmom2, energy2);

    TVector3 mom3;
    mctrack3->GetMomentum(mom3);
    TVector3 tempmom3;
    tempmom3.SetX(SmearValue(mom3.X()));
    tempmom3.SetY(SmearValue(mom3.Y()));
    tempmom3.SetZ(SmearValue(mom3.Z()));
    Double_t energy3 = TMath::Sqrt(tempmom3.Mag2() + M2E);
    TLorentzVector lorVec3(tempmom3, energy3);

    TVector3 mom4;
    mctrack4->GetMomentum(mom4);
    TVector3 tempmom4;
    tempmom4.SetX(SmearValue(mom4.X()));
    tempmom4.SetY(SmearValue(mom4.Y()));
    tempmom4.SetZ(SmearValue(mom4.Z()));
    Double_t energy4 = TMath::Sqrt(tempmom4.Mag2() + M2E);
    TLorentzVector lorVec4(tempmom4, energy4);
*/
  TLorentzVector lorVec1;
  mctrack1->Get4Momentum(lorVec1);

  TLorentzVector lorVec2;
  mctrack2->Get4Momentum(lorVec2);

  TLorentzVector lorVec3;
  mctrack3->Get4Momentum(lorVec3);

  TLorentzVector lorVec4;
  mctrack4->Get4Momentum(lorVec4);


  TLorentzVector sum;
  sum = lorVec1 + lorVec2 + lorVec3 + lorVec4;
  //cout << "mc: \t" << sum.Px() << " / " << sum.Py() << " / " << sum.Pz() << " / " << sum.E() << "\t => mag = " << sum.Mag() << endl;


  return sum.Mag();
}


Double_t CbmAnaConversionReco::SmearValue(Double_t value)
{
  TRandom3 generator(0);
  Double_t result = 0;
  //	Double_t smear = 0;
  Int_t plusminus = 0;
  while (plusminus == 0) {  // should be either 1 or -1, not 0
    plusminus = generator.Uniform(-2, 2);
  }
  //	Double_t gaus = generator.Gaus(1,1);
  //	smear = gaus * plusminus;
  //	result = value * (1. + 1.0*smear/100);		//smearing as wished

  result = value;  // -> no smearing

  return result;
}


Double_t CbmAnaConversionReco::Invmass_4particlesRECO(const TVector3 part1, const TVector3 part2, const TVector3 part3,
                                                      const TVector3 part4)
// calculation of invariant mass from four electrons/positrons
{
  Double_t energy1 = TMath::Sqrt(part1.Mag2() + M2E);
  TLorentzVector lorVec1(part1, energy1);

  Double_t energy2 = TMath::Sqrt(part2.Mag2() + M2E);
  TLorentzVector lorVec2(part2, energy2);

  Double_t energy3 = TMath::Sqrt(part3.Mag2() + M2E);
  TLorentzVector lorVec3(part3, energy3);

  Double_t energy4 = TMath::Sqrt(part4.Mag2() + M2E);
  TLorentzVector lorVec4(part4, energy4);

  TLorentzVector sum;
  sum = lorVec1 + lorVec2 + lorVec3 + lorVec4;
  //cout << "reco: \t" << sum.Px() << " / " << sum.Py() << " / " << sum.Pz() << " / " << sum.E() << "\t => mag = " << sum.Mag() << endl;


  return sum.Mag();
}


void CbmAnaConversionReco::InvariantMassTest_4epem()
// Calculating invariant mass of 4 ep/em, using MC data AND reconstructed momentum
{
  timer.Start();


  cout << "CbmAnaConversionReco: InvariantMassTest_4epem - Start..." << endl;
  cout << "CbmAnaConversionReco: InvariantMassTest_4epem - " << fRecoTracklistEPEM.size() << "\t"
       << fRecoMomentum.size() << endl;
  int fill = 0;
  if (fRecoTracklistEPEM.size() < 4) return;
  for (unsigned int i = 0; i < fRecoTracklistEPEM.size(); i++) {
    if (i % 10 == 0) cout << "CbmAnaConversionReco: InvariantMassTest_4epem - iteration i = " << i << endl;
    for (unsigned int j = i + 1; j < fRecoTracklistEPEM.size(); j++) {
      for (unsigned int k = j + 1; k < fRecoTracklistEPEM.size(); k++) {
        for (unsigned int l = k + 1; l < fRecoTracklistEPEM.size(); l++) {
          if (fRecoTracklistEPEM[i]->GetPdgCode() + fRecoTracklistEPEM[j]->GetPdgCode()
                + fRecoTracklistEPEM[k]->GetPdgCode() + fRecoTracklistEPEM[l]->GetPdgCode()
              != 0)
            continue;

          if (fRecoTracklistEPEM.size() != fRecoMomentum.size()
              || fRecoTracklistEPEM.size() != fRecoRefittedMomentum.size()) {
            cout << "CbmAnaConversionReco: InvariantMassTest_4epem - not "
                    "matching number of entries!"
                 << endl;
            continue;
          }


          // starting points of each electron (-> i.e. conversion points of gamma OR decay points of pi0, depending on decay channel)
          TVector3 pi0start_i;
          fRecoTracklistEPEM[i]->GetStartVertex(pi0start_i);
          TVector3 pi0start_j;
          fRecoTracklistEPEM[j]->GetStartVertex(pi0start_j);
          TVector3 pi0start_k;
          fRecoTracklistEPEM[k]->GetStartVertex(pi0start_k);
          TVector3 pi0start_l;
          fRecoTracklistEPEM[l]->GetStartVertex(pi0start_l);


          int motherId1 = fRecoTracklistEPEM[i]->GetMotherId();
          int motherId2 = fRecoTracklistEPEM[j]->GetMotherId();
          int motherId3 = fRecoTracklistEPEM[k]->GetMotherId();
          int motherId4 = fRecoTracklistEPEM[l]->GetMotherId();

          // decay pi0 -> e+ e- e+ e-
          if (motherId1 == motherId2 && motherId1 == motherId3 && motherId1 == motherId4) {
            if (motherId1 != -1) {
              int mcMotherPdg1    = -1;
              CbmMCTrack* mother1 = (CbmMCTrack*) fMcTracks->At(motherId1);
              if (NULL != mother1) mcMotherPdg1 = mother1->GetPdgCode();
              if ((mcMotherPdg1 == 111 || mcMotherPdg1 == 221)) {  // && NofDaughters(motherId1) == 4) {
                Double_t invmass1 = 0;
                //								Double_t invmass2 = 0;	// momenta from stsMomentumVec
                Double_t invmass3 = 0;
                //	invmass2 = Invmass_4particlesRECO(fRecoMomentum[i], fRecoMomentum[j], fRecoMomentum[k], fRecoMomentum[l]);
                invmass1 = Invmass_4particles(fRecoTracklistEPEM[i], fRecoTracklistEPEM[j], fRecoTracklistEPEM[k],
                                              fRecoTracklistEPEM[l]);  // true MC values
                invmass3 = Invmass_4particlesRECO(fRecoRefittedMomentum[i], fRecoRefittedMomentum[j],
                                                  fRecoRefittedMomentum[k], fRecoRefittedMomentum[l]);
                //	fhInvariantMass_pi0epem->Fill(invmass2);
                cout << "Decay pi0 -> e+e-e+e- detected!\t\t mc mass: " << invmass1 << "\t, reco mass: " << invmass3
                     << endl;
                cout << "motherids: " << motherId1 << "/" << motherId2 << "/" << motherId3 << "/" << motherId4
                     << "\t motherpdg: " << mcMotherPdg1 << "\t mctrack mass: " << mother1->GetMass()
                     << "\t nofdaughters: " << NofDaughters(motherId1) << endl;
                cout << "pdgs " << fRecoTracklistEPEM[i]->GetPdgCode() << "/" << fRecoTracklistEPEM[j]->GetPdgCode()
                     << "/" << fRecoTracklistEPEM[k]->GetPdgCode() << "/" << fRecoTracklistEPEM[l]->GetPdgCode()
                     << endl;
                TVector3 start1;
                fRecoTracklistEPEM[i]->GetStartVertex(start1);
                TVector3 start2;
                fRecoTracklistEPEM[j]->GetStartVertex(start2);
                TVector3 start3;
                fRecoTracklistEPEM[k]->GetStartVertex(start3);
                TVector3 start4;
                fRecoTracklistEPEM[l]->GetStartVertex(start4);
                cout << "start: " << start1.Z() << "/" << start2.Z() << "/" << start3.Z() << "/" << start4.Z() << endl;

                fhEPEM_invmass_eeee_mc->Fill(invmass1);
                fhEPEM_invmass_eeee_refitted->Fill(invmass3);
                //fhEPEM_invmass_all_mc->Fill(invmass1);
                //fhEPEM_invmass_all_refitted->Fill(invmass3);
              }
            }
            else {  // all 4 particles come directly from the vertex
              continue;
            }
          }

          if ((motherId1 == motherId2 && motherId3 == motherId4) || (motherId1 == motherId3 && motherId2 == motherId4)
              || (motherId1 == motherId4 && motherId2 == motherId3)) {


            int grandmotherId1 = -1;
            int grandmotherId2 = -1;
            int grandmotherId3 = -1;
            int grandmotherId4 = -1;

            int mcMotherPdg1      = -1;
            int mcMotherPdg2      = -1;
            int mcMotherPdg3      = -1;
            int mcMotherPdg4      = -1;
            int mcGrandmotherPdg1 = -1;
            //						int mcGrandmotherPdg2  = -1;
            //						int mcGrandmotherPdg3  = -1;
            //						int mcGrandmotherPdg4  = -1;

            CbmMCTrack* grandmother1 = NULL;

            if (motherId1 != -1) {
              CbmMCTrack* mother1 = (CbmMCTrack*) fMcTracks->At(motherId1);
              if (NULL != mother1) mcMotherPdg1 = mother1->GetPdgCode();
              grandmotherId1 = mother1->GetMotherId();
              if (grandmotherId1 != -1) {
                grandmother1 = (CbmMCTrack*) fMcTracks->At(grandmotherId1);
                if (NULL != grandmother1) mcGrandmotherPdg1 = grandmother1->GetPdgCode();
              }
            }
            if (motherId2 != -1) {
              CbmMCTrack* mother2 = (CbmMCTrack*) fMcTracks->At(motherId2);
              if (NULL != mother2) mcMotherPdg2 = mother2->GetPdgCode();
              grandmotherId2 = mother2->GetMotherId();
              if (grandmotherId2 != -1) {
                //								CbmMCTrack* grandmother2 = (CbmMCTrack*) fMcTracks->At(grandmotherId2);
                //								if (NULL != grandmother2) mcGrandmotherPdg2 = grandmother2->GetPdgCode();
              }
            }
            if (motherId3 != -1) {
              CbmMCTrack* mother3 = (CbmMCTrack*) fMcTracks->At(motherId3);
              if (NULL != mother3) mcMotherPdg3 = mother3->GetPdgCode();
              grandmotherId3 = mother3->GetMotherId();
              if (grandmotherId3 != -1) {
                //								CbmMCTrack* grandmother3 = (CbmMCTrack*) fMcTracks->At(grandmotherId3);
                //								if (NULL != grandmother3) mcGrandmotherPdg3 = grandmother3->GetPdgCode();
              }
            }
            if (motherId4 != -1) {
              CbmMCTrack* mother4 = (CbmMCTrack*) fMcTracks->At(motherId4);
              if (NULL != mother4) mcMotherPdg4 = mother4->GetPdgCode();
              grandmotherId4 = mother4->GetMotherId();
              if (grandmotherId4 != -1) {
                //								CbmMCTrack* grandmother4 = (CbmMCTrack*) fMcTracks->At(grandmotherId4);
                //								if (NULL != grandmother4) mcGrandmotherPdg4 = grandmother4->GetPdgCode();
              }
            }

            //if(grandmotherId1 == -1) continue;

            if (motherId1 == motherId2 && motherId3 == motherId4) {
              if (NofDaughters(motherId1) != 2 || NofDaughters(motherId3) != 2) continue;
              if ((grandmotherId1 == motherId3 && mcMotherPdg3 == 111)
                  || (motherId1 == grandmotherId3 && mcMotherPdg1 == 111)) {

                fhPi0_startvertexElectrons_gee->Fill(pi0start_i.Z());
                fhPi0_startvertexElectrons_gee->Fill(pi0start_j.Z());
                fhPi0_startvertexElectrons_gee->Fill(pi0start_k.Z());
                fhPi0_startvertexElectrons_gee->Fill(pi0start_l.Z());
                fhPi0_startvertexElectrons_all->Fill(pi0start_i.Z());
                fhPi0_startvertexElectrons_all->Fill(pi0start_j.Z());
                fhPi0_startvertexElectrons_all->Fill(pi0start_k.Z());
                fhPi0_startvertexElectrons_all->Fill(pi0start_l.Z());

                Double_t invmass1 = 0;
                Double_t invmass3 = 0;
                invmass1 = Invmass_4particles(fRecoTracklistEPEM[i], fRecoTracklistEPEM[j], fRecoTracklistEPEM[k],
                                              fRecoTracklistEPEM[l]);  // true MC values
                invmass3 = Invmass_4particlesRECO(fRecoRefittedMomentum[i], fRecoRefittedMomentum[j],
                                                  fRecoRefittedMomentum[k], fRecoRefittedMomentum[l]);

                // consider only electrons from the target (only then the momenta are correctly refitted/reconstructed)
                if (pi0start_i.Z() > 1 || pi0start_j.Z() > 1 || pi0start_k.Z() > 1 || pi0start_l.Z() > 1) {
                  fhEPEM_InDetector_invmass_gee_mc->Fill(invmass1);
                  fhEPEM_InDetector_invmass_gee_refitted->Fill(invmass3);
                  fhEPEM_InDetector_invmass_all_mc->Fill(invmass1);
                  fhEPEM_InDetector_invmass_all_refitted->Fill(invmass3);
                  continue;
                }


                cout << "HURRAY! .-.-.-.-.-.-..-.-.-.-.-.-.-.-.-.-.-.-." << endl;
                CutEfficiencyStudies(i, j, k, l, motherId1, motherId2, motherId3, motherId4);
                fhInvariantMass_pi0epem->Fill(invmass1);

                fhEPEM_invmass_gee_mc->Fill(invmass1);
                fhEPEM_invmass_gee_refitted->Fill(invmass3);
                fhEPEM_invmass_all_mc->Fill(invmass1);
                fhEPEM_invmass_all_refitted->Fill(invmass3);

                fhEPEM_pt_vs_p_all_mc->Fill(fRecoTracklistEPEM[i]->GetPt(), fRecoTracklistEPEM[i]->GetP());
                fhEPEM_pt_vs_p_all_mc->Fill(fRecoTracklistEPEM[j]->GetPt(), fRecoTracklistEPEM[j]->GetP());
                fhEPEM_pt_vs_p_all_mc->Fill(fRecoTracklistEPEM[k]->GetPt(), fRecoTracklistEPEM[k]->GetP());
                fhEPEM_pt_vs_p_all_mc->Fill(fRecoTracklistEPEM[l]->GetPt(), fRecoTracklistEPEM[l]->GetP());
                fhEPEM_pt_vs_p_all_refitted->Fill(fRecoRefittedMomentum[i].Perp(), fRecoRefittedMomentum[i].Mag());
                fhEPEM_pt_vs_p_all_refitted->Fill(fRecoRefittedMomentum[j].Perp(), fRecoRefittedMomentum[j].Mag());
                fhEPEM_pt_vs_p_all_refitted->Fill(fRecoRefittedMomentum[k].Perp(), fRecoRefittedMomentum[k].Mag());
                fhEPEM_pt_vs_p_all_refitted->Fill(fRecoRefittedMomentum[l].Perp(), fRecoRefittedMomentum[l].Mag());

                LmvmKinePar params1 =
                  CalculateKinematicParams_4particles(fRecoRefittedMomentum[i], fRecoRefittedMomentum[j],
                                                      fRecoRefittedMomentum[k], fRecoRefittedMomentum[l]);
                fhPi0_pt_vs_rap_gee->Fill(params1.fPt, params1.fRapidity);
                fhPi0_pt_vs_rap_all->Fill(params1.fPt, params1.fRapidity);
                fhPi0_pt_gee->Fill(params1.fPt);
                fhPi0_pt_all->Fill(params1.fPt);

                if (mcGrandmotherPdg1 == 111) {  // case: i,j = electrons from gamma, k,l = electrons from pi0
                  Double_t opening_angle1_mc       = 0;
                  Double_t opening_angle1_refitted = 0;
                  opening_angle1_mc = CalculateOpeningAngleMC(fRecoTracklistEPEM[i], fRecoTracklistEPEM[j]);
                  opening_angle1_refitted =
                    CalculateOpeningAngleReco(fRecoRefittedMomentum[i], fRecoRefittedMomentum[j]);
                  fhEPEM_openingAngle_gee_mc->Fill(opening_angle1_mc);
                  fhEPEM_openingAngle_gee_refitted->Fill(opening_angle1_refitted);

                  Double_t opening_angle1_mc_dalitz       = 0;
                  Double_t opening_angle1_refitted_dalitz = 0;
                  opening_angle1_mc_dalitz = CalculateOpeningAngleMC(fRecoTracklistEPEM[k], fRecoTracklistEPEM[l]);
                  opening_angle1_refitted_dalitz =
                    CalculateOpeningAngleReco(fRecoRefittedMomentum[k], fRecoRefittedMomentum[l]);
                  fhEPEM_openingAngle_gee_mc_dalitz->Fill(opening_angle1_mc_dalitz);
                  fhEPEM_openingAngle_gee_refitted_dalitz->Fill(opening_angle1_refitted_dalitz);
                }

                if (mcMotherPdg1 == 111) {  // case: i,j = electrons from pi0, k,l = electrons from gamma
                  Double_t opening_angle1_mc       = 0;
                  Double_t opening_angle1_refitted = 0;
                  opening_angle1_mc = CalculateOpeningAngleMC(fRecoTracklistEPEM[k], fRecoTracklistEPEM[l]);
                  opening_angle1_refitted =
                    CalculateOpeningAngleReco(fRecoRefittedMomentum[k], fRecoRefittedMomentum[l]);
                  fhEPEM_openingAngle_gee_mc->Fill(opening_angle1_mc);
                  fhEPEM_openingAngle_gee_refitted->Fill(opening_angle1_refitted);

                  Double_t opening_angle1_mc_dalitz       = 0;
                  Double_t opening_angle1_refitted_dalitz = 0;
                  opening_angle1_mc_dalitz = CalculateOpeningAngleMC(fRecoTracklistEPEM[i], fRecoTracklistEPEM[j]);
                  opening_angle1_refitted_dalitz =
                    CalculateOpeningAngleReco(fRecoRefittedMomentum[i], fRecoRefittedMomentum[j]);
                  fhEPEM_openingAngle_gee_mc_dalitz->Fill(opening_angle1_mc_dalitz);
                  fhEPEM_openingAngle_gee_refitted_dalitz->Fill(opening_angle1_refitted_dalitz);
                }
              }
            }
            if (motherId1 == motherId3 && motherId2 == motherId4) {
              if (NofDaughters(motherId1) != 2 || NofDaughters(motherId2) != 2) continue;
              if ((grandmotherId1 == motherId2 && mcMotherPdg2 == 111)
                  || (motherId1 == grandmotherId2 && mcMotherPdg1 == 111)) {

                fhPi0_startvertexElectrons_gee->Fill(pi0start_i.Z());
                fhPi0_startvertexElectrons_gee->Fill(pi0start_j.Z());
                fhPi0_startvertexElectrons_gee->Fill(pi0start_k.Z());
                fhPi0_startvertexElectrons_gee->Fill(pi0start_l.Z());
                fhPi0_startvertexElectrons_all->Fill(pi0start_i.Z());
                fhPi0_startvertexElectrons_all->Fill(pi0start_j.Z());
                fhPi0_startvertexElectrons_all->Fill(pi0start_k.Z());
                fhPi0_startvertexElectrons_all->Fill(pi0start_l.Z());

                Double_t invmass1 = 0;
                Double_t invmass3 = 0;
                invmass1 = Invmass_4particles(fRecoTracklistEPEM[i], fRecoTracklistEPEM[j], fRecoTracklistEPEM[k],
                                              fRecoTracklistEPEM[l]);  // true MC values
                invmass3 = Invmass_4particlesRECO(fRecoRefittedMomentum[i], fRecoRefittedMomentum[j],
                                                  fRecoRefittedMomentum[k], fRecoRefittedMomentum[l]);

                // consider only electrons from the target (only then the momenta are correctly refitted/reconstructed)
                if (pi0start_i.Z() > 1 || pi0start_j.Z() > 1 || pi0start_k.Z() > 1 || pi0start_l.Z() > 1) {
                  fhEPEM_InDetector_invmass_gee_mc->Fill(invmass1);
                  fhEPEM_InDetector_invmass_gee_refitted->Fill(invmass3);
                  fhEPEM_InDetector_invmass_all_mc->Fill(invmass1);
                  fhEPEM_InDetector_invmass_all_refitted->Fill(invmass3);
                  continue;
                }


                cout << "HURRAY! .-.-.-.-.-.-..-.-.-.-.-.-.-.-.-.-.-.-." << endl;
                CutEfficiencyStudies(i, j, k, l, motherId1, motherId2, motherId3, motherId4);
                fhInvariantMass_pi0epem->Fill(invmass1);

                fhEPEM_invmass_gee_mc->Fill(invmass1);
                fhEPEM_invmass_gee_refitted->Fill(invmass3);
                fhEPEM_invmass_all_mc->Fill(invmass1);
                fhEPEM_invmass_all_refitted->Fill(invmass3);

                fhEPEM_pt_vs_p_all_mc->Fill(fRecoTracklistEPEM[i]->GetPt(), fRecoTracklistEPEM[i]->GetP());
                fhEPEM_pt_vs_p_all_mc->Fill(fRecoTracklistEPEM[j]->GetPt(), fRecoTracklistEPEM[j]->GetP());
                fhEPEM_pt_vs_p_all_mc->Fill(fRecoTracklistEPEM[k]->GetPt(), fRecoTracklistEPEM[k]->GetP());
                fhEPEM_pt_vs_p_all_mc->Fill(fRecoTracklistEPEM[l]->GetPt(), fRecoTracklistEPEM[l]->GetP());
                fhEPEM_pt_vs_p_all_refitted->Fill(fRecoRefittedMomentum[i].Perp(), fRecoRefittedMomentum[i].Mag());
                fhEPEM_pt_vs_p_all_refitted->Fill(fRecoRefittedMomentum[j].Perp(), fRecoRefittedMomentum[j].Mag());
                fhEPEM_pt_vs_p_all_refitted->Fill(fRecoRefittedMomentum[k].Perp(), fRecoRefittedMomentum[k].Mag());
                fhEPEM_pt_vs_p_all_refitted->Fill(fRecoRefittedMomentum[l].Perp(), fRecoRefittedMomentum[l].Mag());

                LmvmKinePar params1 =
                  CalculateKinematicParams_4particles(fRecoRefittedMomentum[i], fRecoRefittedMomentum[j],
                                                      fRecoRefittedMomentum[k], fRecoRefittedMomentum[l]);
                fhPi0_pt_vs_rap_gee->Fill(params1.fPt, params1.fRapidity);
                fhPi0_pt_vs_rap_all->Fill(params1.fPt, params1.fRapidity);
                fhPi0_pt_gee->Fill(params1.fPt);
                fhPi0_pt_all->Fill(params1.fPt);

                if (mcGrandmotherPdg1 == 111) {
                  Double_t opening_angle1_mc       = 0;
                  Double_t opening_angle1_refitted = 0;
                  opening_angle1_mc = CalculateOpeningAngleMC(fRecoTracklistEPEM[i], fRecoTracklistEPEM[k]);
                  opening_angle1_refitted =
                    CalculateOpeningAngleReco(fRecoRefittedMomentum[i], fRecoRefittedMomentum[k]);
                  fhEPEM_openingAngle_gee_mc->Fill(opening_angle1_mc);
                  fhEPEM_openingAngle_gee_refitted->Fill(opening_angle1_refitted);

                  Double_t opening_angle1_mc_dalitz       = 0;
                  Double_t opening_angle1_refitted_dalitz = 0;
                  opening_angle1_mc_dalitz = CalculateOpeningAngleMC(fRecoTracklistEPEM[j], fRecoTracklistEPEM[l]);
                  opening_angle1_refitted_dalitz =
                    CalculateOpeningAngleReco(fRecoRefittedMomentum[j], fRecoRefittedMomentum[l]);
                  fhEPEM_openingAngle_gee_mc_dalitz->Fill(opening_angle1_mc_dalitz);
                  fhEPEM_openingAngle_gee_refitted_dalitz->Fill(opening_angle1_refitted_dalitz);
                }

                if (mcMotherPdg1 == 111) {
                  Double_t opening_angle1_mc       = 0;
                  Double_t opening_angle1_refitted = 0;
                  opening_angle1_mc = CalculateOpeningAngleMC(fRecoTracklistEPEM[j], fRecoTracklistEPEM[l]);
                  opening_angle1_refitted =
                    CalculateOpeningAngleReco(fRecoRefittedMomentum[j], fRecoRefittedMomentum[l]);
                  fhEPEM_openingAngle_gee_mc->Fill(opening_angle1_mc);
                  fhEPEM_openingAngle_gee_refitted->Fill(opening_angle1_refitted);

                  Double_t opening_angle1_mc_dalitz       = 0;
                  Double_t opening_angle1_refitted_dalitz = 0;
                  opening_angle1_mc_dalitz = CalculateOpeningAngleMC(fRecoTracklistEPEM[i], fRecoTracklistEPEM[k]);
                  opening_angle1_refitted_dalitz =
                    CalculateOpeningAngleReco(fRecoRefittedMomentum[i], fRecoRefittedMomentum[k]);
                  fhEPEM_openingAngle_gee_mc_dalitz->Fill(opening_angle1_mc_dalitz);
                  fhEPEM_openingAngle_gee_refitted_dalitz->Fill(opening_angle1_refitted_dalitz);
                }
              }
            }
            if (motherId1 == motherId4 && motherId2 == motherId3) {
              if (NofDaughters(motherId1) != 2 || NofDaughters(motherId2) != 2) continue;
              if ((grandmotherId1 == motherId2 && mcMotherPdg2 == 111)
                  || (motherId1 == grandmotherId2 && mcMotherPdg1 == 111)) {

                fhPi0_startvertexElectrons_gee->Fill(pi0start_i.Z());
                fhPi0_startvertexElectrons_gee->Fill(pi0start_j.Z());
                fhPi0_startvertexElectrons_gee->Fill(pi0start_k.Z());
                fhPi0_startvertexElectrons_gee->Fill(pi0start_l.Z());
                fhPi0_startvertexElectrons_all->Fill(pi0start_i.Z());
                fhPi0_startvertexElectrons_all->Fill(pi0start_j.Z());
                fhPi0_startvertexElectrons_all->Fill(pi0start_k.Z());
                fhPi0_startvertexElectrons_all->Fill(pi0start_l.Z());

                Double_t invmass1 = 0;
                Double_t invmass3 = 0;
                invmass1 = Invmass_4particles(fRecoTracklistEPEM[i], fRecoTracklistEPEM[j], fRecoTracklistEPEM[k],
                                              fRecoTracklistEPEM[l]);  // true MC values
                invmass3 = Invmass_4particlesRECO(fRecoRefittedMomentum[i], fRecoRefittedMomentum[j],
                                                  fRecoRefittedMomentum[k], fRecoRefittedMomentum[l]);

                // consider only electrons from the target (only then the momenta are correctly refitted/reconstructed)
                if (pi0start_i.Z() > 1 || pi0start_j.Z() > 1 || pi0start_k.Z() > 1 || pi0start_l.Z() > 1) {
                  fhEPEM_InDetector_invmass_gee_mc->Fill(invmass1);
                  fhEPEM_InDetector_invmass_gee_refitted->Fill(invmass3);
                  fhEPEM_InDetector_invmass_all_mc->Fill(invmass1);
                  fhEPEM_InDetector_invmass_all_refitted->Fill(invmass3);
                  continue;
                }


                cout << "HURRAY! .-.-.-.-.-.-..-.-.-.-.-.-.-.-.-.-.-.-." << endl;
                CutEfficiencyStudies(i, j, k, l, motherId1, motherId2, motherId3, motherId4);
                fhInvariantMass_pi0epem->Fill(invmass1);

                fhEPEM_invmass_gee_mc->Fill(invmass1);
                fhEPEM_invmass_gee_refitted->Fill(invmass3);
                fhEPEM_invmass_all_mc->Fill(invmass1);
                fhEPEM_invmass_all_refitted->Fill(invmass3);

                fhEPEM_pt_vs_p_all_mc->Fill(fRecoTracklistEPEM[i]->GetPt(), fRecoTracklistEPEM[i]->GetP());
                fhEPEM_pt_vs_p_all_mc->Fill(fRecoTracklistEPEM[j]->GetPt(), fRecoTracklistEPEM[j]->GetP());
                fhEPEM_pt_vs_p_all_mc->Fill(fRecoTracklistEPEM[k]->GetPt(), fRecoTracklistEPEM[k]->GetP());
                fhEPEM_pt_vs_p_all_mc->Fill(fRecoTracklistEPEM[l]->GetPt(), fRecoTracklistEPEM[l]->GetP());
                fhEPEM_pt_vs_p_all_refitted->Fill(fRecoRefittedMomentum[i].Perp(), fRecoRefittedMomentum[i].Mag());
                fhEPEM_pt_vs_p_all_refitted->Fill(fRecoRefittedMomentum[j].Perp(), fRecoRefittedMomentum[j].Mag());
                fhEPEM_pt_vs_p_all_refitted->Fill(fRecoRefittedMomentum[k].Perp(), fRecoRefittedMomentum[k].Mag());
                fhEPEM_pt_vs_p_all_refitted->Fill(fRecoRefittedMomentum[l].Perp(), fRecoRefittedMomentum[l].Mag());

                LmvmKinePar params1 =
                  CalculateKinematicParams_4particles(fRecoRefittedMomentum[i], fRecoRefittedMomentum[j],
                                                      fRecoRefittedMomentum[k], fRecoRefittedMomentum[l]);
                fhPi0_pt_vs_rap_gee->Fill(params1.fPt, params1.fRapidity);
                fhPi0_pt_vs_rap_all->Fill(params1.fPt, params1.fRapidity);
                fhPi0_pt_gee->Fill(params1.fPt);
                fhPi0_pt_all->Fill(params1.fPt);

                if (mcGrandmotherPdg1 == 111) {
                  Double_t opening_angle1_mc       = 0;
                  Double_t opening_angle1_refitted = 0;
                  opening_angle1_mc = CalculateOpeningAngleMC(fRecoTracklistEPEM[i], fRecoTracklistEPEM[l]);
                  opening_angle1_refitted =
                    CalculateOpeningAngleReco(fRecoRefittedMomentum[i], fRecoRefittedMomentum[l]);
                  fhEPEM_openingAngle_gee_mc->Fill(opening_angle1_mc);
                  fhEPEM_openingAngle_gee_refitted->Fill(opening_angle1_refitted);

                  Double_t opening_angle1_mc_dalitz       = 0;
                  Double_t opening_angle1_refitted_dalitz = 0;
                  opening_angle1_mc_dalitz = CalculateOpeningAngleMC(fRecoTracklistEPEM[j], fRecoTracklistEPEM[k]);
                  opening_angle1_refitted_dalitz =
                    CalculateOpeningAngleReco(fRecoRefittedMomentum[j], fRecoRefittedMomentum[k]);
                  fhEPEM_openingAngle_gee_mc_dalitz->Fill(opening_angle1_mc_dalitz);
                  fhEPEM_openingAngle_gee_refitted_dalitz->Fill(opening_angle1_refitted_dalitz);
                }

                if (mcMotherPdg1 == 111) {
                  Double_t opening_angle1_mc       = 0;
                  Double_t opening_angle1_refitted = 0;
                  opening_angle1_mc = CalculateOpeningAngleMC(fRecoTracklistEPEM[j], fRecoTracklistEPEM[k]);
                  opening_angle1_refitted =
                    CalculateOpeningAngleReco(fRecoRefittedMomentum[j], fRecoRefittedMomentum[k]);
                  fhEPEM_openingAngle_gee_mc->Fill(opening_angle1_mc);
                  fhEPEM_openingAngle_gee_refitted->Fill(opening_angle1_refitted);

                  Double_t opening_angle1_mc_dalitz       = 0;
                  Double_t opening_angle1_refitted_dalitz = 0;
                  opening_angle1_mc_dalitz = CalculateOpeningAngleMC(fRecoTracklistEPEM[i], fRecoTracklistEPEM[l]);
                  opening_angle1_refitted_dalitz =
                    CalculateOpeningAngleReco(fRecoRefittedMomentum[i], fRecoRefittedMomentum[l]);
                  fhEPEM_openingAngle_gee_mc_dalitz->Fill(opening_angle1_mc_dalitz);
                  fhEPEM_openingAngle_gee_refitted_dalitz->Fill(opening_angle1_refitted_dalitz);
                }
              }
            }


            // ===================================================================================================
            // HERE DECAY pi0 -> gamma gamma -> e+e- e+e-
            if (grandmotherId1 == grandmotherId2 && grandmotherId1 == grandmotherId3
                && grandmotherId1 == grandmotherId4) {
              //		if(mcGrandmotherPdg1 != 111 && mcGrandmotherPdg1 != 221) continue; // 111 = pi0, 221 = eta


              if (pi0start_i.Z() <= 1 && pi0start_j.Z() <= 1 && pi0start_k.Z() <= 1 && pi0start_l.Z() <= 1
                  && mcGrandmotherPdg1 == 221) {
                Double_t invmass_eta_mc   = Invmass_4particles(fRecoTracklistEPEM[i], fRecoTracklistEPEM[j],
                                                             fRecoTracklistEPEM[k], fRecoTracklistEPEM[l]);
                Double_t invmass_eta_reco = Invmass_4particlesRECO(fRecoRefittedMomentum[i], fRecoRefittedMomentum[j],
                                                                   fRecoRefittedMomentum[k], fRecoRefittedMomentum[l]);

                fhEPEM_invmass_eta_mc->Fill(invmass_eta_mc);
                fhEPEM_invmass_eta_refitted->Fill(invmass_eta_reco);

                CutEfficiencyStudies(i, j, k, l, motherId1, motherId2, motherId3, motherId4, 1);
              }


              if (mcGrandmotherPdg1 != 111) continue;  // 111 = pi0, 221 = eta

              TVector3 pi0start;
              grandmother1->GetStartVertex(pi0start);
              fhPi0_startvertex->Fill(pi0start.Z());

              fhPi0_startvertexElectrons_gg->Fill(pi0start_i.Z());
              fhPi0_startvertexElectrons_gg->Fill(pi0start_j.Z());
              fhPi0_startvertexElectrons_gg->Fill(pi0start_k.Z());
              fhPi0_startvertexElectrons_gg->Fill(pi0start_l.Z());
              fhPi0_startvertexElectrons_all->Fill(pi0start_i.Z());
              fhPi0_startvertexElectrons_all->Fill(pi0start_j.Z());
              fhPi0_startvertexElectrons_all->Fill(pi0start_k.Z());
              fhPi0_startvertexElectrons_all->Fill(pi0start_l.Z());

              fhPi0_startvertex_vs_chi->Fill(pi0start_i.Z(), fRecoTracklistEPEM_chi[i]);
              fhPi0_startvertex_vs_chi->Fill(pi0start_j.Z(), fRecoTracklistEPEM_chi[j]);
              fhPi0_startvertex_vs_chi->Fill(pi0start_k.Z(), fRecoTracklistEPEM_chi[l]);
              fhPi0_startvertex_vs_chi->Fill(pi0start_l.Z(), fRecoTracklistEPEM_chi[l]);

              fhPi0_startvertex_vs_momentum->Fill(pi0start_i.Z(), fRecoTracklistEPEM[i]->GetP());
              fhPi0_startvertex_vs_momentum->Fill(pi0start_j.Z(), fRecoTracklistEPEM[j]->GetP());
              fhPi0_startvertex_vs_momentum->Fill(pi0start_k.Z(), fRecoTracklistEPEM[k]->GetP());
              fhPi0_startvertex_vs_momentum->Fill(pi0start_l.Z(), fRecoTracklistEPEM[l]->GetP());

              Double_t invmass1 = 0;  // true MC values
              invmass1 = Invmass_4particles(fRecoTracklistEPEM[i], fRecoTracklistEPEM[j], fRecoTracklistEPEM[k],
                                            fRecoTracklistEPEM[l]);
              Double_t invmass2 = 0;  // momenta from stsMomentumVec
              invmass2 = Invmass_4particlesRECO(fRecoMomentum[i], fRecoMomentum[j], fRecoMomentum[k], fRecoMomentum[l]);
              Double_t invmass3 = 0;  // momenta from refitted at primary vertex
              invmass3          = Invmass_4particlesRECO(fRecoRefittedMomentum[i], fRecoRefittedMomentum[j],
                                                fRecoRefittedMomentum[k], fRecoRefittedMomentum[l]);

              // consider only electrons from the target (only then the momenta are correctly refitted/reconstructed)
              if (pi0start_i.Z() > 1 || pi0start_j.Z() > 1 || pi0start_k.Z() > 1 || pi0start_l.Z() > 1) {
                fhEPEM_InDetector_invmass_gg_mc->Fill(invmass1);
                fhEPEM_InDetector_invmass_gg_refitted->Fill(invmass3);
                fhEPEM_InDetector_invmass_all_mc->Fill(invmass1);
                fhEPEM_InDetector_invmass_all_refitted->Fill(invmass3);
                continue;
              }

              fhEPEM_pt_vs_p_all_mc->Fill(fRecoTracklistEPEM[i]->GetPt(), fRecoTracklistEPEM[i]->GetP());
              fhEPEM_pt_vs_p_all_mc->Fill(fRecoTracklistEPEM[j]->GetPt(), fRecoTracklistEPEM[j]->GetP());
              fhEPEM_pt_vs_p_all_mc->Fill(fRecoTracklistEPEM[k]->GetPt(), fRecoTracklistEPEM[k]->GetP());
              fhEPEM_pt_vs_p_all_mc->Fill(fRecoTracklistEPEM[l]->GetPt(), fRecoTracklistEPEM[l]->GetP());
              fhEPEM_pt_vs_p_all_refitted->Fill(fRecoRefittedMomentum[i].Perp(), fRecoRefittedMomentum[i].Mag());
              fhEPEM_pt_vs_p_all_refitted->Fill(fRecoRefittedMomentum[j].Perp(), fRecoRefittedMomentum[j].Mag());
              fhEPEM_pt_vs_p_all_refitted->Fill(fRecoRefittedMomentum[k].Perp(), fRecoRefittedMomentum[k].Mag());
              fhEPEM_pt_vs_p_all_refitted->Fill(fRecoRefittedMomentum[l].Perp(), fRecoRefittedMomentum[l].Mag());

              cout << "########################################################"
                      "##############"
                   << endl;
              cout << fRecoMomentum[i].X() << "\t" << fRecoRefittedMomentum[i].X() << endl;
              cout << fRecoMomentum[i].Y() << "\t" << fRecoRefittedMomentum[i].Y() << endl;
              cout << fRecoMomentum[i].Z() << "\t" << fRecoRefittedMomentum[i].Z() << endl;

              if (fRecoTracklistEPEM_ids.size() != fRecoTracklistEPEM.size()) {
                cout << "size mismatch! " << fRecoTracklistEPEM_ids.size() << "/" << fRecoTracklistEPEM.size() << endl;
              }

              cout << "########################################################"
                      "##############"
                   << endl;
              cout << "index: " << i << "\t" << j << "\t" << k << "\t" << l << endl;
              cout << "mc id: " << fRecoTracklistEPEM_ids[i] << "\t" << fRecoTracklistEPEM_ids[j] << "\t"
                   << fRecoTracklistEPEM_ids[k] << "\t" << fRecoTracklistEPEM_ids[l] << endl;
              cout << "motherid: " << motherId1 << "\t" << motherId2 << "\t" << motherId3 << "\t" << motherId4 << endl;
              cout << "motherpdg: " << mcMotherPdg1 << "\t" << mcMotherPdg2 << "\t" << mcMotherPdg3 << "\t"
                   << mcMotherPdg4 << endl;
              cout << "grandmotherid: " << grandmotherId1 << "\t" << grandmotherId2 << "\t" << grandmotherId3 << "\t"
                   << grandmotherId4 << endl;
              cout << "pdg: " << fRecoTracklistEPEM[i]->GetPdgCode() << "\t" << fRecoTracklistEPEM[j]->GetPdgCode()
                   << "\t" << fRecoTracklistEPEM[k]->GetPdgCode() << "\t" << fRecoTracklistEPEM[l]->GetPdgCode()
                   << endl;
              cout << "invmass reco: " << invmass2 << "\t invmass mc: " << invmass1 << endl;

              fhInvMass_EPEM_mc->Fill(invmass1);
              fhInvMass_EPEM_stsMomVec->Fill(invmass2);
              fhInvMass_EPEM_refitted->Fill(invmass3);
              fhInvMass_EPEM_error_stsMomVec->Fill(1.0 * TMath::Abs(invmass1 - invmass2) / invmass1);
              fhInvMass_EPEM_error_refitted->Fill(1.0 * TMath::Abs(invmass1 - invmass3) / invmass1);

              fhUsedMomenta_stsMomVec->Fill(fRecoMomentum[i].Mag());
              fhUsedMomenta_stsMomVec->Fill(fRecoMomentum[j].Mag());
              fhUsedMomenta_stsMomVec->Fill(fRecoMomentum[k].Mag());
              fhUsedMomenta_stsMomVec->Fill(fRecoMomentum[l].Mag());

              fhUsedMomenta_mc->Fill(fRecoTracklistEPEM[i]->GetP());
              fhUsedMomenta_mc->Fill(fRecoTracklistEPEM[j]->GetP());
              fhUsedMomenta_mc->Fill(fRecoTracklistEPEM[k]->GetP());
              fhUsedMomenta_mc->Fill(fRecoTracklistEPEM[l]->GetP());

              fhUsedMomenta_error_stsMomVec->Fill(TMath::Abs(fRecoTracklistEPEM[i]->GetP() - fRecoMomentum[i].Mag())
                                                  / fRecoTracklistEPEM[i]->GetP());
              fhUsedMomenta_error_stsMomVec->Fill(TMath::Abs(fRecoTracklistEPEM[j]->GetP() - fRecoMomentum[j].Mag())
                                                  / fRecoTracklistEPEM[j]->GetP());
              fhUsedMomenta_error_stsMomVec->Fill(TMath::Abs(fRecoTracklistEPEM[k]->GetP() - fRecoMomentum[k].Mag())
                                                  / fRecoTracklistEPEM[k]->GetP());
              fhUsedMomenta_error_stsMomVec->Fill(TMath::Abs(fRecoTracklistEPEM[l]->GetP() - fRecoMomentum[l].Mag())
                                                  / fRecoTracklistEPEM[l]->GetP());

              //	fhUsedMomenta_error_refitted->Fill(TMath::Abs(fRecoRefittedMomentum[i].Mag() - fRecoMomentum[i].Mag())/fRecoRefittedMomentum[i].Mag());
              //	fhUsedMomenta_error_refitted->Fill(TMath::Abs(fRecoRefittedMomentum[j].Mag() - fRecoMomentum[j].Mag())/fRecoRefittedMomentum[j].Mag());
              //	fhUsedMomenta_error_refitted->Fill(TMath::Abs(fRecoRefittedMomentum[k].Mag() - fRecoMomentum[k].Mag())/fRecoRefittedMomentum[k].Mag());
              //	fhUsedMomenta_error_refitted->Fill(TMath::Abs(fRecoRefittedMomentum[l].Mag() - fRecoMomentum[l].Mag())/fRecoRefittedMomentum[l].Mag());

              fhUsedMomenta_error_refitted->Fill(
                TMath::Abs(fRecoTracklistEPEM[i]->GetP() - fRecoRefittedMomentum[i].Mag())
                / fRecoTracklistEPEM[i]->GetP());
              fhUsedMomenta_error_refitted->Fill(
                TMath::Abs(fRecoTracklistEPEM[j]->GetP() - fRecoRefittedMomentum[j].Mag())
                / fRecoTracklistEPEM[j]->GetP());
              fhUsedMomenta_error_refitted->Fill(
                TMath::Abs(fRecoTracklistEPEM[k]->GetP() - fRecoRefittedMomentum[k].Mag())
                / fRecoTracklistEPEM[k]->GetP());
              fhUsedMomenta_error_refitted->Fill(
                TMath::Abs(fRecoTracklistEPEM[l]->GetP() - fRecoRefittedMomentum[l].Mag())
                / fRecoTracklistEPEM[l]->GetP());


              TVector3 momentumtest5a;
              fRecoTracklistEPEM[i]->GetMomentum(momentumtest5a);
              fhUsedMomenta_errorX_stsMomVec->Fill(TMath::Abs(momentumtest5a.X() - fRecoMomentum[i].X())
                                                   / momentumtest5a.X());
              fhUsedMomenta_errorY_stsMomVec->Fill(TMath::Abs(momentumtest5a.Y() - fRecoMomentum[i].Y())
                                                   / momentumtest5a.Y());
              fhUsedMomenta_errorZ_stsMomVec->Fill(TMath::Abs(momentumtest5a.Z() - fRecoMomentum[i].Z())
                                                   / momentumtest5a.Z());
              fhUsedMomenta_vsX_stsMomVec->Fill(momentumtest5a.X(), fRecoMomentum[i].X());
              fhUsedMomenta_vsY_stsMomVec->Fill(momentumtest5a.Y(), fRecoMomentum[i].Y());
              fhUsedMomenta_vsZ_stsMomVec->Fill(momentumtest5a.Z(), fRecoMomentum[i].Z());
              fhUsedMomenta_errorX_refitted->Fill(TMath::Abs(momentumtest5a.X() - fRecoRefittedMomentum[i].X())
                                                  / momentumtest5a.X());
              fhUsedMomenta_errorY_refitted->Fill(TMath::Abs(momentumtest5a.Y() - fRecoRefittedMomentum[i].Y())
                                                  / momentumtest5a.Y());
              fhUsedMomenta_errorZ_refitted->Fill(TMath::Abs(momentumtest5a.Z() - fRecoRefittedMomentum[i].Z())
                                                  / momentumtest5a.Z());
              fhUsedMomenta_vsX_refitted->Fill(momentumtest5a.X(), fRecoRefittedMomentum[i].X());
              fhUsedMomenta_vsY_refitted->Fill(momentumtest5a.Y(), fRecoRefittedMomentum[i].Y());
              fhUsedMomenta_vsZ_refitted->Fill(momentumtest5a.Z(), fRecoRefittedMomentum[i].Z());

              TVector3 momentumtest5b;
              fRecoTracklistEPEM[j]->GetMomentum(momentumtest5b);
              fhUsedMomenta_errorX_stsMomVec->Fill(TMath::Abs(momentumtest5b.X() - fRecoMomentum[j].X())
                                                   / momentumtest5b.X());
              fhUsedMomenta_errorY_stsMomVec->Fill(TMath::Abs(momentumtest5b.Y() - fRecoMomentum[j].Y())
                                                   / momentumtest5b.Y());
              fhUsedMomenta_errorZ_stsMomVec->Fill(TMath::Abs(momentumtest5b.Z() - fRecoMomentum[j].Z())
                                                   / momentumtest5b.Z());
              fhUsedMomenta_vsX_stsMomVec->Fill(momentumtest5b.X(), fRecoMomentum[j].X());
              fhUsedMomenta_vsY_stsMomVec->Fill(momentumtest5b.Y(), fRecoMomentum[j].Y());
              fhUsedMomenta_vsZ_stsMomVec->Fill(momentumtest5b.Z(), fRecoMomentum[j].Z());
              fhUsedMomenta_errorX_refitted->Fill(TMath::Abs(momentumtest5b.X() - fRecoRefittedMomentum[j].X())
                                                  / momentumtest5b.X());
              fhUsedMomenta_errorY_refitted->Fill(TMath::Abs(momentumtest5b.Y() - fRecoRefittedMomentum[j].Y())
                                                  / momentumtest5b.Y());
              fhUsedMomenta_errorZ_refitted->Fill(TMath::Abs(momentumtest5b.Z() - fRecoRefittedMomentum[j].Z())
                                                  / momentumtest5b.Z());
              fhUsedMomenta_vsX_refitted->Fill(momentumtest5b.X(), fRecoRefittedMomentum[j].X());
              fhUsedMomenta_vsY_refitted->Fill(momentumtest5b.Y(), fRecoRefittedMomentum[j].Y());
              fhUsedMomenta_vsZ_refitted->Fill(momentumtest5b.Z(), fRecoRefittedMomentum[j].Z());

              TVector3 momentumtest5c;
              fRecoTracklistEPEM[k]->GetMomentum(momentumtest5c);
              fhUsedMomenta_errorX_stsMomVec->Fill(TMath::Abs(momentumtest5c.X() - fRecoMomentum[k].X())
                                                   / momentumtest5c.X());
              fhUsedMomenta_errorY_stsMomVec->Fill(TMath::Abs(momentumtest5c.Y() - fRecoMomentum[k].Y())
                                                   / momentumtest5c.Y());
              fhUsedMomenta_errorZ_stsMomVec->Fill(TMath::Abs(momentumtest5c.Z() - fRecoMomentum[k].Z())
                                                   / momentumtest5c.Z());
              fhUsedMomenta_vsX_stsMomVec->Fill(momentumtest5c.X(), fRecoMomentum[k].X());
              fhUsedMomenta_vsY_stsMomVec->Fill(momentumtest5c.Y(), fRecoMomentum[k].Y());
              fhUsedMomenta_vsZ_stsMomVec->Fill(momentumtest5c.Z(), fRecoMomentum[k].Z());
              fhUsedMomenta_errorX_refitted->Fill(TMath::Abs(momentumtest5c.X() - fRecoRefittedMomentum[k].X())
                                                  / momentumtest5c.X());
              fhUsedMomenta_errorY_refitted->Fill(TMath::Abs(momentumtest5c.Y() - fRecoRefittedMomentum[k].Y())
                                                  / momentumtest5c.Y());
              fhUsedMomenta_errorZ_refitted->Fill(TMath::Abs(momentumtest5c.Z() - fRecoRefittedMomentum[k].Z())
                                                  / momentumtest5c.Z());
              fhUsedMomenta_vsX_refitted->Fill(momentumtest5c.X(), fRecoRefittedMomentum[k].X());
              fhUsedMomenta_vsY_refitted->Fill(momentumtest5c.Y(), fRecoRefittedMomentum[k].Y());
              fhUsedMomenta_vsZ_refitted->Fill(momentumtest5c.Z(), fRecoRefittedMomentum[k].Z());

              TVector3 momentumtest5d;
              fRecoTracklistEPEM[l]->GetMomentum(momentumtest5d);
              fhUsedMomenta_errorX_stsMomVec->Fill(TMath::Abs(momentumtest5d.X() - fRecoMomentum[l].X())
                                                   / momentumtest5d.X());
              fhUsedMomenta_errorY_stsMomVec->Fill(TMath::Abs(momentumtest5d.Y() - fRecoMomentum[l].Y())
                                                   / momentumtest5d.Y());
              fhUsedMomenta_errorZ_stsMomVec->Fill(TMath::Abs(momentumtest5d.Z() - fRecoMomentum[l].Z())
                                                   / momentumtest5d.Z());
              fhUsedMomenta_vsX_stsMomVec->Fill(momentumtest5d.X(), fRecoMomentum[l].X());
              fhUsedMomenta_vsY_stsMomVec->Fill(momentumtest5d.Y(), fRecoMomentum[l].Y());
              fhUsedMomenta_vsZ_stsMomVec->Fill(momentumtest5d.Z(), fRecoMomentum[l].Z());
              fhUsedMomenta_errorX_refitted->Fill(TMath::Abs(momentumtest5d.X() - fRecoRefittedMomentum[l].X())
                                                  / momentumtest5d.X());
              fhUsedMomenta_errorY_refitted->Fill(TMath::Abs(momentumtest5d.Y() - fRecoRefittedMomentum[l].Y())
                                                  / momentumtest5d.Y());
              fhUsedMomenta_errorZ_refitted->Fill(TMath::Abs(momentumtest5d.Z() - fRecoRefittedMomentum[l].Z())
                                                  / momentumtest5d.Z());
              fhUsedMomenta_vsX_refitted->Fill(momentumtest5d.X(), fRecoRefittedMomentum[l].X());
              fhUsedMomenta_vsY_refitted->Fill(momentumtest5d.Y(), fRecoRefittedMomentum[l].Y());
              fhUsedMomenta_vsZ_refitted->Fill(momentumtest5d.Z(), fRecoRefittedMomentum[l].Z());


              Double_t opening_angle1_mc       = 0;
              Double_t opening_angle2_mc       = 0;
              Double_t opening_angle1_refitted = 0;
              Double_t opening_angle2_refitted = 0;

              if (motherId1 == motherId2) {
                opening_angle1_mc       = CalculateOpeningAngleMC(fRecoTracklistEPEM[i], fRecoTracklistEPEM[j]);
                opening_angle2_mc       = CalculateOpeningAngleMC(fRecoTracklistEPEM[k], fRecoTracklistEPEM[l]);
                opening_angle1_refitted = CalculateOpeningAngleReco(fRecoRefittedMomentum[i], fRecoRefittedMomentum[j]);
                opening_angle2_refitted = CalculateOpeningAngleReco(fRecoRefittedMomentum[k], fRecoRefittedMomentum[l]);
              }
              if (motherId1 == motherId3) {
                opening_angle1_mc       = CalculateOpeningAngleMC(fRecoTracklistEPEM[i], fRecoTracklistEPEM[k]);
                opening_angle2_mc       = CalculateOpeningAngleMC(fRecoTracklistEPEM[j], fRecoTracklistEPEM[l]);
                opening_angle1_refitted = CalculateOpeningAngleReco(fRecoRefittedMomentum[i], fRecoRefittedMomentum[k]);
                opening_angle2_refitted = CalculateOpeningAngleReco(fRecoRefittedMomentum[j], fRecoRefittedMomentum[l]);
              }
              if (motherId1 == motherId4) {
                opening_angle1_mc       = CalculateOpeningAngleMC(fRecoTracklistEPEM[i], fRecoTracklistEPEM[l]);
                opening_angle2_mc       = CalculateOpeningAngleMC(fRecoTracklistEPEM[j], fRecoTracklistEPEM[k]);
                opening_angle1_refitted = CalculateOpeningAngleReco(fRecoRefittedMomentum[i], fRecoRefittedMomentum[l]);
                opening_angle2_refitted = CalculateOpeningAngleReco(fRecoRefittedMomentum[j], fRecoRefittedMomentum[k]);
              }


              fhInvMass_EPEM_openingAngleRef->Fill(opening_angle1_refitted);
              fhInvMass_EPEM_openingAngleRef->Fill(opening_angle2_refitted);


              fhEPEM_invmass_gg_mc->Fill(invmass1);
              fhEPEM_invmass_gg_refitted->Fill(invmass3);
              fhEPEM_invmass_all_mc->Fill(invmass1);
              fhEPEM_invmass_all_refitted->Fill(invmass3);
              fhEPEM_openingAngle_gg_mc->Fill(opening_angle1_mc);
              fhEPEM_openingAngle_gg_mc->Fill(opening_angle2_mc);
              fhEPEM_openingAngle_gg_refitted->Fill(opening_angle1_refitted);
              fhEPEM_openingAngle_gg_refitted->Fill(opening_angle2_refitted);

              Double_t openingAngleBetweenGammas = CalculateOpeningAngleBetweenGammasMC(
                fRecoTracklistEPEM[i], fRecoTracklistEPEM[j], fRecoTracklistEPEM[k], fRecoTracklistEPEM[l]);
              fhEPEM_openingAngle_betweenGammas_mc->Fill(openingAngleBetweenGammas);
              Double_t openingAngleBetweenGammasReco = CalculateOpeningAngleBetweenGammasReco(
                fRecoRefittedMomentum[i], fRecoRefittedMomentum[j], fRecoRefittedMomentum[k], fRecoRefittedMomentum[l]);
              fhEPEM_openingAngle_betweenGammas_reco->Fill(openingAngleBetweenGammasReco);


              LmvmKinePar params1 = CalculateKinematicParams_4particles(
                fRecoRefittedMomentum[i], fRecoRefittedMomentum[j], fRecoRefittedMomentum[k], fRecoRefittedMomentum[l]);
              fhPi0_pt_vs_rap_gg->Fill(params1.fPt, params1.fRapidity);
              fhPi0_pt_vs_rap_all->Fill(params1.fPt, params1.fRapidity);
              fhPi0_pt_gg->Fill(params1.fPt);
              fhPi0_pt_all->Fill(params1.fPt);


              fhEPEM_openingAngle_vs_pt_gg_reco->Fill(params1.fPt, opening_angle1_refitted);
              fhEPEM_openingAngle_vs_pt_gg_reco->Fill(params1.fPt, opening_angle2_refitted);

              fhEPEM_rap_vs_chi->Fill(params1.fRapidity, fRecoTracklistEPEM_chi[i]);
              fhEPEM_rap_vs_chi->Fill(params1.fRapidity, fRecoTracklistEPEM_chi[j]);
              fhEPEM_rap_vs_chi->Fill(params1.fRapidity, fRecoTracklistEPEM_chi[k]);
              fhEPEM_rap_vs_chi->Fill(params1.fRapidity, fRecoTracklistEPEM_chi[l]);

              fhEPEM_rap_vs_invmass->Fill(params1.fRapidity, invmass3);


              // ####################################
              // STUDIES: efficiency of cuts
              // ####################################
              //fRichElIdAnn = new CbmRichElectronIdAnn();
              //fRichElIdAnn->Init();
              //Double_t ann = fRichElIdAnn->DoSelect(ring, momentum);
              //if (ann > fCuts.fRichAnnCut) return true;		// cut = 0.0


              CutEfficiencyStudies(i, j, k, l, motherId1, motherId2, motherId3, motherId4);

              //Bool_t IsRichElectron1 = CbmLitGlobalElectronId::GetInstance().IsRichElectron(fRecoTracklistEPEM_gtid[i], fRecoRefittedMomentum[i].Mag());
              //Bool_t IsRichElectron2 = CbmLitGlobalElectronId::GetInstance().IsRichElectron(fRecoTracklistEPEM_gtid[j], fRecoRefittedMomentum[j].Mag());
              //Bool_t IsRichElectron3 = CbmLitGlobalElectronId::GetInstance().IsRichElectron(fRecoTracklistEPEM_gtid[k], fRecoRefittedMomentum[k].Mag());
              //Bool_t IsRichElectron4 = CbmLitGlobalElectronId::GetInstance().IsRichElectron(fRecoTracklistEPEM_gtid[l], fRecoRefittedMomentum[l].Mag());

              Bool_t IsRichElectron1ann = IsRichElectronANN(fRecoTracklistEPEM_gtid[i], fRecoRefittedMomentum[i].Mag());
              Bool_t IsRichElectron2ann = IsRichElectronANN(fRecoTracklistEPEM_gtid[j], fRecoRefittedMomentum[j].Mag());
              Bool_t IsRichElectron3ann = IsRichElectronANN(fRecoTracklistEPEM_gtid[k], fRecoRefittedMomentum[k].Mag());
              Bool_t IsRichElectron4ann = IsRichElectronANN(fRecoTracklistEPEM_gtid[l], fRecoRefittedMomentum[l].Mag());

              Bool_t IsRichElectron1normal =
                IsRichElectronNormal(fRecoTracklistEPEM_gtid[i], fRecoRefittedMomentum[i].Mag());
              Bool_t IsRichElectron2normal =
                IsRichElectronNormal(fRecoTracklistEPEM_gtid[j], fRecoRefittedMomentum[j].Mag());
              Bool_t IsRichElectron3normal =
                IsRichElectronNormal(fRecoTracklistEPEM_gtid[k], fRecoRefittedMomentum[k].Mag());
              Bool_t IsRichElectron4normal =
                IsRichElectronNormal(fRecoTracklistEPEM_gtid[l], fRecoRefittedMomentum[l].Mag());

              Bool_t IsRichElectron1MC = (TMath::Abs(fRecoTracklistEPEM[i]->GetPdgCode()) == 11);
              Bool_t IsRichElectron2MC = (TMath::Abs(fRecoTracklistEPEM[j]->GetPdgCode()) == 11);
              Bool_t IsRichElectron3MC = (TMath::Abs(fRecoTracklistEPEM[k]->GetPdgCode()) == 11);
              Bool_t IsRichElectron4MC = (TMath::Abs(fRecoTracklistEPEM[l]->GetPdgCode()) == 11);

              LmvmKinePar paramsCut1;
              LmvmKinePar paramsCut2;

              if (motherId1 == motherId2) {
                paramsCut1 = CalculateKinematicParamsReco(fRecoRefittedMomentum[i], fRecoRefittedMomentum[j]);
                paramsCut2 = CalculateKinematicParamsReco(fRecoRefittedMomentum[k], fRecoRefittedMomentum[l]);
              }
              if (motherId1 == motherId3) {
                paramsCut1 = CalculateKinematicParamsReco(fRecoRefittedMomentum[i], fRecoRefittedMomentum[k]);
                paramsCut2 = CalculateKinematicParamsReco(fRecoRefittedMomentum[j], fRecoRefittedMomentum[l]);
              }
              if (motherId1 == motherId4) {
                paramsCut1 = CalculateKinematicParamsReco(fRecoRefittedMomentum[i], fRecoRefittedMomentum[l]);
                paramsCut2 = CalculateKinematicParamsReco(fRecoRefittedMomentum[j], fRecoRefittedMomentum[k]);
              }


              Double_t Value_invariantMassCut = 0.03;
              Double_t Value_openingAngleCut1 = 1.8 - 0.6 * paramsCut1.fPt;
              Double_t Value_openingAngleCut2 = 1.8 - 0.6 * paramsCut2.fPt;

              Bool_t OpeningAngleCut1  = (opening_angle1_refitted < Value_openingAngleCut1);
              Bool_t OpeningAngleCut2  = (opening_angle2_refitted < Value_openingAngleCut2);
              Bool_t InvariantMassCut1 = (paramsCut1.fMinv < Value_invariantMassCut);
              Bool_t InvariantMassCut2 = (paramsCut2.fMinv < Value_invariantMassCut);


              fhEPEM_efficiencyCuts2->Fill(0);  // no further cuts applied
              // first ANN usage for electron identification
              if (IsRichElectron1ann && IsRichElectron2ann && IsRichElectron3ann
                  && IsRichElectron4ann) {  // all 4 electrons correctly identified with the RICH via ANN
                fhEPEM_efficiencyCuts2->Fill(1);
                if (OpeningAngleCut1 && OpeningAngleCut2) {  // opening angle of e+e- pairs below x
                  fhEPEM_efficiencyCuts2->Fill(2);
                  if (InvariantMassCut1 && InvariantMassCut2) { fhEPEM_efficiencyCuts2->Fill(3); }
                }
              }
              // then standard method for electron identification
              if (IsRichElectron1normal && IsRichElectron2normal && IsRichElectron3normal
                  && IsRichElectron4normal) {  // all 4 electrons correctly identified with the RICH via "normal way"
                fhEPEM_efficiencyCuts2->Fill(4);
                if (OpeningAngleCut1 && OpeningAngleCut2) {  // opening angle of e+e- pairs below x
                  fhEPEM_efficiencyCuts2->Fill(5);
                  if (InvariantMassCut1 && InvariantMassCut2) { fhEPEM_efficiencyCuts2->Fill(6); }
                }
              }
              // MC-true data for electron identification
              if (IsRichElectron1MC && IsRichElectron2MC && IsRichElectron3MC
                  && IsRichElectron4MC) {  // all 4 electrons correctly identified with the RICH via MC-true data
                fhEPEM_efficiencyCuts2->Fill(7);
                if (OpeningAngleCut1 && OpeningAngleCut2) {  // opening angle of e+e- pairs below x
                  fhEPEM_efficiencyCuts2->Fill(8);
                  if (InvariantMassCut1 && InvariantMassCut2) { fhEPEM_efficiencyCuts2->Fill(9); }
                }
              }


              cout << "reco/mc: " << fRecoMomentum[i].Mag() << " / " << fRecoTracklistEPEM[i]->GetP() << " ### "
                   << fRecoMomentum[j].Mag() << " / " << fRecoTracklistEPEM[j]->GetP() << " ### "
                   << fRecoMomentum[k].Mag() << " / " << fRecoTracklistEPEM[k]->GetP() << " ### "
                   << fRecoMomentum[l].Mag() << " / " << fRecoTracklistEPEM[l]->GetP() << endl;

              fill++;
            }
          }
        }
      }
    }
  }
  cout << "CbmAnaConversionReco: InvariantMassTest_4epem - Filled events: " << fill << endl;
  cout << "CbmAnaConversionReco: InvariantMassTest_4epem - End!" << endl;


  timer.Stop();
  fTime += timer.RealTime();
}


void CbmAnaConversionReco::CutEfficiencyStudies(int e1, int e2, int e3, int e4, int motherE1, int motherE2,
                                                int motherE3, int motherE4, int IsEta)
{
  // ####################################
  // STUDIES: efficiency of cuts
  // ####################################
  //fRichElIdAnn = new CbmRichElectronIdAnn();
  //fRichElIdAnn->Init();
  //Double_t ann = fRichElIdAnn->DoSelect(ring, momentum);
  //if (ann > fCuts.fRichAnnCut) return true;		// cut = 0.0

  Double_t opening_angle1_refitted = 0;
  Double_t opening_angle2_refitted = 0;

  if (motherE1 == motherE2) {
    opening_angle1_refitted = CalculateOpeningAngleReco(fRecoRefittedMomentum[e1], fRecoRefittedMomentum[e2]);
    opening_angle2_refitted = CalculateOpeningAngleReco(fRecoRefittedMomentum[e3], fRecoRefittedMomentum[e4]);
  }
  if (motherE1 == motherE3) {
    opening_angle1_refitted = CalculateOpeningAngleReco(fRecoRefittedMomentum[e1], fRecoRefittedMomentum[e3]);
    opening_angle2_refitted = CalculateOpeningAngleReco(fRecoRefittedMomentum[e2], fRecoRefittedMomentum[e4]);
  }
  if (motherE1 == motherE4) {
    opening_angle1_refitted = CalculateOpeningAngleReco(fRecoRefittedMomentum[e1], fRecoRefittedMomentum[e4]);
    opening_angle2_refitted = CalculateOpeningAngleReco(fRecoRefittedMomentum[e2], fRecoRefittedMomentum[e3]);
  }


  Bool_t IsWithinChiCut1 =
    (fRecoTracklistEPEM_chi[e1] < CbmAnaConversionCutSettings::CalcChiCut(fRecoRefittedMomentum[e1].Perp()));
  Bool_t IsWithinChiCut2 =
    (fRecoTracklistEPEM_chi[e2] < CbmAnaConversionCutSettings::CalcChiCut(fRecoRefittedMomentum[e2].Perp()));
  Bool_t IsWithinChiCut3 =
    (fRecoTracklistEPEM_chi[e3] < CbmAnaConversionCutSettings::CalcChiCut(fRecoRefittedMomentum[e3].Perp()));
  Bool_t IsWithinChiCut4 =
    (fRecoTracklistEPEM_chi[e4] < CbmAnaConversionCutSettings::CalcChiCut(fRecoRefittedMomentum[e4].Perp()));

  Bool_t AllWithinChiCut = (IsWithinChiCut1 && IsWithinChiCut2 && IsWithinChiCut3 && IsWithinChiCut4);


  //Bool_t IsRichElectron1 = CbmLitGlobalElectronId::GetInstance().IsRichElectron(fRecoTracklistEPEM_gtid[i], fRecoRefittedMomentum[i].Mag());
  //Bool_t IsRichElectron2 = CbmLitGlobalElectronId::GetInstance().IsRichElectron(fRecoTracklistEPEM_gtid[j], fRecoRefittedMomentum[j].Mag());
  //Bool_t IsRichElectron3 = CbmLitGlobalElectronId::GetInstance().IsRichElectron(fRecoTracklistEPEM_gtid[k], fRecoRefittedMomentum[k].Mag());
  //Bool_t IsRichElectron4 = CbmLitGlobalElectronId::GetInstance().IsRichElectron(fRecoTracklistEPEM_gtid[l], fRecoRefittedMomentum[l].Mag());

  Bool_t IsRichElectron1ann = IsRichElectronANN(fRecoTracklistEPEM_gtid[e1], fRecoRefittedMomentum[e1].Mag());
  Bool_t IsRichElectron2ann = IsRichElectronANN(fRecoTracklistEPEM_gtid[e2], fRecoRefittedMomentum[e2].Mag());
  Bool_t IsRichElectron3ann = IsRichElectronANN(fRecoTracklistEPEM_gtid[e3], fRecoRefittedMomentum[e3].Mag());
  Bool_t IsRichElectron4ann = IsRichElectronANN(fRecoTracklistEPEM_gtid[e4], fRecoRefittedMomentum[e4].Mag());

  Bool_t IsRichElectron1normal = IsRichElectronNormal(fRecoTracklistEPEM_gtid[e1], fRecoRefittedMomentum[e1].Mag());
  Bool_t IsRichElectron2normal = IsRichElectronNormal(fRecoTracklistEPEM_gtid[e2], fRecoRefittedMomentum[e2].Mag());
  Bool_t IsRichElectron3normal = IsRichElectronNormal(fRecoTracklistEPEM_gtid[e3], fRecoRefittedMomentum[e3].Mag());
  Bool_t IsRichElectron4normal = IsRichElectronNormal(fRecoTracklistEPEM_gtid[e4], fRecoRefittedMomentum[e4].Mag());

  Bool_t IsRichElectron1MC = (TMath::Abs(fRecoTracklistEPEM[e1]->GetPdgCode()) == 11);
  Bool_t IsRichElectron2MC = (TMath::Abs(fRecoTracklistEPEM[e2]->GetPdgCode()) == 11);
  Bool_t IsRichElectron3MC = (TMath::Abs(fRecoTracklistEPEM[e3]->GetPdgCode()) == 11);
  Bool_t IsRichElectron4MC = (TMath::Abs(fRecoTracklistEPEM[e4]->GetPdgCode()) == 11);

  Double_t ANNvalueE1 = ElectronANNvalue(fRecoTracklistEPEM_gtid[e1], fRecoRefittedMomentum[e1].Mag());
  Double_t ANNvalueE2 = ElectronANNvalue(fRecoTracklistEPEM_gtid[e2], fRecoRefittedMomentum[e2].Mag());
  Double_t ANNvalueE3 = ElectronANNvalue(fRecoTracklistEPEM_gtid[e3], fRecoRefittedMomentum[e3].Mag());
  Double_t ANNvalueE4 = ElectronANNvalue(fRecoTracklistEPEM_gtid[e4], fRecoRefittedMomentum[e4].Mag());

  LmvmKinePar paramsCut1;
  LmvmKinePar paramsCut2;

  if (motherE1 == motherE2) {
    paramsCut1 = CalculateKinematicParamsReco(fRecoRefittedMomentum[e1], fRecoRefittedMomentum[e2]);
    paramsCut2 = CalculateKinematicParamsReco(fRecoRefittedMomentum[e3], fRecoRefittedMomentum[e4]);
  }
  if (motherE1 == motherE3) {
    paramsCut1 = CalculateKinematicParamsReco(fRecoRefittedMomentum[e1], fRecoRefittedMomentum[e3]);
    paramsCut2 = CalculateKinematicParamsReco(fRecoRefittedMomentum[e2], fRecoRefittedMomentum[e4]);
  }
  if (motherE1 == motherE4) {
    paramsCut1 = CalculateKinematicParamsReco(fRecoRefittedMomentum[e1], fRecoRefittedMomentum[e4]);
    paramsCut2 = CalculateKinematicParamsReco(fRecoRefittedMomentum[e2], fRecoRefittedMomentum[e3]);
  }


  Double_t Value_invariantMassCut = 0.03;
  Double_t Value_openingAngleCut1 = 1.8 - 0.6 * paramsCut1.fPt;
  Double_t Value_openingAngleCut2 = 1.8 - 0.6 * paramsCut2.fPt;

  Bool_t OpeningAngleCut1  = (opening_angle1_refitted < Value_openingAngleCut1);
  Bool_t OpeningAngleCut2  = (opening_angle2_refitted < Value_openingAngleCut2);
  Bool_t InvariantMassCut1 = (paramsCut1.fMinv < Value_invariantMassCut);
  Bool_t InvariantMassCut2 = (paramsCut2.fMinv < Value_invariantMassCut);


  if (IsEta == 0) {
    fhEPEM_pi0_nofLeptons_ann->Fill(IsRichElectron1ann + IsRichElectron2ann + IsRichElectron3ann + IsRichElectron4ann);

    if (IsRichElectron1ann + IsRichElectron2ann + IsRichElectron3ann + IsRichElectron4ann == 3) {
      CbmGlobalTrack* gTrack = NULL;
      if (IsRichElectron1ann == 0) gTrack = (CbmGlobalTrack*) fGlobalTracks->At(fRecoTracklistEPEM_gtid[e1]);
      if (IsRichElectron2ann == 0) gTrack = (CbmGlobalTrack*) fGlobalTracks->At(fRecoTracklistEPEM_gtid[e2]);
      if (IsRichElectron3ann == 0) gTrack = (CbmGlobalTrack*) fGlobalTracks->At(fRecoTracklistEPEM_gtid[e3]);
      if (IsRichElectron4ann == 0) gTrack = (CbmGlobalTrack*) fGlobalTracks->At(fRecoTracklistEPEM_gtid[e4]);
      int richInd           = gTrack->GetRichRingIndex();
      CbmRichRing* richRing = (CbmRichRing*) fRichRings->At(richInd);
      Int_t nofringhits     = richRing->GetNofHits();
      fhEPEM_missingLepton_nofRingHits->Fill(nofringhits);
      fhEPEM_missingLepton_ringMid->Fill(richRing->GetCenterX(), richRing->GetCenterY());
      fhEPEM_missingLepton_ringRadius->Fill(richRing->GetRadius());
      // CbmRichRing::GetDistance() method is no longer supported
      // If you wan to use cuts update code using CbmRichUtil::GetRingTrackDistance()
      fhEPEM_missingLepton_distance->Fill(1. /*richRing->GetDistance()*/);
      fhEPEM_missingLepton_selectionNN->Fill(richRing->GetSelectionNN());
      if (IsRichElectron1ann == 0) {
        fhEPEM_missingLepton_radius_vs_p->Fill(fRecoRefittedMomentum[e1].Mag(), richRing->GetRadius());
        fhEPEM_missingLepton_ANNvalue->Fill(ANNvalueE1);
      }
      if (IsRichElectron2ann == 0) {
        fhEPEM_missingLepton_radius_vs_p->Fill(fRecoRefittedMomentum[e2].Mag(), richRing->GetRadius());
        fhEPEM_missingLepton_ANNvalue->Fill(ANNvalueE2);
      }
      if (IsRichElectron3ann == 0) {
        fhEPEM_missingLepton_radius_vs_p->Fill(fRecoRefittedMomentum[e3].Mag(), richRing->GetRadius());
        fhEPEM_missingLepton_ANNvalue->Fill(ANNvalueE3);
      }
      if (IsRichElectron4ann == 0) {
        fhEPEM_missingLepton_radius_vs_p->Fill(fRecoRefittedMomentum[e4].Mag(), richRing->GetRadius());
        fhEPEM_missingLepton_ANNvalue->Fill(ANNvalueE4);
      }

      for (int i = 0; i < nofringhits; i++) {
        Int_t hitInd    = richRing->GetHit(i);
        CbmRichHit* hit = (CbmRichHit*) fRichHits->At(hitInd);
        if (NULL == hit) continue;
        //CbmRichHitLight hl(hit->GetX(), hit->GetY());
        fhEPEM_missingLepton_rings->Fill(hit->GetX(), hit->GetY());
      }
    }


    if (IsRichElectron1ann > 0) {
      CbmGlobalTrack* gTrack = (CbmGlobalTrack*) fGlobalTracks->At(fRecoTracklistEPEM_gtid[e1]);
      int richInd            = gTrack->GetRichRingIndex();
      CbmRichRing* richRing  = (CbmRichRing*) fRichRings->At(richInd);
      Int_t nofringhits      = richRing->GetNofHits();
      fhEPEM_identifiedLepton_nofRingHits->Fill(nofringhits);
      fhEPEM_identifiedLepton_ringMid->Fill(richRing->GetCenterX(), richRing->GetCenterY());
      fhEPEM_identifiedLepton_ringRadius->Fill(richRing->GetRadius());
      // CbmRichRing::GetDistance() method is no longer supported
      // If you wan to use cuts update code using CbmRichUtil::GetRingTrackDistance()
      fhEPEM_identifiedLepton_distance->Fill(1. /*richRing->GetDistance()*/);
      fhEPEM_identifiedLepton_selectionNN->Fill(richRing->GetSelectionNN());
      fhEPEM_identifiedLepton_radius_vs_p->Fill(fRecoRefittedMomentum[e1].Mag(), richRing->GetRadius());
      fhEPEM_identifiedLepton_ANNvalue->Fill(ANNvalueE1);
    }
    if (IsRichElectron2ann > 0) {
      CbmGlobalTrack* gTrack = (CbmGlobalTrack*) fGlobalTracks->At(fRecoTracklistEPEM_gtid[e2]);
      int richInd            = gTrack->GetRichRingIndex();
      CbmRichRing* richRing  = (CbmRichRing*) fRichRings->At(richInd);
      Int_t nofringhits      = richRing->GetNofHits();
      fhEPEM_identifiedLepton_nofRingHits->Fill(nofringhits);
      fhEPEM_identifiedLepton_ringMid->Fill(richRing->GetCenterX(), richRing->GetCenterY());
      fhEPEM_identifiedLepton_ringRadius->Fill(richRing->GetRadius());
      // CbmRichRing::GetDistance() method is no longer supported
      // If you wan to use cuts update code using CbmRichUtil::GetRingTrackDistance()
      fhEPEM_identifiedLepton_distance->Fill(1. /*richRing->GetDistance()*/);
      fhEPEM_identifiedLepton_selectionNN->Fill(richRing->GetSelectionNN());
      fhEPEM_identifiedLepton_radius_vs_p->Fill(fRecoRefittedMomentum[e2].Mag(), richRing->GetRadius());
      fhEPEM_identifiedLepton_ANNvalue->Fill(ANNvalueE2);
    }
    if (IsRichElectron3ann > 0) {
      CbmGlobalTrack* gTrack = (CbmGlobalTrack*) fGlobalTracks->At(fRecoTracklistEPEM_gtid[e3]);
      int richInd            = gTrack->GetRichRingIndex();
      CbmRichRing* richRing  = (CbmRichRing*) fRichRings->At(richInd);
      Int_t nofringhits      = richRing->GetNofHits();
      fhEPEM_identifiedLepton_nofRingHits->Fill(nofringhits);
      fhEPEM_identifiedLepton_ringMid->Fill(richRing->GetCenterX(), richRing->GetCenterY());
      fhEPEM_identifiedLepton_ringRadius->Fill(richRing->GetRadius());
      // CbmRichRing::GetDistance() method is no longer supported
      // If you wan to use cuts update code using CbmRichUtil::GetRingTrackDistance()
      fhEPEM_identifiedLepton_distance->Fill(1. /*richRing->GetDistance() */);
      fhEPEM_identifiedLepton_selectionNN->Fill(richRing->GetSelectionNN());
      fhEPEM_identifiedLepton_radius_vs_p->Fill(fRecoRefittedMomentum[e3].Mag(), richRing->GetRadius());
      fhEPEM_identifiedLepton_ANNvalue->Fill(ANNvalueE3);
    }
    if (IsRichElectron4ann > 0) {
      CbmGlobalTrack* gTrack = (CbmGlobalTrack*) fGlobalTracks->At(fRecoTracklistEPEM_gtid[e4]);
      int richInd            = gTrack->GetRichRingIndex();
      CbmRichRing* richRing  = (CbmRichRing*) fRichRings->At(richInd);
      Int_t nofringhits      = richRing->GetNofHits();
      fhEPEM_identifiedLepton_nofRingHits->Fill(nofringhits);
      fhEPEM_identifiedLepton_ringMid->Fill(richRing->GetCenterX(), richRing->GetCenterY());
      fhEPEM_identifiedLepton_ringRadius->Fill(richRing->GetRadius());
      // CbmRichRing::GetDistance() method is no longer supported
      // If you wan to use cuts update code using CbmRichUtil::GetRingTrackDistance()
      fhEPEM_identifiedLepton_distance->Fill(1. /*richRing->GetDistance()*/);
      fhEPEM_identifiedLepton_selectionNN->Fill(richRing->GetSelectionNN());
      fhEPEM_identifiedLepton_radius_vs_p->Fill(fRecoRefittedMomentum[e4].Mag(), richRing->GetRadius());
      fhEPEM_identifiedLepton_ANNvalue->Fill(ANNvalueE4);
    }


    fhEPEM_pi0_ANNvalues_noCuts->Fill(ANNvalueE1);
    fhEPEM_pi0_ANNvalues_noCuts->Fill(ANNvalueE2);
    fhEPEM_pi0_ANNvalues_noCuts->Fill(ANNvalueE3);
    fhEPEM_pi0_ANNvalues_noCuts->Fill(ANNvalueE4);
    if (OpeningAngleCut1 && OpeningAngleCut2) {
      fhEPEM_pi0_ANNvalues_angleCut->Fill(ANNvalueE1);
      fhEPEM_pi0_ANNvalues_angleCut->Fill(ANNvalueE2);
      fhEPEM_pi0_ANNvalues_angleCut->Fill(ANNvalueE3);
      fhEPEM_pi0_ANNvalues_angleCut->Fill(ANNvalueE4);
    }

    fhEPEM_pi0_ANNefficiencies->Fill(0);
    if (ANNvalueE1 > -1 && ANNvalueE2 > -1 && ANNvalueE3 > -1 && ANNvalueE4 > -1) {
      fhEPEM_pi0_ANNefficiencies->Fill(1);
    }
    if (ANNvalueE1 > -0.9 && ANNvalueE2 > -0.9 && ANNvalueE3 > -0.9 && ANNvalueE4 > -0.9) {
      fhEPEM_pi0_ANNefficiencies->Fill(2);
    }
    if (ANNvalueE1 > -0.8 && ANNvalueE2 > -0.8 && ANNvalueE3 > -0.8 && ANNvalueE4 > -0.8) {
      fhEPEM_pi0_ANNefficiencies->Fill(3);
    }
    if (ANNvalueE1 > -0.7 && ANNvalueE2 > -0.7 && ANNvalueE3 > -0.7 && ANNvalueE4 > -0.7) {
      fhEPEM_pi0_ANNefficiencies->Fill(4);
    }
    if (ANNvalueE1 > -0.6 && ANNvalueE2 > -0.6 && ANNvalueE3 > -0.6 && ANNvalueE4 > -0.6) {
      fhEPEM_pi0_ANNefficiencies->Fill(5);
    }
    if (ANNvalueE1 > -0.5 && ANNvalueE2 > -0.5 && ANNvalueE3 > -0.5 && ANNvalueE4 > -0.5) {
      fhEPEM_pi0_ANNefficiencies->Fill(6);
    }
    if (ANNvalueE1 > -0.4 && ANNvalueE2 > -0.4 && ANNvalueE3 > -0.4 && ANNvalueE4 > -0.4) {
      fhEPEM_pi0_ANNefficiencies->Fill(7);
    }
    if (ANNvalueE1 > -0.3 && ANNvalueE2 > -0.3 && ANNvalueE3 > -0.3 && ANNvalueE4 > -0.3) {
      fhEPEM_pi0_ANNefficiencies->Fill(8);
    }
    if (ANNvalueE1 > -0.2 && ANNvalueE2 > -0.2 && ANNvalueE3 > -0.2 && ANNvalueE4 > -0.2) {
      fhEPEM_pi0_ANNefficiencies->Fill(9);
    }
    if (ANNvalueE1 > -0.1 && ANNvalueE2 > -0.1 && ANNvalueE3 > -0.1 && ANNvalueE4 > -0.1) {
      fhEPEM_pi0_ANNefficiencies->Fill(10);
    }
    if (ANNvalueE1 > -0.0 && ANNvalueE2 > -0.0 && ANNvalueE3 > -0.0 && ANNvalueE4 > -0.0) {
      fhEPEM_pi0_ANNefficiencies->Fill(11);
    }

    fhEPEM_efficiencyCuts->Fill(0);  // no further cuts applied
    // first ANN usage for electron identification
    if (IsRichElectron1ann && IsRichElectron2ann && IsRichElectron3ann
        && IsRichElectron4ann) {  // all 4 electrons correctly identified with the RICH via ANN
      fhEPEM_efficiencyCuts->Fill(1);
      if (AllWithinChiCut) {  // refitted momenta are within chi cut
        fhEPEM_efficiencyCuts->Fill(2);
        if (OpeningAngleCut1 && OpeningAngleCut2) {  // opening angle of e+e- pairs below x
          fhEPEM_efficiencyCuts->Fill(3);
          if (InvariantMassCut1 && InvariantMassCut2) { fhEPEM_efficiencyCuts->Fill(4); }
        }
      }
    }
    // then standard method for electron identification
    if (IsRichElectron1normal && IsRichElectron2normal && IsRichElectron3normal
        && IsRichElectron4normal) {  // all 4 electrons correctly identified with the RICH via "normal way"
      fhEPEM_efficiencyCuts->Fill(5);
      if (AllWithinChiCut) {  // refitted momenta are within chi cut
        fhEPEM_efficiencyCuts->Fill(6);
        if (OpeningAngleCut1 && OpeningAngleCut2) {  // opening angle of e+e- pairs below x
          fhEPEM_efficiencyCuts->Fill(7);
          if (InvariantMassCut1 && InvariantMassCut2) { fhEPEM_efficiencyCuts->Fill(8); }
        }
      }
    }
    // MC-true data for electron identification
    if (IsRichElectron1MC && IsRichElectron2MC && IsRichElectron3MC
        && IsRichElectron4MC) {  // all 4 electrons correctly identified with the RICH via MC-true data
      fhEPEM_efficiencyCuts->Fill(9);
      if (AllWithinChiCut) {  // refitted momenta are within chi cut
        fhEPEM_efficiencyCuts->Fill(10);
        if (OpeningAngleCut1 && OpeningAngleCut2) {  // opening angle of e+e- pairs below x
          fhEPEM_efficiencyCuts->Fill(11);
          if (InvariantMassCut1 && InvariantMassCut2) { fhEPEM_efficiencyCuts->Fill(12); }
        }
      }
    }
  }

  if (IsEta == 1) {
    fhEPEM_efficiencyCuts_eta->Fill(0);  // no further cuts applied
    // first ANN usage for electron identification
    if (IsRichElectron1ann && IsRichElectron2ann && IsRichElectron3ann
        && IsRichElectron4ann) {  // all 4 electrons correctly identified with the RICH via ANN
      fhEPEM_efficiencyCuts_eta->Fill(1);
      if (AllWithinChiCut) {  // refitted momenta are within chi cut
        fhEPEM_efficiencyCuts_eta->Fill(2);
        if (OpeningAngleCut1 && OpeningAngleCut2) {  // opening angle of e+e- pairs below x
          fhEPEM_efficiencyCuts_eta->Fill(3);
          if (InvariantMassCut1 && InvariantMassCut2) { fhEPEM_efficiencyCuts_eta->Fill(4); }
        }
      }
    }
    // then standard method for electron identification
    if (IsRichElectron1normal && IsRichElectron2normal && IsRichElectron3normal
        && IsRichElectron4normal) {  // all 4 electrons correctly identified with the RICH via "normal way"
      fhEPEM_efficiencyCuts_eta->Fill(5);
      if (AllWithinChiCut) {  // refitted momenta are within chi cut
        fhEPEM_efficiencyCuts_eta->Fill(6);
        if (OpeningAngleCut1 && OpeningAngleCut2) {  // opening angle of e+e- pairs below x
          fhEPEM_efficiencyCuts_eta->Fill(7);
          if (InvariantMassCut1 && InvariantMassCut2) { fhEPEM_efficiencyCuts_eta->Fill(8); }
        }
      }
    }
    // MC-true data for electron identification
    if (IsRichElectron1MC && IsRichElectron2MC && IsRichElectron3MC
        && IsRichElectron4MC) {  // all 4 electrons correctly identified with the RICH via MC-true data
      fhEPEM_efficiencyCuts_eta->Fill(9);
      if (AllWithinChiCut) {  // refitted momenta are within chi cut
        fhEPEM_efficiencyCuts_eta->Fill(10);
        if (OpeningAngleCut1 && OpeningAngleCut2) {  // opening angle of e+e- pairs below x
          fhEPEM_efficiencyCuts_eta->Fill(11);
          if (InvariantMassCut1 && InvariantMassCut2) { fhEPEM_efficiencyCuts_eta->Fill(12); }
        }
      }
    }
  }
}


Int_t CbmAnaConversionReco::NofDaughters(Int_t motherId)
{
  Int_t nofDaughters = 0;
  for (unsigned int i = 0; i < fRecoTracklistEPEM.size(); i++) {
    Int_t motherId_temp = fRecoTracklistEPEM[i]->GetMotherId();
    if (motherId == motherId_temp) nofDaughters++;
  }


  return nofDaughters;
}


Double_t CbmAnaConversionReco::CalculateOpeningAngleReco(TVector3 electron1, TVector3 electron2)
{
  Double_t energyP = TMath::Sqrt(electron1.Mag2() + M2E);
  TLorentzVector lorVecP(electron1, energyP);

  Double_t energyM = TMath::Sqrt(electron2.Mag2() + M2E);
  TLorentzVector lorVecM(electron2, energyM);

  Double_t anglePair = lorVecM.Angle(lorVecP.Vect());
  Double_t theta     = 180. * anglePair / TMath::Pi();

  return theta;
}


Double_t CbmAnaConversionReco::CalculateOpeningAngleMC(CbmMCTrack* mctrack1, CbmMCTrack* mctrack2)
{
  TVector3 electron1;
  mctrack1->GetMomentum(electron1);
  Double_t energyP = TMath::Sqrt(electron1.Mag2() + M2E);
  TLorentzVector lorVecP(electron1, energyP);

  TVector3 electron2;
  mctrack2->GetMomentum(electron2);
  Double_t energyM = TMath::Sqrt(electron2.Mag2() + M2E);
  TLorentzVector lorVecM(electron2, energyM);

  Double_t anglePair = lorVecM.Angle(lorVecP.Vect());
  Double_t theta     = 180. * anglePair / TMath::Pi();

  return theta;
}


Double_t CbmAnaConversionReco::CalculateOpeningAngleBetweenGammasMC(CbmMCTrack* mctrack1, CbmMCTrack* mctrack2,
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


Double_t CbmAnaConversionReco::CalculateOpeningAngleBetweenGammasReco(TVector3 electron1, TVector3 electron2,
                                                                      TVector3 electron3, TVector3 electron4)
{
  Double_t energy1 = TMath::Sqrt(electron1.Mag2() + M2E);
  TLorentzVector lorVec1(electron1, energy1);

  Double_t energy2 = TMath::Sqrt(electron2.Mag2() + M2E);
  TLorentzVector lorVec2(electron2, energy2);

  Double_t energy3 = TMath::Sqrt(electron3.Mag2() + M2E);
  TLorentzVector lorVec3(electron3, energy3);

  Double_t energy4 = TMath::Sqrt(electron4.Mag2() + M2E);
  TLorentzVector lorVec4(electron4, energy4);

  TLorentzVector lorPhoton1 = lorVec1 + lorVec2;
  TLorentzVector lorPhoton2 = lorVec3 + lorVec4;

  Double_t angleBetweenPhotons = lorPhoton1.Angle(lorPhoton2.Vect());
  Double_t theta               = 180. * angleBetweenPhotons / TMath::Pi();

  return theta;
}


void CbmAnaConversionReco::CalculateInvMassWithFullRecoCuts()
{
  Int_t nof = fRecoTracklistEPEM.size();
  cout << "CbmAnaConversionReco::CalculateInvMassWithFullRecoCuts: nof entries - " << nof << endl;
  if (nof >= 4) {
    for (int a = 0; a < nof - 3; a++) {
      for (int b = a + 1; b < nof - 2; b++) {
        for (int c = b + 1; c < nof - 1; c++) {
          for (int d = c + 1; d < nof; d++) {
            Int_t check1 = (fRecoTracklistEPEM[a]->GetPdgCode() > 0);
            Int_t check2 = (fRecoTracklistEPEM[b]->GetPdgCode() > 0);
            Int_t check3 = (fRecoTracklistEPEM[c]->GetPdgCode() > 0);
            Int_t check4 = (fRecoTracklistEPEM[d]->GetPdgCode() > 0);
            Int_t test   = check1 + check2 + check3 + check4;
            if (test != 2) continue;  // need two electrons and two positrons


            Double_t invmass = Invmass_4particlesRECO(fRecoRefittedMomentum[a], fRecoRefittedMomentum[b],
                                                      fRecoRefittedMomentum[c], fRecoRefittedMomentum[d]);
            //fhElectrons_invmass->Fill(invmass);


            LmvmKinePar params1 = CalculateKinematicParamsReco(fRecoRefittedMomentum[a], fRecoRefittedMomentum[b]);
            LmvmKinePar params2 = CalculateKinematicParamsReco(fRecoRefittedMomentum[a], fRecoRefittedMomentum[c]);
            LmvmKinePar params3 = CalculateKinematicParamsReco(fRecoRefittedMomentum[a], fRecoRefittedMomentum[d]);
            LmvmKinePar params4 = CalculateKinematicParamsReco(fRecoRefittedMomentum[b], fRecoRefittedMomentum[c]);
            LmvmKinePar params5 = CalculateKinematicParamsReco(fRecoRefittedMomentum[b], fRecoRefittedMomentum[d]);
            LmvmKinePar params6 = CalculateKinematicParamsReco(fRecoRefittedMomentum[c], fRecoRefittedMomentum[d]);

            Double_t openingAngleCut     = 1;
            Int_t IsPhoton_openingAngle1 = (params1.fAngle < openingAngleCut);
            Int_t IsPhoton_openingAngle2 = (params2.fAngle < openingAngleCut);
            Int_t IsPhoton_openingAngle3 = (params3.fAngle < openingAngleCut);
            Int_t IsPhoton_openingAngle4 = (params4.fAngle < openingAngleCut);
            Int_t IsPhoton_openingAngle5 = (params5.fAngle < openingAngleCut);
            Int_t IsPhoton_openingAngle6 = (params6.fAngle < openingAngleCut);

            Double_t invMassCut     = 0.03;
            Int_t IsPhoton_invMass1 = (params1.fMinv < invMassCut);
            Int_t IsPhoton_invMass2 = (params2.fMinv < invMassCut);
            Int_t IsPhoton_invMass3 = (params3.fMinv < invMassCut);
            Int_t IsPhoton_invMass4 = (params4.fMinv < invMassCut);
            Int_t IsPhoton_invMass5 = (params5.fMinv < invMassCut);
            Int_t IsPhoton_invMass6 = (params6.fMinv < invMassCut);

            if (IsPhoton_openingAngle1 && IsPhoton_openingAngle6 && IsPhoton_invMass1 && IsPhoton_invMass6
                && (check1 + check2 == 1) && (check3 + check4 == 1)) {
              fhInvMassWithFullRecoCuts->Fill(invmass);
            }
            if (IsPhoton_openingAngle2 && IsPhoton_openingAngle5 && IsPhoton_invMass2 && IsPhoton_invMass5
                && (check1 + check3 == 1) && (check2 + check4 == 1)) {
              fhInvMassWithFullRecoCuts->Fill(invmass);
            }
            if (IsPhoton_openingAngle3 && IsPhoton_openingAngle4 && IsPhoton_invMass3 && IsPhoton_invMass4
                && (check1 + check4 == 1) && (check2 + check3 == 1)) {
              fhInvMassWithFullRecoCuts->Fill(invmass);
            }
          }
        }
      }
    }
  }
}


LmvmKinePar CbmAnaConversionReco::CalculateKinematicParamsReco(const TVector3 electron1, const TVector3 electron2)
{
  LmvmKinePar params;

  Double_t energyP = TMath::Sqrt(electron1.Mag2() + M2E);
  TLorentzVector lorVecP(electron1, energyP);

  Double_t energyM = TMath::Sqrt(electron2.Mag2() + M2E);
  TLorentzVector lorVecM(electron2, energyM);

  TVector3 momPair    = electron1 + electron2;
  Double_t energyPair = energyP + energyM;
  Double_t ptPair     = momPair.Perp();
  Double_t pzPair     = momPair.Pz();
  Double_t yPair      = 0.5 * TMath::Log((energyPair + pzPair) / (energyPair - pzPair));
  Double_t anglePair  = lorVecM.Angle(lorVecP.Vect());
  Double_t theta      = 180. * anglePair / TMath::Pi();
  Double_t minv       = 2. * TMath::Sin(anglePair / 2.) * TMath::Sqrt(electron1.Mag() * electron2.Mag());

  params.fMomentumMag = momPair.Mag();
  params.fPt          = ptPair;
  params.fRapidity    = yPair;
  params.fMinv        = minv;
  params.fAngle       = theta;
  return params;
}


LmvmKinePar CbmAnaConversionReco::CalculateKinematicParams_4particles(const TVector3 part1, const TVector3 part2,
                                                                      const TVector3 part3, const TVector3 part4)
{
  LmvmKinePar params;

  Double_t energy1 = TMath::Sqrt(part1.Mag2() + M2E);
  TLorentzVector lorVec1(part1, energy1);

  Double_t energy2 = TMath::Sqrt(part2.Mag2() + M2E);
  TLorentzVector lorVec2(part2, energy2);

  Double_t energy3 = TMath::Sqrt(part3.Mag2() + M2E);
  TLorentzVector lorVec3(part3, energy3);

  Double_t energy4 = TMath::Sqrt(part4.Mag2() + M2E);
  TLorentzVector lorVec4(part4, energy4);

  TLorentzVector sum;
  sum = lorVec1 + lorVec2 + lorVec3 + lorVec4;

  TVector3 momPair    = part1 + part2 + part3 + part4;
  Double_t energyPair = energy1 + energy2 + energy3 + energy4;
  Double_t ptPair     = momPair.Perp();
  Double_t pzPair     = momPair.Pz();
  Double_t yPair      = 0.5 * TMath::Log((energyPair + pzPair) / (energyPair - pzPair));
  //    Double_t anglePair = 0;
  //    Double_t theta = 180.*anglePair/TMath::Pi();
  Double_t minv = sum.Mag();

  params.fMomentumMag = momPair.Mag();
  params.fPt          = ptPair;
  params.fRapidity    = yPair;
  params.fMinv        = minv;
  params.fAngle       = 0;
  return params;
}


Bool_t CbmAnaConversionReco::IsRichElectronANN(Int_t globalTrackIndex, Double_t momentum)
{
  if (NULL == fGlobalTracks || NULL == fRichRings) return false;
  //CbmGlobalTrack* globalTrack = (CbmGlobalTrack*) fGlobalTracks->At(globalTrackIndex);
  const CbmGlobalTrack* globalTrack = static_cast<const CbmGlobalTrack*>(fGlobalTracks->At(globalTrackIndex));
  Int_t richId                      = globalTrack->GetRichRingIndex();
  if (richId < 0) return false;
  CbmRichRing* ring = static_cast<CbmRichRing*>(fRichRings->At(richId));
  if (NULL == ring) return false;

  Double_t fRichAnnCut = -0.8;
  Double_t ann         = CbmRichElectronIdAnn::GetInstance().CalculateAnnValue(globalTrackIndex, momentum);
  if (ann > fRichAnnCut) return true;
  else
    return false;
}


Double_t CbmAnaConversionReco::ElectronANNvalue(Int_t globalTrackIndex, Double_t momentum)
{
  if (NULL == fGlobalTracks || NULL == fRichRings) return -2;
  //CbmGlobalTrack* globalTrack = (CbmGlobalTrack*) fGlobalTracks->At(globalTrackIndex);
  const CbmGlobalTrack* globalTrack = static_cast<const CbmGlobalTrack*>(fGlobalTracks->At(globalTrackIndex));
  Int_t richId                      = globalTrack->GetRichRingIndex();
  if (richId < 0) return -2;
  CbmRichRing* ring = static_cast<CbmRichRing*>(fRichRings->At(richId));
  if (NULL == ring) return -2;

  Double_t ann = CbmRichElectronIdAnn::GetInstance().CalculateAnnValue(globalTrackIndex, momentum);
  return ann;
}


Bool_t CbmAnaConversionReco::IsRichElectronNormal(Int_t globalTrackIndex, Double_t momentum)
{
  if (NULL == fGlobalTracks || NULL == fRichRings) return false;
  CbmGlobalTrack* globalTrack = (CbmGlobalTrack*) fGlobalTracks->At(globalTrackIndex);
  Int_t richId                = globalTrack->GetRichRingIndex();
  if (richId < 0) return false;
  CbmRichRing* ring = static_cast<CbmRichRing*>(fRichRings->At(richId));
  if (NULL == ring) return false;

  Double_t axisA = ring->GetAaxis();
  Double_t axisB = ring->GetBaxis();
  Double_t dist  = CbmRichUtil::GetRingTrackDistance(globalTrackIndex);

  Double_t fMeanA    = 4.95;
  Double_t fMeanB    = 4.54;
  Double_t fRmsA     = 0.30;
  Double_t fRmsB     = 0.22;
  Double_t fRmsCoeff = 4.5;
  Double_t fDistCut  = 1.;

  Bool_t isElectronRICH = 0;

  if (momentum < 5.5) {
    if (fabs(axisA - fMeanA) < fRmsCoeff * fRmsA && fabs(axisB - fMeanB) < fRmsCoeff * fRmsB && dist < fDistCut)
      isElectronRICH = 1;
  }
  else {
    ///3 sigma
    // Double_t polAaxis = 5.80008 - 4.10118 / (momentum - 3.67402);
    // Double_t polBaxis = 5.58839 - 4.75980 / (momentum - 3.31648);
    // Double_t polRaxis = 5.87252 - 7.64641/(momentum - 1.62255);
    ///2 sigma
    Double_t polAaxis = 5.64791 - 4.24077 / (momentum - 3.65494);
    Double_t polBaxis = 5.41106 - 4.49902 / (momentum - 3.52450);
    //Double_t polRaxis = 5.66516 - 6.62229/(momentum - 2.25304);
    if (axisA < (fMeanA + fRmsCoeff * fRmsA) && axisA > polAaxis && axisB < (fMeanB + fRmsCoeff * fRmsB)
        && axisB > polBaxis && dist < fDistCut)
      isElectronRICH = 1;
  }
  return isElectronRICH;
}
