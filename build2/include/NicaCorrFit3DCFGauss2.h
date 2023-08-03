/*
 * NicaCorrFit3DCFGauss2.h
 *
 *  Created on: 21 mar 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACORRFIT3DCFGAUSS2_H_
#define NICACORRFIT3DCFGAUSS2_H_

#include "NicaCorrFit3DCF.h"
/**
 * class for fitting two gaussian source, important note - Rout,Rside,Rlong
 * correspond to radii of first source
 */
class NicaCorrFit3DCF_Gauss2 : public NicaCorrFit3DCF {
  static const Int_t fgRout2;
  static const Int_t fgRside2;
  static const Int_t fgRlong2;
  static const Int_t fgLambda2;

 protected:
  Double_t CalculateCF(const Double_t *x, const Double_t *params) const;

 public:
  NicaCorrFit3DCF_Gauss2();
  /**
   *
   * @return id of parameter R-out-1
   */
  inline static Int_t Rout1() { return Rout(); };
  /**
   *
   * @return id of parameter R-out-2
   */
  inline static Int_t Rout2() { return fgRout2; };
  /**
   *
   * @return id of parameter R-side-1
   */
  inline static Int_t Rside1() { return Rside(); };
  /**
   *
   * @return id of parameter R-side2
   */
  inline static Int_t Rside2() { return fgRside2; };
  /**
   *
   * @return id of parameter R-long1
   */
  inline static Int_t Rlong1() { return Rlong(); };
  /**
   *
   * @return id of parameter R-long2
   */
  inline static Int_t Rlong2() { return fgRlong2; };
  /**
   *
   * @return id of paramter lambda1
   */
  inline static Int_t Lambda1() { return Lambda(); };
  /**
   *
   * @return id of lambda2
   */
  inline static Int_t Lambda2() { return fgLambda2; }
  virtual ~NicaCorrFit3DCF_Gauss2();
  ClassDef(NicaCorrFit3DCF_Gauss2, 1)
};

#endif /* NICACORRFIT3DCFGAUSS2_H_ */
