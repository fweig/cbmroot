/* Copyright (C) 2021 Goethe-University, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau, Pascal Raisig [committer], Dominik Smith */

#include "CbmStsUnpackAlgo.h"

#include "CbmStsDigi.h"

#include <FairParGenericSet.h>
#include <FairTask.h>
#include <Logger.h>

#include <Rtypes.h>
#include <RtypesCore.h>

#include <cstdint>
#include <iomanip>

CbmStsUnpackAlgo::CbmStsUnpackAlgo() : CbmStsUnpackAlgoBase("CbmStsUnpackAlgo") {}

CbmStsUnpackAlgo::~CbmStsUnpackAlgo() {}

// ---- getAsicIndex ----
uint32_t CbmStsUnpackAlgo::getAsicIndex(uint32_t dpbidx, uint32_t crobidx, uint16_t elinkidx)
{

  uint32_t asicidx       = 0;
  const int32_t uFebIdx  = fElinkIdxToFebIdxVec.at(elinkidx);
  const uint32_t febtype = fviFebType[dpbidx][crobidx][uFebIdx];
  // Feb type a
  if (febtype == 0) asicidx = fElinkIdxToAsicIdxVec.at(elinkidx).first;
  //   Feb type b
  if (febtype == 1) asicidx = fElinkIdxToAsicIdxVec.at(elinkidx).second;
  // else would be inactive feb, this was not handled in the previous implementation, this I expect it should not happen

  const uint32_t uAsicIdx = (dpbidx * fNrCrobPerDpb + crobidx) * fNrAsicsPerCrob + asicidx;
  return uAsicIdx;
}

// ---- getFullTimeStamp ----
uint64_t CbmStsUnpackAlgo::getFullTimeStamp(const uint16_t usRawTs)
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
Bool_t CbmStsUnpackAlgo::init() { return kTRUE; }

// ---- initDpbIdIndexMap ----
void CbmStsUnpackAlgo::initDpbIdIndexMap(CbmMcbm2018StsPar* parset)
{
  fDpbIdIndexMap.clear();
  for (uint32_t uDpb = 0; uDpb < parset->GetNrOfDpbs(); ++uDpb) {
    fDpbIdIndexMap[parset->GetDpbId(uDpb)] = uDpb;
    LOG(debug) << "Eq. ID for DPB #" << std::setw(2) << uDpb << " = 0x" << std::setw(4) << std::hex
               << parset->GetDpbId(uDpb) << std::dec << " => " << fDpbIdIndexMap[parset->GetDpbId(uDpb)];
  }
}

// ---- initParSet(FairParGenericSet* parset) ----
Bool_t CbmStsUnpackAlgo::initParSet(FairParGenericSet* parset)
{
  LOG(info) << fName << "::initParSet - for container " << parset->ClassName();
  if (parset->IsA() == CbmMcbm2018StsPar::Class()) return initParSet(static_cast<CbmMcbm2018StsPar*>(parset));

  // If we do not know the derived ParSet class we return false
  LOG(error)
    << fName << "::initParSet - for container " << parset->ClassName()
    << " failed, since CbmStsUnpackAlgo::initParSet() does not know the derived ParSet and what to do with it!";
  return kFALSE;
}

