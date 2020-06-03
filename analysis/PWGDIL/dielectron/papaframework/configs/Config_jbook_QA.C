/// \file Config_jbook_Mee.C
/// \brief A template task configuration macro with example and explanations
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
TString names=("STS;TRDrec;RICHrec;TOFrec;TRD+TOF;TRD;RICH;RICH+TRD;RICH+TRD+TOF");
enum {
  kSTScfg,              /// acceptance study
  kTRDcfg,              /// TRD reconstruction
  kRICHcfg,             /// RICH reconstruction
  kTOFcfg,              /// TOF reconstruction
  kTRDTOFcfg,           /// TRD+TOF reconstruction
  kTRDpidcfg,           /// TRD reconstruction + pid
  kRICHpidcfg,          /// RICH reconstruction + pid
  kRICHTRDpidcfg,       /// TRD+RICH reconstruction + pid
  kRICHTRDTOFpidcfg     /// TRD+RICH+TOF reconstruction + pid
 };

////// OUTPUT
void InitHistograms(    PairAnalysis *papa, Int_t cutDefinition);
void AddEventHistograms(PairAnalysis *papa, Int_t cutDefinition);
void AddHitHistograms(  PairAnalysis *papa, Int_t cutDefinition);
void AddTrackHistograms(PairAnalysis *papa, Int_t cutDefinition);
void AddTrackHistogramsReconstruction(  PairAnalysisHistos *histos);
void AddTrackHistogramsMatching(        PairAnalysisHistos *histos);
void AddTrackHistogramsMCInfo(          PairAnalysisHistos *histos);
void AddTrackHistogramsMVDInfo(         PairAnalysisHistos *histos);
void AddTrackHistogramsSTSInfo(         PairAnalysisHistos *histos);
void AddTrackHistogramsRICHInfo(        PairAnalysisHistos *histos);
void AddTrackHistogramsTRDInfo(         PairAnalysisHistos *histos);
void AddTrackHistogramsTOFInfo(         PairAnalysisHistos *histos);
void AddTrackHistogramsCombinedPIDInfo( PairAnalysisHistos *histos);
void AddPairHistograms( PairAnalysis *papa, Int_t cutDefinition);
void AddPairHistogramsRejection( PairAnalysis *papa, Int_t cutDefinition);
void AddNoCutHistograms(PairAnalysis *papa, Int_t cutDefinition);
void InitHF(            PairAnalysis* papa, Int_t cutDefinition);
////// CUTS
void SetupEventCuts(   AnalysisTaskMultiPairAnalysis *task);
void SetupTrackCuts(   PairAnalysis *papa, Int_t cutDefinition);
void SetupPairCuts(    PairAnalysis *papa,  Int_t cutDefinition);
void SetupTrackCutsMC( PairAnalysis *papa, Int_t cutDefinition);
////// SETTINGS
void ConfigBgrd(       PairAnalysis *papa,  Int_t cutDefinition);
void AddMCSignals(     PairAnalysis *papa,  Int_t cutDefinition);
////// MISC
TObjArray *arrNames=names.Tokenize(";");
const Int_t nPapa=arrNames->GetEntries();

//______________________________________________________________________________________
AnalysisTaskMultiPairAnalysis *Config_jbook_QA(const char *taskname)
{
  ///
  /// creation of one multi task
  ///
  AnalysisTaskMultiPairAnalysis *task = new AnalysisTaskMultiPairAnalysis(taskname);
  task->SetBeamEnergy(8.); //TODO: get internally from FairBaseParSet::GetBeamMom()

  /// apply event cuts
  SetupEventCuts(task);

  /// add PAPA analysis with different cuts to the task
  for (Int_t i=0; i<nPapa; ++i) {

    switch(i) {
    case kSTScfg:
    case kRICHcfg:
    case kTRDcfg:
    case kTOFcfg:
    case kTRDTOFcfg:
    case kRICHpidcfg:
    case kTRDpidcfg:
    case kRICHTRDpidcfg:
    case kRICHTRDTOFpidcfg:
      Info(" Configuration of PAPa-subtask",((TObjString*)arrNames->At(i))->GetName());
      break;
    default:
      Warning(" ATTENTION: temporarily skipped PAPa-subtask",((TObjString*)arrNames->At(i))->GetName());
      continue;
      break;
    }

    /// load configuration
    PairAnalysis *papa=Config_Analysis(i);
    if(!papa) continue;

    /// add PAPA to the task
    task->AddPairAnalysis(papa);

    printf(" %s added\n",papa->GetName());
  }
  return task;

}

//______________________________________________________________________________________
PairAnalysis* Config_Analysis(Int_t cutDefinition)
{
  ///
  /// Setup the instance of PairAnalysis
  ///

  /// task name
  TString name=arrNames->At(cutDefinition)->GetName();
  printf(" Adding config %s \n",name.Data());

  /// init PairAnalysis
  PairAnalysis *papa = new PairAnalysis(Form("%s",name.Data()), Form("%s",name.Data()));
  papa->SetHasMC(kTRUE);  /// TODO: set automatically
  /// ~ type of analysis (leptonic, hadronic or semi-leptonic 2-particle decays are supported)
  papa->SetLegPdg(-11,+11); /// default: dielectron

  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv CUTS vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
  SetupTrackCuts(papa,cutDefinition);
  //  SetupPairCuts(papa,cutDefinition);
  //  SetupTrackCutsMC(papa,cutDefinition);

  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv MISC vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
  /// Monte Carlo Signals
  AddMCSignals(papa, cutDefinition);

  /// internal train usage
  // if(cutDefinition==kSTScfg)    papa->SetDontClearArrays(kTRUE);
  // else                          papa->SetEventProcess(kFALSE);

  /// background estimators
  //  ConfigBgrd(papa,cutDefinition);

  /// pairing settings
  //  if(cutDefinition==kTRDpidcfg)
  papa->SetNoPairing();   /// single particle analysis
 // if(cutDefinition==kGammaRejcfg) papa->SetProcessLS(kFALSE); /// skip LS calculation
 // if(cutDefinition==kSTScfg) papa->SetProcessLS(kFALSE); /// skip LS calculation

  /// TODO: prefilter settings
  //  papa->SetPreFilterAllSigns();
  //  papa->SetPreFilterUnlikeOnly();

  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv OUTPUT vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
  /// histogram setup (single TH1,2,3, TProfile,2,3, THn)
  InitHistograms(papa,cutDefinition);
  /// histogram grid setup (a grid of TH1,2,3, TProfile,2,3, THn)
  ///  InitHF(papa,cutDefinition);

  /// some simple cut QA for events, tracks, OS-pairs
  papa->SetCutQA();

  return papa;
}

//______________________________________________________________________________________
void SetupEventCuts(AnalysisTaskMultiPairAnalysis *task)
{
  ///
  /// Setup the event cuts
  ///

  /// Cut using (a formula based on) variables which are defined and described in PairAnalysisVarManager.h
  /// Cuts can be added by either excluding or including criteria (see PairAnalysisVarCuts.h)
  /// formula function strings are listed here: http://root.cern.ch/root/html/TFormula.html#TFormula:Analyze
  PairAnalysisVarCuts *eventCuts=new PairAnalysisVarCuts("vertex","vertex");
  //  eventCuts->AddCut(PairAnalysisVarManager::kNVtxContrib, 0.0, 800.);  /// inclusion cut
  eventCuts->AddCut(PairAnalysisVarManager::kImpactParam, 0.0, 13.5);
  eventCuts->AddCut("VtxChi/VtxNDF", 6., 1.e3, kTRUE);                 /// 'kTRUE': exclusion cut based on formula
  //  eventCuts->AddCut("abs(ZvPrim)", 0., 10.);                           /// example of TMath in formula-cuts
  eventCuts->Print();                                                  /// for debug purpose (recommended)

  /// add cuts to the event filter
  //  papa->GetEventFilter().AddCuts(eventCuts);
  task->SetEventFilter(eventCuts);

}

