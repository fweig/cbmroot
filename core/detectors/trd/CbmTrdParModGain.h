/** @file CbmTrdParModGain.h
  * @copyright Copyright (C) 2018-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [orginator] **/

#ifndef CBMTRDPARMODGAIN_H
#define CBMTRDPARMODGAIN_H

#include "CbmTrdParMod.h"  // for CbmTrdParMod

#include <Rtypes.h>  // for THashConsistencyHolder, ClassDef

/** \brief Definition of gain parameters for one TRD module **/
class CbmTrdParModGain : public CbmTrdParMod {
public:
  CbmTrdParModGain(const char* name = "CbmTrdParModGain", const char* title = "TRD gain conversion");
  virtual ~CbmTrdParModGain() { ; }

private:
  ClassDef(CbmTrdParModGain,
           1)  // Definition of gain parameters for one TRD module
};

#endif
