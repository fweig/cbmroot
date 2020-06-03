#ifndef PAIRANALYSISMIXEDEVENT_H
#define PAIRANALYSISMIXEDEVENT_H

/* Copyright(c) 1998-2009, ALICE Experiment at CERN, All rights reserved. */

//#############################################################
//#                                                           #
//#         Class PairAnalysisMixedEvent                      #
//#                                                           #
//#  Authors:                                                 #
//#   Julian    Book,     Uni Ffm / Julian.Book@cern.ch       #
//#                                                           #
//#############################################################

#include <TNamed.h>
#include <TClonesArray.h>

#include "PairAnalysisVarManager.h"


class TObjArray;
class TProcessID;

class PairAnalysisMixedEvent : public TNamed {
public:
  PairAnalysisMixedEvent();
  PairAnalysisMixedEvent(const char*name, const char* title);

  virtual ~PairAnalysisMixedEvent();

  void Set(Int_t size=100);

  void SetTracks(const TObjArray &arrP, const TObjArray &arrN);
  void SetEventData(const Double_t data[PairAnalysisVarManager::kNMaxValuesMC]);
  const Double_t* GetEventData() const {return fEventData;}

  const TClonesArray* GetTrackArrayP() const { return &fArrTrackP; }
  const TClonesArray* GetTrackArrayN() const { return &fArrTrackN; }

  Int_t GetNTracksP() const { return fNTracksP; }
  Int_t GetNTracksN() const { return fNTracksN; }

  void SetProcessID(TProcessID *pid) { fPID=pid;    }
  const TProcessID* GetProcessID()   { return fPID; }

  virtual void Clear(Option_t *opt="C");


private:
  TClonesArray fArrTrackP;      //positive tracks
  TClonesArray fArrTrackN;      //negative tracks

  Int_t fNTracksP = 0;          //number of positive tracks
  Int_t fNTracksN = 0;          //number of negative tracks

  Double_t fEventData[PairAnalysisVarManager::kNMaxValuesMC]; // event informaion from the var manager

  TProcessID *fPID = NULL;      //! internal PID for references to buffered objects
  UInt_t      fPIDIndex = 0;    //! index of PID

  PairAnalysisMixedEvent(const PairAnalysisMixedEvent &c);
  PairAnalysisMixedEvent &operator=(const PairAnalysisMixedEvent &c);

  void AssignID(TObject *obj);

  ClassDef(PairAnalysisMixedEvent,2)         // Small mixed event structure
};



#endif
