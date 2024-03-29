#ifndef PAIRANALYSISVARCUTS_H
#define PAIRANALYSISVARCUTS_H
/* Copyright(c) 1998-2009, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

//#############################################################
//#                                                           #
//#         Class PairAnalysisVarCuts                           #
//#         Provide cuts for all variables handled in         #
//#           PairAnalysisVarManager                            #
//#                                                           #
//#  Authors:                                                 #
//#   Julian    Book,     Uni Ffm / Julian.Book@cern.ch       #
//#                                                           #
//#############################################################

#include <Rtypes.h>
#include <TBits.h>

#include "AnalysisCuts.h"
#include "PairAnalysisVarManager.h"

class PairAnalysisVarCuts : public AnalysisCuts {
public:
  // Whether all cut criteria have to be fulfilled of just any
  enum class ECutType
  {
    kAll = 0,
    kAny
  };

  PairAnalysisVarCuts();
  PairAnalysisVarCuts(const char* name, const char* title);
  virtual ~PairAnalysisVarCuts();
  //TODO: make copy constructor and assignment operator public

  void AddCut(PairAnalysisVarManager::ValueTypes type, Double_t min, Double_t max, Bool_t excludeRange = kFALSE);
  void AddCut(const char* formula, Double_t min, Double_t max, Bool_t excludeRange = kFALSE);
  void AddCut(PairAnalysisVarManager::ValueTypes type, Double_t value, Bool_t excludeRange = kFALSE);
  void AddBitCut(PairAnalysisVarManager::ValueTypes type, UInt_t bit, Bool_t excludeRange = kFALSE);

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

  //   virtual Bool_t IsSelected(TObject* track, TObject */*event*/=0);
  //   virtual Long64_t Merge(TCollection* /* list */)      { return 0; }

  //
  // Cut information
  //
  virtual UInt_t GetSelectedCutsMask() const { return fSelectedCutsMask; }

  virtual void Print(const Option_t* option = "") const;


private:
  TBits* fUsedVars;                                             // list of used variables
  UShort_t fActiveCuts[PairAnalysisVarManager::kNMaxValuesMC];  // list of activated cuts
  UShort_t fNActiveCuts;                                        // number of acive cuts
  UInt_t fActiveCutsMask;                                       // mask of active cuts

  UInt_t fSelectedCutsMask;  // Maks of selected cuts, is available after calling IsSelected
  ECutType fCutType;         // type of the cut: any, all

  Double_t fCutMin[PairAnalysisVarManager::kNMaxValuesMC];    // minimum values for the cuts
  Double_t fCutMax[PairAnalysisVarManager::kNMaxValuesMC];    // maximum values for the cuts
  Bool_t fCutExclude[PairAnalysisVarManager::kNMaxValuesMC];  // inverse cut logic?
  Bool_t fBitCut[PairAnalysisVarManager::kNMaxValuesMC];      // bit cut
  TObject* fCutVar[PairAnalysisVarManager::kNMaxValuesMC];    // used formula for variable

  PairAnalysisVarCuts(const PairAnalysisVarCuts& c);
  PairAnalysisVarCuts& operator=(const PairAnalysisVarCuts& c);

  ClassDef(PairAnalysisVarCuts,
           8)  //Cut class providing cuts based on all kind of variables
};


//
//Inline functions
//
inline void PairAnalysisVarCuts::AddCut(PairAnalysisVarManager::ValueTypes type, Double_t value, Bool_t excludeRange)
{
  //
  // Set cut in a small delta around value
  //
  const Double_t kDelta = 1e-20;
  AddCut(type, value - kDelta, value + kDelta, excludeRange);
}

#endif
