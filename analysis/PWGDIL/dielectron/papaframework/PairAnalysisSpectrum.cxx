/** @file PairAnalysisSpectrum.cxx
  * @copyright Copyright (C) 2016-2021 Justus-Liebig-Universitaet Giessen, Giessen
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Julian Book [committer] **/

///////////////////////////////////////////////////////////////////////////
//                PairAnalysisSpectrum                                   //
//                                                                       //
//                                                                       //
/*

  post-processing class to extract signals, efficiency, apply corrections,
  calculate systematics and describe the spectra by some functions/models.

  Example:

  PairAnalysisSpectrum *spectrum = new PairAnalysisSpectrum("legend header","systematics");

  spectrum->SetParticleOfInterest(pdgcode);
  spectrum->SetVariable("Pt",PairAnalysisHelper::MakeLinBinning(20,0.,2.) );

  spectrum->SetSystMethod( PairAnalysisSpectrum::kSystMax );

  // add input spectra coming from PairAnalysisHistos
  spectrum->AddInput( histos->DrawSame("pM_Pt","nomc goff"),           // raw invariant mass spectrum
                      "like-sign",                                     // unique string
		      histos->DrawSame("pPt","onlymc goff sel","phi")  // optional: MC spectra for efficiency
		     );
  spectrum->AddExtractor( sig );    // signal extraction (see PairAnalysisSignalExt)

  .... add more input as much as you want


  // process all inputs
  spectrum->Process();


  // draw spectra using TTree::Draw command + some extra arguments (see Draw)
  // see Extraction for content
  spectrum->Draw("s/eff:var","","leg logY syst P");

  // fit the spectrum by some predefined function (see PairAnalysisFunction)
  spectrum->SetFitRange(0.,2.);
  spectrum->SetDefault( PairAnalysisFunction::kBoltzmann );
  spectrum->SetFitOption("RN0");
  spectrum->Fit("leg L");

 */
//                                                                       //
///////////////////////////////////////////////////////////////////////////

//#include <TObject.h>
#include "PairAnalysisSpectrum.h"

#include <TCanvas.h>
#include <TDatabasePDG.h>
#include <TEventList.h>
#include <TF1.h>
#include <TFile.h>
#include <TFormula.h>
#include <TGraphErrors.h>
#include <TH1.h>
#include <TList.h>
#include <TObjString.h>
#include <TPad.h>
#include <TPaveText.h>
#include <TProfile.h>
#include <TProfile2D.h>
#include <TROOT.h>
#include <TTree.h>
#include <TVectorT.h>

#include "PairAnalysisHF.h"
#include "PairAnalysisHistos.h"
#include "PairAnalysisSignalExt.h"
#include "PairAnalysisStyler.h"
#include "PairAnalysisVarManager.h"

ClassImp(PairAnalysisSpectrum)

  //______________________________________________
  PairAnalysisSpectrum::PairAnalysisSpectrum()
  : PairAnalysisSpectrum("spectrum", "title")
{
  ///
  /// Named Constructor
  ///
}

//______________________________________________
PairAnalysisSpectrum::PairAnalysisSpectrum(const char* name, const char* title)
  : PairAnalysisFunction(name, title)
  , fRawInput(0)
  , fMCInput(0)
  , fMCTruth(0)
  , fExtractor(0)
{
  ///
  /// Named Constructor
  ///
  fTree = new TTree("PAPa", "PAPa-Spectrum");
  fExt  = new Extraction;
  for (Int_t i = 0; i < 100; i++)
    fInputKeys[i] = "";
}

//______________________________________________
PairAnalysisSpectrum::~PairAnalysisSpectrum()
{
  ///
  /// Default Destructor
  ///
  if (fResults) delete fResults;
  if (fExtractions) delete fExtractions;
}

//______________________________________________
void PairAnalysisSpectrum::AddInput(TObjArray* raw, TString identifier, TObjArray* mc, TObjArray* truth)
{
  ///
  /// add input array of histograms for signals extraction "raw" and
  /// efficiency calculation "mc" and unique "idetifier" string
  ///
  fRawInput.Add(raw);
  if (mc) fMCInput.Add(mc);
  if (truth) fMCTruth.Add(truth);
  fInputKeys[fIdx] = identifier;
  fIdx++;
}

