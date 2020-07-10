/*
 * NicaBmnGlobalMCEvent.h
 *
 *  Created on: 20 wrz 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICABMNGLOBALMCEVENT_H_
#define NICABMNGLOBALMCEVENT_H_

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TString.h>

#include "CbmDefs.h"
#include "CbmDetectorID.h"
#include "NicaCbmEvent.h"
#include "NicaCbmMatchedTrackInterface.h"
#include "NicaComplexEvent.h"

class NicaCbmGlobalMCEvent : public NicaComplexEvent {
  const ECbmModuleId fMatching;
  const NicaCbmSetup fSetup;
  NicaCbmMatchedTrackInterface* fRealTrackInterface;

public:
  NicaCbmGlobalMCEvent(NicaCbmSetup mode  = kSis100Hadron,
                       ECbmModuleId match = ECbmModuleId::kSts);
  virtual void Update();
  virtual Bool_t ExistInTree() const;
  virtual TString GetFormatName() const { return "NicaGlobalMCEvent"; };
  virtual NicaEvent* GetNewEvent() const;
  virtual ~NicaCbmGlobalMCEvent();
  ClassDef(NicaCbmGlobalMCEvent, 1)
};

#endif /* NICABMNGLOBALMCEVENT_H_ */
