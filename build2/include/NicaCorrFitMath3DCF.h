/*
 * NicaCorrFitMath.h
 *
 *  Created on: 21 mar 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACORRFITMATH_H_
#define NICACORRFITMATH_H_

#include "NicaCorrFit3DCF.h"

class NicaCorrFitMath3DCF : public NicaCorrFit3DCF {
  Int_t fSubFunctionsNo;
  NicaCorrFit3DCF** fSubFunction;  //[fSubFunctionsNo]
protected:
  void ParametersChanged() const;
  void SetSubfunction(NicaCorrFit3DCF* f, Int_t index) { fSubFunction[index] = f; }
  NicaCorrFit3DCF* GetSubfunction(Int_t index) const { return fSubFunction[index]; };
  /**
   * call CalculateCF(Double_t *x, Double_t *params
   * @param subfunct  -subfucnttion id
   * @param x
   * @param params
   * @return
   */
  Double_t Call(Int_t subfucnt, const Double_t* x, const Double_t* params) const;
  NicaCorrFit3DCF* GetSubFunction(Int_t f) const { return fSubFunction[f]; };

public:
  /**
   * default ctor
   * @param parameters_no
   * @param subfunct
   */
  NicaCorrFitMath3DCF(Int_t parameters_no = 1, Int_t subfunct = 0);
  Int_t GetSubunctionsNo() const { return fSubFunctionsNo; };
  virtual ~NicaCorrFitMath3DCF();
  ClassDef(NicaCorrFitMath3DCF, 1)
};

/**
 * class for fitting two CF's as one following equation is used:
 * CF = CF1+CF2. Standard id's for parameters (like Lambda()) should not be used
 * because order of parameters is modified.
 */
class NicaCorrFit3DCF_Sum : public NicaCorrFitMath3DCF {
protected:
  virtual Double_t CalculateCF(const Double_t* x, const Double_t* params) const;

public:
  /**
   * default constructor
   * @param f1 first function
   * @param f2 second function
   */
  NicaCorrFit3DCF_Sum(NicaCorrFit3DCF* f1, NicaCorrFit3DCF* f2);
  virtual ~NicaCorrFit3DCF_Sum();
  ClassDef(NicaCorrFit3DCF_Sum, 1)
};

#endif /* NICACORRFITMATH_H_ */
