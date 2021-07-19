/* Copyright (C) 2010 - 2021 Goethe-University Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig */

#include "CbmTofUnpackAlgo.h"

#include "CbmFormatDecHexPrintout.h"
#include "CbmFormatMsHeaderPrintout.h"

#include <FairParGenericSet.h>
#include <FairTask.h>
#include <Logger.h>

#include <Rtypes.h>
#include <RtypesCore.h>


CbmTofUnpackAlgo::CbmTofUnpackAlgo() : CbmRecoUnpackAlgo("CbmTofUnpackAlgo") {}

CbmTofUnpackAlgo::~CbmTofUnpackAlgo() {}

// ---- GetParContainerRequest ----
std::vector<std::pair<std::string, std::shared_ptr<FairParGenericSet>>>*
  CbmTofUnpackAlgo::GetParContainerRequest(std::string /*geoTag*/, std::uint32_t /*runId*/)
{
  // Basepath for default Trd parameter sets (those connected to a geoTag)
  std::string basepath = Form("%s", fParFilesBasePath.data());
  std::string temppath = "";

  // // Get parameter container
  temppath = basepath + "mTofCriPar.par";
  LOG(info) << fName << "::GetParContainerRequest - Trying to open file " << temppath;
  fParContVec.emplace_back(std::make_pair(temppath, std::make_shared<CbmMcbm2018TofPar>()));

  return &fParContVec;
}

// ---- init
Bool_t CbmTofUnpackAlgo::init() { return kTRUE; }

// ---- initParSet(FairParGenericSet* parset) ----
Bool_t CbmTofUnpackAlgo::initParSet(FairParGenericSet* parset)
{
  LOG(info) << fName << "::initParSet - for container " << parset->ClassName();
  if (parset->IsA() == CbmMcbm2018TofPar::Class()) return initParSet(static_cast<CbmMcbm2018TofPar*>(parset));

  // If we do not know the derived ParSet class we return false
  LOG(error)
    << fName << "::initParSet - for container " << parset->ClassName()
    << " failed, since CbmTofUnpackAlgo::initParSet() does not know the derived ParSet and what to do with it!";
  return kFALSE;
}

// ---- initParSet(CbmTrdParSetAsic* parset) ----
Bool_t CbmTofUnpackAlgo::initParSet(CbmMcbm2018TofPar* parset)
{
  fUnpackPar = parset;

  LOG(debug) << "InitParameters from " << parset;

  fuNrOfGdpbs = parset->GetNrOfGdpbs();
  LOG(debug) << "Nr. of Tof GDPBs: " << fuNrOfGdpbs;

  fuNrOfFeePerGdpb = parset->GetNrOfFeesPerGdpb();
  LOG(debug) << "Nr. of FEES per Tof GDPB: " << fuNrOfFeePerGdpb;

  fuNrOfGet4PerFee = parset->GetNrOfGet4PerFee();
  LOG(debug) << "Nr. of GET4 per Tof FEE: " << fuNrOfGet4PerFee;

  fuNrOfChannelsPerGet4 = parset->GetNrOfChannelsPerGet4();
  LOG(debug) << "Nr. of channels per GET4: " << fuNrOfChannelsPerGet4;

  fuNrOfChannelsPerFee = fuNrOfGet4PerFee * fuNrOfChannelsPerGet4;
  LOG(debug) << "Nr. of channels per FEE: " << fuNrOfChannelsPerFee;

  fuNrOfGet4 = fuNrOfGdpbs * fuNrOfFeePerGdpb * fuNrOfGet4PerFee;
  LOG(debug) << "Nr. of GET4s: " << fuNrOfGet4;

  fuNrOfGet4PerGdpb = fuNrOfFeePerGdpb * fuNrOfGet4PerFee;
  LOG(debug) << "Nr. of GET4s per GDPB: " << fuNrOfGet4PerGdpb;

  fuNrOfChannelsPerGdpb = fuNrOfGet4PerGdpb * fuNrOfChannelsPerGet4;
  LOG(debug) << "Nr. of channels per GDPB: " << fuNrOfChannelsPerGdpb;

  fGdpbIdIndexMap.clear();
  for (UInt_t i = 0; i < fuNrOfGdpbs; ++i) {
    fGdpbIdIndexMap[parset->GetGdpbId(i)] = i;
    LOG(debug) << "GDPB Id of TOF  " << i << " : " << std::hex << parset->GetGdpbId(i) << std::dec;
  }  // for( UInt_t i = 0; i < fuNrOfGdpbs; ++i )

  fuNrOfGbtx = parset->GetNrOfGbtx();
  LOG(debug) << "Nr. of GBTx: " << fuNrOfGbtx;

  fviRpcType.resize(fuNrOfGbtx);
  fviModuleId.resize(fuNrOfGbtx);
  fviNrOfRpc.resize(fuNrOfGbtx);
  fviRpcSide.resize(fuNrOfGbtx);
  for (UInt_t uGbtx = 0; uGbtx < fuNrOfGbtx; ++uGbtx) {
    fviNrOfRpc[uGbtx]  = parset->GetNrOfRpc(uGbtx);
    fviRpcType[uGbtx]  = parset->GetRpcType(uGbtx);
    fviRpcSide[uGbtx]  = parset->GetRpcSide(uGbtx);
    fviModuleId[uGbtx] = parset->GetModuleId(uGbtx);
  }  // for( UInt_t uGbtx = 0; uGbtx < fuNrOfGbtx; ++uGbtx)

  UInt_t uNrOfChannels = fuNrOfGet4 * fuNrOfChannelsPerGet4;
  LOG(debug) << "Nr. of possible Tof channels: " << uNrOfChannels;

  //   CbmTofDetectorId* fTofId = new CbmTofDetectorId_v14a();
  fviRpcChUId = parset->GetRpcChUidMap();

  TString sPrintout = "";
  for (UInt_t uCh = 0; uCh < uNrOfChannels; ++uCh) {
    if (0 == uCh % 8) sPrintout += "\n";
    if (0 == uCh % fuNrOfChannelsPerGdpb) sPrintout += Form("\n Gdpb %u\n", uCh / fuNrOfChannelsPerGdpb);
    sPrintout += Form(" 0x%08x", fviRpcChUId[uCh]);
  }  // for( UInt_t i = 0; i < uNrOfChannels; ++i)
  LOG(debug) << sPrintout;

  LOG(info) << fName << "::initParSetTofMcbm2018 - Successfully initialized TOF settings";

  return kTRUE;
}

