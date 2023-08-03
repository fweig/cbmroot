/*
 * NicaExpEventInterface.h
 *
 *  Created on: 3 sie 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAEXPEVENTINTERFACE_H_
#define NICAEXPEVENTINTERFACE_H_
#include <TLorentzVector.h>
#include <TVector3.h>

#include "NicaEventInterface.h"

class NicaExpEventInterface : public NicaEventInterface {
 public:
  NicaExpEventInterface();
  /** GETTERS AND SETTERS **/
  virtual void SetRunInfoId(Int_t /*i*/){};
  virtual Int_t GetRunInfoId() const { return 0; };
  virtual void SetMagneticField(TVector3 /*mag*/) const {};
  virtual TVector3 GetMagneticField() const { return TVector3(); };
  virtual TLorentzVector GetVertexVector() const { return TLorentzVector(); };
  virtual ~NicaExpEventInterface();
  ClassDef(NicaExpEventInterface, 1)
};

#endif /* NICAEXPEVENTINTERFACE_H_ */
