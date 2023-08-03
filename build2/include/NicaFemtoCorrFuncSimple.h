/*
 * NicaFemtoCorrFuncSimple.h
 *
 *  Created on: 8 lut 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAFEMTO_ANALYSIS_FEMTO_BASE_CORRFUNC_NICAFEMTOCORRFUNCSIMPLE_H_
#define NICAFEMTO_ANALYSIS_FEMTO_BASE_CORRFUNC_NICAFEMTOCORRFUNCSIMPLE_H_

#include "NicaFemtoCorrFunc.h"

class NicaFemtoCorrFuncSimple : public NicaFemtoCorrFunc1D {
public:
  NicaFemtoCorrFuncSimple();
  NicaFemtoCorrFuncSimple(const NicaDividedHisto1D& h);
  NicaFemtoCorrFuncSimple(const NicaFemtoCorrFuncSimple& other);
  virtual void FillNum(NicaFemtoPair* pair);
  virtual void FillDenPerfect(NicaFemtoPair* pair) { FillDenMixed(pair); };
  virtual void FillDenRotated(NicaFemtoPair* pair);
  virtual void FillDenMixed(NicaFemtoPair* pair);
  virtual void FillDenHemisphere(NicaFemtoPair* pair) { FillDenRotated(pair); };
  virtual void FillDenCharged(NicaFemtoPair* pair) { FillDenMixed(pair); };
  virtual ~NicaFemtoCorrFuncSimple() {};
  ClassDef(NicaFemtoCorrFuncSimple, 1)
};


#endif /* NICAFEMTO_ANALYSIS_FEMTO_BASE_CORRFUNC_NICAFEMTOCORRFUNCSIMPLE_H_ */
