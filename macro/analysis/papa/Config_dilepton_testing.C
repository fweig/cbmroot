/* Copyright (C) 2019 Institut für Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Julian Book, Etienne Bechtel [committer] */

/// \file Config_jbook_AA.C
// \brief A template task configuration macro with example and explanations
///
///         PairAnalysis PAckage (PAPA) -- written by Julian Book
///
///
/// It configures a multi-task with 5 configurations for the study of the TRD performance
/// in the low and intermediate masss regions.
///
/// UPDATES, NOTES:
/// -
///
///
/// \author Julian Book <jbook@ikf.uni-frankfurt.de>, Goethe-University Frankfurt
/// \date Aug 13, 2015
///

PairAnalysis* Config_Analysis(Int_t cutDefinition);

/// names of the tasks
TString names = ("ACC;REC;FULL;");
enum
{
  kACCcfg,      /// for PID setup
  kRECcfg,      /// for PID setup
  kFullPIDcfg,  /// for PID setup
};

////// OUTPUT
void InitHistograms(PairAnalysis* papa, Int_t cutDefinition);
void AddEventHistograms(PairAnalysis* papa, Int_t cutDefinition);
void AddTrackHistograms(PairAnalysis* papa, Int_t cutDefinition);
void AddTrackHistogramsReconstruction(PairAnalysisHistos* histos, Int_t cutDefinition);
void AddPairHistograms(PairAnalysis* papa, Int_t cutDefinition);
/// QA
void InitHF(PairAnalysis* papa, Int_t cutDefinition);
////// CUTS
void SetupEventCuts(AnalysisTaskMultiPairAnalysis* task);
void SetupTrackCuts(PairAnalysis* papa, Int_t cutDefinition);
void SetupPairCuts(PairAnalysis* papa, Int_t cutDefinition);
void SetupTrackCutsMC(PairAnalysis* papa, Int_t cutDefinition);
////// SETTINGS
void ConfigBgrd(PairAnalysis* papa, Int_t cutDefinition);
void AddMCSignals(PairAnalysis* papa, Int_t cutDefinition);
////// MISC
TObjArray* arrNames = names.Tokenize(";");
const Int_t nPapa   = arrNames->GetEntries();

//______________________________________________________________________________________
AnalysisTaskMultiPairAnalysis* Config_dilepton_testing()
{
  ///
  /// creation of one multi task
  ///
  AnalysisTaskMultiPairAnalysis* task = new AnalysisTaskMultiPairAnalysis("testing");
  //  task->SetBeamEnergy(8.); //TODO: get internally from FairBaseParSet::GetBeamMom()

  /// apply event cuts
  SetupEventCuts(task);

  /// add PAPA analysis with different cuts to the task
  for (Int_t i = 0; i < nPapa; ++i) {

    //activate configs
    switch (i) {
        //    case kACCcfg:          continue;
        //    case kRECcfg:          continue;
        //    case kFullPIDcfg:      continue;
      default:
        //      Info(" Config_jbook_AA",Form("Configure PAPa-subtask %s",((TObjString*)arrNames->At(i))->GetName()));
        break;
    }

    /// load configuration
    PairAnalysis* papa = Config_Analysis(i);
    if (!papa) continue;

    /// add PAPA to the task
    task->AddPairAnalysis(papa);
    std::cout << "-I- : Added subtask " << papa->GetName() << std::endl;
  }
  std::cout << "-I- : Added task " << task->GetName() << std::endl;
  return task;
}

//______________________________________________________________________________________
PairAnalysis* Config_Analysis(Int_t cutDefinition)
{
  ///
  /// Setup the instance of PairAnalysis
  ///

  /// task name
  TString name = arrNames->At(cutDefinition)->GetName();
  printf(" Adding config %s \n", name.Data());

  /// init managing object PairAnalysis with unique name,title
  PairAnalysis* papa = new PairAnalysis(Form("%s", name.Data()), Form("%s", name.Data()));
  papa->SetHasMC(kTRUE);  /// TODO: set automatically
  /// ~ type of analysis (leptonic, hadronic or semi-leptonic 2-particle decays are supported)
  papa->SetLegPdg(-11, +11);            /// default: dielectron
  papa->SetRefitWithMassAssump(kTRUE);  /// refit under legpdg-mass assumptions
  //papa->SetUseKF(kTRUE);               /// use KF particle for secondary vertexing

  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv CUTS vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
  SetupTrackCuts(papa, cutDefinition);
  SetupPairCuts(papa, cutDefinition);
  SetupTrackCutsMC(papa, cutDefinition);


  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv MISC vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
  /// internal train usage
  // if(cutDefinition==kStscfg)    papa->SetDontClearArrays(kTRUE);
  // else                          papa->SetEventProcess(kFALSE);

  /// pair settings
  //  if(/*cutDefinition!=kStscfg && */cutDefinition<kRichTRDcfg)
  //  papa->SetNoPairing();   /// single particle analysis
  //  if(cutDefinition<kRichTRDcfg) papa->SetNoPairing();   /// single particle analysis
  //  if(cutDefinition==kStscfg) papa->SetNoPairing();   /// single particle analysis

  /// prefilter settings
  papa->SetPairPreFilterLegCutType(PairAnalysis::kAnyLeg);
  //papa->SetPreFilterAllSigns();
  papa->SetPreFilterUnlikeOnly();

  /// Monte Carlo Signals
  AddMCSignals(papa, cutDefinition);

  /// background estimators
  //  ConfigBgrd(papa,cutDefinition);

  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv OUTPUT vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
  /// histogram setup (single TH1,2,3, TProfile,2,3, THn)
  InitHistograms(papa, cutDefinition);
  /// histogram grid setup (a grid of TH1,2,3, TProfile,2,3, THn)
  ///  InitHF(papa,cutDefinition);

  /// some simple cut QA for events, tracks, OS-pairs
  papa->SetCutQA();

  return papa;
}

//______________________________________________________________________________________
void SetupEventCuts(AnalysisTaskMultiPairAnalysis* task)
{
  ///
  /// Setup the event cuts
  ///

  /// Cut using (a formula based on) variables which are defined and described in PairAnalysisVarManager.h
  /// Cuts can be added by either excluding or including criteria (see PairAnalysisVarCuts.h)
  /// formula function strings are listed here: http://root.cern.ch/root/html/TFormula.html#TFormula:Analyze
  PairAnalysisVarCuts* eventCuts = new PairAnalysisVarCuts("vertex", "vertex");
  //  eventCuts->AddCut(PairAnalysisVarManager::kNVtxContrib, 0.0, 800.);  /// inclusion cut
  //  eventCuts->AddCut(PairAnalysisVarManager::kImpactParam, 0.0, 13.5);
  eventCuts->AddCut("VtxChi/VtxNDF", 6., 1.e3,
                    kTRUE);  /// 'kTRUE': exclusion cut based on formula
  //  eventCuts->AddCut("abs(ZvPrim)", 0., 10.);                           /// example of TMath in formula-cuts

  /// add cuts to the global event filter
  task->SetEventFilter(eventCuts);
  //  papa->GetEventFilter().AddCuts(eventCuts); /// or to each config

  /// for debug purpose (recommended)
  eventCuts->Print();
}