bool CbmTofUnpackAlgo::unpack(const fles::Timeslice* ts, std::uint16_t icomp, UInt_t imslice)
{
  auto msDescriptor        = ts->descriptor(icomp, imslice);
  fuCurrentEquipmentId     = msDescriptor.eq_id;
  const uint8_t* msContent = reinterpret_cast<const uint8_t*>(ts->content(icomp, imslice));

  fulCurrentTsIdx = ts->index();
  uint32_t uSize  = msDescriptor.size;
  fulCurrentMsIdx = msDescriptor.idx;
  //   Double_t dMsTime = (1e-9) * static_cast<double>(fulCurrentMsIdx);
  LOG(debug) << "Microslice: " << fulCurrentMsIdx << " from EqId " << std::hex << fuCurrentEquipmentId << std::dec
             << " has size: " << uSize;

  if (0 == fvbMaskedComponents.size()) fvbMaskedComponents.resize(ts->num_components(), false);

  fuCurrDpbId = static_cast<uint32_t>(fuCurrentEquipmentId & 0xFFFF);
  //   fuCurrDpbIdx = fDpbIdIndexMap[ fuCurrDpbId ];

  /// Check if this sDPB ID was declared in parameter file and stop there if not
  auto it = fGdpbIdIndexMap.find(fuCurrDpbId);
  if (it == fGdpbIdIndexMap.end()) {
    if (false == fvbMaskedComponents[icomp]) {
      LOG(debug) << "---------------------------------------------------------------";
      LOG(debug) << FormatMsHeaderPrintout(msDescriptor);
      LOG(warning) << fName << "::unpack => "
                   << "Could not find the gDPB index for FLIM id 0x" << std::hex << fuCurrDpbId << std::dec
                   << " in timeslice " << fulCurrentTsIdx << " in microslice " << imslice << " component " << icomp
                   << std::endl
                   << "If valid this index has to be added in the TOF parameter file in the DbpIdArray field";
      fvbMaskedComponents[icomp] = true;
    }  // if( false == fvbMaskedComponents[ uMsComp ] )
    else
      return true;

    return false;
  }  // if( it == fGdpbIdIndexMap.end() )
  else
    fuCurrDpbIdx = fGdpbIdIndexMap[fuCurrDpbId];

  fuCurrentMsSysId = static_cast<unsigned int>(msDescriptor.sys_id);

  // If not integer number of message in input buffer, print warning/error
  if (0 != (uSize % sizeof(critof001::Message)))
    LOG(error) << fName << "::unpack => "
               << "The input microslice buffer does NOT contain only complete gDPB messages!";

  // Compute the number of complete messages in the input microslice buffer
  uint32_t uNbMessages = (uSize - (uSize % sizeof(critof001::Message))) / sizeof(critof001::Message);

  // Prepare variables for the loop on contents
  Int_t messageType     = -111;
  fbLastEpochGood       = false;
  fuProcEpochUntilError = 0;

  if (0 == imslice) {
    /// Extract the time base only on MS 0, assuming that we get all TS of a component in order
    ExtractTsStartEpoch(fTsStartTime);
  }

  const uint64_t* pInBuff         = reinterpret_cast<const uint64_t*>(msContent);  // for epoch cycle
  const critof001::Message* pMess = reinterpret_cast<const critof001::Message*>(pInBuff);

  for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx++) {
    /// Due to buffer bug some messages are duplicated
    if (0 < uIdx && pMess[uIdx] == pMess[uIdx - 1]) {
      /// Ignore duplicate message
      continue;
    }

    /// Get message type
    messageType = pMess[uIdx].getMessageType();

    if (uNbMessages - 1 == uIdx) {
      if (pMess[uIdx].isEndOfMs()) {
        /// Tricking clang to avoid one liner
        continue;
      }
      else {
        LOG(warning) << fName << "::unpack => "
                     << "In timeslice " << fulCurrentTsIdx << " in microslice " << imslice << " component " << icomp
                     << " last message is not an EndOfMs: type " << messageType
                     << Form(" dump: 0x%16lX", pMess[uIdx].getData());
      }  // else of if( pMess[uIdx].isEndOfMs() )
    }    // if( uNbMessages - 1 == uIdx )
         /*
    if( 0 == imslice )
        LOG(debug) << fName << "::unpack => "
                   << "Message type " << std::hex << std::setw(2) << static_cast<uint16_t>(messageType) << std::dec;
*/
    fuGet4Id = fUnpackPar->ElinkIdxToGet4Idx(pMess[uIdx].getGet4Idx());
    if (0x90 == fuCurrentMsSysId) fuGet4Id = pMess[uIdx].getGet4Idx();
    fuGet4Nr = (fuCurrDpbIdx * fuNrOfGet4PerGdpb) + fuGet4Id;

    if (fuNrOfGet4PerGdpb <= fuGet4Id && (critof001::kuChipIdMergedEpoch != fuGet4Id))
      LOG(warning) << fName << "::unpack => "
                   << "Message with Get4 ID too high: " << fuGet4Id << " VS " << fuNrOfGet4PerGdpb << " for FLIM "
                   << fuCurrDpbIdx << " set in parameters.";


    if (0 == uIdx && critof001::MSG_EPOCH != messageType) {
      LOG(warning) << fName << "::unpack => "
                   << "In timeslice " << fulCurrentTsIdx << " in microslice " << imslice << " component " << icomp
                   << " first message is not an epoch: type " << messageType
                   << Form(" dump: 0x%16lX", pMess[uIdx].getData());
      LOG(warning) << fName << "::unpack => "
                   << "Ignoring this microslice.";
      return false;
    }

    switch (messageType) {
      case critof001::MSG_HIT: {
        if (fbLastEpochGood) {
          /// Epoch OK
          ProcessHit(pMess[uIdx]);
        }
        break;
      }  // case critof001::MSG_HIT:
      case critof001::MSG_EPOCH: {
        if (critof001::kuChipIdMergedEpoch == fuGet4Id) {
          ProcessEpoch(pMess[uIdx], uIdx);
        }  // if this epoch message is a merged one valid for all chips
        else {
          /// Should be checked in monitor task, here we just jump it
          LOG(debug2) << fName << "::unpack => "
                      << "This unpacker does not support unmerged epoch messages!!!.";
          continue;
        }  // if single chip epoch message
        break;
      }  // case critof001::MSG_EPOCH:
      case critof001::MSG_SLOWC:
      case critof001::MSG_SYST: {
        /// Ignored messages
        /// TODO,FIXME: should be filled into fOptOutAVec as CbmErrorMessage
        break;
      }  // case critof001::MSG_ERROR
      default:
        LOG(error) << fName << "::unpack => "
                   << "Message type " << std::hex << std::setw(2) << static_cast<uint16_t>(messageType) << std::dec
                   << " not included in Get4 unpacker.";
    }  // switch( mess.getMessageType() )
  }    // for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx ++)

  return true;
}

