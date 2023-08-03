/*
 * NicaCorrFit3DSquare.h
 *
 *  Created on: 22 mar 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACORRFIT3DSQUARE_H_
#define NICACORRFIT3DSQUARE_H_
#include "NicaCorrFit3DCF.h"

/**
 * class for estimate background, represent polynomial up to second degree
 * N(1+Ax*q_{out}+Bx*q_{oust}^2+...)
 */
class NicaCorrFit3DCF_Square : public NicaCorrFit3DCF {
  static const Int_t fgAx;
  static const Int_t fgBx;
  static const Int_t fgAy;
  static const Int_t fgBy;
  static const Int_t fgAz;
  static const Int_t fgBz;

 protected:
  Double_t CalculateCF(const Double_t *x, const Double_t *params) const;

 public:
  /**
   *
   * @param pol polynomial degree
   */
  NicaCorrFit3DCF_Square(Int_t pol = 2);
  /**
   *
   * @return id of Ax parameter
   */
  static inline Int_t Ax() { return fgAx; };
  /**
   *
   * @return id of Ax parameter
   */
  static inline Int_t Bx() { return fgBx; };
  /**
   *
   * @return id of Ay parameter
   */
  static inline Int_t Ay() { return fgAy; };
  /**
   *
   * @return id of By parameter
   */
  static inline Int_t By() { return fgBy; };
  /**
   *
   * @return id of Az parameter
   */
  static inline Int_t Az() { return fgAz; };
  /**
   *
   * @return id of Bz parameter
   */
  static inline Int_t Bz() { return fgBz; };
  virtual ~NicaCorrFit3DCF_Square();
  ClassDef(NicaCorrFit3DCF_Square, 1)
};

#endif /* NICACORRFIT3DSQUARE_H_ */
