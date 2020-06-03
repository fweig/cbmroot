#ifndef ANALYSISTASKMULTIPAIRANALYSIS_H
#define ANALYSISTASKMULTIPAIRANALYSIS_H

//#####################################################
//#                                                   #
//#        Basic Analysis task for PairAnalysis       #
//#          single event analysis                    #
//#                                                   #
//#     Julian Book, Uni Ffm / Julian.Book@cern.ch    #
//#                                                   #
//#####################################################

#include "TSystem.h"
#include "TStopwatch.h"
#include "TList.h"


#include "FairTask.h"
#include "CbmRichElectronIdAnn.h"

#include "AnalysisCuts.h"
#include "PairAnalysisEvent.h"
#include "PairAnalysisMetaData.h"

class PairAnalysis;
class TH1D;

class AnalysisTaskMultiPairAnalysis : public FairTask {

public:
  AnalysisTaskMultiPairAnalysis();
  AnalysisTaskMultiPairAnalysis(const char *name);
  virtual ~AnalysisTaskMultiPairAnalysis();

  virtual InitStatus Init();
  virtual void Exec(Option_t *option);
  virtual void FinishTask();

  void SetEventFilter(AnalysisCuts * const filter) {fEventFilter=filter;}

  void AddPairAnalysis(PairAnalysis * const papa)  { fListPairAnalysis.Add((TObject*)papa); } //why cast to tobject????????
  void SetBeamEnergy(Double_t beamEbyHand=-1.)     { fBeamEnergy=beamEbyHand;
    Warning("SetBeamEnergy","This function is only kept for backward compatibility!!! Value will be overwritten, when parameter is availbale (>JUN16 release).");
  } // TODO: this should become obsolete in NOV16

protected:

  PairAnalysisMetaData fMetaData;             //! List of meta data in the framework classes
  TObjArray *fPairArray = NULL;               //! output array
  TList fListPairAnalysis;                    //  List of papa framework instances
  TList fListHistos;                          //! List of histogram manager lists in the framework classes

  Double_t fBeamEnergy  = 0.;                 // beam energy in GeV (set by hand)

  AnalysisCuts      *fEventFilter = NULL;     // event filter
  PairAnalysisEvent *fInputEvent  = NULL;     //! event handler
  Int_t fEventsTotal    = 0;                  // total number of processed events
  Int_t fEventsSelected = 0;                  // number of selcted events

  TStopwatch fTimer;                          //! stopwatch for cpu consumption
  ProcInfo_t fProcInfo;                       //! memory usage

  AnalysisTaskMultiPairAnalysis(const AnalysisTaskMultiPairAnalysis &c);
  AnalysisTaskMultiPairAnalysis& operator= (const AnalysisTaskMultiPairAnalysis &c);

  ClassDef(AnalysisTaskMultiPairAnalysis, 4); //Analysis Task handling multiple instances of PairAnalysis
};
#endif
