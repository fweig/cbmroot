/* Copyright (C) 2017-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau, Ajit Kumar, Florian Uhlig [committer] */

#include "MuchReadoutConfig.h"

#include "CbmMuchAddress.h"

//#include "FairDetParIo.h"
//#include "FairParIo.h"
//#include "FairParamList.h"
#include <Logger.h>
#include <bitset>

//#include "TMath.h"
//#include "TString.h"

using namespace std;

namespace cbm::algo
{

  /*
// -----   Public method clear   -------------------------------------------
void MuchReadoutConfig::clear()
{
  status = false;
  resetInputVersions();
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------

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

// -------------------------------------------------------------------------

bool MuchReadoutConfig::getParams(FairParamList* l)
{

  if (!l) return false;

  if (!l->fill("NrOfDpbs", &numComp)) return false;

  eqId.Set(numComp);
  if (!l->fill("DbpIdArray", &eqId)) return false;

  fiCrobActiveFlag.Set(numComp * kuNbCrobsPerDpb);
  if (!l->fill("CrobActiveFlag", &fiCrobActiveFlag)) return false;

  if (!l->fill("NrOfFebsInGemA", &fuFebsInGemA)) return false;

  fnFebsIdsArrayGemA.Set(GetNrOfFebsInGemA());
  if (!l->fill("nFebsIdsArrayA", &fnFebsIdsArrayGemA)) return false;

  if (!l->fill("NrOfFebsInGemB", &fuFebsInGemB)) return false;

  fnFebsIdsArrayGemB.Set(GetNrOfFebsInGemB());
  if (!l->fill("nFebsIdsArrayB", &fnFebsIdsArrayGemB)) return false;

  if (!l->fill("NrOfFebsInRpc", &fuFebsInRpc)) return false;

  fnFebsIdsArrayRpc.Set(GetNrOfFebsInRpc());
  if (!l->fill("nFebsIdsArrayRpc", &fnFebsIdsArrayRpc)) return false;

  fChannelsToPadXA.Set(GetNrOfFebs() * kuNbChanPerAsic);
  if (!l->fill("ChannelsToPadXA", &fChannelsToPadXA)) return false;

  fChannelsToPadYA.Set(GetNrOfFebs() * kuNbChanPerAsic);
  if (!l->fill("ChannelsToPadYA", &fChannelsToPadYA)) return false;

  fChannelsToPadXB.Set(GetNrOfFebs() * kuNbChanPerAsic);
  if (!l->fill("ChannelsToPadXB", &fChannelsToPadXB)) return false;

  fChannelsToPadYB.Set(GetNrOfFebs() * kuNbChanPerAsic);
  if (!l->fill("ChannelsToPadYB", &fChannelsToPadYB)) return false;

  fChannelsToPadXRpc.Set(GetNrOfFebsInRpc() * kuNbChanPerAsic);
  if (!l->fill("ChannelsToPadXRpc", &fChannelsToPadXRpc)) return false;

  fChannelsToPadYRpc.Set(GetNrOfFebsInRpc() * kuNbChanPerAsic);
  if (!l->fill("ChannelsToPadYRpc", &fChannelsToPadYRpc)) return false;

  fRealX.Set(2232);  // Number of Sectors in one GEM Module
  if (!l->fill("RealX", &fRealX)) return false;

  fRealPadSize.Set(2232);  // Number of Sectors in one GEM Module
  if (!l->fill("PadSize", &fRealPadSize)) return false;

  return true;
}
*/

