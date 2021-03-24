///////////////////////////////////////////////////////////////////////////
//                Variables Manager class
//
// Authors:
//   * Copyright(c) 1998-2009, ALICE Experiment at CERN, All rights reserved. *
//   Julian Book   <Julian.Book@cern.ch>
/*

  This static class holds all event, track, hit, pair information used in the 
  analysis of reconstructed and MC generated data.

  All variables are defined in the enumerator ValueTypes.
  The corresponding names, labels, units are defined in fgkParticleNames and can be 
  accesses via:

   GetValueName(  Int_t i)
   GetValueLabel( Int_t i)
   GetValueUnit(  Int_t i)

  A certain value can be obtained at runtime via:

    GetValue(ValueTypes val)

  For each variable a TFormula is defined in the array fgFormula that are used 
  to calculate new variables using TMath functions (https://root.cern.ch/doc/master/classTFormula.html)
  that can be used for histograms in PairAnalysisHistos and cuts in PairAnalysisVarCuts

*/
//                                                                       //
///////////////////////////////////////////////////////////////////////////

#include "PairAnalysisVarManager.h"

ClassImp(PairAnalysisVarManager)

  const char* PairAnalysisVarManager::fgkParticleNames[PairAnalysisVarManager::kNMaxValuesMC][3] = {
    {"None", "", ""},
    // Constant information
    {"MEL", "m_{e}", "(GeV/#it{c}^{2})"},
    {"MMU", "m_{#mu}", "(GeV/#it{c}^{2})"},
    {"MPI", "m_{#pi}", "(GeV/#it{c}^{2})"},
    {"MKA", "m_{K}", "(GeV/#it{c}^{2})"},
    {"MPR", "m_{p}", "(GeV/#it{c}^{2})"},
    {"MK0", "m_{K^{0}_{s}}", "(GeV/#it{c}^{2})"},
    {"MLA", "m_{#Lambda}", "(GeV/#it{c}^{2})"},
    {"MPair", "m_{pair}", "(GeV/#it{c}^{2})"},
    {"Ebeam", "#it{E}_{beam}", "(#it{A}GeV)"},
    {"ThermalScaling", "scaling-factor", ""},
    // Hit specific variables
    {"PosX", "x_{hit}", "(cm)"},
    {"PosY", "y_{hit}", "(cm)"},
    {"PosZ", "z_{hit}", "(cm)"},
    {"LinksMC", "N_{links}^{MC}", ""},
    {"TRDLayer", "layer^{TRD}", ""},
    {"TRDPads", "N_{pads}^{TRD}", ""},
    {"TRDCols", "N_{cols}^{TRD}", ""},
    {"TRDRows", "N_{rows}^{TRD}", ""},
    {"Eloss", "d#it{E}/d#it{x} + TR", "(keV#upointcm^{2}/g)"},
    //  {"ElossdEdx",              "d#it{E}/d#it{x}",                                    "(keV#upointcm^{2}/g)"},
    //  {"ElossTR",                "TR",                                                 "(keV#upointcm^{2}/g)"},
    {"NPhotons", "N_{photons}^{RICH}", "(a.u.)"},
    {"PmtId", "ID_{Pmt}^{RICH}", "(a.u.)"},
    {"Beta", "#beta^{TOF}", ""},
    {"TOFPidDeltaBetaEL", "#beta^{TOF}-#beta^{TOF}_{e}", ""},
    {"TOFPidDeltaBetaMU", "#beta^{TOF}-#beta^{TOF}_{#mu}", ""},
    {"TOFPidDeltaBetaPI", "#beta^{TOF}-#beta^{TOF}_{#pi}", ""},
    {"TOFPidDeltaBetaKA", "#beta^{TOF}-#beta^{TOF}_{K}", ""},
    {"TOFPidDeltaBetaPR", "#beta^{TOF}-#beta^{TOF}_{p}", ""},
    {"MassSq", "TOF m^{2}", "(GeV/#it{c}^{2})^{2}"},
    // Particle specific variables
    {"Px", "#it{p}_{x}", "(GeV/#it{c})"},
    {"Py", "#it{p}_{y}", "(GeV/#it{c})"},
    {"Pz", "#it{p}_{z}", "(GeV/#it{c})"},
    {"Pt", "#it{p}_{T}", "(GeV/#it{c})"},
    {"PtSq", "#it{p}_{T}^{2}", "((GeV/#it{c})^{2})"},
    {"P", "#it{p}", "(GeV/#it{c})"},
    {"Xv", "x_{vtx}", "(cm)"},
    {"Yv", "y_{vtx}", "(cm)"},
    {"Zv", "z_{vtx}", "(cm)"},
    {"OneOverPt", "1/^{}#it{p}_{T}", "(GeV/#it{c})^{-1}"},
    {"Phi", "#phi", "(rad.)"},
    {"Theta", "#theta", "(rad.)"},
    {"Eta", "#eta", ""},
    {"Y", "#it{y}", ""},
    {"Ylab", "#it{y}_{lab}", ""},
    {"E", "E", "(GeV)"},
    {"M", "m_{inv}", "(GeV/#it{c}^{2})"},
    {"Charge", "q", "(e)"},
    {"Mt", "#it{m}_{T}", "(GeV/#it{c}^{2})"},
    {"Chi2NDFtoVtx", "#chi^{2}/^{}N_{df}^{track#leftrightarrowvtx}", "(#sigma)"},
    {"ImpactParXY", "#it{dca}_{xy}", "(cm)"},
    {"ImpactParZ", "#it{dca}_{z}", "(cm)"},
    {"InclAngle", "#alpha_{incl}", "(rad.)"},
    // Track specific variables
    // global track
    {"TrackLength", "#it{l}_{track}", "(cm)"},
    {"TrackChi2NDF", "#chi^{2}/^{}N_{df}^{glbl}", ""},
    {"Pin", "#it{p}^{in}", "(GeV/#it{c})"},
    {"Ptin", "#it{p}_{T}^{in}", "(GeV/#it{c})"},
    {"Pout", "#it{p}^{out}", "(GeV/#it{c})"},
    {"Ptout", "#it{p}_{T}^{out}", "(GeV/#it{c})"},
    // trd track information
    {"TRDSignal", "#LTTRD d#it{E}/d#it{x} + TR#GT", "(keV#upointcm^{2}/g)"},
    {"TRDPidWkn", "PID_{e}^{TRD} (Wkn)", ""},
    {"TRDPidANN", "PID_{e}^{TRD} (ANN)", ""},
    {"TRDPidLikeEL", "P(PID_{e}^{TRD})", ""},
    {"TRDPidLikePI", "P(PID_{#pi}^{TRD})", ""},
    {"TRDPidLikeKA", "P(PID_{K}^{TRD})", ""},
    {"TRDPidLikePR", "P(PID_{p}^{TRD})", ""},
    {"TRDPidLikeMU", "P(PID_{#muon}^{TRD})", ""},
    {"TRDHits", "N_{hit}^{TRD}", ""},
    {"TRDChi2NDF", "#chi^{2}/^{}N_{df}^{TRD}", ""},
    {"TRDPin", "#it{p}^{TRDin}", "(GeV/#it{c})"},
    {"TRDPtin", "#it{p}_{T}^{TRDin}", "(GeV/#it{c})"},
    {"TRDPhiin", "#phi^{TRDin}", "(rad.)"},
    {"TRDThetain", "#theta^{TRDin}", "(rad.)"},
    {"TRDPout", "#it{p}^{TRDout}", "(GeV/#it{c})"},
    {"TRDPtout", "#it{p}_{T}^{TRDout}", "(GeV/#it{c})"},
    {"TRDPhiCorr", "#phi_{corr}^{TRD}", ""},
    {"TRDThetaCorr", "#theta_{corr}^{TRD}", ""},
    //  {"TRDTrackLength",         "#it{l}_{track}^{TRD}",                               "(cm)"},
    // sts track information
    {"MVDhasEntr", "station^{MVD}_{first} entrance status", ""},
    {"MVDHits", "N_{hit}^{MVD}", ""},
    {"MVDHitClosest", "dist_{MVD}^{d1+closest-Hit}", "(cm)"},
    {"STSHitClosest", "dist_{STS}^{d1+closest-Hit}", "(cm)"},
    {"MVDHitClosestOpeningAngle", "#phi_{MVD}^{d1+closest}", "(rad.)"},
    {"STSHitClosestOpeningAngle", "#phi_{STS}^{d1+closest}", "(rad.)"},
    {"MVDHitClosestMom", "sqrt{p_{d1} * p_{closest}}", "(GeV/c)"},
    {"STSHitClosestMom", "sqrt{p_{d1} * p_{closest}}", "(GeV/c)"},
    {"MVDFirstHitPosZ", "z_{first hit}^{MVD}", "(cm)"},
    {"MVDFirstExtX", "x_{first station}^{MVD}", "(cm)"},
    {"MVDFirstExtY", "y_{first station}^{MVD}", "(cm)"},
    //  {"ImpactParZ",             "#it{dca}_{z}/#Delta#it{dca}_{z}",                    "#sigma"},
    {"STSHits", "N_{hit}^{STS}", ""},
    {"STSMVDHits", "N_{hit}^{STS}+MVD", ""},
    {"STSChi2NDF", "#chi^{2}/^{}N_{df}^{STS}", ""},
    {"STSPin", "#it{p}^{STSin}", "(GeV/#it{c})"},
    {"STSPtin", "#it{p}_{T}^{STSin}", "(GeV/#it{c})"},
    {"STSPout", "#it{p}^{STSout}", "(GeV/#it{c})"},
    {"STSPtout", "#it{p}_{T}^{STSout}", "(GeV/#it{c})"},
    {"STSXv", "x_{STS}", "(cm)"},
    {"STSYv", "y_{STS}", "(cm)"},
    {"STSZv", "z_{STS}", "(cm)"},
    {"STSFirstHitPosZ", "z_{first hit}^{STS}", "(cm)"},
    //RICH ring specific information
    {"RICHhasProj", "projection status RICH", ""},
    {"RICHPidANN", "PID_{e}^{RICH} (ANN)", ""},
    {"RICHHitsOnRing", "N_{hit on Ring}^{RICH}", ""},
    {"RICHHits", "N_{hit}^{RICH}", ""},
    {"RICHChi2NDF", "#chi^{2}/^{}N_{df}^{RICH}", ""},
    {"RICHRadius", "r_{}^{RICH}", "(cm)"},
    {"RICHAxisA", "a_{}^{RICH}", "(cm)"},
    {"RICHAxisB", "b_{}^{RICH}", "(cm)"},
    {"RICHCenterX", "x_{}^{RICH}", "(cm)"},
    {"RICHCenterY", "y_{}^{RICH}", "(cm)"},
    {"RICHDistance", "d_{ring#leftrightarrowtrack}^{RICH}", "(cm)"},
    {"RICHRadialPos", "pos_{rad.}^{RICH}", "(cm)"},
    {"RICHRadialAngle", "#phi_{rad.}^{RICH}", "(rad.)"},
    {"RICHPhi", "#phi_{rot.}^{RICH}", "(rad.)"},
    //TOF track specific information
    {"TOFHits", "N_{hit}^{TOF}", ""},
    // MUCH track information
    {"MUCHHits", "N_{hit}^{MUCH}", ""},
    {"MUCHHitsPixel", "N_{pixel hit}^{MUCH}", ""},
    //  {"MUCHHitsStraw",          "N_{strip hit}^{MUCH}",                               ""},
    {"MUCHChi2NDF", "#chi^{2}/^{}N_{df}^{MUCH}", ""},
    // technical information
    {"RndmTrack", "P", ""},
    {"PRes", "Res(#it{p})", ""},

    // Pair specific variables
    {"Chi2NDF", "#chi^{2}/^{}ndf", ""},
    {"DecayLength", "l_{decay}", "(cm)"},
    {"R", "d_{xy}(origin,pair)", "(cm)"},
    {"OpeningAngle", "#varphi", "(rad.)"},
    {"CosPointingAngle", "cos(#theta)", "(rad.)"},
    {"ArmAlpha", "#alpha^{arm}", ""},
    {"ArmPt", "#it{p}_{T}^{arm}", "(GeV/#it{c})"},
    {"ThetaHE", "cos(#theta_{HE})", ""},
    {"PhiHE", "#phi_{HE}", "(rad.)"},
    {"ThetaSqHE", "cos^{2}(#theta_{HE})", ""},
    {"Cos2PhiHE", "cos(2#phi_{HE})", ""},
    {"CosTilPhiHE", "cos(#phi_{HE})", ""},
    {"ThetaCS", "cos(#theta_{CS})", ""},
    {"PhiCS", "#phi_{CS}", "(rad.)"},
    {"ThetaSqCS", "cos^{2}(#theta_{CS})", ""},
    {"Cos2PhiCS", "cos(2#phi_{CS})", ""},
    {"CosTilPhiCS", "cos(#phi_{CS})", ""},
    {"PsiPair", "#Psi^{pair}", "(rad.)"},
    {"StsMvdFirstDaughter", "Hits_{STS+MVD}^{d1}", ""},
    {"StsMvdSecondDaughter", "Hits_{STS+MVD}^{d2}", ""},
    {"StsMvdTrdFirstDaughter", "Hits_{STS+MVD+TRD}^{d1}", ""},
    {"StsMvdTrdSecondDaughter", "Hits_{STS+MVD+TRD}^{d2}", ""},
    {"StsMvdRichFirstDaughter", "Hits_{STS+MVD+RICH}^{d1}", ""},
    {"StsMvdRichSecondDaughter", "Hits_{STS+MVD+RICH}^{d2}", ""},
    {"StsFirstDaughter", "Hits_{STS}^{d1}", ""},
    {"StsSecondDaughter", "Hits_{STS}^{d2}", ""},
    {"MvdFirstDaughter", "Hits_{MVD}^{d1}", ""},
    {"MvdSecondDaughter", "Hits_{MVD}^{d2}", ""},
    {"TrdFirstDaughter", "Hits_{TRD}^{d1}", ""},
    {"TrdSecondDaughter", "Hits_{TRD}^{d2}", ""},
    {"RichFirstDaughter", "Hits_{RICH}^{d1}", ""},
    {"RichSecondDaughter", "Hits_{RICH}^{d2}", ""},
    {"StsHitDist", "dist_{STS}^{d1&d2}", "(cm)"},
    {"MvdHitDist", "dist_{MVD}^{d1&d2}", "(cm)"},
    {"PhivPair", "#Phi_{v}^{pair}", "(rad.)"},

    {"LegDistance", "d^{legs}", "(cm)"},
    {"LegDistanceXY", "d^{legs}_{xy}", "(cm)"},
    {"DeltaEta", "#Delta #eta", ""},
    {"DeltaPhi", "#Delta #phi", ""},
    {"LegsP", "#sqrt{#it{p}^{leg1}#upoint#it{p}^{leg2}}", "(GeV/#it{c})"},
    {"Merr", "m_{inv} error", "(GeV/#it{c}^{2})"},
    {"DCA", "#it{dca}", "(cm)"},
    {"PairType", "PairType", ""},
    {"MomAsymDau1", "#it{p}^{leg1}/#it{p}^{pair}", ""},
    {"MomAsymDau2", "#it{p}^{leg2}/#it{p}^{pair}", ""},
    {"EffPair", "A#times#varepsilon", ""},
    {"OneOverEffPair", "(A#times#varepsilon)^{-1}", ""},
    {"OneOverEffPairSq", "(A#times#varepsilon)^{-2}", ""},
    {"RndmPair", "P", ""},
    {"Pairs", "pairs/event", ""},

    // Event specific variables
    {"XvPrim", "x_{prim.vtx}", "(cm)"},
    {"YvPrim", "y_{prim.vtx}", "(cm)"},
    {"ZvPrim", "z_{prim.vtx}", "(cm)"},
    {"VtxChi", "#chi^{2}_{prim.vtx}", ""},
    {"VtxNDF", "ndf", ""},
    {"XRes", "#Delta x_{prim.vtx}", "(cm)"},
    {"YRes", "#Delta y_{prim.vtx}", "(cm)"},
    {"ZRes", "#Delta z_{prim.vtx}", "(cm)"},
    {"MaxPt", "#it{p}_{T}^{lead}", "(GeV/#it{c})"},
    {"RndmRej", "P(rejection)", ""},
    {"NTrk", "N_{trk}", ""},
    {"Tracks", "tracks/per event", ""},
    {"NVtxContrib", "N_{vtx. contrib.}", ""},

    {"Centrality", "centrality_{V0M}", "(%)"},
    {"Nevents", "N_{evt}", ""},
    {"RunNumber", "run", ""},
    {"Ybeam", "#it{y}_{beam}", ""},
    {"MixingBin", "mixing bin", ""},
    {"TotalTRDHits", "N_{tot.hit}^{TRD}", ""},


    // MC information
    // Hit specific variables
    {"PosXMC", "x_{hit}^{MC}", "(cm)"},
    {"PosYMC", "y_{hit}^{MC}", "(cm)"},
    {"PosZMC", "z_{hit}^{MC}", "(cm)"},
    {"ElossMC", "d#it{E}/d#it{x}^{MC}", "(keV#upointcm^{2}/g})"},
    // Particle specific variables
    {"PxMC", "#it{p}_{x}^{MC}", "(GeV/#it{c})"},
    {"PyMC", "#it{p}_{y}^{MC}", "(GeV/#it{c})"},
    {"PzMC", "#it{p}_{z}^{MC}", "(GeV/#it{c})"},
    {"PtMC", "#it{p}_{T}^{MC}", "(GeV/#it{c})"},
    {"PtSqMC", "#it{p}_{T}^{2} (MC)", "((GeV/#it{c})^{2})"},
    {"PMC", "#it{p}^{MC}", "(GeV/#it{c})"},
    {"XvMC", "x_{vtx}^{MC}", "(cm)"},
    {"YvMC", "y_{vtx}^{MC}", "(cm)"},
    {"RvMC", "Sqrt(x_{vtx}^{2}_{MC}+y_{vtx}^{2}_{MC})", "(cm)"},
    {"ZvMC", "z_{vtx}^{MC}", "(cm)"},
    {"PhivMC", "#phi_{vtx}^{MC}", "(rad.)"},
    {"ThetavMC", "#theta_{vtx}^{MC}"
                 "(rad.)"},
    {"OneOverPtMC", "1/^{}#it{p}_{T}^{MC}", "(GeV/#it{c})^{-1}"},
    {"PhiMC", "#phi^{MC}", "(rad.)"},
    {"ThetaMC", "#theta^{MC}", "(rad.)"},
    {"EtaMC", "#eta^{MC}", ""},
    {"YMC", "#it{y}^{MC}", ""},
    {"YlabMC", "#it{y}_{lab}^{MC}", ""},
    {"BetaGammaMC", "#beta#gamma^{MC}", ""},
    {"EMC", "E^{MC}", "(GeV)"},
    {"EMotherMC", "E^{MC}_{mother}", "(GeV)"},
    {"MMC", "m_{inv}^{MC}", "(GeV/#it{c}^{2})"},
    {"ChargeMC", "q^{MC}", "(e)"},
    {"PdgCode", "PDG code", ""},
    {"PdgCodeMother", "mothers PDG code", ""},
    {"PdgCodeGrandMother", "grand mothers PDG code", ""},
    {"GeantId", "Geant process", ""},
    {"Weight", "N#times#it{BR}", ""},


    // Track specific variables
    {"TRDHitsMC", "N_{hit}^{TRD} (MC)", ""},
    {"MVDHitsMC", "N_{hit}^{MVD} (MC)", ""},
    {"STSHitsMC", "N_{hit}^{STS} (MC)", ""},
    {"STSMVDHitsMC", "N_{hit}^{STS} (MC)", ""},
    {"TOFHitsMC", "N_{hit}^{TOF} (MC)", ""},
    {"MUCHHitsMC", "N_{hit}^{MUCH} (MC)", ""},
    {"RICHHitsMC", "N_{hit}^{RICH} (MC)", ""},
    {"TRDMCPoints", "N_{diff.MCtrk}^{TRD}", ""},
    {"RICHMCPoints", "N_{MC,pts}^{RICH}", ""},
    {"TRDTrueHits", "N_{true,hit}^{TRD}", ""},
    {"TRDDistHits", "N_{distorted,hit}^{TRD}", ""},
    {"TRDFakeHits", "N_{fake,hit}^{TRD}", ""},
    {"TRDDistortion", "distortion^{TRD}", ""},
    {"STSTrueHits", "N_{true,hit}^{STS}", ""},
    {"STSDistHits", "N_{distorted,hit}^{STS}", ""},
    {"STSFakeHits", "N_{fake,hit}^{STS}", ""},
    {"TRDisMC", "MC status TRD", ""},
    {"MVDisMC", "MC status MVD", ""},
    {"STSisMC", "MC status STS", ""},
    {"MUCHisMC", "MC status MUCH", ""},
    {"RICHisMC", "MC status RICH", ""},
    {"TOFisMC", "MC status TOF", ""},

    // Pair specific MC variables
    {"OpeningAngleMC", "#varphi^{MC}", "(rad.)"},
    {"CosPointingAngleMC", "cos(#theta)^{MC}", "(rad.)"},
    {"StsMvdFirstDaughterMC", "", ""},
    {"StsMvdSecondDaughterMC", "", ""},
    //  {"PhivPairMC",               "#Phi_{v}^{MC}",                                    "(rad.)"},

    // Event specific MCvariables
    {"NTrkMC", "N_{trk}^{MC}", ""},
    {"XvPrimMC", "x_{prim.vtx}^{MC}", "(cm)"},
    {"YvPrimMC", "y_{prim.vtx}^{MC}", "(cm)"},
    {"ZvPrimMC", "z_{prim.vtx}^{MC}", "(cm)"},
    {"STSMatches", "N_{trk.matches}^{STS}", ""},
    {"TRDMatches", "N_{trk.matches}^{TRD}", ""},
    {"VageMatches", "N_{vage.matches}^{STS}", ""},
    {"TotalTRDHitsMC", "N_{tot.hit}^{TRD} (MC)", ""},
    {"ImpactParam", "#it{b}", "(fm)"},
    {"NPrimMC", "N_{prim}^{MC}", ""},

};