//______________________________________________________________________________________
void SetupTrackCuts(PairAnalysis* papa, Int_t cutDefinition)
{
  ///
  /// Setup the track cuts
  ///
  Bool_t hasMC = papa->GetHasMC();


  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv TRACK QUALITY CUTS vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
  /// setup a cut group, in which cut logic can be set (kCompAND, kCompOR)
  //  PairAnalysisCutGroup  *grpQualCuts = new PairAnalysisCutGroup("quality","quality",PairAnalysisCutGroup::kCompAND);

  /// mcPID cuts to reject heavy particle cocktail contributions
  PairAnalysisVarCuts* mcPDGcuts = new PairAnalysisVarCuts("mcPDG", "mcPDG");
  mcPDGcuts->SetCutType(PairAnalysisVarCuts::kAll);  /// wheter 'kAll' or 'kAny' cut has to be fullfilled
  mcPDGcuts->AddCut(PairAnalysisVarManager::kPdgCode, 1000010020. - 0.5, 1000010020. + 0.5, kTRUE);
  mcPDGcuts->AddCut(PairAnalysisVarManager::kPdgCode, 1000010030. - 0.5, 1000010030. + 0.5, kTRUE);
  mcPDGcuts->AddCut(PairAnalysisVarManager::kPdgCode, 1000020030. - 0.5, 1000020030. + 0.5, kTRUE);
  mcPDGcuts->AddCut(PairAnalysisVarManager::kPdgCode, 1000020040. - 0.5, 1000020040. + 0.5, kTRUE);
  mcPDGcuts->AddCut(PairAnalysisVarManager::kPdgCodeMother, 1000010020. - 0.5, 1000010020. + 0.5, kTRUE);
  mcPDGcuts->AddCut(PairAnalysisVarManager::kPdgCodeMother, 1000010030. - 0.5, 1000010030. + 0.5, kTRUE);
  mcPDGcuts->AddCut(PairAnalysisVarManager::kPdgCodeMother, 1000020030. - 0.5, 1000020030. + 0.5, kTRUE);
  mcPDGcuts->AddCut(PairAnalysisVarManager::kPdgCodeMother, 1000020040. - 0.5, 1000020040. + 0.5, kTRUE);

  /// prefilter track cuts
  PairAnalysisVarCuts* preCuts = new PairAnalysisVarCuts("preCuts", "preCuts");
  preCuts->SetCutType(PairAnalysisVarCuts::kAll);  /// wheter 'kAll' or 'kAny' cut has to be fullfilled
  // preCuts->AddCut(PairAnalysisVarManager::kMvdFirstHitPosZ, 0.,   7.5);   /// a hit in 1st MVD layer
  //  preCuts->AddCut("MvdHits+StsHits",                        3.,   15.);
  preCuts->AddCut(PairAnalysisVarManager::kStsHits, 3., 15.);
  //  preCuts->AddCut(PairAnalysisVarManager::kChi2NDFtoVtx,    0.,    3.);      /// tracks pointing to the primary vertex

  PairAnalysisVarCuts* preChiCuts = new PairAnalysisVarCuts("preChiCuts", "preChiCuts");
  preCuts->SetCutType(PairAnalysisVarCuts::kAll);  /// wheter 'kAll' or 'kAny' cut has to be fullfilled
  preCuts->AddCut(PairAnalysisVarManager::kChi2NDFtoVtx, 0.,
                  3.);  /// tracks pointing to the primary vertex

  /// acceptance cuts (applied after pair pre filter)
  PairAnalysisVarCuts* accCuts = new PairAnalysisVarCuts("accRec", "accRec");
  accCuts->SetCutType(PairAnalysisVarCuts::kAll);  /// wheter 'kAll' or 'kAny' cut has to be fullfilled
  //accCuts->AddCut(PairAnalysisVarManager::kMvdHitsMC,      0.,   0.5, kTRUE); // MVD MC acceptance
  accCuts->AddCut(PairAnalysisVarManager::kStsHitsMC, 1., 99.);  // STS MC acceptance
  accCuts->AddCut(PairAnalysisVarManager::kTrdHitsMC, 1., 99.);  // TRD MC acceptance
  //  accCuts->AddCut(PairAnalysisVarManager::kRichhasProj,    0.,   0.5, kTRUE); // RICH MC acceptance
  //  accCuts->AddCut(PairAnalysisVarManager::kPt,             0.05, 1e30);        // NOTE: was 0.2 GeV/c


  PairAnalysisVarCuts* pT = new PairAnalysisVarCuts("pT", "pT");
  pT->AddCut(PairAnalysisVarManager::kPt, 0.05, 1e30);  // NOTE: was 0.2 GeV/c

  /// standard reconstruction cuts
  PairAnalysisVarCuts* recSTS = new PairAnalysisVarCuts("recSTS", "recSTS");
  recSTS->SetCutType(PairAnalysisVarCuts::kAll);
  //  recSTS->AddCut(PairAnalysisVarManager::kChi2NDFtoVtx,    0.,    3.);      /// tracks pointing to the primary vertex
  //recSTS->AddCut(PairAnalysisVarManager::kMvdFirstHitPosZ, 0.,   7.5);   /// a hit in first MVD layer
  recSTS->AddCut(PairAnalysisVarManager::kStsHits, 3., 15.);
  //  recSTS->AddCut("MvdHits+StsHits",         4.,   15.);        /// min+max requieremnt for hits

  /// RICH acceptance cuts
  PairAnalysisVarCuts* accRICH = new PairAnalysisVarCuts("accRICH", "accRICH");
  accRICH->SetCutType(PairAnalysisVarCuts::kAll);
  accRICH->AddCut(PairAnalysisVarManager::kRichhasProj, 0., 0.5, kTRUE);

  /// RICH reconstruction cuts
  PairAnalysisVarCuts* recRICH = new PairAnalysisVarCuts("recRICH", "recRICH");
  recRICH->SetCutType(PairAnalysisVarCuts::kAll);
  recRICH->AddCut(PairAnalysisVarManager::kRichHits, 6.,
                  100.);  /// min+max inclusion for hits, 13=eff95%
  // valid ring parameters
  recRICH->AddCut(PairAnalysisVarManager::kRichAxisA, 0., 10.);
  recRICH->AddCut(PairAnalysisVarManager::kRichAxisB, 0., 10.);
  recRICH->AddCut(PairAnalysisVarManager::kRichDistance, 0., 999.);
  recRICH->AddCut(PairAnalysisVarManager::kRichRadialPos, 0., 999.);
  recRICH->AddCut(PairAnalysisVarManager::kRichRadialAngle, -TMath::TwoPi(), TMath::TwoPi());
  recRICH->AddCut(PairAnalysisVarManager::kRichPhi, -TMath::TwoPi(), TMath::TwoPi());
  // ellipse fit not working (chi2/ndf is nan)
  recRICH->AddCut(PairAnalysisVarManager::kRichChi2NDF, 0., 100.);


  /// TRD reconstruction cuts
  PairAnalysisVarCuts* recTRD = new PairAnalysisVarCuts("recTRD", "recTRD");
  recTRD->SetCutType(PairAnalysisVarCuts::kAll);
  recTRD->AddCut(PairAnalysisVarManager::kTrdHits, 3.,
                 10.);  /// min+max requieremnt for hits
  //  recTRD->AddCut(PairAnalysisVarManager::kElossNew,         27.,   35.);         /// min+max requieremnt for hits

  /// TOF reconstruction cuts
  PairAnalysisVarCuts* recTOF = new PairAnalysisVarCuts("recTOF", "recTOF");
  recTOF->SetCutType(PairAnalysisVarCuts::kAll);
  recTOF->AddCut(PairAnalysisVarManager::kTofHits, 1.,
                 10.0);  /// min+max requieremnt for hits
  //  recTOF->AddCut(PairAnalysisVarManager::kP,               0.,    0.8);         /// momentum selection

  /// RICH+TRD(+TOF) reconstruction 'if available' cuts
  PairAnalysisCutGroup* recDET = new PairAnalysisCutGroup("recDET", "recDET", PairAnalysisCutGroup::kCompOR);
  recDET->AddCut(accRICH);  //  recDET->AddCut(recRICH);
  recDET->AddCut(recTRD);
  //  if (cutDefinition==kRIC)   recDET->AddCut(recTOF);
  /// alternative
  PairAnalysisVarCuts* accPID = new PairAnalysisVarCuts("accPID", "accPID");
  //  accPID->SetCutType(PairAnalysisVarCuts::kAny);
  accPID->SetCutType(PairAnalysisVarCuts::kAll);
  accPID->AddCut(PairAnalysisVarManager::kRichhasProj, 0., 0.5, kTRUE);
  accPID->AddCut(PairAnalysisVarManager::kTrdHits, 2., 10.);
  // if (cutDefinition==kRichTRDTOFcfg)
  // accPID->AddCut(PairAnalysisVarManager::kTofHits,         1.,   10.);


  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv TRACK PID CUTS vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
  PairAnalysisCutGroup* grpPIDCut = new PairAnalysisCutGroup("PID", "PID", PairAnalysisCutGroup::kCompOR);

  /// TRD pid cuts - 2-dimensional

  TGraph* grTRD = NULL;           // lower cut limt
  TGraph* grMax = new TGraph(2);  // upper cut limit
  grMax->SetPoint(0, 0., 999.);
  grMax->SetPoint(1, 999., 999.);
  grMax->GetListOfFunctions()->Add(PairAnalysisHelper::GetFormula("varf", "P"));

  // ANN default min bias 2-4 hits - 90% electron efficiency - 250mum target
  Double_t x[68] = {0.100, 0.457, 0.537, 0.603, 0.661, 0.708, 0.759, 0.794, 0.832, 0.871, 0.912, 0.955,  1.000, 1.023,
                    1.047, 1.072, 1.096, 1.122, 1.148, 1.175, 1.202, 1.230, 1.259, 1.288, 1.318, 1.349,  1.380, 1.413,
                    1.445, 1.479, 1.514, 1.549, 1.585, 1.622, 1.660, 1.698, 1.738, 1.778, 1.820, 1.862,  1.905, 1.950,
                    1.995, 2.042, 2.089, 2.138, 2.188, 2.239, 2.291, 2.344, 2.399, 2.455, 2.512, 2.570,  2.630, 2.754,
                    2.884, 3.020, 3.162, 3.311, 3.467, 3.715, 3.981, 4.365, 4.898, 5.754, 7.413, 999.000};
  Double_t y[68] = {-0.940, -0.940, -0.920, -0.920, -0.900, -0.900, -0.880, -0.880, -0.880, -0.860, -0.860, -0.840,
                    -0.820, -0.800, -0.780, -0.760, -0.760, -0.740, -0.720, -0.720, -0.700, -0.700, -0.700, -0.680,
                    -0.680, -0.680, -0.680, -0.660, -0.660, -0.640, -0.600, -0.560, -0.540, -0.520, -0.520, -0.500,
                    -0.480, -0.480, -0.480, -0.480, -0.460, -0.440, -0.440, -0.380, -0.340, -0.320, -0.320, -0.300,
                    -0.320, -0.280, -0.280, -0.300, -0.280, -0.260, -0.280, -0.280, -0.260, -0.260, -0.220, -0.220,
                    -0.240, -0.240, -0.240, -0.220, -0.220, -0.300, -0.420, -0.420};

  /// ANN 2D TRD pid cut - 4layer geometry - 3-4 rec. TRD hits - 80% ele. eff.
  Double_t x2[107] = {0.100, 0.372, 0.417, 0.447, 0.479, 0.501, 0.525, 0.550, 0.575,  0.603, 0.631, 0.646, 0.661, 0.676,
                      0.692, 0.708, 0.724, 0.741, 0.759, 0.776, 0.794, 0.813, 0.832,  0.851, 0.871, 0.891, 0.912, 0.933,
                      0.955, 0.977, 1.000, 1.023, 1.047, 1.072, 1.096, 1.122, 1.148,  1.175, 1.202, 1.230, 1.259, 1.288,
                      1.318, 1.349, 1.380, 1.413, 1.445, 1.479, 1.514, 1.549, 1.585,  1.622, 1.660, 1.698, 1.738, 1.778,
                      1.820, 1.862, 1.905, 1.950, 1.995, 2.042, 2.089, 2.138, 2.188,  2.239, 2.291, 2.344, 2.399, 2.455,
                      2.512, 2.570, 2.630, 2.692, 2.754, 2.818, 2.884, 2.951, 3.020,  3.090, 3.162, 3.236, 3.311, 3.388,
                      3.467, 3.548, 3.631, 3.715, 3.802, 3.890, 3.981, 4.074, 4.169,  4.266, 4.365, 4.467, 4.677, 4.898,
                      5.129, 5.370, 5.623, 5.888, 6.310, 6.761, 7.413, 8.318, 999.000};
  Double_t y2[107] = {-0.780, -0.820, -0.840, -0.860, -0.860, -0.840, -0.800, -0.780, -0.760, -0.720, -0.720, -0.700,
                      -0.700, -0.720, -0.680, -0.660, -0.660, -0.660, -0.640, -0.660, -0.640, -0.620, -0.620, -0.620,
                      -0.600, -0.580, -0.580, -0.580, -0.580, -0.560, -0.540, -0.420, -0.380, -0.320, -0.280, -0.260,
                      -0.240, -0.200, -0.180, -0.160, -0.140, -0.140, -0.120, -0.100, -0.100, -0.080, -0.060, -0.040,
                      0.040,  0.120,  0.160,  0.180,  0.200,  0.220,  0.240,  0.260,  0.260,  0.280,  0.300,  0.300,
                      0.360,  0.400,  0.440,  0.460,  0.480,  0.500,  0.500,  0.520,  0.540,  0.540,  0.540,  0.560,
                      0.560,  0.560,  0.580,  0.580,  0.580,  0.580,  0.620,  0.620,  0.620,  0.620,  0.640,  0.640,
                      0.640,  0.640,  0.640,  0.660,  0.660,  0.660,  0.660,  0.680,  0.660,  0.680,  0.680,  0.700,
                      0.700,  0.720,  0.720,  0.740,  0.740,  0.740,  0.760,  0.780,  0.780,  0.800,  0.800};

  // Likelihood 4 Layer - 80% EL Eff
  Double_t x3[84] = {0.01,     0.231755, 0.242677, 0.254114, 0.26609,  0.278631, 0.291762, 0.305512, 0.319911, 0.334988,
                     0.350775, 0.367307, 0.384617, 0.402744, 0.421724, 0.4416,   0.462412, 0.484204, 0.507024, 0.53092,
                     0.555941, 0.582142, 0.609577, 0.638306, 0.668388, 0.699888, 0.732873, 0.767412, 0.803579, 0.841451,
                     0.881107, 0.922633, 0.966115, 1.01165,  1.05932,  1.10925,  1.16153,  1.21627,  1.27359,  1.33361,
                     1.39646,  1.46227,  1.53119,  1.60335,  1.67892,  1.75804,  1.84089,  1.92765,  2.0185,   2.11363,
                     2.21324,  2.31755,  2.42677,  2.54114,  2.6609,   2.78631,  2.91762,  3.05512,  3.19911,  3.34988,
                     3.50775,  3.67307,  3.84617,  4.02744,  4.21724,  4.416,    4.62412,  4.84204,  5.07024,  5.3092,
                     5.55941,  5.82142,  6.09577,  6.38306,  6.68388,  6.99888,  7.32873,  7.67412,  8.03579,  8.41451,
                     8.81107,  9.22633,  9.66115,  999.};
  Double_t y3[84] = {0.255, 0.255, 0.075, 0.085, 0.115, 0.095, 0.085, 0.085, 0.065, 0.065, 0.065, 0.065, 0.065, 0.055,
                     0.055, 0.055, 0.055, 0.055, 0.055, 0.075, 0.085, 0.085, 0.095, 0.095, 0.105, 0.105, 0.105, 0.115,
                     0.115, 0.125, 0.125, 0.135, 0.135, 0.155, 0.205, 0.235, 0.265, 0.275, 0.295, 0.305, 0.315, 0.335,
                     0.385, 0.435, 0.475, 0.485, 0.505, 0.525, 0.555, 0.605, 0.625, 0.645, 0.655, 0.665, 0.685, 0.685,
                     0.695, 0.705, 0.715, 0.725, 0.725, 0.735, 0.735, 0.755, 0.765, 0.765, 0.775, 0.775, 0.795, 0.785,
                     0.795, 0.795, 0.805, 0.805, 0.805, 0.815, 0.815, 0.825, 0.835, 0.825, 0.825, 0.845, 0.835, 0.835};

  Double_t x5[113] = {
    0.767412, 0.785288, 0.803579, 0.822297, 0.841451, 0.861051, 0.881107, 0.901631, 0.922633, 0.944123, 0.966115,
    0.988619, 1.01165,  1.03521,  1.05932,  1.084,    1.10925,  1.13509,  1.16153,  1.18858,  1.21627,  1.2446,
    1.27359,  1.30325,  1.33361,  1.36467,  1.39646,  1.42899,  1.46227,  1.49633,  1.53119,  1.56685,  1.60335,
    1.6407,   1.67892,  1.71802,  1.75804,  1.79899,  1.84089,  1.88377,  1.92765,  1.97255,  2.0185,   2.06552,
    2.11363,  2.16286,  2.21324,  2.26479,  2.31755,  2.37153,  2.42677,  2.4833,   2.54114,  2.60033,  2.6609,
    2.72288,  2.78631,  2.85121,  2.91762,  2.98558,  3.05512,  3.12629,  3.19911,  3.27362,  3.34988,  3.42791,
    3.50775,  3.58946,  3.67307,  3.75862,  3.84617,  3.93576,  4.02744,  4.12125,  4.21724,  4.31548,  4.416,
    4.51886,  4.62412,  4.73183,  4.84204,  4.95483,  5.07024,  5.18834,  5.3092,   5.43286,  5.55941,  5.68891,
    5.82142,  5.95702,  6.09577,  6.23776,  6.38306,  6.53174,  6.68388,  6.83957,  6.99888,  7.16191,  7.32873,
    7.49944,  7.67412,  7.85288,  8.03579,  8.22297,  8.41451,  8.61051,  8.81107,  9.01631,  9.22633,  9.44123,
    9.66115,  9.88619,  999.};
  Double_t y5[113] = {0.185, 0.185, 0.185, 0.185, 0.185, 0.195, 0.195, 0.195, 0.195, 0.195, 0.195, 0.205, 0.235,
                      0.265, 0.275, 0.295, 0.295, 0.305, 0.315, 0.325, 0.325, 0.335, 0.345, 0.345, 0.355, 0.355,
                      0.365, 0.375, 0.375, 0.395, 0.445, 0.475, 0.485, 0.505, 0.515, 0.535, 0.545, 0.545, 0.565,
                      0.565, 0.575, 0.585, 0.605, 0.635, 0.655, 0.655, 0.675, 0.685, 0.685, 0.695, 0.705, 0.705,
                      0.705, 0.715, 0.705, 0.705, 0.715, 0.725, 0.715, 0.725, 0.725, 0.745, 0.755, 0.755, 0.755,
                      0.765, 0.755, 0.755, 0.765, 0.755, 0.755, 0.765, 0.775, 0.775, 0.785, 0.785, 0.785, 0.785,
                      0.785, 0.785, 0.785, 0.755, 0.795, 0.775, 0.775, 0.795, 0.805, 0.795, 0.785, 0.775, 0.805,
                      0.795, 0.795, 0.805, 0.795, 0.795, 0.795, 0.795, 0.805, 0.805, 0.825, 0.815, 0.815, 0.815,
                      0.815, 0.825, 0.795, 0.775, 0.805, 0.845, 0.775, 0.845, 0.845};

  // Likelihood 3 Layer - 80% EL Eff
  Double_t x4[80] = {0.1,      0.3,      0.5,      0.767412, 0.785288, 0.803579, 0.822297, 0.841451, 0.861051, 0.881107,
                     0.901631, 0.922633, 0.944123, 0.966115, 0.988619, 1.01165,  1.03521,  1.05932,  1.084,    1.10925,
                     1.13509,  1.16153,  1.18858,  1.21627,  1.2446,   1.27359,  1.30325,  1.33361,  1.36467,  1.39646,
                     1.42899,  1.46227,  1.49633,  1.53119,  1.56685,  1.60335,  1.6407,   1.67892,  1.71802,  1.75804,
                     1.79899,  1.84089,  1.88377,  1.92765,  1.97255,  2.0185,   2.06552,  2.11363,  2.16286,  2.21324,
                     2.26479,  2.42677,  2.60033,  2.6609,   2.72288,  2.78631,  2.85121,  2.98558,  3.1,      3.19911,
                     3.3,      3.4,      3.50775,  3.67307,  3.84617,  3.93576,  4.31548,  5.07024,  5.18834,  5.3092,
                     5.43286,  5.55941,  5.82142,  5.95702,  6.09577,  6.23776,  7.32873,  8.03579,  9.22633,  999.};
  Double_t y4[80] = {0.165, 0.165, 0.165, 0.165, 0.165, 0.165, 0.155, 0.165, 0.175, 0.175, 0.165, 0.165, 0.165, 0.155,
                     0.165, 0.185, 0.205, 0.195, 0.205, 0.215, 0.235, 0.235, 0.225, 0.235, 0.245, 0.235, 0.225, 0.215,
                     0.225, 0.235, 0.245, 0.235, 0.245, 0.265, 0.265, 0.275, 0.265, 0.285, 0.315, 0.315, 0.325, 0.325,
                     0.325, 0.315, 0.315, 0.315, 0.335, 0.335, 0.355, 0.365, 0.365, 0.385, 0.385, 0.385, 0.385, 0.385,
                     0.385, 0.385, 0.385, 0.395, 0.395, 0.395, 0.395, 0.395, 0.395, 0.405, 0.415, 0.415, 0.415, 0.425,
                     0.425, 0.425, 0.455, 0.465, 0.465, 0.465, 0.465, 0.465, 0.465, 0.465};

  // For further testing
  // Double_t x5[53] ={0.1,0.767412,0.822297,0.841451,0.861051,0.881107,0.901631,0.922633,0.966115,0.988619,1.01165,1.03521,1.05932,1.084,1.10925,1.13509,1.16153,1.18858,1.33361,1.36467,1.39646,1.42899,1.46227,1.53119,1.6407,1.71802,1.75804,1.79899,1.84089,1.88377,1.92765,1.97255,2.06552,2.11363,2.42677,2.54114,2.91762,2.98558,3.05512,3.12629,3.50775,3.75862,4.84204,5.07024,5.3092,5.68891,5.82142,6.38306,7.32873,7.85288,8.03579,8.41451,999.};
  // Double_t y5[53] ={0.295,0.295,0.295,0.315,0.305,0.315,0.345,0.345,0.325,0.335,0.355,0.365,0.385,0.405,0.415,0.415,0.425,0.455,0.525,0.525,0.525,0.515,0.515,0.565,0.615,0.615,0.615,0.625,0.635,0.625,0.625,0.655,0.695,0.735,0.755,0.785,0.835,0.835,0.835,0.845,0.875,0.875,0.885,0.885,0.885,0.895,0.875,0.945,0.985,0.985,0.985,0.945,0.985};


  Bool_t LH    = kFALSE;
  Int_t trdCut = 5;
  switch (trdCut) {
    case 0: grTRD = new TGraph(68, x, y); break;
    case 2: grTRD = new TGraph(107, x2, y2); break;
    case 3:
      grTRD = new TGraph(84, x3, y3);
      LH    = kTRUE;
      break;
    case 4:
      grTRD = new TGraph(80, x4, y4);
      LH    = kTRUE;
      break;
    case 5:
      grTRD = new TGraph(113, x5, y5);
      LH    = kTRUE;
      break;
  }
  if (grTRD) grTRD->GetListOfFunctions()->Add(PairAnalysisHelper::GetFormula("varf", "P"));

  /// input to cut object
  PairAnalysisObjectCuts* pidTRD2d = new PairAnalysisObjectCuts("pidTRD2d", "pidTRD2d");
  pidTRD2d->SetCutType(PairAnalysisObjectCuts::kAll);  /// wheter kAll or kAny cut has to be fullfilled
  if (LH) pidTRD2d->AddCut(PairAnalysisVarManager::kTrdPidLikeEL, grTRD, grMax);
  else
    pidTRD2d->AddCut(PairAnalysisVarManager::kTrdPidANN, grTRD, grMax);


  // /// RICH pid cuts - 2-dimensional
  Double_t xR[129] = {0.300, 0.397, 0.434, 0.465, 0.499, 0.526, 0.554, 0.574, 0.594, 0.616, 0.638, 0.660,  0.684,
                      0.708, 0.721, 0.734, 0.747, 0.760, 0.773, 0.787, 0.801, 0.815, 0.829, 0.844, 0.859,  0.874,
                      0.890, 0.905, 0.921, 0.938, 0.954, 0.971, 0.988, 1.006, 1.024, 1.042, 1.060, 1.079,  1.098,
                      1.117, 1.137, 1.157, 1.178, 1.199, 1.220, 1.241, 1.263, 1.286, 1.308, 1.332, 1.355,  1.379,
                      1.403, 1.428, 1.454, 1.479, 1.505, 1.532, 1.559, 1.587, 1.615, 1.643, 1.672, 1.702,  1.732,
                      1.763, 1.794, 1.826, 1.858, 1.891, 1.924, 1.958, 1.993, 2.028, 2.064, 2.100, 2.138,  2.175,
                      2.214, 2.253, 2.293, 2.333, 2.375, 2.417, 2.460, 2.503, 2.547, 2.592, 2.638, 2.685,  2.732,
                      2.781, 2.830, 2.880, 2.931, 2.983, 3.035, 3.089, 3.144, 3.199, 3.256, 3.314, 3.372,  3.432,
                      3.492, 3.554, 3.617, 3.681, 3.746, 3.812, 3.880, 3.949, 4.018, 4.162, 4.310, 4.464,  4.623,
                      4.788, 4.959, 5.136, 5.320, 5.607, 5.910, 6.229, 6.681, 7.294, 8.103, 9.655, 999.000};
  Double_t yR[129] = {-1.090, -1.030, -1.010, -1.010, -0.990, -0.970, -0.970, -0.950, -0.950, -0.930, -0.930, -0.910,
                      -0.910, -0.890, -0.890, -0.890, -0.890, -0.870, -0.870, -0.870, -0.870, -0.850, -0.830, -0.830,
                      -0.810, -0.810, -0.790, -0.770, -0.770, -0.770, -0.770, -0.730, -0.730, -0.710, -0.710, -0.690,
                      -0.690, -0.670, -0.650, -0.630, -0.630, -0.610, -0.590, -0.590, -0.570, -0.550, -0.530, -0.530,
                      -0.510, -0.470, -0.490, -0.450, -0.450, -0.430, -0.410, -0.410, -0.370, -0.370, -0.350, -0.350,
                      -0.310, -0.330, -0.290, -0.290, -0.270, -0.270, -0.250, -0.270, -0.250, -0.210, -0.230, -0.210,
                      -0.210, -0.190, -0.170, -0.170, -0.170, -0.150, -0.150, -0.130, -0.130, -0.110, -0.110, -0.110,
                      -0.110, -0.110, -0.090, -0.090, -0.070, -0.070, -0.070, -0.030, -0.050, -0.030, -0.010, -0.010,
                      0.010,  0.010,  -0.010, 0.010,  0.010,  0.030,  0.010,  0.050,  0.050,  0.050,  0.070,  0.050,
                      0.070,  0.090,  0.090,  0.070,  0.090,  0.110,  0.090,  0.150,  0.130,  0.150,  0.110,  0.150,
                      0.130,  0.170,  0.150,  0.170,  0.150,  0.170,  0.150,  0.170,  0.170};

  //  TGraph *grR =new TGraph(129,xR,yR);
  TGraph* grR = new TGraph(129, xR, yR);
  grR->GetListOfFunctions()->Add(PairAnalysisHelper::GetFormula("varf", "P"));

  PairAnalysisObjectCuts* pidRICH2d = new PairAnalysisObjectCuts("pidRICH2d", "pidRICH2d");
  pidRICH2d->SetCutType(PairAnalysisObjectCuts::kAll);  /// wheter kAll or kAny cut has to be fullfilled
  pidRICH2d->AddCut(PairAnalysisVarManager::kRichPidANN, grR, grMax);


  // TRD Pid - 1-dimensional
  PairAnalysisVarCuts* pidTRD = new PairAnalysisVarCuts("pidTRD", "pidTRD");
  //pidTRD->AddCut(PairAnalysisVarManager::kTrdPidANN,     -0.68,   10.); // -0.76
  pidTRD->AddCut(PairAnalysisVarManager::kTrdPidLikeEL, 0.375, 10.);
  //pidTRD->AddCut(PairAnalysisVarManager::kTrdPidLikeEL,      0.05,   10.);

  // RICH Pid
  PairAnalysisVarCuts* pidRICH = new PairAnalysisVarCuts("pidRICH", "pidRICH");
  pidRICH->AddCut(PairAnalysisVarManager::kRichPidANN, -0.4,
                  10.);  //-0.95,   10.); //-0.77 = cnt 90% eleeff, -0.95 = min bias 90% eleeff
  //  pidRICH->AddCut(PairAnalysisVarManager::kRichPidANN,     0.23,   10.); //-0.95,   10.); //-0.77 = cnt 90% eleeff, -0.95 = min bias 90% eleeff

  // TOF Pid
  PairAnalysisVarCuts* pidTOF = new PairAnalysisVarCuts("pidTOF", "pidTOF");
  pidTOF->AddCut(PairAnalysisVarManager::kTofPidDeltaBetaEL, -1.65 * 3.2e-03,
                 +1.65 * 3.2e-03);  //90%
  //  pidTOF->AddCut(PairAnalysisVarManager::kMassSq,     -0.2,   0.2);

  PairAnalysisVarCuts* recRICHexcl = new PairAnalysisVarCuts("recRICH", "recRICH");
  //  recRICHexcl->AddCut(PairAnalysisVarManager::kRichPidANN,     -0.8,   10., kTRUE);
  recRICHexcl->AddCut(PairAnalysisVarManager::kRichHits, 6., 100.,
                      kTRUE);  /// min+max  exclusion for hits


  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv TRACK PDG CUTS vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
  PairAnalysisCutCombi* pidTRDavai = new PairAnalysisCutCombi("TRDPidAvai", "TRDPidAvai");
  pidTRDavai->AddCut(pidTRD2d, recTRD);


  PairAnalysisCutCombi* pidTOFavai = new PairAnalysisCutCombi("TOFPidAvai", "TOFPidAvai");
  pidTOFavai->AddCut(pidTOF, recTOF);


  // PDG electron + TRD selection
  PairAnalysisVarCuts* selTRDel = new PairAnalysisVarCuts("selTRDel", "selTRDel");
  selTRDel->SetCutType(PairAnalysisVarCuts::kAll);
  selTRDel->AddCut(PairAnalysisVarManager::kTrdHits, 2., 10.);
  selTRDel->AddCut("abs(PdgCode)", 11., 11.);

  // PDG electron + TRD selection
  PairAnalysisVarCuts* selTRDpi = new PairAnalysisVarCuts("selTRDpi", "selTRDpi");
  selTRDpi->SetCutType(PairAnalysisVarCuts::kAll);
  selTRDpi->AddCut(PairAnalysisVarManager::kTrdHits, 2., 10.);
  selTRDpi->AddCut("abs(PdgCode)", 211., 211.);

  // PDG electron exclusion + TRD selection
  PairAnalysisVarCuts* selTRDnotel = new PairAnalysisVarCuts("selTRDnotel", "selTRDnotel");
  selTRDnotel->SetCutType(PairAnalysisVarCuts::kAll);
  selTRDnotel->AddCut(PairAnalysisVarManager::kTrdHits, 2., 10.);
  selTRDnotel->AddCut("abs(PdgCode)", 11., 11., kTRUE);  //exclusion


  PairAnalysisCutGroup* fullPID = new PairAnalysisCutGroup("fullPID", "fullPID", PairAnalysisCutGroup::kCompAND);
  fullPID->AddCut(recRICH);
  fullPID->AddCut(pidRICH2d);
  fullPID->AddCut(recTRD);
  fullPID->AddCut(pidTRD2d);
  fullPID->AddCut(pidTOFavai);

  PairAnalysisCutGroup* fullREC = new PairAnalysisCutGroup("fullREC", "fullREC", PairAnalysisCutGroup::kCompAND);
  fullREC->AddCut(recRICH);
  fullREC->AddCut(recTRD);
  fullREC->AddCut(recTOF);


  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv PAIR PREFILTER CUTS vvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
  /// example of gamma rejection cuts as prefilter in order to remove tracks from
  /// the final track array. NOTE: inverted cut logic!

  /// rejection based on pair informations
  /// leg cuts for pair prefilter
  PairAnalysisCutGroup* topoLegCuts = new PairAnalysisCutGroup("legTopo", "legTopo", PairAnalysisCutGroup::kCompAND);
  //  topoLegCuts->AddCut(topoMom);
  topoLegCuts->AddCut(accCuts);
  topoLegCuts->AddCut(recSTS);
  topoLegCuts->AddCut(accPID);
  //  topoLegCuts->AddCut(pidRICHavai);
  topoLegCuts->AddCut(recRICH);
  topoLegCuts->AddCut(pidRICH);
  topoLegCuts->AddCut(pidTRDavai);
  //if(cutDefinition==kRichTRDTOFcfg)  topoLegCuts->AddCut(pidTOFavai);

  /// rejection based on pair informations
  PairAnalysisVarCuts* gammaCuts = new PairAnalysisVarCuts("gammaCut", "gammaCut");
  //  if(!cuts)    gammaCuts->AddCut(PairAnalysisVarManager::kM,  0.,   0.025); // exponential cut
  gammaCuts->AddCut(PairAnalysisVarManager::kM, 0., 0.025);  // exponential cut
  //  if(cuts)     gammaCuts->AddCut(PairAnalysisVarManager::kM, 0.01+0.01*val,9999.); // exponential cut
  //  if(cuts)     gammaCuts->AddCut(PairAnalysisVarManager::kM,  0., 0.01); // exponential cut
  //  if(cuts)     gammaCuts->AddCut(PairAnalysisVarManager::kM,  0.2, 999.); // exponential cut

  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv ACTIVATE CUTS vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
  /// activate the cut sets (order might be CPU timewise important)
  switch (cutDefinition) {
    case kACCcfg:
      papa->GetTrackFilter().AddCuts(mcPDGcuts);
      mcPDGcuts->Print();
      papa->GetTrackFilter().AddCuts(preCuts);
      preCuts->Print();
      papa->GetFinalTrackFilter().AddCuts(accCuts);
      accCuts->Print();
      break;

    case kRECcfg:
      papa->GetTrackFilter().AddCuts(mcPDGcuts);
      mcPDGcuts->Print();
      papa->GetTrackFilter().AddCuts(preCuts);
      preCuts->Print();
      papa->GetFinalTrackFilter().AddCuts(accCuts);
      accCuts->Print();

      papa->GetFinalTrackFilter().AddCuts(recRICH);
      recRICH->Print();
      papa->GetFinalTrackFilter().AddCuts(recTRD);
      recTRD->Print();
      papa->GetFinalTrackFilter().AddCuts(recTOF);
      recTOF->Print();
      break;

    case kFullPIDcfg:
      if (papa->DoEventProcess()) {
        papa->GetTrackFilter().AddCuts(mcPDGcuts);
        mcPDGcuts->Print();
        papa->GetTrackFilter().AddCuts(preCuts);
        preCuts->Print();
        papa->GetTrackFilter().AddCuts(preChiCuts);
        preChiCuts->Print();
        papa->GetTrackFilter().AddCuts(pT);
        pT->Print();
        papa->GetPairPreFilter().AddCuts(gammaCuts);
        gammaCuts->Print();

        papa->GetFinalTrackFilter().AddCuts(accCuts);
        accCuts->Print();
        papa->GetFinalTrackFilter().AddCuts(recSTS);
        recSTS->Print();
      }
      papa->GetFinalTrackFilter().AddCuts(recRICH);
      recRICH->Print();
      papa->GetFinalTrackFilter().AddCuts(pidRICH2d);
      pidRICH2d->Print();
      papa->GetFinalTrackFilter().AddCuts(recTRD);
      recTRD->Print();
      papa->GetFinalTrackFilter().AddCuts(pidTRD2d);
      pidTRD2d->Print();
      //    papa->           GetFinalTrackFilter().AddCuts(pidTRD);    pidTRD    ->Print();
      papa->GetFinalTrackFilter().AddCuts(pidTOFavai);
      pidTOFavai->Print();
      break;
  }
}

