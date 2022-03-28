/* Copyright (C) 2022 Fair GmbH, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

#include "CbmMuchUnpackAlgo.h"

#include "CbmMuchDigi.h"

#include <FairParGenericSet.h>
#include <FairTask.h>
#include <Logger.h>

#include <Rtypes.h>
#include <RtypesCore.h>

#include <bitset>
#include <cstdint>
#include <iomanip>

CbmMuchUnpackAlgo::CbmMuchUnpackAlgo() : CbmRecoUnpackAlgo("CbmMuchUnpackAlgo") {}

CbmMuchUnpackAlgo::CbmMuchUnpackAlgo(std::string name) : CbmRecoUnpackAlgo(name) {}

CbmMuchUnpackAlgo::~CbmMuchUnpackAlgo() {}

// ---- GetParContainerRequest ----
std::vector<std::pair<std::string, std::shared_ptr<FairParGenericSet>>>*
  CbmMuchUnpackAlgo::GetParContainerRequest(std::string /*geoTag*/, std::uint32_t /*runId*/)
{
  // Basepath for default Much parameter sets (those connected to a geoTag)
  std::string basepath = Form("%s", fParFilesBasePath.data());
  std::string temppath = "";

  // // Get parameter container
  temppath = basepath + "mMuchPar.par";
  LOG(debug) << "mMuchPar path Name: " << temppath;
  fParContVec.emplace_back(std::make_pair(temppath, std::make_shared<CbmMuchUnpackPar>()));

  return &fParContVec;
}

// ---- getAsicIndex ----
/*
uint32_t CbmMuchUnpackAlgo::getFebIndex(uint32_t dpbidx, uint32_t crobidx, uint16_t elinkidx)
{
  // This function generates FEB index between 0 to dpbidx * 9 NrAsicsPerCrob
  uint32_t asicidx       = fUnpackPar->ElinkIdxToFebIdx(elinkidx);
  const uint32_t uAsicIdx = (dpbidx * fNrCrobPerDpb + crobidx) * fNrAsicsPerCrob + asicidx;
  return uAsicIdx;
}
*/

// ---- getFullTimeStamp ----
uint64_t CbmMuchUnpackAlgo::getFullTimeStamp(const uint16_t usRawTs)
{
  // Use TS w/o overlap bits as they will anyway come from the TS_MSB
  const uint64_t ulTime =
    usRawTs + fulTsMsbIndexInTs[fuCurrDpbIdx] * static_cast<uint64_t>(stsxyter::kuHitNbTsBinsBinning);
  /*
    + static_cast<uint64_t>(stsxyter::kuHitNbTsBinsBinning) * static_cast<uint64_t>(fvulCurrentTsMsb[fuCurrDpbIdx])
    + static_cast<uint64_t>(stsxyter::kulTsCycleNbBinsBinning)
        * static_cast<uint64_t>(fvuCurrentTsMsbCycle[fuCurrDpbIdx]);
*/
  return ulTime;
}

// ---- init
Bool_t CbmMuchUnpackAlgo::init() { return kTRUE; }

// ---- initDpbIdIndexMap ----
void CbmMuchUnpackAlgo::initDpbIdIndexMap(CbmMuchUnpackPar* parset)
{
  fDpbIdIndexMap.clear();
  for (uint32_t uDpb = 0; uDpb < parset->GetNrOfDpbs(); ++uDpb) {
    fDpbIdIndexMap[parset->GetDpbId(uDpb)] = uDpb;
    LOG(debug) << "Eq. ID for DPB #" << std::setw(2) << uDpb << " = 0x" << std::setw(4) << std::hex
               << parset->GetDpbId(uDpb) << std::dec << " => " << fDpbIdIndexMap[parset->GetDpbId(uDpb)];
  }
}

// ---- initParSet(FairParGenericSet* parset) ----
Bool_t CbmMuchUnpackAlgo::initParSet(FairParGenericSet* parset)
{
  LOG(info) << fName << "::initParSet - for container " << parset->ClassName();
  if (parset->IsA() == CbmMuchUnpackPar::Class()) return initParSet(static_cast<CbmMuchUnpackPar*>(parset));

  // If we do not know the derived ParSet class we return false
  LOG(error)
    << fName << "::initParSet - for container " << parset->ClassName()
    << " failed, since CbmMuchUnpackAlgo::initParSet() does not know the derived ParSet and what to do with it!";
  return kFALSE;
}

