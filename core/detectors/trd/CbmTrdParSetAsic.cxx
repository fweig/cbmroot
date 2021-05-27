/** @file CbmTrdParSetAsic.cxx
  * @copyright Copyright (C) 2018-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [orginator], Alexandru Bercuci **/

#include "CbmTrdParSetAsic.h"

#include "CbmTrdParAsic.h"    // for CbmTrdParAsic
#include "CbmTrdParFasp.h"    // for CbmTrdParFasp, NFASPCH, CbmTrdParFasp...
#include "CbmTrdParMod.h"     // for CbmTrdParMod
#include "CbmTrdParSpadic.h"  // for CbmTrdParSpadic, NSPADICCH

#include <FairParamList.h>  // for FairParamList
#include <Logger.h>         // for Logger, LOG

#include <TArrayI.h>            // for TArrayI
#include <TGenericClassInfo.h>  // for TGenericClassInfo
#include <TString.h>            // for Form

#include <utility>  // for pair

#include <stdint.h>  // for uint64_t
#include <stdio.h>   // for printf
#include <string.h>  // for strcmp

using std::map;
//_______________________________________________________________________________
CbmTrdParSetAsic::CbmTrdParSetAsic(const char* name, const char* title, const char* context)
  : CbmTrdParSet(name, title, context)
  , fType(8)
  , fModPar()
{
}

//_______________________________________________________________________________
CbmTrdParSetAsic::~CbmTrdParSetAsic()
{
  if (fNrOfModules) {
    //     std::map<Int_t, CbmTrdParSetAsic*>::iterator it=fModPar.begin();
    //     while(it!=fModPar.end()){
    //       if(it->second) delete it->second;
    //       it++;
    //     }
    fModPar.clear();
  }
}
//_______________________________________________________________________________
Bool_t CbmTrdParSetAsic::getParams(FairParamList* l)
{
  if (!l) return kFALSE;
  if (!l->fill("NrOfModules", &fNrOfModules)) return kFALSE;
  TArrayI moduleId(fNrOfModules);
  if (!l->fill("ModuleId", &moduleId)) return kFALSE;
  TArrayI nAsic(fNrOfModules);
  if (!l->fill("nAsic", &nAsic)) return kFALSE;
  TArrayI typeAsic(fNrOfModules);
  if (!l->fill("typeAsic", &typeAsic)) return kFALSE;

  Int_t maxNrAsics {0};
  for (Int_t imod = 0; imod < fNrOfModules; imod++) {
    if (nAsic[imod] > maxNrAsics) maxNrAsics = nAsic[imod];
  }
  Int_t address(0);

  CbmTrdParAsic* asic(nullptr);
  for (Int_t i = 0; i < fNrOfModules; i++) {
    fModPar[moduleId[i]] =
      new CbmTrdParSetAsic(GetName(), Form("%s for Module %d", GetTitle(), moduleId[i]) /*, GetContext()*/);
    // only for FASP
    if (9 == typeAsic[i]) {
      Int_t sizePerFasp = 1 + NFASPCH * 4;
      Int_t maxValues   = maxNrAsics * (sizePerFasp);
      TArrayI values(maxValues);
      if (!l->fill(Form("FaspInfo - Module %d", moduleId[i]), &values)) continue;
      for (Int_t iasic = 0; iasic < nAsic[i]; iasic++) {
        Int_t offset = iasic * (sizePerFasp);
        address      = values[offset + 0];
        asic         = new CbmTrdParFasp(address);
        static_cast<CbmTrdParFasp*>(asic)->LoadParams(values, iasic);
        fModPar[moduleId[i]]->SetAsicPar(address, asic);
      }
    }
    else {
      Int_t maxValues = maxNrAsics * (5 + NSPADICCH);
      TArrayI values(maxValues);

      if (!l->fill(Form("SpadicInfo - Module %d", moduleId[i]), &values)) continue;
      for (Int_t iasic = 0; iasic < nAsic[i]; iasic++) {
        Int_t offset = iasic * (5 + NSPADICCH);
        address      = values[offset + 0];
        asic         = new CbmTrdParSpadic(address);
        asic->SetComponentId(static_cast<CbmTrdParSpadic*>(asic)->CreateComponentId(
          values[offset + 1], values[offset + 2], values[offset + 3], values[offset + 4]));
        std::vector<Int_t> addresses {};
        for (Int_t j = offset + 5; j < offset + 5 + NSPADICCH; j++) {
          addresses.push_back(values[j]);
        }
        asic->SetChannelAddresses(addresses);
        fModPar[moduleId[i]]->SetAsicPar(address, asic);
      }
    }
  }
  return kTRUE;
}

