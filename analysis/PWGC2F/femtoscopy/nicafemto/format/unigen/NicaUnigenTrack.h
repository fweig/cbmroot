/* Copyright (C) 2014-2021 Faculty of Physics, Warsaw University of Technology, Warsaw
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Daniel Wielanek [committer] */

/*
 * NicaUnigenTrack.h
 *
 *  Created on: 23-06-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICAUNIGENTRACK_H_
#define NICAUNIGENTRACK_H_

#include <iostream>

#include "NicaMCTrack.h"
#include "UParticle.h"

/**
 * class for representation of track from unigen in "fake" format
 */
class NicaUnigenTrack : public NicaMCTrack {
public:
  NicaUnigenTrack();
  virtual ~NicaUnigenTrack();
  ClassDef(NicaUnigenTrack, 1)
};

#endif /* NICAUNIGENTRACK_H_ */
