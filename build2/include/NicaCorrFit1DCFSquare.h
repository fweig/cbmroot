/*
 * NicaCorrFit1DCFSquare.h
 *
 *  Created on: 22 mar 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACORRFIT1DCFSQUARE_H_
#define NICACORRFIT1DCFSQUARE_H_

#include "NicaCorrFit1DCF.h"

/**
 * class for fitting background as polynomial acording to equation
 * Norm(1+A*q+B*q2)
 */
class NicaCorrFit1DCF_Square : public NicaCorrFit1DCF {
  static const Int_t fgA;
  static const Int_t fgB;

 protected:
  Double_t CalculateCF(const Double_t *x, const Double_t *params) const;

 public:
  /**
   *
   * @param pol polynomial degree (max = 2)
   */
  NicaCorrFit1DCF_Square(Int_t pol = 2);
  /**
   *
   * @return id for A parameter
   */
  inline static Int_t A() { return fgA; };
  /**
   *
   * @return id for B parameter
   */
  inline static Int_t B() { return fgB; };
  virtual ~NicaCorrFit1DCF_Square();
  ClassDef(NicaCorrFit1DCF_Square, 1)
};

#endif /* NICACORRFIT1DCFSQUARE_H_ */
