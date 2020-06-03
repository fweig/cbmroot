///////////////////////////////////////////////////////////////////////////
//
//
// Authors:                                                              
//   * Copyright(c) 1998-2009, ALICE Experiment at CERN, All rights reserved. *
//   Julian Book   <Julian.Book@cern.ch>                                 
/*

  This class provides cuts for all variables and formulas managed by PairAnalysisVarManager.
  Many cuts can be added to the same object and are processed consecutively according to
  the cut logic/type SetCutType(CutType type). Further the arguments of AddCut allow to 
  include or exclude a certain selection in the analysis.

  It is recommmended to give the cut object a unique and meaning full name(+title), since 
  they are used e.g. in PairAnalysisCutQA and PairAnalysis::GetCutStepHistogramList()
  to monitor the cut impact.

  Many objects can be combined in a PairAnalysisCutGroup with additional cut logics.
  

*/
//                                                                       //
///////////////////////////////////////////////////////////////////////////


#include <TAxis.h>
#include <TFormula.h>

#include "PairAnalysisVarCuts.h"
#include "PairAnalysisHelper.h"

ClassImp(PairAnalysisVarCuts)


PairAnalysisVarCuts::PairAnalysisVarCuts() :
  AnalysisCuts(),
  fUsedVars(new TBits(PairAnalysisVarManager::kNMaxValuesMC)),
  fNActiveCuts(0),
  fActiveCutsMask(0),
  fSelectedCutsMask(0),
  fCutType(kAll)
{
  //
  // Default costructor
  //
  for (Int_t i=0; i<PairAnalysisVarManager::kNMaxValuesMC; ++i){
    fActiveCuts[i]=0;
    fCutMin[i]=0;
    fCutMax[i]=0;
    fCutExclude[i]=kFALSE;
    fBitCut[i]=kFALSE;
    fCutVar[i]=0x0;
  }
  PairAnalysisVarManager::InitFormulas();
}

//________________________________________________________________________
PairAnalysisVarCuts::PairAnalysisVarCuts(const char* name, const char* title) :
  AnalysisCuts(name,title),
  fUsedVars(new TBits(PairAnalysisVarManager::kNMaxValuesMC)),
  fNActiveCuts(0),
  fActiveCutsMask(0),
  fSelectedCutsMask(0),
  fCutType(kAll)
{
  //
  // Named contructor
  //
  for (Int_t i=0; i<PairAnalysisVarManager::kNMaxValuesMC; ++i){
    fActiveCuts[i]=0;
    fCutMin[i]=0;
    fCutMax[i]=0;
    fCutExclude[i]=kFALSE;
    fBitCut[i]=kFALSE;
    fCutVar[i]=0x0;
  }
  PairAnalysisVarManager::InitFormulas();
}

//________________________________________________________________________
PairAnalysisVarCuts::~PairAnalysisVarCuts()
{
  //
  // Destructor
  //
  if (fUsedVars) delete fUsedVars;
  for (Int_t i=0; i<PairAnalysisVarManager::kNMaxValuesMC; ++i) {
    if (fCutVar[i]) delete fCutVar[i];
  }
}

//________________________________________________________________________
Bool_t PairAnalysisVarCuts::IsSelected(Double_t * const values)
{
  //
  // Make cut decision
  //

  //reset
  fSelectedCutsMask=0;
  SetSelected(kFALSE);

  for (Int_t iCut=0; iCut<fNActiveCuts; ++iCut){
    Int_t cut=fActiveCuts[iCut];
    SETBIT(fSelectedCutsMask,iCut);

    // apply 'bit cut'
    if(fBitCut[iCut]) {
      if ( (TESTBIT((UInt_t)values[cut],(UInt_t)fCutMin[iCut]))^(!fCutExclude[iCut]) )  CLRBIT(fSelectedCutsMask,iCut);
    }
    else {
      // standard var cuts
      if ( !fCutVar[iCut] && ((values[cut]<fCutMin[iCut]) || (values[cut]>fCutMax[iCut]))^fCutExclude[iCut] ) {
	CLRBIT(fSelectedCutsMask,iCut);
      }
      else if ( fCutVar[iCut] && fCutVar[iCut]->IsA() == TFormula::Class()) {
	/// use a formula for the variable //
	TFormula *form = static_cast<TFormula*>(fCutVar[iCut]);
	Double_t val   = PairAnalysisHelper::EvalFormula(form,values);
	if ( ((val<fCutMin[iCut]) || (val>fCutMax[iCut]))^fCutExclude[iCut] ) {
	  CLRBIT(fSelectedCutsMask,iCut);
	}
      }
    }
    // cut type and decision
    if ( fCutType==kAll && !TESTBIT(fSelectedCutsMask,iCut) ) return kFALSE; // option to (minor) speed improvement
  }

  Bool_t isSelected=(fSelectedCutsMask==fActiveCutsMask);
  if ( fCutType==kAny ) isSelected=(fSelectedCutsMask>0);
  SetSelected(isSelected);
  return isSelected;
}

//________________________________________________________________________
Bool_t PairAnalysisVarCuts::IsSelected(TObject* track)
{
  //
  // Make cut decision
  //

  //reset
  fSelectedCutsMask=0;
  SetSelected(kFALSE);

  if (!track) return kFALSE;

  //Fill values
  Double_t *values=PairAnalysisVarManager::GetData();
  PairAnalysisVarManager::SetFillMap(fUsedVars);
  PairAnalysisVarManager::Fill(track,values);

  /// selection
  return (IsSelected(values));

}

