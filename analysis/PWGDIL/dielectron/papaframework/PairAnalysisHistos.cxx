/* Copyright (C) 2015-2020 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Julian Book [committer] */
/*
  TOADD: reserved words, MC signals, hist classes, MC weighting, plotting!

  Histograms such as THxF,D, TProfile,2D,3D and n-dimensonal objects such as
  (THn, THnSparse) can be added via the various template functions:
  - AddHistogram
  - AddProfile + different error option (Mean,Rms,.. see: TProfile::BuildOptions(..))
  - AddSparse
  In addition histograms can be filled with weights.

  Different kind of binnings can be passed (linear, arbitrary, logarithmic)
  either via the PairAnalysisHelper functionalities:
  - PairAnalysisHelper::MakeLinBinning(nbins,low,high)
  - PairAnalysisHelper::MakeLogBinning(nbins,low,high)
  - PairAnalysisHelper::MakeArbitraryBinning("1.,4.,10.,..")

  The 'name' and 'title;titleX;titleY;...' of the objects and all axis labels,
  units and names and object names are set in a consistent way and
  it is ensured that they are unique.

  Variables are added via the enumerator of PairAnalysisVarManager. Any
  computation of variables (max=10) are implemented according to TFormula
  using the TMath functions.

  The arguments and options in the function DrawSame and DrawTaskSame provide various and 
  powerful possibilities to plot and post-process histograms. 
  
  Examples:

  // 1 dimensional
  AddHistogram("Event", PairAnalysisHelper::MakeLinBinning(200,-1.,1.),  PairAnalysisVarManager::kXvPrim);

  AddHistogram("Event", PairAnalysisHelper::MakeLinBinning(200,0.0,10.0), "VtxChi/VtxNDF");          // using formulas

  AddHistogram("Track", PairAnalysisHelper::MakeLogBinning(400,0,4.),  PairAnalysisVarManager::kPt); // logarithmic

  AddHistogram("Hit.TRD",PairAnalysisHelper::MakeLinBinning(105,-1.e+2,5.),  "ElossdEdx-ElossMC");   // use MC truth

  // 2 dimensional
  AddHistogram("Track", PairAnalysisHelper::MakeLinBinning(100,-0.5,3.), PairAnalysisVarManager::kY,
                        PairAnalysisHelper::MakeLinBinning(125,0,5.), PairAnalysisVarManager::kPt);

  AddProfile(  "Track", PairAnalysisHelper::MakeLogBinning(50,0.05,10.), PairAnalysisVarManager::kP,
                        PairAnalysisVarManager::kTRDHits, "I"); // Y-average

  ...

*/

#include "PairAnalysisHistos.h"

#include <TAxis.h>
#include <TCanvas.h>
#include <TCollection.h>
#include <TError.h>
#include <TFile.h>
#include <TFormula.h>
#include <TGaxis.h>
#include <TH1.h>
#include <TH1D.h>
#include <TH1F.h>
#include <TH2.h>
#include <TH3.h>
#include <THStack.h>
#include <THashList.h>
#include <THn.h>
#include <THnBase.h>
#include <THnSparse.h>
#include <TKey.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TMath.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TProfile.h>
#include <TProfile2D.h>
#include <TProfile3D.h>
#include <TROOT.h>
#include <TString.h>
#include <TVectorD.h>
#include <TVirtualPS.h>
#include <typeinfo>

#include "PairAnalysis.h"
#include "PairAnalysisHelper.h"
#include "PairAnalysisMetaData.h"
#include "PairAnalysisSignalMC.h"
#include "PairAnalysisStyler.h"
#include "PairAnalysisVarManager.h"

ClassImp(PairAnalysisHistos)


  PairAnalysisHistos::PairAnalysisHistos()
  :  //   TCollection(),
  TNamed("PairAnalysisHistos", "PairAnalysis Histogram Container")
  , fHistoList()
  , fMetaData(0x0)
  , fList(0x0)
  , fUsedVars(new TBits(PairAnalysisVarManager::kNMaxValues))
  , fReservedWords(new TString("Hit;Track;Pair"))
  , fPrecision(Eprecision::kFloat)
{
  //
  // Default constructor
  //
  fHistoList.SetOwner(kTRUE);
  fHistoList.SetName("PairAnalysis_Histos");
  PairAnalysisVarManager::InitFormulas();
  PairAnalysisStyler::LoadStyle();
}

//_____________________________________________________________________________
PairAnalysisHistos::PairAnalysisHistos(const char* name, const char* title)
  :  //   TCollection(),
  TNamed(name, title)
  , fHistoList()
  , fMetaData(0x0)
  , fList(0x0)
  , fUsedVars(new TBits(PairAnalysisVarManager::kNMaxValues))
  , fReservedWords(new TString("Hit;Track;Pair"))
  , fPrecision(Eprecision::kFloat)
{
  //
  // TNamed constructor
  //
  fHistoList.SetOwner(kTRUE);
  fHistoList.SetName(name);
  PairAnalysisVarManager::InitFormulas();
  PairAnalysisStyler::LoadStyle();
}

//_____________________________________________________________________________
PairAnalysisHistos::~PairAnalysisHistos()
{
  //
  // Destructor
  //
  fHistoList.Clear();
  if (fUsedVars) delete fUsedVars;
  if (fList) fList->Clear();
  if (fMetaData) delete fMetaData;
  delete fReservedWords;
}

//_____________________________________________________________________________
void PairAnalysisHistos::UserHistogram(const char* histClass, Int_t ndim, TObjArray* limits, UInt_t* vars,
                                       UInt_t valTypeW)
{
  //
  // Histogram creation n>3 dimension only with non-linear binning
  //

  Bool_t isOk = kTRUE;
  isOk &= (ndim < 21 && ndim > 3);
  if (!isOk) {
    Warning("UserHistogram", "Array sizes should be between 3 and 20. Not adding Histogram to '%s'.", histClass);
    return;
  }
  isOk &= (ndim == limits->GetEntriesFast());
  if (!isOk) return;

  // set automatic histo name
  TString name;
  for (Int_t iv = 0; iv < ndim; iv++)
    name += Form("%s_", PairAnalysisVarManager::GetValueName(vars[iv]));
  name.Resize(name.Length() - 1);

  isOk &= IsHistogramOk(histClass, name);

  THnD* hist;
  Int_t bins[ndim];
  if (isOk) {
    // get number of bins
    for (Int_t idim = 0; idim < ndim; idim++) {
      TVectorD* vec = (TVectorD*) limits->At(idim);
      bins[idim]    = vec->GetNrows() - 1;
    }

    hist = new THnD(name.Data(), "", ndim, bins, 0x0, 0x0);

    // set binning
    for (Int_t idim = 0; idim < ndim; idim++) {
      TVectorD* vec = (TVectorD*) limits->At(idim);
      hist->SetBinEdges(idim, vec->GetMatrixArray());
    }

    // store variales in axes
    StoreVariables(hist, vars);
    hist->SetUniqueID(valTypeW);  // store weighting variable

    // store which variables are used
    for (Int_t i = 0; i < ndim; i++)
      fUsedVars->SetBitNumber(vars[i], kTRUE);
    fUsedVars->SetBitNumber(valTypeW, kTRUE);

    Bool_t isReserved = fReservedWords->Contains(histClass);
    if (isReserved) UserHistogramReservedWords(histClass, hist);
    else
      UserHistogram(histClass, hist);
  }
}

//_____________________________________________________________________________
void PairAnalysisHistos::AddSparse(const char* histClass, Int_t ndim, TObjArray* limits, UInt_t* vars, UInt_t valTypeW)
{
  //
  // THnSparse creation with non-linear binning
  //

  Bool_t isOk = kTRUE;
  isOk &= (ndim == limits->GetEntriesFast());
  if (!isOk) return;

  // set automatic histo name
  TString name;
  for (Int_t iv = 0; iv < ndim; iv++)
    name += Form("%s_", PairAnalysisVarManager::GetValueName(vars[iv]));
  name.Resize(name.Length() - 1);

  isOk &= IsHistogramOk(histClass, name);

  //  THnSparseD *hist;
  PairAnalysisHn* hist;
  Int_t bins[ndim];
  if (isOk) {
    // get number of bins
    for (Int_t idim = 0; idim < ndim; idim++) {
      TVectorD* vec = (TVectorD*) limits->At(idim);
      bins[idim]    = vec->GetNrows() - 1;
    }

    //    hist=new THnSparseD(name.Data(),"", ndim, bins, 0x0, 0x0);
    hist = new PairAnalysisHn(name.Data(), "", ndim, bins, 0x0, 0x0);

    // set binning
    for (Int_t idim = 0; idim < ndim; idim++) {
      TVectorD* vec = (TVectorD*) limits->At(idim);
      hist->SetBinEdges(idim, vec->GetMatrixArray());
    }

    // store variales in axes
    StoreVariables(hist, vars);
    hist->SetUniqueID(valTypeW);  // store weighting variable

    // store which variables are used
    for (Int_t i = 0; i < ndim; i++)
      fUsedVars->SetBitNumber(vars[i], kTRUE);
    fUsedVars->SetBitNumber(valTypeW, kTRUE);

    Bool_t isReserved = fReservedWords->Contains(histClass);
    if (isReserved) UserHistogramReservedWords(histClass, hist);
    else
      UserHistogram(histClass, hist);
  }
}

//_____________________________________________________________________________
void PairAnalysisHistos::AddSparse(const char* histClass, Int_t ndim, TObjArray* limits, TFormula** vars,
                                   UInt_t valTypeW)
{
  //
  // THnSparse creation with non-linear binning
  //

  Bool_t isOk = kTRUE;
  isOk &= (ndim == limits->GetEntriesFast());
  if (!isOk) return;

  // set automatic histo name
  TString name;
  for (Int_t iv = 0; iv < ndim; iv++)
    name += Form("%s_", (PairAnalysisHelper::GetFormulaName(vars[iv])).Data());
  name.Resize(name.Length() - 1);
  name.ReplaceAll("f(", "");
  name.ReplaceAll(")", "");
  name.Prepend("f(");
  name.Append(")");

  isOk &= IsHistogramOk(histClass, name);

  //  THnSparseD *hist;
  PairAnalysisHn* hist;
  Int_t bins[ndim];
  if (isOk) {
    // get number of bins
    for (Int_t idim = 0; idim < ndim; idim++) {
      TVectorD* vec = (TVectorD*) limits->At(idim);
      bins[idim]    = vec->GetNrows() - 1;
    }

    //    hist=new THnSparseD(name.Data(),"", ndim, bins, 0x0, 0x0);
    hist = new PairAnalysisHn(name.Data(), "", ndim, bins, 0x0, 0x0);

    // set binning
    for (Int_t idim = 0; idim < ndim; idim++) {
      TVectorD* vec = (TVectorD*) limits->At(idim);
      hist->SetBinEdges(idim, vec->GetMatrixArray());
    }

    // add formulas to list of functions
    for (Int_t idim = 0; idim < ndim; idim++) {
      vars[idim]->SetName(Form("axis%dFormula", idim));
      hist->GetListOfFunctions()->Add(vars[idim]);
    }

    // store variales in axes
    //    StoreVariables(hist, vars);
    hist->SetUniqueID(valTypeW);  // store weighting variable

    // adapt the name and title of the histogram in case they are empty
    AdaptNameTitle(hist, histClass);

    // store which variables are used
    //for(Int_t i=0; i<ndim; i++)   fUsedVars->SetBitNumber(vars[i],kTRUE);
    fUsedVars->SetBitNumber(valTypeW, kTRUE);

    Bool_t isReserved = fReservedWords->Contains(histClass);
    if (isReserved) UserHistogramReservedWords(histClass, hist);
    else
      UserHistogram(histClass, hist);
  }
}

