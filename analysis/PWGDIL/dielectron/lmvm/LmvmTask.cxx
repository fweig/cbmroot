/* Copyright (C) 2010-2021 UGiessen, JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev, Elena Lebedeva, Semen Lebedev [committer] */

#include "LmvmTask.h"

#include "CbmGlobalTrack.h"
#include "CbmKF.h"
#include "CbmL1PFFitter.h"
#include "CbmMCTrack.h"
#include "CbmMatch.h"
#include "CbmMvdHit.h"
#include "CbmRichHit.h"
#include "CbmRichPoint.h"
#include "CbmRichRing.h"
#include "CbmRichUtil.h"
#include "CbmStsHit.h"
#include "CbmStsKFTrackFitter.h"
#include "CbmStsTrack.h"
#include "CbmTofHit.h"
#include "CbmTofPoint.h"
#include "CbmTofTrack.h"
#include "CbmTrackMatchNew.h"
#include "CbmTrdHit.h"
#include "CbmTrdTrack.h"
#include "CbmVertex.h"
#include "cbm/elid/CbmLitGlobalElectronId.h"
#include "cbm/qa/mc/CbmLitMCTrackCreator.h"

#include "FairEventHeader.h"
#include "FairMCPoint.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairTask.h"
#include "FairTrackParam.h"

#include "TClonesArray.h"
#include "TDatabasePDG.h"
#include "TFile.h"
#include "TMCProcess.h"
#include "TRandom3.h"
#include "TVector3.h"

#include <sstream>
#include <vector>

#include "LmvmHist.h"
#include "LmvmSimParam.h"
#include "LmvmUtils.h"

using namespace std;

ClassImp(LmvmTask);


LmvmTask::LmvmTask() : FairTask("LmvmTask") {}


LmvmTask::~LmvmTask() {}


void LmvmTask::InitHists()
{
  string ax = "Yield";
  string axMinv = "dN/dM_{ee}/N [GeV/c^{2}]^{-1}";

  fH.CreateH2("hMomVsAnglePairSignalMc", "#sqrt{P_{e^{#pm}} P_{e^{#mp}}} [GeV/c]", "#theta_{e^{+},e^{-}} [deg]",
              "Counter", 100, 0., 5., 1000, 0., 50.);

  fH.CreateH1("hMotherPdg", {"mc", "acc"}, "Pdg code", "Particles / Event", 7000, -3500., 3500.);
  fH.CreateH1("hCandPdg", fH.fAnaStepNames, "Pdg code", "Particles / Event", 7001, -3500., 3500.);
  fH.CreateH2("hCandPdgVsMom", fH.fAnaStepNames, "P [GeV/c]", "Particle", "Yield/(Event * Bin)", 200, 0., 10., 6, 0.,
              6.);
  fH.CreateH2("hCandElSrc", "Analysis step", "Mother of Electron Candidate", ax, fH.fNofAnaSteps, 0., fH.fNofAnaSteps,
              8, 0., 8.);
  fH.CreateH2("hBgPairPdg", fH.fAnaStepNames, "PDG of Candidate 1", "PDG of Candidate 2", ax, 8, 0., 8., 8, 0., 8.);

  fH.CreateH2("hPmtXY", fH.fSrcNames, "X [cm]", "Y [cm]", "Counter", 110, -110, 110, 200, -200, 200);

  fH.CreateH2("hVertexGammaXZ", fH.fAnaStepNames, "Z [cm]", "X [cm]", ax, 200, -10., 190., 400, -130., 130.);
  fH.CreateH2("hVertexGammaYZ", fH.fAnaStepNames, "Z [cm]", "Y [cm]", ax, 200, -10., 190., 400, -130., 130.);
  fH.CreateH2("hVertexGammaXY", fH.fAnaStepNames, "X [cm]", "Y [cm]", ax, 400, -130., 130., 400, -130., 130.);
  fH.CreateH2("hVertexGammaRZ", fH.fAnaStepNames, "Z [cm]", "#sqrt{X^{2}+Y^{2}} [cm]", ax, 110, fZ - 10., fZ + 100., 50,
              0., 50.);

  // vertices of not-electron candidates that are misidentified as electrons in ToF
  fH.CreateH2("hVertexXZ_misidTof", fH.fAnaStepNames, "Z [cm]", "X [cm]", ax, 110, fZ - 10., fZ + 100., 100, -50., 50.);
  fH.CreateH2("hVertexYZ_misidTof", fH.fAnaStepNames, "Z [cm]", "Y [cm]", ax, 110, fZ - 10., fZ + 100., 100, -50., 50.);
  fH.CreateH2("hVertexXY_misidTof", fH.fAnaStepNames, "X [cm]", "Y [cm]", ax, 100, -50., 50., 100, -50., 50.);
  fH.CreateH2("hVertexRZ_misidTof", fH.fAnaStepNames, "Z [cm]", "#sqrt{X^{2}+Y^{2}} [cm]", ax, 110, fZ - 10., fZ + 100.,
              100, 0., 50.);
  fH.CreateH1("hNofBgTracks", "Analysis step", "Tracks/event", fH.fNofAnaSteps, 0., fH.fNofAnaSteps);
  fH.CreateH1("hNofSignalTracks", "Analysis step", "Tracks/event", fH.fNofAnaSteps, 0., fH.fNofAnaSteps);
  fH.CreateH2("hBgSrcTracks", "Analysis step", "Candidate Source", ax, fH.fNofAnaSteps, 0., fH.fNofAnaSteps, 8, 0., 8.);

  fH.CreateH1("hNofTopoPairs", {"gamma", "pi0"}, "Pair type", "Pairs/event", 8, 0., 8);
  fH.CreateH1("hNofMismatches", {"all", "rich", "trd", "tof"}, "Analysis step", "Tracks/event", fH.fNofAnaSteps, 0.,
              fH.fNofAnaSteps);
  fH.CreateH1("hNofMismatches_gTracks", {"all", "complete"}, fH.fGTrackNames, "Detector", ax, 7., 0., 7.);
  fH.CreateH1("hNofMismatchedTrackSegments", {"all", "complete"}, fH.fGTrackNames, "nof mism. track segments", ax, 4.,
              0., 4.);  //"all": also partial tracks; "complete": only tracks with entries in all detectors
  fH.CreateH2("hMatchId_gTracks", fH.fGTrackNames, "Nof mismatched Track Segments", "Identification", ax, 4., 0., 4.,
              2., 0., 2.);

  fH.CreateH1("hChi2_mismatch_all", {"rich", "trd", "tof"}, fH.fGTrackNames, "#chi^{2}", ax, 200, 0., 20.);
  fH.CreateH1("hChi2_truematch_all", {"rich", "trd", "tof"}, fH.fGTrackNames, "#chi^{2}", ax, 200, 0., 20.);
  fH.CreateH1("hChi2_mismatch_complete", {"rich", "trd", "tof"}, fH.fGTrackNames, "#chi^{2}", ax, 200, 0., 20.);
  fH.CreateH1("hChi2_truematch_complete", {"rich", "trd", "tof"}, fH.fGTrackNames, "#chi^{2}", ax, 200, 0., 20.);

  fH.CreateH1("hNofGhosts", "Analysis step", "Tracks/event", fH.fNofAnaSteps, 0., fH.fNofAnaSteps);

  fH.CreateH2("hSrcBgPairs", "Analysis step", "Pair", ax, fH.fNofAnaSteps, 0., fH.fNofAnaSteps, fH.fNofBgPairSrc, 0.,
              fH.fNofBgPairSrc);
  fH.CreateH2("hSrcBgPairsEpEm", fH.fAnaStepNames, "mother particle e+", "mother particle e-", ax, 4, 0., 4., 4, 0.,
              4.);

  fH.CreateH1("hEventNumber", "", "", 1, 0, 1.);
  fH.CreateH1("hEventNumberMixed", "", "", 1, 0, 1.);

  fH.CreateH1("hAnnRich", fH.fSrcNames, "RICH ANN output", ax, 100, -1.1, 1.1);
  fH.CreateH2("hAnnRichVsMom", fH.fSrcNames, "P [GeV/c]", "RICH ANN output", ax, 100, 0., 10., 100, -1.1, 1.1);
  fH.CreateH1("hAnnTrd", fH.fSrcNames, "Likelihood output", ax, 100, -.1, 1.1);  // TODO: change back to "TRD ANN"
  fH.CreateH2("hTofM2", fH.fSrcNames, "P [GeV/c]", "m^{2} [GeV/c^{2}]^{2}", ax, 150, 0., 6., 500, -0.1, 1.0);

  fH.CreateH1("hChi2Sts", fH.fSrcNames, "#chi^{2}", ax, 200, 0., 20.);
  fH.CreateH1("hChi2PrimVertex", fH.fSrcNames, "#chi^{2}_{prim}", ax, 200, 0., 20.);
  fH.CreateH1("hNofMvdHits", fH.fSrcNames, "Number of hits in MVD", ax, 5, -0.5, 4.5);
  fH.CreateH1("hNofStsHits", fH.fSrcNames, "Number of hits in STS", ax, 9, -0.5, 8.5);
  fH.CreateH2("hTrdLike", {"El", "Pi"}, fH.fSrcNames, "P [GeV/c]", "Likelihood output", ax, 100, 0., 6., 100, 0., 1.);

  fH.CreateH2("hAnnRichVsMomPur", {"El", "Bg"}, "P [GeV/c]", "RICH ANN output", ax, 100, 0., 10., 100, -1.1, 1.1);
  fH.CreateH2("hTrdElLikePur", {"El", "Bg"}, "P [GeV/c]", "Likelihood output", ax, 100, 0., 10., 100, 0., 1.);

  fH.CreateH2("hTtCut", {"all", "pion", "truePair"}, fH.fSrcNames, "#sqrt{p_{e^{#pm}} p_{rec}} [GeV/c]",
              "#theta_{e^{+},e^{-}} [deg]", ax, 100, 0., 5., 100, 0., 5.);
  fH.CreateH2("hStCut", {"all", "pion", "truePair"}, fH.fSrcNames, "#sqrt{p_{e^{#pm}} p_{rec}} [GeV/c]",
              "#theta_{e^{#pm},rec} [deg]", ax, 100, 0., 5., 100, 0., 5.);
  fH.CreateH2("hRtCut", {"all", "pion", "truePair"}, fH.fSrcNames, "#sqrt{p_{e^{#pm}} p_{rec}} [GeV/c]",
              "#theta_{e^{#pm},rec} [deg]", ax, 100, 0., 5., 100, 0., 5.);

  fH.CreateH2("hMvdCut", {"1", "2"}, fH.fSrcNames, "d_{MVD} [cm]", "P_{e} [GeV/c]", ax, 100, 0., 1., 100, 0., 5.);
  fH.CreateH2("hMvdXY", {"1", "2"}, fH.fSrcNames, "X [cm]", "Y [cm]", ax, 60, -6., 6., 60, -6., 6.);
  fH.CreateH1("hMvdR", {"1", "2"}, fH.fSrcNames, "#sqrt{X^{2}+Y^{2}} [cm]", ax, 60, 0., 6.);
  fH.CreateH1("hMvdCutQa", {"1", "2"}, fH.fSrcNames, "MVD hit assignment", ax, 2, 0.,
              2.);  // [0.5]-correct, [1.5]-wrong
  fH.CreateH1("hMvdMcDist", {"1", "2"}, fH.fSrcNames, "Track-Hit distance [cm]", ax, 100, 0., 10.);

  fH.CreateH1("hMinv", fH.fSrcNames, fH.fAnaStepNames, "M_{ee} [GeV/c^{2}]", axMinv, 250, 0., 2.5);
  fH.CreateH1("hMinv_urqmdAll", fH.fSrcNames, fH.fAnaStepNames, "M_{ee} [GeV/c^{2}]", axMinv, 250, 0.,
              2.5);  // for UrQMD particles only
  fH.CreateH1("hMinv_urqmdEl", fH.fSrcNames, fH.fAnaStepNames, "M_{ee} [GeV/c^{2}]", axMinv, 250, 0.,
              2.5);  // for UrQMD electrons only

  // Pair Yield histograms for combinatorial BG calculation
  for (const string& comb : {"PM", "PP", "MM"}) {
    for (const string& cat : {"", "_urqmdAll", "_urqmdEl"}) {
      string hName = "hMinvComb" + comb + cat;
      fH.CreateH1(hName, {"sameEv", "mixedEv"}, fH.fAnaStepNames, "M_{ee} [GeV/c^{2}]", axMinv, 250, 0., 2.5);
    }
  }

  fH.CreateH1("hMinvBgMatch", {"trueMatch", "trueMatchEl", "trueMatchNotEl", "mismatch"}, fH.fAnaStepNames,
              "M_{ee} [GeV/c^{2}]", ax, 250, 0., 2.5);
  fH.CreateH1("hMinvBgSource", fH.fBgPairSrcNames, fH.fAnaStepNames, "M_{ee} [GeV/c^{2}]", axMinv, 250, 0., 2.5);
  fH.CreateH1("hMinvBgSource2_elid", {"gg", "pipi", "pi0pi0", "oo", "gpi", "gpi0", "go", "pipi0", "pio", "pi0o"},
              "M_{ee} [GeV/c^{2}]", axMinv, 250, 0., 2.5);  // "pi" are misid. charged pions

  fH.CreateH2("hMinvPt", fH.fSrcNames, fH.fAnaStepNames, "M_{ee} [GeV/c^{2}]", "P_{t} [GeV/c]", ax, 100, 0., 2., 25, 0.,
              2.5);

  fH.CreateH1("hMomPairSignal", fH.fAnaStepNames, "P [GeV/c]", ax, 100, 0., 15.);
  fH.CreateH2("hPtYPairSignal", fH.fAnaStepNames, "Rapidity", "P_{t} [GeV/c]", ax, 40, 0., 4., 20, 0., 2.);
  fH.CreateH1("hAnglePair", fH.fSrcNames, fH.fAnaStepNames, "#Theta_{1,2} [deg]", ax, 160, 0., 80.);

  for (const string& suff : {"", "+", "-"}) {
    fH.CreateH1("hMom" + suff, fH.fSrcNames, fH.fAnaStepNames, "P [GeV/c]", ax, 100, 0., 10.);
    fH.CreateH1("hMomPx" + suff, fH.fSrcNames, fH.fAnaStepNames, "Px [GeV/c]", ax, 100, -3., 3.);
    fH.CreateH1("hMomPy" + suff, fH.fSrcNames, fH.fAnaStepNames, "Py [GeV/c]", ax, 100, -3., 3.);
    fH.CreateH1("hMomPz" + suff, fH.fSrcNames, fH.fAnaStepNames, "Pz [GeV/c]", ax, 120, -1., 11.);
    fH.CreateH1("hPt" + suff, fH.fSrcNames, fH.fAnaStepNames, "P_{t} [GeV/c]", ax, 100, 0., 4.);
    fH.CreateH1("hRapidity" + suff, fH.fSrcNames, fH.fAnaStepNames, "Rapidity", ax, 100, 0., 5.);
  }

  fH.CreateH1("hMomAcc+", {"sts", "rich", "trd", "tof"}, fH.fSrcNames, "P [GeV/c]", ax, 100, 0., 10.);
  fH.CreateH1("hMomAcc-", {"sts", "rich", "trd", "tof"}, fH.fSrcNames, "P [GeV/c]", ax, 100, 0., 10.);

  fH.CreateH1("hElMom", {"all", "prim"}, {"mc", "acc", "recSts", "recStsRich", "recStsRichTrd", "recStsRichTrdTof"},
              "P [GeV/c]", ax, 100, 0., 10.);
  fH.CreateH1("hPiMom", {"all", "prim"}, {"mc", "acc", "recSts", "recStsRich", "recStsRichTrd", "recStsRichTrdTof"},
              "P [GeV/c]", ax, 100, 0., 10.);
  fH.CreateH1("hProtonMom", {"all", "prim"}, {"mc", "acc", "recSts", "recStsRich", "recStsRichTrd", "recStsRichTrdTof"},
              "P [GeV/c]", ax, 100, 0., 10.);
  fH.CreateH1("hKaonMom", {"all", "prim"}, {"mc", "acc", "recSts", "recStsRich", "recStsRichTrd", "recStsRichTrdTof"},
              "P [GeV/c]", ax, 100, 0., 10.);

  fH.CreateH1("hPionSupp", {"pi", "idEl"}, fH.fAnaStepNames, "P [GeV/c]", ax, 20, 0., 10.);

  fH.CreateH1("hMom_gTracks", fH.fGTrackNames, "P [GeV/c]", ax, 100, 0., 10.);
  fH.CreateH1("hMom_cands", fH.fCandNames, fH.fAnaStepNames, "P [GeV/c]", ax, 100, 0., 10.);
  fH.CreateH2("hPtY_gTracks", fH.fGTrackNames, "Rapidity", "P_{t} [GeV/c]", ax, 40, 0., 4., 20, 0., 2.);
  fH.CreateH2("hPtY_cands", fH.fCandNames, fH.fAnaStepNames, "Rapidity", "P_{t} [GeV/c]", ax, 40, 0., 4., 20, 0., 2.);
  fH.CreateH2("hTofM2_gTracks", fH.fGTrackNames, "P [GeV/c]", "m^{2} [GeV/c^{2}]^{2}", ax, 100, 0., 10., 500, -0.1, 1.);
  fH.CreateH2("hTofM2_cands", fH.fCandNames, fH.fAnaStepNames, "P [GeV/c]", "m^{2} [GeV/c^{2}]^{2}", ax, 100, 0., 10.,
              500, -0.1, 1.);

  fH.CreateH1("hTofPilePdgs_cands", fH.fAnaStepNames, "Particle", ax, 12, 0., 12.);
  fH.CreateH1("hTofPilePdgs_gTracks", "Particle", ax, 12, 0., 12.);
  fH.CreateH2("hTofPileHitXY", fH.fCandNames, "X [cm]", "Y [cm]", ax, 110., -550., 550., 90., -450., 450.);
  fH.CreateH2("hTofPilePointXY", fH.fCandNames, "X [cm]", "Y [cm]", ax, 110., -550., 550., 90., -450., 450.);
  fH.CreateH1("hTofPileHitPointDist", fH.fCandNames, "#sqrt{dX^{2} + dY^{2}} [cm]", ax, 1000., 0., 20.);
  fH.CreateH2("hTofPilePty_cands", fH.fCandNames, "Rapidity", "P_{t} [GeV/c]", ax, 40, 0., 4., 20, 0., 2.);

  // ToF Hits
  fH.CreateH2("hTofPointXY", {"trueid", "misid", "truematch", "mismatch"}, fH.fGTrackNames, "X [cm]", "Y [cm]", ax,
              110., -550., 550., 90., -450.,
              450.);  // to see if maybe misid/mismatches stem from a certain region in ToF
  fH.CreateH2("hTofHitXY", {"trueid", "misid", "truematch", "mismatch"}, fH.fGTrackNames, "X [cm]", "Y [cm]", ax, 110.,
              -550., 550., 90., -450., 450.);
  fH.CreateH1("hTofHitPointDist", {"trueid", "misid", "truematch", "mismatch"}, fH.fGTrackNames,
              "#sqrt{dX^{2} + dY^{2}} [cm]", ax, 400., 0., 20.);
  fH.CreateH2("hTofHitTrackDist_gTracks", {"trueid", "misid", "truematch", "mismatch"}, fH.fGTrackNames, "P [GeV/c]",
              "Distance [cm]", ax, 100, 0., 10., 200., 0., 50.);
  fH.CreateH2("hTofHitTrackDist_cands", fH.fCandNames, "P [GeV/c]", "Distance [cm]", ax, 100, 0., 10., 200., 0., 50.);

  fH.CreateH1("hMomRatio_cands", fH.fCandNames, fH.fAnaStepNames, "Ratio P_{rec}/P_{MC}", ax, 100, 0., 2.);
  fH.CreateH2("hMomRatioVsMom_cands", fH.fCandNames, fH.fAnaStepNames, "P_{MC} [GeV/c]", "Ratio P_{rec}/P_{MC}", ax,
              100, 0., 10., 200., 0., 2.);

  // compare misid. candidates (pions, proton, ...) with not misid. ones
  for (size_t iP = 4; iP < fH.fCandNames.size(); iP++) {
    fH.CreateH1("hMom_" + fH.fCandNames[iP], {"true", "misid"}, "P [GeV/c]", ax, 100, 0., 10.);
    fH.CreateH2("hPtY_" + fH.fCandNames[iP], {"true", "misid"}, "Rapidity", "P_{t} [GeV/c]", ax, 40, 0., 4., 40, 0.,
                2.);
    fH.CreateH2("hTofM2_" + fH.fCandNames[iP], {"true", "misid"}, "P [GeV/c]", "m^{2} [GeV/c^{2}]^{2}", ax, 100, 0.,
                10., 500, -0.1, 1.);
  }

  // beta-momentum spectra
  fH.CreateH2("hBetaMom", {"gTracks", "cands"}, fH.fCandNames, "P * Q ", "beta", ax, 200, -10., 10., 150., 0., 1.5);
  fH.CreateH2("hBetaMom_allGTracks", "P * Q ", "beta", ax, 200, -10., 10., 150., 0., 1.5);

  // z vertex of global tracks
  fH.CreateH2("hVertexGTrackRZ", "Z [cm]", "R [cm]", ax, 1500, -50., 100., 1000, -50., 50.);

  /*fH.CreateH2("hIndexStsRich", fH.fGTrackNames, "STS Index", "RICH Index", ax, 400., 0., 400., 100., 0., 100.); // TODO: can be deleted
  fH.CreateH2("hIndexStsTrd", fH.fGTrackNames, "STS Index", "TRD Index", ax, 400., 0., 400., 400., 0., 400.);
  fH.CreateH2("hIndexStsTof", fH.fGTrackNames, "STS Index", "ToF Index", ax, 400., 0., 400., 800., 0., 800.);*/

  fH.CreateH2("hPdgVsMom_gTracks", {"rich", "trd", "tof"}, {"all", "complete"}, "P [GeV/c]", "Misid. Particle",
              "Yield/(Event * Bin)", 200, 0., 10., 6, 0., 6.);

  fH.CreateH2("hTofM2Calc_gTracks", fH.fGTrackNames, "Time", "", "Yield", 100., 5., 15., 4., 0., 4.);
  fH.CreateH2("hTofTimeVsMom_gTracks", {"trueid", "misid", "truematch", "mismatch"}, fH.fGTrackNames, "P [GeV/c]",
              "Time (ct) [m]", "Yield", 100., 0., 10., 100., 5., 15.);
  fH.CreateH2("hTofTimeVsMom_cands", fH.fCandNames, "P [GeV/c]", "Time (ct) [m]", "Yield", 100., 0., 10., 100., 5.,
              15.);
  fH.CreateH2("hRichRingTrackDist_gTracks", fH.fGTrackNames, "P [GeV/c]", "D [cm]", "Yield", 100., 0., 10., 200., 0.,
              20.);
  fH.CreateH2("hRichRingTrackDist_cands", fH.fCandNames, fH.fAnaStepNames, "P [GeV/c]", "D [cm]", "Yield", 100., 0.,
              10., 200., 0., 20.);

  // check chi2 of candidates after El-ID step
  fH.CreateH2("hChi2VsMom", {"sts", "rich", "trd", "tof"}, fH.fCandNames, "P [GeV/c]", "#chi^{2}", ax, 100., 0., 10.,
              200, 0., 20.);
  fH.CreateH2("hTofTimeVsChi2", {"sts", "rich", "trd", "tof"}, fH.fCandNames, "#chi^{2}", "Time (ct) [m]", ax, 200., 0.,
              20., 200, 0., 20.);
  fH.CreateH2("hChi2Comb", {"StsRich", "StsTrd", "StsTof", "RichTrd", "RichTof", "TrdTof"}, fH.fCandNames,
              "#chi^{2}_{1}", "#chi^{2}_{2}", ax, 200., 0., 20., 200, 0., 20.);
}

