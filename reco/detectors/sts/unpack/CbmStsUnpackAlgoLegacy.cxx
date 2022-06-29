/* Copyright (C) 2019-2021 Fair GmbH, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau, Dominik Smith [committer] */


#include "CbmStsUnpackAlgoLegacy.h"

#include "CbmStsUnpackMonitor.h"
//#include "CbmFormatMsHeaderPrintout.h"
#include "CbmMcbm2018StsPar.h"

#include <Logger.h>

#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TList.h"
#include "TProfile.h"
#include "TROOT.h"
#include "TString.h"

#include <fstream>
#include <iomanip>
#include <iostream>

#include <stdint.h>

// -------------------------------------------------------------------------

CbmStsUnpackAlgoLegacy::CbmStsUnpackAlgoLegacy()
  : CbmStsUnpackAlgoBase("CbmStsUnpackAlgoLegacy")
  ,
  /// From the class itself
  fvbMaskedComponents()
  , fUnpackPar(nullptr)
  , fuNbFebs(0)
  , fDpbIdIndexMap()
  , fviFebType()
  , fviFebAddress()
  , fviFebSide()
  , fvdFebAdcGain()
  , fvdFebAdcOffs()
  , fdTimeOffsetNs(0.0)
  , fbUseChannelMask(false)
  , fvvbMaskedChannels()
  , fulCurrentMsIdx(0)
  , fuCurrDpbIdx(0)
  , fvulCurrentTsMsb()
  , fdStartTime(0.0)
  , fdStartTimeMsSz(-1.0)
  , ftStartTimeUnix(std::chrono::steady_clock::now())
  , fvmHitsInMs()
  , fvvusLastTsChan()
  , fvvusLastAdcChan()
  , fvvuLastTsMsbChan()
  , fvvusLastTsMsbCycleChan()
{
}

CbmStsUnpackAlgoLegacy::~CbmStsUnpackAlgoLegacy()
{
  /// Clear buffers
  fvmHitsInMs.clear();
  if (nullptr != fUnpackPar) delete fUnpackPar;
}

// -------------------------------------------------------------------------
bool CbmStsUnpackAlgoLegacy::init()
{
  LOG(info) << "Initializing mCBM STS 2019 unpacker algo";
  return true;
}

// -------------------------------------------------------------------------
void CbmStsUnpackAlgoLegacy::reset() {}


// ---- initParSet(FairParGenericSet* parset) ----
Bool_t CbmStsUnpackAlgoLegacy::initParSet(FairParGenericSet* parset)
{
  LOG(info) << fName << "::initParSet - for container " << parset->ClassName();
  if (parset->IsA() == CbmMcbm2018StsPar::Class()) return initParSet(static_cast<CbmMcbm2018StsPar*>(parset));

  // If we do not know the derived ParSet class we return false
  LOG(error)
    << fName << "::initParSet - for container " << parset->ClassName()
    << " failed, since CbmStsUnpackAlgoLegacy::initParSet() does not know the derived ParSet and what to do with it!";
  return kFALSE;
}

// ---- initParSet(CbmMcbm2018StsPar* parset) ----
Bool_t CbmStsUnpackAlgoLegacy::initParSet(CbmMcbm2018StsPar* parset)
{
  fUnpackPar  = parset;
  bool initOK = InitParameters();

  if (fMonitor) initOK &= fMonitor->Init(parset);

  return initOK;
}

