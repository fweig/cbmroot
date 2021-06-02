/** @file CbmTrdGeoHandler.cxx
  * @copyright Copyright (C) 2010-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer], David Emschermann, Andrey Lebedev **/

/**
 * \file CbmTrdGeoHandler.cxx
 * \author Florian Uhlig <f.uhlig@gsi.de>
 */
#include "CbmTrdGeoHandler.h"

#include "CbmTrdAddress.h"  // for CbmTrdAddress

#include <Logger.h>  // for LOG, Logger

#include <TGenericClassInfo.h>  // for TGenericClassInfo
#include <TGeoBBox.h>           // for TGeoBBox
#include <TGeoManager.h>        // for TGeoManager, gGeoManager
#include <TGeoNode.h>           // for TGeoNode
#include <TGeoPhysicalNode.h>   // for TGeoPhysicalNode
#include <TGeoVolume.h>         // for TGeoVolume
#include <TObjArray.h>          // for TObjArray
#include <TObject.h>            // for TObject
#include <TVirtualMC.h>         // for TVirtualMC, gMC

#include <cstdlib>   // for atoi
#include <iostream>  // for string

using std::atoi;
using std::string;

CbmTrdGeoHandler::CbmTrdGeoHandler()
  : TObject()
  , fIsSimulation(kFALSE)
  , fGeoPathHash(0)
  , fCurrentVolume(nullptr)
  , fVolumeShape(nullptr)
  , fGlobal()
  , fGlobalMatrix(nullptr)
  , fLayerId(0)
  , fModuleId(0)
  , fModuleType(0)
  , fRotation(0)
  , fStation(0)
  , fLayer(0)
  , fModuleCopy(0)
{
}

CbmTrdGeoHandler::~CbmTrdGeoHandler() {}

void CbmTrdGeoHandler::Init(Bool_t isSimulation) { fIsSimulation = isSimulation; }

Int_t CbmTrdGeoHandler::GetModuleAddress()
{

  // In the simulation we can not rely on the TGeoManager information
  // In the simulation we get the correct information only from gMC
  Int_t copyNr;
  if (fIsSimulation) {
    LOG(debug4) << gMC->CurrentVolPath();
    // get the copy number of the mother volume (1 volume up in hierarchy)
    gMC->CurrentVolOffID(1, copyNr);
  }
  else {
    LOG(debug4) << gGeoManager->GetPath();
    // We take the mother node (module) of the current node we are in (gas).
    TGeoNode* node = gGeoManager->GetMother();
    // Get the module copy number to get the information about layerId and moduleId.
    copyNr = node->GetNumber();
  }
  LOG(debug4) << "CopyNr: " << copyNr;

  Int_t layerId  = 0;
  Int_t moduleId = 0;

  if ((copyNr / 100000000) > 0)  // has 9 digits => 2014 format
  {
    // In TGeoManager numbering starts with 1, so we have to subtract 1.
    layerId  = ((copyNr / 1000) % 100) - 1;
    moduleId = (copyNr % 1000) - 1;
    LOG(debug4) << "2014 ";
  }
  else  // 2013 and earlier
  {
    // In TGeoManager numbering starts with 1, so we have to subtract 1.
    layerId  = ((copyNr / 100) % 100) - 1;
    moduleId = (copyNr % 100) - 1;
    LOG(debug4) << "2013 ";
  }

  LOG(debug4) << copyNr / 100000000 << " copy " << copyNr << " layerID " << layerId << " moduleId " << moduleId;
  // Form the module address.
  return CbmTrdAddress::GetAddress(layerId, moduleId, 0, 0, 0);
}

Int_t CbmTrdGeoHandler::GetModuleAddress(const TString& path)
{
  if (fGeoPathHash != path.Hash()) { NavigateTo(path); }
  return GetModuleAddress();
}

