/* Copyright (C) 2017-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau, Ajit Kumar, Florian Uhlig [committer] */

#include "MuchReadoutConfig.h"

#include "CbmMuchAddress.h"

#include <Logger.h>

#include <bitset>

using namespace std;

namespace cbm::algo
{

  // -------------------------------------------------------------------------
  /*
void MuchReadoutConfig::putParams(FairParamList* l)
{
  if (!l) return;

  l->add("NrOfDpbs", numComp);
  l->add("DbpIdArray", eqId);
  l->add("CrobActiveFlag", fiCrobActiveFlag);
  l->add("NrOfFebsInGemA", fuFebsInGemA);
  l->add("nFebsIdsArrayA", fnFebsIdsArrayGemA);
  l->add("NrOfFebsInGemB", fuFebsInGemB);
  l->add("nFebsIdsArrayB", fnFebsIdsArrayGemB);
  l->add("NrOfFebsInRpc", fuFebsInRpc);
  l->add("nFebsIdsArrayRpc", fnFebsIdsArrayRpc);
  l->add("ChannelsToPadXA", fChannelsToPadXA);
  l->add("ChannelsToPadYA", fChannelsToPadYA);
  l->add("ChannelsToPadXB", fChannelsToPadXB);
  l->add("ChannelsToPadYB", fChannelsToPadYB);
  l->add("ChannelsToPadXRpc", fChannelsToPadXRpc);
  l->add("ChannelsToPadYRpc", fChannelsToPadYRpc);
  l->add("RealX", fRealX);
  l->add("PadSize", fRealPadSize);
}
*/

  // ---  Constructor  ------------------------------------------------------------------
  MuchReadoutConfig::MuchReadoutConfig() { Init(); }
  // ------------------------------------------------------------------------------------


  // ---   Destructor   -----------------------------------------------------------------
  MuchReadoutConfig::~MuchReadoutConfig() {}
  // ------------------------------------------------------------------------------------

  // ---   Equipment IDs   --------------------------------------------------------------
  std::vector<uint16_t> MuchReadoutConfig::GetEquipmentIds()
  {
    std::vector<uint16_t> result;
    for (auto& entry : fReadoutMap)
      result.push_back(entry.first);
    return result;
  }
  // ------------------------------------------------------------------------------------

  // ---   Number of elinks for a component / equipment   -------------------------------
  size_t MuchReadoutConfig::GetNumElinks(uint16_t equipmentId)
  {
    size_t result = 0;
    auto it       = fReadoutMap.find(equipmentId);
    if (it != fReadoutMap.end()) result = fReadoutMap[equipmentId].size();
    return result;
  }
  // ------------------------------------------------------------------------------------


  // ---  Mapping (equimentId, elink) -> address[channel]  ------------------------------
  std::vector<uint32_t> MuchReadoutConfig::Map(uint16_t equipmentId, uint16_t elinkId)
  {
    std::vector<uint32_t> result;
    auto equipIter = fReadoutMap.find(equipmentId);
    if (equipIter != fReadoutMap.end()) {
      if (elinkId < equipIter->second.size()) { result = equipIter->second.at(elinkId); }
    }
    return result;
  }
  // ------------------------------------------------------------------------------------


  void MuchReadoutConfig::Init()
  {
    // This here refers to the mCBM 2021 setup.
    // Taken from CbmMuchUnpackPar in combination with macro/beamtime/mcbm2021/mSts.par

    // Array to hold the unique IDs (equipment ID) for all MUCH DPBs
    uint16_t eqId[numComp] = {0x18e3, 0x18ef, 0x1861, 0x6601, 0x4f19, 0x5b7b};

    // Mapping of eLink to FEB number within CROB. If -1, elink not used.
    // This mapping is the same for each component.
    const int16_t elink2Feb[numElinksPerCrob] = {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4,
                                                 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 8, 8, 8, 8};

    // Constructing the map (equipmentId, eLink, channel) -> (MUCH address)
    uint16_t numElinksPerComp = numCrobPerComp * numElinksPerCrob;
    for (uint16_t comp = 0; comp < numComp; comp++) {
      uint16_t equipment = eqId[comp];
      fReadoutMap[equipment].resize(numElinksPerComp);
      for (uint16_t crob = 0; crob < numCrobPerComp; crob++) {
        for (uint16_t elink = 0; elink < numElinksPerCrob; elink++) {
          fReadoutMap[equipment][elink].resize(numChanPerAsic);
          for (uint16_t channel = 0; channel < numChanPerAsic; channel++) {


            fReadoutMap[equipment][elink][channel] = 0;
          }  //# channel
        }    //# elink
      }      //# CROB
    }        //# component
  }

