/*
 * NicaFemto3DCFQinv.h
 *
 *  Created on: 19 lut 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAFEMTO3DCFQINV_H_
#define NICAFEMTO3DCFQINV_H_

#include "NicaFemto3DCF.h"
/**
 * 3-d function with addritional map for qinv values
 */
class NicaFemto3DCFQinv : public NicaFemto3DCF {
  TH3D *fPureNum;
  TH3D *fQinvNum;

 public:
  NicaFemto3DCFQinv();
  NicaFemto3DCFQinv(TString name, ENicaFemtoKinematics frame = ENicaFemtoKinematics::kLCMS);
  NicaFemto3DCFQinv(TString name, Int_t binsX, Double_t minX, Double_t maxX,
                    Int_t binsY, Double_t minY, Double_t maxY, Int_t binsZ,
                    Double_t minZ, Double_t maxZ,
                    ENicaFemtoKinematics frame = ENicaFemtoKinematics::kLCMS);
  NicaFemto3DCFQinv(const NicaFemto3DCFQinv &other);
  NicaFemto3DCFQinv &operator=(const NicaFemto3DCFQinv &other);
  virtual void FillNumObj(TObject *obj);
  /**
   * @return map filled with weight*q_inv if divided by numerator - equal to
   * average qinv
   */
  TH3D *GetQinvHist(TString opt = "av") const;
  virtual void AddNum(TH1 *num, Option_t *opt = "");
  virtual void AddDen(TH1 *den, Option_t *opt = "");
  virtual Long64_t Merge(TCollection *collection);
  virtual void Add(const NicaPack *h);
  virtual void Browse(TBrowser *b);
  virtual ~NicaFemto3DCFQinv();
  ClassDef(NicaFemto3DCFQinv, 2)
};

#endif /* NICAFEMTO3DCFQINV_H_ */
