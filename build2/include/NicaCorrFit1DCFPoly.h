/*
 * NicaCorrFit1DCFPoly.h
 *
 *  Created on: 28 lis 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACORRFIT1DCFPOLY_H_
#define NICACORRFIT1DCFPOLY_H_

#include "NicaCorrFit1DCF.h"
/**
 * fit correlation function by polynomial Norm*(Ax+Bx2+Cx3)
 */

class NicaCorrFit1DCF_Poly : public NicaCorrFit1DCF {
 protected:
  virtual Double_t CalculateCF(const Double_t *x, const Double_t *params) const;

 public:
  /**
   *
   * @param degree - polynomial degree (max is 3)
   */
  NicaCorrFit1DCF_Poly(Int_t degree = 2);
  static Int_t A() { return 0; };
  static Int_t B() { return 1; };
  static Int_t C() { return 3; };
  virtual ~NicaCorrFit1DCF_Poly();
  ClassDef(NicaCorrFit1DCF_Poly, 1)
};

class NicaCorrFit1DCF_Poly2 : public NicaCorrFit1DCF_Poly {
 protected:
  virtual Double_t CalculateCF(const Double_t *x, const Double_t *params) const;

 public:
  NicaCorrFit1DCF_Poly2(Int_t degree);
  virtual ~NicaCorrFit1DCF_Poly2();
  ClassDef(NicaCorrFit1DCF_Poly2, 1)
};

#endif /* NICACORRFIT1DCFPOLY_H_ */