// ---- initParSet(CbmMuchUnpackPar* parset) ----
Bool_t CbmMuchUnpackAlgo::initParSet(CbmMuchUnpackPar* parset)
{
  fUnpackPar = parset;
  LOG(debug) << fName << "::initParSetAsic - ";

  //Type of each module: 0 for connectors on the right, 1 for connectors on the left
  //  std::vector<int32_t> viModuleType;

  // STS address for the first strip of each module
  // std::vector<int32_t> viModAddress;

  // Idx of the STS module for each FEB, [ NbDpb ][ NbCrobPerDpb ][ NbFebsPerCrob ], -1 if inactive
  //std::vector<std::vector<std::vector<int32_t>>> viFebModuleIdx;

  // Array to hold the active flag for all CROBs, [ NbDpb ][ NbCrobPerDpb ]
  std::vector<std::vector<bool>> vbCrobActiveFlag;

  //STS module side for each FEB, [ NbDpb ][ NbCrobPerDpb ][ NbFebsPerCrob ], 0 = P, 1 = N, -1 if inactive
  //std::vector<std::vector<std::vector<int32_t>>> viFebModuleSide;

  // Total number of STS modules in the setup
  //const uint32_t uNbModules = parset->GetNbOfModules();
  //LOG(debug) << "Nr. of STS Modules:    " << uNbModules;

  // Total number of MuCH DPBs in system
  const uint32_t uNbOfDpbs = parset->GetNrOfDpbs();
  LOG(debug) << "Nr. of MuCH DPBs:       " << uNbOfDpbs;

  // Get Nr of Febs
  fuNbFebs = parset->GetNrOfFebs();
  LOG(debug) << "Nr. of FEBs:           " << fuNbFebs;

  // Get Nr of eLinks per CROB
  fNrElinksPerCrob = parset->GetNbElinkPerCrob();

  // Get Nr of ASICs per CROB
  fNrAsicsPerCrob = parset->GetNbAsicsPerCrob();

  // Get Nr of CROBs per DPB
  fNrCrobPerDpb = parset->GetNbCrobsPerDpb();

  // Get Number of ASICs per FEB
  fNrAsicsPerFeb = parset->GetNbAsicsPerFeb();

  // Get Number of Channels per Asic
  fNrChsPerAsic = parset->GetNbChanPerAsic();

  // Get Number of Channels per FEB
  fNrChsPerFeb = parset->GetNbChanPerFeb();

  // Get Number of FEBs per CROB
  fNrFebsPerCrob = parset->GetNbFebsPerCrob();

  /*
  for (size_t ielink = 0; ielink < fNrElinksPerCrob; ++ielink) {
    fElinkIdxToFebIdxVec.emplace_back(parset->ElinkIdxToFebIdx(ielink));
    fElinkIdxToAsicIdxVec.emplace_back(
      std::make_pair(parset->ElinkIdxToAsicIdxFebA(ielink), parset->ElinkIdxToAsicIdxFebB(ielink)));
  }*/

  // Get Nr of Asics
  const uint32_t uNbStsXyters = parset->GetNrOfFebs();
  LOG(debug) << "Nr. of StsXyter ASICs: " << uNbStsXyters;

  //Initialize temporary "per Feb" fields
  //initTempVectors(parset, &viModuleType, &viModAddress, &viFebModuleIdx, &vbCrobActiveFlag, &viFebModuleSide);
  //initTempVectors(parset, &vbCrobActiveFlag);

  // Initialize ADC cuts for FEBs
  fvbFebAdcCut.resize(fuNbFebs, fdAdcCut);
  for (auto cut : fdAdcCut_perFeb) {
    fvbFebAdcCut[cut.first] = cut.second;
  }
  // Read dpb index map from parameter container
  initDpbIdIndexMap(parset);

  if (fvdTimeOffsetNsAsics.size() < uNbStsXyters) { fvdTimeOffsetNsAsics.resize(uNbStsXyters, 0.0); }

  /*
  //Initialize class-wide "per Feb" fields
  fviFebType.resize(uNbOfDpbs);

  for (uint32_t uDpb = 0; uDpb < uNbOfDpbs; ++uDpb) {
    fviFebType[uDpb].resize(fNrCrobPerDpb);
    for (uint32_t uCrobIdx = 0; uCrobIdx < fNrCrobPerDpb; ++uCrobIdx) {
      fviFebType[uDpb][uCrobIdx].resize(parset->GetNbFebsPerCrob(), -1);
      for (uint32_t uFebIdx = 0; uFebIdx < parset->GetNbFebsPerCrob(); ++uFebIdx) {
        //fvbFebPulser.push_back(parset->IsFebPulser(uDpb, uCrobIdx, uFebIdx));
        //fvdFebAdcGain.push_back(parset->GetFebAdcGain(uDpb, uCrobIdx, uFebIdx));
        //fvdFebAdcOffs.push_back(parset->GetFebAdcOffset(uDpb, uCrobIdx, uFebIdx));

        if (0 <= viFebModuleIdx[uDpb][uCrobIdx][uFebIdx]
            && static_cast<uint32_t>(viFebModuleIdx[uDpb][uCrobIdx][uFebIdx]) < uNbModules
            && 0 <= viFebModuleSide[uDpb][uCrobIdx][uFebIdx] && viFebModuleSide[uDpb][uCrobIdx][uFebIdx] < 2) {
          switch (viModuleType[viFebModuleIdx[uDpb][uCrobIdx][uFebIdx]]) {
            case 0:  // FEB-8-1 with ZIF connector on the right
            {
              // P side (0) has type A (0)
              // N side (1) has type B (1)
              fviFebType[uDpb][uCrobIdx][uFebIdx] = viFebModuleSide[uDpb][uCrobIdx][uFebIdx];

              ///! FIXME: 1) Geometry is using front/back while we are using P/N !!!!
              ///!            => Assuming that front facing modules have connectors on right side
              ///!            +> Volker warns that the front side should be electrons one so N
              ///!        2) No accessor/setter to change only the side field of an STS address
              ///!            => hardcode the shift
              ///!            +> The bit is unused in the current scheme: the side is encoded in the Digi channel
              fviFebAddress.push_back(viModAddress[viFebModuleIdx[uDpb][uCrobIdx][uFebIdx]]);
              //                       + (viFebModuleSide[uDpb][uCrobIdx][uFebIdx] << 25));
              fviFebSide.push_back(viFebModuleSide[uDpb][uCrobIdx][uFebIdx]);
              break;
            }        // case 0: // FEB-8-1 with ZIF connector on the right
            case 1:  // FEB-8-1 with ZIF connector on the left
            {
              // P side (0) has type B (1)
              // N side (1) has type A (0)
              fviFebType[uDpb][uCrobIdx][uFebIdx] = !(viFebModuleSide[uDpb][uCrobIdx][uFebIdx]);

              ///! FIXME: 1) Geometry is using front/back while we are using P/N !!!!
              ///!            => Assuming that front facing modules have connectors on right side
              ///!            +> Volker warns that the front side should be electrons one so N
              ///!        2) No accessor/setter to change only the side field of an STS address
              ///!            => hardcode the shift
              ///!            +> The bit is unused in the current scheme: the side is encoded in the Digi channel
              fviFebAddress.push_back(viModAddress[viFebModuleIdx[uDpb][uCrobIdx][uFebIdx]]);
              //                        + ((!viFebModuleSide[uDpb][uCrobIdx][uFebIdx]) << 25));
              fviFebSide.push_back(viFebModuleSide[uDpb][uCrobIdx][uFebIdx]);
              break;
            }  // case 1: // FEB-8-1 with ZIF connector on the left
            default:
              LOG(fatal) << Form("Bad module type for DPB #%02u CROB #%u FEB %02u: %d", uDpb, uCrobIdx, uFebIdx,
                                 viModuleType[viFebModuleIdx[uDpb][uCrobIdx][uFebIdx]]);
              break;
          }
        }  // FEB active and module index OK
        else if (-1 == viFebModuleIdx[uDpb][uCrobIdx][uFebIdx] || -1 == viFebModuleSide[uDpb][uCrobIdx][uFebIdx]) {
          fviFebAddress.push_back(0);
          fviFebSide.push_back(-1);
        }  // Module index or type is set to inactive
        else {
          LOG(fatal) << Form("Bad module Index and/or Side for DPB #%02u CROB "
                             "#%u FEB %02u: %d %d",
                             uDpb, uCrobIdx, uFebIdx, viFebModuleIdx[uDpb][uCrobIdx][uFebIdx],
                             viFebModuleSide[uDpb][uCrobIdx][uFebIdx]);
        }  // Bad module index or type for this FEB
      }
    }
  }
*/

  vbCrobActiveFlag.resize(uNbOfDpbs);
  for (uint32_t uDpb = 0; uDpb < uNbOfDpbs; ++uDpb) {
    vbCrobActiveFlag[uDpb].resize(fNrCrobPerDpb);
    for (uint32_t uCrobIdx = 0; uCrobIdx < fNrCrobPerDpb; ++uCrobIdx) {
      vbCrobActiveFlag[uDpb][uCrobIdx] = parset->IsCrobActive(uDpb, uCrobIdx);
    }
  }

  printActiveCrobs(parset, vbCrobActiveFlag);
  //printAddressMaps(parset, viFebModuleIdx, viFebModuleSide);

  LOG(debug) << "Unpacking data in bin sorter FW mode";
  initInternalStatus(parset);

  if (fMonitor) {
    fMonitor->Init(parset);
    LOG(info) << fName << "::initParSet - Successfully initialized MUCH monitor";
  }

  return kTRUE;
}

