/*
 * NicaV0BasicCut.h
 *
 *  Created on: 26 mar 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAV0CANDBASICCUT_H_
#define NICAV0CANDBASICCUT_H_
/**
 * basic class for V0 selection
 */
#include "NicaTwoTrackCut.h"
class NicaV0CandBasicCut : public NicaTwoTrackCut {
 public:
  NicaV0CandBasicCut();
  virtual Bool_t Pass(NicaTwoTrack *pair);
  static Int_t DCA1to2() { return 0; };
  static Int_t DCAToPV() { return 1; };
  static Int_t PtArm() { return 2; };
  static Int_t AlphaArm() { return 3; };
  static Int_t CosAngle() { return 4; };
  static Int_t DecLength() { return 5; };
  static Int_t InvMass() { return 6; };
  virtual ~NicaV0CandBasicCut();
  ClassDef(NicaV0CandBasicCut, 1)
};

#endif /* NICAV0CANDBASICCUT_H_ */
