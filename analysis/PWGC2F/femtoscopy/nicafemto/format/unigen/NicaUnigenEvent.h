/*
 * NicaUnigen.h
 *
 *  Created on: 23-06-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICAUNIGENEVENT_H_
#define NICAUNIGENEVENT_H_

#include "UEvent.h"
#include "UParticle.h"
#include "NicaUnigenTrack.h"
#include "NicaMCEvent.h"
/**
 * class used for representation of unigen event in "fake" format
 */
class NicaUnigenEvent: public NicaMCEvent{
public:
	/**
	 * default constructor
	 */
	NicaUnigenEvent();
	NicaUnigenEvent(const NicaUnigenEvent &other);
	void CreateSource();
	virtual void Update();
	virtual void Clear(Option_t *opt=" ");
	virtual Bool_t ExistInTree()const;
	virtual TString GetFormatName() const;
	virtual ~NicaUnigenEvent();
	ClassDef(NicaUnigenEvent,1)
};

#endif /* NICAUNIGENEVENT_H_ */
