/*
 * NicaTwoTrackFemtoPRFCut.h
 *
 *  Created on: 25 lis 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATWOTRACKFEMTOPRFCUT_H_
#define NICATWOTRACKFEMTOPRFCUT_H_

#include <TLorentzVector.h>
#include "NicaTwoTrackCut.h"

class NicaTwoTrackFemtoPRFCut : public NicaTwoTrackCut {
  TLorentzVector fP1, fP2, fX1, fX2;
  void Compute();

 public:
  NicaTwoTrackFemtoPRFCut();
  virtual Bool_t Pass(NicaTwoTrack *pair);
  virtual Bool_t Init(Int_t task_id);
  static Int_t RStar() { return 1; };
  static Int_t KStar() { return 0; };
  virtual ~NicaTwoTrackFemtoPRFCut();
  ClassDef(NicaTwoTrackFemtoPRFCut, 1)
};

#endif /* NICATWOTRACKFEMTOPRFCUT_H_ */
