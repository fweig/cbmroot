/*
 * NicaGeneratorReadUrQMD.h
 *
 *  Created on: 13 sie 2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAGENERATORREADURQMD_H_
#define NICAGENERATORREADURQMD_H_

#include "NicaGeneratorReadTxt.h"
/**
 * basic class for reading UrQMD files f13 and f14
 */
class NicaGeneratorReadUrQMD : public NicaGeneratorReadTxt {
  Int_t ReadEventTxt();
  Int_t ReadEventGz();
  /**
   * urqmd data format f13 and f14 are supported
   */
  int fFormat;
  Int_t fX_Id, fY_Id, fZ_Id, fT_Id;
  void ProcessParticlesF14Txt(Int_t ntracks);
  void ProcessParticlesF13Txt(Int_t ntracks);
  void ProcessParticlesF14Gz(Int_t ntracks);
  void ProcessParticlesF13Gz(Int_t ntracks);

 protected:
  virtual void CheckParameters();

 public:
  /**
   * default constructor
   * @param inFile
   * @param multi_mode
   */
  NicaGeneratorReadUrQMD(TString inFile, Bool_t multi_mode = kFALSE);
  virtual ~NicaGeneratorReadUrQMD();
  ClassDef(NicaGeneratorReadUrQMD, 1)
};

#endif /* NICAGENERATORREADURQMD_H_ */
