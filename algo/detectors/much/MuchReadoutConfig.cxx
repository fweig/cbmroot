/* Copyright (C) 2017-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau, Ajit Kumar, Florian Uhlig [committer] */

#include "MuchReadoutConfig.h"

#include "CbmMuchAddress.h"

#include <bitset>

#include "AlgoFairloggerCompat.h"

using namespace std;

namespace cbm::algo
{
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
    // This here refers to the mCBM 2022 setup.
    // Taken from CbmMuchUnpackPar in combination with macro/beamtime/mcbm2022/mMuchPar.par

    // Array to hold the unique IDs (equipment ID) for all MUCH DPBs
    uint16_t eqId[numComp] = {0x1001, 0x1002, 0x1003, 0x1006, 0x2002, 0x2003};

    // Mapping of eLink to FEB number within CROB. If -1, elink not used.
    // This mapping is the same for each component.
    const int16_t elink2Feb[numElinksPerCrob] = {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4,
                                                 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 8, 8, 8, 8};

    // FEB IDs connected to GEM Module A
    fnFebsIdsArrayGemA = {
      7,  8,  -1, 10, 17, 9,  14, 11, -1,   //Id for the connected FEB Position in DPB 1001 GEM Module A
      -1, 5,  -1, 16, 13, 12, -1, 15, -1,   //Id for the connected FEB Position in DPB 1002 GEM Module A
      -1, -1, -1, 4,  3,  -1, -1, -7, -1};  //Id for the connected FEB Position in DPB 1003 GEM Module A (-7 is Pulser)

    // FEB IDs connected to GEM Module B
    fnFebsIdsArrayGemB = {
      6,  16, -1, 14, 5,  8,  9,  4, 15,   //Id for the connected FEB Position in DPB 2002 GEM Module B
      -1, 3,  -1, 17, 10, 11, 12, 7, 13};  //Id for the connected FEB Position in  DPB 2003 GEM Module B

    // FEB IDs connected to RPC Module
    fnFebsIdsArrayRpc = {3, -1, -1, 2, 0, -1, 1, -1, -1};  //Id for the connected FEB Position in GEM Module B

    // Init arrays which store positions of PADs
    InitChannelToPadMaps();

    // Constructing the map (equipmentId, eLink, channel) -> (MUCH address)
    uint16_t numElinksPerComp = numCrobPerComp * numElinksPerCrob;
    uint16_t numFebsPerComp   = numCrobPerComp * numFebsPerCrob;

