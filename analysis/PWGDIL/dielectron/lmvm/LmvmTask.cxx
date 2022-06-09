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
#include "CbmStsHit.h"
#include "CbmStsKFTrackFitter.h"
#include "CbmStsTrack.h"
#include "CbmTofHit.h"
#include "CbmTofPoint.h"
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
#include "TRandom3.h"
#include "TVector3.h"

#include <sstream>
#include <vector>

#include "LmvmSimParam.h"
#include "LmvmUtils.h"


using namespace std;

ClassImp(LmvmTask);


LmvmTask::LmvmTask() : FairTask("LmvmTask") {}


LmvmTask::~LmvmTask() {}


void LmvmTask::InitHists()
{
  string ax = "Yield";
  string axMinv =
    "Yield";  // "dN/dM_{ee}/N [GeV/c^{2}]^{-1}"; TODO: when in Draw.cxx the scaling to bin width is implemented this can be changed back to "dN/dM..."

  fH.CreateH2("hMomVsAnglePairSignalMc", "#sqrt{P_{e^{#pm}} P_{e^{#mp}}} [GeV/c]", "#theta_{e^{+},e^{-}} [deg]",
              "Counter", 100, 0., 5., 1000, 0., 50.);

  fH.CreateH1("hMotherPdg", {"mc", "acc"}, "Pdg code", "Particles/event", 7000, -3500., 3500.);
  fH.CreateH1("hCandPdg", fH.fAnaStepNames, "Pdg code", "Particles/event", 7001, -3500., 3500.);
  fH.CreateH2("hCandPdgVsMom", fH.fAnaStepNames, "P [GeV/c]", "Particle ID", "Yield/(Event * Bin)", 120, 0., 6., 6, 0.,
              6.);
  fH.CreateH2("hCandElSrc", "Analysis step", "Mother of Electron Candidate", ax, fH.fNofAnaSteps, 0., fH.fNofAnaSteps,
              8, 0., 8.);
  fH.CreateH2("hBgPairPdg", fH.fAnaStepNames, "PDG of Candidate 1", "PDG of Candidate 2", ax, 8, 0., 8., 8, 0., 8.);

  fH.CreateH2("hPmtXY", fH.fSrcNames, "X [cm]", "Y [cm]", "Counter", 110, -110, 110, 200, -200, 200);

  fH.CreateH2("hVertexGammaXZ", fH.fAnaStepNames, "Z [cm]", "X [cm]", ax, 200, -10., 190., 400, -130., 130.);
  fH.CreateH2("hVertexGammaYZ", fH.fAnaStepNames, "Z [cm]", "Y [cm]", ax, 200, -10., 190., 400, -130., 130.);
  fH.CreateH2("hVertexGammaXY", fH.fAnaStepNames, "X [cm]", "Y [cm]", ax, 400, -130., 130., 400, -130., 130.);
  fH.CreateH2("hVertexGammaRZ", fH.fAnaStepNames, "Z [cm]", "#sqrt{X^{2}+Y^{2}} [cm]", ax, 300, -10., 190., 300, 0.,
              150.);

  fH.CreateH1("hNofBgTracks", "Analysis step", "Tracks/event", fH.fNofAnaSteps, 0., fH.fNofAnaSteps);
  fH.CreateH1("hNofSignalTracks", "Analysis step", "Tracks/event", fH.fNofAnaSteps, 0., fH.fNofAnaSteps);
  fH.CreateH2("hBgSrcTracks", "Analysis step", "Candidate Source", ax, fH.fNofAnaSteps, 0., fH.fNofAnaSteps, 8, 0., 8.);

  fH.CreateH1("hNofTopoPairs", {"gamma", "pi0"}, "Pair type", "Pairs/event", 8, 0., 8);
  fH.CreateH1("hNofMismatches", {"all", "rich", "trd", "tof"}, "Analysis step", "Tracks/event", fH.fNofAnaSteps, 0.,
              fH.fNofAnaSteps);

  fH.CreateH1("hNofGhosts", "Analysis step", "Tracks/event", fH.fNofAnaSteps, 0., fH.fNofAnaSteps);

  fH.CreateH2("hSrcBgPairs", "Analysis step", "Pair", ax, fH.fNofAnaSteps, 0., fH.fNofAnaSteps, fH.fNofBgPairSrc, 0.,
              fH.fNofBgPairSrc);
  fH.CreateH2("hSrcBgPairsEpEm", fH.fAnaStepNames, "mother particle e+", "mother particle e-", ax, 4, 0., 4., 4, 0.,
              4.);

  fH.CreateH1("hEventNumber", "", "", 1, 0, 1.);
  fH.CreateH1("hEventNumberMixed", "", "", 1, 0, 1.);

  fH.CreateH1("hAnnRich", fH.fSrcNames, "RICH ANN output", ax, 100, -1.1, 1.1);
  fH.CreateH2("hAnnRichVsMom", fH.fSrcNames, "P [GeV/c]", "RICH ANN output", ax, 100, 0., 6., 100, -1.1, 1.1);
  fH.CreateH1("hAnnTrd", fH.fSrcNames, "Likelihood output", ax, 100, -.1, 1.1);  // TODO: change back to "TRD ANN"
  fH.CreateH2("hTofM2", fH.fSrcNames, "P [GeV/c]", "m^{2} [GeV/c^{2}]^{2}", ax, 150, 0., 6., 500, -0.1, 1.0);
  fH.CreateH1("hChi2Sts", fH.fSrcNames, "#chi^{2}", ax, 200, 0., 20.);
  fH.CreateH1("hChi2PrimVertex", fH.fSrcNames, "#chi^{2}_{prim}", ax, 200, 0., 20.);
  fH.CreateH1("hNofMvdHits", fH.fSrcNames, "Number of hits in MVD", ax, 5, -0.5, 4.5);
  fH.CreateH1("hNofStsHits", fH.fSrcNames, "Number of hits in STS", ax, 9, -0.5, 8.5);
  fH.CreateH2("hTrdLike", {"El", "Pi"}, fH.fSrcNames, "P [GeV/c]", "Likelihood output", ax, 100, 0., 6., 100, 0., 1.);

  fH.CreateH2("hAnnRichVsMomPur", {"El", "Bg"}, "P [GeV/c]", "RICH ANN output", ax, 100, 0., 6., 100, -1.1, 1.1);
  fH.CreateH2("hTrdElLikePur", {"El", "Bg"}, "P [GeV/c]", "Likelihood output", ax, 100, 0., 6., 100, 0., 1.);
  fH.CreateH2("hTofM2Pur", {"El", "Bg"}, "P [GeV/c]", "m^{2} [GeV/c^{2}]^{2}", ax, 150, 0., 6., 500, -0.1, 1.0);

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

  fH.CreateH1("hMinv", fH.fSrcNames, fH.fAnaStepNames, "M_{ee} [GeV/c^{2}]", axMinv, 2500, 0., 2.5);

  // Combinatorial BG histograms
  fH.CreateH1("hMinvCombPM", {"sameEv", "mixedEv"}, fH.fAnaStepNames, "M_{ee} [GeV/c^{2}]", axMinv, 2500, 0., 2.5);
  fH.CreateH1("hMinvCombPP", {"sameEv", "mixedEv"}, fH.fAnaStepNames, "M_{ee} [GeV/c^{2}]", axMinv, 2500, 0., 2.5);
  fH.CreateH1("hMinvCombMM", {"sameEv", "mixedEv"}, fH.fAnaStepNames, "M_{ee} [GeV/c^{2}]", axMinv, 2500, 0., 2.5);

  // Combinatorial BG histograms to distinguish between PLUTO and UrQMD particles
  fH.CreateH1("hMinvCombPM_pluto", {"sameEv", "mixedEv"}, fH.fAnaStepNames, "M_{ee} [GeV/c^{2}]", axMinv, 2500, 0, 2.5);
  fH.CreateH1("hMinvCombPP_pluto", {"sameEv", "mixedEv"}, fH.fAnaStepNames, "M_{ee} [GeV/c^{2}]", axMinv, 2500, 0, 2.5);
  fH.CreateH1("hMinvCombMM_pluto", {"sameEv", "mixedEv"}, fH.fAnaStepNames, "M_{ee} [GeV/c^{2}]", axMinv, 2500, 0, 2.5);
  fH.CreateH1("hMinvCombPM_urqmd", {"sameEv", "mixedEv"}, fH.fAnaStepNames, "M_{ee} [GeV/c^{2}]", axMinv, 2500, 0, 2.5);
  fH.CreateH1("hMinvCombPP_urqmd", {"sameEv", "mixedEv"}, fH.fAnaStepNames, "M_{ee} [GeV/c^{2}]", axMinv, 2500, 0, 2.5);
  fH.CreateH1("hMinvCombMM_urqmd", {"sameEv", "mixedEv"}, fH.fAnaStepNames, "M_{ee} [GeV/c^{2}]", axMinv, 2500, 0, 2.5);

  fH.CreateH1("hMinvBgMatch", {"trueMatch", "trueMatchEl", "trueMatchNotEl", "mismatch"}, fH.fAnaStepNames,
              "M_{ee} [GeV/c^{2}]", ax, 2000, 0., 2.5);
  fH.CreateH1("hMinvBgSource", fH.fBgPairSrcNames, fH.fAnaStepNames, "M_{ee} [GeV/c^{2}]", axMinv, 2500, 0., 2.5);
  fH.CreateH1("hMinvBgSource2_elid", {"gg", "pipi", "pi0pi0", "oo", "gpi", "gpi0", "go", "pipi0", "pio", "pi0o"},
              "M_{ee} [GeV/c^{2}]", axMinv, 2500, 0., 2.5);  // "pi" are misid. charged pions

  fH.CreateH2("hMinvPt", fH.fSrcNames, fH.fAnaStepNames, "M_{ee} [GeV/c^{2}]", "P_{t} [GeV/c]", ax, 100, 0., 2., 25, 0.,
              2.5);

  fH.CreateH1("hMomPairSignal", fH.fAnaStepNames, "P [GeV/c]", ax, 100, 0., 15.);
  fH.CreateH2("hPtYPairSignal", fH.fAnaStepNames, "Rapidity", "P_{t} [GeV/c]", ax, 40, 0., 4., 20, 0., 2.);
  fH.CreateH2("hPtYCandidate", fH.fAnaStepNames, "Rapidity", "P_{t} [GeV/c]", ax, 40, 0., 4., 20, 0., 2.);
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

  fH.CreateH1("hPiMom", {"all", "prim"}, {"mc", "acc", "recSts", "recStsRich", "recStsRichTrd", "recStsRichTrdTof"},
              "P [GeV/c]", ax, 30, 0., 3.);
  fH.CreateH1("hElMom", {"all", "prim"}, {"mc", "acc", "recSts", "recStsRich", "recStsRichTrd", "recStsRichTrdTof"},
              "P [GeV/c]", ax, 30, 0., 3.);
  fH.CreateH1("hProtonMom", {"all", "prim"}, {"mc", "acc", "recSts", "recStsRich", "recStsRichTrd", "recStsRichTrdTof"},
              "P [GeV/c]", ax, 30, 0., 3.);
  fH.CreateH1("hCandMisIdAsEl", {"pi", "pi0", "proton", "kaon"}, "P [GeV/c]", ax, 30, 0., 3.);
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
  DifferenceSignalAndBg();
  SignalAndBgReco();
  FillAccRecVsMomHist();

  fCandsTotal.insert(fCandsTotal.end(), fCands.begin(), fCands.end());
  LOG(info) << "fCandsTotal.size = " << fCandsTotal.size();
  //}
}  // Exec

