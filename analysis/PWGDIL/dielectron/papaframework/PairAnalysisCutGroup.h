#ifndef PAIRANALYSISCUTGROUP_H
#define PAIRANALYSISCUTGROUP_H

/* Copyright(c) 1998-2009, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

//#################################################################
//#                                                               #
//#             Class PairAnalysisCutGroup                       #
//#              PairAnalysis Group of cuts                         #
//#                                                               #
//#  Authors:                                                     #
//#   Julian    Book,     Uni Ffm / Julian.Book@cern.ch           #
//#                                                               #
//#################################################################

#include <TList.h>

#include "AnalysisCuts.h"

class TCollection;

class PairAnalysisCutGroup : public AnalysisCuts {

public:
  enum class ETruthValues
  {
    kCompAND = kTRUE,
    kCompOR  = kFALSE
  };

  PairAnalysisCutGroup(Bool_t compOperator = static_cast<Bool_t>(ETruthValues::kCompOR));
  PairAnalysisCutGroup(const char* name, const char* title,
                       Bool_t compOperator = static_cast<Bool_t>(ETruthValues::kCompOR));

  virtual ~PairAnalysisCutGroup();

  //Analysis cuts interface
  //
  virtual void Init();
  virtual Bool_t IsSelected(Double_t* const values);
  virtual Bool_t IsSelected(TObject* track);
  virtual Bool_t IsSelected(TList* /* list */) { return kFALSE; }

  void AddCut(AnalysisCuts* fCut);
  void SetCompOperator(Bool_t compOperator);

  virtual void Print(const Option_t* option = "") const;


private:
  TList fCutGroupList;   // List of the different cuts
  Bool_t fCompOperator;  // Determines whether the cuts are AND/OR compared

  ClassDef(PairAnalysisCutGroup,
           1)  // Collect cuts in groups with logical comparators
};

#endif
