/// \file Config_user_Minimum.C
/// \brief A minimum task configuration macro
/// \author Julian Book <jbook@ikf.uni-frankfurt.de>, Goethe-University Frankfurt
/// \date Apr 25, 2016

AnalysisTaskMultiPairAnalysis *Config_user_Minimum(const char *taskname)
{
  /// analysis task
  AnalysisTaskMultiPairAnalysis *task = new AnalysisTaskMultiPairAnalysis(taskname);


  /// apply event cuts for all configs
  PairAnalysisVarCuts *eventCuts=new PairAnalysisVarCuts("vertex","vertex");
  eventCuts->AddCut("VtxChi/VtxNDF", 6., 1.e3, kTRUE);                /// 'kTRUE': exclusion cut based on formula
  task->           SetEventFilter(eventCuts);   /// add cuts to the global event filter


  /// init managing object PairAnalysis with unique name,title
  PairAnalysis *papa = new PairAnalysis("TASK","TASK");
  papa->SetHasMC(kTRUE);
  /// ~ type of analysis (leptonic, hadronic or semi-leptonic 2-particle decays are supported)
  papa->SetLegPdg( +11,-11);
  papa->SetRefitWithMassAssump(kTRUE); /// refit the track under mass assumption


  /// acceptance cuts
  PairAnalysisVarCuts   *accCuts = new PairAnalysisVarCuts("Acc","Acc");
  accCuts->SetCutType(PairAnalysisVarCuts::kAll);
  accCuts->AddCut(PairAnalysisVarManager::kPt,             0.2, 1e30);        // NOTE: was 0.2 GeV/c
  papa->           GetTrackFilter().AddCuts(accCuts);


  /// Monte Carlo Signals
  PairAnalysisSignalMC* ele = new PairAnalysisSignalMC(PairAnalysisSignalMC::kPrimElectron);
  ele->SetFillPureMCStep(kTRUE);
  papa->           AddSignalMC(ele);


  /// Setup histogram Manager
  PairAnalysisHistos *histos=new PairAnalysisHistos(papa->GetName(),papa->GetTitle());
  histos->AddClass("Event"); /// add histogram class
  histos->AddHistogram("Event",PairAnalysisHelper::MakeLinBinning(200,0.0,10.0), "VtxChi/VtxNDF");
  /// Track histogram classes, tracks ('+','-')
  for (Int_t i=0; i<PairAnalysis::kLegTypes; ++i)
    histos->AddClass(Form("Track.%s", PairAnalysis::TrackClassName(i)));
  /// MC histogram classes, tracks (merged: '+'&'-')
  histos->AddClass(Form("Track.%s_%s",PairAnalysis::PairClassName(PairAnalysis::kSEPM),ele->GetName()));
  /// MC truth histograms
  if(ele->GetFillPureMCStep())
    histos->AddClass(Form("Track.%s_%s_MCtruth",PairAnalysis::PairClassName(PairAnalysis::kSEPM),ele->GetName()));
  histos->AddHistogram("Track", PairAnalysisHelper::MakeLogBinning(400,0,4.),  PairAnalysisVarManager::kPt);
  papa->           SetHistogramManager(histos);


  /// some simple cut QA for events, tracks, OS-pairs
  papa->SetCutQA();


  /// add PAPA to the task
  task->           AddPairAnalysis(papa);


  return task;
}