//________________________________________________________________________
void PairAnalysisVarCuts::AddCut(PairAnalysisVarManager::ValueTypes type, Double_t min, Double_t max, Bool_t excludeRange)
{
  //
  // Set cut range and activate it
  //
  if (min>max){
    Double_t tmp=min;
    min=max;
    max=tmp;
  }
  fCutMin[fNActiveCuts]=min;
  fCutMax[fNActiveCuts]=max;
  fCutExclude[fNActiveCuts]=excludeRange;
  SETBIT(fActiveCutsMask,fNActiveCuts);
  fActiveCuts[fNActiveCuts]=(UShort_t)type;
  fUsedVars->SetBitNumber(type,kTRUE);
  ++fNActiveCuts;
}

//________________________________________________________________________
void PairAnalysisVarCuts::AddCut(const char *formula, Double_t min, Double_t max, Bool_t excludeRange)
{
  //
  // Set cut range and activate it
  //
  if (min>max){
    Double_t tmp=min;
    min=max;
    max=tmp;
  }
  fCutMin[fNActiveCuts]=min;
  fCutMax[fNActiveCuts]=max;
  fCutExclude[fNActiveCuts]=excludeRange;
  SETBIT(fActiveCutsMask,fNActiveCuts);
  fActiveCuts[fNActiveCuts]=(UShort_t)PairAnalysisVarManager::kNMaxValuesMC;

  // construct a TFormula
  TFormula *form = new TFormula("varFormula",formula);
  // compile function
  if(form->Compile()) return;
  //set parameter identifier and activate variables in the fill map
  for(Int_t i=0; i<form->GetNpar(); i++) {
    form->SetParName(  i, PairAnalysisVarManager::GetValueName(form->GetParameter(i)) );
    fUsedVars->SetBitNumber((Int_t)form->GetParameter(i),kTRUE);
  }

  fCutVar[fNActiveCuts]=form;
  ++fNActiveCuts;
}

//________________________________________________________________________
void PairAnalysisVarCuts::AddBitCut(PairAnalysisVarManager::ValueTypes type, UInt_t bit, Bool_t excludeRange)
{
  //
  // Set cut range and activate it
  //
  fCutMin[fNActiveCuts]=bit;
  fCutExclude[fNActiveCuts]=excludeRange;
  fBitCut[fNActiveCuts]=kTRUE;
  SETBIT(fActiveCutsMask,fNActiveCuts);
  fActiveCuts[fNActiveCuts]=(UShort_t)type;
  fUsedVars->SetBitNumber(type,kTRUE);
  ++fNActiveCuts;
}

//________________________________________________________________________
void PairAnalysisVarCuts::Print(const Option_t* /*option*/) const
{
  //
  // Print cuts and the range
  //
  printf("------------------------------------------\n");
  printf("cut ranges for '%s'\n",GetTitle());
  if (fCutType==kAll){
    printf("All Cuts have to be fulfilled\n");
  } else {
    printf("Any Cut has to be fulfilled\n");
  }
  for (Int_t iCut=0; iCut<fNActiveCuts; ++iCut){
    Int_t cut=(Int_t)fActiveCuts[iCut];
    Bool_t inverse=fCutExclude[iCut];
    Bool_t bitcut=fBitCut[iCut];
    Bool_t fcut=(fCutVar[iCut]&&fCutVar[iCut]->IsA()==TFormula::Class());

    if(!bitcut && !fcut) {
      // standard cut
      if (!inverse){
	printf("Cut %02d: %f < %s < %f\n", iCut,
	       fCutMin[iCut], PairAnalysisVarManager::GetValueName((Int_t)cut), fCutMax[iCut]);
      } else {
	printf("Cut %02d: !(%f < %s < %f)\n", iCut,
	       fCutMin[iCut], PairAnalysisVarManager::GetValueName((Int_t)cut), fCutMax[iCut]);
      }
    }
    else if(bitcut) {
      // bit cut
      if (!inverse){
	printf("Cut %02d: %s & (1ULL<<%d) \n", iCut,
	       PairAnalysisVarManager::GetValueName((Int_t)cut), (UInt_t)fCutMin[iCut]);
      } else {
	printf("Cut %02d: !(%s & (1ULL<<%d)) \n", iCut,
	       PairAnalysisVarManager::GetValueName((Int_t)cut), (UInt_t)fCutMin[iCut]);
      }
    }
    else if(fcut) {
      // variable defined by a formula
      TFormula *form = static_cast<TFormula*>(fCutVar[iCut]);
      TString tit(form->GetExpFormula());
      // replace parameter variables with names labels
      for(Int_t j=0;j<form->GetNpar();j++) tit.ReplaceAll(Form("[%d]",j),form->GetParName(j));
      if (!inverse) printf("Cut %02d: %f < %s < %f\n", iCut, fCutMin[iCut], tit.Data(), fCutMax[iCut]);
      else          printf("Cut %02d: !(%f < %s < %f)\n", iCut, fCutMin[iCut], tit.Data(), fCutMax[iCut]);
    }
    else
      printf("cut class not found\n");
  } //loop over cuts
  printf("------------------------------------------\n");

}
