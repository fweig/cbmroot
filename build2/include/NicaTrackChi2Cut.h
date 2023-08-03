/*
 * NicaTrackChi2Cut.h
 *
 *  Created on: 13 lis 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATRACKCHI2CUT_H_
#define NICATRACKCHI2CUT_H_
#include "NicaTrackExpCut.h"
class NicaTrackChi2Cut : public NicaTrackExpCut {
 public:
  NicaTrackChi2Cut();
  virtual Bool_t Pass(NicaTrack *tr);
  virtual ~NicaTrackChi2Cut();
  ClassDef(NicaTrackChi2Cut, 1)
};

#endif /* NICATRACKCHI2CUT_H_ */
