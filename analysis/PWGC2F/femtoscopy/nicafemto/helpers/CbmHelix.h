/*
 * CbmHelix.h
 *
 *  Created on: 27 sie 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 *		version of CbmKFTrack without any additional dependencies
 */
#ifndef CBMHELIX_H_
#define CBMHELIX_H_

#include "CbmGlobalTrack.h"
#include "CbmStsTrack.h"
#include "FairField.h"
#include <TObject.h>
#include <iostream>
/**
 * smmaerl version of CbmKFTrack
 */
#define SKIP_LOSSES

class CbmHelix : public TObject {
protected:
  Double_t fT[6], fTb[15];
  Double_t Qp() const { return fT[4]; };
  void SetQp(Double_t qp) { fT[4] = qp; }
  Double_t X() const { return fT[0]; }
  Double_t Y() const { return fT[1]; }
  Double_t Z() const { return fT[5]; }
  Double_t Tx() const { return fT[2]; }
  Double_t Ty() const { return fT[3]; }
  static FairField* fgField;
  Int_t indexS(Int_t i, Int_t j) {
    return (j <= i) ? i * (i + 1) / 2 + j : j * (j + 1) / 2 + i;
  }
  void multQtSQ(const Int_t N, Double_t Q[]);
  void ExtrapolateLine(Double_t z_out);
  Int_t ExtrapolateRK4(Double_t z_out);
  Int_t ExtrapolateALight(Double_t z_out);
  Int_t Propagate(Double_t Z);
  void SetParameters(const FairTrackParam* param);

public:
  CbmHelix();
  static void SetField(FairField* field) { fgField = field; };
  CbmHelix(const CbmHelix& other);
  CbmHelix& operator=(const CbmHelix& other);
  void Build(const FairTrackParam* params) { SetParameters(params); }
  void Build(const CbmGlobalTrack* tr);
  void Build(const CbmStsTrack* tr, Bool_t first);
  void Build(const TVector3& pos, const TVector3& mom, Double_t charge);
  void PrintInfo() const {
    std::cout << Form("T={%4.2f\t%4.2f\t%4.2f\t%4.2f\t%4.2f\t%4.2f}",
                      fT[0],
                      fT[1],
                      fT[2],
                      fT[3],
                      fT[4],
                      fT[5])
              << std::endl;
  }
  Double_t* GetTrack() { return fT; }
  TVector3 Eval(Double_t z);
  TVector3 Eval(Double_t z, TVector3& mom);
  virtual ~CbmHelix();
  ClassDef(CbmHelix, 1)
};

#endif /* NICA_CBM_HELPERS_CBMHELIX_H_ */
