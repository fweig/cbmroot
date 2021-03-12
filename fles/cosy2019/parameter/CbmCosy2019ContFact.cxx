/////////////////////////////////////////////////////////////
//
//  CbmCosy2019ContFact
//
//  Factory for the parameter containers in libCosy2019
//
/////////////////////////////////////////////////////////////

#include "CbmCosy2019ContFact.h"

#include "CbmCosy2019HodoPar.h"

#include "FairRuntimeDb.h"

ClassImp(CbmCosy2019ContFact)

  static CbmCosy2019ContFact gCbmCosy2019ContFact;

CbmCosy2019ContFact::CbmCosy2019ContFact()
{
  // Constructor (called when the library is loaded)
  fName  = "CbmCosy2019ContFact";
  fTitle = "Factory for parameter containers for fles test library";
  setAllContainers();
  FairRuntimeDb::instance()->addContFactory(this);
}

void CbmCosy2019ContFact::setAllContainers()
{
  /** Creates the Container objects with all accepted contexts and adds them to
   *  the list of containers for the fles test library.*/

  FairContainer* pHodo =
    new FairContainer("CbmCosy2019HodoPar", "HODO at MCBM 2018 Unpack Parameters", "TestDefaultContext");
  pHodo->addContext("TestNonDefaultContext");
  containers->Add(pHodo);
}

FairParSet* CbmCosy2019ContFact::createContainer(FairContainer* c)
{
  /** Calls the constructor of the corresponding parameter container.
   * For an actual context, which is not an empty string and not the default context
   * of this container, the name is concatinated with the context. */
  const char* name = c->GetName();
  FairParSet* p    = 0;

  if (strcmp(name, "CbmCosy2019HodoPar") == 0) {
    p = new CbmCosy2019HodoPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
  }

  return p;
}
