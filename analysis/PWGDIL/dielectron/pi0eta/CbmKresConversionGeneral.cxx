/* Copyright (C) 2017-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Ievgenii Kres, Florian Uhlig [committer] */

/**
 *    file CbmKresConversionGeneral.cxx
 *
 *    author Ievgenii Kres
 *    date 04.04.2017
 *    modified 30.01.2020
 *
 *    This class is called from the CbmKresConversionMain. It dedicated to plot general species of reconstructed particles as well as MC information.
 *    Here one can find information of hit distribution on the PMT, ring A and B from the fit, phase-space distribution of produced particles (MC), tomography plots, momenta, rapidity
 *    for single leptons, photons, and eta/pi^0 and etc.
 **/

#include "CbmKresConversionGeneral.h"

#include "CbmGlobalTrack.h"
#include "CbmMCTrack.h"
#include "CbmRichHit.h"
#include "CbmRichPoint.h"
#include "CbmRichRing.h"
#include "CbmRichRingFitterEllipseTau.h"
#include "CbmRichRingLight.h"
#include "CbmRichUtil.h"
#include "CbmStsTrack.h"
#include "CbmTrackMatchNew.h"

#include "FairRootManager.h"

#include "TGraph.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TProfile2D.h"

#include <iostream>

using namespace std;

CbmKresConversionGeneral::CbmKresConversionGeneral()
  : fMcTracks(nullptr)
  , fGlobalTracks(nullptr)
  , fStsTracks(nullptr)
  , fStsTrackMatches(nullptr)
  , fRichPoints(nullptr)
  , fRichHits(nullptr)
  , fRichRings(nullptr)
  , fRichRingMatches(nullptr)
  , fRichProjections(nullptr)
  , fArrayCentrality(nullptr)
  , fTauFit(nullptr)
  , fMinAaxis(0.)
  , fMaxAaxis(0.)
  , fMinBaxis(0.)
  , fMaxBaxis(0.)
  , fMinRadius(0.)
  , fMaxRadius(0.)
  , fitt(nullptr)
  , imageellipse(nullptr)
  , imagehits(nullptr)
  , fHistoList()
  , fHistoList_MC()
  , ForChristian_P_vs_R(nullptr)
  , AllPoints2D(nullptr)
  , AllPoints3D(nullptr)
  , MC_PdgCodes(nullptr)
  , MC_All_photons_Pt(nullptr)
  , MC_Not_Direct_photons_Pt(nullptr)
  , MC_Direct_photons_Pt(nullptr)
  , MC_All_photons_P(nullptr)
  , MC_Not_Direct_photons_P(nullptr)
  , MC_Direct_photons_P(nullptr)
  , MC_photons_mother_Pdg(nullptr)
  , MC_Not_Direct_photons_theta(nullptr)
  , MC_Not_Direct_photons_theta_vs_rap(nullptr)
  , MC_Direct_photons_theta(nullptr)
  , MC_Direct_photons_theta_vs_rap(nullptr)
  , MC_Direct_photons_Pt_vs_rap(nullptr)
  , MC_Direct_photons_Pt_vs_rap_est(nullptr)
  , MC_electrons_Pt_vs_rap_est(nullptr)
  , MC_Reconstructed_electrons_Pt_vs_rap_est(nullptr)
  , MC_omega_Pt_vs_rap_est(nullptr)
  , MC_pi0_Pt(nullptr)
  , MC_pi0_Pt_est(nullptr)
  , MC_pi0_Pt_vs_rap(nullptr)
  , MC_pi0_Pt_vs_rap_primary(nullptr)
  , Pi0_pt_vs_rap_est(nullptr)
  , Pi0_pt_vs_rap_est_primary(nullptr)
  , MC_pi0_theta(nullptr)
  , MC_pi0_phi(nullptr)
  , MC_pi0_Rapidity(nullptr)
  , MC_pi0_theta_vs_rap(nullptr)
  , MC_leptons_conversion_ZY(nullptr)
  , MC_leptons_conversion_XY(nullptr)
  , MC_leptons_conversion_XZ(nullptr)
  , MC_leptons_from_pi0_start_vertex(nullptr)
  , MC_leptons_from_pi0_P(nullptr)
  , MC_eta_Pt(nullptr)
  , MC_eta_Pt_vs_rap(nullptr)
  , MC_eta_Pt_vs_rap_primary(nullptr)
  , MC_eta_theta(nullptr)
  , MC_eta_theta_vs_rap(nullptr)
  , BoA_electrons(nullptr)
  , BoA_1d_electrons(nullptr)
  , A_1d_electrons(nullptr)
  , B_1d_electrons(nullptr)
  , A_electrons(nullptr)
  , B_electrons(nullptr)
  , NumberOfRings_electrons(nullptr)
  , AllHits_electrons(nullptr)
  , dR_electrons(nullptr)
  , dR2d_electrons(nullptr)
  , Distance_electron(nullptr)
  , Distance_positron(nullptr)
  , Tracks_electrons(nullptr)
  , Rings_electrons(nullptr)
  , fhBoverAXYZ(nullptr)
  , fhBaxisXYZ(nullptr)
  , fhAaxisXYZ(nullptr)
  , fhdRXYZ(nullptr)
  , Test_rings(nullptr)
  , AllPointsPerPMT(nullptr)
  , AllPointsPerPixel(nullptr)
  , AllHits2D(nullptr)
  , AllHits3D(nullptr)
  , AllHitsPerPMT(nullptr)
  , AllHitsPerPixel(nullptr)
  , temporarygraph(nullptr)
  , HitsPerPmtFullPlane(nullptr)
  , HitsPerPmtFullMiddle(nullptr)
{
  // ranges for the good rings. A and B are major and minor axes of the ellipse.
  fMinAaxis  = 3.;
  fMaxAaxis  = 7.;
  fMinBaxis  = 3.;
  fMaxBaxis  = 7.;
  fMinRadius = 3.;
  fMaxRadius = 7.;
}

