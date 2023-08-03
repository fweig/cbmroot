/*
 * NicaTrackMassCut.h
 *
 *  Created on: 26-03-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICATRACKMASSCUT_H_
#define NICATRACKMASSCUT_H_

#include "NicaTrackCut.h"
/**
 * check track mass
 */
class NicaTrackMassCut : public NicaTrackCut {
 public:
  NicaTrackMassCut();
  Bool_t Pass(NicaTrack *track);
  virtual ~NicaTrackMassCut();
  ClassDef(NicaTrackMassCut, 1)
};

#endif /* NICATRACKMASSCUT_H_ */
