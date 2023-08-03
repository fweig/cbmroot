/*
 * NicaCorrFuncCollectionKt.h
 *
 *  Created on: 4 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAFEMTOCORRFUNCCOLLECTIONKT_H_
#define NICAFEMTOCORRFUNCCOLLECTIONKT_H_

#include "NicaFemtoCorrFunc.h"
/**
 * collection of correlation functions binned by kT value
 */
class TH1D;
class NicaFemtoCorrFuncKt : public NicaFemtoCorrFunc1D {
  TH1D *fKtNum;
  TH1D *fKtDen;

 protected:
  virtual void AddHTMLCode(NicaHtmlFile & file) const;

 public:
  NicaFemtoCorrFuncKt();
  /**
   *
   * @param h
   * @param init
   * @param ktBins number of kt bins
   * @param ktMin
   * @param ktMax
   */
  NicaFemtoCorrFuncKt(const NicaDividedHisto1D &h,
                      const std::initializer_list<double> &init = {0, 10},
                      Int_t ktBins = 100, Double_t ktMin = 0,
                      Double_t ktMax = 2);
  NicaFemtoCorrFuncKt(const NicaFemtoCorrFuncKt &other);
  TH1D *GetKtNum() const { return fKtNum; };
  TH1D *GetKtDen() const { return fKtDen; };
  virtual Bool_t Check();
  virtual void Add(const NicaPack *pack);
  virtual void FillNum(NicaFemtoPair *pair);
  virtual void FillDenPerfect(NicaFemtoPair *pair) { FillDenMixed(pair); };
  virtual void FillDenRotated(NicaFemtoPair *pair);
  virtual void FillDenMixed(NicaFemtoPair *pair);
  virtual void FillDenHemisphere(NicaFemtoPair *pair) { FillDenRotated(pair); };
  virtual void FillDenCharged(NicaFemtoPair *pair) { FillDenMixed(pair); };
  virtual TString HTMLExtract(Int_t no, TString dir = "") const;
  virtual ~NicaFemtoCorrFuncKt();
  ClassDef(NicaFemtoCorrFuncKt, 2)
};

#endif /* NICAFEMTOCORRFUNCCOLLECTIONKT_H_ */