//______________________________________________________________________________________
void SetupPairCuts(PairAnalysis* papa, Int_t cutDefinition)
{
  ///
  /// Setup the pair cuts
  ///

  /// skip pair cuts if no pairing is done
  if (papa->IsNoPairing()) return;
  //  if(!papa->DoEventProcess()) continue;

  /// MC
  Bool_t hasMC = papa->GetHasMC();

  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv PAIR  CUTS vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
  /// example of rapidity selection
  // PairAnalysisVarCuts *rapCut=new PairAnalysisVarCuts("RapidityCut","RapdityCut");
  // rapCut->AddCut(PairAnalysisVarManager::kY,0.0,1.0);
  // papa->GetPairFilter().AddCuts(rapCut);
}

//______________________________________________________________________________________
void SetupTrackCutsMC(PairAnalysis* papa, Int_t cutDefinition)
{
  ///
  /// Setup the track cuts based on MC information only
  ///
  if (!papa->GetHasMC()) return;

  /// NOTE: skip this
  //  return;

  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv TRACK CUTS ON MCtruth vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
  /// example of adding acceptance cuts
  PairAnalysisVarCuts* accCutsMC = new PairAnalysisVarCuts("accGen", "accGen");
  accCutsMC->SetCutType(PairAnalysisVarCuts::kAll);  /// wheter kAll or kAny cut has to be fullfilled

  /// example for config specific cuts
  switch (cutDefinition) {
    default:
      accCutsMC->AddCut(PairAnalysisVarManager::kGeantId, 36.5, 37.5, kTRUE);
      //  accCutsMC->AddCut(PairAnalysisVarManager::kMvdHitsMC,      0.,   0.5, kTRUE); // MVD MC acceptance
      accCutsMC->AddCut(PairAnalysisVarManager::kStsHitsMC, 0., 0.5,
                        kTRUE);  // STS MC acceptance
      accCutsMC->AddCut(PairAnalysisVarManager::kTrdHitsMC, 0., 0.5,
                        kTRUE);  // TRD MC acceptance
      //    accCutsMC->AddCut(PairAnalysisVarManager::kRichhasProj,    0.,   0.5, kTRUE); // RICH MC acceptance
      //    accCutsMC->AddCut(PairAnalysisVarManager::kPtMC,           0.2, 1e30);
      break;
  }
  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv ACTIVATE CUTS vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
  switch (cutDefinition) {
    default: papa->GetTrackFilterMC().AddCuts(accCutsMC); accCutsMC->Print();
  }
}

