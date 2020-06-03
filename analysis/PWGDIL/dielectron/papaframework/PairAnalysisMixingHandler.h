#ifndef PAIRANALYSISMIXINGHANDLER_H
#define PAIRANALYSISMIXINGHANDLER_H

/* Copyright(c) 1998-2009, ALICE Experiment at CERN, All rights reserved. */

//#############################################################
//#                                                           #
//#         Class PairAnalysisMixingHandler                   #
//#                                                           #
//#  Authors:                                                 #
//#   Julian    Book,     Uni Ffm / Julian.Book@cern.ch       #
//#                                                           #
//#############################################################

#include <TNamed.h>
#include <TProcessID.h>
#include <TObjArray.h>
#include <TClonesArray.h>

#include "PairAnalysisVarManager.h"

class PairAnalysis;
class PairAnalysisTrack;
class PairAnalysisEvent;

class PairAnalysisMixingHandler : public TNamed {
public:
  enum { kMaxCuts=10 };
  enum EMixType {
    kOSonly=0,
    kOSandLS,
    kAll
  };
  PairAnalysisMixingHandler();
  PairAnalysisMixingHandler(const char*name, const char* title);

  virtual ~PairAnalysisMixingHandler();

  void AddVariable(PairAnalysisVarManager::ValueTypes type, TVectorD* const bins);

  void SetDepth(UShort_t depth) { fDepth=depth; }
  UShort_t GetDepth()     const { return fDepth; }

  void SetMixType(EMixType type) { fMixType=type; }
  EMixType GetMixType() const    { return fMixType; }

  Int_t GetNumberOfBins() const;
  Int_t FindBin(const Double_t values[], TString *dim=0x0);
  void Fill(const PairAnalysisEvent *ev, PairAnalysis *papa);

  void Init(const PairAnalysis *papa=0x0);

private:
  UShort_t  fDepth   = 10;        // Number of events per bin to keep in the ring buffer
  TClonesArray fArrPools;         // Array of events in bins
  UShort_t  fEventCuts[kMaxCuts]; // cut variables
  TObjArray fAxes;                // Axis descriptions of the event binning
  EMixType  fMixType = kOSonly;   // which combinations to include in the mixing
  TProcessID *fPID   = NULL;      //! internal PID for references to buffered objects
  
  void DoMixing(TClonesArray &pool, PairAnalysis *papa);

  PairAnalysisMixingHandler(const PairAnalysisMixingHandler &c);
  PairAnalysisMixingHandler &operator=(const PairAnalysisMixingHandler &c);

  
  ClassDef(PairAnalysisMixingHandler,1)         // Mixed event handler
};



#endif