//______________________________________________
void PairAnalysisSpectrum::Init()
{
  ///
  /// Initialize the tree
  ///
  fTree->Branch("Extraction", &fExt);
  //  fTree ->Branch("hSignal","TH1",&fHistSignal,32000,0);
}

//______________________________________________
void PairAnalysisSpectrum::Process()
{
  ///
  /// process the signal extraction
  ///
  Init();

  Int_t i = -1;

  TIter nextRaw(&fRawInput);  // raw content
  TObject* obj          = NULL;
  PairAnalysisHF* hf    = NULL;
  PairAnalysisHistos* h = NULL;

  TObject* objMC          = NULL;
  PairAnalysisHF* hfMC    = NULL;
  PairAnalysisHistos* hMC = NULL;

  TObject* objMCtruth = NULL;

  /// iterate over all raw input arrays
  while ((obj = nextRaw())) {
    i++;
    TObject::Info("Process", "Check Extraction for %s", fInputKeys[i].Data());
    TObject::Info("Process", "------------------------------------------------------------"
                             "-----------");
    TObject::Info("Process", "Input type: %s \n", obj->ClassName());

    if (fVarBinning) {
      TObject::Info("Process", "Binning provided for %s from %.3f to %.3f", fVar.Data(), fVarBinning->Min(),
                    fVarBinning->Max());
      //	fVarBinning->Print();
    }

    TObjArray* histArr = dynamic_cast<TObjArray*>(obj);
    if (!(histArr)) {
      if (!(h = dynamic_cast<PairAnalysisHistos*>(obj))) {
        if (!(hf = dynamic_cast<PairAnalysisHF*>(obj))) {
          TObject::Error("Process", "No input format found");
          continue;
        }
      }
    }

    // get extractor
    PairAnalysisSignalExt* sig = dynamic_cast<PairAnalysisSignalExt*>(fExtractor.At(i));
    if (!sig) continue;


    /// look for MC objects
    objMC                = fMCInput.At(i);
    TObjArray* histArrMC = NULL;
    if (objMC) {
      TObject::Info("Process", "Input MC type: %s \n", objMC->ClassName());
      histArrMC = dynamic_cast<TObjArray*>(objMC);
      if (!(histArrMC)) {
        if (!(hMC = dynamic_cast<PairAnalysisHistos*>(objMC))) {
          if (!(hfMC = dynamic_cast<PairAnalysisHF*>(objMC))) {
            TObject::Error("Process", "No MC input format found");
            //continue;
          }
        }
      }
    }

    /// look for MC truth objects
    objMCtruth            = fMCTruth.At(i);
    TObjArray* histArrMCt = NULL;
    if (objMCtruth) {
      TObject::Info("Process", "Input MC truth type: %s \n", objMCtruth->ClassName());
      histArrMCt = dynamic_cast<TObjArray*>(objMCtruth);
    }


    /// only integrated via histos
    if (!fVarBinning) {

      // get raw input histograms
      if (!histArr && h) histArr = h->DrawSame("pM-wghtWeight",
                                               "nomc goff");  //NOTE w/o "can" it crashes
      //	if(!histArr && h) histArr = h->DrawSame("pM","can nomc goff"); //NOTE w/o "can" it crashes
      //	histArr->Print();
      // get mc input histograms TODO: think about integration
      if (!histArrMC && hMC) histArrMC = hMC->DrawSame("pM", "onlymc eff goff");  //NOTE w/o "can" it crashes


      // process raw signal extraction
      sig->Process(histArr);
      if (gErrorIgnoreLevel < kWarning) sig->Print("");

      // validate signal extraction
      if (sig->GetSignal() < 0.) continue;

      // fill the tree
      fExt->setup      = fInputKeys[i];
      fExt->setupId    = i;
      fExt->poi        = sig->GetParticleOfInterest();
      fExt->var        = 1.;  //integrated
      fExt->varE       = 0.;
      fExt->s          = sig->GetSignal();
      fExt->sE         = sig->GetSignalError();
      fExt->b          = sig->GetBackground();
      fExt->bE         = sig->GetBackgroundError();
      fExt->sb         = sig->GetSB();
      fExt->sbE        = sig->GetSBError();
      fExt->sgn        = sig->GetSignificance();
      fExt->sgnE       = sig->GetSignificanceError();
      fExt->HistSignal = NULL;  //dynamic_cast<TH1F*>(sig->GetSignalHistogram());
      fExt->eff        = 1.;    //TODO: calculate this
      fExt->effE       = 0.;    //TODO: calculate this
      fExt->signal     = sig;   //NULL;
      fExt->sref       = 1.;    //TODO: calculate this
      fExt->srefE      = 0.;    //TODO: calculate this

      fTree->Fill();
    }
    else {

      /// get raw 2D histogram
      if (h && !h->SetCutClass(fInputKeys[i].Data())) continue;

      if (!histArr && h) histArr = h->DrawSame(Form("pM_%s", fVar.Data()), "nomc goff");
      TH2* histPM = (TH2*) sig->FindObject(histArr, PairAnalysis::EPairType::kSEPM);
      if (!histPM) return;

      TObjArray tmpArr;
      tmpArr.SetOwner(kFALSE);

      /// MC
      TH1* histMC = NULL;
      if (!histArrMC && hMC) {
        histArrMC = hMC->DrawSame(Form("p%s", fVar.Data()), "goff sel",
                                  "phi");  // TODO: add search using fPOIpdg
      }
      if (histArrMC) {
        TH1* tmpMCnom = (TH1*) histArrMC->At(0);
        if (histArrMC->GetEntriesFast() < 2) return;
        TH1* tmpMCden = (TH1*) histArrMC->At(1);
        /// rebin and calculate efficiency
        histMC         = tmpMCnom->Rebin(fVarBinning->GetNrows() - 1, "effMC", fVarBinning->GetMatrixArray());
        TH1* histMCden = tmpMCden->Rebin(fVarBinning->GetNrows() - 1, "effMCden", fVarBinning->GetMatrixArray());
        histMC->Divide(histMCden);
        delete histMCden;
      }
      /// debug
      if (histMC) TObject::Info("Process", "MC histogram found and rebinned");

      /// MC
      TH1* histMCtruth = NULL;
      if (histArrMCt) {
        TH1* tmpMCtrue = (TH1*) histArrMCt->At(0);
        if (!tmpMCtrue) return;
        /// rebin and calculate efficiency
        histMCtruth = tmpMCtrue->Rebin(fVarBinning->GetNrows() - 1, "sMCtrue", fVarBinning->GetMatrixArray());
      }
      /// debug
      if (histMCtruth) TObject::Info("Process", "MCtruth reference histogram found and rebinned");

      // loop over all bins
      for (Int_t bin = 0; bin < fVarBinning->GetNrows() - 1; bin++) {
        tmpArr.Clear();

        // var bin limits
        Double_t xLo = fVarBinning->GetMatrixArray()[bin];
        Double_t xHi = fVarBinning->GetMatrixArray()[bin + 1] - 0.000001;
        // axis limits
        Int_t binLo = histPM->GetYaxis()->FindBin(xLo);
        Int_t binHi = histPM->GetYaxis()->FindBin(xHi);
        // found bin limits
        Double_t fndLo = histPM->GetYaxis()->GetBinLowEdge(binLo);
        Double_t fndHi = histPM->GetYaxis()->GetBinLowEdge(binHi + 1);
        //printf("binning requested, found of %s: %.3f-%.3f, %.3f-%.3f \n", fVar.Data(), xLo,xHi, fndLo,fndHi );
        TObject::Info("Process", "Bin %d: %.3f < %s < %.3f", bin, fndLo, fVar.Data(), fndHi);

        // fill array for signal extraction
        for (Int_t ih = 0; ih < histArr->GetEntriesFast(); ih++) {
          TH2* hist = (TH2*) histArr->UncheckedAt(ih);
          tmpArr.Add(hist->ProjectionX(hist->GetTitle(), binLo, binHi, "e"));
        }
        //	    tmpArr.Print();

        // process
        sig->Process(&tmpArr);
        if (gErrorIgnoreLevel < kWarning) sig->Print("");

        // validate signal extraction
        if (sig->GetSignal() < 0.) continue;
        if (TMath::IsNaN(sig->GetSignalError())) continue;

        // fill the tree
        fExt->setup      = fInputKeys[i];
        fExt->setupId    = i;
        fExt->poi        = sig->GetParticleOfInterest();
        fExt->var        = (fndHi - fndLo) / 2 + fndLo;  // center of the found bin
        fExt->varE       = (fndHi - fndLo) / 2;          // found bin width
        fExt->s          = sig->GetSignal();
        fExt->sE         = sig->GetSignalError();
        fExt->b          = sig->GetBackground();
        fExt->bE         = sig->GetBackgroundError();
        fExt->sb         = sig->GetSB();
        fExt->sbE        = sig->GetSBError();
        fExt->sgn        = sig->GetSignificance();
        fExt->sgnE       = sig->GetSignificanceError();
        fExt->HistSignal = NULL;  //dynamic_cast<TH1F*>(sig->GetSignalHistogram());
        fExt->eff        = (histMC ? histMC->GetBinContent(bin + 1) : 1.);
        fExt->effE       = (histMC ? histMC->GetBinError(bin + 1) : 0.);
        fExt->signal     = sig;
        fExt->sref       = (histMCtruth ? histMCtruth->GetBinContent(bin + 1) : 0.);
        fExt->srefE      = (histMCtruth ? histMCtruth->GetBinError(bin + 1) : 0.);

        fTree->Fill();

        // set variable
        // hf->AddCutVariable( (EValueTypes)
        // 		     PairAnalysisVarManager::GetValueType(fVar.Data()),
        // 		     fVarBinning->GetMatrixArray()[bin],
        // 		     fVarBinning->GetMatrixArray()[bin+1]
        // 		     );

        // get histogram array
        //...

      }  //end binning

      /// clean up
      if (histMC) delete histMC;
      if (histMCtruth) delete histMCtruth;

    }  // end 2D

  }  // end raw content

  //  fTree->Print();
}