//TODO: Move this functionality to RichUtil class
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


void LmvmTask::FillMomHists(const CbmMCTrack* mct, const LmvmCand* cand, ELmvmSrc src, ELmvmAnaStep step)
{
  if ((mct != nullptr && cand != nullptr) || (mct == nullptr && cand == nullptr)) {
    LOG(error) << "LmvmTask::FillMomHists: Both mct and cand are [not nullptr] or [nullptr].";
    return;
  }
  bool isMc        = (mct != nullptr);
  string chargeStr = (isMc) ? LmvmUtils::GetChargeStr(mct) : LmvmUtils::GetChargeStr(cand);

  for (const string& suff : {string(""), chargeStr}) {
    if (suff == "0") continue;
    fH.FillH1("hMom" + suff, src, step, (isMc) ? mct->GetP() : cand->fMomentum.Mag(), fW);
    fH.FillH1("hMomPx" + suff, src, step, (isMc) ? mct->GetPx() : cand->fMomentum.X(), fW);
    fH.FillH1("hMomPy" + suff, src, step, (isMc) ? mct->GetPy() : cand->fMomentum.Y(), fW);
    fH.FillH1("hMomPz" + suff, src, step, (isMc) ? mct->GetPz() : cand->fMomentum.Z(), fW);
    fH.FillH1("hPt" + suff, src, step, (isMc) ? mct->GetPt() : cand->fMomentum.Perp(), fW);
    fH.FillH1("hRapidity" + suff, src, step, (isMc) ? mct->GetRapidity() : cand->fRapidity, fW);
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

    FillMomHists(mct, nullptr, src, ELmvmAnaStep::Mc);
    if (isAcc) FillMomHists(mct, nullptr, src, ELmvmAnaStep::Acc);

    if (mct->GetNPoints(ECbmModuleId::kMvd) + mct->GetNPoints(ECbmModuleId::kSts) >= 4)
      fH.FillH1("hMomAcc" + chargeStr + "_sts", src, mom, fW);
    if (fNofHitsInRingMap[i] >= 7) fH.FillH1("hMomAcc" + chargeStr + "_rich", src, mom, fW);
    if (mct->GetNPoints(ECbmModuleId::kTrd) >= 2) fH.FillH1("hMomAcc" + chargeStr + "_trd", src, mom, fW);
    if (mct->GetNPoints(ECbmModuleId::kTof) >= 1) fH.FillH1("hMomAcc" + chargeStr + "_tof", src, mom, fW);

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
  }
}