//_____________________________________________________________________________
TString PairAnalysisHistos::UserHistogram(const char* histClass, TObject* hist)
{
  //
  // Add any type of user histogram
  //

  //special case for the calss Pair. where histograms will be created for all pair classes
  Bool_t isReserved = fReservedWords->Contains(histClass);
  if (isReserved) {
    UserHistogramReservedWords(histClass, hist);
    return hist->GetName();
  }

  // if (!IsHistogramOk(histClass,hist->GetName())) return hist->GetName();
  // THashList *classTable=(THashList*)fHistoList.FindObject(histClass);
  //  //  hist->SetDirectory(0);

  // store variables axis
  UInt_t valType[20] = {0};
  // extract variables from axis
  FillVarArray(hist, valType);
  // TODO: implement THn/PairAnalysisHn
  // change to mc truth variables if available
  TString hclass = histClass;
  if (hclass.Contains("MCtruth") && !(hist->IsA() == PairAnalysisHn::Class())) {
    for (Int_t i = 0; i < 2; i++) {
      /// protection for changes of variable enum (in the postprocessing) --> use axistname indentification
      //      UInt_t valTypeFromTitle = 0;
      if (!i) valType[i] = PairAnalysisVarManager::GetValueType(((TH1*) hist)->GetXaxis()->GetName());
      else
        valType[i] = PairAnalysisVarManager::GetValueType(((TH1*) hist)->GetYaxis()->GetName());
      //Printf("SWITCH TO MC: before: %d %s, (via axis name %d) ---->",valType[i],PairAnalysisVarManager::GetValueName(valType[i]),valTypeFromTitle);
      valType[i] = PairAnalysisVarManager::GetValueTypeMC(valType[i]);
      // if theres no corresponding MCtruth variable, skip adding this histogram
      //      if(valType[i] < PairAnalysisVarManager::kNMaxValues && valType[i]>0) return hist->GetName();
      if (valType[i] < PairAnalysisVarManager::kPairMax && valType[i] > 0) return hist->GetName();
      // request filling of mc variable
      fUsedVars->SetBitNumber(valType[i], kTRUE);
      //      Printf("after: %d %s",valType[i],PairAnalysisVarManager::GetValueName(valType[i]));
    }
    StoreVariables(hist, valType);
    hist->SetUniqueID(valType[19]);  // store weighting variable
    // check for formulas
    if (hist->InheritsFrom(TH1::Class())) {
      TIter next(((TH1*) hist)->GetListOfFunctions());
      TFormula* f = 0;
      while ((f = dynamic_cast<TFormula*>(next()))) {
        for (Int_t i = 0; i < f->GetNpar(); i++) {
          Int_t parMC = PairAnalysisVarManager::GetValueTypeMC(f->GetParameter(i));
          // if theres none corresponding MCtruth variable, skip adding this histogram
          if (parMC < PairAnalysisVarManager::kNMaxValues) return hist->GetName();
          f->SetParameter(i, parMC);
          f->SetParName(i, PairAnalysisVarManager::GetValueName(parMC));
          fUsedVars->SetBitNumber(parMC, kTRUE);
        }
      }
      // change histogram key according to mctruth information
      AdaptNameTitle((TH1*) hist, histClass);
    }
  }
  else {
    StoreVariables(hist, valType);
    hist->SetUniqueID(valType[19]);  // store weighting variable
  }

  // add histogram to class
  if (!IsHistogramOk(histClass, hist->GetName())) return hist->GetName();
  THashList* classTable = (THashList*) fHistoList.FindObject(histClass);
  //  hist->SetDirectory(0);
  if (classTable) classTable->Add(hist);
  return hist->GetName();
}

//_____________________________________________________________________________
TH1* PairAnalysisHistos::GetTHist(const char* histClass, const char* name, const char* title,
                                  const TVectorD* const binsX, const TVectorD* const binsY, const TVectorD* const binsZ)
{
  //
  // retrieve n-dimensional Hist depending on arguments
  //
  Bool_t isOk = kTRUE;
  isOk &= IsHistogramOk(histClass, name);
  isOk &= (binsX != 0x0);
  if (!isOk) return 0x0;
  switch (fPrecision) {
    case Eprecision::kFloat:
      if (!binsY) return (new TH1F(name, title, binsX->GetNrows() - 1, binsX->GetMatrixArray()));
      else if (!binsZ)
        return (new TH2F(name, title, binsX->GetNrows() - 1, binsX->GetMatrixArray(), binsY->GetNrows() - 1,
                         binsY->GetMatrixArray()));
      else
        return (new TH3F(name, title, binsX->GetNrows() - 1, binsX->GetMatrixArray(), binsY->GetNrows() - 1,
                         binsY->GetMatrixArray(), binsZ->GetNrows() - 1, binsZ->GetMatrixArray()));
      break;
    case Eprecision::kDouble:
      if (!binsY) return (new TH1D(name, title, binsX->GetNrows() - 1, binsX->GetMatrixArray()));
      else if (!binsZ)
        return (new TH2D(name, title, binsX->GetNrows() - 1, binsX->GetMatrixArray(), binsY->GetNrows() - 1,
                         binsY->GetMatrixArray()));
      else
        return (new TH3D(name, title, binsX->GetNrows() - 1, binsX->GetMatrixArray(), binsY->GetNrows() - 1,
                         binsY->GetMatrixArray(), binsZ->GetNrows() - 1, binsZ->GetMatrixArray()));
      break;
    default: return 0x0; break;
  }
}

//_____________________________________________________________________________
TH1* PairAnalysisHistos::GetTProf(const char* histClass, const char* name, const char* title,
                                  const TVectorD* const binsX, const TVectorD* const binsY, const TVectorD* const binsZ,
                                  TString option)
{
  //
  // retrieve n-dimensional profile histogram with error options depending on arguments
  //
  Bool_t isOk = kTRUE;
  isOk &= IsHistogramOk(histClass, name);
  isOk &= (binsX != 0x0);
  if (!isOk) return 0x0;
  // parse error option
  TString opt   = "";
  Double_t pmin = 0., pmax = 0.;
  if (!option.IsNull()) {
    TObjArray* arr = option.Tokenize(";");
    arr->SetOwner();
    opt = ((TObjString*) arr->At(0))->GetString();
    if (arr->GetEntriesFast() > 1) pmin = (((TObjString*) arr->At(1))->GetString()).Atof();
    if (arr->GetEntriesFast() > 2) pmax = (((TObjString*) arr->At(2))->GetString()).Atof();
    delete arr;
  }
  // build profile with error options and return it
  TH1* prof = 0x0;
  if (!binsY)
    return (new TProfile(name, title, binsX->GetNrows() - 1, binsX->GetMatrixArray(), pmin, pmax, opt.Data()));
  else if (!binsZ) {
    prof = new TProfile2D(name, title, binsX->GetNrows() - 1, binsX->GetMatrixArray(), binsY->GetNrows() - 1,
                          binsY->GetMatrixArray());
    ((TProfile2D*) prof)->BuildOptions(pmin, pmax, opt.Data());
    return prof;
  }
  else {
    prof = new TProfile3D(name, title, binsX->GetNrows() - 1, binsX->GetMatrixArray(), binsY->GetNrows() - 1,
                          binsY->GetMatrixArray(), binsZ->GetNrows() - 1, binsZ->GetMatrixArray());
    ((TProfile3D*) prof)->BuildOptions(pmin, pmax, opt.Data());
    return prof;
  }
}

//_____________________________________________________________________________
TFormula* PairAnalysisHistos::GetFormula(const char* name, const char* formula)
{
  //
  // build a TFormula object
  //
  TFormula* form = new TFormula(name, formula);
  // compile function
  if (form->Compile()) return 0x0;
  //set parameter/variable identifier
  for (Int_t i = 0; i < form->GetNpar(); i++) {
    form->SetParName(i, PairAnalysisVarManager::GetValueName(form->GetParameter(i)));
    fUsedVars->SetBitNumber((Int_t) form->GetParameter(i), kTRUE);
  }
  return form;
}

//_____________________________________________________________________________
void PairAnalysisHistos::AddClass(const char* histClass)
{
  //
  // Add a class of histograms
  // Several classes can be added by separating them by a ';' e.g. 'class1;class2;class3'
  //
  TString hists(histClass);
  TObjArray* arr = hists.Tokenize(";");
  TIter next(arr);
  TObject* o = 0;
  while ((o = next())) {
    if (fHistoList.FindObject(o->GetName())) {
      Warning("AddClass", "Cannot create class '%s' it already exists.", histClass);
      continue;
    }
    if (fReservedWords->Contains(o->GetName())) {
      Error("AddClass", "Pair is a reserved word, please use another name");
      continue;
    }
    THashList* table = new THashList;
    table->SetOwner(kTRUE);
    table->SetName(o->GetName());
    fHistoList.Add(table);
  }
  delete arr;
}

//_____________________________________________________________________________
void PairAnalysisHistos::FillClass(TString histClass, const Double_t* values)
{
  //
  // Fill class 'histClass' (by name)
  //
  THashList* classTable = (THashList*) fHistoList.FindObject(histClass.Data());
  if (!classTable) {
    //    Warning("FillClass","Cannot fill class '%s' its not defined.",histClass.Data());
    return;
  }

  TIter nextHist(classTable);
  TObject* obj = 0;
  while ((obj = (TObject*) nextHist()))
    FillValues(obj, values);

  return;
}

//_____________________________________________________________________________
void PairAnalysisHistos::UserHistogramReservedWords(const char* histClass, const TObject* hist)
{
  //
  // Creation of histogram for all pair or track types
  //
  TString title(hist->GetTitle());

  TIter nextClass(&fHistoList);
  THashList* l = 0;
  while ((l = static_cast<THashList*>(nextClass()))) {
    TString name(l->GetName());
    if (name.Contains(histClass)) {
      TObject* h = hist->Clone();
      // Tobject has no function SetDirectory, didn't we need this???
      //      h->SetDirectory(0);
      if (h->InheritsFrom(TH1::Class())) ((TH1*) h)->SetTitle(Form("%s %s", title.Data(), l->GetName()));
      else
        ((THnBase*) h)->SetTitle(Form("%s %s", title.Data(), l->GetName()));

      UserHistogram(l->GetName(), h);
    }
  }
  delete hist;

  return;
}

//_____________________________________________________________________________
void PairAnalysisHistos::DumpToFile(const char* file)
{
  //
  // Dump the histogram list to a newly created root file
  //
  TFile f(file, "recreate");
  fHistoList.Write(fHistoList.GetName(), TObject::kSingleKey);
  f.Close();
}

//_____________________________________________________________________________
TObject* PairAnalysisHistos::GetHist(const char* histClass, const char* name) const
{
  //
  // return object 'name' in 'histClass'
  //
  THashList* classTable = (THashList*) fHistoList.FindObject(histClass);
  if (!classTable) return 0x0;
  return classTable->FindObject(name);
}

//_____________________________________________________________________________
TH1* PairAnalysisHistos::GetHistogram(const char* histClass, const char* name) const
{
  //
  // return histogram 'name' in 'histClass'
  //
  return ((TH1*) GetHist(histClass, name));
}

//_____________________________________________________________________________
TObject* PairAnalysisHistos::GetHist(const char* cutClass, const char* histClass, const char* name) const
{
  //
  // return object from list of list of histograms
  // this function is thought for retrieving histograms if a list of PairAnalysisHistos is set
  //

  if (!fList) return 0x0;
  THashList* h = dynamic_cast<THashList*>(fList->FindObject(cutClass));
  if (!h) return 0x0;
  THashList* classTable = dynamic_cast<THashList*>(h->FindObject(histClass));
  if (!classTable) return 0x0;
  return classTable->FindObject(name);
}

//_____________________________________________________________________________
TH1* PairAnalysisHistos::GetHistogram(const char* cutClass, const char* histClass, const char* name) const
{
  //
  // return histogram from list of list of histograms
  // this function is thought for retrieving histograms if a list of PairAnalysisHistos is set
  //
  return ((TH1*) GetHist(cutClass, histClass, name));
}