//Int_t CbmTrdGeoHandler::GetModuleOrientation()
//{
//  // We take the mother node (module) of the current node we are in (gas).
//  TGeoNode* modulenode = gGeoManager->GetMother();
//  // Get the module copy number to get the information about layerId and moduleId.
//  Int_t modulecopyNr = modulenode->GetNumber();
//  // In TGeoManager numbering starts with 1, so we have to subtract 1.
//  fRotation  = ((modulecopyNr /  10000) %  10);  // from module copy number
//  //  LOG(info) << "fRotation: " << modulecopyNr << " " << fRotation;
//  return fRotation;
//}

Int_t CbmTrdGeoHandler::GetModuleOrientation(const TString& path)
{
  if (fGeoPathHash != path.Hash()) { NavigateTo(path); }
  return fRotation;
}

Double_t CbmTrdGeoHandler::GetSizeX(const TString& path)
{
  if (fGeoPathHash != path.Hash()) { NavigateTo(path); }
  return fVolumeShape->GetDX();
}

Double_t CbmTrdGeoHandler::GetSizeY(const TString& path)
{
  if (fGeoPathHash != path.Hash()) { NavigateTo(path); }
  return fVolumeShape->GetDY();
}

Double_t CbmTrdGeoHandler::GetSizeZ(const TString& path)
{
  if (fGeoPathHash != path.Hash()) { NavigateTo(path); }
  return fVolumeShape->GetDZ();
}

Double_t CbmTrdGeoHandler::GetZ(const TString& path)
{
  if (fGeoPathHash != path.Hash()) { NavigateTo(path); }
  return fGlobal[2];
}

Double_t CbmTrdGeoHandler::GetY(const TString& path)
{
  if (fGeoPathHash != path.Hash()) { NavigateTo(path); }
  return fGlobal[1];
}

Double_t CbmTrdGeoHandler::GetX(const TString& path)
{
  if (fGeoPathHash != path.Hash()) { NavigateTo(path); }
  return fGlobal[0];
}

Int_t CbmTrdGeoHandler::GetModuleType(const TString& path)
{
  if (fGeoPathHash != path.Hash()) { NavigateTo(path); }
  return fModuleType;
}

Int_t CbmTrdGeoHandler::GetStation(const TString& path)
{
  if (fGeoPathHash != path.Hash()) { NavigateTo(path); }
  return fStation;
}

Int_t CbmTrdGeoHandler::GetLayer(const TString& path)
{
  if (fGeoPathHash != path.Hash()) {
    //      LOG(info) << "path : " << path.Data();
    NavigateTo(path);
  }
  return fLayer;
}

Int_t CbmTrdGeoHandler::GetModuleCopyNr(const TString& path)
{
  if (fGeoPathHash != path.Hash()) { NavigateTo(path); }
  return fModuleCopy;
}

void CbmTrdGeoHandler::NavigateTo(const TString& path)
{
  //   LOG(info)<< "path : " << path.Data();
  if (fIsSimulation) { LOG(fatal) << "This method is not supported in simulation mode"; }
  else {
    gGeoManager->cd(path.Data());
    fGeoPathHash      = path.Hash();
    fCurrentVolume    = gGeoManager->GetCurrentVolume();
    fVolumeShape      = (TGeoBBox*) fCurrentVolume->GetShape();
    Double_t local[3] = {0., 0., 0.};  // Local center of volume
    gGeoManager->LocalToMaster(local, fGlobal);
    fGlobalMatrix = gGeoManager->GetCurrentMatrix();
    // Get module type information which is decoded in copy number.
    const char* moduleName = gGeoManager->GetMother()->GetName();
    fModuleType            = std::atoi(string(1, *(moduleName + 6)).c_str());  // 6th element module type

    // We take the mother of the mother node (layer) of the current node we are in (gas).
    TGeoNode* layernode = gGeoManager->GetMother(2);  // get layer
    Int_t layercopyNr   = layernode->GetNumber();
    // fIsRotated is the 4th digit from the back
    fStation = ((layercopyNr / 10000) % 10);  // from layer copy number
    //      LOG(info) << "DE fStation: " << fStation << " ";
    fLayer = ((layercopyNr / 100) % 10);  // from layer copy number

    // We take the mother node (module) of the current node we are in (gas).
    TGeoNode* modulenode = gGeoManager->GetMother();
    // Get the module copy number to get the information about layerId and moduleId.
    Int_t modulecopyNr = modulenode->GetNumber();

    if ((modulecopyNr / 100000000) > 0)  // has 9 digits => 2014 format
    {
      // In TGeoManager numbering starts with 1, so we have to subtract 1.
      fModuleCopy = ((modulecopyNr / 1000000) % 100);  // from module copy number
      fRotation   = ((modulecopyNr / 100000) % 10);    // from module copy number
    }
    else  // 2013 and earlier
    {
      // In TGeoManager numbering starts with 1, so we have to subtract 1.
      fModuleCopy = ((modulecopyNr / 100000) % 100);  // from module copy number
      fRotation   = ((modulecopyNr / 10000) % 10);    // from module copy number
    }

    //      LOG(info) << "fRotation: " << modulecopyNr << " " << fRotation;
    //      fLayerId   = ((modulecopyNr /    100) % 100) - 1;
    //      fModuleId  = ((modulecopyNr /      1) % 100) - 1;
  }
}