  //To do address need to be checked carefully
  uint32_t MuchReadoutConfig::CreateMuchAddress(uint32_t dpbidx, int32_t iFebId, uint32_t usChan)
  {
    // For generating Station number (GEM1 station = 0, GEM2 station = 1 and RPC station = 2)
    /// Below FebID is according to FEB Position in Module GEM A or Module GEM B (Carefully write MUCH Par file)
    int32_t station = -1;
    int32_t layer   = -1;
    if (iFebId == -7)  //Pulser FEB
    {
      station = 6;  // for Pulser
      layer   = 0;  //
    }
    else if (dpbidx == 0 || dpbidx == 1 || dpbidx == 2)  //First 3 DPBs are for GEM-1
    {
      station = 0;  // for mCBM setup
      layer   = 0;  // Station 0 for GEM-A and station 1 for Module GEM-B
    }
    else if (dpbidx == 4 || dpbidx == 5)  //Last 2 DPBs are for GEM-2
    {
      station = 1;  // for mCBM setup  station
      layer   = 0;  // 0 for Module GEM-A and 1 for Module GEM-B
    }
    else if (dpbidx == 3) {
      station = 2;  // for mCBM setup only one station
      layer   = 0;  //
    }
    else {
      LOG(warning) << "Wrong DPB Id x " << dpbidx;
      return 0;
    }  //No address generated.

    //Common layer side module will be 0 only for mCBM 2022
    int32_t layerside = 0;  // 0 in mCBM
    int32_t sModule   = 0;  // 0 in mCBM

    int32_t sSector  = -9;  //channel values are from 0-96 therefore as per CbmMuchAddress it is sector
    int32_t sChannel = -9;  //sector values are from 0-22 therefore as per CbmMuchAddress it is channel

    // Channel flip in stsXYTER v2.1 : 0<->1, 2<->3, 3<->4 and so on...
    auto fiFlag = 1;  //! Switch to smx2.0/smx2.1 data-> fiFlag = 0 for 2.0 and fiFlag = 1 for 2.1
    if (fiFlag == 1) {
      if (usChan % 2 == 0) usChan = usChan + 1;
      else
        usChan = usChan - 1;
    }

    // PadX means CHANNEL for CbmMuchAddress
    // PadY means SECTOR for CbmMuchAddress
    // Due to two FLEX cable connected to single FEB; First Flex Connector number
    // 1 - 63 and second flex connector number 64 - 127
    //in few FEB positioned these flex connectors are flipped so below correction applied.
    if (station == 0 && fiFlag == 1 && layer == 0) {  // First layer (GEM1) has old readout PCB
      if (iFebId == 0 || iFebId == 1 || iFebId == 2 || iFebId == 3 || iFebId == 4 || iFebId == 8 || iFebId == 9
          || iFebId == 10 || iFebId == 11 || iFebId == 17) {
        sChannel = GetPadXA(iFebId, 127 - usChan);
        sSector  = GetPadYA(iFebId, 127 - usChan);
      }
      else {
        sChannel = GetPadXA(iFebId, usChan);
        sSector  = GetPadYA(iFebId, usChan);
      }
    }
    else if (station == 1 && fiFlag == 1 && layer == 0) {  // second layer (GEM2) has new readout PCB
      if (iFebId == 0 || iFebId == 1 || iFebId == 2 || iFebId == 3 || iFebId == 4 || iFebId == 8 || iFebId == 9
          || iFebId == 10 || iFebId == 11 || iFebId == 17) {
        sChannel = GetPadXB(iFebId, 127 - usChan);
        sSector  = GetPadYB(iFebId, 127 - usChan);
      }
      else {
        sChannel = GetPadXB(iFebId, usChan);
        sSector  = GetPadYB(iFebId, usChan);
      }
    }
    else if (station == 0 || station == 1) {  // Both layer with same type of PCB (Probably below is not necessary)
      if (iFebId == 0 || iFebId == 1 || iFebId == 2 || iFebId == 3 || iFebId == 4 || iFebId == 8 || iFebId == 9
          || iFebId == 10 || iFebId == 11 || iFebId == 17) {
        sChannel = GetPadXA(iFebId, 127 - usChan);
        sSector  = GetPadYA(iFebId, 127 - usChan);
      }
      else {
        sChannel = GetPadXA(iFebId, usChan);
        sSector  = GetPadYA(iFebId, usChan);
      }
    }
    else if (station == 2) {  //For RPC
      sChannel = GetPadXRpc(iFebId, usChan);
      sSector  = GetPadYRpc(iFebId, usChan);
    }
    else  // Checking for the not connected or misconnected pads
    {
      LOG(debug) << "Sector " << sSector << " channel " << sChannel << " is not connected or misconnected to pad. "
                 << " corresponding Feb is " << iFebId << " and uschan " << usChan << " DPB id " << dpbidx
                 << " Skipping this hit message.";
      return 0;
    }

    //Creating Unique address of the particular channel of GEM
    uint32_t address = CbmMuchAddress::GetAddress(station, layer, layerside, sModule, sSector, sChannel);
    LOG(debug) << "Raw address " << address;
    address = CbmMuchAddress::SetElementId(address, 1, station);
    LOG(debug) << "After adding station " << address << " binary " << std::bitset<32>(address);
    address = CbmMuchAddress::SetElementId(address, 2, layer);
    LOG(debug) << "After adding Layer " << address << " binary " << std::bitset<32>(address);
    address = CbmMuchAddress::SetElementId(address, 3, layerside);
    LOG(debug) << "After adding Layer Side " << address << " binary " << std::bitset<32>(address);
    address = CbmMuchAddress::SetElementId(address, 4, sModule);
    LOG(debug) << "After adding module " << address << " binary " << std::bitset<32>(address);
    address = CbmMuchAddress::SetElementId(address, 5, sSector);
    LOG(debug) << "After adding sector " << address << " binary " << std::bitset<32>(address);
    address = CbmMuchAddress::SetElementId(address, 6, sChannel);
    LOG(debug) << "After adding channel " << address << " binary " << std::bitset<32>(address);

    return address;
  }


