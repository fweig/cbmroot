/** CbmPsdContFact.h
 *
 * @author  F. Uhlig <f.uhlig@gsi.de>
 * @version 1.0
 * @since   25.01.21
 *
 *  Factory for the parameter containers of the RICH detector
 *
 */
#include "CbmPsdContFact.h"

#include "CbmMcbm2018PsdPar.h"  // for CbmMcbm2018PsdPar

#include <FairContFact.h>   // for FairContainer
#include <FairLogger.h>     // for LOG
#include <FairRuntimeDb.h>  // for FairRuntimeDb

#include <TList.h>    // for TList
#include <TString.h>  // for TString

#include <string.h>  // for strcmp

ClassImp(CbmPsdContFact)

  static CbmPsdContFact gCbmPsdContFact;

CbmPsdContFact::CbmPsdContFact()
{
  // Constructor (called when the library is loaded)
  fName  = "CbmPsdContFact";
  fTitle = "Factory for parameter containers in libPsdBase";
  setAllContainers();
  FairRuntimeDb::instance()->addContFactory(this);
}

void CbmPsdContFact::setAllContainers()
{
  /** Creates the Container objects with all accepted contexts and adds them to
   *  the list of containers for the PsdBase library.*/

  FairContainer* beamPars = new FairContainer("CbmMcbm2018PsdPar", "Psd at MCBM 2018 Unpack Parameters", "Default");
  beamPars->addContext("Default");
  containers->Add(beamPars);
}

FairParSet* CbmPsdContFact::createContainer(FairContainer* c)
{
  /** Calls the constructor of the corresponding parameter container.
   * For an actual context, which is not an empty string and not the default context
   * of this container, the name is concatinated with the context. */
  const char* name = c->GetName();
  LOG(info) << " -I container name " << name;
  FairParSet* p = 0;
  if (strcmp(name, "CbmMcbm2018PsdPar") == 0) {
    p = new CbmMcbm2018PsdPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
  }

  return p;
}