CbmKresConversionGeneral::~CbmKresConversionGeneral() {}

void CbmKresConversionGeneral::Init()
{
  InitHistograms();

  FairRootManager* ioman = FairRootManager::Instance();
  if (nullptr == ioman) { Fatal("CbmKresConversionGeneral::Init", "RootManager not instantised!"); }

  fMcTracks = (TClonesArray*) ioman->GetObject("MCTrack");
  if (nullptr == fMcTracks) { Fatal("CbmKresConversionGeneral::Init", "No MCTrack array!"); }

  fGlobalTracks = (TClonesArray*) ioman->GetObject("GlobalTrack");
  if (nullptr == fGlobalTracks) { Fatal("CbmKresConversionGeneral::Init", "No GlobalTrack array!"); }

  fStsTracks = (TClonesArray*) ioman->GetObject("StsTrack");
  if (nullptr == fStsTracks) { Fatal("CbmKresConversionGeneral::Init", "No StsTrack array!"); }

  fStsTrackMatches = (TClonesArray*) ioman->GetObject("StsTrackMatch");
  if (nullptr == fStsTrackMatches) { Fatal("CbmKresConversionGeneral::Init", "No StsTrackMatch array!"); }

  fRichPoints = (TClonesArray*) ioman->GetObject("RichPoint");
  if (nullptr == fRichPoints) { Fatal("CbmKresConversionGeneral::Init", "No RichPoint array!"); }

  fRichHits = (TClonesArray*) ioman->GetObject("RichHit");
  if (nullptr == fRichHits) { Fatal("CbmKresConversionGeneral::Init", "No RichHit array!"); }

  fRichRings = (TClonesArray*) ioman->GetObject("RichRing");
  if (nullptr == fRichRings) { Fatal("CbmKresConversionGeneral::Init", "No RichRing array!"); }

  fRichRingMatches = (TClonesArray*) ioman->GetObject("RichRingMatch");
  if (nullptr == fRichRingMatches) { Fatal("CbmKresConversionGeneral::Init", "No RichRingMatch array!"); }

  fRichProjections = (TClonesArray*) ioman->GetObject("RichProjection");
  if (nullptr == fRichProjections) { Fatal("CbmKresConversionGeneral::Init", "No RichProjection array!"); }

  fArrayCentrality = (FairMCEventHeader*) ioman->GetObject("MCEventHeader.");
  if (nullptr == fArrayCentrality) { Fatal("CbmAnaConversion2::Init", "No fArrayCentrality array!"); }

  fTauFit = new CbmRichRingFitterEllipseTau();  ///// fit of ellipse instead of ring within RICH detector
}


