/*
 * NicaCbmEventInterface.h
 *
 *  Created on: 3 gru 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACBMGLOBALEVENTINTERFACE_H_
#define NICACBMGLOBALEVENTINTERFACE_H_

#include "NicaExpEventInterface.h"
#include "TClonesArray.h"
#include "CbmVertex.h"
#include "NicaTrackClones.h"
#include "NicaCbmGlobalTrackInterface.h"


class NicaCbmGlobalEventInterface : public NicaExpEventInterface{
protected:
	CbmVertex *fVertex;
	NicaTrackClones *fTracks;
	virtual void ConnectToTree();
	void Register(Bool_t write);
public:
	NicaCbmGlobalEventInterface();
	virtual void CopyData(NicaEventInterface *s);
	virtual void Compress(Int_t *map, Int_t map_size);
	virtual void CopyAndCompress(NicaEventInterface *s, Int_t *map, Int_t map_size);
	virtual void FillTrackInterface(NicaTrackInterface *track, Int_t index);
	virtual Int_t GetTotalTrackNo() const;
	virtual TObject *GetRawEventPointer()const {return fVertex;};
	virtual NicaTrackInterface *GetTrackInterface()const{return new NicaCbmGlobalTrackInterface();}
	virtual TObject* GetRawTrackPointer(Int_t index)const;
	/** GETTERS AND SETTERS **/
	virtual void SetRunInfoId(Int_t /*i*/){};
	virtual Int_t GetRunInfoId()const{return 0;};;
	virtual void SetMagneticField(TVector3 /*mag*/)const{};
	virtual TVector3 GetMagneticField()const{return TVector3(0,0,0);};
	virtual TLorentzVector GetVertexError()const;
	virtual TLorentzVector GetVertex()const;
	virtual ~NicaCbmGlobalEventInterface();
	ClassDef(NicaCbmGlobalEventInterface,1)
};

#endif /* NICACBMGLOBALEVENTINTERFACE_H_ */
