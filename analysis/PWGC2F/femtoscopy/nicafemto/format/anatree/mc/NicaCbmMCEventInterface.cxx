/** @file NicaCbmMCEventInterface.cxx
  * @copyright Copyright (C) 2020-2021 Faculty of Physics, Warsaw University of Technology, Warsaw
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Daniel Wielanek [committer] **/

/*
 * NicaCbmDstMcEventInterface.cxx
 *
 *  Created on: 17 wrz 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "NicaCbmMCEventInterface.h"

#include "CbmAnaTreeContainer.h"

#include <FairRootManager.h>

NicaCbmMCEventInterface::NicaCbmMCEventInterface() : fDataContainer(nullptr) {}

void NicaCbmMCEventInterface::ConnectToTree()
{
  FairRootManager* manager = FairRootManager::Instance();
  if (CanDeleteEvent()) { delete fDataContainer; }
  fDataContainer = (CbmAnaTreeMcSourceContainer*) manager->GetObject("CbmAnaTreeMcSourceContainer.");
}

void NicaCbmMCEventInterface::Register(Bool_t write)
{
  FairRootManager* manager = FairRootManager::Instance();
  manager->Register("CbmAnaTreeMcSourceContainer.", "CbmAnaTreeMcSourceContainer.", fDataContainer, write);
}

void NicaCbmMCEventInterface::CopyData(NicaEventInterface* s)
{
  NicaCbmMCEventInterface* ev = (NicaCbmMCEventInterface*) s;
  *fDataContainer             = *ev->fDataContainer;  //TODO make assignment operator
}

void NicaCbmMCEventInterface::Compress(Int_t* /*map*/, Int_t /*map_size*/)
{
  //TODO
}

void NicaCbmMCEventInterface::CopyAndCompress(NicaEventInterface* s, Int_t* /*map*/, Int_t /*map_size*/)
{
  CopyData(s);
}

void NicaCbmMCEventInterface::FillTrackInterface(NicaTrackInterface* /*track*/, Int_t /*index*/) {}

Int_t NicaCbmMCEventInterface::GetTotalTrackNo() const { return fDataContainer->GetParticles()->GetNumberOfChannels(); }

TObject* NicaCbmMCEventInterface::GetRawTrackPointer(Int_t /*index*/) const { return nullptr; }

TLorentzVector NicaCbmMCEventInterface::GetVertexError() const { return TLorentzVector(0, 0, 0, 0); }

TLorentzVector NicaCbmMCEventInterface::GetVertex() const
{
  AnalysisTree::EventHeader* header = fDataContainer->GetEventHeader();
  return TLorentzVector(header->GetVertexX(), header->GetVertexY(), header->GetVertexZ(), 0);
}

NicaCbmMCEventInterface::~NicaCbmMCEventInterface()
{
  if (CanDeleteEvent()) delete fDataContainer;
}
