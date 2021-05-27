/** @file CbmTofContFact.h
  * @copyright Copyright (C) 2020-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [orginator] **/

#ifndef CBMTOFCONTFACT_H
#define CBMTOFCONTFACT_H

#include <FairContFact.h>  // for FairContFact

#include <Rtypes.h>  // for THashConsistencyHolder, ClassDef

class FairParSet;
class FairContainer;

class CbmTofContFact : public FairContFact {

public:
  CbmTofContFact();
  ~CbmTofContFact() {}
  FairParSet* createContainer(FairContainer*);

private:
  void setAllContainers();
  CbmTofContFact(const CbmTofContFact&);
  CbmTofContFact& operator=(const CbmTofContFact&);
  ClassDef(CbmTofContFact, 0)  // Factory for all TOF parameter containers
};

#endif /* !CBMTOFCONTFACT_H */