// ---- initParSet(CbmMcbm2018StsPar* parset) ----
Bool_t CbmStsUnpackAlgo::initParSet(CbmMcbm2018StsPar* parset)
{
  LOG(debug) << fName << "::initParSetAsic - ";

  //Type of each module: 0 for connectors on the right, 1 for connectors on the left
  std::vector<int32_t> viModuleType;

  // STS address for the first strip of each module
  std::vector<int32_t> viModAddress;

  // Idx of the STS module for each FEB, [ NbDpb ][ NbCrobPerDpb ][ NbFebsPerCrob ], -1 if inactive
  std::vector<std::vector<std::vector<int32_t>>> viFebModuleIdx;

  // Array to hold the active flag for all CROBs, [ NbDpb ][ NbCrobPerDpb ]
  std::vector<std::vector<bool>> vbCrobActiveFlag;

  //STS module side for each FEB, [ NbDpb ][ NbCrobPerDpb ][ NbFebsPerCrob ], 0 = P, 1 = N, -1 if inactive
  std::vector<std::vector<std::vector<int32_t>>> viFebModuleSide;

  // Total number of STS modules in the setup
  const uint32_t uNbModules = parset->GetNbOfModules();
  LOG(debug) << "Nr. of STS Modules:    " << uNbModules;

  // Total number of STS DPBs in system
  const uint32_t uNbOfDpbs = parset->GetNrOfDpbs();
  LOG(debug) << "Nr. of STS DPBs:       " << uNbOfDpbs;

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

  for (size_t ielink = 0; ielink < fNrElinksPerCrob; ++ielink) {
    fElinkIdxToFebIdxVec.emplace_back(parset->ElinkIdxToFebIdx(ielink));
    fElinkIdxToAsicIdxVec.emplace_back(
      std::make_pair(parset->ElinkIdxToAsicIdxFebA(ielink), parset->ElinkIdxToAsicIdxFebB(ielink)));
  }

  // Get Nr of Asics
  const uint32_t uNbStsXyters = parset->GetNrOfAsics();
  LOG(debug) << "Nr. of StsXyter ASICs: " << uNbStsXyters;

  //Initialize temporary "per Feb" fields
  initTempVectors(parset, &viModuleType, &viModAddress, &viFebModuleIdx, &vbCrobActiveFlag, &viFebModuleSide);

  // Initialize ADC cuts for FEBs
  fvbFebAdcCut.resize(fuNbFebs, fdAdcCut);
  for (auto cut : fdAdcCut_perFeb) {
    fvbFebAdcCut[cut.first] = cut.second;
  }

  // Read dpb index map from parameter container
  initDpbIdIndexMap(parset);

  if (fvdTimeOffsetNsAsics.size() < uNbStsXyters) { fvdTimeOffsetNsAsics.resize(uNbStsXyters, 0.0); }

  //Initialize class-wide "per Feb" fields
  fviFebType.resize(uNbOfDpbs);

  for (uint32_t uDpb = 0; uDpb < uNbOfDpbs; ++uDpb) {
    fviFebType[uDpb].resize(fNrCrobPerDpb);
    for (uint32_t uCrobIdx = 0; uCrobIdx < fNrCrobPerDpb; ++uCrobIdx) {
      fviFebType[uDpb][uCrobIdx].resize(parset->GetNbFebsPerCrob(), -1);
      for (uint32_t uFebIdx = 0; uFebIdx < parset->GetNbFebsPerCrob(); ++uFebIdx) {
        fvbFebPulser.push_back(parset->IsFebPulser(uDpb, uCrobIdx, uFebIdx));
        fvdFebAdcGain.push_back(parset->GetFebAdcGain(uDpb, uCrobIdx, uFebIdx));
        fvdFebAdcOffs.push_back(parset->GetFebAdcOffset(uDpb, uCrobIdx, uFebIdx));

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

  printActiveCrobs(parset, vbCrobActiveFlag);
  printAddressMaps(parset, viFebModuleIdx, viFebModuleSide);

  LOG(debug) << "Unpacking data in bin sorter FW mode";
  initInternalStatus(parset);

  return kTRUE;
}

// ---- initTempVectors ----
void CbmStsUnpackAlgo::initTempVectors(CbmMcbm2018StsPar* parset, std::vector<int32_t>* viModuleType,
                                       std::vector<int32_t>* viModAddress,
                                       std::vector<std::vector<std::vector<int32_t>>>* viFebModuleIdx,
                                       std::vector<std::vector<bool>>* vbCrobActiveFlag,
                                       std::vector<std::vector<std::vector<int32_t>>>* viFebModuleSide)
{
  const uint32_t uNbModules = parset->GetNbOfModules();
  const uint32_t uNbOfDpbs  = parset->GetNrOfDpbs();

  viModuleType->resize(uNbModules);
  viModAddress->resize(uNbModules);
  for (uint32_t uModIdx = 0; uModIdx < uNbModules; ++uModIdx) {
    (*viModuleType)[uModIdx] = parset->GetModuleType(uModIdx);
    (*viModAddress)[uModIdx] = parset->GetModuleAddress(uModIdx);
    LOG(debug) << "Module #" << std::setw(2) << uModIdx << " Type " << std::setw(4) << (*viModuleType)[uModIdx]
               << " Address 0x" << std::setw(8) << std::hex << (*viModAddress)[uModIdx] << std::dec;
  }
  vbCrobActiveFlag->resize(uNbOfDpbs);
  viFebModuleIdx->resize(uNbOfDpbs);
  viFebModuleSide->resize(uNbOfDpbs);

  for (uint32_t uDpb = 0; uDpb < uNbOfDpbs; ++uDpb) {
    (*vbCrobActiveFlag)[uDpb].resize(fNrCrobPerDpb);
    (*viFebModuleIdx)[uDpb].resize(fNrCrobPerDpb);
    (*viFebModuleSide)[uDpb].resize(fNrCrobPerDpb);
    for (uint32_t uCrobIdx = 0; uCrobIdx < fNrCrobPerDpb; ++uCrobIdx) {
      (*vbCrobActiveFlag)[uDpb][uCrobIdx] = parset->IsCrobActive(uDpb, uCrobIdx);
      (*viFebModuleIdx)[uDpb][uCrobIdx].resize(parset->GetNbFebsPerCrob());
      (*viFebModuleSide)[uDpb][uCrobIdx].resize(parset->GetNbFebsPerCrob());
      for (uint32_t uFebIdx = 0; uFebIdx < parset->GetNbFebsPerCrob(); ++uFebIdx) {
        (*viFebModuleIdx)[uDpb][uCrobIdx][uFebIdx]  = parset->GetFebModuleIdx(uDpb, uCrobIdx, uFebIdx);
        (*viFebModuleSide)[uDpb][uCrobIdx][uFebIdx] = parset->GetFebModuleSide(uDpb, uCrobIdx, uFebIdx);
      }
    }
  }
}

// ---- initInternalStatus ----
void CbmStsUnpackAlgo::initInternalStatus(CbmMcbm2018StsPar* parset)
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
void CbmStsUnpackAlgo::loopMsMessages(const uint8_t* msContent, const uint32_t uSize, const size_t uMsIdx)
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
          LOG(warning) << "CbmStsUnpackAlgo::loopMsMessages => "
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
        LOG(fatal) << "CbmStsUnpackAlgo::loopMsMessages => "
                   << "Unknown message type, should never happen, stopping "
                      "here! Type found was: "
                   << static_cast<int>(typeMess);
      }
    }
  }
}

