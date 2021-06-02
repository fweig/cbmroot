/** @file CbmTrdParSetGas.h
  * @copyright Copyright (C) 2018-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

#ifndef CBMTRDPARSETGAS_H
#define CBMTRDPARSETGAS_H

#include "CbmTrdParSet.h"  // for CbmTrdParSet

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Int_t, Bool_t, Char_t

class FairParamList;
class TH2F;

/**
  * \brief Describe TRD module working settings (HV, etc)
  **/
class CbmTrdParSetGas : public CbmTrdParSet {
public:
  /** Standard constructor **/
  CbmTrdParSetGas(const char* name = "CbmTrdParSetGas", const char* title = "TRD chamber parameters",
                  const char* context = "Default");

  /** \brief Destructor **/
  virtual ~CbmTrdParSetGas() { ; }

  /** \brief Reset all parameters **/
  virtual void clear() { ; }

  void putParams(FairParamList*);
  Bool_t getParams(FairParamList*);

private:
  TH2F* GetDriftMap(const Char_t* g, const Int_t ua, const Int_t ud);

  ClassDef(CbmTrdParSetGas,
           1)  // Container of the chamber parameters for the TRD detector
};
#endif