// ---- initTempVectors ----

//ToDo: Temporary vectors to be initialized

// ---- initInternalStatus ----
void CbmMuchUnpackAlgo::initInternalStatus(CbmMuchUnpackPar* parset)
{

  const uint32_t uNbOfDpbs    = parset->GetNrOfDpbs();
  const uint32_t uNbStsXyters = parset->GetNrOfAsics();

  fvulCurrentTsMsb.resize(uNbOfDpbs);
  fvuCurrentTsMsbCycle.resize(uNbOfDpbs);
  fulTsMsbIndexInTs.resize(uNbOfDpbs);
  for (uint32_t uDpb = 0; uDpb < uNbOfDpbs; ++uDpb) {
    fvulCurrentTsMsb[uDpb]     = 0;
    fvuCurrentTsMsbCycle[uDpb] = 0;
    fulTsMsbIndexInTs[uDpb]    = 0;
  }

  fvvusLastTsChan.resize(uNbStsXyters);
  fvvusLastAdcChan.resize(uNbStsXyters);
  fvvulLastTsMsbChan.resize(uNbStsXyters);
  for (uint32_t uAsicIdx = 0; uAsicIdx < uNbStsXyters; ++uAsicIdx) {
    fvvusLastTsChan[uAsicIdx].resize(fNrChsPerAsic, 0);
    fvvusLastAdcChan[uAsicIdx].resize(fNrChsPerAsic, 0);
    fvvulLastTsMsbChan[uAsicIdx].resize(fNrChsPerAsic, 0);
  }
}

// ---- loopMsMessages ----
void CbmMuchUnpackAlgo::loopMsMessages(const uint8_t* msContent, const uint32_t uSize, const size_t uMsIdx)
{
  // If not integer number of message in input buffer, print warning/error
  if (0 != (uSize % sizeof(stsxyter::Message))) {
    LOG(error) << "The input microslice buffer does NOT "
               << "contain only complete sDPB messages!";
  }
  // Compute the number of complete messages in the input microslice buffer
  const uint32_t uNbMessages = (uSize - (uSize % sizeof(stsxyter::Message))) / sizeof(stsxyter::Message);

  // Prepare variables for the loop on contents
  const stsxyter::Message* pMess = reinterpret_cast<const stsxyter::Message*>(msContent);

  for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx++) {
    /// Get message type
    const stsxyter::MessType typeMess = pMess[uIdx].GetMessType();

    LOG(debug2) << " Msg Idx " << std::setw(6) << uIdx << " Type " << stsxyter::Message::PrintMessType(typeMess);

    if (fMonitor)
      if (fMonitor->GetDebugMode()) { fMonitor->ProcessDebugInfo(pMess[uIdx], fuCurrDpbIdx); }

    switch (typeMess) {
      case stsxyter::MessType::Hit: {
        processHitInfo(pMess[uIdx]);
        break;
      }
      case stsxyter::MessType::TsMsb: {
        processTsMsbInfo(pMess[uIdx], uIdx, uMsIdx);
        break;
      }
      case stsxyter::MessType::Epoch: {
        processEpochInfo(pMess[uIdx]);
        if (0 < uIdx) {
          LOG(warning) << "CbmMuchUnpackAlgo::loopMsMessages => "
                       << "EPOCH message at unexpected position in MS: message " << uIdx << " VS message 0 expected!";
        }
        break;
      }
      case stsxyter::MessType::Status: {
        processStatusInfo(pMess[uIdx], uIdx);
        break;
      }
      case stsxyter::MessType::Empty: {
        break;
      }
      case stsxyter::MessType::EndOfMs: {
        processErrorInfo(pMess[uIdx]);
        break;
      }
      case stsxyter::MessType::Dummy: {
        break;
      }
      default: {
        LOG(fatal) << "CbmMuchUnpackAlgo::loopMsMessages => "
                   << "Unknown message type, should never happen, stopping "
                      "here! Type found was: "
                   << static_cast<int>(typeMess);
      }
    }
  }
}

// ---- MaskNoisyChannel ----
void CbmMuchUnpackAlgo::MaskNoisyChannel(const uint32_t uFeb, const uint32_t uChan, const bool bMasked)
{
  if (false == fbUseChannelMask) {
    fbUseChannelMask = true;
    fvvbMaskedChannels.resize(fuNbFebs);
    for (uint32_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx) {
      fvvbMaskedChannels[uFebIdx].resize(fNrChsPerFeb, false);
    }
  }
  if (uFeb < fuNbFebs && uChan < fNrChsPerFeb) fvvbMaskedChannels[uFeb][uChan] = bMasked;
  else
    LOG(fatal) << "CbmMuchUnpackAlgo::MaskNoisyChannel => Invalid FEB "
                  "and/or CHAN index:"
               << Form(" %u vs %u and %u vs %u", uFeb, fuNbFebs, uChan, fNrChsPerFeb);
}


// ---- printActiveCrobs ----
void CbmMuchUnpackAlgo::printActiveCrobs(CbmMuchUnpackPar* parset,
                                         const std::vector<std::vector<bool>>& vbCrobActiveFlag)
{
  for (uint32_t uDpb = 0; uDpb < parset->GetNrOfDpbs(); ++uDpb) {
    TString sPrintoutLine = Form("DPB #%02u CROB Active ?:       ", uDpb);
    for (uint32_t uCrobIdx = 0; uCrobIdx < fNrCrobPerDpb; ++uCrobIdx) {
      sPrintoutLine += Form("%1u", (vbCrobActiveFlag[uDpb][uCrobIdx] == true));
      LOG(debug) << "at function printActiveCrobs";
    }
    LOG(debug) << sPrintoutLine;
  }
}

