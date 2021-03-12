// -------------------------------------------------------------------------
// -----            CbmCosy2019HodoPar source file                      -----
// -----            Created 31/07/19  by P.-A. Loizeau                 -----
// -------------------------------------------------------------------------

#include "CbmCosy2019HodoPar.h"

#include "FairDetParIo.h"
#include "FairLogger.h"
#include "FairParIo.h"
#include "FairParamList.h"

#include "TMath.h"
#include "TString.h"

using namespace std;

// -----   Standard constructor   ------------------------------------------
CbmCosy2019HodoPar::CbmCosy2019HodoPar(const char* name, const char* title, const char* context)
  : FairParGenericSet(name, title, context)
  , fuNbModules(0)
  , fiModAddress()
  , fdModCenterPosX()
  , fdModCenterPosY()
  , fiModSwapXY()
  , fiModInvertX()
  , fiModInvertY()
  , fuNrOfDpbs(0)
  , fiDbpIdArray()
  , fiCrobActiveFlag()
  , fiFebModuleIdx()
  , fdFebAdcGain()
  , fdFebAdcBase()
  , fdFebAdcThrGain()
  , fiFebAdcThrOffs()
{
  detName = "Hodo";
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmCosy2019HodoPar::~CbmCosy2019HodoPar() {}
// -------------------------------------------------------------------------


// -----   Public method clear   -------------------------------------------
void CbmCosy2019HodoPar::clear()
{
  status = kFALSE;
  resetInputVersions();
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------

void CbmCosy2019HodoPar::putParams(FairParamList* l)
{
  if (!l) return;

  l->add("NbModules", fuNbModules);
  l->add("ModAddress", fiModAddress);
  l->add("ModCenterPosX", fdModCenterPosX);
  l->add("ModCenterPosY", fdModCenterPosY);
  l->add("ModSwapXY", fiModSwapXY);
  l->add("ModInvertX", fiModInvertX);
  l->add("ModInvertY", fiModInvertY);

  l->add("NrOfDpbs", fuNrOfDpbs);
  l->add("DbpIdArray", fiDbpIdArray);
  l->add("CrobActiveFlag", fiCrobActiveFlag);

  l->add("FebModuleIdx", fiFebModuleIdx);
  l->add("FebAdcGain", fdFebAdcGain);
  l->add("FebAdcBase", fdFebAdcBase);
  l->add("FebAdcThrGain", fdFebAdcThrGain);
  l->add("FebAdcThrOffs", fiFebAdcThrOffs);
}

// -------------------------------------------------------------------------

Bool_t CbmCosy2019HodoPar::getParams(FairParamList* l)
{

  if (!l) return kFALSE;

  if (!l->fill("NbModules", &fuNbModules)) return kFALSE;

  fiModAddress.Set(fuNbModules);
  fdModCenterPosX.Set(fuNbModules);
  fdModCenterPosY.Set(fuNbModules);
  fiModSwapXY.Set(fuNbModules);
  fiModInvertX.Set(fuNbModules);
  fiModInvertY.Set(fuNbModules);
  if (!l->fill("ModAddress", &fiModAddress)) return kFALSE;
  if (!l->fill("ModCenterPosX", &fdModCenterPosX)) return kFALSE;
  if (!l->fill("ModCenterPosY", &fdModCenterPosY)) return kFALSE;
  if (!l->fill("ModSwapXY", &fiModSwapXY)) return kFALSE;
  if (!l->fill("ModInvertX", &fiModInvertX)) return kFALSE;
  if (!l->fill("ModInvertY", &fiModInvertY)) return kFALSE;

  if (!l->fill("NrOfDpbs", &fuNrOfDpbs)) return kFALSE;

  fiDbpIdArray.Set(fuNrOfDpbs);
  if (!l->fill("DbpIdArray", &fiDbpIdArray)) return kFALSE;

  fiCrobActiveFlag.Set(fuNrOfDpbs * kuNbCrobsPerDpb);
  if (!l->fill("CrobActiveFlag", &fiCrobActiveFlag)) return kFALSE;

  fiFebModuleIdx.Set(fuNrOfDpbs * kuNbCrobsPerDpb * kuNbFebsPerCrob);
  fdFebAdcGain.Set(fuNrOfDpbs * kuNbCrobsPerDpb * kuNbFebsPerCrob);
  fdFebAdcBase.Set(fuNrOfDpbs * kuNbCrobsPerDpb * kuNbFebsPerCrob);
  fdFebAdcThrGain.Set(fuNrOfDpbs * kuNbCrobsPerDpb * kuNbFebsPerCrob);
  fiFebAdcThrOffs.Set(fuNrOfDpbs * kuNbCrobsPerDpb * kuNbFebsPerCrob);
  if (!l->fill("FebModuleIdx", &fiFebModuleIdx)) return kFALSE;
  if (!l->fill("FebAdcGain", &fdFebAdcGain)) return kFALSE;
  if (!l->fill("FebAdcBase", &fdFebAdcBase)) return kFALSE;
  if (!l->fill("FebAdcThrGain", &fdFebAdcThrGain)) return kFALSE;
  if (!l->fill("FebAdcThrOffs", &fiFebAdcThrOffs)) return kFALSE;

  return kTRUE;
}
// -------------------------------------------------------------------------
Int_t CbmCosy2019HodoPar::ElinkIdxToFebIdx(UInt_t uElink)
{
  //LOG(info) <<" uElink "<<uElink<<" kuNbElinksPerCrob "<<kuNbElinksPerCrob;
  if (uElink < kuNbElinksPerCrob) return kiCrobMapElinkFebIdx[uElink];
  else {
    LOG(warning) << "CbmCosy2019HodoPar::ElinkIdxToFebIdx => Index out of bound, "
                 << "returning crazy value!";
    return -1;
  }  // else of if( uElink < kuNbElinksPerCrob )
}
// -------------------------------------------------------------------------
UInt_t CbmCosy2019HodoPar::ElinkIdxToAsicIdxFebMuch(UInt_t uElink)
{
  if (uElink < kuNbElinksPerCrob) return kuCrobMapElinkFebMuch[uElink];
  else {
    LOG(warning) << "CbmCosy2019HodoPar::ElinkIdxToAsicIdxFebMuch => Index out of bound, "
                 << "returning crazy value!";
    return 0xFFFF;
  }  // else of if( uElink < kuNbElinksPerCrob )
}
// -------------------------------------------------------------------------
UInt_t CbmCosy2019HodoPar::ChannelToFiber(UInt_t uChan)
{
  if (uChan < kuNbChanPerAsic) return kuChannelToFiberMap[uChan];
  else {
    LOG(warning) << "CbmCosy2019HodoPar::ChannelToFiber => Index out of bound, "
                 << "returning crazy value!";
    return 0xFFFF;
  }  // else of if( uChan < kuNbChanPerAsic )
}
UInt_t CbmCosy2019HodoPar::ChannelToPixel(UInt_t uChan)
{
  if (uChan < kuNbChanPerAsic) return kuChannelToPixelMap[uChan];
  else {
    LOG(warning) << "CbmCosy2019HodoPar::ChannelToPixel => Index out of bound, "
                 << "returning crazy value!";
    return 0xFFFF;
  }  // else of if( uChan < kuNbChanPerAsic )
}
UInt_t CbmCosy2019HodoPar::ChannelToAxis(UInt_t uChan)
{
  if (uChan < kuNbChanPerAsic) return kuChannelToPlaneMap[uChan];
  else {
    LOG(warning) << "CbmCosy2019HodoPar::ChannelToAxis => Index out of bound, "
                 << "returning crazy value!";
    return 0xFFFF;
  }  // else of if( uChan < kuNbChanPerAsic )
}
// -------------------------------------------------------------------------
Bool_t CbmCosy2019HodoPar::CheckModuleIndex(UInt_t uModuleIdx)
{
  if (uModuleIdx < fuNbModules) return kTRUE;
  else {
    LOG(warning) << "CbmCosy2019HodoPar::CheckModuleIndex => Index out of bound!";
    return kFALSE;
  }  // else of if( uModuleIdx < fuNbModules )
}
UInt_t CbmCosy2019HodoPar::GetModuleAddress(UInt_t uModuleIdx)
{
  if (uModuleIdx < fuNbModules) return fiModAddress[uModuleIdx];
  else {
    LOG(warning) << "CbmCosy2019HodoPar::GetModuleAddress => Index out of bound, "
                 << "returning crazy value!";
    return 0xFFFFFFFF;
  }  // else of if( uModuleIdx < fuNbModules )
}
Double_t CbmCosy2019HodoPar::GetModuleCenterPosX(UInt_t uModuleIdx)
{
  if (uModuleIdx < fuNbModules) return fdModCenterPosX[uModuleIdx];
  else {
    LOG(warning) << "CbmCosy2019HodoPar::GetModuleCenterPosX => Index out of bound, "
                 << "returning crazy value!";
    return 3.844e11;  // Fly to the Moon!
  }                   // else of if( uModuleIdx < fuNbModules )
}
Double_t CbmCosy2019HodoPar::GetModuleCenterPosY(UInt_t uModuleIdx)
{
  if (uModuleIdx < fuNbModules) return fdModCenterPosY[uModuleIdx];
  else {
    LOG(warning) << "CbmCosy2019HodoPar::GetModuleCenterPosY => Index out of bound, "
                 << "returning crazy value!";
    return 3.844e11;  // Fly to the Moon!
  }                   // else of if( uModuleIdx < fuNbModules )
}
Bool_t CbmCosy2019HodoPar::GetModuleSwapXY(UInt_t uModuleIdx)
{
  if (uModuleIdx < fuNbModules) return fiModSwapXY[uModuleIdx];
  else {
    LOG(warning) << "CbmCosy2019HodoPar::GetModuleSwapXY => Index out of bound, "
                 << "returning false!";
    return kFALSE;  // Fly to the Moon!
  }                 // else of if( uModuleIdx < fuNbModules )
}
Bool_t CbmCosy2019HodoPar::GetModuleInvertX(UInt_t uModuleIdx)
{
  if (uModuleIdx < fuNbModules) return fiModInvertX[uModuleIdx];
  else {
    LOG(warning) << "CbmCosy2019HodoPar::GetModuleInvertX => Index out of bound, "
                 << "returning false!";
    return kFALSE;  // Fly to the Moon!
  }                 // else of if( uModuleIdx < fuNbModules )
}
Bool_t CbmCosy2019HodoPar::GetModuleInvertY(UInt_t uModuleIdx)
{
  if (uModuleIdx < fuNbModules) return fiModInvertY[uModuleIdx];
  else {
    LOG(warning) << "CbmCosy2019HodoPar::GetModuleInvertY => Index out of bound, "
                 << "returning false!";
    return kFALSE;  // Fly to the Moon!
  }                 // else of if( uModuleIdx < fuNbModules )
}
// -------------------------------------------------------------------------
UInt_t CbmCosy2019HodoPar::GetDpbId(UInt_t uDpbIdx)
{
  if (uDpbIdx < fuNrOfDpbs) return fiDbpIdArray[uDpbIdx];
  else {
    LOG(warning) << "CbmCosy2019HodoPar::GetDpbId => Index out of bound, "
                 << "returning crazy value!";
    return 0xFFFFFFFF;
  }  // else of if( uDpbIdx < fuNrOfDpbs )
}
Bool_t CbmCosy2019HodoPar::IsCrobActive(UInt_t uDpbIdx, UInt_t uCrobIdx)
{
  if (uDpbIdx < fuNrOfDpbs) {
    if (uCrobIdx < kuNbCrobsPerDpb) return 0 < fiCrobActiveFlag[uDpbIdx * kuNbCrobsPerDpb + uCrobIdx] ? kTRUE : kFALSE;
    else {
      LOG(warning) << "CbmCosy2019HodoPar::IsCrobActive => Crob Index out of bound, "
                   << "returning default inactive!";
      return kFALSE;
    }  // else of if( uCrobIdx < kuNbCrobsPerDpb )
  }    // if( uDpbIdx < fuNrOfDpbs )
  else {
    LOG(warning) << "CbmCosy2019HodoPar::IsCrobActive => Dpb Index out of bound, "
                 << "returning default inactive!";
    return kFALSE;
  }  // else of if( uDpbIdx < fuNrOfDpbs )
}
Bool_t CbmCosy2019HodoPar::IsFebActive(UInt_t uFebInSystIdx)
{

  if (uFebInSystIdx < GetNrOfFebs()) {
    return (-1 == fiFebModuleIdx[uFebInSystIdx] ? kFALSE : kTRUE);
  }  // if( uFebInSystIdx < GetNrOfFebs() )
  else {
    LOG(warning) << "CbmCosy2019HodoPar::IsFebActive => Feb Index out of bound, "
                 << "returning default inactive!";
    return kFALSE;
  }  // else of if( uFebInSystIdx < GetNrOfFebs() )
}
Bool_t CbmCosy2019HodoPar::IsFebActive(UInt_t uDpbIdx, UInt_t uCrobIdx, UInt_t uFebIdx)
{
  if (uDpbIdx < fuNrOfDpbs) {
    if (uCrobIdx < kuNbCrobsPerDpb) {
      if (uFebIdx < kuNbFebsPerCrob) {
        UInt_t uIdx = (uDpbIdx * kuNbCrobsPerDpb + uCrobIdx) * kuNbFebsPerCrob + uFebIdx;
        return IsFebActive(uIdx);
      }  // if( uFebIdx < kuNbFebsPerCrob )
      else {
        LOG(warning) << "CbmCosy2019HodoPar::IsFebActive => Feb Index out of bound, "
                     << "returning default inactive!";
        return kFALSE;
      }  // else of if( uFebIdx < kuNbCrobsPerDpb )
    }    // if( uCrobIdx < kuNbCrobsPerDpb )
    else {
      LOG(warning) << "CbmCosy2019HodoPar::IsFebActive => Crob Index out of bound, "
                   << "returning default inactive!";
      return kFALSE;
    }  // else of if( uCrobIdx < kuNbCrobsPerDpb )
  }    // if( uDpbIdx < fuNrOfDpbs )
  else {
    LOG(warning) << "CbmCosy2019HodoPar::IsFebActive => Dpb Index out of bound, "
                 << "returning default inactive!";
    return kFALSE;
  }  // else of if( uDpbIdx < fuNrOfDpbs )
}
Int_t CbmCosy2019HodoPar::GetFebModuleIdx(UInt_t uDpbIdx, UInt_t uCrobIdx, UInt_t uFebIdx)
{
  if (uDpbIdx < fuNrOfDpbs) {
    if (uCrobIdx < kuNbCrobsPerDpb) {
      if (uFebIdx < kuNbFebsPerCrob) {
        UInt_t uIdx = (uDpbIdx * kuNbCrobsPerDpb + uCrobIdx) * kuNbFebsPerCrob + uFebIdx;
        return fiFebModuleIdx[uIdx];
      }  // if( uFebIdx < kuNbFebsPerCrob )
      else {
        LOG(warning) << "CbmCosy2019HodoPar::GetFebModuleIdx => Feb Index out of bound, "
                     << "returning default inactive!";
        return -1;
      }  // else of if( uCrobIdx < kuNbCrobsPerDpb )
    }    // if( uCrobIdx < kuNbCrobsPerDpb )
    else {
      LOG(warning) << "CbmCosy2019HodoPar::GetFebModuleIdx => Crob Index out of bound, "
                   << "returning default inactive!";
      return -1;
    }  // else of if( uCrobIdx < kuNbCrobsPerDpb )
  }    // if( uDpbIdx < fuNrOfDpbs )
  else {
    LOG(warning) << "CbmCosy2019HodoPar::GetFebModuleIdx => Dpb Index out of bound, "
                 << "returning default inactive!";
    return -1;
  }  // else of if( uDpbIdx < fuNrOfDpbs )
}
Double_t CbmCosy2019HodoPar::GetFebAdcGain(UInt_t uDpbIdx, UInt_t uCrobIdx, UInt_t uFebIdx)
{
  if (uDpbIdx < fuNrOfDpbs) {
    if (uCrobIdx < kuNbCrobsPerDpb) {
      if (uFebIdx < kuNbFebsPerCrob) {
        UInt_t uIdx = (uDpbIdx * kuNbCrobsPerDpb + uCrobIdx) * kuNbFebsPerCrob + uFebIdx;
        return fdFebAdcGain[uIdx];
      }  // if( uFebIdx < kuNbFebsPerCrob )
      else {
        LOG(warning) << "CbmCosy2019HodoPar::GetFebAdcGain => Feb Index out of bound, "
                     << "returning default value!";
        return 0.0;
      }  // else of if( uCrobIdx < kuNbCrobsPerDpb )
    }    // if( uCrobIdx < kuNbCrobsPerDpb )
    else {
      LOG(warning) << "CbmCosy2019HodoPar::GetFebAdcGain => Crob Index out of bound, "
                   << "returning default value!";
      return 0.0;
    }  // else of if( uCrobIdx < kuNbCrobsPerDpb )
  }    // if( uDpbIdx < fuNrOfDpbs )
  else {
    LOG(warning) << "CbmCosy2019HodoPar::GetFebAdcGain => Dpb Index out of bound, "
                 << "returning default value!";
    return 0.0;
  }  // else of if( uDpbIdx < fuNrOfDpbs )
}
Double_t CbmCosy2019HodoPar::GetFebAdcOffset(UInt_t uDpbIdx, UInt_t uCrobIdx, UInt_t uFebIdx)
{
  if (uDpbIdx < fuNrOfDpbs) {
    if (uCrobIdx < kuNbCrobsPerDpb) {
      if (uFebIdx < kuNbFebsPerCrob) {
        UInt_t uIdx = (uDpbIdx * kuNbCrobsPerDpb + uCrobIdx) * kuNbFebsPerCrob + uFebIdx;
        return (fdFebAdcBase[uIdx] + fdFebAdcThrGain[uIdx] * fiFebAdcThrOffs[uIdx]);
      }  // if( uFebIdx < kuNbFebsPerCrob )
      else {
        LOG(warning) << "CbmCosy2019HodoPar::GetFebAdcOffset => Feb Index out of bound, "
                     << "returning default value!";
        return 0.0;
      }  // else of if( uCrobIdx < kuNbCrobsPerDpb )
    }    // if( uCrobIdx < kuNbCrobsPerDpb )
    else {
      LOG(warning) << "CbmCosy2019HodoPar::GetFebAdcOffset => Crob Index out of bound, "
                   << "returning default value!";
      return 0.0;
    }  // else of if( uCrobIdx < kuNbCrobsPerDpb )
  }    // if( uDpbIdx < fuNrOfDpbs )
  else {
    LOG(warning) << "CbmCosy2019HodoPar::GetFebAdcOffset => Dpb Index out of bound, "
                 << "returning default value!";
    return 0.0;
  }  // else of if( uDpbIdx < fuNrOfDpbs )
}
Double_t CbmCosy2019HodoPar::GetFebAdcBase(UInt_t uDpbIdx, UInt_t uCrobIdx, UInt_t uFebIdx)
{
  if (uDpbIdx < fuNrOfDpbs) {
    if (uCrobIdx < kuNbCrobsPerDpb) {
      if (uFebIdx < kuNbFebsPerCrob) {
        UInt_t uIdx = (uDpbIdx * kuNbCrobsPerDpb + uCrobIdx) * kuNbFebsPerCrob + uFebIdx;
        return fdFebAdcBase[uIdx];
      }  // if( uFebIdx < kuNbFebsPerCrob )
      else {
        LOG(warning) << "CbmCosy2019HodoPar::GetFebAdcBase => Feb Index out of bound, "
                     << "returning default value!";
        return 0.0;
      }  // else of if( uCrobIdx < kuNbCrobsPerDpb )
    }    // if( uCrobIdx < kuNbCrobsPerDpb )
    else {
      LOG(warning) << "CbmCosy2019HodoPar::GetFebAdcBase => Crob Index out of bound, "
                   << "returning default value!";
      return 0.0;
    }  // else of if( uCrobIdx < kuNbCrobsPerDpb )
  }    // if( uDpbIdx < fuNrOfDpbs )
  else {
    LOG(warning) << "CbmCosy2019HodoPar::GetFebAdcBase => Dpb Index out of bound, "
                 << "returning default value!";
    return 0.0;
  }  // else of if( uDpbIdx < fuNrOfDpbs )
}
Double_t CbmCosy2019HodoPar::GetFebAdcThrGain(UInt_t uDpbIdx, UInt_t uCrobIdx, UInt_t uFebIdx)
{
  if (uDpbIdx < fuNrOfDpbs) {
    if (uCrobIdx < kuNbCrobsPerDpb) {
      if (uFebIdx < kuNbFebsPerCrob) {
        UInt_t uIdx = (uDpbIdx * kuNbCrobsPerDpb + uCrobIdx) * kuNbFebsPerCrob + uFebIdx;
        return fdFebAdcThrGain[uIdx];
      }  // if( uFebIdx < kuNbFebsPerCrob )
      else {
        LOG(warning) << "CbmCosy2019HodoPar::GetFebAdcThrGain => Feb Index out of bound, "
                     << "returning default value!";
        return 0.0;
      }  // else of if( uCrobIdx < kuNbCrobsPerDpb )
    }    // if( uCrobIdx < kuNbCrobsPerDpb )
    else {
      LOG(warning) << "CbmCosy2019HodoPar::GetFebAdcThrGain => Crob Index out of bound, "
                   << "returning default value!";
      return 0.0;
    }  // else of if( uCrobIdx < kuNbCrobsPerDpb )
  }    // if( uDpbIdx < fuNrOfDpbs )
  else {
    LOG(warning) << "CbmCosy2019HodoPar::GetFebAdcThrGain => Dpb Index out of bound, "
                 << "returning default value!";
    return 0.0;
  }  // else of if( uDpbIdx < fuNrOfDpbs )
}
Int_t CbmCosy2019HodoPar::GetFebAdcThrOffs(UInt_t uDpbIdx, UInt_t uCrobIdx, UInt_t uFebIdx)
{
  if (uDpbIdx < fuNrOfDpbs) {
    if (uCrobIdx < kuNbCrobsPerDpb) {
      if (uFebIdx < kuNbFebsPerCrob) {
        UInt_t uIdx = (uDpbIdx * kuNbCrobsPerDpb + uCrobIdx) * kuNbFebsPerCrob + uFebIdx;
        return fiFebAdcThrOffs[uIdx];
      }  // if( uFebIdx < kuNbFebsPerCrob )
      else {
        LOG(warning) << "CbmCosy2019HodoPar::GetFebAdcThrOffs => Feb Index out of bound, "
                     << "returning default value!";
        return 0;
      }  // else of if( uCrobIdx < kuNbCrobsPerDpb )
    }    // if( uCrobIdx < kuNbCrobsPerDpb )
    else {
      LOG(warning) << "CbmCosy2019HodoPar::GetFebAdcThrOffs => Crob Index out of bound, "
                   << "returning default value!";
      return 0;
    }  // else of if( uCrobIdx < kuNbCrobsPerDpb )
  }    // if( uDpbIdx < fuNrOfDpbs )
  else {
    LOG(warning) << "CbmCosy2019HodoPar::GetFebAdcThrOffs => Dpb Index out of bound, "
                 << "returning default value!";
    return 0;
  }  // else of if( uDpbIdx < fuNrOfDpbs )
}
// -------------------------------------------------------------------------
UInt_t CbmCosy2019HodoPar::GetChannelInModule(UInt_t uModuleIdx, UInt_t uChan)
{
  if (uModuleIdx < fuNbModules || uChan < kuNbChanPerAsic) {
    UInt_t uAxis       = ChannelToAxis(uChan);
    UInt_t uChanInAxis = ChannelToFiber(uChan);

    /// Check for Axis inversion
    if (uAxis ? GetModuleInvertY(uModuleIdx) : GetModuleInvertX(uModuleIdx)) {
      uChanInAxis = kuNbFiberPerAxis - uChanInAxis - 1;
    }  // if( uAxis ? GetModuleInvertY( uModuleIdx ) : GetModuleInvertX( uModuleIdx ) )

    /// Check for axis swap
    if (GetModuleSwapXY(uModuleIdx)) { uAxis = !uAxis; }  // if( GetModuleSwapXY( uModuleIdx ) )

    return uChanInAxis + uAxis * kuNbFiberPerAxis;
  }  // if( uModuleIdx < fuNbModules || uChan < kuNbChanPerAsic )
  else {
    LOG(warning) << "CbmCosy2019HodoPar::GetChannelInModule => Index out of bound, "
                 << "returning crazy value!";
    return 0xFFFF;
  }  // else of if( uModuleIdx < fuNbModules || uChan < kuNbChanPerAsic )
}
// -------------------------------------------------------------------------

ClassImp(CbmCosy2019HodoPar)
