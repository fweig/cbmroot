/*
 * NicaCbmDstMcEventInterface.cxx
 *
 *  Created on: 17 wrz 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "NicaCbmATMCEventInterface.h"

#include "FairRootManager.h"

#include "CbmAnaTreeContainer.h"

NicaCbmATMCEventInterface::NicaCbmATMCEventInterface()
  : fDataContainer(nullptr) {}

void NicaCbmATMCEventInterface::ConnectToTree() {
  FairRootManager* manager = FairRootManager::Instance();
  if (CanDeleteEvent()) { delete fDataContainer; }
  fDataContainer = (CbmAnaTreeMcSourceContainer*) manager->GetObject(
    "CbmAnaTreeMcSourceContainer.");
}

void NicaCbmATMCEventInterface::Register(Bool_t write) {
  FairRootManager* manager = FairRootManager::Instance();
  manager->Register("CbmAnaTreeMcSourceContainer.",
                    "CbmAnaTreeMcSourceContainer.",
                    fDataContainer,
                    write);
}

void NicaCbmATMCEventInterface::CopyData(NicaEventInterface* s) {
  NicaCbmATMCEventInterface* ev = (NicaCbmATMCEventInterface*) s;
  *fDataContainer = *ev->fDataContainer;  //TODO make assignment operator
}

void NicaCbmATMCEventInterface::Compress(Int_t* /*map*/, Int_t /*map_size*/) {
  //TODO
}

void NicaCbmATMCEventInterface::CopyAndCompress(NicaEventInterface* s,
                                                Int_t* /*map*/,
                                                Int_t /*map_size*/) {
  CopyData(s);
}

void NicaCbmATMCEventInterface::FillTrackInterface(
  NicaTrackInterface* /*track*/,
  Int_t /*index*/) {}

Int_t NicaCbmATMCEventInterface::GetTotalTrackNo() const {
  return fDataContainer->GetParticles()->GetNumberOfChannels();
}

TObject* NicaCbmATMCEventInterface::GetRawTrackPointer(Int_t /*index*/) const {
  return nullptr;
}

TLorentzVector NicaCbmATMCEventInterface::GetVertexError() const {
  return TLorentzVector(0, 0, 0, 0);
}

TLorentzVector NicaCbmATMCEventInterface::GetVertex() const {
  AnalysisTree::EventHeader* header = fDataContainer->GetEventHeader();
  return TLorentzVector(
    header->GetVertexX(), header->GetVertexY(), header->GetVertexZ(), 0);
}

NicaCbmATMCEventInterface::~NicaCbmATMCEventInterface() {
  if (CanDeleteEvent()) delete fDataContainer;
}
