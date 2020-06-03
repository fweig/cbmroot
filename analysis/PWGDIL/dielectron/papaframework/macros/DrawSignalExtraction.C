/// \file DrawSignalExtraction.C
/// \brief A template macro with examples and explanations
///
/// This macro should represent a starting point for the post processing of the
///
///         PairAnalysis PAckage (PAPA) -- written by Julian Book
///
/// output.
///
/// - This macro shows the useage of the signal extraction classes -
///
/// UPDATES, NOTES:
/// - ...
///
///
/// \author Julian Book <jbook@ikf.uni-frankfurt.de>, Goethe-University Frankfurt
/// \date Jun 29, 2015
///

void DrawSignalExtraction(TString fin  = "./data/sis100_electron_TRDdigi.analysis.root")
{
  ///
  /// Draw Signal Extraction - Example macro
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
  histos->ReadFromFile(fileList[0].Data(),"jbook","TRD");
  /// Initialise superior histogram classes
  histos->SetReservedWords("Hit;Track;Pair");

  /// print structure optional (show structure and all keys)
  //  histos->Print();

  //// via PairAnalysisHistos::DrawSame(options)
  /// use the option 'goff' to get a filled array with histograms
  TObjArray *arr = histos->DrawSame("pM",   "Can Leg goff");
  if(!arr) Error("DrawSignalExtraction","something went wrong!");
  else     arr->Print();

  //// signal extraction class setup (use either Ext or Func)
  PairAnalysisSignalBase *sig = new PairAnalysisSignalExt();
  /// set background estimator (see PairAnalysisSignalBase::EBackgroundMethod)
  sig->SetMethod(PairAnalysisSignalBase::kLikeSign); /// kRotation, kEventMixing
  sig->SetRebin(1);                                  /// rebin the spectrum if needed
  sig->SetScaleBackgroundToRaw(1.2,2.);              /// matching region of bgrd to raw spectrum
  //  sig->SetNTrackRotations(5);                    /// track rotation normlaisation
  /// signal extraction
  sig->SetExtractionMethod(PairAnalysisSignalBase::kBinCounting); /// use kBinCounting, kGaus, kMCScaledMax, ...
  sig->SetIntegralRange(0.9,1.1);                    /// bin counting region
  /// process the data array
  sig->Process(arr);
  sig->Print("");                                    /// print result of signal extraction

  /// draw into canvas, add legend, show statitics box
  /// use draw options as desribed in PairAnalysisSignalExt::Draw
  /// 'nomc' no MC signal are plotted on top
  /// 'sb' signal/to background is plotted  instead
  /// 'sgn' significance is plotted instead
  sig->Draw("can leg stat logY");

}
