///////////////////////////////////////////////////////////////////////////
//
//
// Authors:
//   Julian Book   <Julian.Book@cern.ch>
/*

  Meta data container that stores basic information on the setup, system,
  beam/c.m.s. energy, selected events, cbmroot revision, date, ...

  The DrawSame(TString opt) provides an easy way to add a standardised
  caption to a plot.

 */
//                                                                       //
///////////////////////////////////////////////////////////////////////////

#include <TROOT.h>
#include <TSystem.h>
#include <TPad.h>
#include <TCollection.h>
#include <TList.h>
// #include "TParamater<Int_t>.h"
// #include "TParamater<Double_t>.h"
#include <TParameter.h>
#include <TNamed.h>
#include <TPaveText.h>
#include <TGeoElement.h>

#include "URun.h"
#include "PairAnalysisMetaData.h"

ClassImp(PairAnalysisMetaData)


PairAnalysisMetaData::PairAnalysisMetaData() :
  PairAnalysisMetaData("PairAnalysis_MetaData","PairAnalysis MetaData Container")
{
  //
  // Default constructor
  //
}

//_____________________________________________________________________________
PairAnalysisMetaData::PairAnalysisMetaData(const char* name, const char* title) :
  TNamed(name, title),
  fMetaList()
{
  //
  // TNamed constructor
  //
  fMetaList.SetOwner(kTRUE);
  fMetaList.SetName(name);
}

//_____________________________________________________________________________
PairAnalysisMetaData::~PairAnalysisMetaData()
{
  //
  // Destructor
  //
  fMetaList.Clear();
}

//_____________________________________________________________________________
void PairAnalysisMetaData::Init()
{
  //
  // Init meta data objects and add to list
  //

  TNamed *pSetup = new TNamed("setup", "undefined");
  //  pSetup->SetTitle(gSystem->Getenv("USER")); //e.g. sis100
  fMetaList.Add(pSetup);

  TNamed *pSystem = new TNamed("system", "Au+Au");
  //  pSystem->SetTitle(gSystem->Getenv("USER"));
  fMetaList.Add(pSystem);

  TNamed *pProduction = new TNamed("production", "SIS");
  //  pProduction->SetTitle(gProduction->Getenv("USER"));
  fMetaList.Add(pProduction);

  TParameter<Double_t> *pBeamEnergy = new TParameter<Double_t>("beamenergy", 4.107);
  //  pBeamEnergy->SetBit(TParameter<Double_t>::kIsConst);
  pBeamEnergy->SetBit(TParameter<Double_t>::kFirst);
  fMetaList.Add(pBeamEnergy);

  TParameter<Int_t> *pEvents = new TParameter<Int_t>("events", 0);
  pEvents->SetMergeMode('+');
  fMetaList.Add(pEvents);

  TParameter<Bool_t> *pMC = new TParameter<Bool_t>("mc", kTRUE);
  pMC->SetBit(TParameter<Bool_t>::kIsConst);
  fMetaList.Add(pMC);


  TNamed *pTrainOp = new TNamed("operator", "undefined");
  pTrainOp->SetTitle(gSystem->Getenv("USER"));
  fMetaList.Add(pTrainOp);

  // TNamed *pRootVers = new TNamed("root", "XXX");
  // pRootVers->SetTitle(gROOT->GetVersion());
  TParameter<Int_t> *pRootVers = new TParameter<Int_t>("root",gROOT->GetVersionInt());
  pRootVers->SetBit(TParameter<Int_t>::kIsConst);
  fMetaList.Add(pRootVers);

  gSystem->Setenv("CBMROOT_SVN_REVISION",gSystem->GetFromPipe("svnversion $VMCWORKDIR"));
  TString rev=gSystem->Getenv("CBMROOT_SVN_REVISION");
  TParameter<Int_t> *pCbmRev = new TParameter<Int_t>("cbmroot", 0);
  pCbmRev->SetBit(TParameter<Int_t>::kIsConst);
  pCbmRev->SetVal(rev.Atoi());
  fMetaList.Add(pCbmRev);

  TDatime dat;
  TParameter<Int_t> *pDate = new TParameter<Int_t>("date", dat.GetDate());
  pDate->SetBit(TParameter<Int_t>::kMin);
  fMetaList.Add(pDate);

  TParameter<Int_t> *pTime = new TParameter<Int_t>("time", dat.GetTime());
  pTime->SetBit(TParameter<Int_t>::kMin);
  fMetaList.Add(pTime);

}

//_____________________________________________________________________________
void PairAnalysisMetaData::SetMetaData(TList &list, Bool_t setOwner/*=kTRUE*/)
{
  //
  // set container classes to this instance. It will take onwnership!
  //
  ResetMetaData();
  TString name(GetName());
  if (name == "PairAnalysisMetaData") SetName(list.GetName());
  TIter next(&list);
  TObject *o;
  while ( (o=next()) ){
    fMetaList.Add(o);
  }
  if (setOwner){
    list.SetOwner(kFALSE);
    fMetaList.SetOwner(kTRUE);
  } else {
    fMetaList.SetOwner(kFALSE);
  }
}

void PairAnalysisMetaData::FillMeta(const char *name, Double_t val) {
  //
  // fill meta data of doubles
  //
  TParameter<Double_t> *par = dynamic_cast<TParameter<Double_t> *>(fMetaList.FindObject(name));
  par->SetVal(val);
}

void PairAnalysisMetaData::FillMeta(const char *name, Int_t    val) {
  //
  // fill meta data of integers
  //
  TParameter<Int_t> *par = dynamic_cast<TParameter<Int_t> *>(fMetaList.FindObject(name));
  par->SetVal(val);
}

