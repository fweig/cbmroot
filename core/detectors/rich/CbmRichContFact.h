/** CbmRichContFact.h
 *
 * @author  F. Uhlig <f.uhlig@gsi.de>
 * @version 1.0
 * @since   25.01.21
 *
 *  Factory for the parameter containers of the RICH detector
 *
 */

#ifndef CBMRICHCONTFACT_H
#define CBMRICHCONTFACT_H

#include <Rtypes.h>  // for THashConsistencyHolder, ClassDef

#include <FairContFact.h>  // for FairContFact

class FairParIo;
class FairParSet;
class FairContainer;

class CbmRichContFact : public FairContFact {

public:
  CbmRichContFact();
  ~CbmRichContFact() {}
  FairParSet* createContainer(FairContainer*);

private:
  void setAllContainers();
  ClassDef(CbmRichContFact, 0)  // Factory for all Rich parameter containers
};

#endif /* !CBMRICHCONTFACT_H */