//______________________________________________________________________________________
void SetupTrackCuts(PairAnalysis *papa, Int_t cutDefinition)
{
  ///
  /// Setup the track cuts
  ///
  Bool_t hasMC=papa->GetHasMC();

  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv TRACK QUALITY CUTS vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
  /// setup a cut group, in which cut logic can be set (kCompAND, kCompOR)
  //  PairAnalysisCutGroup  *grpQualCuts = new PairAnalysisCutGroup("quality","quality",PairAnalysisCutGroup::kCompAND);

  /// mcPID cuts to reject heavy particle cocktail contributions
  PairAnalysisVarCuts   *mcPDGcuts = new PairAnalysisVarCuts("mcPDG","mcPDG");
  mcPDGcuts->SetCutType(PairAnalysisVarCuts::kAll); /// wheter 'kAll' or 'kAny' cut has to be fullfilled
  mcPDGcuts->AddCut(PairAnalysisVarManager::kPdgCode,      1000010020.-0.5,   1000010020.+0.5, kTRUE);
  mcPDGcuts->AddCut(PairAnalysisVarManager::kPdgCode,      1000010030.-0.5,   1000010030.+0.5, kTRUE);
  mcPDGcuts->AddCut(PairAnalysisVarManager::kPdgCode,      1000020030.-0.5,   1000020030.+0.5, kTRUE);
  mcPDGcuts->AddCut(PairAnalysisVarManager::kPdgCode,      1000020040.-0.5,   1000020040.+0.5, kTRUE);
  mcPDGcuts->AddCut(PairAnalysisVarManager::kPdgCodeMother,1000010020.-0.5,   1000010020.+0.5, kTRUE);
  mcPDGcuts->AddCut(PairAnalysisVarManager::kPdgCodeMother,1000010030.-0.5,   1000010030.+0.5, kTRUE);
  mcPDGcuts->AddCut(PairAnalysisVarManager::kPdgCodeMother,1000020030.-0.5,   1000020030.+0.5, kTRUE);
  mcPDGcuts->AddCut(PairAnalysisVarManager::kPdgCodeMother,1000020040.-0.5,   1000020040.+0.5, kTRUE);

  /// prefilter track cuts
  PairAnalysisVarCuts   *preCuts = new PairAnalysisVarCuts("preCuts","preCuts");
  preCuts->SetCutType(PairAnalysisVarCuts::kAll); /// wheter 'kAll' or 'kAny' cut has to be fullfilled
  // preCuts->AddCut(PairAnalysisVarManager::kMVDFirstHitPosZ, 0.,   7.5);   /// a hit in 1st MVD layer
  // preCuts->AddCut("MVDHits+STSHits",         3.,   15.);
  preCuts->AddCut(PairAnalysisVarManager::kPt,             0.075, 1e30);

  /// acceptance cuts (applied after pair pre filter)
  PairAnalysisVarCuts   *accCuts = new PairAnalysisVarCuts("acceptance","acceptance");
  accCuts->SetCutType(PairAnalysisVarCuts::kAll); /// wheter 'kAll' or 'kAny' cut has to be fullfilled
  //  accCuts->AddCut(PairAnalysisVarManager::kMVDHitsMC,      0.,   0.5, kTRUE); // MVD MC acceptance
  accCuts->AddCut(PairAnalysisVarManager::kSTSHitsMC,      0.,   0.5, kTRUE); // STS MC acceptance
  accCuts->AddCut(PairAnalysisVarManager::kTRDHitsMC,      0.,   0.5, kTRUE); // TRD MC acceptance
  accCuts->AddCut(PairAnalysisVarManager::kRICHhasProj,    0.,   0.5, kTRUE); // RICH MC acceptance
  accCuts->AddCut(PairAnalysisVarManager::kTOFHitsMC,      0.,   0.5, kTRUE); // TOF MC acceptance
  accCuts->AddCut(PairAnalysisVarManager::kPt,             0.2, 1e30);        // NOTE: was 0.2 GeV/c

  /// standard reconstruction cuts
  PairAnalysisVarCuts   *recSTS = new PairAnalysisVarCuts("recSTS","recSTS");
  recSTS->SetCutType(PairAnalysisVarCuts::kAll);
  recSTS->AddCut(PairAnalysisVarManager::kChi2NDFtoVtx,    0.,    3.);      /// tracks pointing to the primary vertex
  //recSTS->AddCut(PairAnalysisVarManager::kMVDFirstHitPosZ, 0.,   7.5);   /// a hit in first MVD layer
  recSTS->AddCut("MVDHits+STSHits",         6.,   15.);        /// min+max requieremnt for hits

  /// RICH reconstruction cuts
  PairAnalysisVarCuts   *recRICH = new PairAnalysisVarCuts("recRICH","recRICH");
  recRICH->SetCutType(PairAnalysisVarCuts::kAll);
  recRICH->AddCut(PairAnalysisVarManager::kRICHHits,       6.,   100.);       /// min+max inclusion for hits, 13=eff95%
  // valid ring parameters
  recRICH->AddCut(PairAnalysisVarManager::kRICHAxisA,      0.,   10.);
  recRICH->AddCut(PairAnalysisVarManager::kRICHAxisB,      0.,   10.);
  recRICH->AddCut(PairAnalysisVarManager::kRICHDistance,   0.,  999.);
  recRICH->AddCut(PairAnalysisVarManager::kRICHRadialPos,  0.,  999.);
  recRICH->AddCut(PairAnalysisVarManager::kRICHRadialAngle,-TMath::TwoPi(), TMath::TwoPi());
  recRICH->AddCut(PairAnalysisVarManager::kRICHPhi,        -TMath::TwoPi(), TMath::TwoPi());
  // ellipse fit not working (chi2/ndf is nan)
  recRICH->AddCut(PairAnalysisVarManager::kRICHChi2NDF,    0.,   100.);


  /// TRD reconstruction cuts
  PairAnalysisVarCuts   *recTRD = new PairAnalysisVarCuts("recTRD","recTRD");
  recTRD->SetCutType(PairAnalysisVarCuts::kAll);
  recTRD->AddCut(PairAnalysisVarManager::kTRDHits,         2.,   4.);         /// min+max requieremnt for hits

  /// TOF reconstruction cuts
  PairAnalysisVarCuts   *recTOF = new PairAnalysisVarCuts("recTOF","recTOF");
  recTOF->SetCutType(PairAnalysisVarCuts::kAll);
  recTOF->AddCut(PairAnalysisVarManager::kTOFHits,         1.,   10.);         /// min+max requieremnt for hits

  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv TRACK PID CUTS vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
  PairAnalysisCutGroup *grpPIDCut  = new PairAnalysisCutGroup("PID","PID",PairAnalysisCutGroup::kCompOR);

  /// TRD pid cuts - 2-dimensional
  Double_t x[68] = {0.100,0.457,0.537,0.603,0.661,0.708,0.759,0.794,0.832,0.871,0.912,0.955,1.000,1.023,1.047,1.072,1.096,1.122,1.148,1.175,1.202,1.230,1.259,1.288,1.318,1.349,1.380,1.413,1.445,1.479,1.514,1.549,1.585,1.622,1.660,1.698,1.738,1.778,1.820,1.862,1.905,1.950,1.995,2.042,2.089,2.138,2.188,2.239,2.291,2.344,2.399,2.455,2.512,2.570,2.630,2.754,2.884,3.020,3.162,3.311,3.467,3.715,3.981,4.365,4.898,5.754,7.413,999.000};
  Double_t y[68] = {-0.940,-0.940,-0.920,-0.920,-0.900,-0.900,-0.880,-0.880,-0.880,-0.860,-0.860,-0.840,-0.820,-0.800,-0.780,-0.760,-0.760,-0.740,-0.720,-0.720,-0.700,-0.700,-0.700,-0.680,-0.680,-0.680,-0.680,-0.660,-0.660,-0.640,-0.600,-0.560,-0.540,-0.520,-0.520,-0.500,-0.480,-0.480,-0.480,-0.480,-0.460,-0.440,-0.440,-0.380,-0.340,-0.320,-0.320,-0.300,-0.320,-0.280,-0.280,-0.300,-0.280,-0.260,-0.280,-0.280,-0.260,-0.260,-0.220,-0.220,-0.240,-0.240,-0.240,-0.220,-0.220,-0.300,-0.420,-0.420};

  TGraph *gr =new TGraph(68,x,y);
  gr->GetListOfFunctions()->Add(PairAnalysisHelper::GetFormula("varf","P"));
  TGraph *grMax =new TGraph(2);
  grMax->SetPoint(0,0.,999.);
  grMax->SetPoint(1,999.,999.);
  grMax->GetListOfFunctions()->Add(PairAnalysisHelper::GetFormula("varf","P"));

  PairAnalysisObjectCuts   *pidTRD2d = new PairAnalysisObjectCuts("pidTRD2d","pidTRD2d");
  pidTRD2d->SetCutType(PairAnalysisVarCuts::kAll); /// wheter kAll or kAny cut has to be fullfilled
  pidTRD2d->AddCut(PairAnalysisVarManager::kTRDPidANN, gr, grMax);


  /// RICH pid cuts - 2-dimensional
  Double_t xR[105] = {0.300,0.426,0.482,0.526,0.564,0.594,0.627,0.660,0.684,0.708,0.734,0.760,0.787,0.815,0.844,0.859,0.874,0.890,0.905,0.921,0.938,0.954,0.971,0.988,1.006,1.024,1.042,1.060,1.079,1.098,1.117,1.137,1.157,1.178,1.199,1.220,1.241,1.263,1.286,1.308,1.332,1.355,1.379,1.403,1.428,1.454,1.479,1.505,1.532,1.559,1.587,1.615,1.643,1.672,1.702,1.732,1.763,1.794,1.826,1.858,1.891,1.924,1.958,1.993,2.028,2.064,2.100,2.138,2.175,2.214,2.253,2.293,2.333,2.375,2.417,2.460,2.503,2.547,2.592,2.638,2.685,2.732,2.781,2.830,2.880,2.931,2.983,3.035,3.089,3.199,3.314,3.432,3.554,3.681,3.812,3.949,4.162,4.387,4.623,4.959,5.414,6.014,6.920,8.691,999.000};
  Double_t yR[105] = {-1.090,-1.050,-1.050,-1.030,-1.010,-1.010,-0.990,-0.990,-0.990,-0.970,-0.970,-0.950,-0.950,-0.930,-0.930,-0.930,-0.910,-0.910,-0.910,-0.910,-0.890,-0.870,-0.870,-0.870,-0.850,-0.850,-0.830,-0.830,-0.830,-0.810,-0.810,-0.790,-0.790,-0.750,-0.770,-0.750,-0.750,-0.730,-0.730,-0.730,-0.710,-0.710,-0.690,-0.690,-0.670,-0.670,-0.650,-0.650,-0.630,-0.610,-0.630,-0.610,-0.610,-0.590,-0.570,-0.570,-0.590,-0.570,-0.570,-0.550,-0.550,-0.530,-0.550,-0.530,-0.530,-0.530,-0.530,-0.510,-0.510,-0.510,-0.510,-0.490,-0.490,-0.490,-0.510,-0.490,-0.510,-0.490,-0.470,-0.490,-0.490,-0.490,-0.510,-0.470,-0.470,-0.490,-0.490,-0.470,-0.490,-0.470,-0.490,-0.510,-0.490,-0.510,-0.510,-0.530,-0.550,-0.570,-0.570,-0.610,-0.650,-0.650,-0.710,-0.790,-0.790};

  TGraph *grR =new TGraph(105,xR,yR);
  grR->GetListOfFunctions()->Add(PairAnalysisHelper::GetFormula("varf","P"));

  PairAnalysisObjectCuts   *pidRICH2d = new PairAnalysisObjectCuts("pidRICH2d","pidRICH2d");
  pidRICH2d->SetCutType(PairAnalysisVarCuts::kAll); /// wheter kAll or kAny cut has to be fullfilled
  pidRICH2d->AddCut(PairAnalysisVarManager::kRICHPidANN, grR, grMax);


  // TRD Pid - 1-dimensional
  PairAnalysisVarCuts  *pidTRD     = new PairAnalysisVarCuts("pidTRD","pidTRD");
  pidTRD->AddCut(PairAnalysisVarManager::kTRDPidANN,     -0.68,   10.); // -0.76
  //pidTRD->AddCut(PairAnalysisVarManager::kTRDPidLikeEL,      0.02,   10.);

  // RICH Pid
  PairAnalysisVarCuts  *pidRICH    = new PairAnalysisVarCuts("pidRICH","pidRICH");
  pidRICH->AddCut(PairAnalysisVarManager::kRICHPidANN,     0.23,   10.); //-0.95,   10.); //-0.77 = cnt 90% eleeff, -0.95 = min bias 90% eleeff

  // TOF Pid
  PairAnalysisVarCuts  *pidTOF    = new PairAnalysisVarCuts("pidTOF","pidTOF");
  pidTOF->AddCut(PairAnalysisVarManager::kTOFPidDeltaBetaEL,     -1.65*3.2e-03, +1.65*3.2e-03); //90%
  //  pidTOF->AddCut(PairAnalysisVarManager::kMassSq,     -0.2,   0.2);

  PairAnalysisVarCuts  *recRICHexcl    = new PairAnalysisVarCuts("recRICH","recRICH");
  //  recRICHexcl->AddCut(PairAnalysisVarManager::kRICHPidANN,     -0.8,   10., kTRUE);
  recRICHexcl->AddCut(PairAnalysisVarManager::kRICHHits,       6.,   100., kTRUE);       /// min+max  exclusion for hits


  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv TRACK PDG CUTS vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */

  // PDG electron + TRD selection
  PairAnalysisVarCuts   *selTRDel = new PairAnalysisVarCuts("selTRDel","selTRDel");
  selTRDel->SetCutType(PairAnalysisVarCuts::kAll);
  selTRDel->AddCut(PairAnalysisVarManager::kTRDHits,         3.,   4.);
  selTRDel->AddCut(   "abs(PdgCode)",                       11.,  11.);

  // PDG electron + TRD selection
  PairAnalysisVarCuts   *selTRDpi = new PairAnalysisVarCuts("selTRDpi","selTRDpi");
  selTRDpi->SetCutType(PairAnalysisVarCuts::kAll);
  selTRDpi->AddCut(PairAnalysisVarManager::kTRDHits,         3.,   4.);
  selTRDpi->AddCut(   "abs(PdgCode)",                      211.,  211.);

  // PDG electron exclusion + TRD selection
  PairAnalysisVarCuts   *selTRDnotel = new PairAnalysisVarCuts("selTRDnotel","selTRDnotel");
  selTRDnotel->SetCutType(PairAnalysisVarCuts::kAll);
  selTRDnotel->AddCut(PairAnalysisVarManager::kTRDHits,         3.,   4.);
  selTRDnotel->AddCut(   "abs(PdgCode)",                       11.,  11., kTRUE); //exclusion

  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv GRAPHICAL CUTS vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */

  /// default open limit cut
  /// NOTE: defined above
  // TGraph *grMax =new TGraph(2);
  // grMax->SetPoint(0,0.,999.);
  // grMax->SetPoint(1,999.,999.);
  // grMax->GetListOfFunctions()->Add(PairAnalysisHelper::GetFormula("varf","P"));

  /// TRD pid cuts - 2-dimensional - 10% efficiency for pions
  Double_t x10[66] = {0.100,0.661,0.724,0.776,0.813,0.851,0.891,0.933,0.977,1.023,1.047,1.072,1.096,1.122,1.148,1.175,1.202,1.230,1.259,1.288,1.318,1.349,1.380,1.413,1.445,1.479,1.514,1.549,1.585,1.622,1.660,1.698,1.738,1.778,1.820,1.862,1.905,1.950,1.995,2.042,2.089,2.138,2.188,2.239,2.291,2.344,2.399,2.455,2.512,2.570,2.630,2.692,2.754,2.884,3.020,3.162,3.311,3.467,3.631,3.802,4.074,4.365,4.786,5.370,6.761,999.000};
  Double_t y10[66] = {-0.140,-0.240,-0.260,-0.260,-0.260,-0.240,-0.240,-0.220,-0.220,-0.200,-0.200,-0.200,-0.200,-0.200,-0.200,-0.200,-0.200,-0.200,-0.200,-0.200,-0.200,-0.200,-0.200,-0.200,-0.200,-0.200,-0.200,-0.200,-0.200,-0.200,-0.200,-0.200,-0.200,-0.200,-0.180,-0.180,-0.180,-0.180,-0.180,-0.180,-0.180,-0.180,-0.180,-0.180,-0.160,-0.160,-0.160,-0.160,-0.160,-0.160,-0.140,-0.140,-0.140,-0.120,-0.120,-0.120,-0.100,-0.100,-0.080,-0.060,-0.060,-0.040,-0.020,0.020,0.100,0.100};
  TGraph *gr10 =new TGraph(66,x10,y10);
  gr10->GetListOfFunctions()->Add(PairAnalysisHelper::GetFormula("varf","P"));

  PairAnalysisObjectCuts   *pidTRD2dpi10 = new PairAnalysisObjectCuts("pidTRD2dpi10","pidTRD2dpi10");
  pidTRD2dpi10->SetCutType(PairAnalysisVarCuts::kAll); /// wheter kAll or kAny cut has to be fullfilled
  pidTRD2dpi10->AddCut(PairAnalysisVarManager::kTRDPidANN, gr10, grMax);

  /// TRD pid cuts - 2-dimensional - 4% efficiency for pions
  Double_t x25[66] = {0.100,0.661,0.724,0.776,0.813,0.851,0.891,0.933,0.977,1.023,1.047,1.072,1.096,1.122,1.148,1.175,1.202,1.230,1.259,1.288,1.318,1.349,1.380,1.413,1.445,1.479,1.514,1.549,1.585,1.622,1.660,1.698,1.738,1.778,1.820,1.862,1.905,1.950,1.995,2.042,2.089,2.138,2.188,2.239,2.291,2.344,2.399,2.455,2.512,2.570,2.630,2.692,2.754,2.884,3.020,3.162,3.311,3.467,3.631,3.802,4.074,4.365,4.786,5.370,6.761,999.000};
  Double_t y25[66] = {0.480,0.420,0.400,0.400,0.400,0.420,0.420,0.440,0.440,0.440,0.440,0.440,0.440,0.440,0.440,0.440,0.440,0.440,0.440,0.440,0.440,0.440,0.420,0.420,0.440,0.420,0.420,0.420,0.440,0.440,0.440,0.440,0.440,0.440,0.440,0.440,0.440,0.440,0.440,0.440,0.440,0.440,0.440,0.440,0.440,0.440,0.440,0.440,0.460,0.460,0.460,0.460,0.460,0.460,0.480,0.480,0.480,0.480,0.500,0.500,0.520,0.520,0.540,0.560,0.600,0.600};
  TGraph *gr25 =new TGraph(66,x25,y25);
  gr25->GetListOfFunctions()->Add(PairAnalysisHelper::GetFormula("varf","P"));

  PairAnalysisObjectCuts   *pidTRD2dpi25 = new PairAnalysisObjectCuts("pidTRD2dpi25","pidTRD2dpi25");
  pidTRD2dpi25->SetCutType(PairAnalysisVarCuts::kAll); /// wheter kAll or kAny cut has to be fullfilled
  pidTRD2dpi25->AddCut(PairAnalysisVarManager::kTRDPidANN, gr25, grMax);

  /// TRD pid cuts - 2-dimensional - 2% efficiency for pions
  Double_t x50[66] = {0.100,0.661,0.724,0.776,0.813,0.851,0.891,0.933,0.977,1.023,1.047,1.072,1.096,1.122,1.148,1.175,1.202,1.230,1.259,1.288,1.318,1.349,1.380,1.413,1.445,1.479,1.514,1.549,1.585,1.622,1.660,1.698,1.738,1.778,1.820,1.862,1.905,1.950,1.995,2.042,2.089,2.138,2.188,2.239,2.291,2.344,2.399,2.455,2.512,2.570,2.630,2.692,2.754,2.884,3.020,3.162,3.311,3.467,3.631,3.802,4.074,4.365,4.786,5.370,6.761,999.000};
  Double_t y50[66] = {0.780,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.760,0.760,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.740,0.760,0.760,0.760,0.760,0.760,0.760,0.780,0.780,0.800,0.800};
  TGraph *gr50 =new TGraph(66,x50,y50);
  gr50->GetListOfFunctions()->Add(PairAnalysisHelper::GetFormula("varf","P"));

  PairAnalysisObjectCuts   *pidTRD2dpi50 = new PairAnalysisObjectCuts("pidTRD2dpi50","pidTRD2dpi50");
  pidTRD2dpi50->SetCutType(PairAnalysisVarCuts::kAll); /// wheter kAll or kAny cut has to be fullfilled
  pidTRD2dpi50->AddCut(PairAnalysisVarManager::kTRDPidANN, gr50, grMax);

  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv COMBINATION CUTS vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */

  PairAnalysisCutCombi *pidTRDavai = new PairAnalysisCutCombi("TRDPidAvai","TRDPidAvai");
  pidTRDavai->AddCut(pidTRD, recTRD);

  PairAnalysisCutCombi *pidTRDman10 = new PairAnalysisCutCombi("TRDPidMan10","TRDPidMan10");
  pidTRDman10->AddCut(pidTRD2d,     selTRDel);    // std electron cut
  pidTRDman10->AddCut(pidTRD2dpi10, selTRDnotel); // fixed hadron cut

  PairAnalysisCutCombi *pidTRDman25 = new PairAnalysisCutCombi("TRDPidMan25","TRDPidMan25");
  pidTRDman25->AddCut(pidTRD2d,     selTRDel);    // std electron cut
  pidTRDman25->AddCut(pidTRD2dpi25, selTRDnotel); // fixed hadron cut

  PairAnalysisCutCombi *pidTRDman50 = new PairAnalysisCutCombi("TRDPidMan50","TRDPidMan50");
  pidTRDman50->AddCut(pidTRD2d,     selTRDel);    // std electron cut
  pidTRDman50->AddCut(pidTRD2dpi50, selTRDnotel); // fixed hadron cut

  PairAnalysisCutCombi *pidTOFavai = new PairAnalysisCutCombi("TOFPidAvai","TOFPidAvai");
  pidTOFavai->AddCut(pidTOF, recTOF);

  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv ACTIVATE CUTS vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
  /// activate the cut sets (order might be CPU timewise important)
  switch(cutDefinition) {
  case kSTScfg:
    //    papa->           GetTrackFilter().     AddCuts(preCuts);   preCuts   ->Print();
    papa->           GetFinalTrackFilter().AddCuts(accCuts);   accCuts   ->Print();
    papa->           GetFinalTrackFilter().AddCuts(recSTS);    recSTS    ->Print();
    break;
  case kTRDcfg:
    if(papa->DoEventProcess()) {
    papa->           GetFinalTrackFilter().AddCuts(accCuts);   accCuts   ->Print();
    papa->           GetFinalTrackFilter().AddCuts(recSTS);    recSTS    ->Print();
    }
    papa->           GetFinalTrackFilter().AddCuts(recTRD);    recTRD    ->Print();
    ///    papa->           GetFinalTrackFilter().AddCuts(pidTRD2d);  pidTRD2d    ->Print();
    /////    papa->           GetFinalTrackFilter().AddCuts(pidTRD);    pidTRD    ->Print();
    break;
  case kRICHcfg:
    if(papa->DoEventProcess()) {
    papa->           GetFinalTrackFilter().AddCuts(accCuts);   accCuts   ->Print();
    papa->           GetFinalTrackFilter().AddCuts(recSTS);    recSTS    ->Print();
    }
    papa->           GetFinalTrackFilter().AddCuts(recRICH);   recRICH   ->Print();
    break;
  case kTOFcfg:
    if(papa->DoEventProcess()) {
    papa->           GetFinalTrackFilter().AddCuts(accCuts);   accCuts   ->Print();
    papa->           GetFinalTrackFilter().AddCuts(recSTS);    recSTS    ->Print();
    }
    papa->           GetFinalTrackFilter().AddCuts(recTOF);    recTOF    ->Print();
    break;
  case kTRDTOFcfg:
    if(papa->DoEventProcess()) {
    papa->           GetFinalTrackFilter().AddCuts(accCuts);   accCuts   ->Print();
    papa->           GetFinalTrackFilter().AddCuts(recSTS);    recSTS    ->Print();
    }
    papa->           GetFinalTrackFilter().AddCuts(recTRD);    recTRD    ->Print();
    papa->           GetFinalTrackFilter().AddCuts(recTOF);    recTOF    ->Print();
    break;
  case kTRDpidcfg:
    if(papa->DoEventProcess()) {
    papa->           GetFinalTrackFilter().AddCuts(accCuts);   accCuts   ->Print();
    papa->           GetFinalTrackFilter().AddCuts(recSTS);    recSTS    ->Print();
    }
    papa->           GetFinalTrackFilter().AddCuts(recTRD);    recTRD    ->Print();
    papa->           GetFinalTrackFilter().AddCuts(pidTRD2d);  pidTRD2d  ->Print();
    break;
  case kRICHpidcfg:
    if(papa->DoEventProcess()) {
    papa->           GetFinalTrackFilter().AddCuts(accCuts);   accCuts   ->Print();
    papa->           GetFinalTrackFilter().AddCuts(recSTS);    recSTS    ->Print();
    }
    papa->           GetFinalTrackFilter().AddCuts(recRICH);   recRICH   ->Print();
    papa->           GetFinalTrackFilter().AddCuts(pidRICH2d); pidRICH2d ->Print();
    break;
  case kRICHTRDpidcfg:
    if(papa->DoEventProcess()) {
    papa->           GetFinalTrackFilter().AddCuts(accCuts);   accCuts   ->Print();
    papa->           GetFinalTrackFilter().AddCuts(recSTS);    recSTS    ->Print();
    }
    papa->           GetFinalTrackFilter().AddCuts(recRICH);   recRICH   ->Print();
    papa->           GetFinalTrackFilter().AddCuts(recTRD);    recTRD    ->Print();
    papa->           GetFinalTrackFilter().AddCuts(pidRICH2d); pidRICH2d ->Print();
    papa->           GetFinalTrackFilter().AddCuts(pidTRD2d);  pidTRD2d  ->Print();
    break;
  case kRICHTRDTOFpidcfg:
    if(papa->DoEventProcess()) {
    papa->           GetFinalTrackFilter().AddCuts(accCuts);   accCuts   ->Print();
    papa->           GetFinalTrackFilter().AddCuts(recSTS);    recSTS    ->Print();
    }
    papa->           GetFinalTrackFilter().AddCuts(recRICH);   recRICH   ->Print();
    papa->           GetFinalTrackFilter().AddCuts(recTRD);    recTRD    ->Print();
    papa->           GetFinalTrackFilter().AddCuts(pidRICH2d); pidRICH2d ->Print();
    papa->           GetFinalTrackFilter().AddCuts(pidTRD2d);  pidTRD2d  ->Print();
    papa->           GetFinalTrackFilter().AddCuts(recTOF);    recTOF    ->Print();
    papa->           GetFinalTrackFilter().AddCuts(pidTOF);    pidTOF    ->Print();
    break;
  }

}