  // -------------------------------------------------------------------------
  /*
  int16_t MuchReadoutConfig::ElinkIdxToFebIdx(uint16_t uElink)
  {
    if (uElink < numElinksPerCrob) return kiCrobMapElinkFebIdx[uElink];
    else {
      LOG(warning) << "MuchReadoutConfig::ElinkIdxToFebIdx => Index out of bound, "
                   << "Elink is " << uElink << " returning crazy value!";
      return -1;
    }
  }
  */

  bool MuchReadoutConfig::IsCrobActive(uint16_t uDpbIdx, uint16_t uCrobIdx)
  {
    if (uDpbIdx < numComp) {
      if (uCrobIdx < numCrobPerComp) return 0 < fiCrobActiveFlag[uDpbIdx * numCrobPerComp + uCrobIdx] ? true : false;
      else {
        LOG(warning) << "MuchReadoutConfig::IsCrobActive => Crob Index out of bound, "
                     << "returning default inactive!";
        return false;
      }
    }
    else {
      LOG(warning) << "MuchReadoutConfig::IsCrobActive => Dpb Index out of bound, "
                   << "returning default inactive!";
      return false;
    }
  }

  bool MuchReadoutConfig::IsFebActive(uint16_t uFebInSystIdx)
  {

    if (uFebInSystIdx < GetNrOfFebs()) {
      /// Always return true for now
      return true;
    }
    else {
      LOG(warning) << "MuchReadoutConfig::IsFebActive => Feb Index out of bound, "
                   << "returning default inactive!";
      return false;
    }
  }

