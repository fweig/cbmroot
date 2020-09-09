// -------------------------------------------------------------------------
// -----                CbmStar2019TofPar source file                  -----
// -----                Created 09/09/18  by P.-A. Loizeau             -----
// -------------------------------------------------------------------------

#include "CbmStar2019TofPar.h"

#include "gDpbMessv100.h"

#include "FairDetParIo.h"
#include <Logger.h>
#include "FairParIo.h"
#include "FairParamList.h"

#include "TString.h"

// -----   Standard constructor   ------------------------------------------
CbmStar2019TofPar::CbmStar2019TofPar(const char* name,
                                     const char* title,
                                     const char* context)
  : FairParGenericSet(name, title, context)
  , fvdPadiThrCodeToValue(GetNrOfPadiThrCodes(), 0.0)
  , fiMonitorMode(0)
  , fiDebugMonitorMode(0)
  , fiNrOfGdpb(-1)
  , fiGdpbIdArray()
  , fiNrOfGbtx(-1)
  , fiNrOfModule(-1)
  , fiNrOfRpc()
  , fiRpcType()
  , fiRpcSide()
  , fiModuleId()
  , fdSizeMsInNs(0.0)
  , fdStarTriggAllowedSpread(0.0)
  , fdStarTriggerDeadtime()
  , fdStarTriggerDelay()
  , fdStarTriggerWinSize() {
  detName = "Tof";

  /// PADI threshold measures and extrapolated code to value map
  for (UInt_t uPadiPoint = 0; uPadiPoint < kuNbThrMeasPoints; ++uPadiPoint) {
    fvdPadiThrCodeToValue[kuThrMeasCode[uPadiPoint]] = kdThrMeasVal[uPadiPoint];

    /// Linear extrapolation between measured points
    if (uPadiPoint + 1 < kuNbThrMeasPoints) {
      UInt_t uNbSteps =
        kuThrMeasCode[uPadiPoint + 1] - kuThrMeasCode[uPadiPoint];
      Double_t dValStep =
        (kdThrMeasVal[uPadiPoint + 1] - kdThrMeasVal[uPadiPoint]) / uNbSteps;
      UInt_t uCode = kuThrMeasCode[uPadiPoint];
      for (UInt_t uStep = 1; uStep < uNbSteps; ++uStep) {
        uCode++;
        fvdPadiThrCodeToValue[uCode] =
          kdThrMeasVal[uPadiPoint] + dValStep * uStep;
      }  // for( UInt_t uStep = 1; uStep < uNbSteps; ++uStep)
    }    // if( uPadiPoint + 1 < kuNbThrMeasPoints )
  }  // for( UInt_t uPadiPoint = 0; uPadiPoint < kuNbThrMeasPoints; ++uPadiPoint )
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmStar2019TofPar::~CbmStar2019TofPar() {}
// -------------------------------------------------------------------------


// -----   Public method clear   -------------------------------------------
void CbmStar2019TofPar::clear() {
  status = kFALSE;
  resetInputVersions();
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------

void CbmStar2019TofPar::putParams(FairParamList* l) {
  if (!l) return;
  l->add("MonitorMode", fiMonitorMode);
  l->add("DebugMonitorMode", fiDebugMonitorMode);
  l->add("NrOfGdpbs", fiNrOfGdpb);
  l->add("GdpbIdArray", fiGdpbIdArray);
  l->add("NrOfGbtx", fiNrOfGbtx);
  l->add("NrOfModule", fiNrOfModule);
  l->add("NrOfRpcs", fiNrOfRpc);
  l->add("RpcType", fiRpcType);
  l->add("RpcSide", fiRpcSide);
  l->add("ModuleId", fiModuleId);
  l->add("SizeMsInNs", fdSizeMsInNs);
  l->add("SizeMsInNs", fdSizeMsInNs);
  l->add("StarTriggAllowedSpread", fdStarTriggAllowedSpread);
  l->add("StarTriggerDelay", fdStarTriggerDelay);
  l->add("StarTriggerWinSize", fdStarTriggerWinSize);
}

//------------------------------------------------------

Bool_t CbmStar2019TofPar::getParams(FairParamList* l) {

  LOG(info) << "CbmStar2019TofPar::getParams";

  if (!l) return kFALSE;

  if (!l->fill("MonitorMode", &fiMonitorMode)) return kFALSE;

  if (!l->fill("DebugMonitorMode", &fiDebugMonitorMode)) return kFALSE;

  if (!l->fill("NrOfGdpbs", &fiNrOfGdpb)) return kFALSE;

  fiGdpbIdArray.Set(fiNrOfGdpb);
  if (!l->fill("GdpbIdArray", &fiGdpbIdArray)) return kFALSE;

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

  if (!l->fill("SizeMsInNs", &fdSizeMsInNs)) return kFALSE;

  if (!l->fill("StarTriggAllowedSpread", &fdStarTriggAllowedSpread))
    return kFALSE;

  fdStarTriggerDeadtime.Set(fiNrOfGdpb);
  fdStarTriggerDelay.Set(fiNrOfGdpb);
  fdStarTriggerWinSize.Set(fiNrOfGdpb);
  if (!l->fill("StarTriggerDeadtime", &fdStarTriggerDeadtime)) return kFALSE;
  if (!l->fill("StarTriggerDelay", &fdStarTriggerDelay)) return kFALSE;
  if (!l->fill("StarTriggerWinSize", &fdStarTriggerWinSize)) return kFALSE;

  LOG(info) << "CbmStar2019TofPar::getParams DONE!";

  return kTRUE;
}

// -------------------------------------------------------------------------
Int_t CbmStar2019TofPar::Get4ChanToPadiChan(UInt_t uChannelInFee) {
  if (uChannelInFee < GetNrOfChannelsPerFee())
    return kuGet4topadi[uChannelInFee] - 1;
  else {
    LOG(fatal)
      << "CbmStar2019TofPar::Get4ChanToPadiChan => Index out of bound, "
      << uChannelInFee << " vs " << GetNrOfChannelsPerFee()
      << ", returning crazy value!";
    return -1;
  }  // else of if( uChannelInFee < GetNrOfChannelsPerFee() )
}
Int_t CbmStar2019TofPar::PadiChanToGet4Chan(UInt_t uChannelInFee) {
  if (uChannelInFee < GetNrOfChannelsPerFee())
    return kuPaditoget4[uChannelInFee] - 1;
  else {
    LOG(fatal)
      << "CbmStar2019TofPar::PadiChanToGet4Chan => Index out of bound, "
      << uChannelInFee << " vs " << GetNrOfChannelsPerFee()
      << ", returning crazy value!";
    return -1;
  }  // else of if( uChannelInFee < GetNrOfChannelsPerFee() )
}
// -------------------------------------------------------------------------
Int_t CbmStar2019TofPar::ElinkIdxToGet4Idx(UInt_t uElink) {
  if (gdpbv100::kuChipIdMergedEpoch == uElink)
    return uElink;
  else if (uElink < GetNrOfGet4PerGdpb())
    return kuElinkToGet4[uElink % kuNbGet4PerGbtx]
           + kuNbGet4PerGbtx * (uElink / kuNbGet4PerGbtx);
  else {
    LOG(fatal) << "CbmStar2019TofPar::ElinkIdxToGet4Idx => Index out of bound, "
               << uElink << " vs " << GetNrOfGet4PerGdpb()
               << ", returning crazy value!";
    return -1;
  }  // else of if( uElink < kuNbGet4PerGbtx )
}
Int_t CbmStar2019TofPar::Get4IdxToElinkIdx(UInt_t uGet4) {
  if (gdpbv100::kuChipIdMergedEpoch == uGet4)
    return uGet4;
  else if (uGet4 < GetNrOfGet4PerGdpb())
    return kuGet4ToElink[uGet4 % kuNbGet4PerGbtx]
           + kuNbGet4PerGbtx * (uGet4 / kuNbGet4PerGbtx);
  else {
    LOG(fatal) << "CbmStar2019TofPar::Get4IdxToElinkIdx => Index out of bound, "
               << uGet4 << " vs " << GetNrOfGet4PerGdpb()
               << ", returning crazy value!";
    return -1;
  }  // else of if( uElink < kuNbGet4PerGbtx )
}
// -------------------------------------------------------------------------
Double_t CbmStar2019TofPar::GetPadiThresholdVal(UInt_t uCode) {
  if (uCode < GetNrOfPadiThrCodes())
    return fvdPadiThrCodeToValue[uCode];
  else {
    LOG(error)
      << "CbmStar2019TofPar::GetPadiThresholdVal => Code out of bound, "
      << uCode << " vs " << GetNrOfPadiThrCodes() << ", returning crazy value!";
    return 1e9;
  }  // else of if( uCode < GetNrOfPadiThrCodes() )
}
// -------------------------------------------------------------------------
Int_t CbmStar2019TofPar::GetNrOfRpc(UInt_t uGbtx) {
  // safe to cast as anyway Nb of GBTx cannot exceed Int limits
  if (static_cast<Int_t>(uGbtx) < fiNrOfGbtx)
    return fiNrOfRpc[uGbtx];
  else {
    LOG(fatal) << "CbmStar2019TofPar::GetNrOfRpc => Index out of bound, "
               << uGbtx << " vs " << fiNrOfGbtx << ", returning crazy value!";
    return -1;
  }  // else of if( static_cast< Int_t >( uGbtx ) < fiNrOfGbtx )
}
Int_t CbmStar2019TofPar::GetRpcType(UInt_t uGbtx) {
  // safe to cast as anyway Nb of GBTx cannot exceed Int limits
  if (static_cast<Int_t>(uGbtx) < fiNrOfGbtx)
    return fiRpcType[uGbtx];
  else {
    LOG(fatal) << "CbmStar2019TofPar::GetRpcType => Index out of bound, "
               << uGbtx << " vs " << fiNrOfGbtx << ", returning crazy value!";
    return -1;
  }  // else of if( static_cast< Int_t >( uGbtx ) < fiNrOfGbtx )
}
Int_t CbmStar2019TofPar::GetRpcSide(UInt_t uGbtx) {
  // safe to cast as anyway Nb of GBTx cannot exceed Int limits
  if (static_cast<Int_t>(uGbtx) < fiNrOfGbtx)
    return fiRpcSide[uGbtx];
  else {
    LOG(fatal) << "CbmStar2019TofPar::GetRpcSide => Index out of bound, "
               << uGbtx << " vs " << fiNrOfGbtx << ", returning crazy value!";
    return -1;
  }  // else of if( static_cast< Int_t >( uGbtx ) < fiNrOfGbtx )
}
Int_t CbmStar2019TofPar::GetModuleId(UInt_t uGbtx) {
  // safe to cast as anyway Nb of GBTx cannot exceed Int limits
  if (static_cast<Int_t>(uGbtx) < fiNrOfGbtx)
    return fiModuleId[uGbtx];
  else {
    LOG(fatal) << "CbmStar2019TofPar::GetModuleId => Index out of bound, "
               << uGbtx << " vs " << fiNrOfGbtx << ", returning crazy value!";
    return -1;
  }  // else of if( static_cast< Int_t >( uGbtx ) < fiNrOfGbtx )
}
// -------------------------------------------------------------------------
Double_t CbmStar2019TofPar::GetStarTriggDeadtime(UInt_t uGdpb) {
  // safe to cast as anyway Nb of gDPB cannot exceed Int limits
  if (static_cast<Int_t>(uGdpb) < fiNrOfGdpb)
    return fdStarTriggerDeadtime[uGdpb];
  else {
    LOG(fatal)
      << "CbmStar2019TofPar::GetStarTriggDeadtime => Index out of bound, "
      << uGdpb << " vs " << fiNrOfGdpb << ", returning crazy value!";
    return -1;
  }  // else of if( static_cast< Int_t >( uGdpb ) < fiNrOfGdpb )
}
Double_t CbmStar2019TofPar::GetStarTriggDelay(UInt_t uGdpb) {
  // safe to cast as anyway Nb of gDPB cannot exceed Int limits
  if (static_cast<Int_t>(uGdpb) < fiNrOfGdpb)
    return fdStarTriggerDelay[uGdpb];
  else {
    LOG(fatal) << "CbmStar2019TofPar::GetStarTriggDelay => Index out of bound, "
               << uGdpb << " vs " << fiNrOfGdpb << ", returning crazy value!";
    return -1;
  }  // else of if( static_cast< Int_t >( uGdpb ) < fiNrOfGdpb )
}
Double_t CbmStar2019TofPar::GetStarTriggWinSize(UInt_t uGdpb) {
  // safe to cast as anyway Nb of gDPB cannot exceed Int limits
  if (static_cast<Int_t>(uGdpb) < fiNrOfGdpb)
    return fdStarTriggerWinSize[uGdpb];
  else {
    LOG(fatal)
      << "CbmStar2019TofPar::GetStarTriggWinSize => Index out of bound, "
      << uGdpb << " vs " << fiNrOfGdpb << ", returning crazy value!";
    return -1;
  }  // else of if( static_cast< Int_t >( uGdpb ) < fiNrOfGdpb )
}
// -------------------------------------------------------------------------

ClassImp(CbmStar2019TofPar)
