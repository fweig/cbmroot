/* Copyright (C) 2019-2020 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Adrian Amatus Weber [committer], Florian Uhlig */

/*
 * CbmRichMCbmDigiMapManager.cxx
 *
 *  Created on: Jul 11, 2019
 *      Author: aweber
 */
#include "CbmRichMCbmDigiMapManager.h"

#include "CbmRichDetectorData.h"  // for CbmRichPmtData, CbmRichPixelData

#include <Logger.h>  // for LOG, Logger

#include <TGeoBBox.h>     // for TGeoBBox
#include <TGeoManager.h>  // for TGeoManager, gGeoManager
#include <TGeoMatrix.h>   // for TGeoMatrix
#include <TGeoNode.h>     // for TGeoIterator, TGeoNode
#include <TGeoVolume.h>   // for TGeoVolume
#include <TRandom.h>      // for TRandom, gRandom
#include <TString.h>      // for TString

#include <iostream>  // for string, operator<<, basic_ostream
#include <string>    // for operator<, stoul
#include <utility>   // for pair

#include <stddef.h>  // for size_t

using namespace std;

CbmRichMCbmDigiMapManager::CbmRichMCbmDigiMapManager()
  : fPixelPathToAddressMap()
  , fPixelAddressToDataMap()
  , fPixelAddresses()
  , fPmtPathToIdMap()
  , fPmtIdToDataMap()
  , fPmtIds()
{
  Init();
}

CbmRichMCbmDigiMapManager::~CbmRichMCbmDigiMapManager() {}

void CbmRichMCbmDigiMapManager::Init()
{

  fPixelPathToAddressMap.clear();
  fPixelAddressToDataMap.clear();
  fPixelAddresses.clear();

  fPmtPathToIdMap.clear();
  fPmtIdToDataMap.clear();
  fPmtIds.clear();


  // Get MAPMT dimensions
  Double_t pmtHeight    = 5.2;
  Double_t pmtWidth     = 5.2;
  TGeoVolume* pmtVolume = gGeoManager->FindVolumeFast("pmt");                      // check for RICH
  if (pmtVolume == nullptr) pmtVolume = gGeoManager->FindVolumeFast("pmt_vol_0");  // check for mRICH  // or pmt_vol_1
  if (pmtVolume != nullptr) {
    const TGeoBBox* shape = (const TGeoBBox*) (pmtVolume->GetShape());
    if (shape != nullptr) {
      pmtHeight = 2. * shape->GetDY();
      pmtWidth  = 2. * shape->GetDX();
    }
  }

  TGeoIterator geoIterator(gGeoManager->GetTopNode()->GetVolume());
  geoIterator.SetTopName("/cave_1");
  TGeoNode* curNode;
  // PMT plane position\rotation
  TString pixelNameStr("pmt_pixel");
  geoIterator.Reset();
  while ((curNode = geoIterator())) {
    TString nodeName(curNode->GetName());
    TString nodePath;
    if (TString(curNode->GetVolume()->GetName()).Contains(pixelNameStr)) {
      geoIterator.GetPath(nodePath);
      const TGeoMatrix* curMatrix = geoIterator.GetCurrentMatrix();
      const Double_t* curNodeTr   = curMatrix->GetTranslation();
      string path                 = string(nodePath.Data());

      size_t pmtInd = path.find_last_of("/");
      if (string::npos == pmtInd) continue;
      string pmtPath = path.substr(0, pmtInd + 1);

      Int_t channel  = std::stoul(path.substr(pmtInd + 11));  // cut away "pmt_pixel_"
      Int_t posAtPmt = channel / 100;
      channel        = channel % 100;

      size_t pmtVolInd = path.rfind("/", pmtInd - 1);
      if (string::npos == pmtVolInd) continue;
      Int_t pmtPosBP = std::stoul(path.substr(pmtVolInd + 11,
                                              pmtInd - pmtVolInd - 11));  // cut away "/pmt_vol_*_" ; position on BP

      size_t bpInd = path.rfind("/", pmtVolInd - 1);
      if (string::npos == bpInd) continue;
      Int_t posBP = std::stoul(path.substr(bpInd + 14,
                                           pmtVolInd - bpInd - 14));  // cut away "/pmt_vol_*_" ; position on BP

      Int_t x = (posBP / 10) + ((((pmtPosBP - 1) / 3) + 1) % 2);
      Int_t y = (posBP % 10) + (2 - ((pmtPosBP - 1) % 3));

      Int_t DiRICH_Add = ((7 & 0xF) << 12) + ((x & 0xF) << 8) + ((y & 0xF) << 4) + (posAtPmt & 0xF);
      Int_t pixelUID   = ((DiRICH_Add << 16) | (channel & 0x00FF));
      Int_t pmtUID     = ((x & 0xF) << 4) + (y & 0xF);
      //std::cout<<"Addr: 0x"<< std::hex << DiRICH_Add << std::dec << "\t" << channel <<"\t"<< std::hex << pixelUID << std::dec << std::endl;

      fPixelPathToAddressMap.insert(pair<string, Int_t>(path, pixelUID));
      CbmRichPixelData* pixelData = new CbmRichPixelData();
      pixelData->fX               = curNodeTr[0];
      pixelData->fY               = curNodeTr[1];
      pixelData->fZ               = curNodeTr[2];
      pixelData->fAddress         = pixelUID;
      fPixelAddressToDataMap.insert(pair<Int_t, CbmRichPixelData*>(pixelData->fAddress, pixelData));
      fPixelAddresses.push_back(pixelUID);
      //currentPixelAddress++;

      if (fPmtPathToIdMap.count(pmtPath) == 0) {
        fPmtPathToIdMap.insert(pair<string, Int_t>(pmtPath, pmtUID));

        CbmRichPmtData* pmtData = new CbmRichPmtData();
        pmtData->fId            = pmtUID;
        pmtData->fPixelAddresses.push_back(pixelData->fAddress);
        pmtData->fHeight = pmtHeight;
        pmtData->fWidth  = pmtWidth;
        fPmtIdToDataMap.insert(pair<Int_t, CbmRichPmtData*>(pmtData->fId, pmtData));
        pixelData->fPmtId = pmtData->fId;

        fPmtIds.push_back(pmtData->fId);
      }
      else {
        //cout << "pmtPath old:" << pmtPath << endl;
        Int_t pmtId             = fPmtPathToIdMap[pmtPath];
        CbmRichPmtData* pmtData = fPmtIdToDataMap[pmtId];
        if (pmtData == nullptr || pmtId != pmtData->fId) {
          LOG(error) << "(pmtData == nullptr || pmtId != pmtData->fId) ";
        }
        pmtData->fPixelAddresses.push_back(pixelData->fAddress);
        pixelData->fPmtId = pmtData->fId;
        if (pmtData->fPixelAddresses.size() > 64) {
          LOG(info) << "size:" << pmtData->fPixelAddresses.size() << " pmtData->fId:" << pmtData->fId
                    << " pmtPath:" << pmtPath << endl
                    << " path:" << path;
        }
      }
    }
  }

  // calculate Pmt center as center of gravity of 64 pixels
  for (auto const& pmt : fPmtIdToDataMap) {
    //	    Int_t pmtId = pmt.first;
    CbmRichPmtData* pmtData = pmt.second;
    pmtData->fX             = 0.;
    pmtData->fY             = 0.;
    pmtData->fZ             = 0.;
    for (int pixelId : pmtData->fPixelAddresses) {
      CbmRichPixelData* pixelData = fPixelAddressToDataMap[pixelId];
      if (pixelData == nullptr) continue;
      pmtData->fX += pixelData->fX;
      pmtData->fY += pixelData->fY;
      pmtData->fZ += pixelData->fZ;
    }
    pmtData->fX /= pmtData->fPixelAddresses.size();
    pmtData->fY /= pmtData->fPixelAddresses.size();
    pmtData->fZ /= pmtData->fPixelAddresses.size();
  }

  LOG(info) << "CbmRichMCbmDigiMapManager is initialized";
  LOG(info) << "fPixelPathToAddressMap.size() = " << fPixelPathToAddressMap.size();
  LOG(info) << "fPixelAddressToDataMap.size() = " << fPixelAddressToDataMap.size();

  LOG(info) << "fPmtPathToIdMap.size() = " << fPmtPathToIdMap.size();
  LOG(info) << "fPmtIdToDataMap.size() = " << fPmtIdToDataMap.size();

  //    for (auto const& pmt : fPmtIdToDataMap) {
  //       // cout << pmt.first << endl;
  //        cout << pmt.second->ToString() << endl;
  //    }
}

