/*
 * NicaCorrFuncAzimuthalKt.h
 *
 *  Created on: 4 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAFEMTOCORRFUNCAZIMUTHALKT_H_
#define NICAFEMTOCORRFUNCAZIMUTHALKT_H_
#include "NicaFemtoCorrFunc.h"
/**
 * collection of cf's grouped by pair-phi and kT bins
 */
class NicaFemtoCorrFuncAzimuthalKt : public NicaFemtoCorrFunc2D {
  Double_t fPhiCorrection[2];

 protected:
  /**
   * return bin number in fArray
   * @param px total px of pair
   * @param py total py of pair
   * @param event_phi event reaction plane angle
   * @return
   */
  Int_t CalculateBin(Double_t px, Double_t py, Double_t event_phi);
  NicaFemtoCorrFuncAzimuthalKt(const NicaDividedHisto1D *h = NULL,
                               const std::initializer_list<double> &init = {0,
                                                                            10},
                               Int_t phi = 1);

 public:
  NicaFemtoCorrFuncAzimuthalKt() : NicaFemtoCorrFunc2D(){};
  NicaFemtoCorrFuncAzimuthalKt(const NicaDividedHisto1D &h,
                               const std::initializer_list<double> &init = {0,
                                                                            10},
                               Int_t phi = 1);
  virtual Bool_t Check();
  virtual TString HTMLExtract(Int_t no, TString dir = "") const;
  virtual void FillNum(NicaFemtoPair *pair);
  virtual void FillDenMixed(NicaFemtoPair *pair);
  virtual void FillDenRotated(NicaFemtoPair *pair);
  virtual void FillDenHemisphere(NicaFemtoPair *pair) { FillDenRotated(pair); };
  virtual ~NicaFemtoCorrFuncAzimuthalKt();
  ClassDef(NicaFemtoCorrFuncAzimuthalKt, 1)
};

#endif /* NICAFEMTOCORRFUNCAZIMUTHALKT_H_ */
