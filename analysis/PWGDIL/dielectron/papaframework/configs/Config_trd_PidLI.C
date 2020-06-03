/// \file Config_trd_PidLI.C
/// \brief configuration for TRD Likelihood input
///
///         PairAnalysis PAckage (PAPA) -- written by Julian Book
///
///
///
/// UPDATES, NOTES:
/// -
///
///
/// \author Julian Book <jbook@ikf.uni-frankfurt.de>, Goethe-University Frankfurt
///         Etienne Bechtel<ebechtel@ikf.uni-frankfurt.de>, Goethe-University Frankfurt
///
/// \date Sep 12, 2016
///

PairAnalysis* Config_Analysis(Int_t cutDefinition);

/// names of the tasks
TString names=("MC;ELE;PIO");
enum {
  kMCcfg,              /// MC pid samples
  kElecfg,             /// electron sample
  kPiocfg              /// pion sample
};

////// OUTPUT functions
void InitHistograms(    PairAnalysis *papa, Int_t cutDefinition);
void AddPairHistograms( PairAnalysis *papa, Int_t cutDefinition);
void AddTrackHistograms(PairAnalysis *papa, Int_t cutDefinition);
void AddHitHistograms  (PairAnalysis *papa, Int_t cutDefinition);
void AddTrackHistogramsReconstruction(  PairAnalysisHistos *histos, Int_t cutDefinition);
void AddTrackHistogramsTRDInfo(         PairAnalysisHistos *histos, Int_t cutDefinition);

/// QA
////// CUTS
void SetupEventCuts(   AnalysisTaskMultiPairAnalysis *task);
void SetupTrackCuts(   PairAnalysis *papa, Int_t cutDefinition);
void SetupPairCuts(    PairAnalysis *papa,  Int_t cutDefinition);
void SetupTrackCutsMC( PairAnalysis *papa, Int_t cutDefinition);

////// SETTINGS
void AddMCSignals(     PairAnalysis *papa,  Int_t cutDefinition);

////// MISC
TObjArray *arrNames=names.Tokenize(";");
const Int_t nPapa=arrNames->GetEntries();

//______________________________________________________________________________________
AnalysisTaskMultiPairAnalysis *Config_trd_PidLI(const char *taskname="PidLI")
{
  ///
  /// creation of one multi task
  ///


  AnalysisTaskMultiPairAnalysis *task = new AnalysisTaskMultiPairAnalysis(taskname);

  /// apply event cuts for all configs
  SetupEventCuts(task);

  /// add PAPA analyses with different cuts to the task
  for (Int_t i=0; i<nPapa; ++i) {

    switch(i) {
    case kMCcfg:
    case kElecfg:
    case kPiocfg:
      Info("Config_trd_PidLI::Configure PAPa subtask",((TObjString*)arrNames->At(i))->GetName());
      break;
    default:
      Warning("Config_trd_PidLI::Skip PAPa subtask",((TObjString*)arrNames->At(i))->GetName());
      continue;
      break;
    }

    /// load configuration
    PairAnalysis *papa=Config_Analysis(i);
    if(!papa) continue;

    /// add PAPA to the task
    task->AddPairAnalysis(papa);
    std::cout << "-I- : Added subtask " << papa->GetName() << std::endl;

  }
  //  std::cout << "-I- : Added task " << task->GetName() << std::endl;
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
  Info("Config_trd_PidLI::Config_Analysis", "Adding config %s \n",name.Data());

  /// init managing object PairAnalysis with unique name,title
  PairAnalysis *papa = new PairAnalysis(Form("%s",name.Data()), Form("%s",name.Data()));
  papa->SetHasMC(kTRUE);

  /// ~ type of analysis (leptonic, hadronic or semi-leptonic 2-particle decays are supported)
  switch(cutDefinition) {
  case kMCcfg:       papa->SetLegPdg( +11, -11); break;
  case kElecfg:      papa->SetMotherPdg(22);  papa->SetLegPdg( +11, -11); break;
  case kPiocfg:      papa->SetMotherPdg(310); papa->SetLegPdg(+211,-211); break;
  }
  papa->SetRefitWithMassAssump(kTRUE); /// refit the track under mass assumption
  papa->SetUseKF(kTRUE);

  if(cutDefinition==kMCcfg) papa->SetNoPairing(); /// switch off the pairing

  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv CUTS vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
  SetupTrackCuts(papa,cutDefinition);
  SetupPairCuts(papa,cutDefinition);
  SetupTrackCutsMC(papa,cutDefinition);

  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv MISC vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */

  /// Monte Carlo Signals
  AddMCSignals(papa, cutDefinition);

  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv OUTPUT vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
  /// histogram setup (single TH1,2,3, TProfile,2,3, THn)
  InitHistograms(papa,cutDefinition);

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
  eventCuts->AddCut(PairAnalysisVarManager::kImpactParam, 0.0, 13.5);
  eventCuts->AddCut("VtxChi/VtxNDF", 6., 1.e3, kTRUE);                /// 'kTRUE': exclusion cut based on formula

  /// add cuts to the global event filter
  task->SetEventFilter(eventCuts);

  /// for debug purpose (recommended)
  //  eventCuts->Print();

}