void PairAnalysisMetaData::FillMeta(const char *name, const char *val) {
  //
  // fill meta data of strings
  //
  TNamed *par = dynamic_cast<TNamed *>(fMetaList.FindObject(name));
  par->SetTitle(val);
}

void PairAnalysisMetaData::GetMeta(const char *name, Int_t *val) {
  //
  // get meta data value for integers
  //
  TParameter<Int_t> *par = dynamic_cast<TParameter<Int_t> *>(fMetaList.FindObject(name));
  if(par) *val=par->GetVal();
}

void PairAnalysisMetaData::GetMeta(const char *name, Double_t *val) {
  //
  // get meta data value for doubles
  //
  TParameter<Double_t> *par = dynamic_cast<TParameter<Double_t> *>(fMetaList.FindObject(name));
  if(par) *val=par->GetVal();
}

void PairAnalysisMetaData::DrawSame(TString opt/*="msb"*/)
{
  //
  // draw meta data into current pad
  // use option string to select information displayed:
  // m := mc -> 'Simulation'
  // s := system e.g. 'Au+Au', 'p+Au'
  // b := lab beam energy Ebeam
  // S := sqrt s_NN
  // n := number of events (after event selection)
  //
  if(fMetaList.GetEntries()<1) return;

  TPaveText *pt = new TPaveText(gPad->GetLeftMargin()+0.05,
				1.-gPad->GetTopMargin()+0.01,
				1.-gPad->GetRightMargin()-0.05,
				0.99,"NDCNB");
  pt->SetName("meta");
  pt->SetTextAlign(kHAlignLeft+kVAlignCenter);
  pt->SetMargin(0.01); //default 0.05

  TString line="CBM";
  TString tmp="";

  // simulation (only if true)
  TParameter<Bool_t> *parB = dynamic_cast<TParameter<Bool_t> *>(fMetaList.FindObject("mc"));
  if(opt.Contains("m") && parB && parB->GetVal()) {
      tmp=Form(" Simulation");
      line+=tmp;
  }

  // system
  TNamed *par = dynamic_cast<TNamed *>(fMetaList.FindObject("system"));
  if(opt.Contains("s") && par)  tmp=par->GetTitle();
  if(!tmp.IsNull()) line+=", " + tmp;

  // beamenergy
  TParameter<Double_t> *parD = dynamic_cast<TParameter<Double_t> *>(fMetaList.FindObject("beamenergy"));
  if(opt.Contains("b") && parD) {
    if(tmp.Contains("p")) tmp=Form("#it{E}_{beam} = %.2f GeV",parD->GetVal());
    else                  tmp=Form("#it{E}_{beam} = %.2f #it{A}GeV",parD->GetVal());
    if(!tmp.IsNull()) line+=" " + tmp;
  }
  else if(opt.Contains("S") && parD) {
    TString sys(par->GetTitle());
    sys.ReplaceAll("+","");     sys.ReplaceAll("-","");
    Int_t aProj=0; Int_t zProj=0;
    Int_t aTarg=0; Int_t zTarg=0;

    if(sys.EqualTo("pp"))        { aProj=1;   zProj=1;  aTarg=1;   zTarg=1; }
    else {
      /// get charge and atomic number of elements from database
      TGeoElementTable g(0);
      TString targ=( sys(sys.Length()-2, sys.Length())  );
      TString proj=( sys(0,              sys.Length()-2));
      proj.ReplaceAll("p","H");
      aProj=g.FindElement(proj.Data())->A()+0.05; // add a small number to get proper rounding (e.g. Au: 196->197)
      zProj=g.FindElement(proj.Data())->Z();
      aTarg=g.FindElement(targ.Data())->A()+0.05;
      zTarg=g.FindElement(targ.Data())->Z();
    }
    // else if(sys.EqualTo("pAu"))  { aProj=1;   zProj=1;  aTarg=197; zTarg=79; }
    // else if(sys.EqualTo("pNi"))  { aProj=1;   zProj=1;  aTarg=58;  zTarg=28; }
    // else if(sys.EqualTo("AuAu")) { aProj=197; zProj=79; aTarg=197; zTarg=79; }
    // else if(sys.EqualTo("AgAg")) { aProj=107; zProj=47; aTarg=107; zTarg=47; }
    // else if(sys.EqualTo("NiNi")) { aProj=58;  zProj=28; aTarg=58;  zTarg=28; }
    // else if(sys.EqualTo("NiAu")) { aProj=58;  zProj=28; aTarg=197; zTarg=79; }
    // else if(sys.EqualTo("AgAu")) { aProj=107; zProj=47; aTarg=197; zTarg=79; }
    // Get the cm energy, according to URun::GetNNSqrtS
    URun run("","",aProj,zProj,parD->GetVal(),aTarg,zTarg,0.,0.,0.,0,0.,0.,0.,0);
    tmp=Form("#sqrt{#it{s}_{NN}} = %.2f GeV",run.GetNNSqrtS());
    if(!tmp.IsNull()) line+=" " + tmp;
  }

  // events
  TParameter<Int_t> *parI = dynamic_cast<TParameter<Int_t> *>(fMetaList.FindObject("events"));
  if(opt.Contains("n") && parD) {
    //    tmp=Form("#it{N}_{evt} = %.1f#times10^{6}",parI->GetVal()/1.e+6);
    tmp=Form("#it{N}_{evt} = %.1fM",parI->GetVal()/1.e+6);
    if(!tmp.IsNull()) line+=", " + tmp;
  }

  pt->AddText(line.Data());


  pt->SetLineColorAlpha(0, 0.0);
  pt->SetFillColorAlpha(0, 0.0);
  pt->SetFillStyle(kFEmpty);
  //  pt->Print();
  pt->Draw();

}