  bool MuchReadoutConfig::IsFebActive(uint16_t uDpbIdx, uint16_t uCrobIdx, uint16_t uFebIdx)
  {
    if (uDpbIdx < numComp) {
      if (uCrobIdx < numCrobPerComp) {
        if (uFebIdx < kuNbFebsPerCrob) {
          uint16_t uIdx = (uDpbIdx * numCrobPerComp + uCrobIdx) * kuNbFebsPerCrob + uFebIdx;
          return IsFebActive(uIdx);
        }
        else {
          LOG(warning) << "MuchReadoutConfig::IsFebActive => Feb Index out of bound, "
                       << "returning default inactive!";
          return false;
        }
      }
      else {
        LOG(warning) << "MuchReadoutConfig::IsFebActive => Crob Index out of bound, "
                     << "returning default inactive!";
        return false;
      }
    }
    else {
      LOG(warning) << "MuchReadoutConfig::IsFebActive => Dpb Index out of bound, "
                   << "returning default inactive!";
      return false;
    }
  }

  int8_t MuchReadoutConfig::GetPadXA(uint8_t febid, uint8_t channelid)
  {
    if (fChannelsToPadXA.size() <= static_cast<int16_t>((febid * numChanPerAsic) + channelid)) {
      LOG(debug) << "MuchReadoutConfig::GetPadXA => Index out of bounds: " << ((febid * numChanPerAsic) + channelid)
                 << " VS " << fChannelsToPadXA.size() << " (" << febid << " and " << channelid << ")";
      return -2;
    }
    return fChannelsToPadXA[(febid * numChanPerAsic) + channelid];
  }

  int8_t MuchReadoutConfig::GetPadYA(uint8_t febid, uint8_t channelid)
  {
    if (fChannelsToPadYA.size() <= static_cast<int16_t>((febid * numChanPerAsic) + channelid)) {
      LOG(debug) << "MuchReadoutConfig::GetPadYA => Index out of bounds: " << ((febid * numChanPerAsic) + channelid)
                 << " VS " << fChannelsToPadYA.size() << " (" << febid << " and " << channelid << ")";
      return -2;
    }
    return fChannelsToPadYA[(febid * numChanPerAsic) + channelid];
  }

  int8_t MuchReadoutConfig::GetPadXB(uint8_t febid, uint8_t channelid)
  {
    if (fChannelsToPadXB.size() <= static_cast<int16_t>((febid * numChanPerAsic) + channelid)) {
      LOG(debug) << "MuchReadoutConfig::GetPadXB => Index out of bounds: " << ((febid * numChanPerAsic) + channelid)
                 << " VS " << fChannelsToPadXB.size() << " (" << febid << " and " << channelid << ")";
      return -2;
    }
    return fChannelsToPadXB[(febid * numChanPerAsic) + channelid];
  }

  int8_t MuchReadoutConfig::GetPadYB(uint8_t febid, uint8_t channelid)
  {
    if (fChannelsToPadYB.size() <= static_cast<int16_t>((febid * numChanPerAsic) + channelid)) {
      LOG(debug) << "MuchReadoutConfig::GetPadYB => Index out of bounds: " << ((febid * numChanPerAsic) + channelid)
                 << " VS " << fChannelsToPadYB.size() << " (" << febid << " and " << channelid << ")";
      return -2;
    }
    return fChannelsToPadYB[(febid * numChanPerAsic) + channelid];
  }

