/** @file KFParticleMatch.cxx
  * @copyright Copyright (C) 2013 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors M.Zyzak, Maxim Zyzak [committer], Florian Uhlig **/

/*
 *====================================================================
 *
 *  KF Particles Finder performance 
 *
 *====================================================================
 */

#include "KFParticleMatch.h"


KFParticleMatch::KFParticleMatch() : TObject(), fMatch(-1), fMatchType(-1) {}

KFParticleMatch::~KFParticleMatch() {}

ClassImp(KFParticleMatch)
