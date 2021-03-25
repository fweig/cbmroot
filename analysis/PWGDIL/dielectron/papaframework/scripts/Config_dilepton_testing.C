/// \file Config_dilepton_testing.C
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

PairAnalysis* Config_Analysis(Int_t cutDefinition);

//WEIGHTS:
// 1 - 4.5 GeV  0-5% centr
// 2 - 8   GeV  0-5% centr
// 3 - 12  GeV  0-5% centr
// 4 - 4.5 GeV  mbias
// 5 - 8   GeV  mbias
// 6 - 12  GeV  mbias
// 7 - 3.42 GeV  0-5% centr
Int_t WEIGHTS = 3;

Bool_t GMC = false;

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
void AddHitHistograms(PairAnalysis* papa, Int_t cutDefinition);
void AddTrackHistograms(PairAnalysis* papa, Int_t cutDefinition);
void AddTrackHistogramsReconstruction(PairAnalysisHistos* histos, Int_t cutDefinition);
void AddPairHistograms(PairAnalysis* papa, Int_t cutDefinition);
////// CUTS
void SetupEventCuts(AnalysisTaskMultiPairAnalysis* task);
void SetupTrackCuts(PairAnalysis* papa, Int_t cutDefinition);
void SetupTrackCutsMC(PairAnalysis* papa, Int_t cutDefinition);
void AddCutStepHistograms(PairAnalysis* papa, Int_t cutDefinition);
////// SETTINGS
void AddMCSignals(PairAnalysis* papa, Int_t cutDefinition);
////// MISC
TObjArray* arrNames = names.Tokenize(";");
const Int_t nPapa   = arrNames->GetEntries();

