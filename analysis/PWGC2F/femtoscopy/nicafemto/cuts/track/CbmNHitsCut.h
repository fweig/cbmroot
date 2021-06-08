/* Copyright (C) 2020 Faculty of Physics, Warsaw University of Technology, Warsaw
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Daniel Wielanek [committer] */

/*
 * CbmNHitsCut.h
 *
 *  Created on: 22 wrz 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef CBMNHITSCUT_H_
#define CBMNHITSCUT_H_

#include "NicaTrackExpCut.h"

class CbmNHitsCut : public NicaTrackExpCut {
public:
  CbmNHitsCut();
  virtual Bool_t Init(Int_t task_id);
  virtual Bool_t Pass(NicaTrack* track);
  virtual ~CbmNHitsCut();
  ClassDef(CbmNHitsCut, 1)
};

#endif /* CBMROOT_ANALYSIS_PWGC2F_FEMTOSCOPY_NICAFEMTO_CUTS_ANATREECUTS_TRACK_CBMNHITSCUT_H_ */
