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

#include "NicaCbmATMCEvent.h"

CbmHbtFullEvent::CbmHbtFullEvent()
  : NicaComplexEvent(new CbmHbtEvent(), new NicaCbmATMCEvent()) {}

CbmHbtFullEvent::~CbmHbtFullEvent() {}

/*
Bool_t CbmHbtFullEvent::IsCompatible(const NicaEvent *non_buffered) const {
    if(non_buffered->InheritsFrom("NicaCbmATFullEvent")) return kTRUE;
    return kFALSE;
}
*/