//______________________________________________________________________________________
void SetupPairCuts(PairAnalysis *papa, Int_t cutDefinition)
{
  ///
  /// Setup the pair cuts
  ///

  /// skip pair cuts if no pairing is done
  if(cutDefinition!=kSTScfg)  return;
  //  if(!papa->DoEventProcess()) continue;

  /// MC
  Bool_t hasMC=papa->GetHasMC();
  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv PAIR PREFILTER CUTS vvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
  /// example of gamma rejection cuts as prefilter in order to remove tracks from the final

  PairAnalysisCutGroup *grpGammaRejCut  = new PairAnalysisCutGroup("GammaRej","GammaRej",PairAnalysisCutGroup::kCompAND);

  /// pairing based on pair informations
  PairAnalysisVarCuts *gammaCuts = new PairAnalysisVarCuts("GammaCuts","GammaCuts");
  //  gammaCuts->AddCut(PairAnalysisVarManager::kM,             0.,   0.02);
  // gammaCuts->AddCut(PairAnalysisVarManager::kOpeningAngle,  0.,   0.025);
  // gammaCuts->AddCut(PairAnalysisVarManager::kLegsP,         0.,   1.5);
  //  gammaCuts->AddCut("0.035-LegsP*0.01-OpeningAngle",         0.,   1.e30); // straight line cut
  gammaCuts->AddCut("exp(-1.37937-1.52985*LegsP)-OpeningAngle",  0.,   1.e30); // exponential cut


  /// graph cut
  Double_t x[89] = {0.625,0.675,0.725,0.775,0.825,0.875,0.925,0.975,1.025,1.075,1.125,1.175,1.225,1.275,1.325,1.375,1.425,1.475,1.525,1.575,1.625,1.675,1.725,1.775,1.825,1.875,1.925,1.975,2.025,2.075,2.125,2.175,2.225,2.275,2.325,2.375,2.425,2.475,2.525,2.575,2.625,2.675,2.725,2.775,2.825,2.875,2.925,2.975,3.025,3.075,3.125,3.175,3.225,3.275,3.325,3.375,3.425,3.475,3.525,3.575,3.625,3.675,3.725,3.775,3.825,3.875,3.925,3.975,4.025,4.075,4.125,4.175,4.225,4.275,4.325,4.375,4.425,4.475,4.525,4.575,4.625,4.675,4.725,4.775,4.825,4.875,4.925,4.975,999.000};
  Double_t y[89] = {0.046,0.040,0.041,0.033,0.031,0.031,0.029,0.028,0.027,0.026,0.025,0.021,0.021,0.022,0.019,0.020,0.019,0.019,0.018,0.017,0.017,0.017,0.017,0.016,0.016,0.015,0.015,0.014,0.014,0.014,0.014,0.013,0.013,0.013,0.013,0.011,0.012,0.012,0.012,0.011,0.011,0.011,0.011,0.010,0.010,0.010,0.010,0.010,0.010,0.010,0.009,0.010,0.009,0.009,0.009,0.009,0.009,0.009,0.009,0.009,0.008,0.008,0.009,0.008,0.008,0.008,0.008,0.008,0.008,0.008,0.008,0.007,0.008,0.007,0.008,0.009,0.008,0.007,0.008,0.007,0.010,0.007,0.008,0.007,0.008,0.007,0.006,0.007,0.007};
  TGraph *gr =new TGraph(89,x,y);
  gr->GetListOfFunctions()->Add(PairAnalysisHelper::GetFormula("varf","LegsP"));
  TGraph *grMax =new TGraph(2);
  grMax->SetPoint(0,0.,999.);
  grMax->SetPoint(1,999.,999.);
  grMax->GetListOfFunctions()->Add(PairAnalysisHelper::GetFormula("varf","LegsP"));

  PairAnalysisObjectCuts   *gammaRejGraph = new PairAnalysisObjectCuts("gammaRejGraph","gammaRejGraph");
  gammaRejGraph->SetCutType(PairAnalysisVarCuts::kAll); /// wheter kAll or kAny cut has to be fullfilled
  gammaRejGraph->AddCut(PairAnalysisVarManager::kOpeningAngle, gr, grMax);


  // PairAnalysisCutCombi *gammaCuts = new PairAnalysisCutCombi("GammaCuts","GammaCuts");
  // gammaCuts->AddCut(PairAnalysisVarManager::kM,  0., 0.06, kTRUE,
  // // 		    //		    PairAnalysisVarManager::kM,  0., 0.5);
  // PairAnalysisVarManager::kOpeningAngle,   0., 0.08);

  /// additional pair leg requirements
  PairAnalysisVarCuts *legcut = new PairAnalysisVarCuts("LegCut","LegCut");
  legcut->AddCut(PairAnalysisVarManager::kTRDHitsMC,      0.,   0.5, kTRUE); // TRD MC acceptance
  legcut->AddCut(PairAnalysisVarManager::kRICHhasProj,    0.,   0.5, kTRUE); // RICH MC acceptance
  legcut->AddCut(PairAnalysisVarManager::kPt,             0.2, 1e30);        // NOTE: was 0.2 GeV/c
  legcut->AddCut(PairAnalysisVarManager::kChi2NDFtoVtx,    0.,    3.);      /// tracks pointing to the primary vertex
  //  legcut->AddCut(PairAnalysisVarManager::kMVDFirstHitPosZ, 0.,   7.5);   /// a hit in first MVD layer
  legcut->AddCut(PairAnalysisVarManager::kRICHHits,       6.,   100.);       /// min+max  exclusion for hits, 13=eff95%
  legcut->AddCut(PairAnalysisVarManager::kRICHPidANN,     0.23,   10.); //-0.95,   10.); //-0.77 = cnt 90% eleeff, -0.95 = min bias 90% eleeff

  // PairAnalysisCutCombi *legcutTrdPIDavai = new PairAnalysisCutCombi("LegCutTRD","LegCutTRD");
  // legcutTrdPIDavai->AddCut(PairAnalysisVarManager::kTRDPidANN,  -0.68,   10., kFALSE,
  // 			   PairAnalysisVarManager::kTRDHits,     3.,     4.,  kFALSE);


  PairAnalysisPairLegCuts *gammaLegCuts = new PairAnalysisPairLegCuts("GammaLegCuts","GammaLegCuts");
  gammaLegCuts->SetCutType(PairAnalysisPairLegCuts::kAnyLeg); /// only one leg has to pass the cuts
  gammaLegCuts->GetLeg1Filter().AddCuts(legcut);
  //gammaLegCuts->GetLeg1Filter().AddCuts(legcutTrdPIDavai);
  gammaLegCuts->GetLeg2Filter().AddCuts(legcut);
  //gammaLegCuts->GetLeg2Filter().AddCuts(legcutTrdPIDavai);

  /// add to pair pre filter
  //  papa->           GetPairPreFilter().AddCuts(gammaCuts);      gammaCuts   ->Print();
  grpGammaRejCut ->AddCut(gammaCuts);
  //  grpGammaRejCut ->AddCut(gammaRejGraph); /// NOTE: graph cut cost cpu time
  grpGammaRejCut ->AddCut(gammaLegCuts);
  papa->           GetPairPreFilter().AddCuts(grpGammaRejCut);     grpGammaRejCut ->Print();
  // papa->GetPairPreFilter().AddCuts(gammaLegCuts);   gammaLegCuts->Print();


  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv PAIR  CUTS vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
  /// example of rapidity selection
  if(papa->IsNoPairing()) return;
  // PairAnalysisVarCuts *rapCut=new PairAnalysisVarCuts("RapidityCut","RapdityCut");
  // rapCut->AddCut(PairAnalysisVarManager::kY,0.0,1.0);
  // papa->GetPairFilter().AddCuts(rapCut);

}

//______________________________________________________________________________________
void SetupTrackCutsMC(PairAnalysis *papa, Int_t cutDefinition)
{
  ///
  /// Setup the track cuts based on MC information only
  ///
  if(!papa->GetHasMC()) return;

  /// NOTE: skip this
  return;

  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv TRACK CUTS ON MCtruth vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
  /// example of adding acceptance cuts
  PairAnalysisVarCuts   *varAccCutsMC = new PairAnalysisVarCuts("accCutMCtruth","accCutMCtruth");
  varAccCutsMC->SetCutType(PairAnalysisVarCuts::kAll); /// wheter kAll or kAny cut has to be fullfilled

  /// example for config specific cuts
  switch(cutDefinition) {
  case kSTScfg:
    //    varAccCutsMC->AddCut(PairAnalysisVarManager::kPtMC,           0.2, 1e30);
    //varAccCutsMC->AddCut(PairAnalysisVarManager::kTRDHitsMC,      0.,   0.5, kTRUE); // TRD MC acceptance
    //    varAccCutsMC->AddCut(PairAnalysisVarManager::kRICHhasProj,    0.,   0.5, kTRUE); // RICH MC acceptance
    break;
  case kTRDcfg:
  case kRICHcfg:
  case kRICHTRDpidcfg:
    varAccCutsMC->AddCut(PairAnalysisVarManager::kPtMC,           0.05, 1e30);
    // varAccCutsMC->AddCut(PairAnalysisVarManager::kEtaMC,         -0.8,   0.8);
    break;
  default:
    varAccCutsMC->AddCut(PairAnalysisVarManager::kPtMC,           0.4, 1e30);
    break;
  }

  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv ACTIVATE CUTS vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
  switch(cutDefinition) {
  default:
    papa->           GetTrackFilterMC().AddCuts(varAccCutsMC);    varAccCutsMC->Print();
  }

}

//______________________________________________________________________________________
void ConfigBgrd(PairAnalysis *papa, Int_t cutDefinition)
{
  ///
  /// Configurate the background estimators
  ///

  // skip configs
  if(!papa->DontClearArrays()) {
    /// skip config
    switch(cutDefinition) {
    case kSTScfg:
      //    case kRICHTRDTOFpidcfg:
    case kTRDcfg:
      papa->SetProcessLS(kFALSE); /// skip LS calculation
      // case kRICHcfg:
      //  case kRICHTRDpidcfg: /// only LS calculation
      //  case kRICHTRD10cfg: /// only LS calculation
      return; /// skip TrackRotation and Mixed Event
    }
  }

  /// Track Rotations
  /// Randomly rotate in phi one of the daughter tracks in-side a 'ConeAngle'
  /// for a given 'StartAngle'
  PairAnalysisTrackRotator *rot=new PairAnalysisTrackRotator;
  rot->SetIterations(5);              /// how often you want to rotate a track
  rot->SetConeAnglePhi(TMath::Pi());   /// cone angle
  rot->SetStartAnglePhi(TMath::Pi());  /// starting angle
  /// kRotatePositive, kRotateNegative or kRotateBothRandom (default)
  rot->SetRotationType(PairAnalysisTrackRotator::kRotateBothRandom);
  papa->SetTrackRotator(rot);

  /// Event mixing
  PairAnalysisMixingHandler *mix=new PairAnalysisMixingHandler;
  //  mix->AddVariable(PairAnalysisVarManager::kZvPrim, PairAnalysisHelper::MakeArbitraryBinning("-10.,-5.,-4.,-3.,-2.,-1.,1.,2.,3.,4.,5.,10.") );
  mix->AddVariable(PairAnalysisVarManager::kXvPrim, PairAnalysisHelper::MakeArbitraryBinning("-10.,-2.5.,-2.,-1.5,-1.,-0.5.,0.,0.5,1.,1.5,2.,2.5,10.") );
  mix->AddVariable(PairAnalysisVarManager::kYvPrim, PairAnalysisHelper::MakeArbitraryBinning("-10.,-2.5.,-2.,-1.5,-1.,-0.5.,0.,0.5,1.,1.5,2.,2.5,10.") );
  mix->AddVariable(PairAnalysisVarManager::kNTrk,   PairAnalysisHelper::MakeLinBinning(10,0.,500.) );
  //  mix->AddVariable(PairAnalysisVarManager::kNVtxContrib,  PairAnalysisHelper::MakeLinBinning(9,0.,450.) );
  // mix->AddVariable(PairAnalysisVarManager::kTPCrpH2,     10, TMath::Pi()/-2, TMath::Pi()/2);
  //  mix->SetMixType(PairAnalysisMixingHandler::kAll);   /// checkout PairAnalysisMixingHandler.h (LS ME, OS ME or both)
  mix->SetMixType(PairAnalysisMixingHandler::kOSonly);   /// checkout PairAnalysisMixingHandler.h (LS ME, OS ME or both)
  mix->SetDepth(5);                                 /// pool depth
  papa->SetMixingHandler(mix);

}

