/** @file CbmHadronAnalysis.cxx
  * @copyright Copyright (C) 2012-2021 PI-UHd, GSI
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors nh, Norbert Herrmann [committer], Florian Uhlig **/

// ------------------------------------------------------------------
// -----                     CbmHadronAnalysis                  -----
// -----              Created 14/12/2012 by nh                  -----
// ------------------------------------------------------------------
#include <iostream>
using namespace std;

#include "CbmCluster.h"
#include "CbmDigiManager.h"
#include "CbmEvent.h"
#include "CbmGlobalTrack.h"
#include "CbmHadron.h"
#include "CbmHadronAnalysis.h"
#include "CbmKFVertex.h"
#include "CbmLink.h"
#include "CbmMCDataArray.h"
#include "CbmMCDataManager.h"
#include "CbmMCTrack.h"
#include "CbmMatch.h"
#include "CbmStsCluster.h"
#include "CbmStsDigi.h"
#include "CbmStsHit.h"
#include "CbmStsKFTrackFitter.h"
#include "CbmStsPoint.h"
#include "CbmStsTrack.h"
#include "CbmTofCell.h"
#include "CbmTofDetectorId_v12b.h"
#include "CbmTofGeoHandler.h"
#include "CbmTofHit.h"
#include "CbmTofPoint.h"
#include "CbmTofTrack.h"
#include "CbmTrdHit.h"
#include "CbmTrdPoint.h"
#include "CbmVertex.h"

#include "FairMCEventHeader.h"
#include "FairMCPoint.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include <Logger.h>

#include "TClonesArray.h"
#include "TFile.h"
#include "TH1.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"
#include "TROOT.h"
#include "TRandom.h"
#include "TString.h"

CbmDigiManager* fDigiMan;   // TOF Input Digis
TClonesArray* fEventsColl;  // CBMEvents (time based)
TClonesArray* fTofTracks;   // CbmTofTrack array
TClonesArray* fTofHitsColl;
TClonesArray* fStsHitsColl;

static Int_t iNbTs = 0;

#define M2PI 0.019479835
#define M2KA 0.24371698
#define M2PROT 0.88035435
#define clight 29.9792
#define MinWallDist 550.

Int_t nMCTracks = 0, nTofPoints = 0, nTofHits = 0;
Int_t nTofTracks   = 0;
Int_t nStsHits     = 0;
Int_t nGlobTracks  = 0;
Int_t NMASS        = 3;
Float_t refMass[3] = {0.139, 0.494, 0.938};

static TH1F* fhTofHitMul;
static TH1F* fhStsHitMul;
const Int_t fiTofHitMulMax = 200;
static TFile* fHist;

//___________________________________________________________________
//
// CbmHadronAnalysis
//
// Task for analysis of hadron spectra
//
// ------------------------------------------------------------------
CbmHadronAnalysis::CbmHadronAnalysis() : CbmHadronAnalysis::CbmHadronAnalysis("Hadron Analysis", 0)
{
  cout << "CbmHadronAnalysis: Task started with defaults" << endl;
}
// ------------------------------------------------------------------

// ------------------------------------------------------------------
CbmHadronAnalysis::CbmHadronAnalysis(const char* name, Int_t verbose)
  : FairTask(name, verbose)
  , fEvents(0)
  , fBeamMomentum(10)
  , fMidY(0.)
  , fDY(0.)
  , fFlowMinPtm(0.)
  , fBSelMin(0.)
  , fBSelMax(0.)
  , fwxy2(0.)
  , fWMax(0.)
  , fVtxBMax(0.)
  , fPdfFileName("")
  , fFlowFileName("")
  , fflowFile(NULL)
  , fMCEventHeader(NULL)
  , fGeoHandler(NULL)
  , fCellInfo(NULL)
  , fMCTracks(NULL)
  , fStsPoints(NULL)
  , fMCTracksColl(NULL)
  , fStsPointsColl(NULL)
  , fStsHits(NULL)
  , fStsClusters(NULL)
  , fStsTracks(NULL)
  , fStsDigis(NULL)
  , fStsDigiMatchColl(NULL)
  , fTrdPoints(NULL)
  , fTrdHits(NULL)
  , fTofPoints(NULL)
  , fTofHits(NULL)
  , fTofDigis(NULL)
  , fTofDigiMatchColl(NULL)
  , fTofDigiMatchPointsColl(NULL)
  , fGlobalTracks(NULL)
  , fHadrons(NULL)
  , fPrimVertex(NULL)
  , fTrackFitter()
  , fa_ptm_rap_gen_pip(NULL)
  , fa_ptm_rap_gen_pim(NULL)
  , fa_ptm_rap_gen_kp(NULL)
  , fa_ptm_rap_gen_km(NULL)
  , fa_ptm_rap_gen_p(NULL)
  , fa_ptm_rap_gen_pbar(NULL)
  , fa_ptm_rap_gen_d(NULL)
  , fa_ptm_rap_gen_t(NULL)
  , fa_ptm_rap_gen_h(NULL)
  , fa_ptm_rap_gen_a(NULL)
  , fa_ptm_rap_gen_imf(NULL)
  , fa_plab_sts_pip(NULL)
  , fa_plab_sts_pim(NULL)
  , fa_plab_sts_kp(NULL)
  , fa_plab_sts_km(NULL)
  , fa_plab_sts_p(NULL)
  , fa_plab_sts_pbar(NULL)
  , fa_ptm_rap_sts_pip(NULL)
  , fa_ptm_rap_sts_pim(NULL)
  , fa_ptm_rap_sts_kp(NULL)
  , fa_ptm_rap_sts_km(NULL)
  , fa_ptm_rap_sts_p(NULL)
  , fa_ptm_rap_sts_pbar(NULL)
  , fa_ptm_rap_sts_d(NULL)
  , fa_ptm_rap_sts_t(NULL)
  , fa_ptm_rap_sts_h(NULL)
  , fa_ptm_rap_sts_a(NULL)
  , fa_ptm_rap_sts_imf(NULL)
  , fa_ptm_rap_poi_pip(NULL)
  , fa_ptm_rap_poi_pim(NULL)
  , fa_ptm_rap_poi_kp(NULL)
  , fa_ptm_rap_poi_km(NULL)
  , fa_ptm_rap_poi_p(NULL)
  , fa_ptm_rap_poi_pbar(NULL)
  , fa_ptm_rap_poi_d(NULL)
  , fa_ptm_rap_poi_t(NULL)
  , fa_ptm_rap_poi_h(NULL)
  , fa_ptm_rap_poi_a(NULL)
  , fa_ptm_rap_poi_imf(NULL)
  , fa_ptm_rap_hit_pip(NULL)
  , fa_ptm_rap_hit_pim(NULL)
  , fa_ptm_rap_hit_kp(NULL)
  , fa_ptm_rap_hit_km(NULL)
  , fa_ptm_rap_hit_p(NULL)
  , fa_ptm_rap_hit_pbar(NULL)
  , fa_ptm_rap_hit_d(NULL)
  , fa_ptm_rap_hit_t(NULL)
  , fa_ptm_rap_hit_h(NULL)
  , fa_ptm_rap_hit_a(NULL)
  , fa_ptm_rap_hit_imf(NULL)
  , fa_ptm_rap_glo_pip(NULL)
  , fa_ptm_rap_glo_pim(NULL)
  , fa_ptm_rap_glo_kp(NULL)
  , fa_ptm_rap_glo_km(NULL)
  , fa_ptm_rap_glo_p(NULL)
  , fa_ptm_rap_glo_pbar(NULL)
  , fa_ptm_rap_glo_d(NULL)
  , fa_ptm_rap_glo_t(NULL)
  , fa_ptm_rap_glo_h(NULL)
  , fa_ptm_rap_glo_a(NULL)
  , fa_ptm_rap_glo_imf(NULL)
  , fa_mul_b_gen(NULL)
  , fa_mul_b_poi(NULL)
  , fa_mul_b_hit(NULL)
  , fa_mul_b_glo(NULL)
  , fa_mul_b_had(NULL)
  , fa_phirp_b_gen(NULL)
  , fa_phgrp_b_gen(NULL)
  , fa_phphrp_gen(NULL)
  , fa_delrp_b_gen(NULL)
  , fa_delrp_b_poi(NULL)
  , fa_delrp_b_hit(NULL)
  , fa_delrp_b_glo(NULL)
  , fa_drp_b_gen(NULL)
  , fa_cdrp_b_gen(NULL)
  , fa_drp_b_poi(NULL)
  , fa_cdrp_b_poi(NULL)
  , fa_drp_b_hit(NULL)
  , fa_cdrp_b_hit(NULL)
  , fa_drp_b_glo(NULL)
  , fa_cdrp_b_glo(NULL)
  , fa_drp_b_had(NULL)
  , fa_cdrp_b_had(NULL)
  , fa_cdelrp_b_gen(NULL)
  , fa_cdelrp_b_poi(NULL)
  , fa_cdelrp_b_hit(NULL)
  , fa_cdelrp_b_glo(NULL)
  , fa_cdelrp_b_had(NULL)
  , fa_phirp_gen(NULL)
  , fa_phirp_poi(NULL)
  , fa_phirp_hit(NULL)
  , fa_phirp_glo(NULL)
  , fa_phirp_had(NULL)
  , fa_phirps_gen(NULL)
  , fa_phirps_poi(NULL)
  , fa_phirps_hit(NULL)
  , fa_phirps_glo(NULL)
  , fa_phirps_had(NULL)
  , fa_v1_rap_gen_pip(NULL)
  , fa_v1_rap_gen_pim(NULL)
  , fa_v1_rap_gen_kp(NULL)
  , fa_v1_rap_gen_km(NULL)
  , fa_v1_rap_gen_p(NULL)
  , fa_v1_rap_gen_pbar(NULL)
  , fa_v1_rap_gen_d(NULL)
  , fa_v1_rap_gen_t(NULL)
  , fa_v1_rap_gen_h(NULL)
  , fa_v1_rap_gen_a(NULL)
  , fa_v1_rap_gen_imf(NULL)
  , fa_v2_rap_gen_pip(NULL)
  , fa_v2_rap_gen_pim(NULL)
  , fa_v2_rap_gen_kp(NULL)
  , fa_v2_rap_gen_km(NULL)
  , fa_v2_rap_gen_p(NULL)
  , fa_v2_rap_gen_pbar(NULL)
  , fa_v2_rap_gen_d(NULL)
  , fa_v2_rap_gen_t(NULL)
  , fa_v2_rap_gen_h(NULL)
  , fa_v2_rap_gen_a(NULL)
  , fa_v2_rap_gen_imf(NULL)
  , fa_v1_rap_poi_pip(NULL)
  , fa_v1_rap_poi_pim(NULL)
  , fa_v1_rap_poi_kp(NULL)
  , fa_v1_rap_poi_km(NULL)
  , fa_v1_rap_poi_p(NULL)
  , fa_v1_rap_poi_pbar(NULL)
  , fa_v1_rap_poi_d(NULL)
  , fa_v1_rap_poi_t(NULL)
  , fa_v1_rap_poi_h(NULL)
  , fa_v1_rap_poi_a(NULL)
  , fa_v1_rap_poi_imf(NULL)
  , fa_v2_rap_poi_pip(NULL)
  , fa_v2_rap_poi_pim(NULL)
  , fa_v2_rap_poi_kp(NULL)
  , fa_v2_rap_poi_km(NULL)
  , fa_v2_rap_poi_p(NULL)
  , fa_v2_rap_poi_pbar(NULL)
  , fa_v2_rap_poi_d(NULL)
  , fa_v2_rap_poi_t(NULL)
  , fa_v2_rap_poi_h(NULL)
  , fa_v2_rap_poi_a(NULL)
  , fa_v2_rap_poi_imf(NULL)
  , fa_v1_rap_hit_pip(NULL)
  , fa_v1_rap_hit_pim(NULL)
  , fa_v1_rap_hit_kp(NULL)
  , fa_v1_rap_hit_km(NULL)
  , fa_v1_rap_hit_p(NULL)
  , fa_v1_rap_hit_pbar(NULL)
  , fa_v1_rap_hit_d(NULL)
  , fa_v1_rap_hit_t(NULL)
  , fa_v1_rap_hit_h(NULL)
  , fa_v1_rap_hit_a(NULL)
  , fa_v1_rap_hit_imf(NULL)
  , fa_v2_rap_hit_pip(NULL)
  , fa_v2_rap_hit_pim(NULL)
  , fa_v2_rap_hit_kp(NULL)
  , fa_v2_rap_hit_km(NULL)
  , fa_v2_rap_hit_p(NULL)
  , fa_v2_rap_hit_pbar(NULL)
  , fa_v2_rap_hit_d(NULL)
  , fa_v2_rap_hit_t(NULL)
  , fa_v2_rap_hit_h(NULL)
  , fa_v2_rap_hit_a(NULL)
  , fa_v2_rap_hit_imf(NULL)
  , fa_v1_rap_glo_pip(NULL)
  , fa_v1_rap_glo_pim(NULL)
  , fa_v1_rap_glo_kp(NULL)
  , fa_v1_rap_glo_km(NULL)
  , fa_v1_rap_glo_p(NULL)
  , fa_v1_rap_glo_pbar(NULL)
  , fa_v1_rap_glo_d(NULL)
  , fa_v1_rap_glo_t(NULL)
  , fa_v1_rap_glo_h(NULL)
  , fa_v1_rap_glo_a(NULL)
  , fa_v1_rap_glo_imf(NULL)
  , fa_v2_rap_glo_pip(NULL)
  , fa_v2_rap_glo_pim(NULL)
  , fa_v2_rap_glo_kp(NULL)
  , fa_v2_rap_glo_km(NULL)
  , fa_v2_rap_glo_p(NULL)
  , fa_v2_rap_glo_pbar(NULL)
  , fa_v2_rap_glo_d(NULL)
  , fa_v2_rap_glo_t(NULL)
  , fa_v2_rap_glo_h(NULL)
  , fa_v2_rap_glo_a(NULL)
  , fa_v2_rap_glo_imf(NULL)
  , fa_xy_poi1(NULL)
  , fa_xy_poi2(NULL)
  , fa_xy_poi3(NULL)
  , fa_xy_hit1(NULL)
  , fa_xy_hit2(NULL)
  , fa_xy_hit3(NULL)
  , fa_xy_glo1(NULL)
  , fa_xy_glo_pip(NULL)
  , fa_xy_glo_pim(NULL)
  , fa_xy_glo_kp(NULL)
  , fa_xy_glo_km(NULL)
  , fa_xy_glo_p(NULL)
  , fa_xy_glo_pbar(NULL)
  , fa_xy_glo_d(NULL)
  , fa_xy_glo_t(NULL)
  , fa_xy_glo_h(NULL)
  , fa_xy_glo_a(NULL)
  , fa_pv_poi(NULL)
  , fa_tm_poi(NULL)
  , fa_tm_poiprim(NULL)
  , fa_dxx(NULL)
  , fa_dxy(NULL)
  , fa_dxz(NULL)
  , fa_dyx(NULL)
  , fa_dyy(NULL)
  , fa_dyz(NULL)
  , fa_dzx(NULL)
  , fa_dzy(NULL)
  , fa_dzz(NULL)
  , fa_hit_ch(NULL)
  , fa_dhit_ch(NULL)
  , fa_tof_hit(NULL)
  , fa_dtof_hit(NULL)
  , fa_tof_hitprim(NULL)
  , fa_pv_hit(NULL)
  , fa_tm_hit(NULL)
  , fa_tm_hitprim(NULL)
  , fa_tn_hit(NULL)
  , fa_t0_hit(NULL)
  , fa_t0m_hit(NULL)
  , fa_t0mn_hit(NULL)
  , fa_t0m_b_hit(NULL)
  , fa_t0mn_b_hit(NULL)
  , fa_t0m_f_hit(NULL)
  , fa_t0mn_f_hit(NULL)
  , fa_t0m_f_b_hit(NULL)
  , fa_t0mn_f_b_hit(NULL)
  , fa_t0m_nf_hit(NULL)
  , fa_t0mn_nf_hit(NULL)
  , fa_t0m_nf_b_hit(NULL)
  , fa_t0mn_nf_b_hit(NULL)
  , fa_TofTrackMul(NULL)
  , fa_VtxB(NULL)
  , fa_chi2_mom_glo(NULL)
  , fa_chi2_mom_gloprim(NULL)
  , fa_len_mom_glo(NULL)
  , fa_pv_glo(NULL)
  , fa_tm_glo(NULL)
  , fa_tm_glo_pip(NULL)
  , fa_tm_glo_pim(NULL)
  , fa_tm_glo_kp(NULL)
  , fa_tm_glo_km(NULL)
  , fa_tm_glo_p(NULL)
  , fa_tm_glo_pbar(NULL)
  , fa_tm_glo_d(NULL)
  , fa_tm_glo_t(NULL)
  , fa_tm_glo_h(NULL)
  , fa_tm_glo_a(NULL)
  , fa_tm_gloprim(NULL)
  , fa_tm_glomis(NULL)
  , fa_tm_glovtxb(NULL)
  , fa_tm_gloprimvtxb(NULL)
  , fa_m2mom_glo(NULL)
  , fa_m2mom_glovtxb(NULL)
  , fa_m2mom_gloprim(NULL)
  , fa_m2mom_gloprimvtxb(NULL)
  , fa_m2mom_glo_pip(NULL)
  , fa_m2mom_glo_pim(NULL)
  , fa_m2mom_glo_kp(NULL)
  , fa_m2mom_glo_km(NULL)
  , fa_m2mom_glo_p(NULL)
  , fa_m2mom_glo_pbar(NULL)
  , fa_m2mom_glo_d(NULL)
  , fa_m2mom_glo_t(NULL)
  , fa_m2mom_glo_h(NULL)
  , fa_m2mom_glo_a(NULL)
  , fa_pMCmom_glo(NULL)
  , fa_pMCmom_glo_pip(NULL)
  , fa_pMCmom_glo_pim(NULL)
  , fa_pMCmom_glo_kp(NULL)
  , fa_pMCmom_glo_km(NULL)
  , fa_pMCmom_glo_p(NULL)
  , fa_pMCmom_glo_pbar(NULL)
  , fa_pMCmom_glo_d(NULL)
  , fa_pMCmom_glo_t(NULL)
  , fa_pMCmom_glo_h(NULL)
  , fa_pMCmom_glo_a(NULL)
  , fa_w_mom_glo(NULL)
  , fa_w_mom_glo_pip(NULL)
  , fa_w_mom_glo_pim(NULL)
  , fa_w_mom_glo_kp(NULL)
  , fa_w_mom_glo_km(NULL)
  , fa_w_mom_glo_p(NULL)
  , fa_w_mom_glo_pbar(NULL)
  , fa_w_mom_glo_d(NULL)
  , fa_w_mom_glo_t(NULL)
  , fa_w_mom_glo_h(NULL)
  , fa_w_mom_glo_a(NULL)
  , fa_w_mom_gloprim(NULL)
  , fa_w_mom_glomis(NULL)
  , fa_LenDismom_glo(NULL)
  , fa_LenDismom_glo_pip(NULL)
  , fa_LenDismom_glo_pim(NULL)
  , fa_LenDismom_glo_kp(NULL)
  , fa_LenDismom_glo_km(NULL)
  , fa_LenDismom_glo_p(NULL)
  , fa_LenDismom_glo_pbar(NULL)
  , fa_LenDismom_glo_d(NULL)
  , fa_LenDismom_glo_t(NULL)
  , fa_LenDismom_glo_h(NULL)
  , fa_LenDismom_glo_a(NULL)
  , fa_LenMcLenGlomom_glo(NULL)
  , fa_LenMcDismom_glo(NULL)
  , fhwdist(NULL)
  , fhwmindelmass(NULL)
  , fhwminlen(NULL)
  , fhwdelp(NULL)
  , fhTofTrkDx(NULL)
  , fhTofTrkDy(NULL)
  , fhTofTrkDxsel(NULL)
  , fhTofTrkDysel(NULL)
  , bRecSec(kFALSE)
  , fdDistPrimLim(1.5)
  ,  // Ext Parameter: Max Tof-Sts trans distance for primaries
  fdDistPrimLim2(0.3)
  ,  // Ext Parameter: Max Sts-Sts trans distance for primaries
  fdDistSecLim2(0.5)
  ,  // Ext Parameter: Max Sts-Sts trans distance from TOF direction for secondaries
  fdD0ProtLim(0.4)
  ,  // Ext Parameter: Min impact parameter for secondary proton
  fdOpAngMin(0.01)
  ,  // Ext Parameter: Min opening angle for accepting pair
  fdDCALim(0.2)
  ,  // Ext Parameter: Max DCA for accepting pair
  fdVLenMin(5.)
  ,  // Ext Parameter: Min Lambda flight path length for accepting pair
  fdVLenMax(25.)
  ,  // Ext Parameter: Max Lambda flight path length for accepting pair
  fdDistTRD(10.)
  ,  // max accepted distance of Trd Hit from STS-TOF line
  fdTRDHmulMin(0.)
  ,  // min associated Trd Hits to Track candidates
  fNMixedEvents(1)
{
}
// ------------------------------------------------------------------


// ------------------------------------------------------------------
CbmHadronAnalysis::~CbmHadronAnalysis()
{
  // Destructor
  fPrimVertex = NULL;
}
// ------------------------------------------------------------------


