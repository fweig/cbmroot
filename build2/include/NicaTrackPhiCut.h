/*
 * NicaTrackPhiCut.h
 *
 *  Created on: 16-08-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATRACKPHICUT_H_
#define NICATRACKPHICUT_H_
#include "NicaTrackCut.h"
/**
 * check track azimuthal angle
 */
class NicaTrackPhiCut : public NicaTrackCut {
  Bool_t fRound;

 public:
  NicaTrackPhiCut();
  virtual Bool_t Init(Int_t task_id);
  virtual Bool_t Pass(NicaTrack *track);
  virtual ~NicaTrackPhiCut();
  ClassDef(NicaTrackPhiCut, 1)
};

#endif /* NICATRACKPHICUT_H_ */
