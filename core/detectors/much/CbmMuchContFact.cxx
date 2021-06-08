/* Copyright (C) 2006-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: M. Ryzhinskiy, Denis Bertini [committer], Florian Uhlig, Volker Friese, Mohammad Al-Turany */

/** CbmMuchContFact.cxx
 *
 * @author  M. Ryzhinskiy <m.ryzhinskiy@gsi.de>
 * @version 0.0
 * @since   15.03.07
 *
 * Factory for the parameter containers for MUon CHambers detector
 *
 */
#include "CbmMuchContFact.h"

#include "CbmGeoMuchPar.h"       // for CbmGeoMuchPar
#include "CbmMcbm2018MuchPar.h"  // for CbmMcbm2018MuchPar

#include <FairContFact.h>   // for FairContainer
#include <FairRuntimeDb.h>  // for FairRuntimeDb
#include <Logger.h>         // for LOG

#include <TList.h>    // for TList
#include <TString.h>  // for TString

#include <string.h>  // for strcmp

ClassImp(CbmMuchContFact)

  static CbmMuchContFact gCbmMuchContFact;

CbmMuchContFact::CbmMuchContFact()
{
  // Constructor (called when the library is loaded)
  fName  = "CbmMuchContFact";
  fTitle = "Factory for parameter containers in libMuch";
  setAllContainers();
  FairRuntimeDb::instance()->addContFactory(this);
}

void CbmMuchContFact::setAllContainers()
{
  /** Creates the Container objects with all accepted contexts and adds them to
   *  the list of containers for the MuCh library.*/
  //   FairContainer* p1= new FairContainer("CbmMuchDigiPar",
  // 				     "Much Digitization Parameters",
  // 				     "TestDefaultContext");
  //   p1->addContext("TestNonDefaultContext");
  //  containers->Add(p1);

  FairContainer* p2 = new FairContainer("CbmGeoMuchPar", "Much Geometry Parameters", "TestDefaultContext");
  p2->addContext("TestNonDefaultContext");
  containers->Add(p2);

  FairContainer* beamPars = new FairContainer("CbmMcbm2018MuchPar", "Much at MCBM 2018 Unpack Parameters", "Default");
  beamPars->addContext("Default");
  containers->Add(beamPars);
}

FairParSet* CbmMuchContFact::createContainer(FairContainer* c)
{
  /** Calls the constructor of the corresponding parameter container.
   * For an actual context, which is not an empty string and not the default context
   * of this container, the name is concatinated with the context. */
  const char* name = c->GetName();
  LOG(info) << " -I container name " << name;
  FairParSet* p = 0;
  if (strcmp(name, "CbmGeoMuchPar") == 0) {
    p = new CbmGeoMuchPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
  }
  else if (strcmp(name, "CbmMcbm2018MuchPar") == 0) {
    p = new CbmMcbm2018MuchPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
  }

  return p;
}

void CbmMuchContFact::activateParIo(FairParIo*)
{
  // activates the input/output class for the parameters
  // needed by the MuCh
  //   if (strcmp(io->IsA()->GetName(),"FairParRootFileIo")==0) {
  //     CbmMuchParRootFileIo* p=new CbmMuchParRootFileIo(((FairParRootFileIo*)io)->getParRootFile());
  //     io->setDetParIo(p);
  //   }
  //   else if (strcmp(io->IsA()->GetName(),"FairParAsciiFileIo")==0) {
  //     CbmMuchParAsciiFileIo* p=new CbmMuchParAsciiFileIo(((FairParAsciiFileIo*)io)->getFile());
  //     io->setDetParIo(p);
  //   }
}