// -------------------------------------------------------------------------
bool CbmStsUnpackAlgoLegacy::InitParameters()
{
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
  const uint32_t uNbModules = fUnpackPar->GetNbOfModules();
  LOG(info) << "Nr. of STS Modules:    " << uNbModules;

  //Initialize temporary "per Feb" fields
  InitTempVectors(&viModuleType, &viModAddress, &viFebModuleIdx, &vbCrobActiveFlag, &viFebModuleSide);

  // Total number of STS DPBs in system
  const uint32_t uNbOfDpbs = fUnpackPar->GetNrOfDpbs();
  LOG(info) << "Nr. of STS DPBs:       " << uNbOfDpbs;

  // Read dpb index map from parameter container
  InitDpbIdIndexMap();

  // Get Nr of Febs
  fuNbFebs = fUnpackPar->GetNrOfFebs();
  LOG(info) << "Nr. of FEBs:           " << fuNbFebs;

  // Get Nr of Asics
  const uint32_t uNbStsXyters = fUnpackPar->GetNrOfAsics();
  LOG(info) << "Nr. of StsXyter ASICs: " << uNbStsXyters;

  if (fvdTimeOffsetNsAsics.size() < uNbStsXyters) { fvdTimeOffsetNsAsics.resize(uNbStsXyters, 0.0); }

  //Initialize class-wide "per Feb" fields
  fviFebType.resize(uNbOfDpbs);

  for (uint32_t uDpb = 0; uDpb < uNbOfDpbs; ++uDpb) {
    fviFebType[uDpb].resize(fUnpackPar->GetNbCrobsPerDpb());
    for (uint32_t uCrobIdx = 0; uCrobIdx < fUnpackPar->GetNbCrobsPerDpb(); ++uCrobIdx) {
      fviFebType[uDpb][uCrobIdx].resize(fUnpackPar->GetNbFebsPerCrob(), -1);
      for (uint32_t uFebIdx = 0; uFebIdx < fUnpackPar->GetNbFebsPerCrob(); ++uFebIdx) {
        fvbFebPulser.push_back(fUnpackPar->IsFebPulser(uDpb, uCrobIdx, uFebIdx));
        fvdFebAdcGain.push_back(fUnpackPar->GetFebAdcGain(uDpb, uCrobIdx, uFebIdx));
        fvdFebAdcOffs.push_back(fUnpackPar->GetFebAdcOffset(uDpb, uCrobIdx, uFebIdx));

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
              //                        + (viFebModuleSide[uDpb][uCrobIdx][uFebIdx] << 25));
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

  PrintActiveCrobs(vbCrobActiveFlag);
  PrintAddressMaps(viFebModuleIdx, viFebModuleSide);

  LOG(info) << "Unpacking data in bin sorter FW mode";
  InitInternalStatus();

  return true;
}

// -------------------------------------------------------------------------
void CbmStsUnpackAlgoLegacy::InitDpbIdIndexMap()
{
  fDpbIdIndexMap.clear();
  for (uint32_t uDpb = 0; uDpb < fUnpackPar->GetNrOfDpbs(); ++uDpb) {
    fDpbIdIndexMap[fUnpackPar->GetDpbId(uDpb)] = uDpb;
    LOG(info) << "Eq. ID for DPB #" << std::setw(2) << uDpb << " = 0x" << std::setw(4) << std::hex
              << fUnpackPar->GetDpbId(uDpb) << std::dec << " => " << fDpbIdIndexMap[fUnpackPar->GetDpbId(uDpb)];
  }
}

// -------------------------------------------------------------------------
void CbmStsUnpackAlgoLegacy::InitTempVectors(std::vector<int32_t>* viModuleType, std::vector<int32_t>* viModAddress,
                                             std::vector<std::vector<std::vector<int32_t>>>* viFebModuleIdx,
                                             std::vector<std::vector<bool>>* vbCrobActiveFlag,
                                             std::vector<std::vector<std::vector<int32_t>>>* viFebModuleSide)
{
  const uint32_t uNbModules = fUnpackPar->GetNbOfModules();
  const uint32_t uNbOfDpbs  = fUnpackPar->GetNrOfDpbs();

  viModuleType->resize(uNbModules);
  viModAddress->resize(uNbModules);
  for (uint32_t uModIdx = 0; uModIdx < uNbModules; ++uModIdx) {
    (*viModuleType)[uModIdx] = fUnpackPar->GetModuleType(uModIdx);
    (*viModAddress)[uModIdx] = fUnpackPar->GetModuleAddress(uModIdx);
    LOG(info) << "Module #" << std::setw(2) << uModIdx << " Type " << std::setw(4) << (*viModuleType)[uModIdx]
              << " Address 0x" << std::setw(8) << std::hex << (*viModAddress)[uModIdx] << std::dec;
  }
  vbCrobActiveFlag->resize(uNbOfDpbs);
  viFebModuleIdx->resize(uNbOfDpbs);
  viFebModuleSide->resize(uNbOfDpbs);

  for (uint32_t uDpb = 0; uDpb < uNbOfDpbs; ++uDpb) {
    (*vbCrobActiveFlag)[uDpb].resize(fUnpackPar->GetNbCrobsPerDpb());
    (*viFebModuleIdx)[uDpb].resize(fUnpackPar->GetNbCrobsPerDpb());
    (*viFebModuleSide)[uDpb].resize(fUnpackPar->GetNbCrobsPerDpb());
    for (uint32_t uCrobIdx = 0; uCrobIdx < fUnpackPar->GetNbCrobsPerDpb(); ++uCrobIdx) {
      (*vbCrobActiveFlag)[uDpb][uCrobIdx] = fUnpackPar->IsCrobActive(uDpb, uCrobIdx);
      (*viFebModuleIdx)[uDpb][uCrobIdx].resize(fUnpackPar->GetNbFebsPerCrob());
      (*viFebModuleSide)[uDpb][uCrobIdx].resize(fUnpackPar->GetNbFebsPerCrob());
      for (uint32_t uFebIdx = 0; uFebIdx < fUnpackPar->GetNbFebsPerCrob(); ++uFebIdx) {
        (*viFebModuleIdx)[uDpb][uCrobIdx][uFebIdx]  = fUnpackPar->GetFebModuleIdx(uDpb, uCrobIdx, uFebIdx);
        (*viFebModuleSide)[uDpb][uCrobIdx][uFebIdx] = fUnpackPar->GetFebModuleSide(uDpb, uCrobIdx, uFebIdx);
      }
    }
  }
}

// -------------------------------------------------------------------------
void CbmStsUnpackAlgoLegacy::PrintActiveCrobs(const std::vector<std::vector<bool>>& vbCrobActiveFlag)
{
  for (uint32_t uDpb = 0; uDpb < fUnpackPar->GetNrOfDpbs(); ++uDpb) {
    TString sPrintoutLine = Form("DPB #%02u CROB Active ?:       ", uDpb);
    for (uint32_t uCrobIdx = 0; uCrobIdx < fUnpackPar->GetNbCrobsPerDpb(); ++uCrobIdx) {
      sPrintoutLine += Form("%1u", (vbCrobActiveFlag[uDpb][uCrobIdx] == true));
    }
    LOG(info) << sPrintoutLine;
  }
}

// -------------------------------------------------------------------------
void CbmStsUnpackAlgoLegacy::PrintAddressMaps(const std::vector<std::vector<std::vector<int32_t>>>& viFebModuleIdx,
                                              const std::vector<std::vector<std::vector<int32_t>>>& viFebModuleSide)
{
  uint32_t uGlobalFebIdx = 0;
  for (uint32_t uDpb = 0; uDpb < fUnpackPar->GetNrOfDpbs(); ++uDpb) {
    for (uint32_t uCrobIdx = 0; uCrobIdx < fUnpackPar->GetNbCrobsPerDpb(); ++uCrobIdx) {
      LOG(info) << Form("DPB #%02u CROB #%u:       ", uDpb, uCrobIdx);
      for (uint32_t uFebIdx = 0; uFebIdx < fUnpackPar->GetNbFebsPerCrob(); ++uFebIdx) {
        if (0 <= viFebModuleIdx[uDpb][uCrobIdx][uFebIdx])
          LOG(info) << Form("      FEB #%02u (%02u): Mod. Idx = %03d Side %c (%2d) Type %c "
                            "(%2d) (Addr. 0x%08x) ADC gain %4.0f e- ADC Offs %5.0f e-",
                            uFebIdx, uGlobalFebIdx, viFebModuleIdx[uDpb][uCrobIdx][uFebIdx],
                            1 == viFebModuleSide[uDpb][uCrobIdx][uFebIdx] ? 'N' : 'P',
                            viFebModuleSide[uDpb][uCrobIdx][uFebIdx],
                            1 == fviFebType[uDpb][uCrobIdx][uFebIdx] ? 'B' : 'A', fviFebType[uDpb][uCrobIdx][uFebIdx],
                            fviFebAddress[uGlobalFebIdx], fvdFebAdcGain[uGlobalFebIdx], fvdFebAdcOffs[uGlobalFebIdx]);
        else
          LOG(info) << Form("Disabled FEB #%02u (%02u): Mod. Idx = %03d Side %c (%2d) Type %c "
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
void CbmStsUnpackAlgoLegacy::InitInternalStatus()
{
  const uint32_t uNbOfDpbs    = fUnpackPar->GetNrOfDpbs();
  const uint32_t uNbStsXyters = fUnpackPar->GetNrOfAsics();

  fvulCurrentTsMsb.resize(uNbOfDpbs);
  fvuCurrentTsMsbCycle.resize(uNbOfDpbs);
  for (uint32_t uDpb = 0; uDpb < uNbOfDpbs; ++uDpb) {
    fvulCurrentTsMsb[uDpb]     = 0;
    fvuCurrentTsMsbCycle[uDpb] = 0;
  }

  fvvusLastTsChan.resize(uNbStsXyters);
  fvvusLastAdcChan.resize(uNbStsXyters);
  fvvuLastTsMsbChan.resize(uNbStsXyters);
  fvvusLastTsMsbCycleChan.resize(uNbStsXyters);
  for (uint32_t uAsicIdx = 0; uAsicIdx < uNbStsXyters; ++uAsicIdx) {
    fvvusLastTsChan[uAsicIdx].resize(fUnpackPar->GetNbChanPerAsic(), 0);
    fvvusLastAdcChan[uAsicIdx].resize(fUnpackPar->GetNbChanPerAsic(), 0);
    fvvuLastTsMsbChan[uAsicIdx].resize(fUnpackPar->GetNbChanPerAsic(), 0);
    fvvusLastTsMsbCycleChan[uAsicIdx].resize(fUnpackPar->GetNbChanPerAsic(), 0);
  }
}

void CbmStsUnpackAlgoLegacy::AddHitsToDigiVect(std::vector<stsxyter::FinalHit>* vmHitsIn,
                                               std::vector<CbmStsDigi>* vDigiVectOut)
{
  for (auto itHitIn = vmHitsIn->begin(); itHitIn < vmHitsIn->end(); ++itHitIn) {
    const uint32_t uAsicIdx = itHitIn->GetAsic();
    const uint32_t uFebIdx  = itHitIn->GetAsic() / fUnpackPar->GetNbAsicsPerFeb();
    uint32_t uChanInMod =
      itHitIn->GetChan() + fUnpackPar->GetNbChanPerAsic() * (itHitIn->GetAsic() % fUnpackPar->GetNbAsicsPerFeb());
    /// FIXME: see issue #1549
    /// N side: 0-1023 =>    0-1023
    /// P side: 0-1023 => 2047-1024
    if (0 == fviFebSide[uFebIdx])
      uChanInMod = fUnpackPar->GetNbChanPerFeb() - uChanInMod - 1  // Invert channel order
                   + fUnpackPar->GetNbChanPerFeb();                // Offset for P (back) side

    double dTimeInNs = itHitIn->GetTs() * stsxyter::kdClockCycleNs - fdTimeOffsetNs;
    if (uAsicIdx < fvdTimeOffsetNsAsics.size()) dTimeInNs -= fvdTimeOffsetNsAsics[uAsicIdx];

    const uint64_t ulTimeInNs = static_cast<uint64_t>(dTimeInNs);
    const double dCalAdc      = fvdFebAdcOffs[uFebIdx] + (itHitIn->GetAdc() - 1) * fvdFebAdcGain[uFebIdx];

    if (0 == fviFebAddress[uFebIdx] || -1 == fviFebSide[uFebIdx]) {
      LOG(error) << Form("Digi on disabled FEB %02u has address 0x%08x and side %d", uFebIdx, fviFebAddress[uFebIdx],
                         fviFebSide[uFebIdx]);
    }

    /// Catch the pulser digis and either save them to their own output or drop them
    if (fvbFebPulser[uFebIdx]) {
      if (fOptOutAVec) {
        fOptOutAVec->emplace_back(fviFebAddress[uFebIdx], uChanInMod, ulTimeInNs, dCalAdc);
      }  // if (fOptOutAVec)
    }    // if (fvbFebPulser[uFebIdx])
    else {
      vDigiVectOut->emplace_back(fviFebAddress[uFebIdx], uChanInMod, ulTimeInNs, dCalAdc);
    }  // else of if (fvbFebPulser[uFebIdx])
  }
}

bool CbmStsUnpackAlgoLegacy::unpack(const fles::Timeslice* ts, std::uint16_t uMsCompIdx, UInt_t uMsIdx)
{
  /// Ignore First TS as first MS is typically corrupt
  if (0 == fTsIndex) { return true; }

  auto msDescriptor = ts->descriptor(uMsCompIdx, uMsIdx);

  //Current equipment ID, tells from which DPB the current MS is originating
  const uint32_t uCurrentEquipmentId = msDescriptor.eq_id;
  const uint8_t* msContent           = reinterpret_cast<const uint8_t*>(ts->content(uMsCompIdx, uMsIdx));
  const uint32_t uSize               = msDescriptor.size;

  fulCurrentMsIdx = msDescriptor.idx;
  LOG(debug) << "Microslice: " << fulCurrentMsIdx << " from EqId " << std::hex << uCurrentEquipmentId << std::dec
             << " has size: " << uSize;

  if (0 == fvbMaskedComponents.size()) fvbMaskedComponents.resize(ts->num_components(), false);

  //Temp holder until current equipment ID is properly filled in MS
  const uint32_t uCurrDpbId = static_cast<uint32_t>(uCurrentEquipmentId & 0xFFFF);

  if (fMonitor && fMonitor->GetDebugMode()) {
    const double dMsTime = (1e-9) * static_cast<double>(fulCurrentMsIdx);
    if (uMsCompIdx < fMonitor->GetMaxNbFlibLinks()) {
      if (fdStartTimeMsSz < 0) fdStartTimeMsSz = dMsTime;
      fMonitor->FillMsSize(uMsCompIdx, uSize);
      fMonitor->FillMsSizeTime(uMsCompIdx, dMsTime - fdStartTimeMsSz, uSize);
    }
  }

  /// Check if this sDPB ID was declared in parameter file and stop there if not
  auto it = fDpbIdIndexMap.find(uCurrDpbId);
  if (it == fDpbIdIndexMap.end()) {
    if (false == fvbMaskedComponents[uMsCompIdx]) {
      LOG(info) << "---------------------------------------------------------------";
      //LOG(info) << FormatMsHeaderPrintout(msDescriptor);
      LOG(warning) << "Could not find the sDPB index for AFCK id 0x" << std::hex << uCurrDpbId << std::dec
                   << " in timeslice " << fTsIndex << " in microslice " << uMsIdx << " component " << uMsCompIdx << "\n"
                   << "If valid this index has to be added in the STS "
                      "parameter file in the DbpIdArray field";
      fvbMaskedComponents[uMsCompIdx] = true;

      /// If first TS being analyzed, we are probably detecting STS/MUCH boards with same sysid
      /// => Do not report the MS as bad, just ignore it
      if (1 == fTsIndex) return true;
    }
    else
      return true;

    return false;
  }
  else
    fuCurrDpbIdx = fDpbIdIndexMap[uCurrDpbId];

  if (fMonitor) fMonitor->FillMsCntEvo(fulCurrentMsIdx);

  // Check the current TS_MSb cycle and correct it if wrong
  RefreshTsMsbFields(uMsIdx);

  //Reset internal monitor variables for debugging info
  if (fMonitor && fMonitor->GetDebugMode()) { fMonitor->ResetDebugInfo(); }

  //Main processing of MS content
  LoopMsMessages(msContent, uSize, uMsIdx);

  //Output debugging info
  if (fMonitor) {
    if (fMonitor->GetDebugMode()) { fMonitor->PrintDebugInfo(fulCurrentMsIdx, fTsIndex, msDescriptor.flags, uSize); }
    for (auto itHit = fOutputVec.begin(); itHit != fOutputVec.end(); ++itHit) {
      fMonitor->FillDigisTimeInRun(itHit->GetTime());  //check whether this does what it should
    }
    fMonitor->FillVectorSize(ts->index(), fOutputVec.size());  //check whether this does what it should
    //fMonitor->DrawCanvases();
  }

  AddHitsToDigiVect(&fvmHitsInMs, &fOutputVec);

  /// Clear the buffer of hits
  fvmHitsInMs.clear();

  return true;
}

// -------------------------------------------------------------------------
void CbmStsUnpackAlgoLegacy::RefreshTsMsbFields(const size_t uMsIdx)
{
  uint32_t uTsMsbCycleHeader;
  if (fbUseFwBinning) {
    uTsMsbCycleHeader = std::floor(fulCurrentMsIdx / (stsxyter::kulTsCycleNbBinsBinning * stsxyter::kdClockCycleNs));
  }
  else {
    uTsMsbCycleHeader = std::floor(fulCurrentMsIdx / (stsxyter::kulTsCycleNbBins * stsxyter::kdClockCycleNs));
  }

  if (0 == uMsIdx) {
    if (uTsMsbCycleHeader != fvuCurrentTsMsbCycle[fuCurrDpbIdx])
      LOG(info) << " TS " << std::setw(12) << fTsIndex << " MS " << std::setw(12) << fulCurrentMsIdx << " MS Idx "
                << std::setw(4) << uMsIdx << " Msg Idx " << std::setw(5) << 0 << " DPB " << std::setw(2) << fuCurrDpbIdx
                << " Old TsMsb " << std::setw(5) << fvulCurrentTsMsb[fuCurrDpbIdx] << " Old MsbCy " << std::setw(5)
                << fvuCurrentTsMsbCycle[fuCurrDpbIdx] << " New MsbCy " << uTsMsbCycleHeader;
    fvuCurrentTsMsbCycle[fuCurrDpbIdx] = uTsMsbCycleHeader;
    fvulCurrentTsMsb[fuCurrDpbIdx]     = 0;
  }
  else if (uTsMsbCycleHeader != fvuCurrentTsMsbCycle[fuCurrDpbIdx]) {
    if (4194303 == fvulCurrentTsMsb[fuCurrDpbIdx]) {
      LOG(info) << " TS " << std::setw(12) << fTsIndex << " MS " << std::setw(12) << fulCurrentMsIdx << " MS Idx "
                << std::setw(4) << uMsIdx << " Msg Idx " << std::setw(5) << 0 << " DPB " << std::setw(2) << fuCurrDpbIdx
                << " Old TsMsb " << std::setw(5) << fvulCurrentTsMsb[fuCurrDpbIdx] << " Old MsbCy " << std::setw(5)
                << fvuCurrentTsMsbCycle[fuCurrDpbIdx] << " New MsbCy " << uTsMsbCycleHeader;
    }
    else {
      LOG(warning) << "TS MSB cycle from MS header does not match current cycle from data "
                   << "for TS " << std::setw(12) << fTsIndex << " MS " << std::setw(12) << fulCurrentMsIdx << " MsInTs "
                   << std::setw(3) << uMsIdx << " ====> " << fvuCurrentTsMsbCycle[fuCurrDpbIdx] << " (cnt) VS "
                   << uTsMsbCycleHeader << " (header)";
    }
    fvuCurrentTsMsbCycle[fuCurrDpbIdx] = uTsMsbCycleHeader;
  }
}

// -------------------------------------------------------------------------
void CbmStsUnpackAlgoLegacy::LoopMsMessages(const uint8_t* msContent, const uint32_t uSize, const size_t uMsIdx)
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

    if (fMonitor && fMonitor->GetDebugMode()) { fMonitor->ProcessDebugInfo(pMess[uIdx], fuCurrDpbIdx); }
    switch (typeMess) {
      case stsxyter::MessType::Hit: {
        ProcessHitInfo(pMess[uIdx]);
        break;
      }
      case stsxyter::MessType::TsMsb: {
        ProcessTsMsbInfo(pMess[uIdx], uIdx, uMsIdx);
        break;
      }
      case stsxyter::MessType::Epoch: {
        ProcessEpochInfo(pMess[uIdx]);
        if (0 < uIdx) {
          LOG(info) << "CbmStsUnpackAlgoLegacy::DoUnpack => "
                    << "EPOCH message at unexpected position in MS: message " << uIdx << " VS message 0 expected!";
        }
        break;
      }
      case stsxyter::MessType::Status: {
        ProcessStatusInfo(pMess[uIdx], uIdx);
        break;
      }
      case stsxyter::MessType::Empty: {
        break;
      }
      case stsxyter::MessType::EndOfMs: {
        ProcessErrorInfo(pMess[uIdx]);
        break;
      }
      case stsxyter::MessType::Dummy: {
        break;
      }
      default: {
        LOG(fatal) << "CbmStsUnpackAlgoLegacy::DoUnpack => "
                   << "Unknown message type, should never happen, stopping "
                      "here! Type found was: "
                   << static_cast<int>(typeMess);
      }
    }
  }
}

// -------------------------------------------------------------------------
void CbmStsUnpackAlgoLegacy::ProcessHitInfo(const stsxyter::Message& mess)
{
  uint16_t usElinkIdx;
  if (fbUseFwBinning) { usElinkIdx = mess.GetLinkIndexHitBinning(); }
  else {
    usElinkIdx = mess.GetLinkIndex();
  }
  const uint32_t uCrobIdx = usElinkIdx / fUnpackPar->GetNbElinkPerCrob();
  const int32_t uFebIdx   = fUnpackPar->ElinkIdxToFebIdx(usElinkIdx);
  if (-1 == uFebIdx) {
    LOG(warning) << "CbmStsUnpackAlgoLegacy::ProcessHitInfo => "
                 << "Wrong elink Idx! Elink raw " << Form("%d remap %d", usElinkIdx, uFebIdx);
    return;
  }
  const uint32_t uAsicIdx =
    (fuCurrDpbIdx * fUnpackPar->GetNbCrobsPerDpb() + uCrobIdx) * fUnpackPar->GetNbAsicsPerCrob()
    + fUnpackPar->ElinkIdxToAsicIdx(1 == fviFebType[fuCurrDpbIdx][uCrobIdx][uFebIdx], usElinkIdx);
  //LOG(info) << "Test " << uFebIdx << " " << uAsicIdx / fUnpackPar->GetNbAsicsPerFeb();
  //const uint32_t uFebIdx    = uAsicIdx / fUnpackPar->GetNbAsicsPerFeb();  // delete this?

  const uint16_t usChan   = mess.GetHitChannel();
  const uint16_t usRawAdc = mess.GetHitAdc();
  uint16_t usRawTs;
  if (fbUseFwBinning) { usRawTs = mess.GetHitTimeBinning(); }
  else {
    usRawTs = mess.GetHitTimeFull();
  }
  const uint32_t uChanInFeb = usChan + fUnpackPar->GetNbChanPerAsic() * (uAsicIdx % fUnpackPar->GetNbAsicsPerFeb());

  /// Duplicate hits rejection
  if (fbRejectDuplicateDigis) {
    if (usRawTs == fvvusLastTsChan[uAsicIdx][usChan]
        && (fbDupliWithoutAdc || usRawAdc == fvvusLastAdcChan[uAsicIdx][usChan])
        && fvulCurrentTsMsb[fuCurrDpbIdx] - fvvuLastTsMsbChan[uAsicIdx][usChan] < kuMaxTsMsbDiffDuplicates
        && fvuCurrentTsMsbCycle[fuCurrDpbIdx] == fvvusLastTsMsbCycleChan[uAsicIdx][usChan]) {
      /// FIXME: add plots to check what is done in this rejection
      LOG(debug) << "CbmStsUnpackAlgoLegacy::ProcessHitInfo => "
                 << Form("Rejecting duplicate on Asic %3u channel %3u, TS %3u, MSB %lu, Cycle %u, ADC %2u", uAsicIdx,
                         usChan, usRawTs, fvulCurrentTsMsb[fuCurrDpbIdx], fvuCurrentTsMsbCycle[fuCurrDpbIdx], usRawAdc);
      return;
    }  // if SMX 2.0 DPB and same TS, ADC, TS MSB, TS MSB cycle!
  }    // if (fbRejectDuplicateDigis)
  fvvusLastTsChan[uAsicIdx][usChan]         = usRawTs;
  fvvusLastAdcChan[uAsicIdx][usChan]        = usRawAdc;
  fvvuLastTsMsbChan[uAsicIdx][usChan]       = fvulCurrentTsMsb[fuCurrDpbIdx];
  fvvusLastTsMsbCycleChan[uAsicIdx][usChan] = fvuCurrentTsMsbCycle[fuCurrDpbIdx];

  // Compute the Full time stamp
  const uint64_t ulHitTime = GetFullTimeStamp(usRawTs);

  /// Store hit for output only if it is mapped to a module!!!
  if (0 != fviFebAddress[uFebIdx] && fdAdcCut < usRawAdc) {
    /// Store only if masking is disabled or if channeld is not masked
    /// 2D vector is safe as always right size if masking enabled
    if (false == fbUseChannelMask || false == fvvbMaskedChannels[uFebIdx][uChanInFeb])
      fvmHitsInMs.push_back(stsxyter::FinalHit(ulHitTime, usRawAdc, uAsicIdx, usChan, fuCurrDpbIdx, uCrobIdx));
  }

  // Convert the Hit time in bins to Hit time in ns
  const double dHitTimeNs = ulHitTime * stsxyter::kdClockCycleNs;

  /// If EM flag ON, store a corresponding error message with the next flag after all other possible status flags set
  if (mess.IsHitMissedEvts() && fOptOutBVec != nullptr)
    fOptOutBVec->push_back(
      CbmErrorMessage(ECbmModuleId::kSts, dHitTimeNs, uAsicIdx, 1 << stsxyter::kusLenStatStatus, usChan));

  if (fMonitor) {
    // Check Starting point of histos with time as X axis
    if (-1 == fdStartTime) { fdStartTime = dHitTimeNs; }
    if (fMonitor->GetDebugMode()) {
      fMonitor->FillHitDebugMonitoringHistos(uAsicIdx, usChan, usRawAdc, usRawTs, mess.IsHitMissedEvts());
    }
    const uint32_t uAsicInFeb       = uAsicIdx % fUnpackPar->GetNbAsicsPerFeb();
    const double dTimeSinceStartSec = (dHitTimeNs - fdStartTime) * 1e-9;
    const double dCalAdc            = fvdFebAdcOffs[uFebIdx] + (usRawAdc - 1) * fvdFebAdcGain[uFebIdx];
    fMonitor->FillHitMonitoringHistos(uFebIdx, usChan, uChanInFeb, usRawAdc, dCalAdc, usRawTs, mess.IsHitMissedEvts());
    fMonitor->FillHitEvoMonitoringHistos(uFebIdx, uAsicIdx, uAsicInFeb, uChanInFeb, dTimeSinceStartSec,
                                         mess.IsHitMissedEvts());
  }
}


void CbmStsUnpackAlgoLegacy::ProcessTsMsbInfo(const stsxyter::Message& mess, uint32_t uMessIdx, uint32_t uMsIdx)
{
  uint32_t uVal;
  if (fbUseFwBinning) { uVal = mess.GetTsMsbValBinning(); }
  else {
    uVal = mess.GetTsMsbVal();
  }

  // Update Status counters
  if (uVal < fvulCurrentTsMsb[fuCurrDpbIdx]) {

    LOG(info) << " TS " << std::setw(12) << fTsIndex << " MS " << std::setw(12) << fulCurrentMsIdx << " MS Idx "
              << std::setw(4) << uMsIdx << " Msg Idx " << std::setw(5) << uMessIdx << " DPB " << std::setw(2)
              << fuCurrDpbIdx << " Old TsMsb " << std::setw(5) << fvulCurrentTsMsb[fuCurrDpbIdx] << " Old MsbCy "
              << std::setw(5) << fvuCurrentTsMsbCycle[fuCurrDpbIdx] << " new TsMsb " << std::setw(5) << uVal;

    fvuCurrentTsMsbCycle[fuCurrDpbIdx]++;
  }
  if (
    uVal != fvulCurrentTsMsb[fuCurrDpbIdx] + 1 && !(0 == uVal && 4194303 == fvulCurrentTsMsb[fuCurrDpbIdx])
    &&                /// Case where we reach a normal cycle edge
    1 != uMessIdx &&  /// First TS_MSB in MS may jump if TS dropped by DAQ
    !(0 == uVal && 0 == fvulCurrentTsMsb[fuCurrDpbIdx] && 2 == uMessIdx) &&  /// case with cycle et edge of 2 MS
    !(uVal == fvulCurrentTsMsb[fuCurrDpbIdx] && 2 == uMessIdx)
    &&  /// Msg 1 and 2 will be same TS_MSB if data in 1st bin
    uVal < fvulCurrentTsMsb
        [fuCurrDpbIdx]  /// New FW introduced TS_MSB suppression + large TS_MSB => warning only if value not increasing
  ) {
    LOG(info) << "TS MSb Jump in "
              << " TS " << std::setw(12) << fTsIndex << " MS " << std::setw(12) << fulCurrentMsIdx << " MS Idx "
              << std::setw(4) << uMsIdx << " Msg Idx " << std::setw(5) << uMessIdx << " DPB " << std::setw(2)
              << fuCurrDpbIdx << " => Old TsMsb " << std::setw(5) << fvulCurrentTsMsb[fuCurrDpbIdx] << " new TsMsb "
              << std::setw(5) << uVal;
  }

  /// Catch case where previous MS ended up on a TS MSB cycle as it is then
  /// already updated from the MS index
  if (4194303 == uVal && 1 == uMessIdx) fvulCurrentTsMsb[fuCurrDpbIdx] = 0;
  else
    fvulCurrentTsMsb[fuCurrDpbIdx] = uVal;

  if (fMonitor && fMonitor->GetDebugMode()) {  //also if( 1 < uMessIdx )?
    fMonitor->FillStsDpbRawTsMsb(fuCurrDpbIdx, fvulCurrentTsMsb[fuCurrDpbIdx]);
    fMonitor->FillStsDpbRawTsMsbSx(fuCurrDpbIdx, fvulCurrentTsMsb[fuCurrDpbIdx]);
    fMonitor->FillStsDpbRawTsMsbDpb(fuCurrDpbIdx, fvulCurrentTsMsb[fuCurrDpbIdx]);
  }
}

void CbmStsUnpackAlgoLegacy::ProcessEpochInfo(const stsxyter::Message& /*mess*/)
{
  //   Currently not used
  //   uint32_t uVal    = mess.GetEpochVal();
  //   uint32_t uCurrentCycle = uVal % stsxyter::kulTsCycleNbBins;
}

// -------------------------------------------------------------------------
void CbmStsUnpackAlgoLegacy::ProcessStatusInfo(const stsxyter::Message& mess, uint32_t uIdx)
{
  if (fMonitor && fMonitor->GetDebugMode()) {
    std::cout << Form("DPB %2u TS %12lu MS %12lu mess %5u ", fuCurrDpbIdx, fTsIndex, fulCurrentMsIdx, uIdx);
    mess.PrintMess(std::cout, stsxyter::MessagePrintMask::msg_print_Human);
  }

  const uint16_t usElinkIdx = mess.GetStatusLink();
  const uint32_t uCrobIdx   = usElinkIdx / fUnpackPar->GetNbElinkPerCrob();
  const int32_t uFebIdx     = fUnpackPar->ElinkIdxToFebIdx(usElinkIdx);
  if (-1 == uFebIdx) {
    LOG(warning) << "CbmStsUnpackAlgoLegacy::DoUnpack => "
                 << "Wrong elink Idx! Elink raw " << Form("%d remap %d", usElinkIdx, uFebIdx);
    return;
  }
  const uint32_t uAsicIdx =
    (fuCurrDpbIdx * fUnpackPar->GetNbCrobsPerDpb() + uCrobIdx) * fUnpackPar->GetNbAsicsPerCrob()
    + fUnpackPar->ElinkIdxToAsicIdx(1 == fviFebType[fuCurrDpbIdx][uCrobIdx][uFebIdx], usElinkIdx);

  if (fMonitor) {
    const uint16_t usStatusField = mess.GetStatusStatus();
    fMonitor->FillStsStatusMessType(uAsicIdx, usStatusField);
  }
  /// Compute the Full time stamp
  const int64_t ulTime = GetFullTimeStamp(0);

  /// Convert the time in bins to Hit time in ns
  const double dTimeNs = ulTime * stsxyter::kdClockCycleNs;
  if (fOptOutBVec) {
    fOptOutBVec->push_back(
      CbmErrorMessage(ECbmModuleId::kSts, dTimeNs, uAsicIdx, mess.GetStatusStatus(), mess.GetData()));
  }
}

// -------------------------------------------------------------------------
uint64_t CbmStsUnpackAlgoLegacy::GetFullTimeStamp(const uint16_t usRawTs)
{
  // Use TS w/o overlap bits as they will anyway come from the TS_MSB

  uint64_t ulTime;
  if (fbUseFwBinning) {
    ulTime =
      usRawTs
      + static_cast<uint64_t>(stsxyter::kuHitNbTsBinsBinning) * static_cast<uint64_t>(fvulCurrentTsMsb[fuCurrDpbIdx])
      + static_cast<uint64_t>(stsxyter::kulTsCycleNbBinsBinning)
          * static_cast<uint64_t>(fvuCurrentTsMsbCycle[fuCurrDpbIdx]);
  }
  else {
    ulTime =
      usRawTs
      + static_cast<uint64_t>(stsxyter::kuHitNbTsBins) * (static_cast<uint64_t>(fvulCurrentTsMsb[fuCurrDpbIdx]) % 4);
  }
  return ulTime;
}

// -------------------------------------------------------------------------
void CbmStsUnpackAlgoLegacy::ProcessErrorInfo(const stsxyter::Message& mess)
{
  if (mess.IsMsErrorFlagOn()) {
    if (fMonitor) { fMonitor->FillMsErrorsEvo(fulCurrentMsIdx, mess.GetMsErrorType()); }
    if (fOptOutBVec) {
      fOptOutBVec->push_back(
        CbmErrorMessage(ECbmModuleId::kSts, fulCurrentMsIdx, fuCurrDpbIdx, 0x20, mess.GetMsErrorType()));
    }
  }
}

// -------------------------------------------------------------------------
void CbmStsUnpackAlgoLegacy::MaskNoisyChannel(const uint32_t uFeb, const uint32_t uChan, const bool bMasked)
{
  if (false == fbUseChannelMask) {
    fbUseChannelMask = true;
    fvvbMaskedChannels.resize(fuNbFebs);
    for (uint32_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx) {
      fvvbMaskedChannels[uFebIdx].resize(fUnpackPar->GetNbChanPerFeb(), false);
    }
  }
  if (uFeb < fuNbFebs && uChan < fUnpackPar->GetNbChanPerFeb()) fvvbMaskedChannels[uFeb][uChan] = bMasked;
  else
    LOG(fatal) << "CbmStsUnpackAlgoLegacy::MaskNoisyChannel => Invalid FEB "
                  "and/or CHAN index:"
               << Form(" %u vs %u and %u vs %u", uFeb, fuNbFebs, uChan, fUnpackPar->GetNbChanPerFeb());
}
// -------------------------------------------------------------------------
