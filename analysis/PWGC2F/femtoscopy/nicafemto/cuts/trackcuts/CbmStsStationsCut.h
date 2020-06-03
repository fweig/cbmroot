/*
 * CbmStsStationsCut.h
 *
 *  Created on: 7 sie 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef CBMSTSSTATIONSCUT_H_
#define CBMSTSSTATIONSCUT_H_

#include "NicaTrackCut.h"

class CbmStsStationsCut : public NicaTrackCut{
public:
	CbmStsStationsCut();
	Bool_t Init(Int_t format_id=0);
	Bool_t Pass(NicaTrack *h);
	virtual ~CbmStsStationsCut();
	ClassDef(CbmStsStationsCut,1)
};

#endif /* NICA_CBM_CUTS_TRACKCUTS_CBMSTSSTATIONSCUT_H_ */
