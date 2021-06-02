/** @file CbmRichContFact.h
  * @copyright Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

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

#include <FairContFact.h>  // for FairContFact

#include <Rtypes.h>  // for THashConsistencyHolder, ClassDef

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
