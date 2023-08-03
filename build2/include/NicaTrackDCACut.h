/*
 * NicaTrackDCACut.h
 *
 *  Created on: 27 lut 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATRACKDCACUT_H_
#define NICATRACKDCACUT_H_

#include "NicaTrackCut.h"

class NicaTrackDCACut : public NicaTrackCut {
 public:
  NicaTrackDCACut();
  virtual Bool_t Pass(NicaTrack *track);
  virtual Bool_t Init(Int_t id_format = 0);
  static Int_t DCA() { return 0; };
  static Int_t DCAxy() { return 1; };
  static Int_t DCAz() { return 2; };
  virtual ~NicaTrackDCACut();
  ClassDef(NicaTrackDCACut, 1)
};

#endif /* NICATRACKDCACUT_H_ */
