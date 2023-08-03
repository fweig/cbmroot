/*
 * NicaTrackOnlyPrimariesCut.h
 *
 *  Created on: 07-08-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATRACKONLYPRIMARIESCUT_H_
#define NICATRACKONLYPRIMARIESCUT_H_
#include "NicaTrackCut.h"
/**
 * check if track is primary
 */
class NicaTrackOnlyPrimariesCut : public NicaTrackCut {
public:
  NicaTrackOnlyPrimariesCut();
  void AcceptOnlyNonPrimaries() { SetMinAndMax(0); };
  virtual Bool_t Pass(NicaTrack* track);
  virtual ~NicaTrackOnlyPrimariesCut();
  ClassDef(NicaTrackOnlyPrimariesCut, 1)
};

#endif /* NICATRACKONLYPRIMARIESCUT_H_ */