PairAnalysisEvent* PairAnalysisVarManager::fgEvent = 0x0;
CbmKFVertex* PairAnalysisVarManager::fgKFVertex    = 0x0;
CbmVertex* PairAnalysisVarManager::fgVertexMC      = 0x0;
//CbmStsKFTrackFitter* PairAnalysisVarManager::fgKFFitter    = 0x0;
//CbmL1PFFitter*  PairAnalysisVarManager::fgL1Fitter    = 0x0;
TBits* PairAnalysisVarManager::fgFillMap                                       = 0x0;
Int_t PairAnalysisVarManager::fgCurrentRun                                     = -1;
Double_t PairAnalysisVarManager::fgData[PairAnalysisVarManager::kNMaxValuesMC] = {0.};
TFormula* PairAnalysisVarManager::fgFormula[kNMaxValuesMC]                     = {0x0};

//________________________________________________________________
PairAnalysisVarManager::PairAnalysisVarManager() : TNamed("PairAnalysisVarManager", "PairAnalysisVarManager")
{
  //
  // Default constructor
  //
  for (Int_t i = 1; i < kNMaxValuesMC; ++i) {
    fgFormula[i] = 0x0;
  }

  gRandom->SetSeed();
}

//________________________________________________________________
PairAnalysisVarManager::PairAnalysisVarManager(const char* name, const char* title) : TNamed(name, title)
{
  //
  // Named constructor
  //
  for (Int_t i = 1; i < kNMaxValuesMC; ++i) {
    fgFormula[i] = 0x0;
  }

  gRandom->SetSeed();
}