//______________________________________________
void PairAnalysisSpectrum::DrawSpectrum(const char* varexp, const char* selection, Option_t* option)
{
  //
  // TTree draw alias
  //
  /// additional plotting options:
  ///
  /// "syst":       draw and calculate systematic uncertainties according to fSystMthd (central point=mean value)
  /// "samepad":    draws spectrum into current pad
  ///
  /// "logx,y,z":   the axis are plotted in log-scale (labels are added automatically according to the range)
  /// "leg(f)":     a ("filled") legend will be created with caption=className ,
  ///               can be modified by PairAnalysisHistos::SetName("mycaption"),
  ///               change of legend position: see PairAnalysisStyler::SetLegendAlign
  ///
  /// "print":      prints the data points to stdout
  ///

  TString optString(option);
  optString.ToLower();
  printf("Plot spectrum: '%s' \t selection: '%s' \t options: '%s' \n", varexp, selection, optString.Data());
  Bool_t optLegFull = optString.Contains("legf");
  optString.ReplaceAll("legf", "");
  Bool_t optLeg = optString.Contains("leg");
  optString.ReplaceAll("leg", "");
  Bool_t optSyst = optString.Contains("syst");
  optString.ReplaceAll("syst", "");
  Bool_t optPrint = optString.Contains("print");
  optString.ReplaceAll("print", "");
  Bool_t optSamePad = optString.Contains("samepad");
  optString.ReplaceAll("samepad", "");

  /// counter
  Long64_t n = 1;

  /// load style
  PairAnalysisStyler::LoadStyle();

  /// canvas key
  TString ckey(varexp);
  Int_t ndim = ckey.CountChar(':') + 1;

  /// for variable/formula you look at, e.g. signal/eff
  TObjArray* carr = ckey.Tokenize(":");
  carr->SetOwner();
  //  delete carr;

  /// first variable e.g. signal in signal/eff
  TObjArray* oarr = ckey.Tokenize("+-*/:");
  oarr->SetOwner();
  TString fkey = ((TObjString*) oarr->At(0))->GetString();
  delete oarr;

  /// canvas
  ckey.ReplaceAll("/", "#");  /// canvas name does not allow '/'
  TCanvas* c = NULL;
  if (optSamePad) c = gPad->GetCanvas();
  else
    c = (TCanvas*) gROOT->FindObject(ckey.Data());
  if (!c) {
    TObject::Info("Draw", "create new canvas: '%s'", ckey.Data());
    c = new TCanvas(ckey.Data(), ckey.Data());
  }
  c->cd();

  /// count number of drawn objects in pad
  TObject* obj;
  Int_t nobj  = 0;
  TList* prim = gPad->GetListOfPrimitives();
  //////    if(prim->GetSize()>1) prim->RemoveLast(); // remove redraw axis histogram
  for (Int_t io = 0; io < prim->GetSize(); io++) {
    obj = prim->At(io);
    if (obj->InheritsFrom(TGraph::Class()) && obj != prim->At(io + 1)) nobj++;
  }

  TLegend* leg = 0;
  if ((optLeg && !nobj)) {
    //  if ( (optLeg && optTask && !nobj) || (optLeg && !optTask && !optDet) ) {
    leg = new TLegend(0. + gPad->GetLeftMargin() + gStyle->GetTickLength("Y"),
                      0. + gPad->GetBottomMargin() + gStyle->GetTickLength("X"),
                      1. - gPad->GetRightMargin() + gStyle->GetTickLength("Y"),
                      1. - gPad->GetTopMargin() + gStyle->GetTickLength("X"), GetName(), "nbNDC");
  }
  else if (optLeg && nobj) {
    leg = (TLegend*) prim->FindObject("TPave");
  }

  Info("Draw", "Basics: nobj: %d \t leg: %p", nobj, leg);

  // logaritmic style
  if (optString.Contains("logx")) gPad->SetLogx();
  if (optString.Contains("logy")) gPad->SetLogy();
  if (optString.Contains("logz")) gPad->SetLogz();
  optString.ReplaceAll("logx", "");
  optString.ReplaceAll("logy", "");
  optString.ReplaceAll("logz", "");

  if (ndim < 3 && !ckey.Contains("signal->")) {  // build own histogram with labels

    //    printf("try to get errors for %d dimensional input \n",ndim);
    TString varkey(varexp);

    // get event list
    n = fTree->Draw(">>elist", selection);
    if (!n) {
      delete carr;
      return;
    }
    TEventList* elist = (TEventList*) gDirectory->Get("elist");
    if (elist) {
      elist->SetReapplyCut(kTRUE);  // important!
      //	elist->Print("all");
      fTree->SetEventList(elist);
    }

    TH1D* hist = 0x0;
    // set up a proper histogram with labels
    if (varkey.Contains("setup")) {

      hist = new TH1D();  // activate buffer
      hist->SetNameTitle(varexp, selection);

      //read strings for all entries
      for (Long64_t i = 0; i < n; i++) {
        fTree->GetEntry(i);
        hist->Fill((fExt->setup).Data(), 1.);
        //	  printf(" read from tree: %s \n",(fExt->setup).Data());
      }

      hist->Draw("AXIS");
      hist->GetXaxis()->SetRange(1, n);
    }

    // get errors from tree
    TString errkey = ((TObjString*) carr->At(0))->GetString();
    errkey.ReplaceAll(fkey, fkey + "E");  /// first variables error
    //    if(ndim>1) varkey.ReplaceAll(fkey+":",fkey+":"+fkey+"E:");
    //    else       varkey.ReplaceAll(fkey,fkey+":"+fkey+"E");
    // if(ndim>1) varkey.ReplaceAll(fkey+":",fkey+":"+fkey+"E:");
    // else
    if (!fkey.Contains("E")) {
      varkey.Append(errkey.Prepend(":"));
      Info("Draw", " Appended '%s' by '%s' for error caluclation", varkey.Data(), errkey.Data());
    }

    /// TTree:Draw command with graphics off to get errors
    //printf("execute collect/draw command for %s \n",varkey.Data());
    fTree->Draw(varkey, selection, "goff");

    /// for one dimensional histograms
    Double_t* xval = new Double_t[fTree->GetSelectedRows()];
    for (Int_t ix = 0; ix < fTree->GetSelectedRows(); ix++)
      xval[ix] = 1.;

    /// setup the final tgraph WITH error bars
    TGraphErrors* gr = 0x0;
    if (ndim > 1) gr = new TGraphErrors(fTree->GetSelectedRows(), fTree->GetV2(), fTree->GetV1(), 0, fTree->GetV3());
    else
      gr = new TGraphErrors(fTree->GetSelectedRows(), fTree->GetV1(), xval, fTree->GetV2(), 0);
    delete[] xval;

    if (!gr) {
      delete carr;
      return;
    }

    /// style and legend entry
    PairAnalysisStyler::Style(gr, nobj);
    TString sel(selection);
    sel.ReplaceAll("setup.Contains", "");
    sel.ReplaceAll("(\"", "*");
    sel.ReplaceAll("\")", "*");
    sel.ReplaceAll("setup==", "");
    sel.ReplaceAll("\"", "");
    gr->SetName(Form("%s", sel.Data()));
    if (sel.Contains("setupId==") && sel.Length() < 11) {
      sel.ReplaceAll("setupId==", "");
      Int_t iId = sel.Atoi();
      gr->SetName(Form("%s", fInputKeys[iId].Data()));
    }
    if (optSyst) gr->SetName(GetTitle());
    if (fkey.Contains("ref")) gr->SetName("MC-truth");

    // sort x-values
    gr->Sort();
    TGraphErrors* grE = NULL;              // statistical
    TGraphErrors* grS = NULL;              // systematic
    TGraphErrors* grC = NULL;              // stat + syst
    if (optSyst && fVarBinning) {          //TODO: implement systematic calculation w/o binning
      grE           = new TGraphErrors();  // statistical graph
      grS           = new TGraphErrors();  // systematics graph
      grC           = new TGraphErrors();  // systematics graph
      Double_t* gx  = gr->GetX();
      Double_t* gy  = gr->GetY();
      Double_t* gye = gr->GetEY();

      // loop over all variable bins
      Int_t first = 0;  //TMath::BinarySearch(gr->GetN(),gx,xLo); //first bin
      Int_t ibin  = 0;
      while (first < gr->GetN()) {
        //      for(ibin=0; ibin<fVarBinning->GetNrows()-1; ibin++) {

        Int_t nsys    = 0;   // counter #systematics
        Double_t ysys = 0.;  // y-vlaue of systematic = mean value
        Double_t esys = 0.;  // uncertainty depends on method
        // calculate mean
        Double_t xvar = gx[first];
        for (Int_t i = first; i < gr->GetN(); i++) {
          //	  printf("gx[i]:%f xvar:%f xLo:%f\n",gx[i],xvar,xLo);
          //	  if(TMath::Abs(gx[i]-xvar)>1.e-8) break;
          if ((gx[i] - xvar) > 1.e-8) break;
          // printf("graph entry %d, found index first: %d with value %.3f \t y: %.3f+-%.3f \n",
          //        i,first,xvar,gy[i],gye[i]);
          nsys++;
          ysys += gy[i];
        }
        ysys /= (nsys ? nsys : 1);  // protect for zero division
        //	printf("bin %d, found index first: %d, %.3f \t y:%3.f \t nsys:%d\n",ibin,first,xvar,ysys,nsys);

        // y-syst. uncertainty
        //	printf("syst %f , <y> %f\n",esys,ysys);
        for (Int_t i = 0; i < nsys; i++) {
          Int_t j = first + i;
          //	  if(gx[j]!=xLo) break; // check should not be needed
          Double_t uce = 0.;
          switch (fSystMthd) {
            case ESystMethod::kBarlow:
              // I.  calc uncorr. stat. error from sub/superset w.r.t. first measurement
              uce = TMath::Sqrt(TMath::Abs(gye[j] * gye[j] - gye[first] * gye[first]));
              // II. calc max. deviation w.r.t. mean y-value incl. 1sigma* 0.9 of I.
              // NOTE: 0.9 can be change to a max value of 1->1sigma, 0.9 is more consevative
              esys = TMath::Max(esys, TMath::Abs(ysys - gy[j]) - 0.9 * uce);
              break;
            case ESystMethod::kSystMax: esys = TMath::Max(esys, TMath::Abs(ysys - gy[j])); break;
            case ESystMethod::kSystRMS: esys += gy[j] * gy[j]; break;
          }
          //	  printf("bin error %f \t  syst %f  from abs %f \n",gye[j],esys, TMath::Abs( gy[j] ));
        }

        // normalisation
        switch (fSystMthd) {
          case ESystMethod::kBarlow: /* nothing to be done */ break;
          case ESystMethod::kSystMax: /* nothing to be done */ break;
          case ESystMethod::kSystRMS: esys = TMath::Sqrt(TMath::Abs(esys / (nsys ? nsys : 1) - ysys * ysys)); break;
        }

        // fill statistical and systematic graph values and errors
        grE->SetPoint(ibin, xvar, ysys);            // mean
        grE->SetPointError(ibin, 0.0, gye[first]);  // stat.uncert. of first set

        Double_t boxW = (fVarBinning->Max() - fVarBinning->Min()) / (fVarBinning->GetNrows() - 1);
        grS->SetPoint(ibin, xvar, ysys);              // mean
        grS->SetPointError(ibin, boxW * 0.35, esys);  // systematic value

        // calculate err = sqrt(stat.**2 + syst**2)
        grC->SetPoint(ibin, xvar, ysys);  // mean
        grC->SetPointError(ibin, boxW * 0.35, TMath::Sqrt(esys * esys + gye[first] * gye[first]));

        // increase index counter
        first += nsys;
        ibin++;
      }  //next bin
      //      grS->Print();
    }
    else {
      grC = new TGraphErrors(*gr);
    }

    if (optPrint) grC->Print();

    Info("Draw", " Draw object with options: '%s'", optString.Data());
    if (!PairAnalysisStyler::GetFirstHistogram()) gr->Draw((optString + "A").Data());
    else {
      gr->Draw((optString + "same").Data());

      // set axis maximum
      Double_t* valE = grC->GetEY();
      Double_t* val  = grC->GetY();
      Int_t npnts    = grC->GetN();
      Int_t idx[1000];
      TMath::Sort(npnts, val, idx, kTRUE);  // kFALSE=increasing numbers

      Double_t errmin = (TMath::IsNaN(valE[idx[npnts - 1]]) ? 0. : valE[idx[npnts - 1]]);
      Double_t min    = (val[idx[npnts - 1]] - errmin) * 0.9;
      Double_t tmpmin = PairAnalysisStyler::GetFirstHistogram()->GetMinimum();
      PairAnalysisStyler::GetFirstHistogram()->SetMinimum((tmpmin < min ? tmpmin : min));
      Double_t errmax = (TMath::IsNaN(valE[idx[0]]) ? 0. : valE[idx[0]]);
      Double_t max    = (val[idx[0]] + errmax) * 1.1;
      Double_t tmpmax = PairAnalysisStyler::GetFirstHistogram()->GetMaximum();
      PairAnalysisStyler::GetFirstHistogram()->SetMaximum((tmpmax > max ? tmpmax : max));
    }

    // draw systemtaic graph ontop
    if (grS) {
      PairAnalysisStyler::Style(grE, nobj);
      grE->Draw((optString + "A").Data());
      PairAnalysisStyler::Style(grS, nobj);
      grS->SetFillColor(grS->GetLineColor());
      grS->SetFillStyle(kFEmpty);
      grS->Draw("2same");
    }

    // legend
    TString legOpt = optString + "L";
    legOpt.ReplaceAll("hist", "");
    legOpt.ReplaceAll("scat", "");
    if (legOpt.Contains("col")) legOpt = "";
    legOpt.ReplaceAll("z", "");
    legOpt.ReplaceAll("e", "");

    TString legkey = gr->GetName();
    if (leg) leg->AddEntry(gr, gr->GetName(), legOpt.Data());

    fSignal = grC;
    PairAnalysisStyler::Style(fSignal, nobj);
  }
  else {
    // execute tree draw command
    fTree->Draw(varexp, selection, optString.Data());
    fprintf(stderr, "use plain TTree::Draw command \n");
    return;
  }


  /// modify axis and titles
  //  printf("modify axis titles \n");
  if (!optSamePad) {
    UInt_t varx = PairAnalysisVarManager::GetValueType(fVar.Data());
    TString var(varexp);
    TObjArray* arr = var.Tokenize(":");
    arr->SetOwner();
    TString xt = "";
    TString yt = "Entries";
    xt         = ((TObjString*) arr->At(0))->GetString();
    if (xt.EqualTo("sb")) xt = PairAnalysisSignalExt::GetValueName(3);
    else if (xt.EqualTo("s"))
      xt = PairAnalysisSignalExt::GetValueName(0);
    else if (xt.EqualTo("b"))
      xt = PairAnalysisSignalExt::GetValueName(1);
    else if (xt.EqualTo("sgn"))
      xt = PairAnalysisSignalExt::GetValueName(2);
    else if (xt.EqualTo("var"))
      xt = Form("%s %s", PairAnalysisVarManager::GetValueLabel(varx), PairAnalysisVarManager::GetValueUnit(varx));
    else if (xt.Contains("var")) {
      xt.ReplaceAll("varE", Form("#Delta%s", PairAnalysisVarManager::GetValueLabel(varx)));
      xt.ReplaceAll("var", PairAnalysisVarManager::GetValueLabel(varx));
    }

    if (arr->GetEntriesFast() < 2) {
      PairAnalysisStyler::GetFirstHistogram()->SetXTitle(xt.Data());
      PairAnalysisStyler::GetFirstHistogram()->SetYTitle(yt.Data());
    }
    else {
      PairAnalysisStyler::GetFirstHistogram()->SetYTitle(xt.Data());
      xt = ((TObjString*) arr->At(1))->GetString();
      if (xt.EqualTo("sb")) xt = PairAnalysisSignalExt::GetValueName(3);
      else if (xt.EqualTo("s"))
        xt = PairAnalysisSignalExt::GetValueName(0);
      else if (xt.EqualTo("b"))
        xt = PairAnalysisSignalExt::GetValueName(1);
      else if (xt.EqualTo("sgn"))
        xt = PairAnalysisSignalExt::GetValueName(2);
      else if (xt.EqualTo("var"))
        xt = Form("%s %s", PairAnalysisVarManager::GetValueLabel(varx), PairAnalysisVarManager::GetValueUnit(varx));
      else if (xt.Contains("var")) {
        xt.ReplaceAll("varE", Form("#Delta%s", PairAnalysisVarManager::GetValueLabel(varx)));
        xt.ReplaceAll("var", PairAnalysisVarManager::GetValueLabel(varx));
      }
      PairAnalysisStyler::GetFirstHistogram()->SetXTitle(xt.Data());
    }

    /// delete array
    if (arr) delete arr;
  }

  /// clean up
  if (carr) delete carr;

  /// set ndivisions
  if (fVarBinning) PairAnalysisStyler::GetFirstHistogram()->SetAxisRange(fVarBinning->Min(), fVarBinning->Max(), "X");
  //    PairAnalysisStyler::GetFirstHistogram()->GetXaxis()->SetNdivisions(, 0, 0, kFALSE);


  // legend
  if (leg) {
    PairAnalysisStyler::SetLegendAttributes(leg, optLegFull);  // coordinates, margins, fillstyle, fontsize
    if (!nobj) leg->Draw();                                    // only draw the legend once
    ///    gPad->GetCanvas()->Update();
  }

  /// release eventlist
  fTree->SetEventList(0);
}