//______________________________________________________________________________________
//AnalysisTaskMultiPairAnalysis *Config_dilepton_testing()
AnalysisTaskMultiPairAnalysis* Config_dilepton_testing(Int_t id = 1, TString TASK = "", Int_t scale = 0)
{
  ///
  /// creation of one multi task
  ///
  //  AnalysisTaskMultiPairAnalysis *task = new AnalysisTaskMultiPairAnalysis("testing",id);
  //  AnalysisTaskMultiPairAnalysis *task = new AnalysisTaskMultiPairAnalysis("testing",id,scale);
  AnalysisTaskMultiPairAnalysis* task = new AnalysisTaskMultiPairAnalysis("testing", id, WEIGHTS);
  //  task->SetBeamEnergy(8.); //TODO: get internally from FairBaseParSet::GetBeamMom()

  /// apply event cuts
  SetupEventCuts(task);

  /// add PAPA analysis with different cuts to the task
  for (Int_t i = 0; i < nPapa; ++i) {
    //activate configs
    switch (i) {
      case kACCcfg:
        continue;
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
  if (!GMC) SetupTrackCutsMC(papa, cutDefinition);

  papa->SetPreFilterUnlikeOnly();

  /// Monte Carlo Signals
  AddMCSignals(papa, cutDefinition);

  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv OUTPUT vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
  InitHistograms(papa, cutDefinition);

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
  //  eventCuts->AddCut("VtxChi/VtxNDF", 6., 1.e3, kTRUE);                 /// 'kTRUE': exclusion cut based on formula
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
  mcPDGcuts->SetCutType(PairAnalysisVarCuts::ECutType::kAll);  /// wheter 'kAll' or 'kAny' cut has to be fullfilled
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
  preCuts->SetCutType(PairAnalysisVarCuts::ECutType::kAll);  /// wheter 'kAll' or 'kAny' cut has to be fullfilled
  // preCuts->AddCut(PairAnalysisVarManager::kMvdFirstHitPosZ, 0.,   7.5);   /// a hit in 1st MVD layer
  //  preCuts->AddCut("MvdHits+StsHits",                        3.,   15.);
  //  preCuts->AddCut(PairAnalysisVarManager::kStsHits,                        3.,   15.);
  //  preCuts->AddCut(PairAnalysisVarManager::kStsHits,                        3.,   15.);
  preCuts->AddCut(PairAnalysisVarManager::kChi2NDFtoVtx, 0., 3.);  /// tracks pointing to the primary vertex

  PairAnalysisVarCuts* mvdCut = new PairAnalysisVarCuts("mvdCut", "mvdCut");
  mvdCut->SetCutType(PairAnalysisVarCuts::ECutType::kAll);  /// wheter 'kAll' or 'kAny' cut has to be fullfilled
  mvdCut->AddCut(PairAnalysisVarManager::kMvdHits, 3., 5.);

  /// acceptance cuts (applied after pair pre filter)
  PairAnalysisVarCuts* accCuts = new PairAnalysisVarCuts("accRec", "accRec");
  accCuts->SetCutType(PairAnalysisVarCuts::ECutType::kAll);  /// wheter 'kAll' or 'kAny' cut has to be fullfilled
  //accCuts->AddCut(PairAnalysisVarManager::kMvdHitsMC,      0.,   0.5, kTRUE); // MVD MC acceptance
  accCuts->AddCut(PairAnalysisVarManager::kStsHitsMC, 1., 99.);  // STS MC acceptance
  accCuts->AddCut(PairAnalysisVarManager::kTrdHitsMC, 1., 99.);  // TRD MC acceptance
  //  accCuts->AddCut(PairAnalysisVarManager::kRichhasProj,    0.,   0.5, kTRUE); // RICH MC acceptance
  //  accCuts->AddCut(PairAnalysisVarManager::kPt,             0.05, 1e30);        // NOTE: was 0.2 GeV/c


  PairAnalysisVarCuts* pT = new PairAnalysisVarCuts("pT", "pT");
  pT->AddCut(PairAnalysisVarManager::kPt, 0.05, 1e30);  // NOTE: was 0.2 GeV/c

  /// standard reconstruction cuts
  PairAnalysisVarCuts* recSTS = new PairAnalysisVarCuts("recSTS", "recSTS");
  recSTS->SetCutType(PairAnalysisVarCuts::ECutType::kAll);
  recSTS->AddCut(PairAnalysisVarManager::kStsHits, 3., 15.);

  PairAnalysisVarCuts* recMVD = new PairAnalysisVarCuts("recMVD", "recMVD");
  recMVD->SetCutType(PairAnalysisVarCuts::ECutType::kAll);
  recMVD->AddCut(PairAnalysisVarManager::kMvdHits, 3., 5.);

  /// RICH acceptance cuts
  PairAnalysisVarCuts* accRICH = new PairAnalysisVarCuts("accRICH", "accRICH");
  accRICH->SetCutType(PairAnalysisVarCuts::ECutType::kAll);
  accRICH->AddCut(PairAnalysisVarManager::kRichhasProj, 0., 0.5, kTRUE);

  /// RICH reconstruction cuts
  PairAnalysisVarCuts* recRICH = new PairAnalysisVarCuts("recRICH", "recRICH");
  recRICH->SetCutType(PairAnalysisVarCuts::ECutType::kAll);
  recRICH->AddCut(PairAnalysisVarManager::kRichHits, 6., 100.);  /// min+max inclusion for hits, 13=eff95%
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
  recTRD->SetCutType(PairAnalysisVarCuts::ECutType::kAll);
  recTRD->AddCut(PairAnalysisVarManager::kTrdHits, 3., 10.);  /// min+max requieremnt for hits
  //  recTRD->AddCut(PairAnalysisVarManager::kElossNew,         27.,   35.);         /// min+max requieremnt for hits

  /// TOF reconstruction cuts
  PairAnalysisVarCuts* recTOF = new PairAnalysisVarCuts("recTOF", "recTOF");
  recTOF->SetCutType(PairAnalysisVarCuts::ECutType::kAll);
  recTOF->AddCut(PairAnalysisVarManager::kTofHits, 1., 10.0);  /// min+max requieremnt for hits
  //  recTOF->AddCut(PairAnalysisVarManager::kP,               0.,    0.8);         /// momentum selection

  PairAnalysisVarCuts* accPID = new PairAnalysisVarCuts("accPID", "accPID");
  accPID->SetCutType(PairAnalysisVarCuts::ECutType::kAll);
  accPID->AddCut(PairAnalysisVarManager::kRichhasProj, 0., 0.5, kTRUE);
  accPID->AddCut(PairAnalysisVarManager::kTrdHits, 2., 10.);

  /// TRD pid cuts - 2-dimensional

  TGraph* grTRD = NULL;           // lower cut limt
  TGraph* grMax = new TGraph(2);  // upper cut limit
  grMax->SetPoint(0, 0., 999.);
  grMax->SetPoint(1, 999., 999.);
  grMax->GetListOfFunctions()->Add(PairAnalysisHelper::GetFormula("varf", "P"));


  Double_t x8[200] = {
    0.101342, 0.104063, 0.106857, 0.109725, 0.112671, 0.115695, 0.118801, 0.121991, 0.125266, 0.128628, 0.132082,
    0.135627, 0.139268, 0.143007, 0.146846, 0.150788, 0.154836, 0.158993, 0.163261, 0.167644, 0.172145, 0.176766,
    0.181511, 0.186384, 0.191388, 0.196526, 0.201802, 0.207219, 0.212782, 0.218494, 0.22436,  0.230383, 0.236568,
    0.242919, 0.24944,  0.256136, 0.263012, 0.270073, 0.277323, 0.284768, 0.292413, 0.300263, 0.308324, 0.316601,
    0.3251,   0.333828, 0.34279,  0.351992, 0.361441, 0.371144, 0.381108, 0.391339, 0.401845, 0.412633, 0.42371,
    0.435085, 0.446765, 0.458759, 0.471074, 0.483721, 0.496706, 0.510041, 0.523733, 0.537793, 0.55223,  0.567055,
    0.582278, 0.59791,  0.613961, 0.630443, 0.647368, 0.664747, 0.682592, 0.700917, 0.719734, 0.739055, 0.758896,
    0.779269, 0.800189, 0.82167,  0.843728, 0.866379, 0.889637, 0.91352,  0.938044, 0.963226, 0.989085, 1.01564,
    1.0429,   1.0709,   1.09965,  1.12917,  1.15948,  1.19061,  1.22257,  1.25539,  1.2891,   1.3237,   1.35924,
    1.39573,  1.4332,   1.47167,  1.51118,  1.55175,  1.59341,  1.63618,  1.68011,  1.72521,  1.77152,  1.81908,
    1.86792,  1.91806,  1.96955,  2.02243,  2.07672,  2.13247,  2.18972,  2.2485,   2.30886,  2.37085,  2.43449,
    2.49985,  2.56696,  2.63587,  2.70663,  2.77929,  2.85391,  2.93052,  3.00919,  3.08998,  3.17293,  3.25811,
    3.34557,  3.43539,  3.52761,  3.62231,  3.71956,  3.81941,  3.92194,  4.02723,  4.13534,  4.24636,  4.36036,
    4.47741,  4.59761,  4.72104,  4.84778,  4.97792,  5.11155,  5.24878,  5.38968,  5.53437,  5.68295,  5.83551,
    5.99217,  6.15303,  6.31821,  6.48783,  6.662,    6.84084,  7.02449,  7.21306,  7.4067,   7.60554,  7.80972,
    8.01937,  8.23466,  8.45572,  8.68272,  8.91581,  9.15516,  9.40094,  9.65331,  9.91246,  10.1786,  10.4518,
    10.7324,  11.0205,  11.3164,  11.6202,  11.9321,  12.2524,  12.5814,  12.9191,  13.2659,  13.6221,  13.9878,
    14.3633,  14.7489,  15.1448,  15.5514,  15.9689,  16.3976,  16.8378,  17.2898,  17.7539,  18.2306,  18.72,
    19.2225,  19.7386};
  Double_t y8[200] = {
    0.075, 0.075, 0.075, 0.075, 0.075, 0.075, 0.075, 0.075, 0.075, 0.075, 0.075, 0.075, 0.075, 0.075, 0.075, 0.075,
    0.075, 0.005, 0.075, 0.075, 0.075, 0.075, 0.075, 0.075, 0.075, 0.075, 0.075, 0.005, 0.005, 0.075, 0.305, 0.035,
    0.115, 0.125, 0.135, 0.115, 0.115, 0.075, 0.155, 0.115, 0.145, 0.105, 0.155, 0.155, 0.175, 0.165, 0.205, 0.185,
    0.145, 0.175, 0.165, 0.155, 0.165, 0.155, 0.175, 0.165, 0.185, 0.165, 0.165, 0.155, 0.175, 0.165, 0.175, 0.165,
    0.165, 0.165, 0.175, 0.185, 0.175, 0.175, 0.185, 0.175, 0.185, 0.195, 0.195, 0.195, 0.195, 0.185, 0.205, 0.195,
    0.195, 0.195, 0.195, 0.195, 0.185, 0.185, 0.185, 0.205, 0.215, 0.215, 0.215, 0.225, 0.225, 0.225, 0.225, 0.245,
    0.245, 0.255, 0.255, 0.255, 0.255, 0.265, 0.295, 0.305, 0.315, 0.345, 0.315, 0.355, 0.355, 0.365, 0.375, 0.375,
    0.405, 0.385, 0.405, 0.425, 0.435, 0.435, 0.435, 0.455, 0.445, 0.465, 0.445, 0.455, 0.455, 0.505, 0.495, 0.505,
    0.495, 0.525, 0.515, 0.535, 0.525, 0.495, 0.515, 0.515, 0.515, 0.495, 0.525, 0.515, 0.535, 0.475, 0.505, 0.485,
    0.505, 0.495, 0.515, 0.505, 0.485, 0.475, 0.255, 0.265, 0.295, 0.305, 0.315, 0.345, 0.315, 0.355, 0.355, 0.365,
    0.375, 0.375, 0.405, 0.385, 0.405, 0.425, 0.435, 0.435, 0.435, 0.455, 0.445, 0.465, 0.445, 0.455, 0.455, 0.505,
    0.495, 0.505, 0.495, 0.525, 0.515, 0.535, 0.525, 0.495, 0.515, 0.515, 0.515, 0.495, 0.525, 0.515, 0.535, 0.475,
    0.505, 0.485, 0.505, 0.495, 0.515, 0.505, 0.485, 0.475};


  Bool_t LH = kFALSE;
  grTRD     = new TGraph(200, x8, y8);
  LH        = kTRUE;
  if (grTRD) grTRD->GetListOfFunctions()->Add(PairAnalysisHelper::GetFormula("varf", "P"));

  /// input to cut object
  PairAnalysisObjectCuts* pidTRD2d = new PairAnalysisObjectCuts("pidTRD2d", "pidTRD2d");
  pidTRD2d->SetCutType(PairAnalysisObjectCuts::ECutType::kAll);  /// wheter kAll or kAny cut has to be fullfilled
  if (LH) pidTRD2d->AddCut(PairAnalysisVarManager::kTrdPidLikeEL, grTRD, grMax);
  else
    pidTRD2d->AddCut(PairAnalysisVarManager::kTrdPidANN, grTRD, grMax);

  // /// RICH pid cuts - 2-dimensional
  Double_t xR[200] = {
    0.302653, 0.308006, 0.313454, 0.318998, 0.324641, 0.330383, 0.336226, 0.342173, 0.348225, 0.354384, 0.360653,
    0.367032, 0.373523, 0.38013,  0.386854, 0.393696, 0.400659, 0.407746, 0.414958, 0.422298, 0.429767, 0.437368,
    0.445104, 0.452977, 0.460989, 0.469143, 0.477441, 0.485885, 0.494479, 0.503225, 0.512126, 0.521184, 0.530403,
    0.539784, 0.549332, 0.559048, 0.568936, 0.578999, 0.58924,  0.599662, 0.610268, 0.621062, 0.632047, 0.643227,
    0.654604, 0.666182, 0.677965, 0.689956, 0.70216,  0.714579, 0.727218, 0.740081, 0.753171, 0.766493, 0.78005,
    0.793847, 0.807888, 0.822177, 0.83672,  0.851519, 0.86658,  0.881908, 0.897506, 0.913381, 0.929536, 0.945977,
    0.962709, 0.979737, 0.997066, 1.0147,   1.03265,  1.05091,  1.0695,   1.08842,  1.10767,  1.12726,  1.1472,
    1.16749,  1.18814,  1.20916,  1.23054,  1.25231,  1.27446,  1.297,    1.31994,  1.34329,  1.36705,  1.39123,
    1.41583,  1.44087,  1.46636,  1.4923,   1.51869,  1.54555,  1.57289,  1.60071,  1.62902,  1.65784,  1.68716,
    1.717,    1.74737,  1.77827,  1.80973,  1.84174,  1.87431,  1.90746,  1.9412,   1.97554,  2.01048,  2.04604,
    2.08223,  2.11906,  2.15654,  2.19468,  2.2335,   2.27301,  2.31321,  2.35412,  2.39576,  2.43814,  2.48126,
    2.52515,  2.56981,  2.61526,  2.66152,  2.7086,   2.7565,   2.80526,  2.85488,  2.90537,  2.95676,  3.00906,
    3.06228,  3.11645,  3.17157,  3.22766,  3.28475,  3.34285,  3.40198,  3.46215,  3.52339,  3.58571,  3.64913,
    3.71367,  3.77936,  3.8462,   3.91423,  3.98347,  4.05392,  4.12563,  4.1986,   4.27286,  4.34844,  4.42535,
    4.50362,  4.58328,  4.66434,  4.74684,  4.8308,   4.91625,  5.0032,   5.0917,   5.18176,  5.27341,  5.36668,
    5.4616,   5.5582,   5.65651,  5.75656,  5.85838,  5.962,    6.06745,  6.17477,  6.28399,  6.39513,  6.50825,
    6.62336,  6.74051,  6.85973,  6.98106,  7.10454,  7.2302,   7.35808,  7.48823,  7.62068,  7.75547,  7.89264,
    8.03224,  8.17431,  8.31889,  8.46603,  8.61577,  8.76817,  8.92325,  9.08108,  9.2417,   9.40516,  9.57152,
    9.74081,  9.9131};
  Double_t yR[200] = {
    -1.17, -1.05, -1.11, -1.03, -1.11, -1.07, -1.05, -1.05, -1.03, -1.03, -1.01, -1.03, -0.97, -0.99, -1.01, -1.05,
    -0.97, -0.99, -1.03, -0.99, -0.99, -0.91, -0.95, -0.87, -0.97, -0.93, -0.95, -0.89, -0.95, -0.95, -0.89, -0.85,
    -0.91, -0.95, -0.91, -0.81, -0.87, -0.77, -0.77, -0.85, -0.79, -0.63, -0.69, -0.59, -0.65, -0.55, -0.77, -0.65,
    -0.61, -0.63, -0.55, -0.49, -0.47, -0.63, -0.53, -0.51, -0.37, -0.27, -0.35, -0.41, -0.35, -0.33, -0.23, -0.25,
    -0.23, -0.13, -0.11, -0.25, -0.19, -0.17, -0.13, -0.11, -0.17, -0.01, -0.01, 0.05,  0.05,  0.09,  0.09,  0.07,
    0.15,  0.13,  0.23,  0.19,  0.19,  0.25,  0.31,  0.27,  0.35,  0.33,  0.35,  0.33,  0.39,  0.39,  0.45,  0.45,
    0.45,  0.43,  0.47,  0.53,  0.53,  0.55,  0.57,  0.51,  0.57,  0.59,  0.61,  0.59,  0.63,  0.63,  0.65,  0.63,
    0.63,  0.67,  0.67,  0.67,  0.71,  0.69,  0.71,  0.71,  0.73,  0.71,  0.75,  0.75,  0.75,  0.75,  0.77,  0.75,
    0.79,  0.79,  0.79,  0.81,  0.79,  0.83,  0.83,  0.79,  0.81,  0.81,  0.83,  0.85,  0.83,  0.85,  0.85,  0.83,
    0.83,  0.85,  0.85,  0.85,  0.85,  0.85,  0.85,  0.87,  0.87,  0.85,  0.85,  0.85,  0.87,  0.87,  0.85,  0.87,
    0.87,  0.87,  0.85,  0.85,  0.87,  0.87,  0.87,  0.87,  0.87,  0.87,  0.87,  0.87,  0.89,  0.87,  0.85,  0.87,
    0.87,  0.87,  0.87,  0.87,  0.87,  0.85,  0.87,  0.87,  0.87,  0.87,  0.87,  0.89,  0.87,  0.87,  0.87,  0.87,
    0.89,  0.89,  0.89,  0.89,  0.87,  0.87,  0.89,  0.89};


  TGraph* grR = new TGraph(200, xR, yR);
  grR->GetListOfFunctions()->Add(PairAnalysisHelper::GetFormula("varf", "P"));

  PairAnalysisObjectCuts* pidRICH2d = new PairAnalysisObjectCuts("pidRICH2d", "pidRICH2d");
  pidRICH2d->SetCutType(PairAnalysisObjectCuts::ECutType::kAll);  /// wheter kAll or kAny cut has to be fullfilled
  pidRICH2d->AddCut(PairAnalysisVarManager::kRichPidANN, grR, grMax);


  Double_t mvP[8] = {0., 0.2, 0.4, 0.6, 0.8, 1., 1.2, 99.};
  Double_t mvD[8] = {0.4, 0.333, 0.2666, 0.2, 0.1333, 0.06666, 0., 0.};
  TGraph* grMVD   = new TGraph(7, mvP, mvD);
  grMVD->GetListOfFunctions()->Add(PairAnalysisHelper::GetFormula("varf", "P"));
  TGraph* grMVDMax = new TGraph(2);
  grMVDMax->SetPoint(0, 0., 999.);
  grMVDMax->SetPoint(1, 100., 999.);
  grMVDMax->GetListOfFunctions()->Add(PairAnalysisHelper::GetFormula("varf", "P"));
  PairAnalysisObjectCuts* MVDClosest = new PairAnalysisObjectCuts("MVDClosest", "MVDClosest");
  MVDClosest->SetCutType(PairAnalysisObjectCuts::ECutType::kAll);  /// wheter kAll or kAny cut has to be fullfilled
  MVDClosest->AddCut(PairAnalysisVarManager::kMvdHitClosest, grMVD, grMVDMax);

  Double_t mvS[8]  = {0., 0.2, 0.4, 0.6, 0.8, 1., 1.2, 99.};
  Double_t mvSD[8] = {1., 0.8333, 0.6666, 0.5, 0.3333, 0.1666, 0., 0.};
  TGraph* grSTS    = new TGraph(7, mvS, mvSD);
  grSTS->GetListOfFunctions()->Add(PairAnalysisHelper::GetFormula("varf", "P"));
  TGraph* grSTSMax = new TGraph(2);
  grSTSMax->SetPoint(0, 0., 999.);
  grSTSMax->SetPoint(1, 100., 999.);
  grSTSMax->GetListOfFunctions()->Add(PairAnalysisHelper::GetFormula("varf", "P"));
  PairAnalysisObjectCuts* STSClosest = new PairAnalysisObjectCuts("STSClosest", "STSClosest");
  STSClosest->SetCutType(PairAnalysisObjectCuts::ECutType::kAll);  /// wheter kAll or kAny cut has to be fullfilled
  STSClosest->AddCut(PairAnalysisVarManager::kStsHitClosest, grSTS, grSTSMax);


  Double_t mvdPO[12] = {0., 0.2, 0.4, 0.6, 0.8, 1., 1.2, 1.4, 1.6, 1.8, 2., 99.};
  Double_t mvdDO[12] = {0.035, 0.0315, 0.028, 0.0245, 0.021, 0.0175, 0.014, 0.0105, 0.007, 0.0035, 0., 0.};
  TGraph* grMVDOp    = new TGraph(12, mvdPO, mvdDO);
  grMVDOp->GetListOfFunctions()->Add(PairAnalysisHelper::GetFormula("varf", "MVDHitClosestMom"));
  TGraph* grMVDMaxOp = new TGraph(2);
  grMVDMaxOp->SetPoint(0, 0., 999.);
  grMVDMaxOp->SetPoint(1, 100., 999.);
  grMVDMaxOp->GetListOfFunctions()->Add(PairAnalysisHelper::GetFormula("varf", "MVDHitClosestMom"));
  PairAnalysisObjectCuts* MVDClosestOpeningAngle =
    new PairAnalysisObjectCuts("MVDClosestOpeningAngle", "MVDClosestOpeningAngle");
  MVDClosestOpeningAngle->SetCutType(
    PairAnalysisObjectCuts::ECutType::kAll);  /// wheter kAll or kAny cut has to be fullfilled
  MVDClosestOpeningAngle->AddCut(PairAnalysisVarManager::kMvdHitClosestOpeningAngle, grMVDOp, grMVDMaxOp);

  Double_t stsPO[12] = {0., 0.2, 0.4, 0.6, 0.8, 1., 1.2, 1.4, 1.6, 1.8, 2., 99.};
  Double_t stsDO[12] = {0.035, 0.0315, 0.028, 0.0245, 0.021, 0.0175, 0.014, 0.0105, 0.007, 0.0035, 0., 0.};
  TGraph* grSTSOp    = new TGraph(12, stsPO, stsDO);
  grSTSOp->GetListOfFunctions()->Add(PairAnalysisHelper::GetFormula("varf", "STSHitClosestMom"));
  TGraph* grSTSMaxOp = new TGraph(2);
  grSTSMaxOp->SetPoint(0, 0., 999.);
  grSTSMaxOp->SetPoint(1, 100., 999.);
  grSTSMaxOp->GetListOfFunctions()->Add(PairAnalysisHelper::GetFormula("varf", "STSHitClosestMom"));
  PairAnalysisObjectCuts* STSClosestOpeningAngle =
    new PairAnalysisObjectCuts("STSClosestOpeningAngle", "STSClosestOpeningAngle");
  STSClosestOpeningAngle->SetCutType(
    PairAnalysisObjectCuts::ECutType::kAll);  /// wheter kAll or kAny cut has to be fullfilled
  STSClosestOpeningAngle->AddCut(PairAnalysisVarManager::kStsHitClosestOpeningAngle, grSTSOp, grSTSMaxOp);

  // TOF Pid
  PairAnalysisVarCuts* pidTOF = new PairAnalysisVarCuts("pidTOF", "pidTOF");
  pidTOF->AddCut(PairAnalysisVarManager::kTofPidDeltaBetaEL, -1.65 * 3.2e-03, +1.65 * 3.2e-03);  //90%
  //  pidTOF->AddCut(PairAnalysisVarManager::kMassSq,     -0.2,   0.2);

  PairAnalysisCutCombi* pidTOFavai = new PairAnalysisCutCombi("TOFPidAvai", "TOFPidAvai");
  pidTOFavai->AddCut(pidTOF, recTOF);

  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv PAIR PREFILTER CUTS vvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
  /// example of gamma rejection cuts as prefilter in order to remove tracks from
  /// the final track array. NOTE: inverted cut logic!

  /// rejection based on pair informations
  /// leg cuts for pair prefilter
  /// rejection based on pair informations
  PairAnalysisVarCuts* gammaCuts = new PairAnalysisVarCuts("gammaCut", "gammaCut");
  gammaCuts->AddCut(PairAnalysisVarManager::kM, 0., 0.025);  // exponential cut

  PairAnalysisVarCuts* prepairMvdCuts = new PairAnalysisVarCuts("prepairMVD", "prepairMVD");
  prepairMvdCuts->AddCut(PairAnalysisVarManager::kMvdHitDist, 0., 1.);  // exponential cut

  PairAnalysisVarCuts* prepairStsCuts = new PairAnalysisVarCuts("prepairSTS", "prepairSTS");
  prepairStsCuts->AddCut(PairAnalysisVarManager::kStsHitDist, 0., 5.);  // exponential cut


  PairAnalysisVarCuts* preRecMvdCut = new PairAnalysisVarCuts("preRecMvdCut", "preRecMvdCut");
  preRecMvdCut->SetCutType(PairAnalysisVarCuts::ECutType::kAll);  /// wheter 'kAll' or 'kAny' cut has to be fullfilled
  preRecMvdCut->AddCut(PairAnalysisVarManager::kMvdHits, 3., 4.);
  preRecMvdCut->AddCut(PairAnalysisVarManager::kStsHits, 3., 20.);
  preRecMvdCut->AddCut(PairAnalysisVarManager::kRichHits, 6., 99.);
  preRecMvdCut->AddCut(PairAnalysisVarManager::kTrdHits, 3., 5.);

  PairAnalysisVarCuts* preRecStsCut = new PairAnalysisVarCuts("preRecStsCut", "preRecStsCut");
  preRecStsCut->SetCutType(PairAnalysisVarCuts::ECutType::kAll);  /// wheter 'kAll' or 'kAny' cut has to be fullfilled
  preRecStsCut->AddCut(PairAnalysisVarManager::kStsHits, 3., 20.);
  preRecStsCut->AddCut(PairAnalysisVarManager::kRichHits, 6., 99.);
  preRecStsCut->AddCut(PairAnalysisVarManager::kTrdHits, 3., 5.);


  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv ACTIVATE CUTS vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
  /// activate the cut sets (order might be CPU timewise important)
  switch (cutDefinition) {
    case kACCcfg:
      papa->GetTrackFilter().AddCuts(mcPDGcuts);
      mcPDGcuts->Print();
      papa->GetTrackFilter().AddCuts(accCuts);
      accCuts->Print();
      break;

    case kRECcfg:
      papa->GetTrackFilter().AddCuts(mcPDGcuts);
      mcPDGcuts->Print();
      papa->GetTrackFilter().AddCuts(preCuts);
      preCuts->Print();
      papa->GetFinalTrackFilter().AddCuts(recMVD);
      recMVD->Print();
      papa->GetFinalTrackFilter().AddCuts(recSTS);
      recSTS->Print();
      papa->GetFinalTrackFilter().AddCuts(recRICH);
      recRICH->Print();
      papa->GetFinalTrackFilter().AddCuts(recTRD);
      recTRD->Print();
      papa->GetFinalTrackFilter().AddCuts(recTOF);
      recTOF->Print();
      break;

    case kFullPIDcfg:
      papa->GetTrackFilter().AddCuts(mcPDGcuts);
      mcPDGcuts->Print();
      papa->GetTrackFilter().AddCuts(preCuts);
      preCuts->Print();
      papa->GetTrackFilter().AddCuts(accCuts);
      accCuts->Print();
      papa->GetTrackFilter().AddCuts(pT);
      pT->Print();
      papa->GetTrackFilter().AddCuts(preRecMvdCut);
      preRecMvdCut->Print();
      papa->GetPairPreFilter().AddCuts(gammaCuts);
      gammaCuts->Print();
      papa->GetFinalTrackFilter().AddCuts(MVDClosestOpeningAngle);
      MVDClosestOpeningAngle->Print();
      papa->GetFinalTrackFilter().AddCuts(recRICH);
      recRICH->Print();
      papa->GetFinalTrackFilter().AddCuts(pidRICH2d);
      pidRICH2d->Print();
      papa->GetFinalTrackFilter().AddCuts(recTRD);
      recTRD->Print();
      papa->GetFinalTrackFilter().AddCuts(pidTRD2d);
      pidTRD2d->Print();
      papa->GetFinalTrackFilter().AddCuts(pidTOFavai);
      pidTOFavai->Print();
      break;
  }
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
  accCutsMC->SetCutType(PairAnalysisVarCuts::ECutType::kAll);  /// wheter kAll or kAny cut has to be fullfilled

  /// example for config specific cuts
  switch (cutDefinition) {
    default:
      accCutsMC->AddCut(PairAnalysisVarManager::kGeantId, 36.5, 37.5, kTRUE);
      //  accCutsMC->AddCut(PairAnalysisVarManager::kMvdHitsMC,      0.,   0.5, kTRUE); // MVD MC acceptance
      accCutsMC->AddCut(PairAnalysisVarManager::kStsHitsMC, 0., 0.5, kTRUE);  // STS MC acceptance
      //    accCutsMC->AddCut(PairAnalysisVarManager::kRichHitsMC,     0.,   0.5, kTRUE); // STS MC acceptance
      accCutsMC->AddCut(PairAnalysisVarManager::kTrdHitsMC, 0., 0.5, kTRUE);  // TRD MC acceptance
      //    accCutsMC->AddCut(PairAnalysisVarManager::kRichhasProj,    0.,   0.5, kTRUE); // RICH MC acceptance
      //    accCutsMC->AddCut(PairAnalysisVarManager::kPtMC,           0.2, 1e30);
      break;
  }
  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv ACTIVATE CUTS vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
  // switch(cutDefinition) {
  // default:
  papa->GetTrackFilterMC().AddCuts(accCutsMC);
  accCutsMC->Print();
  // }
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
  histos->SetPrecision(PairAnalysisHistos::Eprecision::kFloat);

  /// Initialize superior histogram classes (reserved words)
  histos->SetReservedWords("Hit;Track;Pair");

  ////// PAIR HISTOS /////
  AddPairHistograms(papa, cutDefinition);

  ///// TRACK HISTOS /////
  AddTrackHistograms(papa, cutDefinition);

  //  AddHitHistograms(papa, cutDefinition);

  if (!GMC) AddCutStepHistograms(papa, cutDefinition);


  ////// DEBUG //////
  TIter nextClass(histos->GetHistogramList());
  THashList* l = 0;
  while ((l = static_cast<THashList*>(nextClass()))) {
    printf(" [D] HistogramManger: Class %s: Histograms: %04d \n", l->GetName(), l->GetEntries());
  }

}  //end: init histograms


//______________________________________________________________________________________
void AddMCSignals(PairAnalysis* papa, Int_t cutDefinition)
{
  /// Do we have an MC handler?
  if (!papa->GetHasMC()) return;


  Double_t Momega = 0;
  Double_t Mphi   = 0;
  Double_t Meta   = 0;
  Double_t Metap  = 0;
  Double_t Mrho   = 0;
  Double_t Mpi0   = 0;
  Double_t Minmed = 0;
  Double_t Mqgp   = 0;
  Double_t MDp    = 0;

  if (WEIGHTS == 1) {
    Momega = 0.270774;
    Mphi   = 0.022367;
    Meta   = 1.43997;
    Metap  = 0.0214123;
    Mrho   = 0.919404;
    Minmed = 0.0100221;
    Mqgp   = 0.000483824;
    MDp    = 70.5;
  }
  if (WEIGHTS == 2) {
    Momega = 2.28721;
    Mphi   = 0.311619;
    Meta   = 7.45497;
    Metap  = 0.262172;
    Mrho   = 7.57283;
    Minmed = 0.0304706;
    Mqgp   = 0.000452941;
    MDp    = 168;
  }
  if (WEIGHTS == 3) {
    Momega = 5.821;
    Mphi   = 1.018;
    Meta   = 14.8779;
    Metap  = 0.803;
    Mrho   = 19.0323;
    Minmed = 0.041625;
    Mqgp   = 0.00061875;
    MDp    = 229.5;
  }
  if (WEIGHTS == 7) {
    Momega = 0.270774;
    Mphi   = 0.022367;
    Meta   = 1.43997;
    Metap  = 0.0214123;
    Mrho   = 0.919404;
    Minmed = 0.0100221;
    Mqgp   = 0.00;
    MDp    = 70.5;
  }

  Double_t centr_mbias_ratio = 0.2613;
  if (WEIGHTS == 4) {
    Momega = 0.380235 * centr_mbias_ratio;
    Mphi   = 0.031409 * centr_mbias_ratio;
    Meta   = 2.02209 * centr_mbias_ratio;
    Metap  = 0.0300683 * centr_mbias_ratio;
    Mrho   = 1.29108 * centr_mbias_ratio;
    Minmed = 0.0140735 * centr_mbias_ratio;
    Mqgp   = 0.000679412 * centr_mbias_ratio;
    MDp    = 99 * centr_mbias_ratio;
  }
  if (WEIGHTS == 5) {
    Momega = 2.28721 * centr_mbias_ratio;
    Mphi   = 0.311619 * centr_mbias_ratio;
    Meta   = 7.45497 * centr_mbias_ratio;
    Metap  = 0.262172 * centr_mbias_ratio;
    Mrho   = 7.57283 * centr_mbias_ratio;
    Minmed = 0.0304706 * centr_mbias_ratio;
    Mqgp   = 0.000452941 * centr_mbias_ratio;
    MDp    = 168 * centr_mbias_ratio;
  }
  if (WEIGHTS == 6) {
    Momega = 5.821 * centr_mbias_ratio;
    Mphi   = 1.018 * centr_mbias_ratio;
    Meta   = 14.8779 * centr_mbias_ratio;
    Metap  = 0.803 * centr_mbias_ratio;
    Mrho   = 19.0323 * centr_mbias_ratio;
    Minmed = 0.041625 * centr_mbias_ratio;
    Mqgp   = 0.00061875 * centr_mbias_ratio;
    MDp    = 229.5 * centr_mbias_ratio;
  }

  ///// single particle signals /////
  TString fillMC                 = kFALSE;
  PairAnalysisSignalMC* deltaele = new PairAnalysisSignalMC(PairAnalysisSignalMC::EDefinedSignal::kDeltaElectron);
  deltaele->SetFillPureMCStep(fillMC);

  PairAnalysisSignalMC* eleGam = new PairAnalysisSignalMC("e^{#gamma}", "eGam");
  eleGam->SetFillPureMCStep(fillMC);
  eleGam->SetLegPDGs(11, 0);
  eleGam->SetCheckBothChargesLegs(kTRUE, kFALSE);
  eleGam->SetMotherPDGs(22, 0);  //0:default all
  eleGam->SetMothersRelation(PairAnalysisSignalMC::EBranchRelation::kDifferent);

  PairAnalysisSignalMC* eleGamPi = new PairAnalysisSignalMC("e^{#gamma}Pi", "eGamPi");
  eleGamPi->SetFillPureMCStep(fillMC);
  eleGamPi->SetLegPDGs(11, 0);
  eleGamPi->SetCheckBothChargesLegs(kTRUE, kFALSE);
  eleGamPi->SetMotherPDGs(111, 0);  //0:default all
  eleGamPi->SetMothersRelation(PairAnalysisSignalMC::EBranchRelation::kDifferent);

  PairAnalysisSignalMC* elePi = new PairAnalysisSignalMC("ePi", "ePi");
  elePi->SetFillPureMCStep(fillMC);
  elePi->SetLegPDGs(11, 1);
  elePi->SetIsSingleParticle(kTRUE);
  elePi->SetCheckBothChargesLegs(kTRUE, kFALSE);
  elePi->SetMotherPDGs(111, 0);  //0:default all

  PairAnalysisSignalMC* conv = new PairAnalysisSignalMC(PairAnalysisSignalMC::EDefinedSignal::kConversion);
  conv->SetFillPureMCStep(GMC);

  PairAnalysisSignalMC* conv2 = new PairAnalysisSignalMC("X #gamma e (comb.)", "X #gamma e (comb.)");
  conv2->SetFillPureMCStep(fillMC);
  conv2->SetLegPDGs(11, -11);
  conv2->SetMotherPDGs(22, 22);
  conv2->SetGrandMotherPDGs(111, 111);
  conv2->SetMothersRelation(PairAnalysisSignalMC::EBranchRelation::kSame);
  conv2->SetCheckBothChargesLegs(kTRUE, kTRUE);


  PairAnalysisSignalMC* ele = new PairAnalysisSignalMC(PairAnalysisSignalMC::EDefinedSignal::kPrimElectron);
  ele->SetFillPureMCStep(GMC);
  //  ele->SetMothersRelation(PairAnalysisSignalMC::kSame);

  PairAnalysisSignalMC* e = new PairAnalysisSignalMC("e", "e");
  e->SetFillPureMCStep(fillMC);
  e->SetIsSingleParticle(kTRUE);
  e->SetLegPDGs(11, 1);
  e->SetCheckBothChargesLegs(kTRUE, kFALSE);
  //  e->SetGEANTProcess(kPPrimary);
  //  e->SetInverse();

  PairAnalysisSignalMC* pio = new PairAnalysisSignalMC(PairAnalysisSignalMC::EDefinedSignal::kPrimPion);
  pio->SetFillPureMCStep(fillMC);
  Double_t br = 1.0;  // branching ratio

  // omega dalitz decays
  PairAnalysisSignalMC* omegaDalitz = new PairAnalysisSignalMC(PairAnalysisSignalMC::EDefinedSignal::kOmegaDalitz);
  omegaDalitz->SetFillPureMCStep(GMC);
  br = 7.7e-04;
  omegaDalitz->SetWeight(Momega * br);  //HSD

  PairAnalysisSignalMC* etap = new PairAnalysisSignalMC("etap", "etap");
  etap->SetFillPureMCStep(GMC);
  etap->SetIsDalitz(PairAnalysisSignalMC::EDalitz::kIsDalitz, 22);
  etap->SetLegPDGs(-11, 11);
  etap->SetCheckBothChargesLegs(kTRUE, kTRUE);
  etap->SetMotherPDGs(331, 331);
  etap->SetMothersRelation(PairAnalysisSignalMC::EBranchRelation::kSame);
  br = 4.73e-4;
  etap->SetWeight(Meta * br);

  PairAnalysisSignalMC* pibend = new PairAnalysisSignalMC("bendpi", "bendpi");
  pibend->SetFillPureMCStep(fillMC);
  pibend->SetLegPDGs(-11, 11);
  pibend->SetCheckBothChargesLegs(kTRUE, kTRUE);
  pibend->SetMotherPDGs(111, 111);
  //  pibend->SetMothersRelation(PairAnalysisSignalMC::EBranchRelation::kSame);
  pibend->SetIsSingleParticle(kTRUE);
  pibend->SetGEANTProcess(kPMagneticFieldL);

  // omega
  PairAnalysisSignalMC* omega = new PairAnalysisSignalMC(PairAnalysisSignalMC::EDefinedSignal::kOmega);
  omega->SetFillPureMCStep(GMC);
  br = 7.28e-04;
  //  omega->SetWeight(5.389 * br);//HSD
  omega->SetWeight(Momega * br);  //HSD

  // pi0
  PairAnalysisSignalMC* pi0 = new PairAnalysisSignalMC(PairAnalysisSignalMC::EDefinedSignal::kPi0);
  pi0->SetFillPureMCStep(fillMC);


  // pi0 -> gamma gamma
  PairAnalysisSignalMC* pi0Gamma = new PairAnalysisSignalMC(PairAnalysisSignalMC::EDefinedSignal::kPi0Gamma);
  pi0Gamma->SetFillPureMCStep(fillMC);

  // pi0 -> e+e- gamma
  PairAnalysisSignalMC* pi0Dalitz = new PairAnalysisSignalMC(PairAnalysisSignalMC::EDefinedSignal::kPi0Dalitz);
  //  pi0Dalitz->SetFillPureMCStep(fillMC);
  pi0Dalitz->SetFillPureMCStep(GMC);
  pi0Dalitz->SetIsDalitz(PairAnalysisSignalMC::EDalitz::kWhoCares, 0);

  PairAnalysisSignalMC* eta = new PairAnalysisSignalMC(PairAnalysisSignalMC::EDefinedSignal::kEta);
  eta->SetFillPureMCStep(fillMC);


  // eta dalitz
  PairAnalysisSignalMC* etaDalitz = new PairAnalysisSignalMC(PairAnalysisSignalMC::EDefinedSignal::kEtaDalitz);
  //  etaDalitz->SetFillPureMCStep(fillMC);
  etaDalitz->SetFillPureMCStep(GMC);
  etaDalitz->SetIsDalitz(PairAnalysisSignalMC::EDalitz::kWhoCares, 0);
  br = 6.9e-3;
  etaDalitz->SetWeight(Meta * br);

  /// activate mc signal
  /// IMPORTANT NOTE: single particle and pair signals are sorted, BUT later pair signals
  ///                 should be the most detailed ones if you apply weights

  PairAnalysisSignalMC* xx = new PairAnalysisSignalMC("xx (comb.)", "xx");
  xx->SetFillPureMCStep(GMC);
  xx->SetLegPDGs(0, 0);
  xx->SetMothersRelation(PairAnalysisSignalMC::EBranchRelation::kDifferent);

  PairAnalysisSignalMC* ee = new PairAnalysisSignalMC("ee (comb.)", "ee");
  ee->SetFillPureMCStep(fillMC);
  ee->SetLegPDGs(11, -11);
  ///  eleele->SetCheckBothChargesLegs(kTRUE,kTRUE);
  ee->SetMothersRelation(PairAnalysisSignalMC::EBranchRelation::kDifferent);

  PairAnalysisSignalMC* epi = new PairAnalysisSignalMC("epi (comb.)", "epi");
  epi->SetFillPureMCStep(fillMC);
  epi->SetLegPDGs(11, 211);
  epi->SetCheckBothChargesLegs(kTRUE, kTRUE);
  epi->SetMothersRelation(PairAnalysisSignalMC::EBranchRelation::kDifferent);

  PairAnalysisSignalMC* pipi = new PairAnalysisSignalMC("pipi (comb.)", "pipi");
  pipi->SetFillPureMCStep(fillMC);
  pipi->SetLegPDGs(211, 211);
  pipi->SetCheckBothChargesLegs(kTRUE, kTRUE);
  pipi->SetMothersRelation(PairAnalysisSignalMC::EBranchRelation::kDifferent);


  PairAnalysisSignalMC* inmed = new PairAnalysisSignalMC("in-medium SF", "in-medium SF");
  inmed->SetFillPureMCStep(GMC);
  inmed->SetLegPDGs(11, -11);
  inmed->SetMotherPDGs(99009011, 99009011);  //0:default all
  //  inmed->SetMotherPDGs(99009911,99009911); //0:default all
  inmed->SetMothersRelation(PairAnalysisSignalMC::EBranchRelation::kSame);
  inmed->SetGEANTProcess(kPPrimary);
  br = 4.45e-02 / 2;
  inmed->SetWeight(Minmed);  //default
  //inmed->SetWeight(Mqgp);//default


  // phi
  PairAnalysisSignalMC* phi = new PairAnalysisSignalMC(PairAnalysisSignalMC::EDefinedSignal::kPhi);
  phi->SetFillPureMCStep(GMC);
  br = 2.97e-04;
  phi->SetWeight(Mphi * br);


  PairAnalysisSignalMC* qgp = new PairAnalysisSignalMC("QGP rad.", "QGP rad.");
  qgp->SetFillPureMCStep(GMC);
  qgp->SetLegPDGs(11, -11);
  qgp->SetMotherPDGs(99009111, 99009111);  //0:default all
  //  qgp->SetMotherPDGs(99009911,99009911); //0:default all
  qgp->SetMothersRelation(PairAnalysisSignalMC::EBranchRelation::kSame);
  qgp->SetGEANTProcess(kPPrimary);
  br = 1.15e-02 / 2;
  qgp->SetWeight(Mqgp);  //default

  PairAnalysisSignalMC* Dp = new PairAnalysisSignalMC("D+", "D+");
  Dp->SetFillPureMCStep(GMC);
  Dp->SetLegPDGs(-11, 11);
  Dp->SetCheckBothChargesLegs(kTRUE, kTRUE);
  Dp->SetMotherPDGs(2214, 2214);
  Dp->SetMothersRelation(PairAnalysisSignalMC::EBranchRelation::kSame);
  br = 4.2e-5;
  Dp->SetWeight(MDp * br);

  // rho0
  PairAnalysisSignalMC* rho0 = new PairAnalysisSignalMC(PairAnalysisSignalMC::EDefinedSignal::kRho0);
  rho0->SetFillPureMCStep(GMC);
  br = 4.72e-05;
  rho0->SetWeight(Mrho * br);  //HSD


  switch (cutDefinition) {
    default:
      /// Single particles
      papa->AddSignalMC(ele);
      papa->AddSignalMC(conv);
      papa->AddSignalMC(pio);
      papa->AddSignalMC(xx);
      papa->AddSignalMC(ee);
      papa->AddSignalMC(epi);
      papa->AddSignalMC(pipi);

      papa->AddSignalMC(omegaDalitz);
      omegaDalitz->Print();
      papa->AddSignalMC(pi0Dalitz);
      pi0Dalitz->Print();
      papa->AddSignalMC(omega);
      omega->Print();
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
    for (Int_t i = 0; i < static_cast<Int_t>(PairAnalysis::EPairType::kPairTypes); i++) {
      switch (i) {
        case static_cast<Int_t>(PairAnalysis::EPairType::kSEPP):
        case static_cast<Int_t>(PairAnalysis::EPairType::kSEMM):
          if (!papa->DoProcessLS()) {
            continue;
            break;
          }
        case static_cast<Int_t>(PairAnalysis::EPairType::kMEPP):
        case static_cast<Int_t>(PairAnalysis::EPairType::kMEMM):
          if (!mix || mix->GetMixType() != PairAnalysisMixingHandler::EMixType::kOSonly) {
            continue;
            break;
          }
        case static_cast<Int_t>(PairAnalysis::EPairType::kMEMP):
          if (!mix) {
            continue;
            break;
          }
        case static_cast<Int_t>(PairAnalysis::EPairType::kMEPM):
          if (!mix || mix->GetMixType() != PairAnalysisMixingHandler::EMixType::kAll) {
            continue;
            break;
          }
        case static_cast<Int_t>(PairAnalysis::EPairType::kSEPMRot):
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

  for (Int_t i = 0; i < static_cast<Int_t>(PairAnalysis::ELegType::kLegTypes); ++i)
    histos->AddClass(Form("Track.%s", PairAnalysis::TrackClassName(i)));


  /// add MC signal (if any) histograms to pair class
  if (papa->GetMCSignals()) {
    for (Int_t i = 0; i < papa->GetMCSignals()->GetEntriesFast(); ++i) {
      PairAnalysisSignalMC* sigMC = (PairAnalysisSignalMC*) papa->GetMCSignals()->At(i);

      /// selection
      if (!sigMC) continue;
      //if(!sigMC->IsSingleParticle()) continue; /// skip pair particle signals (no pairs)
      TString sigMCname = sigMC->GetName();
      /// by hand switched off
      if (sigMCname.EqualTo("eleGamPiOmega")) continue;

      /// mc truth - pair leg class
      //      if(!papa->IsNoPairing() && sigMC->GetFillPureMCStep()) histos->AddClass(Form("Track.Legs_%s_MCtruth",sigMCname.Data()));

      /// mc reconstructed - pair leg class
      //      if(!papa->IsNoPairing()) histos->AddClass(Form("Track.Legs_%s",        sigMCname.Data()));

      /// single tracks (merged +-)
      histos->AddClass(Form("Track.%s_%s",
                            PairAnalysis::PairClassName(static_cast<Int_t>(PairAnalysis::EPairType::kSEPM)),
                            sigMCname.Data()));
      if (sigMC->GetFillPureMCStep() && GMC)
        histos->AddClass(Form("Track.%s_%s_MCtruth",
                              PairAnalysis::PairClassName(static_cast<Int_t>(PairAnalysis::EPairType::kSEPM)),
                              sigMCname.Data()));
    }
  }


  histos->AddProfile("Track", PairAnalysisHelper::MakeLogBinning(200, 0., 20.), PairAnalysisVarManager::kP,
                     PairAnalysisVarManager::kPRes, "I");
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLogBinning(200, 0., 20.), PairAnalysisVarManager::kP,
                       PairAnalysisHelper::MakeLogBinning(200, 0., 0.2), PairAnalysisVarManager::kPRes);


  /// define MC and REC histograms
  AddTrackHistogramsReconstruction(histos, cutDefinition);
}

void AddTrackHistogramsReconstruction(PairAnalysisHistos* histos, Int_t cutDefinition)
{
  //
  // add track histograms
  //

  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(49, -0.5, 48.5), PairAnalysisVarManager::kGeantId);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(100, 0., 500.), PairAnalysisVarManager::kNTrk);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(400, 0, 4.), PairAnalysisVarManager::kPt);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(400, 0, 20.), PairAnalysisVarManager::kP);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(300, -3., 6.), PairAnalysisVarManager::kYlab,
                       PairAnalysisHelper::MakeLinBinning(125, 0, 5.), PairAnalysisVarManager::kPt);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(300, -3., 6.), PairAnalysisVarManager::kYlab,
                       PairAnalysisHelper::MakeLinBinning(125, 0, 5.), PairAnalysisVarManager::kPt,
                       PairAnalysisHelper::MakeLinBinning(5, -0.5, 4.5), PairAnalysisVarManager::kTrdHits);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(300, -3.0, 6.0), PairAnalysisVarManager::kYlab);
  TVectorD* loM2 = PairAnalysisHelper::MakeLinBinning(400, -0.25, 0.15);
  TVectorD* hiM2 = PairAnalysisHelper::MakeLinBinning(550, 0.15, 11.15);

  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(100, -0.5, 9.5),
                       PairAnalysisVarManager::kChi2NDFtoVtx);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(100, -0.5, 9.5),
                       PairAnalysisVarManager::kTrdChi2NDF);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(17, -0.5, 16.5), PairAnalysisVarManager::kStsHits);
  //  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(21,-0.5, 20.5), PairAnalysisVarManager::kStsMvdHits);
  //histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(17,-0.5, 16.5), "MvdHits+StsHits");
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(6, -0.5, 5.5), PairAnalysisVarManager::kMvdHits);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(103, -2.5, 100.5), PairAnalysisVarManager::kZvMC,
                       PairAnalysisHelper::MakeLinBinning(200, -100, 100), PairAnalysisVarManager::kRvMC);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(510, -0.5, 100.5), PairAnalysisVarManager::kZvMC,
                       PairAnalysisHelper::MakeLinBinning(6, -0.5, 5.5), PairAnalysisVarManager::kMvdHits);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(7, -0.5, 6.5), PairAnalysisVarManager::kTrdHits);

  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(3, -0.5, 2.5), PairAnalysisVarManager::kTofHits);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(46, 4.5, 50.5), PairAnalysisVarManager::kRichHits);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(110, -0.05, 1.05),
                       PairAnalysisVarManager::kTrdPidLikeEL);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLogBinning(200, 0.1, 20.), PairAnalysisVarManager::kTrdPin,
                       PairAnalysisHelper::MakeLinBinning(110, -0.05, 1.05), PairAnalysisVarManager::kTrdPidLikeEL);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(200, -2.0, 2.0),
                       PairAnalysisVarManager::kRichPidANN);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(75, 0., 15.), PairAnalysisVarManager::kP,
                       PairAnalysisHelper::MakeLinBinning(200, -2.0, 2.0), PairAnalysisVarManager::kRichPidANN);

  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(200, 0., 1.),
                       PairAnalysisVarManager::kMvdHitClosest);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(100, 0., 10.), PairAnalysisVarManager::kP,
                       PairAnalysisHelper::MakeLinBinning(400, 0., 4.), PairAnalysisVarManager::kMvdHitClosest);
  histos->AddHistogram(
    "Track", PairAnalysisHelper::MakeLinBinning(100, 0., 10.), PairAnalysisVarManager::kMvdHitClosestMom,
    PairAnalysisHelper::MakeLinBinning(200, 0., TMath::Pi() / 4), PairAnalysisVarManager::kMvdHitClosestOpeningAngle);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(100, 0., 10.), PairAnalysisVarManager::kP,
                       PairAnalysisHelper::MakeLinBinning(200, 0., TMath::Pi() / 4),
                       PairAnalysisVarManager::kMvdHitClosestOpeningAngle);

  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(200, 0., 1.),
                       PairAnalysisVarManager::kStsHitClosest);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(100, 0., 10.), PairAnalysisVarManager::kP,
                       PairAnalysisHelper::MakeLinBinning(400, 0., 4.), PairAnalysisVarManager::kStsHitClosest);
  histos->AddHistogram(
    "Track", PairAnalysisHelper::MakeLinBinning(100, 0., 10.), PairAnalysisVarManager::kStsHitClosestMom,
    PairAnalysisHelper::MakeLinBinning(200, 0., TMath::Pi() / 4), PairAnalysisVarManager::kStsHitClosestOpeningAngle);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(100, 0., 10.), PairAnalysisVarManager::kP,
                       PairAnalysisHelper::MakeLinBinning(200, 0., TMath::Pi() / 4),
                       PairAnalysisVarManager::kStsHitClosestOpeningAngle);

  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(200, 0., TMath::Pi() / 4),
                       PairAnalysisVarManager::kMvdHitClosestOpeningAngle);

  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(7, -0.5, 6.5), PairAnalysisVarManager::kTrdHitsMC);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(7, -0.5, 6.5), PairAnalysisVarManager::kStsHitsMC);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(7, -0.5, 6.5), PairAnalysisVarManager::kMvdHitsMC);

  histos->AddHistogram("Track", PairAnalysisHelper::MakePdgBinning(), PairAnalysisVarManager::kPdgCode);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(49, -0.5, 48.5), PairAnalysisVarManager::kGeantId);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(50, 0., 10.), PairAnalysisVarManager::kE);

  //  histos->AddHistogram("Track", PairAnalysisHelper::MakePdgBinning(), PairAnalysisVarManager::kPdgCodeMother);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(10, -0.5, 9.5),
                       PairAnalysisVarManager::kPdgCodeMother);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(10, -0.5, 9.5),
                       PairAnalysisVarManager::kPdgCodeMother, PairAnalysisHelper::MakeLinBinning(12, -0.5, 11.5),
                       PairAnalysisVarManager::kGeantId);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(10, -0.5, 9.5),
                       PairAnalysisVarManager::kPdgCodeMother, PairAnalysisHelper::MakeLinBinning(12, -0.5, 11.5),
                       PairAnalysisVarManager::kGeantId, PairAnalysisVarManager::kWeight);

  // histos->AddHistogram("Track",PairAnalysisHelper::MakeLinBinning(510,-0.5,100.), PairAnalysisVarManager::kZvMC,
  // 		       PairAnalysisHelper::MakeLinBinning(500,-10,100.), PairAnalysisVarManager::kRvMC);

  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(120, 0., 30.),
                       PairAnalysisVarManager::kMvdFirstHitPosZ);

  histos->AddHistogram("Track", PairAnalysisHelper::MakeLogBinning(100, 0., 10.), PairAnalysisVarManager::kP,
                       PairAnalysisHelper::MakeLinBinning(1000, -0.5, 0.5), PairAnalysisVarManager::kTofPidDeltaBetaEL);
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
  for (Int_t i = 0; i < static_cast<Int_t>(PairAnalysis::EPairType::kPairTypes); i++) {
    switch (i) {
      case static_cast<Int_t>(PairAnalysis::EPairType::kSEPP):
      case static_cast<Int_t>(PairAnalysis::EPairType::kSEMM):
        if (!papa->DoProcessLS()) {
          continue;
          break;
        }
      case static_cast<Int_t>(PairAnalysis::EPairType::kMEPP):
      case static_cast<Int_t>(PairAnalysis::EPairType::kMEMM):
        if (!mix || mix->GetMixType() != PairAnalysisMixingHandler::EMixType::kOSonly) {
          continue;
          break;
        }
      case static_cast<Int_t>(PairAnalysis::EPairType::kMEMP):
        if (!mix) {
          continue;
          break;
        }
      case static_cast<Int_t>(PairAnalysis::EPairType::kMEPM):
        if (!mix || mix->GetMixType() != PairAnalysisMixingHandler::EMixType::kAll) {
          continue;
          break;
        }
      case static_cast<Int_t>(PairAnalysis::EPairType::kSEPMRot):
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
      if (sigMC->GetFillPureMCStep() && GMC) histos->AddClass(Form("Pair_%s_MCtruth", sigMCname.Data()));
    }
  }


  ///// define output objects for MC and REC /////
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(300, 0., 3.),
                       PairAnalysisVarManager::kM);  /// 20MeV bins, 5 GeV/c2
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(100, 0., 10.),
                       PairAnalysisVarManager::kP);  /// 20MeV bins, 5 GeV/2c
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(300, 0., 3.), PairAnalysisVarManager::kM,
                       PairAnalysisVarManager::kWeight);  // 40MeV bins, 12GeV/c2
  // histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(300,0.,3.), PairAnalysisVarManager::kM,
  // 		       PairAnalysisHelper::MakeLinBinning(1000,0.000000000001,0.0001),PairAnalysisVarManager::kWeight); // 40MeV bins, 12GeV/c2
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(250, 0, 5.), PairAnalysisVarManager::kPt);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(200, -2., 6.), PairAnalysisVarManager::kYlab);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(200, -2., 6.), PairAnalysisVarManager::kYlab,
                       PairAnalysisHelper::MakeLinBinning(250, 0, 5.), PairAnalysisVarManager::kPt);


  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(49, -0.5, 48.5), PairAnalysisVarManager::kGeantId);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(10, -0.5, 9.5),
                       PairAnalysisVarManager::kPdgCodeMother);
  //  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(49,-0.5,48.5), PairAnalysisVarManager::kGeantIdMother);

  // histos->AddHistogram("Pair",PairAnalysisHelper::MakeLinBinning(900,0.,900.), PairAnalysisVarManager::kZvMC,
  // 		       PairAnalysisHelper::MakeLinBinning(1000,0.,5000.), PairAnalysisVarManager::kRvMC);

  // histos->AddHistogram("Pair",PairAnalysisHelper::MakeLinBinning(510,-0.5,100.5), PairAnalysisVarManager::kZvMC,
  // 		       PairAnalysisHelper::MakeLinBinning(100,0.,.5), PairAnalysisVarManager::kRvMC);

  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(21, -0.5, 20.5),
                       PairAnalysisVarManager::kStsMvdFirstDaughter, PairAnalysisHelper::MakeLinBinning(21, -0.5, 20.5),
                       PairAnalysisVarManager::kStsMvdSecondDaughter);
  histos->AddHistogram(
    "Pair", PairAnalysisHelper::MakeLinBinning(21, -0.5, 20.5), PairAnalysisVarManager::kStsMvdTrdFirstDaughter,
    PairAnalysisHelper::MakeLinBinning(21, -0.5, 20.5), PairAnalysisVarManager::kStsMvdTrdSecondDaughter);
  histos->AddHistogram(
    "Pair", PairAnalysisHelper::MakeLinBinning(71, -0.5, 70.5), PairAnalysisVarManager::kStsMvdRichFirstDaughter,
    PairAnalysisHelper::MakeLinBinning(71, -0.5, 70.5), PairAnalysisVarManager::kStsMvdRichSecondDaughter);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(21, -0.5, 20.5),
                       PairAnalysisVarManager::kStsFirstDaughter, PairAnalysisHelper::MakeLinBinning(21, -0.5, 20.5),
                       PairAnalysisVarManager::kStsSecondDaughter);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(21, -0.5, 20.5),
                       PairAnalysisVarManager::kMvdFirstDaughter, PairAnalysisHelper::MakeLinBinning(21, -0.5, 20.5),
                       PairAnalysisVarManager::kMvdSecondDaughter);

  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(200, 0., 10.), PairAnalysisVarManager::kMvdHitDist);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(100, 0., 10.), PairAnalysisVarManager::kP,
                       PairAnalysisHelper::MakeLinBinning(200, 0., 10.), PairAnalysisVarManager::kMvdHitDist);

  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(400, 0., 40.), PairAnalysisVarManager::kStsHitDist);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(100, 0., 10.), PairAnalysisVarManager::kP,
                       PairAnalysisHelper::MakeLinBinning(400, 0., 40.), PairAnalysisVarManager::kStsHitDist);


  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(100, 0., TMath::Pi() / 2),
                       PairAnalysisVarManager::kOpeningAngle);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(100, 0., 5.), PairAnalysisVarManager::kLegsP);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(314, 0, 3.14), PairAnalysisVarManager::kPhivPair);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(314, 0, 3.14),
                       PairAnalysisVarManager::kCosPointingAngleMC);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(314, 0, 3.14),
                       PairAnalysisVarManager::kCosPointingAngle);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(100, 0., 5.), PairAnalysisVarManager::kLegsP,
                       PairAnalysisHelper::MakeLinBinning(100, 0., 0.5), PairAnalysisVarManager::kOpeningAngle);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(100, 0., 5.), PairAnalysisVarManager::kLegsP,
                       PairAnalysisHelper::MakeLinBinning(314, 0., 3.14), PairAnalysisVarManager::kPhivPair);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(200, 0., TMath::Pi() / 2),
                       PairAnalysisVarManager::kOpeningAngle, PairAnalysisHelper::MakeLinBinning(314, 0., 3.14),
                       PairAnalysisVarManager::kPhivPair);
  //  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(200,-1.,1.), "0.035-LegsP*0.01-OpeningAngle");
}