//_____________________________________________________________________________
void PairAnalysisHistos::Draw(const Option_t* option)
{
  //
  // Draw histograms
  //

  TString drawStr(option);
  TObjArray* arr = drawStr.Tokenize(";");
  arr->SetOwner();
  TIter nextOpt(arr);

  TString drawClasses;
  TObjString* ostr = 0x0;

  TString currentOpt;
  TString testOpt;
  while ((ostr = (TObjString*) nextOpt())) {
    currentOpt = ostr->GetString();
    currentOpt.Remove(TString::kBoth, '\t');
    currentOpt.Remove(TString::kBoth, ' ');

    testOpt = "classes=";
    if (currentOpt.Contains(testOpt.Data())) { drawClasses = currentOpt(testOpt.Length(), currentOpt.Length()); }
  }

  delete arr;
  drawStr.ToLower();
  //optionsfList
  //   Bool_t same=drawOpt.Contains("same"); //FIXME not yet implemented

  TCanvas* c = 0x0;
  if (gVirtualPS) {
    if (!gPad) {
      Error("Draw", "When writing to a file you have to create a canvas before opening "
                    "the file!!!");
      return;
    }
    c = gPad->GetCanvas();
    c->cd();
    //     c=new TCanvas;
  }

  TIter nextClass(&fHistoList);
  THashList* classTable = 0;
  //   Bool_t first=kTRUE;
  while ((classTable = (THashList*) nextClass())) {
    //test classes option
    if (!drawClasses.IsNull() && !drawClasses.Contains(classTable->GetName())) continue;
    //optimised division
    Int_t nPads = classTable->GetEntries();
    Int_t nCols = (Int_t) TMath::Ceil(TMath::Sqrt(nPads));
    Int_t nRows = (Int_t) TMath::Ceil((Double_t) nPads / (Double_t) nCols);

    //create canvas
    if (!gVirtualPS) {
      TString canvasName;
      canvasName.Form("c%s_%s", GetName(), classTable->GetName());
      c = (TCanvas*) gROOT->FindObject(canvasName.Data());
      if (!c) c = new TCanvas(canvasName.Data(), Form("%s: %s", GetName(), classTable->GetName()));
      c->Clear();
    }
    else {
      //       if (first){
      //         first=kFALSE;
      //         if (nPads>1) gVirtualPS->NewPage();
      //       } else {
      if (nPads > 1) c->Clear();
      //       }
    }
    if (nCols > 1 || nRows > 1) c->Divide(nCols, nRows);

    //loop over histograms and draw them
    TIter nextHist(classTable);
    Int_t iPad = 0;
    TH1* h     = 0;
    while ((h = (TH1*) nextHist())) {
      TString drawOpt;
      if ((h->InheritsFrom(TH2::Class()))) drawOpt = "colz";
      if (nCols > 1 || nRows > 1) c->cd(++iPad);
      if (TMath::Abs(h->GetXaxis()->GetBinWidth(1) - h->GetXaxis()->GetBinWidth(2)) > 1e-10) gPad->SetLogx();
      if (TMath::Abs(h->GetYaxis()->GetBinWidth(1) - h->GetYaxis()->GetBinWidth(2)) > 1e-10) gPad->SetLogy();
      if (TMath::Abs(h->GetZaxis()->GetBinWidth(1) - h->GetZaxis()->GetBinWidth(2)) > 1e-10) gPad->SetLogz();
      TString histOpt = h->GetOption();
      histOpt.ToLower();
      if (histOpt.Contains("logx")) gPad->SetLogx();
      if (histOpt.Contains("logy")) gPad->SetLogy();
      if (histOpt.Contains("logz")) gPad->SetLogz();
      histOpt.ReplaceAll("logx", "");
      histOpt.ReplaceAll("logy", "");
      histOpt.ReplaceAll("logz", "");
      h->Draw(drawOpt.Data());
    }
    if (gVirtualPS) { c->Update(); }
  }
  //   if (gVirtualPS) delete c;
}

//_____________________________________________________________________________
void PairAnalysisHistos::Print(const Option_t* option) const
{
  //
  // Print classes and histograms
  //
  TString optString(option);

  if (optString.IsNull()) PrintStructure();
}

//_____________________________________________________________________________
void PairAnalysisHistos::PrintStructure() const
{
  //
  // Print classes and histograms in the class to stdout
  //
  if (!fList) {
    TIter nextClass(&fHistoList);
    THashList* classTable = 0;
    while ((classTable = (THashList*) nextClass())) {
      TIter nextHist(classTable);
      TObject* o = 0;
      Printf("+ %s\n", classTable->GetName());
      while ((o = nextHist()))
        Printf("| ->%s\n", o->GetName());
    }
  }
  else {
    TIter nextCutClass(fList);
    THashList* cutClass = 0x0;
    while ((cutClass = (THashList*) nextCutClass())) {
      TString cla = cutClass->GetName();
      if (cla.Contains("QAcuts")) continue;
      Printf("+ %s\n", cutClass->GetName());
      TIter nextClass(cutClass);
      THashList* classTable = 0;
      while ((classTable = (THashList*) nextClass())) {
        TIter nextHist(classTable);
        TObject* o = 0;
        Printf("|  + %s\n", classTable->GetName());
        while ((o = nextHist()))
          Printf("|  | ->%s\n", o->GetName());
      }
    }
  }
}

//_____________________________________________________________________________
void PairAnalysisHistos::SetHistogramList(THashList& list, Bool_t setOwner /*=kTRUE*/)
{
  //
  // set histogram classes and histograms to this instance. It will take onwnership!
  //
  ResetHistogramList();
  TString name(GetName());
  //  if (name == "PairAnalysisHistos")
  SetName(list.GetName());
  TIter next(&list);
  TObject* o;
  while ((o = next())) {
    fHistoList.Add(o);
  }
  if (setOwner) {
    list.SetOwner(kFALSE);
    fHistoList.SetOwner(kTRUE);
    fHistoList.SetName(list.GetName());
  }
  else {
    fHistoList.SetOwner(kFALSE);
    fHistoList.SetName(list.GetName());
  }
}

//_____________________________________________________________________________
Bool_t PairAnalysisHistos::SetCutClass(const char* cutClass)
{
  //
  // Assign histogram list according to cutClass
  //

  if (!fList) return kFALSE;
  ResetHistogramList();
  THashList* h = dynamic_cast<THashList*>(fList->FindObject(cutClass));
  if (!h) {
    Warning("SetCutClass", "cutClass '%s' not found", cutClass);
    return kFALSE;
  }
  SetHistogramList(*h, kFALSE);
  return kTRUE;
}
//_____________________________________________________________________________
Bool_t PairAnalysisHistos::IsHistogramOk(const char* histClass, const char* name)
{
  //
  // check whether the histogram class exists and the histogram itself does not exist yet
  //
  Bool_t isReserved = fReservedWords->Contains(histClass);
  if (!fHistoList.FindObject(histClass) && !isReserved) {
    Warning("IsHistogramOk",
            "Cannot create histogram. Class '%s' not defined. Please create it "
            "using AddClass before.",
            histClass);
    return kFALSE;
  }
  if (GetHist(histClass, name)) {
    Warning("IsHistogramOk", "Cannot create histogram '%s' in class '%s': It already exists!", name, histClass);
    return kFALSE;
  }
  return kTRUE;
}

// //_____________________________________________________________________________
// TIterator* PairAnalysisHistos::MakeIterator(Bool_t dir) const
// {
//   //
//   //
//   //
//   return new TListIter(&fHistoList, dir);
// }

//_____________________________________________________________________________
void PairAnalysisHistos::ReadFromFile(const char* file, const char* task, const char* config)
{
  //
  // Read histos from file
  //
  TFile f(file);
  TIter nextKey(f.GetListOfKeys());
  TKey* key = 0;
  while ((key = (TKey*) nextKey())) {
    TString name = key->GetName();
    // check for meta data
    if (name.Contains(Form("PairAnalysisMetaData_%s", task))) {
      fMetaData = new PairAnalysisMetaData();
      fMetaData->SetMetaData(*dynamic_cast<TList*>(f.Get(key->GetName())), kFALSE);
    }
    // check for histos
    if (!name.Contains(Form("PairAnalysisHistos_%s", task))) continue;
    if (!strlen(task) && !name.Contains(task)) continue;
    TObject* o  = f.Get(key->GetName());
    TList* list = dynamic_cast<TList*>(o);
    if (!list) continue;
    else
      fList = list;
    THashList* listCfg = dynamic_cast<THashList*>(list->FindObject(config));
    if (!listCfg) continue;
    SetHistogramList(*listCfg);
    break;
  }
  f.Close();
  // load style
  PairAnalysisStyler::LoadStyle();
}

//_____________________________________________________________________________
TObjArray* PairAnalysisHistos::DrawTaskSame(TString histName, TString opt, TString histClassDenom, TString taskDenom)
{
  ///
  /// Draw histograms of different tasks into the same canvas
  ///
  /// additional plotting options to DrawSame:
  /// "selcfg":     option to (de-)select certain task/config specified in "histClassDenom",
  ///               delimiters ':;,' are recognized, exclusion done by prepending '!'
  /// "cutstep":    histograms of the cut steps are compared for a single task (see AnalysisFilter::AddHistos)
  ///
  /// if argument "taskDenom" is provided histogram ratios to this task/config are calculated


  TObjArray* selections = histClassDenom.Tokenize(":;,");

  opt.ToLower();
  TString optString(opt);
  Bool_t optGoff    = optString.Contains("goff");
  Bool_t optEff     = optString.Contains("eff");
  Bool_t optCutStep = optString.Contains("cutstep");
  Bool_t optSelCfg  = optString.Contains("selcfg");
  opt.ReplaceAll("selcfg", "");
  fHistoList.SetOwner(kFALSE);

  /// output array
  TObjArray* arr = NULL;
  if (optGoff) Info("DrawTaskSame", "graphics option off, collect an array");
  {
    //    arr=(TObjArray*)gROOT->FindObject(Form("%s",histName.Data()));
    //arr=(TObjArray*)gROOT->FindObject(GetName());
    //    if(arr) { Warning("DrawTaskSame","Clear existing array %s",GetName()); arr->ls(); arr->Clear(); }
    //    else
    arr = new TObjArray();
    //    arr->SetName(Form("%s",histName.Data()));
    arr->SetName(GetName());
    arr->SetOwner(kFALSE);
  }

  /// legend
  TString legendname = GetName();
  //  printf("DrawTaskSame: legendname %s\n",legendname.Data());

  /// reserved words
  TObjArray* reservedWords = fReservedWords->Tokenize(":;");

  /// find cut step lists if option 'cutstep' is active
  if (optCutStep) {
    TString cutstepTask    = fHistoList.GetName();
    THashList* listCutStep = dynamic_cast<THashList*>(fList->FindObject(cutstepTask));
    if (listCutStep) fList = listCutStep;
  }

  /// find denominator list 'taskDenom'
  THashList* listDenom = dynamic_cast<THashList*>(fList->FindObject(taskDenom.Data()));
  if (listDenom) opt += "div";

  /// iterate over all tasks and their lists in the file
  TIter nextCfg(fList);
  THashList* listCfg = 0;
  while ((listCfg = static_cast<THashList*>(nextCfg()))) {

    TString lname = listCfg->GetName();

    /// exclude QAcuts
    if (lname.Contains("QAcuts_")) continue;
    Info("DrawTaskSame", " Task name %s ", lname.Data());

    /// skip same config if argument 'taskDenom' is set
    if (!optEff && listDenom && lname.EqualTo(taskDenom)) continue;

    /// skip configs according to selection done via histClassDenom and option 'selcfg'
    if (optSelCfg && selections->GetEntriesFast()) {
      Bool_t pass = kFALSE;
      for (Int_t is = 0; is < selections->GetEntriesFast(); is++) {
        Bool_t testIgnore = kFALSE;
        TString raw       = ((TObjString*) selections->At(is))->GetString();

        //	printf("sel: '%s' \n",  histClassDenom.Data());

        /// check if selection string contains reserved word
        /// if true ignore it for config selection
        for (Int_t ir = 0; ir < reservedWords->GetEntriesFast(); ir++) {
          testIgnore = raw.Contains(((TObjString*) reservedWords->At(ir))->GetString());
          if (testIgnore) break;
        }
        if (testIgnore) continue;

        TString srch = raw;
        srch.ReplaceAll("!", "");
        Bool_t optExclSel = !(srch.EqualTo(raw));  // exclude or not
        // printf("selection %d/%d: raw '%s' \t search: '%s' exclude: %d compare to: '%s' \t pass %d \n",
        //        is,selections->GetEntriesFast()-1,raw.Data(),srch.Data(),optExclSel,lname.Data(),pass);
        // printf("\t decision !(%d^%d)=%d  \n",(!lname.EqualTo(srch)),(optExclSel),!(!lname.EqualTo(srch))^(optExclSel));
        /// decision
        if (!(!lname.EqualTo(srch)) ^ (optExclSel)) pass = kTRUE;
        /// same config found - direct decision (skip other selections)
        if (lname.EqualTo(srch)) {
          pass = !(!lname.EqualTo(srch)) ^ (optExclSel);
          // printf("\t same config found -> stop with pass: %d \n",pass);
          break;
        }
        // remove config selection string abgearbeitet
        //      histClassDenom.ReplaceAll(raw.Data(),"");
      }
      if (!pass) continue;
    }

    /// keep only legend name!=task name
    if (lname.EqualTo(legendname)) legendname = "";

    /// update current histogram list
    ResetHistogramList();
    TIter next(listCfg);
    Int_t idx = 0;
    TObject* o;
    while ((o = next())) {
      fHistoList.AddAt(o, idx++);
    }
    //fHistoList.Print();

    /// adapt name for legend
    SetName(listCfg->GetName());
    arr->AddAll(DrawSame(histName, (opt + "task").Data(), histClassDenom, listDenom));
  }

  /// set legend name
  if (optString.Contains("leg")) {
    //    printf("DrawTaskSame: SET legendname %s\n",legendname.Data());
    legendname.ReplaceAll(".", "");
    TList* prim  = gPad->GetListOfPrimitives();
    TLegend* leg = (TLegend*) prim->FindObject("TPave");
    // remove legend header from legend entry
    TList* llist = leg->GetListOfPrimitives();
    Int_t nent   = llist->GetEntries();
    for (Int_t il = 0; il < nent; il++) {
      TLegendEntry* lent = static_cast<TLegendEntry*>(llist->At(il));
      TString lst(lent->GetLabel());
      lst.ReplaceAll(legendname.Data(), "");
      lent->SetLabel(lst.Data());
    }

    if (!legendname.EqualTo("none")) leg->SetHeader("");
    else
      leg->SetHeader(legendname.Data());
    PairAnalysisStyler::SetLegendAttributes(leg);  // coordinates, margins, fillstyle, fontsize
    leg->Draw();
  }

  /// cleanup
  if (selections) delete selections;
  if (reservedWords) delete reservedWords;
  if (!optGoff) delete arr;

  return arr;
}

