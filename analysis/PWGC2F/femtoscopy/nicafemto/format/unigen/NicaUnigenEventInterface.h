/* Copyright (C) 2017-2021 Faculty of Physics, Warsaw University of Technology, Warsaw
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Daniel Wielanek [committer] */

/*
 * NicaUnigenSource.h
 *
 *  Created on: 2 sie 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAUNIGENEVENTINTERFACE_H_
#define NICAUNIGENEVENTINTERFACE_H_

#include <TObject.h>

#include "NicaEventInterface.h"
#include "NicaUnigenEvent.h"

/**
 * event interface for UnigenData
 */

class NicaUnigenEventInterface : public NicaEventInterface {
  friend class NicaUnigenEvent;
  UEvent* fEvent;

protected:
  virtual void ConnectToTree();
#ifdef UNIGEN_OLD
  void CopyUnigen(UEvent* from, UEvent* to);
#endif
public:
  NicaUnigenEventInterface();
  virtual Int_t GetTotalTrackNo() const { return fEvent->GetNpa(); }
  virtual void Boost(Double_t vx, Double_t vy, Double_t vz);
  virtual void Register(Bool_t write);
  virtual void Compress(Int_t* map, Int_t map_size);
  virtual void CopyData(NicaEventInterface* s);
  virtual void CopyAndCompress(NicaEventInterface* s, Int_t* map, Int_t map_size);
  virtual void FillTrackInterface(NicaTrackInterface* track, Int_t index);
  TObject* GetRawEventPointer() const { return fEvent; };
  virtual TObject* GetRawTrackPointer(Int_t index) const { return fEvent->GetParticle(index); };
  virtual NicaTrackInterface* GetTrackInterface() const;
  virtual void Clear(Option_t* opt = "") { fEvent->Clear(opt); };
  /** GETTER SETTER SECTION*/
  virtual void SetB(Double_t b) { fEvent->SetB(b); };
  virtual void SetVertex(Double_t /*x*/, Double_t /*y*/, Double_t /*z*/) {};
  virtual void SetPhi(Double_t phi, Double_t /*phi_error*/) { fEvent->SetPhi(phi); };
  virtual Double_t GetB() const { return fEvent->GetB(); };
  virtual Double_t GetPhi() const { return fEvent->GetPhi(); };
  virtual ~NicaUnigenEventInterface();
  ClassDef(NicaUnigenEventInterface, 1)
};

#endif /* NICAUNIGENEVENTINTERFACE_H_ */
