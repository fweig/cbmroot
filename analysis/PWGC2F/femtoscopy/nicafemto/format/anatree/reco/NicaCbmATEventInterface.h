/*
 * NicaCbmDstEventInterface.h
 *
 *  Created on: 3 gru 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACBMATRECOEVENTINTERFACE_H_
#define NICACBMATRECOEVENTINTERFACE_H_

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TVector3.h>

#include "CbmAnaTreeContainer.h"
#include "NicaCbmGlobalTrackInterface.h"
#include "NicaExpEventInterface.h"

class NicaCbmATEventInterface : public NicaExpEventInterface {
protected:
  CbmAnaTreeRecoSourceContainer* fDataContainer;
  virtual void ConnectToTree();
  void Register(Bool_t write);

public:
  NicaCbmATEventInterface();
  virtual void CopyData(NicaEventInterface* s);
  virtual void Compress(Int_t* map, Int_t map_size);
  virtual void
  CopyAndCompress(NicaEventInterface* s, Int_t* map, Int_t map_size);
  virtual void FillTrackInterface(NicaTrackInterface* track, Int_t index);
  virtual Int_t GetTotalTrackNo() const;
  virtual TObject* GetRawEventPointer() const { return fDataContainer; };
  virtual NicaTrackInterface* GetTrackInterface() const {
    return new NicaCbmGlobalTrackInterface();
  }
  virtual TObject* GetRawTrackPointer(Int_t index) const;
  /** GETTERS AND SETTERS **/
  virtual void SetRunInfoId(Int_t /*i*/) {};
  virtual Int_t GetRunInfoId() const { return 0; };
  ;
  virtual void SetMagneticField(TVector3 /*mag*/) const {};
  virtual TVector3 GetMagneticField() const { return TVector3(0, 0, 0); };
  virtual TLorentzVector GetVertexError() const;
  virtual TLorentzVector GetVertex() const;
  virtual ~NicaCbmATEventInterface();
  ClassDef(NicaCbmATEventInterface, 1)
};

#endif /* NICACBMATRECOEVENTINTERFACE_H_ */