// ---- printAddressMaps ----
//To Do: fill the body accordingly
void CbmMuchUnpackAlgo::printAddressMaps(CbmMuchUnpackPar* /*parset*/,
                                         const std::vector<std::vector<std::vector<int32_t>>>& /*viFebModuleIdx*/,
                                         const std::vector<std::vector<std::vector<int32_t>>>& /*viFebModuleSide*/)
{

  LOG(info) << "Currently this function not printing any info for MuCh.";
  /*
  uint32_t uGlobalFebIdx = 0;
  for (uint32_t uDpb = 0; uDpb < parset->GetNrOfDpbs(); ++uDpb) {
    for (uint32_t uCrobIdx = 0; uCrobIdx < fNrCrobPerDpb; ++uCrobIdx) {
      LOG(debug) << Form("DPB #%02u CROB #%u:       ", uDpb, uCrobIdx);
      for (uint32_t uFebIdx = 0; uFebIdx < parset->GetNbFebsPerCrob(); ++uFebIdx) {
        if (0 <= viFebModuleIdx[uDpb][uCrobIdx][uFebIdx])
          LOG(debug) << Form("      FEB #%02u (%02u): Mod. Idx = %03d Side %c (%2d) Type %c "
                             "(%2d) (Addr. 0x%08x) ADC gain %4.0f e- ADC Offs %5.0f e-",
                             uFebIdx, uGlobalFebIdx, viFebModuleIdx[uDpb][uCrobIdx][uFebIdx],
                             1 == viFebModuleSide[uDpb][uCrobIdx][uFebIdx] ? 'N' : 'P',
                             viFebModuleSide[uDpb][uCrobIdx][uFebIdx],
                             1 == fviFebType[uDpb][uCrobIdx][uFebIdx] ? 'B' : 'A', fviFebType[uDpb][uCrobIdx][uFebIdx],
                             fviFebAddress[uGlobalFebIdx], fvdFebAdcGain[uGlobalFebIdx], fvdFebAdcOffs[uGlobalFebIdx]);
        else
          LOG(debug) << Form("Disabled FEB #%02u (%02u): Mod. Idx = %03d Side %c (%2d) Type %c "
                             "(%2d) (Addr. 0x%08x) ADC gain %4.0f e- ADC Offs %5.0f e-",
                             uFebIdx, uGlobalFebIdx, viFebModuleIdx[uDpb][uCrobIdx][uFebIdx],
                             1 == viFebModuleSide[uDpb][uCrobIdx][uFebIdx] ? 'N' : 'P',
                             viFebModuleSide[uDpb][uCrobIdx][uFebIdx],
                             1 == fviFebType[uDpb][uCrobIdx][uFebIdx] ? 'B' : 'A', fviFebType[uDpb][uCrobIdx][uFebIdx],
                             fviFebAddress[uGlobalFebIdx], fvdFebAdcGain[uGlobalFebIdx], fvdFebAdcOffs[uGlobalFebIdx]);
        uGlobalFebIdx++;
      }
    }
  }
*/
}

// -------------------------------------------------------------------------
void CbmMuchUnpackAlgo::processErrorInfo(const stsxyter::Message& mess)
{
  if (mess.IsMsErrorFlagOn()) {
    //   I do pass here the Ts start time instead of the ms time, since, we removed the ms time as member for the time being
    if (fMonitor) { fMonitor->FillMsErrorsEvo(fMsStartTime, mess.GetMsErrorType()); }
    if (fOptOutBVec)
      fOptOutBVec->emplace_back(
        CbmErrorMessage(ECbmModuleId::kMuch, fMsStartTime, fuCurrDpbIdx, 0x20, mess.GetMsErrorType()));
  }
}