// -------------------------------------------------------------------------
void CbmTofUnpackAlgo::ExtractTsStartEpoch(const uint64_t& ulTsStart)
{
  fulTsStartInEpoch = static_cast<uint64_t>(ulTsStart / critof001::kuEpochInNs) % critof001::kulEpochCycleEp;

  /// FIXME: seems there is an offset of +4 Epoch between data and header
  ///        from dt to PSD, the epoch seem to be right => placed in wrong MS!
  if (fulTsStartInEpoch < 4) { fulTsStartInEpoch = critof001::kulEpochCycleEp + fulTsStartInEpoch - 4; }
  else {
    fulTsStartInEpoch -= 4;
  }
}

void CbmTofUnpackAlgo::ProcessEpoch(const critof001::Message& mess, uint32_t uMesgIdx)
{
  /// FIXME: seems there is an offset of +4 Epoch between data and header
  ///        from dt to PSD, the epoch seem to be right => placed in wrong MS!
  ULong64_t ulEpochNr = mess.getGdpbEpEpochNb();
  //ULong64_t ulEpochNr = (mess.getGdpbEpEpochNb() + 4) % critof001::kulEpochCycleEp;

  if (0 == uMesgIdx) {
    uint64_t ulMsStartInEpoch =
      static_cast<uint64_t>(fulCurrentMsIdx / critof001::kuEpochInNs) % critof001::kulEpochCycleEp;
    /// FIXME: seems there is an offset of +4 Epoch between data and header
    ///        from dt to PSD, the epoch seem to be right => placed in wrong MS!
    if (ulMsStartInEpoch < 4) { ulMsStartInEpoch = critof001::kulEpochCycleEp + ulMsStartInEpoch - 4; }
    else {
      ulMsStartInEpoch -= 4;
    }

    if (ulEpochNr != ulMsStartInEpoch) {
      LOG(error) << fName << "::ProcessEpoch => Error first global epoch, "
                 << Form(
                      "from MS index 0x%06lx, current 0x%06llx, diff %lld, raw 0x%016lx, NoErr %d, current 0x%06lx  %f",
                      ulMsStartInEpoch, ulEpochNr, ulEpochNr - ulMsStartInEpoch, mess.getData(), fuProcEpochUntilError,
                      static_cast<uint64_t>(fulCurrentMsIdx / critof001::kuEpochInNs),
                      fulCurrentMsIdx / critof001::kuEpochInNs);
      LOG(error) << fName << "::ProcessEpoch => Ignoring data until next valid epoch";

      fbLastEpochGood       = false;
      ulEpochNr             = ulMsStartInEpoch;
      fuProcEpochUntilError = 0;
    }  // if( ulEpochNr != ulMsStartInEpoch )
    else {
      fbLastEpochGood = true;
      fuProcEpochUntilError++;
    }  // else of if( ulEpochNr != ulMsStartInEpoch )
  }    // if( 0 < uMesgIdx )
  else if (((fulCurrentEpoch + 1) % critof001::kulEpochCycleEp) != ulEpochNr) {
    LOG(error) << fName << "::ProcessEpoch => Error global epoch, "
               << Form("last 0x%06llx, current 0x%06llx, diff %lld, raw 0x%016lx, NoErr %d", fulCurrentEpoch, ulEpochNr,
                       ulEpochNr - fulCurrentEpoch, mess.getData(), fuProcEpochUntilError);
    LOG(error) << fName << "::ProcessEpoch => Ignoring data until next valid epoch";

    ulEpochNr             = (fulCurrentEpoch + 1) % critof001::kulEpochCycleEp;
    fbLastEpochGood       = false;
    fuProcEpochUntilError = 0;
  }  // if( ( (fulCurrentEpoch + 1) % critof001::kuEpochCounterSz ) != ulEpochNr )
  else {
    fbLastEpochGood = true;
    fuProcEpochUntilError++;
  }

  fulCurrentEpoch = ulEpochNr;
  if (fulTsStartInEpoch <= ulEpochNr) { fulEpochIndexInTs = ulEpochNr - fulTsStartInEpoch; }
  else {
    fulEpochIndexInTs = ulEpochNr + critof001::kulEpochCycleEp - fulTsStartInEpoch;
  }
  if (10e9 < critof001::kuEpochInNs * fulEpochIndexInTs)
    LOG(debug) << fName << "::ProcessEpoch => "
               << Form("Raw Epoch: 0x%06llx, Epoch offset 0x%06lx, Epoch in Ts: 0x%07lx, time %f ns (%f * %lu)",
                       ulEpochNr, fulTsStartInEpoch, fulEpochIndexInTs, critof001::kuEpochInNs * fulEpochIndexInTs,
                       critof001::kuEpochInNs, fulEpochIndexInTs);
}

