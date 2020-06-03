/*
 * CbmStsFarDetectorHit.h
 *
 *  Created on: 31 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef CBMSTSFARDETECTORHIT_H_
#define CBMSTSFARDETECTORHIT_H_

#include "NicaTrackCut.h"

class CbmStsFarDetectorHit : public NicaTrackCut{
public:
	CbmStsFarDetectorHit();
	Bool_t Pass(NicaTrack *track);
	virtual ~CbmStsFarDetectorHit();
	ClassDef(CbmStsFarDetectorHit,1)
};

#endif /* CBMSTSFARDETECTORHIT_H_ */