InitStatus LmvmTask::Init()
{
  fMCEventHeader   = InitOrFatal<FairMCEventHeader>("MCEventHeader.");
  fMCTracks        = InitOrFatal<TClonesArray>("MCTrack");
  fRichHits        = InitOrFatal<TClonesArray>("RichHit");
  fRichRings       = InitOrFatal<TClonesArray>("RichRing");
  fRichPoints      = InitOrFatal<TClonesArray>("RichPoint");
  fRichRingMatches = InitOrFatal<TClonesArray>("RichRingMatch");
  fRichProj        = InitOrFatal<TClonesArray>("RichProjection");
  fStsTrackMatches = InitOrFatal<TClonesArray>("StsTrackMatch");
  fStsTracks       = InitOrFatal<TClonesArray>("StsTrack");
  fStsHits         = InitOrFatal<TClonesArray>("StsHit");
  if (fUseMvd) {
    fMvdHits       = InitOrFatal<TClonesArray>("MvdHit");
    fMvdPoints     = InitOrFatal<TClonesArray>("MvdPoint");
    fMvdHitMatches = InitOrFatal<TClonesArray>("MvdHitMatch");
  }
  fGlobalTracks    = InitOrFatal<TClonesArray>("GlobalTrack");
  fTrdTracks       = InitOrFatal<TClonesArray>("TrdTrack");
  fTrdTrackMatches = InitOrFatal<TClonesArray>("TrdTrackMatch");
  fTofPoints       = InitOrFatal<TClonesArray>("TofPoint");
  fTofHits         = InitOrFatal<TClonesArray>("TofHit");
  fTofHitsMatches  = InitOrFatal<TClonesArray>("TofHitMatch");
  fPrimVertex      = InitOrFatal<CbmVertex>("PrimaryVertex.");

  //fTofTracks       = InitOrFatal<TClonesArray>("TofTracks"); // TODO: check this and next lines
  FairRootManager* fairRootMan = FairRootManager::Instance();
  fTofTracks                   = (TClonesArray*) fairRootMan->GetObject("TofTrack");
  if (fTofTracks == nullptr) { LOG(warning) << "LmvmTask::Init : no TofTrack array!"; }

  InitHists();

  fKFFitter.Init();
  CbmLitMCTrackCreator::Instance();
  CbmLitGlobalElectronId::GetInstance();

  return kSUCCESS;
}

void LmvmTask::Exec(Option_t*)
{
  fH.FillH1("hEventNumber", 0.5);
  fEventNumber++;
  // bool useMbias = false;  // false for 40% central agag collisions (b<7.7fm)
  // bool isCentralCollision = false;

  // if (!useMbias) {
  //   double impactPar = fMCEventHeader->GetB();
  //   if (impactPar <= 7.7) isCentralCollision = true;
  // }

  cout << "========================================================" << endl;
  LOG(info) << "LmvmTask event number " << fEventNumber;
  LOG(info) << "fPionMisidLevel = " << fPionMisidLevel;
  LOG(info) << fCuts.ToString();
  LOG(info) << "fW = " << fW;

  if (fPrimVertex != nullptr) { fKFVertex = CbmKFVertex(*fPrimVertex); }
  else {
    Fatal("LmvmTask::Exec", "No PrimaryVertex array!");
  }

  //if (useMbias || (!useMbias && isCentralCollision)) {
  FillRichRingNofHits();
  DoMcTrack();
  DoMcPair();
  RichPmtXY();
  FillTopologyCands();
  FillCands();
  CalculateNofTopologyPairs("hNofTopoPairs_gamma", ELmvmSrc::Gamma);
  CalculateNofTopologyPairs("hNofTopoPairs_pi0", ELmvmSrc::Pi0);
  AnalyseGlobalTracks();
  AnalyseCandidates();

  fCandsTotal.insert(fCandsTotal.end(), fCands.begin(), fCands.end());
  LOG(info) << "fCandsTotal.size = " << fCandsTotal.size();
  //}
}  // Exec

void LmvmTask::FillRichRingNofHits()
{
  fNofHitsInRingMap.clear();
  int nofRichHits = fRichHits->GetEntriesFast();
  for (int iHit = 0; iHit < nofRichHits; iHit++) {
    CbmRichHit* hit = static_cast<CbmRichHit*>(fRichHits->At(iHit));
    if (hit == nullptr || hit->GetRefId() < 0) continue;
    FairMCPoint* point = static_cast<FairMCPoint*>(fRichPoints->At(hit->GetRefId()));
    if (point == nullptr) continue;
    CbmMCTrack* track = static_cast<CbmMCTrack*>(fMCTracks->At(point->GetTrackID()));
    if (track == nullptr || track->GetMotherId() < 0) continue;
    fNofHitsInRingMap[track->GetMotherId()]++;
  }
}

double LmvmTask::MinvScale(double minv)
{
  double scale = -1.;
  if (fParticle == "inmed") scale = LmvmUtils::GetMassScaleInmed(minv);
  else if (fParticle == "qgp")
    scale = LmvmUtils::GetMassScaleQgp(minv);
  else
    scale = 1.;
  return scale;
}


void LmvmTask::FillMomHists(const CbmMCTrack* mct, const LmvmCand* cand, ELmvmSrc src, ELmvmAnaStep step)
{
  if ((mct != nullptr && cand != nullptr) || (mct == nullptr && cand == nullptr)) {
    LOG(error) << "LmvmTask::FillMomHists: Both mct and cand are [not nullptr] or [nullptr].";
    return;
  }
  bool isMc        = (mct != nullptr);
  string chargeStr = (isMc) ? LmvmUtils::GetChargeStr(mct) : LmvmUtils::GetChargeStr(cand);
  double w = ((mct != nullptr && LmvmUtils::IsMcSignalEl(mct)) || (cand != nullptr && cand->IsMcSignal())) ? fW : 1.;

  for (const string& suff : {string(""), chargeStr}) {
    if (suff == "0") continue;
    fH.FillH1("hMom" + suff, src, step, (isMc) ? mct->GetP() : cand->fMomentum.Mag(), w);
    fH.FillH1("hMomPx" + suff, src, step, (isMc) ? mct->GetPx() : cand->fMomentum.X(), w);
    fH.FillH1("hMomPy" + suff, src, step, (isMc) ? mct->GetPy() : cand->fMomentum.Y(), w);
    fH.FillH1("hMomPz" + suff, src, step, (isMc) ? mct->GetPz() : cand->fMomentum.Z(), w);
    fH.FillH1("hPt" + suff, src, step, (isMc) ? mct->GetPt() : cand->fMomentum.Perp(), w);
    fH.FillH1("hRapidity" + suff, src, step, (isMc) ? mct->GetRapidity() : cand->fRapidity, w);
  }
}

