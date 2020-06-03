/** @file CbmBuildEventsSimple.h
 ** @author //Dr.Sys <Mikhail.Prokudin@cern.ch>
 ** @date 07.12.2016
 **/
#ifndef CBMBUILDEVENTSSIMPLE_H
#define CBMBUILDEVENTSSIMPLE_H


#include <FairTask.h>

class TClonesArray;

/** @class CbmStsBuildEventsSimple
 ** @brief Task class for associating digis to events using STS digis dynamics in time
 ** @author //Dr.Sys <Mikhail.Prokudin@cern.ch>
 ** @date 07.12.2016 
 ** @version 1.0
 **
 ** Event formation starts when number of STS digis in a given time window
 ** exceeds the threshold. STS digis in extended (in comparison with
 ** found time window) time period are attributed to a found event.
 **/
class CbmBuildEventsSimple: public FairTask
{
public:
  /** Constructor **/
  CbmBuildEventsSimple();

  /** Destructor **/
  virtual ~CbmBuildEventsSimple();

  /** Task execution **/
  virtual void Exec(Option_t* opt);

  // Setters. Time in [ns], events in counts :) Only STS information is used.
  void SetWindDur         (Double_t winddur=5)           {fWindDur=winddur;}
  void SetMinusDeltaT     (Double_t minusdeltat=10)       {fMinusDeltaT=minusdeltat;}
  void SetPlusDeltaT      (Double_t plusdeltat=15)        {fPlusDeltaT=plusdeltat;}
  void SetMinHitStations  (Int_t minhitstations=8)       {fMinHitStations=minhitstations;}
  void SetMinDigis        (Int_t mindigis=2000)            {fMinDigis=mindigis;}
  void SetDeadT           (Double_t deadt=10)            {fDeadT=deadt;}


private:
  TClonesArray* fStsDigis;   ///< Input array (class CbmStsDigi)
  TClonesArray* fEvents;     ///< Output array (class CbmEvent)
  Int_t fSliceN;	     // Number of current time slice

  Int_t fEv;		// Number of current event
  Int_t fNDigis;	// Number of digis in current time window
  Int_t fSN;		// N of start of time window
  Double_t fST;		// Time of start of time window
  Int_t fNStsDigis[16];	// Number of STS digis per station
  Double_t fWindDur;	// Duration of time window [ns]
  Double_t fMinusDeltaT;// [ns] before start of time window for event construction
  Double_t fPlusDeltaT;	// [ns] after end of time window for event construction
  Double_t fDeadT;	// [ns] after last accepted digi of blindless
  Int_t fMinHitStations;// Minimum number of hit STS stations for event formation
  Int_t fMinDigis;	// Minimum number of digis in time window for event formation

  /** Task initialisation **/
  virtual InitStatus Init();

   /** Fills Stsdigis array. STS separate, because start and end end digi is known. **/
  void FillEvent(Int_t st, Int_t end);

  CbmBuildEventsSimple(const CbmBuildEventsSimple&);
  CbmBuildEventsSimple& operator=(const CbmBuildEventsSimple&);

  ClassDef(CbmBuildEventsSimple, 1);

};

#endif /* CBMBUILDEVENTSSIMPLE_H */
