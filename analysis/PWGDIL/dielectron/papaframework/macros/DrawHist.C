/// \file DrawHist.C
/// \brief A template macro with examples and explanations
///
/// This macro should represent a starting point for the post processing of the
///
///         PairAnalysis PAckage (PAPA) -- written by Julian Book
///
/// output.
///
/// - This macro show 2 different ways of accesing and plotting histograms -
///
/// UPDATES, NOTES:
/// - ...
///
///
/// \author Julian Book <jbook@ikf.uni-frankfurt.de>, Goethe-University Frankfurt
/// \date Jun 08, 2015
///

void DrawHist(TString fin  = "./data/sis100_electron_TRDdigi.analysis.root")
{
  ///
  /// Draw histograms - Example macro (2 ways)
  ///

  /// files sed for QA trending
  const Int_t nfiles=2;
  TString fileList[nfiles]  = {"./data/sis100_electron_TRDdigi.analysis.root",
			       "./data/sis100_electron_TRDclustering.analysis.root"
  };

  ///  gErrorIgnoreLevel = kPrint, kInfo, kWarning, kError, kBreak, kSysError, kFatal;
  gErrorIgnoreLevel = kError;

  /// set own dielectron style
  // PairAnalysisStyler::SetStyle(); /// use own TStyle
  PairAnalysisStyler::LoadStyle();

  /// get histogram manager from file #1
  PairAnalysisHistos *histos = new PairAnalysisHistos();
  histos->ReadFromFile(fileList[0].Data(),"jbook","Acc");
  /// Initialise superior histogram classes
  histos->SetReservedWords("Hit;Track;Pair");

  /// print structure optional (show structure and all keys)
  histos->Print();


  //// via PairAnalysisHistos::DrawSame(options)
  /// draw either via histogram 'key' OR
  /// use the return value of the 'AddHistogram' functions
  /// (hint: copy the lines fom the config file)
  /// -------
  /// the follwing draw options are implemented in addition to ROOT's default options:
  /// if option contains 'leg' a legend will be created with the class name as caption
  /// if option contains 'can' a new canvas is created
  /// if option contains 'rebin' the objects are rebinned by 2
  /// if option contains 'norm' the objects are normalized to 1
  /// if option contains 'logx,y,z' the axis are plotted in log
  /// if option contains 'nomc' mc signals are not plotted

  histos->DrawSame("Pt",   "CanLegNormE1P");
  /// OR
  // histos->DrawSame(
  // 		   histos->AddHistogram("Track",
  // 					PairAnalysisHelper::MakeLinBinning(200,0.,10.),
  // 					PairAnalysisVarManager::kPt),
  // 		   "CanLegLogx");


  //// via PairAnalysisHistos::GetHist("config","class","key")
  TCanvas *can = new TCanvas("can","can",900,900);
  can->cd();
  TH1D *h1 = (TH1D*) histos->GetHist("Acc","Track.SE+-","Pt");
  PairAnalysisStyler::Style(h1,0);
  h1->DrawCopy("E1P");




}