//______________________________________________________________________________________
void InitHistograms(PairAnalysis *papa, Int_t cutDefinition)
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

  Bool_t hasMC=papa->GetHasMC();

  ///Setup histogram Manager
  PairAnalysisHistos *histos=new PairAnalysisHistos(papa->GetName(),papa->GetTitle());
  papa->SetHistogramManager(histos);
  histos->SetPrecision(PairAnalysisHistos::kFloat);

  ///Initialise superior hisotogram classes
  histos->SetReservedWords("Hit;Track;Pair");

  ////// EVENT HISTOS /////
  AddEventHistograms(papa, cutDefinition);

  ////// PAIR HISTOS /////
  //  AddPairHistograms(papa, cutDefinition);

  ///// TRACK HISTOS /////
  AddTrackHistograms(papa, cutDefinition);

  ////// HIT HISTOS /////
  AddHitHistograms(  papa, cutDefinition);

  ////// QA HISTOS /////
  //  AddPairHistogramsRejection(papa, cutDefinition);
  /// NOTE: should come last, otherwise all histograms added above are filled
  AddNoCutHistograms(papa, cutDefinition);

  ////// DEBUG //////
  TIter nextClass(histos->GetHistogramList());
  THashList *l=0;
  while ( (l=static_cast<THashList*>(nextClass())) ) {
    printf(" [D] HistogramManger: Class %s: Histograms: %04d \n", l->GetName(), l->GetEntries());
  }

} //end: init histograms

//______________________________________________________________________________________
void InitHF(PairAnalysis* papa, Int_t cutDefinition)
{
  ///
  /// Setup the Histogram Framework (a matrix of histograms)
  ///

  /// container
  PairAnalysisHF *hf=new PairAnalysisHF(papa->GetName(),papa->GetTitle());
  papa->SetHistogramArray(hf);

  ////// Add Variables and Dimensions /////
  hf->AddCutVariable(PairAnalysisVarManager::kNTrk,        PairAnalysisHelper::MakeLinBinning(5,0.,500));
  hf->AddCutVariable(PairAnalysisVarManager::kNVtxContrib, PairAnalysisHelper::MakeArbitraryBinning("0.,1.,40.,150.,500."));

  ///Initialise superior histogram classes
  hf->SetReservedWords("Track;Pair");

  ////// EVENT HISTOS /////
  hf->AddClass("Event"); /// add histogram class
  /// define output objects
  hf->AddHistogram("Event",PairAnalysisHelper::MakeLinBinning(200,-1.,1.),  PairAnalysisVarManager::kXvPrim);
  hf->AddHistogram("Event",PairAnalysisHelper::MakeLinBinning(200,-1.,1.),  PairAnalysisVarManager::kYvPrim);
  hf->AddHistogram("Event",PairAnalysisHelper::MakeLinBinning(200,-1.,1.),  PairAnalysisVarManager::kZvPrim);

}

//______________________________________________________________________________________
void AddMCSignals(PairAnalysis *papa, Int_t cutDefinition){
  /// Do we have an MC handler?
  if (!papa->GetHasMC()) return;

  // switchers for signal activations
  Bool_t bSinglePart=kTRUE;
  Bool_t bHeavyFragm=kFALSE;
  Bool_t bLMVM      =kFALSE;
  Bool_t bRadiation =kFALSE;
  Bool_t bBgrdComb  =kFALSE;
  Bool_t bBgrdSig   =kFALSE;


  /// Predefined signals see PairAnalysisSignalMC::EDefinedSignal

  ///// single particle signals /////
  Bool_t fillMC=kFALSE;
  if(cutDefinition==kSTScfg) fillMC=kTRUE;
  PairAnalysisSignalMC* deltaele = new PairAnalysisSignalMC(PairAnalysisSignalMC::kDeltaElectron);
  deltaele->SetFillPureMCStep(fillMC);

  PairAnalysisSignalMC* eleGam = new PairAnalysisSignalMC("e^{#gamma}","eGam");
  eleGam->SetIsSingleParticle(kTRUE);
  eleGam->SetFillPureMCStep(fillMC);
  eleGam->SetLegPDGs(11,1);
  eleGam->SetCheckBothChargesLegs(kTRUE,kTRUE);
  eleGam->SetMotherPDGs(22,1); //0:default all
  //  eleGam->SetMothersRelation(PairAnalysisSignalMC::kDifferent);

  PairAnalysisSignalMC* ele = new PairAnalysisSignalMC(PairAnalysisSignalMC::kPrimElectron);
  ele->SetFillPureMCStep(fillMC);

  PairAnalysisSignalMC* muo = new PairAnalysisSignalMC(PairAnalysisSignalMC::kPrimMuon);
  muo->SetFillPureMCStep(fillMC);
  //  muo->SetGEANTProcess(kPPrimary, kFALSE); // do NOT check

  PairAnalysisSignalMC* pio = new PairAnalysisSignalMC(PairAnalysisSignalMC::kPrimPion);
  pio->SetFillPureMCStep(fillMC);

  PairAnalysisSignalMC* kao = new PairAnalysisSignalMC(PairAnalysisSignalMC::kPrimKaon);
  kao->SetFillPureMCStep(fillMC);

  PairAnalysisSignalMC* pro = new PairAnalysisSignalMC(PairAnalysisSignalMC::kPrimProton);
  pro->SetFillPureMCStep(fillMC);

  // heavy fragements
  fillMC=kFALSE;
  PairAnalysisSignalMC* deu = new PairAnalysisSignalMC(PairAnalysisSignalMC::kDeuteron);
  deu->SetFillPureMCStep(fillMC);
  deu->SetWeight(6.01e+00);//SHM

  PairAnalysisSignalMC* tri = new PairAnalysisSignalMC(PairAnalysisSignalMC::kTriton);
  tri->SetFillPureMCStep(fillMC);
  tri->SetWeight(2.33e-01);//SHM

  PairAnalysisSignalMC* he3 = new PairAnalysisSignalMC(PairAnalysisSignalMC::kHe3);
  he3->SetFillPureMCStep(fillMC);
  he3->SetWeight(2.33e-01);//SHM

  PairAnalysisSignalMC* alp = new PairAnalysisSignalMC(PairAnalysisSignalMC::kAlpha);
  alp->SetFillPureMCStep(fillMC);
  alp->SetWeight(7.47e-03);//SHM


  ////// low mass vector mesons ////
  fillMC=(cutDefinition==kSTScfg ? kTRUE : kFALSE);
  Bool_t bUseSHM  = kFALSE;
  Bool_t bUseBdep = kFALSE; //use impact parameter dependence (new HSD)
  Bool_t bUseMB   = kFALSE;  //use minimum bias value (new HSD)
  Int_t n = 31;
  Double_t br = 1.0; // branching ratio
  Double_t b[] = { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 
		   3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 
		   6.0, 6.5, 7.0, 7.5, 8.0, 8.5, 
		   9.0, 9.5, 10.0, 10.5, 11.0, 11.5, 
		   12.0, 12.5, 13.0, 13.5, 14.0, 14.5,
		   20.};

  // rho0
  PairAnalysisSignalMC* rho0 = new PairAnalysisSignalMC(PairAnalysisSignalMC::kRho0);
  rho0->SetFillPureMCStep(fillMC);
  br = 4.72e-05;
  Double_t rho0w[] = {
    4.90e+01*br, 4.90e+01*br, 4.61e+01*br, 4.46e+01*br, 4.39e+01*br,
    4.30e+01*br, 3.89e+01*br, 3.81e+01*br, 3.34e+01*br, 3.03e+01*br,
    2.78e+01*br, 2.64e+01*br, 2.38e+01*br, 2.03e+01*br, 1.84e+01*br,
    1.47e+01*br, 1.23e+01*br, 1.05e+01*br, 8.30e+00*br, 7.32e+00*br,
    6.32e+00*br, 4.18e+00*br, 3.32e+00*br, 2.44e+00*br, 1.28e+00*br,
    7.20e-01*br, 5.00e-01*br, 1.40e-01*br, 1.20e-01*br, 0.00e+00*br,
    0.0};
  if(bUseSHM)        rho0->SetWeight(2.03e+00 * br);//SHM
  else if(bUseMB)    rho0->SetWeight(0.10067E+02 * br);//HSD
  else if(!bUseBdep) rho0->SetWeight(9        * br);//HSD
  else               {
    TSpline3 *weight = new TSpline3("rho0wghts",b,rho0w,n);
    if(bUseBdep) rho0->SetWeight(weight, PairAnalysisVarManager::kImpactParam);
  }

  // omega
  PairAnalysisSignalMC* omega = new PairAnalysisSignalMC(PairAnalysisSignalMC::kOmega);
  omega->SetFillPureMCStep(fillMC);
  br = 7.28e-04;
  Double_t omegaw[] = {
    2.48e+01*br, 2.48e+01*br, 2.41e+01*br, 2.40e+01*br, 2.41e+01*br,
    2.20e+01*br, 2.02e+01*br, 1.91e+01*br, 1.75e+01*br, 1.69e+01*br,
    1.59e+01*br, 1.44e+01*br, 1.26e+01*br, 1.09e+01*br, 9.84e+00*br,
    7.22e+00*br, 7.40e+00*br, 6.24e+00*br, 5.54e+00*br, 4.02e+00*br,
    3.54e+00*br, 2.18e+00*br, 2.12e+00*br, 1.26e+00*br, 9.40e-01*br,
    4.40e-01*br, 3.60e-01*br, 2.40e-01*br, 4.00e-02*br, 2.00e-02*br,
    0.0};
  if(bUseSHM)        omega->SetWeight(1  * br);//SHM
  else if(bUseMB)    omega->SetWeight(0.55144E+01 * br);//HSD
  else if(!bUseBdep) omega->SetWeight(19 * br);//HSD
  else               {
    TSpline3 *weight = new TSpline3("omegawghts",b,omegaw,n);
    printf("omega weight at b=16fm: %f \n",weight->Eval(16.));
    if(bUseBdep) omega->SetWeight(weight, PairAnalysisVarManager::kImpactParam);
  }

  // omega dalitz decays
  PairAnalysisSignalMC* omegaDalitz = new PairAnalysisSignalMC(PairAnalysisSignalMC::kOmegaDalitz);
  omegaDalitz->SetFillPureMCStep(fillMC);
  br = 7.7e-04;
  Double_t omegaDalitzw[] = {
    2.48e+01*br, 2.48e+01*br, 2.41e+01*br, 2.40e+01*br, 2.41e+01*br,
    2.20e+01*br, 2.02e+01*br, 1.91e+01*br, 1.75e+01*br, 1.69e+01*br,
    1.59e+01*br, 1.44e+01*br, 1.26e+01*br, 1.09e+01*br, 9.84e+00*br,
    7.22e+00*br, 7.40e+00*br, 6.24e+00*br, 5.54e+00*br, 4.02e+00*br,
    3.54e+00*br, 2.18e+00*br, 2.12e+00*br, 1.26e+00*br, 9.40e-01*br,
    4.40e-01*br, 3.60e-01*br, 2.40e-01*br, 4.00e-02*br, 2.00e-02*br,
    0.0};
  if(bUseSHM)        omegaDalitz->SetWeight(1  * br);//SHM
  else if(bUseMB)    omegaDalitz->SetWeight(0.55144E+01 * br);//HSD
  else if(!bUseBdep) omegaDalitz->SetWeight(19 * br);//HSD
  else               {
    TSpline3 *weight = new TSpline3("omegaDalitzwghts",b,omegaDalitzw,n);
    if(bUseBdep) omegaDalitz->SetWeight(weight, PairAnalysisVarManager::kImpactParam);
  }

  // single electrons from decay: omega->e+e-gamma->pi0pi0->gamma->e+e-
  PairAnalysisSignalMC* eleGamPiOmega = new PairAnalysisSignalMC("eleGamPiOmega","eleGamPiOmega");
  eleGamPiOmega->SetFillPureMCStep(fillMC);
  eleGamPiOmega->SetIsSingleParticle(kTRUE);
  eleGamPiOmega->SetLegPDGs(-11,1);
  eleGamPiOmega->SetCheckBothChargesLegs(kTRUE,kTRUE);
  eleGamPiOmega->SetMotherPDGs(22,1);
  eleGamPiOmega->SetGrandMotherPDGs(111,111);
  eleGamPiOmega->SetGreatGrandMotherPDGs(223,223);
  if(bUseSHM)        eleGamPiOmega->SetWeight(1  * br);//SHM
  else if(bUseMB)    eleGamPiOmega->SetWeight(0.55144E+01 * br);//HSD
  else if(!bUseBdep) eleGamPiOmega->SetWeight(19 * br);//HSD
  else               {
    TSpline3 *weight = new TSpline3("eleGamPiOmegawghts",b,omegaDalitzw,n);
    if(bUseBdep) eleGamPiOmega->SetWeight(weight, PairAnalysisVarManager::kImpactParam);
  }

  // phi
  PairAnalysisSignalMC* phi = new PairAnalysisSignalMC(PairAnalysisSignalMC::kPhi);
  phi->SetFillPureMCStep(fillMC);
  br = 2.97e-04;
  Double_t phiw[] = {
    2.20e-01*br, 2.20e-01*br, 2.20e-01*br, 2.00e-01*br, 1.40e-01*br,
    2.20e-01*br, 1.80e-01*br, 8.00e-02*br, 1.20e-01*br, 1.40e-01*br,
    1.20e-01*br, 2.40e-01*br, 1.00e-01*br, 6.00e-02*br, 6.00e-02*br,
    6.00e-02*br, 1.40e-01*br, 4.00e-02*br, 8.00e-02*br, 4.00e-02*br,
    6.00e-02*br, 0.00e+00*br, 0.00e+00*br, 2.00e-02*br, 0.00e+00*br,
    0.00e+00*br, 2.00e-02*br, 4.00e-02*br, 0.00e+00*br, 0.00e+00*br,
    0.0};
  if(bUseSHM)        phi->SetWeight(1.31e-01  * br);//SHM
  else if(bUseMB)    phi->SetWeight(0.52230E-01 * br);//HSD
  else if(!bUseBdep) phi->SetWeight(0.12      * br);//HSD
  else               {
    TSpline3 *weight = new TSpline3("phiwghts",b,phiw,n);
    if(bUseBdep) phi->SetWeight(weight, PairAnalysisVarManager::kImpactParam);
  }

  /// UrQMD signals

  // eta
  PairAnalysisSignalMC* eta = new PairAnalysisSignalMC(PairAnalysisSignalMC::kEta);
  eta->SetFillPureMCStep(fillMC);
  eta->SetMotherPDGs(0,0); //0:default all
  eta->SetMothersRelation(PairAnalysisSignalMC::kSame);
  eta->SetGrandMotherPDGs(221,221);
  // eta->SetWeight(2.66e+00);

  // eta dalitz
  PairAnalysisSignalMC* etaDalitz = new PairAnalysisSignalMC(PairAnalysisSignalMC::kEtaDalitz);
  etaDalitz->SetFillPureMCStep(fillMC);
  etaDalitz->SetIsDalitz(PairAnalysisSignalMC::kWhoCares,0);
  // etaDalitz->SetWeight(2.66e+00);

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
  pi0Dalitz->SetIsDalitz(PairAnalysisSignalMC::kWhoCares,0);
  // pi0Dalitz->SetWeight(6.02e+01);//SHM


  ///// background combinatorics ////
  fillMC=kFALSE;
  PairAnalysisSignalMC* elepio = new PairAnalysisSignalMC("e#pi (comb.)","e#pi");
  elepio->SetFillPureMCStep(fillMC);
  elepio->SetLegPDGs(11,211);
  elepio->SetCheckBothChargesLegs(kTRUE,kTRUE);

  PairAnalysisSignalMC* piopio = new PairAnalysisSignalMC("#pi#pi (comb.)","#pi#pi");
  piopio->SetFillPureMCStep(fillMC);
  piopio->SetLegPDGs(211,211);
  piopio->SetCheckBothChargesLegs(kTRUE,kTRUE);

  PairAnalysisSignalMC* eleele = new PairAnalysisSignalMC("ee (comb.)","ee");
  eleele->SetFillPureMCStep(fillMC);
  eleele->SetLegPDGs(11,11);
  eleele->SetCheckBothChargesLegs(kTRUE,kTRUE);
  eleele->SetMothersRelation(PairAnalysisSignalMC::kDifferent);

  PairAnalysisSignalMC* eleGamX = new PairAnalysisSignalMC("e^{#gamma}X (comb.)","eX");
  eleGamX->SetFillPureMCStep(fillMC);
  eleGamX->SetLegPDGs(11,0);
  eleGamX->SetCheckBothChargesLegs(kTRUE,kTRUE);
  eleGamX->SetMotherPDGs(22,0); //0:default all
  eleGamX->SetMothersRelation(PairAnalysisSignalMC::kDifferent);

  PairAnalysisSignalMC* protonX = new PairAnalysisSignalMC("pX (comb.)","pX");
  protonX->SetFillPureMCStep(fillMC);
  protonX->SetLegPDGs(2212,0);
  protonX->SetCheckBothChargesLegs(kTRUE,kTRUE);


  ///// background signals ////
  fillMC=(cutDefinition==kSTScfg ? kTRUE : kFALSE);
  PairAnalysisSignalMC* conv = new PairAnalysisSignalMC(PairAnalysisSignalMC::kConversion);
  conv->SetFillPureMCStep(fillMC);


  ///// QGP and medium radiation /////
  fillMC=(cutDefinition==kSTScfg ? kTRUE : kFALSE);
  PairAnalysisSignalMC* inmed = new PairAnalysisSignalMC("in-medium SF","in-medium SF");
  inmed->SetFillPureMCStep(fillMC);
  inmed->SetLegPDGs(11,-11);
  inmed->SetCheckBothChargesLegs(kTRUE,kTRUE);
  inmed->SetMotherPDGs(99009011,99009011); //0:default all
  inmed->SetMothersRelation(PairAnalysisSignalMC::kSame);
  inmed->SetGEANTProcess(kPPrimary);
  br = 4.45e-02 / 2;
  Double_t inmedw[] = {
    1.00e+00*br, 9.87e-01*br, 9.69e-01*br, 9.43e-01*br, 9.09e-01*br,
    8.69e-01*br, 8.26e-01*br, 7.77e-01*br, 7.29e-01*br, 6.76e-01*br,
    6.25e-01*br, 5.73e-01*br, 5.20e-01*br, 4.70e-01*br, 4.19e-01*br,
    3.70e-01*br, 3.23e-01*br, 2.79e-01*br, 2.37e-01*br, 1.97e-01*br,
    1.61e-01*br, 1.28e-01*br, 9.91e-02*br, 7.41e-02*br, 5.29e-02*br,
    3.66e-02*br, 2.42e-02*br, 1.56e-02*br, 9.92e-03*br, 6.43e-03*br,
    4.49e-03*br};
  if(!bUseBdep) inmed->SetWeight(br);//default
  else               {
    TSpline3 *weight = new TSpline3("inmedwghts",b,inmedw,n);
    if(bUseBdep) inmed->SetWeight(weight, PairAnalysisVarManager::kImpactParam);
  }

  PairAnalysisSignalMC* qgp = new PairAnalysisSignalMC("QGP rad.","QGP rad.");
  qgp->SetFillPureMCStep(fillMC);
  qgp->SetLegPDGs(11,-11);
  qgp->SetCheckBothChargesLegs(kTRUE,kTRUE);
  qgp->SetMotherPDGs(99009111,99009111); //0:default all
  qgp->SetMothersRelation(PairAnalysisSignalMC::kSame);
  qgp->SetGEANTProcess(kPPrimary);
  br = 1.15e-02 / 2;
  Double_t qgpw[] = {
    1.00e+00*br, 9.87e-01*br, 9.69e-01*br, 9.43e-01*br, 9.09e-01*br,
    8.69e-01*br, 8.26e-01*br, 7.77e-01*br, 7.29e-01*br, 6.76e-01*br,
    6.25e-01*br, 5.73e-01*br, 5.20e-01*br, 4.70e-01*br, 4.19e-01*br,
    3.70e-01*br, 3.23e-01*br, 2.79e-01*br, 2.37e-01*br, 1.97e-01*br,
    1.61e-01*br, 1.28e-01*br, 9.91e-02*br, 7.41e-02*br, 5.29e-02*br,
    3.66e-02*br, 2.42e-02*br, 1.56e-02*br, 9.92e-03*br, 6.43e-03*br,
    4.49e-03*br};
  if(!bUseBdep) qgp->SetWeight(br);//default
  else               {
    TSpline3 *weight = new TSpline3("qgpwghts",b,qgpw,n);
    if(bUseBdep) qgp->SetWeight(weight, PairAnalysisVarManager::kImpactParam);
  }

  /*
  /// full mc stack
  PairAnalysisSignalMC* mc = new PairAnalysisSignalMC("gammaPi0","gammaPi0");
  //mc->SetIsSingleParticle(kTRUE);
  mc->SetLegPDGs(11,-11);
  mc->SetCheckBothChargesLegs(kTRUE,kTRUE);
  mc->SetMotherPDGs(22,22); //0:default all
  mc->SetMothersRelation(PairAnalysisSignalMC::kSame);
  // mc->SetMothersRelation(PairAnalysisSignalMC::kSame);
  mc->SetGrandMotherPDGs(111,111);
  // mc->SetCheckBothChargesLegs(kTRUE,kTRUE);
  //  mc->SetGEANTProcess(kPPrimary);
  mc->SetFillPureMCStep(kTRUE);

  PairAnalysisSignalMC* gromega = new PairAnalysisSignalMC(PairAnalysisSignalMC::kOmega);
  //  omega->SetFillPureMCStep(kTRUE);
  gromega->SetWeight(19 * 7.28e-04);//HSD //omega->SetWeight(1.00e+00 * 7.28e-04);//SHM
  PairAnalysisSignalMC* gromegaDalitz = new PairAnalysisSignalMC(PairAnalysisSignalMC::kOmegaDalitz);
  //  omegaDalitz->SetFillPureMCStep(kTRUE);
  gromegaDalitz->SetWeight(19 * 7.7e-04);//HSD  //omegaDalitz->SetWeight(1.00e+00 * 7.7e-04);//SHM
  PairAnalysisSignalMC* grphi = new PairAnalysisSignalMC(PairAnalysisSignalMC::kPhi);
  //  phi->SetFillPureMCStep(kTRUE);
  grphi->SetWeight(0.12 * 2.97e-4);//HSD  //phi->SetWeight(1.31e-01 * 2.97e-4);//SHM

  /// single particle signals
  // PairAnalysisSignalMC* ele = new PairAnalysisSignalMC(PairAnalysisSignalMC::kSingleInclEle);
  // ele->SetFillPureMCStep(kTRUE);

  /// Example of a MC signal configuration (see PairAnalysisSignalMC.h for full functionality)
  PairAnalysisSignalMC* all = new PairAnalysisSignalMC("EPEM","EPEM");
  all->SetLegPDGs(11,211);
  //  all->SetMotherPDGs(22,22,kTRUE,kTRUE);            /// requierement on the mother pdg code
  //  all->SetFillPureMCStep(kTRUE);                        /// whether to fill MC truth (generator level) output
  all->SetCheckBothChargesLegs(kTRUE,kTRUE);        /// check both daughter charge combinations
  //  all->SetCheckBothChargesMothers(kTRUE,kTRUE);     /// check both mother charge combinations
  //  all->SetGEANTProcess(kPPrimary);                  /// set an requirement on the Geant process (see root: TMCProcess.h)


  /// Example of a MC signal configuration (see PairAnalysisSignalMC.h for full functionality)
  PairAnalysisSignalMC* omdPiGamEle = new PairAnalysisSignalMC("omdPiGamEle","omdPiGamEle");
  omdPiGamEle->SetIsSingleParticle(kTRUE);
  omdPiGamEle->SetFillPureMCStep(kTRUE);                        /// whether to fill MC truth (generator level) output
  omdPiGamEle->SetWeight(19 * 7.7e-04);//HSD  //omegaDalitz->SetWeight(1.00e+00 * 7.7e-04);//SHM
  omdPiGamEle->SetCheckBothChargesLegs(kTRUE,kTRUE);        /// check both daughter charge combinations
  omdPiGamEle->SetLegPDGs(-11,1);
  omdPiGamEle->SetMotherPDGs(22,1);            /// requierement on the mother pdg code
  omdPiGamEle->SetCheckBothChargesMothers(kTRUE,kTRUE);     /// check both mother charge combinations
  // omdPiGamEle->SetMothersRelation(PairAnalysisSignalMC::kSame);
  omdPiGamEle->SetGrandMotherPDGs(111,111);
  omdPiGamEle->SetGreatGrandMotherPDGs(223,223);
  //  omdPiGamEle->SetGEANTProcess(kPPrimary);                  /// set an requirement on the Geant process (see root: TMCProcess.h)
*/


  /// activate mc signal
  switch(cutDefinition) {
    /*
  case kTRDpidcfg:
    // papa->AddSignalMC(ele);
    // papa->AddSignalMC(muo);
    // papa->AddSignalMC(pio);
    // papa->AddSignalMC(kao);
    // papa->AddSignalMC(pro);
    papa->AddSignalMC(deu);
    papa->AddSignalMC(tri);
    papa->AddSignalMC(he3);
    papa->AddSignalMC(alp);
    break;
    */
  default:
    //    papa->AddSignalMC(all);
    //    papa->AddSignalMC(mc);

    /// Single particles
    if(bSinglePart) {
      papa->AddSignalMC(ele);
      papa->AddSignalMC(pio);
      ///      papa->AddSignalMC(eleGam);
      papa->AddSignalMC(kao);
      papa->AddSignalMC(pro);
      //      papa->AddSignalMC(muo);
      // papa->AddSignalMC(deltaele);
    }
    //papa->AddSignalMC(eleGamPiOmega);

    /// Heavy fragments
    if(bHeavyFragm) {
      papa->AddSignalMC(deu);
      papa->AddSignalMC(tri);
      papa->AddSignalMC(he3);
      papa->AddSignalMC(alp);
    }

    /// Background - combinatorics
    if(bBgrdComb) {
      papa->AddSignalMC(elepio);
      papa->AddSignalMC(piopio);
      papa->AddSignalMC(eleele);
      papa->AddSignalMC(eleGamX);
      papa->AddSignalMC(protonX);
    }

    /// Background - signals
    if(bBgrdSig) {
      papa->AddSignalMC(conv);
    }

    /// LMVM
    if(bLMVM) {
      papa->AddSignalMC(eleGamPiOmega);
      //    papa->AddSignalMC(pi0);
      //    papa->AddSignalMC(pi0Gamma);
      papa->AddSignalMC(pi0Dalitz);   pi0Dalitz->Print();
      //    papa->AddSignalMC(eta);   eta->Print();
      papa->AddSignalMC(etaDalitz);   etaDalitz->Print();
      papa->AddSignalMC(rho0);        rho0->Print();
      papa->AddSignalMC(omega);       omega->Print();
      papa->AddSignalMC(omegaDalitz); omegaDalitz->Print();
      papa->AddSignalMC(phi);         phi->Print();
    }

    /// Radiations
    if(bRadiation) {
      papa->AddSignalMC(inmed);
      papa->AddSignalMC(qgp);
    }
  }

}