// ------------------------------------------------------------------
void CbmHadronAnalysis::CreateHistogramms()
{
  // Create histogramms
  // gROOT->cd();
  FairRunAna* fRun = FairRunAna::Instance();
  fHist            = fRun->GetOutputFile();
  LOG(info) << "CreateHistograms in " << fHist->GetName();
  // gSystem->cd(fHist->GetName());
  fHist->ReOpen("Update");

  fhTofHitMul = new TH1F(Form("hTofHitMul"), Form("TofHit Multiplicity; M_{TofHit} "), fiTofHitMulMax, 0.,
                         (Double_t) fiTofHitMulMax);
  fhStsHitMul = new TH1F(Form("hStsHitMul"), Form("StsHit Multiplicity; M_{StsHit} "), fiTofHitMulMax, 0.,
                         (Double_t) fiTofHitMulMax);

  Float_t ymin   = -1.;
  Float_t ymax   = 4.;
  Float_t ptmmax = 2.5;
  Int_t ptm_nbx  = 30;
  Int_t ptm_nby  = 30;

  // generator level

  fa_ptm_rap_gen_pip =
    new TH2F("ptm_rap_gen_pip", "MCTrack-gen pi-plus; y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_gen_pim =
    new TH2F("ptm_rap_gen_pim", "MCTrack-gen pi-minus;y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_gen_kp =
    new TH2F("ptm_rap_gen_kp", "MCTrack-gen k-plus;  y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_gen_km =
    new TH2F("ptm_rap_gen_km", "MCTrack-gen k-minus; y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_gen_p =
    new TH2F("ptm_rap_gen_p", "MCTrack-gen proton;  y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_gen_pbar =
    new TH2F("ptm_rap_gen_pbar", "MCTrack-gen antiproton;y;p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_gen_d =
    new TH2F("ptm_rap_gen_d", "MCTrack-gen deuteron;y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_gen_t =
    new TH2F("ptm_rap_gen_t", "MCTrack-gen triton;  y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_gen_h =
    new TH2F("ptm_rap_gen_h", "MCTrack-gen 3he;     y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_gen_a =
    new TH2F("ptm_rap_gen_a", "MCTrack-gen alpha;   y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_gen_imf =
    new TH2F("ptm_rap_gen_imf", "MCTrack-gen imf;     y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);

  Float_t v1_nbx = 20.;
  Float_t v1_nby = 20.;
  Float_t yvmax  = 1.3;

  fa_v1_rap_gen_pip =
    new TH2F("v1_rap_gen_pip", "MCTrack-gen pi-plus; y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_gen_pim =
    new TH2F("v1_rap_gen_pim", "MCTrack-gen pi-minus;y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_gen_kp = new TH2F("v1_rap_gen_kp", "MCTrack-gen k-plus;  y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_gen_km = new TH2F("v1_rap_gen_km", "MCTrack-gen k-minus; y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_gen_p  = new TH2F("v1_rap_gen_p", "MCTrack-gen proton;  y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_gen_pbar =
    new TH2F("v1_rap_gen_pbar", "MCTrack-gen antiproton;y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_gen_d = new TH2F("v1_rap_gen_d", "MCTrack-gen deuteron;y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_gen_t = new TH2F("v1_rap_gen_t", "MCTrack-gen triton;  y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_gen_h = new TH2F("v1_rap_gen_h", "MCTrack-gen 3he;     y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_gen_a = new TH2F("v1_rap_gen_a", "MCTrack-gen alpha;   y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_gen_imf =
    new TH2F("v1_rap_gen_imf", "MCTrack-gen imf;     y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);

  fa_v2_rap_gen_pip =
    new TH2F("v2_rap_gen_pip", "MCTrack-gen pi-plus; y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_gen_pim =
    new TH2F("v2_rap_gen_pim", "MCTrack-gen pi-minus;y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_gen_kp = new TH2F("v2_rap_gen_kp", "MCTrack-gen k-plus;  y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_gen_km = new TH2F("v2_rap_gen_km", "MCTrack-gen k-minus; y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_gen_p  = new TH2F("v2_rap_gen_p", "MCTrack-gen proton;  y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_gen_pbar =
    new TH2F("v2_rap_gen_pbar", "MCTrack-gen antiproton;y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_gen_d = new TH2F("v2_rap_gen_d", "MCTrack-gen deuteron;y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_gen_t = new TH2F("v2_rap_gen_t", "MCTrack-gen triton;  y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_gen_h = new TH2F("v2_rap_gen_h", "MCTrack-gen 3he;     y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_gen_a = new TH2F("v2_rap_gen_a", "MCTrack-gen alpha;   y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_gen_imf =
    new TH2F("v2_rap_gen_imf", "MCTrack-gen imf;     y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);

  // TofPoint level

  fa_ptm_rap_poi_pip =
    new TH2F("ptm_rap_poi_pip", "MCTrack-poi pi-plus; y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_poi_pim =
    new TH2F("ptm_rap_poi_pim", "MCTrack-poi pi-minus;y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_poi_kp =
    new TH2F("ptm_rap_poi_kp", "MCTrack-poi k-plus;  y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_poi_km =
    new TH2F("ptm_rap_poi_km", "MCTrack-poi k-minus; y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_poi_p =
    new TH2F("ptm_rap_poi_p", "MCTrack-poi proton;  y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_poi_pbar =
    new TH2F("ptm_rap_poi_pbar", "MCTrack-poi antiproton;y;p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_poi_d =
    new TH2F("ptm_rap_poi_d", "MCTrack-poi deuteron;y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_poi_t =
    new TH2F("ptm_rap_poi_t", "MCTrack-poi triton;  y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_poi_h =
    new TH2F("ptm_rap_poi_h", "MCTrack-poi 3he;     y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_poi_a =
    new TH2F("ptm_rap_poi_a", "MCTrack-poi alpha;   y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_poi_imf =
    new TH2F("ptm_rap_poi_imf", "MCTrack-poi imf;    y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);

  fa_v1_rap_poi_pip =
    new TH2F("v1_rap_poi_pip", "MCTrack-poi pi-plus; y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_poi_pim =
    new TH2F("v1_rap_poi_pim", "MCTrack-poi pi-minus;y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_poi_kp = new TH2F("v1_rap_poi_kp", "MCTrack-poi k-plus;  y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_poi_km = new TH2F("v1_rap_poi_km", "MCTrack-poi k-minus; y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_poi_p  = new TH2F("v1_rap_poi_p", "MCTrack-poi proton;  y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_poi_pbar =
    new TH2F("v1_rap_poi_pbar", "MCTrack-poi antiproton;y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_poi_d = new TH2F("v1_rap_poi_d", "MCTrack-poi deuteron;y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_poi_t = new TH2F("v1_rap_poi_t", "MCTrack-poi triton;  y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_poi_h = new TH2F("v1_rap_poi_h", "MCTrack-poi 3he;     y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_poi_a = new TH2F("v1_rap_poi_a", "MCTrack-poi alpha;   y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_poi_imf =
    new TH2F("v1_rap_poi_imf", "MCTrack-poi imf;     y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);

  fa_v2_rap_poi_pip =
    new TH2F("v2_rap_poi_pip", "MCTrack-poi pi-plus; y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_poi_pim =
    new TH2F("v2_rap_poi_pim", "MCTrack-poi pi-minus;y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_poi_kp = new TH2F("v2_rap_poi_kp", "MCTrack-poi k-plus;  y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_poi_km = new TH2F("v2_rap_poi_km", "MCTrack-poi k-minus; y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_poi_p  = new TH2F("v2_rap_poi_p", "MCTrack-poi proton;  y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_poi_pbar =
    new TH2F("v2_rap_poi_pbar", "MCTrack-poi antiproton;y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_poi_d = new TH2F("v2_rap_poi_d", "MCTrack-poi deuteron;y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_poi_t = new TH2F("v2_rap_poi_t", "MCTrack-poi triton;  y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_poi_h = new TH2F("v2_rap_poi_h", "MCTrack-poi 3he;     y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_poi_a = new TH2F("v2_rap_poi_a", "MCTrack-poi alpha;   y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_poi_imf =
    new TH2F("v2_rap_poi_imf", "MCTrack-poi imf;     y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);

  // TofHit level

  fa_ptm_rap_hit_pip =
    new TH2F("ptm_rap_hit_pip", "MCTrack-hit pi-plus; y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_hit_pim =
    new TH2F("ptm_rap_hit_pim", "MCTrack-hit pi-minus;y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_hit_kp =
    new TH2F("ptm_rap_hit_kp", "MCTrack-hit k-plus;  y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_hit_km =
    new TH2F("ptm_rap_hit_km", "MCTrack-hit k-minus; y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_hit_p =
    new TH2F("ptm_rap_hit_p", "MCTrack-hit proton;  y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_hit_pbar =
    new TH2F("ptm_rap_hit_pbar", "MCTrack-hit antiproton;y;p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_hit_d =
    new TH2F("ptm_rap_hit_d", "MCTrack-hit deuteron;y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_hit_t =
    new TH2F("ptm_rap_hit_t", "MCTrack-hit triton;  y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_hit_h =
    new TH2F("ptm_rap_hit_h", "MCTrack-hit 3he;     y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_hit_a =
    new TH2F("ptm_rap_hit_a", "MCTrack-hit alpha;   y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_hit_imf =
    new TH2F("ptm_rap_hit_imf", "MCTrack-hit imf;     y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);

  fa_v1_rap_hit_pip =
    new TH2F("v1_rap_hit_pip", "MCTrack-hit pi-plus; y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_hit_pim =
    new TH2F("v1_rap_hit_pim", "MCTrack-hit pi-minus;y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_hit_kp = new TH2F("v1_rap_hit_kp", "MCTrack-hit k-plus;  y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_hit_km = new TH2F("v1_rap_hit_km", "MCTrack-hit k-minus; y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_hit_p  = new TH2F("v1_rap_hit_p", "MCTrack-hit proton;  y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_hit_pbar =
    new TH2F("v1_rap_hit_pbar", "MCTrack-hit antiproton;y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_hit_d = new TH2F("v1_rap_hit_d", "MCTrack-hit deuteron;y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_hit_t = new TH2F("v1_rap_hit_t", "MCTrack-hit triton;  y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_hit_h = new TH2F("v1_rap_hit_h", "MCTrack-hit 3he;     y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_hit_a = new TH2F("v1_rap_hit_a", "MCTrack-hit alpha;   y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_hit_imf =
    new TH2F("v1_rap_hit_imf", "MCTrack-hit imf;     y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);

  fa_v2_rap_hit_pip =
    new TH2F("v2_rap_hit_pip", "MCTrack-hit pi-plus; y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_hit_pim =
    new TH2F("v2_rap_hit_pim", "MCTrack-hit pi-minus;y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_hit_kp = new TH2F("v2_rap_hit_kp", "MCTrack-hit k-plus;  y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_hit_km = new TH2F("v2_rap_hit_km", "MCTrack-hit k-minus; y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_hit_p  = new TH2F("v2_rap_hit_p", "MCTrack-hit proton;  y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_hit_pbar =
    new TH2F("v2_rap_hit_pbar", "MCTrack-hit antiproton;y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_hit_d = new TH2F("v2_rap_hit_d", "MCTrack-hit deuteron;y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_hit_t = new TH2F("v2_rap_hit_t", "MCTrack-hit triton;  y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_hit_h = new TH2F("v2_rap_hit_h", "MCTrack-hit 3he;     y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_hit_a = new TH2F("v2_rap_hit_a", "MCTrack-hit alpha;   y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_hit_imf =
    new TH2F("v2_rap_hit_imf", "MCTrack-hit imf;     y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);

  // GlobalTrack level

  fa_plab_sts_pip  = new TH1F("plab_sts_pip", "MCTrack-sts pi-plus; p_{Lab}(GeV/c)", 100, 0., 10.);
  fa_plab_sts_pim  = new TH1F("plab_sts_pim", "MCTrack-sts pi-minus; p_{Lab}(GeV/c)", 100, 0., 10.);
  fa_plab_sts_kp   = new TH1F("plab_sts_kp", "MCTrack-sts k-plus; p_{Lab}(GeV/c)", 100, 0., 10.);
  fa_plab_sts_km   = new TH1F("plab_sts_km", "MCTrack-sts k-minus; p_{Lab}(GeV/c)", 100, 0., 10.);
  fa_plab_sts_p    = new TH1F("plab_sts_p", "MCTrack-sts proton; p_{Lab}(GeV/c)", 100, 0., 10.);
  fa_plab_sts_pbar = new TH1F("plab_sts_pbar", "MCTrack-sts pbar; p_{Lab}(GeV/c)", 100, 0., 10.);

  fa_ptm_rap_sts_pip =
    new TH2F("ptm_rap_sts_pip", "MCTrack-sts pi-plus; y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_sts_pim =
    new TH2F("ptm_rap_sts_pim", "MCTrack-sts pi-minus;y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_sts_kp =
    new TH2F("ptm_rap_sts_kp", "MCTrack-sts k-plus;  y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_sts_km =
    new TH2F("ptm_rap_sts_km", "MCTrack-sts k-minus; y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_sts_p =
    new TH2F("ptm_rap_sts_p", "MCTrack-sts proton;  y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_sts_pbar =
    new TH2F("ptm_rap_sts_pbar", "MCTrack-sts antiproton;y;p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_sts_d =
    new TH2F("ptm_rap_sts_d", "MCTrack-sts deuteron;y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_sts_t =
    new TH2F("ptm_rap_sts_t", "MCTrack-sts triton;  y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_sts_h =
    new TH2F("ptm_rap_sts_h", "MCTrack-sts 3he;     y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_sts_a =
    new TH2F("ptm_rap_sts_a", "MCTrack-sts alpha;   y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_sts_imf =
    new TH2F("ptm_rap_sts_imf", "MCTrack-sts imf;     y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);

  fa_ptm_rap_glo_pip =
    new TH2F("ptm_rap_glo_pip", "MCTrack-glo pi-plus; y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_glo_pim =
    new TH2F("ptm_rap_glo_pim", "MCTrack-glo pi-minus;y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_glo_kp =
    new TH2F("ptm_rap_glo_kp", "MCTrack-glo k-plus;  y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_glo_km =
    new TH2F("ptm_rap_glo_km", "MCTrack-glo k-minus; y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_glo_p =
    new TH2F("ptm_rap_glo_p", "MCTrack-glo proton;  y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_glo_pbar =
    new TH2F("ptm_rap_glo_pbar", "MCTrack-glo antiproton;y;p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_glo_d =
    new TH2F("ptm_rap_glo_d", "MCTrack-glo deuteron;y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_glo_t =
    new TH2F("ptm_rap_glo_t", "MCTrack-glo triton;  y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_glo_h =
    new TH2F("ptm_rap_glo_h", "MCTrack-glo 3he;     y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_glo_a =
    new TH2F("ptm_rap_glo_a", "MCTrack-glo alpha;   y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
  fa_ptm_rap_glo_imf =
    new TH2F("ptm_rap_glo_imf", "MCTrack-glo imf;     y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);

  fa_v1_rap_glo_pip =
    new TH2F("v1_rap_glo_pip", "MCTrack-glo pi-plus; y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_glo_pim =
    new TH2F("v1_rap_glo_pim", "MCTrack-glo pi-minus;y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_glo_kp = new TH2F("v1_rap_glo_kp", "MCTrack-glo k-plus;  y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_glo_km = new TH2F("v1_rap_glo_km", "MCTrack-glo k-minus; y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_glo_p  = new TH2F("v1_rap_glo_p", "MCTrack-glo proton;  y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_glo_pbar =
    new TH2F("v1_rap_glo_pbar", "MCTrack-glo antiproton;y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_glo_d = new TH2F("v1_rap_glo_d", "MCTrack-glo deuteron;y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_glo_t = new TH2F("v1_rap_glo_t", "MCTrack-glo triton;  y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_glo_h = new TH2F("v1_rap_glo_h", "MCTrack-glo 3he;     y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_glo_a = new TH2F("v1_rap_glo_a", "MCTrack-glo alpha;   y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v1_rap_glo_imf =
    new TH2F("v1_rap_glo_imf", "MCTrack-glo imf;     y; v_{1}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);

  fa_v2_rap_glo_pip =
    new TH2F("v2_rap_glo_pip", "MCTrack-glo pi-plus; y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_glo_pim =
    new TH2F("v2_rap_glo_pim", "MCTrack-glo pi-minus;y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_glo_kp = new TH2F("v2_rap_glo_kp", "MCTrack-glo k-plus;  y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_glo_km = new TH2F("v2_rap_glo_km", "MCTrack-glo k-minus; y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_glo_p  = new TH2F("v2_rap_glo_p", "MCTrack-glo proton;  y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_glo_pbar =
    new TH2F("v2_rap_glo_pbar", "MCTrack-glo antiproton;y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_glo_d = new TH2F("v2_rap_glo_d", "MCTrack-glo deuteron;y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_glo_t = new TH2F("v2_rap_glo_t", "MCTrack-glo triton;  y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_glo_h = new TH2F("v2_rap_glo_h", "MCTrack-glo 3he;     y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_glo_a = new TH2F("v2_rap_glo_a", "MCTrack-glo alpha;   y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);
  fa_v2_rap_glo_imf =
    new TH2F("v2_rap_glo_imf", "MCTrack-glo imf;     y; v_{2}", v1_nbx, -yvmax, yvmax, v1_nby, -1., 1.);

  // xy - hit densities and rates
  Int_t nbinx    = 200;
  Int_t nbiny    = 200;
  Float_t xrange = 750.;
  Float_t yrange = 500.;
  fwxy2          = nbinx * nbiny / 4. / xrange / yrange;

  fa_xy_poi1 = new TH2F("xy_poi1", "TofPoint; ;", nbinx, -xrange, xrange, nbiny, -yrange, yrange);
  fa_xy_poi2 = new TH2F("xy_poi2", "TofPoint /cm^{2}; ;", nbinx, -xrange, xrange, nbiny, -yrange, yrange);
  fa_xy_poi3 = new TH2F("xy_poi3", "TofPoint /cm^{2}/s; ;", nbinx, -xrange, xrange, nbiny, -yrange, yrange);

  fa_pv_poi = new TH2F("pv_poi", "TofPoint(all); velocity;momentum;", 100, 0., 32., 100., 0., 5.);
  fa_tm_poi =
    new TH2F("tm_poi", "Tofpoi(all); momentum (GeV/c); M_{ToF}*sign(Z) (GeV/c^{2});", 100, 0., 10., 200., -1.5, 2.5);
  fa_tm_poiprim = new TH2F("tm_poiprim", "Tofpoi(primary); momentum; Tofmass", 100, 0., 10., 200., -1.5, 2.5);

  fa_xy_hit1 = new TH2F("xy_hit1", "TofHit; ;", nbinx, -xrange, xrange, nbiny, -yrange, yrange);
  fa_xy_hit2 = new TH2F("xy_hit2", "TofHit /cm^{2}; ;", nbinx, -xrange, xrange, nbiny, -yrange, yrange);
  fa_xy_hit3 = new TH2F("xy_hit3", "TofHit /cm^{2}/s; ;", nbinx, -xrange, xrange, nbiny, -yrange, yrange);


  fa_tof_hit     = new TH1F("tof_hit", "TofHit(all); t (ns); counts;", 100, 10., 110.);
  fa_dtof_hit    = new TH1F("dtof_hit", "TofHit(all); #Deltat (ns); counts;", 100, -1., 1.);
  fa_tof_hitprim = new TH1F("tof_hitprim", "TofHit(prim); t (ns); counts;", 100, 10., 110.);
  fa_pv_hit      = new TH2F("pv_hit", "TofHit(all); velocity; momentum;", 100, 0., 32., 100., 0., 5.);
  fa_tm_hit =
    new TH2F("tm_hit", "TofHit(all); momentum (GeV/c); M_{ToF}*sign(Z) (GeV/c^{2});", 100, 0., 10., 200., -1.5, 2.5);
  fa_tm_hitprim = new TH2F("tm_hitprim", "TofHit(primary); momentum; Tofmass", 100, 0., 10., 200., -1.5, 2.5);
  fa_tn_hit     = new TH1F("tn_hit", "fastest TofHit(all); t (ns); counts;", 100, -1.0, 1.);
  fa_t0_hit     = new TH1F("t0_hit", "time zero; t0 (ns) ; counts;", 100, -0.5, 0.5);
  fa_t0m_hit    = new TH1F("t0m_hit", "average time zero; t0 (ns) ; counts;", 100, -0.1, 0.1);
  fa_t0mn_hit   = new TH1F("t0mn_hit", "average time zero hits; number of hits ; counts;", 100, 0., 500.);
  fa_t0m_b_hit  = new TH2F("t0m_b_hit", "average time zero; b (fm); t0 (ns) ; counts;", 28, 0., 14., 500, -0.3, 0.7);
  fa_t0mn_b_hit =
    new TH2F("t0mn_b_hit", "average time zero hits; b(fm); number of hits ; counts;", 28, 0., 14., 100, 0., 500.);

  fa_t0m_f_hit  = new TH1F("t0m_f_hit", "average time zero forward; t0 (ns) ; counts;", 100, -0.1, 0.1);
  fa_t0mn_f_hit = new TH1F("t0mn_f_hit", "average time zero hits forward; number of hits ; counts;", 100, 0., 500.);
  fa_t0m_f_b_hit =
    new TH2F("t0m_f_b_hit", "average time zero forward; b (fm); t0 (ns) ; counts;", 28, 0., 14., 500, -0.3, 0.7);
  fa_t0mn_f_b_hit = new TH2F("t0mn_f_b_hit", "average time zero hits forward ; b(fm); number of hits ; counts;", 28, 0.,
                             14., 50, 0., 50.);

  fa_t0m_nf_hit  = new TH1F("t0m_nf_hit", "average time zero noforward; t0 (ns) ; counts;", 100, -0.1, 0.1);
  fa_t0mn_nf_hit = new TH1F("t0mn_nf_hit", "average time zero hits noforward; number of hits ; counts;", 100, 0., 500.);
  fa_t0m_nf_b_hit =
    new TH2F("t0m_nf_b_hit", "average time zero noforward; b (fm); t0 (ns) ; counts;", 28, 0., 14., 500, -0.3, 0.7);
  fa_t0mn_nf_b_hit = new TH2F("t0mn_nf_b_hit", "average time zero hits noforward ; b(fm); number of hits ; counts;", 28,
                              0., 14., 100, 0., 500.);

  fa_dxx = new TH2F("dxx", "TofHit; x; Delta x;", 100, -xrange, xrange, 50., -10., 10.);
  fa_dxy = new TH2F("dxy", "TofHit; y; Delta x;", 100, -yrange, yrange, 50., -10., 10.);
  fa_dxz = new TH2F("dxz", "TofHit; z; Delta x;", 100, 400., 650., 50., -10., 10.);
  fa_dyx = new TH2F("dyx", "TofHit; x; Delta y;", 100, -xrange, xrange, 50., -10., 10.);
  fa_dyy = new TH2F("dyy", "TofHit; y; Delta y;", 100, -yrange, yrange, 50., -10., 10.);
  fa_dyz = new TH2F("dyz", "TofHit; z; Delta y;", 100, 400., 650., 50., -10., 10.);
  fa_dzx = new TH2F("dzx", "TofHit; x; Delta z;", 100, -xrange, xrange, 50, -20., 20.);
  fa_dzy = new TH2F("dzy", "TofHit; y; Delta z;", 100, -yrange, yrange, 50, -20., 20.);
  fa_dzz = new TH2F("dzz", "TofHit; z; Delta z;", 100, 400., 650., 50, -20., 20.);

  fa_hit_ch  = new TH1F("hit_ch", "TofHits; channel; rate (Hz/s);", 50000, 0., 50000.);
  fa_dhit_ch = new TH2F("dhit_ch", "Tof Double Hits; channel; counts;", 50000, 0., 50000., 2, 0.5, 2.5);

  fa_xy_glo1 = new TH2F("xy_glo1", "GlobalTrack(all); x (cm); y(cm);", nbinx, -xrange, xrange, nbiny, -yrange, yrange);
  fa_xy_glo_pip =
    new TH2F("xy_glo_pip", "GlobalTrack(pip); x (cm); y(cm);", nbinx, -xrange, xrange, nbiny, -yrange, yrange);
  fa_xy_glo_pim =
    new TH2F("xy_glo_pim", "GlobalTrack(pim); x (cm); y(cm);", nbinx, -xrange, xrange, nbiny, -yrange, yrange);
  fa_xy_glo_p = new TH2F("xy_glo_p", "GlobalTrack(p); x (cm); y(cm);", nbinx, -xrange, xrange, nbiny, -yrange, yrange);
  fa_xy_glo_pbar =
    new TH2F("xy_glo_pbar", "GlobalTrack(pbar); x (cm); y(cm);", nbinx, -xrange, xrange, nbiny, -yrange, yrange);
  fa_xy_glo_kp =
    new TH2F("xy_glo_kp", "GlobalTrack(kp); x (cm); y(cm);", nbinx, -xrange, xrange, nbiny, -yrange, yrange);
  fa_xy_glo_km =
    new TH2F("xy_glo_km", "GlobalTrack(km); x (cm); y(cm);", nbinx, -xrange, xrange, nbiny, -yrange, yrange);
  fa_xy_glo_d = new TH2F("xy_glo_d", "GlobalTrack(d); x (cm); y(cm);", nbinx, -xrange, xrange, nbiny, -yrange, yrange);
  fa_xy_glo_t = new TH2F("xy_glo_t", "GlobalTrack(t); x (cm); y(cm);", nbinx, -xrange, xrange, nbiny, -yrange, yrange);
  fa_xy_glo_h = new TH2F("xy_glo_h", "GlobalTrack(h); x (cm); y(cm);", nbinx, -xrange, xrange, nbiny, -yrange, yrange);
  fa_xy_glo_a = new TH2F("xy_glo_a", "GlobalTrack(a); x (cm); y(cm);", nbinx, -xrange, xrange, nbiny, -yrange, yrange);

  fa_TofTrackMul = new TH1F("TofTrackMul", "number of assigned TofTrack / global track; multiplicity; ", 50, 0., 50.);
  fa_VtxB        = new TH1F("VtxB", "Chi2 to primary vertex; ", 100, 0., 20.);

  Float_t TMMIN   = -1.5;
  Float_t TMMAX   = 2.5;
  Int_t TMYBIN    = 400;
  fa_pv_glo       = new TH2F("pv_glo", "GlobalTrack(all); velocity; momentum;", 100, 0., 32., 100., 0., 5.);
  fa_tm_glo       = new TH2F("tm_glo", "GlobalTrack(all); momentum (GeV/c); M_{ToF}*sign(Z) (GeV/c^{2});", 100, 0., 10.,
                       TMYBIN, TMMIN, TMMAX);
  fa_chi2_mom_glo = new TH2F("chi2_mom_glo", "GlobalTrack(all); momentum; chi2;", 100, 0., 10., 100., 0., 50.);
  fa_chi2_mom_gloprim =
    new TH2F("chi2_mom_gloprim", "GlobalTrack(primaries); momentum; chi2;", 100, 0., 10., 100., 0., 50.);
  fa_len_mom_glo = new TH2F("len_mom_glo", "GlobalTrack(all); momentum; len;", 100, 0., 10., 100., 0., 1500.);
  fa_tm_gloprim = new TH2F("tm_gloprim", "GlobalTrack(primary); momentum; Tofmass", 100, 0., 10., TMYBIN, TMMIN, TMMAX);
  fa_tm_glovtxb = new TH2F("tm_glovtxb", "GlobalTrack(vtxb); momentum (GeV/c); M_{ToF}*sign(Z) (GeV/c^{2});", 100, 0.,
                           10., TMYBIN, TMMIN, TMMAX);
  fa_tm_gloprimvtxb =
    new TH2F("tm_gloprimvtxb", "GlobalTrack(prim,vtxb); momentum; Tofmass", 100, 0., 10., TMYBIN, TMMIN, TMMAX);
  fa_tm_glomis = new TH2F("tm_glomis", "GlobalTrack(mis); momentum (GeV/c); M_{ToF}*sign(Z) (GeV/c^{2});", 100, 0., 10.,
                          TMYBIN, TMMIN, TMMAX);
  fa_tm_glo_pip = new TH2F("tm_glo_pip", "GlobalTrack(pip); momentum (GeV/c); M_{ToF}*sign(Z) (GeV/c^{2});", 100, 0.,
                           10., TMYBIN, TMMIN, TMMAX);
  fa_tm_glo_pim = new TH2F("tm_glo_pim", "GlobalTrack(pim); momentum (GeV/c); M_{ToF}*sign(Z) (GeV/c^{2});", 100, 0.,
                           10., TMYBIN, TMMIN, TMMAX);
  fa_tm_glo_kp  = new TH2F("tm_glo_kp", "GlobalTrack(kp); momentum (GeV/c); M_{ToF}*sign(Z) (GeV/c^{2});", 100, 0., 10.,
                          TMYBIN, TMMIN, TMMAX);
  fa_tm_glo_km  = new TH2F("tm_glo_km", "GlobalTrack(km); momentum (GeV/c); M_{ToF}*sign(Z) (GeV/c^{2});", 100, 0., 10.,
                          TMYBIN, TMMIN, TMMAX);
  fa_tm_glo_p   = new TH2F("tm_glo_p", "GlobalTrack(p); momentum (GeV/c); M_{ToF}*sign(Z) (GeV/c^{2});", 100, 0., 10.,
                         TMYBIN, TMMIN, TMMAX);
  fa_tm_glo_pbar = new TH2F("tm_glo_pbar", "GlobalTrack(pbar); momentum (GeV/c); M_{ToF}*sign(Z) (GeV/c^{2});", 100, 0.,
                            10., TMYBIN, TMMIN, TMMAX);
  fa_tm_glo_d    = new TH2F("tm_glo_d", "GlobalTrack(d); momentum (GeV/c); M_{ToF}*sign(Z) (GeV/c^{2});", 100, 0., 10.,
                         TMYBIN, TMMIN, TMMAX);
  fa_tm_glo_t    = new TH2F("tm_glo_t", "GlobalTrack(t); momentum (GeV/c); M_{ToF}*sign(Z) (GeV/c^{2});", 100, 0., 10.,
                         TMYBIN, TMMIN, TMMAX);
  fa_tm_glo_h    = new TH2F("tm_glo_h", "GlobalTrack(h); momentum (GeV/c); M_{ToF}*sign(Z) (GeV/c^{2});", 100, 0., 10.,
                         TMYBIN, TMMIN, TMMAX);
  fa_tm_glo_a    = new TH2F("tm_glo_a", "GlobalTrack(a); momentum (GeV/c); M_{ToF}*sign(Z) (GeV/c^{2});", 100, 0., 10.,
                         TMYBIN, TMMIN, TMMAX);

  Double_t M2MIN = -0.4;
  Double_t M2MAX = 1.4;
  Int_t M2YBIN   = 360;
  fa_m2mom_glo = new TH2F("m2mom_glo", "GlobalTrack(all); p ^{.} sign(Z)(GeV); M_{ToF}^{2} (GeV^{2});", 200, -10., 10.,
                          M2YBIN, M2MIN, M2MAX);
  fa_m2mom_glovtxb = new TH2F("m2mom_glovtxb", "GlobalTrack(vtxb); p ^{.} sign(Z)(GeV); M_{ToF}^{2} (GeV^{2});", 200,
                              -10., 10., M2YBIN, M2MIN, M2MAX);
  fa_m2mom_gloprim = new TH2F("m2mom_gloprim", "GlobalTrack(prim); p ^{.} sign(Z)(GeV); M_{ToF}^{2} (GeV^{2});", 200,
                              -10., 10., M2YBIN, M2MIN, M2MAX);
  fa_m2mom_gloprimvtxb =
    new TH2F("m2mom_gloprimvtxb", "GlobalTrack(primvtxb); p ^{.} sign(Z)(GeV); M_{ToF}^{2} (GeV^{2});", 200, -10., 10.,
             M2YBIN, M2MIN, M2MAX);
  fa_m2mom_glo_pip = new TH2F("m2mom_glo_pip", "GlobalTrack(pip); p ^{.} sign(Z)(GeV); M_{ToF}^{2} (GeV^{2});", 200,
                              -10., 10., M2YBIN, M2MIN, M2MAX);
  fa_m2mom_glo_pim = new TH2F("m2mom_glo_pim", "GlobalTrack(pim); p ^{.} sign(Z)(GeV); M_{ToF}^{2} (GeV^{2});", 200,
                              -10., 10., M2YBIN, M2MIN, M2MAX);
  fa_m2mom_glo_kp  = new TH2F("m2mom_glo_kp", "GlobalTrack(kp); p ^{.} sign(Z)(GeV); M_{ToF}^{2} (GeV^{2});", 200, -10.,
                             10., M2YBIN, M2MIN, M2MAX);
  fa_m2mom_glo_km  = new TH2F("m2mom_glo_km", "GlobalTrack(km); p ^{.} sign(Z)(GeV); M_{ToF}^{2} (GeV^{2});", 200, -10.,
                             10., M2YBIN, M2MIN, M2MAX);
  fa_m2mom_glo_p   = new TH2F("m2mom_glo_p", "GlobalTrack(p); p ^{.} sign(Z)(GeV); M_{ToF}^{2} (GeV^{2});", 200, -10.,
                            10., M2YBIN, M2MIN, M2MAX);
  fa_m2mom_glo_pbar = new TH2F("m2mom_glo_pbar", "GlobalTrack(pbar); p ^{.} sign(Z)(GeV); M_{ToF}^{2} (GeV^{2});", 200,
                               -10., 10., M2YBIN, M2MIN, M2MAX);

  fa_pMCmom_glo      = new TH2F("pMCmom_glo", "GlobalTrack(all); momentum; p_{MC};", 100, 0., 10., 100, 0., 10.);
  fa_pMCmom_glo_pip  = new TH2F("pMCmom_glo_pip", "GlobalTrack(pip); momentum; p_{MC};", 100, 0., 10., 100, 0., 10.);
  fa_pMCmom_glo_pim  = new TH2F("pMCmom_glo_pim", "GlobalTrack(pim); momentum; p_{MC};", 100, 0., 10., 100, 0., 10.);
  fa_pMCmom_glo_kp   = new TH2F("pMCmom_glo_kp", "GlobalTrack(kp); momentum; p_{MC};", 100, 0., 10., 100, 0., 10.);
  fa_pMCmom_glo_km   = new TH2F("pMCmom_glo_km", "GlobalTrack(km); momentum; p_{MC};", 100, 0., 10., 100, 0., 10.);
  fa_pMCmom_glo_p    = new TH2F("pMCmom_glo_p", "GlobalTrack(p); momentum; p_{MC};", 100, 0., 10., 100, 0., 10.);
  fa_pMCmom_glo_pbar = new TH2F("pMCmom_glo_pbar", "GlobalTrack(pbar); momentum; p_{MC};", 100, 0., 10., 100, 0., 10.);
  fa_pMCmom_glo_d    = new TH2F("pMCmom_glo_d", "GlobalTrack(d); momentum; p_{MC};", 100, 0., 10., 100, 0., 10.);
  fa_pMCmom_glo_t    = new TH2F("pMCmom_glo_t", "GlobalTrack(t); momentum; p_{MC};", 100, 0., 10., 100, 0., 10.);
  fa_pMCmom_glo_h    = new TH2F("pMCmom_glo_h", "GlobalTrack(h); momentum; p_{MC};", 100, 0., 10., 100, 0., 10.);
  fa_pMCmom_glo_a    = new TH2F("pMCmom_glo_a", "GlobalTrack(a); momentum; p_{MC};", 100, 0., 10., 100, 0., 10.);

  Double_t LenDisMax = 20.;
  fa_LenDismom_glo =
    new TH2F("LenDismom_glo", "GlobalTrack(all); momentum; len-dis;", 100, 0., 10., 100., -LenDisMax, LenDisMax);
  fa_LenDismom_glo_pip =
    new TH2F("LenDismom_glo_pip", "GlobalTrack(pip); momentum; len-dis;", 100, 0., 10., 100., -LenDisMax, LenDisMax);
  fa_LenDismom_glo_pim =
    new TH2F("LenDismom_glo_pim", "GlobalTrack(pim); momentum; len-dis;", 100, 0., 10., 100., -LenDisMax, LenDisMax);
  fa_LenDismom_glo_kp =
    new TH2F("LenDismom_glo_kp", "GlobalTrack(kp); momentum; len-dis;", 100, 0., 10., 100., -LenDisMax, LenDisMax);
  fa_LenDismom_glo_km =
    new TH2F("LenDismom_glo_km", "GlobalTrack(km); momentum; len-dis;", 100, 0., 10., 100., -LenDisMax, LenDisMax);
  fa_LenDismom_glo_p =
    new TH2F("LenDismom_glo_p", "GlobalTrack(p); momentum; len-dis;", 100, 0., 10., 100., -LenDisMax, LenDisMax);
  fa_LenDismom_glo_pbar =
    new TH2F("LenDismom_glo_pbar", "GlobalTrack(pbar); momentum; len-dis;", 100, 0., 10., 100., -LenDisMax, LenDisMax);
  fa_LenDismom_glo_d =
    new TH2F("LenDismom_glo_d", "GlobalTrack(d); momentum; len-dis;", 100, 0., 10., 100., -LenDisMax, LenDisMax);
  fa_LenDismom_glo_t =
    new TH2F("LenDismom_glo_t", "GlobalTrack(t); momentum; len-dis;", 100, 0., 10., 100., -LenDisMax, LenDisMax);
  fa_LenDismom_glo_h =
    new TH2F("LenDismom_glo_h", "GlobalTrack(h); momentum; len-dis;", 100, 0., 10., 100., -LenDisMax, LenDisMax);
  fa_LenDismom_glo_a =
    new TH2F("LenDismom_glo_a", "GlobalTrack(a); momentum; len-dis;", 100, 0., 10., 100., -LenDisMax, LenDisMax);

  fa_LenMcLenGlomom_glo = new TH2F("LenMcLenGlomom_glo", "GlobalTrack(all); momentum [GeV]; len glo - len MC pnt[cm];",
                                   100, 0., 10., 400., -LenDisMax, LenDisMax);
  fa_LenMcDismom_glo = new TH2F("LenMcDismom_glo", "GlobalTrack(all); momentum [GeV]; len MC pnt - dis [cm];", 100, 0.,
                                10., 400., -LenDisMax, LenDisMax);

  Int_t WYBIN      = 100;
  Float_t WYMAX    = 20.;
  fa_w_mom_glo     = new TH2F("w_mom_glo", "GlobalTrack(all); momentum; weight;", 100, 0., 10., WYBIN, 0., WYMAX);
  fa_w_mom_glo_pip = new TH2F("w_mom_glo_pip", "GlobalTrack(pip); momentum; weight;", 100, 0., 10., WYBIN, 0., WYMAX);
  fa_w_mom_glo_pim = new TH2F("w_mom_glo_pim", "GlobalTrack(pim); momentum; weight;", 100, 0., 10., WYBIN, 0., WYMAX);
  fa_w_mom_glo_kp  = new TH2F("w_mom_glo_kp", "GlobalTrack(kp); momentum; weight;", 100, 0., 10., WYBIN, 0., WYMAX);
  fa_w_mom_glo_km  = new TH2F("w_mom_glo_km", "GlobalTrack(km); momentum; weight;", 100, 0., 10., WYBIN, 0., WYMAX);
  fa_w_mom_glo_p   = new TH2F("w_mom_glo_p", "GlobalTrack(p); momentum; weight;", 100, 0., 10., WYBIN, 0., WYMAX);
  fa_w_mom_glo_pbar =
    new TH2F("w_mom_glo_pbar", "GlobalTrack(pbar); momentum; weight;", 100, 0., 10., WYBIN, 0., WYMAX);
  fa_w_mom_glo_d  = new TH2F("w_mom_glo_d", "GlobalTrack(d); momentum; weight;", 100, 0., 10., WYBIN, 0., WYMAX);
  fa_w_mom_glo_t  = new TH2F("w_mom_glo_t", "GlobalTrack(t); momentum; weight;", 100, 0., 10., WYBIN, 0., WYMAX);
  fa_w_mom_glo_h  = new TH2F("w_mom_glo_h", "GlobalTrack(h); momentum; weight;", 100, 0., 10., WYBIN, 0., WYMAX);
  fa_w_mom_glo_a  = new TH2F("w_mom_glo_a", "GlobalTrack(a); momentum; weight;", 100, 0., 10., WYBIN, 0., WYMAX);
  fa_w_mom_glomis = new TH2F("w_mom_glomis", "GlobalTrack(mis); momentum; weight;", 100, 0., 10., WYBIN, 0., WYMAX);

  //centrality
  fa_mul_b_gen =
    new TH2F("mul_b_gen", "Centrality - gen;impact parameter b; multiplicity;", 15, 0., 15., 100, 0., 10000.);
  fa_mul_b_poi =
    new TH2F("mul_b_poi", "Centrality - poi;impact parameter b; multiplicity;", 15, 0., 15., 100, 0., 10000.);
  fa_mul_b_hit =
    new TH2F("mul_b_hit", "Centrality - hit;impact parameter b; multiplicity;", 15, 0., 15., 100, 0., 2000.);
  fa_mul_b_glo =
    new TH2F("mul_b_glo", "Centrality - glo;impact parameter b; multiplicity;", 15, 0., 15., 100, 0., 2000.);
  fa_mul_b_had =
    new TH2F("mul_b_had", "Centrality - had;impact parameter b; multiplicity;", 15, 0., 15., 100, 0., 2000.);

  //reaction plane
  fa_cdrp_b_gen = new TH2F("cdrp_b_gen", "reaction plane resolution - gen;impact parameter b; cos#Delta#phi_{rp};", 15,
                           0., 15., 20, -1., 1.);
  fa_drp_b_gen =
    new TH2F("drp_b_gen", "#Delta#phi-reaction plane - gen ;impact parameter b;", 15, 0., 15., 36, -180., 180.);
  fa_phirp_b_gen =
    new TH2F("phirp_b_gen", "#phi_{reaction plane} - gen   ;impact parameter b;", 15, 0., 15., 36, -180., 180.);
  fa_phgrp_b_gen =
    new TH2F("phgrp_b_gen", "#phi_{G reaction plane} - gen ;impact parameter b;", 15, 0., 15., 36, -180., 180.);
  fa_phphrp_gen = new TH2F("phphrp_gen", "#phi_#phi - gen ;#phi_{rec}; #phi_{gen} ;", 36, -180., 180., 36, -180., 180.);
  fa_delrp_b_gen = new TH2F("delrp_b_gen",
                            "#Delta#phi_{G}-reaction plane - gen ;impact "
                            "parameter b;#phi_{rec}-#phi_{gen}",
                            15, 0., 15., 36, -180., 180.);
  fa_delrp_b_poi = new TH2F("delrp_b_poi",
                            "#Delta#phi_{G}-reaction plane - poi ;impact "
                            "parameter b;#phi_{rec}-#phi_{gen}",
                            15, 0., 15., 36, -180., 180.);
  fa_delrp_b_hit = new TH2F("delrp_b_hit",
                            "#Delta#phi_{G}-reaction plane - hit ;impact "
                            "parameter b;#phi_{rec}-#phi_{gen}",
                            15, 0., 15., 36, -180., 180.);
  fa_delrp_b_glo = new TH2F("delrp_b_glo",
                            "#Delta#phi_{G}-reaction plane - glo ;impact "
                            "parameter b;#phi_{rec}-#phi_{gen}",
                            15, 0., 15., 36, -180., 180.);

  fa_cdelrp_b_gen = new TH2F("cdelrp_b_gen",
                             "reaction plane resolution - gen;impact parameter "
                             "b;cos(#phi_{rec}-#phi_{gen})",
                             15, 0., 15., 20, -1., 1.);
  fa_cdelrp_b_poi = new TH2F("cdelrp_b_poi",
                             "reaction plane resolution - poi;impact parameter "
                             "b;cos(#phi_{rec}-#phi_{gen})",
                             15, 0., 15., 20, -1., 1.);
  fa_cdelrp_b_hit = new TH2F("cdelrp_b_hit",
                             "reaction plane resolution - hit;impact parameter "
                             "b;cos(#phi_{rec}-#phi_{gen})",
                             15, 0., 15., 20, -1., 1.);
  fa_cdelrp_b_glo = new TH2F("cdelrp_b_glo",
                             "reaction plane resolution - glo;impact parameter "
                             "b;cos(#phi_{rec}-#phi_{gen})",
                             15, 0., 15., 20, -1., 1.);
  fa_cdelrp_b_had = new TH2F("cdelrp_b_had",
                             "reaction plane resolution - had;impact parameter "
                             "b;cos(#phi_{rec}-#phi_{gen})",
                             15, 0., 15., 20, -1., 1.);

  fa_cdrp_b_poi = new TH2F("cdrp_b_poi", "reaction plane resolution - poi;impact parameter b; cos#Delta#phi_{rp};", 15,
                           0., 15., 20, -1., 1.);
  fa_drp_b_poi =
    new TH2F("drp_b_poi", "#Delta#phi-reaction plane - poi ;impact parameter b;", 15, 0., 15., 36, -180., 180.);

  fa_cdrp_b_hit = new TH2F("cdrp_b_hit", "reaction plane resolution - hit;impact parameter b; cos#Delta#phi_{rp};", 15,
                           0., 15., 20, -1., 1.);
  fa_drp_b_hit =
    new TH2F("drp_b_hit", "#Delta#phi-reaction plane - hit ;impact parameter b;", 15, 0., 15., 36, -180., 180.);

  fa_cdrp_b_glo = new TH2F("cdrp_b_glo", "reaction plane resolution - glo;impact parameter b; cos#Delta#phi_{rp};", 15,
                           0., 15., 20, -1., 1.);
  fa_drp_b_glo =
    new TH2F("drp_b_glo", "#Delta#phi-reaction plane - glo ;impact parameter b;", 15, 0., 15., 36, -180., 180.);

  fa_cdrp_b_had = new TH2F("cdrp_b_had", "reaction plane resolution - had;impact parameter b; cos#Delta#phi_{rp};", 15,
                           0., 15., 20, -1., 1.);
  fa_drp_b_had =
    new TH2F("drp_b_had", "#Delta#phi-reaction plane - had ;impact parameter b;", 15, 0., 15., 36, -180., 180.);

  fa_phirp_gen = new TH1F("phirp_gen", "#phi_{reaction plane} - gen   ;#phi_{RPgen};", 72, -180., 180.);
  fa_phirp_poi = new TH1F("phirp_poi", "#phi_{reaction plane} - poi   ;#phi_{RPpoi};", 72, -180., 180.);
  fa_phirp_hit = new TH1F("phirp_hit", "#phi_{reaction plane} - hit   ;#phi_{RPhit};", 72, -180., 180.);
  fa_phirp_glo = new TH1F("phirp_glo", "#phi_{reaction plane} - glo   ;#phi_{RPglo};", 72, -180., 180.);
  fa_phirp_had = new TH1F("phirp_had", "#phi_{reaction plane} - had   ;#phi_{RPhad};", 72, -180., 180.);

  fa_phirps_gen = new TH1F("phirps_gen", "#phi_{reaction plane} - gen   ;#phi_{sRPgen};", 72, -180., 180.);
  fa_phirps_poi = new TH1F("phirps_poi", "#phi_{reaction plane} - poi   ;#phi_{sRPpoi};", 72, -180., 180.);
  fa_phirps_hit = new TH1F("phirps_hit", "#phi_{reaction plane} - hit   ;#phi_{sRPhit};", 72, -180., 180.);
  fa_phirps_glo = new TH1F("phirps_glo", "#phi_{reaction plane} - glo   ;#phi_{sRPglo};", 72, -180., 180.);

  fhwdist       = new TH2F("hwdist", "matching wdist; p (GeV/c); dist;", 100, 0., 10., 50, 0., 10.);
  fhwmindelmass = new TH2F("hwmindelmass", "matching wmindelmass ; p (GeV/c); #Delta M;", 100, 0., 10., 50, 0., 1.);
  fhwminlen     = new TH2F("hwminlen", "matching wminlen ; p (GeV/c); MinPathlength;", 100, 0., 10., 50, 0., 2.);
  fhwdelp       = new TH2F("hwdelp", "matching wdelp ; p (GeV/c); #Delta p/p;", 100, 0., 10., 50, 0., 1.);

  fhTofTrkDx    = new TH1F("hTofTrkDx", " x - position resolution   ; #deltax;", 50, -1., 1.);
  fhTofTrkDy    = new TH1F("hTofTrkDy", " y - position resolution   ; #deltay;", 50, -1., 1.);
  fhTofTrkDxsel = new TH1F("hTofTrkDxsel", " x - position resolution   ; #deltax;", 50, -1., 1.);
  fhTofTrkDysel = new TH1F("hTofTrkDysel", " y - position resolution   ; #deltay;", 50, -1., 1.);

  cout << "CbmHadronAnalysis::CreateHistogramms: histograms booked in directory " << gDirectory->GetName() << endl;
}

// ------------------------------------------------------------------
InitStatus CbmHadronAnalysis::ReadPdfFile()
{  // Open PDF file and get histogramms for PID
  if (fPdfFileName.IsNull()) return kSUCCESS;

  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  TFile* pdfFile = new TFile(fPdfFileName);

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;

  if (NULL == pdfFile) {
    cout << "-E- CbmHadronAnalysis::ReadPdfFile : "
         << "file " << fPdfFileName << " does not exist!" << endl;
    return kSUCCESS;
  }
  return kSUCCESS;
}

// ------------------------------------------------------------------
InitStatus CbmHadronAnalysis::ReadFlowFile()
{  // Open file and get histogramms for RP corrections
  if (fFlowFileName.IsNull()) return kSUCCESS;

  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  fflowFile = new TFile(fFlowFileName);

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;

  if (NULL == fflowFile) {
    cout << "-E- CbmHadronAnalysis::ReadFlowFile : "
         << "file " << fFlowFileName << " does not exist!" << endl;
    return kSUCCESS;
  }
  cout << "-I- CbmHadronAnalysis::ReadFlowFile : RP corrections from " << fFlowFileName << endl;

  return kSUCCESS;
}

// ------------------------------------------------------------------
InitStatus CbmHadronAnalysis::Init()
{
  // Task initialisation
  FairRootManager* rootMgr = FairRootManager::Instance();
  if (NULL == rootMgr) {
    cout << "-E- CbmHadronAnalysis::Init : ROOT manager is not instantiated." << endl;
    return kFATAL;
  }

  fEventsColl = dynamic_cast<TClonesArray*>(rootMgr->GetObject("Event"));
  if (NULL == fEventsColl) fEventsColl = dynamic_cast<TClonesArray*>(rootMgr->GetObject("CbmEvent"));

  if (NULL == fEventsColl) LOG(info) << "CbmEvent not found in input file, assume eventwise input";
  else
    LOG(info) << "CbmEvent found in input file, timebased analysis";

  fMCEventHeader = (FairMCEventHeader*) rootMgr->GetObject("MCEventHeader.");
  if (NULL == fMCEventHeader) { cout << "-W- CbmHadronAnalysis::Init : no MC Header Info" << endl; }

  fTofPoints = (TClonesArray*) rootMgr->GetObject("TofPoint");
  if (NULL == fTofPoints) { cout << "-W- CbmHadronAnalysis::Init : no TOF point array!" << endl; }

  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->Init();
  if (!fDigiMan->IsPresent(ECbmModuleId::kTof)) {
    LOG(error) << GetName() << ": No input digis!";
    return kFATAL;
  }
  else
    LOG(info) << "DigiManager has Tof Digis";

  /*
   fTofDigis   = (TClonesArray *) rootMgr->GetObject("TofDigi");

   if( NULL == fTofDigis)
     fTofDigis = (TClonesArray *) rootMgr->GetObject("CbmTofDigiExp");

   if( NULL == fTofDigis)
      fTofDigis = (TClonesArray *) rootMgr->GetObject("CbmTofDigi");
   if( NULL == fTofDigis)
   {
      LOG(error)<<"CbmHadronAnalysis::RegisterInputs => Could not get the TofDigi TClonesArray!!!";
   } // if( NULL == fTofDigis)
*/

  fTofDigiMatchColl = (TClonesArray*) rootMgr->GetObject("TofHitCalDigiMatch");
  if (NULL == fTofDigiMatchColl) {
    cout << "-I- CbmHadronAnalysis::Init : no TOF HitCalDigiMatch array!" << endl;
    fTofDigiMatchColl = (TClonesArray*) rootMgr->GetObject("TofDigiMatch");
    if (NULL == fTofDigiMatchColl) { cout << "-I- CbmHadronAnalysis::Init : no TOF digiMatch array!" << endl; }
  }

  fTofDigiMatchPointsColl = (TClonesArray*) rootMgr->GetObject("TofDigiMatchPoints");
  if (NULL == fTofDigiMatchPointsColl) {
    cout << "-I- CbmHadronAnalysis::Init : no TOF digiMatchPoints array!" << endl;
  }

  fTofHitsColl = (TClonesArray*) rootMgr->GetObject("TofHit");
  if (NULL == fTofHitsColl) { cout << "-W- CbmHadronAnalysis::Init : no TOF Hit array!" << endl; }

  fTofTracks = (TClonesArray*) rootMgr->GetObject("TofTrack");
  if (NULL == fTofTracks) {
    cout << "-W- CbmHadronAnalysis::Init : "
         << "no Tof Track array!" << endl;
  }

  fTrdPoints = (TClonesArray*) rootMgr->GetObject("TrdPoint");
  if (NULL == fTofPoints) { cout << "-W- CbmHadronAnalysis::Init : no TRD point array!" << endl; }

  fTrdHits = (TClonesArray*) rootMgr->GetObject("TrdHit");
  if (NULL == fTrdHits) { cout << "-W- CbmHadronAnalysis::Init : no TRD Hit array!" << endl; }

  fGlobalTracks = (TClonesArray*) rootMgr->GetObject("GlobalTrack");
  if (NULL == fGlobalTracks) { cout << "-W- CbmHadronAnalysis::Init : no Global Track array!" << endl; }

  fHadrons = (TClonesArray*) rootMgr->GetObject("Hadron");
  if (NULL == fHadrons) { cout << "-W- CbmHadronAnalysis::Init : no Hadron array!" << endl; }

  fStsTracks = (TClonesArray*) rootMgr->GetObject("StsTrack");
  if (NULL == fStsTracks) { cout << "-W- CbmHadronAnalysis::Init : no STS Track array!" << endl; }
  fStsHitsColl = (TClonesArray*) rootMgr->GetObject("StsHit");
  if (NULL == fStsHitsColl) { cout << "-W- CbmHadronAnalysis::Init : no STS Hit array!" << endl; }
  fStsClusters = (TClonesArray*) rootMgr->GetObject("StsCluster");
  if (NULL == fStsClusters) { cout << "-W- CbmHadronAnalysis::Init : no STS Cluster array!" << endl; }
  fStsDigis = (TClonesArray*) rootMgr->GetObject("StsDigi");
  if (NULL == fStsDigis) { cout << "-W- CbmHadronAnalysis::Init : no STS Digi array!" << endl; }
  fStsDigiMatchColl = (TClonesArray*) rootMgr->GetObject("StsDigiMatch");
  if (NULL == fStsDigiMatchColl) {
    cout << "-W- CbmHadronAnalysis::Init : "
         << "no STS DigiMatch array!" << endl;
  }

  // Get pointer to PrimaryVertex object from IOManager if it exists
  // The old name for the object is "PrimaryVertex" the new one
  // "PrimaryVertex." Check first for the new name
  fPrimVertex = dynamic_cast<CbmVertex*>(rootMgr->GetObject("PrimaryVertex."));
  if (nullptr == fPrimVertex) { fPrimVertex = dynamic_cast<CbmVertex*>(rootMgr->GetObject("PrimaryVertex")); }
  if (nullptr == fPrimVertex) { LOG(warn) << "No primary vertex"; }

  if (NULL == fEventsColl) {
    fTofHits = fTofHitsColl;
    fStsHits = fStsHitsColl;
  }
  else {  // for time based analysis generate temporary Event TClonesArrays
    fTofHits = new TClonesArray("CbmTofHit", 100);
    fStsHits = new TClonesArray("CbmStsHit", 100);
  }

  // --- MC data manager
  CbmMCDataManager* mcManager = (CbmMCDataManager*) rootMgr->GetObject("MCDataManager");

  if (NULL != mcManager) {
    // --- Data arrays
    fMCTracks  = mcManager->InitBranch("MCTrack");
    fStsPoints = mcManager->InitBranch("StsPoint");
  }

  fMCTracksColl = (TClonesArray*) rootMgr->GetObject("MCTrack");
  if (NULL == fMCTracksColl) {
    cout << "-W- CbmHadronAnalysis::Init : "
         << "no MC track array!" << endl;
  }

  fStsPointsColl = (TClonesArray*) rootMgr->GetObject("StsPoint");
  if (NULL == fStsPointsColl) {
    cout << "-W- CbmHadronAnalysis::Init : "
         << "no STS Point array!" << endl;
  }

  fTrackFitter.Init();

  InitStatus status = ReadPdfFile();
  if (kSUCCESS != status) { return status; }

  InitStatus statf = ReadFlowFile();
  if (kSUCCESS != statf) { return statf; }

  Float_t pbeam = GetBeamMomentum();
  {
    Float_t ep = TMath::Sqrt(pbeam * pbeam + M2PROT);
    Float_t gp = ep / TMath::Sqrt(M2PROT);
    Float_t bp = TMath::Sqrt(1. - 1. / gp / gp);
    Float_t yp = 0.5 * TMath::Log((1. + bp) / (1. - bp));
    SetMidY(yp * 0.5);
    cout << "-I- CbmHadronAnalysis: Initialize for pbeam = " << pbeam << " rap:" << GetMidY() << endl;
  }

  CreateHistogramms();

  cout << "-I- CbmHadronAnalysis::Init : "
       << "initialisation completed." << endl;

  return kSUCCESS;
}
// ------------------------------------------------------------------
// ------------------------------------------------------------------
void CbmHadronAnalysis::Exec(Option_t* option)
{
  // Task execution on TS or Events
  if (NULL != fEventsColl) {  // working for STS and TOF only
    iNbTs++;
    LOG(debug) << "process TS " << iNbTs << " with " << fEventsColl->GetEntriesFast() << " events";
    if (NULL == fTofHits || NULL == fTofHitsColl) { assert("Invalid pointer to Tof TClonesArray"); }
    if (NULL == fStsHits || NULL == fStsHitsColl) { assert("Invalid pointer to Sts TClonesArray"); }
    LOG(debug) << "TS contains " << fTofHitsColl->GetEntriesFast() << "TOF and " << fStsHitsColl->GetEntriesFast()
               << " Sts hits";

    for (Int_t iEvent = 0; iEvent < fEventsColl->GetEntriesFast(); iEvent++) {
      CbmEvent* tEvent = dynamic_cast<CbmEvent*>(fEventsColl->At(iEvent));
      // Inspect CbmEvent
      LOG(debug) << "CbmEvent with " << tEvent->GetNofData() << " total data";
      LOG(debug) << "        " << tEvent->ToString();
      // copy TOF hits
      fTofHits->Clear();
      Int_t iNbHits = 0;
      LOG(debug) << "Fill Tof array with mul " << tEvent->GetNofData(ECbmDataType::kTofHit);
      for (Int_t iHit = 0; iHit < tEvent->GetNofData(ECbmDataType::kTofHit); iHit++) {
        Int_t iHitIndex = static_cast<Int_t>(tEvent->GetIndex(ECbmDataType::kTofHit, iHit));
        CbmTofHit* pHit = (CbmTofHit*) fTofHitsColl->At(iHitIndex);
        new ((*fTofHits)[iNbHits]) CbmTofHit(*pHit);  // fill temporary working TClonesArray
        iNbHits++;
      }
      // copy STS hits
      iNbHits = 0;
      fStsHits->Clear();
      LOG(debug) << "Fill Sts array with mul " << tEvent->GetNofData(ECbmDataType::kStsHit) << " out of "
                 << fStsHitsColl->GetEntriesFast();
      for (Int_t iHit = 0; iHit < tEvent->GetNofData(ECbmDataType::kStsHit); iHit++) {
        Int_t iHitIndex       = static_cast<Int_t>(tEvent->GetIndex(ECbmDataType::kStsHit, iHit));
        const CbmStsHit* pHit = (CbmStsHit*) fStsHitsColl->At(iHitIndex);
        new ((*fStsHits)[iNbHits]) CbmStsHit(*pHit);  // fill temporary working TClonesArray
        iNbHits++;
      }
      LOG(debug) << Form("analyze TS %d, Ev %d with %d STS, %d TOF hits", iNbTs, iEvent, fStsHits->GetEntriesFast(),
                         fTofHits->GetEntriesFast());
      ExecEvent(option);
    }
  }
  else {  // event based analysis
    ExecEvent(option);
  }
}

// ------------------------------------------------------------------
void CbmHadronAnalysis::ExecEvent(Option_t*)
{
  // Task execution

  // Declare variables outside the loop
  CbmMCTrack* MCTrack   = NULL;
  CbmStsTrack* StsTrack = NULL;
  CbmTofPoint* TofPoint = NULL;
  CbmTofHit* TofHit     = NULL;
  CbmTofTrack* TofTrack = NULL;
  //    CbmTofTrack *TofTrackh;
  CbmTofTrack* BestTofTrack = NULL;
  CbmGlobalTrack* GlobTrack = NULL;
  //    CbmMatch *tofHitMatch;


  Int_t pdgCode, Np1, Np2;
  Float_t Qx1, Qy1, Qx2, Qy2, phirp1, phirp2, phirp, delrp, rp_weight;
  Float_t RADDEG = 57.29577951;
  Float_t p_MC, px_MC, py_MC, pz_MC;
  Float_t mfrag = 0.;
  Float_t MaxT0 = 0.1;

  Int_t TrackP[100000];
  Float_t t_hit;
  Bool_t use_pions_for_flow = kTRUE;

  Int_t verbose = 1;

  if (GetBSelMax() > 0.) {
    if (fMCEventHeader->GetB() > GetBSelMax()) { return; };
  }

  if (GetBSelMin() > 0.) {
    if (fMCEventHeader->GetB() < GetBSelMin()) { return; };
  }

  Float_t yrp_mid = GetMidY();  // midrapidity -> update from simulation!
  if (fMCTracksColl != NULL) nMCTracks = fMCTracksColl->GetEntriesFast();
  if (fTofPoints != NULL) nTofPoints = fTofPoints->GetEntriesFast();
  if (fTofHits != NULL) nTofHits = fTofHits->GetEntriesFast();
  if (fTofTracks != NULL) nTofTracks = fTofTracks->GetEntriesFast();
  if (fGlobalTracks != NULL) nGlobTracks = fGlobalTracks->GetEntriesFast();
  if (fStsHits != NULL) nStsHits = fStsHits->GetEntriesFast();

  fhTofHitMul->Fill((Double_t) nTofHits);
  fhStsHitMul->Fill((Double_t) nStsHits);

  if (verbose > 0) {  //nh-debug
    LOG(debug) << "<D> HadronAnalysis::Exec starting with MCtrks " << nMCTracks << ", TofPoi " << nTofPoints
               << ", TofHit " << nTofHits << ", TofTrk " << nTofTracks << ", GlbTrk " << nGlobTracks << ", StsHit "
               << nStsHits;
    if (fMCEventHeader != NULL)
      LOG(debug) << "-D- b = " << fMCEventHeader->GetB() << ", phi = " << fMCEventHeader->GetRotZ();
  }
  if (fair::Logger::Logging(fair::Severity::debug)) {
    for (Int_t j = 0; j < nTofHits; j++) {
      TofHit = (CbmTofHit*) fTofHits->At(j);
      if (NULL == TofHit) continue;
      LOG(DEBUG) << Form("TofHit %d, addr 0x%08x, x %6.1f, y %6.1f, z %6.1f, t %6.1f ", j, TofHit->GetAddress(),
                         TofHit->GetX(), TofHit->GetY(), TofHit->GetZ(), TofHit->GetTime());
    }
  }

  Double_t dT0 = 0.;
  for (Int_t j = 0; j < nTofHits; j++) {
    TofHit = (CbmTofHit*) fTofHits->At(j);
    if (NULL == TofHit) continue;
    if (TofHit->GetZ() == 0.) dT0 = TofHit->GetTime();
  }

  if (dT0 != 0.) {
    for (Int_t j = 0; j < nTofHits; j++) {
      TofHit = (CbmTofHit*) fTofHits->At(j);
      if (NULL == TofHit) continue;
      TofHit->SetTime(TofHit->GetTime() - dT0);
    }
  }

  if (fair::Logger::Logging(fair::Severity::debug)) {
    for (Int_t j = 0; j < nTofHits; j++) {
      TofHit = (CbmTofHit*) fTofHits->At(j);
      if (NULL == TofHit) continue;
      LOG(DEBUG) << Form("TofHit %d, addr 0x%08x, x %6.1f, y %6.1f, z %6.1f, t %6.1f ", j, TofHit->GetAddress(),
                         TofHit->GetX(), TofHit->GetY(), TofHit->GetZ(), TofHit->GetTime());
    }
  }

  if (bRecSec && nTofHits > 1) ReconstructSecondaries();  // independent method

  // some local arrays
  Int_t MAXNHT = 50;
  Int_t NTHMUL[nGlobTracks];                  // number of candidate TofTracks
  Int_t IndTHMUL[nGlobTracks][MAXNHT];        // ordered array of candidate number of TofTrack indices
  Float_t Weight_THMUL[nGlobTracks][MAXNHT];  // weights for ordered array of candidate number of TofTrack indices
  //    Int_t IndTofTracks[nGlobTracks][MAXNHT][MAXNHT]; // array of TofTrack Indices that selected TofHit is assigned to
  Int_t NTofHitTMul[nTofHits];                 // number of GlobalTracks assigned to a specific TofHit
  Int_t IndTofTrack_TofHit[nTofHits][MAXNHT];  // index of TofTracks assigned to specific TofHit

  // generator level
  if (NULL != fMCEventHeader) fa_mul_b_gen->Fill(fMCEventHeader->GetB(), nMCTracks);

  Qx1 = 0.;
  Qy1 = 0.;
  Np1 = 0;
  Qx2 = 0.;
  Qy2 = 0.;
  Np2 = 0;
  for (Int_t k = 0; k < nMCTracks; k++) {  // inspect MCTracks

    MCTrack = (CbmMCTrack*) fMCTracksColl->At(k);

    if (MCTrack->GetMotherId() != -1) continue;  // primary particles only

    // Process only hadrons
    pdgCode = MCTrack->GetPdgCode();
    //        cout << " Track k="<<k<<", pdgCode = "<<pdgCode<<endl;

    if (pdgCode < 100000000) {
      if (211 != TMath::Abs(pdgCode) &&  // pions
          321 != TMath::Abs(pdgCode) &&  // kaons
          2212 != TMath::Abs(pdgCode))   // protons
        continue;
    }
    else {
      mfrag = (pdgCode % 1000) / 10 * .931494028;  // ignoring binding energies ...
      // where is the proper mass stored ?
    }

    Float_t Phip = RADDEG * atan2(MCTrack->GetPy(), MCTrack->GetPx());
    Float_t dphi = Phip - RADDEG * fMCEventHeader->GetRotZ();
    if (dphi < -180.) { dphi += 360.; };
    if (dphi > 180.) { dphi -= 360.; };
    dphi      = dphi / RADDEG;
    rp_weight = 0.;

    //cout << " Track k="<<k<<", pdgCode = "<<pdgCode<<
    //  " Mass " << MCTrack->GetMass()<<","<<mfrag<<" Y " << MCTrack->GetRapidity() <<
    //  " Pt " << MCTrack->GetPt() <<endl;

    switch (pdgCode) {
      case 211: {
        fa_ptm_rap_gen_pip->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
        fa_v1_rap_gen_pip->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
        fa_v2_rap_gen_pip->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));

        if (use_pions_for_flow && TMath::Abs((MCTrack->GetRapidity() - yrp_mid) / yrp_mid) > GetDY()
            && MCTrack->GetPt() / MCTrack->GetMass() > GetFlowMinPtm()) {
          if (MCTrack->GetRapidity() > yrp_mid) {  // set weights for reaction plane
            rp_weight = -1.;
          }
          else {
            rp_weight = +1.;
          }
        }
        else {
          rp_weight = 0.;
        }
        break;
      };
      case -211: {
        fa_ptm_rap_gen_pim->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
        fa_v1_rap_gen_pim->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
        fa_v2_rap_gen_pim->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));

        if (use_pions_for_flow && TMath::Abs((MCTrack->GetRapidity() - yrp_mid) / yrp_mid) > GetDY()
            && MCTrack->GetPt() / MCTrack->GetMass() > GetFlowMinPtm()) {
          if (MCTrack->GetRapidity() > yrp_mid) {  // set weights for reaction plane
            rp_weight = -1.;
          }
          else {
            rp_weight = +1.;
          }
        }
        else {
          rp_weight = 0.;
        }
        break;
      };
      case 321: {
        fa_ptm_rap_gen_kp->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
        fa_v1_rap_gen_kp->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
        fa_v2_rap_gen_kp->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));
        break;
      };
      case -321: {
        fa_ptm_rap_gen_km->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
        fa_v1_rap_gen_km->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
        fa_v2_rap_gen_km->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));
        break;
      };
      case 2212: {  //protons
        fa_ptm_rap_gen_p->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
        fa_v1_rap_gen_p->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
        fa_v2_rap_gen_p->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));

        // reaction plane determination
        if (TMath::Abs((MCTrack->GetRapidity() - yrp_mid) / yrp_mid) > GetDY()
            && MCTrack->GetPt() / MCTrack->GetMass() > GetFlowMinPtm()) {
          if (MCTrack->GetRapidity() > yrp_mid) {  // set weights for reaction plane
            rp_weight = 1.;
          }
          else {
            rp_weight = -1.;
          }
        }
        break;
      };
      case -2212: {
        fa_ptm_rap_gen_pbar->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
        fa_v1_rap_gen_pbar->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
        fa_v2_rap_gen_pbar->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));
        break;
      };

      case 1000010020: {  // deuteron
        fa_ptm_rap_gen_d->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / mfrag);
        fa_v1_rap_gen_d->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
        fa_v2_rap_gen_d->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));
        // reaction plane determination
        if (TMath::Abs((MCTrack->GetRapidity() - yrp_mid) / yrp_mid) > GetDY()
            && MCTrack->GetPt() / MCTrack->GetMass() > GetFlowMinPtm()) {
          if (MCTrack->GetRapidity() > yrp_mid) {  // set weights for reaction plane
            rp_weight = 1.;
          }
          else {
            rp_weight = -1.;
          }
        }
        break;
      };

      case 1000010030: {  // triton
        fa_ptm_rap_gen_t->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / mfrag);
        fa_v1_rap_gen_t->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
        fa_v2_rap_gen_t->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));
        // reaction plane determination
        if (TMath::Abs((MCTrack->GetRapidity() - yrp_mid) / yrp_mid) > GetDY()
            && MCTrack->GetPt() / MCTrack->GetMass() > GetFlowMinPtm()) {
          if (MCTrack->GetRapidity() > yrp_mid) {  // set weights for reaction plane
            rp_weight = 1.;
          }
          else {
            rp_weight = -1.;
          }
        }
        break;
      };

      case 1000020030: {  // 3he
        fa_ptm_rap_gen_h->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / mfrag);
        fa_v1_rap_gen_h->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
        fa_v2_rap_gen_h->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));
        // reaction plane determination
        if (TMath::Abs((MCTrack->GetRapidity() - yrp_mid) / yrp_mid) > GetDY()
            && MCTrack->GetPt() / MCTrack->GetMass() > GetFlowMinPtm()) {
          if (MCTrack->GetRapidity() > yrp_mid) {  // set weights for reaction plane
            rp_weight = 1.;
          }
          else {
            rp_weight = -1.;
          }
        }
        break;
      };

      case 1000020040: {  // alpha
        fa_ptm_rap_gen_a->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / mfrag);
        fa_v1_rap_gen_a->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
        fa_v2_rap_gen_a->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));
        // reaction plane determination
        if (TMath::Abs((MCTrack->GetRapidity() - yrp_mid) / yrp_mid) > GetDY()
            && MCTrack->GetPt() / MCTrack->GetMass() > GetFlowMinPtm()) {
          if (MCTrack->GetRapidity() > yrp_mid) {  // set weights for reaction plane
            rp_weight = 1.;
          }
          else {
            rp_weight = -1.;
          }
        }
        break;
      };

      default:
      {  // intermediate mass fragments
         //cout << " Track k="<<k<<", pdgCode = "<<pdgCode<<
        //" Mass " << MCTrack->GetMass()<<","<<mfrag<<" Y " << MCTrack->GetRapidity() <<
        //" Pt " << MCTrack->GetPt() <<endl;
        fa_ptm_rap_gen_imf->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / mfrag);
        fa_v1_rap_gen_imf->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
        fa_v2_rap_gen_imf->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));
        // reaction plane determination  (optimistic)
        if (TMath::Abs((MCTrack->GetRapidity() - yrp_mid) / yrp_mid) > GetDY()
            && MCTrack->GetPt() / MCTrack->GetMass() > GetFlowMinPtm()) {
          if (MCTrack->GetRapidity() > yrp_mid) {  // set weights for reaction plane
            rp_weight = 1.;
          }
          else {
            rp_weight = -1.;
          }
        }
        break;
      };
    }
    if (rp_weight != 0.) {
      if (gRandom->Uniform(1) > 0.5) {  //subdivide events into 2 random subevents
        Np1++;
        Qx1 = Qx1 + rp_weight * MCTrack->GetPx();
        Qy1 = Qy1 + rp_weight * MCTrack->GetPy();
      }
      else {
        Np2++;
        Qx2 = Qx2 + rp_weight * MCTrack->GetPx();
        Qy2 = Qy2 + rp_weight * MCTrack->GetPy();
      }
    }
  }

  if (Np1 > 0 && Np2 > 0) {
    phirp1 = atan2(Qy1, Qx1);
    phirp2 = atan2(Qy2, Qx2);
    if (fflowFile != NULL) {  // subevent RP flattening
      TH1F* phirp_gen_fpar = (TH1F*) fflowFile->Get("phirps_gen_fpar");
      Float_t dphir        = 0.;
      for (int j = 0; j < 4; j++) {
        Float_t i = (float) (j + 1);
        dphir += (-phirp_gen_fpar->GetBinContent(j) * TMath::Cos(i * phirp1)
                  + phirp_gen_fpar->GetBinContent(j + 4) * TMath::Sin(i * phirp1))
                 / i;
      }
      phirp1 += dphir;

      dphir = 0.;
      for (int j = 0; j < 4; j++) {
        Float_t i = (float) (j + 1);
        dphir += (-phirp_gen_fpar->GetBinContent(j) * TMath::Cos(i * phirp2)
                  + phirp_gen_fpar->GetBinContent(j + 4) * TMath::Sin(i * phirp2))
                 / i;
      }
      phirp2 += dphir;
    }  // subevent RP flattening end

    delrp = (phirp1 - phirp2);
    fa_phirps_gen->Fill(phirp1 * RADDEG);  // 1D histo
    fa_phirps_gen->Fill(phirp2 * RADDEG);  // 1D histo
    if (0) {                               //nh-debug
      cout << "<D-gen> Impact parameter " << fMCEventHeader->GetB() << ", delrp = " << delrp << endl;
    }
    fa_cdrp_b_gen->Fill(fMCEventHeader->GetB(), TMath::Cos(delrp));
    delrp = delrp * RADDEG;
    while (delrp < -180.) {
      delrp += 360.;
    }
    while (delrp > 180.) {
      delrp -= 360.;
    }
    fa_drp_b_gen->Fill(fMCEventHeader->GetB(), delrp);

    phirp = RADDEG * atan2(Qy1 + Qy2, Qx1 + Qx2);  // full reaction plane
    while (phirp < -180.) {
      phirp += 360.;
    }
    while (phirp > 180.) {
      phirp -= 360.;
    }
    if (fflowFile != NULL) {  // RP flattening
      TH1F* phirp_gen_fpar = (TH1F*) fflowFile->Get("phirp_gen_fpar");
      Float_t dphir        = 0.;
      for (int j = 0; j < 4; j++) {
        Float_t i = (float) (j + 1);
        //cout << " RP flat par "<< i << ","<<j<< " par " << phirp_gen_fpar->GetBinContent(j)
        //     << ","<< phirp_gen_fpar->GetBinContent(j+4) << " phirp "<<phirp<<" dphir "<< dphir << endl;
        dphir += ((-phirp_gen_fpar->GetBinContent(j) * TMath::Cos(i * phirp / RADDEG)
                   + phirp_gen_fpar->GetBinContent(j + 4) * TMath::Sin(i * phirp / RADDEG))
                  / i);
      }
      //cout << " phirp " << phirp << " dphir " << dphir*RADDEG << endl;

      phirp += dphir * RADDEG;
      while (phirp < -180.) {
        phirp += 360.;
      }
      while (phirp > 180.) {
        phirp -= 360.;
      }
    }  // RP flattening end
    delrp = phirp - RADDEG * fMCEventHeader->GetRotZ();
    while (delrp < -180.) {
      delrp += 360.;
    }
    while (delrp > 180.) {
      delrp -= 360.;
    }

    fa_phirp_gen->Fill(phirp);
    fa_delrp_b_gen->Fill(fMCEventHeader->GetB(), delrp);
    fa_cdelrp_b_gen->Fill(fMCEventHeader->GetB(), TMath::Cos(delrp / RADDEG));
    fa_phirp_b_gen->Fill(fMCEventHeader->GetB(), phirp);
    fa_phgrp_b_gen->Fill(fMCEventHeader->GetB(), RADDEG * fMCEventHeader->GetRotZ());
    fa_phphrp_gen->Fill(phirp, RADDEG * fMCEventHeader->GetRotZ());
  }  // Np1 && Np2 end

  // TofPoint level

  for (Int_t k = 0; k < nMCTracks; k++)
    TrackP[k] = 0;  // reset track detected flag

  Qx1 = 0.;
  Qy1 = 0.;
  Np1 = 0;
  Qx2 = 0.;
  Qy2 = 0.;
  Np2 = 0;
  if (NULL != fMCEventHeader) fa_mul_b_poi->Fill(fMCEventHeader->GetB(), nTofPoints);

  for (Int_t l = 0; l < nTofPoints; l++) {
    TofPoint = (CbmTofPoint*) fTofPoints->At(l);
    if (NULL == TofPoint) {
      LOG(WARNING) << " Missing TofPoint at " << l << ", mul " << nTofPoints;
      continue;
    }
    Int_t k = TofPoint->GetTrackID();
    MCTrack = (CbmMCTrack*) fMCTracksColl->At(k);
    pdgCode = MCTrack->GetPdgCode();
    if (pdgCode > 100000000) {
      mfrag = (pdgCode % 1000) / 10 * .931494028;  // ignoring binding energies ...
    }
    px_MC = MCTrack->GetPx();
    py_MC = MCTrack->GetPy();
    pz_MC = MCTrack->GetPz();
    p_MC  = sqrt(px_MC * px_MC + py_MC * py_MC + pz_MC * pz_MC);

    if (TrackP[k] == 0) {  // for efficiency
      TrackP[k] = 1;

      fa_xy_poi1->Fill(TofPoint->GetX(), TofPoint->GetY());
      fa_xy_poi2->Fill(TofPoint->GetX(), TofPoint->GetY(), fwxy2);

      Float_t vel = TofPoint->GetLength() / TofPoint->GetTime();
      Float_t bet = vel / clight;
      if (bet > 0.99999) { bet = 0.99999; }
      Float_t tofmass = p_MC / bet * TMath::Sqrt(1. - bet * bet) * TMath::Sign(1, pdgCode);

      fa_pv_poi->Fill(vel, p_MC);
      fa_tm_poi->Fill(p_MC, tofmass);

      if (MCTrack->GetMotherId() != -1) continue;  // primary particles only
      fa_tm_poiprim->Fill(p_MC, tofmass);

      Float_t Phip = RADDEG * atan2(MCTrack->GetPy(), MCTrack->GetPx());
      Float_t dphi = Phip - RADDEG * fMCEventHeader->GetRotZ();
      if (dphi < -180.) { dphi += 360.; };
      if (dphi > 180.) { dphi -= 360.; };
      dphi      = dphi / RADDEG;
      rp_weight = 0.;

      switch (pdgCode) {
        case 211: {
          fa_ptm_rap_poi_pip->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
          if (use_pions_for_flow && TMath::Abs((MCTrack->GetRapidity() - yrp_mid) / yrp_mid) > GetDY()
              && MCTrack->GetPt() / MCTrack->GetMass() > GetFlowMinPtm()) {
            if (MCTrack->GetRapidity() > yrp_mid) {  // set weights for reaction plane
              rp_weight = -1.;
            }
            else {
              rp_weight = +1.;
            }
          }
          else {
            rp_weight = 0.;
          }
          break;
        };
        case -211: {
          fa_ptm_rap_poi_pim->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
          if (use_pions_for_flow && TMath::Abs((MCTrack->GetRapidity() - yrp_mid) / yrp_mid) > GetDY()
              && MCTrack->GetPt() / MCTrack->GetMass() > GetFlowMinPtm()) {
            if (MCTrack->GetRapidity() > yrp_mid) {  // set weights for reaction plane
              rp_weight = -1.;
            }
            else {
              rp_weight = +1.;
            }
          }
          else {
            rp_weight = 0.;
          }
          break;
        };
        case 321: {
          fa_ptm_rap_poi_kp->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
          break;
        };
        case -321: {
          fa_ptm_rap_poi_km->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
          break;
        };
        case 2212: {
          fa_ptm_rap_poi_p->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
          // reaction plane determination
          if (TMath::Abs((MCTrack->GetRapidity() - yrp_mid) / yrp_mid) > GetDY()
              && MCTrack->GetPt() / MCTrack->GetMass() > GetFlowMinPtm()) {
            if (MCTrack->GetRapidity() > yrp_mid) {  // set weights for reaction plane
              rp_weight = +1.;
            }
            else {
              rp_weight = -1.;
            }
          }
          break;
        };
        case -2212: {
          fa_ptm_rap_poi_pbar->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
          break;
        };

        case 1000010020: {  // deuteron
          fa_ptm_rap_poi_d->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / mfrag);
          fa_v1_rap_poi_d->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
          fa_v2_rap_poi_d->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));
          // reaction plane determination
          if (TMath::Abs((MCTrack->GetRapidity() - yrp_mid) / yrp_mid) > GetDY()
              && MCTrack->GetPt() / MCTrack->GetMass() > GetFlowMinPtm()) {
            if (MCTrack->GetRapidity() > yrp_mid) {  // set weights for reaction plane
              rp_weight = 1.;
            }
            else {
              rp_weight = -1.;
            }
          }
          break;
        };

        case 1000010030: {  // triton
          fa_ptm_rap_poi_t->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / mfrag);
          fa_v1_rap_poi_t->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
          fa_v2_rap_poi_t->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));
          // reaction plane determination
          if (TMath::Abs((MCTrack->GetRapidity() - yrp_mid) / yrp_mid) > GetDY()
              && MCTrack->GetPt() / MCTrack->GetMass() > GetFlowMinPtm()) {
            if (MCTrack->GetRapidity() > yrp_mid) {  // set weights for reaction plane
              rp_weight = 1.;
            }
            else {
              rp_weight = -1.;
            }
          }
          break;
        };

        case 1000020030: {  // 3he
          fa_ptm_rap_poi_h->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / mfrag);
          fa_v1_rap_poi_h->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
          fa_v2_rap_poi_h->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));
          // reaction plane determination
          if (TMath::Abs((MCTrack->GetRapidity() - yrp_mid) / yrp_mid) > GetDY()
              && MCTrack->GetPt() / MCTrack->GetMass() > GetFlowMinPtm()) {
            if (MCTrack->GetRapidity() > yrp_mid) {  // set weights for reaction plane
              rp_weight = 1.;
            }
            else {
              rp_weight = -1.;
            }
          }
          break;
        };

        case 1000020040: {  // alpha
          fa_ptm_rap_poi_a->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / mfrag);
          fa_v1_rap_poi_a->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
          fa_v2_rap_poi_a->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));
          // reaction plane determination
          if (TMath::Abs((MCTrack->GetRapidity() - yrp_mid) / yrp_mid) > GetDY()
              && MCTrack->GetPt() / MCTrack->GetMass() > GetFlowMinPtm()) {
            if (MCTrack->GetRapidity() > yrp_mid) {  // set weights for reaction plane
              rp_weight = 1.;
            }
            else {
              rp_weight = -1.;
            }
          }
          break;
        };
        default:
        {  // intermediate mass fragments
           //cout << " Track k="<<k<<", pdgCode = "<<pdgCode<<
          //" Mass " << MCTrack->GetMass()<<","<<mfrag<<" Y " << MCTrack->GetRapidity() <<
          //" Pt " << MCTrack->GetPt() <<endl;
          fa_ptm_rap_poi_imf->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / mfrag);
          fa_v1_rap_poi_imf->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
          fa_v2_rap_poi_imf->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));
          // reaction plane determination  (optimistic)
          if (TMath::Abs((MCTrack->GetRapidity() - yrp_mid) / yrp_mid) > GetDY()
              && MCTrack->GetPt() / MCTrack->GetMass() > GetFlowMinPtm()) {
            if (MCTrack->GetRapidity() > yrp_mid) {  // set weights for reaction plane
              rp_weight = 1.;
            }
            else {
              rp_weight = -1.;
            }
          }
          break;
        };
      }
      if (rp_weight != 0.) {
        if (gRandom->Uniform(1) > 0.5) {  //subdivide events into 2 random subevents
          Np1++;
          Qx1 = Qx1 + rp_weight * MCTrack->GetPx();
          Qy1 = Qy1 + rp_weight * MCTrack->GetPy();
        }
        else {
          Np2++;
          Qx2 = Qx2 + rp_weight * MCTrack->GetPx();
          Qy2 = Qy2 + rp_weight * MCTrack->GetPy();
        }
      }
    }
  }
  if (Np1 > 0 && Np2 > 0) {
    phirp1 = atan2(Qy1, Qx1);
    phirp2 = atan2(Qy2, Qx2);
    if (fflowFile != NULL) {  // subevent RP flattening
      TH1F* phirp_poi_fpar = (TH1F*) fflowFile->Get("phirps_poi_fpar");
      Float_t dphir        = 0.;
      for (int j = 0; j < 4; j++) {
        Float_t i = (float) (j + 1);
        dphir += (-phirp_poi_fpar->GetBinContent(j) * TMath::Cos(i * phirp1)
                  + phirp_poi_fpar->GetBinContent(j + 4) * TMath::Sin(i * phirp1))
                 / i;
      }
      phirp1 += dphir;

      dphir = 0.;
      for (int j = 0; j < 4; j++) {
        Float_t i = (float) (j + 1);
        dphir += (-phirp_poi_fpar->GetBinContent(j) * TMath::Cos(i * phirp2)
                  + phirp_poi_fpar->GetBinContent(j + 4) * TMath::Sin(i * phirp2))
                 / i;
      }
      phirp2 += dphir;
    }  // subevent RP flattening end
    delrp = (phirp1 - phirp2);
    fa_phirps_poi->Fill(phirp1 * RADDEG);  // 1D histo
    fa_phirps_poi->Fill(phirp2 * RADDEG);  // 1D histo
    if (0) {                               //nh-debug
      cout << "<D-poi> Impact parameter " << fMCEventHeader->GetB() << ", delrp = " << delrp << endl;
    }
    fa_cdrp_b_poi->Fill(fMCEventHeader->GetB(), TMath::Cos(delrp));
    delrp = delrp * RADDEG;
    if (delrp < -180.) delrp += 360.;
    if (delrp > 180.) delrp -= 360.;
    fa_drp_b_poi->Fill(fMCEventHeader->GetB(), delrp);
  }

  phirp = RADDEG * atan2(Qy1 + Qy2, Qx1 + Qx2);  // full reaction plane
  while (phirp < -180.) {
    phirp += 360.;
  }
  while (phirp > 180.) {
    phirp -= 360.;
  }
  if (fflowFile != NULL) {  // RP flattening
    TH1F* phirp_poi_fpar = (TH1F*) fflowFile->Get("phirp_poi_fpar");
    Float_t dphir        = 0.;
    for (int j = 0; j < 4; j++) {
      Float_t i = (float) (j + 1);
      //cout << " RP flat par "<< i << ","<<j<< " par " << phirp_gen_fpar->GetBinContent(j)
      //     << ","<< phirp_gen_fpar->GetBinContent(j+4) << " phirp "<<phirp<<" dphir "<< dphir << endl;
      dphir += ((-phirp_poi_fpar->GetBinContent(j) * TMath::Cos(i * phirp / RADDEG)
                 + phirp_poi_fpar->GetBinContent(j + 4) * TMath::Sin(i * phirp / RADDEG))
                / i);
    }
    //cout << " phirp " << phirp << " dphir " << dphir*RADDEG << endl;

    phirp += dphir * RADDEG;
    while (phirp < -180.) {
      phirp += 360.;
    }
    while (phirp > 180.) {
      phirp -= 360.;
    }
  }  // RP flattening end
  if (NULL != fMCEventHeader) {
    delrp = phirp - RADDEG * fMCEventHeader->GetRotZ();
    while (delrp < -180.) {
      delrp += 360.;
    }
    while (delrp > 180.) {
      delrp -= 360.;
    }
    fa_phirp_poi->Fill(phirp);  // 1D histo
    fa_delrp_b_poi->Fill(fMCEventHeader->GetB(), delrp);
    fa_cdelrp_b_poi->Fill(fMCEventHeader->GetB(), TMath::Cos(delrp / RADDEG));
  }
  // TofHit   level
  for (Int_t k = 0; k < nMCTracks; k++)
    TrackP[k] = 0;  // reset track detected flag

  Qx1               = 0.;
  Qy1               = 0.;
  Np1               = 0;
  Qx2               = 0.;
  Qy2               = 0.;
  Np2               = 0;
  Int_t NT0         = 0;
  Float_t t0m_hit   = 0.;
  Int_t NT0F        = 0;
  Float_t t0mf_hit  = 0.;
  Int_t NT0NF       = 0;
  Float_t t0mnf_hit = 0.;
  if (NULL != fMCEventHeader) fa_mul_b_hit->Fill(fMCEventHeader->GetB(), nTofHits);

  Float_t T0MIN   = 0.;
  Int_t NFHITS    = 10;
  Int_t nFTofHits = 0;
  Float_t T0FAST[NFHITS];
  for (Int_t l = 0; l < NFHITS; l++) {
    T0FAST[l] = 100;
  };

  for (Int_t j = 0; j < nTofHits; j++) {
    // reset track assignment vector
    NTofHitTMul[j] = 0;
    //cout << "<D-hit> j= " << j << endl;
    TofHit = (CbmTofHit*) fTofHits->At(j);
    if (NULL == TofHit) continue;
    t_hit = TofHit->GetTime();
    if (1) {  // use everything
      //if ((TMath::Abs(TofHit->GetX())<55.&&TMath::Abs(TofHit->GetY())<55.)) { //  use region E
      //if (!(TMath::Abs(TofHit->GetX())<55.&&TMath::Abs(TofHit->GetY())<55.))  { //  exclude region E
      nFTofHits++;
      Float_t dist   = TMath::Sqrt(TMath::Power(TofHit->GetX(), 2) + TMath::Power(TofHit->GetY(), 2)
                                 + TMath::Power(TofHit->GetZ(), 2));
      Float_t t0_hit = t_hit - dist / clight;
      for (Int_t l = 0; l < NFHITS; l++) {
        if (t0_hit < T0FAST[l]) {
          //cout << "Sort fasted hits: j="<<j<<", l="<<l<<","<<t0_hit<<","<<T0FAST[l]<<endl;
          if (T0FAST[l] < 100.) {
            for (Int_t ll = NFHITS - 1; ll >= l; ll--) {
              T0FAST[ll + 1] = T0FAST[ll];
              //cout <<"Move: "<<ll<<" -> "<< ll+1 <<": "<< T0FAST[ll] << endl;
            }
          }
          T0FAST[l] = t0_hit;
          //cout<<"Sort:"; for(Int_t ll=0; ll<NFHITS; ll++){ cout << T0FAST[ll] <<",";}cout << endl;
          break;
        }
        else {
        }
      }
    }
  }
  //    cout << "Interpreting result: ";
  // T0MIN=average
  Int_t nfh    = 0;
  Int_t nfhmax = nFTofHits;  ///2;
  Int_t nT0It  = 0;
  Int_t lmin   = 0;
  Int_t lmax   = NFHITS;
  Float_t T02  = 0.;
  //    nfhmax=TMath::Max(nfhmax,3);
  for (Int_t l = 0; l < NFHITS; l++) {
    if (T0FAST[l] < 100. && nfh < nfhmax) {
      lmax = l;
      //       cout  << T0FAST[l] << ", ";
      T0MIN += T0FAST[l];
      T02 += TMath::Power(T0FAST[l], 2.);
      nfh++;
    }
  }
  T0MIN         = T0MIN / (Float_t) nfh;
  Float_t T0RMS = TMath::Sqrt(T02 - T0MIN * T0MIN);
  //    cout << " T0MIN = "<<T0MIN << ", T0RMS = "<< T0RMS << ", nfh = "<<nfh
  //     << " lmin = "<<lmin << ", lmax = "<<lmax <<endl;

  while (T0RMS > 1. && nT0It < 10 && nfh > 2) {
    nT0It++;
    Float_t T0AV = T0MIN;
    if (TMath::Abs(T0FAST[lmin] - T0AV) > TMath::Abs(T0FAST[lmax] - T0AV)) {
      //remove first entry
      lmin++;
    }
    else {  // remove last entry
      lmax--;
    }
    T0MIN = 0.;
    nfh   = 0;
    T02   = 0.;
    for (Int_t l = lmin; l < lmax; l++) {
      if (T0FAST[l] < 100. && nfh < nfhmax) {
        //cout  << T0FAST[l] << ", ";
        T0MIN += T0FAST[l];
        T02 += TMath::Power(T0FAST[l], 2.);
        nfh++;
      }
    }
    T0MIN = T0MIN / (Float_t) nfh;
    T0RMS = TMath::Sqrt(T02 - T0MIN * T0MIN);
    //cout << "Redo("<<nT0It<<"): T0MIN = "<<T0MIN << ", T0RMS = "<< T0RMS << ", nfh = "
    //     << nfh << " lmin = "<<lmin << ", lmax = "<<lmax <<endl;
  }

  Int_t lp = -1;
  for (Int_t j = 0; j < nTofHits; j++) {
    //cout << "<D-hit> j= " << j << endl;
    TofHit = (CbmTofHit*) fTofHits->At(j);
    if (NULL == TofHit) continue;
    //      Int_t l = TofHit->GetRefId();  // pointer to Digi
    Int_t l = j;  // One CbmMatch per hit and in same order!!!!
    if (fTofDigiMatchColl != NULL && fTofDigiMatchPointsColl != NULL) {
      CbmMatch* digiMatch = (CbmMatch*) fTofDigiMatchColl->At(l);
      // take first digi's point link
      CbmLink L0         = digiMatch->GetLink(0);
      Int_t iDigInd0     = L0.GetIndex();
      CbmMatch* poiMatch = (CbmMatch*) fTofDigiMatchPointsColl->At(iDigInd0);
      if (NULL == poiMatch) {
        LOG(warn) << "No MC point found for hit " << j << ", digi " << iDigInd0;
        continue;
      }
      CbmLink LP = poiMatch->GetMatchedLink();
      lp         = LP.GetIndex();
      /*
       for (Int_t iLink=0; iLink<digiMatch->GetNofLinks(); iLink+=2){  // loop over digis
           CbmLink L0 = digiMatch->GetLink(iLink);   //vDigish.at(ivDigInd);
	   Int_t iDigInd0=L0.GetIndex();
           Int_t iDigInd1=(digiMatch->GetLink(iLink+1)).GetIndex(); //vDigish.at(ivDigInd+1);
           LOG(debug1)<<" " << iDigInd0<<", "<<iDigInd1;

           if (iDigInd0 < fTofDigisColl->GetEntriesFast() && iDigInd1 < fTofDigisColl->GetEntriesFast()){
            CbmTofDigiExp *pDig0 = (CbmTofDigiExp*) (fTofDigisColl->At(iDigInd0));
            CbmTofDigiExp *pDig1 = (CbmTofDigiExp*) (fTofDigisColl->At(iDigInd1));
	   }
       }
       */
    }
    else {
      lp = -1;
      LOG(debug) << "No Link to MCTofPoint found for hit " << j;
      continue;
    }
    TofPoint = (CbmTofPoint*) fTofPoints->At(lp);
    Int_t k  = TofPoint->GetTrackID();
    MCTrack  = (CbmMCTrack*) fMCTracksColl->At(k);
    pdgCode  = MCTrack->GetPdgCode();
    px_MC    = MCTrack->GetPx();
    py_MC    = MCTrack->GetPy();
    pz_MC    = MCTrack->GetPz();
    p_MC     = sqrt(px_MC * px_MC + py_MC * py_MC + pz_MC * pz_MC);
    if (k > 100000) {
      cout << "<E-hit> Too many MCTracks " << k << " from " << nMCTracks << endl;
      continue;
    }
    //cout << "<D-hit> k= " << k << endl;
    //cout << Form("HadronAnalysis:: hit %d, poi %d, MCt %d, Eff %d ",j,lp,k,TrackP[k]) << endl;
    /*
      if(TofHit->GetX()<-90.) { //nh-debug
	LOG(INFO)  << Form(" Invalid Hit in ev %d: %6.1f, %6.1f, %6.1f, poi: %6.1f, %6.1f, %6.1f, pdg: %d",
			   fEvents,TofHit->GetX(),TofHit->GetY(),TofHit->GetZ(),
			   TofPoint->GetX(),TofPoint->GetY(),TofPoint->GetZ(),
			   pdgCode);
	LOG(FATAL) << "<D-hit> j " << j << ", l " << l << ", k " << k << ", lp ";
      }
      */
    if (TrackP[k] == 0) {  // for efficiency
                           //     if (1) {
      TrackP[k]++;
      t_hit             = TofHit->GetTime();
      Float_t delta_tof = TofPoint->GetTime() - t_hit;
      Float_t delta_x   = TofPoint->GetX() - TofHit->GetX();
      Float_t delta_y   = TofPoint->GetY() - TofHit->GetY();
      Float_t delta_z   = TofPoint->GetZ() - TofHit->GetZ();

      fa_dxx->Fill(TofPoint->GetX(), delta_x);
      fa_dxy->Fill(TofPoint->GetY(), delta_x);
      fa_dxz->Fill(TofPoint->GetZ(), delta_x);
      fa_dyx->Fill(TofPoint->GetX(), delta_y);
      fa_dyy->Fill(TofPoint->GetY(), delta_y);
      fa_dyz->Fill(TofPoint->GetZ(), delta_y);
      fa_dzx->Fill(TofPoint->GetX(), delta_z);
      fa_dzy->Fill(TofPoint->GetY(), delta_z);
      fa_dzz->Fill(TofPoint->GetZ(), delta_z);

      fa_xy_hit1->Fill(TofHit->GetX(), TofHit->GetY());
      fa_xy_hit2->Fill(TofHit->GetX(), TofHit->GetY(), fwxy2);
      fa_hit_ch->Fill(TofHit->GetCh());
      fa_dhit_ch->Fill(TofHit->GetCh(), TofHit->GetFlag());

      Float_t vel = TofPoint->GetLength() / t_hit;
      Float_t bet = vel / clight;
      if (bet > 0.99999) { bet = 0.99999; }
      Float_t tofmass = p_MC / bet * TMath::Sqrt(1. - bet * bet) * TMath::Sign(1, pdgCode);
      Float_t dist    = TMath::Sqrt(TMath::Power(TofHit->GetX(), 2) + TMath::Power(TofHit->GetY(), 2)
                                 + TMath::Power(TofHit->GetZ(), 2));
      fa_tof_hit->Fill(t_hit);
      fa_pv_hit->Fill(vel, p_MC);
      fa_tm_hit->Fill(p_MC, tofmass);
      fa_dtof_hit->Fill(delta_tof);
      Float_t t0_hit = t_hit - dist / clight;
      fa_t0_hit->Fill(t0_hit);
      //if(t0_hit<MaxT0) {
      if (t0_hit < T0MIN + 2.4 * MaxT0) {
        NT0++;
        t0m_hit = ((Float_t)(NT0 - 1) * t0m_hit + t0_hit) / (Float_t) NT0;
        if ((TMath::Abs(TofHit->GetX()) < 55. && TMath::Abs(TofHit->GetY()) < 55.)) {  //  region E
          NT0F++;
          t0mf_hit = ((Float_t)(NT0F - 1) * t0mf_hit + t0_hit) / (Float_t) NT0F;
        }
        else {
          NT0NF++;
          t0mnf_hit = ((Float_t)(NT0NF - 1) * t0mnf_hit + t0_hit) / (Float_t) NT0NF;
        }
      }

      if (MCTrack->GetMotherId() != -1) continue;  // primary particles only
      if (TrackP[k] > 1) continue;                 // for efficiency consider only first hit of track
      fa_tm_hitprim->Fill(p_MC, tofmass);
      fa_tof_hitprim->Fill(t_hit);

      Float_t Phip = RADDEG * atan2(MCTrack->GetPy(), MCTrack->GetPx());
      Float_t dphi = Phip - RADDEG * fMCEventHeader->GetRotZ();
      if (dphi < -180.) { dphi += 360.; };
      if (dphi > 180.) { dphi -= 360.; };
      dphi = dphi / RADDEG;

      rp_weight = 0.;

      switch (pdgCode) {
        case 211: {
          fa_ptm_rap_hit_pip->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
          fa_v1_rap_hit_pip->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
          fa_v2_rap_hit_pip->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));

          if (use_pions_for_flow && TMath::Abs((MCTrack->GetRapidity() - yrp_mid) / yrp_mid) > GetDY()
              && MCTrack->GetPt() / MCTrack->GetMass() > GetFlowMinPtm()) {
            if (MCTrack->GetRapidity() > yrp_mid) {  // set weights for reaction plane
              rp_weight = -1.;
            }
            else {
              rp_weight = +1.;
            }
          }
          else {
            rp_weight = 0.;
          }
          break;
        };
        case -211: {
          fa_ptm_rap_hit_pim->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
          fa_v1_rap_hit_pim->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
          fa_v2_rap_hit_pim->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));

          if (use_pions_for_flow && TMath::Abs((MCTrack->GetRapidity() - yrp_mid) / yrp_mid) > GetDY()
              && MCTrack->GetPt() / MCTrack->GetMass() > GetFlowMinPtm()) {
            if (MCTrack->GetRapidity() > yrp_mid) {  // set weights for reaction plane
              rp_weight = -1.;
            }
            else {
              rp_weight = +1.;
            }
          }
          else {
            rp_weight = 0.;
          }
          break;
        };
        case 321: {
          fa_ptm_rap_hit_kp->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
          fa_v1_rap_hit_kp->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
          fa_v2_rap_hit_kp->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));
          break;
        };
        case -321: {
          fa_ptm_rap_hit_km->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
          fa_v1_rap_hit_km->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
          fa_v2_rap_hit_km->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));
          break;
        };
        case 2212: {
          fa_ptm_rap_hit_p->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
          fa_v1_rap_hit_p->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
          fa_v2_rap_hit_p->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));
          // reaction plane determination
          if (TMath::Abs((MCTrack->GetRapidity() - yrp_mid) / yrp_mid) > GetDY()
              && MCTrack->GetPt() / MCTrack->GetMass() > GetFlowMinPtm()) {
            if (MCTrack->GetRapidity() > yrp_mid) {  // set weights for reaction plane
              rp_weight = +1.;
            }
            else {
              rp_weight = -1.;
            }
          }
          else {
            rp_weight = 0.;
          }
          break;
        };
        case -2212: {
          fa_ptm_rap_hit_pbar->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
          fa_v1_rap_hit_pbar->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
          fa_v2_rap_hit_pbar->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));
          break;
        };

        case 1000010020: {  // deuteron
          fa_ptm_rap_hit_d->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
          fa_v1_rap_hit_d->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
          fa_v2_rap_hit_d->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));
          // reaction plane determination
          if (TMath::Abs((MCTrack->GetRapidity() - yrp_mid) / yrp_mid) > GetDY()
              && MCTrack->GetPt() / MCTrack->GetMass() > GetFlowMinPtm()) {
            if (MCTrack->GetRapidity() > yrp_mid) {  // set weights for reaction plane
              rp_weight = 1.;
            }
            else {
              rp_weight = -1.;
            }
          }
          break;
        };

        case 1000010030: {  // triton
          fa_ptm_rap_hit_t->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
          fa_v1_rap_hit_t->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
          fa_v2_rap_hit_t->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));
          // reaction plane determination
          if (TMath::Abs((MCTrack->GetRapidity() - yrp_mid) / yrp_mid) > GetDY()
              && MCTrack->GetPt() / MCTrack->GetMass() > GetFlowMinPtm()) {
            if (MCTrack->GetRapidity() > yrp_mid) {  // set weights for reaction plane
              rp_weight = 1.;
            }
            else {
              rp_weight = -1.;
            }
          }
          break;
        };

        case 1000020030: {  // 3he
          fa_ptm_rap_hit_h->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
          fa_v1_rap_hit_h->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
          fa_v2_rap_hit_h->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));
          // reaction plane determination
          if (TMath::Abs((MCTrack->GetRapidity() - yrp_mid) / yrp_mid) > GetDY()
              && MCTrack->GetPt() / MCTrack->GetMass() > GetFlowMinPtm()) {
            if (MCTrack->GetRapidity() > yrp_mid) {  // set weights for reaction plane
              rp_weight = 1.;
            }
            else {
              rp_weight = -1.;
            }
          }
          break;
        };
        case 1000020040: {  // alpha
          fa_ptm_rap_hit_a->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
          fa_v1_rap_hit_a->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
          fa_v2_rap_hit_a->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));
          // reaction plane determination
          if (TMath::Abs((MCTrack->GetRapidity() - yrp_mid) / yrp_mid) > GetDY()
              && MCTrack->GetPt() / MCTrack->GetMass() > GetFlowMinPtm()) {
            if (MCTrack->GetRapidity() > yrp_mid) {  // set weights for reaction plane
              rp_weight = 1.;
            }
            else {
              rp_weight = -1.;
            }
          }
          break;
        };

        default:
        {  // intermediate mass fragments
           //cout << " Track k="<<k<<", pdgCode = "<<pdgCode<<
          //" Mass " << MCTrack->GetMass()<<","<<MCTrack->GetMass()<<" Y " << MCTrack->GetRapidity() <<
          //" Pt " << MCTrack->GetPt() <<endl;
          fa_ptm_rap_hit_imf->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
          fa_v1_rap_hit_imf->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
          fa_v2_rap_hit_imf->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));
          // reaction plane determination  (optimistic)
          if (TMath::Abs((MCTrack->GetRapidity() - yrp_mid) / yrp_mid) > GetDY()
              && MCTrack->GetPt() / MCTrack->GetMass() > GetFlowMinPtm()) {
            if (MCTrack->GetRapidity() > yrp_mid) {  // set weights for reaction plane
              rp_weight = 1.;
            }
            else {
              rp_weight = -1.;
            }
          }
          break;
        };
      }

      if (rp_weight != 0.) {
        if (gRandom->Uniform(1) > 0.5) {  //subdivide events into 2 random subevents
          Np1++;
          Qx1 = Qx1 + rp_weight * MCTrack->GetPx();
          Qy1 = Qy1 + rp_weight * MCTrack->GetPy();
        }
        else {
          Np2++;
          Qx2 = Qx2 + rp_weight * MCTrack->GetPx();
          Qy2 = Qy2 + rp_weight * MCTrack->GetPy();
        }
      }
    }
    else {
      if (0) {
        cout << "<W> CHA: >=2. hit from track k =" << k << " Hit# "
             << j
             //   << " Str(Cell) "<< TofHit->GetCell()    << "," << TofPoint->GetCell()
             //   << " Module "   << TofHit->GetModule()  << "," << TofPoint->GetModule()
             //   << " SM "       << TofHit->GetSModule() << "," << TofPoint->GetSModule()
             //   << " SMType "   << TofHit->GetSMtype()  << "," << TofPoint->GetSMtype()
             << endl;
      }
    }
  }
  if (Np1 > 0 && Np2 > 0) {
    phirp1 = atan2(Qy1, Qx1);
    phirp2 = atan2(Qy2, Qx2);
    if (fflowFile != NULL) {  // subevent RP flattening
      TH1F* phirp_hit_fpar = (TH1F*) fflowFile->Get("phirps_hit_fpar");
      Float_t dphir        = 0.;
      for (int j = 0; j < 4; j++) {
        Float_t i = (float) (j + 1);
        dphir += (-phirp_hit_fpar->GetBinContent(j) * TMath::Cos(i * phirp1)
                  + phirp_hit_fpar->GetBinContent(j + 4) * TMath::Sin(i * phirp1))
                 / i;
      }
      phirp1 += dphir;

      dphir = 0.;
      for (int j = 0; j < 4; j++) {
        Float_t i = (float) (j + 1);
        dphir += (-phirp_hit_fpar->GetBinContent(j) * TMath::Cos(i * phirp2)
                  + phirp_hit_fpar->GetBinContent(j + 4) * TMath::Sin(i * phirp2))
                 / i;
      }
      phirp2 += dphir;
    }                                      // subevent RP flattening end
    fa_phirps_hit->Fill(phirp1 * RADDEG);  // 1D histo
    fa_phirps_hit->Fill(phirp2 * RADDEG);  // 1D histo
    delrp = (phirp1 - phirp2);
    if (NULL != fMCEventHeader) {
      if (0) {  //nh-debug
        cout << "<D-hit> Impact parameter " << fMCEventHeader->GetB() << ", delrp = " << delrp << endl;
      }
      fa_cdrp_b_hit->Fill(fMCEventHeader->GetB(), TMath::Cos(delrp));
      delrp = delrp * RADDEG;
      if (delrp < -180.) delrp += 360.;
      if (delrp > 180.) delrp -= 360.;
      fa_drp_b_hit->Fill(fMCEventHeader->GetB(), delrp);
    }
  }
  phirp = RADDEG * atan2(Qy1 + Qy2, Qx1 + Qx2);  // full reaction plane
  while (phirp < -180.) {
    phirp += 360.;
  }
  while (phirp > 180.) {
    phirp -= 360.;
  }
  if (fflowFile != NULL) {  // RP flattening
    TH1F* phirp_hit_fpar = (TH1F*) fflowFile->Get("phirp_hit_fpar");
    Float_t dphir        = 0.;
    for (int j = 0; j < 4; j++) {
      Float_t i = (float) (j + 1);
      //cout << " RP flat par "<< i << ","<<j<< " par " << phirp_gen_fpar->GetBinContent(j)
      //     << ","<< phirp_gen_fpar->GetBinContent(j+4) << " phirp "<<phirp<<" dphir "<< dphir << endl;
      dphir += ((-phirp_hit_fpar->GetBinContent(j) * TMath::Cos(i * phirp / RADDEG)
                 + phirp_hit_fpar->GetBinContent(j + 4) * TMath::Sin(i * phirp / RADDEG))
                / i);
    }
    //cout << " phirp " << phirp << " dphir " << dphir*RADDEG << endl;

    phirp += dphir * RADDEG;
    while (phirp < -180.) {
      phirp += 360.;
    }
    while (phirp > 180.) {
      phirp -= 360.;
    }
  }  // RP flattening end
  if (NULL != fMCEventHeader) {
    delrp = phirp - RADDEG * fMCEventHeader->GetRotZ();
    while (delrp < -180.) {
      delrp += 360.;
    }
    while (delrp > 180.) {
      delrp -= 360.;
    }
    fa_phirp_hit->Fill(phirp);  // 1D histo
    fa_delrp_b_hit->Fill(fMCEventHeader->GetB(), delrp);
    fa_cdelrp_b_hit->Fill(fMCEventHeader->GetB(), TMath::Cos(delrp / RADDEG));

    fa_tn_hit->Fill(T0MIN);
    fa_t0mn_hit->Fill((Float_t) NT0);
    fa_t0mn_b_hit->Fill(fMCEventHeader->GetB(), (Float_t) NT0);
    if (NT0 > 0) {
      fa_t0m_hit->Fill(t0m_hit);
      fa_t0m_b_hit->Fill(fMCEventHeader->GetB(), t0m_hit);
    }

    fa_t0mn_f_hit->Fill((Float_t) NT0F);
    fa_t0mn_f_b_hit->Fill(fMCEventHeader->GetB(), (Float_t) NT0F);
    if (NT0F > 0) {
      fa_t0m_f_hit->Fill(t0mf_hit);
      fa_t0m_f_b_hit->Fill(fMCEventHeader->GetB(), t0mf_hit);
    }
    fa_t0mn_nf_hit->Fill((Float_t) NT0NF);
    fa_t0mn_nf_b_hit->Fill(fMCEventHeader->GetB(), (Float_t) NT0NF);
    if (NT0NF > 0) {
      fa_t0m_nf_hit->Fill(t0mnf_hit);
      fa_t0m_nf_b_hit->Fill(fMCEventHeader->GetB(), t0mnf_hit);
    }
  }
  //    cout << "<I> CbmHadronAnalysis: Number of T0 particles: NTO = " << NT0 << endl;
  ////////////////////////////////////////////////////////////////////////////////////////////
  // GlobalTrack level analysis

  const Double_t DISTMAX = 100.;
  Double_t WMAX          = 10.;  //0.02;
  if (fWMax != 0.) WMAX = fWMax;

  Int_t NGTofTrack = 0;
  Qx1              = 0.;
  Qy1              = 0.;
  Np1              = 0;
  Qx2              = 0.;
  Qy2              = 0.;
  Np2              = 0;
  if (NULL != fMCEventHeader) fa_mul_b_glo->Fill(fMCEventHeader->GetB(), nGlobTracks);

  Int_t NReas = 0;  //100;  // activate reassignment of hits to global tracks
  Int_t NRIt  = 0;
  while (NReas > 0) {
    NRIt++;
    if (verbose > 5) {  // nh-debug readability
      cout << endl;
      cout << Form(" TofTrack selection: %d. iteration, reassigned hits: %d, "
                   "GlobTracks %d, TofTracks %d",
                   NRIt, NReas, nGlobTracks, nTofTracks)
           << endl;
    }
    NReas = 0;
    for (Int_t i = 0; i < nGlobTracks; i++) {  // inspect global tracks
      GlobTrack = (CbmGlobalTrack*) fGlobalTracks->At(i);
      if (NRIt == 1) NTHMUL[i] = 0;  // number of TofTracks assigned to GlobTrack
      Int_t Btt  = -1;               // best  TofTrack index
      Int_t Bthi = -1;               // best TofHit index

      Int_t s = GlobTrack->GetStsTrackIndex();
      Int_t j = GlobTrack->GetTofHitIndex();

      if (verbose > 10) {  // nh-debug
        cout << "<Di> NRIt " << NRIt << ": Global Track " << i << ", TofHit " << j << " StsTrk " << s << endl;
      }

      const FairTrackParam* tparf = GlobTrack->GetParamFirst();
      if (0 == tparf->GetQp()) {
        if (verbose > 10) {  // nh-debug
          cout << "<W> Global Track " << i << " without Qp!, take from Sts " << s << endl;
        }
        StsTrack = (CbmStsTrack*) fStsTracks->At(s);
        if (NULL == StsTrack) {
          cout << "<E> Invalid StsTrack pointer at location " << s << endl;
          continue;
        }
        GlobTrack->SetParamFirst(StsTrack->GetParamFirst());
      }
      tparf = GlobTrack->GetParamFirst();
      if (0 == tparf->GetQp()) {
        cout << "<E> Global Track " << i << " without Qp! " << endl;
        continue;
      }

      if (211 != StsTrack->GetPidHypo()) {
        cout << "<E> Invalid StsTrack PID " << StsTrack->GetPidHypo() << " at " << s << endl;
        continue;
      }

      FairTrackParam paramExtr;
      fTrackFitter.FitToVertex(StsTrack, fPrimVertex, &paramExtr);
      // GlobTrack -> SetParamFirst(&paramExtr);         // nh: attach track parameter to global track
      //     cout <<Form("<D> Extrapolate StsTrack %d with PidHypo %6.0f to vertex (%5.2f,%5.2f,%5.2f)",
      //	  s,StsTrack->GetPidHypo(),fPrimVertex->GetX(),fPrimVertex->GetY(),fPrimVertex->GetZ())	<<endl;
      Double_t vtxb = fTrackFitter.GetChiToVertex(StsTrack, fPrimVertex);  //impact paramter ???
      if (verbose > 10) {                                                  // nh-debug
        cout << Form("<D> Extrapolate Glob Track %d to prim. vertex %6.2f with "
                     "chi2 %6.2f",
                     i, fPrimVertex->GetZ(), vtxb)
             << endl;
        //GlobTrack->GetParamFirst()->Print();
      }


      Float_t momf = 1. / tparf->GetQp();
      if (momf < 0.) momf = -momf;  // positive momentum at vertex

      Float_t dist = 0.;

      if (j > -1) {  // TOF Track analysis

        if (NRIt == 1) {
          Float_t DISTMIN    = WMAX;
          Int_t nth          = -1;                     // number of TofHits for global track i
          Weight_THMUL[i][0] = WMAX;                   // initialize limit
          for (Int_t tt = 0; tt < nTofTracks; tt++) {  // loop over all TofTracks
            TofTrack = (CbmTofTrack*) fTofTracks->At(tt);
            if (i == TofTrack->GetTrackIndex()) {  // select TofTrack belonging to global track i
              if (verbose > 10) {                  // nh-debug
                cout << "<Dt> Global Track " << i << ", TofHit " << j << ", StsTrk " << s << ", TofTrack " << tt
                     << endl;
              }

              Int_t thi = TofTrack->GetTofHitIndex();
              TofHit    = (CbmTofHit*) fTofHits->At(thi);
              if (NULL == TofHit) continue;

              dist = TofTrack->GetDistance();

              if (isinf(dist)) {
                cout << "<E>  invalid dist for gt " << i << ",  tt " << tt << ", d:" << dist << endl;
                break;
              }
              /*
	  fhTofTrkDx->Fill(TofTrack->GetTrackDx());
	  fhTofTrkDy->Fill(TofTrack->GetTrackDy());
	  */

              //dist=TMath::Abs(TMath::Abs(dist)-0.5);

              const FairTrackParam* tpar = TofTrack->GetTrackParameter();
              //cout << " Inspect TrackParameter  "; tpar->Print();
              Float_t moml = 1. / tpar->GetQp();
              if (moml < 0.) moml = -moml;
              Float_t bet = TofHit->GetR() / TofHit->GetTime() / clight;
              //nh-inconsistent (?), TrackLength  needs to be determined experimentally
              if (bet > 0.99999) { bet = 0.99999; }
              Float_t tofmass = momf / bet * sqrt(1. - bet * bet) * TMath::Sign(1., tpar->GetQp());
              TofTrack->SetMass(tofmass);  // store tofmass as part of PID info
              if (TofTrack->GetMass() != tofmass) { cout << "<E> did not store tofmass properly " << tofmass << endl; }
              // calculate attachment weight, to be refined ... (nh, 03.01.2014)
              Float_t mindelmass = 1.E6;
              Float_t minlen;
              Int_t immin;  //          minlen  = (TofTrack->GetTrackLength()-MinWallDist)/MinWallDist;
              minlen     = (TofHit->GetR() - MinWallDist) / MinWallDist;
              mindelmass = 1.E6;
              immin      = 0;
              for (Int_t im = 0; im < NMASS; im++) {
                Float_t delmass = TMath::Abs(TMath::Abs(TofTrack->GetMass()) - refMass[im]);
                if (delmass < mindelmass) {
                  mindelmass = delmass;
                  immin      = im;
                }
              }
              mindelmass /= refMass[immin];
              Float_t delp = TMath::Abs((momf - moml) / momf);
              //	  Float_t w = dist * mindelmass * minlen * delp * 10000.;
              Float_t w = dist;
              //Float_t w = minlen;
              //Float_t w = mindelmass ;
              //Float_t w = delp;
              //Float_t w = TMath::Sqrt((dist*dist +  delp*delp)/2.);
              //Float_t w = TMath::Sqrt((dist*dist +  mindelmass*mindelmass)/2.);
              //Float_t w = TMath::Sqrt((dist*dist + delp*delp + mindelmass*mindelmass + minlen*minlen)/4.);
              //Float_t w = dist * mindelmass * minlen ;
              //Float_t w = dist * mindelmass * 15.;
              //Float_t w = dist * mindelmass *delp;
              //Float_t w = dist * delp;

              fhwdist->Fill(momf, dist);
              fhwmindelmass->Fill(momf, mindelmass);
              fhwminlen->Fill(momf, minlen);
              fhwdelp->Fill(momf, delp);
              if (verbose > 5) {
                cout << Form("<D> w for gt %3d, tt %3d, w: %9.5f, d: %7.2f, m: "
                             "%7.3f, l: %7.2f, dp: %7.3f, p: %7.2f ",
                             i, tt, w, dist, mindelmass, minlen, delp, momf)
                     << endl;
              }
              if (w < WMAX) {
                nth++;
                if (nth == MAXNHT) {
                  if (verbose > 1) {
                    cout << "<W> Too many TofTrack candidates for track " << i << ", limit to " << nth << endl;
                  }
                  nth = MAXNHT - 1;
                }
                // sort TofTracks according to weight into array IndTHMUL
                Int_t jthmin = nth;
                for (Int_t jth = 0; jth < nth; jth++) {  //determine position in array
                  if (verbose > 10) {
                    cout << " Compare for position " << jth << " w " << w << " - " << Weight_THMUL[i][jth] << endl;
                  }
                  if (w < Weight_THMUL[i][jth]) {
                    jthmin = jth;
                    break;
                  }
                }
                if (verbose > 10) {
                  cout << " Put Track " << tt << " with w = " << w << " to position " << jthmin << " of " << nth
                       << endl;
                }
                for (Int_t jth = nth; jth > jthmin; jth--) {  // save old entries
                  if (verbose > 10) {
                    cout << " Save Track " << IndTHMUL[i][jth - 1] << " with w " << Weight_THMUL[i][jth - 1]
                         << " to position " << jth << endl;
                  }
                  IndTHMUL[i][jth]     = IndTHMUL[i][jth - 1];
                  Weight_THMUL[i][jth] = Weight_THMUL[i][jth - 1];
                }
                IndTHMUL[i][jthmin]     = tt;  // store index  of TofTrack
                Weight_THMUL[i][jthmin] = w;   // store weight of TofTrack

                if (w < DISTMIN) {
                  DISTMIN      = w;
                  BestTofTrack = TofTrack;
                  Bthi         = thi;  // best TofHit index
                  Btt          = tt;   // best TofTrack index
                  if (verbose > 5) { cout << Form("<DMin> gt %d, hit %d, tt %d, w: %6.2f", i, Bthi, Btt, w) << endl; }
                }
              }  //w < WMAX end
              if (verbose > 10) { cout << Form("<D> tt-loop: gt %d, tt %d, w: %6.2f", i, tt, w) << endl; }
            }                   // (i==TofTrack->GetTrackIndex())
          }                     // inspection of all TofTracks finished
          NTHMUL[i] = nth + 1;  // number of TofHit candidates
          fa_TofTrackMul->Fill(NTHMUL[i]);

          // report summary:
          if (verbose > 5) {
            for (Int_t k = 0; k < NTHMUL[i]; k++) {
              if (verbose > 3) {
                cout << Form("<Ddeb> i %d, k %d, M %d, Ind %d ", i, k, NTHMUL[i], IndTHMUL[i][k]) << endl;
              }

              TofTrack = (CbmTofTrack*) fTofTracks->At(IndTHMUL[i][k]);
              cout << "<DSum> GlobTrack " << i << ", TMul: " << NTHMUL[i] << ", TofTrack " << IndTHMUL[i][k]
                   << ", TofHit " << TofTrack->GetTofHitIndex() << ", TMul_hit "
                   << NTofHitTMul[TofTrack->GetTofHitIndex()] << ", dist " << TofTrack->GetDistance() << ", len "
                   << TofTrack->GetTrackLength() << ", R " << ((CbmTofHit*) fTofHits->At(j))->GetR() << ", mass "
                   << TofTrack->GetMass() << ", mom " << momf << ", w " << Weight_THMUL[i][k] << endl;
            }
          }
        }
        else {  // NRIt>1; initialize from array
          if (NTHMUL[i] > 0) {
            Btt = IndTHMUL[i][0];
            if (Btt < 0 || Btt > fTofTracks->GetEntriesFast()) {
              cout << "<E> invalid TofTrackIndex " << Btt << ", gt " << i << ", NRIt " << NRIt << endl;
              Btt = -1;
              continue;
            }

            BestTofTrack = (CbmTofTrack*) fTofTracks->At(Btt);
            Bthi         = BestTofTrack->GetTofHitIndex();
            if (verbose > 5) {
              cout << "<DBest> GloBTrack " << i << ", TMul: " << NTHMUL[i] << ", TofTrack " << IndTHMUL[i][0]
                   << ", TofHit " << BestTofTrack->GetTofHitIndex() << ", TMul_hit "
                   << NTofHitTMul[BestTofTrack->GetTofHitIndex()] << ", dist " << BestTofTrack->GetDistance()
                   << ", len " << BestTofTrack->GetTrackLength() << ", mass " << BestTofTrack->GetMass() << ", w "
                   << Weight_THMUL[i][0] << endl;
            }
          }
        }
      }  //(j > -1) end

      // now do global distribution of TofHits to GlobalTracks
      // attach BestTofTrack candidate to GlobalTrack
      if (verbose > 10) {
        cout << Form("<Ddis> NRIt %d, gt %d, BestTofTrack j=%d, best 0x%p, %d, "
                     "%d, w: %7.2f ",
                     NRIt, i, j, BestTofTrack, Btt, Bthi, Weight_THMUL[i][0])
             << endl;
      }

      if (NRIt == 1) GlobTrack->SetLength(0.);
      if (Btt > -1)
        while (j > -1 && GlobTrack->GetLength() != BestTofTrack->GetTrackLength()) {
          if (verbose > 10) {
            cout << Form("<Ddeb> BestTofTrack j=%d, best 0x%p, %d", j, BestTofTrack, BestTofTrack->GetTofHitIndex())
                 << endl;
          }
          dist = BestTofTrack->GetDistance();
          if (isinf(dist)) {
            cout << "<E2>  invalid dist for gt " << i << ",  Btt " << Btt << ", d:" << dist << endl;
            break;
          }
          if (dist < DISTMAX && Weight_THMUL[i][0] < WMAX) {
            Int_t jh  = NTofHitTMul[Bthi]++;
            Int_t nht = NTofHitTMul[Bthi];
            if (nht == MAXNHT) {
              if (verbose > -1) { cout << "<E> Too many TofTrack candidates for hit " << Bthi << ", break!" << endl; }
              break;
            }
            IndTofTrack_TofHit[Bthi][jh] = Btt;  // index of TofTrack assigned to specific TofHit
            if (verbose > 3) {
              cout << "<Ias> GlobTrack " << i << " -> TofTrack " << Btt << ", TofHitIndex " << Bthi << ", TMul_hit "
                   << nht << endl;
            }

            Int_t io = -1;
            if (NTofHitTMul[BestTofTrack->GetTofHitIndex()] > 1) {
              CbmTofTrack* TofTracko =
                (CbmTofTrack*) fTofTracks->At(IndTofTrack_TofHit[BestTofTrack->GetTofHitIndex()][0]);
              io = TofTracko->GetTrackIndex();  // Global Track index
              if (verbose > 2) {                // nh-debug
                cout << "<D> GlobTrack " << i << ": update TofHitIndex from " << j << " (Mul " << NTofHitTMul[j] << ") "
                     << " to " << BestTofTrack->GetTofHitIndex() << " (Mul "
                     << NTofHitTMul[BestTofTrack->GetTofHitIndex()] << ")"
                     << ", m  " << BestTofTrack->GetMass() << ", w  " << Weight_THMUL[i][0] << ", cur: tt "
                     << IndTofTrack_TofHit[Bthi][0] << ", gt " << io << ", w " << Weight_THMUL[io][0] << " ? " << endl;
              }

              // decide now!
              if (Weight_THMUL[i][0] < Weight_THMUL[io][0]) {  // new assignment better than old one -> change
                if (verbose > 1) {                             //nh-debug
                  cout << "<D> New cand. is better, invalidate entry for gt " << io << endl;
                }
                NReas++;
                NTofHitTMul[Bthi]--;                // deregister old toftrack
                IndTofTrack_TofHit[Bthi][0] = Btt;  // update
                CbmGlobalTrack* GlobTrack2  = (CbmGlobalTrack*) fGlobalTracks->At(io);
                GlobTrack2->SetLength(0.);  // signal entry invalid
              }
              else {                // old assignment better than current candidate
                if (verbose > 0) {  //nh-debug
                  cout << Form("<D> Stick to old assignment, Bthi %d, TM %d, THM %d", Bthi, NTofHitTMul[Bthi],
                               NTHMUL[i])
                       << endl;
                }
                NTofHitTMul[Bthi]--;  // deregister toftrack
                if (NTHMUL[i] > 1) {  // take next one from list
                  NTHMUL[i]--;
                  for (Int_t jth = 0; jth < NTHMUL[i]; jth++) {  // shift  old entries
                    IndTHMUL[i][jth]     = IndTHMUL[i][jth + 1];
                    Weight_THMUL[i][jth] = Weight_THMUL[i][jth + 1];
                  }
                  Btt          = IndTHMUL[i][0];  // next  best TofTrack index
                  BestTofTrack = (CbmTofTrack*) fTofTracks->At(Btt);
                  Bthi         = BestTofTrack->GetTofHitIndex();  // next best TofHit index
                }
                else {                // no other candidate available
                  if (verbose > 0) {  //nh-debug
                    cout << "<I> no TofTrack candidate for Global Track " << i << endl;
                  }
                  //		BestTofTrack->Delete();
                  Bthi = -1;
                  Btt  = -1;
                  GlobTrack->SetTofHitIndex(-1);
                  j = -1;
                  continue;
                }
                if (verbose > 0) {  //nh-debug
                  cout << "<D> Old choice better, current options: NTHMUL " << NTHMUL[i]
                       << ", take btt = " << IndTHMUL[i][0] << ", bthi " << Bthi << endl;
                }
              }

              if (NTofHitTMul[BestTofTrack->GetTofHitIndex()] > 1) {
                if (verbose > -1) {
                  cout << "<E> GlobTrack " << i
                       << ": double assignment of hit, check all possibilities "
                          "... "
                       << endl;
                  continue;
                }
              }
              j = BestTofTrack->GetTofHitIndex();
              GlobTrack->SetTofHitIndex(j);  // update Global Track info
            }
            GlobTrack->SetParamLast(BestTofTrack->GetTrackParameter());
            GlobTrack->SetLength(BestTofTrack->GetTrackLength());
          }
          else {
            if (verbose > 3) {
              cout << "<D> GlobTrack " << i << ", dist = " << dist << ", w = " << Weight_THMUL[i][0]
                   << " -> remove TofTrack" << endl;
            }
            GlobTrack->SetTofHitIndex(-1);
            j = -1;
          }
        }  // GetTrackLength matching while end
      else {
        if (verbose > 3) {
          cout << "<D> GlobTrack " << i << ", dist = " << dist << ", w = " << Weight_THMUL[i][0] << " -> no TofTrack"
               << endl;
        }
        GlobTrack->SetTofHitIndex(-1);
        j = -1;
      }
      if (verbose > 10) { cout << "<Dch> GlobTrack " << i << "(" << nGlobTracks << "), Btt " << Btt << endl; }
    }  // endfor  of GlobalTrack inspection and TofHit assignment
    if (verbose > 1) {
      cout << "<Q> Reassignment iteration for b= " << fMCEventHeader->GetB() << ": " << NReas << endl;
    }
  }  //end of reassignment while
  // Analysis of GlobalTracks
  for (Int_t i = 0; i < nGlobTracks; i++) {  // loop over global tracks
    GlobTrack = (CbmGlobalTrack*) fGlobalTracks->At(i);
    Int_t s   = GlobTrack->GetStsTrackIndex();
    Int_t j   = GlobTrack->GetTofHitIndex();

    if (j > -1 && Weight_THMUL[i][0] >= WMAX) {
      cout << "<E> TofHit assigned beyond w-limit, Track " << i << " w= " << Weight_THMUL[i][0] << endl;
      break;  // less drastic response
    }

    if (verbose > 10) { cout << "<Da> gt " << i << ", th " << j << ", s " << s << endl; }
    const FairTrackParam* tparf = GlobTrack->GetParamFirst();
    if (0 == tparf->GetQp()) {
      if (verbose > 10) cout << "<W2> Global Track " << i << " without Qp!, take from Sts " << s << endl;
      StsTrack = (CbmStsTrack*) fStsTracks->At(s);
      GlobTrack->SetParamFirst(StsTrack->GetParamFirst());
    }
    tparf       = GlobTrack->GetParamFirst();
    Float_t qpf = tparf->GetQp();  //
    if (qpf == 0.) {
      cout << "<E2> GlobTrack " << i << ", STS " << s << ", TofHit " << j << " without momentum " << endl;
      break;
    }

    // STS
    Double_t vtxb = 100.;
    Int_t smc     = -1;
    Int_t StsMCt[100];               // array of MC track indices for current StsTrack
    Int_t NStsMCc[100] = {100 * 0};  // number of contributions
    Int_t NStsMCt      = 0;          // number of MC tracks contributing to this Ststrack
    if (s > -1) {                    // STS Track analysis, disable, bad referencing to StsHits
      StsTrack = (CbmStsTrack*) fStsTracks->At(s);

      FairTrackParam paramExtr;
      fTrackFitter.FitToVertex(StsTrack, fPrimVertex, &paramExtr);
      vtxb = fTrackFitter.GetChiToVertex(StsTrack,
                                         fPrimVertex);  //impact paramter ???
      fa_VtxB->Fill(vtxb);

      Int_t NStsHits = StsTrack->GetNofStsHits();
      //if(NStsHits<8) continue; // nh-debugging
      for (Int_t ih = 0; ih < NStsHits; ih++) {
        Int_t iHind = StsTrack->GetHitIndex(ih);
        LOG(debug1) << " inspect STS track " << s << ", hit " << ih << ", hitindex " << iHind;
        if (NULL == fStsHits) LOG(fatal) << " No STS Hits available ";
        //CbmStsHit* hit = (CbmStsHit*) fStsHits->At(iHind); // still valid ? - ok?
        CbmStsHit* hit = dynamic_cast<CbmStsHit*>(fStsHits->At(iHind));
        if (NULL == hit) continue;
        LOG(debug1) << " valid hit " << ih << ", hitindex " << iHind
                    << " cluster index f:  " << hit->GetFrontClusterId() << ", b:  " << hit->GetBackClusterId();

        CbmStsCluster* fclu = (CbmStsCluster*) fStsClusters->At(hit->GetFrontClusterId());
        CbmStsCluster* bclu = (CbmStsCluster*) fStsClusters->At(hit->GetBackClusterId());
        LOG(debug1) << " Mul f: " << fclu->GetNofDigis() << " (";
        for (Int_t iDigi = 0; iDigi < fclu->GetNofDigis(); iDigi++) {
          LOG(debug1) << fclu->GetDigi(iDigi) << " ";
          //CbmStsDigi* stsdigi      = (CbmStsDigi*) fStsDigis->At( fclu->GetDigi(iDigi) );
          CbmMatch* stsdigiMatch = (CbmMatch*) fStsDigiMatchColl->At(fclu->GetDigi(iDigi));
          LOG(debug1) << stsdigiMatch->GetNofLinks() << " ";
          for (Int_t iL = 0; iL < stsdigiMatch->GetNofLinks(); iL++) {
            const CbmLink& link = stsdigiMatch->GetLink(iL);
            CbmStsPoint* poi    = (CbmStsPoint*) fStsPointsColl->At(link.GetIndex());
            if (NULL == poi) continue;
            Int_t MCInd = poi->GetTrackID();
            LOG(debug1) << " MCInd " << poi->GetTrackID() << " ";
            Int_t iMCt = 0;
            for (; iMCt < NStsMCt; iMCt++) {
              if (MCInd == StsMCt[iMCt]) {
                NStsMCc[iMCt]++;
                break;
              }
            }
            if (iMCt == NStsMCt) {
              LOG(debug) << "contribution by new MC track: " << MCInd;
              StsMCt[iMCt]  = MCInd;
              NStsMCc[iMCt] = 1;
              NStsMCt++;
            }
          }
        }

        for (Int_t iDigi = 0; iDigi < bclu->GetNofDigis(); iDigi++) {
          LOG(debug1) << bclu->GetDigi(iDigi) << " ";
          //CbmStsDigi* stsdigi      = (CbmStsDigi*) fStsDigis->At( bclu->GetDigi(iDigi) );
          CbmMatch* stsdigiMatch = (CbmMatch*) fStsDigiMatchColl->At(bclu->GetDigi(iDigi));
          LOG(debug1) << stsdigiMatch->GetNofLinks() << " ";
          for (Int_t iL = 0; iL < stsdigiMatch->GetNofLinks(); iL++) {
            const CbmLink& link = stsdigiMatch->GetLink(iL);
            CbmStsPoint* poi    = (CbmStsPoint*) fStsPointsColl->At(link.GetIndex());
            Int_t MCInd         = poi->GetTrackID();
            LOG(debug1) << " MCInd " << poi->GetTrackID() << " ";
            Int_t iMCt = 0;
            for (; iMCt < NStsMCt; iMCt++) {
              if (MCInd == StsMCt[iMCt]) {
                NStsMCc[iMCt]++;
                break;
              }
            }
            if (iMCt == NStsMCt) {
              LOG(debug) << "contribution by new back MC track: " << MCInd;
              StsMCt[iMCt]  = MCInd;
              NStsMCc[iMCt] = 1;
              NStsMCt++;
            }
          }
        }

        LOG(debug1) << "), mul b: " << bclu->GetNofDigis();
      }  // loop over STS hits finished

      std::stringstream ss;
      ss << "STS summary: NStsMCt =" << NStsMCt;
      for (Int_t iT = 0; iT < NStsMCt; iT++) {
        ss << " iT " << iT << " NMCc " << NStsMCc[iT] << " MCt " << StsMCt[iT];
      }
      LOG(debug) << ss.str();

      if (NStsMCt == 0) {
        smc = -1;
        LOG(debug) << "StsTrack " << s << " with " << NStsHits << " Hits without StsPoints ??? from Global Track " << i
                   << ", TofHit " << j;
      }
      else {  // find most probable MCtrack
        smc             = -1;
        Int_t iMaxCount = 0;
        for (Int_t k = 0; k < NStsMCt; k++) {
          if (NStsMCc[k] > iMaxCount) {
            smc       = StsMCt[k];
            iMaxCount = NStsMCc[k];
            //            cout << "-D- STS Track "<<smc<<" with "<<NStsMCc[k]<<"("<< NStsMCt<<") matches "
            //                 <<" in "<<k<<". position"<<endl;
            // continue;
          }
        }
      }
      // analysis of STS tracks
      if (-1 < smc) {
        MCTrack = (CbmMCTrack*) fMCTracksColl->At(smc);
        pdgCode = MCTrack->GetPdgCode();
        px_MC   = MCTrack->GetPx();
        py_MC   = MCTrack->GetPy();
        pz_MC   = MCTrack->GetPz();
        p_MC    = sqrt(px_MC * px_MC + py_MC * py_MC + pz_MC * pz_MC);
        if (MCTrack->GetMotherId() == -1) {  // select primaries
          //	if (0 == MCTrack->GetMass()) continue;
          switch (pdgCode) {
            case 211: {
              fa_ptm_rap_sts_pip->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
              fa_plab_sts_pip->Fill(p_MC);
              break;
            };
            case -211: {
              fa_ptm_rap_sts_pim->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
              fa_plab_sts_pim->Fill(p_MC);
              break;
            };
            case 321: {
              fa_ptm_rap_sts_kp->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
              fa_plab_sts_kp->Fill(p_MC);
              break;
            };
            case -321: {
              fa_ptm_rap_sts_km->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
              fa_plab_sts_km->Fill(p_MC);
              break;
            };
            case 2212: {
              fa_ptm_rap_sts_p->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
              fa_plab_sts_p->Fill(p_MC);
              break;
            };
            case -2212: {
              fa_ptm_rap_sts_pbar->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
              fa_plab_sts_pbar->Fill(p_MC);
              break;
            };
            case 1000010020: {  // deuteron
              fa_ptm_rap_sts_d->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
              break;
            };
            case 1000010030: {  // triton
              fa_ptm_rap_sts_t->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
              break;
            };
            case 1000020030: {  // 3he
              fa_ptm_rap_sts_h->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
              break;
            };
            case 1000020040: {  // alpha
              fa_ptm_rap_sts_a->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
              break;
            };
            default:
            {  // intermediate mass fragments
               //cout << " Track k="<<k<<", pdgCode = "<<pdgCode<<
              //" Mass " << MCTrack->GetMass()<<","<<MCTrack->GetMass()<<" Y " << MCTrack->GetRapidity() <<
              //" Pt " << MCTrack->GetPt() <<endl;
              fa_ptm_rap_sts_imf->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
              break;
            };
          }
        }
      }
    }

    if (j > -1) {  // TofHit available for global track
      NGTofTrack++;
      TofHit = (CbmTofHit*) fTofHits->At(j);
      if (NULL == TofHit) continue;
      //       Int_t l = TofHit->GetRefId();
      Int_t l = j;  // One CbmMatch per Hit and in the same order!!!!
      Int_t k = -1;

      if (NULL == fTofDigiMatchColl) {
        LOG(fatal) << "No Digi Info available for TofHit !?? ";
        //TofPoint = (CbmTofPoint*) fTofPoints->At( TofHit->GetRefId() );
        //k = TofPoint->GetTrackID();
      }
      else {
        CbmMatch* digiMatch = (CbmMatch*) fTofDigiMatchColl->At(l);
        // take first digi's point link
        Int_t iDigiMul = digiMatch->GetNofLinks();
        Int_t iPoiMul  = 0;
        Int_t iTrkMul  = 0;
        Int_t iPoiArr[iDigiMul];
        Int_t iTrkArr[iDigiMul];
        iPoiArr[0] = -1;
        iTrkArr[0] = -1;
        for (Int_t iLink = 0; iLink < digiMatch->GetNofLinks(); iLink++) {  // loop over digis
          CbmLink L          = digiMatch->GetLink(iLink);
          Int_t iDigInd      = L.GetIndex();
          CbmMatch* poiMatch = (CbmMatch*) fTofDigiMatchPointsColl->At(iDigInd);
          CbmLink LP         = poiMatch->GetMatchedLink();
          lp                 = LP.GetIndex();
          if (lp != iPoiArr[iPoiMul]) {
            //	   cout << Form("<D> HadronAnalysis: gt %d, Hit %d, Link %d, poi %d, lpoi %d, PoiMul %d",
            //		i,j,iLink,lp, iPoiArr[iPoiMul], iPoiMul)<<endl;
            iPoiArr[iPoiMul] = lp;
            iPoiMul++;
            iPoiArr[iPoiMul] = lp;
          }

          TofPoint = (CbmTofPoint*) fTofPoints->At(lp);
          k        = TofPoint->GetTrackID();
          if (k != iTrkArr[iTrkMul]) {
            iTrkArr[iTrkMul] = k;
            iTrkMul++;
            iTrkArr[iTrkMul] = k;
          }
        }
        /*
       if(iTrkMul>1 || iPoiMul>1) {
	 //	 cout << Form("HadronAnalysis: McTrkMul %d for TofHit %d, PoiMul %d,",iTrkMul,j,iPoiMul)<<endl;
	 continue;        // for debugging response
       }
       */
        TofPoint = (CbmTofPoint*) fTofPoints->At(iPoiArr[0]);
        k        = iTrkArr[0];
      }

      MCTrack = (CbmMCTrack*) fMCTracksColl->At(k);
      pdgCode = MCTrack->GetPdgCode();
      px_MC   = MCTrack->GetPx();
      py_MC   = MCTrack->GetPy();
      pz_MC   = MCTrack->GetPz();
      p_MC    = sqrt(px_MC * px_MC + py_MC * py_MC + pz_MC * pz_MC);

      Double_t len               = GlobTrack->GetLength();
      const FairTrackParam* tpar = GlobTrack->GetParamFirst();
      //       FairTrackParam *tpar = GlobTrack->GetParamLast();
      if (tpar->GetQp() == 0.) {
        cout << "Invalid momentum for global track " << i << " TofHit " << j << endl;
        break;
      }
      Double_t mom = 1. / tpar->GetQp();
      if (mom < 0.) mom = -mom;
      Float_t vel = TofHit->GetR() / TofHit->GetTime();  // GetR() instead of len
      Float_t bet = vel / clight;
      Double_t m2 = mom * mom * (1. / bet / bet - 1.);

      if (bet > 0.99999) { bet = 0.99999; }
      Float_t tofmass = mom / bet * sqrt(1. - bet * bet) * TMath::Sign(1., tpar->GetQp());
      //       Double_t chi2=0.;//(Double_t)(GlobTrack->GetChi2())/(GlobTrack->GetNDF());
      //cout << "GlobTrack-Chi2 "<<GlobTrack->GetChi2()<<", "<<GlobTrack->GetNDF()<<", "<<chi2<<endl;

      if (k != smc) {
        //	 cout << " Ana GlobalTrack: MCTrack TOF - STS mismatch: "<< k <<" - "<<smc<<endl;
        fa_tm_glomis->Fill(mom, tofmass);
        fa_w_mom_glomis->Fill(mom, Weight_THMUL[i][0]);
        // continue; // for debugging
      }
      //       if(TofHit->GetRt()<150.) continue;  // nh-debugging

      fa_xy_glo1->Fill(TofHit->GetX(), TofHit->GetY());
      fa_pv_glo->Fill(vel, mom);
      fa_tm_glo->Fill(mom, tofmass);
      fa_m2mom_glo->Fill(mom * TMath::Sign(1., tpar->GetQp()), m2);
      fa_pMCmom_glo->Fill(mom, p_MC);
      fa_chi2_mom_glo->Fill(mom, vtxb);
      fa_w_mom_glo->Fill(mom, Weight_THMUL[i][0]);
      fa_len_mom_glo->Fill(mom, len);
      fa_LenDismom_glo->Fill(mom, len - TofHit->GetR());

      if (NULL != TofPoint) {
        fa_LenMcLenGlomom_glo->Fill(mom, len - TofPoint->GetLength());
        fa_LenMcDismom_glo->Fill(mom, TofPoint->GetLength() - TofHit->GetR());
      }
      /*
       fhTofTrkDxsel->Fill(TofTrack->GetTrackDx());
       fhTofTrkDysel->Fill(TofTrack->GetTrackDy());
       */
      if (vtxb < fVtxBMax) {
        fa_tm_glovtxb->Fill(mom, tofmass);
        fa_m2mom_glovtxb->Fill(mom * TMath::Sign(1., tpar->GetQp()), m2);
      }

      if (MCTrack->GetMotherId() == -1) {  // select primaries
        fa_tm_gloprim->Fill(mom, tofmass);
        fa_m2mom_gloprim->Fill(mom * TMath::Sign(1., tpar->GetQp()), m2);
        fa_chi2_mom_gloprim->Fill(mom, vtxb);

        if (vtxb < fVtxBMax) {
          fa_tm_gloprimvtxb->Fill(mom, tofmass);
          fa_m2mom_gloprimvtxb->Fill(mom * TMath::Sign(1., tpar->GetQp()), m2);
        }
        Float_t Phip = RADDEG * atan2(MCTrack->GetPy(), MCTrack->GetPx());
        Float_t dphi = Phip - RADDEG * fMCEventHeader->GetRotZ();
        if (dphi < -180.) { dphi += 360.; };
        if (dphi > 180.) { dphi -= 360.; };
        dphi      = dphi / RADDEG;
        rp_weight = 0.;

        switch (pdgCode) {
          case 211: {
            fa_ptm_rap_glo_pip->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
            fa_v1_rap_glo_pip->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
            fa_v2_rap_glo_pip->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));
            fa_xy_glo_pip->Fill(TofHit->GetX(), TofHit->GetY());
            fa_tm_glo_pip->Fill(mom, tofmass);
            fa_m2mom_glo_pip->Fill(mom * TMath::Sign(1., tpar->GetQp()), m2);
            fa_pMCmom_glo_pip->Fill(mom, p_MC);
            fa_w_mom_glo_pip->Fill(mom, Weight_THMUL[i][0]);
            fa_LenDismom_glo_pip->Fill(mom, len - TofHit->GetR());

            if (use_pions_for_flow && TMath::Abs((MCTrack->GetRapidity() - yrp_mid) / yrp_mid) > GetDY()
                && MCTrack->GetPt() / MCTrack->GetMass() > GetFlowMinPtm()) {
              if (MCTrack->GetRapidity() > yrp_mid) {  // set weights for reaction plane
                rp_weight = -1.;
              }
              else {
                rp_weight = +1.;
              }
            }
            else {
              rp_weight = 0.;
            }
            break;
          };
          case -211: {
            fa_ptm_rap_glo_pim->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
            fa_v1_rap_glo_pim->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
            fa_v2_rap_glo_pim->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));
            fa_xy_glo_pim->Fill(TofHit->GetX(), TofHit->GetY());
            fa_tm_glo_pim->Fill(mom, tofmass);
            fa_m2mom_glo_pim->Fill(mom * TMath::Sign(1., tpar->GetQp()), m2);
            fa_pMCmom_glo_pim->Fill(mom, p_MC);
            fa_w_mom_glo_pim->Fill(mom, Weight_THMUL[i][0]);
            fa_LenDismom_glo_pim->Fill(mom, len - TofHit->GetR());

            if (use_pions_for_flow && TMath::Abs((MCTrack->GetRapidity() - yrp_mid) / yrp_mid) > GetDY()
                && MCTrack->GetPt() / MCTrack->GetMass() > GetFlowMinPtm()) {
              if (MCTrack->GetRapidity() > yrp_mid) {  // set weights for reaction plane
                rp_weight = -1.;
              }
              else {
                rp_weight = +1.;
              }
            }
            else {
              rp_weight = 0.;
            }
            break;
          };
          case 321: {
            fa_ptm_rap_glo_kp->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
            fa_v1_rap_glo_kp->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
            fa_v2_rap_glo_kp->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));
            fa_xy_glo_kp->Fill(TofHit->GetX(), TofHit->GetY());
            fa_tm_glo_kp->Fill(mom, tofmass);
            fa_m2mom_glo_kp->Fill(mom * TMath::Sign(1., tpar->GetQp()), m2);
            fa_pMCmom_glo_kp->Fill(mom, p_MC);
            fa_w_mom_glo_kp->Fill(mom, Weight_THMUL[i][0]);
            fa_LenDismom_glo_kp->Fill(mom, len - TofHit->GetR());

            break;
          };
          case -321: {
            fa_ptm_rap_glo_km->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
            fa_v1_rap_glo_km->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
            fa_v2_rap_glo_km->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));
            fa_xy_glo_km->Fill(TofHit->GetX(), TofHit->GetY());
            fa_tm_glo_km->Fill(mom, tofmass);
            fa_m2mom_glo_km->Fill(mom * TMath::Sign(1., tpar->GetQp()), m2);
            fa_pMCmom_glo_km->Fill(mom, p_MC);
            fa_w_mom_glo_km->Fill(mom, Weight_THMUL[i][0]);
            fa_LenDismom_glo_km->Fill(mom, len - TofHit->GetR());

            break;
          };
          case 2212: {
            fa_ptm_rap_glo_p->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
            fa_v1_rap_glo_p->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
            fa_v2_rap_glo_p->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));
            fa_xy_glo_p->Fill(TofHit->GetX(), TofHit->GetY());
            fa_tm_glo_p->Fill(mom, tofmass);
            fa_m2mom_glo_p->Fill(mom * TMath::Sign(1., tpar->GetQp()), m2);
            fa_pMCmom_glo_p->Fill(mom, p_MC);
            fa_w_mom_glo_p->Fill(mom, Weight_THMUL[i][0]);
            fa_LenDismom_glo_p->Fill(mom, len - TofHit->GetR());

            // reaction plane determination
            if (TMath::Abs((MCTrack->GetRapidity() - yrp_mid) / yrp_mid) > GetDY()
                && MCTrack->GetPt() / MCTrack->GetMass() > GetFlowMinPtm()) {
              if (MCTrack->GetRapidity() > yrp_mid) {  // set weights for reaction plane
                rp_weight = 1.;
              }
              else {
                rp_weight = -1.;
              }
            }
            else {
              rp_weight = 0.;
            }
            break;
          };
          case -2212: {
            fa_ptm_rap_glo_pbar->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
            fa_v1_rap_glo_pbar->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
            fa_v2_rap_glo_pbar->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));
            fa_xy_glo_pbar->Fill(TofHit->GetX(), TofHit->GetY());
            fa_tm_glo_pbar->Fill(mom, tofmass);
            fa_m2mom_glo_pbar->Fill(mom * TMath::Sign(1., tpar->GetQp()), m2);
            fa_pMCmom_glo_pbar->Fill(mom, p_MC);
            fa_w_mom_glo_pbar->Fill(mom, Weight_THMUL[i][0]);
            fa_LenDismom_glo_pbar->Fill(mom, len - TofHit->GetR());

            break;
          };
          case 1000010020: {  // deuteron
            fa_ptm_rap_glo_d->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
            fa_v1_rap_glo_d->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
            fa_v2_rap_glo_d->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));
            fa_xy_glo_d->Fill(TofHit->GetX(), TofHit->GetY());
            fa_tm_glo_d->Fill(mom, tofmass);
            fa_pMCmom_glo_d->Fill(mom, p_MC);
            fa_w_mom_glo_d->Fill(mom, Weight_THMUL[i][0]);

            // reaction plane determination
            if (TMath::Abs((MCTrack->GetRapidity() - yrp_mid) / yrp_mid) > GetDY()
                && MCTrack->GetPt() / MCTrack->GetMass() > GetFlowMinPtm()) {
              if (MCTrack->GetRapidity() > yrp_mid) {  // set weights for reaction plane
                rp_weight = 1.;
              }
              else {
                rp_weight = -1.;
              }
            }
            break;
          };
          case 1000010030: {  // triton
            fa_ptm_rap_glo_t->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
            fa_v1_rap_glo_t->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
            fa_v2_rap_glo_t->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));
            fa_xy_glo_t->Fill(TofHit->GetX(), TofHit->GetY());
            fa_tm_glo_t->Fill(mom, tofmass);
            fa_pMCmom_glo_t->Fill(mom, p_MC);
            fa_w_mom_glo_t->Fill(mom, Weight_THMUL[i][0]);

            // reaction plane determination
            if (TMath::Abs((MCTrack->GetRapidity() - yrp_mid) / yrp_mid) > GetDY()
                && MCTrack->GetPt() / MCTrack->GetMass() > GetFlowMinPtm()) {
              if (MCTrack->GetRapidity() > yrp_mid) {  // set weights for reaction plane
                rp_weight = 1.;
              }
              else {
                rp_weight = -1.;
              }
            }
            break;
          };
          case 1000020030: {  // 3he
            fa_ptm_rap_glo_h->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
            fa_v1_rap_glo_h->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
            fa_v2_rap_glo_h->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));
            fa_xy_glo_h->Fill(TofHit->GetX(), TofHit->GetY());
            fa_tm_glo_h->Fill(mom, tofmass);
            fa_pMCmom_glo_h->Fill(mom, p_MC);
            fa_w_mom_glo_h->Fill(mom, Weight_THMUL[i][0]);

            // reaction plane determination
            if (TMath::Abs((MCTrack->GetRapidity() - yrp_mid) / yrp_mid) > GetDY()
                && MCTrack->GetPt() / MCTrack->GetMass() > GetFlowMinPtm()) {
              if (MCTrack->GetRapidity() > yrp_mid) {  // set weights for reaction plane
                rp_weight = 1.;
              }
              else {
                rp_weight = -1.;
              }
            }
            break;
          };
          case 1000020040: {  // alpha
            fa_ptm_rap_glo_a->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
            fa_v1_rap_glo_a->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
            fa_v2_rap_glo_a->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));
            fa_xy_glo_a->Fill(TofHit->GetX(), TofHit->GetY());
            fa_tm_glo_a->Fill(mom, tofmass);
            fa_pMCmom_glo_a->Fill(mom, p_MC);
            fa_w_mom_glo_a->Fill(mom, Weight_THMUL[i][0]);

            // reaction plane determination
            if (TMath::Abs((MCTrack->GetRapidity() - yrp_mid) / yrp_mid) > GetDY()
                && MCTrack->GetPt() / MCTrack->GetMass() > GetFlowMinPtm()) {
              if (MCTrack->GetRapidity() > yrp_mid) {  // set weights for reaction plane
                rp_weight = 1.;
              }
              else {
                rp_weight = -1.;
              }
            }
            break;
          };
          default:
          {  // intermediate mass fragments
             //cout << " Track k="<<k<<", pdgCode = "<<pdgCode<<
            //" Mass " << MCTrack->GetMass()<<","<<MCTrack->GetMass()<<" Y " << MCTrack->GetRapidity() <<
            //" Pt " << MCTrack->GetPt() <<endl;
            fa_ptm_rap_glo_imf->Fill(MCTrack->GetRapidity(), MCTrack->GetPt() / MCTrack->GetMass());
            fa_v1_rap_glo_imf->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(dphi));
            fa_v2_rap_glo_imf->Fill((MCTrack->GetRapidity() - GetMidY()) / GetMidY(), TMath::Cos(2 * dphi));
            // reaction plane determination  (optimistic)
            if (TMath::Abs((MCTrack->GetRapidity() - yrp_mid) / yrp_mid) > GetDY()
                && MCTrack->GetPt() / MCTrack->GetMass() > GetFlowMinPtm()) {
              if (MCTrack->GetRapidity() > yrp_mid) {  // set weights for reaction plane
                rp_weight = 1.;
              }
              else {
                rp_weight = -1.;
              }
            }
            break;
          };
        }
        if (rp_weight != 0.) {
          if (gRandom->Uniform(1) > 0.5) {  //subdivide events into 2 random subevents
            Np1++;
            Qx1 = Qx1 + rp_weight * MCTrack->GetPx();
            Qy1 = Qy1 + rp_weight * MCTrack->GetPy();
          }
          else {
            Np2++;
            Qx2 = Qx2 + rp_weight * MCTrack->GetPx();
            Qy2 = Qy2 + rp_weight * MCTrack->GetPy();
          }
        }
      }
    }
  }
  if (verbose > 10) { cout << "<D> RP analysis " << Np1 << ", " << Np2 << endl; }
  if (Np1 > 0 && Np2 > 0) {
    phirp1 = atan2(Qy1, Qx1);
    phirp2 = atan2(Qy2, Qx2);
    if (fflowFile != NULL) {  // subevent RP flattening
      TH1F* phirp_glo_fpar = (TH1F*) fflowFile->Get("phirps_glo_fpar");
      Float_t dphir        = 0.;
      for (int j = 0; j < 4; j++) {
        Float_t i = (float) (j + 1);
        dphir += (-phirp_glo_fpar->GetBinContent(j) * TMath::Cos(i * phirp1)
                  + phirp_glo_fpar->GetBinContent(j + 4) * TMath::Sin(i * phirp1))
                 / i;
      }
      phirp1 += dphir;

      dphir = 0.;
      for (int j = 0; j < 4; j++) {
        Float_t i = (float) (j + 1);
        dphir += (-phirp_glo_fpar->GetBinContent(j) * TMath::Cos(i * phirp2)
                  + phirp_glo_fpar->GetBinContent(j + 4) * TMath::Sin(i * phirp2))
                 / i;
      }
      phirp2 += dphir;
    }  // subevent RP flattening end
    delrp = (phirp1 - phirp2);
    fa_phirps_glo->Fill(phirp1 * RADDEG);  // 1D histo
    fa_phirps_glo->Fill(phirp2 * RADDEG);  // 1D histo
    if (NULL != fMCEventHeader) {
      // cout << " Impact parameter "<<fMCEventHeader->GetB()<< ", delrp = "<< delrp << endl;
      fa_cdrp_b_glo->Fill(fMCEventHeader->GetB(), TMath::Cos(delrp));
      delrp = delrp * RADDEG;
      if (delrp < -180.) delrp += 360.;
      if (delrp > 180.) delrp -= 360.;
      fa_drp_b_glo->Fill(fMCEventHeader->GetB(), delrp);
    }
    phirp = RADDEG * atan2(Qy1 + Qy2, Qx1 + Qx2);  // full reaction plane
    while (phirp < -180.) {
      phirp += 360.;
    }
    while (phirp > 180.) {
      phirp -= 360.;
    }
    if (fflowFile != NULL) {  // RP flattening
      TH1F* phirp_glo_fpar = (TH1F*) fflowFile->Get("phirp_glo_fpar");
      Float_t dphir        = 0.;
      for (int j = 0; j < 4; j++) {
        Float_t i = (float) (j + 1);
        //cout << " RP flat par "<< i << ","<<j<< " par " << phirp_glo_fpar->GetBinContent(j)
        //     << ","<< phirp_glo_fpar->GetBinContent(j+4) << " phirp "<<phirp<<" dphir "<< dphir << endl;
        dphir += ((-phirp_glo_fpar->GetBinContent(j) * TMath::Cos(i * phirp / RADDEG)
                   + phirp_glo_fpar->GetBinContent(j + 4) * TMath::Sin(i * phirp / RADDEG))
                  / i);
      }
      //cout << " phirp " << phirp << " dphir " << dphir*RADDEG << endl;

      phirp += dphir * RADDEG;
      while (phirp < -180.) {
        phirp += 360.;
      }
      while (phirp > 180.) {
        phirp -= 360.;
      }
    }  // RP flattening end
    if (NULL != fMCEventHeader) {
      delrp = phirp - RADDEG * fMCEventHeader->GetRotZ();
      while (delrp < -180.) {
        delrp += 360.;
      }
      while (delrp > 180.) {
        delrp -= 360.;
      }
      fa_phirp_glo->Fill(phirp);  // 1D histo
      fa_delrp_b_glo->Fill(fMCEventHeader->GetB(), delrp);
      fa_cdelrp_b_glo->Fill(fMCEventHeader->GetB(), TMath::Cos(delrp / RADDEG));

      fa_mul_b_had->Fill(fMCEventHeader->GetB(), NGTofTrack);
    }
  }

  // Hadron level

  if (0 == (fEvents % 1000)) {
    LOG(info) << "-I- CbmHadronAnalysis::Exec : "
              << "event " << fEvents << " processed.";
  }
  fEvents += 1;
}
// ------------------------------------------------------------------


// ------------------------------------------------------------------
void CbmHadronAnalysis::Finish()
{
  // Normalisations
  cout << "CbmHadronAnalysis::Finish up with " << fEvents << " analyzed events " << endl;

  Double_t sfe  = 1. / fEvents;
  Double_t sfac = 1.E7;

  cout << "<I> Normalisation factors " << sfe << "," << sfac << endl;

  fa_hit_ch->Scale(sfe * sfac);

  fa_xy_poi2->Scale(sfe);
  fa_xy_poi3->Add(fa_xy_poi2, fa_xy_poi2, sfac, 0.);

  fa_xy_hit2->Scale(sfe);
  fa_xy_hit3->Add(fa_xy_hit2, fa_xy_hit2, sfac, 0.);

  // Finish of the task execution

  WriteHistogramms();
}
// ------------------------------------------------------------------


// ------------------------------------------------------------------
void CbmHadronAnalysis::WriteHistogramms()
{
  // Write histogramms to the file
  if (NULL != fHist) {
    TIter next(gDirectory->GetList());
    TH1* h;
    TObject* obj;
    while ((obj = (TObject*) next())) {
      if (obj->InheritsFrom(TH1::Class())) {
        h = (TH1*) obj;
        //cout << "Write histo " << h->GetTitle() << endl;
        h->Write();
      }
    }
  }
  fHist->ls();
  //fHist->Close();
}
// ------------------------------------------------------------------
static Int_t iCandEv = 0;
void CbmHadronAnalysis::ReconstructSecondaries()
{
#include "TLorentzVector.h"
#include "TVector3.h"

  static TH1F* fhTofChi;
  static TH1F* fhDperp;
  static TH2F* fhdEdxMul;
  static TH2F* fhdEdxMulsec;
  static TH1F* fhDTRDprim;
  static TH1F* fhDTRDsec;
  static TH1F* fhDperp2;
  static TH1F* fhDperpS;
  static TH1F* fhD0prim;
  static TH1F* fhOpAng;
  static TH1F* fhDCA;
  static TH1F* fhMinv;
  static TH1F* fhPathLen;
  static TH1F* fhMMom;
  static TH1F* fhMIXOpAng;
  static TH1F* fhMIXDCA;
  static TH1F* fhMIXMinv;
  static TH1F* fhMIXPathLen;
  static TH1F* fhMIXMMom;
  static TH1F* fhMCLamMom;
  static TH1F* fhMCPathLen;

  static TH2F* fa_ptm_rap_gen_lam;
  static TH2F* fa_ptm_rap_rec_lam;
  static TH2F* fa_ptm_rap_mix_lam;
  //static TH2F* fa_ptm_rap_sub_lam;

  static std::vector<std::list<std::vector<TLorentzVector>>> fvP;
  static std::vector<std::list<std::vector<TLorentzVector>>> fvX;
  static std::vector<std::list<std::vector<TVector3>>> fvX0;
  static std::vector<std::list<std::vector<TVector3>>> fvDX;

  /*
  Double_t fdDistPrimLim =1.5;  // Ext Parameter: Max Tof-Sts trans distance for primaries
  Double_t fdDistPrimLim2=0.3;  // Ext Parameter: Max Sts-Sts trans distance for primaries
  Double_t fdDistSecLim2=0.5;   // Ext Parameter: Max Sts-Sts trans distance from TOF direction for secondaries
  Double_t fdD0ProtLim=0.4;     // Ext Parameter: Min impact parameter for secondary proton
  Double_t fdOpAngMin=0.01;     // Ext Parameter: Min opening angle for accepting pair
  Double_t fdDCALim=0.2;        // Ext Parameter: Max DCA for accepting pair
  Double_t fdVLenMax=25.;       // Ext Parameter: Max Lambda flight path length for accepting pair
  Double_t fdDistTRD    = 10.;  // max accepted distance of Trd Hit from STS-TOF line
  Double_t fdTRDHmulMin =  0.;  // min associated Trd Hits to Track candidates
  */


  const Int_t fiNMixClasses = 10;
  const Double_t beamRotY   = -25.;
  const Double_t MLAM       = 1.1156;
  const Double_t DMLAM      = 0.015;
  const Int_t NSECMASS      = 2;  // pi-minus, proton, he3, alpha
  const Int_t iMode         = 0;
  Float_t secMass[NSECMASS] = {0.139, 0.938};
  switch (iMode) {
    case 0:  // Lambda
      break;
    case 1:                   // hypertriton
      secMass[1] = 2.808381;  //3he
      break;
  }
  const Double_t dTofSigX   = 0.5;
  const Double_t dTofSigY   = 0.8;
  const Double_t dTofSigT   = 0.08;
  const Double_t dChiTofLim = 3.;

  nStsHits = 0;
  if (NULL != fStsHits) nStsHits = fStsHits->GetEntriesFast();
  Int_t nTrdHits = 0;
  if (NULL != fTrdHits) nTrdHits = fTrdHits->GetEntriesFast();

  LOG(DEBUG) << "Secondaries from " << nTofHits << " TofHits, " << nStsHits << " StsHits and " << nTrdHits
             << " TrdHits in event " << iCandEv;

  if (iCandEv == 0) {  //initialize
    // define some histograms
    Double_t MinvMin = secMass[0] + secMass[1];
    cout << "Add secondary histos to " << fHist->GetName() << endl;
    fhTofChi = new TH1F(Form("hTofChi"), Form("TofHit Merger; #chi "), 100, 0., dChiTofLim * 2.);
    fhDperp  = new TH1F(Form("hDperp"), Form("transverse matching distance; d [cm]"), 100, 0., 5.);
    fhdEdxMul =
      new TH2F(Form("hdEdxMul"), Form("average energy loss vs TrdHitMul; TrdHitMul; dE []"), 4, 1, 5, 100, 0., 5.E-5);
    fhdEdxMulsec = new TH2F(Form("hdEdxMulsec"), Form("av. energy loss vs TrdHitMul for secondaries; TrdHitMul; dE []"),
                            4, 1, 5, 100, 0., 5.E-5);
    fhDTRDprim =
      new TH1F(Form("hDTRDprim"), Form("TRD transverse matching distance (prim); d [cm]"), 100, 0., 2. * fdDistTRD);
    fhDTRDsec =
      new TH1F(Form("hDTRDsec"), Form("TRD transverse matching distance (sec); d [cm]"), 100, 0., 2. * fdDistTRD);

    fhDperp2     = new TH1F(Form("hDperp2"), Form("transverse matching distance (prim); d [cm]"), 100, 0., 1.);
    fhDperpS     = new TH1F(Form("hDperpS"), Form("transverse matching distance (sec); d [cm]"), 100, 0., 1.);
    fhD0prim     = new TH1F(Form("hD0prim"), Form("transverse distance to primary vertex; d [cm]"), 100, 0., 2.);
    fhOpAng      = new TH1F(Form("hOpAng"), Form("opening angle; #alpha [rad]"), 100, 0., 0.5);
    fhDCA        = new TH1F(Form("hDCA"), Form("distance of closest approach; d [cm]"), 100, 0., 2.);
    fhMinv       = new TH1F(Form("hMinv"), Form("invariant mass; M_{inv} [GeV]"), 100, MinvMin, MinvMin + 0.2);
    fhPathLen    = new TH1F(Form("hPathLen"), Form("path length; L [cm]"), 100, 0., 30.);
    fhMMom       = new TH1F(Form("hMMom"), Form("momentum of mother ; p [GeV]"), 100, 0., 5.);
    fhMIXOpAng   = new TH1F(Form("hMIXOpAng"), Form("opening angle; #alpha [rad]"), 100, 0., 0.5);
    fhMIXDCA     = new TH1F(Form("hMIXDCA"), Form("distance of closest approach; d [cm]"), 100, 0., 2.);
    fhMIXMinv    = new TH1F(Form("hMIXMinv"), Form("invariant mass; M_{inv} [GeV]"), 100, MinvMin, MinvMin + 0.2);
    fhMIXPathLen = new TH1F(Form("hMIXPathLen"), Form("path length; L [cm]"), 100, 0., 30.);
    fhMIXMMom    = new TH1F(Form("hMIXMMom"), Form("momentum of mother ; p [GeV]"), 100, 0., 5.);
    fhMCPathLen  = new TH1F(Form("hMCPathLen"), Form("MC hyperon path length; L [cm]"), 100, 0., 30.);
    fhMCLamMom   = new TH1F(Form("hMCLamMom"), Form("MC hyperon momentum; p [GeV]"), 100, 0., 5.);


    // physics observables
    Float_t ymin   = -1.;
    Float_t ymax   = 4.;
    Float_t ptmmax = 2.5;
    Int_t ptm_nbx  = 30;
    Int_t ptm_nby  = 30;

    fa_ptm_rap_gen_lam =
      new TH2F("ptm_rap_gen_lam", "MCTrack-gen lam; y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
    fa_ptm_rap_rec_lam = new TH2F("ptm_rap_rec_lam", "rec lam; y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
    fa_ptm_rap_mix_lam = new TH2F("ptm_rap_mix_lam", "mix lam; y; p_{T}/m", ptm_nbx, ymin, ymax, ptm_nby, 0., ptmmax);
    //    fa_ptm_rap_sub_lam = new TH2F("ptm_rap_sub_lam","sub lam; y; p_{T}/m",ptm_nbx,ymin,ymax,ptm_nby,0.,ptmmax);
  }
  iCandEv++;  // count events locally

  // fill generator distributions for reference
  for (Int_t k = 0; k < nMCTracks; k++) {  // inspect MCTracks
    CbmMCTrack* MCTrack = (CbmMCTrack*) fMCTracksColl->At(k);
    Int_t pdgCode       = MCTrack->GetPdgCode();
    //cout<<"MCTrack pdg "<< pdgCode << ", Mother "<<MCTrack->GetMotherId()<<endl;
    if (MCTrack->GetMotherId() == -1 && pdgCode == 3122) {
      fhMCLamMom->Fill(MCTrack->GetP());
      TLorentzVector PLAM(MCTrack->GetPx(), MCTrack->GetPy(), MCTrack->GetPz(), MCTrack->GetEnergy());
      PLAM.RotateY(beamRotY * TMath::Pi() / 180.);
      fa_ptm_rap_gen_lam->Fill(PLAM.Rapidity(), PLAM.Pt() / MCTrack->GetMass());
    }
    if (MCTrack->GetMotherId() > -1 && pdgCode == -211) {  // decay pion
      CbmMCTrack* MCTrackm = (CbmMCTrack*) fMCTracksColl->At(MCTrack->GetMotherId());
      if (MCTrackm->GetPdgCode() == 3122) {
        TVector3 MCV;
        MCTrack->GetStartVertex(MCV);
        fhMCPathLen->Fill(MCV.Mag());
        LOG(debug) << "MC vertex at Pathlen =  " << MCV.Mag() << ", pi-mom " << MCTrack->GetP();
      }
    }
  }

  fvP.resize(fiNMixClasses);
  fvX.resize(fiNMixClasses);
  fvX0.resize(fiNMixClasses);
  fvDX.resize(fiNMixClasses);

  Int_t iMixClass = nTofHits * fiNMixClasses / fiTofHitMulMax;
  if (iMixClass >= fiNMixClasses) iMixClass = fiNMixClasses - 1;

  std::vector<TLorentzVector> P;
  P.resize(nTofHits);  // define array of momentum Lorentzvectors
  std::vector<TLorentzVector> X;
  X.resize(nTofHits);  // define array of space Lorentzvectors
  std::vector<TVector3> X0;
  X0.resize(nTofHits);  // first measured point of line
  std::vector<TVector3> DX;
  DX.resize(nTofHits);  // direction of line
  const Int_t NTrdStations = 4;
  std::vector<std::vector<Int_t>> iTRD;
  iTRD.resize(nTofHits);

  Double_t dStsDistMin[nTofHits];
  Double_t dSts2DistMin[nTofHits];
  Double_t dTofDistMin[nStsHits];
  Double_t dTofDist2Min[nStsHits];
  Int_t iStsMin[nTofHits][2];  // storage of track candidate STS hits
  Int_t iTofMin[nStsHits];

  Double_t dTrdDistMin[nTofHits][NTrdStations];

  Int_t proton_cand = 0;
  Int_t pion_cand   = 0;

  for (Int_t j = 0; j < nStsHits; j++) {
    iTofMin[j]      = -1;    //initialize
    dTofDistMin[j]  = 100.;  //initialize
    dTofDist2Min[j] = 100.;  //initialize
  }

  //0. merge TOF hits due to counter overlaps
  for (Int_t i = 0; i < nTofHits; i++) {
    CbmTofHit* pTofHit = (CbmTofHit*) fTofHits->At(i);
    if (NULL == pTofHit) continue;
    if (pTofHit->GetZ() == 0.) continue;  // don't merge with fake beam counter
    for (Int_t i2 = 0; i2 < nTofHits; i2++) {
      if (i2 != i) {
        CbmTofHit* pTofHit2 = (CbmTofHit*) fTofHits->At(i2);
        if (NULL == pTofHit2) continue;
        if (pTofHit2->GetZ() == 0.) continue;  // don't merge with fake beam counter
        // Project to plane with smallest z coordinate
        if (pTofHit2->GetZ() < pTofHit->GetZ()) {  //invert order
          CbmTofHit* pTofHittmp = pTofHit;
          pTofHit               = pTofHit2;
          pTofHit2              = pTofHittmp;
        }
        Double_t dPosZExp = pTofHit->GetZ() / pTofHit2->GetZ();
        Double_t dPosXExp = pTofHit2->GetX() * dPosZExp;
        Double_t dPosYExp = pTofHit2->GetY() * dPosZExp;
        Double_t dTimeExp = pTofHit2->GetTime() * dPosZExp;
        Double_t dChi2    = TMath::Power((dPosXExp - pTofHit->GetX()) / dTofSigX, 2)
                         + TMath::Power((dPosYExp - pTofHit->GetY()) / dTofSigY, 2)
                         + TMath::Power((dTimeExp - pTofHit->GetTime()) / dTofSigT, 2);
        Double_t dChi = TMath::Sqrt(dChi2) / 3.;
        fhTofChi->Fill(dChi);
        if (dChi < dChiTofLim) {                                    // merge info
          pTofHit->SetTime((pTofHit->GetTime() + dTimeExp) * 0.5);  // update time
          fTofHits->Remove(pTofHit2);
          //pTofHit2->Delete();                                  // remove from TClonesArray
          LOG(debug) << "Tof Hits " << i << " and " << i2 << " merged ";
          LOG(debug) << "Tof " << i << ", xyz " << pTofHit->GetX() << ", " << pTofHit->GetY() << ", "
                     << pTofHit->GetZ();
        }
      }
    }
  }

  //1. find best tof silicon match for primary track hypothesis
  for (Int_t i = 0; i < nTofHits; i++) {
    CbmTofHit* pTofHit = (CbmTofHit*) fTofHits->At(i);
    if (NULL == pTofHit) continue;
    if (pTofHit->GetZ() == 0.) continue;  // skip fake beam counter
    dStsDistMin[i]  = 1.E3;
    dSts2DistMin[i] = 1.E3;
    for (Int_t l = 0; l < NTrdStations; l++)
      dTrdDistMin[i][l] = 1.E3;
    iStsMin[i][0] = -1;
    iStsMin[i][1] = -1;
    for (Int_t j = 0; j < nStsHits; j++) {
      CbmStsHit* pStsHit = (CbmStsHit*) fStsHits->At(j);
      // Check for primary track
      Double_t sPosZ    = pStsHit->GetZ();
      Double_t sPosXext = pTofHit->GetX() * sPosZ / pTofHit->GetZ();
      Double_t sPosYext = pTofHit->GetY() * sPosZ / pTofHit->GetZ();
      Double_t dDist2   = TMath::Power(pStsHit->GetX() - sPosXext, 2) + TMath::Power(pStsHit->GetY() - sPosYext, 2);
      Double_t dDist    = TMath::Sqrt(dDist2);
      fhDperp->Fill(dDist);
      LOG(debug) << "Sts " << j << ", xyz " << pStsHit->GetX() << ", " << pStsHit->GetY() << ", " << sPosZ;
      LOG(debug) << "Tof " << i << ", xyz " << pTofHit->GetX() << ", " << pTofHit->GetY() << ", " << pTofHit->GetZ();
      LOG(debug) << "Tof " << i << ", Sts " << j << Form(" -> dist %6.3f, Min %6.3f ", dDist, dStsDistMin[i]);

      if (dDist < fdDistPrimLim && dDist < dStsDistMin[i]) {  // primary or proton candidate
        if (iTofMin[j] > -1) {
          LOG(debug) << Form("Sts hit %d already assigned to tof hit %d with "
                             "dist= %6.3f, prev %6.3f",
                             j, iTofMin[j], dDist, dTofDistMin[j]);
          if (dDist > dTofDistMin[j]) continue;  // previous assignment was better
        }
        dStsDistMin[i] = dDist;
        iTofMin[j]     = i;
        dTofDistMin[j] = dDist;
        LOG(debug) << "Prim Track cand started for Tof " << i << ", Sts " << j
                   << Form(": dist %6.3f, Min %6.3f at z = %4.1f", dDist, dStsDistMin[i], sPosZ);
      }
    }  // for (Int_t j=0; j<nStsHits; j++) {
  }    //for (Int_t i=0; i<nTofHits; i++) {

  //2.: find second silicon hit for primary tracks
  for (Int_t j = 0; j < nStsHits; j++) {
    if (iTofMin[j] < 0) continue;
    Int_t i            = iTofMin[j];  // index of Tof Hit
    CbmStsHit* pStsHit = (CbmStsHit*) fStsHits->At(j);
    // Check for confirmation of primary track
    for (Int_t k = 0; k < nStsHits; k++) {
      if (j == k) continue;
      CbmStsHit* pSts2Hit = (CbmStsHit*) fStsHits->At(k);
      Double_t sPos2Z     = pSts2Hit->GetZ();
      Double_t sPos2Xext  = pStsHit->GetX() * sPos2Z / pStsHit->GetZ();
      Double_t sPos2Yext  = pStsHit->GetY() * sPos2Z / pStsHit->GetZ();
      Double_t dDist2 = TMath::Power(pSts2Hit->GetX() - sPos2Xext, 2) + TMath::Power(pSts2Hit->GetY() - sPos2Yext, 2);
      Double_t dDist  = TMath::Sqrt(dDist2);
      fhDperp2->Fill(dDist);
      LOG(debug) << "Tof " << i << ", Sts " << j
                 << Form(" Sts2 %d -> dist %6.3f, Min %6.3f at z = %4.1f", k, dDist, dSts2DistMin[i], sPos2Z);

      if (dDist < fdDistPrimLim2 && dDist < dSts2DistMin[i]) {  // look for primary or proton candidate
        if (iTofMin[k] > -1) {
          LOG(debug) << Form("Sts2hit %d already assigned to tof hit %d with "
                             "dist= %6.3f, prev %6.3f",
                             k, iTofMin[k], dDist, dTofDistMin[k]);
          if (dDist > dTofDist2Min[k]) continue;  // previous assignment was better
        }
        dSts2DistMin[i] = dDist;
        iTofMin[k]      = i;
        if (pStsHit->GetZ() < pSts2Hit->GetZ()) {
          iStsMin[i][0] = j;
          iStsMin[i][1] = k;
        }
        else {
          iStsMin[i][0] = k;
          iStsMin[i][1] = j;
        }
        dTofDistMin[k] = dDist;
        LOG(debug) << "Prim Track cand found for Tof " << i << ", Sts " << j << ", Sts2 " << k
                   << Form(": dist %6.3f, Min %6.3f at z = %4.1f", dDist, dSts2DistMin[i], sPos2Z);
      }  // primary or proton candidate
    }    //  for (Int_t k=0; k<nStsHits; k++) {
  }      // for (Int_t j=0; j<nStsHits; j++) {

  //2.a - confirm primary track hypothesis by TRD
  if (nTrdHits > 0) {
    for (Int_t i = 0; i < nTofHits; i++) {
      Int_t j = iStsMin[i][1];  // index of STS hit in second plane
      if (j < 0) continue;      // no STS assigned
      CbmTofHit* pTofHit = (CbmTofHit*) fTofHits->At(i);
      if (NULL == pTofHit) continue;
      if (pTofHit->GetZ() == 0) continue;  // skip fake beam counter
      CbmStsHit* pStsHit = (CbmStsHit*) fStsHits->At(j);
      Double_t dDx       = pTofHit->GetX() - pStsHit->GetX();
      Double_t dDy       = pTofHit->GetY() - pStsHit->GetY();
      Double_t dDz       = pTofHit->GetZ() - pStsHit->GetZ();
      LOG(DEBUG) << "Check for TRD hits between STS " << j << " and TOF " << i;

      for (Int_t l = 0; l < nTrdHits; l++) {
        CbmTrdHit* pTrdHit = (CbmTrdHit*) fTrdHits->At(l);
        // calculate expected position in Trd layer
        Double_t dXexp = pStsHit->GetX() + dDx * (pTrdHit->GetZ() - pStsHit->GetZ()) / dDz;
        Double_t dYexp = pStsHit->GetY() + dDy * (pTrdHit->GetZ() - pStsHit->GetZ()) / dDz;
        Double_t dDtrans =
          TMath::Sqrt(TMath::Power(dXexp - pTrdHit->GetX(), 2) + TMath::Power(dYexp - pTrdHit->GetY(), 2));
        UInt_t iTrdLayer = CbmTrdAddress::GetLayerId(pTrdHit->GetAddress());
        LOG(DEBUG) << "Inspect TRD hit " << l << " in "
                   << Form("Module 0x%08x, layer %d", pTrdHit->GetAddress(),
                           CbmTrdAddress::GetLayerId(pTrdHit->GetAddress()))
                   << " at z= " << pTrdHit->GetZ() << " dD  = " << dDtrans << " < " << fdDistTRD;
        fhDTRDprim->Fill(dDtrans);
        if (dDtrans < fdDistTRD && dDtrans < dTrdDistMin[i][iTrdLayer]) {  // check if acceptable and take best match
          Int_t iMul = iTRD[i].size();
          if (dTrdDistMin[i][iTrdLayer] < 1.E3) {  // modify previous entry
            //find old entry in vector
            Int_t ll = 0;
            for (; ll < iMul; ll++)
              if (CbmTrdAddress::GetLayerId(((CbmTrdHit*) fTrdHits->At(iTRD[i][ll]))->GetAddress()) == iTrdLayer) break;
            iTRD[i][ll] = l;
          }
          else {  //add hit
            dTrdDistMin[i][iTrdLayer] = dDtrans;
            iTRD[i].resize(iMul + 1);
            iTRD[i][iMul] = l;
          }
          LOG(DEBUG) << "assign TrdHit " << l << " to TofHit " << i << " in layer " << iTrdLayer
                     << " with d = " << dDtrans << ", TrdMul" << iMul << ", dEdx = " << pTrdHit->GetELoss();
        }
      }
    }
    //2.b - monitor TRD dEdx
    for (Int_t i = 0; i < nTofHits; i++) {
      Int_t iMul = iTRD[i].size();
      if (iMul > 0) {
        Double_t ddEdx = 0.;
        for (Int_t l = 0; l < iMul; l++) {
          CbmTrdHit* pTrdHit = (CbmTrdHit*) fTrdHits->At(iTRD[i][l]);
          ddEdx += pTrdHit->GetELoss();
        }
        ddEdx /= (Double_t) iMul;
        fhdEdxMul->Fill((Double_t) iMul, ddEdx);
      }
    }
  }

  //3. find secondary proton candidate
  for (Int_t i = 0; i < nTofHits; i++) {
    CbmTofHit* pTofHit = (CbmTofHit*) fTofHits->At(i);
    if (NULL == pTofHit) continue;
    if (pTofHit->GetZ() == 0.) continue;  // skip fake beam counter
    if (iStsMin[i][0] > -1 && iStsMin[i][1] > -1) {
      CbmStsHit* pStsHit  = (CbmStsHit*) fStsHits->At(iStsMin[i][0]);
      CbmStsHit* pSts2Hit = (CbmStsHit*) fStsHits->At(iStsMin[i][1]);

      Double_t dDx = pStsHit->GetX() - pSts2Hit->GetX();
      Double_t dDy = pStsHit->GetY() - pSts2Hit->GetY();
      Double_t dDz = pStsHit->GetZ() - pSts2Hit->GetZ();
      // Extrapolate to z=0 (from Si information)
      Double_t dX0 = pSts2Hit->GetX() - dDx / dDz * pSts2Hit->GetZ();
      Double_t dY0 = pSts2Hit->GetY() - dDy / dDz * pSts2Hit->GetZ();
      Double_t dD0 = TMath::Sqrt(dX0 * dX0 + dY0 * dY0);
      fhD0prim->Fill(dD0);

      if (dD0 > fdD0ProtLim
          && (Double_t) iTRD[i].size() >= fdTRDHmulMin) {  // secondary proton canditate, memorize relevant quantities
        Double_t dDd = TMath::Sqrt(dDx * dDx + dDy * dDy + dDz * dDz);
        Double_t vel = pTofHit->GetR() / pTofHit->GetTime();
        Double_t bet = vel / clight;
        if (bet > 0.9999) continue;                             // bet=0.9999;
        Double_t m    = secMass[1];                             // assume proton
        Double_t pmag = m * bet / TMath::Sqrt(1. - bet * bet);  // natural units
        Double_t pz   = pmag * dDz / dDd;
        Double_t px   = pmag * dDx / dDd;
        Double_t py   = pmag * dDy / dDd;
        Double_t E    = TMath::Sqrt(pmag * pmag + m * m);
        P[i].SetPxPyPzE(px, py, pz, E);
        X[i].SetXYZT(pTofHit->GetX(), pTofHit->GetY(), pTofHit->GetZ(), pTofHit->GetTime());
        LOG(debug) << "Init proton LV at ind " << Form("%d %d %d", i, iStsMin[i][0], iStsMin[i][1])
                   << " with beta = " << bet << ", minv = " << P[i].M() << ", tof " << X[i].T() << ", TRDHmul "
                   << iTRD[i].size();
        X0[i].SetXYZ(pSts2Hit->GetX(), pSts2Hit->GetY(), pSts2Hit->GetZ());
        DX[i].SetXYZ(dDx, dDy, dDz);
        proton_cand++;
      }  // secondary proton canditate,
    }
  }

  //4. secondary pion candidate
  for (Int_t i = 0; i < nTofHits; i++) {
    LOG(DEBUG) << "Tof " << i << Form(" sec cand Min %6.3f > %6.3f ?", dStsDistMin[i], fdDistPrimLim);
    if (dStsDistMin[i] > fdDistPrimLim) {  // Tof hit not in the primary class
      Double_t dDistMin  = 100.;
      Int_t jbest        = -1;
      Int_t kbest        = -1;
      CbmTofHit* pTofHit = (CbmTofHit*) fTofHits->At(i);
      if (NULL == pTofHit) continue;
      if (pTofHit->GetZ() == 0.) continue;  // skip fake beam counter
      for (Int_t j = 0; j < nStsHits; j++) {
        LOG(debug) << "Tof " << i << ", Sts " << j
                   << Form(" ? sec cand %6.3f Min %6.3f ", dTofDistMin[j], fdDistPrimLim);
        if (dTofDistMin[j] > fdDistPrimLim) {  // Sts hit not in the primary class
          CbmStsHit* pStsHit = (CbmStsHit*) fStsHits->At(j);
          // check for extension of pair to 2nd silicon plane
          Double_t dDx = pTofHit->GetX() - pStsHit->GetX();
          Double_t dDy = pTofHit->GetY() - pStsHit->GetY();
          Double_t dDz = pTofHit->GetZ() - pStsHit->GetZ();

          for (Int_t k = 0; k < nStsHits; k++) {
            if (j == k) continue;
            CbmStsHit* pSts2Hit = (CbmStsHit*) fStsHits->At(k);
            Double_t sPos2Z     = pSts2Hit->GetZ();
            Double_t sPos2Xext  = pStsHit->GetX() + dDx / dDz * (sPos2Z - pStsHit->GetZ());
            Double_t sPos2Yext  = pStsHit->GetY() + dDy / dDz * (sPos2Z - pStsHit->GetZ());
            Double_t dDist2 =
              TMath::Power(pSts2Hit->GetX() - sPos2Xext, 2) + TMath::Power(pSts2Hit->GetY() - sPos2Yext, 2);
            Double_t dDist = TMath::Sqrt(dDist2);
            fhDperpS->Fill(dDist);
            LOG(debug) << "Sec Tof " << i << ", Sts " << j
                       << Form(" Sts2 %d -> dist %6.3f < %6.3f ? at z = %4.1f", k, dDist,
                               TMath::Min(dDistMin, fdDistSecLim2), sPos2Z);
            if (dDist < fdDistSecLim2 && dDist < dDistMin) {  // secondary or pion candidate
              dDistMin = dDist;
              jbest    = j;
              kbest    = k;
            }
          }  // for (Int_t k=0; k<nStsHits; k++) {
        }    //if( dTofDistMin[j] > dDistPrimLim) {  // Sts hit not in the primary class
      }      // for (Int_t j=0; j<nStsHits; j++) {

      LOG(debug) << "Sec Dist for TofHit " << i << ": " << dDistMin << ", j " << jbest << ", k " << kbest;

      if (dDistMin < 100.) {  // secondary candidate found, store vectors
        CbmStsHit* pStsHit  = (CbmStsHit*) fStsHits->At(jbest);
        CbmStsHit* pSts2Hit = (CbmStsHit*) fStsHits->At(kbest);
        if (pSts2Hit->GetZ() > pStsHit->GetZ()) {  // swap order
          pStsHit  = (CbmStsHit*) fStsHits->At(kbest);
          pSts2Hit = (CbmStsHit*) fStsHits->At(jbest);
        }
        //4.a - confirm secondary  track hypothesis by TRD
        if (nTrdHits > 0) {
          Double_t dDx = pTofHit->GetX() - pStsHit->GetX();
          Double_t dDy = pTofHit->GetY() - pStsHit->GetY();
          Double_t dDz = pTofHit->GetZ() - pStsHit->GetZ();
          for (Int_t l = 0; l < nTrdHits; l++) {
            CbmTrdHit* pTrdHit = (CbmTrdHit*) fTrdHits->At(l);
            // calculate expected position in Trd layer
            Double_t dXexp = pStsHit->GetX() + dDx * (pTrdHit->GetZ() - pStsHit->GetZ()) / dDz;
            Double_t dYexp = pStsHit->GetY() + dDy * (pTrdHit->GetZ() - pStsHit->GetZ()) / dDz;
            Double_t dDtrans =
              TMath::Sqrt(TMath::Power(dXexp - pTrdHit->GetX(), 2) + TMath::Power(dYexp - pTrdHit->GetY(), 2));
            UInt_t iTrdLayer = CbmTrdAddress::GetLayerId(pTrdHit->GetAddress());
            fhDTRDsec->Fill(dDtrans);
            LOG(debug) << "Inspect sec. TRD hit " << l << " in "
                       << Form("Module 0x%08x, layer %d", pTrdHit->GetAddress(),
                               CbmTrdAddress::GetLayerId(pTrdHit->GetAddress()))
                       << " at z= " << pTrdHit->GetZ() << " dD  = " << dDtrans << " < " << fdDistTRD;
            if (dDtrans < fdDistTRD
                && dDtrans < dTrdDistMin[i][iTrdLayer]) {  // check if acceptable and take best match
              Int_t iMul = iTRD[i].size();
              if (dTrdDistMin[i][iTrdLayer] < 1.E3) {  // modify previous entry
                //find old entry in vector
                Int_t ll = 0;
                for (; ll < iMul; ll++)
                  if (CbmTrdAddress::GetLayerId(((CbmTrdHit*) fTrdHits->At(iTRD[i][ll]))->GetAddress()) == iTrdLayer)
                    break;
                iTRD[i][ll] = l;
              }
              else {  //add hit
                dTrdDistMin[i][iTrdLayer] = dDtrans;
                iTRD[i].resize(iMul + 1);
                iTRD[i][iMul] = l;
              }
              LOG(DEBUG) << "assign TrdHit " << l << " to TofHit " << i << " in layer " << iTrdLayer
                         << " with d = " << dDtrans << ", TrdMul" << iMul << ", dEdx = " << pTrdHit->GetELoss();
            }
          }
          //4.b - monitor TRD dEdx
          Int_t iMul = iTRD[i].size();
          if (iMul > 0) {
            Double_t ddEdx = 0.;
            for (Int_t l = 0; l < iMul; l++) {
              CbmTrdHit* pTrdHit = (CbmTrdHit*) fTrdHits->At(iTRD[i][l]);
              ddEdx += pTrdHit->GetELoss();
            }
            ddEdx /= (Double_t) iMul;
            fhdEdxMulsec->Fill((Double_t) iMul, ddEdx);
          }
        }  // (nTrdHits > 0) end
        if ((Double_t) iTRD[i].size() >= fdTRDHmulMin) {
          Double_t dDx = pStsHit->GetX() - pSts2Hit->GetX();
          Double_t dDy = pStsHit->GetY() - pSts2Hit->GetY();
          Double_t dDz = pStsHit->GetZ() - pSts2Hit->GetZ();
          Double_t dDd = TMath::Sqrt(dDx * dDx + dDy * dDy + dDz * dDz);
          Double_t vel = pTofHit->GetR() / pTofHit->GetTime();  // approximation, ignoring decay kinematics
          Double_t bet = vel / clight;
          if (bet > 0.9999) continue;                             //bet=0.9999;
          Double_t m    = secMass[0];                             // assume pion
          Double_t pmag = m * bet / TMath::Sqrt(1. - bet * bet);  // natural units
          Double_t pz   = pmag * dDz / dDd;
          Double_t px   = pmag * dDx / dDd;
          Double_t py   = pmag * dDy / dDd;
          Double_t E    = TMath::Sqrt(pmag * pmag + m * m);
          P[i].SetPxPyPzE(px, py, pz, E);
          X[i].SetXYZT(pTofHit->GetX(), pTofHit->GetY(), pTofHit->GetZ(), pTofHit->GetTime());
          LOG(DEBUG) << "Init pion LV at ind " << i << " with beta = " << bet << ", minv = " << P[i].M() << ", tof "
                     << X[i].T() << ", TRDHmul " << iTRD[i].size();
          X0[i].SetXYZ(pSts2Hit->GetX(), pSts2Hit->GetY(), pSts2Hit->GetZ());
          DX[i].SetXYZ(dDx, dDy, dDz);
          pion_cand++;
        }
      }
    }  //if( dStsDistMin[i] > dDistPrimLim) {  // Sts hit not in the primary class
  }    //for (Int_t i=0; i<nTofHits; i++)
  LOG(DEBUG) << " Ev " << iCandEv << " has " << proton_cand << " protons and " << pion_cand << " pion candidates";
  if (proton_cand > 0 && pion_cand > 0) {
    LOG(DEBUG) << "add event " << iCandEv << " to mixing class " << iMixClass << " of size " << fvP[iMixClass].size();

    fvP[iMixClass].push_front(P);    //insert to mixed event vector
    fvX[iMixClass].push_front(X);    //insert to mixed event vector
    fvX0[iMixClass].push_front(X0);  //insert to mixed event vector
    fvDX[iMixClass].push_front(DX);  //insert to mixed event vector

    if (fvP[iMixClass].size() > fNMixedEvents + 1) {
      fvP[iMixClass].pop_back();
      fvX[iMixClass].pop_back();
      fvX0[iMixClass].pop_back();
      fvDX[iMixClass].pop_back();
    }
  }
  else
    return;  // nothing to be done

  //5. do combinatorics
  for (Int_t i = 0; i < nTofHits; i++) {
    if (TMath::Abs(P[i].M() - secMass[1]) < 0.01) {  // proton candidate
      std::list<std::vector<TLorentzVector>>::iterator itX = fvX[iMixClass].begin();
      std::list<std::vector<TVector3>>::iterator itX0      = fvX0[iMixClass].begin();
      std::list<std::vector<TVector3>>::iterator itDX      = fvDX[iMixClass].begin();
      Int_t iMixEv                                         = 0;
      itX0--;
      itDX--;
      itX--;
      LOG(debug1) << "LV P has size " << P.size() << ", fvP size " << fvP[iMixClass].size() << " in mix class "
                  << iMixClass;
      for (std::list<std::vector<TLorentzVector>>::iterator itP = fvP[iMixClass].begin(); itP != fvP[iMixClass].end();
           ++itP) {
        iMixEv++;
        ++itX;
        ++itX0;
        ++itDX;
        std::vector<TLorentzVector> PE = *itP;   //fvP[iEv];
        std::vector<TLorentzVector> XE = *itX;   //fvX[iEv];
        std::vector<TVector3> X0E      = *itX0;  //fvX0[iEv];
        std::vector<TVector3> DXE      = *itDX;  //fvDX[iEv];
        LOG(debug1) << "iMixEv " << iMixEv << ": PE has size " << PE.size() << ", X0E: " << X0E.size();
        if (iMixEv == 1) {
          if (PE != P) LOG(fatal) << "P not properly restored from list";
        }

        for (UInt_t j = 0; j < PE.size(); j++) {
          if (TMath::Abs(PE[j].M() - secMass[0]) < 0.01) {  // pion candidate
            //request minimum opening angle
            Double_t dOpAngle = DX[i].Angle(DXE[j]);
            if (iMixEv == 1) fhOpAng->Fill(dOpAngle);
            else
              fhMIXOpAng->Fill(dOpAngle);
            if (dOpAngle < fdOpAngMin) continue;
            // calculate decay vertex
            TVector3 N = DX[i].Cross(DXE[j]);
            if (N.Mag() == 0.) continue;
            N.SetMag(1.);
            Double_t dDCA = TMath::Abs((X0[i] - X0E[j]) * N);
            if (iMixEv == 1) fhDCA->Fill(dDCA);
            else
              fhMIXDCA->Fill(dDCA);
            LOG(debug) << "DCA for iMixEv " << iMixEv << " at ind i " << i << ", j " << j << ": " << dDCA;
            if (dDCA == 0.) continue;
            if (dDCA < fdDCALim) {
              // vertex position
              TVector3 D     = dDCA * N;
              TVector3 Ni    = DX[i].Cross(N);
              Double_t cj    = -(X0E[j] - X0[i] - D) * Ni / (DXE[j] * Ni);
              TVector3 V     = X0E[j] + cj * DXE[j] - 0.5 * D;  // Vertex vector
              Double_t dVLen = V.Mag();                         // Lambda flight pathlength
              if (dVLen > fdVLenMax) continue;
              if (dVLen < fdVLenMin) continue;
              TLorentzVector PM = P[i] + PE[j];
              TVector3 PV       = TVector3(PM.Px(), PM.Py(), PM.Pz());
              Double_t TofM     = dVLen / PM.Beta() / clight;  // flight time of M in ns

              Double_t TofMLast                 = 100.;
              Int_t Niter                       = 0;
              std::vector<TLorentzVector> Ptmp  = P;
              std::vector<TLorentzVector> PEtmp = PE;
              std::vector<TLorentzVector> Xtmp  = X;
              std::vector<TLorentzVector> XEtmp = XE;

              while (TMath::Abs(TofM - TofMLast) > 0.001 && Niter++ < 3) {
                LOG(debug) << "MinvI at ind i " << i << ", j " << j << ": " << PM.M() << ", vertex: " << V[0] << " "
                           << V[1] << " " << V[2] << ", Len " << dVLen << ", mom = " << PV.Mag() << ", TofM " << TofM;

                // update momentum calculation
                for (Int_t ii = 0; ii < 2; ii++) {
                  Int_t k;
                  TVector3 TofX;
                  Double_t m;
                  Double_t tof;
                  if (ii == 0) {
                    k    = i;
                    TofX = Xtmp[k].Vect();
                    m    = Ptmp[k].M();
                    tof  = Xtmp[k].T();
                  }
                  else {
                    k    = j;
                    TofX = XEtmp[k].Vect();
                    m    = PEtmp[k].M();
                    tof  = XEtmp[k].T();
                  }
                  TVector3 vDTofV = TofX - V;
                  Double_t vel    = vDTofV.Mag() / (tof - TofM);
                  Double_t bet    = vel / clight;
                  if (bet > 0.9999) bet = 0.9999;
                  Double_t pmag  = m * bet / TMath::Sqrt(1. - bet * bet);  // natural units
                  TVector3 vPsec = vDTofV;
                  vPsec.SetMag(pmag);
                  Double_t E = TMath::Sqrt(pmag * pmag + m * m);
                  if (ii == 0) {
                    Ptmp[k].SetVect(vPsec);
                    Ptmp[k].SetE(E);
                  }
                  else {
                    PEtmp[k].SetVect(vPsec);
                    PEtmp[k].SetE(E);
                  }
                }
                PM       = Ptmp[i] + PEtmp[j];
                PV       = TVector3(PM.Px(), PM.Py(), PM.Pz());
                TofMLast = TofM;
                TofM     = dVLen / PM.Beta() / clight;  // flight time of M in ns
              }
              Double_t minv = PM.M();
              if (iMixEv == 1) {  // fill correlated event distributions
                fhMinv->Fill(minv);
                fhPathLen->Fill(dVLen);
                fhMMom->Fill(PV.Mag());

                LOG(debug) << "MinvII in event " << fEvents << " at ind i " << i << ", j " << j << ": " << minv
                           << ", vertex: " << V[0] << " " << V[1] << " " << V[2] << ", Len " << dVLen
                           << ", mom = " << PV.Mag() << ", tof " << TofM;

                if (TMath::Abs(MLAM - minv) < DMLAM) {
                  PM.RotateY(beamRotY * TMath::Pi() / 180.);
                  fa_ptm_rap_rec_lam->Fill(PM.Rapidity(), PM.Pt() / MLAM);
                }
              }
              else {  // fill mixed event distributions
                fhMIXMinv->Fill(minv);
                fhMIXPathLen->Fill(dVLen);
                fhMIXMMom->Fill(PV.Mag());
                if (TMath::Abs(MLAM - minv) < DMLAM) {
                  PM.RotateY(beamRotY * TMath::Pi() / 180.);
                  fa_ptm_rap_mix_lam->Fill(PM.Rapidity(), PM.Pt() / MLAM);
                }
              }
            }
          }
        }
      }
    }
  }
}  //void CbmHadronAnalysis::ReconstructSecondaries()

ClassImp(CbmHadronAnalysis);
