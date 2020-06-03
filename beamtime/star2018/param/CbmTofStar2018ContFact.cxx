/////////////////////////////////////////////////////////////
//
//  CbmTofStar2018ContFact
//
//  Factory for the parameter containers in libFlesTest
//
/////////////////////////////////////////////////////////////

#include "CbmTofStar2018ContFact.h"

#include "CbmTofStar2018Par.h"

#include "FairRuntimeDb.h"

ClassImp(CbmTofStar2018ContFact)

static CbmTofStar2018ContFact gCbmTofStar2018ContFact;

CbmTofStar2018ContFact::CbmTofStar2018ContFact() {
  // Constructor (called when the library is loaded)
  fName="CbmTofStar2018ContFact";
  fTitle="Factory for parameter containers for fles test library";
  setAllContainers();
  FairRuntimeDb::instance()->addContFactory(this);
}

void CbmTofStar2018ContFact::setAllContainers() {
  /** Creates the Container objects with all accepted contexts and adds them to
   *  the list of containers for the fles test library.*/

    FairContainer* p= new FairContainer("CbmTofStar2018Par",
                                          "TOF at STAR 2018 Unpack Parameters",
                                          "TestDefaultContext");
    p->addContext("TestNonDefaultContext");

    containers->Add(p);
}

FairParSet* CbmTofStar2018ContFact::createContainer(FairContainer* c) {
  /** Calls the constructor of the corresponding parameter container.
   * For an actual context, which is not an empty string and not the default context
   * of this container, the name is concatinated with the context. */
  const char* name=c->GetName();
  FairParSet* p=0;
  if (strcmp(name,"CbmTofStar2018Par")==0) {
      p = new CbmTofStar2018Par(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  }
  return p;
}

