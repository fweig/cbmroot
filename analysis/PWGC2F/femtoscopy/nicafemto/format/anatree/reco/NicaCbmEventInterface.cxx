/*
 * NicaCbmEventInterface.cxx
 *
 *  Created on: 3 gru 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "NicaCbmEventInterface.h"

#include "CbmAnaTreeSource.h"
#include "CbmGlobalTrack.h"
#include "CbmStsTrack.h"
#include "CbmTofTrack.h"

#include <FairRootManager.h>

#include <RtypesCore.h>
#include <TObjArray.h>

#include <stddef.h>

NicaCbmEventInterface::NicaCbmEventInterface() : fDataContainer(nullptr) {}

void NicaCbmEventInterface::ConnectToTree()
{
  FairRootManager* manager = FairRootManager::Instance();
  if (CanDeleteEvent()) { delete fDataContainer; }

  fDataContainer = (CbmAnaTreeRecoSourceContainer*) manager->GetObject("CbmAnaTreeSourceContainer.");
}

void NicaCbmEventInterface::Register(Bool_t write)
{
  FairRootManager* manager = FairRootManager::Instance();
  manager->Register("CbmAnaTreeSourceContainer.", "CbmAnaTreeSourceContainer.", fDataContainer, write);
}

void NicaCbmEventInterface::CopyData(NicaEventInterface* s)
{
  NicaCbmEventInterface* ev = (NicaCbmEventInterface*) s;
  *fDataContainer           = *ev->fDataContainer;  //TODO make assignment operator
}

void NicaCbmEventInterface::Compress(Int_t* /*map*/, Int_t /*map_size*/)
{
  //NOT AVAIABLE IN ANATREE
}

void NicaCbmEventInterface::CopyAndCompress(NicaEventInterface* s, Int_t* /*map*/, Int_t /*map_size*/)
{
  NicaCbmEventInterface* ev = (NicaCbmEventInterface*) s;
  *fDataContainer           = *ev->fDataContainer;
}

void NicaCbmEventInterface::FillTrackInterface(NicaTrackInterface* /*track*/, Int_t /*index*/)
{
  // OPS NO POINTER TO TOBJECT !
}

Int_t NicaCbmEventInterface::GetTotalTrackNo() const { return fDataContainer->GetVtxTracks()->GetNumberOfChannels(); }

TObject* NicaCbmEventInterface::GetRawTrackPointer(Int_t /*index*/) const { return nullptr; }

TLorentzVector NicaCbmEventInterface::GetVertexError() const { return TLorentzVector(0, 0, 0, 0); }

TLorentzVector NicaCbmEventInterface::GetVertex() const
{
  AnalysisTree::EventHeader* header = fDataContainer->GetEventHeader();
  return TLorentzVector(header->GetVertexX(), header->GetVertexY(), header->GetVertexZ(), 0);
}

NicaCbmEventInterface::~NicaCbmEventInterface()
{
  if (CanDeleteEvent()) { delete fDataContainer; }
}
