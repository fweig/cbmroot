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

#include "UParticle.h"
#include "NicaMCTrack.h"
/**
 * class for representation of track from unigen in "fake" format
 */
class NicaUnigenTrack: public NicaMCTrack {
public:
	NicaUnigenTrack();
	virtual ~NicaUnigenTrack();
	ClassDef(NicaUnigenTrack,1)
};

#endif /* NICAUNIGENTRACK_H_ */
