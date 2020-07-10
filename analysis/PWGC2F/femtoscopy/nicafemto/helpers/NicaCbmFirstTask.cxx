/*
 * NicaCbmFirstTask.cxx
 *
 *  Created on: 3 wrz 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "NicaCbmFirstTask.h"
#include "FairRunAna.h"

NicaCbmFirstTask::NicaCbmFirstTask() {
  // TODO Auto-generated constructor stub
}

InitStatus NicaCbmFirstTask::Init() {
  CbmHelix::SetField(FairRunAna::Instance()->GetField());
  return kSUCCESS;
}

NicaCbmFirstTask::~NicaCbmFirstTask() {
  // TODO Auto-generated destructor stub
}