  int8_t MuchReadoutConfig::GetPadXRpc(uint8_t febid, uint8_t channelid)
  {
    if (fChannelsToPadXRpc.size() <= static_cast<int16_t>((febid * numChanPerAsic) + channelid)) {
      LOG(debug) << "CbmMcbm2018MuchPar::GetPadXRpc => Index out of bounds: " << ((febid * numChanPerAsic) + channelid)
                 << " VS " << fChannelsToPadXRpc.size() << " (" << febid << " and " << channelid << ")";
      return -2;
    }
    return fChannelsToPadXRpc[(febid * numChanPerAsic) + channelid];
  }

  int8_t MuchReadoutConfig::GetPadYRpc(uint8_t febid, uint8_t channelid)
  {
    if (fChannelsToPadYRpc.size() <= static_cast<int16_t>((febid * numChanPerAsic) + channelid)) {
      LOG(debug) << "CbmMcbm2018MuchPar::GetPadYRpc => Index out of bounds: " << ((febid * numChanPerAsic) + channelid)
                 << " VS " << fChannelsToPadYRpc.size() << " (" << febid << " and " << channelid << ")";
      return -2;
    }
    return fChannelsToPadYRpc[(febid * numChanPerAsic) + channelid];
  }

  int32_t MuchReadoutConfig::GetFebId(uint16_t uAsicIdx)
  {
    if (uAsicIdx >= GetNrOfFebsInGemA() && uAsicIdx < (GetNrOfFebsInGemA() + GetNrOfFebsInRpc()))  //Check
      return fnFebsIdsArrayRpc[uAsicIdx - GetNrOfFebsInGemA()];                                    //Check Vikas
    else if (uAsicIdx >= (GetNrOfFebsInGemA() + GetNrOfFebsInRpc())
             && uAsicIdx < (GetNrOfFebsInGemA() + GetNrOfFebsInRpc() + GetNrOfFebsInGemB()))  //Check
      return fnFebsIdsArrayGemB[uAsicIdx - (GetNrOfFebsInGemA() + GetNrOfFebsInRpc())];
    else if (uAsicIdx < GetNrOfFebsInGemA())
      return fnFebsIdsArrayGemA[uAsicIdx];
    else {
      LOG(warning) << "MuchReadoutConfig::GetFebId => provided uAsicIdx : " << uAsicIdx
                   << " not in the range of :" << (GetNrOfFebsInGemA() + GetNrOfFebsInRpc() + GetNrOfFebsInGemB())
                   << "Returning large value -2";
      return -2;
    }
  }

  double MuchReadoutConfig::GetRealX(int16_t SectorIndex)
  {
    if (SectorIndex < 0 || SectorIndex <= 97) {
      LOG(debug) << "MuchReadoutConfig::GetRealX => Index out of bounds: ";
      return -2;
    }
    return fRealX[SectorIndex];
  }

  double MuchReadoutConfig::GetRealPadSize(int16_t SectorIndex)
  {
    if (SectorIndex < 0 || SectorIndex <= 97) {
      LOG(debug) << "MuchReadoutConfig::GetRealX => Index out of bounds: ";
      return -2;
    }
    return fRealPadSize[SectorIndex];
  }

  double MuchReadoutConfig::GetRealX(int16_t Channel, int16_t Sector)
  {
    int16_t PadIndex = Channel + 97 * Sector;
    if (Channel < 0 || Sector < 0) return -2;
    if (fRealX.size() <= PadIndex) {
      LOG(info) << "MuchReadoutConfig::GetRealX => Index out of bounds: " << Channel << " " << Sector << " "
                << PadIndex;
      return -1;
    }
    return fRealX[PadIndex];
  }

  double MuchReadoutConfig::GetRealPadSize(int16_t Channel, int16_t Sector)
  {
    int16_t PadIndex = Channel + 97 * Sector;
    if (Channel < 0 || Sector < 0) return -2;
    if (fRealPadSize.size() <= PadIndex) {
      LOG(info) << "MuchReadoutConfig::GetRealPadSize => Index out of bounds: " << Channel << " " << Sector << " "
                << PadIndex;
      return -1;
    }
    return fRealPadSize[PadIndex];
  }

} /* namespace cbm::algo */