void LmvmTask::DoMcTrack()
{
  int nMcTracks = fMCTracks->GetEntriesFast();
  LOG(info) << "nMcTracks = " << nMcTracks;

  for (int i = 0; i < nMcTracks; i++) {
    CbmMCTrack* mct = static_cast<CbmMCTrack*>(fMCTracks->At(i));
    if (mct == nullptr) continue;
    ELmvmSrc src     = LmvmUtils::GetMcSrc(mct, fMCTracks);
    string chargeStr = (mct->GetCharge() > 0) ? "+" : "-";
    bool isAcc       = IsMcTrackAccepted(i);
    double mom       = mct->GetP();
    double w         = (LmvmUtils::IsMcSignalEl(mct)) ? fW : 1.;

    FillMomHists(mct, nullptr, src, ELmvmAnaStep::Mc);
    if (isAcc) FillMomHists(mct, nullptr, src, ELmvmAnaStep::Acc);

    if (mct->GetNPoints(ECbmModuleId::kMvd) + mct->GetNPoints(ECbmModuleId::kSts) >= 4)
      fH.FillH1("hMomAcc" + chargeStr + "_sts", src, mom, w);
    if (fNofHitsInRingMap[i] >= 7) fH.FillH1("hMomAcc" + chargeStr + "_rich", src, mom, w);
    if (mct->GetNPoints(ECbmModuleId::kTrd) >= 2) fH.FillH1("hMomAcc" + chargeStr + "_trd", src, mom, w);
    if (mct->GetNPoints(ECbmModuleId::kTof) >= 1) fH.FillH1("hMomAcc" + chargeStr + "_tof", src, mom, w);

    if (LmvmUtils::IsMcGammaEl(mct, fMCTracks)) {
      TVector3 v;
      mct->GetStartVertex(v);
      for (const auto step : {ELmvmAnaStep::Mc, ELmvmAnaStep::Acc}) {
        if (step == ELmvmAnaStep::Acc && !isAcc) continue;
        fH.FillH2("hVertexGammaXZ", step, v.Z(), v.X());
        fH.FillH2("hVertexGammaYZ", step, v.Z(), v.Y());
        fH.FillH2("hVertexGammaXY", step, v.X(), v.Y());
        fH.FillH2("hVertexGammaRZ", step, v.Z(), sqrt(v.X() * v.X() + v.Y() * v.Y()));
      }
    }

    // Fill PDG histos
    int mcPdg = mct->GetPdgCode();
    if (std::abs(mcPdg) == 11 || mcPdg == 99009911) {
      int mcMotherPdg = 0;
      if (mct->GetMotherId() != -1) {
        CbmMCTrack* mother = static_cast<CbmMCTrack*>(fMCTracks->At(mct->GetMotherId()));
        if (mother != nullptr) mcMotherPdg = mother->GetPdgCode();
      }
      if (std::abs(mcPdg) == 11) {
        fH.FillH1("hMotherPdg_mc", mcMotherPdg);
        if (isAcc) fH.FillH1("hMotherPdg_acc", mcMotherPdg);
      }
    }

    if (std::abs(mcPdg) == 211) fH.FillH1("hPionSupp_pi", ELmvmAnaStep::Mc, mom, w);
    if (std::abs(mcPdg) == 211 && IsMcTrackAccepted(i)) fH.FillH1("hPionSupp_pi", ELmvmAnaStep::Acc, mom, w);
  }
}

void LmvmTask::DoMcPair()
{
  int nMcTracks = fMCTracks->GetEntries();
  for (int iMc1 = 0; iMc1 < nMcTracks; iMc1++) {  // TODO: range until iMc1 < nMcTracks-1 ??
    CbmMCTrack* mct1 = static_cast<CbmMCTrack*>(fMCTracks->At(iMc1));
    ELmvmSrc src     = LmvmUtils::GetMcSrc(mct1, fMCTracks);

    // To speed up: select only signal, eta and pi0 electrons
    if (!(src == ELmvmSrc::Signal || src == ELmvmSrc::Pi0 || src == ELmvmSrc::Eta)) continue;

    bool isAcc1 = IsMcTrackAccepted(iMc1);
    for (int iMc2 = iMc1 + 1; iMc2 < nMcTracks; iMc2++) {
      CbmMCTrack* mct2 = static_cast<CbmMCTrack*>(fMCTracks->At(iMc2));
      bool isAccPair   = isAcc1 && IsMcTrackAccepted(iMc2);
      ELmvmSrc srcPair = LmvmUtils::GetMcPairSrc(mct1, mct2, fMCTracks);
      LmvmKinePar pKin = LmvmKinePar::Create(mct1, mct2);

      if (srcPair == ELmvmSrc::Signal) {
        fH.FillH2("hMomVsAnglePairSignalMc", std::sqrt(mct1->GetP() * mct2->GetP()), pKin.fAngle);
      }

      for (const auto step : {ELmvmAnaStep::Mc, ELmvmAnaStep::Acc}) {
        if (step == ELmvmAnaStep::Acc && !isAccPair) continue;
        //fH.FillH1("hAnglePair", srcPair, step, pKin.fAngle, w);
        if (srcPair == ELmvmSrc::Signal) {
          fH.FillH2("hPtYPairSignal", step, pKin.fRapidity, pKin.fPt, fW);
          fH.FillH1("hMomPairSignal", step, pKin.fMomentumMag, fW);
        }

        // MC and Acc minv only for signal, eta and pi0
        if (srcPair == ELmvmSrc::Signal || srcPair == ELmvmSrc::Pi0 || srcPair == ELmvmSrc::Eta) {
          double w = 1;
          if (srcPair == ELmvmSrc::Signal) w = fW * MinvScale(pKin.fMinv);
          fH.FillH1("hMinv", srcPair, step, pKin.fMinv, w);
        }
      }
    }
  }
}

void LmvmTask::RichPmtXY()
{
  int nofRichHits = fRichHits->GetEntriesFast();
  for (int iH = 0; iH < nofRichHits; iH++) {
    CbmRichHit* richHit = static_cast<CbmRichHit*>(fRichHits->At(iH));
    if (richHit == nullptr || richHit->GetRefId() < 0) continue;
    FairMCPoint* pointPhoton = static_cast<FairMCPoint*>(fRichPoints->At(richHit->GetRefId()));
    if (pointPhoton == nullptr) continue;
    CbmMCTrack* trackPhoton = static_cast<CbmMCTrack*>(fMCTracks->At(pointPhoton->GetTrackID()));
    if (trackPhoton == nullptr || trackPhoton->GetMotherId() < 0) continue;
    CbmMCTrack* mct = static_cast<CbmMCTrack*>(fMCTracks->At(trackPhoton->GetMotherId()));
    if (mct == nullptr) continue;

    TVector3 v;
    mct->GetStartVertex(v);
    ELmvmSrc src = LmvmUtils::GetMcSrc(mct, fMCTracks);
    double w     = (LmvmUtils::IsMcSignalEl(mct)) ? fW : 1.;
    if (v.Z() < 2.) { fH.FillH2("hPmtXY", src, richHit->GetX(), richHit->GetY(), w); }
  }
}

bool LmvmTask::IsMcTrackAccepted(int mcTrackInd)
{
  CbmMCTrack* tr = static_cast<CbmMCTrack*>(fMCTracks->At(mcTrackInd));
  if (tr == nullptr) return false;
  int nRichPoints = fNofHitsInRingMap[mcTrackInd];
  return (tr->GetNPoints(ECbmModuleId::kMvd) + tr->GetNPoints(ECbmModuleId::kSts) >= 4 && nRichPoints >= 7
          && tr->GetNPoints(ECbmModuleId::kTrd) >= 2 && tr->GetNPoints(ECbmModuleId::kTof) > 1);
}

void LmvmTask::AnalyseGlobalTracks()
{
  int nofMcTracks = fMCTracks->GetEntriesFast();

  for (int i = 0; i < nofMcTracks; i++) {
    CbmMCTrack* mct = static_cast<CbmMCTrack*>(fMCTracks->At(i));
    int pdg         = mct->GetPdgCode();
    bool isAccSts   = (mct->GetNPoints(ECbmModuleId::kMvd) + mct->GetNPoints(ECbmModuleId::kSts) >= 4);
    TVector3 vertex;
    mct->GetStartVertex(vertex);

    if (std::abs(pdg) == 11 || std::abs(pdg) == 211 || pdg == 2212 || std::abs(pdg) == 321) {
      string hName = (std::abs(pdg) == 11) ? "hEl" : (std::abs(pdg) == 211) ? "hPi" : pdg == 2212 ? "hProton" : "hKaon";
      fH.FillH1(hName + "Mom_all_mc", mct->GetP());
      if (isAccSts) fH.FillH1(hName + "Mom_all_acc", mct->GetP());

      if (vertex.Mag() <= 0.1) {
        fH.FillH1(hName + "Mom_prim_mc", mct->GetP());
        if (isAccSts) fH.FillH1(hName + "Mom_prim_acc", mct->GetP());
      }
    }
  }  // MC tracks

  int ngTracks = fGlobalTracks->GetEntriesFast();
  for (int i = 0; i < ngTracks; i++) {
    CbmGlobalTrack* gTrack = static_cast<CbmGlobalTrack*>(fGlobalTracks->At(i));
    if (gTrack == nullptr) continue;
    int stsInd  = gTrack->GetStsTrackIndex();
    bool isRich = (gTrack->GetRichRingIndex() >= 0);
    bool isTrd  = (gTrack->GetTrdTrackIndex() >= 0);
    bool isTof  = (gTrack->GetTofHitIndex() >= 0);

    if (stsInd < 0) continue;
    CbmStsTrack* stsTrack = static_cast<CbmStsTrack*>(fStsTracks->At(stsInd));
    if (stsTrack == nullptr) continue;
    CbmTrackMatchNew* stsMatch = static_cast<CbmTrackMatchNew*>(fStsTrackMatches->At(stsInd));
    if (stsMatch == nullptr || stsMatch->GetNofLinks() == 0) continue;
    int stsMcTrackId = stsMatch->GetMatchedLink().GetIndex();
    if (stsMcTrackId < 0) continue;
    CbmMCTrack* mct = (CbmMCTrack*) fMCTracks->At(stsMcTrackId);
    if (mct == nullptr) continue;

    double w = (LmvmUtils::IsMcSignalEl(mct)) ? fW : 1.;

    int pdg    = mct->GetPdgCode();
    double p   = mct->GetP();
    double pt  = mct->GetPt();
    double rap = mct->GetRapidity();
    double m2  = CbmLitGlobalElectronId::GetInstance().GetTofM2(i, p);

    TVector3 v;
    mct->GetStartVertex(v);
    bool isPrim = IsPrimary(v.Mag());
    if (std::abs(pdg) == 11 || std::abs(pdg) == 211 || pdg == 2212 || std::abs(pdg) == 321) {
      string hName = (std::abs(pdg) == 11) ? "hEl" : (std::abs(pdg) == 211) ? "hPi" : pdg == 2212 ? "hProton" : "hKaon";
      fH.FillH1(hName + "Mom_all_recSts", p);
      if (isRich) fH.FillH1(hName + "Mom_all_recStsRich", p);
      if (isRich && isTrd) fH.FillH1(hName + "Mom_all_recStsRichTrd", p);
      if (isRich && isTrd && isTof) fH.FillH1(hName + "Mom_all_recStsRichTrdTof", p);

      if (isPrim) {
        fH.FillH1(hName + "Mom_prim_recSts", p);
        if (isRich) fH.FillH1(hName + "Mom_prim_recStsRich", p);
        if (isRich && isTrd) fH.FillH1(hName + "Mom_prim_recStsRichTrd", p);
        if (isRich && isTrd && isTof) fH.FillH1(hName + "Mom_prim_recStsRichTrdTof", p);
      }
    }

    string ptcl = GetPidString(v.Mag(), pdg);

    fH.FillH2("hVertexGTrackRZ", v.Z(), sqrt(v.X() * v.X() + v.Y() * v.Y()));
    /*fH.FillH2("hIndexStsRich_" + ptcl, stsInd, gTrack->GetRichRingIndex());
    fH.FillH2("hIndexStsTrd_"  + ptcl, stsInd, gTrack->GetTrdTrackIndex());
    fH.FillH2("hIndexStsTof_"  + ptcl, stsInd, gTrack->GetTofHitIndex());*/

    bool isTofEl    = (CbmLitGlobalElectronId::GetInstance().IsTofElectron(i, p)) ? true : false;
    bool isElectron = (CbmLitGlobalElectronId::GetInstance().IsRichElectron(i, p)
                       && CbmLitGlobalElectronId::GetInstance().IsTrdElectron(i, p)
                       && CbmLitGlobalElectronId::GetInstance().IsTofElectron(i, p))
                        ? true
                        : false;

    if (gTrack != nullptr) { CheckMismatches(gTrack, pdg, isElectron, ptcl, w); }
    if (std::abs(pdg) != 11) PidVsMom(gTrack, i, pdg, p);

    Double_t richDist = CbmRichUtil::GetRingTrackDistance(i);
    fH.FillH2("hRichRingTrackDist_gTracks_" + ptcl, p, richDist);

    // investigate misidentifications: compare misidentified candidates with same not-misidentified particles
    // first check if global track has entries in all detectors
    if (!IsInAllDets(gTrack)) continue;  // Mind: all following actions are done only for fully rec. gTracks

    CheckTofIdentification(gTrack, ptcl, p, m2, pdg, isTofEl);

    fH.FillH1("hMom_gTracks_" + ptcl, p);
    fH.FillH2("hPtY_gTracks_" + ptcl, rap, pt);
    fH.FillH2("hTofM2_gTracks_" + ptcl, p, m2);

    // check PIDs in "Tof pile"
    if (p > 0.3 && p < 1. && m2 > -0.012 && m2 < 0.01) {
      bool isSignal = (mct != nullptr && mct->GetGeantProcessId() == kPPrimary && std::abs(mct->GetPdgCode()) == 11);
      double pdgBin = (pdg == 11 && isSignal)                ? 0.5
                      : (pdg == -11 && isSignal)             ? 1.5
                      : (pdg == 11 && !isSignal && isPrim)   ? 2.5
                      : (pdg == -11 && !isSignal && isPrim)  ? 3.5
                      : (pdg == 11 && !isSignal && !isPrim)  ? 4.5
                      : (pdg == -11 && !isSignal && !isPrim) ? 5.5
                      : (pdg == 211)                         ? 6.5
                      : (pdg == -211)                        ? 7.5
                      : (pdg == 2212)                        ? 8.5
                      : (pdg == 321)                         ? 9.5
                      : (pdg == -321)                        ? 10.5
                                                             : 11.5;
      fH.FillH1("hTofPilePdgs_gTracks", pdgBin);
    }

    string ptcl2 = GetPidString(mct, nullptr);
    if (!isElectron && !(ptcl2 == "plutoEl+" || ptcl2 == "plutoEl-" || ptcl2 == "urqmdEl+" || ptcl2 == "urqmdEl-")) {
      fH.FillH1("hMom_" + ptcl2 + "_true", p);
      fH.FillH2("hPtY_" + ptcl2 + "_true", rap, pt);
      fH.FillH2("hTofM2_" + ptcl2 + "_true", p, m2);
    }
    else if (isElectron
             && !(ptcl2 == "plutoEl+" || ptcl2 == "plutoEl-" || ptcl2 == "urqmdEl+" || ptcl2 == "urqmdEl-")) {
      fH.FillH1("hMom_" + ptcl2 + "_misid", p);
      fH.FillH2("hPtY_" + ptcl2 + "_misid", rap, pt);
      fH.FillH2("hTofM2_" + ptcl2 + "_misid", p, m2);
    }

    BetaMom(mct, gTrack, ptcl2);
  }
}

