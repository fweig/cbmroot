/*
 * NicaPdgBinPairCut.h
 *
 *  Created on: 2 sie 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAPDGBINPAIRCUT_H_
#define NICAPDGBINPAIRCUT_H_
#include "NicaMCTrack.h"
#include "NicaTwoTrackCut.h"
class NicaPdgBinPairCut : public NicaTwoTrackCut {
  Int_t PidToID(NicaMCTrack *tr) const;

 public:
  NicaPdgBinPairCut();
  virtual Bool_t Pass(NicaTwoTrack *pair);
  static Int_t FirstParticle() { return 0; }
  static Int_t SecondParticle() { return 1; };
  virtual ~NicaPdgBinPairCut();
  ClassDef(NicaPdgBinPairCut, 1)
};

#endif /* NICAPDGBINPAIRCUT_H_ */
