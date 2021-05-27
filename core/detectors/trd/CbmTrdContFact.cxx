//*-- AUTHOR : Ilse Koenig
//*-- Created : 25/10/2004

/////////////////////////////////////////////////////////////
//
//  CbmTrdContFact
//
//  Factory for the parameter containers in libTrd
//
/////////////////////////////////////////////////////////////
#include "CbmTrdContFact.h"

#include "CbmMcbm2020TrdTshiftPar.h"  // for CbmMcbm2020TrdTshiftPar
#include "CbmTrdParSetAsic.h"         // for CbmTrdParSetAsic
#include "CbmTrdParSetDigi.h"         // for CbmTrdParSetDigi
#include "CbmTrdParSetGain.h"         // for CbmTrdParSetGain
#include "CbmTrdParSetGas.h"          // for CbmTrdParSetGas
#include "CbmTrdParSetGeo.h"          // for CbmTrdParSetGeo

#include <FairContFact.h>   // for FairContainer
#include <FairRuntimeDb.h>  // for FairRuntimeDb
#include <Logger.h>         // for Logger, LOG

#include <TList.h>    // for TList
#include <TString.h>  // for TString

#include <string.h>  // for strcmp

ClassImp(CbmTrdContFact)

  static CbmTrdContFact gCbmTrdContFact;

CbmTrdContFact::CbmTrdContFact()
{
  // Constructor (called when the library is loaded)
  fName  = "CbmTrdContFact";
  fTitle = "Factory for parameter containers in libTrd";
  setAllContainers();
  FairRuntimeDb::instance()->addContFactory(this);
}

void CbmTrdContFact::setAllContainers()
{
  /** Creates the Container objects with all accepted contexts and adds them to
   *  the list of containers for the STS library.*/

  // AB
  FairContainer* par(nullptr);
  // ASIC parametsr
  par = new FairContainer("CbmTrdParSetAsic", "Trd ASIC Parameters", "TestDefaultContext");
  par->addContext("TestNonDefaultContext");
  containers->Add(par);
  // read-out parameters
  par = new FairContainer("CbmTrdParSetDigi", "Trd Read-Out Parameters", "TestDefaultContext");
  par->addContext("TestNonDefaultContext");
  containers->Add(par);
  // gas parameters
  par = new FairContainer("CbmTrdParSetGas", "Trd Gas Parameters", "TestDefaultContext");
  par->addContext("TestNonDefaultContext");
  containers->Add(par);
  // gain parameters
  par = new FairContainer("CbmTrdParSetGain", "Trd Gain Parameters", "TestDefaultContext");
  par->addContext("TestNonDefaultContext");
  containers->Add(par);
  // geometry parameters
  par = new FairContainer("CbmTrdParSetGeo", "Trd Geometry Parameters", "TestDefaultContext");
  par->addContext("TestNonDefaultContext");
  containers->Add(par);

  FairContainer* pTrd =
    new FairContainer("CbmMcbm2020TrdTshiftPar", "TRD timeshift unpacker parameters mCbm 2020", "Default");
  pTrd->addContext("Default");
  containers->Add(pTrd);
}

FairParSet* CbmTrdContFact::createContainer(FairContainer* c)
{
  /** Calls the constructor of the corresponding parameter container.
   * For an actual context, which is not an empty string and not the default context
   * of this container, the name is concatinated with the context. */
  const char* name = c->GetName();
  LOG(info) << GetName() << "::createContainer :" << name;

  FairParSet* p(nullptr);
  if (strcmp(name, "CbmTrdParSetAsic") == 0)
    p = new CbmTrdParSetAsic(c->getConcatName().Data(), c->GetTitle(), c->getContext());
  else if (strcmp(name, "CbmTrdParSetDigi") == 0)
    p = new CbmTrdParSetDigi(c->getConcatName().Data(), c->GetTitle(), c->getContext());
  else if (strcmp(name, "CbmTrdParSetGas") == 0)
    p = new CbmTrdParSetGas(c->getConcatName().Data(), c->GetTitle(), c->getContext());
  else if (strcmp(name, "CbmTrdParSetGain") == 0)
    p = new CbmTrdParSetGain(c->getConcatName().Data(), c->GetTitle(), c->getContext());
  else if (strcmp(name, "CbmTrdParSetGeo") == 0)
    p = new CbmTrdParSetGeo(c->getConcatName().Data(), c->GetTitle(), c->getContext());
  else if (strcmp(name, "CbmMcbm2020TrdTshiftPar") == 0) {
    p = new CbmMcbm2020TrdTshiftPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
  }

  return p;
}
