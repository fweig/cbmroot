/*
 * NicaCorrFitInfo.h
 *
 *  Created on: 10 kwi 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACORRFITINFO_H_
#define NICACORRFITINFO_H_

#include <TNamed.h>
#include <TString.h>

#include "NicaCorrFitDatabase.h"
/**
 * class that represent basic info about settings used to calculate CF map
 */

class NicaCorrFitInfo : public TNamed {
  TObject* fCF;
  NicaCorrFitDbParamsSetup fSetup;
  TString fMacroText;
  TString fPairFile;

public:
  NicaCorrFitInfo();
  NicaCorrFitInfo(const NicaCorrFitInfo& other);
  void SetCf(TObject* cf) { fCF = cf; }
  void SetPairFile(TString pairFile) { fPairFile = pairFile; }
  void SetSetup(const NicaCorrFitDbParamsSetup& setup) { fSetup = setup; }
  void SetMacroText(const TString& macroText) { fMacroText = macroText; }
  void Print(Option_t* option = "") const;
  TObject* GetCf() const { return fCF; }
  TString GetMacroText() const { return fMacroText; }
  TString GetPairFile() const { return fPairFile; }
  NicaCorrFitDbParamsSetup GetSetup() const { return fSetup; }
  virtual ~NicaCorrFitInfo();
  ClassDef(NicaCorrFitInfo, 1)
};

#endif /* NICACORRFITINFO_H_ */
