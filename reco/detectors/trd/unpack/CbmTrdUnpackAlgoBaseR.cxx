/* Copyright (C) 2021 Goethe-University Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig [committer] */

#include "CbmTrdUnpackAlgoBaseR.h"

#include "CbmMcbm2020TrdTshiftPar.h"
#include "CbmTrdDigi.h"
#include "CbmTrdHardwareSetupR.h"  // for channel address parameters
#include "CbmTrdParSetAsic.h"

#include <FairParGenericSet.h>
#include <FairTask.h>
#include <Logger.h>

#include <Rtypes.h>
#include <RtypesCore.h>


CbmTrdUnpackAlgoBaseR::CbmTrdUnpackAlgoBaseR(std::string name) : CbmRecoUnpackAlgo(name) {}

CbmTrdUnpackAlgoBaseR::~CbmTrdUnpackAlgoBaseR() {}

// ---- digestOutput ----
void CbmTrdUnpackAlgoBaseR::digestOutput(std::unique_ptr<CbmTrdDigi> digi, CbmTrdRawMessageSpadic raw)
{
  // If requested lets monitor something
  if (fMonitor) { fMonitor->FillHistos(digi.get(), &raw); }

  // Save raw message:
  if (fOptOutAVec) { fOptOutAVec->emplace_back(raw); }


  // Save the digi
  fOutputVec.emplace_back(*std::move(digi));
}

// ---- getAsicAddress ----
std::uint32_t CbmTrdUnpackAlgoBaseR::getAsicAddress(std::uint32_t criid, std::uint32_t crobid, std::uint32_t elinkid)
{
  size_t spadicHwAddress = 0;
  spadicHwAddress = elinkid + (CbmTrdParAsic::kCriIdPosition * criid) + (CbmTrdParAsic::kCrobIdPosition * crobid);
  auto mapIt      = fSpadicAddressMap.find(spadicHwAddress);  // check if asic exists
  if (mapIt == fSpadicAddressMap.end()) {
    LOG(info) << fName
              << "::makeDigi - No asic address "
                 "found for Spadic hardware address "
              << spadicHwAddress;
    return 0;
  }

  return mapIt->second;
}

// ---- getAsicAddress ----
std::uint32_t CbmTrdUnpackAlgoBaseR::getChannelId(std::uint32_t asicaddress, std::uint32_t elinkid,
                                                  std::uint32_t elinkchannelid)
{
  // GetChannelId per eLink add NSPADICCH / 2 to the second(first) eLink in the case we start with odd(even) eLinks, since, our mapping is based on odd eLinks
  auto asicChannelId =
    (elinkid % 2) == fIsFirstChannelsElinkEven ? elinkchannelid : elinkchannelid + (CbmTrdSpadic::GetNrChannels() / 2);

  auto channelId = (fAsicChannelMap.find(asicaddress))->second.at(asicChannelId);

  return channelId;
}

// ---- initParSet(FairParGenericSet* parset) ----
Bool_t CbmTrdUnpackAlgoBaseR::initParSet(FairParGenericSet* parset)
{
  LOG(info) << fName << "::initParSet - for container " << parset->ClassName();
  if (parset->IsA() == CbmTrdParSetAsic::Class()) return initParSet(static_cast<CbmTrdParSetAsic*>(parset));

  if (parset->IsA() == CbmTrdParSetDigi::Class()) return initParSet(static_cast<CbmTrdParSetDigi*>(parset));

  if (parset->IsA() == CbmMcbm2020TrdTshiftPar::Class())
    return initParSet(static_cast<CbmMcbm2020TrdTshiftPar*>(parset));

  // If we do not know the derived ParSet class we return false
  LOG(error)
    << fName << "::initParSet - for container " << parset->ClassName()
    << " failed, since CbmTrdUnpackAlgoBaseR::initParSet() does not know the derived ParSet and what to do with it!";
  return kFALSE;
}

// ---- initParSet(CbmTrdParSetAsic* parset) ----
Bool_t CbmTrdUnpackAlgoBaseR::initParSet(CbmTrdParSetAsic* parset)
{
  LOG(debug) << fName << "::initParSetAsic - ";
  CbmTrdHardwareSetupR hwSetup;
  fSpadicAddressMap = hwSetup.CreateHwToSwAsicAddressTranslatorMap(parset);
  if (fSpadicAddressMap.empty()) {
    LOG(error) << fName << "::initParSetAsic - SpadicAddressMap creation failed, the map is empty";
    return kFALSE;
  }
  // at least check that the loaded map is not empty

  fAsicChannelMap = hwSetup.CreateAsicChannelMap(parset);
  if (fAsicChannelMap.empty()) {
    LOG(error) << fName << "::initParSetAsic - AsicChannelMap creation failed, the map is empty";
    return kFALSE;
  }


  LOG(info) << fName << "::initParSetAsic - Successfully initialized Spadic hardware address map";

  return kTRUE;
}

// ---- initParSet(CbmTrdParSetDigi* parset) ----
Bool_t CbmTrdUnpackAlgoBaseR::initParSet(CbmTrdParSetDigi* parset)
{
  Bool_t initOk = kTRUE;
  // The monitor needs the ParSetDigi to extract module informations and other stuff
  if (fMonitor) {
    LOG(info) << fName << "::initParSet(CbmTrdParSetDigi) - Forwarding ParSetDigi to the monitor";
    CbmTrdParSetAsic* asics(nullptr);
    for (auto pair : fParContVec) {
      if ((pair.second).get()->IsA() != CbmTrdParSetAsic::Class()) continue;
      asics = static_cast<CbmTrdParSetAsic*>((pair.second).get());
    }
    initOk &= fMonitor->Init(parset, asics);
  }

  return initOk;
}

// ---- initParSet(CbmMcbm2020TrdTshiftPar* parset) ----
Bool_t CbmTrdUnpackAlgoBaseR::initParSet(CbmMcbm2020TrdTshiftPar* parset)
{
  auto maptimeshifts = parset->GetTimeshiftsMap();
  fTimeshiftsMap.clear();
  fTimeshiftsMap.insert(maptimeshifts->begin(), maptimeshifts->end());
  LOG(info) << fName << "::initParSetTimeshift2020() - Parsing timeshift correction map to unpacker algo";

  return (!fTimeshiftsMap.empty());
}

// ---- initR ----
Bool_t CbmTrdUnpackAlgoBaseR::init()
{
  auto initOk = kTRUE;
  // Check if we have already a Spadic object. If not check if we have a raw to digi object, it has by default a Spadic object. If we do not have a rtd object we create a default spadic object on our own.
  if (!fSpadic) {
    // First we check if there is a spadic definition stored in the raw to digi method
    if (fRTDMethod) fSpadic = fRTDMethod->GetSpadicObject();
    // If we still do not have spadic, we create it on our own
    if (!fSpadic) fSpadic = std::make_shared<CbmTrdSpadic>();
  }
  if (!fSpadic) {
    LOG(error) << fName
               << "::initR - We are missing a CbmTrdSpadic object, to extract the basic spadic functionalities!";
    initOk &= kFALSE;
  }
  if (fMonitor) fMonitor->SetSpadicObject(fSpadic);

  return initOk;
}

ClassImp(CbmTrdUnpackAlgoBaseR)
