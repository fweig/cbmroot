/*
 * NicaCbmMCEvent.h
 *
 *  Created on: 20 wrz 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef BMN_NICA_BMN_FORMAT_MC_NICACBMMCEVENT_H_
#define BMN_NICA_BMN_FORMAT_MC_NICACBMMCEVENT_H_

#include "NicaMCEvent.h"
class NicaCbmMCEvent : public NicaMCEvent{
protected:
	virtual void CreateSource();
public:
	NicaCbmMCEvent();
	NicaCbmMCEvent(const NicaCbmMCEvent &other);
	virtual void Update();
	virtual Bool_t ExistInTree()const;
	virtual TString GetFormatName()const{return "CbmMCEvent";};
	virtual ~NicaCbmMCEvent();
	ClassDef(NicaCbmMCEvent,1)
};

#endif /* BMN_NICA_BMN_FORMAT_MC_NICACBMMCEVENT_H_ */
