/*
 * NicaFemto1DCFAnaMapPairs.h
 *
 *  Created on: 27 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAFEMTO1DCFANAMAPPAIRSDUMPED_H_
#define NICAFEMTO1DCFANAMAPPAIRSDUMPED_H_

#include <TString.h>

#include "NicaFemto1DCFAnaMapMC.h"

class TFile;
class TTree;
class TClonesArray;

/**
 * class for generating 1-dim HBT map, use pairs from dumped file
 */
class NicaFemto1DCFAnaMapPairsDumped : public NicaFemto1DCFAnaMapMC {
protected:
  TString fInFile;
  TFile* fFile;
  TTree* fTree;
  TClonesArray* fPairsSignal;
  TClonesArray* fPairsBackground;
  Bool_t fUseBackground;
  Bool_t fWeightedBackround;
  Bool_t fIgnoreSign;
  Int_t fMaxEvents;
  void MakePairsWeighted(TClonesArray* arr, Bool_t num);
  void MakePairsUnWeighted(TClonesArray* arr, Bool_t num);

public:
  NicaFemto1DCFAnaMapPairsDumped();
  void SetInFile(TString filename);
  void UseBackground() { fUseBackground = kTRUE; };
  void UseWeightedBackground() { fWeightedBackround = kTRUE; };
  void DisableSignIgnoring() { fIgnoreSign = kFALSE; };
  virtual Bool_t Init();
  virtual void Exec(Int_t /* npairs*/) { Run(0, 0); }
  /**
   *
   * @param start_event - first event
   * @param end_event - last event
   * if start == end process all events
   */
  virtual void Run(Int_t start_event = 0, Int_t end_event = 0);
  virtual ~NicaFemto1DCFAnaMapPairsDumped();
  ClassDef(NicaFemto1DCFAnaMapPairsDumped, 1)
};

#endif /* NICAFEMTO1DCFANAMAPPAIRSDUMPED_H_ */
