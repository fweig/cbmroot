/*
 * NicaCbmMatchedEvent.h
 *
 *  Created on: 12 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACBMMATCHEDEVENT_H_
#define NICACBMMATCHEDEVENT_H_

#include "CbmDefs.h"
#include "NicaCbmEvent.h"

class NicaCbmMatchedEvent : public NicaCbmEvent{
protected:
	virtual void CreateSource();
	NicaCbmMatchedEvent(TString classname,NicaCbmSetup mode=kSis100Hadron);
public:
	NicaCbmMatchedEvent(NicaCbmSetup mode=kSis100Hadron);
	virtual Bool_t ExistInTree() const;
	virtual NicaEvent *GetNewEvent() const{return new NicaCbmMatchedEvent(fMode);};
	virtual TString GetFormatName()const{return "CbmMatchedEvent";};
	virtual ~NicaCbmMatchedEvent();
	ClassDef(NicaCbmMatchedEvent,1)
};

#endif /* NICACBMMATCHEDEVENT_H_ */
