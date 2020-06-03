/*
 * NicaCbmMatchedEventInterface.h
 *
 *  Created on: 11 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACBMEVENTINTERFACE_H_
#define NICACBMEVENTINTERFACE_H_

#include "NicaCbmGlobalEvent.h"
#include "NicaCbmGlobalEventInterface.h"
#include "CbmDefs.h"
#include "NicaTrackClones.h"
#include "NicaCbmTrackInterface.h"
#include "CbmDetectorID.h"

class NicaCbmEvent;
class NicaCbmEventInterface : public NicaCbmGlobalEventInterface{
public :
	friend class NicaCbmEvent;
protected:
	const NicaCbmSetup fMode;
	NicaTrackClones *fStsTracks;
	NicaTrackClones *fTrdTracks;
	NicaTrackClones *fMuchTracks;
	NicaTrackClones *fTofHits;
	NicaTrackClones *fRichRings;
	virtual void ConnectToTree();
	void Register(Bool_t write);
public:
	NicaCbmEventInterface(NicaCbmSetup mode=kSis100Electron);
	NicaTrackClones *GetStsTracks()const{return fStsTracks;}
	NicaTrackClones *GetTrdTracks()const{return fTrdTracks;}
	NicaTrackClones *GetMuchTracks()const{return fMuchTracks;}
	NicaTrackClones *GetTofHits()const{return fTofHits;}
	NicaTrackClones *GetRichRings()const{return fRichRings;}
	NicaCbmSetup GetMode()const{return fMode;};
	virtual void CopyData(NicaEventInterface *s);
	virtual void Compress(Int_t *map, Int_t map_size);
	virtual void CopyAndCompress(NicaEventInterface *s, Int_t *map, Int_t map_size);
	virtual void FillTrackInterface(NicaTrackInterface *track, Int_t index);
	virtual NicaTrackInterface *GetTrackInterface()const{return new NicaCbmTrackInterface();}
	virtual TObject* GetRawTrackPointer(Int_t index)const;
	/** GETTERS AND SETTERS **/
	virtual ~NicaCbmEventInterface();
	ClassDef(NicaCbmEventInterface,1)
};


#endif /* NICACBMEVENTINTERFACE_H_ */