//______________________________________________
void PairAnalysisSpectrum::Write()
{
  ///
  /// write to output file
  ///

  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  TFile* fout = new TFile("test.root", "RECREATE");
  fout->cd();
  //  fTree->Print();
  fTree->Write();
  fout->Close();

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;
}

//______________________________________________
void PairAnalysisSpectrum::Fit(TString drawoption)
{
  ///
  /// Fit the spectrum according to the selected method
  ///
  /// additional plotting options:
  ///
  /// "leg":     add fit to legend if it exists
  ///

  drawoption.ToLower();
  Bool_t optLeg = drawoption.Contains("leg");
  drawoption.ReplaceAll("leg", "");

  //  fSignal->Print();

  Info("Fit", "Spectrum fit method: %s", fFuncSigBack->GetName());
  Int_t fitResult = fSignal->Fit(fFuncSigBack, (fFitOpt + "EX0").Data());

  // warning in case of fit issues
  if (fitResult != 0) {
    Error("Fit", "fit has error/issue (%d)", fitResult);
    return;
  }

  PairAnalysisStyler::Style(fFuncSigBack, static_cast<Int_t>(PairAnalysisStyler::Eidx::kFit));
  fFuncSigBack->SetLineColor(fSignal->GetLineColor());
  //  fFuncSigBack->SetLineStyle(kDashed);

  //  PairAnalysisStyler::Style(fit, PairAnalysisStyler::kFit);
  TF1* fit = fFuncSigBack->DrawCopy((drawoption + "same").Data());

  /// store chi2/ndf of the fit
  fDof = fFuncSigBack->GetNDF();
  if (fDof) fChi2Dof = fFuncSigBack->GetChisquare() / fFuncSigBack->GetNDF();

  /// add fit to legend
  if (optLeg) {
    TList* prim    = gPad->GetListOfPrimitives();
    TLegend* leg   = (TLegend*) prim->FindObject("TPave");
    TString legkey = fFuncSigBack->GetName();
    /// recalc legend coordinates, margins
    if (leg) {
      leg->AddEntry(fit, fFuncSigBack->GetName(), drawoption.Data());
      PairAnalysisStyler::SetLegendAttributes(leg);
      ///leg->Draw(); // was w/o !nobj
    }
  }
}