    for (uint16_t comp = 0; comp < numComp; comp++) {
      uint16_t equipment = eqId[comp];
      fReadoutMap[equipment].resize(numElinksPerComp);
      for (uint16_t crob = 0; crob < numCrobPerComp; crob++) {
        for (uint16_t elink = 0; elink < numElinksPerCrob; elink++) {

          fReadoutMap[equipment][elink].resize(numChanPerAsic);
          uint16_t elinkId   = numElinksPerCrob * crob + elink;    // elink within component
          uint32_t febInCrob = elink2Feb[elinkId];                 // FEB within CROB
          uint32_t feb       = comp * numFebsPerComp + febInCrob;  // global FEB index
          int32_t febId      = GetFebId(feb);                      // FEB Id in MUCH addressing scheme

          for (uint16_t chanInAsic = 0; chanInAsic < numChanPerAsic; chanInAsic++) {
            uint32_t channel                       = chanInAsic + numChanPerAsic * (feb % numAsicsPerFeb);  //Fix this!
            fReadoutMap[equipment][elink][channel] = CreateMuchAddress(comp, febId, channel);
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

  int8_t MuchReadoutConfig::GetPadXA(uint8_t febid, uint8_t channelid)
  {
    if (fChannelsToPadXA.size() <= (static_cast<uint32_t>(febid) * numChanPerAsic) + channelid) {
      LOG(debug) << "MuchReadoutConfig::GetPadXA => Index out of bounds: " << ((febid * numChanPerAsic) + channelid)
                 << " VS " << fChannelsToPadXA.size() << " (" << febid << " and " << channelid << ")";
      return -2;
    }
    return fChannelsToPadXA[(febid * numChanPerAsic) + channelid];
  }

  int8_t MuchReadoutConfig::GetPadYA(uint8_t febid, uint8_t channelid)
  {
    if (fChannelsToPadYA.size() <= (static_cast<uint32_t>(febid) * numChanPerAsic) + channelid) {
      LOG(debug) << "MuchReadoutConfig::GetPadYA => Index out of bounds: " << ((febid * numChanPerAsic) + channelid)
                 << " VS " << fChannelsToPadYA.size() << " (" << febid << " and " << channelid << ")";
      return -2;
    }
    return fChannelsToPadYA[(febid * numChanPerAsic) + channelid];
  }

  int8_t MuchReadoutConfig::GetPadXB(uint8_t febid, uint8_t channelid)
  {
    if (fChannelsToPadXB.size() <= (static_cast<uint32_t>(febid) * numChanPerAsic) + channelid) {
      LOG(debug) << "MuchReadoutConfig::GetPadXB => Index out of bounds: " << ((febid * numChanPerAsic) + channelid)
                 << " VS " << fChannelsToPadXB.size() << " (" << febid << " and " << channelid << ")";
      return -2;
    }
    return fChannelsToPadXB[(febid * numChanPerAsic) + channelid];
  }

  int8_t MuchReadoutConfig::GetPadYB(uint8_t febid, uint8_t channelid)
  {
    if (fChannelsToPadYB.size() <= (static_cast<uint32_t>(febid) * numChanPerAsic) + channelid) {
      LOG(debug) << "MuchReadoutConfig::GetPadYB => Index out of bounds: " << ((febid * numChanPerAsic) + channelid)
                 << " VS " << fChannelsToPadYB.size() << " (" << febid << " and " << channelid << ")";
      return -2;
    }
    return fChannelsToPadYB[(febid * numChanPerAsic) + channelid];
  }

  int8_t MuchReadoutConfig::GetPadXRpc(uint8_t febid, uint8_t channelid)
  {
    if (fChannelsToPadXRpc.size() <= (static_cast<uint32_t>(febid) * numChanPerAsic) + channelid) {
      LOG(debug) << "CbmMcbm2018MuchPar::GetPadXRpc => Index out of bounds: " << ((febid * numChanPerAsic) + channelid)
                 << " VS " << fChannelsToPadXRpc.size() << " (" << febid << " and " << channelid << ")";
      return -2;
    }
    return fChannelsToPadXRpc[(febid * numChanPerAsic) + channelid];
  }

  int8_t MuchReadoutConfig::GetPadYRpc(uint8_t febid, uint8_t channelid)
  {
    if (fChannelsToPadYRpc.size() <= (static_cast<uint32_t>(febid) * numChanPerAsic) + channelid) {
      LOG(debug) << "CbmMcbm2018MuchPar::GetPadYRpc => Index out of bounds: " << ((febid * numChanPerAsic) + channelid)
                 << " VS " << fChannelsToPadYRpc.size() << " (" << febid << " and " << channelid << ")";
      return -2;
    }
    return fChannelsToPadYRpc[(febid * numChanPerAsic) + channelid];
  }

  int32_t MuchReadoutConfig::GetFebId(uint16_t uAsicIdx)
  {
    if (uAsicIdx >= numFebsInGemA && uAsicIdx < (numFebsInGemA + numFebsInRpc))  //Check
      return fnFebsIdsArrayRpc[uAsicIdx - numFebsInGemA];                        //Check Vikas
    else if (uAsicIdx >= (numFebsInGemA + numFebsInRpc)
             && uAsicIdx < (numFebsInGemA + numFebsInRpc + numFebsInGemB))  //Check
      return fnFebsIdsArrayGemB[uAsicIdx - (numFebsInGemA + numFebsInRpc)];
    else if (uAsicIdx < numFebsInGemA)
      return fnFebsIdsArrayGemA[uAsicIdx];
    else {
      LOG(warning) << "MuchReadoutConfig::GetFebId => provided uAsicIdx : " << uAsicIdx
                   << " not in the range of :" << (numFebsInGemA + numFebsInRpc + numFebsInGemB)
                   << "Returning large value -2";
      return -2;
    }
  }

  void MuchReadoutConfig::InitChannelToPadMaps()
  {
    // Array which stores the corresponding x position of PAD of RPC module
    fChannelsToPadXRpc = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1,              //
                          1, 1, 1, 1, 2, 0, 2, 2, 2, 2, 2, 2, 2, 2, 1, 3,              //
                          3, 2, 3, 3, 3, 3, 3, 3, 0, 3, 2, 3, 4, 4, 4, 1,              //
                          4, 4, 4, 4, 4, 4, 3, 4, 2, 5, 1, 0, 5, 4, 5, 5,              //
                          5, 5, 5, 5, 5, 5, 3, 5, 3, 2, 2, 4, 0, 1, -3, -3,            //
                          6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 4, 6, 3, 5, 1, 2,              //
                          7, 0, 7, 7, 7, 7, 4, 7, 7, 5, 7, 7, 7, 7, 5, 6,              //
                          0, 1, 3, 2, 8, 6, 8, 8, 8, 8, 3, 8, 8, 4, 8, 8,              //
                                                                                       //
                          8, 8, 5, 0, 6, 1, 7, 7, 9, 9, 9, 0, 4, 2, 9, 9,              //
                          9, 3, 9, 9, 9, 9, 4, 9, 3, 4, 0, 2, 5, 1, 5, 5,              //
                          6, 6, 6, 6, 7, 7, 7, 7, 8, 7, 8, 8, 8, 8, 2, 1,              //
                          4, 3, 6, 5, 0, 7, 8, 1, 9, 9, 9, 2, 3, 8, 5, 4,              //
                          0, 6, 1, 8, 7, 2, 9, 9, 4, 3, 6, 5, 8, 7, 6, 7,              //
                          4, 5, 2, 3, 0, 1, 8, 8, 9, 9, 9, 9, 4, 9, 6, 5,              //
                          8, 7, 9, 9, 8, 9, 2, 3, 7, 1, 6, 4, 9, 5, 8, 0,              //
                                                                                       //
                          0, 0, 2, 1, 8, 9, 6, 7, 0, 5, 1, 4, 1, 1, 9, 3,              //
                          2, 7, 5, 6, 2, 3, 2, 3, 0, 1, 8, 4, 9, 4, 7, 8,              //
                          5, 6, 8, 9, 7, 1, 2, 6, 4, 5, 9, 3, 7, 8, 5, 6,              //
                          3, 4, 9, 2, 0, 7, 3, 5, 1, -3, 4, -3, 6, -3, 8, -3,          //
                          9, 2, 3, 8, 5, 4, 7, 6, 8, 9, 6, 7, 4, 5, 1, 3,              //
                          9, 2, 0, 8, 6, 7, 4, 5, 2, 3, 8, 9, 5, 6, 4, 1,              //
                          7, 3, 2, 9, -3, 8, -3, 7, -3, 6, -3, 5, -3, 4, -3, 3,        //
                          -3, 1, -3, 2, 8, 9, -3, 7, 1, 6, 0, 0, -3, -3, 0, -3,        //
                          6, 7, 1, 5, 3, 2, 3, 4, 1, 2, 0, 0, 0, 0, -3, -3,            //
                                                                                       //
                          0, 0, 1, 0, 3, 2, 5, 4, 8, 9, 7, 0, 5, 6, 4, 1,              //
                          2, 3, 8, 9, 0, 1, 6, 7, 4, 5, 2, 3, 8, 9, 1, 7,              //
                          5, 6, 3, 4, 9, 2, 7, 8, 1, 6, 2, 5, 4, 3, 8, 9,              //
                          6, 7, 4, 5, 2, 3, 9, 1, 7, 8, 5, 6, 3, 4, 9, 2,              //
                          7, 8, 5, 6, 3, 4, 2, 0, 9, 1, 7, 8, 5, 6, 3, 4,              //
                          1, 2, 8, 9, 6, 7, 4, 5, 2, 3, 1, 1, -3, -3, -3, -3,          //
                          -3, -3, -3, -3, -3, -3, 0, 0, 0, -3, 0, -3, -3, -3, -3, -3,  //
                          -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3};

    // Array which stores the corresponding y position of PAD of RPC module
    fChannelsToPadYRpc = {1, 0, 3, 2, 5, 4, 7, 6, 0, 8, 2, 1, 3, 9, 5, 4,                  //
                          7, 6, 9, 8, 0, 10, 2, 1, 4, 3, 6, 5, 8, 7, 10, 0,                //
                          1, 9, 3, 2, 5, 4, 7, 6, 11, 8, 10, 9, 1, 0, 2, 11,               //
                          4, 3, 6, 5, 8, 7, 10, 9, 11, 0, 12, 12, 1, 10, 3, 2,             //
                          5, 4, 7, 6, 9, 8, 11, 10, 12, 12, 13, 11, 13, 13, -3, -3,        //
                          1, 0, 3, 2, 5, 4, 7, 6, 9, 8, 12, 10, 13, 11, 14, 14,            //
                          0, 14, 2, 1, 4, 3, 13, 5, 6, 12, 8, 7, 10, 9, 13, 11,            //
                          15, 15, 14, 15, 0, 12, 2, 1, 4, 3, 15, 5, 6, 14, 8, 7,           //
                                                                                           //
                          10, 9, 14, 16, 13, 16, 11, 12, 1, 0, 3, 17, 15, 16, 2, 4,        //
                          5, 16, 7, 6, 9, 8, 16, 10, 17, 17, 18, 17, 15, 17, 17, 16,       //
                          15, 14, 17, 16, 15, 14, 17, 16, 12, 13, 14, 13, 16, 15, 18, 18,  //
                          18, 18, 18, 18, 19, 18, 11, 19, 12, 11, 13, 19, 19, 17, 19, 19,  //
                          20, 19, 20, 18, 19, 20, 15, 14, 20, 20, 20, 20, 20, 20, 21, 21,  //
                          21, 21, 21, 21, 21, 21, 19, 21, 17, 16, 19, 18, 22, 20, 22, 22,  //
                          22, 22, 22, 21, 23, 23, 22, 22, 23, 22, 23, 23, 24, 23, 24, 22,  //
                          24, 24, 23, 24, 23, 23, 24, 24, 24, 24, 23, 24, 26, 25, -3, -3,  //
                                                                                           //
                          27, 28, 25, 25, 25, 25, 25, 25, 29, 25, 26, 25, 28, 27, 26, 25,  //
                          26, 26, 26, 26, 27, 26, 28, 27, 30, 29, 26, 26, 27, 27, 27, 27,  //
                          27, 27, 28, 28, 28, 30, 29, 28, 28, 28, 29, 28, 29, 29, 29, 29,  //
                          29, 29, 30, 30, 31, 30, 30, 30, 31, -3, 30, -3, 30, -3, 30, -3,  //
                          31, 31, 31, 31, 31, 31, 31, 31, 32, 32, 32, 32, 32, 32, 32, 32,  //
                          33, 32, 32, 33, 33, 33, 33, 33, 33, 33, 34, 34, 34, 34, 34, 33,  //
                          34, 34, 34, 35, -3, 35, -3, 35, -3, 35, -3, 35, -3, 35, -3, 35,  //
                          -3, 34, -3, 35, 36, 36, -3, 36, 35, 36, 35, 34, -3, -3, 33, -3,  //
                                                                                           //
                          37, 36, 36, 38, 36, 36, 36, 36, 37, 37, 37, 39, 37, 37, 37, 37,  //
                          37, 37, 38, 38, 40, 38, 38, 38, 38, 38, 38, 38, 39, 39, 39, 39,  //
                          39, 39, 39, 39, 40, 39, 40, 40, 40, 40, 40, 40, 40, 40, 41, 41,  //
                          41, 41, 41, 41, 41, 41, 42, 41, 42, 42, 42, 42, 42, 42, 43, 42,  //
                          43, 43, 43, 43, 43, 43, 43, 41, 44, 42, 44, 44, 44, 44, 44, 44,  //
                          43, 44, 45, 45, 45, 45, 45, 45, 45, 45, 44, 45, -3, -3, -3, -3,  //
                          -3, -3, -3, -3, -3, -3, 44, 45, 43, -3, 42, -3, -3, -3, -3, -3,  //
                          -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3};

    // Array which stores the corresponding x position of PAD of entire module A
    fChannelsToPadXA = {8, 6, 8, 6, 8, 6, 8, 5, 7, 5, 7, 5, 7, 5, 7, 5,          //
                        7, 4, 6, 4, 6, 4, 8, 4, 9, 4, 9, 3, 9, 3, 9, 3,          //
                        9, 3, 10, 3, 10, 2, 10, 2, 10, 2, 10, 2, 11, 2, 11, 1,   //
                        11, 1, 11, 1, 11, 1, 12, 1, 12, 0, 12, 0, 12, 0, 12, 0,  //
                        13, 0, 13, 6, 13, 5, 13, 5, 13, 5, 14, 5, 14, 5, 14, 4,  //
                        14, 4, 14, 4, 15, 4, 15, 4, 15, 3, 15, 3, 15, 3, 16, 3,  //
                        16, 3, 16, 2, 16, 2, 16, 2, 17, 2, 17, 2, 17, 1, 17, 1,  //
                        17, 1, 18, 1, 18, 1, 18, 0, 18, 0, 18, 0, 19, 0, 19, 0,  //
                        //
                        -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,  //
                        -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,  //
                        -2, -2, -2, -2, 17, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,  //
                        -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,  //
                        -2, 19, -2, 20, -2, 20, -2, 20, 21, 20, 21, 20, 21, 20, 21, 20,  //
                        21, 20, 21, 21, 22, 21, 22, 21, 22, -1, 22, -1, 22, -1, 22, -1,  //
                        22, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  //
                        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  //
                        //
                        8, 6, 8, 6, 8, 6, 9, 7, 9, 6, 9, 7, 9, 7, 9, 7,          //
                        10, 7, 10, 7, 10, 7, 10, 7, 10, 8, 11, 8, 11, 0, 11, 0,  //
                        12, 0, 12, 11, 6, 11, 6, 4, 6, 4, 6, 5, 7, 5, 7, 5,      //
                        7, 4, 8, 4, 8, 4, 8, 4, 8, 4, 8, 4, 8, 3, 8, 3,          //
                        8, 3, 9, 3, 9, 3, 9, 3, 9, 3, 9, 2, 9, 2, 9, 2,          //
                        9, 2, 10, 2, 10, 2, 10, 2, 10, 2, 10, 1, 10, 1, 10, 1,   //
                        10, 1, 11, 1, 11, 1, 11, 1, 11, 1, 11, 0, 11, 0, 11, 0,  //
                        6, 0, 6, 0, 6, 0, 6, 0, 5, 0, 5, 0, 5, 3, 5, 5,          //
                        //
                        5, 4, 5, 4, 5, 4, 5, 4, 7, 4, 6, 4, 6, 4, 6, 3,  //
                        6, 3, 6, 3, 6, 3, 6, 3, 6, 3, 6, 3, 6, 3, 7, 3,  //
                        7, 3, 7, 2, 7, 2, 7, 2, 7, 2, 7, 2, 7, 2, 7, 2,  //
                        8, 2, 8, 2, 8, 2, 8, 1, 8, 1, 8, 1, 8, 1, 8, 1,  //
                        8, 1, 8, 1, 9, 1, 9, 1, 9, 1, 9, 0, 9, 0, 9, 0,  //
                        4, 0, 4, 0, 4, 0, 4, 4, 4, 4, 4, 4, 4, 5, 4, 5,  //
                        4, 5, 3, 5, 3, 5, 3, 5, 3, 3, 4, 3, 5, 3, 5, 3,  //
                        5, 3, 5, 3, 5, 2, 5, 2, 5, 2, 5, 2, 5, 2, 5, 2,  //
                        //
                        11, 9, 11, 9, 11, 9, 12, 10, 12, 10, 12, 10, 12, 10, 12, 10,     //
                        12, 10, 12, 10, 12, 10, 13, 10, 13, 10, 13, 11, 13, 11, 13, 11,  //
                        14, 11, 14, 11, 14, 11, 14, 12, 14, 12, 15, 13, 15, 13, 15, 13,  //
                        15, 13, 15, 13, 16, 14, 16, 14, 16, 14, 16, 14, 17, 14, 17, 15,  //
                        9, 15, 9, 9, 9, 8, 9, 8, 9, 8, 9, 8, 9, 8, 9, 8,                 //
                        9, 9, -1, 8, -1, 8, 10, 8, 10, 8, 10, 7, 10, 7, 10, 7,           //
                        10, 7, 10, 7, 10, 7, 10, 7, 11, 7, 11, 7, 11, 7, 11, 6,          //
                        11, 6, 11, 6, 11, 6, 11, 6, 11, 6, 11, 6, 12, 6, 12, 6,          //
                        //
                        17, 15, 17, 15, 17, 16, 17, 16, 17, 16, 18, 16, -1, 16, 18, 17,  //
                        18, 17, 18, 17, 18, 18, 19, 15, 19, 18, 19, 18, 19, 18, 19, 18,  //
                        19, 19, 20, 19, 20, 19, 20, 19, 20, 20, 20, 20, 21, 20, 21, 20,  //
                        21, 20, 21, 21, 21, 21, 22, 21, 22, 21, 22, 21, 22, 22, 22, 22,  //
                        22, 22, 22, 22, 22, 22, 14, 22, 14, 22, 14, 13, 14, 13, 14, 13,  //
                        14, 13, 14, 13, 14, 12, 14, 12, 14, 12, 16, 13, 15, 13, 15, 13,  //
                        15, 13, 15, 13, 15, 12, 15, 12, 15, 12, 15, 12, 16, 12, 16, 15,  //
                        16, 16, 16, 16, 16, 16, 16, 16, 17, 17, 17, 17, 17, 17, 17, 17,  //
                        //
                        19, 17, 19, 17, 19, 18, 19, 18, 19, 18, 19, 18, 19, 18, 20, 18,  //
                        20, 18, 20, 18, 20, 18, 20, 18, 21, 19, 21, 19, 21, 19, 21, 20,  //
                        21, 20, 22, 20, 22, 20, 22, 20, 22, 21, 22, 21, 22, 21, 16, 21,  //
                        16, 21, 16, 22, 16, 16, 16, 16, 16, 16, 17, 16, 17, 16, 17, 16,  //
                        18, 17, 18, 17, 18, 17, 18, 17, 18, 17, 18, 17, 19, 17, 19, 17,  //
                        19, 17, 19, 18, 19, 18, 19, 18, 19, 18, 20, 18, 20, 18, 20, 19,  //
                        20, 19, 20, 19, 20, 19, 21, 19, 21, 20, 21, 20, 21, 20, 21, 20,  //
                        21, 20, 21, 20, 22, 21, 22, 21, 22, 21, 22, 21, 22, 21, 22, 21,  //
                        //
                        12, 11, 12, 11, 12, 11, 12, 11, 13, 11, 13, 11, 13, 11, 13, 12,  //
                        13, 12, 13, 12, 13, 12, 13, 12, 14, 12, 14, 12, 14, 12, 14, 13,  //
                        14, 13, 14, 13, 14, 13, 15, 14, 15, 14, 15, 14, 15, 14, 15, 14,  //
                        15, 15, 12, 15, 12, 15, 12, 15, 12, 15, 12, 15, 12, 11, 12, 11,  //
                        13, 12, 13, 12, 13, 12, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13,  //
                        14, 13, 15, 13, 15, 13, 15, 13, 15, 14, 15, 14, 15, 14, 15, 14,  //
                        16, 14, 16, 14, 16, 15, 16, 15, 16, 15, 16, 15, 17, 15, 17, 16,  //
                        17, 16, 11, 16, -1, 16, -1, 16, -1, 16, -1, 17, -1, 17, -1, 17,  //
                        //
                        8, 7, 8, 7, 8, 7, 9, 7, -2, 7, -2, 7, -2, 7, -2, 8,            //
                        -2, 8, -2, 8, 9, 8, -2, 8, -2, 8, -2, 8, 10, 8, 10, 8,         //
                        10, 9, 10, 10, 10, 10, 10, 10, 11, 10, 11, 10, 11, 10, 8, 11,  //
                        8, 11, 8, 7, 8, 7, 8, 7, 8, 7, 8, 7, 8, 7, 8, 8,               //
                        8, 7, 8, 7, 9, 7, 9, 7, 9, 7, 9, 7, 9, 6, 9, 6,                //
                        9, 6, 10, 6, 10, 6, 10, 6, 10, 6, 10, 9, 10, 9, 11, 9,         //
                        11, 9, 11, 9, 11, 10, 11, 10, 11, 10, 12, 10, -1, 10, -1, 10,  //
                        -1, 11, -1, 11, -1, 11, -1, 11, 8, 12, -1, 6, -1, 7, -1, 6,    //
                        //
                        4, 2, 4, 2, -1, 2, 4, 2, 4, 1, 4, 1, 3, 1, 3, 1,         //
                        3, 1, 3, 1, 3, 1, 3, 1, 4, 1, 4, 1, 4, 0, 4, 0,          //
                        4, 0, 4, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,          //
                        5, 3, 5, 3, 5, 3, 5, 3, 5, 3, 5, 3, 6, 2, 6, 2,          //
                        6, 2, 6, 2, 6, 2, 6, 2, 6, 2, 6, 2, 6, 2, 6, 2,          //
                        6, 2, 6, 2, 6, 1, 7, 1, 7, 1, 7, 1, 7, 1, 7, 1,          //
                        -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 0, -1, 0,  //
                        -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0,  //
                        //
                        3, 0, 3, 0, 3, 2, 4, 2, 4, 2, 4, 2, 4, 2, 4, 2,   //
                        4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 5, 3,   //
                        5, 3, 5, 2, 5, 2, 5, 2, 5, 2, 5, 2, 5, 2, 5, 1,   //
                        5, 1, 5, 1, 5, 1, 6, 1, 6, -1, 6, 1, 6, 1, 6, 1,  //
                        3, 1, 3, 1, 3, 1, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0,   //
                        2, 0, 2, 0, 2, 0, 3, 0, 3, 0, 3, 0, 3, 0, 4, 2,   //
                        4, 2, 4, 2, 4, 2, 4, 2, 4, 2, 4, 2, 4, 2, 4, 2,   //
                        4, 1, 4, 1, 4, 1, 5, 1, 5, 1, 5, 1, 5, 1, 5, 1,   //
                        //
                        8, 6, 8, 6, 8, 6, 8, 6, 8, 6, 9, 6, 9, 6, 9, 6,          //
                        9, 6, 9, 7, 9, 7, 9, 7, 10, 7, 10, 7, 10, 8, 10, 8,      //
                        10, 8, 10, 9, 11, 9, 11, 9, 11, 9, 11, 9, 7, 10, 7, 10,  //
                        6, 10, 7, 10, 7, 10, 6, 10, 6, 11, 6, 11, 6, 11, 6, 6,   //
                        7, 6, 7, 6, 7, 5, 7, 5, 7, 5, 7, 5, 7, 5, 8, 5,          //
                        8, 5, 8, 5, 8, 5, 8, 5, 8, 5, 8, 5, 8, -1, 8, -1,        //
                        8, 4, 8, 4, 8, 4, 9, 4, 9, 4, 9, 6, 7, 6, 7, 6,          //
                        7, 7, 7, 6, 7, 6, 7, -2, 7, 5, 6, 5, 6, 5, 6, 5,         //
                        //
                        12, 11, 12, 11, 12, 11, 12, 11, 12, 12, 13, 12, 13, 12, 13, 12,  //
                        13, 12, 10, 12, 10, 12, 10, 13, 11, 13, 11, 13, 11, 13, 11, 11,  //
                        10, 11, 10, 9, 10, 9, 10, 10, 11, 10, 11, 10, 11, 9, 11, 9,      //
                        11, 9, 12, 10, 12, 9, 12, 9, -1, 9, 12, 9, 10, 12, 12, 12,       //
                        13, 12, 13, 12, 13, 12, 13, 11, 13, 12, 14, 13, 14, 13, 14, 13,  //
                        14, 13, 14, 13, 14, 13, 15, 13, 15, 14, 15, 14, 10, 14, 10, 14,  //
                        10, 14, 11, 14, 11, 15, 11, 9, 11, 9, 10, 9, 11, 9, -1, 8,       //
                        11, 8, 11, 8, 10, 8, 10, 8, -1, 8, 9, 9, -1, 8, 10, 9,           //
                        //
                        18, 17, 18, 17, 18, 17, 19, 17, 19, 17, 19, 17, 14, 18, 14, 18,  //
                        15, 18, 15, 18, 15, 18, 15, 18, 15, 18, 15, 18, 15, 18, 15, 19,  //
                        14, 19, 15, 19, 15, 19, 15, 14, 15, 14, 16, 14, 16, 14, 16, 14,  //
                        16, 14, 16, 14, 16, 14, 16, 14, 16, 13, 16, 13, 16, 13, 16, 13,  //
                        15, 16, 15, 17, 15, 17, 16, 17, 16, 17, 16, 17, 16, 15, 16, 15,  //
                        16, 15, 16, 15, 16, 15, 13, 16, 16, 16, 16, 12, 17, 12, 17, 12,  //
                        13, 12, 13, -1, 13, -1, 14, -1, 14, -1, -1, -1, 14, -1, 14, -1,  //
                        13, -1, 14, -1, 14, -1, 13, -1, 13, -1, 12, -1, 12, -1, 13, -1,  //
                        //
                        -2, 19, 22, -2, -2, 19, 22, 19, 20, 19, 20, 20, 20, 20, 21, 20,  //
                        21, 20, 21, 20, 21, 21, 21, 21, 22, 21, 22, 21, 22, 21, 22, 21,  //
                        22, 22, 22, 22, 22, 22, 18, 22, 18, 22, 18, -2, 18, 17, 18, 17,  //
                        18, 17, 18, 17, 18, 17, 19, 17, 19, 18, 19, 18, 19, 18, 19, 18,  //
                        20, 19, 20, 19, 20, 19, 21, 19, 21, 19, 21, 19, 21, 19, 21, 20,  //
                        21, 20, 21, 20, 22, 20, 22, 20, 22, 20, 22, 20, 22, 20, 22, 20,  //
                        22, 21, 22, 21, 17, 21, 17, 21, 17, 21, 17, 22, 17, 22, 17, 22,  //
                        17, 22, 18, -1, 18, 17, -1, 17, 18, 18, 18, 18, 18, 18, 18, 18,  //
                        //
                        22, 22, -1, 22, 21, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 21,  //
                        21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 20, 20, 20, 20, 20,  //
                        20, 20, 20, 20, 20, 20, 20, 19, 18, 19, 22, 19, 22, 19, 21, 19,  //
                        22, 19, 17, 19, 21, 19, 22, 19, 21, 19, 22, 19, 22, 19, 22, 18,  //
                        16, 18, 20, 18, 22, 16, 21, 18, 20, 17, 22, 18, 17, 18, 15, 18,  //
                        21, 18, 20, 15, 19, 18, 19, 17, 18, 17, 18, 17, 17, 16, 17, 17,  //
                        16, 17, 15, 17, 16, 16, 14, 16, 14, 15, 19, 14, 20, 15, 21, 16,  //
                        22, 16, 21, 16, 20, 16, 19, 15, 19, 15, 20, 15, 20, 15, 13, 15,  //
                        //
                        12, 12, -1, 19, -1, 20, -1, 19, -1, 21, -1, 20, -1, 21, -1, 19,  //
                        -1, 11, -1, 21, -1, 20, -1, 20, -1, 19, -1, 19, -1, -1, -1, -1,  //
                        -1, -1, -1, -1, 11, -1, 13, -1, 15, -1, 14, -1, 10, 10, 12, -1,  //
                        13, -1, 11, -1, 12, -1, 11, -1, 10, -1, 10, -1, 9, -1, 9, 9,     //
                        -1, -1, -1, -1, -1, -1, -1, 8, -1, -1, 8, -1, -1, -1, -1, -1,    //
                        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  //
                        -1, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 8, -1, 8, 7,      //
                        7, 9, 6, 7, 4, 6, 5, 5, 6, 6, 5, 6, 5, 5, 4, 5,                  //
                        //
                        5, 1, -2, 1, -2, -2, -2, 0, -2, 0, -2, 0, -2, -1, -2, 0,   //
                        4, 0, 3, -2, 3, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, -1,          //
                        2, 1, 2, 1, 2, 2, 1, 1, 2, 1, 2, 1, -1, 0, -2, 0,          //
                        -2, 0, -2, 0, -2, 0, -2, 0, -2, 0, -2, 0, -2, -1, -2, -1,  //
                        -2, -1, 3, -1, 3, -1, 3, -1, 3, -1, 3, -1, 3, -1, 3, -1,   //
                        3, -1, 4, -1, 4, -1, 4, -1, 4, -1, 4, 2, 4, 2, 4, 2,       //
                        3, 2, 4, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,            //
                        0, 0, 0, 0, 2, 0, 0, 0, 3, 1, 4, 0, 3, 0, 2, 1};

    // Array which stores the corresponding y position of PAD of entire module A
    fChannelsToPadYA = {1, 2, 2, 3, 3, 4, 4, 0, 0, 1, 1, 2, 2, 3, 3, 4,  //
                        4, 0, 0, 1, 1, 2, 0, 3, 4, 4, 3, 0, 2, 1, 1, 2,  //
                        0, 3, 4, 4, 3, 0, 2, 1, 1, 2, 0, 3, 4, 4, 3, 0,  //
                        2, 1, 1, 2, 0, 3, 4, 4, 3, 0, 2, 1, 1, 2, 0, 3,  //
                        4, 4, 3, 7, 2, 5, 1, 6, 0, 9, 4, 8, 3, 7, 2, 5,  //
                        1, 6, 0, 9, 4, 8, 3, 7, 2, 5, 1, 6, 0, 9, 4, 8,  //
                        3, 7, 2, 5, 1, 6, 0, 9, 4, 8, 3, 7, 2, 5, 1, 6,  //
                        0, 9, 4, 8, 3, 7, 2, 5, 1, 6, 0, 9, 4, 8, 3, 7,  //
                        //
                        -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,  //
                        -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,  //
                        -2, -2, -2, -2, 11, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,  //
                        -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,  //
                        -2, 17, -2, 10, -2, 11, -2, 12, 17, 13, 9, 14, 8, 15, 7, 16,     //
                        6, 17, 5, 10, 15, 11, 16, 12, 17, -1, 14, -1, 13, -1, 12, -1,    //
                        11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  //
                        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  //
                        //
                        7, 8, 6, 9, 5, 5, 9, 7, 8, 6, 7, 8, 6, 9, 5, 14,                 //
                        9, 13, 8, 12, 7, 11, 6, 10, 5, 9, 9, 8, 8, 12, 6, 11,            //
                        9, 10, 8, 7, 13, 5, 12, 11, 11, 10, 10, 15, 15, 16, 16, 17,      //
                        17, 12, 10, 13, 11, 14, 12, 17, 13, 16, 14, 15, 15, 10, 16, 11,  //
                        17, 13, 10, 14, 11, 17, 12, 16, 13, 15, 14, 10, 15, 11, 16, 12,  //
                        17, 13, 10, 14, 11, 17, 12, 16, 13, 15, 14, 10, 15, 11, 16, 12,  //
                        17, 13, 10, 14, 11, 17, 12, 16, 13, 15, 14, 17, 15, 18, 16, 19,  //
                        14, 20, 17, 21, 16, 16, 15, 15, 10, 14, 11, 13, 12, 12, 13, 14,  //
                        //
                        27, 24, 26, 23, 25, 22, 24, 21, 21, 20, 18, 19, 19, 18, 20, 27,  //
                        21, 26, 22, 25, 23, 24, 24, 23, 25, 22, 26, 21, 27, 20, 18, 19,  //
                        19, 18, 20, 27, 22, 26, 23, 25, 24, 24, 25, 23, 26, 22, 27, 21,  //
                        18, 20, 19, 19, 20, 18, 21, 27, 22, 26, 23, 25, 24, 24, 25, 23,  //
                        26, 22, 27, 21, 18, 20, 19, 19, 20, 18, 21, 27, 22, 26, 23, 25,  //
                        34, 24, 35, 23, 36, 22, 37, 25, 32, 26, 31, 27, 30, 18, 29, 19,  //
                        28, 20, 28, 21, 29, 22, 30, 23, 31, 32, 33, 37, 33, 36, 34, 35,  //
                        35, 34, 36, 33, 37, 33, 32, 34, 31, 35, 30, 36, 29, 37, 28, 32,  //
                        //
                        25, 27, 26, 26, 27, 25, 18, 18, 19, 19, 20, 20, 21, 21, 22, 28,  //
                        24, 23, 26, 24, 27, 25, 19, 26, 21, 27, 23, 18, 25, 19, 27, 20,  //
                        19, 21, 21, 22, 22, 23, 24, 23, 26, 25, 18, 18, 20, 20, 22, 22,  //
                        24, 24, 26, 26, 18, 18, 20, 20, 22, 23, 24, 25, 18, 27, 24, 19,  //
                        30, 21, 31, 24, 32, 36, 37, 35, 36, 34, 35, 33, 34, 32, 29, 37,  //
                        28, 33, -1, 28, -1, 29, 29, 30, 30, 31, 31, 28, 37, 29, 36, 30,  //
                        35, 31, 34, 32, 33, 36, 32, 35, 34, 34, 35, 37, 36, 33, 37, 30,  //
                        33, 31, 32, 32, 31, 35, 30, 36, 29, 33, 28, 34, 28, 29, 29, 28,  //
                        //
                        19, 25, 21, 27, 23, 19, 25, 21, 27, 23, 18, 25, -1, 26, 21, 20,  //
                        23, 22, 24, 26, 26, 19, 18, 23, 19, 20, 21, 22, 23, 25, 25, 27,  //
                        27, 20, 19, 22, 21, 24, 22, 26, 24, 18, 26, 20, 18, 23, 20, 25,  //
                        22, 27, 24, 19, 26, 21, 18, 23, 20, 25, 21, 27, 23, 19, 24, 22,  //
                        32, 25, 30, 26, 28, 27, 28, 31, 29, 29, 30, 29, 37, 30, 36, 31,  //
                        35, 28, 34, 37, 33, 32, 32, 33, 31, 34, 27, 36, 28, 35, 37, 34,  //
                        36, 33, 35, 32, 34, 35, 33, 36, 32, 37, 31, 31, 28, 30, 37, 30,  //
                        35, 29, 33, 36, 31, 34, 30, 32, 29, 28, 37, 36, 35, 34, 33, 32,  //
                        //
                        36, 31, 35, 30, 34, 28, 33, 29, 32, 37, 31, 36, 30, 35, 29, 34,  //
                        36, 33, 34, 32, 32, 31, 30, 30, 29, 28, 36, 29, 35, 37, 33, 28,  //
                        31, 37, 33, 35, 39, 33, 38, 31, 37, 28, 36, 37, 35, 34, 49, 32,  //
                        48, 30, 47, 34, 46, 38, 45, 39, 44, 40, 38, 41, 39, 42, 40, 43,  //
                        39, 41, 40, 42, 42, 49, 48, 48, 46, 47, 44, 46, 38, 45, 40, 44,  //
                        42, 43, 48, 38, 46, 41, 45, 49, 43, 47, 39, 45, 41, 43, 49, 39,  //
                        47, 41, 45, 49, 43, 47, 39, 44, 41, 38, 42, 40, 49, 42, 47, 48,  //
                        45, 46, 43, 44, 40, 38, 41, 40, 42, 50, 43, 48, 49, 46, 48, 44,  //
                        //
                        44, 44, 45, 45, 46, 46, 43, 47, 45, 48, 46, 49, 47, 43, 48, 38,  //
                        49, 39, 43, 40, 41, 41, 39, 42, 45, 49, 47, 48, 49, 47, 43, 44,  //
                        41, 42, 39, 40, 38, 38, 39, 46, 41, 48, 49, 44, 47, 42, 45, 40,  //
                        43, 38, 55, 40, 54, 42, 53, 48, 52, 46, 61, 44, 60, 55, 59, 54,  //
                        61, 58, 59, 57, 57, 56, 50, 50, 52, 51, 54, 52, 61, 53, 59, 54,  //
                        57, 55, 50, 60, 52, 58, 54, 56, 61, 51, 59, 53, 57, 55, 56, 60,  //
                        51, 58, 53, 56, 55, 51, 60, 53, 58, 55, 56, 60, 51, 58, 53, 50,  //
                        55, 52, 67, 54, -1, 61, -1, 59, -1, 57, -1, 50, -1, 52, -1, 54,  //
                        //
                        45, 43, 44, 44, 43, 49, 38, 42, -2, 41, -2, 40, -2, 39, -2, 38,  //
                        -2, 39, -2, 40, 47, 41, -2, 42, -2, 49, -2, 48, 39, 47, 41, 46,  //
                        49, 44, 47, 38, 45, 40, 43, 42, 39, 48, 40, 46, 42, 44, 61, 38,  //
                        60, 41, 59, 61, 55, 60, 54, 59, 53, 58, 52, 57, 51, 56, 58, 50,  //
                        57, 52, 56, 53, 50, 54, 51, 55, 52, 51, 54, 50, 61, 56, 59, 57,  //
                        57, 58, 50, 59, 52, 60, 54, 61, 61, 55, 59, 53, 57, 55, 50, 60,  //
                        52, 58, 61, 56, 60, 51, 59, 53, 57, 55, 50, 60, -1, 58, -1, 56,  //
                        -1, 51, -1, 53, -1, 58, -1, 56, 67, 51, -1, 62, -1, 62, -1, 63,  //
                        //
                        43, 31, 42, 30, -1, 29, 40, 28, 39, 28, 38, 29, 44, 30, 45, 31,  //
                        46, 32, 47, 37, 48, 36, 49, 35, 49, 34, 48, 33, 47, 33, 46, 34,  //
                        45, 35, 44, 36, 38, 37, 39, 32, 40, 31, 41, 30, 42, 29, 43, 28,  //
                        49, 40, 48, 41, 47, 42, 46, 43, 45, 39, 44, 38, 38, 44, 47, 45,  //
                        39, 46, 40, 47, 41, 48, 42, 49, 49, 43, 48, 42, 46, 41, 45, 40,  //
                        44, 39, 43, 38, 37, 44, 38, 45, 45, 46, 46, 47, 47, 48, 48, 49,  //
                        -1, 43, -1, 42, -1, 41, -1, 40, -1, 39, -1, 38, -1, 44, -1, 45,  //
                        -1, 46, -1, 47, -1, 48, -1, 49, -1, 43, -1, 42, -1, 41, -1, 40,  //
                        //
                        58, 39, 57, 38, 56, 56, 50, 57, 51, 58, 52, 59, 53, 60, 54, 61,  //
                        55, 50, 61, 51, 60, 52, 59, 53, 58, 54, 57, 55, 56, 61, 50, 60,  //
                        51, 59, 52, 55, 53, 54, 54, 53, 55, 52, 61, 51, 60, 50, 59, 56,  //
                        58, 57, 57, 58, 56, 59, 50, 60, 51, -1, 52, 55, 53, 54, 54, 53,  //
                        70, 52, 71, 51, 72, 50, 73, 50, 65, 51, 64, 52, 63, 53, 62, 54,  //
                        68, 55, 69, 61, 70, 60, 69, 59, 68, 58, 67, 57, 66, 56, 62, 71,  //
                        63, 72, 64, 73, 69, 67, 68, 66, 67, 65, 73, 64, 72, 63, 71, 62,  //
                        70, 68, 66, 69, 65, 70, 68, 71, 69, 72, 70, 73, 71, 67, 72, 66,  //
                        //
                        72, 65, 73, 64, 70, 66, 65, 67, 63, 73, 67, 72, 70, 71, 72, 70,  //
                        66, 69, 65, 63, 64, 64, 62, 65, 69, 66, 71, 67, 73, 66, 66, 64,  //
                        64, 62, 62, 69, 69, 71, 71, 73, 73, 68, 66, 63, 75, 68, 74, 70,  //
                        74, 72, 77, 67, 76, 65, 75, 63, 76, 68, 77, 70, 78, 72, 82, 81,  //
                        84, 80, 83, 79, 82, 74, 81, 75, 80, 76, 79, 77, 78, 78, 82, 79,  //
                        83, 80, 84, 81, 85, 85, 81, 84, 80, 83, 79, 82, 78, -1, 77, -1,  //
                        76, 76, 75, 77, 74, 78, 74, 79, 75, 80, 76, 85, 86, 84, 87, 83,  //
                        88, 85, 89, 87, 90, 88, 91, -2, 92, 87, 91, 88, 90, 89, 89, 90,  //
                        //
                        64, 65, 66, 64, 73, 63, 71, 62, 69, 62, 65, 63, 67, 65, 70, 67,  //
                        69, 72, 79, 70, 78, 68, 77, 66, 76, 68, 77, 73, 78, 72, 79, 75,  //
                        80, 74, 81, 79, 82, 78, 84, 74, 84, 83, 83, 75, 82, 80, 81, 81,  //
                        80, 82, 74, 76, 75, 83, 76, 84, -1, 85, 77, 77, 85, 79, 78, 84,  //
                        83, 83, 85, 82, 78, 81, 76, 85, 74, 80, 75, 80, 77, 81, 78, 82,  //
                        84, 84, 81, 79, 79, 77, 75, 75, 76, 74, 77, 76, 88, 82, 87, 85,  //
                        86, 83, 86, 80, 87, 74, 88, 88, 89, 87, 89, 86, 90, 89, -1, 86,  //
                        91, 87, 92, 88, 92, 89, 90, 90, -1, 91, 92, 90, -1, 92, 91, 91,  //
                        //
                        60, 61, 58, 60, 56, 59, 51, 58, 53, 57, 55, 56, 63, 50, 62, 51,  //
                        68, 52, 69, 53, 70, 54, 71, 55, 72, 61, 73, 59, 67, 57, 66, 50,  //
                        67, 52, 65, 54, 64, 61, 63, 64, 62, 65, 68, 66, 69, 73, 70, 72,  //
                        71, 71, 72, 70, 73, 69, 67, 68, 66, 71, 65, 62, 64, 63, 63, 64,  //
                        82, 62, 81, 68, 80, 69, 80, 70, 81, 71, 82, 72, 83, 78, 84, 79,  //
                        85, 85, 79, 84, 77, 83, 89, 78, 75, 76, 74, 86, 80, 87, 81, 88,  //
                        88, 89, 87, -1, 86, -1, 86, -1, 87, -1, -1, -1, 89, -1, 90, -1,  //
                        90, -1, 91, -1, 92, -1, 91, -1, 92, -1, 91, -1, 92, -1, 93, -1,  //
                        //
                        -2, 60, 46, -2, -2, 58, 44, 57, 58, 56, 57, 50, 56, 51, 52, 52,  //
                        54, 53, 61, 54, 59, 51, 57, 53, 56, 55, 58, 60, 60, 58, 61, 56,  //
                        54, 57, 52, 59, 50, 55, 73, 53, 72, 51, 67, -2, 66, 67, 65, 66,  //
                        64, 65, 63, 64, 62, 63, 68, 62, 69, 68, 70, 69, 71, 70, 72, 71,  //
                        66, 73, 64, 67, 62, 66, 69, 65, 71, 64, 73, 63, 66, 62, 64, 68,  //
                        63, 69, 62, 70, 63, 71, 65, 72, 67, 73, 72, 67, 71, 65, 70, 63,  //
                        69, 68, 68, 70, 82, 72, 83, 67, 84, 65, 85, 62, 78, 64, 76, 66,  //
                        74, 73, 81, -1, 83, 77, -1, 75, 78, 80, 77, 82, 76, 84, 75, 79,  //
                        //
                        80, 81, -1, 82, 94, 83, 94, 84, 85, 79, 78, 77, 76, 75, 74, 74,  //
                        75, 76, 77, 78, 79, 85, 84, 83, 82, 81, 80, 74, 75, 76, 77, 78,  //
                        79, 85, 84, 83, 82, 81, 80, 74, 94, 75, 92, 76, 91, 77, 91, 78,  //
                        90, 79, 94, 85, 90, 84, 89, 83, 89, 82, 88, 81, 87, 80, 86, 74,  //
                        94, 93, 94, 92, 95, 93, 95, 91, 95, 92, 96, 90, 93, 89, 94, 88,  //
                        96, 87, 96, 93, 95, 85, 96, 91, 95, 90, 96, 89, 95, 92, 96, 88,  //
                        95, 87, 95, 86, 96, 90, 95, 91, 94, 92, 93, 93, 93, 91, 93, 89,  //
                        93, 88, 92, 87, 92, 86, 92, 90, 91, 89, 91, 88, 90, 87, 94, 86,  //
                        //
                        94, 93, -1, 90, -1, 89, -1, 89, -1, 88, -1, 88, -1, 87, -1, 88,  //
                        -1, 93, -1, 86, -1, 87, -1, 86, -1, 87, -1, 86, -1, -1, -1, -1,  //
                        -1, -1, -1, -1, 94, -1, 95, -1, 96, -1, 96, -1, 94, 93, 95, -1,  //
                        96, -1, 95, -1, 96, -1, 96, -1, 95, -1, 96, -1, 95, -1, 94, 93,  //
                        -1, -1, -1, -1, -1, -1, -1, 93, -1, -1, 94, -1, -1, -1, -1, -1,  //
                        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  //
                        -1, 93, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 95, -1, 96, 94,  //
                        96, 96, 95, 95, 96, 96, 95, 96, 94, 92, 94, 93, 93, 91, 94, 92,  //
                        //
                        73, 64, -2, 61, -2, -2, -2, 68, -2, 69, -2, 70, -2, -1, -2, 72,  //
                        75, 71, 75, -2, 74, 66, 76, 65, 77, 64, 78, 63, 79, 62, 80, -1,  //
                        81, 78, 85, 80, 84, 83, 85, 83, 82, 82, 75, 75, -1, 76, -2, 78,  //
                        -2, 80, -2, 85, -2, 83, -2, 82, -2, 75, -2, 74, -2, -1, -2, -1,  //
                        -2, -1, 92, -1, 93, -1, 91, -1, 90, -1, 89, -1, 88, -1, 87, -1,  //
                        86, -1, 86, -1, 87, -1, 88, -1, 89, -1, 90, 91, 91, 90, 92, 89,  //
                        94, 88, 93, 87, 94, 86, 91, 92, 89, 93, 87, 90, 86, 88, 87, 94,  //
                        89, 86, 91, 88, 95, 90, 94, 93, 95, 95, 95, 95, 96, 96, 96, 96};

    // Array which stores the corresponding x position of PAD of entire module B
    fChannelsToPadXB = {8, 6, 8, 6, 8, 6, 9, 6, 9, 6, 9, 6, 9, 7, 9, 7,                  //
                        9, 7, 10, 7, 10, 7, 10, 7, 10, 8, 11, 8, 11, 8, 11, 5,           //
                        12, 10, 12, 10, 12, 11, 13, 11, 13, 11, 13, 12, 14, 12, 14, 12,  //
                        14, 13, 14, 13, 15, 13, 15, 14, 15, 14, 15, 5, 15, 5, 16, 5,     //
                        16, 5, 16, 5, 16, 4, 16, 4, 17, 4, 17, 4, 17, 4, 17, 4,          //
                        17, 3, 18, 3, 18, 3, 18, 3, 18, 3, 18, 3, 19, 2, 19, 2,          //
                        19, 2, 19, 2, 19, 2, 20, 2, 20, 1, 20, 1, 20, 1, 20, 1,          //
                        21, 1, 21, 1, 21, 0, 21, 0, 21, 0, 22, 0, 22, 0, 21, 0,          //
                        //
                        17, 14, 17, 15, 18, 15, 18, 15, 18, 15, 18, 15, 18, 15, 18, 16,  //
                        19, 16, 19, 16, 19, 16, 19, 16, 19, 16, 19, 17, 20, 17, 20, 17,  //
                        20, 17, 20, 14, 20, 14, 20, 14, 21, 14, 21, 14, 21, 13, 21, 13,  //
                        21, 13, 21, 13, 22, 13, 22, 13, 22, 12, 22, 12, 22, 12, 22, 12,  //
                        22, 12, 22, 12, 22, 12, 13, 12, 13, 12, 13, 11, 13, 11, 13, 11,  //
                        14, 11, 14, 11, 14, 10, 14, 14, 15, 15, 15, 15, 15, 16, 16, 16,  //
                        16, 16, 17, 17, 17, 17, 17, 18, 18, 18, 18, 18, 19, 19, 19, 19,  //
                        19, 20, 20, 20, 20, 20, 21, 21, 21, 21, 21, 22, 22, 22, 22, 22,  //
                        //
                        7, 0, 7, 0, 7, 6, 8, 6, 8, 6, 8, 6, 8, 5, 8, 5,          //
                        8, 5, 9, 5, 9, 5, 9, 5, 9, 4, 9, 4, 9, 4, 10, 4,         //
                        10, 4, 10, 3, 10, 3, 10, 3, 10, 3, 11, 3, 11, 2, 11, 2,  //
                        11, 2, 11, 2, 11, 2, 12, 1, 12, 1, 7, 1, 7, 1, 7, 1,     //
                        6, 6, 6, 6, 6, 0, 6, 0, 7, 0, 7, 4, 7, 4, 7, 3,          //
                        7, 3, 6, 3, 8, 3, 8, 3, 8, 2, 8, 2, 8, 2, 9, 2,          //
                        9, 2, 9, 2, 9, 1, 9, 1, 10, 1, 10, 1, 10, 1, 10, 1,      //
                        5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 4, 0, 4, 0, 4, 0,          //
                        //
                        5, 4, 5, 4, 5, 4, 5, 5, 6, 5, 6, 4, 6, 4, 6, 4,      //
                        6, 4, 6, 3, 7, 3, 7, 3, 7, 3, 7, 3, 7, 3, 7, 3,      //
                        8, 2, 8, 2, 8, 2, 8, 2, 8, 2, 8, 2, 9, 1, 9, 1,      //
                        9, 1, 9, 1, 9, 1, 9, 1, 10, 0, 10, 0, 10, 0, 10, 0,  //
                        4, 2, 4, 2, 4, 2, 4, 2, 3, 2, 3, 2, 3, 2, 3, 2,      //
                        3, 2, 3, 1, 3, 1, 3, 1, 3, 1, 4, 1, 4, 1, 4, 1,      //
                        4, 1, 4, 1, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,      //
                        5, 0, 5, 0, 5, 0, 6, 0, 6, 0, 6, 0, 6, 0, 6, 0,      //
                        //
                        11, 8, 10, 8, 10, 8, 11, 8, 11, 8, 11, 7, 11, 7, 11, 8,          //
                        12, 8, 12, 7, 12, 8, 12, 8, 12, 7, 12, 7, 13, 7, 13, 7,          //
                        13, 7, 13, 7, 13, 6, 13, 6, 14, 6, 14, 6, 14, 9, 14, 9,          //
                        14, 9, 14, 9, 15, 9, 15, 9, 15, 9, 15, 9, 15, 9, 15, 10,         //
                        12, 11, 12, 11, 12, 11, 12, 11, 13, 11, 13, 11, 13, 12, 13, 12,  //
                        13, 12, 13, 12, 17, 12, 10, 13, 10, 13, 10, 13, 10, 8, 10, 8,    //
                        10, 8, 10, 8, 10, 8, 10, 8, 10, 8, 10, 11, 11, 11, 11, 11,       //
                        11, 10, 11, 10, 11, 10, 11, 10, 11, 10, 11, 10, 11, 10, 11, 10,  //
                        //
                        16, 14, 15, 14, 16, 14, 16, 14, 16, 14, 16, 14, 16, 14, 16, 14,  //
                        17, 14, 17, 15, 17, 15, 17, 15, 17, 15, 17, 15, 17, 15, 18, 15,  //
                        18, 15, 18, 15, 18, 16, 18, 16, 18, 16, 18, 16, 19, 16, 19, 16,  //
                        19, 16, 19, 16, 19, 16, 19, 17, 19, 17, 20, 17, 20, 17, 20, 17,  //
                        20, 17, 20, 17, 20, 17, 20, 18, 21, 18, 21, 18, 21, 18, 21, 18,  //
                        21, 18, 21, 18, 22, 18, 22, 18, 22, 19, 22, 19, 22, 19, 22, 19,  //
                        22, 19, 22, 19, 22, 19, 22, 19, 21, 19, 21, 20, 21, 20, 21, 20,  //
                        21, 20, 21, 20, 21, 20, 22, 20, 22, 20, 22, 20, 22, 21, 22, 21,  //
                        //
                        20, 19, 20, 19, 20, 20, 20, 20, 20, 20, 21, 20, 21, 16, 21, 16,  //
                        21, 16, 21, 16, 21, 16, 21, 16, 21, 16, 21, 16, 21, 16, 21, 16,  //
                        22, 16, 22, 16, 22, 17, 22, 17, 22, 17, 22, 17, 22, 17, 22, 17,  //
                        22, 17, 22, 17, 22, 17, 22, 17, 22, 17, 22, 17, 22, 18, 22, 18,  //
                        19, 19, 19, 19, 19, 19, 20, 19, 20, 19, 20, 19, 20, 19, 20, 20,  //
                        20, 20, 21, 20, 21, 20, 21, 20, 21, 20, 21, 21, 21, 21, 21, 21,  //
                        22, 21, 22, 21, 22, 22, 22, 22, 22, 14, 22, 14, 22, 14, 22, 14,  //
                        22, 14, 22, 14, 22, 14, 22, 14, 22, 14, 22, 14, 22, 13, 22, 14,  //
                        //
                        16, 15, 16, 15, 16, 15, 16, 15, 17, 15, 17, 15, 17, 15, 17, 15,  //
                        17, 15, 17, 16, 17, 16, 17, 16, 18, 16, 18, 16, 18, 16, 18, 16,  //
                        18, 17, 18, 17, 19, 17, 19, 18, 19, 18, 19, 18, 19, 18, 20, 18,  //
                        12, 19, 12, 19, 13, 19, 13, 19, 13, 20, 13, 11, 13, 11, 13, 11,  //
                        13, 11, 14, 11, 14, 11, 14, 12, 14, 11, 14, 13, 14, 12, 14, 12,  //
                        14, 12, 14, 12, 14, 12, 14, 12, 15, 12, 15, 12, 15, 12, 15, 13,  //
                        15, 13, 15, 13, 15, 14, 15, 15, 18, 15, 18, 15, 18, 15, 18, 10,  //
                        18, 10, 18, 10, 18, 10, 18, 10, 18, 10, 18, 10, 19, 10, 19, 10,  //
                        //
                        12, 8, 12, 8, 12, 8, 12, 7, 12, 7, 12, 7, 12, 7, 12, 7,      //
                        12, 7, 12, 7, 13, 7, 13, 7, 13, 6, 13, 6, 13, 8, 13, 6,      //
                        13, 6, 13, 9, 13, 9, 13, 9, 13, 9, 14, 9, 14, 9, 14, 9,      //
                        14, 9, 14, 9, 14, 9, 14, 9, 14, 11, 14, 12, 15, 14, 15, 14,  //
                        8, 7, 8, 7, 9, 7, 9, 7, 9, 8, 9, 8, 9, 8, 9, 8,              //
                        9, 8, 9, 7, 9, 7, 9, 8, 9, 8, 9, 8, 10, 8, 10, 8,            //
                        10, 7, 10, 7, 10, 7, 10, 7, 10, 7, 10, 7, 10, 6, 10, 6,      //
                        10, 6, 10, 6, 11, 6, 13, 6, 11, 6, 11, 6, 11, 6, 11, 6,      //
                        //
                        4, 3, 4, 3, 5, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3,  //
                        4, 3, 4, 3, 4, 2, 4, 2, 3, 2, 5, 2, 5, 2, 5, 2,  //
                        5, 2, 5, 2, 5, 2, 5, 2, 5, 2, 5, 1, 5, 1, 6, 1,  //
                        7, 1, 6, 1, 7, 1, 6, 1, 6, 1, 6, 1, 6, 1, 6, 1,  //
                        0, 0, 0, 0, 0, 0, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3,  //
                        4, 3, 4, 3, 4, 3, 3, 3, 3, 2, 3, 2, 4, 2, 4, 2,  //
                        4, 2, 4, 2, 5, 2, 5, 2, 5, 2, 5, 2, 5, 2, 5, 2,  //
                        5, 1, 5, 1, 5, 1, 5, 1, 5, 1, 5, 1, 6, 1, 6, 1,  //
                        //
                        1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 3, 0, 3, 2, 3, 2,  //
                        3, 2, 3, 2, 2, 2, 2, 2, 2, 1, 2, 1, 2, 1, 2, 1,  //
                        2, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1,  //
                        3, 1, 4, 0, 4, 0, 4, 0, 4, 0, 4, 0, 4, 0, 4, 0,  //
                        0, 0, 0, 0, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 1,  //
                        0, 1, 0, 1, 0, 1, 0, 1, 3, 1, 2, 1, 2, 1, 2, 1,  //
                        3, 1, 3, 1, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0,  //
                        3, 0, 3, 0, 4, 0, 4, 0, 4, 0, 4, 2, 4, 2, 4, 2,  //
                        //
                        7, 6, 7, 6, 7, 6, 7, 6, 7, 6, 7, 6, 7, 6, 7, 6,  //
                        7, 6, 7, 6, 7, 6, 7, 6, 7, 6, 8, 5, 8, 5, 8, 5,  //
                        8, 5, 8, 5, 8, 5, 8, 5, 8, 5, 8, 5, 8, 5, 8, 5,  //
                        8, 5, 8, 5, 9, 4, 9, 4, 9, 4, 9, 4, 9, 4, 9, 4,  //
                        6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 7, 5, 7, 5, 7, 5,  //
                        7, 5, 7, 5, 7, 5, 7, 4, 7, 4, 7, 4, 7, 4, 7, 4,  //
                        7, 6, 7, 6, 7, 6, 7, 6, 7, 6, 7, 6, 7, 6, 7, 6,  //
                        7, 6, 7, 6, 8, 6, 8, 6, 8, 6, 8, 6, 8, 6, 7, 6,  //
                        //
                        11, 9, 11, 9, 11, 9, 11, 9, 11, 9, 12, 9, 12, 9, 12, 10,         //
                        12, 10, 12, 10, 12, 10, 12, 11, 12, 11, 12, 11, 12, 11, 12, 11,  //
                        12, 11, 12, 11, 13, 11, 13, 9, 13, 9, 13, 9, 13, 9, 13, 9,       //
                        13, 9, 13, 9, 13, 9, 13, 9, 14, 9, 13, 9, 13, 10, 14, 10,        //
                        11, 8, 11, 8, 11, 8, 11, 8, 11, 8, 11, 8, 11, 8, 11, 8,          //
                        11, 8, 11, 8, 11, 8, 12, 10, 12, 10, 12, 10, 12, 10, 12, 10,     //
                        12, 10, 12, 10, 12, 10, 10, 10, 10, 9, 10, 9, 10, 9, 10, 9,      //
                        10, 9, 10, 9, 10, 9, 10, 9, 10, 9, 11, 9, 11, 10, 11, 9,         //
                        //
                        16, 15, 16, 15, 16, 15, 16, 15, 16, 15, 16, 15, 16, 15, 16, 15,  //
                        16, 15, 17, 15, 17, 15, 17, 15, 17, 15, 17, 16, 17, 16, 18, 16,  //
                        18, 16, 18, 17, 18, 17, 18, 17, 18, 17, 18, 17, 19, 17, 19, 17,  //
                        19, 18, 19, 18, 14, 18, 14, 18, 14, 18, 14, 18, 14, 19, 14, 19,  //
                        14, 12, 14, 12, 14, 12, 14, 13, 14, 13, 14, 13, 15, 13, 14, 13,  //
                        13, 13, 13, 13, 13, 13, 13, 13, 14, 13, 14, 13, 14, 13, 14, 13,  //
                        14, 12, 14, 12, 14, 12, 14, 12, 16, 12, 15, 12, 15, 12, 14, 12,  //
                        15, 12, 15, 12, 15, 12, 15, 13, 15, 13, 15, 13, 15, 13, 16, 13,  //
                        //
                        20, 19, 20, 19, 20, 19, 20, 19, 20, 19, 20, 19, 20, 19, 20, 20,  //
                        20, 20, 20, 20, 21, 16, 21, 16, 21, 16, 21, 16, 21, 16, 21, 16,  //
                        21, 16, 21, 16, 21, 16, 21, 16, 21, 16, 21, 17, 21, 17, 22, 17,  //
                        22, 17, 22, 17, 22, 17, 22, 17, 22, 17, 22, 17, 22, 17, 22, 17,  //
                        18, 15, 18, 15, 18, 15, 18, 15, 18, 15, 19, 15, 19, 15, 19, 15,  //
                        19, 15, 19, 15, 19, 15, 19, 18, 20, 18, 20, 18, 20, 18, 20, 18,  //
                        20, 18, 20, 19, 21, 19, 21, 19, 21, 19, 21, 20, 21, 20, 21, 20,  //
                        21, 20, 22, 20, 22, 21, 22, 21, 22, 21, 22, 21, 22, 17, 22, 17,  //
                        //
                        22, 22, 21, 21, 21, 22, 22, 22, 21, 22, 22, 22, 22, 22, 22, 22,  //
                        22, 22, 22, 22, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 20,  //
                        18, 20, 19, 20, 20, 20, 20, 20, 21, 20, 21, 20, 21, 20, 20, 20,  //
                        20, 20, 20, 20, 17, 19, 19, 19, 18, 19, 19, 19, 19, 19, 19, 19,  //
                        16, 19, -1, 19, 20, 19, -1, 19, -1, 19, -1, 18, 19, 18, 18, 18,  //
                        -1, 18, 17, 15, 15, 18, 16, 18, -1, 18, 20, 18, 19, 18, 18, 18,  //
                        17, 18, 16, 17, -1, 17, 15, 17, 15, 17, -1, 17, 14, 14, 14, 17,  //
                        14, 17, 13, 17, 13, 16, 17, 16, 16, 16, 16, 16, 16, 16, 15, 16,  //
                        //
                        12, 18, 12, 17, 13, 18, 12, 17, -1, 12, -1, 16, -1, 18, -1, 17,  //
                        -1, -1, 11, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, 15, -1, -1,  //
                        11, 14, -1, 16, -1, 15, -1, 16, -1, 13, -1, 12, -1, 10, -1, -1,  //
                        -1, 11, -1, 11, -1, 11, -1, 11, -1, 11, 16, 11, 17, 11, 13, 11,  //
                        -1, 9, -1, 11, -1, -1, -1, -1, -1, 8, -1, -1, -1, -1, -1, -1,    //
                        -1, -1, -1, -1, -1, -1, -1, -1, -1, 14, -1, 13, -1, 15, -1, 14,  //
                        -1, 13, -1, 7, -1, 12, -1, 10, -1, 9, 10, 11, 11, 12, 9, 10,     //
                        8, 11, 7, 8, -1, 9, 10, 10, 10, 9, 9, 8, 9, 7, 6, 6,             //
                        //
                        3, 0, 4, 0, 4, 0, 4, 0, 4, 0, 4, 0, 4, 0, 4, 0,  //
                        4, 3, 4, 3, 4, 3, 4, 3, 5, 3, 5, 3, 5, 3, 5, 3,  //
                        5, 3, 5, 3, 5, 2, 5, 2, 5, 2, 5, 2, 5, 2, 6, 2,  //
                        8, 2, 8, 2, 8, 2, 8, 2, 8, 2, 8, 1, 1, 1, 1, 1,  //
                        5, 1, 5, 1, 4, 1, 5, 1, 3, 1, 6, 0, 6, 0, 4, 0,  //
                        7, 1, 8, 0, 7, 0, 6, 1, 5, 0, 3, 1, 4, 0, 5, 1,  //
                        8, 2, 8, 3, 7, 3, 7, 3, 6, 4, 6, 4, 5, 2, 5, 3,  //
                        4, 2, 3, 2, 4, 2, -1, 2, -1, 2, -1, 1, -1, 1, 1, 1};

    // Array which stores the corresponding y position of PAD of entire module B
    fChannelsToPadYB = {0, 2, 1, 3, 2, 4, 5, 5, 4, 1, 3, 0, 2, 5, 1, 4,  //
                        0, 3, 5, 2, 4, 1, 2, 0, 0, 5, 4, 4, 2, 3, 0, 0,  //
                        4, 3, 2, 1, 0, 5, 4, 3, 3, 1, 1, 5, 5, 3, 3, 1,  //
                        2, 5, 0, 2, 4, 0, 3, 4, 2, 1, 1, 1, 0, 2, 4, 3,  //
                        3, 4, 2, 5, 1, 0, 0, 1, 4, 2, 3, 3, 2, 4, 1, 5,  //
                        0, 0, 4, 1, 3, 2, 2, 3, 1, 4, 0, 5, 4, 0, 3, 1,  //
                        2, 2, 1, 3, 0, 4, 4, 5, 3, 0, 2, 1, 1, 2, 0, 3,  //
                        5, 4, 4, 5, 3, 0, 2, 1, 1, 3, 1, 5, 0, 7, 0, 6,  //
                        //
                        9, 11, 10, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 5,              //
                        5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 5, 5, 6, 6, 7,                //
                        7, 8, 8, 10, 9, 9, 10, 8, 6, 7, 7, 6, 8, 11, 9, 10,              //
                        10, 9, 11, 8, 2, 7, 3, 6, 4, 11, 5, 10, 6, 9, 7, 8,              //
                        10, 12, 9, 13, 8, 14, 16, 15, 15, 16, 14, 12, 13, 13, 12, 14,    //
                        16, 15, 15, 16, 14, 12, 12, 13, 14, 15, 12, 13, 11, 15, 14, 13,  //
                        12, 11, 15, 14, 13, 12, 11, 15, 14, 13, 12, 11, 15, 14, 13, 12,  //
                        11, 15, 14, 13, 12, 11, 16, 15, 14, 13, 12, 14, 15, 13, 12, 11,  //
                        //
                        9, 4, 10, 2, 11, 6, 6, 7, 7, 8, 8, 9, 9, 11, 10, 10,             //
                        11, 9, 6, 8, 7, 7, 8, 6, 9, 10, 10, 9, 11, 8, 6, 7,              //
                        7, 6, 8, 10, 9, 9, 10, 8, 11, 7, 6, 6, 7, 10, 8, 9,              //
                        9, 8, 10, 7, 11, 6, 6, 10, 7, 9, 8, 8, 7, 7, 6, 6,               //
                        15, 10, 14, 11, 13, 10, 12, 9, 16, 8, 15, 14, 14, 15, 13, 11,    //
                        12, 12, 16, 13, 16, 14, 15, 15, 14, 11, 13, 12, 12, 13, 16, 14,  //
                        15, 15, 14, 16, 13, 11, 12, 12, 16, 13, 15, 14, 14, 15, 13, 16,  //
                        12, 14, 13, 15, 14, 16, 15, 17, 16, 18, 11, 13, 12, 12, 13, 11,  //
                        //
                        22, 20, 21, 21, 20, 22, 19, 17, 17, 18, 18, 19, 19, 18, 20, 17,  //
                        21, 16, 22, 22, 17, 21, 18, 20, 19, 19, 20, 18, 21, 17, 22, 16,  //
                        17, 22, 18, 21, 19, 20, 20, 19, 21, 18, 22, 17, 17, 22, 18, 21,  //
                        19, 20, 20, 19, 21, 18, 22, 17, 17, 22, 18, 21, 19, 20, 20, 19,  //
                        28, 23, 29, 24, 30, 25, 31, 26, 23, 27, 24, 28, 25, 29, 26, 30,  //
                        27, 31, 28, 23, 29, 24, 30, 25, 31, 26, 27, 27, 26, 28, 25, 29,  //
                        24, 30, 23, 31, 31, 23, 30, 24, 29, 25, 28, 26, 27, 27, 26, 28,  //
                        25, 29, 24, 30, 23, 31, 26, 36, 31, 35, 30, 34, 29, 33, 28, 32,  //
                        //
                        17, 23, 22, 24, 21, 25, 18, 26, 19, 27, 20, 23, 21, 24, 22, 28,  //
                        17, 29, 18, 25, 19, 30, 20, 31, 21, 26, 22, 28, 17, 29, 18, 27,  //
                        19, 30, 20, 31, 21, 23, 22, 24, 17, 25, 18, 27, 19, 31, 20, 30,  //
                        21, 29, 22, 28, 16, 27, 17, 26, 18, 25, 19, 24, 20, 23, 21, 31,  //
                        29, 28, 27, 27, 25, 26, 23, 25, 30, 24, 28, 23, 26, 31, 25, 30,  //
                        24, 28, 23, 26, 26, 24, 34, 31, 35, 29, 33, 27, 32, 42, 36, 41,  //
                        37, 40, 38, 39, 39, 38, 40, 37, 41, 36, 42, 29, 32, 30, 33, 31,  //
                        34, 23, 35, 24, 36, 25, 37, 26, 38, 27, 39, 28, 40, 29, 41, 30,  //
                        //
                        16, 28, 22, 27, 17, 26, 18, 29, 19, 30, 20, 31, 21, 25, 22, 24,  //
                        16, 23, 17, 31, 18, 30, 19, 29, 20, 28, 21, 27, 22, 26, 16, 25,  //
                        17, 24, 18, 23, 19, 31, 20, 30, 21, 29, 22, 28, 16, 27, 17, 26,  //
                        18, 25, 19, 24, 20, 23, 21, 31, 22, 30, 16, 29, 17, 28, 18, 27,  //
                        19, 25, 20, 24, 21, 23, 22, 31, 17, 30, 18, 29, 19, 28, 20, 27,  //
                        21, 26, 22, 25, 16, 24, 17, 23, 18, 31, 19, 30, 20, 29, 21, 28,  //
                        25, 27, 24, 26, 23, 25, 22, 24, 29, 23, 28, 31, 27, 30, 26, 29,  //
                        25, 28, 24, 27, 23, 26, 30, 25, 29, 24, 28, 23, 27, 31, 26, 30,  //
                        //
                        38, 40, 39, 42, 40, 33, 41, 35, 42, 36, 32, 37, 33, 49, 34, 48,  //
                        35, 47, 36, 46, 37, 45, 38, 44, 39, 43, 40, 50, 41, 51, 42, 52,  //
                        32, 53, 33, 54, 34, 43, 35, 44, 36, 45, 37, 46, 38, 47, 39, 48,  //
                        40, 49, 41, 50, 42, 51, 46, 52, 45, 53, 44, 54, 43, 43, 31, 44,  //
                        51, 44, 52, 45, 53, 46, 43, 48, 45, 49, 47, 50, 49, 54, 51, 44,  //
                        53, 46, 43, 48, 45, 50, 46, 52, 47, 54, 49, 44, 51, 48, 53, 50,  //
                        47, 52, 49, 54, 51, 48, 52, 50, 53, 65, 54, 63, 55, 67, 56, 62,  //
                        57, 61, 58, 60, 64, 59, 63, 58, 62, 57, 61, 56, 60, 55, 59, 55,  //
                        //
                        39, 34, 40, 35, 41, 36, 42, 37, 32, 38, 33, 39, 34, 40, 35, 41,  //
                        36, 42, 38, 32, 39, 33, 41, 34, 32, 35, 34, 36, 36, 37, 38, 38,  //
                        40, 37, 42, 40, 32, 42, 34, 33, 36, 35, 38, 37, 39, 39, 32, 41,  //
                        51, 33, 52, 35, 45, 37, 46, 41, 47, 34, 48, 54, 49, 53, 50, 51,  //
                        54, 50, 43, 49, 44, 48, 45, 43, 46, 52, 47, 44, 48, 44, 49, 45,  //
                        50, 46, 51, 47, 52, 48, 54, 49, 44, 50, 46, 53, 48, 54, 49, 51,  //
                        50, 52, 52, 53, 53, 53, 54, 43, 45, 45, 46, 47, 47, 51, 48, 65,  //
                        49, 66, 50, 67, 51, 64, 52, 63, 53, 62, 54, 61, 43, 60, 47, 59,  //
                        //
                        33, 35, 34, 34, 35, 33, 36, 40, 37, 42, 38, 41, 39, 39, 40, 34,  //
                        41, 35, 42, 36, 32, 37, 33, 38, 34, 37, 35, 38, 36, 32, 37, 42,  //
                        38, 41, 39, 32, 40, 33, 41, 34, 42, 35, 32, 36, 33, 37, 34, 38,  //
                        35, 39, 36, 40, 37, 41, 39, 42, 41, 42, 42, 32, 32, 38, 33, 40,  //
                        53, 52, 54, 51, 43, 53, 44, 54, 45, 43, 46, 44, 47, 45, 48, 46,  //
                        49, 47, 50, 50, 51, 49, 52, 48, 53, 49, 54, 50, 43, 51, 44, 52,  //
                        45, 48, 46, 47, 47, 46, 48, 45, 49, 44, 50, 43, 51, 54, 52, 53,  //
                        53, 52, 54, 51, 43, 50, 43, 49, 44, 48, 45, 43, 46, 44, 47, 46,  //
                        //
                        42, 41, 41, 40, 32, 39, 40, 38, 39, 37, 38, 36, 37, 35, 36, 34,  //
                        35, 33, 34, 32, 33, 42, 32, 41, 42, 40, 33, 39, 34, 38, 35, 37,  //
                        36, 36, 37, 35, 38, 34, 39, 33, 40, 32, 41, 42, 42, 41, 32, 40,  //
                        32, 39, 33, 38, 33, 37, 34, 36, 35, 35, 36, 34, 39, 33, 40, 32,  //
                        42, 39, 41, 38, 40, 37, 50, 51, 49, 50, 48, 49, 47, 48, 46, 47,  //
                        45, 46, 44, 45, 43, 44, 54, 43, 53, 54, 52, 53, 51, 52, 52, 51,  //
                        53, 50, 54, 49, 43, 48, 44, 47, 45, 46, 46, 45, 47, 44, 48, 43,  //
                        49, 54, 50, 53, 51, 52, 52, 51, 53, 50, 54, 49, 45, 48, 47, 47,  //
                        //
                        46, 48, 45, 47, 44, 46, 43, 45, 49, 44, 63, 43, 64, 62, 65, 63,  //
                        66, 64, 67, 65, 55, 66, 56, 67, 57, 55, 58, 56, 59, 57, 60, 58,  //
                        61, 59, 62, 60, 61, 61, 60, 62, 59, 63, 58, 64, 57, 65, 56, 66,  //
                        55, 67, 67, 58, 66, 59, 65, 60, 64, 61, 63, 62, 62, 63, 61, 64,  //
                        65, 51, 66, 50, 67, 76, 68, 77, 69, 78, 70, 75, 57, 74, 56, 68,  //
                        55, 69, 54, 70, 53, 71, 52, 72, 78, 73, 68, 74, 69, 75, 70, 76,  //
                        77, 77, 76, 78, 75, 71, 74, 72, 73, 73, 72, 74, 71, 75, 70, 76,  //
                        69, 77, 68, 78, 78, 79, 77, 80, 76, 81, 75, 73, 74, 72, 73, 71,  //
                        //
                        67, 66, 66, 67, 65, 65, 64, 64, 63, 63, 62, 62, 61, 61, 60, 60,  //
                        59, 59, 58, 58, 57, 57, 56, 56, 55, 55, 67, 55, 66, 56, 65, 57,  //
                        64, 58, 63, 59, 62, 60, 61, 61, 60, 62, 59, 63, 58, 64, 57, 65,  //
                        56, 66, 55, 67, 67, 55, 66, 56, 65, 57, 64, 58, 63, 59, 62, 60,  //
                        72, 69, 71, 70, 70, 71, 69, 68, 68, 72, 78, 73, 77, 74, 76, 75,  //
                        75, 76, 74, 77, 73, 78, 72, 68, 71, 69, 70, 70, 69, 71, 68, 72,  //
                        83, 78, 84, 77, 82, 76, 81, 75, 80, 74, 79, 73, 85, 83, 86, 82,  //
                        87, 81, 88, 80, 79, 79, 80, 84, 81, 85, 82, 86, 83, 87, 89, 88,  //
                        //
                        59, 56, 58, 57, 57, 58, 56, 59, 55, 60, 67, 61, 66, 55, 65, 58,  //
                        64, 57, 63, 56, 62, 55, 61, 67, 60, 66, 59, 65, 58, 64, 57, 63,  //
                        56, 62, 55, 61, 67, 60, 66, 72, 65, 71, 64, 70, 63, 69, 62, 68,  //
                        61, 73, 60, 74, 59, 75, 58, 76, 64, 77, 57, 78, 56, 78, 66, 77,  //
                        78, 68, 77, 69, 76, 70, 75, 71, 74, 72, 73, 73, 72, 74, 71, 75,  //
                        70, 76, 69, 77, 68, 78, 78, 76, 77, 75, 76, 74, 75, 73, 74, 72,  //
                        73, 71, 72, 70, 71, 69, 86, 68, 87, 87, 88, 86, 89, 85, 85, 84,  //
                        84, 83, 82, 82, 83, 81, 81, 80, 80, 79, 79, 88, 80, 79, 81, 89,  //
                        //
                        66, 67, 65, 66, 64, 65, 63, 64, 62, 63, 60, 62, 58, 61, 56, 60,  //
                        55, 59, 66, 58, 64, 57, 62, 56, 60, 55, 58, 67, 56, 61, 67, 59,  //
                        65, 57, 63, 67, 61, 65, 60, 63, 58, 61, 56, 59, 67, 57, 65, 55,  //
                        63, 66, 62, 64, 78, 62, 77, 59, 76, 57, 75, 55, 74, 66, 73, 64,  //
                        72, 70, 71, 69, 70, 68, 69, 78, 68, 77, 79, 76, 79, 75, 80, 74,  //
                        88, 73, 87, 72, 86, 71, 85, 70, 81, 69, 82, 68, 83, 80, 84, 79,  //
                        85, 89, 86, 88, 88, 87, 87, 86, 79, 85, 80, 84, 81, 83, 89, 82,  //
                        82, 81, 83, 80, 84, 79, 85, 81, 86, 82, 87, 83, 88, 84, 80, 89,  //
                        //
                        64, 61, 63, 60, 62, 59, 61, 58, 60, 57, 59, 56, 58, 55, 57, 67,  //
                        56, 66, 55, 65, 67, 74, 66, 75, 65, 73, 64, 72, 63, 71, 62, 70,  //
                        61, 76, 60, 77, 59, 78, 58, 69, 57, 68, 56, 78, 55, 77, 73, 76,  //
                        72, 75, 71, 74, 70, 73, 69, 72, 68, 71, 67, 70, 66, 69, 65, 68,  //
                        72, 71, 71, 72, 70, 73, 69, 74, 68, 75, 78, 76, 77, 77, 76, 78,  //
                        75, 70, 73, 69, 71, 68, 69, 78, 78, 77, 76, 76, 74, 75, 72, 74,  //
                        70, 73, 68, 74, 77, 72, 75, 70, 74, 68, 72, 77, 70, 75, 69, 73,  //
                        68, 71, 80, 69, 79, 78, 78, 76, 77, 73, 76, 71, 75, 79, 74, 80,  //
                        //
                        96, 93, 96, 93, 95, 92, 95, 91, 94, 90, 94, 89, 88, 87, 86, 85,  //
                        84, 83, 82, 81, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 89,  //
                        94, 88, 94, 87, 94, 86, 93, 85, 92, 84, 91, 83, 90, 82, 92, 81,  //
                        91, 80, 90, 79, 94, 89, 93, 88, 93, 87, 92, 86, 91, 85, 90, 84,  //
                        94, 83, -1, 82, 95, 81, -1, 80, -1, 79, -1, 89, 95, 88, 95, 87,  //
                        -1, 86, 95, 93, 94, 85, 95, 84, -1, 83, 96, 82, 96, 81, 96, 80,  //
                        96, 79, 96, 89, -1, 88, 95, 87, 96, 86, -1, 85, 94, 93, 95, 83,  //
                        96, 82, 96, 81, 94, 88, 84, 86, 89, 85, 87, 83, 84, 82, 89, 81,  //
                        //
                        94, 91, 95, 92, 95, 92, 96, 91, -1, 93, -1, 92, -1, 90, -1, 90,  //
                        -1, -1, 94, 93, -1, -1, -1, -1, -1, -1, -1, -1, -1, 92, -1, -1,  //
                        95, 92, -1, 91, -1, 91, -1, 90, -1, 92, -1, 92, -1, 93, -1, -1,  //
                        -1, 89, -1, 88, -1, 87, -1, 86, -1, 85, 93, 84, 93, 83, 93, 82,  //
                        -1, 93, -1, 92, -1, -1, -1, -1, -1, 93, -1, -1, -1, -1, -1, -1,  //
                        -1, -1, -1, -1, -1, -1, -1, -1, -1, 91, -1, 91, -1, 90, -1, 90,  //
                        -1, 90, -1, 93, -1, 91, -1, 92, -1, 92, 94, 91, 96, 90, 94, 91,  //
                        94, 90, 94, 92, -1, 91, 95, 90, 96, 90, 95, 91, 96, 92, 94, 93,  //
                        //
                        89, 85, 79, 86, 80, 84, 81, 83, 82, 82, 83, 87, 84, 88, 85, 89,  //
                        86, 79, 87, 80, 88, 81, 89, 82, 79, 83, 80, 84, 81, 85, 82, 86,  //
                        83, 87, 84, 88, 85, 89, 86, 88, 87, 87, 88, 86, 89, 85, 89, 84,  //
                        84, 83, 85, 82, 86, 81, 87, 80, 88, 79, 89, 89, 86, 88, 85, 87,  //
                        90, 84, 91, 83, 92, 82, 92, 81, 93, 80, 90, 90, 91, 91, 93, 92,  //
                        90, 93, 90, 93, 91, 94, 92, 94, 93, 95, 94, 95, 94, 96, 94, 96,  //
                        95, 94, 96, 90, 95, 91, 96, 92, 95, 90, 96, 91, 95, 95, 96, 96,  //
                        95, 96, 95, 93, 96, 92, -1, 91, -1, 90, -1, 90, -1, 91, 79, 92};
  }

} /* namespace cbm::algo */
