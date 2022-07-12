/* Copyright (C) 2018-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau, Florian Uhlig [committer] */

// -------------------------------------------------------------------------
// -----                CbmMcbm2018TofPar source file                  -----
// -----                Created 09/09/18  by P.-A. Loizeau             -----
// -------------------------------------------------------------------------

#include "CbmMcbm2018TofPar.h"

#include "CbmTofAddress.h"
#include "CbmTofDetectorId_v14a.h"  // in cbmdata/tof

#include "FairDetParIo.h"
#include "FairParIo.h"
#include "FairParamList.h"
#include <Logger.h>

#include "TString.h"

#include <iomanip>

#include "gDpbMessv100.h"

// -----   Standard constructor   ------------------------------------------
CbmMcbm2018TofPar::CbmMcbm2018TofPar(const char* name, const char* title, const char* context)
  : FairParGenericSet(name, title, context)
  , fvdPadiThrCodeToValue(GetNrOfPadiThrCodes(), 0.0)
  , fiNrOfGdpb(-1)
  , fiGdpbIdArray()
  , fiNrOfFeesPerGdpb(-1)
  , fiNrOfGet4PerFee(-1)
  , fiNrOfChannelsPerGet4(-1)
  , fiNrOfGbtx(-1)
  , fiNrOfModule(-1)
  , fiNrOfRpc()
  , fiRpcType()
  , fiRpcSide()
  , fiModuleId()
  , fiNbMsTot(0)
  , fiNbMsOverlap(0)
  , fdSizeMsInNs(0.0)
  , fdStarTriggerDeadtime()
  , fdStarTriggerDelay()
  , fdStarTriggerWinSize()
  , fdTsDeadtimePeriod(0.0)
{
  detName = "Tof";

  /// PADI threshold measures and extrapolated code to value map
  for (UInt_t uPadiPoint = 0; uPadiPoint < kuNbThrMeasPoints; ++uPadiPoint) {
    fvdPadiThrCodeToValue[kuThrMeasCode[uPadiPoint]] = kdThrMeasVal[uPadiPoint];

    /// Linear extrapolation between measured points
    if (uPadiPoint + 1 < kuNbThrMeasPoints) {
      UInt_t uNbSteps   = kuThrMeasCode[uPadiPoint + 1] - kuThrMeasCode[uPadiPoint];
      Double_t dValStep = (kdThrMeasVal[uPadiPoint + 1] - kdThrMeasVal[uPadiPoint]) / uNbSteps;
      UInt_t uCode      = kuThrMeasCode[uPadiPoint];
      for (UInt_t uStep = 1; uStep < uNbSteps; ++uStep) {
        uCode++;
        fvdPadiThrCodeToValue[uCode] = kdThrMeasVal[uPadiPoint] + dValStep * uStep;
      }  // for( UInt_t uStep = 1; uStep < uNbSteps; ++uStep)
    }    // if( uPadiPoint + 1 < kuNbThrMeasPoints )
  }      // for( UInt_t uPadiPoint = 0; uPadiPoint < kuNbThrMeasPoints; ++uPadiPoint )
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmMcbm2018TofPar::~CbmMcbm2018TofPar() {}
// -------------------------------------------------------------------------


// -----   Public method clear   -------------------------------------------
void CbmMcbm2018TofPar::clear()
{
  status = kFALSE;
  resetInputVersions();
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------

void CbmMcbm2018TofPar::putParams(FairParamList* l)
{
  if (!l) return;
  l->add("NrOfGdpbs", fiNrOfGdpb);
  l->add("GdpbIdArray", fiGdpbIdArray);
  l->add("NrOfFeesPerGdpb", fiNrOfFeesPerGdpb);
  l->add("NrOfGet4PerFee", fiNrOfGet4PerFee);
  l->add("NrOfChannelsPerGet4", fiNrOfChannelsPerGet4);
  l->add("NrOfGbtx", fiNrOfGbtx);
  l->add("NrOfModule", fiNrOfModule);
  l->add("NrOfRpcs", fiNrOfRpc);
  l->add("RpcType", fiRpcType);
  l->add("RpcSide", fiRpcSide);
  l->add("ModuleId", fiModuleId);
  l->add("NbMsTot", fiNbMsTot);
  l->add("NbMsOverlap", fiNbMsOverlap);
  l->add("SizeMsInNs", fdSizeMsInNs);
  l->add("StarTriggerDeadtime", fdStarTriggerDeadtime);
  l->add("StarTriggerDelay", fdStarTriggerDelay);
  l->add("StarTriggerWinSize", fdStarTriggerWinSize);
  l->add("TsDeadtimePeriod", fdTsDeadtimePeriod);
}

//------------------------------------------------------

Bool_t CbmMcbm2018TofPar::getParams(FairParamList* l)
{

  if (!l) return kFALSE;

  if (!l->fill("NrOfGdpbs", &fiNrOfGdpb)) return kFALSE;

  fiGdpbIdArray.Set(fiNrOfGdpb);
  if (!l->fill("GdpbIdArray", &fiGdpbIdArray)) return kFALSE;

  if (!l->fill("NrOfFeesPerGdpb", &fiNrOfFeesPerGdpb)) return kFALSE;
  if (!l->fill("NrOfGet4PerFee", &fiNrOfGet4PerFee)) return kFALSE;
  if (!l->fill("NrOfChannelsPerGet4", &fiNrOfChannelsPerGet4)) return kFALSE;

  if (!l->fill("NrOfGbtx", &fiNrOfGbtx)) return kFALSE;

  if (!l->fill("NrOfModule", &fiNrOfModule)) return kFALSE;

  fiNrOfRpc.Set(fiNrOfGbtx);
  if (!l->fill("NrOfRpc", &fiNrOfRpc)) return kFALSE;

  fiRpcType.Set(fiNrOfGbtx);
  if (!l->fill("RpcType", &fiRpcType)) return kFALSE;

  fiRpcSide.Set(fiNrOfGbtx);
  if (!l->fill("RpcSide", &fiRpcSide)) return kFALSE;

  fiModuleId.Set(fiNrOfGbtx);
  if (!l->fill("ModuleId", &fiModuleId)) return kFALSE;

  if (!l->fill("NbMsTot", &fiNbMsTot)) return kFALSE;
  if (!l->fill("NbMsOverlap", &fiNbMsOverlap)) return kFALSE;
  if (!l->fill("SizeMsInNs", &fdSizeMsInNs)) return kFALSE;

  fdStarTriggerDeadtime.Set(fiNrOfGdpb);
  fdStarTriggerDelay.Set(fiNrOfGdpb);
  fdStarTriggerWinSize.Set(fiNrOfGdpb);
  if (!l->fill("StarTriggerDeadtime", &fdStarTriggerDeadtime)) return kFALSE;
  if (!l->fill("StarTriggerDelay", &fdStarTriggerDelay)) return kFALSE;
  if (!l->fill("StarTriggerWinSize", &fdStarTriggerWinSize)) return kFALSE;
  if (!l->fill("TsDeadtimePeriod", &fdTsDeadtimePeriod)) return kFALSE;

  LOG(info) << "Build TOF Channels UId Map:";
  BuildChannelsUidMap();

  return kTRUE;
}
// -------------------------------------------------------------------------
Int_t CbmMcbm2018TofPar::Get4ChanToPadiChan(UInt_t uChannelInFee)
{
  if (uChannelInFee < kuNbChannelsPerFee) return kuGet4topadi[uChannelInFee];
  else {
    LOG(fatal) << "CbmMcbm2018TofPar::Get4ChanToPadiChan => Index out of bound, " << uChannelInFee << " vs "
               << static_cast<uint32_t>(kuNbChannelsPerFee) << ", returning crazy value!";
    return -1;
  }  // else of if( uChannelInFee < kuNbChannelsPerFee )
}
Int_t CbmMcbm2018TofPar::PadiChanToGet4Chan(UInt_t uChannelInFee)
{
  if (uChannelInFee < kuNbChannelsPerFee) return kuPaditoget4[uChannelInFee];
  else {
    LOG(fatal) << "CbmMcbm2018TofPar::PadiChanToGet4Chan => Index out of bound, " << uChannelInFee << " vs "
               << static_cast<uint32_t>(kuNbChannelsPerFee) << ", returning crazy value!";
    return -1;
  }  // else of if( uChannelInFee < kuNbChannelsPerFee )
}
// -------------------------------------------------------------------------
Int_t CbmMcbm2018TofPar::ElinkIdxToGet4Idx(UInt_t uElink)
{
  if (gdpbv100::kuChipIdMergedEpoch == uElink) return uElink;
  else if (uElink < kuNbGet4PerGdpb)
    return kuElinkToGet4[uElink % kuNbGet4PerGbtx] + kuNbGet4PerGbtx * (uElink / kuNbGet4PerGbtx);
  else {
    LOG(fatal) << "CbmMcbm2018TofPar::ElinkIdxToGet4Idx => Index out of bound, " << uElink << " vs "
               << static_cast<uint32_t>(kuNbGet4PerGdpb) << ", returning crazy value!";
    return -1;
  }  // else of if( uElink < kuNbGet4PerGbtx )
}
Int_t CbmMcbm2018TofPar::Get4IdxToElinkIdx(UInt_t uGet4)
{
  if (gdpbv100::kuChipIdMergedEpoch == uGet4) return uGet4;
  else if (uGet4 < kuNbGet4PerGdpb)
    return kuGet4ToElink[uGet4 % kuNbGet4PerGbtx] + kuNbGet4PerGbtx * (uGet4 / kuNbGet4PerGbtx);
  else {
    LOG(fatal) << "CbmMcbm2018TofPar::Get4IdxToElinkIdx => Index out of bound, " << uGet4 << " vs "
               << static_cast<uint32_t>(kuNbGet4PerGdpb) << ", returning crazy value!";
    return -1;
  }  // else of if( uElink < kuNbGet4PerGbtx )
}
// -------------------------------------------------------------------------
Double_t CbmMcbm2018TofPar::GetPadiThresholdVal(UInt_t uCode)
{
  if (uCode < GetNrOfPadiThrCodes()) return fvdPadiThrCodeToValue[uCode];
  else {
    LOG(error) << "CbmStar2019TofPar::GetPadiThresholdVal => Code out of bound, " << uCode << " vs "
               << GetNrOfPadiThrCodes() << ", returning crazy value!";
    return 1e9;
  }  // else of if( uCode < GetNrOfPadiThrCodes() )
}
// -------------------------------------------------------------------------
void CbmMcbm2018TofPar::BuildChannelsUidMap()
{
  UInt_t uNrOfGbtx     = fiNrOfGbtx;
  UInt_t uNrOfGet4     = fiNrOfGdpb * fiNrOfFeesPerGdpb * fiNrOfGet4PerFee;
  UInt_t uNrOfChannels = uNrOfGet4 * fiNrOfChannelsPerGet4;
  fviRpcChUId.resize(uNrOfChannels);

  UInt_t uCh = 0;
  for (UInt_t uGbtx = 0; uGbtx < uNrOfGbtx; ++uGbtx) {
    uint32_t uCh0 = uCh;
    switch (fiRpcType[uGbtx]) {
      case 2:  // intended fall-through
      case 0: {
        // CBM modules
        BuildChannelsUidMapCbm(uCh, uGbtx);
        break;
      }
      case 1: {
        // STAR eTOF  modules
        BuildChannelsUidMapStar(uCh, uGbtx);
        break;
      }
      case 5: {
        /// Special Treatment for the T0 diamond
        BuildChannelsUidMapT0(uCh, uGbtx);
        break;
      }
      case 99: {
        /// Special Treatment for the 2022 T0 diamond, keep past behavior for older data!
        BuildChannelsUidMapT0_2022(uCh, uGbtx);
        break;
      }
      case 78: {
        // cern-20-gap + ceramic module
        BuildChannelsUidMapCern(uCh, uGbtx);
      }
        [[fallthrough]];  // fall through is intended
      case 8:             // ceramics
      {
        BuildChannelsUidMapCera(uCh, uGbtx);
        break;
      }
      case 4:  // intended fallthrough
        [[fallthrough]];
      case 9:  // Star2 boxes
      {
        BuildChannelsUidMapStar2(uCh, uGbtx);
        break;
      }
      case 6:  // Buc box
      {
        BuildChannelsUidMapBuc(uCh, uGbtx);
        break;
      }
      case 69: {
        /// 2022 case: 69 is followed by 4 and 9
        BuildChannelsUidMapBuc(uCh, uGbtx);
        /// Map also 4 and 9 (equivalent to fallthrough to 4 then 9 but without changing past behaviors)
        uCh -= 80;  // PAL, 2022/03/17: ?!?
        BuildChannelsUidMapStar2(uCh, uGbtx);
        uCh -= 80;  // PAL, 2022/03/17: ?!?
        break;
      }
      case -1: {
        LOG(info) << " Found unused GBTX link at uCh = " << uCh;
        uCh += 160;
        break;
      }
      default: {
        LOG(error) << "Invalid Tof Type specifier for GBTx " << std::setw(2) << uGbtx << ": " << fiRpcType[uGbtx];
      }
    }  // switch (fiRpcType[uGbtx])
    if ((int32_t)(uCh - uCh0) != fiNrOfFeesPerGdpb * fiNrOfGet4PerFee * fiNrOfChannelsPerGet4 / 2) {
      LOG(fatal) << "Tof mapping error for Gbtx " << uGbtx << ",  diff = " << uCh - uCh0;
    }
  }    // for (UInt_t uGbtx = 0; uGbtx < uNrOfGbtx; ++uGbtx)
}
// -------------------------------------------------------------------------
void CbmMcbm2018TofPar::BuildChannelsUidMapCbm(UInt_t& uCh, UInt_t uGbtx)
{
  LOG(info) << " Map mTof box " << fiModuleId[uGbtx] << " at GBTX " << uGbtx << " -  uCh = " << uCh;
  if (fiRpcSide[uGbtx] < 2) {  // mTof modules
    LOG(debug) << " Map mTof box " << fiModuleId[uGbtx] << " at GBTX  -  uCh = " << uCh;
    const Int_t RpcMap[5] = {4, 2, 0, 3, 1};
    for (Int_t iRpc = 0; iRpc < fiNrOfRpc[uGbtx]; iRpc++) {
      Int_t iStrMax  = 32;
      UInt_t uChNext = 1;

      for (Int_t iStr = 0; iStr < iStrMax; iStr++) {
        Int_t iStrMap = iStr;
        Int_t iRpcMap = RpcMap[iRpc];

        if (fiRpcSide[uGbtx] == 0) iStrMap = 31 - iStr;
        if (fiModuleId[uGbtx] > -1)
          fviRpcChUId[uCh] =
            CbmTofAddress::GetUniqueAddress(fiModuleId[uGbtx], iRpcMap, iStrMap, fiRpcSide[uGbtx], fiRpcType[uGbtx]);
        else
          fviRpcChUId[uCh] = 0;
        //  LOG(debug)<<Form("Map Ch %d to Address 0x%08x",uCh,fviRpcChUId[uCh]);
        uCh += uChNext;
      }  // for (Int_t iStr = 0; iStr < iStrMax; iStr++)
    }    // for (Int_t iRpc = 0; iRpc < fiNrOfRpc[uGbtx]; iRpc++)
  }      // if (fiRpcSide[uGbtx] < 2)
}
// -------------------------------------------------------------------------
void CbmMcbm2018TofPar::BuildChannelsUidMapStar(UInt_t& uCh, UInt_t uGbtx)
{
  if (fiRpcSide[uGbtx] < 2) {
    // mTof modules
    LOG(info) << "Start eTOF module side " << fiRpcSide[uGbtx] << " at " << uCh;
    const Int_t RpcMap[3] = {0, 1, 2};
    for (Int_t iRpc = 0; iRpc < fiNrOfRpc[uGbtx]; iRpc++) {
      Int_t iStrMax = 32;
      Int_t uChNext = 1;

      for (Int_t iStr = 0; iStr < iStrMax; iStr++) {
        Int_t iStrMap = iStr;
        Int_t iRpcMap = RpcMap[iRpc];

        if (fiRpcSide[uGbtx] == 0) iStrMap = 31 - iStr;
        if (fiModuleId[uGbtx] > -1)
          fviRpcChUId[uCh] =
            CbmTofAddress::GetUniqueAddress(fiModuleId[uGbtx], iRpcMap, iStrMap, fiRpcSide[uGbtx], fiRpcType[uGbtx]);
        else
          fviRpcChUId[uCh] = 0;
        //  LOG(debug)<<Form("Map Ch %d to Address 0x%08x",uCh,fviRpcChUId[uCh]);
        uCh += uChNext;
      }
    }
  }
  uCh += 64;
}
// -------------------------------------------------------------------------
void CbmMcbm2018TofPar::BuildChannelsUidMapT0(UInt_t& uCh, UInt_t uGbtx)
{
  LOG(info) << " Map diamond  at GBTX  -  uCh = " << uCh;
  for (UInt_t uFee = 0; uFee < kuNbFeePerGbtx; ++uFee) {
    for (UInt_t uFeeCh = 0; uFeeCh < kuNbChannelsPerFee; ++uFeeCh) {
      /// Mapping for the 2019 beamtime
      if (0 == uFee && 1 == fiNrOfRpc[uGbtx]) {
        switch (uCh % 8) {
          case 0:
          case 2:
          case 4: {
            /// 2019 mapping with 320/640 Mb/s FW
            /// => 4 GET4 per GBTx
            /// => 1 T0 channel per GET4
            /// => 1-2 eLinks per GET4 => GET4 ID = GET4 * 2 (+ 1)
            UInt_t uChannelT0 = uFeeCh / 8 + 4 * fiRpcSide[uGbtx];
            fviRpcChUId[uCh]  = CbmTofAddress::GetUniqueAddress(fiModuleId[uGbtx], 0, uChannelT0, 0, fiRpcType[uGbtx]);
            LOG(info) << Form("T0 channel: %u from GBTx %2u Fee %2u "
                              "Channel %2u, indx %d address %08x",
                              uChannelT0, uGbtx, uFeeCh, uCh, uCh, fviRpcChUId[uCh]);
            break;
          }  // Valid T0 channel
          default: {
            fviRpcChUId[uCh] = 0;
          }  // Invalid T0 channel
        }    // switch( uCh % 4 )
      }      // if( 0 == uFee )

      uCh++;
    }  // for( UInt_t uCh = 0; uCh < kuNbFeePerGbtx; ++uCh )
  }    // for( UInt_t uFee = 0; uFee < kuNbChannelsPerFee; ++uFee )
}
// -------------------------------------------------------------------------
void CbmMcbm2018TofPar::BuildChannelsUidMapT0_2022(UInt_t& uCh, UInt_t uGbtx)
{
  LOG(info) << " Map 2022 diamond " << fiModuleId[uGbtx] << " at GBTX " << uGbtx << " -  uCh = " << uCh;
  for (UInt_t uGet4 = 0; uGet4 < kuNbGet4PerGbtx; ++uGet4) {
    for (UInt_t uGet4Ch = 0; uGet4Ch < kuNbChannelsPerGet4; ++uGet4Ch) {
      /// Mapping for the 2022 beamtime
      if (-1 < fiModuleId[uGbtx] && uGet4 < 32 && 0 == uGet4 % 4 && 0 == uGet4Ch) {
        /// 1 channel per physical GET4, 2 links per physical GET4, 4 physical GET4s per GBTx, 1 GBTx per comp.
        /// 16 channels for one side, 16 for the other
        UInt_t uChannelT0 = (uGet4 / 8 + 4 * (uGbtx / 2)) % 16;
        /// Type hard-coded to allow different parameter values to separate 2022 T0 and pre-2022 T0
        fviRpcChUId[uCh] = CbmTofAddress::GetUniqueAddress(fiModuleId[uGbtx], 0, uChannelT0, fiRpcSide[uGbtx], 5);
        LOG(info) << Form("  Bmon channel: %u side %u from GBTx %2u, "
                          "indx %d address %08x",
                          uChannelT0, fiRpcSide[uGbtx], uGbtx, uCh, fviRpcChUId[uCh]);
      }  // Valid T0 channel
      else {
        fviRpcChUId[uCh] = 0;
      }  // Invalid T0 channel
      uCh++;
    }  // for( UInt_t uCh = 0; uCh < kuNbFeePerGbtx; ++uCh )
  }    // for( UInt_t uFee = 0; uFee < kuNbChannelsPerFee; ++uFee )
}
// -------------------------------------------------------------------------
void CbmMcbm2018TofPar::BuildChannelsUidMapCern(UInt_t& uCh, UInt_t /*uGbtx*/)
{
  LOG(info) << " Map CERN 20 gap  at GBTX  -  uCh = " << uCh;
  // clang-format off
  const Int_t StrMap[32] = {0,  1,  2,  3,  4,  31, 5,  6,  7,  30, 8,
                            9,  10, 29, 11, 12, 13, 14, 28, 15, 16, 17,
                            18, 27, 26, 25, 24, 23, 22, 21, 20, 19};
  // clang-format on
  Int_t iModuleId   = 0;
  Int_t iModuleType = 7;
  Int_t iRpcMap     = 0;
  for (Int_t iFeet = 0; iFeet < 2; iFeet++) {
    for (Int_t iStr = 0; iStr < 32; iStr++) {
      Int_t iStrMap  = 31 - 12 - StrMap[iStr];
      Int_t iSideMap = iFeet;
      if (iStrMap < 20)
        fviRpcChUId[uCh] = CbmTofAddress::GetUniqueAddress(iModuleId, iRpcMap, iStrMap, iSideMap, iModuleType);
      else
        fviRpcChUId[uCh] = 0;
      uCh++;
    }
  }
  LOG(info) << " Map end CERN 20 gap  at GBTX  -  uCh = " << uCh;
}
// -------------------------------------------------------------------------
void CbmMcbm2018TofPar::BuildChannelsUidMapCera(UInt_t& uCh, UInt_t /*uGbtx*/)
{
  Int_t iModuleId   = 0;
  Int_t iModuleType = 8;
  for (Int_t iRpc = 0; iRpc < 8; iRpc++) {
    fviRpcChUId[uCh] = CbmTofAddress::GetUniqueAddress(iModuleId, 7 - iRpc, 0, 0, iModuleType);
    uCh++;
  }
  uCh += (24 + 2 * 32);
  LOG(info) << " Map end ceramics  box  at GBTX  -  uCh = " << uCh;
}
// -------------------------------------------------------------------------
void CbmMcbm2018TofPar::BuildChannelsUidMapStar2(UInt_t& uCh, UInt_t uGbtx)
{
  LOG(info) << " Map Star2 box " << fiModuleId[uGbtx] << " at GBTX " << uGbtx << " -  uCh = " << uCh;
  const Int_t iRpc[5]  = {1, -1, 1, 0, 0};
  const Int_t iSide[5] = {1, -1, 0, 1, 0};
  for (Int_t iFeet = 0; iFeet < 5; iFeet++) {
    for (Int_t iStr = 0; iStr < 32; iStr++) {
      Int_t iStrMap  = iStr;
      Int_t iRpcMap  = iRpc[iFeet];
      Int_t iSideMap = iSide[iFeet];
      if (iSideMap == 0) iStrMap = 31 - iStr;
      switch (fiRpcSide[uGbtx]) {
        case 0:; break;
        case 1:;
          iRpcMap = 1 - iRpcMap;  // swap counters
          break;
        case 2:
          switch (iFeet) {
            case 1:
              iRpcMap  = iRpc[4];
              iSideMap = iSide[4];
              iStrMap  = 31 - iStrMap;
              break;
            case 4:
              iRpcMap  = iRpc[1];
              iSideMap = iSide[1];
              break;
            default:;
          }
          break;
        case 3:  // direct beam 20210524
          switch (iFeet) {
            case 0:
              iRpcMap  = 0;
              iSideMap = 0;
              iStrMap  = iStr;
              break;
            case 1:
              iRpcMap  = 0;
              iSideMap = 1;
              iStrMap  = 31 - iStr;
              break;
            default: iSideMap = -1;
          }
          break;
      }
      if (iSideMap > -1)
        fviRpcChUId[uCh] =
          CbmTofAddress::GetUniqueAddress(fiModuleId[uGbtx], iRpcMap, iStrMap, iSideMap, fiRpcType[uGbtx]);
      else
        fviRpcChUId[uCh] = 0;
      uCh++;
    }
  }
}
// -------------------------------------------------------------------------
void CbmMcbm2018TofPar::BuildChannelsUidMapBuc(UInt_t& uCh, UInt_t uGbtx)
{
  LOG(info) << " Map Buc box " << fiModuleId[uGbtx] << " at GBTX " << uGbtx << " -  uCh = " << uCh;

  Int_t iModuleIdMap   = fiModuleId[uGbtx];
  const Int_t iRpc[5]  = {0, -1, 0, 1, 1};
  const Int_t iSide[5] = {1, -1, 0, 1, 0};
  for (Int_t iFeet = 0; iFeet < 5; iFeet++) {
    for (Int_t iStr = 0; iStr < 32; iStr++) {
      Int_t iStrMap  = iStr;
      Int_t iRpcMap  = iRpc[iFeet];
      Int_t iSideMap = iSide[iFeet];
      switch (fiRpcSide[uGbtx]) {
        case 0:; break;
        case 1:  // HD cosmic 2019, Buc2018, v18n
          iStrMap = 31 - iStr;
          iRpcMap = 1 - iRpcMap;
          break;
        case 2:  // v18m_cosmicHD
          //   iStrMap=31-iStr;
          iSideMap = 1 - iSideMap;
          break;
        case 3:
          iStrMap  = 31 - iStr;
          iRpcMap  = 1 - iRpcMap;
          iSideMap = 1 - iSideMap;
          break;
        case 4:  // HD cosmic 2019, Buc2018, v18o
          iRpcMap = 1 - iRpcMap;
          break;
        case 5:  // HD cosmic 2020, Buc2018, v20a
          iStrMap = 31 - iStr;
          break;
        case 6:  //BUC special
        {
          switch (fiModuleId[uGbtx]) {
            case 0: iRpcMap = 0; break;
            case 1: iRpcMap = 1; break;
          }
          if (iFeet % 2 == 1) iModuleIdMap = 1;
          else
            iModuleIdMap = 0;

          switch (iFeet) {
            case 0:
            case 3: iSideMap = 0; break;
            case 1:
            case 2: iSideMap = 1; break;
          }
          break;
        }
        case 8: {
          // Special case for two channels in 2022
          // Fallthrough to 7 for all other channels
          if (2 == iFeet) {
            if (7 == iStr) {
              ///                                               SM Rpc St Si Type
              fviRpcChUId[uCh] = CbmTofAddress::GetUniqueAddress(0, 0, 0, 0, 8);
              uCh++;
              continue;
            }
            else if (23 == iStr) {
              ///                                               SM Rpc St Si Type
              fviRpcChUId[uCh] = CbmTofAddress::GetUniqueAddress(1, 0, 0, 0, 8);
              uCh++;
              continue;
            }
          }
        }
        case 7: {
          // clang-format off
          /*
          const Int_t iChMap[160]={
          127, 126, 125, 124,  12,  13,  14,  15,   7,   6,   5,   4,  28,  29,  30,  31, 123, 122, 121, 120,   8,   9,  10,  11, 107, 106, 105, 104, 108, 109, 110, 111,
           39,  38,  37,  36,  52,  53,  54,  55,  63,  62,  61,  60, 128, 129, 130, 131,  43,  42,  41,  40, 148, 149, 150, 151,  59,  58,  57,  56, 132, 133, 134, 135,
          139, 138, 137, 136, 140, 141, 142, 143,  99,  98,  97,  96,  64,  65,  66,  67, 103, 102, 101, 100,  84,  85,  86,  87, 155, 154, 153, 152,  68,  69,  70,  71,
          159, 158, 157, 156, 144, 145, 146, 147,  47,  46,  45,  44,  76,  77,  78,  79,  51,  50,  49,  48,  20,  21,  22,  23,  35,  34,  33,  32, 116, 117, 118, 119,
           75,  74,  73,  72,  92,  93,  94,  95,  19,  18,  17,  16,  80,  81,  82,  83, 115, 114, 113, 112,  24,  25,  26,  27,  91,  90,  89,  88,   0,   1,   2,   3
          };
          */
          const Int_t iChMap[160]={
          124, 125, 126, 127,  12,  13,  14,  15,   4,   5,   6,   7,  28,  29,  30,  31, 120, 121, 122, 123,   8,  9,   10,  11, 104, 105, 106, 107, 108, 109, 110, 111,
           36,  37,  38,  39,  52,  53,  54,  55,  60,  61,  62,  63, 128, 129, 130, 131,  40,  41,  42,  43, 148, 149, 150, 151,  56,  57,  58,  59, 132, 133, 134, 135,
          136, 137, 138, 139, 140, 141, 142, 143,  96,  97,  98,  99,  64,  65,  66,  67, 100, 101, 102, 103,  84,  85,  86,  87, 152, 153, 154, 155,  68,  69,  70,  71,
          156, 157, 158, 159, 144, 145, 146, 147,  44,  45,  46,  47,  76,  77,  78,  79,  48,  49,  50,  51,  20,  21,  22,  23,  32,  33,  34,  35, 116, 117, 118, 119,
           75,  74,  73,  72,  92,  93,  94,  95,  16,  17,  18,  19,  80,  81,  82,  83, 115, 114, 113, 112,  24,  25,  26,  27,  88,  89,  90,  91,   0,   1,   2,   3
          };
          // clang-format on
          Int_t iInd = iFeet * 32 + iStr;
          Int_t i    = 0;
          for (; i < 160; i++) {
            if (iInd == iChMap[i]) break;
          }
          iStrMap        = i % 32;
          Int_t iFeetInd = (i - iStrMap) / 32;
          switch (iFeetInd) {
            case 0: {
              iRpcMap  = 0;
              iSideMap = 1;
              break;
            }
            case 1: {
              iRpcMap  = 1;
              iSideMap = 1;
              break;
            }
            case 2: {
              iRpcMap  = 1;
              iSideMap = 0;
              break;
            }
            case 3: {
              iRpcMap  = 0;
              iSideMap = 0;
              break;
            }
            case 4: {
              iSideMap = -1;
              break;
            }
          }
          iModuleIdMap = fiModuleId[uGbtx];
          LOG(debug) << "Buc of GBTX " << uGbtx << " Ch " << uCh
                     << Form(", Feet %1d, Str %2d, Ind %3d, i %3d, FeetInd %1d, Rpc %1d, Side %1d, Str %2d ", iFeet,
                             iStr, iInd, i, iFeetInd, iRpcMap, iSideMap, iStrMap);
          break;
        }
        default: {
          break;
        }
      }  // switch (fiRpcSide[uGbtx])
      if (iSideMap > -1)
        fviRpcChUId[uCh] = CbmTofAddress::GetUniqueAddress(iModuleIdMap, iRpcMap, iStrMap, iSideMap, fiRpcType[uGbtx]);
      else
        fviRpcChUId[uCh] = 0;

      uCh++;
    }  // for (Int_t iStr = 0; iStr < 32; iStr++)
  }    // for (Int_t iFeet = 0; iFeet < 5; iFeet++)
}
// -------------------------------------------------------------------------


ClassImp(CbmMcbm2018TofPar)