//______________________________________________________________________________________
void ConfigBgrd(PairAnalysis* papa, Int_t cutDefinition)
{
  ///
  /// Configurate the background estimators
  ///

  // skip configs w/o pairing
  if (papa->IsNoPairing()) return;
  /// skip config user defined

  // /// Track Rotations
  // /// Randomly rotate in phi one of the daughter tracks in-side a 'ConeAngle'
  // /// for a given 'StartAngle'
  // PairAnalysisTrackRotator *rot=new PairAnalysisTrackRotator;
  // rot->SetConeAnglePhi(TMath::Pi());   /// cone angle
  // rot->SetStartAnglePhi(TMath::Pi());  /// starting angle
  // /// kRotatePositive, kRotateNegative or kRotateBothRandom (default)
  // rot->SetRotationType(PairAnalysisTrackRotator::kRotateBothRandom);
  // rot->SetIterations(5);              /// how often you want to rotate a track (think on CPU time)
  // papa->SetTrackRotator(rot);

  /// Ring buffer event mixing
  /// Event can be categorized by event variables their binning to get a better background description
  /// NOTE: the more granularity the less events you might find to mix with
  PairAnalysisMixingHandler* mix = new PairAnalysisMixingHandler;
  //  mix->AddVariable(PairAnalysisVarManager::kZvPrim, PairAnalysisHelper::MakeArbitraryBinning("-10.,-5.,-4.,-3.,-2.,-1.,1.,2.,3.,4.,5.,10.") );
  mix->AddVariable(PairAnalysisVarManager::kXvPrim,
                   PairAnalysisHelper::MakeArbitraryBinning("-10.,-2.5.,-2.,-1.5,-1.,-0.5.,0.,0.5,1.,1.5,2.,2.5,10."));
  mix->AddVariable(PairAnalysisVarManager::kYvPrim,
                   PairAnalysisHelper::MakeArbitraryBinning("-10.,-2.5.,-2.,-1.5,-1.,-0.5.,0.,0.5,1.,1.5,2.,2.5,10."));
  // mix->AddVariable(PairAnalysisVarManager::kXvPrim, PairAnalysisHelper::MakeArbitraryBinning("-2.5,-2.25,-2.,-1.75,-1.5,-1.25,-1,-0.75,-0.5,-0.25,0.,0.25,0.5,0.75,1.,1.25,1.5,1.75,2.,2.25,2.5") );
  // mix->AddVariable(PairAnalysisVarManager::kYvPrim, PairAnalysisHelper::MakeArbitraryBinning("-2.5,-2.25,-2.,-1.75,-1.5,-1.25,-1,-0.75,-0.5,-0.25,0.,0.25,0.5,0.75,1.,1.25,1.5,1.75,2.,2.25,2.5") );
  mix->AddVariable(PairAnalysisVarManager::kNTrk, PairAnalysisHelper::MakeLinBinning(10, 0., 500.));
  mix->AddVariable(PairAnalysisVarManager::kImpactParam, PairAnalysisHelper::MakeLinBinning(10, 0., 5.));
  mix->SetMixType(PairAnalysisMixingHandler::kOSonly);  /// which types to mix (LS ME, OS ME or both)
  mix->SetDepth(10);                                    /// pool depth (think on CPU time)
  papa->SetMixingHandler(mix);
}