// ---- processHitInfo ----
void CbmMuchUnpackAlgo::processHitInfo(const stsxyter::Message& mess)
{
  const uint16_t usElinkIdx = mess.GetLinkIndexHitBinning();
  // const uint32_t uCrobIdx   = usElinkIdx / fNrElinksPerCrob;
  // Get the asic index
  //uint32_t uAsicIdx = getFebIndex(fuCurrDpbIdx, uCrobIdx, usElinkIdx);
  //Below will generate asicidx from 0 to 8. According to Elink Number.
  uint32_t asicidx = fUnpackPar->ElinkIdxToFebIdx(usElinkIdx);
  //Below will generate uAsicIdx from 0 to maximum number of FEBs.
  uint32_t uAsicIdx = fuCurrDpbIdx * fUnpackPar->GetNbFebsPerDpb() + asicidx;
  //Below will generate FEB Position Number according to GEM and RPC.
  int32_t uFebIdx = fUnpackPar->GetFebId(uAsicIdx);
  if (-1 == uFebIdx) {
    LOG(warning) << "CbmMuchUnpackAlgo::processHitInfo => "
                 << "Particular FEB is not connected. Digi is not created for this hit. Check FEB number " << uAsicIdx
                 << " generate FEB Position " << uFebIdx;
    fuSkippMessNoValidFebPosition++;
    return;
  }
  if (-2 == uFebIdx) {
    LOG(error) << "CbmMuchUnpackAlgo::processHitInfo => "
               << "Wrong Elink Idx! "
               << " Feb " << uFebIdx << " Elink " << usElinkIdx;
    fuSkippMessNoValidElink++;
    return;
  }
  const uint16_t usChan     = mess.GetHitChannel();
  const uint16_t usRawAdc   = mess.GetHitAdc();
  const uint16_t usRawTs    = mess.GetHitTimeBinning();
  const uint32_t uChanInFeb = usChan + fNrChsPerAsic * (uAsicIdx % fNrAsicsPerFeb);

  // Compute the Full time stamp
  const uint64_t ulHitTime = getFullTimeStamp(usRawTs);

  //uFebIdx +=  ( fuCurrDpbIdx * fNrCrobPerDpb + ____CrobIndexCalculationIfNeeded___() ) * fuNbFebsPerCrob;
  //uncomment below line if kuNbCrobsPerDpb > 1
  //uFebIdx += (fuCurrDpbIdx * fNrCrobPerDpb) * fNrFebsPerCrob;  //no Crob index calculation for now


  if (fMonitor) fMonitor->CountRawHit(uFebIdx, uChanInFeb);

  /// Store hit !!!
  if (fvbFebAdcCut[uFebIdx] < usRawAdc) {
    /// Store only if masking is disabled or if channeld is not masked
    /// 2D vector is safe as always right size if masking enabled
    if (false == fbUseChannelMask || false == fvvbMaskedChannels[uFebIdx][uChanInFeb]) {

      /// Duplicate hits rejection
      if (fbRejectDuplicateDigis) {
        if (usRawTs == fvvusLastTsChan[uAsicIdx][usChan]
            && (fbDupliWithoutAdc || usRawAdc == fvvusLastAdcChan[uAsicIdx][usChan])
            && fulTsMsbIndexInTs[fuCurrDpbIdx] == fvvulLastTsMsbChan[uAsicIdx][usChan]) {
          /// FIXME: add plots to check what is done in this rejection
          LOG(debug1) << "CbmMuchUnpackAlgo::processHitInfo => "
                      << Form("Rejecting duplicate on Asic %3u channel %3u, TS %3u, ADC %2u", uAsicIdx, usChan, usRawTs,
                              usRawAdc);

          if (fMonitor) fMonitor->FillDuplicateHitsAdc(uAsicIdx, uChanInFeb, usRawAdc);
          return;
        }  // if same TS, (ADC,) TS MSB, TS MSB cycle, reject
        fvvusLastTsChan[uAsicIdx][usChan]    = usRawTs;
        fvvusLastAdcChan[uAsicIdx][usChan]   = usRawAdc;
        fvvulLastTsMsbChan[uAsicIdx][usChan] = fulTsMsbIndexInTs[fuCurrDpbIdx];
      }  // if (fbRejectDuplicateDigis)

      //uint32_t uChanInMod = usChan + fNrChsPerAsic * (uAsicIdx % fNrAsicsPerFeb);

      /// FIXME: see issue #1549
      /// N side: 0-1023 =>    0-1023
      /// P side: 0-1023 => 2047-1024

      //if (0 == fviFebSide[uFebIdx])
      // uChanInMod = fNrChsPerFeb - uChanInMod - 1  // Invert channel order
      //              + fNrChsPerFeb;                // Offset for P (back) side

      const double_t tsreltime = static_cast<double_t>(ulHitTime) * stsxyter::kdClockCycleNs;
      double dTimeInNs         = tsreltime - fSystemTimeOffset;
      if (uAsicIdx < fvdTimeOffsetNsAsics.size()) dTimeInNs -= fvdTimeOffsetNsAsics[uAsicIdx];

      const uint64_t ulTimeInNs = static_cast<uint64_t>(dTimeInNs);
      //const double dCalAdc      = fvdFebAdcOffs[uFebIdx] + (usRawAdc - 1) * fvdFebAdcGain[uFebIdx];

      //if (0 == fviFebAddress[uFebIdx] || -1 == fviFebSide[uFebIdx]) {
      // LOG(error) << Form("Digi on disabled FEB %02u has address 0x%08x and side %d", uFebIdx, fviFebAddress[uFebIdx],
      //                     fviFebSide[uFebIdx]);
      //}

      if (fMonitor) fMonitor->CountDigi(uAsicIdx, uChanInFeb);
      // uFebIdx is FEB position in the GEM and RPC
      const uint32_t address = CreateMuchAddress(fuCurrDpbIdx, uFebIdx, uChanInFeb);
      LOG(debug) << "Created Much Address = " << address << " from fuCurrDpbIdx " << fuCurrDpbIdx << " Feb Id "
                 << uFebIdx << " Channnel Id " << uChanInFeb;
      if (address) {
        /// Prevent Clang from onlining
        // Storing Pulser FEB Data in another output stream (pulser feb gives -7 )
        if (-7 == uFebIdx) {
          //presently for Pulser Output CbmMuchDigi address is 0
          fOptOutAVec->emplace_back(CbmMuchDigi(address, usRawAdc, ulTimeInNs));
          return;
        }  // if (-7 == uFebIdx)
        fOutputVec.emplace_back(CbmMuchDigi(address, usRawAdc, ulTimeInNs));
      }
      else {
        LOG(warning) << "Digi not created due to generated address is " << address;
        fuSkippMessNoValidAddress++;
      }
      /// Catch the pulser digis and either save them to their own output or drop them
      //Commented below 6 lines as we do not use FebPulser Vector
      //if (fvbFebPulser[uFebIdx]) {
      //  if (fOptOutAVec) {
      //    fOptOutAVec->emplace_back(CbmMuchDigi(address, usRawAdc, ulTimeInNs));
      //  }  // if (fOptOutAVec)
      // }    // if (fvbFebPulser[uFebIdx])
      //else {
      //  fOutputVec.emplace_back(CbmMuchDigi(address, usRawAdc, ulTimeInNs));
      //}  // else of if (fvbFebPulser[uFebIdx])

      /// If EM flag ON, store a corresponding error message with the next flag after all other possible status flags set
      if (mess.IsHitMissedEvts())
        if (fOptOutBVec)
          fOptOutBVec->emplace_back(
            CbmErrorMessage(ECbmModuleId::kMuch, dTimeInNs, uAsicIdx, 1 << stsxyter::kusLenStatStatus, usChan));
    }
  }

  // Convert the Hit time in bins to Hit time in ns
  const double dHitTimeNs = ulHitTime * stsxyter::kdClockCycleNs;

  if (fMonitor) {
    // Check Starting point of histos with time as X axis
    if (-1 == fdStartTime) { fdStartTime = dHitTimeNs; }
    if (fMonitor->GetDebugMode()) {
      fMonitor->FillHitDebugMonitoringHistos(uAsicIdx, usChan, usRawAdc, usRawTs, mess.IsHitMissedEvts());
    }
    const uint32_t uAsicInFeb       = uAsicIdx % fNrAsicsPerFeb;
    const double dTimeSinceStartSec = (dHitTimeNs - fdStartTime) * 1e-9;
    const double dCalAdc            = usRawAdc;
    // const double dCalAdc            = fvdFebAdcOffs[uFebIdx] + (usRawAdc - 1) * fvdFebAdcGain[uFebIdx];
    fMonitor->FillHitMonitoringHistos(uFebIdx, usChan, uChanInFeb, usRawAdc, dCalAdc, usRawTs, mess.IsHitMissedEvts());
    fMonitor->FillHitEvoMonitoringHistos(uFebIdx, uAsicIdx, uAsicInFeb, uChanInFeb, dTimeSinceStartSec,
                                         mess.IsHitMissedEvts());
  }
}

