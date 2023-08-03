/*
 * NicaFemtoDumpPairAna.h
 *
 *  Created on: 9 lut 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAFEMTODUMPPAIRANA_H_
#define NICAFEMTODUMPPAIRANA_H_

#include "NicaFemtoBasicAna.h"
#include "NicaFemtoMiniPair.h"

class TClonesArray;

class NicaFemtoDumpPairAna : public NicaFemtoBasicAna {
  Int_t fBinLimit;
  Int_t fNBins;
  NicaArray_1<Double_t> fLimitsN;
  NicaArray_1<Double_t> fLimitsD;
  Double_t fStep, fMax;
  Bool_t fWriteBackground;
  TClonesArray* fSignalPairs;
  TClonesArray* fBackgroundPairs;

protected:
  virtual void ProcessFemtoPair();
  virtual void ProcessFemtoPair_Perfect() {};
  virtual void ProcessFemtoPair_Rotated();
  virtual void ProcessFemtoPair_Hemisphere();
  virtual void ProcessFemtoPair_Mixed();
  virtual void FinishEventIdentical();
  virtual void FinishEventNonIdentical();
  virtual InitStatus Init();

public:
  NicaFemtoDumpPairAna();
  void SetPairLimitPerBin(Int_t limit) { fBinLimit = limit; };
  void WriteBackground() { fWriteBackground = kTRUE; }
  virtual ~NicaFemtoDumpPairAna();
  ClassDef(NicaFemtoDumpPairAna, 1)
};

#endif /* NICAFEMTODUMPPAIRANA_H_ */