//_____________________________________________________________________________
TObjArray* PairAnalysisHistos::DrawSame(TString histName, TString option, TString histClassDenom, THashList* listDenom)
{
  ///
  /// Draw histograms with the same histName into a canvas
  ///
  /// additional plotting options to TH1::Draw():
  /// "back":       iterate backwards over the histClasses
  /// "goff":       graphics are switched off & an array with the objects is returned
  /// "task":       histograms of different tasks are compared (see DrawTaskSame)
  ///
  /// "div":        histograms of different tasks are divided (see DrawTaskSame) INTERNAL?
  /// "eff":        efficiencies are calculated
  /// "ratio":      ratios of any histclass to "histClassDenom" are calculated
  /// "oneOver":    the histogram is rescaled by 1./(bin contents)
  /// "stack":      build and plot a stack histogram
  ///
  /// "noMc":       no mc signals are plotted
  /// "noMcTrue":   no mc truth signals are plotted
  /// "onlyMc":     only mc signals are plotted
  /// "onlyMcTrue": only mc truth signals are plotted
  /// "sel":        option to (de-)select certain histClasses specified in "histClassDenom",
  ///               delimiters ":;," are recognized, exclusion done by prepending "!"
  ///
  /// "can":        a new canvas is created with name: "c"+"histName"
  /// "logx,y,z":   the axis are plotted in log-scale (labels are added automatically according to the range)
  /// "meta"        adds default meta data to the pad (see PairAnalysisMetaData::DrawSame)
  /// "legF":       (option "F":filled) legend will be created with caption=className ,
  ///               can be modified by PairAnalysisHistos::SetName("mycaption"),
  ///               change of legend position: see PairAnalysisStyler::SetLegendAlign
  /// "meanX,Y":    quote the mean in the legend (Y only works for TProfile a.t.m.)
  /// "rmsX,Y":     quote the rms in the legend (Y only works for TProfile a.t.m.)
  /// "det":        adds the detector name to the legend for detector specific histograms (e.g. hit histograms)
  ///
  /// "width":      scale histograms by 1./bin width according to TH1::Scale
  /// "rebinstat0.XY":  the objects are rebinned to have <"0.XY" stat uncertainty in each bin (max two digits)
  /// "rebinX":     rebins the histogram along x-axis by factor "X" (for X<10)
  /// "sclmax":     scale the histogram by 1./maximum
  /// "norm":       histogram is normalized to 1.
  /// "normY":      2D-histograms are normalized in x-axis slices to 1.
  /// "cum":        calculate cumulated sum of bins (+under- & overflow) for 1D-histograms (left-to-right)
  /// "cumR":       calculate cumulated sum of bins (+under- & overflow)for 1D-histograms ("R"=reverse: right-to-left)
  /// "events":     use number of selected (after cuts) events in meta data to normalize the histograms
  /// "smoothX":    smooths the histogram along x-axis by factor "X" (for X<10)
  ///
  /// "rststy":     reset style index to zero (usefull when the same color code should be plotted on top of each other)
  /// "geant":      translate geantId to geant process names (see PairAnalysisHelper::SetGEANTBinLabels)
  /// "pdg":        translate bin low edges into pdg label
  /// "pdgc":       translate bin low edges into pdg label for bins with entries

  TString optString(option);
  optString.ToLower();
  printf("Plot hist: '%s' class-denom/sel: '%s' \t listDenom: '%s' \t options: "
         "'%s' \n",
         histName.Data(), histClassDenom.Data(), (listDenom ? listDenom->GetName() : ""), optString.Data());
  Bool_t optBack = optString.Contains("back");
  optString.ReplaceAll("back", "");
  Bool_t optGoff = optString.Contains("goff");
  optString.ReplaceAll("goff", "");
  Bool_t optTask = optString.Contains("task");
  optString.ReplaceAll("task", "");
  Bool_t optCutStep = optString.Contains("cutstep");
  optString.ReplaceAll("cutstep", "");
  /// options - calulation
  Bool_t optDiv = optString.Contains("div");
  optString.ReplaceAll("div", "");
  Bool_t optEff = optString.Contains("eff");
  optString.ReplaceAll("eff", "");
  Bool_t optRatio = optString.Contains("ratio");
  optString.ReplaceAll("ratio", "");
  Bool_t optOneOver = optString.Contains("oneover");
  optString.ReplaceAll("oneover", "");
  /// options - selection
  Bool_t optNoMCtrue = optString.Contains("nomctrue");
  optString.ReplaceAll("nomctrue", "");
  Bool_t optNoMC = optString.Contains("nomc");
  optString.ReplaceAll("nomc", "");
  Bool_t optOnlyMCtrue = optString.Contains("onlymctrue");
  optString.ReplaceAll("onlymctrue", "");
  Bool_t optOnlyMC = optString.Contains("onlymc");
  optString.ReplaceAll("onlymc", "");
  Bool_t optSel = optString.Contains("sel");
  optString.ReplaceAll("sel", "");
  /// options - representation
  Bool_t optCan = optString.Contains("can");
  optString.ReplaceAll("can", "");
  Bool_t optLegFull = optString.Contains("legf");
  optString.ReplaceAll("legf", "");
  Bool_t optLeg = optString.Contains("leg");
  optString.ReplaceAll("leg", "");
  Bool_t optDet = optString.Contains("det");
  optString.ReplaceAll("det", "");
  Bool_t optMeta = optString.Contains("meta");
  optString.ReplaceAll("meta", "");
  Bool_t optWdth = optString.Contains("width");
  optString.ReplaceAll("width", "");
  Bool_t optRbnStat = optString.Contains("rebinstat");
  optString.ReplaceAll("rebinstat", "stat");
  Bool_t optRbn    = optString.Contains("rebin");
  Bool_t optSmooth = optString.Contains("smooth");
  Bool_t optSclMax = optString.Contains("sclmax");
  optString.ReplaceAll("sclmax", "");
  Bool_t optNormY = optString.Contains("normy");
  optString.ReplaceAll("normy", "");
  Bool_t optNorm = optString.Contains("norm");
  optString.ReplaceAll("norm", "");
  Bool_t optCumR = optString.Contains("cumr");
  optString.ReplaceAll("cumr", "");
  Bool_t optCum = optString.Contains("cum");
  optString.ReplaceAll("cum", "");
  Bool_t optEvt = optString.Contains("events");
  optString.ReplaceAll("events", "");
  Bool_t optStack = optString.Contains("stack");
  optString.ReplaceAll("stack", "");
  Bool_t optRstSty = optString.Contains("rststy");
  optString.ReplaceAll("rststy", "");
  Bool_t optSlicesY = optString.Contains("slicesy");
  optString.ReplaceAll("slicesy", "");
  /// options - information
  Bool_t optMeanX = optString.Contains("meanx");
  optString.ReplaceAll("meanx", "");
  Bool_t optRmsX = optString.Contains("rmsx");
  optString.ReplaceAll("rmsx", "");
  Bool_t optMeanY = optString.Contains("meany");
  optString.ReplaceAll("meany", "");
  Bool_t optRmsY = optString.Contains("rmsy");
  optString.ReplaceAll("rmsy", "");
  Bool_t optGeant = optString.Contains("geant");
  optString.ReplaceAll("geant", "");
  Bool_t optPdgClean = optString.Contains("pdgc");
  optString.ReplaceAll("pdgc", "");
  Bool_t optPdg = optString.Contains("pdg");
  optString.ReplaceAll("pdg", "");

  /// set rebinning
  Int_t rbn = 2;
  if (optRbn) {
    TString rebin(optString(optString.Index("rebin", 5, 0, TString::kExact) + 5));
    if (rebin.IsDigit()) {
      rbn = rebin.Atoi();
      optString.ReplaceAll(rebin, "");
    }
    optString.ReplaceAll("rebin", "");
  }
  /// set stat. uncertainty limit for rebinning
  /// NOTE: first check for one digit, then 2 digits
  TArrayD* limits = NULL;
  Double_t stat   = 0.8;
  if (optRbnStat) {
    TString rebin(optString(optString.Index("stat", 4, 0, TString::kExact) + 4, 4));
    if (!rebin.IsFloat()) rebin = optString(optString.Index("stat", 4, 0, TString::kExact) + 4, 3);
    if (rebin.IsFloat()) {
      stat = rebin.Atof();
      //    printf("rebinstat string: '%s' -> %f\n",rebin.Data(),stat);
      optString.ReplaceAll(rebin, "");
    }
    optString.ReplaceAll("stat", "");
  }

  /// set smoothing
  Int_t smth = 1;
  if (optSmooth) {
    TString smooth(optString(optString.Index("smooth", 6, 0, TString::kExact) + 6));
    if (smooth.IsDigit()) {
      smth = smooth.Atoi();
      optString.ReplaceAll(smooth, "");
    }
    optString.ReplaceAll("smooth", "");
  }

  /// activate std option for legend and conncetions
  if (optLegFull) optLeg = kTRUE;
  if (optCumR) optCum = kTRUE;

  /// selection string
  TString select("");
  TObjArray* selections = (optSel ? histClassDenom.Tokenize(":;,") : NULL);
  if (optSel) histClassDenom = "";

  /// output array
  TObjArray* arr = 0x0;
  if (optGoff) {
    Info("DrawSame", "graphics option off, collect an array");
    //    arr=(TObjArray*)gROOT->FindObject(Form("%s",histName.Data()));
    arr = (TObjArray*) gROOT->FindObject(GetName());
    if (arr) {
      Warning("DrawSame", "Clear existing array %s", GetName());
      arr->ls();
      arr->Clear();
    }
    else
      arr = new TObjArray();
    //    arr->SetName(Form("%s",histName.Data()));
    arr->SetName(GetName());
    arr->SetOwner(kFALSE);
  }

  /// stack option
  THStack* hs = NULL;
  // if(optStack) {
  //   hs = new THStack("hs","stacked histograms");
  // }

  /// add canvas
  TCanvas* c = 0;
  if (optCan) {
    c = (TCanvas*) gROOT->FindObject(Form("c%s", histName.Data()));
    if (!c) { c = new TCanvas(Form("c%s", histName.Data()), Form("All '%s' histograms", histName.Data())); }
    // c->Clear();
    c->cd();
  }

  /// count number of drawn objects in pad
  TH1* hFirst = 0x0;
  TObject* obj;
  Int_t nobj  = 0;
  TList* prim = (gPad ? gPad->GetListOfPrimitives() : 0x0);
  //  if(prim) prim->ls();
  //  if(prim && prim->GetSize()>1 && !optGoff) prim->RemoveLast(); // remove redraw axis histogram
  for (Int_t io = 0; io < (prim ? prim->GetSize() : 0); io++) {
    obj = prim->At(io);
    if (obj->InheritsFrom(TH1::Class()) && obj != prim->At(io + 1)) nobj++;
    if (obj->InheritsFrom(THStack::Class()) && obj != prim->At(io + 1)) nobj++;
    if (nobj == 1) hFirst = (TH1*) obj;
  }

  /// add or get legend
  TLegend* leg = 0;
  if ((optLeg && !nobj)) {
    leg = new TLegend(0. + gPad->GetLeftMargin() + gStyle->GetTickLength("Y"),
                      0. + gPad->GetBottomMargin() + gStyle->GetTickLength("X"),
                      1. - gPad->GetRightMargin() + gStyle->GetTickLength("Y"),
                      1. - gPad->GetTopMargin() + gStyle->GetTickLength("X"), GetName(), "nbNDC");
    if (optTask && !optCutStep) leg->SetHeader("");
  }
  else if (nobj) {
    leg = (TLegend*) prim->FindObject("TPave");
    // leg->SetX1(0. + gPad->GetLeftMargin()  + gStyle->GetTickLength("Y"));
    // leg->SetY1(0. + gPad->GetBottomMargin()+ gStyle->GetTickLength("X"));
    // leg->SetX2(1. - gPad->GetRightMargin() + gStyle->GetTickLength("Y"));
    // leg->SetY2(1. - gPad->GetTopMargin()   + gStyle->GetTickLength("X"));
    // leg->SetOption("nbNDC");
  }

  Info("DrawSame", "Basics: nobj: %d \t leg: %p", nobj, leg);

  /// logaritmic axes
  if (optString.Contains("logx")) gPad->SetLogx();
  if (optString.Contains("logy")) gPad->SetLogy();
  if (optString.Contains("logz")) gPad->SetLogz();
  optString.ReplaceAll("logx", "");
  optString.ReplaceAll("logy", "");
  optString.ReplaceAll("logz", "");

  /// meta data
  Int_t events = 1;
  if (fMetaData && optEvt) fMetaData->GetMeta("events", &events);

  Int_t i = nobj;
  if (optTask && nobj) i = nobj;
  TListIter next(&fHistoList, (optBack ? kIterBackward : kIterForward));
  //TIter next(&fHistoList);
  THashList* classTable = 0;
  //  TH1 *hFirst=0x0;
  /// iterate over all histogram classes (events,pairs,tracks,hits)
  while ((classTable = (THashList*) next())) {
    TString histClass = classTable->GetName();

    Int_t ndel = histClass.CountChar('_');
    Info("DrawSame",
         "Search for hist: '%s' class-denom: '%s' select: '%s' \t ndel: %d \t "
         "for class: '%s'",
         histName.Data(), histClassDenom.Data(), select.Data(), ndel, histClass.Data());

    /// check selections done via MC options 'onlyMC', 'noMC', 'noMCtrue', 'eff'
    if ((optNoMC && ndel > 0) || (optEff && ndel < 1) || (optNoMCtrue && histClass.Contains("_MCtruth"))
        || (optOnlyMC && ndel < 1) || (optOnlyMCtrue && ndel < 2))
      continue;

    /// histclass selections done via option 'sel' and argument 'histClassDenom' -> into array selections
    if (optSel && selections->GetEntriesFast()) {
      Bool_t pass = kFALSE;
      for (Int_t is = 0; is < selections->GetEntriesFast(); is++) {
        TString raw  = ((TObjString*) selections->At(is))->GetString();
        TString srch = raw;
        srch.ReplaceAll("!", "");
        Bool_t optExclSel = !(srch.EqualTo(raw));  // exclude or not
        /// decision
        if (!(!histClass.Contains(srch, TString::kIgnoreCase)) ^ (optExclSel)) pass = kTRUE;
        /// exact string found - direct decision (ignore other selections)
        if (histClass.EqualTo(srch, TString::kIgnoreCase)) {
          pass = !(!histClass.EqualTo(srch, TString::kIgnoreCase)) ^ (optExclSel);
          break;
        }
      }
      if (!pass) continue;
    }


    /// find the histogram 'histName' in the class table
    /// or the corressponding MC histogram
    TH1* h = (TH1*) classTable->FindObject(histName.Data());
    if (!h && !optNoMC && !optNoMCtrue) {
      /// TODO: improve for tprofiles, formulas and more dimensional histos
      TString histdenomMC = histName + "MC";
      h                   = (TH1*) classTable->FindObject(histdenomMC.Data());
    }
    if (!h) continue;
    if (h->GetEntries() < 1.) continue;

    /// get histClassDenom for efficiency caluclation, e.g. the MCtruth (denominator)
    if (optEff && !histClass.Contains("_MCtruth")) histClassDenom = histClass + "_MCtruth";
    Info("DrawSame", " Hist found in histClass '%s' (search for denom '%s') ", histClass.Data(), histClassDenom.Data());

    /// check if 'ratio' or 'eff' should be build
    if ((optEff || optRatio) /*&& !optTask*/
        && (histClass.EqualTo(histClassDenom) || !fHistoList.FindObject(histClassDenom.Data())))
      continue;  /// TODO: why  '!optTask' was needed?
    else if (!histClassDenom.IsNull())
      Info("DrawSame", " Denom histClass '%s' found ", histClassDenom.Data());

    /// set first histogram
    if (i == 0 && !hFirst) hFirst = h;

    /// print normalisation option
    if (optRbn || optRbnStat)
      Info("DrawSame", " Rebin by %d, to <%.1f%% stat. uncertainty per bin", (optRbn ? rbn : 0),
           (optRbnStat ? stat * 100 : 0));
    if (optNormY || optNorm || optEvt)
      Info("DrawSame", " Normalize in y-axis,2D's only(%d), by int.(%d), by #events(%d)", optNormY, optNorm, optEvt);
    if (optSclMax) Info("DrawSame", " Scale to maximum(%d)", optSclMax);
    if (optCum)
      Info("DrawSame",
           " Cumulate sum of bins along x-axis, 1D's left-to-right(%d), "
           "right-to-left(%d)",
           !optCumR, optCumR);

    /// rebin, normalize, cumulate and scale spectra according to options 'rebinX','rebinStat','norm','normY','cum','cumR','events','sclMax'
    h->Sumw2();
    if (optRbn && h->InheritsFrom(TH2::Class())) h = ((TH2*) h)->RebinX(rbn, h->GetName());
    else if (optRbn)
      h->Rebin(rbn);
    if (optNormY && h->GetDimension() == 2 && !(h->GetSumOfWeights() == 0) && !optCum)
      PairAnalysisHelper::NormalizeSlicesY((TH2*) h);
    if (optRbnStat && h->GetDimension() == 1) {
      /// rebin until stat. uncertainty is lower than 'stat'
      limits = PairAnalysisHelper::MakeStatBinLimits(h, stat);
      h      = h->Rebin(limits->GetSize() - 1, h->GetName(), limits->GetArray());
      h->Scale(1., "width");
      //      delete limits;
    }
    if (optCum) PairAnalysisHelper::Cumulate(h, optCumR, optNorm);
    if (optSmooth) h->Smooth(smth);

    /// get default histogram titles
    TString ytitle = h->GetYaxis()->GetTitle();
    TString ztitle = h->GetZaxis()->GetTitle();
    if (ytitle.Contains("{evt}")) optEvt = kFALSE;

    if (optNorm && !(h->GetSumOfWeights() == 0) && !optCum)
      h = h->DrawNormalized(i > 0 ? (optString + "same").Data() : optString.Data());
    if (optEvt) h->Scale(1. / events);
    if (optSclMax) h->Scale(1. / h->GetBinContent(h->GetMaximumBin()));

    /// modify titles according to options
    switch (h->GetDimension()) {
      case 1:
        if (optEvt) h->SetYTitle((ytitle + "/N_{evt}").Data());
        if (optNorm) h->SetYTitle((ytitle.Append(" (normalized)")).Data());
        if (optCum) h->SetYTitle((ytitle.Prepend("cumulated ")).Data());
        if (optRatio) h->SetYTitle("ratio");
        if (optDiv) h->SetYTitle("ratio");
        if (optEff) h->SetYTitle("acceptance #times efficiency");
        if (optSclMax) h->SetYTitle((ytitle + "/N_{max}").Data());
        break;
      case 2:
        if (optEvt) h->SetZTitle((ztitle + "/N_{evt}").Data());
        //	if(optNormY)  h->SetYTitle( (ytitle.Append(" (normalized)")).Data() );
        if (optNorm) h->SetZTitle((ztitle.Prepend("normalized ")).Data());
        if (optRatio) h->SetZTitle("ratio");
        if (optDiv) h->SetZTitle("ratio");
        if (optEff) h->SetZTitle("acceptance #times efficiency");
        if (optSclMax) h->SetZTitle((ztitle + "/N_{max}").Data());
        break;
    }

    /// Calculate ratios and Draw histograms
    if ((optEff || optRatio) && !optNorm && !optEvt && !optTask) {
      Info("DrawSame", " Calculate '%s' w/o normalisation and within the same task", (optEff ? "efficiency" : "ratio"));
      //	TString    clMC     = histClass+"_MCtruth";
      THashList* clDenom  = (THashList*) fHistoList.FindObject(histClassDenom.Data());
      TH1* hMC            = (TH1*) h->Clone();  // needed to preserve the labeling of non-mc histogram
      TString histdenomMC = UserHistogram(histClassDenom.Data(), hMC);
      TH1* hdenom         = (TH1*) clDenom->FindObject(histdenomMC.Data());
      if (!hdenom) {
        Error("DrawSame", "Denominator object not found");
        continue;
      }
      Info("DrawSame", " Divide %s(#=%.3e) by %s(#=%.3e)", h->GetName(), h->GetEntries(), hdenom->GetName(),
           hdenom->GetEntries());
      delete hMC;  //delete the surplus object
      // normalize and rebin only once
      hdenom->Sumw2();  //why is it crashing here
      if (optRbnStat && (optEff || !(i % 10))) {
        hdenom = hdenom->Rebin(limits->GetSize() - 1, hdenom->GetName(), limits->GetArray());
        hdenom->Scale(1., "width");
      }
      if (optRbn && (optEff || !(i % 10))) hdenom->RebinX(rbn);
      if (optEvt && (optEff || !(i % 10))) hdenom->Scale(1. / events);
      if (!hdenom || !h->Divide(hdenom)) {
        Warning("DrawSame(eff/ratio)", "Division failed!!!!");
        continue;
      }
    }
    else if (optTask && (optDiv || optEff || optRatio)) {
      Info("DrawSame", " Calculate '%s' using different tasks",
           (optEff ? "efficiency" : (optRatio ? "ratio" : "divison")));
      // denominators
      TH1* hdenom = 0x0;
      TH1* htden  = 0x0;
      if (optEff || optRatio) {
        THashList* clDenom  = (THashList*) fHistoList.FindObject(histClassDenom.Data());
        TH1* hMC            = (TH1*) h->Clone();  // needed to preserve the labeling of non-mc histogram
        TString histdenomMC = UserHistogram(histClassDenom.Data(), hMC);
        //delete the surplus object
        delete hMC;

        if (clDenom) { hdenom = (TH1*) clDenom->FindObject(histdenomMC.Data()); }

        if (listDenom) {
          THashList* clTaskDen = (THashList*) listDenom->FindObject(histClassDenom.Data());
          if (clTaskDen) {
            //	  htden=(TH1*)clTaskDen->FindObject(hdenom->GetName());
            htden = (TH1*) clTaskDen->FindObject(histdenomMC.Data());
            Info("DrawSame",
                 "calculate eff/ratio using task-denom: '%s' class-denom: '%s' "
                 "hist-denom: '%s'",
                 listDenom->GetName(), histClassDenom.Data(), histdenomMC.Data());
            // keep only one of them, otherwise you might divide the same objects twice
            if (htden) hdenom = 0x0;
          }
        }

      }  // optEff || optRatio


      // task ratio
      TH1* htnom = 0x0;
      if (optDiv && !optEff) {
        Info("DrawSame", " Search for '%s' in task '%s'", histClass.Data(), listDenom->GetName());
        THashList* clTaskNom = (THashList*) listDenom->FindObject(histClass.Data());
        if (!clTaskNom) continue;
        htnom = (TH1*) clTaskNom->FindObject(histName.Data());
        if (!htnom) continue;
      }

      if (hdenom) hdenom->Sumw2();
      if (htden) htden->Sumw2();
      if (htnom) htnom->Sumw2();

      /// check consistent bins and rebin if needed
      Int_t nbinsh  = (h ? h->GetNbinsX() : -1);
      Int_t nbinsd  = (hdenom ? hdenom->GetNbinsX() : -2);
      Int_t nbinstd = (htden ? htden->GetNbinsX() : -3);
      Int_t nbinstn = (htnom ? htnom->GetNbinsX() : -4);

      // normalize and rebin only once
      if (optRbn) {
        if (hdenom && nbinsd > nbinsh) hdenom->RebinX(rbn);
        if (htden && nbinstd > nbinsh) htden->RebinX(rbn);
        if (htnom && nbinstn > nbinsh) htnom->RebinX(rbn);
      }
      if (optRbnStat) {
        if (hdenom && nbinsd > nbinsh) {
          hdenom = hdenom->Rebin(limits->GetSize() - 1, hdenom->GetName(), limits->GetArray());
          hdenom->Scale(1., "width");
        }
        if (htden && nbinstd > nbinsh) {
          htden = htden->Rebin(limits->GetSize() - 1, htden->GetName(), limits->GetArray());
          htden->Scale(1., "width");
        }
        if (htnom && nbinstn > nbinsh) {
          htnom = htnom->Rebin(limits->GetSize() - 1, htnom->GetName(), limits->GetArray());
          htnom->Scale(1., "width");
        }
      }

      if (optEvt && !i) {
        if (hdenom) hdenom->Scale(1. / events);
        if (htden) htden->Scale(1. / events);
        if (htnom) htnom->Scale(1. / events);
      }

      Info("DrawSame",
           "  Use nominator (%p,#=%.3e) and denominator 'same task & different "
           "class'(%p,#=%.3e), 'certain task & different class'(%p,#=%.3e), "
           "'certain task & same class'(%p,#=%.3e)",
           h, h->GetEntries(), hdenom, (hdenom ? hdenom->GetEntries() : 0), htden, (htden ? htden->GetEntries() : 0),
           htnom, (htnom ? htnom->GetEntries() : 0));
      // Printf("h %p (bins%d) \t hdenom %p (bins%d) \t htdenom %p (bins%d) \t htnom %p (bins%d)",
      //        h,h->GetNbinsX(),hdenom,(hdenom?hdenom->GetNbinsX():0),
      //        htden,(htden?htden->GetNbinsX():0),htnom,(htnom?htnom->GetNbinsX():0));

      // standard ratio
      if (hdenom && !h->Divide(hdenom)) {
        Warning("DrawSame(eff/ratio)", "h & denom division failed!!!!");
        continue;
      }
      else if (htden && htnom && !i && !htnom->Divide(htden)) {
        Warning("DrawSame(eff/ratio)", "task-nom/task-denom division failed!!!!");
        continue;
      }
      else if (optDiv && htnom && !h->Divide(htnom)) {
        Warning("DrawSame(eff/ratio)", "h & task-nom division failed!!!!");
        continue;
      }
      else if (htden && !h->Divide(htden)) {
        Warning("DrawSame(eff/ratio)", "h & task-denom division failed!!!!");
        continue;
      }
    }

    /// scale by 1./bin width, helpfull for arbitrary bin sizes
    if (optWdth && !optRbnStat) h->Scale(1., "width");


    /// flip bin contents of histograms to 1./content
    if (optOneOver) {
      Info("DrawSame", " Scale by 1/content");
      TH1* hOne = (TH1*) h->Clone("one");
      hOne->Reset("ICSE");
      for (Int_t ib = 0; ib < (h->GetNbinsX() + 2) * (h->GetNbinsY() + 2) * (h->GetNbinsZ() + 2); ib++)
        hOne->SetBinContent(ib, 1.);
      if (hOne->Divide(h)) h = hOne;
    }

    /// set special geant labels
    if (optGeant) {
      Info("DrawSame", " Set GEANT bin labels");
      PairAnalysisHelper::SetGEANTBinLabels(h);
    }

    /// set special pdg labels
    if (optPdg || optPdgClean) {
      Info("DrawSame", " Set PDG bin labels");
      PairAnalysisHelper::SetPDGBinLabels(h, optPdgClean);
    }

    /// style histograms if not done before
    /// take into account reset style option 'RstSty'
    if (h->GetLineColor() == kBlack && !optString.Contains("col")) {  // avoid color updates
      h->UseCurrentStyle();
      PairAnalysisStyler::Style(h, i - (optRstSty ? nobj : 0));
    }
    /// some default styles for certain options
    if (optString.Contains("scat")) h->SetMarkerStyle(kDot);
    if (optString.Contains("e")) h->SetLineStyle(kSolid);
    //    if(optString.Contains("text")) { h->SetLineColor(1); h->SetMarkerColor(1); }

    /// set histogram title to current histClass
    //      h->SetName(histClass.Data());
    h->SetTitle(histClass.Data());

    /// add histograms to returned array if option 'goff' is active, otherwise
    /// draw the histogram
    if (optGoff) {
      if (optTask) h->SetTitle(Form("%s %s", GetName(), h->GetTitle()));
      arr->Add(h);
    }
    else if (optStack) {
      if (!hs) hs = new THStack("hs", Form(";%s;%s", h->GetXaxis()->GetTitle(), h->GetYaxis()->GetTitle()));
      hs->Add(h);
    }
    else {
      optString.ReplaceAll(" ", "");
      Info("DrawSame", " Draw object with options: '%s'", optString.Data());
      //      h->Draw(i>0?(optString+"same").Data():optString.Data());
      if (!optSlicesY) { h->Draw(h != hFirst ? (optString + "same").Data() : optString.Data()); }
      else if (h->GetDimension() == 2) {
        // loop over all projections
        for (Int_t bin = 1; bin < h->GetNbinsX(); bin++) {
          TH1* hp = ((TH2*) h)->ProjectionY(Form("%s_%d", h->GetName(), bin), bin, bin, "e");
          if (!hp) continue;
          Long64_t nentries = Long64_t(hp->GetEntries());
          if (nentries == 0) {
            delete hp;
            continue;
          }
          PairAnalysisStyler::Style(hp, i + (bin - 1) - (optRstSty ? nobj : 0));
          if (optRbn) hp->Rebin(rbn);
          if (optRbnStat) {
            /// rebin until stat. uncertainty is lower than 'stat'
            limits = PairAnalysisHelper::MakeStatBinLimits(hp, stat);
            hp     = hp->Rebin(limits->GetSize() - 1, hp->GetName(), limits->GetArray());
            hp->Scale(1., "width");
          }
          if (optCum) PairAnalysisHelper::Cumulate(hp, optCumR, optNorm);
          if (optSmooth) hp->Smooth(smth);
          hp->Draw(hFirst ? (optString + "same").Data() : optString.Data());
        }
      }
    }


    /// protection e.g. normalization not possible TProfile
    if (h && h->GetEntries() > 0.) {

      TString ratioName = histClassDenom;
      TString divName   = (listDenom ? listDenom->GetName() : "");
      if (optEff) divName += (listDenom ? "(MC)" : "");
      // adapt legend name
      // remove reserved words
      TObjArray* reservedWords = fReservedWords->Tokenize(":;");
      for (Int_t ir = 0; ir < reservedWords->GetEntriesFast(); ir++) {
        //	  printf("histClass %s \t search for %s \n",histClass.Data(),((TObjString*)reservedWords->At(ir))->GetString().Data());
        histClass.ReplaceAll(((TObjString*) reservedWords->At(ir))->GetString(), "");
        ratioName.ReplaceAll(((TObjString*) reservedWords->At(ir))->GetString(), "");
        divName.ReplaceAll(((TObjString*) reservedWords->At(ir))->GetString(), "");
      }
      // change default signal names to titles
      for (Int_t isig = 0; isig < static_cast<Int_t>(PairAnalysisSignalMC::EDefinedSignal::kNSignals); isig++) {
        TString src = PairAnalysisSignalMC::fgkSignals[isig][0];
        TString rpl = PairAnalysisSignalMC::fgkSignals[isig][1];
        // avoid mc signal in header AND leg-entry
        if (leg && (rpl.EqualTo(leg->GetHeader()) || src.EqualTo(leg->GetHeader()))) rpl = "";
        histClass.ReplaceAll(src, rpl);
        ratioName.ReplaceAll(src, rpl);
        divName.ReplaceAll(src, rpl);
      }
      //	printf("histClass %s \n",histClass.Data());

      // change MCtruth to MC
      for (Int_t isig = 0; isig < static_cast<Int_t>(PairAnalysisSignalMC::EDefinedSignal::kNSignals); isig++) {
        histClass.ReplaceAll("MCtruth", "MC");
        ratioName.ReplaceAll("MCtruth", "MC");
        divName.ReplaceAll("MCtruth", "MC");
      }
      // remove pairing name if it is a MC
      for (Int_t iptype = 0; iptype < PairAnalysis::fNTypes; iptype++) {
        if (ndel > 0) histClass.ReplaceAll(PairAnalysis::PairClassName(iptype), "");
        if (ratioName.CountChar('_') > 0) ratioName.ReplaceAll(PairAnalysis::PairClassName(iptype), "");
        if (divName.CountChar('_') > 0) divName.ReplaceAll(PairAnalysis::PairClassName(iptype), "");
      }
      // save Dalitz and Short underscore
      histClass.ReplaceAll("_{Dalitz}", "#{Dalitz}");
      ratioName.ReplaceAll("_{Dalitz}", "#{Dalitz}");
      divName.ReplaceAll("_{Dalitz}", "#{Dalitz}");
      histClass.ReplaceAll("_{S}", "#{S}");
      ratioName.ReplaceAll("_{S}", "#{S}");
      divName.ReplaceAll("_{S}", "#{S}");
      // remove delimiters
      histClass.ReplaceAll("_", " ");
      ratioName.ReplaceAll("_", " ");
      divName.ReplaceAll("_", " ");
      histClass.ReplaceAll(".", "");
      ratioName.ReplaceAll(".", "");
      divName.ReplaceAll(".", "");
      // get Dalitz and Short back
      histClass.ReplaceAll("#{Dalitz}", "_{Dalitz}");
      ratioName.ReplaceAll("#{Dalitz}", "_{Dalitz}");
      divName.ReplaceAll("#{Dalitz}", "_{Dalitz}");
      histClass.ReplaceAll("#{S}", "_{S}");
      ratioName.ReplaceAll("#{S}", "_{S}");
      divName.ReplaceAll("#{S}", "_{S}");
      // remove trailing and leading spaces
      histClass.Remove(TString::kBoth, ' ');
      ratioName.Remove(TString::kBoth, ' ');
      divName.Remove(TString::kBoth, ' ');

      //build final ratio name
      if (optRatio) histClass += "/" + ratioName;

      // delete the surplus
      delete reservedWords;

      // modify legend option
      TString legOpt = optString + "L";
      legOpt.ReplaceAll("hist", "");
      legOpt.ReplaceAll("scat", "");
      if (legOpt.Contains("col")) legOpt = "";
      legOpt.ReplaceAll("z", "");
      legOpt.ReplaceAll("e", "");
      if (optTask) histClass.Prepend(Form("%s ", GetName()));
      if ((optTask && optCutStep && i) || (optStack && (i - nobj))) histClass.Prepend("+");
      if (optDiv && !optOneOver) histClass.ReplaceAll(GetName(), Form("%s/%s", GetName(), divName.Data()));
      if (optDiv && optOneOver) histClass.Prepend(Form("%s/", divName.Data()));
      if (optDet) {
        for (ECbmModuleId idet = ECbmModuleId::kRef; idet < ECbmModuleId::kNofSystems; ++idet) {
          if (histName.Contains(PairAnalysisHelper::GetDetName(idet))) histClass = PairAnalysisHelper::GetDetName(idet);
        }
      }
      //	else if(nobj)     histClass="";
      if (optMeanX) histClass += Form(" #LTx#GT=%.1e", h->GetMean());
      if (optRmsX) histClass += Form(" RMS(x)=%.1e", h->GetRMS());
      if (optMeanY) histClass += Form(" #LTy#GT=%.2e", h->GetMean(2));
      if (optRmsY) histClass += Form(" RMS(y)=%.2e", h->GetRMS(2));
      histClass.ReplaceAll("e+00", "");
      // no entry for colored plots
      if (optLeg && leg /*&& !legOpt.Contains("col")*/) leg->AddEntry(h, histClass.Data(), legOpt.Data());
      //      if (leg) leg->AddEntry(h,classTable->GetName(),(optString+"L").Data());
      ++i;
    }
    else if (nobj && leg)
      leg->AddEntry(hFirst, "", "");

    //++i;


    //    }
    // cleanup
    if (limits) delete limits;
  }  //while loop histclass

  /// draw stack histogram
  if (optStack) {
    optString.ReplaceAll(" ", "");
    Info("DrawSame", " Draw stacked object with options: '%s'", optString.Data());
    hs->Draw(nobj > 0 ? (optString + "same").Data() : optString.Data());
  }

  /// draw legend only once
  /// set legend coordinates, margins, fillstyle, fontsize
  if (leg) {
    PairAnalysisStyler::SetLegendAttributes(leg, optLegFull);
    if (!nobj) leg->Draw();
  }

  /// automatic axis minimum and maximum
  if (gPad) {
    Double_t max = -1e10;
    Double_t min = +1e10;
    TListIter nextObj(gPad->GetListOfPrimitives(), kIterBackward);
    //  TObject *obj;
    while ((obj = nextObj())) {
      if (obj->InheritsFrom(TH1::Class())) {
        TH1* h1 = static_cast<TH1*>(obj);

        max             = TMath::Max(max, PairAnalysisHelper::GetContentMaximum(h1));  //h1->GetMaximum();
        Double_t tmpmax = max * (gPad->GetLogy() ? 5. : 1.1);
        if (optEff) tmpmax = 1.1;
        h1->SetMaximum(tmpmax);

        Double_t objmin = PairAnalysisHelper::GetContentMinimum(h1);
        if (gPad->GetLogy() && objmin < 0.) objmin = 0.5;
        min             = TMath::Min(min, objmin);
        Double_t tmpmin = min * (min < 0. ? 1.1 : 0.9);
        //      if(optEff) tmpmin=0.;
        h1->SetMinimum(tmpmin);

        // Printf("after %s max%f \t min%f \t for logy %.3e >? %.3e",
        // 	     h1->GetTitle(),tmpmax,tmpmin, tmpmax/(tmpmin>0.?tmpmin:1.),TMath::Power(10.,TGaxis::GetMaxDigits()));

        //      Printf("max%f \t min%f",h1->GetMaximum(),PairAnalysisHelper::GetContentMinimum(h1));
        // max=TMath::Max(max,PairAnalysisHelper::GetContentMaximum(h1));
        // min=TMath::Min(min,PairAnalysisHelper::GetContentMinimum(h1));//hobj->GetBinContent(hobj->GetMinimumBin()));
        // Printf("max%f \t min%f",max,min);
        // if(!optEff) h1->SetMaximum(max*(gPad->GetLogy()?5.:1.1));
        // else        h1->SetMaximum(1.1);
        // if(!optEff) h1->SetMinimum( min*(min<0.?1.1:0.9) ); //TODO: doesnt work, why?? Negative values?

        /// automatically set log option labels
        if (gPad->GetLogy()
            && (tmpmax / (tmpmin > 0. ? tmpmin : 1.) > TMath::Power(10., TGaxis::GetMaxDigits())
                || tmpmin < TMath::Power(10., -TGaxis::GetMaxDigits())
                || tmpmin > TMath::Power(10., +TGaxis::GetMaxDigits()))) {
          //      if(gPad->GetLogy() && tmpmax/(tmpmin>0.?tmpmin:1.) > TMath::Power(10.,TGaxis::GetMaxDigits())) {
          h1->GetYaxis()->SetMoreLogLabels(kFALSE);
          h1->GetYaxis()->SetNoExponent(kFALSE);
        }
        Double_t tmpXmin = h1->GetXaxis()->GetXmin();
        if (gPad->GetLogx()
            && h1->GetXaxis()->GetXmax() / (TMath::Abs(tmpXmin) < 1.e-10 ? 1. : tmpXmin)
                 > TMath::Power(10., TGaxis::GetMaxDigits())) {
          //	printf("Xaxis: max%f , min%f \t ratio %.3e >? %.3e \n",h1->GetXaxis()->GetXmax(),(TMath::Abs(tmpXmin)<1.e-10?1.:tmpXmin),
          //              h1->GetXaxis()->GetXmax()/(TMath::Abs(tmpXmin)<1.e-10?1.:tmpXmin),TMath::Power(10.,TGaxis::GetMaxDigits()));
          h1->GetXaxis()->SetMoreLogLabels(kFALSE);
          h1->GetXaxis()->SetNoExponent(kFALSE);
        }
      }
    }
    /// draw only once the default metadata if option 'meta' is active
    if (!nobj && optMeta && fMetaData && !gPad->GetPrimitive("meta")) { fMetaData->DrawSame(""); }

    /// force legend to be drawn always on top, remove multiple versions of it
    /// they show up when one uses the 'task' draw option
    if (!optTask && 0) {
      if (leg) leg->DrawClone();
      nextObj.Reset();
      Int_t ileg = 0;
      while ((obj = nextObj())) {
        if (obj->InheritsFrom(TLegend::Class())) {
          if (ileg > 0) delete obj;
          ileg++;
        }
      }
    }
  }

  /// styling
  /// NOTE: this draws a copy of the first histogram
  //if(gPad && !optGoff) gPad->RedrawAxis();

  /// remove canvas if graphics are switched off ('goff')
  //  if(optGoff) { c->Close(); delete c; }

  /// clean up
  if (selections) delete selections;

  return arr;
}

