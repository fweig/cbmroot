/*
 * NicaTwoTrackLCMSCut.h
 *
 *  Created on: 3 kwi 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef KINEMATICS_NICATWOTRACKLCMSCUT_H_
#define KINEMATICS_NICATWOTRACKLCMSCUT_H_

#include "NicaTwoTrackCut.h"

class NicaTwoTrackLCMSCut : public NicaTwoTrackCut {
  Float_t fM;

 public:
  NicaTwoTrackLCMSCut();
  virtual Bool_t Pass(NicaTwoTrack *pair);
  void SetMass(Double_t m) { fM = m * m; };
  static Int_t Rout() { return 0; };
  static Int_t Rside() { return 1; };
  static Int_t Rlong() { return 2; };
  virtual ~NicaTwoTrackLCMSCut();
  ClassDef(NicaTwoTrackLCMSCut, 1)
};

#endif /* KINEMATICS_NICATWOTRACKLCMSCUT_H_ */
