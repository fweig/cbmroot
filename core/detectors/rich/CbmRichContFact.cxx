/** CbmRichContFact.h
 *
 * @author  F. Uhlig <f.uhlig@gsi.de>
 * @version 1.0
 * @since   25.01.21
 *
 *  Factory for the parameter containers of the RICH detector
 *
 */
#include "CbmRichContFact.h"

#include "CbmMcbm2018RichPar.h"  // for CbmMcbm2018RichPar

#include <FairContFact.h>   // for FairContainer
#include <FairLogger.h>     // for LOG
#include <FairRuntimeDb.h>  // for FairRuntimeDb

#include <TList.h>    // for TList
#include <TString.h>  // for TString

#include <string.h>  // for strcmp

ClassImp(CbmRichContFact)

  static CbmRichContFact gCbmRichContFact;

CbmRichContFact::CbmRichContFact() {
  // Constructor (called when the library is loaded)
  fName  = "CbmRichContFact";
  fTitle = "Factory for parameter containers in libRichBase";
  setAllContainers();
  FairRuntimeDb::instance()->addContFactory(this);
}

void CbmRichContFact::setAllContainers() {
  /** Creates the Container objects with all accepted contexts and adds them to
   *  the list of containers for the RichBase library.*/

  FairContainer* beamPars = new FairContainer(
    "CbmMcbm2018RichPar", "Rich at MCBM 2018 Unpack Parameters", "Default");
  beamPars->addContext("Default");
  containers->Add(beamPars);
}

FairParSet* CbmRichContFact::createContainer(FairContainer* c) {
  /** Calls the constructor of the corresponding parameter container.
   * For an actual context, which is not an empty string and not the default context
   * of this container, the name is concatinated with the context. */
  const char* name = c->GetName();
  LOG(info) << " -I container name " << name;
  FairParSet* p = 0;
  if (strcmp(name, "CbmMcbm2018RichPar") == 0) {
    p = new CbmMcbm2018RichPar(
      c->getConcatName().Data(), c->GetTitle(), c->getContext());
  }

  return p;
}
