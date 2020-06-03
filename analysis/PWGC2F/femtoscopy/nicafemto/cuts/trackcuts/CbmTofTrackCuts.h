/*
 * CbmTofTrackCuts.h
 *
 *  Created on: 31 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICA_CBM_CUTS_TRACKCUTS_CBMTOFTRACKCUTS_H_
#define NICA_CBM_CUTS_TRACKCUTS_CBMTOFTRACKCUTS_H_

#include "NicaTrackCut.h"
#include "NicaTrackToFMass2Cut.h"

class CbmTofTrackCuts : public NicaTrackCut{
	const Double_t fOneOverC;
public:
	CbmTofTrackCuts();
	static Int_t M2(){return 0;}
	static Int_t Beta(){return 1;}
	void SetM2Cut(Double_t min, Double_t max);
	void SetBetaCut(Double_t min, Double_t max);
	Bool_t Pass(NicaTrack *track);
	virtual ~CbmTofTrackCuts();
	ClassDef(CbmTofTrackCuts,1)
};

class CbmTofM2Cut:  public NicaTrackToFMass2Cut{
	const Double_t fOneOverC;
public:
	CbmTofM2Cut();
	CbmTofM2Cut(const CbmTofM2Cut &other);
	virtual Bool_t Init(Int_t format_id=0);
	Bool_t Pass(NicaTrack *track);
	virtual ~CbmTofM2Cut();
	ClassDef(CbmTofM2Cut,1)
};

class CbmTofMassCut: public NicaTrackCut{
	const Double_t fOneOverC;
public:
	CbmTofMassCut();
	Bool_t Pass(NicaTrack *track);
	virtual ~CbmTofMassCut();
	ClassDef(CbmTofMassCut,1)
};
#endif /* NICA_CBM_CUTS_TRACKCUTS_CBMTOFTRACKCUTS_H_ */