//_____________________________________________________________________________
void PairAnalysisHistos::SetReservedWords(const char* words)
{
  //
  // set reserved words
  //

  (*fReservedWords) = words;
}

//_____________________________________________________________________________
void PairAnalysisHistos::StoreVariables(TObject* obj, UInt_t valType[20])
{
  //
  //
  //
  if (!obj) return;
  if (obj->InheritsFrom(TH1::Class())) StoreVariables(static_cast<TH1*>(obj), valType);
  else if (obj->InheritsFrom(THnBase::Class()))
    StoreVariables(static_cast<THnBase*>(obj), valType);

  return;
}


//_____________________________________________________________________________
void PairAnalysisHistos::StoreVariables(TH1* obj, UInt_t valType[20])
{
  //
  // store variables in the axis (special for TProfile3D)
  //

  Int_t dim = obj->GetDimension();

  // dimension correction for profiles
  if (obj->IsA() == TProfile::Class() || obj->IsA() == TProfile2D::Class() || obj->IsA() == TProfile3D::Class()) {
    dim++;
  }

  if (dim >= 1) { obj->GetXaxis()->SetUniqueID(valType[0]); }
  if (dim >= 2) { obj->GetYaxis()->SetUniqueID(valType[1]); }
  if (dim >= 3) { obj->GetZaxis()->SetUniqueID(valType[2]); }
  // Tprofile3D variable
  if (dim >= 4) { obj->SetUniqueID(valType[3]); }

  return;
}

