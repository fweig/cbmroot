/* Copyright (C) 2018-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#ifndef CBMTRDPARSETDIGI_H
#define CBMTRDPARSETDIGI_H

#include "CbmTrdParSet.h"  // for CbmTrdParSet

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Bool_t

class FairParamList;

/**
  * \brief Describe TRD module working settings (HV, etc)
  **/
class FairParamList;
class CbmTrdParSetDigi : public CbmTrdParSet {
public:
  /** Standard constructor **/
  CbmTrdParSetDigi(const char* name = "CbmTrdParSetDigi", const char* title = "TRD chamber parameters",
                   const char* context = "Default");

  /** \brief Destructor **/
  virtual ~CbmTrdParSetDigi() { ; }

  /** \brief Reset all parameters **/
  virtual void clear() { ; }

  void putParams(FairParamList*);
  Bool_t getParams(FairParamList*);

private:
  ClassDef(CbmTrdParSetDigi,
           1)  // Container of the chamber parameters for the TRD detector
};
#endif
