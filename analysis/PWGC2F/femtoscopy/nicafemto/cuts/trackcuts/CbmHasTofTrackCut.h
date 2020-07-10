/*
 * CbmTofTrackCut.h
 *
 *  Created on: 17 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef CBMTOFTRACKCUT_H_
#define CBMTOFTRACKCUT_H_

#include "NicaTrackCut.h"

class CbmHasTofTrackCut : public NicaTrackCut {
public:
  CbmHasTofTrackCut();
  virtual Bool_t Init(Int_t format_id = 0);
  virtual Bool_t Pass(NicaTrack* track);
  virtual ~CbmHasTofTrackCut();
  ClassDef(CbmHasTofTrackCut, 1)
};

#endif /* CBMTOFTRACKCUT_H_ */
