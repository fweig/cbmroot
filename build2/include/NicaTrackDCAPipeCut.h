/*
 * NicaTrackDCAPipeCut.h
 *
 *  Created on: 4 cze 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATRACKDCAPIPECUT_H_
#define NICATRACKDCAPIPECUT_H_
#include "NicaTrackExpCut.h"
class NicaTrackDCAPipeCut : public NicaTrackExpCut {
 public:
  NicaTrackDCAPipeCut();
  virtual Bool_t Pass(NicaTrack *track);
  virtual Bool_t Init(Int_t id_format = 0);
  static Int_t DCA() { return 0; };
  static Int_t DCAxy() { return 1; };
  static Int_t DCAz() { return 2; };
  virtual ~NicaTrackDCAPipeCut();
  ClassDef(NicaTrackDCAPipeCut, 1)
};

#endif /* NICATRACKDCAPIPECUT_H_ */