//_____________________________________________________________________________
void PairAnalysisHistos::StoreVariables(THnBase* obj, UInt_t valType[20])
{
  //
  // store variables in the axis
  //

  obj->Sumw2();

  // check for formulas and skip the rest if needed
  TList* list = obj->GetListOfFunctions();
  if (obj->IsA() == PairAnalysisHn::Class()) list = (static_cast<PairAnalysisHn*>(obj))->GetListOfFunctions();
  if (list && list->Last()) return;

  Int_t dim = obj->GetNdimensions();

  for (Int_t it = 0; it < dim; it++) {
    obj->GetAxis(it)->SetUniqueID(valType[it]);
    obj->GetAxis(it)->SetName(Form("%s", PairAnalysisVarManager::GetValueName(valType[it])));
    obj->GetAxis(it)->SetTitle(Form("%s %s", PairAnalysisVarManager::GetValueLabel(valType[it]),
                                    PairAnalysisVarManager::GetValueUnit(valType[it])));
  }
  return;
}

//_____________________________________________________________________________
void PairAnalysisHistos::FillValues(TObject* obj, const Double_t* values)
{
  //
  //
  //
  if (!obj) return;
  if (obj->InheritsFrom(TH1::Class())) FillValues(static_cast<TH1*>(obj), values);
  else if (obj->InheritsFrom(THnBase::Class()))
    FillValues(static_cast<THnBase*>(obj), values);

  return;
}

