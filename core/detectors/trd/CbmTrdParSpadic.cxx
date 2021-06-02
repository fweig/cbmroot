/** @file CbmTrdParSpadic.cxx
  * @copyright Copyright (C) 2018-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer], Pascal Raisig **/

#include "CbmTrdParSpadic.h"

#include "CbmTrdDefs.h"  // for eCbmTrdModuleTypes, eCbmTrdModuleType...

#include <FairParamList.h>  // for FairParamList
#include <Logger.h>         // for Logger, LOG

#include <TArrayI.h>  // for TArrayI
#include <TString.h>  // for Form

#include <vector>  // for vector

Double_t CbmTrdParSpadic::fgSizeX = 6.0;
Double_t CbmTrdParSpadic::fgSizeY = 3.0;
Double_t CbmTrdParSpadic::fgSizeZ = 0.5;

//___________________________________________________________________
CbmTrdParSpadic::CbmTrdParSpadic(Int_t address, Int_t FebGrouping, Double_t x, Double_t y, Double_t z,
                                 std::uint64_t compId)
  : CbmTrdParAsic(address, FebGrouping, x, y, z, compId)
{
  fChannelAddresses.resize(NSPADICCH);
  FillAsicChannelToElinkMap(&fMapAsicChannelToElink);
}

// ---- LoadParams ----------------------------------------------------
void CbmTrdParSpadic::LoadParams(FairParamList* inList)
{

  // inList->print();

  Int_t criId(-1);
  Int_t crobId(-1);
  Int_t crobNr(-1);
  Int_t eLinkId(-1);
  TArrayI channelAddresses(NSPADICCH);

  Bool_t loadOk = kTRUE;

  loadOk &= inList->fill(Form("%d-criId", fAddress), &criId);
  loadOk &= inList->fill(Form("%d-crobId", fAddress), &crobId);
  loadOk &= inList->fill(Form("%d-crobNr", fAddress), &crobNr);
  loadOk &= inList->fill(Form("%d-eLinkId", fAddress), &eLinkId);
  loadOk &= inList->fill(Form("%d-eLinkId", fAddress), &eLinkId);
  loadOk &= inList->fill(Form("%d-channelAddresses", fAddress), &channelAddresses);
  Int_t iAsicChannel(0);
  for (auto& channelIt : fChannelAddresses) {
    channelIt = channelAddresses[iAsicChannel];
    iAsicChannel++;
  }
  loadOk &= !fChannelAddresses.empty();

  if (!loadOk) {
    LOG(error) << GetName() << Form("Params could not be correctly loaded for asic %d", fAddress);
    fComponentId = 100098;
  }
  else {
    fComponentId = CreateComponentId(criId, crobId, crobNr, eLinkId);
    LOG(debug4) << GetName() << "Params correctly loaded for asic " << fAddress << " with componentId " << fComponentId;
  }
}

// ---- CreateComponentId ---------------------------------------------
std::uint64_t CbmTrdParSpadic::CreateComponentId(Int_t criId, Int_t crobId, Int_t nThCrobOnModule, Int_t eLinkId)
{
  std::uint64_t compId = criId * ECbmTrdComponentIdDecoding::kCriIdPosition
                         + crobId * ECbmTrdComponentIdDecoding::kCrobIdPosition
                         + nThCrobOnModule * ECbmTrdComponentIdDecoding::kCrobNrPosition + eLinkId;
  return compId;
}

// ---- GetCriId ----------------------------------------------------
std::uint16_t CbmTrdParSpadic::GetCriId() { return GetCriId(fComponentId); }

// ---- GetCriId ----------------------------------------------------
std::uint16_t CbmTrdParSpadic::GetCriId(std::uint64_t componentId)
{
  uint16_t returnId = (componentId / (ECbmTrdComponentIdDecoding::kCriIdPosition));
  return returnId;
}

// ---- GetCrobId ----------------------------------------------------
std::uint8_t CbmTrdParSpadic::GetCrobId() { return GetCrobId(fComponentId); }

// ---- GetCrobId ----------------------------------------------------
std::uint8_t CbmTrdParSpadic::GetCrobId(std::uint64_t componentId)
{
  std::uint8_t returnId(-1);
  returnId = ((componentId % ECbmTrdComponentIdDecoding::kCriIdPosition) / ECbmTrdComponentIdDecoding::kCrobIdPosition);
  return returnId;
}

// ---- GetCrobNumber ----------------------------------------------------
std::uint8_t CbmTrdParSpadic::GetCrobNumber() { return GetCrobNumber(fComponentId); }

// ---- GetCrobNumber ----------------------------------------------------
std::uint8_t CbmTrdParSpadic::GetCrobNumber(std::uint64_t componentId)
{
  std::uint8_t returnId(-1);
  returnId = (componentId % ECbmTrdComponentIdDecoding::kCrobIdPosition / ECbmTrdComponentIdDecoding::kCrobNrPosition);
  return returnId;
}

// ---- GetElinkId ----------------------------------------------------
std::uint8_t CbmTrdParSpadic::GetElinkId(Int_t channelId) { return GetElinkId(fComponentId, channelId); }