// ---- MaskNoisyChannel ----
void CbmStsUnpackAlgo::MaskNoisyChannel(const uint32_t uFeb, const uint32_t uChan, const bool bMasked)
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
    LOG(fatal) << "CbmStsUnpackAlgo::MaskNoisyChannel => Invalid FEB "
                  "and/or CHAN index:"
               << Form(" %u vs %u and %u vs %u", uFeb, fuNbFebs, uChan, fNrChsPerFeb);
}


// ---- printActiveCrobs ----
void CbmStsUnpackAlgo::printActiveCrobs(CbmMcbm2018StsPar* parset,
                                        const std::vector<std::vector<bool>>& vbCrobActiveFlag)
{
  for (uint32_t uDpb = 0; uDpb < parset->GetNrOfDpbs(); ++uDpb) {
    TString sPrintoutLine = Form("DPB #%02u CROB Active ?:       ", uDpb);
    for (uint32_t uCrobIdx = 0; uCrobIdx < fNrCrobPerDpb; ++uCrobIdx) {
      sPrintoutLine += Form("%1u", (vbCrobActiveFlag[uDpb][uCrobIdx] == true));
    }
    LOG(debug) << sPrintoutLine;
  }
}

// ---- printAddressMaps ----
void CbmStsUnpackAlgo::printAddressMaps(CbmMcbm2018StsPar* parset,
                                        const std::vector<std::vector<std::vector<int32_t>>>& viFebModuleIdx,
                                        const std::vector<std::vector<std::vector<int32_t>>>& viFebModuleSide)
{
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
}

