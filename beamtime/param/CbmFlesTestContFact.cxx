/////////////////////////////////////////////////////////////
//
//  CbmFlesTestContFact
//
//  Factory for the parameter containers in libFlesTest
//
/////////////////////////////////////////////////////////////

#include "CbmFlesTestContFact.h"

#include "CbmFHodoUnpackPar.h"

#include "CbmTofUnpackPar.h"

#include "CbmMuchUnpackPar.h"

#include "FairRuntimeDb.h"

ClassImp(CbmFlesTestContFact)

static CbmFlesTestContFact gCbmFlesTestContFact;

CbmFlesTestContFact::CbmFlesTestContFact() {
  // Constructor (called when the library is loaded)
  fName="CbmFlesTestContFact";
  fTitle="Factory for parameter containers for fles test library";
  setAllContainers();
  FairRuntimeDb::instance()->addContFactory(this);
}

void CbmFlesTestContFact::setAllContainers() {
  /** Creates the Container objects with all accepted contexts and adds them to
   *  the list of containers for the fles test library.*/

    FairContainer* p= new FairContainer("CbmFHodoUnpackPar",
                                          "Fibre Hodoscope Unpack Parameters",
                                          "TestDefaultContext");
    p->addContext("TestNonDefaultContext");

    containers->Add(p);

    FairContainer* p2= new FairContainer("CbmTofUnpackPar",
                                          "Tof Unpack Parameters",
                                          "TestDefaultContext");
    p2->addContext("TestNonDefaultContext");

    containers->Add(p2); 

    FairContainer* p3 = new FairContainer("CbmMuchUnpackPar",
                                          "Much Unpack Parameters",
                                          "TestDefaultContext");
    p3->addContext("TestNonDefaultContext");

    containers->Add(p3); 
}

FairParSet* CbmFlesTestContFact::createContainer(FairContainer* c) {
  /** Calls the constructor of the corresponding parameter container.
   * For an actual context, which is not an empty string and not the default context
   * of this container, the name is concatinated with the context. */
  const char* name=c->GetName();
  FairParSet* p=0;
  if (strcmp(name,"CbmFHodoUnpackPar")==0) {
      p=new CbmFHodoUnpackPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  }
  if (strcmp(name,"CbmTofUnpackPar")==0) {
      p=new CbmTofUnpackPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  }
  if (strcmp(name,"CbmMuchUnpackPar")==0) {
      p=new CbmMuchUnpackPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  }
  return p;
}

