/*
 * NicaCbmDstMcEventInterface.h
 *
 *  Created on: 17 wrz 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACBMATMCEVENTINTERFACE_H_
#define NICACBMATMCEVENTINTERFACE_H_

#include <TVector3.h>

#include "CbmAnaTreeContainer.h"
#include "NicaCbmGlobalTrackInterface.h"
#include "NicaMCEventInterface.h"

class CbmAnaTreeMcSourceContainer;
class NicaCbmATMCEvent;
class NicaCbmATMCEventInterface : public NicaMCEventInterface {
  friend class NicaCbmATMCEvent;

protected:
  CbmAnaTreeMcSourceContainer* fDataContainer;
  virtual void ConnectToTree();
  void Register(Bool_t write);

public:
  NicaCbmATMCEventInterface();
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
  virtual ~NicaCbmATMCEventInterface();
  ClassDef(NicaCbmATMCEventInterface, 1)
};

#endif /* NICACBMATMCEVENTINTERFACE_H_ */