//______________________________________________________________________________________
void InitHistograms(PairAnalysis* papa, Int_t cutDefinition)
{
  ///
  /// Initialise the histograms
  ///
  /// NOTE: Histograms are added to a specific class type such as 'event,pair,track'.
  ///       Several different objects of x-dimensions can be added via 'AddHisogram', 'AddProfile', 'AddSparse'.
  ///       Please have a look at PairAnalysisHistos.h to understand all possible input arguments.
  ///       For histograms and profiles you can use formulas with full 'TMath'-support to calculate new variables:
  ///       checkout http://root.cern.ch/root/html/TFormula.html#TFormula:Analyze for formula function strings
  ///       The type of binning is provided by some 'PairAnalysisHelper' functions.
  ///       Some examples are given below....

  Bool_t hasMC = papa->GetHasMC();

  ///Setup histogram Manager
  PairAnalysisHistos* histos = new PairAnalysisHistos(papa->GetName(), papa->GetTitle());
  papa->SetHistogramManager(histos);
  histos->SetPrecision(PairAnalysisHistos::kFloat);

  /// Initialize superior histogram classes (reserved words)
  histos->SetReservedWords("Hit;Track;Pair");

  ////// EVENT HISTOS /////
  //  AddEventHistograms(papa, cutDefinition);

  ////// PAIR HISTOS /////
  AddPairHistograms(papa, cutDefinition);

  ///// TRACK HISTOS /////
  AddTrackHistograms(papa, cutDefinition);

  ////// DEBUG //////
  TIter nextClass(histos->GetHistogramList());
  THashList* l = 0;
  while ((l = static_cast<THashList*>(nextClass()))) {
    printf(" [D] HistogramManger: Class %s: Histograms: %04d \n", l->GetName(), l->GetEntries());
  }

}  //end: init histograms

