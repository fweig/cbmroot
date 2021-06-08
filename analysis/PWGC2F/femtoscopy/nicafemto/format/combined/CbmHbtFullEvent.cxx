/* Copyright (C) 2020-2021 Faculty of Physics, Warsaw University of Technology, Warsaw
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Daniel Wielanek [committer] */

/*
 * NicaCbmHbtEvent.cxx
 *
 *  Created on: 21 paź 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CbmHbtFullEvent.h"

#include "CbmHbtEvent.h"
#include "CbmHbtFullEvent.h"

#include "NicaCbmMCEvent.h"

CbmHbtFullEvent::CbmHbtFullEvent() : NicaComplexEvent(new CbmHbtEvent(), new NicaCbmMCEvent()) {}

CbmHbtFullEvent::~CbmHbtFullEvent() {}

/*
Bool_t CbmHbtFullEvent::IsCompatible(const NicaEvent *non_buffered) const {
    if(non_buffered->InheritsFrom("NicaCbmATFullEvent")) return kTRUE;
    return kFALSE;
}
*/
