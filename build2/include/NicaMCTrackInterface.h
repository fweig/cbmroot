/*
 * NicaMCTrackInterface.h
 *
 *  Created on: 2 sie 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAMCTRACKINTERFACE_H_
#define NICAMCTRACKINTERFACE_H_

#include "NicaTrackInterface.h"

class NicaMCTrackInterface : public NicaTrackInterface {
 public:
  NicaMCTrackInterface();
  virtual Double_t GetStartX() const { return 0.; };
  virtual Double_t GetStartY() const { return 0.; };
  virtual Double_t GetStartZ() const { return 0.; };
  virtual Double_t GetStartT() const { return 0.; };
  virtual void SetStart(Double_t /*x*/, Double_t /*y*/, Double_t /*z*/,
                        Double_t /*t*/){};
  virtual Double_t GetFreezX() const { return 0.; };
  virtual Double_t GetFreezY() const { return 0.; };
  virtual Double_t GetFreezZ() const { return 0.; };
  virtual Double_t GetFreezT() const { return 0.; };
  virtual void SetFreez(Double_t /*x*/, Double_t /*y*/, Double_t /*z*/,
                        Double_t /*t*/){};
  virtual Int_t GetPdg() const { return 0; };
  virtual void SetPdg(Int_t /*pdg*/){};
  virtual ~NicaMCTrackInterface();
  ClassDef(NicaMCTrackInterface, 1)
};

#endif /* NICAROOT_DATAFORMAT_NICAMCTRACKINTERFACE_H_ */
