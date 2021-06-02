/** @file CbmAnaTreeObjectTrack.cxx
  * @copyright Copyright (C) 2020-2021 Faculty of Physics, Warsaw University of Technology, Warsaw
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Daniel Wielanek [committer] **/

/*
 * CbmAnaTreeObjectTrack.cxx
 *
 *  Created on: 14 paź 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CbmAnaTreeObjectTrack.h"

CbmAnaTreeObjectTrack::CbmAnaTreeObjectTrack() : fTreeParticle(nullptr), fTreeTrack(nullptr), fTreeHit(nullptr) {}

CbmAnaTreeObjectTrack::~CbmAnaTreeObjectTrack() {}

CbmAnaTreeObjectTrack::CbmAnaTreeObjectTrack(const CbmAnaTreeObjectTrack& other)
  : fTreeParticle(other.fTreeParticle)
  , fTreeTrack(other.fTreeTrack)
  , fTreeHit(other.fTreeTrack)
{
}

CbmAnaTreeObjectTrack& CbmAnaTreeObjectTrack::operator=(const CbmAnaTreeObjectTrack& other)
{
  if (this == &other) return *this;
  fTreeParticle = other.fTreeParticle;
  fTreeTrack    = other.fTreeTrack;
  fTreeHit      = other.fTreeHit;
  return *this;
}
