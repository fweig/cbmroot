/*
 * NicaTrackTpcHitsCut.h
 *
 *  Created on: 4 maj 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATRACKTPCHITCUT_H_
#define NICATRACKTPCHITCUT_H_

#include "NicaTrackExpCut.h"

class NicaTrackTpcHitCut : public NicaTrackExpCut {
 public:
  NicaTrackTpcHitCut();
  virtual Bool_t Init(Int_t format_id = 0);
  virtual Bool_t Pass(NicaTrack *tr);
  virtual ~NicaTrackTpcHitCut();
  ClassDef(NicaTrackTpcHitCut, 1)
};

#endif /* NICATRACKTPCHITCUT_H_ */