//______________________________________________________________________________________
void AddEventHistograms(PairAnalysis *papa, Int_t cutDefinition)
{
  //
  // add event histograms
  //

  if(cutDefinition!=kSTScfg) return;

  PairAnalysisHistos *histos = papa->GetHistoManager();
  histos->AddClass("Event"); /// add histogram class

  /// define output objects
  histos->AddHistogram("Event",PairAnalysisHelper::MakeLinBinning(200,-1.,1.),  PairAnalysisVarManager::kXvPrim);
  histos->AddHistogram("Event",PairAnalysisHelper::MakeLinBinning(200,-1.,1.),  PairAnalysisVarManager::kYvPrim);
  histos->AddHistogram("Event",PairAnalysisHelper::MakeLinBinning(200,-1.,1.),  PairAnalysisVarManager::kZvPrim);
  histos->AddHistogram("Event",PairAnalysisHelper::MakeLinBinning(250,-2.5,2.5),  PairAnalysisVarManager::kXvPrim,
		       PairAnalysisHelper::MakeLinBinning(250,-2.5,2.5),  PairAnalysisVarManager::kYvPrim);
  histos->AddHistogram("Event",PairAnalysisHelper::MakeLinBinning(100,0.,400.), PairAnalysisVarManager::kNVtxContrib);
  histos->AddHistogram("Event",PairAnalysisHelper::MakeLinBinning(200,0.0,10.0), "VtxChi/VtxNDF");
  histos->AddHistogram("Event",PairAnalysisHelper::MakeLinBinning(100,0.,100.), PairAnalysisVarManager::kVageMatches);
  histos->AddHistogram("Event",PairAnalysisHelper::MakeLinBinning(100,0.,500.), PairAnalysisVarManager::kSTSMatches);
  histos->AddHistogram("Event",PairAnalysisHelper::MakeLinBinning(100,0.,500.), PairAnalysisVarManager::kTRDMatches);
  histos->AddHistogram("Event",PairAnalysisHelper::MakeLinBinning(100,0.,500.), PairAnalysisVarManager::kNTrk);
  histos->AddHistogram("Event",PairAnalysisHelper::MakeLinBinning(100,0.,5000.),PairAnalysisVarManager::kNTrkMC);
  histos->AddHistogram("Event",PairAnalysisHelper::MakeLinBinning(100,0.,1000.),PairAnalysisVarManager::kTotalTRDHits);
  histos->AddHistogram("Event",PairAnalysisHelper::MakeLinBinning(100,0.,1000.),PairAnalysisVarManager::kTotalTRDHitsMC);
  histos->AddHistogram("Event",PairAnalysisHelper::MakeLinBinning(80,0.,20.),PairAnalysisVarManager::kImpactParam);
  histos->AddHistogram("Event",PairAnalysisHelper::MakeLinBinning(100,0.,20.),PairAnalysisVarManager::kImpactParam,
		       PairAnalysisHelper::MakeLinBinning(100,0.,2000.),PairAnalysisVarManager::kNPrimMC);
  // prefilter rejection
  if(papa->GetPairPreFilter().GetCuts()->GetEntries()) {
    histos->AddProfile("Event",PairAnalysisHelper::MakeLinBinning(100,0.,400.), PairAnalysisVarManager::kNVtxContrib,
		       PairAnalysisVarManager::kRndmRej,"I");
  }

  // mixing statistics
  Int_t mixBins=0;
  if(papa->GetMixingHandler()) {
    mixBins = papa->GetMixingHandler()->GetNumberOfBins();
    histos->AddHistogram("Event",PairAnalysisHelper::MakeLinBinning(mixBins,0.,mixBins), PairAnalysisVarManager::kMixingBin);
  }

}