std::map<Int_t, TGeoPhysicalNode*> CbmTrdGeoHandler::FillModuleMap()
{
  // The geometry structure is treelike with cave as
  // the top node. For the TRD there are keeping volumes with names
  // like trd_vXXy_1 which are only a container for the different layers.
  // The trd layer is again only a container for all volumes of this layer.
  // Loop over all nodes below the top node (cave). If one of
  // the nodes contains a string trd it must be TRD detector.
  // Now loop over the layers and then over all modules of the layer
  // to extract the node information for each detector module
  // all active regions (gas) of the complete TRD.

  std::map<Int_t, TGeoPhysicalNode*> moduleMap;

  TGeoNode* topNode = gGeoManager->GetTopNode();
  TObjArray* nodes  = topNode->GetNodes();
  for (Int_t iNode = 0; iNode < nodes->GetEntriesFast(); iNode++) {
    TGeoNode* node = static_cast<TGeoNode*>(nodes->At(iNode));
    if (!TString(node->GetName()).Contains("trd", TString::kIgnoreCase))
      continue;  // trd_vXXy top node, e.g. trd_v13a, trd_v14b
    TGeoNode* station = node;

    TObjArray* layers = station->GetNodes();
    for (Int_t iLayer = 0; iLayer < layers->GetEntriesFast(); iLayer++) {
      TGeoNode* layer = static_cast<TGeoNode*>(layers->At(iLayer));
      if (!TString(layer->GetName()).Contains("layer", TString::kIgnoreCase)) continue;  // only layers

      TObjArray* modules = layer->GetNodes();
      for (Int_t iModule = 0; iModule < modules->GetEntriesFast(); iModule++) {
        TGeoNode* module = static_cast<TGeoNode*>(modules->At(iModule));
        TObjArray* parts = module->GetNodes();
        for (Int_t iPart = 0; iPart < parts->GetEntriesFast(); iPart++) {
          TGeoNode* part = static_cast<TGeoNode*>(parts->At(iPart));
          if (!TString(part->GetName()).Contains("gas", TString::kIgnoreCase)) continue;  // only active gas volume

          // Put together the full path to the interesting volume, which
          // is needed to navigate with the geomanager to this volume.
          // Extract the geometry information (size, global position)
          // from this volume.
          TString path = TString("/") + topNode->GetName() + "/" + station->GetName() + "/" + layer->GetName() + "/"
                         + module->GetName() + "/" + part->GetName();

          LOG(debug) << "Adding detector with path " << path;
          // Generate a physical node which has all needed information
          TGeoPhysicalNode* pNode = new TGeoPhysicalNode(path.Data());
          Int_t address           = GetModuleAddress();
          moduleMap[address]      = pNode;
        }
      }
    }
  }
  return moduleMap;
}


ClassImp(CbmTrdGeoHandler)
