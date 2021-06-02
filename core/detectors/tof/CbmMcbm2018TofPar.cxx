/** @file CbmMcbm2018TofPar.cxx
  * @copyright Copyright (C) 2018-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors P.-A. Loizeau, Florian Uhlig [committer] **/

// -------------------------------------------------------------------------
// -----                CbmMcbm2018TofPar source file                  -----
// -----                Created 09/09/18  by P.-A. Loizeau             -----
// -------------------------------------------------------------------------

#include "CbmMcbm2018TofPar.h"

#include "FairDetParIo.h"
#include "FairParIo.h"
#include "FairParamList.h"
#include <Logger.h>

#include "TString.h"

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


ClassImp(CbmMcbm2018TofPar)
