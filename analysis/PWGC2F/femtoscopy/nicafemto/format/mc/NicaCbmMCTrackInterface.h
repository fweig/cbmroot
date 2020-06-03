/*
 * NicaCbmMCTrackInterface.h
 *
 *  Created on: 20 wrz 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef BMN_NICA_BMN_FORMAT_MC_NICACBMMCTRACKINTERFACE_H_
#define BMN_NICA_BMN_FORMAT_MC_NICACBMMCTRACKINTERFACE_H_
#include "NicaMCTrackInterface.h"

class NicaCbmMCTrackInterface : public NicaMCTrackInterface{
public:
	NicaCbmMCTrackInterface();
	virtual ~NicaCbmMCTrackInterface();
	ClassDef(NicaCbmMCTrackInterface,1)
};

#endif /* BMN_NICA_BMN_FORMAT_MC_NICACBMMCTRACKINTERFACE_H_ */