//______________________________________________________________________________________
void AddHitHistograms(PairAnalysis* papa, Int_t cutDefinition)
{
  ///
  /// add hit histograms
  ///

  PairAnalysisHistos* histos = papa->GetHistoManager();

  /// Add hit classes - TRD tracks (before pairing)
  histos->AddClass("Hit.TRD");


  /// add MC signal (if any) histograms to hit class
  if (papa->GetMCSignals()) {
    for (Int_t i = 0; i < papa->GetMCSignals()->GetEntriesFast(); ++i) {
      PairAnalysisSignalMC* sigMC = (PairAnalysisSignalMC*) papa->GetMCSignals()->At(i);
      if (!sigMC) continue;
      if (!sigMC->IsSingleParticle()) continue;  /// skip pair particle signals (no pairs)
      TString sigMCname = sigMC->GetName();
      /// by hand switched off
      if (sigMCname.EqualTo("eleGamPiOmega")) continue;

      /// DET specific MC histograms
      histos->AddClass(Form("Hit.TRD_%s", sigMCname.Data()));


      histos->AddHistogram(Form("Hit.TRD_%s", sigMCname.Data()), PairAnalysisHelper::MakeLinBinning(400, 0., 1.e+2),
                           PairAnalysisVarManager::kEloss);
      histos->AddHistogram(Form("Hit.TRD_%s", sigMCname.Data()), PairAnalysisHelper::MakeLinBinning(400, 0., 1.e+2),
                           PairAnalysisVarManager::kElossMC);
      histos->AddHistogram(Form("Hit.TRD_%s", sigMCname.Data()),
                           PairAnalysisHelper::MakeArbitraryBinning("0.,0.25,0.5,1.,1.5,2.,3.,4.,5.,6.,7.,8.,9.,10."),
                           PairAnalysisVarManager::kTrdPin, PairAnalysisHelper::MakeLinBinning(400, 0., 1.e+2),
                           PairAnalysisVarManager::kEloss);

      histos->AddHistogram(Form("Hit.TRD_%s", sigMCname.Data()), PairAnalysisHelper::MakeLinBinning(16, -0.5, 15.5),
                           PairAnalysisVarManager::kTrdPads, PairAnalysisHelper::MakeLinBinning(10, -0.5, 9.5),
                           PairAnalysisVarManager::kLinksMC);
      histos->AddProfile(Form("Hit.TRD_%s", sigMCname.Data()), PairAnalysisHelper::MakeLinBinning(16, -0.5, 15.5),
                         PairAnalysisVarManager::kTrdPads, PairAnalysisVarManager::kLinksMC, "I");

      histos->AddHistogram(Form("Hit.TRD_%s", sigMCname.Data()), PairAnalysisHelper::MakeLinBinning(16, -0.5, 15.5),
                           PairAnalysisVarManager::kTrdPads);
      histos->AddHistogram(Form("Hit.TRD_%s", sigMCname.Data()), PairAnalysisHelper::MakeLinBinning(7, -0.5, 6.5),
                           PairAnalysisVarManager::kTrdLayer);
    }
  }


  /// histograms
  histos->AddHistogram("Hit", PairAnalysisHelper::MakeLinBinning(200, 0., 5.e+1), PairAnalysisVarManager::kEloss);
  histos->AddHistogram("Hit",
                       PairAnalysisHelper::MakeArbitraryBinning("0.,0.25,0.5,1.,1.5,2.,3.,4.,5.,6.,7.,8.,9.,10.,20."),
                       PairAnalysisVarManager::kTrdPin, PairAnalysisHelper::MakeLinBinning(800, 0., 8.e+1),
                       PairAnalysisVarManager::kEloss);
  histos->AddHistogram(
    "Hit", PairAnalysisHelper::MakeArbitraryBinning("0.,0.25,0.5,1.,1.5,2.,3.,4.,5.,6.,7.,8.,9.,10.,20."),
    PairAnalysisVarManager::kP, PairAnalysisHelper::MakeLinBinning(800, 0., 8.e+1), PairAnalysisVarManager::kEloss);

  /// mc matching
  histos->AddHistogram("Hit", PairAnalysisHelper::MakeLinBinning(200, 0., 5.e+1), PairAnalysisVarManager::kElossMC);
  histos->AddHistogram("Hit", PairAnalysisHelper::MakeLogBinning(200, 0.1, 20.), PairAnalysisVarManager::kTrdPin,
                       PairAnalysisHelper::MakeLinBinning(300, 0., 3.e+1), PairAnalysisVarManager::kElossMC);

  histos->AddHistogram("Hit", PairAnalysisHelper::MakeLinBinning(400, -400., 400.), PairAnalysisVarManager::kPosX,
                       PairAnalysisHelper::MakeLinBinning(400, -400., 400.), PairAnalysisVarManager::kPosY);
}


