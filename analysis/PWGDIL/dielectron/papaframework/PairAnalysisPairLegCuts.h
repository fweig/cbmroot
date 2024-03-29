#ifndef PAIRANALYSISPAIRLEGCUTS_H
#define PAIRANALYSISPAIRLEGCUTS_H

/* Copyright(c) 1998-2009, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

//#############################################################
//#                                                           #
//#         Class PairAnalysisPairLegCuts                       #
//#         Manage Cuts on the legs of the pair               #
//#                                                           #
//#  Authors:                                                 #
//#   Julian    Book,     Uni Ffm / Julian.Book@cern.ch       #
//#                                                           #
//#############################################################

#include "AnalysisCuts.h"
#include "AnalysisFilter.h"

class PairAnalysisPairLegCuts : public AnalysisCuts {
public:
  enum ECutType
  {
    kBothLegs = 0,
    kAnyLeg,
    kMixLegs,
    kOneLeg
  };

  PairAnalysisPairLegCuts();
  PairAnalysisPairLegCuts(const char* name, const char* title);
  virtual ~PairAnalysisPairLegCuts() { ; }
  //TODO: make copy constructor and assignment operator public
  //      and implement them

  //
  //AnalysisCuts interface
  //
  virtual Bool_t IsSelected(TObject* track);
  virtual Bool_t IsSelected(TList* /* list */) { return kFALSE; }
  //   virtual Long64_t Merge(TCollection* /* list */)      { return 0; }

  AnalysisFilter& GetLeg1Filter() { return fFilterLeg1; }
  AnalysisFilter& GetLeg2Filter() { return fFilterLeg2; }

  void SetCutType(ECutType type) { fCutType = type; }
  virtual void Print(const Option_t* option = "") const;

private:
  AnalysisFilter fFilterLeg1;  // Analysis Filter for leg1
  AnalysisFilter fFilterLeg2;  // Analysis Filter for leg2

  ECutType fCutType = kBothLegs;  // Type of the cut

  PairAnalysisPairLegCuts(const PairAnalysisPairLegCuts& c);
  PairAnalysisPairLegCuts& operator=(const PairAnalysisPairLegCuts& c);

  ClassDef(PairAnalysisPairLegCuts,
           1)  //Cut class providing cuts for both legs of a pair
};

#endif
