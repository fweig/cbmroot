/* Copyright (C) 2018-2021 Faculty of Physics, Warsaw University of Technology, Warsaw
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Daniel Wielanek [committer] */

/*
 * NicaCbmTrackInterface.h
 *
 *  Created on: 3 gru 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACBMATRECOTRACKINTERFACE_H_
#define NICACBMATRECOTRACKINTERFACE_H_

#include "CbmStsTrack.h"

#include "NicaTrackInterface.h"
class NicaCbmTrackInterface : public NicaTrackInterface {
public:
  NicaCbmTrackInterface();
  virtual ~NicaCbmTrackInterface();
  ClassDef(NicaCbmTrackInterface, 1)
};

#endif /* NICACBMATTRACKINTERFACE_H_ */