bool LmvmTask::IsInTofPile(double p, double m2) { return (p > 0.3 && p < 1. && m2 > -0.012 && m2 < 0.01); }

void LmvmTask::PidVsMom(const CbmGlobalTrack* gTrack, int iGTrack, int pdg, double p)
{
  bool isRichEl = CbmLitGlobalElectronId::GetInstance().IsRichElectron(iGTrack, p);
  bool isTrdEl  = CbmLitGlobalElectronId::GetInstance().IsTrdElectron(iGTrack, p);
  bool isTofEl  = CbmLitGlobalElectronId::GetInstance().IsTofElectron(iGTrack, p);

  double pdgBin = (pdg == 211)    ? 0.5
                  : (pdg == -211) ? 1.5
                  : (pdg == 2212) ? 2.5
                  : (pdg == 321)  ? 3.5
                  : (pdg == -321) ? 4.5
                                  : 5.5;

  if (isRichEl) fH.FillH2("hPdgVsMom_gTracks_rich_all", p, pdgBin);
  if (isTrdEl) fH.FillH2("hPdgVsMom_gTracks_trd_all", p, pdgBin);
  if (isTofEl) fH.FillH2("hPdgVsMom_gTracks_tof_all", p, pdgBin);

  if (isRichEl && IsInAllDets(gTrack)) fH.FillH2("hPdgVsMom_gTracks_rich_complete", p, pdgBin);
  if (isTrdEl && IsInAllDets(gTrack)) fH.FillH2("hPdgVsMom_gTracks_trd_complete", p, pdgBin);
  if (isTofEl && IsInAllDets(gTrack)) fH.FillH2("hPdgVsMom_gTracks_tof_complete", p, pdgBin);
}

void LmvmTask::CheckTofIdentification(const CbmGlobalTrack* gTrack, const string& gtString, double pMc, double m2,
                                      int pdg, bool isTofEl)
{
  // Get STS ID
  int stsInd = gTrack->GetStsTrackIndex();
  if (stsInd < 0) return;
  CbmTrackMatchNew* stsMatch = static_cast<CbmTrackMatchNew*>(fStsTrackMatches->At(stsInd));
  if (stsMatch == nullptr || stsMatch->GetNofLinks() == 0) return;
  int stsMcTrackId = stsMatch->GetMatchedLink().GetIndex();
  if (stsMcTrackId < 0) return;

  // Get ToF Match
  Int_t tofInd = gTrack->GetTofHitIndex();
  if (tofInd < 0) return;
  CbmTofHit* tofHit = (CbmTofHit*) fTofHits->At(tofInd);
  if (tofHit == nullptr) return;
  CbmTofTrack* tofTrack = static_cast<CbmTofTrack*>(fTofTracks->At(gTrack->GetStsTrackIndex()));
  if (tofTrack == nullptr) return;
  CbmMatch* tofHitMatch = static_cast<CbmMatch*>(fTofHitsMatches->At(tofInd));
  if (tofHitMatch == nullptr) return;
  int tofPointIndex = tofHitMatch->GetMatchedLink().GetIndex();
  if (tofPointIndex < 0) return;
  FairMCPoint* tofPoint = static_cast<FairMCPoint*>(fTofPoints->At(tofPointIndex));
  if (tofPoint == nullptr) return;
  int tofMcTrackId = tofPoint->GetTrackID();

  bool isTofMismatch = (tofMcTrackId == stsMcTrackId) ? false : true;
  bool isTofMisid    = (std::abs(pdg) == 11 && isTofEl) ? false : (std::abs(pdg) != 11 && !isTofEl) ? false : true;

  double pointX = tofPoint->GetX();
  double pointY = tofPoint->GetY();
  double hitX   = tofHit->GetX();
  double hitY   = tofHit->GetY();
  double dX     = pointX - hitX;
  double dY     = pointY - hitY;
  double dR     = sqrt(dX * dX + dY * dY);
  double dist   = tofTrack->GetDistance();

  string hNameId = (!isTofMisid) ? "_trueid_" + gtString : "_misid_" + gtString;
  fH.FillH2("hTofPointXY" + hNameId, pointX, pointY);
  fH.FillH2("hTofHitXY" + hNameId, hitX, hitY);
  fH.FillH1("hTofHitPointDist" + hNameId, dR);
  fH.FillH2("hTofHitTrackDist_gTracks" + hNameId, pMc, dist);

  string hNameMatch = (!isTofMismatch) ? "_truematch_" + gtString : "_mismatch_" + gtString;
  fH.FillH2("hTofPointXY" + hNameMatch, pointX, pointY);
  fH.FillH2("hTofHitXY" + hNameMatch, hitX, hitY);
  fH.FillH1("hTofHitPointDist" + hNameMatch, dR);
  fH.FillH2("hTofHitTrackDist_gTracks" + hNameMatch, pMc, dist);

  // check m2 calculation in ToF
  double eventTime      = FairRunAna::Instance()->GetEventHeader()->GetEventTime();
  Double_t noOffsetTime = tofHit->GetTime() - eventTime;
  Double_t t            = 0.2998 * noOffsetTime;  // time in ns -> transfrom to ct in m

  fH.FillH2("hTofTimeVsMom_gTracks" + hNameId, pMc, t);
  fH.FillH2("hTofTimeVsMom_gTracks" + hNameMatch, pMc, t);

  if (!isTofMisid) fH.FillH2("hTofM2Calc_gTracks_" + gtString, t, 0.5);
  if (isTofMisid) fH.FillH2("hTofM2Calc_gTracks_" + gtString, t, 1.5);
  if (!isTofMismatch) fH.FillH2("hTofM2Calc_gTracks_" + gtString, t, 2.5);
  if (isTofMismatch) fH.FillH2("hTofM2Calc_gTracks_" + gtString, t, 3.5);

  // check ToF data for particles in ToF pile // TODO: needed or can be deleted?
  if (isTofEl && std::abs(pdg) != 11 && IsInTofPile(pMc, m2)) {
    string candString = (pdg == 211)    ? fH.fCandNames[4]
                        : (pdg == -211) ? fH.fCandNames[5]
                        : (pdg == 2212) ? fH.fCandNames[6]
                        : (pdg == 321)  ? fH.fCandNames[7]
                        : (pdg == -321) ? fH.fCandNames[8]
                                        : fH.fCandNames[9];
    fH.FillH2("hTofPileHitXY_" + candString, hitX, hitY);
    fH.FillH2("hTofPilePointXY_" + candString, pointX, pointY);
    fH.FillH1("hTofPileHitPointDist_" + candString, dR);
  }
}

void LmvmTask::BetaMom(const CbmMCTrack* mct, const CbmGlobalTrack* gTrack, const string& ptcl)
{
  Int_t tofInd = gTrack->GetTofHitIndex();
  if (tofInd < 0) return;
  CbmTofHit* tofHit = (CbmTofHit*) fTofHits->At(tofInd);
  if (NULL == tofHit) return;
  double eventTime      = FairRunAna::Instance()->GetEventHeader()->GetEventTime();
  Double_t noOffsetTime = tofHit->GetTime() - eventTime;

  double p    = mct->GetP();
  double l    = gTrack->GetLength();
  double t    = 0.2998 * noOffsetTime;  // time in ns -> transfrom to ct in m
  double q    = (mct->GetCharge() > 0) ? 1 : (mct->GetCharge() < 0) ? -1. : 0.;
  double beta = l / (t * 100);  // cm to m
  fH.FillH2("hBetaMom_gTracks_" + ptcl, q * p, beta);
  fH.FillH2("hBetaMom_allGTracks", q * p, beta);
}

void LmvmTask::CheckMismatches(const CbmGlobalTrack* gTrack, int pdg, bool isElectron, const string& ptcl, double w)
{
  // Chi2 of true-/mismatched; number of mismatches (general and seperated for detectors)
  int stsInd = gTrack->GetStsTrackIndex();
  if (stsInd < 0) return;
  CbmTrackMatchNew* stsMatch = static_cast<CbmTrackMatchNew*>(fStsTrackMatches->At(stsInd));
  if (stsMatch == nullptr || stsMatch->GetNofLinks() == 0) return;
  int stsMcTrackId = stsMatch->GetMatchedLink().GetIndex();
  if (stsMcTrackId < 0) return;

  bool isFull = IsInAllDets(gTrack);

  // first calculate Chi2
  CbmL1PFFitter fPFFitter;
  CbmStsTrack* stsTrack = static_cast<CbmStsTrack*>(fStsTracks->At(stsInd));
  vector<CbmStsTrack> stsTracks;
  stsTracks.resize(1);
  stsTracks[0] = *stsTrack;
  vector<CbmL1PFFitter::PFFieldRegion> vField;
  vector<float> chiPrim;
  fPFFitter.GetChiToVertex(stsTracks, vField, chiPrim, fKFVertex, 3e6);
  double chi2 = stsTracks[0].GetChiSq() / stsTracks[0].GetNDF();

  CbmTrackMatchNew* richMatch = nullptr;
  CbmTrackMatchNew* trdMatch  = nullptr;
  CbmMatch* tofMatch          = nullptr;

  int richRingInd = gTrack->GetRichRingIndex();
  int trdTrackInd = gTrack->GetTrdTrackIndex();
  int tofHitInd   = gTrack->GetTofHitIndex();

  if (richRingInd >= 0) richMatch = static_cast<CbmTrackMatchNew*>(fRichRingMatches->At(richRingInd));
  if (trdTrackInd >= 0) trdMatch = static_cast<CbmTrackMatchNew*>(fTrdTrackMatches->At(trdTrackInd));
  if (tofHitInd >= 0) tofMatch = static_cast<CbmMatch*>(fTofHitsMatches->At(tofHitInd));

  int nofMismTrackSegmentsAll  = 0;
  int nofMismTrackSegmentsComp = 0;

  fH.FillH1("hNofMismatches_gTracks_all_" + ptcl, 0.5, w);
  if (isFull) fH.FillH1("hNofMismatches_gTracks_complete_" + ptcl, 0.5, w);

  if (richMatch != nullptr) {
    int richMcTrackId = richMatch->GetMatchedLink().GetIndex();
    if (richMcTrackId >= 0) fH.FillH1("hNofMismatches_gTracks_all_" + ptcl, 1.5, w);
    if (richMcTrackId >= 0 && richMcTrackId == stsMcTrackId) fH.FillH1("hChi2_truematch_all_rich_" + ptcl, chi2, w);
    if (richMcTrackId >= 0 && richMcTrackId != stsMcTrackId) {
      fH.FillH1("hChi2_mismatch_all_rich_" + ptcl, chi2, w);
      fH.FillH1("hNofMismatches_gTracks_all_" + ptcl, 2.5, w);
      nofMismTrackSegmentsAll++;
    }
    if (isFull) {
      if (richMcTrackId >= 0) fH.FillH1("hNofMismatches_gTracks_complete_" + ptcl, 1.5, w);
      if (richMcTrackId >= 0 && richMcTrackId == stsMcTrackId)
        fH.FillH1("hChi2_truematch_complete_rich_" + ptcl, chi2, w);
      if (richMcTrackId >= 0 && richMcTrackId != stsMcTrackId) {
        fH.FillH1("hChi2_mismatch_complete_rich_" + ptcl, chi2, w);
        fH.FillH1("hNofMismatches_gTracks_complete_" + ptcl, 2.5, w);
        nofMismTrackSegmentsComp++;
      }
    }
  }

  if (trdMatch != nullptr) {
    int trdMcTrackId = trdMatch->GetMatchedLink().GetIndex();
    if (trdMcTrackId >= 0) fH.FillH1("hNofMismatches_gTracks_all_" + ptcl, 3.5, w);
    if (trdMcTrackId >= 0 && trdMcTrackId == stsMcTrackId) fH.FillH1("hChi2_truematch_all_trd_" + ptcl, chi2, w);
    if (trdMcTrackId >= 0 && trdMcTrackId != stsMcTrackId) {
      fH.FillH1("hNofMismatches_gTracks_all_" + ptcl, 4.5, w);
      fH.FillH1("hChi2_mismatch_all_trd_" + ptcl, chi2, w);
      nofMismTrackSegmentsAll++;
    }
    if (isFull) {
      if (trdMcTrackId >= 0) fH.FillH1("hNofMismatches_gTracks_complete_" + ptcl, 3.5, w);
      if (trdMcTrackId >= 0 && trdMcTrackId == stsMcTrackId) fH.FillH1("hChi2_truematch_complete_trd_" + ptcl, chi2, w);
      if (trdMcTrackId >= 0 && trdMcTrackId != stsMcTrackId) {
        fH.FillH1("hNofMismatches_gTracks_complete_" + ptcl, 4.5, w);
        fH.FillH1("hChi2_mismatch_complete_trd_" + ptcl, chi2, w);
        nofMismTrackSegmentsComp++;
      }
    }
  }

  if (tofMatch != nullptr && tofMatch->GetMatchedLink().GetIndex() >= 0) {
    FairMCPoint* tofPoint = static_cast<FairMCPoint*>(fTofPoints->At(tofMatch->GetMatchedLink().GetIndex()));
    if (tofPoint != nullptr) {
      int tofMcTrackId = tofPoint->GetTrackID();
      if (tofMcTrackId >= 0) fH.FillH1("hNofMismatches_gTracks_all_" + ptcl, 5.5, w);
      if (tofMcTrackId >= 0 && tofMcTrackId == stsMcTrackId) fH.FillH1("hChi2_truematch_all_tof_" + ptcl, chi2, w);
      if (tofMcTrackId >= 0 && tofMcTrackId != stsMcTrackId) {
        fH.FillH1("hNofMismatches_gTracks_all_" + ptcl, 6.5, w);
        fH.FillH1("hChi2_mismatch_all_tof_" + ptcl, chi2, w);
        nofMismTrackSegmentsAll++;
      }
      if (isFull) {
        if (tofMcTrackId >= 0) fH.FillH1("hNofMismatches_gTracks_complete_" + ptcl, 5.5, w);
        if (tofMcTrackId >= 0 && tofMcTrackId == stsMcTrackId)
          fH.FillH1("hChi2_truematch_complete_tof_" + ptcl, chi2, w);
        if (tofMcTrackId >= 0 && tofMcTrackId != stsMcTrackId) {
          fH.FillH1("hNofMismatches_gTracks_complete_" + ptcl, 6.5, w);
          fH.FillH1("hChi2_mismatch_complete_tof_" + ptcl, chi2, w);
          nofMismTrackSegmentsComp++;
        }
      }
    }
  }

  fH.FillH1("hNofMismatchedTrackSegments_all_" + ptcl, nofMismTrackSegmentsAll + 0.5, w);
  if (isFull) fH.FillH1("hNofMismatchedTrackSegments_complete_" + ptcl, nofMismTrackSegmentsComp + 0.5, w);

  bool isTrueId = (std::abs(pdg) == 11 && isElectron) ? true : (std::abs(pdg) != 11 && !isElectron) ? true : false;
  double binX   = nofMismTrackSegmentsComp + 0.5;
  double binY   = (isTrueId == true) ? 0.5 : 1.5;

  fH.FillH2("hMatchId_gTracks_" + ptcl, binX, binY, w);
}