//---------------CreateMuchDigi -------
//To do address need to be checked carefully
uint32_t CbmMuchUnpackAlgo::CreateMuchAddress(uint32_t dpbidx, int32_t iFebId, uint32_t usChan)
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

  //Int_t usChan = itHitIn->GetChan();

  // ----------------------------- //
  // Channel flip in stsXYTER v2.1 : 0<->1, 2<->3, 3<->4 and so on...
  auto fiFlag = 1;  //! Switch to smx2.0/smx2.1 data-> fiFlag = 0 for 2.0 and fiFlag = 1 for 2.1
  if (fiFlag == 1) {
    if (usChan % 2 == 0) usChan = usChan + 1;
    else
      usChan = usChan - 1;
  }
  // ---------------------------- //
  // auto iFebId = >GetFebId(febId);
  // ---------------------------- //
  // PadX means CHANNEL for CbmMuchAddress
  // PadY means SECTOR for CbmMuchAddress
  //Due to two FLEX cable connected to single FEB; First Flex Connector number 1 - 63 and second flex connector number 64 - 127
  //in few FEB positioned these flex connectors are flipped so below correction applied.
  if (station == 0 && fiFlag == 1 && layer == 0) {  // First layer (GEM1) has old readout PCB
    if (iFebId == 0 || iFebId == 1 || iFebId == 2 || iFebId == 3 || iFebId == 4 || iFebId == 8 || iFebId == 9
        || iFebId == 10 || iFebId == 11 || iFebId == 17) {
      sChannel = fUnpackPar->GetPadXA(iFebId, 127 - usChan);
      sSector  = fUnpackPar->GetPadYA(iFebId, 127 - usChan);
    }
    else {
      sChannel = fUnpackPar->GetPadXA(iFebId, usChan);
      sSector  = fUnpackPar->GetPadYA(iFebId, usChan);
    }
  }                                                      // if( fiFlag == 1 && layer == 0 )
  else if (station == 1 && fiFlag == 1 && layer == 0) {  // second layer (GEM2) has new readout PCB
    if (iFebId == 0 || iFebId == 1 || iFebId == 2 || iFebId == 3 || iFebId == 4 || iFebId == 8 || iFebId == 9
        || iFebId == 10 || iFebId == 11 || iFebId == 17) {
      sChannel = fUnpackPar->GetPadXB(iFebId, 127 - usChan);
      sSector  = fUnpackPar->GetPadYB(iFebId, 127 - usChan);
    }
    else {
      sChannel = fUnpackPar->GetPadXB(iFebId, usChan);
      sSector  = fUnpackPar->GetPadYB(iFebId, usChan);
    }
  }                                         // else if( fiFlag == 1 && layer == 1 )
  else if (station == 0 || station == 1) {  // Both layer with same type of PCB (Probably below is not necessary)
    if (iFebId == 0 || iFebId == 1 || iFebId == 2 || iFebId == 3 || iFebId == 4 || iFebId == 8 || iFebId == 9
        || iFebId == 10 || iFebId == 11 || iFebId == 17) {
      sChannel = fUnpackPar->GetPadXA(iFebId, 127 - usChan);
      sSector  = fUnpackPar->GetPadYA(iFebId, 127 - usChan);
    }
    else {
      sChannel = fUnpackPar->GetPadXA(iFebId, usChan);
      sSector  = fUnpackPar->GetPadYA(iFebId, usChan);
    }
  }                         // else{
  else if (station == 2) {  //For RPC
    sChannel = fUnpackPar->GetPadXRpc(iFebId, usChan);
    sSector  = fUnpackPar->GetPadYRpc(iFebId, usChan);
  }
  else  // Checking for the not connected or misconnected pads
  {
    LOG(debug) << "Sector " << sSector << " channel " << sChannel << " is not connected or misconnected to pad. "
               << " corresponding Feb is " << iFebId << " and uschan " << usChan << " DPB id " << dpbidx
               << " Skipping this hit message.";
    return 0;
  }
  //Creating Unique address of the particular channel of GEM
  //UInt_t address     = CbmMuchAddress::GetAddress(station, layer, layerside, sModule, sChannel, sSector);
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
  /*
  Double_t dTimeInNs = itHitIn->GetTs() * stsxyter::kdClockCycleNs - fdTimeOffsetNs;
  if (uAsicIdx < fvdTimeOffsetNsAsics.size()) dTimeInNs -= fvdTimeOffsetNsAsics[uAsicIdx];
  ULong64_t ulTimeInNs = static_cast<ULong64_t>(dTimeInNs);

  /// With Main MUCH digi
  //         fDigiVect.push_back( CbmMuchDigi( address, itHitIn->GetAdc(), ulTimeInNs ) );

  /// With Beamtime MUCH digi

  CbmMuchBeamTimeDigi* digi = new CbmMuchBeamTimeDigi(address, itHitIn->GetAdc(), ulTimeInNs);
  LOG(debug) << "Sector " << sSector << " channel " << sChannel << " layer " << layer << " Address " << address
             << " Time " << ulTimeInNs;

  digi->SetPadX(sSector);
  digi->SetPadY(sChannel);
  digi->SetRocId(itHitIn->GetDpb());
  digi->SetNxId(itHitIn->GetAsic());
  digi->SetNxCh(itHitIn->GetChan()); */
  // Add Elink also in the stsxyter::FinalHit as one more variable such that may be used.
  //digi.SetElink(itHitIn->GetElink());

  return address;
}

//----------------------------------

// ---- processStatusInfo ----
void CbmMuchUnpackAlgo::processStatusInfo(const stsxyter::Message& mess, uint32_t uIdx)
{
  // again fMonitor settings used for debugging printouts, I would propose to separate this
  if (fMonitor)
    if (fMonitor->GetDebugMode()) {
      std::cout << Form("DPB %2u TS %12lu mess %5u ", fuCurrDpbIdx, fTsIndex, uIdx);
      mess.PrintMess(std::cout, stsxyter::MessagePrintMask::msg_print_Human);
    }

  const uint16_t usElinkIdx = mess.GetStatusLink();
  // const uint32_t uCrobIdx   = usElinkIdx / fNrElinksPerCrob;
  const int32_t uFebIdx = fElinkIdxToFebIdxVec.at(usElinkIdx);
  if (-1 == uFebIdx) {
    LOG(warning) << "CbmMuchUnpackAlgo::processStatusInfo => "
                 << "Wrong elink Idx! Elink raw " << Form("%d remap %d", usElinkIdx, uFebIdx);
    return;
  }
  //const uint32_t uAsicIdx = getFebIndex(fuCurrDpbIdx, uCrobIdx, usElinkIdx);
  //Below will generate asicidx from 0 to 8. According to Elink Number.
  uint32_t asicidx = fUnpackPar->ElinkIdxToFebIdx(usElinkIdx);
  //Below will generate uAsicIdx from 0 to maximum number of FEBs.
  uint32_t uAsicIdx = fuCurrDpbIdx * fUnpackPar->GetNbFebsPerDpb() + asicidx;
  //Below will generate FEB Position Number according to GEM and RPC.
  //int32_t uFebIdx   = fUnpackPar->GetFebId(uAsicIdx);
  if (fMonitor) {
    const uint16_t usStatusField = mess.GetStatusStatus();
    fMonitor->FillMuchStatusMessType(uAsicIdx, usStatusField);
  }
  /// Compute the Full time stamp
  const int64_t ulTime = getFullTimeStamp(0);

  /// Convert the time in bins to Hit time in ns
  const double dTimeNs = ulTime * stsxyter::kdClockCycleNs;
  if (fOptOutBVec)
    fOptOutBVec->emplace_back(
      CbmErrorMessage(ECbmModuleId::kMuch, dTimeNs, uAsicIdx, mess.GetStatusStatus(), mess.GetData()));
}


