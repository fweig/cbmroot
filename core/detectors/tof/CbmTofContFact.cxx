//*-- AUTHOR : Ilse Koenig
//*-- Created : 25/10/2004

/////////////////////////////////////////////////////////////
//
//  CbmTofContFact
//
//  Factory for the parameter containers in libTof
//
/////////////////////////////////////////////////////////////
#include "CbmTofContFact.h"

#include "CbmTofDigiBdfPar.h"  // for CbmTofDigiBdfPar
#include "CbmTofDigiPar.h"     // for CbmTofDigiPar

#include <FairContFact.h>   // for FairContainer
#include <FairLogger.h>     // for LOG
#include <FairRuntimeDb.h>  // for FairRuntimeDb

#include <TList.h>    // for TList
#include <TString.h>  // for TString

#include <string.h>  // for strcmp

ClassImp(CbmTofContFact)

  static CbmTofContFact gCbmTofContFact;

CbmTofContFact::CbmTofContFact() {
  // Constructor (called when the library is loaded)
  fName  = "CbmTofContFact";
  fTitle = "Factory for parameter containers in libTof";
  setAllContainers();
  FairRuntimeDb::instance()->addContFactory(this);
}

void CbmTofContFact::setAllContainers() {
  /** Creates the Container objects with all accepted contexts and adds them to
   *  the list of containers for the Tof library.*/

  FairContainer* p1 = new FairContainer(
    "CbmTofDigiPar", "TOF Digitization parameters", "TestDefaultContext");
  p1->addContext("TestNonDefaultContext");

  containers->Add(p1);

  FairContainer* p2 = new FairContainer("CbmTofDigiBdfPar",
                                        "TOF BDF Digitization parameters",
                                        "TestDefaultContext");
  p2->addContext("TestNonDefaultContext");

  containers->Add(p2);
}

FairParSet* CbmTofContFact::createContainer(FairContainer* c) {
  /** Calls the constructor of the corresponding parameter container.
   * For an actual context, which is not an empty string and not the default context
   * of this container, the name is concatinated with the context. */
  const char* name = c->GetName();
  LOG(info) << "container name " << name;
  FairParSet* p = nullptr;
  if (strcmp(name, "CbmTofDigiPar") == 0) {
    p = new CbmTofDigiPar(
      c->getConcatName().Data(), c->GetTitle(), c->getContext());
  }
  if (strcmp(name, "CbmTofDigiBdfPar") == 0) {
    p = new CbmTofDigiBdfPar(
      c->getConcatName().Data(), c->GetTitle(), c->getContext());
  }
  return p;
}