void CbmKresConversionGeneral::Exec(int /*fEventNumGen*/)
{
  // cout << "CbmKresConversionGeneral, event No. " <<  fEventNumGen << endl;

  // ========================================================================================
  ///////   Fill all RICH points in histograms -> before QE
  CbmRichRingLight ringPoint;
  int nofRichPoints = fRichPoints->GetEntriesFast();
  for (int i = 0; i < nofRichPoints; i++) {
    CbmRichPoint* point = (CbmRichPoint*) fRichPoints->At(i);
    if (point == nullptr) continue;
    // cout << "Point:  id = " << point->GetTrackID() << endl;
    double xPOINT = point->GetX();
    double yPOINT = point->GetY();
    double zPOINT = point->GetZ();
    AllPoints3D->Fill(xPOINT, yPOINT, zPOINT);
    AllPoints2D->Fill(xPOINT, yPOINT);
    AllPointsPerPMT->Fill(xPOINT, yPOINT);
    AllPointsPerPixel->Fill(xPOINT, yPOINT);
  }
  ///////   Fill all points in histograms (END)
  // ========================================================================================

  temporarygraph->Reset();
  // ========================================================================================
  ///////   Fill all hits in histograms -> after QE
  int nofRichHits = fRichHits->GetEntriesFast();
  for (int i = 0; i < nofRichHits; i++) {
    CbmRichHit* hit = (CbmRichHit*) fRichHits->At(i);
    if (hit == nullptr) continue;
    // cout << "Hits:  id = " << hit->GetRefId() << endl;
    double xHIT = hit->GetX();
    double yHIT = hit->GetY();
    double zHIT = hit->GetZ();
    AllHits3D->Fill(xHIT, yHIT, zHIT);
    AllHits2D->Fill(xHIT, yHIT);
    AllHitsPerPMT->Fill(xHIT, yHIT);
    AllHitsPerPixel->Fill(xHIT, yHIT);
    temporarygraph->Fill(xHIT, yHIT);
  }
  ///////   Fill all hits in histograms (END)
  // ========================================================================================

  for (int ix = 1; ix <= temporarygraph->GetNbinsX(); ix++) {
    for (int iy = 1; iy <= temporarygraph->GetNbinsY(); iy++) {
      if (temporarygraph->GetBinContent(ix, iy) == 0) continue;
      HitsPerPmtFullPlane->Fill(temporarygraph->GetBinContent(ix, iy));
      if (iy <= 6) HitsPerPmtFullMiddle->Fill(temporarygraph->GetBinContent(ix, iy));
    }
  }

  // ========================================================================================
  ///////   START - Analyse MC tracks
  Int_t nofMcTracks = fMcTracks->GetEntriesFast();
  for (int i = 0; i < nofMcTracks; i++) {
    CbmMCTrack* mctrack = (CbmMCTrack*) fMcTracks->At(i);
    if (mctrack == nullptr) continue;
    // cout << "Track: id = " << i << ";  pdg = " << mctrack->GetPdgCode() << ";  motherID = " << mctrack->GetMotherId() << endl;
    MC_PdgCodes->Fill(TMath::Abs(mctrack->GetPdgCode()));


    //***** pi0 analysis
    if (TMath::Abs(mctrack->GetPdgCode()) == 111) {
      TVector3 v, momentum;
      mctrack->GetStartVertex(v);
      mctrack->GetMomentum(momentum);
      MC_pi0_Pt->Fill(mctrack->GetPt());
      MC_pi0_Rapidity->Fill(mctrack->GetRapidity());
      if (mctrack->GetRapidity() > 1.2 && mctrack->GetRapidity() < 4.0) MC_pi0_Pt_est->Fill(mctrack->GetPt());
      MC_pi0_Pt_vs_rap->Fill(mctrack->GetRapidity(), mctrack->GetPt());
      Pi0_pt_vs_rap_est->Fill(mctrack->GetRapidity(), mctrack->GetPt());
      if (mctrack->GetMotherId() == -1) {
        MC_pi0_Pt_vs_rap_primary->Fill(mctrack->GetRapidity(), mctrack->GetPt());
        Pi0_pt_vs_rap_est_primary->Fill(mctrack->GetRapidity(), mctrack->GetPt());
      }
      MC_pi0_theta->Fill(momentum.Theta() * 180. / TMath::Pi());
      MC_pi0_phi->Fill(momentum.Phi() * 180. / TMath::Pi());
      MC_pi0_theta_vs_rap->Fill(momentum.Theta() * 180. / TMath::Pi(), mctrack->GetRapidity());
    }

    //***** eta analysis
    if (TMath::Abs(mctrack->GetPdgCode()) == 221) {
      TVector3 v, momentum;
      mctrack->GetStartVertex(v);
      mctrack->GetMomentum(momentum);
      MC_eta_Pt->Fill(mctrack->GetPt());
      MC_eta_Pt_vs_rap->Fill(mctrack->GetRapidity(), mctrack->GetPt());
      if (mctrack->GetMotherId() == -1) MC_eta_Pt_vs_rap_primary->Fill(mctrack->GetRapidity(), mctrack->GetPt());
      MC_eta_theta->Fill(momentum.Theta() * 180. / TMath::Pi());
      MC_eta_theta_vs_rap->Fill(momentum.Theta() * 180. / TMath::Pi(), mctrack->GetRapidity());
    }

    //***** photons analysis
    if (mctrack->GetPdgCode() == 22) {
      TVector3 v, momentum;
      mctrack->GetStartVertex(v);
      mctrack->GetMomentum(momentum);

      MC_All_photons_Pt->Fill(mctrack->GetPt());
      MC_All_photons_P->Fill(mctrack->GetP());
      if (mctrack->GetMotherId() != -1) {
        CbmMCTrack* mcTrackmama = (CbmMCTrack*) fMcTracks->At(mctrack->GetMotherId());
        MC_photons_mother_Pdg->Fill(mcTrackmama->GetPdgCode());
        MC_Not_Direct_photons_Pt->Fill(mctrack->GetPt());
        MC_Not_Direct_photons_P->Fill(mctrack->GetP());
        MC_Not_Direct_photons_theta->Fill(momentum.Theta() * 180. / TMath::Pi());
        MC_Not_Direct_photons_theta_vs_rap->Fill(momentum.Theta() * 180. / TMath::Pi(), mctrack->GetRapidity());
      }
      if (mctrack->GetMotherId() == -1) {
        MC_photons_mother_Pdg->Fill(mctrack->GetMotherId());
        MC_Direct_photons_Pt->Fill(mctrack->GetPt());
        MC_Direct_photons_P->Fill(mctrack->GetP());
        MC_Direct_photons_Pt_vs_rap->Fill(mctrack->GetRapidity(), mctrack->GetPt());
        MC_Direct_photons_Pt_vs_rap_est->Fill(mctrack->GetRapidity(), mctrack->GetPt());
        MC_Direct_photons_theta->Fill(momentum.Theta() * 180. / TMath::Pi());
        MC_Direct_photons_theta_vs_rap->Fill(momentum.Theta() * 180. / TMath::Pi(), mctrack->GetRapidity());
      }
    }

    //***** electrons and positrons from -> gamma conversion -> pi0
    if (TMath::Abs(mctrack->GetPdgCode()) == 11) {
      int motherId = mctrack->GetMotherId();
      MC_electrons_Pt_vs_rap_est->Fill(mctrack->GetRapidity(), mctrack->GetPt());
      if (motherId == -1) continue;
      CbmMCTrack* mother = (CbmMCTrack*) fMcTracks->At(motherId);
      if (nullptr == mother) continue;
      int grandmotherId = mother->GetMotherId();
      if (grandmotherId == -1) continue;
      CbmMCTrack* grandmother = (CbmMCTrack*) fMcTracks->At(grandmotherId);
      if (nullptr == grandmother) continue;
      int mcGrandmotherPdg = grandmother->GetPdgCode();
      if (mcGrandmotherPdg == 111) {
        MC_leptons_from_pi0_start_vertex->Fill(mctrack->GetStartZ(), mctrack->GetStartY());
        MC_leptons_from_pi0_P->Fill(mctrack->GetP());
      }
      MC_leptons_conversion_ZY->Fill(mctrack->GetStartZ(), mctrack->GetStartY());
      MC_leptons_conversion_XY->Fill(mctrack->GetStartX(), mctrack->GetStartY());
      MC_leptons_conversion_XZ->Fill(mctrack->GetStartX(), mctrack->GetStartZ());
    }

    // omega
    if (TMath::Abs(mctrack->GetPdgCode()) == 223) {
      MC_omega_Pt_vs_rap_est->Fill(mctrack->GetRapidity(), mctrack->GetPt());
    }
  }
  ///////   START - Analyse MC tracks (END)
  // ========================================================================================


  ///////   Global tracks analysis
  ////// Global track is completely reconstructed track. It has assigned indices from each detector. ID == -1  means no signal from this detector
  // ========================================================================================
  Int_t ngTracks = fGlobalTracks->GetEntriesFast();
  for (Int_t iTr = 0; iTr < ngTracks; iTr++) {
    CbmGlobalTrack* gTrack = (CbmGlobalTrack*) fGlobalTracks->At(iTr);
    if (nullptr == gTrack) continue;
    int stsInd  = gTrack->GetStsTrackIndex();  /// track index
    int richInd = gTrack->GetRichRingIndex();  /// ring index

    // ========================================================================================
    ///////   STS
    if (stsInd < 0) continue;
    CbmStsTrack* stsTrack = (CbmStsTrack*) fStsTracks->At(stsInd);
    if (stsTrack == nullptr) continue;
    CbmTrackMatchNew* stsMatch =
      (CbmTrackMatchNew*) fStsTrackMatches->At(stsInd);  /// match of reconstructed track to Monte Carlo
    if (stsMatch == nullptr) continue;
    if (stsMatch->GetNofLinks() <= 0) continue;
    int stsMcTrackId = stsMatch->GetMatchedLink().GetIndex();
    if (stsMcTrackId < 0) continue;
    CbmMCTrack* mcTrackSTS = (CbmMCTrack*) fMcTracks->At(stsMcTrackId);
    if (mcTrackSTS == nullptr) continue;
    int pdgSTS = mcTrackSTS->GetPdgCode();

    ////// histo for Christian CBM coll.meeting
    if (TMath::Abs(mcTrackSTS->GetPdgCode()) == 11 || TMath::Abs(mcTrackSTS->GetPdgCode()) == 211) {
      if (richInd >= 0) {
        CbmRichRing* richRing = (CbmRichRing*) fRichRings->At(richInd);
        if (richRing == nullptr) continue;
        CbmTrackMatchNew* richMatch = (CbmTrackMatchNew*) fRichRingMatches->At(richInd);
        if (richMatch == nullptr) continue;
        if (richMatch->GetNofLinks() <= 0) continue;
        int richMcTrackId = richMatch->GetMatchedLink().GetIndex();
        if (richMcTrackId < 0) continue;
        CbmMCTrack* mcTrackRICH = (CbmMCTrack*) fMcTracks->At(richMcTrackId);
        if (mcTrackRICH == nullptr) continue;
        int pdgRICH = mcTrackRICH->GetPdgCode();
        if (TMath::Abs(pdgRICH) != 11 && TMath::Abs(pdgRICH) != 211) continue;
        if (stsMcTrackId != richMcTrackId) continue;
        ForChristian_P_vs_R->Fill(mcTrackSTS->GetP(), richRing->GetRadius());
      }
    }

    if (TMath::Abs(pdgSTS) != 11) continue;


    Tracks_electrons->Fill(1);
    ///////   STS (END)
    // ========================================================================================

    // ========================================================================================
    ///////   RICH
    if (richInd < 0) continue;
    CbmRichRing* richRing = (CbmRichRing*) fRichRings->At(richInd);
    if (richRing == nullptr) continue;
    CbmTrackMatchNew* richMatch =
      (CbmTrackMatchNew*) fRichRingMatches->At(richInd);  /// match of reconstructed ring to Monte Carlo
    if (richMatch == nullptr) continue;
    if (richMatch->GetNofLinks() <= 0) continue;
    int richMcTrackId = richMatch->GetMatchedLink().GetIndex();
    if (richMcTrackId < 0) continue;
    CbmMCTrack* mcTrackRICH = (CbmMCTrack*) fMcTracks->At(richMcTrackId);
    if (mcTrackRICH == nullptr) continue;
    int pdgRICH = mcTrackRICH->GetPdgCode();
    if (TMath::Abs(pdgRICH) != 11) continue;
    ///////   RICH (END)
    // ========================================================================================

    if (stsMcTrackId != richMcTrackId) continue;

    Rings_electrons->Fill(1);

    // ========================================================================================
    ///////   dR calculation
    CbmRichRingLight ringHit;
    int nofHits = richRing->GetNofHits();
    for (int i = 0; i < nofHits; i++) {
      Int_t hitInd    = richRing->GetHit(i);
      CbmRichHit* hit = (CbmRichHit*) fRichHits->At(hitInd);
      if (nullptr == hit) continue;
      CbmRichHitLight hl(hit->GetX(), hit->GetY());
      ringHit.AddHit(hl);
    }
    FitAndFillHistEllipse(&ringHit);  // fit hits with ellipse

    if (ringHit.GetAaxis() > fMinAaxis && ringHit.GetAaxis() < fMaxAaxis && ringHit.GetBaxis() > fMinBaxis
        && ringHit.GetAaxis() < fMaxBaxis) {  //// only good rings
      if (mcTrackRICH->GetPdgCode() == -11)
        Distance_positron->Fill(richRing->GetCenterX(), richRing->GetCenterY(), CbmRichUtil::GetRingTrackDistance(iTr));
      if (mcTrackRICH->GetPdgCode() == 11)
        Distance_electron->Fill(richRing->GetCenterX(), richRing->GetCenterY(), CbmRichUtil::GetRingTrackDistance(iTr));
      double a  = ringHit.GetAaxis();
      double b  = ringHit.GetBaxis();
      double p  = ringHit.GetPhi();
      double xc = ringHit.GetCenterX();
      double yc = ringHit.GetCenterY();
      BoA_electrons->Fill(xc, yc, b / a);
      BoA_1d_electrons->Fill(b / a);
      A_1d_electrons->Fill(a);
      B_1d_electrons->Fill(b);
      A_electrons->Fill(xc, yc, a);
      B_electrons->Fill(xc, yc, b);
      NumberOfRings_electrons->Fill(xc, yc);

      fhBoverAXYZ->Fill(xc, yc, b / a);
      fhBaxisXYZ->Fill(xc, yc, b);
      fhAaxisXYZ->Fill(xc, yc, a);

      MC_Reconstructed_electrons_Pt_vs_rap_est->Fill(mcTrackRICH->GetRapidity(), mcTrackRICH->GetPt());

      // ding a closest position of the ring to the hit
      double minAngle  = 0.;
      double maxAngle  = 2 * 3.14159265358979323846;
      double stepAngle = 0.01;
      for (int iH = 0; iH < ringHit.GetNofHits(); iH++) {
        double xh = ringHit.GetHit(iH).fX;
        double yh = ringHit.GetHit(iH).fY;
        AllHits_electrons->Fill(xh, yh);
        double Lmin = 50.;
        double xmin = 0.;
        double ymin = 0.;
        for (double iT = minAngle; iT < maxAngle; iT = iT + stepAngle) {
          double xEll = a * cos(iT) * cos(p) - b * sin(iT) * sin(p) + xc;
          double yEll = a * cos(iT) * sin(p) + b * sin(iT) * cos(p) + yc;
          double L    = sqrt((xh - xEll) * (xh - xEll) + (yh - yEll) * (yh - yEll));
          if (L < Lmin) {
            Lmin = L;
            xmin = xEll;
            ymin = yEll;
          }
        }
        Double_t sign = sqrt((xmin - xc) * (xmin - xc) + (ymin - yc) * (ymin - yc))
                        - sqrt((xh - xc) * (xh - xc) + (yh - yc) * (yh - yc));
        double dr = Lmin;
        if (sign < 0) { dr = -Lmin; }
        dR_electrons->Fill(dr);
        dR2d_electrons->Fill(xc, yc, dr);
        fhdRXYZ->Fill(xc, yc, dr);
      }
    }
    ///////   dR (END)
    // ========================================================================================
  }
  ///////   Global tracks analysis (END)
  // ========================================================================================
}

