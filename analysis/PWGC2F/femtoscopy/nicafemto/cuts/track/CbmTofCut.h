/** @file CbmTofCut.h
  * @copyright Copyright (C) 2020 Faculty of Physics, Warsaw University of Technology, Warsaw
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Daniel Wielanek [committer] **/

/*
 * CbmTofCut.h
 *
 *  Created on: 21 wrz 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef CBMTOFCUT_H_
#define CBMTOFCUT_H_

#include "NicaTrackToFMass2Cut.h"

class CbmTofCut : public NicaTrackToFMass2Cut {
public:
  CbmTofCut();
  void AcceptTracksWithoutTof() { SetMinMax(0, 1, 1); };
  void AcceptTracksOnlyWithToF() { SetMinMax(1, 1, 1); };
  static Int_t Flag() { return 1; };
  static Int_t M2() { return 0; };
  virtual Bool_t Init(Int_t task_id);
  virtual Bool_t Pass(NicaTrack* track);
  virtual ~CbmTofCut();
  ClassDef(CbmTofCut, 1)
};

#endif /* CBMROOT_ANALYSIS_PWGC2F_FEMTOSCOPY_NICAFEMTO_CUTS_ANATREECUTS_TRACK_CBMTOFCUT_H_ */
