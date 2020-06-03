//////////////////////////////////////////////////////////////////////////
//                                                                      //
// Authors:
//   * Copyright(c) 1998-2009, ALICE Experiment at CERN, All rights reserved. *
//   Julian Book   <Julian.Book@cern.ch>
//
//
//                                                                      //
//   Allow to define groups of cut conditions which are tested with     //
//      an OR condition between groups and an AND within groups         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "PairAnalysisCutGroup.h"
#include "PairAnalysisVarManager.h"

ClassImp(PairAnalysisCutGroup)

PairAnalysisCutGroup::PairAnalysisCutGroup(Bool_t compOperator /*=kCompOR*/) :
  AnalysisCuts(),
  fCutGroupList(0x0),
  fCompOperator(compOperator)
{
  //
  // Default constructor
  //
}

//_____________________________________________________________________
PairAnalysisCutGroup::PairAnalysisCutGroup(const char* name, const char* title, Bool_t compOperator /*=kCompOR*/) :
  AnalysisCuts(name, title),
  fCutGroupList(0x0),
  fCompOperator(compOperator)
{
  //
  // Named Constructor
  //
}

//_____________________________________________________________________
PairAnalysisCutGroup::~PairAnalysisCutGroup() 
{
  //
  //Default Destructor
  //
}

//_____________________________________________________________________
void PairAnalysisCutGroup::Init()
{
    // Loop over all cuts and call Init
  TIter next(&fCutGroupList);
  while (AnalysisCuts *thisCut = (AnalysisCuts*) next())    thisCut->Init();
}

//________________________________________________________________________
Bool_t PairAnalysisCutGroup::IsSelected(Double_t * const values)
{
  //
  // Make cut decision
  //

  //Different init for and/or makes code shorter
  Bool_t selectionResult=fCompOperator;

  TIter listIterator(&fCutGroupList);
  while (AnalysisCuts *thisCut = (AnalysisCuts*) listIterator()) {
    if (fCompOperator == kCompOR) {
      selectionResult = (selectionResult || thisCut->IsSelected(values));
    }
    else { //kCompAND
      selectionResult = (selectionResult && thisCut->IsSelected(values));
      ///      if (selectionResult==kFALSE) break; //Save loops vs. additional check?
    }
  }
  return selectionResult;

}

//_____________________________________________________________________
Bool_t PairAnalysisCutGroup::IsSelected(TObject* track) 
{
  //
  // Selection-finder handling different comparison operations
  //

  if (!track) return kFALSE;

  //Fill values
  Double_t *values=PairAnalysisVarManager::GetData();
  ///  PairAnalysisVarManager::SetFillMap(fUsedVars);
  PairAnalysisVarManager::Fill(track,values);

  /// selection
  return (IsSelected(values));

}

//_____________________________________________________________________

void PairAnalysisCutGroup::AddCut(AnalysisCuts* fCut) 
{
  //
  // Add a defined cut to the list
  //
  
  fCutGroupList.Add(fCut);
}

//_____________________________________________________________________
void PairAnalysisCutGroup::SetCompOperator(Bool_t compOperator) 
{
  //
  // Switch between AND/OR
  //
  
  fCompOperator = compOperator;
}

//________________________________________________________________________
void PairAnalysisCutGroup::Print(const Option_t* /*option*/) const
{
  //
  // Print cuts and the range
  //

  printf("*****************************************************************\n");
  printf("cut group '%s'\n",GetTitle());
  if (fCompOperator==kCompAND){
    printf("Cut are compared with AND \n");
  } else {
    printf("Cut are compared with OR \n");
  }
  TIter listIterator(&fCutGroupList);
  while (AnalysisCuts *thisCut = (AnalysisCuts*) listIterator()) {
    thisCut->Print();
  }
  printf("*****************************************************************\n");

}
