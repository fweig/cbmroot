/// \file DrawQA.C
/// \brief A template macro with examples and explanations
///
/// This macro should represent a starting point for the post processing of the
///
///         PairAnalysis PAckage (PAPA) -- written by Julian Book
///
/// output.
///
/// - simple setup of a QA trending using meta data -
///
/// UPDATES, NOTES:
/// - ...
///
///
/// \author Julian Book <jbook@ikf.uni-frankfurt.de>, Goethe-University Frankfurt
/// \date Jun 08, 2015
///

void DrawQA()
{

  // files used for QA trending
  const Int_t nfiles=2;
  TString fileList[nfiles]  = {"./data/sis100_electron_TRDdigi.analysis.root",
			       "./data/sis100_electron_TRDclustering.analysis.root"
  };

  // switcher
  Bool_t doTime = kTRUE; // do time trending

  // log level
  //  gErrorIgnoreLevel = kPrint, kInfo, kWarning, kError, kBreak, kSysError, kFatal;
  gErrorIgnoreLevel = kError;

  // set own dielectron style
  //  PairAnalysisStyler::SetStyle(); // use a TStyle
  PairAnalysisStyler::LoadStyle();

  // set time offset
  TDatime da(2015,06,02,12,00,00);
  Int_t t0 = 0;da.Convert();
  gStyle->SetTimeOffset(t0);

  // build canvas
  TCanvas *can = new TCanvas("canQA","papa-QA",900,600);
  can->cd();

  // int qa histogra
  TH1F *hQA = new TH1F("hQA","hQA",10000,1.,0.); // find automatically the best limits
  //  hQA->SetYTitle("");   // set the y-title of the qa histogram

  // style
  PairAnalysisStyler::Style(hQA);
  hQA->GetXaxis()->SetLabelSize(0.02);

  // time dependent style
  if(doTime) {
    hQA->GetXaxis()->SetLabelOffset(0.05);
    hQA->GetXaxis()->SetTimeDisplay(1);
    //hQA->GetXaxis()->SetTimeFormat("%d\/%m\/%y");
    hQA->GetXaxis()->SetTimeFormat("#splitline{%d\/%m\/%Y}{%H:%M}");
  }

  // get histogram manager from file #1
  PairAnalysisHistos *histos  = new PairAnalysisHistos();
  //Initialise superior histogram classes
  histos->SetReservedWords("Hit;Track;Pair");

  // get histogram used for QA
  TH1D *htest =0x0;

  // loop over all file and open histogram for qa
  for(Int_t ifile=0; ifile<nfiles; ifile++) {

    // read file
    histos->ReadFromFile(fileList[ifile].Data(),"jbook","Acc");

    // get hist you want to look at
    htest = (TH1D*) histos->GetHist("Acc",        // config
				    "Track.SE+-", // class
				    "TRDHitsMC"   // histogram for QA
				    );
    if(!htest) continue;

    // get meta data
    PairAnalysisMetaData *meta = histos->GetMetaData();
    if(!meta) continue;
    TParameter<Int_t> *evts    = dynamic_cast<TParameter<Int_t> *>(meta->FindObject("events"));
    TParameter<Int_t> *date    = dynamic_cast<TParameter<Int_t> *>(meta->FindObject("date"));
    TParameter<Int_t> *time    = dynamic_cast<TParameter<Int_t> *>(meta->FindObject("time"));
    Printf("ifile%d: date %d time%d ",ifile,date->GetVal(), time->GetVal());
    TParameter<Int_t> *par     = dynamic_cast<TParameter<Int_t> *>(meta->FindObject("cbmroot"));
    Printf("ifile%d: meta data %s %d ",ifile,par->GetName(),par->GetVal());

    // get time stamp from meta data
    da.Set(date->GetVal(), time->GetVal());
    Int_t tX = da.Convert(1)-t0;

    // fill value (example)
    Double_t qaval = htest->Integral()*htest->GetMean()/ evts->GetVal();

    if(doTime) hQA->Fill( tX,            qaval );  // time dependence
    else       hQA->Fill( par->GetVal(), qaval );  // par  dependence

    // set axis titles
    if(doTime) hQA->SetXTitle("");
    else       hQA->SetXTitle(par->GetName());
    hQA->SetYTitle(htest->GetXaxis()->GetTitle());

    // cleanup
    histos->ResetHistogramList();

  }

  // draw qa
  hQA->BufferEmpty();
  hQA->DrawCopy("P");




}
