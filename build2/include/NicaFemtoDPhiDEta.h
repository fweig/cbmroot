/*
 * NicaFemtoDPhiDEta.h
 *
 *  Created on: 30 lis 2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAFEMTODPHIDETA_H_
#define NICAFEMTODPHIDETA_H_

#include "NicaDividedHisto.h"
/**
 * basic class for storing dphi-deta plots
 */
class NicaFemtoDPhiDEta : public NicaDividedHisto2D {
 protected:
  virtual void SetAxisNames(TH1 *h);

 public:
  /**
   * constructor for Streamer
   */
  NicaFemtoDPhiDEta();
  /**
   * main constructor
   * @param name
   * @param phibins
   * @param eta_bins
   * @param min
   * @param max
   */
  NicaFemtoDPhiDEta(TString name, Int_t phibins, Int_t eta_bins, Double_t min,
                    Double_t max);
  /**
   * draw this object
   * @param opt if "num" then only numerator is drawn, if "den" only denominator
   * is drawn, if "all" current pad is divided and numertor, by default when
   * "all" is called then histograms are rebinned for performance optimization,
   * this rebinning can be ingored by adding "hd", othrerwise only divided
   * histogram is drawn denominator and CF are drawn
   */
  virtual void Draw(Option_t *opt);
  virtual void Browse(TBrowser *b);
  void FillNumObj(TObject *obj);
  void FillDenObj(TObject *obj);
  virtual TString HTMLExtract(Int_t counter = 0, TString dir = " ") const;
  virtual ~NicaFemtoDPhiDEta();
  ClassDef(NicaFemtoDPhiDEta, 2)
};

#endif /* NICAFEMTODPHIDETA_H_ */