//______________________________________________________________________________________
void InitHF(PairAnalysis* papa, Int_t cutDefinition)
{
  ///
  /// Setup the Histogram Framework (a matrix of histograms)
  ///

  /// container
  PairAnalysisHF* hf = new PairAnalysisHF(papa->GetName(), papa->GetTitle());
  papa->SetHistogramArray(hf);

  ////// Add Variables and Dimensions /////
  hf->AddCutVariable(PairAnalysisVarManager::kNTrk, PairAnalysisHelper::MakeLinBinning(5, 0., 500));
  hf->AddCutVariable(PairAnalysisVarManager::kNVtxContrib,
                     PairAnalysisHelper::MakeArbitraryBinning("0.,1.,40.,150.,500."));

  ///Initialise superior histogram classes
  hf->SetReservedWords("Track;Pair");

  ////// EVENT HISTOS /////
  hf->AddClass("Event");  /// add histogram class
  /// define output objects
  hf->AddHistogram("Event", PairAnalysisHelper::MakeLinBinning(200, -1., 1.), PairAnalysisVarManager::kXvPrim);
  hf->AddHistogram("Event", PairAnalysisHelper::MakeLinBinning(200, -1., 1.), PairAnalysisVarManager::kYvPrim);
  hf->AddHistogram("Event", PairAnalysisHelper::MakeLinBinning(200, -1., 1.), PairAnalysisVarManager::kZvPrim);
}

