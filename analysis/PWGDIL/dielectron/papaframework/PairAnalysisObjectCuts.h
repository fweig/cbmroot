/* Copyright (C) 2015-2020 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Julian Book [committer], Etienne Bechtel */

#ifndef PAIRANALYSISOBJECTCUTS_H
#define PAIRANALYSISOBJECTCUTS_H

//#############################################################
//#                                                           #
//#         Class PairAnalysisObjectCuts                     #
//#         Provide cuts for using objects                    #
//#                                                           #
//#  Authors:                                                 #
//#   Julian    Book,     Uni Ffm / Julian.Book@cern.ch       #
//#                                                           #
//#############################################################

#include <Rtypes.h>
#include <TBits.h>

#include "AnalysisCuts.h"
#include "PairAnalysisVarManager.h"

class TGraph;
class THnBase;
class PairAnalysisObjectCuts : public AnalysisCuts {
public:
  // Whether all cut criteria have to be fulfilled of just any
  enum class ECutType
  {
    kAll = 0,
    kAny
  };
  static const Int_t fMaxCuts = 10;

  PairAnalysisObjectCuts();
  PairAnalysisObjectCuts(const char* name, const char* title);
  virtual ~PairAnalysisObjectCuts();
  //TODO: make copy constructor and assignment operator public
  void AddCut(PairAnalysisVarManager::ValueTypes type, const char* formulaMin, const char* formulaMax,
              Bool_t excludeRange = kFALSE);
  void AddCut(const char* formula, const char* formulaMin, const char* formulaMax, Bool_t excludeRange = kFALSE);

  void AddCut(PairAnalysisVarManager::ValueTypes type, TGraph* const graphMin, TGraph* const graphMax,
              Bool_t excludeRange = kFALSE);
  void AddCut(const char* formula, TGraph* const graphMin, TGraph* const graphMax, Bool_t excludeRange = kFALSE);

  void AddCut(PairAnalysisVarManager::ValueTypes type, THnBase* const histMin, THnBase* const histMax,
              Bool_t excludeRange = kFALSE);
  void AddCut(const char* formula, THnBase* const histMin, THnBase* const histMax, Bool_t excludeRange = kFALSE);

  // setters
  void SetCutType(ECutType type) { fCutType = type; }

  // getters
  ECutType GetCutType() const { return fCutType; }

  Int_t GetNCuts() { return fNActiveCuts; }

  //
  //Analysis cuts interface
  //
  virtual Bool_t IsSelected(Double_t* const values);
  virtual Bool_t IsSelected(TObject* track);
  virtual Bool_t IsSelected(TList* /* list */) { return kFALSE; }

  //
  // Cut information
  //
  virtual UInt_t GetSelectedCutsMask() const { return fSelectedCutsMask; }

  virtual void Print(const Option_t* option = "") const;


private:
  TBits* fUsedVars;                // list of used variables
  UShort_t fActiveCuts[fMaxCuts];  // list of activated cuts
  UShort_t fNActiveCuts  = 0;      // number of acive cuts
  UInt_t fActiveCutsMask = 0;      // mask of active cuts

  UInt_t fSelectedCutsMask = 0;               // Maks of selected cuts, is available after calling IsSelected
  ECutType fCutType        = ECutType::kAll;  // type of the cut: any, all

  Bool_t fCutExclude[fMaxCuts];     // inverse cut logic?
  TObject* fCutMin[fMaxCuts];       // use object as lower cut
  TObject* fCutMax[fMaxCuts];       // use object as upper cut
  TFormula* fVarFormula[fMaxCuts];  // use a formula for the variable

  PairAnalysisObjectCuts(const PairAnalysisObjectCuts& c);
  PairAnalysisObjectCuts& operator=(const PairAnalysisObjectCuts& c);

  ClassDef(PairAnalysisObjectCuts,
           1)  // Cut class for special cuts (formulas, graphs, histograms)
};


#endif