bool LmvmTask::IsInAllDets(const CbmGlobalTrack* gTrack)
{
  if (gTrack == nullptr) return false;

  int stsInd = gTrack->GetStsTrackIndex();
  if (stsInd < 0) return false;
  CbmStsTrack* stsTrack = static_cast<CbmStsTrack*>(fStsTracks->At(stsInd));
  if (stsTrack == nullptr) return false;

  int richInd = gTrack->GetRichRingIndex();
  int trdInd  = gTrack->GetTrdTrackIndex();
  int tofInd  = gTrack->GetTofHitIndex();
  if (richInd < 0 || trdInd < 0 || tofInd < 0) return false;

  CbmRichRing* richRing = static_cast<CbmRichRing*>(fRichRings->At(richInd));
  CbmTrdTrack* trdTrack = static_cast<CbmTrdTrack*>(fTrdTracks->At(trdInd));
  CbmTofHit* tofHit     = static_cast<CbmTofHit*>(fTofHits->At(tofInd));
  if (richRing == nullptr || trdTrack == nullptr || tofHit == nullptr) return false;

  return true;
}

void LmvmTask::FillTopologyCands()
{
  fSTCands.clear();
  fRTCands.clear();
  int ngTracks = fGlobalTracks->GetEntriesFast();

  for (int iGTrack = 0; iGTrack < ngTracks; iGTrack++) {
    LmvmCand cand;

    CbmGlobalTrack* gTrack = (CbmGlobalTrack*) fGlobalTracks->At(iGTrack);
    if (gTrack == nullptr) continue;

    cand.fStsInd = gTrack->GetStsTrackIndex();
    if (cand.fStsInd < 0) continue;
    CbmStsTrack* stsTrack = static_cast<CbmStsTrack*>(fStsTracks->At(cand.fStsInd));
    if (stsTrack == nullptr) continue;

    cand.fRichInd     = gTrack->GetRichRingIndex();
    cand.fTrdInd      = gTrack->GetTrdTrackIndex();
    cand.fTofInd      = gTrack->GetTofHitIndex();    // TODO: is TofHitIndex; change this name everywhere
    cand.fTofTrackInd = gTrack->GetStsTrackIndex();  // TODO: is this right??

    LmvmUtils::CalculateAndSetTrackParams(&cand, stsTrack, fKFVertex);
    cand.fIsChi2Prim = fCuts.IsChi2PrimaryOk(cand.fChi2Prim);
    if (!cand.fIsChi2Prim) continue;

    // ST cut candidates, only STS
    if (cand.fRichInd < 0 && cand.fTrdInd < 0 && cand.fTofInd < 0) fSTCands.push_back(cand);

    // RT cut candidates, STS + at least one detector (RICH, TRD, TOF) but not all
    // Candidates must be identified as electron in registered detectors:
    // if it is registered in RICH it must be identified in the RICH as electron
    // RICH
    bool isRichRT = (cand.fRichInd < 0) ? false : true;
    if (isRichRT) {
      CbmRichRing* richRing = static_cast<CbmRichRing*>(fRichRings->At(cand.fRichInd));
      if (richRing == nullptr) isRichRT = false;
      if (isRichRT) isRichRT = CbmLitGlobalElectronId::GetInstance().IsRichElectron(iGTrack, cand.fMomentum.Mag());
    }

    // TRD
    bool isTrdRT = (cand.fTrdInd < 0) ? false : true;
    if (isTrdRT) {
      CbmTrdTrack* trdTrack = static_cast<CbmTrdTrack*>(fTrdTracks->At(cand.fTrdInd));
      if (trdTrack == nullptr) isTrdRT = false;
      if (isTrdRT) isTrdRT = CbmLitGlobalElectronId::GetInstance().IsTrdElectron(iGTrack, cand.fMomentum.Mag());
    }

    // ToF
    bool isTofRT = (cand.fTofInd < 0) ? false : true;
    if (isTofRT) {
      CbmTofHit* tofHit = static_cast<CbmTofHit*>(fTofHits->At(cand.fTofInd));
      if (tofHit == nullptr) isTofRT = false;
      if (isTofRT) isTofRT = CbmLitGlobalElectronId::GetInstance().IsTofElectron(iGTrack, cand.fMomentum.Mag());
    }

    if (isRichRT || isTrdRT || isTofRT) {
      if (!(cand.fRichInd >= 0 && cand.fTrdInd >= 0 && cand.fTofInd >= 0)) { fRTCands.push_back(cand); }
    }
  }
  LOG(info) << "fSTCands.size = " << fSTCands.size();
  LOG(info) << "fRTCands.size = " << fRTCands.size();

  AssignMcToTopologyCands(fSTCands);
  AssignMcToTopologyCands(fRTCands);
}

void LmvmTask::FillCands()
{
  fCands.clear();
  fTTCands.clear();
  int nGTracks = fGlobalTracks->GetEntriesFast();
  fCands.reserve(nGTracks);

  for (int iGTrack = 0; iGTrack < nGTracks; iGTrack++) {
    LmvmCand cand;
    // if MVD is not used set mvd cuts to true
    cand.fIsMvd1Cut   = !fUseMvd;
    cand.fIsMvd2Cut   = !fUseMvd;
    cand.fEventNumber = fEventNumber;

    CbmGlobalTrack* gTrack = static_cast<CbmGlobalTrack*>(fGlobalTracks->At(iGTrack));
    if (gTrack == nullptr) continue;

    // STS
    cand.fStsInd = gTrack->GetStsTrackIndex();
    if (cand.fStsInd < 0) continue;
    CbmStsTrack* stsTrack = static_cast<CbmStsTrack*>(fStsTracks->At(cand.fStsInd));
    if (stsTrack == nullptr) continue;

    // RICH - TRD - TOF
    cand.fRichInd     = gTrack->GetRichRingIndex();
    cand.fTrdInd      = gTrack->GetTrdTrackIndex();
    cand.fTofInd      = gTrack->GetTofHitIndex();
    cand.fTofTrackInd = gTrack->GetStsTrackIndex();

    // Set length and time
    cand.fLength = gTrack->GetLength() / 100.;
    Int_t tofInd = gTrack->GetTofHitIndex();
    if (tofInd >= 0) {
      CbmTofHit* tofHit = (CbmTofHit*) fTofHits->At(tofInd);
      if (NULL != tofHit) {
        double eventTime      = FairRunAna::Instance()->GetEventHeader()->GetEventTime();
        Double_t noOffsetTime = tofHit->GetTime() - eventTime;
        cand.fTime            = 0.2998 * noOffsetTime;  // time in ns -> transfrom to ct in m
      }
    }

    LmvmUtils::CalculateAndSetTrackParams(&cand, stsTrack, fKFVertex);
    cand.fIsChi2Prim = fCuts.IsChi2PrimaryOk(cand.fChi2Prim);
    cand.fIsPtCut    = fCuts.IsPtCutOk(cand.fMomentum.Perp());

    // Add all pions from STS for pion misidentification level study
    if (fPionMisidLevel >= 0.0) {
      CbmTrackMatchNew* stsMatch = (CbmTrackMatchNew*) fStsTrackMatches->At(cand.fStsInd);
      if (stsMatch == nullptr || stsMatch->GetNofLinks() == 0) continue;
      cand.fStsMcTrackId = stsMatch->GetMatchedLink().GetIndex();
      if (cand.fStsMcTrackId < 0) continue;
      CbmMCTrack* mcTrack1 = static_cast<CbmMCTrack*>(fMCTracks->At(cand.fStsMcTrackId));
      if (mcTrack1 == nullptr) continue;

      //check that pion track has track projection onto the photodetector plane
      const FairTrackParam* richProj = static_cast<FairTrackParam*>(fRichProj->At(iGTrack));
      if (richProj == nullptr || richProj->GetX() == 0 || richProj->GetY() == 0) continue;

      if (std::abs(mcTrack1->GetPdgCode()) == 211) {
        LmvmUtils::IsElectronMc(&cand, fMCTracks, fPionMisidLevel);
        fCands.push_back(cand);
        continue;
      }
    }

    if (cand.fRichInd < 0 || cand.fTrdInd < 0 || cand.fTofInd < 0 || cand.fTofTrackInd < 0) continue;

    cand.fGTrackInd = iGTrack;

    CbmRichRing* richRing = static_cast<CbmRichRing*>(fRichRings->At(cand.fRichInd));
    CbmTrdTrack* trdTrack = static_cast<CbmTrdTrack*>(fTrdTracks->At(cand.fTrdInd));
    CbmTofHit* tofHit     = static_cast<CbmTofHit*>(fTofHits->At(cand.fTofInd));
    CbmTofTrack* tofTrack = static_cast<CbmTofTrack*>(fTofTracks->At(cand.fTofTrackInd));
    if (richRing == nullptr || trdTrack == nullptr || tofHit == nullptr || tofTrack == nullptr) continue;

    cand.fChi2Rich = (richRing->GetNDF() > 0.) ? richRing->GetChi2() / richRing->GetNDF() : 0.;
    cand.fChi2Trd  = (trdTrack->GetNDF() > 0.) ? trdTrack->GetChiSq() / trdTrack->GetNDF() : 0.;

    cand.fTrdLikeEl = trdTrack->GetPidLikeEL();
    cand.fTrdLikePi = trdTrack->GetPidLikePI();

    cand.fTofDist = tofTrack->GetDistance();

    LmvmUtils::IsElectron(iGTrack, cand.fMomentum.Mag(), fCuts.fMomentumCut, &cand);
    LmvmUtils::IsRichElectron(iGTrack, cand.fMomentum.Mag(), &cand);
    LmvmUtils::IsTrdElectron(iGTrack, cand.fMomentum.Mag(), &cand);
    LmvmUtils::IsTofElectron(iGTrack, cand.fMomentum.Mag(), &cand);

    fCands.push_back(cand);

    if (!cand.fIsElectron && cand.fIsChi2Prim) fTTCands.push_back(cand);
  }
  LOG(info) << "fTTCands.size = " << fTTCands.size();
  LOG(info) << "fCands.size = " << fCands.size();

  AssignMcToCands(fCands);
  AssignMcToTopologyCands(fTTCands);
}

void LmvmTask::CombinatorialPairs()
{
  size_t nCand = fCandsTotal.size();
  for (size_t iC1 = 0; iC1 < nCand - 1; iC1++) {
    const auto& cand1 = fCandsTotal[iC1];

    for (size_t iC2 = iC1 + 1; iC2 < nCand; iC2++) {
      const auto& cand2 = fCandsTotal[iC2];
      LmvmKinePar pRec = LmvmKinePar::Create(&cand1, &cand2);
      double w          = 1.;
      if (cand1.IsMcSignal()) w = fW * MinvScale(cand1.fMassSig);
      else if (cand2.IsMcSignal())
        w = fW * MinvScale(cand2.fMassSig);
      bool isSameEvent = (cand1.fEventNumber == cand2.fEventNumber);

      // PLUTO electrons from two different events have to be scaled with w^2!
      if (!isSameEvent && cand1.IsMcSignal() && cand2.IsMcSignal())
        w = fW * fW * MinvScale(cand1.fMassSig) * MinvScale(cand2.fMassSig);
      for (auto step : fH.fAnaSteps) {
        if (cand1.IsCutTill(step) && cand2.IsCutTill(step)) {

          // all particles
          if (step == ELmvmAnaStep::Mc || step == ELmvmAnaStep::Acc) continue;
          if (cand1.fCharge * cand2.fCharge < 0) {
            if (isSameEvent) {
              fH.FillH1("hMinvCombPM_sameEv", step, pRec.fMinv, w);
              cout << "LmvmTask::CombinatorialPairs(): e+e- same event (all).  w = " << w
                   << endl;  //TODO: delete this line
            }
            else
              fH.FillH1("hMinvCombPM_mixedEv", step, pRec.fMinv, w);
          }
          if (cand1.fCharge < 0 && cand2.fCharge < 0) {
            if (isSameEvent) {
              fH.FillH1("hMinvCombMM_sameEv", step, pRec.fMinv, w);
              cout << "LmvmTask::CombinatorialPairs(): e-e- same event (all).  w = " << w
                   << endl;  //TODO: delete this line
            }
            else
              fH.FillH1("hMinvCombMM_mixedEv", step, pRec.fMinv, w);
          }
          if (cand1.fCharge > 0 && cand2.fCharge > 0) {
            if (isSameEvent) {
              fH.FillH1("hMinvCombPP_sameEv", step, pRec.fMinv, w);
              cout << "LmvmTask::CombinatorialPairs(): e+e+ same event (all).  w = " << w
                   << endl;  //TODO: delete this line
            }
            else
              fH.FillH1("hMinvCombPP_mixedEv", step, pRec.fMinv, w);
          }

          // only UrQMD particles (also misidentified)
          if (!cand1.IsMcSignal() && !cand2.IsMcSignal()) {
            if (isSameEvent) {
              if (cand1.fCharge * cand2.fCharge < 0) fH.FillH1("hMinvCombPM_urqmdAll_sameEv", step, pRec.fMinv, w);
              else if (cand1.fCharge < 0 && cand2.fCharge < 0)
                fH.FillH1("hMinvCombMM_urqmdAll_sameEv", step, pRec.fMinv, w);
              else if (cand1.fCharge > 0 && cand2.fCharge > 0)
                fH.FillH1("hMinvCombPP_urqmdAll_sameEv", step, pRec.fMinv, w);
            }
            else {
              if (cand1.fCharge * cand2.fCharge < 0) fH.FillH1("hMinvCombPM_urqmdAll_mixedEv", step, pRec.fMinv, w);
              else if (cand1.fCharge < 0 && cand2.fCharge < 0)
                fH.FillH1("hMinvCombMM_urqmdAll_mixedEv", step, pRec.fMinv, w);
              else if (cand1.fCharge > 0 && cand2.fCharge > 0)
                fH.FillH1("hMinvCombPP_urqmdAll_mixedEv", step, pRec.fMinv, w);
            }
          }

          // only UrQMD electrons
          if (!cand1.IsMcSignal() && !cand2.IsMcSignal() && std::abs(cand1.fMcPdg) == 11
              && std::abs(cand2.fMcPdg) == 11) {  // TODO: delete double brackets
            if (isSameEvent) {
              if (cand1.fCharge * cand2.fCharge < 0) fH.FillH1("hMinvCombPM_urqmdEl_sameEv", step, pRec.fMinv, w);
              else if (cand1.fCharge < 0 && cand2.fCharge < 0)
                fH.FillH1("hMinvCombMM_urqmdEl_sameEv", step, pRec.fMinv, w);
              else if (cand1.fCharge > 0 && cand2.fCharge > 0)
                fH.FillH1("hMinvCombPP_urqmdEl_sameEv", step, pRec.fMinv, w);
            }
            else {
              if (cand1.fCharge * cand2.fCharge < 0) fH.FillH1("hMinvCombPM_urqmdEl_mixedEv", step, pRec.fMinv, w);
              else if (cand1.fCharge < 0 && cand2.fCharge < 0)
                fH.FillH1("hMinvCombMM_urqmdEl_mixedEv", step, pRec.fMinv, w);
              else if (cand1.fCharge > 0 && cand2.fCharge > 0)
                fH.FillH1("hMinvCombPP_urqmdEl_mixedEv", step, pRec.fMinv, w);
            }
          }
        }  // isCutTillStep
      }    // steps
    }      // cand2
  }        // cand1
}