// ---- processTsMsbInfo ----
void CbmMuchUnpackAlgo::processTsMsbInfo(const stsxyter::Message& mess, uint32_t uMessIdx, uint32_t uMsIdx)
{
  const uint32_t uVal = mess.GetTsMsbValBinning();

  // Update Status counters
  if (uVal < fvulCurrentTsMsb[fuCurrDpbIdx]) {

    LOG(debug) << " TS " << std::setw(12) << fTsIndex << " MS Idx " << std::setw(4) << uMsIdx << " Msg Idx "
               << std::setw(5) << uMessIdx << " DPB " << std::setw(2) << fuCurrDpbIdx << " Old TsMsb " << std::setw(5)
               << fvulCurrentTsMsb[fuCurrDpbIdx] << " Old MsbCy " << std::setw(5) << fvuCurrentTsMsbCycle[fuCurrDpbIdx]
               << " new TsMsb " << std::setw(5) << uVal;

    fvuCurrentTsMsbCycle[fuCurrDpbIdx]++;
  }
  if (uVal != fvulCurrentTsMsb[fuCurrDpbIdx] + 1
      /// Case where we reach a normal cycle edge
      && !(0 == uVal && stsxyter::kuTsMsbNbTsBinsBinning == fvulCurrentTsMsb[fuCurrDpbIdx])
      /// First TS_MSB in MS may jump if TS dropped by DAQ
      && !(1 == uMessIdx && 0 == uMsIdx)
      /// Msg 1 and 2 will be same TS_MSB if data in 1st bin
      && !(uVal == fvulCurrentTsMsb[fuCurrDpbIdx] && 2 == uMessIdx)
      /// New FW introduced TS_MSB suppression + large TS_MSB => warning only if value not increasing
      && uVal < fvulCurrentTsMsb[fuCurrDpbIdx]) {
    LOG(debug) << "TS MSB Jump in "
               << " TS " << std::setw(12) << fTsIndex << " MS Idx " << std::setw(4) << uMsIdx << " Msg Idx "
               << std::setw(5) << uMessIdx << " DPB " << std::setw(2) << fuCurrDpbIdx << " => Old TsMsb "
               << std::setw(5) << fvulCurrentTsMsb[fuCurrDpbIdx] << " new TsMsb " << std::setw(5) << uVal;
  }

  fvulCurrentTsMsb[fuCurrDpbIdx] = uVal;

  LOG(debug1) << " TS " << std::setw(12) << fTsIndex << " MS Idx " << std::setw(4) << uMsIdx << " Msg Idx "
              << std::setw(5) << uMessIdx << " DPB " << std::setw(2) << fuCurrDpbIdx << " TsMsb " << std::setw(5)
              << fvulCurrentTsMsb[fuCurrDpbIdx] << " MsbCy " << std::setw(5) << fvuCurrentTsMsbCycle[fuCurrDpbIdx];

  fulTsMsbIndexInTs[fuCurrDpbIdx] =
    fvulCurrentTsMsb[fuCurrDpbIdx]
    + (fvuCurrentTsMsbCycle[fuCurrDpbIdx] * static_cast<uint64_t>(1 << stsxyter::kusLenTsMsbValBinning));
  if (fulTsMsbIndexInTs[fuCurrDpbIdx] < fulTsStartInTsMsb) {
    LOG(fatal) << "CbmMuchUnpackAlgo::processTsMsbInfo => "
               << "Value computed from TS_MSB and TS_MSB cycle smaller than Timeslice start in TS_MSB, "
               << "would lead to a negative value so it cannot be recovered!!!!"
               << std::endl
               /// Values Printout
               << "TS_MSB: " << fvulCurrentTsMsb[fuCurrDpbIdx] << " Cycle: " << fvuCurrentTsMsbCycle[fuCurrDpbIdx]
               << " Full TS_MSB: " << fulTsMsbIndexInTs[fuCurrDpbIdx] << " TS Start offset: " << fulTsStartInTsMsb;
  }
  fulTsMsbIndexInTs[fuCurrDpbIdx] -= fulTsStartInTsMsb;

  if (fMonitor)
    if (fMonitor->GetDebugMode()) {  //also if( 1 < uMessIdx )?
      fMonitor->FillMuchDpbRawTsMsb(fuCurrDpbIdx, fvulCurrentTsMsb[fuCurrDpbIdx]);
      fMonitor->FillMuchDpbRawTsMsbSx(fuCurrDpbIdx, fvulCurrentTsMsb[fuCurrDpbIdx]);
      fMonitor->FillMuchDpbRawTsMsbDpb(fuCurrDpbIdx, fvulCurrentTsMsb[fuCurrDpbIdx]);
    }
}

// ---- refreshTsMsbFields ----
void CbmMuchUnpackAlgo::refreshTsMsbFields(const uint32_t imslice, const size_t mstime)
{
  const uint32_t uTsMsbCycleHeader =
    std::floor(mstime / (stsxyter::kulTsCycleNbBinsBinning * stsxyter::kdClockCycleNs));
  const uint32_t uTsMsbHeader =
    std::floor((mstime - uTsMsbCycleHeader * (stsxyter::kulTsCycleNbBinsBinning * stsxyter::kdClockCycleNs))
               / (stsxyter::kuHitNbTsBinsBinning * stsxyter::kdClockCycleNs));

  LOG(debug1) << " TS " << std::setw(12) << fTsIndex << " MS " << std::setw(12) << mstime << " MS Idx " << std::setw(4)
              << imslice << " Msg Idx " << std::setw(5) << 0 << " DPB " << std::setw(2) << fuCurrDpbIdx << " Old TsMsb "
              << std::setw(5) << fvulCurrentTsMsb[fuCurrDpbIdx] << " Old MsbCy " << std::setw(5)
              << fvuCurrentTsMsbCycle[fuCurrDpbIdx] << " header TsMsb " << uTsMsbHeader << " New MsbCy "
              << uTsMsbCycleHeader;

  if (0 == imslice) {
    if (uTsMsbCycleHeader != fvuCurrentTsMsbCycle[fuCurrDpbIdx])
      LOG(debug) << " TS " << std::setw(12) << fTsIndex << " MS " << std::setw(12) << mstime << " MS Idx "
                 << std::setw(4) << imslice << " Msg Idx " << std::setw(5) << 0 << " DPB " << std::setw(2)
                 << fuCurrDpbIdx << " Old TsMsb " << std::setw(5) << fvulCurrentTsMsb[fuCurrDpbIdx] << " Old MsbCy "
                 << std::setw(5) << fvuCurrentTsMsbCycle[fuCurrDpbIdx] << " New TsMsb " << uTsMsbHeader << " New MsbCy "
                 << uTsMsbCycleHeader;
    fvuCurrentTsMsbCycle[fuCurrDpbIdx] = uTsMsbCycleHeader;
    fvulCurrentTsMsb[fuCurrDpbIdx]     = uTsMsbHeader;

    fulTsMsbIndexInTs[fuCurrDpbIdx] =
      fvulCurrentTsMsb[fuCurrDpbIdx]
      + (fvuCurrentTsMsbCycle[fuCurrDpbIdx] * static_cast<uint64_t>(1 << stsxyter::kusLenTsMsbValBinning));
    if (fulTsMsbIndexInTs[fuCurrDpbIdx] < fulTsStartInTsMsb) {
      LOG(fatal) << "CbmMuchUnpackAlgo::refreshTsMsbFields => "
                 << "Value computed from TS_MSB and TS_MSB cycle smaller than Timeslice start in TS_MSB, "
                 << "would lead to a negative value so it cannot be recovered!!!!"
                 << std::endl
                 /// Values Printout
                 << "TS_MSB: " << fvulCurrentTsMsb[fuCurrDpbIdx] << " Cycle: " << fvuCurrentTsMsbCycle[fuCurrDpbIdx]
                 << " Full TS_MSB: " << fulTsMsbIndexInTs[fuCurrDpbIdx] << " TS Start offset: " << fulTsStartInTsMsb;
    }
    fulTsMsbIndexInTs[fuCurrDpbIdx] -= fulTsStartInTsMsb;
  }
  else if (uTsMsbCycleHeader != fvuCurrentTsMsbCycle[fuCurrDpbIdx]) {
    if ((stsxyter::kuTsMsbNbTsBinsBinning - 1) == fvulCurrentTsMsb[fuCurrDpbIdx]) {
      /// Transition at the edge of two MS, the first TS_MSB message (idx 1) will make the cycle
      LOG(debug) << " TS " << std::setw(12) << fTsIndex << " MS " << std::setw(12) << mstime << " MS Idx "
                 << std::setw(4) << imslice << " Msg Idx " << std::setw(5) << 0 << " DPB " << std::setw(2)
                 << fuCurrDpbIdx << " Old TsMsb " << std::setw(5) << fvulCurrentTsMsb[fuCurrDpbIdx] << " Old MsbCy "
                 << std::setw(5) << fvuCurrentTsMsbCycle[fuCurrDpbIdx] << " New MsbCy " << uTsMsbCycleHeader;
    }
    else {
      LOG(warning) << "TS MSB cycle from MS header does not match current cycle from data "
                   << "for TS " << std::setw(12) << fTsIndex << " MS " << std::setw(12) << mstime << " MsInTs "
                   << std::setw(3) << imslice << " ====> " << fvuCurrentTsMsbCycle[fuCurrDpbIdx] << " (cnt) VS "
                   << uTsMsbCycleHeader << " (header)";
      fvuCurrentTsMsbCycle[fuCurrDpbIdx] = uTsMsbCycleHeader;
    }
  }
}

