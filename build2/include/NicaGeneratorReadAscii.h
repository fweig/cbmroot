/*
 * NicaGeneratorReadAscii.h
 *
 *  Created on: 13 sie 2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef _NICAGENERATORREADASCII_H_
#define _NICAGENERATORREADASCII_H_

#include "NicaGeneratorReadTxt.h"
/**
 * basic class for reading ascii generators
 */
class NicaGeneratorReadAscii : public NicaGeneratorReadTxt {
 private:
  Int_t fX_Id, fY_Id, fZ_Id, fT_Id;
  Int_t fVx_Id, fVy_Id, fVz_Id, fVt_Id;
  Int_t fEVx_Id, fEVy_Id, fEVz_Id, fNPa_Id;
  Int_t fParent_Id;
  Int_t ReadEventTxt();
  Int_t ReadEventGz();
  // void Reopen();
  Bool_t fExtended;

 protected:
  virtual void CheckParameters();

 public:
  /**
   * basic constructor
   * @param filename name of input file
   * @param multi if true then filename is treate like list of files
   */
  NicaGeneratorReadAscii(TString filename, Bool_t multi = kFALSE);
  /**
   * enable extended conversion (if freezout coordinates are available)
   */
  void EnableExtended() { fExtended = kTRUE; };
  virtual ~NicaGeneratorReadAscii();
  ClassDef(NicaGeneratorReadAscii, 1)
};

#endif /* _NICAGENERATORREADASCII_H_ */
