/*
 * NicaTrackDeltaMomCut.h
 *
 *  Created on: 28 gru 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATRACKDELTAMOMCUT_H_
#define NICATRACKDELTAMOMCUT_H_

#include "NicaTrackDeltaMomentumCut.h"

class NicaTrackDeltaPtCut : public NicaTrackDeltaMomCut {
 public:
  NicaTrackDeltaPtCut();
  Bool_t Pass(NicaTrack *track);
  virtual ~NicaTrackDeltaPtCut();
  ClassDef(NicaTrackDeltaPtCut, 1)
};

class NicaTrackDeltaPCut : public NicaTrackDeltaMomCut {
 public:
  NicaTrackDeltaPCut();
  Bool_t Pass(NicaTrack *track);
  virtual ~NicaTrackDeltaPCut();
  ClassDef(NicaTrackDeltaPCut, 1)
};

class NicaTrackDeltaPzCut : public NicaTrackDeltaMomCut {
 public:
  NicaTrackDeltaPzCut();
  Bool_t Pass(NicaTrack *track);
  virtual ~NicaTrackDeltaPzCut();
  ClassDef(NicaTrackDeltaPzCut, 1)
};

class NicaTrackDeltaPhiCut : public NicaTrackDeltaMomCut {
 public:
  NicaTrackDeltaPhiCut();
  Bool_t Pass(NicaTrack *track);
  virtual ~NicaTrackDeltaPhiCut();
  ClassDef(NicaTrackDeltaPhiCut, 1)
};
class NicaTrackDeltaThetaCut : public NicaTrackDeltaMomCut {
 public:
  NicaTrackDeltaThetaCut();
  Bool_t Pass(NicaTrack *track);
  virtual ~NicaTrackDeltaThetaCut();
  ClassDef(NicaTrackDeltaThetaCut, 1)
};

class NicaTrackDeltaVectPtCut : public NicaTrackDeltaVectMomCut {
 public:
  NicaTrackDeltaVectPtCut();
  Bool_t Pass(NicaTrack *track);
  virtual ~NicaTrackDeltaVectPtCut();
  ClassDef(NicaTrackDeltaVectPtCut, 1)
};

class NicaTrackDeltaVectPCut : public NicaTrackDeltaVectMomCut {
 public:
  NicaTrackDeltaVectPCut();
  Bool_t Pass(NicaTrack *track);
  virtual ~NicaTrackDeltaVectPCut();
  ClassDef(NicaTrackDeltaVectPCut, 1)
};

class NicaTrackDeltaVectPhiCut : public NicaTrackDeltaVectMomCut {
 public:
  NicaTrackDeltaVectPhiCut();
  Bool_t Pass(NicaTrack *track);
  virtual ~NicaTrackDeltaVectPhiCut();
  ClassDef(NicaTrackDeltaVectPhiCut, 1)
};

class NicaTrackDeltaVectThetaCut : public NicaTrackDeltaVectMomCut {
 public:
  NicaTrackDeltaVectThetaCut();
  Bool_t Pass(NicaTrack *track);
  virtual ~NicaTrackDeltaVectThetaCut();
  ClassDef(NicaTrackDeltaVectThetaCut, 1)
};

#endif /* NICATRACKDELTAMOMCUT_H_ */
