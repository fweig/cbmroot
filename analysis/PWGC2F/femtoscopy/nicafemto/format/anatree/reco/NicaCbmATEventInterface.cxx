/*
 * NicaCbmEventInterface.cxx
 *
 *  Created on: 3 gru 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "NicaCbmATEventInterface.h"

#include <FairRootManager.h>
#include <RtypesCore.h>
#include <TObjArray.h>
#include <stddef.h>

#include "CbmAnaTreeSource.h"
#include "CbmGlobalTrack.h"
#include "CbmStsTrack.h"
#include "CbmTofTrack.h"

NicaCbmATEventInterface::NicaCbmATEventInterface() : fDataContainer(nullptr) {}

void NicaCbmATEventInterface::ConnectToTree() {
  FairRootManager* manager = FairRootManager::Instance();
  if (CanDeleteEvent()) { delete fDataContainer; }

  fDataContainer = (CbmAnaTreeRecoSourceContainer*) manager->GetObject(
    "CbmAnaTreeSourceContainer.");
}

void NicaCbmATEventInterface::Register(Bool_t write) {
  FairRootManager* manager = FairRootManager::Instance();
  manager->Register("CbmAnaTreeSourceContainer.",
                    "CbmAnaTreeSourceContainer.",
                    fDataContainer,
                    write);
}

void NicaCbmATEventInterface::CopyData(NicaEventInterface* s) {
  NicaCbmATEventInterface* ev = (NicaCbmATEventInterface*) s;
  *fDataContainer = *ev->fDataContainer;  //TODO make assignment operator
}

void NicaCbmATEventInterface::Compress(Int_t* /*map*/, Int_t /*map_size*/) {
  //NOT AVAIABLE IN ANATREE
}

void NicaCbmATEventInterface::CopyAndCompress(NicaEventInterface* s,
                                              Int_t* /*map*/,
                                              Int_t /*map_size*/) {
  NicaCbmATEventInterface* ev = (NicaCbmATEventInterface*) s;
  *fDataContainer             = *ev->fDataContainer;
}

void NicaCbmATEventInterface::FillTrackInterface(NicaTrackInterface* /*track*/,
                                                 Int_t /*index*/) {
  // OPS NO POINTER TO TOBJECT !
}

Int_t NicaCbmATEventInterface::GetTotalTrackNo() const {
  return fDataContainer->GetVtxTracks()->GetNumberOfChannels();
}

TObject* NicaCbmATEventInterface::GetRawTrackPointer(Int_t /*index*/) const {
  return nullptr;
}

TLorentzVector NicaCbmATEventInterface::GetVertexError() const {
  return TLorentzVector(0, 0, 0, 0);
}

TLorentzVector NicaCbmATEventInterface::GetVertex() const {
  AnalysisTree::EventHeader* header = fDataContainer->GetEventHeader();
  return TLorentzVector(
    header->GetVertexX(), header->GetVertexY(), header->GetVertexZ(), 0);
}

NicaCbmATEventInterface::~NicaCbmATEventInterface() {
  if (CanDeleteEvent()) { delete fDataContainer; }
}
