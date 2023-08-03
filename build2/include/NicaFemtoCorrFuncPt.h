/*
 * NicaCorrFuncCollectionPt.h
 *
 *  Created on: 4 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAFEMTOCORRFUNCCOLLECTIONPT_H_
#define NICAFEMTOCORRFUNCCOLLECTIONPT_H_
#include "NicaFemtoCorrFunc.h"
/**
 * collection of cf's grouped in pt-sum bins
 */
class NicaFemtoCorrFuncPt : public NicaFemtoCorrFunc1D {
 protected:
  NicaFemtoCorrFuncPt(const NicaDividedHisto1D *h = NULL,
                      const std::initializer_list<double> &init = {0, 10});

 public:
  NicaFemtoCorrFuncPt() : NicaFemtoCorrFunc1D(){};
  NicaFemtoCorrFuncPt(const NicaDividedHisto1D &h,
                      const std::initializer_list<double> &init = {0, 10});
  virtual Bool_t Check();
  virtual void FillNum(NicaFemtoPair *pair);
  virtual void FillDenPerfect(NicaFemtoPair *pair) { FillDenMixed(pair); };
  virtual void FillDenRotated(NicaFemtoPair *pair) { FillDenMixed(pair); };
  virtual void FillDenMixed(NicaFemtoPair *pair);
  virtual void FillDenHemisphere(NicaFemtoPair *pair) { FillDenMixed(pair); };
  virtual void FillDenCharged(NicaFemtoPair *pair) { FillDenMixed(pair); };
  virtual ~NicaFemtoCorrFuncPt();
  ClassDef(NicaFemtoCorrFuncPt, 1)
};

#endif /* NICAFEMTOCORRFUNCCOLLECTIONPT_H_ */