//______________________________________________________________________________________
void AddMCSignals(PairAnalysis* papa, Int_t cutDefinition)
{
  /// Do we have an MC handler?
  if (!papa->GetHasMC()) return;

  // // switchers for signal activations
  Bool_t bSinglePart = kFALSE;
  Bool_t bHeavyFragm = kFALSE;
  Bool_t bLMVM       = kFALSE;
  Bool_t bRadiation  = kFALSE;
  Bool_t bBgrdComb   = kFALSE;
  Bool_t bBgrdPhys   = kFALSE;
  Bool_t bBgrdSig    = kFALSE;


  /// Predefined signals see PairAnalysisSignalMC::EDefinedSignal

  ///// single particle signals /////
  //  Bool_t fillMC=(cutDefinition==kStscfg ? kTRUE : kFALSE);
  TString fillMC                 = kFALSE;
  PairAnalysisSignalMC* deltaele = new PairAnalysisSignalMC(PairAnalysisSignalMC::kDeltaElectron);
  deltaele->SetFillPureMCStep(fillMC);

  PairAnalysisSignalMC* eleGam = new PairAnalysisSignalMC("e^{#gamma}", "eGam");
  eleGam->SetIsSingleParticle(kTRUE);
  eleGam->SetFillPureMCStep(fillMC);
  eleGam->SetLegPDGs(11, 1);
  eleGam->SetCheckBothChargesLegs(kTRUE, kFALSE);
  eleGam->SetMotherPDGs(22, 1);  //0:default all
  //  eleGam->SetMothersRelation(PairAnalysisSignalMC::kDifferent);

  PairAnalysisSignalMC* ele = new PairAnalysisSignalMC(PairAnalysisSignalMC::kPrimElectron);
  ele->SetFillPureMCStep(kFALSE);
  //  ele->SetMothersRelation(PairAnalysisSignalMC::kSame);

  PairAnalysisSignalMC* pio = new PairAnalysisSignalMC(PairAnalysisSignalMC::kPrimPion);
  pio->SetFillPureMCStep(fillMC);


  ////// low mass vector mesons ////
  Bool_t bUseSHM  = kFALSE;
  Bool_t bUseBdep = kTRUE;   //use impact parameter dependence (new HSD)
  Bool_t bUseMB   = kFALSE;  //use minimum bias value (new HSD)
  Int_t n         = 31;
  Double_t br     = 1.0;  // branching ratio
  Double_t b[]    = {0.0, 0.5, 1.0, 1.5, 2.0,  2.5,  3.0,  3.5,  4.0,  4.5,  5.0,  5.5,  6.0,  6.5,  7.0, 7.5,
                  8.0, 8.5, 9.0, 9.5, 10.0, 10.5, 11.0, 11.5, 12.0, 12.5, 13.0, 13.5, 14.0, 14.5, 20.};


  // omega
  PairAnalysisSignalMC* omega = new PairAnalysisSignalMC(PairAnalysisSignalMC::kOmega);
  omega->SetFillPureMCStep(fillMC);

  br                = 7.28e-04 * 1.5 / 4;
  Double_t omegaw[] = {2.48e+01 * br,
                       2.48e+01 * br,
                       2.41e+01 * br,
                       2.40e+01 * br,
                       2.41e+01 * br,
                       2.20e+01 * br,
                       2.02e+01 * br,
                       1.91e+01 * br,
                       1.75e+01 * br,
                       1.69e+01 * br,
                       1.59e+01 * br,
                       1.44e+01 * br,
                       1.26e+01 * br,
                       1.09e+01 * br,
                       9.84e+00 * br,
                       7.22e+00 * br,
                       7.40e+00 * br,
                       6.24e+00 * br,
                       5.54e+00 * br,
                       4.02e+00 * br,
                       3.54e+00 * br,
                       2.18e+00 * br,
                       2.12e+00 * br,
                       1.26e+00 * br,
                       9.40e-01 * br,
                       4.40e-01 * br,
                       3.60e-01 * br,
                       2.40e-01 * br,
                       4.00e-02 * br,
                       2.00e-02 * br,
                       0.0};
  if (bUseSHM) omega->SetWeight(1 * br);  //SHM
  else if (bUseMB)
    omega->SetWeight(0.55144E+01 * br);  //HSD
  //  else if(!bUseBdep) omega->SetWeight(19 * br );//HSD
  else if (!bUseBdep)
    omega->SetWeight(19 * br * 1.5 / 4);  //HSD
  else {
    TSpline3* weight = new TSpline3("omegawghts", b, omegaw, n);
    printf("omega weight at b=16fm: %f \n", weight->Eval(16.));
    if (bUseBdep) omega->SetWeight(weight, PairAnalysisVarManager::kImpactParam);
  }

  // pi0
  PairAnalysisSignalMC* pi0 = new PairAnalysisSignalMC(PairAnalysisSignalMC::kPi0);
  pi0->SetFillPureMCStep(fillMC);
  // pi0->SetWeight(6.02e+01);//SHM

  // pi0 -> gamma gamma
  PairAnalysisSignalMC* pi0Gamma = new PairAnalysisSignalMC(PairAnalysisSignalMC::kPi0Gamma);
  pi0Gamma->SetFillPureMCStep(fillMC);
  // pi0gam->SetWeight(6.02e+01);//SHM

  // pi0 -> e+e- gamma
  PairAnalysisSignalMC* pi0Dalitz = new PairAnalysisSignalMC(PairAnalysisSignalMC::kPi0Dalitz);
  pi0Dalitz->SetFillPureMCStep(fillMC);
  pi0Dalitz->SetIsDalitz(PairAnalysisSignalMC::kWhoCares, 0);
  //  pi0Dalitz->SetWeight(9.29);//SHM


  /// activate mc signal
  /// IMPORTANT NOTE: single particle and pair signals are sorted, BUT later pair signals
  ///                 should be the most detailed ones if you apply weights

  switch (cutDefinition) {
    default:
      /// Single particles
      papa->AddSignalMC(ele);
      papa->AddSignalMC(eleGam);

      // //embedded signals
      papa->AddSignalMC(pi0Dalitz);
      pi0Dalitz->Print();
      papa->AddSignalMC(omega);
      omega->Print();
      // papa->AddSignalMC(omegaDalitz); omegaDalitz->Print();
  }
}

//______________________________________________________________________________________
void AddEventHistograms(PairAnalysis* papa, Int_t cutDefinition)
{
  //
  // add event histograms
  //

  //  if(cutDefinition!=kTrdcfg) return;

  PairAnalysisHistos* histos = papa->GetHistoManager();
  histos->AddClass("Event");  /// add histogram class

  /// define output objects
  histos->AddHistogram("Event", PairAnalysisHelper::MakeLinBinning(200, 0., 2000.),
                       PairAnalysisVarManager::kTotalTRDHits);
  histos->AddHistogram("Event", PairAnalysisHelper::MakeLinBinning(200, 0., 2000.),
                       PairAnalysisVarManager::kTotalTRDHitsMC);
  histos->AddHistogram("Event", PairAnalysisHelper::MakeLinBinning(200, -1., 1.), PairAnalysisVarManager::kXvPrim);
  histos->AddHistogram("Event", PairAnalysisHelper::MakeLinBinning(200, -1., 1.), PairAnalysisVarManager::kYvPrim);
  histos->AddHistogram("Event", PairAnalysisHelper::MakeLinBinning(200, -1., 1.), PairAnalysisVarManager::kZvPrim);
  histos->AddHistogram("Event", PairAnalysisHelper::MakeLinBinning(250, -2.5, 2.5), PairAnalysisVarManager::kXvPrim,
                       PairAnalysisHelper::MakeLinBinning(250, -2.5, 2.5), PairAnalysisVarManager::kYvPrim);
  histos->AddHistogram("Event", PairAnalysisHelper::MakeLinBinning(100, 0., 400.),
                       PairAnalysisVarManager::kNVtxContrib);
  histos->AddHistogram("Event", PairAnalysisHelper::MakeLinBinning(200, 0.0, 10.0), "VtxChi/VtxNDF");
  histos->AddHistogram("Event", PairAnalysisHelper::MakeLinBinning(100, 0., 5000.), PairAnalysisVarManager::kNTrk);
  histos->AddHistogram("Event", PairAnalysisHelper::MakeLinBinning(100, 0., 5000.), PairAnalysisVarManager::kNTrkMC);
  histos->AddHistogram("Event", PairAnalysisHelper::MakeLinBinning(80, 0., 20.), PairAnalysisVarManager::kImpactParam);
  histos->AddHistogram("Event", PairAnalysisHelper::MakeLinBinning(100, 0., 20.), PairAnalysisVarManager::kImpactParam,
                       PairAnalysisHelper::MakeLinBinning(100, 0., 2000.), PairAnalysisVarManager::kNPrimMC);
  // prefilter rejection
  if (papa->GetPairPreFilter().GetCuts()->GetEntries()) {
    histos->AddProfile("Event", PairAnalysisHelper::MakeLinBinning(100, 0., 500.), PairAnalysisVarManager::kNTrk,
                       PairAnalysisVarManager::kRndmRej, "I");
  }

  // mixing statistics
  Int_t mixBins = 0;
  if (papa->GetMixingHandler()) {
    mixBins = papa->GetMixingHandler()->GetNumberOfBins();
    histos->AddHistogram("Event", PairAnalysisHelper::MakeLinBinning(mixBins, 0., mixBins),
                         PairAnalysisVarManager::kMixingBin);
  }
}

