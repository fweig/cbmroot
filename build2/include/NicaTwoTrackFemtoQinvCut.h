/*
 * NicaTwoTrackQinvCut.h
 *
 *  Created on: 5 lip 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATWOTRACKFEMTOQINVCUT_H_
#define NICATWOTRACKFEMTOQINVCUT_H_
#include "NicaTwoTrackCut.h"
class NicaTwoTrackFemtoQinvCut : public NicaTwoTrackCut {
  Double_t fMass2;

 public:
  NicaTwoTrackFemtoQinvCut(Double_t mass = 0.13957);
  Bool_t Pass(NicaTwoTrack* pair);
  NicaPackage* Report() const;
  virtual ~NicaTwoTrackFemtoQinvCut();
  ClassDef(NicaTwoTrackFemtoQinvCut, 1)
};

#endif /* NICATWOTRACKFEMTOQINVCUT_H_ */
