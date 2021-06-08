/* Copyright (C) 2006-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Denis Bertini [committer], Florian Uhlig, Mohammad Al-Turany */

#ifndef CBMTRDCONTFACT_H
#define CBMTRDCONTFACT_H

#include "FairContFact.h"  // for FairContFact

#include <Rtypes.h>  // for ClassDef

class FairParSet;
class FairContainer;

class CbmTrdContFact : public FairContFact {
private:
  void setAllContainers();
  CbmTrdContFact(const CbmTrdContFact&);
  CbmTrdContFact& operator=(const CbmTrdContFact&);

public:
  CbmTrdContFact();
  ~CbmTrdContFact() {}
  FairParSet* createContainer(FairContainer*);
  ClassDef(CbmTrdContFact, 0)  // Factory for all TRD parameter containers
};

#endif /* !CBMTRDCONTFACT_H */
