/** @file NicaGeneratorWriteUnigen.cxx
  * @copyright Copyright (C) 2015-2021 Faculty of Physics, Warsaw University of Technology, Warsaw
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Daniel Wielanek [committer], Daniel Wielanek [committer] **/

/*
 * NicaGeneratorWriteUnigen.cxx
 *
 *  Created on: 13 sie 2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "NicaGeneratorWriteUnigen.h"

#include <FairLogger.h>
#include <FairTask.h>

#include <TTree.h>

#include "NicaGeneratorParametersArray.h"
#include "NicaRunSim.h"

NicaGeneratorWriteUnigen::NicaGeneratorWriteUnigen() : NicaGeneratorWrite(4, 3, 1, 0, 9, 9, 0, 0)
{
  fEvent = new UEvent();
  // parameter 0 is reserverd for random stuff !
  //register event parameters int
  Register(4, "Nes", "Step_nr", "Npa", "Nr");
  //register event parameters double
  Register(3, "B", "Phi", "Step_t");
  //register track int parameters
  Register(9, "index", "pdg", "status", "parent", "parent_decay", "mate", "decay", "first_child", "last_child");
  //register track double parameters
  Register(9, "px", "py", "pz", "e", "frx", "fry", "frz", "frt", "weight");
}

InitStatus NicaGeneratorWriteUnigen::Init()
{
  InitStatus stat     = NicaGeneratorWrite::Init();
  NicaRunSim* manager = NicaRunSim::Instance();
  manager->GetTree()->Branch("event", &fEvent);
  LOG(info) << "Data will be set to Unigen format";
  return stat;
}

void NicaGeneratorWriteUnigen::ClearEvent() { fEvent->Clear(); }

void NicaGeneratorWriteUnigen::AddEvent()
{
  fEvent->SetNes(fEventParameters->GetInt(1));
  fEvent->SetStepNr(fEventParameters->GetInt(2));
  fEvent->SetEventNr(fEventParameters->GetInt(4));
  fEvent->SetB(fEventParameters->GetDouble(1));
  fEvent->SetPhi(fEventParameters->GetDouble(2));
  fEvent->SetStepT(fEventParameters->GetDouble(3));
}

void NicaGeneratorWriteUnigen::AddParticle()
{
  Int_t child[2] = {fTrackParameters->GetInt(8), fTrackParameters->GetInt(9)};
  fEvent->AddParticle(fTrackParameters->GetInt(1),  //index
                      fTrackParameters->GetInt(2),  //pdg
                      fTrackParameters->GetInt(3),  ///status
                      fTrackParameters->GetInt(4),  //parent
                      fTrackParameters->GetInt(5),  //parent_decay
                      fTrackParameters->GetInt(6),  //mate
                      fTrackParameters->GetInt(7),  //decay
                      child, fTrackParameters->GetDouble(1), fTrackParameters->GetDouble(2),
                      fTrackParameters->GetDouble(3), fTrackParameters->GetDouble(4), fTrackParameters->GetDouble(5),
                      fTrackParameters->GetDouble(6), fTrackParameters->GetDouble(7), fTrackParameters->GetDouble(8),
                      fTrackParameters->GetDouble(9));
}

NicaGeneratorWriteUnigen::~NicaGeneratorWriteUnigen() {}
