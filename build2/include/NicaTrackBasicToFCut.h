/*
 * NicaToFBasicCut.h
 *
 *  Created on: 26 wrz 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATRACKBASICTOFCUT_H_
#define NICATRACKBASICTOFCUT_H_

#include "NicaTrackExpCut.h"
class NicaTrackBasicToFCut : public NicaTrackExpCut {
  static const Int_t fgMass2;
  static const Int_t fgBeta;
  static const Int_t fgFlag;

 public:
  NicaTrackBasicToFCut();
  Bool_t Init(Int_t formad_id);
  static Int_t Beta() { return fgBeta; };
  static Int_t Mass2() { return fgMass2; };
  static Int_t Flag() { return fgFlag; };
  virtual Bool_t Pass(NicaTrack *tr);
  virtual ~NicaTrackBasicToFCut();
  ClassDef(NicaTrackBasicToFCut, 1)
};

#endif /* NICATRACKBASICTOFCUT_H_ */
