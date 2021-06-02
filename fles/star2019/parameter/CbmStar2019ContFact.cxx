/** @file CbmStar2019ContFact.cxx
  * @copyright Copyright (C) 2018 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Pierre-Alain Loizeau [committer] **/

/////////////////////////////////////////////////////////////
//
//  CbmStar2019ContFact
//
//  Factory for the parameter containers in libStar2019
//
/////////////////////////////////////////////////////////////

#include "CbmStar2019ContFact.h"

#include "CbmStar2019TofPar.h"

#include "FairRuntimeDb.h"

ClassImp(CbmStar2019ContFact)

  static CbmStar2019ContFact gCbmStar2019ContFact;

CbmStar2019ContFact::CbmStar2019ContFact()
{
  // Constructor (called when the library is loaded)
  fName  = "CbmStar2019ContFact";
  fTitle = "Factory for parameter containers for fles test library";
  setAllContainers();
  FairRuntimeDb::instance()->addContFactory(this);
}

void CbmStar2019ContFact::setAllContainers()
{
  /** Creates the Container objects with all accepted contexts and adds them to
   *  the list of containers for the fles test library.*/


  FairContainer* pTof =
    new FairContainer("CbmStar2019TofPar", "TOF at MCBM 2018 Unpack Parameters", "TestDefaultContext");
  pTof->addContext("TestNonDefaultContext");

  containers->Add(pTof);
}

FairParSet* CbmStar2019ContFact::createContainer(FairContainer* c)
{
  /** Calls the constructor of the corresponding parameter container.
   * For an actual context, which is not an empty string and not the default context
   * of this container, the name is concatinated with the context. */
  const char* name = c->GetName();
  FairParSet* p    = 0;

  if (strcmp(name, "CbmStar2019TofPar") == 0) {
    p = new CbmStar2019TofPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
  }

  return p;
}
