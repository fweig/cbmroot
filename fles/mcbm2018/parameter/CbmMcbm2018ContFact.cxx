/////////////////////////////////////////////////////////////
//
//  CbmMcbm2018ContFact
//
//  Factory for the parameter containers in libMcbm2018
//
/////////////////////////////////////////////////////////////

#include "CbmMcbm2018ContFact.h"

#include "CbmMcbm2018HodoPar.h"
#include "CbmMcbm2018MuchPar.h"
#include "CbmMcbm2018PsdPar.h"
#include "CbmMcbm2018RichPar.h"
#include "CbmMcbm2018StsPar.h"
#include "CbmMcbm2018TofPar.h"
#include "CbmMcbm2020TrdTshiftPar.h"

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

  FairContainer* pSts = new FairContainer("CbmMcbm2018StsPar",
                                          "STS at MCBM 2018 Unpack Parameters",
                                          "TestDefaultContext");
  pSts->addContext("TestNonDefaultContext");
  containers->Add(pSts);

  FairContainer* pMuch =
    new FairContainer("CbmMcbm2018MuchPar",
                      "MUCH at MCBM 2018 Unpack Parameters",
                      "TestDefaultContext");
  pMuch->addContext("TestNonDefaultContext");
  containers->Add(pMuch);


  FairContainer* pTof = new FairContainer("CbmMcbm2018TofPar",
                                          "TOF at MCBM 2018 Unpack Parameters",
                                          "TestDefaultContext");
  pTof->addContext("TestNonDefaultContext");
  containers->Add(pTof);

  FairContainer* pRich =
    new FairContainer("CbmMcbm2018RichPar",
                      "RICH at MCBM 2018 Unpack Parameters",
                      "TestDefaultContext");
  pRich->addContext("TestNonDefaultContext");
  containers->Add(pRich);

  FairContainer* pHodo =
    new FairContainer("CbmMcbm2018HodoPar",
                      "HODO at MCBM 2018 Unpack Parameters",
                      "TestDefaultContext");
  pHodo->addContext("TestNonDefaultContext");
  containers->Add(pHodo);

  FairContainer* pPsd = new FairContainer("CbmMcbm2018PsdPar",
                                          "PSD at MCBM 2018 Unpack Parameters",
                                          "TestDefaultContext");
  pPsd->addContext("TestNonDefaultContext");
  containers->Add(pPsd);

  FairContainer* pTrd =
    new FairContainer("CbmMcbm2020TrdTshiftPar",
                      "TRD timeshift unpacker parameters mCbm 2020",
                      "Trd mCbm 2020 timeshift correction");
  pTrd->addContext("TestDefaultContext");
  containers->Add(pTrd);
}

FairParSet* CbmMcbm2018ContFact::createContainer(FairContainer* c) {
  /** Calls the constructor of the corresponding parameter container.
   * For an actual context, which is not an empty string and not the default context
   * of this container, the name is concatinated with the context. */
  const char* name = c->GetName();
  FairParSet* p    = 0;

  if (strcmp(name, "CbmMcbm2018StsPar") == 0) {
    p = new CbmMcbm2018StsPar(
      c->getConcatName().Data(), c->GetTitle(), c->getContext());
  }
  if (strcmp(name, "CbmMcbm2018MuchPar") == 0) {
    p = new CbmMcbm2018MuchPar(
      c->getConcatName().Data(), c->GetTitle(), c->getContext());
  }
  if (strcmp(name, "CbmMcbm2018TofPar") == 0) {
    p = new CbmMcbm2018TofPar(
      c->getConcatName().Data(), c->GetTitle(), c->getContext());
  }
  if (strcmp(name, "CbmMcbm2018RichPar") == 0) {
    p = new CbmMcbm2018RichPar(
      c->getConcatName().Data(), c->GetTitle(), c->getContext());
  }
  if (strcmp(name, "CbmMcbm2018HodoPar") == 0) {
    p = new CbmMcbm2018HodoPar(
      c->getConcatName().Data(), c->GetTitle(), c->getContext());
  }
  if (strcmp(name, "CbmMcbm2018PsdPar") == 0) {
    p = new CbmMcbm2018PsdPar(
      c->getConcatName().Data(), c->GetTitle(), c->getContext());
  }
  if (strcmp(name, "CbmMcbm2020TrdTshiftPar") == 0) {
    p = new CbmMcbm2020TrdTshiftPar(
      c->getConcatName().Data(), c->GetTitle(), c->getContext());
  }

  return p;
}