void CbmKresConversionGeneral::FitAndFillHistEllipse(CbmRichRingLight* ring) { fTauFit->DoFit(ring); }


void CbmKresConversionGeneral::Finish()
{
  BoA_electrons->SetMaximum(1.);
  BoA_electrons->SetMinimum(0.8);
  A_electrons->SetMaximum(5.5);
  A_electrons->SetMinimum(4.);
  B_electrons->SetMaximum(5.);
  B_electrons->SetMinimum(4.);
  dR2d_electrons->SetMaximum(0.35);
  dR2d_electrons->SetMinimum(0.15);
  Distance_electron->SetMaximum(2.);
  Distance_electron->SetMinimum(0.);
  Distance_positron->SetMaximum(2.);
  Distance_positron->SetMinimum(0.);

  gDirectory->mkdir("General");
  gDirectory->cd("General");
  gDirectory->mkdir("MC_info");
  gDirectory->cd("MC_info");
  for (size_t i = 0; i < fHistoList_MC.size(); i++) {
    fHistoList_MC[i]->Write();
  }
  gDirectory->cd("..");
  for (size_t i = 0; i < fHistoList.size(); i++) {
    fHistoList[i]->Write();
  }
  gDirectory->cd("..");
}


void CbmKresConversionGeneral::InitHistograms()
{
  //***** photons analysis
  MC_All_photons_Pt =
    new TH1D("MC_All_photons_Pt", "Monte Carlo,   all #gamma,   p_{t} distribution; p_{t} in GeV/c", 200, 0, 2);
  fHistoList_MC.push_back(MC_All_photons_Pt);

  MC_Not_Direct_photons_Pt = new TH1D(
    "MC_Not_Direct_photons_Pt", "Monte Carlo,   #gamma from decays,   p_{t} distribution; p_{t} in GeV/c", 200, 0, 2);
  fHistoList_MC.push_back(MC_Not_Direct_photons_Pt);

  MC_Direct_photons_Pt =
    new TH1D("MC_Direct_photons_Pt", "Monte Carlo,   direct #gamma,   p_{t} distribution; p_{t} in GeV/c", 200, 0, 2);
  fHistoList_MC.push_back(MC_Direct_photons_Pt);

  MC_All_photons_P =
    new TH1D("MC_All_photons_P", "Monte Carlo,   all #gamma,   p distribution; p in GeV/c", 1000, 0, 10);
  fHistoList_MC.push_back(MC_All_photons_P);

  MC_Not_Direct_photons_P =
    new TH1D("MC_Not_Direct_photons_P", "Monte Carlo,   #gamma from decays,   p distribution; p in GeV/c", 1000, 0, 10);
  fHistoList_MC.push_back(MC_Not_Direct_photons_P);

  MC_Direct_photons_P =
    new TH1D("MC_Direct_photons_P", "Monte Carlo,   direct #gamma,   p distribution; p in GeV/c", 1000, 0, 10);
  fHistoList_MC.push_back(MC_Direct_photons_P);

  MC_Direct_photons_Pt_vs_rap = new TH2D("MC_Direct_photons_Pt_vs_rap",
                                         "Monte Carlo,   direct #gamma,   p_{t} vs rapidity distribution; "
                                         "rapidity y; p_{t} in GeV/c ",
                                         90, -2., 7., 60, -1., 5.);
  fHistoList_MC.push_back(MC_Direct_photons_Pt_vs_rap);

  MC_Direct_photons_Pt_vs_rap_est = new TH2D("MC_Direct_photons_Pt_vs_rap_est",
                                             "Monte Carlo,   direct #gamma,   p_{t} vs rapidity distribution; "
                                             "rapidity y; p_{t} in GeV/c ",
                                             10, 0., 4., 40, 0., 4.);
  fHistoList_MC.push_back(MC_Direct_photons_Pt_vs_rap_est);

  MC_photons_mother_Pdg = new TH1D("MC_photons_mother_Pdg", "MC_photons_mother_Pdg; Id;#", 2500, -10, 2511);
  fHistoList_MC.push_back(MC_photons_mother_Pdg);

  MC_Not_Direct_photons_theta = new TH1D("MC_Not_Direct_photons_theta",
                                         "Monte Carlo,   #gamma from decays,   #theta distribution; "
                                         "emission angle #theta in deg;Entries",
                                         180., 0., 180.);
  fHistoList_MC.push_back(MC_Not_Direct_photons_theta);

  MC_Not_Direct_photons_theta_vs_rap = new TH2D("MC_Not_Direct_photons_theta_vs_rap",
                                                "Monte Carlo,   #gamma from decays,   #theta vs rapidity "
                                                "distribution; emission angle #theta in deg; rapidity y",
                                                180., 0., 180., 270, -2., 7.);
  fHistoList_MC.push_back(MC_Not_Direct_photons_theta_vs_rap);

  MC_Direct_photons_theta = new TH1D("MC_Direct_photons_theta",
                                     "Monte Carlo,   direct #gamma,   #theta distribution; emission "
                                     "angle #theta in deg;Entries",
                                     180., 0., 180.);
  fHistoList_MC.push_back(MC_Direct_photons_theta);

  MC_Direct_photons_theta_vs_rap = new TH2D("MC_Direct_photons_theta_vs_rap",
                                            "Monte Carlo,   direct #gamma,   #theta vs rapidity distribution; "
                                            "emission angle #theta in deg; rapidity y",
                                            180., 0., 180., 270, -2., 7.);
  fHistoList_MC.push_back(MC_Direct_photons_theta_vs_rap);

  MC_electrons_Pt_vs_rap_est = new TH2D("MC_electrons_Pt_vs_rap_est",
                                        "Monte Carlo,   direct e,   p_{t} vs rapidity distribution; "
                                        "rapidity y; p_{t} in GeV/c ",
                                        10, 0., 4., 40, 0., 4.);
  fHistoList_MC.push_back(MC_electrons_Pt_vs_rap_est);

  MC_Reconstructed_electrons_Pt_vs_rap_est = new TH2D("MC_Reconstructed_electrons_Pt_vs_rap_est",
                                                      "Monte Carlo,   reconstructed e,   p_{t} vs rapidity "
                                                      "distribution; rapidity y; p_{t} in GeV/c ",
                                                      10, 0., 4., 40, 0., 4.);
  fHistoList_MC.push_back(MC_Reconstructed_electrons_Pt_vs_rap_est);

  MC_omega_Pt_vs_rap_est = new TH2D("MC_omega_Pt_vs_rap_est",
                                    "Monte Carlo,   #omega,   p_{t} vs rapidity distribution; "
                                    "rapidity y; p_{t} in GeV/c ",
                                    10, 0., 4., 40, 0., 4.);
  fHistoList_MC.push_back(MC_omega_Pt_vs_rap_est);


  //***** pi0 analysis
  MC_pi0_Pt =
    new TH1D("MC_pi0_Pt", "Monte Carlo,   #pi^{0},   p_{t} distribution;p_{t} in GeV/c; Entries", 200., 0., 10.);
  fHistoList_MC.push_back(MC_pi0_Pt);

  MC_pi0_Pt_est =
    new TH1D("MC_pi0_Pt_est", "Monte Carlo,   #pi^{0},   p_{t} distribution;p_{t} in GeV/c; Entries", 20., 0., 2.);
  fHistoList_MC.push_back(MC_pi0_Pt_est);

  MC_pi0_Pt_vs_rap = new TH2D("MC_pi0_Pt_vs_rap",
                              "Monte Carlo,   #pi^{0},   p_{t} vs rapidity "
                              "distribution; rapidity y; p_{t} in GeV/c ",
                              90, -2., 7., 60, -1., 5.);
  fHistoList_MC.push_back(MC_pi0_Pt_vs_rap);

  MC_pi0_Pt_vs_rap_primary = new TH2D("MC_pi0_Pt_vs_rap_primary",
                                      "Monte Carlo,   primary #pi^{0},   p_{t} vs rapidity "
                                      "distribution; rapidity y; p_{t} in GeV/c ",
                                      90, -2., 7., 60, -1., 5.);
  fHistoList_MC.push_back(MC_pi0_Pt_vs_rap_primary);

  Pi0_pt_vs_rap_est =
    new TH2D("Pi0_pt_vs_rap_est", "Pi0_pt_vs_rap_est; rapidity y; p_{t} in GeV/c ", 10, 0., 4., 40, 0., 4.);
  fHistoList_MC.push_back(Pi0_pt_vs_rap_est);

  Pi0_pt_vs_rap_est_primary = new TH2D(
    "Pi0_pt_vs_rap_est_primary", "Pi0_pt_vs_rap_est_primary; rapidity y; p_{t} in GeV/c ", 10, 0., 4., 40, 0., 4.);
  fHistoList_MC.push_back(Pi0_pt_vs_rap_est_primary);

  MC_pi0_theta = new TH1D("MC_pi0_theta",
                          "Monte Carlo,   #pi^{0},   #theta distribution; "
                          "emission angle #theta in deg;Entries",
                          180., 0., 180.);
  fHistoList_MC.push_back(MC_pi0_theta);

  MC_pi0_phi = new TH1D("MC_pi0_phi",
                        "Monte Carlo,   #pi^{0},   #phi distribution; emission "
                        "angle #phi in deg; Entries",
                        180., -180., 180.);
  fHistoList_MC.push_back(MC_pi0_phi);

  MC_pi0_Rapidity = new TH1D("MC_pi0_Rapidity",
                             "Monte Carlo,   #pi^{0},   #rapidity distribution; emission angle "
                             "#phi in deg; Entries",
                             90, -2., 7.);
  fHistoList_MC.push_back(MC_pi0_Rapidity);

  MC_pi0_theta_vs_rap = new TH2D("MC_pi0_theta_vs_rap",
                                 "Monte Carlo,   #pi^{0},   #theta vs rapidity distribution; "
                                 "emission angle #theta in deg; rapidity y",
                                 180., 0., 180., 270, -2., 7.);
  fHistoList_MC.push_back(MC_pi0_theta_vs_rap);

  MC_leptons_conversion_ZY =
    new TH2D("MC_leptons_conversion_ZY", "Start vertices of leptons coming from #gamma; z[cm]; y[cm]", 800, -1, 400,
             880, -220, 220);
  fHistoList_MC.push_back(MC_leptons_conversion_ZY);
  MC_leptons_conversion_XY =
    new TH2D("MC_leptons_conversion_XY", "Start vertices of leptons coming from #gamma; x[cm]; y[cm]", 1200, -300, 300,
             880, -220, 220);
  fHistoList_MC.push_back(MC_leptons_conversion_XY);
  MC_leptons_conversion_XZ =
    new TH2D("MC_leptons_conversion_XZ", "Start vertices of leptons coming from #gamma; x[cm]; z[cm]", 1200, -300, 300,
             800, -1, 400);
  fHistoList_MC.push_back(MC_leptons_conversion_XZ);

  MC_leptons_from_pi0_start_vertex =
    new TH2D("MC_leptons_from_pi0_start_vertex", "Start vertices of leptons coming from #pi^{0}; z[cm]; y[cm]", 200, -1,
             100, 200, -50, 50);
  fHistoList_MC.push_back(MC_leptons_from_pi0_start_vertex);

  MC_leptons_from_pi0_P = new TH1D("MC_leptons_from_pi0_P", "MC_leptons_from_pi0_P; P_{e} in GeV/c", 200, 0, 5);
  fHistoList_MC.push_back(MC_leptons_from_pi0_P);

  //***** eta analysis
  MC_eta_Pt = new TH1D("MC_eta_Pt", "Monte Carlo,   #eta,   p_{t} distribution; p_{t} in GeV/c;Entries", 200., 0., 10.);
  fHistoList_MC.push_back(MC_eta_Pt);

  MC_eta_Pt_vs_rap = new TH2D("MC_eta_Pt_vs_rap",
                              "Monte Carlo,   #eta,   p_{t} vs rapidity "
                              "distribution; rapidity y; p_{t} in GeV/c ",
                              90, -2., 7., 60, -1., 5.);
  fHistoList_MC.push_back(MC_eta_Pt_vs_rap);

  MC_eta_Pt_vs_rap_primary = new TH2D("MC_eta_Pt_vs_rap_primary",
                                      "Monte Carlo,   primary #eta,   p_{t} vs rapidity distribution; "
                                      "rapidity y; p_{t} in GeV/c ",
                                      90, -2., 7., 60, -1., 5.);
  fHistoList_MC.push_back(MC_eta_Pt_vs_rap_primary);

  MC_eta_theta = new TH1D("MC_eta_theta",
                          "Monte Carlo,   #eta,   #theta distribution; "
                          "emission angle #theta in deg; Entries",
                          180., 0., 180.);
  fHistoList_MC.push_back(MC_eta_theta);

  MC_eta_theta_vs_rap = new TH2D("MC_eta_theta_vs_rap",
                                 "Monte Carlo,   #eta,   #theta vs rapidity distribution; emission "
                                 "angle #theta in deg; rapidity y",
                                 180., 0., 180., 270, -2., 7.);
  fHistoList_MC.push_back(MC_eta_theta_vs_rap);

  //***** RICH analysis: A, B, dR, points, hits
  ForChristian_P_vs_R =
    new TH2D("ForChristian_P_vs_R", "ForChristian_P_vs_R; P [GeV]; R [cm]; Nof", 100, 0, 10, 100, 0, 8);
  fHistoList.push_back(ForChristian_P_vs_R);

  AllPoints2D = new TH2D("AllPoints2D", "AllPoints2D; X [cm]; Y [cm]; Nof", 300, -150, 150, 120, 120, 240);
  fHistoList.push_back(AllPoints2D);

  AllPoints3D =
    new TH3D("AllPoints3D", "AllPoints3D; X [cm]; Y [cm]; Z [cm]", 300, -150, 150, 120, 120, 240, 100, 160, 260);
  fHistoList.push_back(AllPoints3D);

  MC_PdgCodes = new TH1D("MC_PdgCodes", "All PdgCodes from Monte Carlo; PDG code", 3500, 0, 3500);
  fHistoList.push_back(MC_PdgCodes);

  BoA_electrons = new TProfile2D("BoA_electrons", "B/A for electrons; X [cm]; Y [cm]", 60, -150, 150, 20, 120, 220);
  fHistoList.push_back(BoA_electrons);

  A_1d_electrons = new TH1D("A_1d_electrons", "A for electrons; A [cm]", 200, 3., 7.);
  fHistoList.push_back(A_1d_electrons);

  B_1d_electrons = new TH1D("B_1d_electrons", "B for electrons; B [cm]", 200, 3., 7.);
  fHistoList.push_back(B_1d_electrons);

  BoA_1d_electrons = new TH1D("BoA_1d_electrons", "BoA for electrons; B/A", 250, 0.5, 1.);
  fHistoList.push_back(BoA_1d_electrons);

  Tracks_electrons = new TH1D("Tracks_electrons", "Electron tracks in STS", 3, -0.5, 2.5);
  fHistoList.push_back(Tracks_electrons);

  Rings_electrons = new TH1D("Rings_electrons", "Electron tracks in STS+RICH", 3, -0.5, 2.5);
  fHistoList.push_back(Rings_electrons);

  A_electrons = new TProfile2D("A_electrons", "A for electrons; X [cm]; Y [cm]", 60, -150, 150, 20, 120, 220);
  fHistoList.push_back(A_electrons);

  B_electrons = new TProfile2D("B_electrons", "B for electrons; X [cm]; Y [cm]", 60, -150, 150, 20, 120, 220);
  fHistoList.push_back(B_electrons);

  NumberOfRings_electrons = new TH2D("NumberOfRings_electrons", "Number of rings from electrons; X [cm]; Y [cm]; Nof",
                                     60, -150, 150, 20, 120, 220);
  fHistoList.push_back(NumberOfRings_electrons);

  AllHits_electrons = new TH2D("AllHits_electrons", "Hits from electrons after unfolding; X [cm]; Y [cm]; Nof", 300,
                               -150, 150, 120, 120, 240);
  fHistoList.push_back(AllHits_electrons);

  dR_electrons = new TH1D("dR_electrons", "dR for electrons; dR [cm]", 100, -2., 2.);
  fHistoList.push_back(dR_electrons);

  dR2d_electrons =
    new TProfile2D("dR2d_electrons", "dR for electrons; X [cm]; Y [cm]", 60, -150, 150, 20, 120, 220, 0, 1);
  fHistoList.push_back(dR2d_electrons);

  Distance_electron = new TProfile2D("Distance_electron",
                                     "Distance between projected track and center of the ring "
                                     "for electrons; X [cm];Y [cm]; Nof",
                                     60, -150, 150, 20, 120, 220);
  fHistoList.push_back(Distance_electron);

  Distance_positron = new TProfile2D("Distance_positron",
                                     "Distance between projected track and center of the ring "
                                     "for positrons; X [cm];Y [cm]; Nof",
                                     60, -150, 150, 20, 120, 220);
  fHistoList.push_back(Distance_positron);

  fhBoverAXYZ = new TH3D("fhBoverAXYZ", "fhBoverAXYZ; X [cm]; Y [cm]; B/A", 60, -150, 150, 20, 120, 220, 50, 0., 1.);
  fHistoList.push_back(fhBoverAXYZ);
  fhBaxisXYZ = new TH3D("fhBaxisXYZ", "fhBaxisXYZ; X [cm]; Y [cm]; B [cm]", 60, -150, 150, 20, 120, 220, 80, 3., 7.);
  fHistoList.push_back(fhBaxisXYZ);
  fhAaxisXYZ = new TH3D("fhAaxisXYZ", "fhAaxisXYZ; X [cm]; Y [cm]; A [cm]", 60, -150, 150, 20, 120, 220, 80, 3., 7.);
  fHistoList.push_back(fhAaxisXYZ);
  fhdRXYZ = new TH3D("fhdRXYZ", "fhdRXYZ; X [cm]; Y [cm]; dR [cm]", 60, -150, 150, 20, 120, 220, 100, -1., 1.);
  fHistoList.push_back(fhdRXYZ);

  Test_rings = new TH2D("Test_rings", "Test_rings", 60, -150, 150, 20, 120, 220);
  fHistoList.push_back(Test_rings);

  AllPointsPerPMT = new TH2D("AllPointsPerPMT", "AllPointsPerPMT; X [cm]; Y [cm]; Nof", 41, -105, 105, 17, 125, 210);
  fHistoList.push_back(AllPointsPerPMT);
  AllPointsPerPixel =
    new TH2D("AllPointsPerPixel", "AllPointsPerPixel; X [cm]; Y [cm]; Nof", 350, -105, 105, 142, 125, 210);
  fHistoList.push_back(AllPointsPerPixel);
  AllHitsPerPMT = new TH2D("AllHitsPerPMT", "AllHitsPerPMT; X [cm]; Y [cm]; Nof", 41, -105, 105, 17, 125, 210);
  fHistoList.push_back(AllHitsPerPMT);
  AllHitsPerPixel = new TH2D("AllHitsPerPixel", "AllHitsPerPixel; X [cm]; Y [cm]; Nof", 350, -105, 105, 142, 125, 210);
  fHistoList.push_back(AllHitsPerPixel);

  AllHits2D = new TH2D("AllHits2D", "AllHits2D; X [cm]; Y [cm]; Nof", 300, -150, 150, 120, 120, 240);
  fHistoList.push_back(AllHits2D);
  AllHits3D = new TH3D("AllHits3D", "AllHits3D; X [cm]; Y [cm]; Z [cm]", 300, -150., 150, 120, 120, 240, 100, 160, 260);
  fHistoList.push_back(AllHits3D);

  temporarygraph = new TH2D("temporarygraph", "temporarygraph; X [cm]; Y [cm]; Nof", 41, -105, 105, 17, 125, 210);
  fHistoList.push_back(temporarygraph);
  HitsPerPmtFullPlane =
    new TH1D("HitsPerPmtFullPlane", "Number of hits per PMT. Distribution for all PMTs; Nof photons", 50, -0.5, 49.5);
  fHistoList.push_back(HitsPerPmtFullPlane);
  HitsPerPmtFullMiddle = new TH1D("HitsPerPmtFullMiddle",
                                  "Number of hits per PMT. Distribution for all PMT in "
                                  "y={125#;155}, x={-105#;105}; Nof photons",
                                  50, -0.5, 49.5);
  fHistoList.push_back(HitsPerPmtFullMiddle);

  fitt = new TH2D("fitt", "fitt; x; y", 500, -110, 110, 50, 100, 200);
  fHistoList.push_back(fitt);

  imagehits    = new TGraph(10);
  imageellipse = new TGraph(10);
}