//_______________________________________________________________________________
void CbmTrdParSetAsic::putParams(FairParamList* l)
{
  if (!l) return;
  LOG(info) << GetName() << "::putParams(FairParamList*)";

  Int_t idx(0);
  TArrayI moduleId(fNrOfModules), nAsic(fNrOfModules), typeAsic(fNrOfModules);
  for (std::map<Int_t, CbmTrdParSetAsic*>::iterator imod = fModPar.begin(); imod != fModPar.end(); imod++) {
    moduleId[idx]   = imod->first;
    nAsic[idx]      = imod->second->GetNofAsics();
    typeAsic[idx++] = imod->second->GetAsicType();
  }
  l->add("NrOfModules", fNrOfModules);
  l->add("ModuleId", moduleId);
  l->add("nAsic", nAsic);
  l->add("typeAsic", typeAsic);

  CbmTrdParSetAsic* mod(nullptr);
  for (Int_t i = 0; i < fNrOfModules; i++) {
    mod = (CbmTrdParSetAsic*) fModPar[moduleId[i]];

    Int_t iAsicNr(0);
    Int_t currentAsicAddress(-1);

    if (mod->fModuleMap.begin()->second->IsA() == CbmTrdParSpadic::Class()) {
      // Calculate the size of the array to hold all values realted to all
      // asics of a full detector module
      // each ASCIC has a maximum of NSPADICCH channels attached + 5 values
      // for AsicID, CriId, CrobId, CrobNumber and ElinkId
      Int_t sizePerSpadic = 5 + NSPADICCH;
      Int_t fullSize      = nAsic[i] * sizePerSpadic;
      TArrayI asicInfo(fullSize);
      iAsicNr = 0;
      for (auto iModuleIt : mod->fModuleMap) {
        std::uint64_t asicComponentId(100098);  // 100098 = undefined
        currentAsicAddress   = iModuleIt.first;
        asicComponentId      = ((CbmTrdParSpadic*) iModuleIt.second)->GetComponentId();
        int offset           = iAsicNr * sizePerSpadic;
        asicInfo[offset]     = currentAsicAddress;
        asicInfo[offset + 1] = CbmTrdParSpadic::GetCriId(asicComponentId);
        asicInfo[offset + 2] = CbmTrdParSpadic::GetCrobId(asicComponentId);
        asicInfo[offset + 3] = CbmTrdParSpadic::GetCrobNumber(asicComponentId);
        asicInfo[offset + 4] = CbmTrdParSpadic::GetElinkId(asicComponentId, 0);

        if ((((CbmTrdParSpadic*) iModuleIt.second)->GetNchannels()) != NSPADICCH) {
          LOG(fatal) << "Number of channels found " << ((CbmTrdParSpadic*) iModuleIt.second)->GetNchannels()
                     << " is differnt from the expected " << NSPADICCH;
        }

        Int_t iAsicChannel(0);
        for (auto channelAddressIt : ((CbmTrdParSpadic*) iModuleIt.second)->GetChannelAddresses()) {
          asicInfo[offset + 5 + iAsicChannel] = channelAddressIt;
          iAsicChannel++;
        }
        iAsicNr++;
      }
      l->add(Form("SpadicInfo - Module %d", moduleId[i]), asicInfo);
    }
    if (mod->fModuleMap.begin()->second->IsA() == CbmTrdParFasp::Class()) {

      Int_t sizePerFasp = 1 + NFASPCH * 4;
      Int_t fullSize    = nAsic[i] * sizePerFasp;
      TArrayI asicInfo(fullSize);

      iAsicNr = 0;
      for (auto iModuleIt : mod->fModuleMap) {
        int offset       = iAsicNr * sizePerFasp;
        asicInfo[offset] = iModuleIt.first;
        Int_t nchannels(((CbmTrdParAsic*) iModuleIt.second)->GetNchannels());
        CbmTrdParFasp* fasp = (CbmTrdParFasp*) iModuleIt.second;

        for (Int_t ich(0); ich < nchannels; ich += 2) {
          for (Int_t ipair(0); ipair < 2; ipair++) {
            Int_t faspAddress                                  = fasp->GetChannelAddress(ich);
            const CbmTrdParFaspChannel* ch                     = fasp->GetChannel(faspAddress, ipair);
            asicInfo[offset + 1 + ich + ipair]                 = faspAddress;
            asicInfo[offset + 1 + (1 * NFASPCH) + ich + ipair] = ch->GetPileUpTime();
            asicInfo[offset + 1 + (2 * NFASPCH) + ich + ipair] = ch->GetThreshold();
            asicInfo[offset + 1 + (3 * NFASPCH) + ich + ipair] = ch->GetMinDelaySignal();
          }
        }
        iAsicNr++;
      }
      l->add(Form("FaspInfo - Module %d", moduleId[i]), asicInfo);
    }
  }
}

