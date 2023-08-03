/*
 * NicaPairDeltaQinvCut.h
 *
 *  Created on: 12 lis 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAPAIRDELTAQINVCUT_H_
#define NICAPAIRDELTAQINVCUT_H_

#include "NicaTwoTrackCut.h"

class NicaTrack;

class NicaPairDeltaQinvCut : public NicaTwoTrackCut {
  Double_t fMass1, fMass2;
  Double_t Q(NicaTrack *tr1, NicaTrack *tr2) const;

 public:
  NicaPairDeltaQinvCut();
  virtual Bool_t Pass(NicaTwoTrack *pair);
  static Int_t Relative() { return 0; }
  static Int_t Absolute() { return 1; };
  Bool_t Init(Int_t task_id);
  void SetMass(Double_t m) { fMass1 = fMass2 = m; };
  void SetMass(Double_t m1, Double_t m2) {
    fMass1 = m1;
    fMass2 = m2;
  };
  virtual ~NicaPairDeltaQinvCut();
  ClassDef(NicaPairDeltaQinvCut, 1)
};

#endif /* NICAPAIRDELTAQINVCUT_H_ */