//______________________________________________________________________________________                
void SetupTrackCutsMC(PairAnalysis *papa, Int_t cutDefinition)
{
  ///
  /// Setup the track cuts based on MC information only
  ///
  if(!papa->GetHasMC()) return;

  /// NOTE: skip this

  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv TRACK CUTS ON MCtruth vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
  /// example of adding acceptance cuts
  PairAnalysisVarCuts   *varAccCutsMC = new PairAnalysisVarCuts("accCutMCtruth","accCutMCtruth");
  varAccCutsMC->SetCutType(PairAnalysisVarCuts::kAll); /// wheter kAll or kAny cut has to be fullfilled

  varAccCutsMC->AddCut(PairAnalysisVarManager::kPtMC,           0.05, 1.e30);
  varAccCutsMC->AddCut(PairAnalysisVarManager::kSTSHitsMC,        2., 1.e10  );

  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv ACTIVATE CUTS vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
  papa->           GetTrackFilterMC().AddCuts(varAccCutsMC);    //varAccCutsMC->Print();

}

//______________________________________________________________________________________
void SetupTrackCuts(PairAnalysis *papa, Int_t cutDefinition)
{
  ///
  /// Setup the track cuts
  ///
  Bool_t hasMC=papa->GetHasMC();

  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv TRACK FILTER - QUALITY CUTS vvvvvvvvvvvvvvvvvvvvvvvvvvvv */

  /// acceptance cuts
  PairAnalysisVarCuts   *accCuts = new PairAnalysisVarCuts("Acc","Acc");
  accCuts->SetCutType(PairAnalysisVarCuts::kAll);
  accCuts->AddCut(PairAnalysisVarManager::kTRDHitsMC,      0.,   0.5, kTRUE); // TRD MC acceptance
  accCuts->AddCut(PairAnalysisVarManager::kPt,             0.05, 1.e30);

  /// (MVD+)STS standard reconstruction cuts
  PairAnalysisVarCuts   *recSTS = new PairAnalysisVarCuts("recSTS","recSTS");
  recSTS->SetCutType(PairAnalysisVarCuts::kAll);
  recSTS->AddCut("MVDHits+STSHits",                        6.,   15.);
  if(cutDefinition==kMCcfg) recSTS->AddCut(PairAnalysisVarManager::kChi2NDFtoVtx,    0.,    3., kFALSE); /// tracks pointing to the primary vertex
  else                      recSTS->AddCut(PairAnalysisVarManager::kChi2NDFtoVtx,    0.,    6., kTRUE);  /// tracks NOT pointing to the primary vertex

  /// TRD reconstruction cuts
  PairAnalysisVarCuts   *recTRD = new PairAnalysisVarCuts("recTRD","recTRD");
  recTRD->SetCutType(PairAnalysisVarCuts::kAll);
  recTRD->AddCut(PairAnalysisVarManager::kTRDHits,         1.,   4.);        /// min+max requieremnt for hits

  /// momentum selection for PID samples
  PairAnalysisVarCuts   *momCut = new PairAnalysisVarCuts("momCut","momCut");
  momCut->SetCutType(PairAnalysisVarCuts::kAll);
  momCut->AddCut(PairAnalysisVarManager::kTRDPin,         1.,  1.e10);


  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv ACTIVATE CUTS vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
  papa->           GetTrackFilter().AddCuts(accCuts);   //accCuts   ->Print();
  papa->           GetTrackFilter().AddCuts(recSTS);    //recSTS    ->Print();
  papa->           GetTrackFilter().AddCuts(recTRD);    //recTRD    ->Print();
  papa->           GetTrackFilter().AddCuts(momCut);    //momCut    ->Print();
}

