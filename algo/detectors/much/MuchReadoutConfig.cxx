/* Copyright (C) 2017-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau, Ajit Kumar, Florian Uhlig [committer] */

#include "MuchReadoutConfig.h"

//#include "FairDetParIo.h"
//#include "FairParIo.h"
//#include "FairParamList.h"
#include <Logger.h>

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

  l->add("NrOfDpbs", fuNrOfDpbs);
  l->add("DbpIdArray", fiDbpIdArray);
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

  if (!l->fill("NrOfDpbs", &fuNrOfDpbs)) return false;

  fiDbpIdArray.Set(fuNrOfDpbs);
  if (!l->fill("DbpIdArray", &fiDbpIdArray)) return false;

  fiCrobActiveFlag.Set(fuNrOfDpbs * kuNbCrobsPerDpb);
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

  // -------------------------------------------------------------------------
  uint16_t MuchReadoutConfig::GetDpbId(uint16_t uDpbIdx)
  {
    if (uDpbIdx < fuNrOfDpbs) return fiDbpIdArray[uDpbIdx];
    else {
      LOG(warning) << "MuchReadoutConfig::GetDpbId => Index out of bound, "
                   << "DPB Id is " << std::hex << uDpbIdx << " returning crazy value!";
      return 0xFFFFFFFF;
    }
  }

  bool MuchReadoutConfig::IsCrobActive(uint16_t uDpbIdx, uint16_t uCrobIdx)
  {
    if (uDpbIdx < fuNrOfDpbs) {
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
    if (uDpbIdx < fuNrOfDpbs) {
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