void CbmTofUnpackAlgo::ProcessHit(const critof001::Message& mess)
{
  UInt_t uChannel = mess.getGdpbHitChanId();
  UInt_t uTot     = mess.getGdpbHit32Tot();

  UInt_t uChannelNr      = fuGet4Id * fuNrOfChannelsPerGet4 + uChannel;
  UInt_t uChannelNrInFee = (fuGet4Id % fuNrOfGet4PerFee) * fuNrOfChannelsPerGet4 + uChannel;
  UInt_t uFeeNr          = (fuGet4Id / fuNrOfGet4PerFee);
  UInt_t uFeeNrInSys     = fuCurrDpbIdx * fuNrOfFeePerGdpb + uFeeNr;
  // UInt_t uRemappedChannelNr = uFeeNr * fuNrOfChannelsPerFee + fUnpackPar->Get4ChanToPadiChan(uChannelNrInFee);

  UInt_t uRemappedChannelNrInSys = fuCurrDpbIdx * fuNrOfChannelsPerGdpb + uFeeNr * fuNrOfChannelsPerFee
                                   + fUnpackPar->Get4ChanToPadiChan(uChannelNrInFee);
  /// Diamond FEE have straight connection from Get4 to eLink and from PADI to GET4
  if (0x90 == fuCurrentMsSysId) {
    // uRemappedChannelNr      = uChannelNr;
    uRemappedChannelNrInSys = fuCurrDpbIdx * fUnpackPar->GetNrOfChannelsPerGdpb() + uChannelNr;
  }  // if(0x90 == fuCurrentMsSysId)

  Double_t dHitTime = mess.getMsgFullTimeD(fulEpochIndexInTs);
  Double_t dHitTot  = uTot;  // in bins

  if (fviRpcChUId.size() < uRemappedChannelNrInSys) {
    LOG(fatal) << fName << "::unpack => "
               << "Invalid mapping index " << uRemappedChannelNrInSys << " VS " << fviRpcChUId.size() << ", from FLIM "
               << fuCurrDpbIdx << ", Get4 " << fuGet4Id << ", Ch " << uChannel << ", ChNr " << uChannelNr << ", ChNrIF "
               << uChannelNrInFee << ", FiS " << uFeeNrInSys;
    return;
  }  // if( fviRpcChUId.size() < uRemappedChannelNrInSys )

  UInt_t uChanUId = fviRpcChUId[uRemappedChannelNrInSys];

  if (0 == uChanUId) {
    if (0 < fuMapWarnToPrint--)
      LOG(warning) << fName << "::unpack => "
                   << "Unused data item at " << uRemappedChannelNrInSys << ", from FLIM " << fuCurrDpbIdx << ", Get4 "
                   << fuGet4Id << ", Ch " << uChannel << ", ChNr " << uChannelNr << ", ChNrIF " << uChannelNrInFee
                   << ", FiS " << uFeeNrInSys;
    return;  // Hit not mapped to digi
  }

  /// Apply offset to T0 only to TOF digis
  if (0x90 != fuCurrentMsSysId) {
    /// Tricking clang to avoid one liner
    dHitTime -= fdTimeOffsetNs;
  }

  /// FIXME: seems there is an offset of +4 Epoch between data and header
  ///        from dt to PSD, the epoch are probably the one off, not the MS time!
  dHitTime -= 4.0 * critof001::kuEpochInNs;

  LOG(debug) << Form("Insert 0x%08x digi with time ", uChanUId) << dHitTime << Form(", Tot %4.0f", dHitTot)
             << " at epoch " << fulEpochIndexInTs;

  /// Create output object and store it
  std::unique_ptr<CbmTofDigi> digi(new CbmTofDigi(uChanUId, dHitTime, dHitTot));
  if (digi) fOutputVec.emplace_back(*std::move(digi));
}

ClassImp(CbmTofUnpackAlgo)
