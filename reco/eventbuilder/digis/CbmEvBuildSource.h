// FairSource which reads a time slice and creates events using a simple approach
// Use the same approach as CbmBuildEventSimple
// //Dr.Sys : mikhail.prokudin@cern.ch

#ifndef CBMEVBUILDSOURCE_H
#define CBMEVBUILDSOURCE_H

#include "FairSource.h"

#include <iostream>

class TChain;
class TClonesArray;
class CbmTimeSlice;
class FairEventHeader;

class CbmEvBuildSource : public FairSource 
{
public:
  CbmEvBuildSource();
  CbmEvBuildSource(const char* fname);
  virtual ~CbmEvBuildSource();
  void AddFile(const char* fname);

  virtual Bool_t Init();
  virtual Int_t ReadEvent(UInt_t par=0);
  virtual void Close();

  virtual void Reset();

  //virtual Source_Type GetSourceType() {return kFILE;}

  virtual Bool_t ActivateObject(TObject**, const char*)  { return kFALSE; }
    
  /**Check the maximum event number we can run to*/
  virtual Int_t CheckMaxEventNo(Int_t =0) {return 1000000000;}
  /**Read the tre entry on one branch**/
  virtual void ReadBranchEvent(const char*) {return;}
  virtual void ReadBranchEvent(const char*, Int_t) {return;}
  virtual void FillEventHeader(FairEventHeader* feh); 

  // Setters. Time in [ns], events in counts :) Only STS information is used.
  void SetWindDur         (Double_t winddur=2)           {fWindDur=winddur;}
  void SetMinusDeltaT     (Double_t minusdeltat=2)       {fMinusDeltaT=minusdeltat;}
  void SetPlusDeltaT      (Double_t plusdeltat=5)        {fPlusDeltaT=plusdeltat;}
  void SetMinHitStations  (Int_t minhitstations=8)       {fMinHitStations=minhitstations;}
  void SetMinDigis        (Int_t mindigis=2000)          {fMinDigis=mindigis;}
  void SetDeadT           (Double_t deadt=10)            {fDeadT=deadt;}

public:
  CbmEvBuildSource(const CbmEvBuildSource& source);
  void FillEvent(Int_t st, Int_t end);	//Fill output TClonesArray
  Int_t GetNextTimeSlice();		//Load next time slice from disk

private:
  TChain* fCh;			//!Chain of input files
  Int_t fN;			//Number of entries in input tree
  Int_t fI;			//Current entry in tree
  TClonesArray* fSTSDigi;	//!Array of output STS digis for each built event
  Int_t fNSTSDigis;		//Number of STS digis in current event
  CbmTimeSlice* fSlice;		//!Current time slice
  Int_t fISts;			//Number of STS digi under consideration

  UInt_t fEv;		// Number of current event **/
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
  FairEventHeader* fEvHeader; //! Event header 

  CbmEvBuildSource& operator=(const CbmEvBuildSource&);

  ClassDef(CbmEvBuildSource, 1)
};


#endif