// ---- GetElinkId ----------------------------------------------------
std::uint8_t CbmTrdParSpadic::GetElinkId(std::uint64_t componentId, Int_t channelId)
{
  std::uint8_t eLinkId(-1);

  if (channelId > (NSPADICCH - 1))  // check for maximum number of spadic channels
  {
    LOG(error) << Form("CbmTrdParSpadic::GetElinkId(%d) - Incorrect channelId "
                       "(out of range %d > %d",
                       channelId, channelId, (NSPADICCH));
    return eLinkId;
  }

  // eLinkId = channelId < 15 ? ((((componentId % ECbmTrdComponentIdDecoding::kCriIdPosition)
  //                                            % ECbmTrdComponentIdDecoding::kCrobIdPosition))
  //                                            % ECbmTrdComponentIdDecoding::kCrobNrPosition)
  //                                            :
  //                                            ((((componentId % ECbmTrdComponentIdDecoding::kCriIdPosition)
  //                                            % ECbmTrdComponentIdDecoding::kCrobIdPosition))
  //                                            % ECbmTrdComponentIdDecoding::kCrobNrPosition) + 1;
  eLinkId = channelId < 15 ? (componentId % ECbmTrdComponentIdDecoding::kCrobNrPosition)
                           : (componentId % ECbmTrdComponentIdDecoding::kCrobNrPosition) + 1;
  return eLinkId;
}

// ---- GetNasicsOnModule ----------------------------------------------------
Int_t CbmTrdParSpadic::GetNasicsOnModule(Int_t moduleType)
{
  switch (moduleType) {
    case (Int_t) eCbmTrdModuleTypes::kHighChDensitySmallR: return 80; break;
    case (Int_t) eCbmTrdModuleTypes::kLowChDensitySmallR: return 20; break;
    case (Int_t) eCbmTrdModuleTypes::kHighChDensityLargeR: return 108; break;
    case (Int_t) eCbmTrdModuleTypes::kLowChDensityLargeR: return 36; break;
    case (Int_t) eCbmTrdModuleTypes::kMcbmModule:
      return 24;  // 24 is the maximum on a kMcbmModule it can also be less
      break;
    default: return 1; break;
  }
}

// ---- GetNasicsPerCrob ----------------------------------------------------
Int_t CbmTrdParSpadic::GetNasicsPerCrob(Int_t moduleType)
{
  Int_t nAsicsPerCrob = GetNasicsOnModule(moduleType);
  switch (moduleType) {
    case (Int_t) eCbmTrdModuleTypes::kHighChDensitySmallR: nAsicsPerCrob /= 4; break;
    case (Int_t) eCbmTrdModuleTypes::kLowChDensitySmallR: nAsicsPerCrob /= 1; break;
    case (Int_t) eCbmTrdModuleTypes::kHighChDensityLargeR: nAsicsPerCrob /= 6; break;
    case (Int_t) eCbmTrdModuleTypes::kLowChDensityLargeR: nAsicsPerCrob /= 2; break;
    case (Int_t) eCbmTrdModuleTypes::kMcbmModule:
      nAsicsPerCrob /= 1;  // 24 is the maximum on a kMcbmModule it can also be less
      break;
    default: nAsicsPerCrob /= -1; break;
  }
  return nAsicsPerCrob;
}

// ---- GetAsicChAddress ----
Int_t CbmTrdParSpadic::GetAsicChAddress(const Int_t asicChannel)
{
  ///< Returns the nth asic Channel in asic coordinates in single asic padplane coordinates. Spadic channels are not mapped from 00 to 31 in padplane coordinates, this function returns the padplane channelnumber in the system of one asic(not in the channel map of a full module !)

  Int_t address = -1;
  // Channel mapping based on channels 0-15 on the odd eLink and 16-31 on the even eLink, check setting in the unpacker for your dataset

  address = fVecSpadicChannels.at(asicChannel);
  return address;
}

// ---- FillAsicChannelToElinkMap ----
void CbmTrdParSpadic::FillAsicChannelToElinkMap(std::map<UInt_t, UInt_t>* map)
{

  // Only emplace pairs in an empty map.
  if (map->size() > 0) return;

  // This assumes that we have 2 elinks per SPADIC and that positions 00..15 in fVecSpadicChannels corresponds to the first elink while 16..31 corresponds to the second
  UInt_t nthAsicElink = 0;
  UInt_t rawchannel   = 0;
  for (size_t ichannel = 0; ichannel < fVecSpadicChannels.size(); ichannel++) {
    rawchannel = fVecSpadicChannels[ichannel];
    if (rawchannel >= NSPADICCH / 2) nthAsicElink = 1;
    else
      nthAsicElink = 0;
    auto channelpair = std::pair<UInt_t, UInt_t>(ichannel, nthAsicElink);
    map->emplace(channelpair);
  }
}

// ---- GetElinkNr ----
UInt_t CbmTrdParSpadic::GetElinkNr(Int_t moduleChannel, UInt_t nChannelsPerRow)
{
  ///< Return the number of the elink (counting started in channel order from bottom left to right) correlated to module wide channel number passed as argument, e.g. 000...767 for the mcbm module type
  UInt_t row           = moduleChannel / nChannelsPerRow;
  UInt_t column        = moduleChannel % nChannelsPerRow;
  UInt_t nelinksPerRow = nChannelsPerRow / (NSPADICCH / 2);

  // each SPADIC has its channels distributed over 2 rows. Hence, we have (nspadics per row) * 2 elinks per row
  // row 0 and 1, 2 and 3, ... use the same elinks
  UInt_t nthelink = (column / (NSPADICCH / 2)) * 2 + (row / 2) * nelinksPerRow;

  // up to now we do not know if we have the first or second elink on the asic. Hence, we get the channel number in the asic coordinates and check the asicChannel to elink map, wether it is on the first (0) or second (1) elink. And add the result.
  UInt_t asicRow     = row % 2;
  UInt_t asicChannel = column % (NSPADICCH / 2) + (NSPADICCH / 2) * asicRow;
  nthelink += fMapAsicChannelToElink.find(asicChannel)->second;

  return nthelink;
}

ClassImp(CbmTrdParSpadic)
