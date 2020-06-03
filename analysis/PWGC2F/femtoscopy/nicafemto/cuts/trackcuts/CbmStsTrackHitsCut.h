/*
 * CbmStsTrackHitsCut.h
 *
 *  Created on: 17 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef CBMSTSTRACKHITSCUT_H_
#define CBMSTSTRACKHITSCUT_H_

#include "NicaTrackCut.h"
#include "NicaCbmTrack.h"
#include "CbmStsTrack.h"

class CbmStsTrackHitsCut : public NicaTrackCut{
public:
	CbmStsTrackHitsCut();
	static Int_t Sts(){return 0;};
	static Int_t Mvd(){return 1;};
	static Int_t StsMvd(){return 2;}
	void SetNStsHits(Int_t min, Int_t max = 100){SetMinMax(min,max,Sts());};
	void SetNMvdHits(Int_t min, Int_t max = 100){SetMinMax(min,max,Mvd());};
	void SetNStsMvdHits(Int_t min, Int_t max = 100){SetMinMax(min,max,StsMvd());};
	virtual Bool_t  Init(Int_t format_id=0);
	virtual Bool_t Pass(NicaTrack *track);
	virtual ~CbmStsTrackHitsCut();
	ClassDef(CbmStsTrackHitsCut,1)

};

#endif /* CBMSTSTRACKHITSCUT_H_ */
