/*
 * NicaCorrFit3DCFMultiDim.h
 *
 *  Created on: 18 maj 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACORRFIT3DCFMULTIDIM_H_
#define NICACORRFIT3DCFMULTIDIM_H_


#include "NicaArray.h"
#include "NicaCorrFit3DCF.h"

class NicaCorrFitFunctor;

class NicaCorrFit3DCFMultiDim : public NicaCorrFit3DCF {
protected:
  Int_t fParameterIDs[3];
  NicaCorrFitFunctor* fFunctor;
  mutable NicaArray_1<Float_t>* fData;
  virtual void RecalculateFunction() const;
  virtual void PrepareNicaMinimizer();
  virtual Double_t CalculateCF(const Double_t* x, const Double_t* params) const;

public:
  NicaCorrFit3DCFMultiDim(Int_t params = 5);
  void SetAutoLimits();
  void SetMapFunctor(NicaCorrFitFunctor* functor) { fFunctor = functor; };
  virtual void SetMinimizer(EMinAlgo alg);
  virtual ~NicaCorrFit3DCFMultiDim();
  ClassDef(NicaCorrFit3DCFMultiDim, 1)
};

#endif /* NICACORRFIT3DCFMULTIDIM_H_ */