  void MuchReadoutConfig::Init()
  {
    // This here refers to the mCBM 2021 setup.
    // Taken from CbmMuchUnpackPar in combination with macro/beamtime/mcbm2021/mSts.par

    // Array to hold the unique IDs (equipment ID) for all MUCH DPBs
    uint16_t eqId[numComp] = {0x18e3, 0x18ef, 0x1861, 0x6601, 0x4f19, 0x5b7b};
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
  int16_t MuchReadoutConfig::ElinkIdxToFebIdx(uint16_t uElink)
  {
    if (uElink < kuNbElinksPerCrob) return kiCrobMapElinkFebIdx[uElink];
    else {
      LOG(warning) << "MuchReadoutConfig::ElinkIdxToFebIdx => Index out of bound, "
                   << "Elink is " << uElink << " returning crazy value!";
      return -1;
    }
  }

  bool MuchReadoutConfig::IsCrobActive(uint16_t uDpbIdx, uint16_t uCrobIdx)
  {
    if (uDpbIdx < numComp) {
      if (uCrobIdx < kuNbCrobsPerDpb) return 0 < fiCrobActiveFlag[uDpbIdx * kuNbCrobsPerDpb + uCrobIdx] ? true : false;
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
      if (uCrobIdx < kuNbCrobsPerDpb) {
        if (uFebIdx < kuNbFebsPerCrob) {
          uint16_t uIdx = (uDpbIdx * kuNbCrobsPerDpb + uCrobIdx) * kuNbFebsPerCrob + uFebIdx;
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
    if (fChannelsToPadXA.size() <= static_cast<int16_t>((febid * kuNbChanPerAsic) + channelid)) {
      LOG(debug) << "MuchReadoutConfig::GetPadXA => Index out of bounds: " << ((febid * kuNbChanPerAsic) + channelid)
                 << " VS " << fChannelsToPadXA.size() << " (" << febid << " and " << channelid << ")";
      return -2;
    }
    return fChannelsToPadXA[(febid * kuNbChanPerAsic) + channelid];
  }

  int8_t MuchReadoutConfig::GetPadYA(uint8_t febid, uint8_t channelid)
  {
    if (fChannelsToPadYA.size() <= static_cast<int16_t>((febid * kuNbChanPerAsic) + channelid)) {
      LOG(debug) << "MuchReadoutConfig::GetPadYA => Index out of bounds: " << ((febid * kuNbChanPerAsic) + channelid)
                 << " VS " << fChannelsToPadYA.size() << " (" << febid << " and " << channelid << ")";
      return -2;
    }
    return fChannelsToPadYA[(febid * kuNbChanPerAsic) + channelid];
  }

  int8_t MuchReadoutConfig::GetPadXB(uint8_t febid, uint8_t channelid)
  {
    if (fChannelsToPadXB.size() <= static_cast<int16_t>((febid * kuNbChanPerAsic) + channelid)) {
      LOG(debug) << "MuchReadoutConfig::GetPadXB => Index out of bounds: " << ((febid * kuNbChanPerAsic) + channelid)
                 << " VS " << fChannelsToPadXB.size() << " (" << febid << " and " << channelid << ")";
      return -2;
    }
    return fChannelsToPadXB[(febid * kuNbChanPerAsic) + channelid];
  }

  int8_t MuchReadoutConfig::GetPadYB(uint8_t febid, uint8_t channelid)
  {
    if (fChannelsToPadYB.size() <= static_cast<int16_t>((febid * kuNbChanPerAsic) + channelid)) {
      LOG(debug) << "MuchReadoutConfig::GetPadYB => Index out of bounds: " << ((febid * kuNbChanPerAsic) + channelid)
                 << " VS " << fChannelsToPadYB.size() << " (" << febid << " and " << channelid << ")";
      return -2;
    }
    return fChannelsToPadYB[(febid * kuNbChanPerAsic) + channelid];
  }

  int8_t MuchReadoutConfig::GetPadXRpc(uint8_t febid, uint8_t channelid)
  {
    if (fChannelsToPadXRpc.size() <= static_cast<int16_t>((febid * kuNbChanPerAsic) + channelid)) {
      LOG(debug) << "CbmMcbm2018MuchPar::GetPadXRpc => Index out of bounds: " << ((febid * kuNbChanPerAsic) + channelid)
                 << " VS " << fChannelsToPadXRpc.size() << " (" << febid << " and " << channelid << ")";
      return -2;
    }
    return fChannelsToPadXRpc[(febid * kuNbChanPerAsic) + channelid];
  }

  int8_t MuchReadoutConfig::GetPadYRpc(uint8_t febid, uint8_t channelid)
  {
    if (fChannelsToPadYRpc.size() <= static_cast<int16_t>((febid * kuNbChanPerAsic) + channelid)) {
      LOG(debug) << "CbmMcbm2018MuchPar::GetPadYRpc => Index out of bounds: " << ((febid * kuNbChanPerAsic) + channelid)
                 << " VS " << fChannelsToPadYRpc.size() << " (" << febid << " and " << channelid << ")";
      return -2;
    }
    return fChannelsToPadYRpc[(febid * kuNbChanPerAsic) + channelid];
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

  //GetModule() is not used in unpacker
  uint16_t MuchReadoutConfig::GetModule(uint16_t uAsicIdx)
  {
    if (uAsicIdx >= GetNrOfFebsInGemA()) {
      if ((uAsicIdx % GetNrOfFebsInGemA()) < GetNrOfFebsInRpc()) return 1;
      else
        return 2;
    }
    else
      return 0;
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
