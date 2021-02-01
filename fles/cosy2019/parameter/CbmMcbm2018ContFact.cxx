/////////////////////////////////////////////////////////////
//
//  CbmMcbm2018ContFact
//
//  Factory for the parameter containers in libMcbm2018
//
/////////////////////////////////////////////////////////////

#include "CbmMcbm2018ContFact.h"

#include "CbmMcbm2018HodoPar.h"

#include "FairRuntimeDb.h"

ClassImp(CbmMcbm2018ContFact)

  static CbmMcbm2018ContFact gCbmMcbm2018ContFact;

CbmMcbm2018ContFact::CbmMcbm2018ContFact() {
  // Constructor (called when the library is loaded)
  fName  = "CbmMcbm2018ContFact";
  fTitle = "Factory for parameter containers for fles test library";
  setAllContainers();
  FairRuntimeDb::instance()->addContFactory(this);
}

void CbmMcbm2018ContFact::setAllContainers() {
  /** Creates the Container objects with all accepted contexts and adds them to
   *  the list of containers for the fles test library.*/

  FairContainer* pHodo =
    new FairContainer("CbmMcbm2018HodoPar",
                      "HODO at MCBM 2018 Unpack Parameters",
                      "TestDefaultContext");
  pHodo->addContext("TestNonDefaultContext");
  containers->Add(pHodo);

}

FairParSet* CbmMcbm2018ContFact::createContainer(FairContainer* c) {
  /** Calls the constructor of the corresponding parameter container.
   * For an actual context, which is not an empty string and not the default context
   * of this container, the name is concatinated with the context. */
  const char* name = c->GetName();
  FairParSet* p    = 0;

  if (strcmp(name, "CbmMcbm2018HodoPar") == 0) {
    p = new CbmMcbm2018HodoPar(
      c->getConcatName().Data(), c->GetTitle(), c->getContext());
  }

  return p;
}
