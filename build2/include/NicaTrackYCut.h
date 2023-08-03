/*
 * NicaTrackYCut.h
 *
 *  Created on: 03-11-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATRACKYCUT_H_
#define NICATRACKYCUT_H_
#include "NicaTrackCut.h"
/**
 * check track rapidity
 */
class NicaTrackYCut : public NicaTrackCut {
 public:
  NicaTrackYCut();
  virtual Bool_t Pass(NicaTrack *track);
  virtual ~NicaTrackYCut();
  ClassDef(NicaTrackYCut, 1)
};

#endif /* NICATRACKYCUT_H_ */
