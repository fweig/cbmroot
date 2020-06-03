#ifndef PAIRANALYSISCUTCOMBI_H
#define PAIRANALYSISCUTCOMBI_H
//#############################################################
//#                                                           # 
//#         Class PairAnalysisCutCombi                     #
//#                                                           #
//#  Authors:                                                 #
//#   Julian    Book,     Uni Ffm / Julian.Book@cern.ch       #
//#                                                           #
//#############################################################

#include <TBits.h>
#include <TString.h>

#include <AnalysisCuts.h>

class TList;
class PairAnalysisVarManager;

class PairAnalysisCutCombi : public AnalysisCuts {
public:
  // Whether all cut criteria have to be fulfilled of just any
  enum CutType { kAll=0, kAny };

  PairAnalysisCutCombi();
  PairAnalysisCutCombi(const char*name, const char* title);
  virtual ~PairAnalysisCutCombi();

  void AddCut(AnalysisCuts *cuts, AnalysisCuts *range );

  // setters
  void    SetCutType(CutType type)         { fCutType=type;    }

  // getters
  CutType GetCutType()      const { return fCutType;      }
  Int_t GetNCuts() {                return fNActiveCuts;  }

  //
  //Analysis cuts interface
  //
  virtual Bool_t IsSelected(Double_t * const values);
  virtual Bool_t IsSelected(TObject* track);
  virtual Bool_t IsSelected(TList*   /* list */ ) {return kFALSE;}

  //
  // Cut information
  //
  virtual UInt_t GetSelectedCutsMask() const { return fSelectedCutsMask; }
  virtual void Print(const Option_t* option = "") const;


private:
  enum {kNmaxCuts=30};

  UShort_t  fNActiveCuts;           // number of acive cuts
  UInt_t    fActiveCutsMask;        // mask of active cuts

  UInt_t   fSelectedCutsMask;       // Maks of selected cuts, is available after calling IsSelected
  CutType  fCutType;                // type of the cut: any, all

  AnalysisCuts *fRangeCuts[kNmaxCuts];     // cuts to select a range
  AnalysisCuts *fCuts[kNmaxCuts];          // where these cuts are applied

  PairAnalysisCutCombi(const PairAnalysisCutCombi &c);
  PairAnalysisCutCombi &operator=(const PairAnalysisCutCombi &c);

  ClassDef(PairAnalysisCutCombi,1)         // Apply cuts for certain conditions/ranges
};

#endif