//______________________________________________________________________________________
void AddCutStepHistograms(PairAnalysis* papa, Int_t cutDefinition)
{
  //
  // add QA histograms for each track cut applied in the analysis
  // NOTE: this is rather CPU expencive (at least for MCtruth) since
  //       there signal check are reperformed
  //

  /// skip certain configs
  // //if(cutDefinition!=kRichTRDTOFcfg) return;
  // if(cutDefinition!=kTrdcfg) return;

  PairAnalysisHistos* histos = new PairAnalysisHistos();

  /// add MC signal (if any) histograms to pair class
  if (papa->GetMCSignals()) {
    for (Int_t i = 0; i < papa->GetMCSignals()->GetEntriesFast(); ++i) {
      PairAnalysisSignalMC* sigMC = (PairAnalysisSignalMC*) papa->GetMCSignals()->At(i);

      /// selection
      if (!sigMC) continue;
      TString sigMCname = sigMC->GetName();
      //      if(!sigMCname.Contains("PrimEle") && !sigMCname.Contains("PrimPio")) continue; /// skip pair particle signals (no pairs)
      /// single tracks (merged +-)
      histos->AddClass(Form("Track.%s_%s",
                            PairAnalysis::PairClassName(static_cast<Int_t>(PairAnalysis::EPairType::kSEPM)),
                            sigMCname.Data()));
      //      histos->AddClass(Form("Track.Legs.%s_%s",PairAnalysis::PairClassName(static_cast<Int_t>(PairAnalysis::EPairType::kSEPM)),sigMCname.Data()));
      //      histos->AddClass(Form("Pair_%s",sigMCname.Data()));
      // if(sigMC->GetFillPureMCStep())
      // 	histos->AddClass(Form("Track.%s_%s_MCtruth",PairAnalysis::PairClassName(PairAnalysis::kSEPM),sigMCname.Data()));
    }
  }

  /// histograms
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(100, 0.0, TMath::Pi() / 4),
                       PairAnalysisVarManager::kTrdThetain);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLogBinning(400, 0, 20.), PairAnalysisVarManager::kP);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLogBinning(250, 0, 2.5), PairAnalysisVarManager::kM);

  papa->GetTrackFilter().AddHistos(histos);        //prefilter histograms
  papa->GetPairPreFilterLegs().AddHistos(histos);  //prefilter histograsm
  papa->GetFinalTrackFilter().AddHistos(histos);
}
