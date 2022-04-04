/* Copyright (C) 2020-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

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

#include "CbmMcbm2018TofPar.h"  // for CbmMcbm2018TofPar
#include "CbmTofDigiBdfPar.h"   // for CbmTofDigiBdfPar
#include "CbmTofDigiPar.h"      // for CbmTofDigiPar

#include <FairContFact.h>   // for FairContainer
#include <FairRuntimeDb.h>  // for FairRuntimeDb
#include <Logger.h>         // for LOG

#include <TList.h>    // for TList
#include <TString.h>  // for TString

#include <string.h>  // for strcmp

ClassImp(CbmTofContFact)

  static CbmTofContFact gCbmTofContFact;

CbmTofContFact::CbmTofContFact()
{
  // Constructor (called when the library is loaded)
  fName  = "CbmTofContFact";
  fTitle = "Factory for parameter containers in libTof";
  setAllContainers();
  FairRuntimeDb::instance()->addContFactory(this);
}

void CbmTofContFact::setAllContainers()
{
  /** Creates the Container objects with all accepted contexts and adds them to
   *  the list of containers for the Tof library.*/

  FairContainer* p1 = new FairContainer("CbmTofDigiPar", "TOF Digitization parameters", "TestDefaultContext");
  p1->addContext("TestNonDefaultContext");

  containers->Add(p1);

  FairContainer* p2 = new FairContainer("CbmTofDigiBdfPar", "TOF BDF Digitization parameters", "TestDefaultContext");
  p2->addContext("TestNonDefaultContext");

  containers->Add(p2);

  FairContainer* beamPars = new FairContainer("CbmMcbm2018TofPar", "TOF at MCBM 2018 Unpack Parameters", "Default");
  beamPars->addContext("Default");
  containers->Add(beamPars);

  FairContainer* beamParsBmon = new FairContainer(
    "CbmMcbm2018BmonPar", "BMon at MCBM 2022+ Unpack Parameters, interim for dual instances", "Default");
  beamParsBmon->addContext("Default");
  containers->Add(beamParsBmon);
}

FairParSet* CbmTofContFact::createContainer(FairContainer* c)
{
  /** Calls the constructor of the corresponding parameter container.
   * For an actual context, which is not an empty string and not the default context
   * of this container, the name is concatinated with the context. */
  const char* name = c->GetName();
  LOG(info) << "container name " << name;
  FairParSet* p = nullptr;
  if (strcmp(name, "CbmTofDigiPar") == 0) {
    p = new CbmTofDigiPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
  }
  else if (strcmp(name, "CbmTofDigiBdfPar") == 0) {
    p = new CbmTofDigiBdfPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
  }
  else if (strcmp(name, "CbmMcbm2018TofPar") == 0) {
    p = new CbmMcbm2018TofPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
  }
  else if (strcmp(name, "CbmMcbm2018BmonPar") == 0) {
    p = new CbmMcbm2018BmonPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
  }

  return p;
}
