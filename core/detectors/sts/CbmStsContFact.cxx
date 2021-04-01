/** @file CbmStsContFact.cxx
 ** @author Denis Bertini <d.bertini@gsi.de>
 ** @since 20.06.2005
 **/
#include "CbmStsContFact.h"

#include "CbmMcbm2018StsPar.h"       // for CbmMcbm2018StsPar
#include "CbmStsParSetModule.h"      // for CbmStsParSetModule
#include "CbmStsParSetSensor.h"      // for CbmStsParSetSensor
#include "CbmStsParSetSensorCond.h"  // for CbmStsParSetSensorCond
#include "CbmStsParSim.h"            // for CbmStsParSim

#include <FairParSet.h>     // for FairParSet
#include <FairRuntimeDb.h>  // for FairRuntimeDb
#include <Logger.h>         // for Logger, LOG

#include <TList.h>    // for TList
#include <TString.h>  // for TString

#include <string.h>  // for strcmp

ClassImp(CbmStsContFact)

  static CbmStsContFact gCbmStsContFact;


// -----   Constructor   ----------------------------------------------------
CbmStsContFact::CbmStsContFact() {
  SetName("CbmStsContFact");
  SetTitle("STS parameter container factory");
  setAllContainers();
  FairRuntimeDb::instance()->addContFactory(this);
}
// --------------------------------------------------------------------------


// -----   Create a parameter set   -----------------------------------------
FairParSet* CbmStsContFact::createContainer(FairContainer* container) {

  const char* contName = container->GetName();
  FairParSet* parSet   = nullptr;

  // --- Simulation settings
  if (strcmp(contName, "CbmStsParSim") == 0) {
    parSet = new CbmStsParSim(container->getConcatName().Data(),
                              container->GetTitle(),
                              container->getContext());
  }

  // --- Module parameters
  else if (strcmp(contName, "CbmStsParSetModule") == 0) {
    parSet = new CbmStsParSetModule(container->getConcatName().Data(),
                                    container->GetTitle(),
                                    container->getContext());
  }

  // --- Sensor parameters
  else if (strcmp(contName, "CbmStsParSetSensor") == 0) {
    LOG(info) << "createContainer " << container->getConcatName().Data() << " "
              << container->GetTitle() << " " << container->getContext();
    parSet = new CbmStsParSetSensor(container->getConcatName().Data(),
                                    container->GetTitle(),
                                    container->getContext());
    LOG(info) << "Done";
  }

  // --- Sensor conditions
  else if (strcmp(contName, "CbmStsParSetSensorCond") == 0) {
    parSet = new CbmStsParSetSensorCond(container->getConcatName().Data(),
                                        container->GetTitle(),
                                        container->getContext());
  }

  // --- Beamtime parameters
  else if (strcmp(contName, "CbmMcbm2018StsPar") == 0) {
    parSet = new CbmMcbm2018StsPar(container->getConcatName().Data(), container->GetTitle(), container->getContext());
  }

  LOG(info) << GetName() << ": Create container " << contName
            << " with parameter set " << parSet->GetName();
  return parSet;
}
// --------------------------------------------------------------------------


// ----   Define containers and contexts   ----------------------------------
void CbmStsContFact::setAllContainers() {

  // --- Simulation settings
  FairContainer* simPars =
    new FairContainer("CbmStsParSim", "STS simulation settings", "Default");
  simPars->addContext("Default");
  containers->Add(simPars);

  // --- Module parameters
  FairContainer* modulePars =
    new FairContainer("CbmStsParSetModule", "STS module parameters", "Default");
  modulePars->addContext("Default");
  containers->Add(modulePars);

  // --- Sensor parameters
  FairContainer* sensorPars =
    new FairContainer("CbmStsParSetSensor", "STS sensor parameters", "Default");
  sensorPars->addContext("Default");
  containers->Add(sensorPars);

  // --- Sensor conditions
  FairContainer* sensorCond = new FairContainer(
    "CbmStsParSetSensorCond", "STS sensor conditions", "Default");
  sensorCond->addContext("Default");
  containers->Add(sensorCond);

  // Beamtime parameters
  FairContainer* beamPars = new FairContainer("CbmMcbm2018StsPar", "STS at MCBM 2018 Unpack Parameters", "Default");
  beamPars->addContext("Default");
  containers->Add(beamPars);
}
// --------------------------------------------------------------------------