// ---- unpack ----
bool CbmMuchUnpackAlgo::unpack(const fles::Timeslice* ts, std::uint16_t icomp, UInt_t imslice)
{
  auto msDescriptor = ts->descriptor(icomp, imslice);

  /// If monitoring enable, fill here the "Per Timeslice" histograms with info from the previous TS
  /// as only way to detect TS transitions due to the multiple calls in case of multiple components.
  /// Time set to tlast MS in previous TS. Last TS will be missed.
  if (fMonitor && 0 == imslice && 0 < fMsStartTime && msDescriptor.idx == ts->start_time()
      && fMsStartTime < msDescriptor.idx) {
    fMonitor->FillPerTimesliceCountersHistos(static_cast<double_t>(fMsStartTime) / 1e9);
  }

  //Current equipment ID, tells from which DPB the current MS is originating
  const uint32_t uCurrentEquipmentId = msDescriptor.eq_id;
  const uint8_t* msContent           = reinterpret_cast<const uint8_t*>(ts->content(icomp, imslice));
  const uint32_t uSize               = msDescriptor.size;

  fMsStartTime = msDescriptor.idx;
  LOG(debug) << "Microslice: " << fMsStartTime << " from EqId " << std::hex << uCurrentEquipmentId << std::dec
             << " has size: " << uSize << " (index " << imslice << ")";

  if (0 == fvbMaskedComponents.size()) fvbMaskedComponents.resize(ts->num_components(), false);

  //Temp holder until current equipment ID is properly filled in MS
  const uint32_t uCurrDpbId = static_cast<uint32_t>(uCurrentEquipmentId & 0xFFFF);

  if (fMonitor)
    if (fMonitor->GetDebugMode()) {
      const double dMsTime = (1e-9) * static_cast<double>(fMsStartTime);
      if (icomp < fMonitor->GetMaxNbFlibLinks()) {
        if (fdStartTimeMsSz < 0) fdStartTimeMsSz = dMsTime;
        fMonitor->CreateMsComponentSizeHistos(icomp);
        fMonitor->FillMsSize(icomp, uSize);
        fMonitor->FillMsSizeTime(icomp, dMsTime - fdStartTimeMsSz, uSize);
      }
    }

  /// Check if this sDPB ID was declared in parameter file and stop there if not
  auto it = fDpbIdIndexMap.find(uCurrDpbId);
  if (it == fDpbIdIndexMap.end()) {
    if (false == fvbMaskedComponents[icomp]) {
      // LOG(debug) << "---------------------------------------------------------------";
      // Had to remove this line otherwise we would get circle dependencies in the current stage of cbmroot, since we still have Unpackers in the fles folders, which require the reco folders
      // LOG(debug) << FormatMsHeaderPrintout(msDescriptor);
      LOG(warning) << fName << "::unpack(...)::Could not find the sDPB index for AFCK id 0x" << std::hex << uCurrDpbId
                   << std::dec << " in timeslice " << fNrProcessedTs << " in microslice " << imslice << " component "
                   << icomp << "\n"
                   << "If valid this index has to be added in the STS "
                      "parameter file in the DbpIdArray field";
      fvbMaskedComponents[icomp] = true;

      /// If first TS being analyzed, we are probably detecting STS/MUCH boards with same sysid
      /// => Do not report the MS as bad, just ignore it
      if (1 == fNrProcessedTs) return true;
    }
    else
      return true;

    // REMARK please check experts, but I think this point can never be reached PR 072021
    return false;
  }
  else
    fuCurrDpbIdx = fDpbIdIndexMap[uCurrDpbId];

  if (fMonitor) fMonitor->FillMsCntEvo(fMsStartTime);

  if (0 == imslice) {
    /// Extract the time base only on MS 0, assuming that we get all TS of a component in order
    fulTsStartInTsMsb =
      static_cast<uint64_t>(fTsStartTime / (stsxyter::kuHitNbTsBinsBinning * stsxyter::kdClockCycleNs));
  }

  // Check the current TS_MSb cycle and correct it if wrong
  refreshTsMsbFields(imslice, fMsStartTime);

  //Reset internal monitor variables for debugging info
  if (fMonitor) {
    if (fMonitor->GetDebugMode()) { fMonitor->ResetDebugInfo(); }
  }

  //Main processing of MS content
  loopMsMessages(msContent, uSize, imslice);

  //Output debugging info
  if (fMonitor) {
    if (fMonitor->GetDebugMode()) { fMonitor->PrintDebugInfo(fMsStartTime, fNrProcessedTs, msDescriptor.flags, uSize); }
    for (auto itHit = fOutputVec.begin(); itHit != fOutputVec.end(); ++itHit) {
      fMonitor->FillDigisTimeInRun(itHit->GetTime());
    }
    fMonitor->FillVectorSize(ts->index(), fOutputVec.size());
    //fMonitor->DrawCanvases();
  }
  return true;
}

ClassImp(CbmMuchUnpackAlgo)
