/*
 * NicaCorrFit3DCFBowlerSinyukov.h
 *
 *  Created on: 22 sty 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACORRFIT3DCFBOWLERSINYUKOV_H_
#define NICACORRFIT3DCFBOWLERSINYUKOV_H_

#include "NicaCorrFit3DCF.h"
#include "NicaSplines.h"
#include "TF1.h"
class NicaFemto3DCF;
/**
 * fit by using bowler sinyukov method can be used for bosons or fermions
 */
class NicaCorrFit3DCFBowlerSinyukov : public NicaCorrFit3DCF {
protected:
  NicaSpline2D* fCFs;
  NicaSpline3D* fQinvMap;
  Bool_t fTStarZero;
  mutable Double_t fRinv;
  Double_t fAverageBeta;
  Double_t fGamma;
  Double_t fQSMode;
  const Double_t fSqrt3;
  ENicaFemtoKinematics fFrame;
  //	Double_t Integral(Double_t *x, Double_t *params)const;
  virtual void Fit(TObject* histo);
  virtual void FitDummy(TObject* histo);
  virtual Double_t GetRoutPRF(Double_t r_out, Double_t tau) const;
  inline Double_t Gaus(Double_t r, Double_t R) const { return TMath::Exp(-r * r / (4.0 * R * R)) * r * r; }
  virtual Double_t CalculateCF(const Double_t* x, const Double_t* params) const;
  virtual void RecalculateFunction() const;
  NicaCorrFit3DCFBowlerSinyukov(Int_t extra_params);

public:
  NicaCorrFit3DCFBowlerSinyukov();
  /**
   * consider fermion pairs
   */
  void AsFermions() { fQSMode = -0.5; };
  /**
   * consider boson pairs
   */
  void AsBosons() { fQSMode = 1; }
  /**
   * ignore quantum-statistic effects
   */
  void AsNonIdentical() { fQSMode = 0.0; };
  /**
   * load map that is used for calculations
   * @param h map histogram
   * @param opt  define type of loaded map if "lcms" then assume that there is
   * C(qinv,r_inv) if option is "" then assume that map is C(k*,r*), if "krmap"
   * then assume that there is W_av(k*,r*) where W_av is averaged weight of
   * pairs with given k* and r*
   */
  void LoadMap(TH2D* h, Option_t* opt = "");
  /**
   * set average pair beta, this value must be set to correctly fit CF in LCMS
   * where we need to know how to convert R_out to R_out*
   * @param beta average pair beta
   */
  void SetAveragePairBeta(Double_t beta);
  /**
   * set average pair beta @see
   * NicaCorrFit3DCFBowlerSinyukov::SetAveragePairBeta(Double_t beta)
   * @param m1 mass of first particle
   * @param m2 mass of second particle
   * @param avkt average kt
   */
  void SetAveragePairBeta(Double_t m1, Double_t m2, Double_t avkt);
  void SetTStarZero() { fTStarZero = kTRUE; };
  NicaSpline2D* GetKqMap() const { return fCFs; };
  /**
   * divide numerator by value of CF with Norm=1, to remove coulomb interaction
   * only you should call AsNonIdentical before calling this function
   * @return
   */
  NicaFemto3DCF* UnBowlerize() const;
  static Int_t Tau() { return 5; };
  virtual ~NicaCorrFit3DCFBowlerSinyukov();
  ClassDef(NicaCorrFit3DCFBowlerSinyukov, 1)
};

#endif /* NICACORRFIT3DCFBOWLERSINYUKOV_H_ */
