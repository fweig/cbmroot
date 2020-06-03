///////////////////////////////////////////////////////////////////////////
//
// Manager class for filter decisions based on cuts
// The filter contains a list of sets of cuts and controll histograms (optional).
// A bit field is filled in order to store the decision of each cut-set. 
//
// Authors:
//   * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
//   Julian Book   <Julian.Book@cern.ch>
//
// Add cut object to list via AddCuts(AnalysisCuts* cuts)
// Add control histograms via AddHistos(PairAnalysisHistos* histos)
//
////////////////////////////////////////////////////////////////////////

#include <TObject.h>
#include <TList.h>
#include <THashList.h>
#include "AnalysisFilter.h"
#include "PairAnalysisHistos.h"
#include "AnalysisCuts.h"

#include "PairAnalysisVarManager.h"

ClassImp(AnalysisFilter)


AnalysisFilter::AnalysisFilter():
    TNamed(),
  fCuts(0),
  fHistos()
{
  // Default constructor
  fHistos.SetOwner(kTRUE);
  fHistos.SetName("PairAnalysis_FilterHistos");

}

AnalysisFilter::AnalysisFilter(const char* name, const char* title):
    TNamed(name, title),
    fCuts(new TList()),
    fHistos()
    //    fHistos(new TList())
{
  // Constructor
  fHistos.SetOwner(kTRUE);
  fHistos.SetName(name);
}

AnalysisFilter::AnalysisFilter(const AnalysisFilter& obj):
    TNamed(obj),
    fCuts(0),
    fHistos()
{
// Copy constructor
    fCuts   = obj.fCuts;
    //    fHistos = obj.fHistos;
}

AnalysisFilter::~AnalysisFilter()
{
// Destructor
   if (fCuts)   fCuts->Delete("slow");
   //if (fHistos) fHistos->Delete("slow");
   fHistos.Clear();
   delete fCuts;
}   

AnalysisFilter& AnalysisFilter::operator=(const AnalysisFilter& other)
{
// Assignment
   if (&other != this) {
	   TNamed::operator=(other);
	   fCuts = other.fCuts;
	   //	   fHistos = other.fHistos;
   }
   return *this;
   }
   
UInt_t AnalysisFilter::IsSelected(TObject* obj)
{
    //
    // Loop over all set of cuts
    // and store the decision
    UInt_t result = 0;
    UInt_t filterMask;
    
    TIter next(fCuts);
    AnalysisCuts *cuts;
    Int_t iCutB = 1;
	

    //Fill values - fill only once the variables
    // TODO: check carefully the case of pairlegcuts, 
    //       before switching to IsSelected(values)
    // Double_t values[PairAnalysisVarManager::kNMaxValuesMC];
    // PairAnalysisVarManager::Fill(obj,values);

    while((cuts = (AnalysisCuts*)next())) {
      //Bool_t acc = cuts->IsSelected(values);
      Bool_t acc = cuts->IsSelected(obj); /// original
	if ((filterMask = cuts->GetFilterMask()) > 0) {
	    acc = (acc && (filterMask == result));
	}
	cuts->SetSelected(acc);
	if (acc) {result |= iCutB & 0x00ffffff;}
	iCutB *= 2;
    }  

    return result;
}

UInt_t AnalysisFilter::IsSelected(Double_t * const values)
{
    //
    // Loop over all set of cuts
    // and store the decision
    UInt_t result = 0;
    UInt_t filterMask;

    TIter next(fCuts);
    AnalysisCuts *cuts;
    Int_t iCutB = 1;

    while((cuts = (AnalysisCuts*)next())) {
      Bool_t acc = cuts->IsSelected(values);
	if ((filterMask = cuts->GetFilterMask()) > 0) {
	    acc = (acc && (filterMask == result));
	}
	cuts->SetSelected(acc);
	if (acc) {result |= iCutB & 0x00ffffff;}
	iCutB *= 2;
    }

    return result;
}

UInt_t AnalysisFilter::IsSelected(TList* list)
{
    //
    // Loop over all set of cuts
    // and store the decision
    UInt_t result = 0;
    UInt_t filterMask;

    TIter next(fCuts);
    AnalysisCuts *cuts;
    Int_t iCutB = 1;
	
    while((cuts = (AnalysisCuts*)next())) {
	Bool_t acc = cuts->IsSelected(list);
	if ((filterMask = cuts->GetFilterMask()) > 0) {
	    acc = (acc && (filterMask & result));
	}
	cuts->SetSelected(acc);
	if (acc) {result |= iCutB & 0x00ffffff;}
	iCutB *= 2;
    }  

    return result;
}

void AnalysisFilter::Init()
{
    //
    // Loop over all set of cuts and call Init
    TIter next(fCuts);
    AnalysisCuts *cuts;
    while((cuts = (AnalysisCuts*)next())) cuts->Init();
}

void AnalysisFilter::AddCuts(AnalysisCuts* cuts)
{
    // Add a set of cuts
    fCuts->Add(cuts);
}

void AnalysisFilter::AddHistos(PairAnalysisHistos* histos)
{
  //
  // add histos for each cut
  //
  fHistos.Clear();
  TIter next(fCuts);
  AnalysisCuts *cuts;
  Int_t iCut=0;
  while((cuts = (AnalysisCuts*)next()))  {
    fHistos.AddAt( (PairAnalysisHistos*) histos->GetHistogramList()->Clone(cuts->GetName()) , iCut++);
    //    printf("AnalysisFilter::AddHistos add histos for %s at %d \n",cuts->GetName(),iCut);
  }

}

Bool_t AnalysisFilter::IsSelected(char* name)
{
    //
    // Returns current result for cut with name
    AnalysisCuts* cut = (AnalysisCuts*) (fCuts->FindObject(name));
    if (cut) {
      return (cut->Selected());
    } else  {
      return 0;
    }
}
