/////////////////////////////////////////////////////////////
//
//  CbmCern2017ContFact
//
//  Factory for the parameter containers in libFlibCern2017
//
/////////////////////////////////////////////////////////////

#include "CbmCern2017ContFact.h"


#include "CbmCern2017UnpackParSts.h"
//#include "CbmMuchUnpackPar.h"
#include "CbmCern2017UnpackParHodo.h"

#include "FairRuntimeDb.h"

ClassImp(CbmCern2017ContFact)

static CbmCern2017ContFact gCbmCern2017ContFact;

CbmCern2017ContFact::CbmCern2017ContFact() {
  // Constructor (called when the library is loaded)
  fName="CbmCern2017ContFact";
  fTitle="Factory for parameter containers for fles CERN 2017 library";
  setAllContainers();
  FairRuntimeDb::instance()->addContFactory(this);
}

void CbmCern2017ContFact::setAllContainers() {
  /** Creates the Container objects with all accepted contexts and adds them to
   *  the list of containers for the fles test library.*/

    FairContainer* p= new FairContainer("CbmCern2017UnpackParSts",
                                          "Sts Unpack Parameters",
                                          "TestDefaultContext");
    p->addContext("TestNonDefaultContext");

    containers->Add(p);
/*
    FairContainer* p2= new FairContainer("CbmCern2017UnpackParMuch",
                                          "Much Unpack Parameters",
                                          "TestDefaultContext");
    p2->addContext("TestNonDefaultContext");

    containers->Add(p2);
*/

    FairContainer* p3= new FairContainer("CbmCern2017UnpackParHodo",
                                          "Hodo Unpack Parameters",
                                          "TestDefaultContext");
    p3->addContext("TestNonDefaultContext");

    containers->Add(p3);

}

FairParSet* CbmCern2017ContFact::createContainer(FairContainer* c) {
  /** Calls the constructor of the corresponding parameter container.
   * For an actual context, which is not an empty string and not the default context
   * of this container, the name is concatinated with the context. */
  const char* name=c->GetName();
  FairParSet* p=0;
  if (strcmp(name,"CbmCern2017UnpackParSts")==0) {
      p=new CbmCern2017UnpackParSts(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  }
/*
  if (strcmp(name,"CbmCern2017UnpackParMuch")==0) {
      p=new CbmCern2017UnpackParMuch(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  }
*/
  if (strcmp(name,"CbmCern2017UnpackParHodo")==0) {
      p=new CbmCern2017UnpackParHodo(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  }

  return p;
}

