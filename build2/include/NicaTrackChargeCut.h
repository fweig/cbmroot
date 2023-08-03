/*
 * NicaTrackChargeCut.h
 *
 *  Created on: 21 maj 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATRACKCHARGECUT_H_
#define NICATRACKCHARGECUT_H_
#include "NicaTrackCut.h"
class NicaTrackChargeCut : public NicaTrackCut {
 public:
  NicaTrackChargeCut();
  Bool_t Pass(NicaTrack *tr);
  virtual ~NicaTrackChargeCut();
  ClassDef(NicaTrackChargeCut, 1)
};

#endif /* NICATRACKCHARGECUT_H_ */
