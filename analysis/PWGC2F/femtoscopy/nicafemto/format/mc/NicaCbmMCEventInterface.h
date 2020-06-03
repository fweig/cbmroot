/*
 * NicaCbmMCEventInterface.h
 *
 *  Created on: 20 wrz 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef BMN_NICA_BMN_FORMAT_MC_NICACBMMCEVENTINTERFACE_H_
#define BMN_NICA_BMN_FORMAT_MC_NICACBMMCEVENTINTERFACE_H_

#include "NicaMCEventInterface.h"
#include "FairMCEventHeader.h"
#include <TClonesArray.h>

class NicaCbmMCEvent;
class NicaCbmMCEventInterface : public NicaMCEventInterface{
	friend class NicaCbmMCEvent;
protected:
	FairMCEventHeader *fEvent;
	TClonesArray *fCbmTracks;
	virtual void ConnectToTree();
public:
	NicaCbmMCEventInterface();
	virtual Int_t GetTotalTrackNo() const {return fCbmTracks->GetEntriesFast();};
	virtual void Register(Bool_t write);
	virtual void Clear(Option_t *opt="");
	virtual void Compress(Int_t *map, Int_t map_size);
	virtual void CopyData(NicaEventInterface *s);
	virtual void CopyAndCompress(NicaEventInterface *s, Int_t *map, Int_t map_size);
	virtual void FillTrackInterface(NicaTrackInterface *track, Int_t index);
	virtual TObject *GetRawEventPointer()const{return fEvent;};
	virtual TObject * GetRawTrackPointer(Int_t index)const{return fCbmTracks->UncheckedAt(index);}
	virtual NicaTrackInterface *GetTrackInterface()const;
	/** GETTERS SETTERS */
	virtual void SetVertex(Double_t x, Double_t y, Double_t z){fEvent->SetVertex(x,y,z);};
	virtual void SetPhi(Double_t /*phi*/, Double_t /*phi_error*/){};
	virtual void SetB(Double_t /*b*/){};
	virtual Double_t GetB()const{return 0;};
	virtual Double_t GetPhi()const{return 0;};
	virtual Double_t GetPhiError()const{return 0;};
	virtual TLorentzVector GetVertex()const;
	virtual ~NicaCbmMCEventInterface();
	ClassDef(NicaCbmMCEventInterface,1)
};

#endif /* BMN_NICA_BMN_FORMAT_MC_NICACBMMCEVENTINTERFACE_H_ */
