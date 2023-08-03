/*
 * NicaTrackBetaCut.h
 *
 *  Created on: 23 lut 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATRACKBETACUT_H_
#define NICATRACKBETACUT_H_

#include "NicaTrackCut.h"
/**
 * class for making beta cut
 */
class NicaTrackBetaCut : public NicaTrackCut {
 public:
  NicaTrackBetaCut();
  Bool_t Pass(NicaTrack *track);
  virtual ~NicaTrackBetaCut();
  ClassDef(NicaTrackBetaCut, 1);
};

#endif /* NICATRACKBETACUT_H_ */
