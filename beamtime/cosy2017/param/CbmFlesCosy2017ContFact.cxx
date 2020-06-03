/////////////////////////////////////////////////////////////
//
//  CbmFlesCosy2017ContFact
//
//  Factory for the parameter containers in libFlesTest
//
/////////////////////////////////////////////////////////////

#include "CbmFlesCosy2017ContFact.h"

#include "CbmStsUnpackPar.h"

#include "FairRuntimeDb.h"

ClassImp(CbmFlesCosy2017ContFact)

static CbmFlesCosy2017ContFact gCbmFlesCosy2017ContFact;

CbmFlesCosy2017ContFact::CbmFlesCosy2017ContFact() {
  // Constructor (called when the library is loaded)
  fName="CbmFlesCosy2017ContFact";
  fTitle="Factory for parameter containers for fles cosy 2017 library";
  setAllContainers();
  FairRuntimeDb::instance()->addContFactory(this);
}

void CbmFlesCosy2017ContFact::setAllContainers() {
  /** Creates the Container objects with all accepted contexts and adds them to
   *  the list of containers for the fles test library.*/

    FairContainer* p1= new FairContainer("CbmStsUnpackPar",
                                          "STS Unpack Parameters",
                                          "TestDefaultContext");
    p1->addContext("TestNonDefaultContext");

    containers->Add(p1); 
}

FairParSet* CbmFlesCosy2017ContFact::createContainer(FairContainer* c) {
  /** Calls the constructor of the corresponding parameter container.
   * For an actual context, which is not an empty string and not the default context
   * of this container, the name is concatinated with the context. */
  const char* name=c->GetName();
  FairParSet* p=0;
  if (strcmp(name,"CbmStsUnpackPar")==0) {
      p=new CbmStsUnpackPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  }
  return p;
}