Int_t CbmRichMCbmDigiMapManager::GetPixelAddressByPath(const string& path)
{
  std::map<string, Int_t>::iterator it;
  it = fPixelPathToAddressMap.find(path);
  if (it == fPixelPathToAddressMap.end()) return -1;
  return it->second;
}


CbmRichPixelData* CbmRichMCbmDigiMapManager::GetPixelDataByAddress(Int_t address)
{
  std::map<Int_t, CbmRichPixelData*>::iterator it;
  it = fPixelAddressToDataMap.find(address);
  if (it == fPixelAddressToDataMap.end()) return nullptr;
  return it->second;
}

Int_t CbmRichMCbmDigiMapManager::GetRandomPixelAddress()
{
  Int_t nofPixels = fPixelAddresses.size();
  Int_t index     = gRandom->Integer(nofPixels);
  return fPixelAddresses[index];
}

vector<Int_t> CbmRichMCbmDigiMapManager::GetPixelAddresses() { return fPixelAddresses; }


vector<Int_t> CbmRichMCbmDigiMapManager::GetPmtIds() { return fPmtIds; }


CbmRichPmtData* CbmRichMCbmDigiMapManager::GetPmtDataById(Int_t id)
{
  std::map<Int_t, CbmRichPmtData*>::iterator it;
  it = fPmtIdToDataMap.find(id);
  if (it == fPmtIdToDataMap.end()) return nullptr;
  return it->second;
}

vector<Int_t> CbmRichMCbmDigiMapManager::GetDirectNeighbourPixels(Int_t /*address*/)
{
  std::vector<Int_t> v;

  return v;
}

vector<Int_t> CbmRichMCbmDigiMapManager::GetDiagonalNeighbourPixels(Int_t /*address*/)
{
  std::vector<Int_t> v;

  return v;
}