//______________________________________________________________________________________
void AddHitHistograms(  PairAnalysis *papa, Int_t cutDefinition)
{
  //
  // add hit histograms
  //

  // skip histograms in case of internal train
  if(!papa->DoEventProcess()) return;

  // skip configs
  if(cutDefinition!=kSTScfg /*&& cutDefinition!=kTRDcfg*/) return;

  PairAnalysisHistos *histos = papa->GetHistoManager();
  /// add histogram class
  //  histos->AddClass("Hit.MVD");
  //  histos->AddClass("Hit.STS");
  //  histos->AddClass("Hit.STS_MCtruth");

  histos->AddClass("Hit.RICH");
  histos->AddClass("Hit.TOF");
  histos->AddClass("Hit.TRD");
  histos->AddClass("Hit.TRD_true");
  histos->AddClass("Hit.TRD_fake");
  histos->AddClass("Hit.TRD_dist");
  /// add MC signal (if any) histograms to hit class
  if(papa->GetMCSignals()) {
    for (Int_t i=0; i<papa->GetMCSignals()->GetEntriesFast(); ++i) {
      PairAnalysisSignalMC *sigMC = (PairAnalysisSignalMC*)papa->GetMCSignals()->At(i);
      if(!sigMC) continue;
      if(!sigMC->IsSingleParticle()) continue; /// skip pair particle signals (no pairs)
      TString sigMCname = sigMC->GetName();
      /// by hand switched off
      if(sigMCname.EqualTo("eleGamPiOmega")) continue;

      /// single tracks (merged +-)
      histos->AddClass(Form("Hit.TRD_%s",sigMCname.Data()));
      histos->AddHistogram(Form("Hit.TRD_%s",sigMCname.Data()), PairAnalysisHelper::MakeLinBinning(200,  0.,5.e+1),  PairAnalysisVarManager::kEloss);
      histos->AddHistogram(Form("Hit.TRD_%s",sigMCname.Data()), PairAnalysisHelper::MakeLinBinning(200,  0.,5.e+1),  PairAnalysisVarManager::kElossdEdx);
      histos->AddHistogram(Form("Hit.TRD_%s",sigMCname.Data()), PairAnalysisHelper::MakeLinBinning(200,  0.,5.e+1),  PairAnalysisVarManager::kElossTR);
      histos->AddHistogram(Form("Hit.TRD_%s",sigMCname.Data()), PairAnalysisHelper::MakeArbitraryBinning("0.,0.25,0.5,1.,1.5,2.,3.,4.,5.,6.,7.,8.,9.,10."),
			   PairAnalysisVarManager::kTRDPin,  PairAnalysisHelper::MakeLinBinning(200,0., 5.e+1), PairAnalysisVarManager::kEloss);
      histos->AddProfile(Form("Hit.TRD_%s",sigMCname.Data()), PairAnalysisHelper::MakeArbitraryBinning("0.,0.25,0.5,1.,1.5,2.,3.,4.,5.,6.,7.,8.,9.,10."),
			 PairAnalysisVarManager::kTRDPin, PairAnalysisVarManager::kEloss,"I");
      histos->AddProfile(Form("Hit.TRD_%s",sigMCname.Data()), PairAnalysisHelper::MakeArbitraryBinning("0.,0.25,0.5,1.,1.5,2.,3.,4.,5.,6.,7.,8.,9.,10."),
			 PairAnalysisVarManager::kTRDPin, PairAnalysisVarManager::kElossdEdx,"I");
      histos->AddProfile(Form("Hit.TRD_%s",sigMCname.Data()), PairAnalysisHelper::MakeArbitraryBinning("0.,0.25,0.5,1.,1.5,2.,3.,4.,5.,6.,7.,8.,9.,10."),
			 PairAnalysisVarManager::kTRDPin, PairAnalysisVarManager::kEloss,"S");
      histos->AddProfile(Form("Hit.TRD_%s",sigMCname.Data()), PairAnalysisHelper::MakeArbitraryBinning("0.,0.25,0.5,1.,1.5,2.,3.,4.,5.,6.,7.,8.,9.,10."),
			 PairAnalysisVarManager::kTRDPin, PairAnalysisVarManager::kElossdEdx,"S");
      histos->AddProfile(Form("Hit.TRD_%s",sigMCname.Data()), PairAnalysisHelper::MakeArbitraryBinning("0.,0.25,0.5,1.,1.5,2.,3.,4.,5.,6.,7.,8.,9.,10."),
			 PairAnalysisVarManager::kTRDPin, PairAnalysisVarManager::kElossTR,"I");

    }
  }
  /// define common output objects
  histos->AddHistogram("Hit",PairAnalysisHelper::MakeLinBinning(400,-400.,400.),  PairAnalysisVarManager::kPosX,
		       PairAnalysisHelper::MakeLinBinning(400,-400.,400.),  PairAnalysisVarManager::kPosY);
  histos->AddHistogram("Hit",PairAnalysisHelper::MakeLinBinning(400,-400.,400.),  PairAnalysisVarManager::kPosX);
  histos->AddHistogram("Hit",PairAnalysisHelper::MakeLinBinning(300,-300.,300.),  PairAnalysisVarManager::kPosY);
  histos->AddHistogram("Hit",PairAnalysisHelper::MakeLinBinning(325, 0.,650.),  PairAnalysisVarManager::kPosZ);

   // histos->AddProfile("Hit",PairAnalysisHelper::MakeLinBinning(400,-4.,4.),  PairAnalysisVarManager::kPosX,
   // 		     PairAnalysisHelper::MakeLinBinning(400,-4.,4.),  PairAnalysisVarManager::kPosY,
   // 		      PairAnalysisVarManager::kPosZ,"I;7.5;12.5");

  // mc point information
  histos->AddHistogram("Hit",PairAnalysisHelper::MakeLinBinning(7,-0.5,6.5),  PairAnalysisVarManager::kLinksMC);
  // histos->AddHistogram("Hit", PairAnalysisHelper::MakeLinBinning(200,  0.,5.e+1),  PairAnalysisVarManager::kElossMC);
  // histos->AddHistogram("Hit",PairAnalysisHelper::MakeLinBinning(7,-0.5,6.5),  PairAnalysisVarManager::kLinksMC);
  // histos->AddHistogram("Hit",PairAnalysisHelper::MakeLinBinning(400,-400.,400.),  PairAnalysisVarManager::kPosXMC,
  // 		       PairAnalysisHelper::MakeLinBinning(400,-400.,400.),  PairAnalysisVarManager::kPosYMC);
  // histos->AddHistogram("Hit",PairAnalysisHelper::MakeLinBinning(400,-400.,400.),  PairAnalysisVarManager::kPosXMC);
  // histos->AddHistogram("Hit",PairAnalysisHelper::MakeLinBinning(300,-300.,300.),  PairAnalysisVarManager::kPosYMC);
  // histos->AddHistogram("Hit",PairAnalysisHelper::MakeLinBinning(325, 0.,650.),  PairAnalysisVarManager::kPosZMC);

  /// pulls
  histos->AddHistogram("Hit",PairAnalysisHelper::MakeLinBinning(200,-40.,40.),  "PosXMC/LinksMC-PosX");
  histos->AddHistogram("Hit",PairAnalysisHelper::MakeLinBinning(200,-40.,40.),  "PosYMC/LinksMC-PosY");

  // TRD histograms
  histos->AddHistogram("Hit.TRD",PairAnalysisHelper::MakeLinBinning(16,-0.5,15.5),  PairAnalysisVarManager::kTRDPads);
  histos->AddHistogram("Hit.TRD",PairAnalysisHelper::MakeLinBinning(6,-0.5,5.5),  PairAnalysisVarManager::kTRDLayer);
  histos->AddHistogram("Hit.TRD", PairAnalysisHelper::MakeLinBinning(200,  0.,5.e+1),  PairAnalysisVarManager::kEloss);
  histos->AddProfile("Hit.TRD", PairAnalysisHelper::MakeLinBinning(100, 0.0,TMath::Pi()/4),
		     PairAnalysisVarManager::kTRDThetain, PairAnalysisVarManager::kEloss,"I");
  histos->AddProfile("Hit.TRD",PairAnalysisHelper::MakeLinBinning(200,0,TMath::TwoPi()),
		     PairAnalysisVarManager::kTRDPhiin, PairAnalysisVarManager::kEloss,"I");
  histos->AddProfile("Hit.TRD", PairAnalysisHelper::MakeArbitraryBinning("0.,0.25,0.5,1.,1.5,2.,3.,4.,5.,6.,7.,8.,9.,10."),
		     PairAnalysisVarManager::kTRDPin, PairAnalysisVarManager::kEloss,"I");
  histos->AddHistogram("Hit.TRD",PairAnalysisHelper::MakeLinBinning(105,-1.e+2,5.),  "ElossdEdx-ElossMC");
  histos->AddHistogram("Hit.TRD_true",PairAnalysisHelper::MakeLinBinning(105,-1.e+2,5.),  "ElossdEdx-ElossMC");
  histos->AddHistogram("Hit.TRD_dist",PairAnalysisHelper::MakeLinBinning(105,-1.e+2,5.),  "ElossdEdx-ElossMC");
  histos->AddHistogram("Hit.TRD_fake",PairAnalysisHelper::MakeLinBinning(105,-1.e+2,5.),  "ElossdEdx-ElossMC");
  histos->AddHistogram("Hit.TRD_true", PairAnalysisHelper::MakeLinBinning(6,-0.5,5.5),  PairAnalysisVarManager::kTRDLayer,
		       PairAnalysisHelper::MakeLinBinning(200,-40.,40.),  "PosXMC/LinksMC-PosX");
  histos->AddHistogram("Hit.TRD_true", PairAnalysisHelper::MakeLinBinning(6,-0.5,5.5),  PairAnalysisVarManager::kTRDLayer,
		       PairAnalysisHelper::MakeLinBinning(200,-40.,40.),  "PosYMC/LinksMC-PosY");
  // histos->AddHistogram("Hit.TRD_true",PairAnalysisHelper::MakeLogBinning(200,0.1,15.), PairAnalysisVarManager::kTRDPin,
  // 		       PairAnalysisHelper::MakeLinBinning(200,-40.,40.),  "PosXMC/LinksMC-PosX",
  // 		       PairAnalysisHelper::MakeLinBinning(6,-0.5,5.5),  PairAnalysisVarManager::kTRDLayer);
  // histos->AddHistogram("Hit.TRD_true",PairAnalysisHelper::MakeLogBinning(200,0.1,15.), PairAnalysisVarManager::kTRDPin,
  // 		       PairAnalysisHelper::MakeLinBinning(200,-40.,40.),  "PosYMC/LinksMC-PosY",
  // 		       PairAnalysisHelper::MakeLinBinning(6,-0.5,5.5),  PairAnalysisVarManager::kTRDLayer);

  /// add multidimensional histogram
  TObjArray *limits  = new TObjArray(); // array of bin limits
  limits->Add(PairAnalysisHelper::MakeLinBinning(300,-3.,3.));
  limits->Add(PairAnalysisHelper::MakeLinBinning(300,-3.,3.));
  limits->Add(PairAnalysisHelper::MakeLinBinning(13,-364.,364.));
  limits->Add(PairAnalysisHelper::MakeLinBinning(13,-364.,364.));
  limits->Add(PairAnalysisHelper::MakeArbitraryBinning("0.,0.25,0.5,1.,1.5,2.,3.,4.,5.,6.,7.,8.,9.,10."));
  limits->Add(PairAnalysisHelper::MakeLinBinning(50, 0.0,TMath::Pi()/4));
  limits->Add(PairAnalysisHelper::MakeLinBinning(2,-0.5,1.5));
  limits->Add(PairAnalysisHelper::MakeLinBinning(15,0.,15.));
  limits->Add(PairAnalysisHelper::MakeLinBinning(15,0.,15.));
  limits->Add(PairAnalysisHelper::MakeLinBinning(15,0.,15.));
  TFormula *varf[10];
  varf[0] = PairAnalysisHelper::GetFormula("form0","PosXMC/LinksMC-PosX");
  varf[1] = PairAnalysisHelper::GetFormula("form1","PosYMC/LinksMC-PosY");
  varf[2] = PairAnalysisHelper::GetFormula("form2","PosX");
  varf[3] = PairAnalysisHelper::GetFormula("form3","PosY");
  varf[4] = PairAnalysisHelper::GetFormula("form4","P");
  varf[5] = PairAnalysisHelper::GetFormula("form5","TRDThetain");
  varf[6] = PairAnalysisHelper::GetFormula("form6","TRDLayer%2"); // use modulo to merge layers
  varf[7] = PairAnalysisHelper::GetFormula("form7","TRDPads");
  varf[8] = PairAnalysisHelper::GetFormula("form8","TRDCols");
  varf[9] = PairAnalysisHelper::GetFormula("form9","TRDRows");
  //  histos->AddSparse("Hit.TRD_true", limits->GetEntriesFast(), limits, varf);

}

//______________________________________________________________________________________
void AddTrackHistograms( PairAnalysis *papa, Int_t cutDefinition)
{
  //
  // add track histograms
  //

  /// skip histograms in case of internal train
  if(!papa->DoEventProcess()) return;

  /// skip certain configs
  //  if(cutDefinition==kGammaRejcfg) return;

  PairAnalysisHistos *histos = papa->GetHistoManager();

  /// Add track classes - LEGS of the pairs
  if(!papa->IsNoPairing()) {
    /// loop over all pair types and add classes (pair types in PairAnalysis.h EPairType)
    /// automatically skip pair types w.r.t. configured bgrd estimators
    for (Int_t i=0; i<PairAnalysis::kPairTypes; i++){
      if(!papa->DoProcessLS()      && (i==PairAnalysis::kSEPP || i==PairAnalysis::kSEMM) ) continue; /// check SE LS
      if(!papa->GetMixingHandler() && (i>=PairAnalysis::kMEPP && i<=PairAnalysis::kMEMM) ) continue; /// check ME OS,LS
      if(!papa->GetTrackRotator()  && (i==PairAnalysis::kSEPMRot)          ) continue; /// check SE ROT
      ///      histos->AddClass(Form("Track_Legs_%s",PairAnalysis::PairClassName(i)));
    }
  }

  /// Add track classes - single tracks used for any pairing
  /// loop over all leg types and add classes (leg types in PairAnalysis.h ELegType)

  // for (Int_t i=0; i<PairAnalysis::kLegTypes; ++i)
  //   histos->AddClass(Form("Track.%s", PairAnalysis::TrackClassName(i)));

  /// OR/AND add merged leg histograms (e.g. e+-)
  histos->AddClass(Form("Track.%s",     PairAnalysis::PairClassName(PairAnalysis::kSEPM)));

  /// define REC ONLY histograms
  // ....

  /// add MC signal (if any) histograms to pair class
  if(papa->GetMCSignals()) {
    for (Int_t i=0; i<papa->GetMCSignals()->GetEntriesFast(); ++i) {
      PairAnalysisSignalMC *sigMC = (PairAnalysisSignalMC*)papa->GetMCSignals()->At(i);

      /// selection
      if(!sigMC) continue;
      if(!sigMC->IsSingleParticle()) continue; /// skip pair particle signals (no pairs)
      TString sigMCname = sigMC->GetName();
      /// by hand switched off
      if(sigMCname.EqualTo("eleGamPiOmega")) continue;

      /// mc truth - pair leg class
      //      if(!papa->IsNoPairing() && sigMC->GetFillPureMCStep()) histos->AddClass(Form("Track.Legs_%s_MCtruth",sigMCname.Data()));

      /// mc reconstructed - pair leg class
      //      if(!papa->IsNoPairing()) histos->AddClass(Form("Track.Legs_%s",        sigMCname.Data()));

      /// single tracks (merged +-)
      histos->AddClass(Form("Track.%s_%s",PairAnalysis::PairClassName(PairAnalysis::kSEPM),sigMCname.Data()));
      if(sigMC->GetFillPureMCStep())
	histos->AddClass(Form("Track.%s_%s_MCtruth",PairAnalysis::PairClassName(PairAnalysis::kSEPM),sigMCname.Data()));
    }
  }

  /// define MC and REC histograms
  AddTrackHistogramsReconstruction(  histos);
  AddTrackHistogramsMatching(        histos);
  AddTrackHistogramsMVDInfo(         histos);
  AddTrackHistogramsSTSInfo(         histos);
  AddTrackHistogramsRICHInfo(        histos);
  AddTrackHistogramsTRDInfo(         histos);
  AddTrackHistogramsTOFInfo(         histos);
  AddTrackHistogramsCombinedPIDInfo( histos);
  AddTrackHistogramsMCInfo(          histos);

}

