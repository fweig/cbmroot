// ------------------------------------------------------------------
// -----                     TMbsUnpackTofPar                   -----
// -----              Created 08/05/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------

#include "TMbsUnpackTofContFact.h"

#include "TMbsUnpackTofPar.h"

#include "TMbsCalibTofPar.h"

#include "TMbsMappingTofPar.h"

#include "TMbsConvTofPar.h"

#include "FairRuntimeDb.h"

#include <iostream>

using std::cout;
using std::endl;

ClassImp(TMbsUnpackTofContFact)

static TMbsUnpackTofContFact gTMbsUnpackTofContFact;

TMbsUnpackTofContFact::TMbsUnpackTofContFact() {
  // Constructor (called when the library is loaded)
  fName="TMbsUnpackTofContFact";
  fTitle="Factory for parameter containers in libTofUnpack";
  setAllContainers();
  FairRuntimeDb::instance()->addContFactory(this);
}

void TMbsUnpackTofContFact::setAllContainers() {
  /** Creates the Container objects with all accepted contexts and adds them to
   *  the list of containers for the Tof library.*/

    FairContainer* p= new FairContainer("TMbsUnpackTofPar",
                                          "Tof MBS Unpack Parameters",
                                          "TestDefaultContext");
    p->addContext("TestNonDefaultContext");

    containers->Add(p);

    FairContainer* p2= new FairContainer("TMbsCalibTofPar",
                                          "Tof MBS Calibration Parameters",
                                          "TestDefaultContext");
    p2->addContext("TestNonDefaultContext");

    containers->Add(p2);

    FairContainer* p3= new FairContainer("TMbsMappingTofPar",
                                          "Tof MBS Mapping Parameters",
                                          "TestDefaultContext");
    p3->addContext("TestNonDefaultContext");

    containers->Add(p3);

    FairContainer* p4= new FairContainer("TMbsConvTofPar",
                                          "Tof MBS Conversion Parameters",
                                          "TestDefaultContext");
    p4->addContext("TestNonDefaultContext");

    containers->Add(p4);
}

FairParSet* TMbsUnpackTofContFact::createContainer(FairContainer* c) {
  /** Calls the constructor of the corresponding parameter container.
   * For an actual context, which is not an empty string and not the default context
   * of this container, the name is concatinated with the context. */
  const char* name=c->GetName();
  cout << " -I container name " << name << endl;
  
  FairParSet* p=NULL;
  if (strcmp(name,"TMbsUnpackTofPar")==0) {
    p=new TMbsUnpackTofPar(c->getConcatName().Data(), c->GetTitle(),c->getContext());
  }
  else if (strcmp(name,"TMbsCalibTofPar")==0) {
    p=new TMbsCalibTofPar(c->getConcatName().Data(), c->GetTitle(),c->getContext());
  }
  else if (strcmp(name,"TMbsMappingTofPar")==0) {
    p=new TMbsMappingTofPar(c->getConcatName().Data(), c->GetTitle(),c->getContext());
  }
  else if (strcmp(name,"TMbsConvTofPar")==0) {
    p=new TMbsConvTofPar(c->getConcatName().Data(), c->GetTitle(),c->getContext());
  }
  
  return p;
}

