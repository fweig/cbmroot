/*
 * NicaFemto1DCFAnaMap.h
 *
 *  Created on: 8 gru 2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAFEMTO1DCFANAMAP_H_
#define NICAFEMTO1DCFANAMAP_H_

#include <FairTask.h>

#include "NicaFemtoBasicAna.h"
#include "NicaFemtoConst.h"
#include "NicaFemtoCorrFunc.h"

class NicaFemtoCorrFuncKt;

/**
 * class for generating maps of CF's based on 1Dim CF's
 */

class NicaFemtoCorrFuncKtMap1D : public NicaFemtoCorrFunc1D {
  Double_t fR;

 public:
  NicaFemtoCorrFuncKtMap1D() : fR(){};
  NicaFemtoCorrFuncKtMap1D(NicaFemtoCorrFuncKt *cf, Int_t Rbins, Double_t Rmin,
                           Double_t Rmax, ENicaFemtoKinematics kin);
  virtual Bool_t Check() { return NicaFemtoCorrFunc1D::Check(); };
  inline void SetR(Double_t R) { fR = R; };
  virtual void FillNum(NicaFemtoPair *pair);
  virtual void FillDenPerfect(NicaFemtoPair *pair) { FillDenMixed(pair); };
  virtual void FillDenRotated(NicaFemtoPair *pair);
  virtual void FillDenMixed(NicaFemtoPair *pair);
  virtual void FillDenHemisphere(NicaFemtoPair *pair) { FillDenRotated(pair); };
  virtual void FillDenCharged(NicaFemtoPair *pair) { FillDenMixed(pair); };
  virtual ~NicaFemtoCorrFuncKtMap1D();
  ClassDef(NicaFemtoCorrFuncKtMap1D, 1)
};

class NicaFemto1DCFAnaMap : public NicaFemtoBasicAna {
  Int_t fRbins;
  Double_t fRMinEff, fRStep;
  Double_t fRmin;  //[fRbins]
  Double_t fRmax;  //[fRbins]
 protected:
  virtual void ProcessFemtoPair();
  virtual void ProcessFemtoPair_Perfect();
  virtual void ProcessFemtoPair_Rotated();
  virtual void ProcessFemtoPair_Hemisphere();
  virtual void ProcessFemtoPair_Mixed();
  virtual void ProcessFemtoPair_Charged();
  virtual void CheckCutContainerCollections();
  virtual Bool_t InitArray();

 public:
  NicaFemto1DCFAnaMap();
  /**
   * set radii range in generated map
   * @param r_bins number of r bins
   * @param r_min minimal radius
   * @param r_max maximal radius
   */
  void SetRadiusRange(Int_t r_bins, Double_t r_min, Double_t r_max);
  virtual InitStatus Init();
  virtual NicaPackage *Report() const;
  virtual ~NicaFemto1DCFAnaMap();
  ClassDef(NicaFemto1DCFAnaMap, 1)
};

#endif /* NICAFEMTO1DCFANAMAP_H_ */