void LmvmTask::DoMcPair()
{
  int nMcTracks = fMCTracks->GetEntries();
  for (int iMc1 = 0; iMc1 < nMcTracks; iMc1++) {
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
        //fH.FillH1("hAnglePair", srcPair, step, pKin.fAngle, fW);
        if (srcPair == ELmvmSrc::Signal) {
          fH.FillH2("hPtYPairSignal", step, pKin.fRapidity, pKin.fPt, fW);
          fH.FillH1("hMomPairSignal", step, pKin.fMomentumMag, fW);
        }
        // MC and Acc minv only for signal, eta and pi0
        if (srcPair == ELmvmSrc::Signal || srcPair == ELmvmSrc::Pi0 || srcPair == ELmvmSrc::Eta) {
          fH.FillH1("hMinv", srcPair, step, pKin.fMinv, fW);
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
    if (v.Z() < 2.) { fH.FillH2("hPmtXY", src, richHit->GetX(), richHit->GetY(), fW); }
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

void LmvmTask::FillAccRecVsMomHist()
{
  for (const int& pdg : {11, 211, 2212}) {  // TODO: restructure this more efficiently (wihtout these pdg-loops)
    int nofMcTracks = fMCTracks->GetEntriesFast();
    string hName    = (pdg == 11) ? "hElMom" : (pdg == 211) ? "hPiMom" : "hProtonMom";

    for (int i = 0; i < nofMcTracks; i++) {
      CbmMCTrack* mct = static_cast<CbmMCTrack*>(fMCTracks->At(i));
      bool isAccSts   = (mct->GetNPoints(ECbmModuleId::kMvd) + mct->GetNPoints(ECbmModuleId::kSts) >= 4);
      TVector3 vertex;
      mct->GetStartVertex(vertex);

      if (std::abs(mct->GetPdgCode()) == pdg) {
        fH.FillH1(hName + "_all_mc", mct->GetP());
        if (isAccSts) fH.FillH1(hName + "_all_acc", mct->GetP());

        if (vertex.Mag() < 0.1) {
          fH.FillH1(hName + "_prim_mc", mct->GetP());
          if (isAccSts) fH.FillH1(hName + "_prim_acc", mct->GetP());
        }
      }
    }

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
      TVector3 vertex;
      mct->GetStartVertex(vertex);

      if (std::abs(mct->GetPdgCode()) == pdg) {
        fH.FillH1(hName + "_all_recSts", mct->GetP());
        if (isRich) { fH.FillH1(hName + "_all_recStsRich", mct->GetP()); }
        if (isRich && isTrd) { fH.FillH1(hName + "_all_recStsRichTrd", mct->GetP()); }
        if (isRich && isTrd && isTof) { fH.FillH1(hName + "_all_recStsRichTrdTof", mct->GetP()); }

        if (vertex.Mag() < 0.1) {
          fH.FillH1(hName + "_prim_recSts", mct->GetP());
          if (isRich) { fH.FillH1(hName + "_prim_recStsRich", mct->GetP()); }
          if (isRich && isTrd) { fH.FillH1(hName + "_prim_recStsRichTrd", mct->GetP()); }
          if (isRich && isTrd && isTof) { fH.FillH1(hName + "_prim_recStsRichTrdTof", mct->GetP()); }
        }
      }
    }
  }

  // Fill histo with misidentified pions, protons, kaons     // TODO: move this to SignalAndBgReco()?
  for (const auto& cand : fCands) {
    int pdg = cand.fMcPdg;
    if (!(pdg == 211 || pdg == 111 || pdg == 2212 || pdg == 321)) continue;
    if (!cand.fIsElectron) continue;
    cout << "LmvmTask::FillAccRecVsMomHist(): pdg = " << pdg << endl;
    string hName = (pdg == 211)    ? "hCandMisIdAsEl_pi"
                   : (pdg == 111)  ? "hCandMisIdAsEl_pi0"
                   : (pdg == 2212) ? "hCandMisIdAsEl_proton"
                                   : "hCandMisIdAsEl_kaon";
    fH.FillH1(hName, cand.fMomentum.Mag());
  }
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

    cand.fRichInd = gTrack->GetRichRingIndex();
    cand.fTrdInd  = gTrack->GetTrdTrackIndex();
    cand.fTofInd  = gTrack->GetTofHitIndex();

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

    // RICH - TRD - TOF
    cand.fRichInd = gTrack->GetRichRingIndex();
    cand.fTrdInd  = gTrack->GetTrdTrackIndex();
    cand.fTofInd  = gTrack->GetTofHitIndex();
    if (cand.fRichInd < 0 || cand.fTrdInd < 0 || cand.fTofInd < 0) continue;

    CbmRichRing* richRing = static_cast<CbmRichRing*>(fRichRings->At(cand.fRichInd));
    CbmTrdTrack* trdTrack = static_cast<CbmTrdTrack*>(fTrdTracks->At(cand.fTrdInd));
    CbmTofHit* tofHit     = static_cast<CbmTofHit*>(fTofHits->At(cand.fTofInd));
    if (richRing == nullptr || trdTrack == nullptr || tofHit == nullptr) continue;

    LmvmUtils::IsElectron(iGTrack, cand.fMomentum.Mag(), fCuts.fMomentumCut, &cand);
    cand.fTrdLikeEl = trdTrack->GetPidLikeEL();
    cand.fTrdLikePi = trdTrack->GetPidLikePI();

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
      double w         = (cand1.IsMcSignal() && cand2.IsMcSignal()) ? fW : 1.;
      bool isSameEvent = (cand1.fEventNumber == cand2.fEventNumber);

      for (auto step : fH.fAnaSteps) {
        // seperate PLUTO and UrQMD electrons
        if (cand1.IsCutTill(step) && cand2.IsCutTill(step) && std::abs(cand1.fMcPdg) == 11
            && std::abs(cand2.fMcPdg) == 11) {
          // only PLUTO electrons
          if (cand1.IsMcSignal() && cand2.IsMcSignal()) {
            if (isSameEvent) {
              if (cand1.fCharge * cand2.fCharge < 0) fH.FillH1("hMinvCombPM_pluto_sameEv", step, pRec.fMinv, w);
              else if (cand1.fCharge < 0 && cand2.fCharge < 0)
                fH.FillH1("hMinvCombMM_pluto_sameEv", step, pRec.fMinv, w);
              else if (cand1.fCharge > 0 && cand2.fCharge > 0)
                fH.FillH1("hMinvCombPP_pluto_sameEv", step, pRec.fMinv, w);
            }
            else {
              if (cand1.fCharge * cand2.fCharge < 0) fH.FillH1("hMinvCombPM_pluto_mixedEv", step, pRec.fMinv, w);
              else if (cand1.fCharge < 0 && cand2.fCharge < 0)
                fH.FillH1("hMinvCombMM_pluto_mixedEv", step, pRec.fMinv, w);
              else if (cand1.fCharge > 0 && cand2.fCharge > 0)
                fH.FillH1("hMinvCombPP_pluto_mixedEv", step, pRec.fMinv, w);
            }
          }
          // only UrQMD electrons
          else if (!cand1.IsMcSignal() && !cand2.IsMcSignal()) {
            if (isSameEvent) {
              if (cand1.fCharge * cand2.fCharge < 0) fH.FillH1("hMinvCombPM_urqmd_sameEv", step, pRec.fMinv, w);
              else if (cand1.fCharge < 0 && cand2.fCharge < 0)
                fH.FillH1("hMinvCombMM_urqmd_sameEv", step, pRec.fMinv, w);
              else if (cand1.fCharge > 0 && cand2.fCharge > 0)
                fH.FillH1("hMinvCombPP_urqmd_sameEv", step, pRec.fMinv, w);
            }
            else {
              if (cand1.fCharge * cand2.fCharge < 0) fH.FillH1("hMinvCombPM_urqmd_mixedEv", step, pRec.fMinv, w);
              else if (cand1.fCharge < 0 && cand2.fCharge < 0)
                fH.FillH1("hMinvCombMM_urqmd_mixedEv", step, pRec.fMinv, w);
              else if (cand1.fCharge > 0 && cand2.fCharge > 0)
                fH.FillH1("hMinvCombPP_urqmd_mixedEv", step, pRec.fMinv, w);
            }
          }
        }

        // for common CB, don't use PLUTO signals
        if (cand1.IsMcSignal() or cand2.IsMcSignal()) continue;
        if (step == ELmvmAnaStep::Mc || step == ELmvmAnaStep::Acc) continue;
        if (cand1.IsCutTill(step) && cand2.IsCutTill(step)) {
          if (cand1.fCharge * cand2.fCharge < 0) {
            if (isSameEvent) fH.FillH1("hMinvCombPM_sameEv", step, pRec.fMinv, w);
            else
              fH.FillH1("hMinvCombPM_mixedEv", step, pRec.fMinv, w);
          }
          if (cand1.fCharge < 0 && cand2.fCharge < 0) {
            if (isSameEvent) fH.FillH1("hMinvCombMM_sameEv", step, pRec.fMinv, w);
            else
              fH.FillH1("hMinvCombMM_mixedEv", step, pRec.fMinv, w);
          }
          if (cand1.fCharge > 0 && cand2.fCharge > 0) {
            if (isSameEvent) fH.FillH1("hMinvCombPP_sameEv", step, pRec.fMinv, w);
            else
              fH.FillH1("hMinvCombPP_mixedEv", step, pRec.fMinv, w);
          }
        }
      }  // steps
    }    // cand2
  }      // cand1
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
  fH.FillH1("hAnglePair", src, step, parRec.fAngle, fW);

  if (src == ELmvmSrc::Bg) {
    // gamma=0.5, pi0=1.5, pions=2.5,  other=3.5
    double indM = candM.IsMcGamma() ? 0.5 : (candM.IsMcPi0() ? 1.5 : (std::abs(candM.fMcPdg) == 211) ? 2.5 : 3.5);
    double indP = candP.IsMcGamma() ? 0.5 : (candP.IsMcPi0() ? 1.5 : (std::abs(candP.fMcPdg) == 211) ? 2.5 : 3.5);
    fH.FillH2("hSrcBgPairsEpEm", step, indP, indM);

    ELmvmBgPairSrc bgSrc = LmvmUtils::GetBgPairSrc(candP, candM);
    //if (bgSrc != ELmvmBgPairSrc::Undefined) {
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
    //}
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

  fH.FillH1("hMinv", src, step, parRec.fMinv, fW);
  fH.FillH2("hMinvPt", src, step, parRec.fMinv, parMc.fPt, fW);

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

void LmvmTask::SignalAndBgReco()
{
  CheckGammaConvAndPi0();
  CheckTopologyCut(ELmvmTopologyCut::ST, "hStCut");
  CheckTopologyCut(ELmvmTopologyCut::TT, "hTtCut");
  CheckTopologyCut(ELmvmTopologyCut::RT, "hRtCut");
  if (fUseMvd) {
    CheckClosestMvdHit(1, "hMvdCut_1", "hMvdCutQa_1");
    CheckClosestMvdHit(2, "hMvdCut_2", "hMvdCutQa_2");
  }

  for (const auto& cand : fCands) {
    int pdg = 0;
    if (cand.fStsMcTrackId > 0) {
      CbmMCTrack* mcTrack = static_cast<CbmMCTrack*>(fMCTracks->At(cand.fStsMcTrackId));
      if (mcTrack != nullptr) pdg = mcTrack->GetPdgCode();
    }
    for (auto step : fH.fAnaSteps) {
      if (cand.IsCutTill(step)) {
        TrackSource(cand, step, pdg);
        fH.FillH2("hPtYCandidate", step, cand.fRapidity, cand.fMomentum.Perp(), fW);
      }
    }
  }

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

void LmvmTask::DifferenceSignalAndBg()
{
  for (const auto& cand : fCands) {
    fH.FillH1("hChi2PrimVertex", cand.fMcSrc, cand.fChi2Prim, fW);

    if (!cand.fIsChi2Prim) continue;
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
      fH.FillH2("hTofM2Pur_El", cand.fMomentum.Mag(), cand.fMass2, fW);
    }
    else if (!cand.IsMcSignal() && std::abs(cand.fMcPdg != 11)) {
      fH.FillH2("hAnnRichVsMomPur_Bg", cand.fMomentum.Mag(), cand.fRichAnn, fW);
      fH.FillH2("hTrdElLikePur_Bg", cand.fMomentum.Mag(), cand.fTrdLikeEl, fW);
      fH.FillH2("hTofM2Pur_Bg", cand.fMomentum.Mag(), cand.fMass2, fW);
    }

    if (!cand.IsCutTill(ELmvmAnaStep::ElId)) continue;
    //fH.FillSourceH1("hPt", cand.fMcSrc, cand.fMomentum.Perp(), fW);
    //fH.FillSourceH1("hMom", cand.fMcSrc, cand.fMomentum.Mag(), fW);
    fH.FillH1("hChi2Sts", cand.fMcSrc, cand.fChi2sts, fW);

    CbmStsTrack* stsTrack = static_cast<CbmStsTrack*>(fStsTracks->At(cand.fStsInd));
    if (stsTrack == nullptr) continue;
    fH.FillH1("hNofStsHits", cand.fMcSrc, stsTrack->GetNofHits(), fW);

    if (fUseMvd) {
      double mvd1x = 0., mvd1y = 0., mvd2x = 0., mvd2y = 0.;
      for (int iM = 0; iM < stsTrack->GetNofMvdHits(); iM++) {
        CbmMvdHit* mvdHit = static_cast<CbmMvdHit*>(fMvdHits->At(stsTrack->GetMvdHitIndex(iM)));
        if (mvdHit == nullptr) continue;
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
      cout << "NofMvdHits = " << stsTrack->GetNofMvdHits() << endl;  // TODO: delete
      for (int iM = 0; iM < stsTrack->GetNofMvdHits(); iM++) {
        CbmMvdHit* candHit = static_cast<CbmMvdHit*>(fMvdHits->At(stsTrack->GetMvdHitIndex(iM)));
        if (candHit != nullptr && candHit->GetStationNr() == mvdStationNum) {
          candV.emplace_back(candHit->GetX(), candHit->GetY(), iC);
        }
      }
    }
  }

  cout << "mvdV.size() = " << mvdV.size() << endl;    // TODO: delete this line
  cout << "candV.size() = " << candV.size() << endl;  // TODO: delete this line

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

    cout << "cand.fIsMvd1Cut = " << cand.fIsMvd1Cut << endl;  // TODO: delete these cout lines
    cout << "cand.fIsMvd2Cut = " << cand.fIsMvd2Cut << endl;
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
}
