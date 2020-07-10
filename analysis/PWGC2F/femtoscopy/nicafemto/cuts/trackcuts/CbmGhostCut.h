/*
 * CbmGhostCut.h
 *
 *  Created on: 6 sie 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICA_CBM_CUTS_TRACKCUTS_CBMGHOSTCUT_H_
#define NICA_CBM_CUTS_TRACKCUTS_CBMGHOSTCUT_H_
#include "NicaTrackCut.h"
class CbmGhostCut : public NicaTrackCut {
public:
  CbmGhostCut();
  virtual Bool_t Init(Int_t task_id = 0);
  virtual Bool_t Pass(NicaTrack* track);
  virtual ~CbmGhostCut();
  ClassDef(CbmGhostCut, 1)
};

#endif /* NICA_CBM_CUTS_TRACKCUTS_CBMGHOSTCUT_H_ */