void AddTrackHistogramsReconstruction(  PairAnalysisHistos *histos)
{
  //
  // add track histograms
  //
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(80,0.,20.),    PairAnalysisVarManager::kImpactParam);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(100,0.,500.),  PairAnalysisVarManager::kNTrk);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(100,0.,50000.), PairAnalysisVarManager::kNTrkMC);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(101,-0.05,10.05),  PairAnalysisVarManager::kChi2NDFtoVtx);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(100,0.,0.01),  PairAnalysisVarManager::kImpactParXY);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(100,0.,0.01),  PairAnalysisVarManager::kImpactParXY,
		       PairAnalysisHelper::MakeLinBinning(101,-0.05,10.05),  PairAnalysisVarManager::kChi2NDFtoVtx);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(100,0.,0.01),  PairAnalysisVarManager::kImpactParZ);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLogBinning(400,1.e-3.,4.),  "sqrt(Xv*Xv+Yv*Yv+Zv*Zv)");
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(200,650.,1450.),  PairAnalysisVarManager::kTrackLength);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(400,0,4.),  PairAnalysisVarManager::kPt);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(400,0,4.),  PairAnalysisVarManager::kPt, PairAnalysisVarManager::kWeight);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(400,0,20.),  PairAnalysisVarManager::kPout);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(400,0,20.),  PairAnalysisVarManager::kP,
		       PairAnalysisVarManager::kWeight);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(400,0,20.),  PairAnalysisVarManager::kP);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(360,0.,TMath::TwoPi()),  PairAnalysisVarManager::kPhi);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(100,0.,TMath::Pi()/4),  PairAnalysisVarManager::kTheta);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(100,-0.5,3.), PairAnalysisVarManager::kY,
		       PairAnalysisHelper::MakeLinBinning(125,0,5.), PairAnalysisVarManager::kPt);
  // histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(400,-4.,4.),  PairAnalysisVarManager::kPx);
  // histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(400,-4.,4.),  PairAnalysisVarManager::kPy);
  // histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(400,-0.5,10.),PairAnalysisVarManager::kPz);
  // histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(100,-10.0,10.0),  PairAnalysisVarManager::kEta);
  // histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(100,-3.0,3.0),  PairAnalysisVarManager::kY);
  // histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(100,0.5,4.5),  PairAnalysisVarManager::kYlab);
  // histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(100,-3.0,3.0),  PairAnalysisVarManager::kY,
  // 			 PairAnalysisHelper::MakeLinBinning(200,0,TMath::TwoPi()),PairAnalysisVarManager::kPhi);
  // histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(101,-0.05,10.05),  PairAnalysisVarManager::kChi2NDFtoVtx);

}

//______________________________________________________________________________________
void AddTrackHistogramsMatching(        PairAnalysisHistos *histos)
{
  //
  // add track histograms
  //
  /// general
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(200,0.,2.),  "P/PMC");
  //  histos->AddProfile("Track", PairAnalysisHelper::MakeLinBinning(100,0.,10.), PairAnalysisVarManager::kP, "abs(P-PMC)/PMC", "S");
  /// MVD
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(2,-0.5,1.5),  PairAnalysisVarManager::kMVDisMC);
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLinBinning(80,0.,20.),  PairAnalysisVarManager::kImpactParam, PairAnalysisVarManager::kMVDisMC,"I");
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLinBinning(100,0.,TMath::Pi()/4),  PairAnalysisVarManager::kTheta, PairAnalysisVarManager::kMVDisMC,"I");
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLogBinning(23,1.,500.),   PairAnalysisVarManager::kNTrk, PairAnalysisVarManager::kMVDisMC,"I");
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLogBinning(100,0.05,10.), PairAnalysisVarManager::kP, PairAnalysisVarManager::kMVDisMC,"I");
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLinBinning(7,-0.5, 6.5),PairAnalysisVarManager::kMVDHits, PairAnalysisVarManager::kMVDisMC,"I");
  /// STS
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(2,-0.5,1.5),  PairAnalysisVarManager::kSTSisMC);
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLinBinning(80,0.,20.),  PairAnalysisVarManager::kImpactParam, PairAnalysisVarManager::kSTSisMC,"I");
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLinBinning(100,0.,TMath::Pi()/4),  PairAnalysisVarManager::kTheta, PairAnalysisVarManager::kSTSisMC,"I");
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLogBinning(23,1.,500.), PairAnalysisVarManager::kNTrk, PairAnalysisVarManager::kSTSisMC,"I");
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLogBinning(100,0.05,10.), PairAnalysisVarManager::kP, PairAnalysisVarManager::kSTSisMC,"I");
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLinBinning(11,-0.5, 10.5),PairAnalysisVarManager::kSTSHits, PairAnalysisVarManager::kSTSisMC,"I");
  /// RICH
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(2,-0.5,1.5),  PairAnalysisVarManager::kRICHisMC);
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLinBinning(80,0.,20.),  PairAnalysisVarManager::kImpactParam, PairAnalysisVarManager::kRICHisMC,"I");
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLinBinning(100,0.,TMath::Pi()/4),  PairAnalysisVarManager::kTheta, PairAnalysisVarManager::kRICHisMC,"I");
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLogBinning(23,1.,500.), PairAnalysisVarManager::kNTrk, PairAnalysisVarManager::kRICHisMC,"I");
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLogBinning(100,0.05,10.), PairAnalysisVarManager::kP, PairAnalysisVarManager::kRICHisMC,"I");
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLinBinning(61,-0.5, 60.5),PairAnalysisVarManager::kRICHHits, PairAnalysisVarManager::kRICHisMC,"I");
  /// TRD
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(2,-0.5,1.5),  PairAnalysisVarManager::kTRDisMC);
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLinBinning(80,0.,20.),  PairAnalysisVarManager::kImpactParam, PairAnalysisVarManager::kTRDisMC,"I");
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLinBinning(100,0.,TMath::Pi()/4),  PairAnalysisVarManager::kTheta, PairAnalysisVarManager::kTRDisMC,"I");
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLogBinning(23,1.,500.), PairAnalysisVarManager::kNTrk, PairAnalysisVarManager::kTRDisMC,"I");
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLogBinning(100,0.05,10.), PairAnalysisVarManager::kP, PairAnalysisVarManager::kTRDisMC,"I");
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLinBinning(7,-0.5, 6.5),PairAnalysisVarManager::kTRDHits, PairAnalysisVarManager::kTRDisMC,"I");
  /// TOF
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(2,-0.5,1.5),  PairAnalysisVarManager::kTOFisMC);
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLinBinning(80,0.,20.),  PairAnalysisVarManager::kImpactParam, PairAnalysisVarManager::kTOFisMC,"I");
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLinBinning(100,0.,TMath::Pi()/4),  PairAnalysisVarManager::kTheta, PairAnalysisVarManager::kTOFisMC,"I");
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLogBinning(23,1.,500.), PairAnalysisVarManager::kNTrk, PairAnalysisVarManager::kTOFisMC,"I");
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLogBinning(100,0.05,10.), PairAnalysisVarManager::kP, PairAnalysisVarManager::kTOFisMC,"I");
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLinBinning(3,-0.5, 2.5),PairAnalysisVarManager::kTOFHits, PairAnalysisVarManager::kTOFisMC,"I");
}

//______________________________________________________________________________________
void AddTrackHistogramsMCInfo(          PairAnalysisHistos *histos)
{
  //
  // add track histograms
  //
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLogBinning(200,1.e-05,1.e+01), PairAnalysisVarManager::kWeight);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(80,0.,20.),PairAnalysisVarManager::kImpactParam,
		       PairAnalysisHelper::MakeLogBinning(200,1.e-05,1.e+01), PairAnalysisVarManager::kWeight);

  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(49,-0.5,48.5), PairAnalysisVarManager::kGeantId);
  histos->AddHistogram("Track", PairAnalysisHelper::MakePdgBinning(), PairAnalysisVarManager::kPdgCode);
  histos->AddHistogram("Track", PairAnalysisHelper::MakePdgBinning(), PairAnalysisVarManager::kPdgCodeMother);
  histos->AddHistogram("Track", PairAnalysisHelper::MakePdgBinning(), PairAnalysisVarManager::kPdgCodeGrandMother);
  /*
  histos->AddHistogram("Track", PairAnalysisHelper::MakePdgBinning(), PairAnalysisVarManager::kPdgCode,
		       PairAnalysisHelper::MakePdgBinning(), PairAnalysisVarManager::kPdgCodeMother);
  histos->AddHistogram("Track", PairAnalysisHelper::MakePdgBinning(), PairAnalysisVarManager::kPdgCode,
		       PairAnalysisHelper::MakeLinBinning(49,-0.5,48.5), PairAnalysisVarManager::kGeantId);
  histos->AddHistogram("Track", PairAnalysisHelper::MakePdgBinning(), PairAnalysisVarManager::kPdgCodeMother,
		       PairAnalysisHelper::MakeLinBinning(49,-0.5,48.5), PairAnalysisVarManager::kGeantId);
  */
  /// for acceptance defintion
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(11,-0.5, 10.5), PairAnalysisVarManager::kMVDHitsMC);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(16,-0.5, 15.5), PairAnalysisVarManager::kSTSHitsMC);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(2,-0.5,1.5),    PairAnalysisVarManager::kRICHhasProj);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(51,-0.5, 50.5), PairAnalysisVarManager::kRICHHitsMC);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(6,-0.5, 5.5),   PairAnalysisVarManager::kTRDHitsMC);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(3,-0.5, 2.5),   PairAnalysisVarManager::kTOFHitsMC);

}

//______________________________________________________________________________________
void AddTrackHistogramsMVDInfo(         PairAnalysisHistos *histos)
{
  //
  // add track histograms
  //

  /// hit association
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(11,-0.5, 10.5), PairAnalysisVarManager::kMVDHits);
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLogBinning(50,0.05,10.), PairAnalysisVarManager::kP, PairAnalysisVarManager::kMVDHits,"I");
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLogBinning(23,1.,500.), PairAnalysisVarManager::kNTrk, PairAnalysisVarManager::kMVDHits,"I");
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(120,0.,30.),    PairAnalysisVarManager::kMVDFirstHitPosZ);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(200,-5.,5.),   PairAnalysisVarManager::kMVDFirstExtX,
		       PairAnalysisHelper::MakeLinBinning(200,-5.,5.),   PairAnalysisVarManager::kMVDFirstExtY);
}

//______________________________________________________________________________________
void AddTrackHistogramsSTSInfo(         PairAnalysisHistos *histos)
{
  //
  // add track histograms
  //

  /// hit association
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(16,-0.5, 15.5), PairAnalysisVarManager::kSTSHits);
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLogBinning(50,0.05,10.), PairAnalysisVarManager::kP, PairAnalysisVarManager::kSTSHits,"I");
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLogBinning(23,1.,500.), PairAnalysisVarManager::kNTrk, PairAnalysisVarManager::kSTSHits,"I");
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(105,0.,105.),  PairAnalysisVarManager::kSTSFirstHitPosZ);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(101,-0.5, 50.5), PairAnalysisVarManager::kSTSChi2NDF);
  // histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(400,-20.,20.),  PairAnalysisVarManager::kSTSXv);
  // histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(400,-20.,20.),  PairAnalysisVarManager::kSTSYv);
  // histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(105,0.,105.),  PairAnalysisVarManager::kSTSZv);
  // histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(100,-10.,10.),  PairAnalysisVarManager::kImpactParZ);
}

//______________________________________________________________________________________
void AddTrackHistogramsRICHInfo(        PairAnalysisHistos *histos)
{
  //
  // add track histograms
  //
  /// PID
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(200,-2.0, 2.0), PairAnalysisVarManager::kRICHPidANN);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLogBinning(200,0.3,10.),  PairAnalysisVarManager::kP,
		       PairAnalysisHelper::MakeLinBinning(200,-2.0, 2.0), PairAnalysisVarManager::kRICHPidANN);
  /// reconstruction quality
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(100,-0.5, 1.5), PairAnalysisVarManager::kRICHChi2NDF);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(46,4.5, 50.5), PairAnalysisVarManager::kRICHHits);
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLogBinning(100,0.05,10.), PairAnalysisVarManager::kP, PairAnalysisVarManager::kRICHHits,"I;1.;100.");
histos->AddProfile(  "Track", PairAnalysisHelper::MakeLogBinning(23,1.,500.), PairAnalysisVarManager::kNTrk, PairAnalysisVarManager::kRICHHits,"I;1.;100.");
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(200,2., 10.), PairAnalysisVarManager::kRICHRadius);
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLogBinning(100,0.05,10.), PairAnalysisVarManager::kP, PairAnalysisVarManager::kRICHRadius,"I;1.0;50.");
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(200,0.0,10.),  PairAnalysisVarManager::kP,
		                PairAnalysisHelper::MakeLinBinning(200,0., 10.),  PairAnalysisVarManager::kRICHRadius);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(150,0., 15.), PairAnalysisVarManager::kRICHAxisA);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(150,0., 15.), PairAnalysisVarManager::kRICHAxisB);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(110,0., 1.1), "RICHAxisB/RICHAxisA");
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(100,0., 5.), PairAnalysisVarManager::kRICHDistance);
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLogBinning(100,0.05,10.), PairAnalysisVarManager::kP, PairAnalysisVarManager::kRICHDistance,"S");
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(150,0., 150.), PairAnalysisVarManager::kRICHRadialPos);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(100,0.,TMath::TwoPi()), PairAnalysisVarManager::kRICHRadialAngle);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(180,-TMath::Pi()/2,TMath::Pi()/2), PairAnalysisVarManager::kRICHPhi);
}

//______________________________________________________________________________________
void AddTrackHistogramsTRDInfo(         PairAnalysisHistos *histos)
{
  //
  // add track histograms
  //
  /// PID
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(500,0., 2.5e+2), PairAnalysisVarManager::kTRDSignal);
  histos->AddProfile("Track", PairAnalysisHelper::MakeLogBinning(200,0.1,15.),  PairAnalysisVarManager::kTRDPin,PairAnalysisVarManager::kTRDSignal, "I");
  histos->AddProfile("Track", PairAnalysisHelper::MakeLogBinning(200,0.1,15.),  PairAnalysisVarManager::kTRDPin,PairAnalysisVarManager::kTRDSignal, "S");
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLogBinning(200,0.1,15.),  PairAnalysisVarManager::kTRDPin,
   		       PairAnalysisHelper::MakeLinBinning(1000,0., 2.5e+2), PairAnalysisVarManager::kTRDSignal);
  // histos->AddHistogram("Track",PairAnalysisHelper::MakeArbitraryBinning("0.,0.25,0.5,1.,1.5,2.,3.,4.,5.,6.,7.,8.,9.,10."), PairAnalysisVarManager::kP,
  // 		       PairAnalysisHelper::MakeLinBinning(200,0., 2.5e+2), PairAnalysisVarManager::kTRDSignal);
  histos->AddProfile("Track",PairAnalysisHelper::MakeLogBinning(200,0.2,20000.), PairAnalysisVarManager::kBetaGammaMC,
		     PairAnalysisVarManager::kTRDSignal,"S");
  histos->AddHistogram("Track",PairAnalysisHelper::MakeLogBinning(200,0.2,20000.), PairAnalysisVarManager::kBetaGammaMC,
		       PairAnalysisHelper::MakeLinBinning(1000,0., 2.5e+2), PairAnalysisVarManager::kTRDSignal);
  // histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(100,0., 150./1.e6), "TRDPin-TRDPout",
  // 		       PairAnalysisHelper::MakeLinBinning(1000,0., 2.5e+2), PairAnalysisVarManager::kTRDSignal);

  // PairAnalysisVarManager::kP,
  // PairAnalysisHelper::MakeLinBinning(200,0., 5.e-4), PairAnalysisVarManager::kTRDSignal);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(125,-1.25, 1.25), PairAnalysisVarManager::kTRDPidANN);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(110,-0.05, 1.05), PairAnalysisVarManager::kTRDPidLikeEL);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(110,-0.05, 1.05), PairAnalysisVarManager::kTRDPidLikePI);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(110,-0.05, 1.05), PairAnalysisVarManager::kTRDPidLikeKA);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(110,-0.05, 1.05), PairAnalysisVarManager::kTRDPidLikePR);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLogBinning(200,0.1,10.),  PairAnalysisVarManager::kP,
		       PairAnalysisHelper::MakeLinBinning(110,-0.05, 1.05), PairAnalysisVarManager::kTRDPidLikeEL);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLogBinning(200,0.1,10.),  PairAnalysisVarManager::kP,
		       PairAnalysisHelper::MakeLinBinning(125,-1.25, 1.25), PairAnalysisVarManager::kTRDPidANN);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(6,-0.5, 5.5), PairAnalysisVarManager::kTRDHits,
		       PairAnalysisHelper::MakeLinBinning(125,-1.25, 1.25), PairAnalysisVarManager::kTRDPidANN);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(110,-0.05, 1.05), PairAnalysisVarManager::kTRDPidLikeEL,
		       PairAnalysisHelper::MakeLinBinning(125,-1.25, 1.25), PairAnalysisVarManager::kTRDPidANN);


  /// reconstruction quality
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(101,-0.5, 50.5), PairAnalysisVarManager::kTRDChi2NDF);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(400,-0.3, 0.1), "(TRDPout-TRDPin)/TRDPin");
  histos->AddProfile("Track", PairAnalysisHelper::MakeLinBinning(100,0., 10.), PairAnalysisVarManager::kTRDPin,
		       "(TRDPout-TRDPin)/TRDPin","S");
  /// hit association
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(6,-0.5, 5.5), PairAnalysisVarManager::kTRDHits);
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLogBinning(50,0.05,10.), PairAnalysisVarManager::kP, PairAnalysisVarManager::kTRDHits,"I");
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLogBinning(23,1.,500.), PairAnalysisVarManager::kNTrk, PairAnalysisVarManager::kTRDHits,"I");
  // histos->AddHistogram("Track", PairAnalysisHelper::MakeLogBinning(100,0.01,10.), PairAnalysisVarManager::kP,
  // 		       PairAnalysisHelper::MakeLinBinning(6,-0.5, 5.5), PairAnalysisVarManager::kTRDHits);

  //  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(6,-0.5, 5.5), PairAnalysisVarManager::kTRDHitsMC);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(6,-0.5, 5.5), PairAnalysisVarManager::kTRDTrueHits);
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLogBinning(50,0.05,10.), PairAnalysisVarManager::kP, PairAnalysisVarManager::kTRDTrueHits,"I");
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(6,-0.5, 5.5), PairAnalysisVarManager::kTRDFakeHits);
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLogBinning(50,0.05,10.), PairAnalysisVarManager::kP, PairAnalysisVarManager::kTRDFakeHits,"I");

  // histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(6,-0.5, 5.5), PairAnalysisVarManager::kTRDTrueHits,
  // 		       PairAnalysisHelper::MakeLinBinning(12,-1.5,10.5), PairAnalysisVarManager::kTRDFakeHits);
  // histos->AddHistogram("Track", PairAnalysisHelper::MakeLogBinning(100,0.01,10.), PairAnalysisVarManager::kP,
  // 			 PairAnalysisHelper::MakeLinBinning(5,0.,1.25), "TRDFakeHits/TRDHits");
  // histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(6,-0.5, 5.5), PairAnalysisVarManager::kTRDHits,
  // 		       PairAnalysisHelper::MakeLinBinning(5,0.,1.25), "TRDFakeHits/TRDHits");
  // histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(5,0.,1.25), "TRDTrueHits/TRDHits");
  // histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(5,0.,1.25), "TRDFakeHits/TRDHits");
  // histos->AddProfile(  "Track", PairAnalysisHelper::MakeLogBinning(50,0.05,10.), PairAnalysisVarManager::kP,"TRDFakeHits/TRDHits","I;-0.5;2."); ///S
  // histos->AddProfile(  "Track", PairAnalysisHelper::MakeLinBinning(100,0.,TMath::Pi()/4), PairAnalysisVarManager::kTheta,"TRDFakeHits/TRDHits","I;-0.5;2."); ///S
  // histos->AddProfile(  "Track", PairAnalysisHelper::MakeLinBinning(23,1.,500.), PairAnalysisVarManager::kNTrk,"TRDFakeHits/TRDHits","I;-0.5;2."); ///S
  // histos->AddProfile(  "Track", PairAnalysisHelper::MakeLogBinning(23,1.,500.), PairAnalysisVarManager::kNTrkMC, PairAnalysisVarManager::kTRDHits,"I");
  // histos->AddProfile(  "Track", PairAnalysisHelper::MakeLinBinning(5,0.,1.25), "TRDTrueHits/TRDHits", PairAnalysisVarManager::kTRDisMC,"I");

}

