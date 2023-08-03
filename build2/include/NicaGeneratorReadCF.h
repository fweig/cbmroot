/*
 * NicaGeneratorReadCF.h
 *
 *  Created on: 13 sie 2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAGENERATORREADCF_H_
#define NICAGENERATORREADCF_H_
#include "NicaGeneratorReadTree.h"
/**
 * basic class for reading CF model
 */
class NicaGeneratorReadCF : public NicaGeneratorReadTree {
  static const Int_t fNmax = 25000;
  Float_t fPx_i[fNmax];
  Float_t fPy_i[fNmax];
  Float_t fPz_i[fNmax];
  Float_t fE_i[fNmax];
  Float_t fX_i[fNmax];
  Float_t fY_i[fNmax];
  Float_t fZ_i[fNmax];
  Float_t fT_i[fNmax];
  Int_t fId_i[fNmax];
  Int_t fMid_i[fNmax];
  Short_t fEle_i[fNmax];
  Short_t fBar_i[fNmax];
  Short_t fStr_i[fNmax];
  Float_t fPx[fNmax];
  Float_t fPy[fNmax];
  Float_t fPz[fNmax];
  Float_t fE[fNmax];
  Float_t fX[fNmax];
  Float_t fY[fNmax];
  Float_t fZ[fNmax];
  Float_t fT[fNmax];
  Int_t fId[fNmax];
  Int_t fMid[fNmax];
  Short_t fEle[fNmax];
  Short_t fBar[fNmax];
  Short_t fStr[fNmax];
  Int_t fConvertOption;
  TTree *fInTree_i;
  Int_t fNp, fNp_i;
  Int_t fX_Id, fY_Id, fZ_Id, fT_Id;
  Int_t fStatus_Id, fMother_Id;

 protected:
  virtual Int_t ReadEvent();
  virtual Int_t Reopen();
  virtual void CheckParameters();

 public:
  /**
   *
   * @param name name of input file or list
   * @param mulit_mode if true then input file is file with list
   */
  NicaGeneratorReadCF(TString name, Bool_t mulit_mode = kFALSE);
  /**
   *
   * @param opt can be "init" (processing only partilcles from init branch or
   * "final" to process particles after UrQMD booster
   */
  void SetOption(Option_t *opt);
  void FinishTask();
  virtual ~NicaGeneratorReadCF();
  ClassDef(NicaGeneratorReadCF, 1)
};

#endif /* NICAGENERATORREADCF_H_ */