void LmvmTask::AssignMcToCands(vector<LmvmCand>& cands)
{
  for (auto& cand : cands) {
    cand.ResetMcParams();

    //STS. MCTrackId of the candidate is defined by STS track
    CbmTrackMatchNew* stsMatch = static_cast<CbmTrackMatchNew*>(fStsTrackMatches->At(cand.fStsInd));
    if (stsMatch == nullptr || stsMatch->GetNofLinks() == 0) continue;
    cand.fStsMcTrackId = stsMatch->GetMatchedLink().GetIndex();
    if (cand.fStsMcTrackId < 0) continue;
    CbmMCTrack* mct = static_cast<CbmMCTrack*>(fMCTracks->At(cand.fStsMcTrackId));
    if (mct == nullptr) continue;
    cand.fMcMotherId = mct->GetMotherId();
    cand.fMcPdg      = mct->GetPdgCode();
    cand.fMcSrc      = LmvmUtils::GetMcSrc(mct, fMCTracks);

    // Get mass of mother particle if it is signal (for mass-dependant scaling)
    if (cand.IsMcSignal()) {
      int nMcTracks = fMCTracks->GetEntries();
      for (int iMc2 = 0; iMc2 < nMcTracks; iMc2++) {
        CbmMCTrack* mct2 = static_cast<CbmMCTrack*>(fMCTracks->At(iMc2));
        if (mct2->GetMotherId() == cand.fMcMotherId && iMc2 != cand.fStsMcTrackId) {
          LmvmKinePar pKin = LmvmKinePar::Create(mct, mct2);
          cand.fMassSig    = pKin.fMinv;
        }
      }
    }

    if (std::abs(cand.fMcPdg) == 211 && fPionMisidLevel >= 0.) continue;

    // RICH
    CbmTrackMatchNew* richMatch = static_cast<CbmTrackMatchNew*>(fRichRingMatches->At(cand.fRichInd));
    if (richMatch == nullptr) continue;
    cand.fRichMcTrackId = richMatch->GetMatchedLink().GetIndex();

    // TRD
    CbmTrackMatchNew* trdMatch = static_cast<CbmTrackMatchNew*>(fTrdTrackMatches->At(cand.fTrdInd));
    if (trdMatch == nullptr) continue;
    cand.fTrdMcTrackId = trdMatch->GetMatchedLink().GetIndex();

    // ToF
    if (cand.fTofInd < 0) continue;
    CbmTofHit* tofHit = static_cast<CbmTofHit*>(fTofHits->At(cand.fTofInd));
    if (tofHit == nullptr) continue;
    CbmMatch* tofHitMatch = static_cast<CbmMatch*>(fTofHitsMatches->At(cand.fTofInd));
    if (tofHitMatch == nullptr) continue;
    int tofPointIndex = tofHitMatch->GetMatchedLink().GetIndex();
    if (tofPointIndex < 0) continue;
    FairMCPoint* tofPoint = static_cast<FairMCPoint*>(fTofPoints->At(tofPointIndex));
    if (tofPoint == nullptr) continue;
    cand.fTofMcTrackId = tofPoint->GetTrackID();
  }
}

void LmvmTask::AssignMcToTopologyCands(vector<LmvmCand>& topoCands)
{
  for (auto& cand : topoCands) {
    cand.ResetMcParams();
    if (cand.fStsInd < 0) continue;
    CbmTrackMatchNew* stsMatch = static_cast<CbmTrackMatchNew*>(fStsTrackMatches->At(cand.fStsInd));
    if (stsMatch == nullptr || stsMatch->GetNofLinks() == 0) continue;
    cand.fStsMcTrackId = stsMatch->GetMatchedLink().GetIndex();
    if (cand.fStsMcTrackId < 0) continue;
    CbmMCTrack* mct = static_cast<CbmMCTrack*>(fMCTracks->At(cand.fStsMcTrackId));
    if (mct == nullptr) continue;

    cand.fMcMotherId = mct->GetMotherId();
    cand.fMcPdg      = mct->GetPdgCode();
    cand.fMcSrc      = LmvmUtils::GetMcSrc(mct, fMCTracks);
  }
}

void LmvmTask::PairSource(const LmvmCand& candP, const LmvmCand& candM, ELmvmAnaStep step, const LmvmKinePar& parRec)
{
  ELmvmSrc src = LmvmUtils::GetMcPairSrc(candP, candM);
  double w     = (candP.IsMcSignal() || candM.IsMcSignal()) ? fW : 1.;
  fH.FillH1("hAnglePair", src, step, parRec.fAngle, w);

  if (src == ELmvmSrc::Bg) {
    // gamma=0.5, pi0=1.5, pions=2.5,  other=3.5
    double indM = candM.IsMcGamma() ? 0.5 : (candM.IsMcPi0() ? 1.5 : (std::abs(candM.fMcPdg) == 211) ? 2.5 : 3.5);
    double indP = candP.IsMcGamma() ? 0.5 : (candP.IsMcPi0() ? 1.5 : (std::abs(candP.fMcPdg) == 211) ? 2.5 : 3.5);
    fH.FillH2("hSrcBgPairsEpEm", step, indP, indM);

    ELmvmBgPairSrc bgSrc = LmvmUtils::GetBgPairSrc(candP, candM);
    if (bgSrc != ELmvmBgPairSrc::Undefined) {
      string name = fH.GetName("hMinvBgSource_" + fH.fBgPairSrcNames[static_cast<int>(bgSrc)], step);
      fH.FillH1(name, parRec.fMinv);
      fH.FillH2("hSrcBgPairs", static_cast<int>(step) + 0.5, static_cast<double>(bgSrc) + 0.5);

      if (step == ELmvmAnaStep::ElId) {
        string hName = "hMinvBgSource2_elid_";

        if (std::abs(candP.fMcPdg) == 11) {

          // cand1 is El and from Gamma
          if (candP.IsMcGamma()) {
            if (std::abs(candM.fMcPdg) == 11 && candM.IsMcGamma()) fH.FillH1(hName + "gg", parRec.fMinv);
            else if (std::abs(candM.fMcPdg) == 11 && candM.IsMcPi0())
              fH.FillH1(hName + "gpi0", parRec.fMinv);
            else if (std::abs(candM.fMcPdg) == 211)
              fH.FillH1(hName + "gpi", parRec.fMinv);
            else
              fH.FillH1(hName + "go", parRec.fMinv);
          }

          // cand1 is El and from Pi0
          else if (candP.IsMcPi0()) {
            if (std::abs(candM.fMcPdg) == 11 && candM.IsMcGamma()) fH.FillH1(hName + "gpi0", parRec.fMinv);
            else if (std::abs(candM.fMcPdg) == 11 && candM.IsMcPi0())
              fH.FillH1(hName + "pi0pi0", parRec.fMinv);
            else if (std::abs(candM.fMcPdg) == 211)
              fH.FillH1(hName + "pipi0", parRec.fMinv);
            else
              fH.FillH1(hName + "pi0o", parRec.fMinv);
          }

          // cand1 is El but not from Gamma or Pi0
          else {
            if (std::abs(candM.fMcPdg) == 11 && candM.IsMcGamma()) fH.FillH1(hName + "go", parRec.fMinv);
            else if (std::abs(candM.fMcPdg) == 11 && candM.IsMcPi0())
              fH.FillH1(hName + "pi0o", parRec.fMinv);
            else if (std::abs(candM.fMcPdg) == 211)
              fH.FillH1(hName + "pio", parRec.fMinv);
            else
              fH.FillH1(hName + "oo", parRec.fMinv);
          }
        }

        // cand1 is misid. charged pion
        else if (std::abs(candP.fMcPdg) == 211) {
          if (std::abs(candM.fMcPdg) == 11 && candM.IsMcGamma()) fH.FillH1(hName + "gpi", parRec.fMinv);
          else if (std::abs(candM.fMcPdg) == 11 && candM.IsMcPi0())
            fH.FillH1(hName + "pipi0", parRec.fMinv);
          else if (std::abs(candM.fMcPdg) == 211)
            fH.FillH1(hName + "pipi", parRec.fMinv);
          else
            fH.FillH1(hName + "pio", parRec.fMinv);
        }

        // cand1 is neither electron nor misid. charged pion
        else {
          if (std::abs(candM.fMcPdg) == 11 && candM.IsMcGamma()) fH.FillH1(hName + "go", parRec.fMinv);
          else if (std::abs(candM.fMcPdg) == 11 && candM.IsMcPi0())
            fH.FillH1(hName + "pi0o", parRec.fMinv);
          else if (std::abs(candM.fMcPdg) == 211)
            fH.FillH1(hName + "pipi0", parRec.fMinv);
          else
            fH.FillH1(hName + "oo", parRec.fMinv);
        }
      }
    }
  }
}

void LmvmTask::TrackSource(const LmvmCand& cand, ELmvmAnaStep step, int pdg)
{
  // no need to fill histograms for MC and Acc steps
  if (step == ELmvmAnaStep::Mc || step == ELmvmAnaStep::Acc) return;

  double stepBin = static_cast<double>(step) + 0.5;

  FillMomHists(nullptr, &cand, cand.fMcSrc, step);
  fH.FillH1("hCandPdg", step, cand.fMcPdg);

  int absPdg    = std::abs(pdg);
  double pdgBin = (absPdg == 11 && cand.IsMcSignal())    ? 0.5
                  : (absPdg == 11 && !cand.IsMcSignal()) ? 1.5
                  : (absPdg == 211)                      ? 2.5
                  : (absPdg == 2212)                     ? 3.5
                  : (absPdg == 321)                      ? 4.5
                                                         : 5.5;
  fH.FillH2("hCandPdgVsMom", step, cand.fMomentum.Mag(), pdgBin);

  if (cand.IsMcSignal()) {
    fH.FillH1("hNofSignalTracks", stepBin, fW);
    fH.FillH2("hCandElSrc", stepBin, 7.5, fW);
  }
  else {
    if (LmvmUtils::IsMismatch(cand)) fH.FillH1("hNofMismatches_all", stepBin);
    if (cand.fStsMcTrackId != cand.fRichMcTrackId) fH.FillH1("hNofMismatches_rich", stepBin);
    if (cand.fStsMcTrackId != cand.fTrdMcTrackId) fH.FillH1("hNofMismatches_trd", stepBin);
    if (cand.fStsMcTrackId != cand.fTofMcTrackId) fH.FillH1("hNofMismatches_tof", stepBin);
    if (LmvmUtils::IsGhost(cand)) fH.FillH1("hNofGhosts", stepBin);
    fH.FillH1("hNofBgTracks", stepBin);

    if (cand.IsMcGamma()) {
      CbmMCTrack* mctrack = static_cast<CbmMCTrack*>(fMCTracks->At(cand.fStsMcTrackId));
      if (mctrack != nullptr) {
        TVector3 v;
        mctrack->GetStartVertex(v);
        fH.FillH2("hVertexGammaXZ", step, v.Z(), v.X());
        fH.FillH2("hVertexGammaYZ", step, v.Z(), v.Y());
        fH.FillH2("hVertexGammaXY", step, v.X(), v.Y());
        fH.FillH2("hVertexGammaRZ", step, v.Z(), sqrt(v.X() * v.X() + v.Y() * v.Y()));
      }
    }

    double srcBin = 0.0;
    if (cand.IsMcGamma()) srcBin = 0.5;
    else if (cand.IsMcPi0())
      srcBin = 1.5;
    else if (std::abs(pdg) == 211)
      srcBin = 2.5;
    else if (pdg == 2212)
      srcBin = 3.5;
    else if (std::abs(pdg) == 321)
      srcBin = 4.5;
    else if ((std::abs(pdg) == 11) && !cand.IsMcGamma() && !cand.IsMcPi0() && !cand.IsMcSignal())
      srcBin = 5.5;
    else
      srcBin = 6.5;
    fH.FillH2("hBgSrcTracks", stepBin, srcBin);
    if (std::abs(cand.fMcPdg) == 11) fH.FillH2("hCandElSrc", stepBin, srcBin);
  }
}

void LmvmTask::BgPairPdg(const LmvmCand& candP, const LmvmCand& candM, ELmvmAnaStep step)
{
  int pdgX = candP.fMcPdg;
  int pdgY = candM.fMcPdg;

  double pdgBinX = (std::abs(pdgX) == 11 && candP.IsMcSignal())    ? 0.5
                   : (std::abs(pdgX) == 11 && !candP.IsMcSignal()) ? 1.5
                   : (std::abs(pdgX) == 211)                       ? 2.5
                   : (pdgX == 2212)                                ? 3.5
                   : (pdgX == 321)                                 ? 4.5
                   : (pdgX == 3112 or pdgX == 3222)                ? 5.5
                   : (std::abs(pdgX) == 13)                        ? 6.5
                                                                   : 7.5;
  double pdgBinY = (std::abs(pdgY) == 11 && candM.IsMcSignal())    ? 0.5
                   : (std::abs(pdgY) == 11 && !candM.IsMcSignal()) ? 1.5
                   : (std::abs(pdgY) == 211)                       ? 2.5
                   : (pdgY == 2212)                                ? 3.5
                   : (pdgY == 321)                                 ? 4.5
                   : (pdgY == 3112 or pdgY == 3222)                ? 5.5
                   : (std::abs(pdgY) == 13)                        ? 6.5
                                                                   : 7.5;

  fH.FillH2("hBgPairPdg", step, pdgBinX, pdgBinY);
}

