/*
 * NicaTrackSmeared.h
 *
 *  Created on: 20 lis 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICASMEAREDTRACK_H_
#define NICASMEAREDTRACK_H_


#include "NicaMCTrack.h"

class NicaSmearedEvent;
/**
 * class for storing smeared tracks
 */
class NicaSmearedTrack : public NicaMCTrack {
  friend class NicaSmearedEvent;
  NicaTrack *fNicaTrack;
  TLorentzVector *fTrueMomenta;
  NicaSmearedTrack(NicaTrack *track);

 public:
  NicaSmearedTrack();
  void CopyData(NicaTrack *other);
  inline TLorentzVector *GetTrueMomentum() const { return fTrueMomenta; };
  virtual ~NicaSmearedTrack();
  ClassDef(NicaSmearedTrack, 1)
};

#endif /* NICASMEAREDTRACK_H_ */