//_____________________________________________________________________________
void PairAnalysisHistos::FillValues(TH1* obj, const Double_t* values)
{
  //
  // fill values for TH1 inherted classes
  //

  Int_t dim   = obj->GetDimension();
  Bool_t bprf = kFALSE;
  //  UInt_t nValues = (UInt_t) PairAnalysisVarManager::kNMaxValues;
  UInt_t valueTypes = obj->GetUniqueID();
  if (valueTypes == static_cast<UInt_t>(PairAnalysisHistos::Eoption::kNoAutoFill)) return;
  Bool_t weight = (valueTypes != static_cast<UInt_t>(Eoption::kNoWeights));

  // check if tprofile
  if (obj->IsA() == TProfile::Class() || obj->IsA() == TProfile2D::Class() || obj->IsA() == TProfile3D::Class())
    bprf = kTRUE;

  // TO BEAUTIFY: switch off manually weighting of profile3Ds
  if (obj->IsA() == TProfile3D::Class()) weight = kFALSE;

  // check for formulas
  TList* list     = obj->GetListOfFunctions();
  TFormula* xform = dynamic_cast<TFormula*>(list->FindObject("xFormula"));
  TFormula* yform = dynamic_cast<TFormula*>(list->FindObject("yFormula"));
  TFormula* zform = dynamic_cast<TFormula*>(list->FindObject("zFormula"));
  TFormula* pform = dynamic_cast<TFormula*>(list->FindObject("pFormula"));
  TFormula* wform = dynamic_cast<TFormula*>(list->FindObject("wFormula"));
  //  Bool_t bform = (xform || yform || zform /*|| wform*/);

  // get variables from axis unique IDs
  UInt_t value1 = obj->GetXaxis()->GetUniqueID();
  UInt_t value2 = obj->GetYaxis()->GetUniqueID();
  UInt_t value3 = obj->GetZaxis()->GetUniqueID();
  UInt_t value4 = obj->GetUniqueID();  // get weighting/profile var stored in the unique ID

  Double_t fvals[4] = {values[value1], values[value2], values[value3], values[value4]};

  // use formulas to update fill values
  if (xform) fvals[0] = PairAnalysisHelper::EvalFormula(xform, values);
  if (yform) fvals[1] = PairAnalysisHelper::EvalFormula(yform, values);
  if (zform) fvals[2] = PairAnalysisHelper::EvalFormula(zform, values);
  if (wform) fvals[3] = PairAnalysisHelper::EvalFormula(wform, values);
  if (pform) fvals[3] = PairAnalysisHelper::EvalFormula(pform, values);  // weighting overwriting for Profile3D

  /*
  // ask for inclusive trigger map variables
  if(value1!=PairAnalysisVarManager::kTriggerInclONL && value1!=PairAnalysisVarManager::kTriggerInclOFF &&
     value2!=PairAnalysisVarManager::kTriggerInclONL && value2!=PairAnalysisVarManager::kTriggerInclOFF &&
     value3!=PairAnalysisVarManager::kTriggerInclONL && value3!=PairAnalysisVarManager::kTriggerInclOFF &&
     value4!=PairAnalysisVarManager::kTriggerInclONL && value4!=PairAnalysisVarManager::kTriggerInclOFF ) {
    // no trigger map variable selected
    */
  switch (dim) {
    case 1:
      if (!bprf && !weight) obj->Fill(fvals[0]);  // histograms
      else if (!bprf && weight)
        obj->Fill(fvals[0], fvals[3]);  // weighted histograms
      else if (bprf && !weight)
        ((TProfile*) obj)->Fill(fvals[0], fvals[1]);  // profiles
      else
        ((TProfile*) obj)->Fill(fvals[0], fvals[1], fvals[3]);  // weighted profiles
      break;
    case 2:
      if (!bprf && !weight) obj->Fill(fvals[0], fvals[1]);  // histograms
      else if (!bprf && weight)
        ((TH2*) obj)->Fill(fvals[0], fvals[1], fvals[3]);  // weighted histograms
      else if (bprf && !weight)
        ((TProfile2D*) obj)->Fill(fvals[0], fvals[1], fvals[2]);  // profiles
      else
        ((TProfile2D*) obj)->Fill(fvals[0], fvals[1], fvals[2], fvals[3]);  // weighted profiles
      break;
    case 3:
      if (!bprf && !weight) ((TH3*) obj)->Fill(fvals[0], fvals[1], fvals[2]);  // histograms
      else if (!bprf && weight)
        ((TH3*) obj)->Fill(fvals[0], fvals[1], fvals[2], fvals[3]);  // weighted histograms
      else if (bprf && !weight)
        ((TProfile3D*) obj)->Fill(fvals[0], fvals[1], fvals[2], fvals[3]);  // profiles
      else
        Printf(" WARNING: weighting NOT yet possible for TProfile3Ds !");
      break;
  }
  /*  }
  else {
    // fill inclusive trigger map variables
    if(weight) return;
    switch ( dim ) {
    case 1:
      for(Int_t i=0; i<30; i++) { if(TESTBIT((UInt_t)fvals[0],i)) obj->Fill(i); }
      break;
    case 2:
      if((value1==PairAnalysisVarManager::kTriggerInclOFF && value2==PairAnalysisVarManager::kTriggerInclONL) ||
	 (value1==PairAnalysisVarManager::kTriggerInclONL && value2==PairAnalysisVarManager::kTriggerInclOFF) ) {
	for(Int_t i=0; i<30; i++) {
	  if((UInt_t)fvals[0]==BIT(i)) {
	    for(Int_t i2=0; i2<30; i2++) {
	      if((UInt_t)fvals[1]==BIT(i2)) {
		obj->Fill(i, i2);
	      } // bit fired
	    } //loop 2
	  }//bit fired
	} // loop 1
      }
      else if(value1==PairAnalysisVarManager::kTriggerInclONL || value1==PairAnalysisVarManager::kTriggerInclOFF) {
	for(Int_t i=0; i<30; i++) { if(TESTBIT((UInt_t)fvals[0],i)) obj->Fill(i, fvals[1]); }
      }
      else if(value2==PairAnalysisVarManager::kTriggerInclONL || value2==PairAnalysisVarManager::kTriggerInclOFF) {
	for(Int_t i=0; i<30; i++) { if(TESTBIT((UInt_t)fvals[1],i)) obj->Fill(fvals[0], i); }
      }
      else //makes no sense
	return;
      break;
    default: return;
    }

  } //end: trigger filling
  */

  return;
}