//______________________________________________________________________________________
void AddTrackHistogramsTOFInfo(         PairAnalysisHistos *histos)
{
  //
  // add track histograms
  //

  /// hit association
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(3,-0.5, 2.5), PairAnalysisVarManager::kTOFHits);
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLogBinning(50,0.05,10.), PairAnalysisVarManager::kP, PairAnalysisVarManager::kTOFHits,"I");
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLogBinning(23,1.,500.), PairAnalysisVarManager::kNTrk, PairAnalysisVarManager::kTOFHits,"I");

  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(110,0., 1.1), PairAnalysisVarManager::kBeta);
  /// PID
  TVectorD *loM2 = PairAnalysisHelper::MakeLinBinning(400,-0.25, 0.15);
  TVectorD *hiM2 = PairAnalysisHelper::MakeLinBinning(550, 0.15,11.15);
  histos->AddHistogram("Track", PairAnalysisHelper::CombineBinning(loM2,hiM2), PairAnalysisVarManager::kMassSq);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLogBinning(100,0.05,10.), PairAnalysisVarManager::kP,
		       PairAnalysisHelper::CombineBinning(loM2,hiM2), PairAnalysisVarManager::kMassSq);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLogBinning(100,0.05,10.), PairAnalysisVarManager::kP,
		       PairAnalysisHelper::MakeLinBinning(110,0., 1.1), PairAnalysisVarManager::kBeta);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLogBinning(100,0.05,5.), PairAnalysisVarManager::kP,
		       PairAnalysisHelper::MakeLinBinning(500,-1., 1.), PairAnalysisVarManager::kTOFPidDeltaBetaEL);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLogBinning(100,0.05,5.), PairAnalysisVarManager::kP,
		       PairAnalysisHelper::MakeLinBinning(500,-1., 1.), PairAnalysisVarManager::kTOFPidDeltaBetaPI);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLogBinning(100,0.05,5.), PairAnalysisVarManager::kP,
		       PairAnalysisHelper::MakeLinBinning(500,-1., 1.), PairAnalysisVarManager::kTOFPidDeltaBetaPR);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLogBinning(100,0.05,5.), PairAnalysisVarManager::kP,
		       PairAnalysisHelper::MakeLinBinning(500,-1., 1.), PairAnalysisVarManager::kTOFPidDeltaBetaKA);

}

//______________________________________________________________________________________
void AddTrackHistogramsCombinedPIDInfo(         PairAnalysisHistos *histos)
{
  //
  // add track histograms
  //
  // histos->AddProfile("Track", PairAnalysisHelper::MakeLinBinning(500,0., 2.5e+2), PairAnalysisVarManager::kTRDSignal,
  // 		     PairAnalysisHelper::MakeLinBinning(1000, -0.5, 11.5), PairAnalysisVarManager::kMassSq,
  // 		     PairAnalysisVarManager::kP,"I;1.5;2.0");
  TVectorD *loM2 = PairAnalysisHelper::MakeLinBinning(400,-0.25, 0.15);
  TVectorD *hiM2 = PairAnalysisHelper::MakeLinBinning(550, 0.15,11.15);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(350,0., 3.5e+2), PairAnalysisVarManager::kTRDSignal,
		       PairAnalysisHelper::CombineBinning(loM2,hiM2), PairAnalysisVarManager::kMassSq);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(125,-1.25, 1.25), PairAnalysisVarManager::kTRDPidANN,
		       PairAnalysisHelper::MakeLinBinning(200,-2.0, 2.0), PairAnalysisVarManager::kRICHPidANN);

}

//______________________________________________________________________________________
void AddPairHistograms( PairAnalysis *papa, Int_t cutDefinition)
{
  ///
  /// add pair histograms
  ///

  /// skip if no pairing done
  if(papa->IsNoPairing()) return;

  PairAnalysisHistos *histos = papa->GetHistoManager();

  /// add histogram classes
  /// loop over all pair types and add classes (pair types in PairAnalysis.h EPairType)
  /// automatically skip pair types w.r.t. configured bgrd estimators
  for (Int_t i=0; i<PairAnalysis::kPairTypes; i++){
    //    if(cutDefinition==kGammaRejcfg)     continue;   /// skip certain configs
    if(!papa->DoProcessLS()      && (i==PairAnalysis::kSEPP || i==PairAnalysis::kSEMM) ) continue; /// check SE LS
    if(!papa->GetMixingHandler() && (i>=PairAnalysis::kMEPP && i<=PairAnalysis::kMEMM) ) continue; /// check ME OS,LS
    if(!papa->GetTrackRotator()  && (i==PairAnalysis::kSEPMRot)          ) continue; /// check SE ROT
      histos->AddClass(Form("Pair.%s",PairAnalysis::PairClassName(i)));
  }

  ///// define output objects for REC ONLY /////
  // histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(100,-2.,2.),      PairAnalysisVarManager::kY);
  // histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(100,1.,4.5),      PairAnalysisVarManager::kYlab);
  // histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(100,0.,20),       PairAnalysisVarManager::kChi2NDF);
  // histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(100,0.,3.15),     PairAnalysisVarManager::kPsiPair);
  // histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(500,0.,2.5),      PairAnalysisVarManager::kImpactParXY);
  // histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(200,0.,10.),      PairAnalysisVarManager::kImpactParZ);

  /// mixing statistics
  Int_t mixBins=0;
  if(papa->GetMixingHandler()) {
    mixBins = papa->GetMixingHandler()->GetNumberOfBins();
    histos->AddHistogram("Pair",PairAnalysisHelper::MakeLinBinning(mixBins,0.,mixBins), PairAnalysisVarManager::kMixingBin);
    histos->AddHistogram("Pair",PairAnalysisHelper::MakeLinBinning(100,0.,400.), PairAnalysisVarManager::kNVtxContrib);
  }

  ////// add MC signal histo classes
  if(papa->GetMCSignals()) {
    for (Int_t i=0; i<papa->GetMCSignals()->GetEntriesFast(); ++i) {
      PairAnalysisSignalMC *sigMC = (PairAnalysisSignalMC*)papa->GetMCSignals()->At(i);
      if(!sigMC) continue;
      if(sigMC->IsSingleParticle()) continue; /// skip pair particle signals (no pairs)
      TString sigMCname = sigMC->GetName();
      histos->AddClass(Form("Pair_%s",        sigMCname.Data()));
      if(sigMC->GetFillPureMCStep()) histos->AddClass(Form("Pair_%s_MCtruth",sigMCname.Data()));
    }
  }
    
  ///// define output objects for MC and REC /////
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(500,0.0,500*0.01), PairAnalysisVarManager::kM); /// 20MeV bins, 5 GeV/c2
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(500,0.0,500*0.01), PairAnalysisVarManager::kM,
		       PairAnalysisVarManager::kWeight); // 40MeV bins, 12GeV/c2
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(250,0,5.),       PairAnalysisVarManager::kPt);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(100,0.,2.),      PairAnalysisVarManager::kY);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(125,.0,125*0.04), PairAnalysisVarManager::kM,
		       PairAnalysisHelper::MakeLinBinning(125,0,5.), PairAnalysisVarManager::kPt);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(100,-2.,2.), PairAnalysisVarManager::kY,
		       PairAnalysisHelper::MakeLinBinning(125,0,5.), PairAnalysisVarManager::kPt);

  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(100,0.,TMath::Pi()/2), PairAnalysisVarManager::kOpeningAngle);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(250,0.0,250*0.01), PairAnalysisVarManager::kM,
		       PairAnalysisHelper::MakeLinBinning(200,0.,TMath::Pi()/2), PairAnalysisVarManager::kOpeningAngle);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(100,0.,5.), PairAnalysisVarManager::kLegsP,
		       PairAnalysisHelper::MakeLinBinning(500,0.,0.05), PairAnalysisVarManager::kOpeningAngle);
  // histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(100,0.,5.), PairAnalysisVarManager::kLegsP,
  // 		       PairAnalysisHelper::MakeLinBinning(1000,0.,TMath::Pi()/2), PairAnalysisVarManager::kOpeningAngle);
  //		       PairAnalysisHelper::MakeLinBinning(100,0.,5./180.*TMath::Pi()), PairAnalysisVarManager::kOpeningAngle);

  //MC info
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLogBinning(200,1.e-05,1.e+01), PairAnalysisVarManager::kWeight);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(80,0.,20.),PairAnalysisVarManager::kImpactParam,
		       PairAnalysisHelper::MakeLogBinning(200,1.e-05,1.e+01), PairAnalysisVarManager::kWeight);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakePdgBinning(), PairAnalysisVarManager::kPdgCode);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakePdgBinning(), PairAnalysisVarManager::kPdgCodeMother);

}

//______________________________________________________________________________________
void AddPairHistogramsRejection( PairAnalysis *papa, Int_t cutDefinition)
{
  //
  // add pair histograms
  //

  // check if rejection was active
  if(!(papa->GetPairPreFilter().GetCuts()->GetEntries())) return;

  // fill only once
  //  if(cutDefinition!=kSTSGammaRejcfg) return;

  PairAnalysisHistos *histos = papa->GetHistoManager();

  /// add histogram classes
  /// loop over all pair types and add classes (pair types in PairAnalysis.h EPairType)
  histos->AddClass(Form("RejPair.%s",PairAnalysis::PairClassName(PairAnalysis::kSEPM)));
  
  ///// define output objects for REC ONLY /////
  // histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(100,0.,3.15),     PairAnalysisVarManager::kPsiPair);


  ////// add MC signal histo classes
  // if(papa->GetMCSignals()) {
  //   for (Int_t i=0; i<papa->GetMCSignals()->GetEntriesFast(); ++i) {
  //     PairAnalysisSignalMC *sigMC = (PairAnalysisSignalMC*)papa->GetMCSignals()->At(i);
  //     if(!sigMC) continue;
  //     if(sigMC->IsSingleParticle()) continue; /// skip pair particle signals (no pairs)
  //     TString sigMCname = sigMC->GetName();
  //     histos->AddClass(Form("Pair_%s",        sigMCname.Data()));
  //     if(sigMC->GetFillPureMCStep()) histos->AddClass(Form("Pair_%s_MCtruth",sigMCname.Data()));
  //   }
  // }
    
  ///// define output objects for MC and REC /////
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(500,0.0,500*0.001), PairAnalysisVarManager::kM); /// 20MeV bins, 5 GeV/c2
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(100,0.,0.05), PairAnalysisVarManager::kOpeningAngle);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(200,-1.,1.), "0.035-LegsP*0.01-OpeningAngle");
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(250,0.0,250*0.002), PairAnalysisVarManager::kM,
		       PairAnalysisHelper::MakeLinBinning(250,0.,0.05), PairAnalysisVarManager::kOpeningAngle);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(100,0.,5.), PairAnalysisVarManager::kLegsP,
		       PairAnalysisHelper::MakeLinBinning(500,0.,0.05), PairAnalysisVarManager::kOpeningAngle);
  //		       PairAnalysisHelper::MakeLinBinning(100,0.,5./180.*TMath::Pi()), PairAnalysisVarManager::kOpeningAngle);
  //MC info
  histos->AddHistogram("Pair", PairAnalysisHelper::MakePdgBinning(), PairAnalysisVarManager::kPdgCode);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakePdgBinning(), PairAnalysisVarManager::kPdgCodeMother);

}


//______________________________________________________________________________________
void AddNoCutHistograms(PairAnalysis *papa, Int_t cutDefinition)
{
  //
  // histograms before any cuts
  // for qa mostly
  // 

  if(cutDefinition!=kSTScfg) return;

  PairAnalysisHistos *histos = papa->GetHistoManager();
  histos->AddClass(Form("Track.noCuts"));

  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(400,0,4.),  PairAnalysisVarManager::kPt);
  //histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(100,0.,TMath::Pi()/4),  PairAnalysisVarManager::kTheta);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(2,-0.5,1.5),    PairAnalysisVarManager::kMVDhasEntr);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(11,-0.5, 10.5), PairAnalysisVarManager::kMVDHitsMC);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(16,-0.5, 15.5), PairAnalysisVarManager::kSTSHitsMC);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(2,-0.5,1.5),    PairAnalysisVarManager::kRICHhasProj);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(51,-0.5, 50.5), PairAnalysisVarManager::kRICHHitsMC);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(6,-0.5, 5.5),   PairAnalysisVarManager::kTRDHitsMC);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(3,-0.5, 2.5),   PairAnalysisVarManager::kTOFHitsMC);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(16,-0.5, 15.5), "MVDHits+STSHits");
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(101,-0.05,10.05),  PairAnalysisVarManager::kChi2NDFtoVtx);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(120,0.,30.),  PairAnalysisVarManager::kMVDFirstHitPosZ);

}
