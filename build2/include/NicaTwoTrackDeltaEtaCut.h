/*
 * NicaTwoTrackDeltaEtaCut.h
 *
 *  Created on: 28 lip 2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef _NICATWOTRACKDELTAETACUT_H_
#define _NICATWOTRACKDELTAETACUT_H_
#include "NicaTwoTrackCut.h"
/**
 * check delta eta value pass pair if values of cut are outside of given range
 */
class NicaTwoTrackDeltaEtaCut : public NicaTwoTrackCut {
  Bool_t fAntiValidate;

 public:
  NicaTwoTrackDeltaEtaCut();
  NicaTwoTrackDeltaEtaCut(const NicaTwoTrackDeltaEtaCut &other);
  virtual void StandardValidation() { fAntiValidate = kFALSE; };
  virtual Bool_t Pass(NicaTwoTrack *pair);
  virtual ~NicaTwoTrackDeltaEtaCut();
  virtual NicaPackage *Report() const;
  ClassDef(NicaTwoTrackDeltaEtaCut, 1)
};

#endif /* _NICATWOTRACKDELTAETACUT_H_ */
