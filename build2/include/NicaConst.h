/*
 * NicaConst.h
 *
 *  Created on: 1 lip 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACONST_H_
#define NICACONST_H_
/**
 * const list it shuold be namespace but root don't use namespaces "out of box"
 *
 */
#include "TObject.h"
class NicaConst {
public:
  NicaConst();
  Int_t static PionPlusPID() { return 211; };
  Int_t static PionZeroPID() { return 111; }
  Int_t static ElectronPID() { return 11; };
  Int_t static MuonPID() { return 13; };
  Int_t static KaonPlusPID() { return 321; };
  Int_t static ProtonPID() { return 2212; };
  Int_t static LambdaPID() { return 3122; };
  Int_t static NeutronPID() { return 2112; };
  Double_t static ElectronMass() { return 0.000510998; };
  Double_t static MuonMass() { return 0.105658; };
  Double_t static PionPlusMass() { return 0.13957; };
  Double_t static KaonPlusMass() { return 0.49367; };
  Double_t static ProtonMass() { return 0.938272029; };
  Double_t static LambdaMass() { return 1.115638; };
  Double_t static NeutronMass() { return 0.939565; }
  Double_t static KaonZeroMass() { return 0.497611; };
  virtual ~NicaConst();
  ClassDef(NicaConst, 1)
};

#endif /* NICACONST_H_ */
