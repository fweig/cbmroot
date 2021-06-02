/** @file CbmPsdContFact.h
  * @copyright Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

/** CbmPsdContFact.h
 *
 * @author  F. Uhlig <f.uhlig@gsi.de>
 * @version 1.0
 * @since   25.01.21
 *
 *  Factory for the parameter containers of the PSD detector
 *
 */

#ifndef CBMPSDCONTFACT_H
#define CBMPSDCONTFACT_H

#include <FairContFact.h>  // for FairContFact

#include <Rtypes.h>  // for THashConsistencyHolder, ClassDef

class FairParIo;
class FairParSet;
class FairContainer;

class CbmPsdContFact : public FairContFact {
public:
  CbmPsdContFact();
  ~CbmPsdContFact() {}
  FairParSet* createContainer(FairContainer*);

private:
  void setAllContainers();

  ClassDef(CbmPsdContFact, 0)  // Factory for all Psd parameter containers
};

#endif /* !CBMPSDCONTFACT_H */
