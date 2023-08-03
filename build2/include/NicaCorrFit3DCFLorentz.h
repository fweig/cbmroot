/*
 * NicaCorrFit3DCFLorentz.h
 *
 *  Created on: 24 mar 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACORRFIT3DCFLORENTZ_H_
#define NICACORRFIT3DCFLORENTZ_H_

#include "NicaCorrFit3DCF.h"

class NicaCorrFit3DCF_Lorentz : public NicaCorrFit3DCF {
 protected:
  Double_t CalculateCF(const Double_t *x, const Double_t *params) const;

 public:
  NicaCorrFit3DCF_Lorentz();
  virtual ~NicaCorrFit3DCF_Lorentz();
  ClassDef(NicaCorrFit3DCF_Lorentz, 1)
};

#endif /* NICACORRFIT3DCFLORENTZ_H_ */