void LmvmTask::FillPairHists(const LmvmCand& candP, const LmvmCand& candM, const LmvmKinePar& parMc,
                             const LmvmKinePar& parRec, ELmvmAnaStep step)
{
  // no need to fill histograms for MC and Acc steps
  if (step == ELmvmAnaStep::Mc || step == ELmvmAnaStep::Acc) return;
  bool isMismatch = (LmvmUtils::IsMismatch(candP) || LmvmUtils::IsMismatch(candM));
  ELmvmSrc src    = LmvmUtils::GetMcPairSrc(candP, candM);

  double w = 1.;
  if (candP.IsMcSignal()) w = fW * MinvScale(candP.fMassSig);
  if (candM.IsMcSignal()) w = fW * MinvScale(candM.fMassSig);
  if (w < 0) LOG(warning) << "LmvmTask::FillPairHists(): Signal mass < 0!";

  fH.FillH1("hMinv", src, step, parRec.fMinv, w);
  if (!candP.IsMcSignal() && !candM.IsMcSignal()) fH.FillH1("hMinv_urqmdAll", src, step, parRec.fMinv, w);
  if (!candP.IsMcSignal() && !candM.IsMcSignal() && std::abs(candP.fMcPdg) == 11 && std::abs(candM.fMcPdg) == 11)
    fH.FillH1("hMinv_urqmdEl", src, step, parRec.fMinv, w);

  fH.FillH2("hMinvPt", src, step, parRec.fMinv, parMc.fPt, w);

  PairSource(candP, candM, step, parRec);

  if (src == ELmvmSrc::Signal) {
    fH.FillH2("hPtYPairSignal", step, parMc.fRapidity, parMc.fPt, fW);
    fH.FillH1("hMomPairSignal", step, parMc.fMomentumMag, fW);
  }
  if (src == ELmvmSrc::Bg) {
    BgPairPdg(candP, candM, step);
    if (isMismatch) { fH.FillH1("hMinvBgMatch_mismatch", step, parRec.fMinv); }
    else {
      fH.FillH1("hMinvBgMatch_trueMatch", step, parRec.fMinv);
      if (std::abs(candP.fMcPdg) == 11 && std::abs(candM.fMcPdg) == 11)
        fH.FillH1("hMinvBgMatch_trueMatchEl", step, parRec.fMinv);
      if (std::abs(candP.fMcPdg) != 11 || std::abs(candM.fMcPdg) != 11)
        fH.FillH1("hMinvBgMatch_trueMatchNotEl", step, parRec.fMinv);
    }
  }
}

string LmvmTask::GetPidString(const CbmMCTrack* mct, const LmvmCand* cand)
{
  if ((mct != nullptr && cand != nullptr) || (mct == nullptr && cand == nullptr)) {
    LOG(error) << "LmvmTask::GetPidString: Both mct and cand are [not nullptr] or [nullptr].";
    return 0;
  }

  int pdg         = (mct != nullptr) ? mct->GetPdgCode() : cand->fMcPdg;
  bool isMcSignal = (mct != nullptr) ? (mct->GetGeantProcessId() == kPPrimary && std::abs(mct->GetPdgCode()) == 11)
                                     : cand->IsMcSignal();

  string pidString = fH.fCandNames[9];
  if (isMcSignal && pdg == -11) pidString = fH.fCandNames[0];
  else if (isMcSignal && pdg == 11)
    pidString = fH.fCandNames[1];
  else if (!isMcSignal && pdg == -11)
    pidString = fH.fCandNames[2];
  else if (!isMcSignal && pdg == 11)
    pidString = fH.fCandNames[3];
  else if (pdg == 211)
    pidString = fH.fCandNames[4];
  else if (pdg == -211)
    pidString = fH.fCandNames[5];
  else if (pdg == 2212)
    pidString = fH.fCandNames[6];
  else if (pdg == 321)
    pidString = fH.fCandNames[7];
  else if (pdg == -321)
    pidString = fH.fCandNames[8];

  return pidString;
}

string LmvmTask::GetPidString(double vertexMag, int pdg)
{
  string pidString = fH.fGTrackNames[14];
  bool isPrim      = IsPrimary(vertexMag);

  if (isPrim) {
    if (pdg == -11) pidString = fH.fGTrackNames[1];
    else if (pdg == 11)
      pidString = fH.fGTrackNames[3];
    else if (pdg == 211)
      pidString = fH.fGTrackNames[5];
    else if (pdg == -211)
      pidString = fH.fGTrackNames[7];
    else if (pdg == 2212)
      pidString = fH.fGTrackNames[9];
    else if (pdg == 321)
      pidString = fH.fGTrackNames[11];
    else if (pdg == -321)
      pidString = fH.fGTrackNames[13];
  }
  else {
    if (pdg == -11) pidString = fH.fGTrackNames[0];
    else if (pdg == 11)
      pidString = fH.fGTrackNames[2];
    else if (pdg == 211)
      pidString = fH.fGTrackNames[4];
    else if (pdg == -211)
      pidString = fH.fGTrackNames[6];
    else if (pdg == 2212)
      pidString = fH.fGTrackNames[8];
    else if (pdg == 321)
      pidString = fH.fGTrackNames[10];
    else if (pdg == -321)
      pidString = fH.fGTrackNames[12];
  }

  return pidString;
}

void LmvmTask::AnalyseCandidates()
{
  CheckGammaConvAndPi0();
  CheckTopologyCut(ELmvmTopologyCut::ST, "hStCut");
  CheckTopologyCut(ELmvmTopologyCut::TT, "hTtCut");
  CheckTopologyCut(ELmvmTopologyCut::RT, "hRtCut");
  if (fUseMvd) {
    CheckClosestMvdHit(1, "hMvdCut_1", "hMvdCutQa_1");
    CheckClosestMvdHit(2, "hMvdCut_2", "hMvdCutQa_2");
  }

  // single candidates
  for (const auto& cand : fCands) {
    double w            = (cand.IsMcSignal()) ? fW : 1.;
    CbmMCTrack* mcTrack = nullptr;
    if (cand.fStsMcTrackId >= 0) mcTrack = static_cast<CbmMCTrack*>(fMCTracks->At(cand.fStsMcTrackId));
    int pdg          = mcTrack->GetPdgCode();
    double mom       = mcTrack->GetP();
    string pidString = GetPidString(nullptr, &cand);

    for (auto step : fH.fAnaSteps) {
      if (cand.IsCutTill(step)) {
        TrackSource(cand, step, pdg);
        FillCandPidValues(mcTrack, cand, step);  // if std::abs(pdg) != 11, this is misidentification
        if (mcTrack != nullptr) CheckTofId(mcTrack, cand, step, pdg);

        if (step >= ELmvmAnaStep::ElId && std::abs(pdg) == 211) fH.FillH1("hPionSupp_idEl", step, mom, w);

        //double richDist  = CbmRichUtil::GetRingTrackDistance(cand.fGTrackInd);
        //fH.FillH2("hRichRingTrackDist_cands_" + pidString, step, mom, richDist, w); // TODO: causes 'Error in <TClonesArray::At>: index xxx out of bounds' error
      }
    }

    if (cand.IsCutTill(ELmvmAnaStep::ElId)) {
      fH.FillH2("hChi2VsMom_sts_" + pidString, cand.fMomentum.Mag(), cand.fChi2Sts, w);
      fH.FillH2("hChi2VsMom_rich_" + pidString, cand.fMomentum.Mag(), cand.fChi2Rich, w);
      fH.FillH2("hChi2VsMom_trd_" + pidString, cand.fMomentum.Mag(), cand.fChi2Trd, w);

      fH.FillH2("hTofTimeVsChi2_sts_" + pidString, cand.fChi2Sts, cand.fTime, w);
      fH.FillH2("hTofTimeVsChi2_rich_" + pidString, cand.fChi2Rich, cand.fTime, w);
      fH.FillH2("hTofTimeVsChi2_trd_" + pidString, cand.fChi2Trd, cand.fTime, w);

      fH.FillH2("hChi2Comb_StsRich_" + pidString, cand.fChi2Sts, cand.fChi2Rich, w);
      fH.FillH2("hChi2Comb_StsTrd_" + pidString, cand.fChi2Sts, cand.fChi2Trd, w);
      fH.FillH2("hChi2Comb_RichTrd_" + pidString, cand.fChi2Rich, cand.fChi2Trd, w);

      fH.FillH2("hTofTimeVsMom_cands_" + pidString, cand.fMomentum.Mag(), cand.fTime, w);
      fH.FillH2("hTofHitTrackDist_cands_" + pidString, cand.fMomentum.Mag(), cand.fTofDist, w);
    }

    DifferenceSignalAndBg(cand);

    // beta-momentum spectrum
    string ptcl = GetPidString(nullptr, &cand);
    double p    = cand.fMomentum.Mag();
    double m    = cand.fMass;
    double r    = p / m;
    double beta = r / sqrt(1. + r * r);
    double q    = (cand.fCharge > 0) ? 1 : (cand.fCharge < 0) ? -1. : 0.;
    fH.FillH2("hBetaMom_cands_" + ptcl, q * p, beta);
  }  // single candidates

  // candidate pairs
  for (const auto& candP : fCands) {
    if (candP.fCharge < 0) continue;
    CbmMCTrack* mctrackP =
      (candP.fStsMcTrackId >= 0) ? static_cast<CbmMCTrack*>(fMCTracks->At(candP.fStsMcTrackId)) : nullptr;
    for (const auto& candM : fCands) {
      if (candM.fCharge > 0) continue;

      CbmMCTrack* mctrackM =
        (candM.fStsMcTrackId >= 0) ? static_cast<CbmMCTrack*>(fMCTracks->At(candM.fStsMcTrackId)) : nullptr;

      LmvmKinePar pMC  = LmvmKinePar::Create(mctrackP, mctrackM);
      LmvmKinePar pRec = LmvmKinePar::Create(&candP, &candM);

      for (auto step : fH.fAnaSteps) {
        if (candP.IsCutTill(step) && candM.IsCutTill(step)) FillPairHists(candP, candM, pMC, pRec, step);
      }
    }
  }
}

void LmvmTask::CheckTofId(const CbmMCTrack* mct, const LmvmCand& cand, ELmvmAnaStep step, int pdg)
{
  TVector3 v;
  mct->GetStartVertex(v);
  bool isPrim = IsPrimary(v.Mag());
  double pt   = mct->GetPt();
  double rap  = mct->GetRapidity();

  // check PIDs in "Tof pile"
  if (cand.fMomentum.Mag() > 0.3 && cand.fMomentum.Mag() < 1. && cand.fMass2 > -0.012 && cand.fMass2 < 0.01) {
    string pidString = GetPidString(nullptr, &cand);
    double pdgBin    = (pdg == 11 && cand.IsMcSignal())                ? 0.5
                       : (pdg == -11 && cand.IsMcSignal())             ? 1.5
                       : (pdg == 11 && !cand.IsMcSignal() && isPrim)   ? 2.5
                       : (pdg == -11 && !cand.IsMcSignal() && isPrim)  ? 3.5
                       : (pdg == 11 && !cand.IsMcSignal() && !isPrim)  ? 4.5
                       : (pdg == -11 && !cand.IsMcSignal() && !isPrim) ? 5.5
                       : (pdg == 211)                                  ? 6.5
                       : (pdg == -211)                                 ? 7.5
                       : (pdg == 2212)                                 ? 8.5
                       : (pdg == 321)                                  ? 9.5
                       : (pdg == -321)                                 ? 10.5
                                                                       : 11.5;
    fH.FillH1("hTofPilePdgs_cands", step, pdgBin);
    if (step == ELmvmAnaStep::ElId && cand.IsCutTill(step))
      fH.FillH2("hTofPilePty_cands_" + pidString, rap, pt);  // TODO: check newly (25.10.22) added '&& IsTill(step)'!!
  }

  // check vertex of misidentified particles in ToF after electron ID // TODO: split this up into single contributions?
  if (std::abs(pdg) != 11 && cand.fIsTofElectron) {
    fH.FillH2("hVertexXZ_misidTof", step, v.Z(), v.X());
    fH.FillH2("hVertexYZ_misidTof", step, v.Z(), v.Y());
    fH.FillH2("hVertexXY_misidTof", step, v.X(), v.Y());
    fH.FillH2("hVertexRZ_misidTof", step, v.Z(), sqrt(v.X() * v.X() + v.Y() * v.Y()));
  }
}

void LmvmTask::FillCandPidValues(const CbmMCTrack* mct, const LmvmCand& cand, ELmvmAnaStep step)
{
  double w         = (cand.IsMcSignal()) ? fW : 1.;
  string pidString = GetPidString(nullptr, &cand);

  fH.FillH1("hMom_cands_" + pidString, step, cand.fMomentum.Mag(), w);
  fH.FillH2("hPtY_cands_" + pidString, step, cand.fRapidity, cand.fMomentum.Perp(), w);
  fH.FillH2("hTofM2_cands_" + pidString, step, cand.fMomentum.Mag(), cand.fMass2, w);

  if (mct != nullptr) {
    double rat  = cand.fMomentum.Mag() / mct->GetP();
    string ptcl = GetPidString(nullptr, &cand);
    fH.FillH1("hMomRatio_cands_" + pidString, step, rat);
    fH.FillH2("hMomRatioVsMom_cands_" + ptcl, step, mct->GetP(), rat);
  }
}

void LmvmTask::CheckGammaConvAndPi0()
{
  for (auto& candP : fCands) {
    if (candP.fCharge < 0) continue;
    for (auto& candM : fCands) {
      if (candM.fCharge > 0) continue;
      if (candP.IsCutTill(ELmvmAnaStep::ElId) && candM.IsCutTill(ELmvmAnaStep::ElId)) {
        LmvmKinePar pRec = LmvmKinePar::Create(&candP, &candM);
        if (!fCuts.IsGammaCutOk(pRec.fMinv)) {
          candM.fIsGammaCut = false;
          candP.fIsGammaCut = false;
        }
      }
    }
  }
}