//_______________________________________________________________________________
void CbmTrdParSetAsic::AddParameters(CbmTrdParSetAsic* mod)
{
  //printf("CbmTrdParSetAsic::addParam() :\n");

  fModPar[mod->fModuleMap.begin()->first / 1000] = mod;
  fNrOfModules++;
}

//_______________________________________________________________________________
Int_t CbmTrdParSetAsic::GetAsicAddress(Int_t chAddress) const
{
  /** Query the ASICs in the module set for the specified read-out channel. 
 * Returns the id of the ASIC within the module or -1 if all returns false.   
 */
  std::map<Int_t, CbmTrdParMod*>::const_iterator it = fModuleMap.begin();
  CbmTrdParAsic* asic(nullptr);
  while (it != fModuleMap.end()) {
    asic = (CbmTrdParAsic*) it->second;
    if (asic->QueryChannel(chAddress) >= 0) return it->first;
    it++;
  }
  return -1;
}

//_______________________________________________________________________________
void CbmTrdParSetAsic::GetAsicAddresses(std::vector<Int_t>* a) const
{
  /** Query the ASICs in the module set for their addresses. 
 * Returns the list of these addresses in the vector prepared by the user   
 */
  std::map<Int_t, CbmTrdParMod*>::const_iterator it = fModuleMap.begin();
  CbmTrdParAsic* asic(nullptr);
  while (it != fModuleMap.end()) {
    asic = (CbmTrdParAsic*) it->second;
    a->push_back(asic->GetAddress());
    it++;
  }
}

//_______________________________________________________________________________
const CbmTrdParSet* CbmTrdParSetAsic::GetModuleSet(Int_t detId) const
{
  /** Access the list of ASICs operating on detector detId. Use the class in the sense of
 * container of all module wise containers.
 */
  map<Int_t, CbmTrdParSetAsic*>::const_iterator imod = fModPar.find(detId);
  if (imod == fModPar.end()) return nullptr;
  return imod->second;
}

//_______________________________________________________________________________
void CbmTrdParSetAsic::Print(Option_t* opt) const
{
  if (fModPar.size()) {
    printf(" %s Modules[%d]\n", GetName(), fNrOfModules);
    map<Int_t, CbmTrdParSetAsic*>::const_iterator imod = fModPar.begin();
    while (imod != fModPar.end()) {
      printf("  %d %s(%s)\n", imod->first, imod->second->GetName(), imod->second->GetTitle());
      imod->second->Print(opt);
      imod++;
    }
  }
  else if (fModuleMap.size()) {
    printf(" %s Asics[%d]\n", GetName(), GetNofAsics());
    if (strcmp(opt, "all") == 0) {
      map<Int_t, CbmTrdParMod*>::const_iterator iasic = fModuleMap.begin();
      while (iasic != fModuleMap.end()) {
        iasic->second->Print(opt);
        iasic++;
      }
    }
  }
}

//_______________________________________________________________________________
void CbmTrdParSetAsic::SetAsicPar(Int_t address, CbmTrdParAsic* p)
{
  std::map<Int_t, CbmTrdParMod*>::iterator it = fModuleMap.find(address);
  if (it != fModuleMap.end()) {
    LOG(warn) << GetName() << "::SetAsicPar : The ASIC @ " << address << " already initialized. Skip.";
    return;
  }
  fModuleMap[address] = p;
  fNrOfModules++;
}

ClassImp(CbmTrdParSetAsic)