//_____________________________________________________________________________
void PairAnalysisHistos::FillValues(THnBase* obj, const Double_t* values)
{
  //
  // fill values for THn inherted classes
  //

  // skip if manual filling
  UInt_t value4 = obj->GetUniqueID();  // weighting variable if any
  if (value4 == static_cast<UInt_t>(PairAnalysisHistos::Eoption::kNoAutoFill)) return;

  // check for formulas and skip the rest if needed
  TList* list = obj->GetListOfFunctions();
  if (obj->IsA() == PairAnalysisHn::Class()) list = (static_cast<PairAnalysisHn*>(obj))->GetListOfFunctions();
  Bool_t useFormulas = (list && list->Last());

  //  do weighting
  Bool_t weight = (value4 != static_cast<UInt_t>(Eoption::kNoWeights));

  // fill array
  const Int_t dim = obj->GetNdimensions();
  Double_t fill[dim];

  // loop over all axes
  for (Int_t it = 0; it < dim; it++) {
    if (useFormulas) {
      TString formName = Form("axis%dFormula", it);
      TFormula* form   = dynamic_cast<TFormula*>(list->FindObject(formName));
      fill[it]         = PairAnalysisHelper::EvalFormula(form, values);
    }
    else {
      fill[it] = values[obj->GetAxis(it)->GetUniqueID()];
    }
  }

  // fill object
  if (!weight) obj->Fill(fill);
  else
    obj->Fill(fill, values[value4]);

  return;
}

//_____________________________________________________________________________
void PairAnalysisHistos::FillVarArray(TObject* obj, UInt_t* valType)
{
  //
  // extract variables stored in the axis (special for TProfile3D)
  //


  if (!obj) return;

  if (obj->InheritsFrom(TH1::Class())) {
    valType[0] = ((TH1*) obj)->GetXaxis()->GetUniqueID();
    valType[1] = ((TH1*) obj)->GetYaxis()->GetUniqueID();
    valType[2] = ((TH1*) obj)->GetZaxis()->GetUniqueID();
    valType[3] = ((TH1*) obj)->GetUniqueID();  // weighting(profile) var stored in unique ID
  }
  else if (obj->InheritsFrom(THnBase::Class())) {
    for (Int_t it = 0; it < ((THn*) obj)->GetNdimensions(); it++)
      valType[it] = ((THn*) obj)->GetAxis(it)->GetUniqueID();
  }
  valType[19] = obj->GetUniqueID();  //weights
  return;
}

//_____________________________________________________________________________
void PairAnalysisHistos::AdaptNameTitle(TH1* hist, const char* histClass)
{

  //
  // adapt name and title of the histogram
  //

  Int_t dim            = hist->GetDimension();
  TString currentName  = "";  //hist->GetName();
  TString currentTitle = "";  //hist->GetTitle();
  TString hclass       = histClass;
  //get reserved class
  TObjArray* arr = hclass.Tokenize("_.");
  arr->SetOwner();
  hclass = ((TObjString*) arr->At(0))->GetString();
  delete arr;
  hclass.ToLower();

  Bool_t bname  = (currentName.IsNull());
  Bool_t btitle = (currentTitle.IsNull());
  Bool_t bprf   = kFALSE;
  if (hist->IsA() == TProfile::Class() || hist->IsA() == TProfile2D::Class() || hist->IsA() == TProfile3D::Class())
    bprf = kTRUE;

  // tprofile options
  Double_t pmin = 0., pmax = 0.;
  TString option = "", calcrange = "";
  Bool_t bStdOpt = kTRUE;
  if (bprf) {
    switch (dim) {
      case 3:
        option = ((TProfile3D*) hist)->GetErrorOption();
        pmin   = ((TProfile3D*) hist)->GetTmin();
        pmax   = ((TProfile3D*) hist)->GetTmax();
        break;
      case 2:
        option = ((TProfile2D*) hist)->GetErrorOption();
        pmin   = ((TProfile2D*) hist)->GetZmin();
        pmax   = ((TProfile2D*) hist)->GetZmax();
        break;
      case 1:
        option = ((TProfile*) hist)->GetErrorOption();
        pmin   = ((TProfile*) hist)->GetYmin();
        pmax   = ((TProfile*) hist)->GetYmax();
        break;
    }
    if (option.Contains("s", TString::kIgnoreCase)) bStdOpt = kFALSE;
    if (pmin != pmax)
      calcrange = Form("#cbar_{%+.*f}^{%+.*f}", PairAnalysisHelper::GetPrecision(pmin), pmin,
                       PairAnalysisHelper::GetPrecision(pmax), pmax);
  }

  UInt_t varx   = hist->GetXaxis()->GetUniqueID();
  UInt_t vary   = hist->GetYaxis()->GetUniqueID();
  UInt_t varz   = hist->GetZaxis()->GetUniqueID();
  UInt_t varp   = hist->GetUniqueID();
  Bool_t weight = (varp != static_cast<UInt_t>(Eoption::kNoWeights));
  if (bprf && dim == 3) weight = kFALSE;  // no weighting for profile3D

  // store titles in the axis
  if (btitle) {
    TString tit = "";
    /////// set NAMES
    hist->GetXaxis()->SetName(PairAnalysisVarManager::GetValueName(varx));
    hist->GetYaxis()->SetName(PairAnalysisVarManager::GetValueName(vary));
    hist->GetZaxis()->SetName(PairAnalysisVarManager::GetValueName(varz));
    // adapt according to formula
    TFormula* xform = dynamic_cast<TFormula*>(hist->GetListOfFunctions()->FindObject("xFormula"));
    TFormula* yform = dynamic_cast<TFormula*>(hist->GetListOfFunctions()->FindObject("yFormula"));
    TFormula* zform = dynamic_cast<TFormula*>(hist->GetListOfFunctions()->FindObject("zFormula"));
    TFormula* wform = dynamic_cast<TFormula*>(hist->GetListOfFunctions()->FindObject("wFormula"));
    if (xform) { hist->GetXaxis()->SetName(PairAnalysisHelper::GetFormulaName(xform).Data()); }
    if (yform) { hist->GetYaxis()->SetName(PairAnalysisHelper::GetFormulaName(yform).Data()); }
    if (zform) { hist->GetZaxis()->SetName(PairAnalysisHelper::GetFormulaName(zform).Data()); }
    /////// set TITLE
    hist->GetXaxis()->SetTitle(PairAnalysisVarManager::GetValueLabel(varx));
    hist->GetYaxis()->SetTitle(PairAnalysisVarManager::GetValueLabel(vary));
    hist->GetZaxis()->SetTitle(PairAnalysisVarManager::GetValueLabel(varz));
    // adapt according to formula
    if (xform) { hist->GetXaxis()->SetTitle(PairAnalysisHelper::GetFormulaTitle(xform).Data()); }
    if (yform) { hist->GetYaxis()->SetTitle(PairAnalysisHelper::GetFormulaTitle(yform).Data()); }
    if (zform) { hist->GetZaxis()->SetTitle(PairAnalysisHelper::GetFormulaTitle(zform).Data()); }
    // profile axis
    if (bprf && dim < 3) {
      TAxis* ax = 0x0;
      switch (dim) {
        case 2: ax = hist->GetZaxis(); break;
        case 1: ax = hist->GetYaxis(); break;
      }
      tit = ax->GetTitle();
      tit.Prepend((bStdOpt ? "#LT" : "RMS("));
      tit.Append((bStdOpt ? "#GT" : ")"));
      // TODO: activate -->      tit.Append ( Form("_{%ss}",hclass.Data()));
      tit.Append(calcrange.Data());
      ax->SetTitle(tit.Data());
    }
    // append the units for all axes (except formula)
    tit = Form("%s %s", hist->GetXaxis()->GetTitle(), PairAnalysisVarManager::GetValueUnit(varx));
    if (!xform) hist->GetXaxis()->SetTitle(tit.Data());
    tit = Form("%s %s", hist->GetYaxis()->GetTitle(), PairAnalysisVarManager::GetValueUnit(vary));
    if (!yform) hist->GetYaxis()->SetTitle(tit.Data());
    tit = Form("%s %s", hist->GetZaxis()->GetTitle(), PairAnalysisVarManager::GetValueUnit(varz));
    if (!zform) hist->GetZaxis()->SetTitle(tit.Data());
    // overwrite titles with hist class if needed
    if (!bprf) {
      switch (dim) {
        case 1: hist->GetYaxis()->SetTitle(Form("%ss", hclass.Data())); break;
        case 2: hist->GetZaxis()->SetTitle(Form("%ss", hclass.Data())); break;
      }
    }
    // weighted axis (maximal 2 dimensional)
    if (weight) {
      TAxis* ax = hist->GetYaxis();
      if (dim == 2) ax = hist->GetZaxis();
      tit = PairAnalysisVarManager::GetValueLabel(varp);
      if (wform) { tit = PairAnalysisHelper::GetFormulaTitle(wform); }
      ax->SetTitle(Form("%s weighted %s", tit.Data(), ax->GetTitle()));
    }

    // create an unique name
    TFormula* pform = dynamic_cast<TFormula*>(hist->GetListOfFunctions()->FindObject("pFormula"));
    if (bname) switch (dim) {
        case 3:
          currentName += Form("%s_", hist->GetXaxis()->GetName());
          currentName += Form("%s_", hist->GetYaxis()->GetName());
          currentName += Form("%s", hist->GetZaxis()->GetName());
          if (bprf && !pform)
            currentName += Form("-%s%s", PairAnalysisVarManager::GetValueName(varp), (bStdOpt ? "avg" : "rms"));
          else if (bprf)
            currentName += Form("-%s%s", PairAnalysisHelper::GetFormulaName(pform).Data(), (bStdOpt ? "avg" : "rms"));
          if (weight && !bprf) currentName += Form("-wght%s", PairAnalysisVarManager::GetValueName(varp));
          break;
        case 2:
          currentName += Form("%s_", hist->GetXaxis()->GetName());
          currentName += Form("%s", hist->GetYaxis()->GetName());
          if (bprf) currentName += Form("-%s%s", hist->GetZaxis()->GetName(), (bStdOpt ? "avg" : "rms"));
          if (weight && !wform) currentName += Form("-wght%s", PairAnalysisVarManager::GetValueName(varp));
          else if (weight && wform)
            currentName += Form("-wght%s", PairAnalysisHelper::GetFormulaName(wform).Data());
          break;
        case 1:
          currentName += Form("%s", hist->GetXaxis()->GetName());
          if (bprf) currentName += Form("-%s%s", hist->GetYaxis()->GetName(), (bStdOpt ? "avg" : "rms"));
          if (weight && !wform) currentName += Form("-wght%s", PairAnalysisVarManager::GetValueName(varp));
          else if (weight && wform)
            currentName += Form("-wght%s", PairAnalysisHelper::GetFormulaName(wform).Data());
          break;
      }
    // to differentiate btw. leg and pair histos
    //    if(!strcmp(histClass,"Pair")) currentName.Prepend("p");
    if (hclass.Contains("pair")) currentName.Prepend("p");
    hist->SetName(currentName.Data());
  }
}


//_____________________________________________________________________________
void PairAnalysisHistos::AdaptNameTitle(THnBase* hist, const char* histClass)
{

  //
  // adapt name and title of the histogram
  //

  Int_t dim            = hist->GetNdimensions();
  TString currentName  = "";  //hist->GetName();
  TString currentTitle = "";  //hist->GetTitle();
  TString hclass       = histClass;
  //get reserved class
  TObjArray* arr = hclass.Tokenize("_.");
  arr->SetOwner();
  hclass = ((TObjString*) arr->At(0))->GetString();
  delete arr;
  hclass.ToLower();

  Bool_t bname  = (currentName.IsNull());
  Bool_t btitle = (currentTitle.IsNull());

  TList* list = (static_cast<PairAnalysisHn*>(hist))->GetListOfFunctions();

  // store titles in the axis
  if (btitle) {
    TString tit = "";
    // adapt according to formula
    for (Int_t it = 0; it < dim; it++) {
      TString formName = Form("axis%dFormula", it);
      TFormula* form   = dynamic_cast<TFormula*>(list->FindObject(formName));
      hist->GetAxis(it)->SetName(PairAnalysisHelper::GetFormulaName(form).Data());
      // adapt according to formula
      hist->GetAxis(it)->SetTitle(PairAnalysisHelper::GetFormulaTitle(form).Data());
    }
  }

  // create an unique name
  if (bname) {
    currentName = hist->GetName();
    if (hclass.Contains("pair")) currentName.Prepend("p");
    hist->SetName(currentName.Data());
  }
}