void LmvmTask::CheckTopologyCut(ELmvmTopologyCut cut, const string& name)
{
  string hcut         = name + "_all";
  string hcutPion     = name + "_pion";
  string hcutTruePair = name + "_truePair";

  vector<LmvmDataAngMomInd> dataV;

  vector<LmvmCand>& tpCands = fSTCands;
  if (cut == ELmvmTopologyCut::ST) { tpCands = fSTCands; }
  else if (cut == ELmvmTopologyCut::RT) {
    tpCands = fRTCands;
  }
  else if (cut == ELmvmTopologyCut::TT) {
    tpCands = fTTCands;
  }
  else {
    LOG(error) << "LmvmTask::CheckTopologyCut cut is not defined.";
  }

  for (auto& cand : fCands) {
    if (cand.IsCutTill(ELmvmAnaStep::ElId)) {
      dataV.clear();
      for (size_t iM = 0; iM < tpCands.size(); iM++) {
        // different charges, charge iM != charge iP
        if (tpCands[iM].fCharge != cand.fCharge) {
          LmvmKinePar pRec = LmvmKinePar::Create(&cand, &tpCands[iM]);
          dataV.emplace_back(pRec.fAngle, tpCands[iM].fMomentum.Mag(), iM);
        }
      }
      //find min opening angle
      double minAng = 360.;
      int minInd    = -1;
      for (size_t i = 0; i < dataV.size(); i++) {
        if (minAng > dataV[i].fAngle) {
          minAng = dataV[i].fAngle;
          minInd = i;
        }
      }
      if (minInd == -1) {
        cand.SetIsTopologyCutElectron(cut, true);
        continue;
      }
      bool isCut = fCuts.IsTopologyCutOk(cut, cand.fMomentum.Mag(), dataV[minInd].fMom, minAng);
      cand.SetIsTopologyCutElectron(cut, isCut);

      // histogramms
      double sqrt_mom = TMath::Sqrt(cand.fMomentum.Mag() * dataV[minInd].fMom);
      int cutCandInd  = dataV[minInd].fInd;
      int stsInd      = tpCands[cutCandInd].fStsInd;
      if (stsInd < 0) continue;
      int pdgAbs   = std::abs(tpCands[cutCandInd].fMcPdg);
      int motherId = tpCands[cutCandInd].fMcMotherId;

      fH.FillH2(hcut, cand.fMcSrc, sqrt_mom, minAng, fW);
      if (pdgAbs == 211) fH.FillH2(hcutPion, cand.fMcSrc, sqrt_mom, minAng, fW);
      if (cand.IsMcSignal()) {
        if (motherId == cand.fMcMotherId) fH.FillH2(hcutTruePair, cand.fMcSrc, sqrt_mom, minAng, fW);
      }
      else {
        if (motherId != -1 && motherId == cand.fMcMotherId) fH.FillH2(hcutTruePair, cand.fMcSrc, sqrt_mom, minAng, fW);
      }
    }
  }
}

void LmvmTask::CalculateNofTopologyPairs(const string& name, ELmvmSrc src)
{
  size_t nCand = fCands.size();
  for (size_t iP = 0; iP < nCand; iP++) {
    const LmvmCand& cand = fCands[iP];
    if (cand.fMcMotherId == -1) continue;
    if (src != cand.fMcSrc) continue;
    if (!cand.IsCutTill(ELmvmAnaStep::ElId)) continue;

    bool isAdded = false;

    // 3 topology cuts: ST, RT, TT
    for (int i = 0; i < 3; i++) {
      if (isAdded) continue;
      vector<LmvmCand>& cands = fSTCands;
      double binNum           = 4.5;
      if (i == 1) {
        cands  = fRTCands;
        binNum = 5.5;
      }
      else if (i == 2) {
        cands  = fTTCands;
        binNum = 6.5;
      }
      for (const auto& candT : cands) {
        if (candT.fMcMotherId == cand.fMcMotherId) {
          fH.FillH1(name, binNum);
          isAdded = true;
          break;
        }
      }
    }
    if (isAdded) continue;

    for (size_t iM = 0; iM < fCands.size(); iM++) {
      if (iM != iP && fCands[iM].fMcMotherId == cand.fMcMotherId && fCands[iM].IsCutTill(ELmvmAnaStep::ElId)) {
        fH.FillH1(name, 7.5);
        isAdded = true;
        break;
      }
    }
    if (isAdded) continue;

    int nofStsPoints = 0;
    int nofMcTracks  = fMCTracks->GetEntriesFast();
    for (int iMc = 0; iMc < nofMcTracks; iMc++) {
      const CbmMCTrack* mcTrack = static_cast<const CbmMCTrack*>(fMCTracks->At(iMc));
      if (mcTrack == nullptr || mcTrack->GetMotherId() != cand.fMcMotherId || iMc == cand.fStsMcTrackId) continue;

      int eventId = FairRun::Instance()->GetEventHeader()->GetMCEntryNumber();
      if (!CbmLitMCTrackCreator::Instance()->TrackExists(eventId, iMc)) continue;
      const CbmLitMCTrack& litMCTrack = CbmLitMCTrackCreator::Instance()->GetTrack(eventId, iMc);
      nofStsPoints                    = litMCTrack.GetNofPointsInDifferentStations(ECbmModuleId::kSts);
      break;
    }
    if (nofStsPoints == 0) fH.FillH1(name, 0.5);
    if (nofStsPoints >= 1 && nofStsPoints <= 3) fH.FillH1(name, 1.5);
    if (nofStsPoints >= 4 && nofStsPoints <= 5) fH.FillH1(name, 2.5);
    if (nofStsPoints >= 6) fH.FillH1(name, 3.5);
  }
}

void LmvmTask::DifferenceSignalAndBg(const LmvmCand& cand)
{
  fH.FillH1("hChi2PrimVertex", cand.fMcSrc, cand.fChi2Prim, fW);

  if (!cand.fIsChi2Prim) return;
  fH.FillH1("hAnnRich", cand.fMcSrc, cand.fRichAnn, fW);
  fH.FillH2("hAnnRichVsMom", cand.fMcSrc, cand.fMomentum.Mag(), cand.fRichAnn, fW);
  //fH.FillH1("hAnnTrd", cand.fMcSrc, cand.fTrdAnn, fW);  // TODO: uncomment when TRD ANN is working (CbmLitGlobalElectronId::GetTrdAnn() gives back El-Likelihood)
  fH.FillH2("hTrdLike_El", cand.fMcSrc, cand.fMomentum.Mag(), cand.fTrdLikeEl, fW);
  fH.FillH2("hTrdLike_Pi", cand.fMcSrc, cand.fMomentum.Mag(), cand.fTrdLikePi, fW);
  fH.FillH2("hTofM2", cand.fMcSrc, cand.fMomentum.Mag(), cand.fMass2, fW);

  // electron purity
  if (!cand.IsMcSignal() && std::abs(cand.fMcPdg) == 11) {
    fH.FillH2("hAnnRichVsMomPur_El", cand.fMomentum.Mag(), cand.fRichAnn, fW);
    fH.FillH2("hTrdElLikePur_El", cand.fMomentum.Mag(), cand.fTrdLikeEl, fW);
  }
  else if (!cand.IsMcSignal() && std::abs(cand.fMcPdg) != 11) {
    fH.FillH2("hAnnRichVsMomPur_Bg", cand.fMomentum.Mag(), cand.fRichAnn, fW);
    fH.FillH2("hTrdElLikePur_Bg", cand.fMomentum.Mag(), cand.fTrdLikeEl, fW);
  }

  if (!cand.IsCutTill(ELmvmAnaStep::ElId)) return;
  //fH.FillSourceH1("hPt", cand.fMcSrc, cand.fMomentum.Perp(), fW);
  //fH.FillSourceH1("hMom", cand.fMcSrc, cand.fMomentum.Mag(), fW);
  fH.FillH1("hChi2Sts", cand.fMcSrc, cand.fChi2Sts, fW);

  CbmStsTrack* stsTrack = static_cast<CbmStsTrack*>(fStsTracks->At(cand.fStsInd));
  if (stsTrack == nullptr) return;
  fH.FillH1("hNofStsHits", cand.fMcSrc, stsTrack->GetNofHits(), fW);

  if (fUseMvd) {
    double mvd1x = 0., mvd1y = 0., mvd2x = 0., mvd2y = 0.;
    for (int iM = 0; iM < stsTrack->GetNofMvdHits(); iM++) {
      CbmMvdHit* mvdHit = static_cast<CbmMvdHit*>(fMvdHits->At(stsTrack->GetMvdHitIndex(iM)));
      if (mvdHit == nullptr) return;
      if (mvdHit->GetStationNr() == 1) {
        mvd1x = mvdHit->GetX();
        mvd1y = mvdHit->GetY();
      }
      else if (mvdHit->GetStationNr() == 2) {
        mvd2x = mvdHit->GetX();
        mvd2y = mvdHit->GetY();
      }
    }
    double mvd1r = sqrt(mvd1x * mvd1x + mvd1y * mvd1y);
    double mvd2r = sqrt(mvd2x * mvd2x + mvd2y * mvd2y);
    fH.FillH1("hNofMvdHits", cand.fMcSrc, stsTrack->GetNofMvdHits(), fW);
    fH.FillH2("hMvdXY_1", cand.fMcSrc, mvd1x, mvd1y, fW);
    fH.FillH1("hMvdR_1", cand.fMcSrc, mvd1r, fW);
    fH.FillH2("hMvdXY_2", cand.fMcSrc, mvd2x, mvd2y, fW);
    fH.FillH1("hMvdR_2", cand.fMcSrc, mvd2r, fW);
  }
}

void LmvmTask::CheckClosestMvdHit(int mvdStationNum, const string& hist, const string& histQa)
{
  vector<LmvmDataXYInd> mvdV;
  vector<LmvmDataXYInd> candV;

  for (int iHit = 0; iHit < fMvdHits->GetEntriesFast(); iHit++) {
    CbmMvdHit* mvdHit = static_cast<CbmMvdHit*>(fMvdHits->At(iHit));
    if (mvdHit != nullptr && mvdHit->GetStationNr() == mvdStationNum) {
      mvdV.emplace_back(mvdHit->GetX(), mvdHit->GetY(), iHit);
    }
  }

  for (size_t iC = 0; iC < fCands.size(); iC++) {
    if (fCands[iC].IsCutTill(ELmvmAnaStep::ElId)) {
      CbmStsTrack* stsTrack = static_cast<CbmStsTrack*>(fStsTracks->At(fCands[iC].fStsInd));
      if (stsTrack == nullptr) continue;
      for (int iM = 0; iM < stsTrack->GetNofMvdHits(); iM++) {
        CbmMvdHit* candHit = static_cast<CbmMvdHit*>(fMvdHits->At(stsTrack->GetMvdHitIndex(iM)));
        if (candHit != nullptr && candHit->GetStationNr() == mvdStationNum) {
          candV.emplace_back(candHit->GetX(), candHit->GetY(), iC);
        }
      }
    }
  }

  for (size_t iC = 0; iC < candV.size(); iC++) {
    LmvmCand& cand = fCands[candV[iC].fInd];
    double minD    = 9999999.;
    int minMvdInd  = -1;
    for (size_t iH = 0; iH < mvdV.size(); iH++) {
      double d2 = LmvmUtils::Distance2(mvdV[iH].fX, mvdV[iH].fY, candV[iC].fX, candV[iC].fY);
      if (d2 < 1.e-9) continue;
      if (d2 < minD) {
        minMvdInd = mvdV[iH].fInd;
        minD      = d2;
      }
    }
    double dmvd = sqrt(minD);

    // Check MVD cut quality
    double bin               = -1.;
    const CbmMatch* hitMatch = static_cast<const CbmMatch*>(fMvdHitMatches->At(minMvdInd));
    if (hitMatch != nullptr) {
      CbmMCTrack* mct1 = static_cast<CbmMCTrack*>(fMCTracks->At(hitMatch->GetMatchedLink().GetIndex()));
      int mcMvdHitPdg  = TMath::Abs(mct1->GetPdgCode());
      int mvdMotherId  = mct1->GetMotherId();

      int stsMotherId = -2;
      if (cand.fStsMcTrackId >= 0) {
        CbmMCTrack* mct2 = static_cast<CbmMCTrack*>(fMCTracks->At(cand.fStsMcTrackId));
        stsMotherId      = (mct2 != nullptr) ? mct2->GetMotherId() : -2;
      }

      bin = (mvdMotherId != -1 && mvdMotherId == stsMotherId) ? 0.5 : 1.5;  // correct or wrong assignment
      if (cand.IsMcSignal()) {
        bin = (mvdMotherId == stsMotherId && mcMvdHitPdg == 11) ? 0.5 : 1.5;  // correct or wrong assignment
      }
    }

    // Fill histograms
    fH.FillH1(histQa, cand.fMcSrc, bin, fW);
    fH.FillH2(hist, cand.fMcSrc, dmvd, cand.fMomentum.Mag(), fW);

    // Apply MVD cut
    bool isMvdCut = fCuts.IsMvdCutOk(mvdStationNum, dmvd, cand.fMomentum.Mag());
    if (mvdStationNum == 1) cand.fIsMvd1Cut = isMvdCut;
    else if (mvdStationNum == 2)
      cand.fIsMvd2Cut = isMvdCut;
  }
}

void LmvmTask::MvdCutMcDistance()
{
  if (!fUseMvd) return;
  for (const auto& cand : fCands) {
    if (!cand.IsCutTill(ELmvmAnaStep::ElId)) continue;
    CbmStsTrack* stsTrack = static_cast<CbmStsTrack*>(fStsTracks->At(cand.fStsInd));
    if (stsTrack == nullptr) continue;
    for (int iM = 0; iM < stsTrack->GetNofMvdHits(); iM++) {
      CbmMvdHit* mvdHit1 = static_cast<CbmMvdHit*>(fMvdHits->At(stsTrack->GetMvdHitIndex(iM)));
      if (mvdHit1 == nullptr) continue;

      int nofMvdHits = fMvdHitMatches->GetEntriesFast();
      for (int iMvd = 0; iMvd < nofMvdHits; iMvd++) {
        const CbmMatch* hitMatch = static_cast<const CbmMatch*>(fMvdHitMatches->At(iMvd));
        if (hitMatch == nullptr) continue;
        if (cand.fStsMcTrackId != hitMatch->GetMatchedLink().GetIndex()) continue;
        CbmMvdHit* mvdHit2 = static_cast<CbmMvdHit*>(fMvdHits->At(iMvd));
        if (mvdHit2 == nullptr || mvdHit2->GetStationNr() != mvdHit1->GetStationNr()) continue;
        double d = LmvmUtils::Distance(mvdHit1->GetX(), mvdHit1->GetY(), mvdHit2->GetX(), mvdHit2->GetY());
        if (mvdHit1->GetStationNr() == 1) { fH.FillH1("hMvdMcDist_1", cand.fMcSrc, d, fW); }
        else if (mvdHit1->GetStationNr() == 2) {
          fH.FillH1("hMvdMcDist_2", cand.fMcSrc, d, fW);
        }
      }
    }
  }
}

bool LmvmTask::IsPrimary(double vertexMag)
{
  if (vertexMag < fZ + 0.1 && vertexMag > fZ - 0.1) return true;
  return false;
}

void LmvmTask::Finish()
{
  CombinatorialPairs();
  TDirectory* oldir = gDirectory;
  TFile* outFile    = FairRootManager::Instance()->GetOutFile();
  if (outFile != nullptr) {
    outFile->cd();
    fH.WriteToFile();
  }
  gDirectory->cd(oldir->GetPath());
}

void LmvmTask::SetEnergyAndPlutoParticle(const string& energy, const string& particle)
{
  this->SetWeight(LmvmSimParam::GetWeight(energy, particle));
  fParticle = particle;
}
