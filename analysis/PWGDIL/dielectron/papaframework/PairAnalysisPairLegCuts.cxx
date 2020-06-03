///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                                                                       //
// Authors:
//   * Copyright(c) 1998-2009, ALICE Experiment at CERN, All rights reserved. *
//   Julian Book   <Julian.Book@cern.ch>
/*
  Cut class providing cuts for both legs in the PairAnalysisPair
  Add any number of leg cuts using e.g. for leg 1
    GetFilterLeg1().AddCuts(mycut)
  where mycut has to inherit from AnalysisCuts.

  the following cut types are defined and selected via SetCutType():
    kBothLegs, kAnyLeg, kMixLegs, kOneLeg

*/
//                                                                       //
///////////////////////////////////////////////////////////////////////////

#include <TList.h>

#include "PairAnalysisPairLV.h"
#include "PairAnalysisTrack.h"

#include "PairAnalysisPairLegCuts.h"

ClassImp(PairAnalysisPairLegCuts)


PairAnalysisPairLegCuts::PairAnalysisPairLegCuts() :
  PairAnalysisPairLegCuts("pairlegcut","pairlegcut")
{
  //
  // Default contructor
  //
}

//________________________________________________________________________
PairAnalysisPairLegCuts::PairAnalysisPairLegCuts(const char* name, const char* title) :
  AnalysisCuts(name,title),
  fFilterLeg1("PairFilterLeg1","PairFilterLeg1"),
  fFilterLeg2("PairFilterLeg2","PairFilterLeg2")
{
  //
  // Named contructor
  //
}


//________________________________________________________________________
Bool_t PairAnalysisPairLegCuts::IsSelected(TObject* track)
{
  //
  // check if cuts are fulfilled
  //

  //check if we have a PairAnalysisPair
  PairAnalysisPair *pair=dynamic_cast<PairAnalysisPair*>(track);
  if (!pair) return kFALSE;

  //get both legs
  PairAnalysisTrack *leg1=pair->GetFirstDaughter();
  PairAnalysisTrack *leg2=pair->GetSecondDaughter();

  //mask used to require that all cuts are fulfilled
  UInt_t selectedMaskLeg1=(1<<fFilterLeg1.GetCuts()->GetEntries())-1;
  UInt_t selectedMaskLeg2=(1<<fFilterLeg2.GetCuts()->GetEntries())-1;

  //test cuts
  Bool_t isLeg1selected=(fFilterLeg1.IsSelected(leg1)==selectedMaskLeg1);
  if(fCutType==kBothLegs && !isLeg1selected) {
    SetSelected(isLeg1selected);
    return isLeg1selected;
  }
  //skip further checks if first leg passes cuts and cuttype is any
  if (fCutType==kAnyLeg && isLeg1selected) {
    SetSelected(isLeg1selected);
    return isLeg1selected;
  }

  Bool_t isLeg2selected=(fFilterLeg2.IsSelected(leg2)==selectedMaskLeg2);
  Bool_t isSelected=isLeg1selected&&isLeg2selected;
  if (fCutType==kAnyLeg) {
    isSelected=isLeg1selected||isLeg2selected;
    SetSelected(isSelected);
    return isSelected;
  }
  if (fCutType==kOneLeg) {
    isSelected=(isLeg1selected!=isLeg2selected);
    SetSelected(isSelected);
    return isSelected;
  }

  if (fCutType==kMixLegs) {
    Bool_t isLeg1selectedMirror=(fFilterLeg1.IsSelected(leg2)==selectedMaskLeg1);
    Bool_t isLeg2selectedMirror=(fFilterLeg2.IsSelected(leg1)==selectedMaskLeg2);
    isSelected=(isLeg1selected&&isLeg2selected)||(isLeg1selectedMirror&&isLeg2selectedMirror);
    SetSelected(isSelected);
  }
  return isSelected;
}

//________________________________________________________________________
void PairAnalysisPairLegCuts::Print(const Option_t* /*option*/) const
{
  //
  // Print cuts and the range
  //
  printf("------------------------------------------\n");
  printf("pair-leg cut ranges for '%s'\n",GetTitle());
  switch(fCutType) {
  case kBothLegs:     printf("Both legs have to fulfill the cuts\n"); break;
  case kAnyLeg:       printf("Any leg have to fulfill the cuts\n"); break;
  case kMixLegs:      printf("Leg1(leg2) has to fullfill the leg1(2)- or leg2(1)-cuts (mix mode)\n"); break;
  case kOneLeg:       printf("Only one of legs is allowed to fulfill the cuts\n"); break;
  }

  printf("Leg filter1: \n");
  TIter listIterator(fFilterLeg1.GetCuts());
  while (AnalysisCuts *thisCut = (AnalysisCuts*) listIterator()) {
    thisCut->Print();
  }

  printf("Leg filter2: \n");
  TIter listIterator2(fFilterLeg2.GetCuts());
  while (AnalysisCuts *thisCut = (AnalysisCuts*) listIterator2()) {
    thisCut->Print();
  }
  printf("------------------------------------------\n");

}