//________________________________________________________________
PairAnalysisVarManager::~PairAnalysisVarManager()
{
  //
  // Default destructor
  //
  for (Int_t i = 1; i < kNMaxValuesMC; ++i) {
    if (fgFormula[i]) delete fgFormula[i];
  }
}

//________________________________________________________________
UInt_t PairAnalysisVarManager::GetValueType(const char* valname)
{
  //
  // Get value type by value name
  //

  TString name(valname);
  for (UInt_t i = 0; i < PairAnalysisVarManager::kNMaxValuesMC; i++) {
    if (!name.CompareTo(fgkParticleNames[i][0])) return i;
  }
  return 0;  // alias none
}

//________________________________________________________________
UInt_t PairAnalysisVarManager::GetValueTypeMC(UInt_t var)
{
  //
  // Get MC value type by standard value name
  //
  // protection against PairAnalysisHistos::kNoAutoFill, kNoProfile, kNoWeights, kNone
  if (var == 997 || var == 998 || var == 999 || var == 1000000000) return var;

  TString valname = GetValueName(var);
  TString name    = valname + "MC";

  //  name+="MC";
  for (UInt_t i = 0; i < PairAnalysisVarManager::kNMaxValuesMC; i++) {
    if (!name.CompareTo(fgkParticleNames[i][0])) return i;
  }
  return GetValueType(valname);
}

/*
Double_t PairAnalysisVarManager::GetChiToVertex(const CbmStsTrack *track, CbmVertex *vertex)
{
  //
  // Get  vertex chi/ndf between ststrack and vertex
  //

  // KFFitter
  return fgKFFitter->GetChiToVertex(track,vertex);

  // L1PFFitter
  vector<CbmStsTrack> stsTracks;
  stsTracks.resize(1);
  stsTracks[0] = *track;
  vector<L1FieldRegion> vField;
  vector<float> chiPrim;
  fgL1Fitter->GetChiToVertex(stsTracks, vField, chiPrim, fgKFVertex, 3.e+6);
  printf("L1fitter: %f\n", chiPrim[0]);
  return chiPrim[0];

}
*/