//______________________________________________________________________________________
void SetupPairCuts(PairAnalysis *papa, Int_t cutDefinition)
{
  ///
  /// Setup the pair cuts
  ///

  /// skip pair cuts if no pairing is done
  if(papa->IsNoPairing()) return;

  /// MC
  Bool_t hasMC=papa->GetHasMC();

  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv PAIR FILTER - PAIR CUTS vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */

  /// Decay topology cuts
  PairAnalysisVarCuts *topoCut=new PairAnalysisVarCuts("TopoCut","TopoCut");

  switch(cutDefinition) {
  case kElecfg:
    /// soft cuts (efficiency)
    // topoCut->AddCut(PairAnalysisVarManager::kOpeningAngle,         0.0,        0.0299);
    // topoCut->AddCut(PairAnalysisVarManager::kLegDist,              0.0,        0.1);

    /// tight cuts (purity)
    topoCut->AddCut(PairAnalysisVarManager::kCosPointingAngle,     0.999983,   1.0 );
    topoCut->AddCut(PairAnalysisVarManager::kOpeningAngle,         0.0,        0.009999);
    topoCut->AddCut(PairAnalysisVarManager::kLegDist,              0.0,        0.009999);
    topoCut->AddCut(PairAnalysisVarManager::kR,                    2.2,        20.0);
    break;

  case kPiocfg:
    //  topoCut->AddCut(PairAnalysisVarManager::kCosPointingAngle, 0.999975,   1.0 );
    topoCut->AddCut(PairAnalysisVarManager::kOpeningAngle,      0.35,  0.8);
    topoCut->AddCut(PairAnalysisVarManager::kLegDist,           0.0,   0.02);
    break;

  }


  /// pair reconstruction cuts
  PairAnalysisVarCuts *pairRecCut=new PairAnalysisVarCuts("PairRecCut","PairRecCut");
  pairRecCut->AddCut(PairAnalysisVarManager::kChi2NDF,        -1.,     10. );


  /// mass cuts
  PairAnalysisVarCuts *massCut=new PairAnalysisVarCuts("massCut","massCut");
  switch(cutDefinition) {
  case kElecfg:   massCut->AddCut(PairAnalysisVarManager::kM,        0.0,     0.05 );   break;
  case kPiocfg:   massCut->AddCut("abs(M-MK0)",                      0.0,     0.005 );  break;
  }


  /// Armenteros - Podolanski Cuts
  PairAnalysisVarCuts *armInclCut=new PairAnalysisVarCuts("armInclCut","armInclCut");
  armInclCut->AddCut(PairAnalysisVarManager::kArmPt,         0.,   0.05);

  PairAnalysisObjectCuts *armCut=new PairAnalysisObjectCuts("armCut","armCut");
  switch(cutDefinition) {
  case kElecfg:
    armCut->AddCut(PairAnalysisVarManager::kArmPt,
		   "sqrt(-(((ArmAlpha-0.7159)*(ArmAlpha-0.7159))/((2*0.07/MLA)*(2*0.07/MLA)))*(0.07*0.07)+(0.07*0.07))",       // lambda - lower cut
		   "sqrt(-(((ArmAlpha-0.7159)*(ArmAlpha-0.7159))/((2*0.134/MLA)*(2*0.134/MLA)))*(0.134*0.134)+(0.134*0.134))", // lambda - upper cut
		   kTRUE);                                                                                                     // exclusion
    armCut->AddCut(PairAnalysisVarManager::kArmPt,
		   "sqrt(-(((ArmAlpha+0.7159)*(ArmAlpha+0.7159))/((2*0.07/MLA)*(2*0.07/MLA)))*(0.07*0.07)+(0.07*0.07))",       // anti-lambda - lower cut
		   "sqrt(-(((ArmAlpha+0.7159)*(ArmAlpha+0.7159))/((2*0.130/MLA)*(2*0.130/MLA)))*(0.130*0.130)+(0.130*0.130))", // anti-lambda - upper cut
		   kTRUE);                                                                                                     // exclusion
    armCut->AddCut(PairAnalysisVarManager::kArmPt,
		   "sqrt(-TMath::Power(((ArmAlpha)/0.83)*0.19575,2)+0.19575*0.19575)",                                         // k0s - lower cut
		   "sqrt(-TMath::Power(((ArmAlpha)/0.85)*0.215,2)+0.215*0.215)",                                               // k0s - upper cut
		   kTRUE);                                                                                                     // exclusion
    break;
  case kPiocfg:
    armCut->AddCut(PairAnalysisVarManager::kArmPt,
		   "sqrt(-TMath::Power(((ArmAlpha)/0.83)*0.19575,2)+0.19575*0.19575)",                                         // k0s - lower cut
		   "sqrt(-TMath::Power(((ArmAlpha)/0.85)*0.215,2)+0.215*0.215)",                                               // k0s - upper cut
		   kFALSE);                                                                                                    // inclusion
    break;
  }


  /* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv ACTIVATE CUTS vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
  switch(cutDefinition) {
  case kElecfg:
    papa->           GetPairFilter().AddCuts(topoCut);          //topoCut->Print();
    papa->           GetPairFilter().AddCuts(pairRecCut);       //pairRecCut->Print();
    papa->           GetPairFilter().AddCuts(massCut);          //massCut->Print();
    papa->           GetPairFilter().AddCuts(armInclCut);       //armInclCut->Print();
    papa->           GetPairFilter().AddCuts(armCut);           //armCut->Print();
    break;
  case kPiocfg:
    papa->           GetPairFilter().AddCuts(topoCut);          //topoCut->Print();
    papa->           GetPairFilter().AddCuts(pairRecCut);       //pairRecCut->Print();
    papa->           GetPairFilter().AddCuts(massCut);          //massCut->Print();
    papa->           GetPairFilter().AddCuts(armCut);           //armCut->Print();
    break;
  }

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
  ///       Some examples are given below...

  Bool_t hasMC=papa->GetHasMC();

  /// Setup histogram Manager
  PairAnalysisHistos *histos=new PairAnalysisHistos(papa->GetName(),papa->GetTitle());
  papa->SetHistogramManager(histos);

  /// Initialise superior histogram classes (reserved words)
  histos->SetReservedWords("Hit;Track;Pair");

  ////// EVENT HISTOS /////

  ////// PAIR HISTOS /////
  AddPairHistograms(papa, cutDefinition);

  ///// TRACK HISTOS /////
  AddTrackHistograms(papa, cutDefinition);

  ///// Hit Histos /////
  AddHitHistograms(papa, cutDefinition);

  ////// QA HISTOS /////

  ////// DEBUG //////
  TIter nextClass(histos->GetHistogramList());
  THashList *l=0;
  while ( (l=static_cast<THashList*>(nextClass())) ) {
    //    printf(" [D] HistogramManger: Class %s: Histograms: %04d \n", l->GetName(), l->GetEntries());
  }

} //end: init histograms

//______________________________________________________________________________________
void AddMCSignals(PairAnalysis *papa, Int_t cutDefinition){
  /// NOTE: recommended usage of predefined signals, see PairAnalysisSignalMC::EDefinedSignal

  /// Do we have an MC handler?
  if (!papa->GetHasMC()) return;
  Bool_t fillMC     =kFALSE; /// fill mc truth step (generated step)


  ///// single particle signals (primaries) /////
  fillMC=kFALSE;

  PairAnalysisSignalMC* ele = new PairAnalysisSignalMC(PairAnalysisSignalMC::kPrimElectron);
  ele->SetFillPureMCStep(fillMC);

  PairAnalysisSignalMC* pio = new PairAnalysisSignalMC(PairAnalysisSignalMC::kPrimPion);
  pio->SetFillPureMCStep(fillMC);

  PairAnalysisSignalMC* K = new PairAnalysisSignalMC(PairAnalysisSignalMC::kPrimKaon);
  K->SetFillPureMCStep(fillMC);

  PairAnalysisSignalMC* p = new PairAnalysisSignalMC(PairAnalysisSignalMC::kPrimProton);
  p->SetFillPureMCStep(fillMC);

  PairAnalysisSignalMC* muo = new PairAnalysisSignalMC(PairAnalysisSignalMC::kPrimMuon);
  muo->SetFillPureMCStep(fillMC);


  ////// pair signals ////
  fillMC=kFALSE;

  PairAnalysisSignalMC* conv = new PairAnalysisSignalMC(PairAnalysisSignalMC::kConversion);
  conv->SetFillPureMCStep(fillMC);

  PairAnalysisSignalMC* k0s = new PairAnalysisSignalMC(PairAnalysisSignalMC::kK0Short);
  k0s->SetFillPureMCStep(fillMC);

  PairAnalysisSignalMC* lambda = new PairAnalysisSignalMC(PairAnalysisSignalMC::kLambda);
  lambda->SetFillPureMCStep(fillMC);


  ////// background signals ////
  fillMC=kFALSE;

  PairAnalysisSignalMC* xx = new PairAnalysisSignalMC("xx (comb.)","xx");
  xx->SetFillPureMCStep(fillMC);
  xx->SetLegPDGs(0,0);
  xx->SetCheckBothChargesLegs(kFALSE,kFALSE);
  xx->SetMothersRelation(PairAnalysisSignalMC::kDifferent);

  PairAnalysisSignalMC* ee = new PairAnalysisSignalMC("X #rightarrow e^{+}e^{-}","ee");
  ee->SetFillPureMCStep(fillMC);
  ee->SetLegPDGs(11,-11);
  ee->SetMothersRelation(PairAnalysisSignalMC::kSame);

  /// ACTIVATE MC SIGNALS ///

  /// Single particles
  papa->AddSignalMC(ele);
  papa->AddSignalMC(pio);
  papa->AddSignalMC(K);
  papa->AddSignalMC(p);
  papa->AddSignalMC(muo);

  /// Pair Signal
  if(cutDefinition!=kMCcfg) {
    papa->AddSignalMC(conv);         //conv->Print();
    papa->AddSignalMC(k0s);          //k0s->Print();
    papa->AddSignalMC(lambda);       //lambda->Print();
    papa->AddSignalMC(xx);           //xx->Print();
    papa->AddSignalMC(ee);           //ee->Print();
  }

}

//______________________________________________________________________________________
void AddTrackHistograms( PairAnalysis *papa, Int_t cutDefinition)
{
  //
  // add track histograms
  //

  PairAnalysisHistos *histos = papa->GetHistoManager();

  /// Add track classes - LEGS of the pairs
  if(!papa->IsNoPairing()) {
    histos->AddClass(Form("Track.Legs.%s",PairAnalysis::PairClassName(PairAnalysis::kSEPM)));
  }

  /// Add track classes - single tracks used for the actual pairing
  /// loop over all leg types and add classes (leg types in PairAnalysis.h ELegType)
  for (Int_t i=0; i<PairAnalysis::kLegTypes; ++i)
    histos->AddClass(Form("Track.%s", PairAnalysis::TrackClassName(i)));

  /// add merged track histogram class (e.g. e+-)
  histos->AddClass(Form("Track.%s",     PairAnalysis::PairClassName(PairAnalysis::kSEPM)));

  /// add MC signal (if any) histograms classes
  if(papa->GetMCSignals()) {
    for (Int_t i=0; i<papa->GetMCSignals()->GetEntriesFast(); ++i) {
      PairAnalysisSignalMC *sigMC = (PairAnalysisSignalMC*)papa->GetMCSignals()->At(i);

      /// selection
      if(!sigMC) continue;
      TString sigMCname = sigMC->GetName();

      if(!sigMC->IsSingleParticle()){
	/// mc truth - pair leg class
	if(!papa->IsNoPairing() && sigMC->GetFillPureMCStep()) histos->AddClass(Form("Track.Legs_%s_MCtruth",sigMCname.Data()));

	/// mc reconstructed - pair leg class
	if(!papa->IsNoPairing())	                       histos->AddClass(Form("Track.Legs_%s",        sigMCname.Data()));
      }

      /// mc truth - single tracks (merged +-)
      if(sigMC->GetFillPureMCStep())
	histos->AddClass(Form("Track.%s_%s_MCtruth",PairAnalysis::PairClassName(PairAnalysis::kSEPM),sigMCname.Data()));

      /// mc reconstructed - single tracks (merged +-)
      if(sigMC->IsSingleParticle())      histos->AddClass(Form("Track.%s_%s",PairAnalysis::PairClassName(PairAnalysis::kSEPM),sigMCname.Data()));
    }
  }

  /// define MC and REC histograms
  AddTrackHistogramsReconstruction(  histos, cutDefinition);
  AddTrackHistogramsTRDInfo(         histos, cutDefinition);

}

//______________________________________________________________________________________
void AddTrackHistogramsReconstruction(  PairAnalysisHistos *histos, Int_t cutDefinition)
{
  //
  // add track histograms
  //
  histos->AddHistogram("Track",PairAnalysisHelper::MakeLogBinning(3,0.1,10.),  PairAnalysisVarManager::kP,
		       PairAnalysisHelper::MakeLinBinning(101,-0.05,10.05),  PairAnalysisVarManager::kChi2NDFtoVtx);
  histos->AddHistogram("Track",PairAnalysisHelper::MakeLogBinning(3,0.1,10.),  PairAnalysisVarManager::kP,
 		       PairAnalysisHelper::MakeLinBinning(200,0.,2.),  PairAnalysisVarManager::kImpactParXY);
  histos->AddHistogram("Track",PairAnalysisHelper::MakeLogBinning(3,0.1,10.),  PairAnalysisVarManager::kP,
		       PairAnalysisHelper::MakeLinBinning(100,0.,0.001),  PairAnalysisVarManager::kImpactParZ);

  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(101,-0.05,10.05),  PairAnalysisVarManager::kChi2NDFtoVtx);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(200,0.,2.),  PairAnalysisVarManager::kImpactParXY);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(100,0.,0.01),  PairAnalysisVarManager::kImpactParXY,
		       PairAnalysisHelper::MakeLinBinning(101,-0.05,10.05),  PairAnalysisVarManager::kChi2NDFtoVtx);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(100,0.,0.01),  PairAnalysisVarManager::kImpactParZ);

  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(400,0,4.),  PairAnalysisVarManager::kPt);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(400,0,20.),  PairAnalysisVarManager::kP);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(316,0.,TMath::TwoPi()),  PairAnalysisVarManager::kPhi);
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(180,0.,TMath::Pi()/2),  PairAnalysisVarManager::kTheta);
}

//______________________________________________________________________________________
void AddTrackHistogramsTRDInfo(         PairAnalysisHistos *histos, Int_t cutDefinition)
{
  //
  // add track histograms
  //

  /// hit association
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(6,-0.5, 5.5), PairAnalysisVarManager::kTRDHits);
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLogBinning(50,0.05,10.), PairAnalysisVarManager::kP, PairAnalysisVarManager::kTRDHits,"I");
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLogBinning(23,1.,500.), PairAnalysisVarManager::kNTrk, PairAnalysisVarManager::kTRDHits,"I");

  // MC matching
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(6,-0.5, 5.5), PairAnalysisVarManager::kTRDTrueHits);
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLogBinning(50,0.05,10.), PairAnalysisVarManager::kP, PairAnalysisVarManager::kTRDTrueHits,"I");
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(6,-0.5, 5.5), PairAnalysisVarManager::kTRDFakeHits);
  histos->AddProfile(  "Track", PairAnalysisHelper::MakeLogBinning(50,0.05,10.), PairAnalysisVarManager::kP, PairAnalysisVarManager::kTRDFakeHits,"I");

}

//______________________________________________________________________________________
void AddPairHistograms( PairAnalysis *papa, Int_t cutDefinition)
{
  //
  // add pair histograms
  //

  // skip if no pairing done
  if(papa->IsNoPairing()) return;

  PairAnalysisHistos *histos = papa->GetHistoManager();

  /// add pair histogram classes
  histos->AddClass(Form("Pair.%s",PairAnalysis::PairClassName(PairAnalysis::kSEPM)));

  /// add MC signal histo classes
  if(papa->GetMCSignals()) {
    for (Int_t i=0; i<papa->GetMCSignals()->GetEntriesFast(); ++i) {
      PairAnalysisSignalMC *sigMC = (PairAnalysisSignalMC*)papa->GetMCSignals()->At(i);
      if(!sigMC) continue;

      /// skip pair particle signals (no pairs)
      if(sigMC->IsSingleParticle()) continue;

      TString sigMCname = sigMC->GetName();

      /// mc truth - pairs
      if(sigMC->GetFillPureMCStep()) histos->AddClass(Form("Pair_%s_MCtruth",sigMCname.Data()));

      /// mc reconstructed - pairs
      histos->AddClass(Form("Pair_%s",        sigMCname.Data()));
    }
  }

  ///// define output objects for MC and REC /////
  TVectorD *minvBins = NULL;
  if(cutDefinition==kElecfg) minvBins = PairAnalysisHelper::MakeLinBinning(300,0.0,0.0+300*0.002);
  else                       minvBins = PairAnalysisHelper::MakeLinBinning(300,0.3,0.3+300*0.002);
  if(cutDefinition==kElecfg) histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(750,0.0,10*0.005), PairAnalysisVarManager::kM);
  else                       histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(750,0.495-5*0.005,0.495+5*0.005), PairAnalysisVarManager::kM);

  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(250,0,5.),    PairAnalysisVarManager::kPt);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(100,-2.5,5.), PairAnalysisVarManager::kY);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(50,0.,5.), PairAnalysisVarManager::kTheta);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(200,-1.,1.),  "cos(Theta)");

  /// V0s variables
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(100,-1.,1.),PairAnalysisVarManager::kArmAlpha,
		       PairAnalysisHelper::MakeLinBinning(100,0.,0.25), PairAnalysisVarManager::kArmPt);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(300,0.,30.), PairAnalysisVarManager::kR);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(50,0.,0.5), PairAnalysisVarManager::kLegDist);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(100,0.9998,1.), PairAnalysisVarManager::kCosPointingAngle);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(101,-0.5,10.5), PairAnalysisVarManager::kChi2NDF);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(100,0.,1.),   PairAnalysisVarManager::kOpeningAngle);

  histos->AddHistogram("Pair",PairAnalysisHelper::MakeLogBinning(100,0.1,10.),  PairAnalysisVarManager::kP,
		       PairAnalysisHelper::MakeLinBinning(50,0.,0.5),  PairAnalysisVarManager::kLegDist);
  histos->AddHistogram("Pair",PairAnalysisHelper::MakeLogBinning(3,0.1,10.),  PairAnalysisVarManager::kP,
		       PairAnalysisHelper::MakeLinBinning(50,0.,50.),  PairAnalysisVarManager::kR);

  /// mc matching
  histos->AddHistogram("Pair",PairAnalysisHelper::MakeLogBinning(3,0.1,10.),  PairAnalysisVarManager::kPMC,
		       PairAnalysisHelper::MakeLinBinning(100,0.9998,1.),  PairAnalysisVarManager::kCosPointingAngleMC);
  histos->AddHistogram("Pair",PairAnalysisHelper::MakeLogBinning(3,0.1,10.),  PairAnalysisVarManager::kPMC,
		       PairAnalysisHelper::MakeLinBinning(100,0.,1.),  PairAnalysisVarManager::kOpeningAngleMC);


  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(100,0.,1.), PairAnalysisVarManager::kOpeningAngleMC);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(100,0.9998,1.), PairAnalysisVarManager::kCosPointingAngleMC);
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(100,0.,100.), PairAnalysisVarManager::kPhiMC);

  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(50,0.,5.), PairAnalysisVarManager::kThetaMC);

  /// pulls
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(600,-.3,0.3), "CosPointingAngleMC-CosPointingAngle");
  histos->AddHistogram("Pair", PairAnalysisHelper::MakeLinBinning(600,-.3,.3), "OpeningAngleMC-OpeningAngle");

  //MC truth info
  histos->AddHistogram("Pair", PairAnalysisHelper::MakePdgBinning(), PairAnalysisVarManager::kPdgCode);

}


//______________________________________________________________________________________
void AddHitHistograms(         PairAnalysis *papa, Int_t cutDefinition)
{
  ///
  /// add hit histograms
  ///

  PairAnalysisHistos *histos = papa->GetHistoManager();

  /// Add hit classes - TRD tracks (before pairing)
  histos->AddClass("Hit.TRD");

  /// Add hit classes - LEGS of the pairs (after pair cuts)
  histos->AddClass("Hit.Legs.TRD");

  /// add MC signal (if any) histograms classes
  if(papa->GetMCSignals()) {

    for (Int_t i=0; i<papa->GetMCSignals()->GetEntriesFast(); ++i) {
      PairAnalysisSignalMC *sigMC = (PairAnalysisSignalMC*)papa->GetMCSignals()->At(i);
      if(!sigMC) continue;

      TString sigMCname = sigMC->GetName();

      // mc reconstructed - hits
      histos->AddClass(Form("Hit.TRD_%s",sigMCname.Data()));

      // mc reconstructed - hits legs
      if(!sigMC->IsSingleParticle())
	histos->AddClass(Form("Hit.Legs.TRD_%s",sigMCname.Data()));
    }
  }

  /// histograms
  histos->AddHistogram("Hit", PairAnalysisHelper::MakeLinBinning(320,  0.,8.e+1),  PairAnalysisVarManager::kEloss);
  histos->AddHistogram("Hit", PairAnalysisHelper::MakeArbitraryBinning("0.,0.25,0.5,1.,1.5,2.,3.,4.,5.,6.,7.,8.,9.,10.,20."),
		       PairAnalysisVarManager::kTRDPin,
		       PairAnalysisHelper::MakeLinBinning(320,  0.,8.e+1),  PairAnalysisVarManager::kEloss);
  histos->AddHistogram("Hit", PairAnalysisHelper::MakeArbitraryBinning("0.,0.25,0.5,1.,1.5,2.,3.,4.,5.,6.,7.,8.,9.,10.,20."),
		       PairAnalysisVarManager::kP,
		       PairAnalysisHelper::MakeLinBinning(320,  0.,8.e+1),  PairAnalysisVarManager::kEloss);

  /// mc matching
  histos->AddHistogram("Hit", PairAnalysisHelper::MakeLinBinning(320,  0.,8.e+1),  PairAnalysisVarManager::kElossdEdx);
  histos->AddHistogram("Hit", PairAnalysisHelper::MakeLinBinning(250,  0.,5.e-4),  PairAnalysisVarManager::kElossTR);
  histos->AddHistogram("Hit", PairAnalysisHelper::MakeLinBinning(320,  0.,8.e+1),  PairAnalysisVarManager::kElossMC);
  histos->AddHistogram("Hit" ,	PairAnalysisHelper::MakeLogBinning(200,0.1,20.    ),  PairAnalysisVarManager::kTRDPin,
		       PairAnalysisHelper::MakeLinBinning(320,  0., 8.e+1),  PairAnalysisVarManager::kElossMC);

  /// pulls
  histos->AddHistogram("Hit", PairAnalysisHelper::MakeLinBinning(200,  0.,5.e+1),  "ElossMC-Eloss");

}