//______________________________________________________________________________________
void AddTrackHistograms(PairAnalysis* papa, Int_t cutDefinition)
{
  //
  // add track histograms
  //

  /// skip histograms in case of internal train
  if (!papa->DoEventProcess()) return;

  /// skip certain configs

  PairAnalysisHistos* histos = papa->GetHistoManager();

  /// Add track classes - LEGS of the pairs
  if (!papa->IsNoPairing()) {
    /// loop over all pair types and add classes (pair types in PairAnalysis.h EPairType)
    /// automatically skip pair types w.r.t. configured bgrd estimators
    PairAnalysisMixingHandler* mix = papa->GetMixingHandler();
    for (Int_t i = 0; i < PairAnalysis::kPairTypes; i++) {
      switch (i) {
        case PairAnalysis::kSEPP:
        case PairAnalysis::kSEMM:
          if (!papa->DoProcessLS()) {
            continue;
            break;
          }
        case PairAnalysis::kMEPP:
        case PairAnalysis::kMEMM:
          if (!mix || mix->GetMixType() != PairAnalysisMixingHandler::kOSonly) {
            continue;
            break;
          }
        case PairAnalysis::kMEMP:
          if (!mix) {
            continue;
            break;
          }
        case PairAnalysis::kMEPM:
          if (!mix || mix->GetMixType() != PairAnalysisMixingHandler::kAll) {
            continue;
            break;
          }
        case PairAnalysis::kSEPMRot:
          if (!papa->GetTrackRotator()) {
            continue;
            break;
          }
      }
      //      histos->AddClass(Form("Track.Legs.%s",PairAnalysis::PairClassName(i)));
    }
  }

  /// Add track classes - single tracks used for any pairing
  /// loop over all leg types and add classes (leg types in PairAnalysis.h ELegType)

  for (Int_t i = 0; i < PairAnalysis::kLegTypes; ++i)
    histos->AddClass(Form("Track.%s", PairAnalysis::TrackClassName(i)));

  /// OR/AND add merged leg histograms (e.g. e+-)
  //  histos->AddClass(Form("Track.%s",     PairAnalysis::PairClassName(PairAnalysis::kSEPM)));

  /// define REC ONLY histograms
  // ....

  /// add MC signal (if any) histograms to pair class
  if (papa->GetMCSignals()) {
    for (Int_t i = 0; i < papa->GetMCSignals()->GetEntriesFast(); ++i) {
      PairAnalysisSignalMC* sigMC = (PairAnalysisSignalMC*) papa->GetMCSignals()->At(i);

      /// selection
      if (!sigMC) continue;
      if (!sigMC->IsSingleParticle()) continue;  /// skip pair particle signals (no pairs)
      TString sigMCname = sigMC->GetName();
      /// by hand switched off
      if (sigMCname.EqualTo("eleGamPiOmega")) continue;

      /// mc truth - pair leg class
      //      if(!papa->IsNoPairing() && sigMC->GetFillPureMCStep()) histos->AddClass(Form("Track.Legs_%s_MCtruth",sigMCname.Data()));

      /// mc reconstructed - pair leg class
      //      if(!papa->IsNoPairing()) histos->AddClass(Form("Track.Legs_%s",        sigMCname.Data()));

      /// single tracks (merged +-)
      histos->AddClass(Form("Track.%s_%s", PairAnalysis::PairClassName(PairAnalysis::kSEPM), sigMCname.Data()));
      if (sigMC->GetFillPureMCStep())
        histos->AddClass(
          Form("Track.%s_%s_MCtruth", PairAnalysis::PairClassName(PairAnalysis::kSEPM), sigMCname.Data()));
    }
  }

  /// define MC and REC histograms
  AddTrackHistogramsReconstruction(histos, cutDefinition);
}

void AddTrackHistogramsReconstruction(PairAnalysisHistos* histos, Int_t cutDefinition)
{
  //
  // add track histograms
  //

  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(100, 0., 500.), PairAnalysisVarManager::kNTrk);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(400, 0, 4.), PairAnalysisVarManager::kPt);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(400, 0, 20.), PairAnalysisVarManager::kP);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(200, -3., 3.), PairAnalysisVarManager::kY,
                       PairAnalysisHelper::MakeLinBinning(125, 0, 5.), PairAnalysisVarManager::kPt);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(200, -3.0, 3.0), PairAnalysisVarManager::kY);
  TVectorD* loM2 = PairAnalysisHelper::MakeLinBinning(400, -0.25, 0.15);
  TVectorD* hiM2 = PairAnalysisHelper::MakeLinBinning(550, 0.15, 11.15);

  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(7, -0.5, 6.5), PairAnalysisVarManager::kTrdHits);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(3, -0.5, 2.5), PairAnalysisVarManager::kTofHits);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(46, 4.5, 50.5), PairAnalysisVarManager::kRichHits);
}


//______________________________________________________________________________________
void AddPairHistograms(PairAnalysis* papa, Int_t cutDefinition)
{
  ///
  /// add pair histograms
  ///

  /// skip if no pairing done
  if (papa->IsNoPairing()) return;

  PairAnalysisHistos* histos = papa->GetHistoManager();

  /// add histogram classes
  /// loop over all pair types and add classes (pair types in PairAnalysis.h EPairType)
  /// automatically skip pair types w.r.t. configured bgrd estimators
  PairAnalysisMixingHandler* mix = papa->GetMixingHandler();
  for (Int_t i = 0; i < PairAnalysis::kPairTypes; i++) {
    switch (i) {
      case PairAnalysis::kSEPP:
      case PairAnalysis::kSEMM:
        if (!papa->DoProcessLS()) {
          continue;
          break;
        }
      case PairAnalysis::kMEPP:
      case PairAnalysis::kMEMM:
        if (!mix || mix->GetMixType() != PairAnalysisMixingHandler::kOSonly) {
          continue;
          break;
        }
      case PairAnalysis::kMEMP:
        if (!mix) {
          continue;
          break;
        }
      case PairAnalysis::kMEPM:
        if (!mix || mix->GetMixType() != PairAnalysisMixingHandler::kAll) {
          continue;
          break;
        }
      case PairAnalysis::kSEPMRot:
        if (!papa->GetTrackRotator()) {
          continue;
          break;
        }
    }
    histos->AddClass(Form("Pair.%s", PairAnalysis::PairClassName(i)));
  }


  /// mixing statistics
  Int_t mixBins = 0;
  if (mix) {
    mixBins = mix->GetNumberOfBins();
    histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(mixBins, 0., mixBins),
                         PairAnalysisVarManager::kMixingBin);
    histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(100, 0., 400.),
                         PairAnalysisVarManager::kNVtxContrib);
  }

  ////// add MC signal histo classes
  if (papa->GetMCSignals()) {
    for (Int_t i = 0; i < papa->GetMCSignals()->GetEntriesFast(); ++i) {
      PairAnalysisSignalMC* sigMC = (PairAnalysisSignalMC*) papa->GetMCSignals()->At(i);
      if (!sigMC) continue;
      if (sigMC->IsSingleParticle()) continue;  /// skip pair particle signals (no pairs)
      TString sigMCname = sigMC->GetName();
      histos->AddClass(Form("Pair_%s", sigMCname.Data()));
      //      if(sigMC->GetFillPureMCStep()) histos->AddClass(Form("Pair_%s_MCtruth",sigMCname.Data()));
    }
  }


  ///// define output objects for MC and REC /////
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(300, 0., 3.),
                       PairAnalysisVarManager::kM);  /// 20MeV bins, 5 GeV/c2
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(100, 0., 1.),
                       PairAnalysisVarManager::kP);  /// 20MeV bins, 5 GeV/c2
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(300, 0., 3.), PairAnalysisVarManager::kM,
                       PairAnalysisVarManager::kWeight);  // 40MeV bins, 12GeV/c2
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(250, 0, 5.), PairAnalysisVarManager::kPt);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(200, -2., 6.), PairAnalysisVarManager::kY);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(200, -2., 6.), PairAnalysisVarManager::kY,
                       PairAnalysisHelper::MakeLinBinning(250, 0, 5.), PairAnalysisVarManager::kPt);
}