// -------------------------------------------------------------------------
void CbmStsUnpackAlgo::processErrorInfo(const stsxyter::Message& mess)
{
  if (mess.IsMsErrorFlagOn()) {
    //   I do pass here the Ts start time instead of the ms time, since, we removed the ms time as member for the time being
    if (fMonitor) { fMonitor->FillMsErrorsEvo(fMsStartTime, mess.GetMsErrorType()); }
    if (fOptOutBVec)
      fOptOutBVec->emplace_back(
        CbmErrorMessage(ECbmModuleId::kSts, fMsStartTime, fuCurrDpbIdx, 0x20, mess.GetMsErrorType()));
  }
}

// ---- processHitInfo ----
void CbmStsUnpackAlgo::processHitInfo(const stsxyter::Message& mess)
{
  const uint16_t usElinkIdx = mess.GetLinkIndexHitBinning();
  const uint32_t uCrobIdx   = usElinkIdx / fNrElinksPerCrob;
  int32_t uFebIdx           = fElinkIdxToFebIdxVec.at(usElinkIdx);
  if (-1 == uFebIdx) {
    LOG(warning) << "CbmStsUnpackAlgo::processHitInfo => "
                 << "Wrong elink Idx! Elink raw " << Form("%d remap %d", usElinkIdx, uFebIdx);
    return;
  }
  //uFebIdx +=  ( fuCurrDpbIdx * fNrCrobPerDpb + ____CrobIndexCalculationIfNeeded___() ) * fuNbFebsPerCrob;
  uFebIdx += (fuCurrDpbIdx * fNrCrobPerDpb) * fNrFebsPerCrob;  //no Crob index calculation for now

  // Get the asic index
  uint32_t uAsicIdx = getAsicIndex(fuCurrDpbIdx, uCrobIdx, usElinkIdx);

  const uint16_t usChan     = mess.GetHitChannel();
  const uint16_t usRawAdc   = mess.GetHitAdc();
  const uint16_t usRawTs    = mess.GetHitTimeBinning();
  const uint32_t uChanInFeb = usChan + fNrChsPerAsic * (uAsicIdx % fNrAsicsPerFeb);

  // Compute the Full time stamp
  const uint64_t ulHitTime = getFullTimeStamp(usRawTs);

  if (fMonitor) fMonitor->CountRawHit(uFebIdx, uChanInFeb);

  /// Store hit for output only if it is mapped to a module!!!
  if (fviFebAddress[uFebIdx] != 0 && fvbFebAdcCut[uFebIdx] < usRawAdc) {
    /// Store only if masking is disabled or if channeld is not masked
    /// 2D vector is safe as always right size if masking enabled
    if (false == fbUseChannelMask || false == fvvbMaskedChannels[uFebIdx][uChanInFeb]) {
      // If you want to store this as well, add it to the template as TOptOut, otherwise I do not see a reason to create it at all
      // auto finalhit       = stsxyter::FinalHit(ulHitTime, usRawAdc, uAsicIdx, usChan, fuCurrDpbIdx, uCrobIdx);

      /// Duplicate hits rejection
      if (fbRejectDuplicateDigis) {
        if (usRawTs == fvvusLastTsChan[uAsicIdx][usChan]
            && (fbDupliWithoutAdc || usRawAdc == fvvusLastAdcChan[uAsicIdx][usChan])
            && fulTsMsbIndexInTs[fuCurrDpbIdx] == fvvulLastTsMsbChan[uAsicIdx][usChan]) {
          /// FIXME: add plots to check what is done in this rejection
          LOG(debug) << "CbmStsUnpackAlgo::processHitInfo => "
                     << Form("Rejecting duplicate on Asic %3u channel %3u, TS %3u, ADC %2u", uAsicIdx, usChan, usRawTs,
                             usRawAdc);

          if (fMonitor) fMonitor->FillDuplicateHitsAdc(uFebIdx, uChanInFeb, usRawAdc);
          return;
        }  // if same TS, (ADC,) TS MSB, TS MSB cycle, reject
        fvvusLastTsChan[uAsicIdx][usChan]         = usRawTs;
        fvvusLastAdcChan[uAsicIdx][usChan]        = usRawAdc;
        fvvulLastTsMsbChan[uAsicIdx][usChan]      = fulTsMsbIndexInTs[fuCurrDpbIdx];
      }  // if (fbRejectDuplicateDigis)

      uint32_t uChanInMod = usChan + fNrChsPerAsic * (uAsicIdx % fNrAsicsPerFeb);

      /// FIXME: see issue #1549
      /// N side: 0-1023 =>    0-1023
      /// P side: 0-1023 => 2047-1024

      if (0 == fviFebSide[uFebIdx])
        uChanInMod = fNrChsPerFeb - uChanInMod - 1  // Invert channel order
                     + fNrChsPerFeb;                // Offset for P (back) side

      /*
      // Get the time relative to the Timeslice time, I hope that the cast here works as expected. Otherwise Sts will also get into trouble with the size of UTC here
      auto tsreltime =
        static_cast<uint64_t>((ulHitTime - (fTsStartTime / stsxyter::kdClockCycleNs)) * stsxyter::kdClockCycleNs);
*/
      const double_t tsreltime = static_cast<double_t>(ulHitTime) * stsxyter::kdClockCycleNs;
      double dTimeInNs         = tsreltime - fSystemTimeOffset;
      if (uAsicIdx < fvdTimeOffsetNsAsics.size()) dTimeInNs -= fvdTimeOffsetNsAsics[uAsicIdx];

      const uint64_t ulTimeInNs = static_cast<uint64_t>(dTimeInNs);
      const double dCalAdc      = fvdFebAdcOffs[uFebIdx] + (usRawAdc - 1) * fvdFebAdcGain[uFebIdx];

      if (0 == fviFebAddress[uFebIdx] || -1 == fviFebSide[uFebIdx]) {
        LOG(error) << Form("Digi on disabled FEB %02u has address 0x%08x and side %d", uFebIdx, fviFebAddress[uFebIdx],
                           fviFebSide[uFebIdx]);
      }

      if (fMonitor) fMonitor->CountDigi(uFebIdx, uChanInFeb);

      /// Catch the pulser digis and either save them to their own output or drop them
      if (fvbFebPulser[uFebIdx]) {
        if (fOptOutAVec) {
          fOptOutAVec->emplace_back(CbmStsDigi(fviFebAddress[uFebIdx], uChanInMod, ulTimeInNs, dCalAdc));
        }  // if (fOptOutAVec)
      }    // if (fvbFebPulser[uFebIdx])
      else {
        fOutputVec.emplace_back(CbmStsDigi(fviFebAddress[uFebIdx], uChanInMod, ulTimeInNs, dCalAdc));
      }  // else of if (fvbFebPulser[uFebIdx])

      /// If EM flag ON, store a corresponding error message with the next flag after all other possible status flags set
      if (mess.IsHitMissedEvts())
        if (fOptOutBVec)
          fOptOutBVec->emplace_back(
            CbmErrorMessage(ECbmModuleId::kSts, dTimeInNs, uAsicIdx, 1 << stsxyter::kusLenStatStatus, usChan));
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
    const double dCalAdc            = fvdFebAdcOffs[uFebIdx] + (usRawAdc - 1) * fvdFebAdcGain[uFebIdx];
    fMonitor->FillHitMonitoringHistos(uFebIdx, usChan, uChanInFeb, usRawAdc, dCalAdc, usRawTs, mess.IsHitMissedEvts());
    fMonitor->FillHitEvoMonitoringHistos(uFebIdx, uAsicIdx, uAsicInFeb, uChanInFeb, dTimeSinceStartSec,
                                         mess.IsHitMissedEvts());
  }
}

// ---- processStatusInfo ----
void CbmStsUnpackAlgo::processStatusInfo(const stsxyter::Message& mess, uint32_t uIdx)
{
  // again fMonitor settings used for debugging printouts, I would propose to separat this
  if (fMonitor)
    if (fMonitor->GetDebugMode()) {
      std::cout << Form("DPB %2u TS %12lu mess %5u ", fuCurrDpbIdx, fTsIndex, uIdx);
      mess.PrintMess(std::cout, stsxyter::MessagePrintMask::msg_print_Human);
    }

  const uint16_t usElinkIdx = mess.GetStatusLink();
  const uint32_t uCrobIdx   = usElinkIdx / fNrElinksPerCrob;
  const int32_t uFebIdx     = fElinkIdxToFebIdxVec.at(usElinkIdx);
  if (-1 == uFebIdx) {
    LOG(warning) << "CbmStsUnpackAlgo::processStatusInfo => "
                 << "Wrong elink Idx! Elink raw " << Form("%d remap %d", usElinkIdx, uFebIdx);
    return;
  }
  const uint32_t uAsicIdx = getAsicIndex(fuCurrDpbIdx, uCrobIdx, usElinkIdx);
  if (fMonitor) {
    const uint16_t usStatusField = mess.GetStatusStatus();
    fMonitor->FillStsStatusMessType(uAsicIdx, usStatusField);
  }
  /// Compute the Full time stamp
  const int64_t ulTime = getFullTimeStamp(0);

  /// Convert the time in bins to Hit time in ns
  const double dTimeNs = ulTime * stsxyter::kdClockCycleNs;
  if (fOptOutBVec)
    fOptOutBVec->emplace_back(
      CbmErrorMessage(ECbmModuleId::kSts, dTimeNs, uAsicIdx, mess.GetStatusStatus(), mess.GetData()));
}


// ---- processTsMsbInfo ----
void CbmStsUnpackAlgo::processTsMsbInfo(const stsxyter::Message& mess, uint32_t uMessIdx, uint32_t uMsIdx)
{
  const uint32_t uVal = mess.GetTsMsbValBinning();

  // Update Status counters
  if (uVal < fvulCurrentTsMsb[fuCurrDpbIdx]) {

    LOG(debug) << " TS " << std::setw(12) << fTsIndex << uMsIdx << " MS Idx " << std::setw(4) << uMsIdx << " Msg Idx "
               << std::setw(5) << uMessIdx << " DPB " << std::setw(2) << fuCurrDpbIdx << " Old TsMsb " << std::setw(5)
               << fvulCurrentTsMsb[fuCurrDpbIdx] << " Old MsbCy " << std::setw(5) << fvuCurrentTsMsbCycle[fuCurrDpbIdx]
               << " new TsMsb " << std::setw(5) << uVal;

    fvuCurrentTsMsbCycle[fuCurrDpbIdx]++;
  }
  if (uVal != fvulCurrentTsMsb[fuCurrDpbIdx] + 1
      /// Case where we reach a normal cycle edge
      && !(0 == uVal && 4194303 == fvulCurrentTsMsb[fuCurrDpbIdx])
      /// First TS_MSB in MS may jump if TS dropped by DAQ
      && 1 != uMessIdx
      /// case with cycle et edge of 2 MS
      && !(0 == uVal && 0 == fvulCurrentTsMsb[fuCurrDpbIdx] && 2 == uMessIdx)
      /// Msg 1 and 2 will be same TS_MSB if data in 1st bin
      && !(uVal == fvulCurrentTsMsb[fuCurrDpbIdx] && 2 == uMessIdx)
      /// New FW introduced TS_MSB suppression + large TS_MSB => warning only if value not increasing
      && uVal < fvulCurrentTsMsb[fuCurrDpbIdx]) {
    LOG(debug) << "TS MSb Jump in "
               << " TS " << std::setw(12) << fTsIndex << " MS Idx " << std::setw(4) << uMsIdx << " Msg Idx "
               << std::setw(5) << uMessIdx << " DPB " << std::setw(2) << fuCurrDpbIdx << " => Old TsMsb "
               << std::setw(5) << fvulCurrentTsMsb[fuCurrDpbIdx] << " new TsMsb " << std::setw(5) << uVal;
  }

  /// Catch case where previous MS ended up on a TS MSB cycle as it is then
  /// already updated from the MS index
  if (4194303 == uVal && 1 == uMessIdx) fvulCurrentTsMsb[fuCurrDpbIdx] = 0;
  else
    fvulCurrentTsMsb[fuCurrDpbIdx] = uVal;

  fulTsMsbIndexInTs[fuCurrDpbIdx] =
    fvulCurrentTsMsb[fuCurrDpbIdx]
    + (fvuCurrentTsMsbCycle[fuCurrDpbIdx] * static_cast<uint64_t>(1 << stsxyter::kusLenTsMsbValBinning));
  if (fulTsMsbIndexInTs[fuCurrDpbIdx] < fulTsStartInTsMsb) {
    LOG(fatal) << "CbmStsUnpackAlgo::processTsMsbInfo => "
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
      fMonitor->FillStsDpbRawTsMsb(fuCurrDpbIdx, fvulCurrentTsMsb[fuCurrDpbIdx]);
      fMonitor->FillStsDpbRawTsMsbSx(fuCurrDpbIdx, fvulCurrentTsMsb[fuCurrDpbIdx]);
      fMonitor->FillStsDpbRawTsMsbDpb(fuCurrDpbIdx, fvulCurrentTsMsb[fuCurrDpbIdx]);
    }
}

// ---- refreshTsMsbFields ----
void CbmStsUnpackAlgo::refreshTsMsbFields(const uint32_t imslice, const size_t mstime)
{
  const uint32_t uTsMsbCycleHeader =
    std::floor(mstime / (stsxyter::kulTsCycleNbBinsBinning * stsxyter::kdClockCycleNs));
  const uint32_t uTsMsbHeader =
    std::floor((mstime - uTsMsbCycleHeader * (stsxyter::kulTsCycleNbBinsBinning * stsxyter::kdClockCycleNs))
               / (stsxyter::kuHitNbTsBinsBinning * stsxyter::kdClockCycleNs));

  if (0 == imslice) {
    if (uTsMsbCycleHeader != fvuCurrentTsMsbCycle[fuCurrDpbIdx])
      LOG(debug) << " TS " << std::setw(12) << fTsIndex << " MS " << std::setw(12) << mstime << " MS Idx "
                 << std::setw(4) << imslice << " Msg Idx " << std::setw(5) << 0 << " DPB " << std::setw(2)
                 << fuCurrDpbIdx << " Old TsMsb " << std::setw(5) << fvulCurrentTsMsb[fuCurrDpbIdx] << " Old MsbCy "
                 << std::setw(5) << fvuCurrentTsMsbCycle[fuCurrDpbIdx] << " New MsbCy " << uTsMsbCycleHeader;
    fvuCurrentTsMsbCycle[fuCurrDpbIdx] = uTsMsbCycleHeader;
    fvulCurrentTsMsb[fuCurrDpbIdx]     = uTsMsbHeader;
  }
  else if (uTsMsbCycleHeader != fvuCurrentTsMsbCycle[fuCurrDpbIdx]) {
    if (4194303 == fvulCurrentTsMsb[fuCurrDpbIdx]) {
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
    }
    fvuCurrentTsMsbCycle[fuCurrDpbIdx] = uTsMsbCycleHeader;
  }
  fulTsMsbIndexInTs[fuCurrDpbIdx] =
    fvulCurrentTsMsb[fuCurrDpbIdx]
    + (fvuCurrentTsMsbCycle[fuCurrDpbIdx] * static_cast<uint64_t>(1 << stsxyter::kusLenTsMsbValBinning));
  if (fulTsMsbIndexInTs[fuCurrDpbIdx] < fulTsStartInTsMsb) {
    LOG(fatal) << "CbmStsUnpackAlgo::refreshTsMsbFields => "
               << "Value computed from TS_MSB and TS_MSB cycle smaller than Timeslice start in TS_MSB, "
               << "would lead to a negative value so it cannot be recovered!!!!"
               << std::endl
               /// Values Printout
               << "TS_MSB: " << fvulCurrentTsMsb[fuCurrDpbIdx] << " Cycle: " << fvuCurrentTsMsbCycle[fuCurrDpbIdx]
               << " Full TS_MSB: " << fulTsMsbIndexInTs[fuCurrDpbIdx] << " TS Start offset: " << fulTsStartInTsMsb;
  }
  fulTsMsbIndexInTs[fuCurrDpbIdx] -= fulTsStartInTsMsb;
}

// ---- unpack ----
bool CbmStsUnpackAlgo::unpack(const fles::Timeslice* ts, std::uint16_t icomp, UInt_t imslice)
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
             << " has size: " << uSize;

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

ClassImp(CbmStsUnpackAlgo)
