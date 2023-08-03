/*
 * NicaFemtoImaging1D.h
 *
 *  Created on: 20 lut 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAFEMTOIMAGING1D_H_
#define NICAFEMTOIMAGING1D_H_
#include <TRandom.h>

#include "NicaFemto1DCF.h"
#include "NicaFemtoImaging.h"
/**
 * class for imagin 1D CF's with QS only
 */
class NicaFemtoImaging1D : public NicaFemtoImaging {
 protected:
  Double_t fQMax;
  Double_t fQMin;
  Double_t fRMax;
  Double_t fRMin;
  Int_t fBins;
  ENicaFemtoKinematics fKinematics;
  NicaFemto1DCF *fCF;
  TH1D *fR;
  TH1D *fSource;
  /**
   * check if required resolution can be achieved with current data, and create
   * histogram with source function
   */
  virtual void Init();
  /**
   * calculate inversed transform
   */
  virtual void InverseTransofrm();
  virtual void MagicInverseTransform(Int_t l);
  double BesselJ0(double x) const;
  double BesselJ1(double x) const;
  double BesselJn(int n, double x) const;
  // void Integral(Double_t x, Double_t a, Double_t n, Double_t min, Double_t
  // max );
 public:
  NicaFemtoImaging1D();
  /**
   * inverse correlation function and return source image
   * @param cf
   */
  TH1D *Inverse(NicaFemto1DCF *cf);
  /**
   * inverse one dimensional histogram
   * @param cf histogram to inverse
   * @param normalize if true then histogram is normalized
   * @return image of CF
   */
  TH1D *Inverse(TH1D *cf, Bool_t normalize = kTRUE);
  TH1D *MagicInverse(TH1D *cf, Double_t l, Bool_t normalize = kTRUE);
  void SetRAxis(Int_t bins, Double_t min, Double_t max);
  void SetQMax(Double_t qmax) { fQMax = qmax; };
  void SetQMin(Double_t qmin) { fQMin = qmin; };
  virtual ~NicaFemtoImaging1D();
  ClassDef(NicaFemtoImaging1D, 1)
};

#endif /* NICAROOT_ANALYSIS_FEMTO_IMAGING_NICAFEMTOIMAGING1D_H_ */
