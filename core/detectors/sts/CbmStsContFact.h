/** @file CbmStsContFact.h
  * @copyright Copyright (C) 2006-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Volker Friese, Denis Bertini [committer] **/

/** @file CbmStsContFact.h
 ** @author Denis Bertini <d.bertini@gsi.de>
 ** @since 20.06.2005
 **/

#ifndef CBMSTSCONTFACT_H
#define CBMSTSCONTFACT_H 1

#include <FairContFact.h>  // for FairContFact, FairContainer (ptr only)

#include <Rtypes.h>  // for THashConsistencyHolder, ClassDef

class FairContainer;
class FairParSet;


/** @class CbmStsContFact
 ** @brief Factory class for STS parameter container
 ** @author Denis Bertini <d.bertini@gsi.de>
 ** @author Volker Friese <v.friese@gsi.de> (revision 2020)
 ** @since 20.06.2005
 ** @date 23.03.2020
 **
 ** The container factory enables FairRuntimeDb to create STS
 ** parameter containers when requested.
 **/
class CbmStsContFact : public FairContFact {

public:
  /** @brief Constructor **/
  CbmStsContFact();


  /** @brief Destructor **/
  ~CbmStsContFact() {}


  /** @brief Create a parameter set
     **
     ** Calls the constructor of the corresponding parameter set.
     ** For an actual context, which is not an empty string and not the
     ** default context of this container, the name is concatenated with
     ** the context.
     **
     ** The name of the method is somewhat misleading, since what is
     ** created is not a FairContainer, but a FairParSet. Only God
     ** and Ilse König can tell the difference, though. Doxygen surely cannot.
     **/
  FairParSet* createContainer(FairContainer*);


private:
  /** @brief Define parameter containers and their contexts **/
  void setAllContainers();

  ClassDef(CbmStsContFact, 0);
};

#endif /* CBMSTSCONTFACT_H */
