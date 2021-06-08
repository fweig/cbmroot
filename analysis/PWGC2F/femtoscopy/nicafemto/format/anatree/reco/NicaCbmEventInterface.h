/* Copyright (C) 2018-2021 Faculty of Physics, Warsaw University of Technology, Warsaw
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Daniel Wielanek [committer] */

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

#include "CbmAnaTreeContainer.h"

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TVector3.h>

#include "NicaCbmTrackInterface.h"
#include "NicaExpEventInterface.h"

class NicaCbmEventInterface : public NicaExpEventInterface {
protected:
  CbmAnaTreeRecoSourceContainer* fDataContainer;
  virtual void ConnectToTree();
  void Register(Bool_t write);

public:
  NicaCbmEventInterface();
  virtual void CopyData(NicaEventInterface* s);
  virtual void Compress(Int_t* map, Int_t map_size);
  virtual void CopyAndCompress(NicaEventInterface* s, Int_t* map, Int_t map_size);
  virtual void FillTrackInterface(NicaTrackInterface* track, Int_t index);
  virtual Int_t GetTotalTrackNo() const;
  virtual TObject* GetRawEventPointer() const { return fDataContainer; };
  virtual NicaTrackInterface* GetTrackInterface() const { return new NicaCbmTrackInterface(); }
  virtual TObject* GetRawTrackPointer(Int_t index) const;
  /** GETTERS AND SETTERS **/
  virtual void SetRunInfoId(Int_t /*i*/) {};
  virtual Int_t GetRunInfoId() const { return 0; };
  ;
  virtual void SetMagneticField(TVector3 /*mag*/) const {};
  virtual TVector3 GetMagneticField() const { return TVector3(0, 0, 0); };
  virtual TLorentzVector GetVertexError() const;
  virtual TLorentzVector GetVertex() const;
  virtual ~NicaCbmEventInterface();
  ClassDef(NicaCbmEventInterface, 1)
};

#endif /* NICACBMATRECOEVENTINTERFACE_H_ */
