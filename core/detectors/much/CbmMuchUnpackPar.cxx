/* Copyright (C) 2017-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau, Ajit Kumar, Florian Uhlig [committer] */

#include "CbmMuchUnpackPar.h"

#include "FairDetParIo.h"
#include "FairParIo.h"
#include "FairParamList.h"
#include <Logger.h>

#include "TMath.h"
#include "TString.h"

using namespace std;

// -----   Standard constructor   ------------------------------------------
CbmMuchUnpackPar::CbmMuchUnpackPar(const char* name, const char* title, const char* context)
  : FairParGenericSet(name, title, context)
  , fuNrOfDpbs(0)
  , fiDbpIdArray()
  , fiCrobActiveFlag()
  , fuFebsInGemA(0)
  , fuFebsInGemB(0)
  , fuFebsInRpc(0)
  , fnFebsIdsArrayGemA()
  , fnFebsIdsArrayGemB()
  , fnFebsIdsArrayRpc()
  , fChannelsToPadXA()
  , fChannelsToPadYA()
  , fChannelsToPadXB()
  , fChannelsToPadYB()
  , fChannelsToPadXRpc()
  , fChannelsToPadYRpc()
  , fRealX()
  , fRealPadSize()
{
  detName = "Much";
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmMuchUnpackPar::~CbmMuchUnpackPar() {}
// -------------------------------------------------------------------------


// -----   Public method clear   -------------------------------------------
void CbmMuchUnpackPar::clear()
{
  status = kFALSE;
  resetInputVersions();
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------

void CbmMuchUnpackPar::putParams(FairParamList* l)
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

Bool_t CbmMuchUnpackPar::getParams(FairParamList* l)
{

  if (!l) return kFALSE;

  if (!l->fill("NrOfDpbs", &fuNrOfDpbs)) return kFALSE;

  fiDbpIdArray.Set(fuNrOfDpbs);
  if (!l->fill("DbpIdArray", &fiDbpIdArray)) return kFALSE;

  fiCrobActiveFlag.Set(fuNrOfDpbs * kuNbCrobsPerDpb);
  if (!l->fill("CrobActiveFlag", &fiCrobActiveFlag)) return kFALSE;

  if (!l->fill("NrOfFebsInGemA", &fuFebsInGemA)) return kFALSE;

  fnFebsIdsArrayGemA.Set(GetNrOfFebsInGemA());
  if (!l->fill("nFebsIdsArrayA", &fnFebsIdsArrayGemA)) return kFALSE;

  if (!l->fill("NrOfFebsInGemB", &fuFebsInGemB)) return kFALSE;

  fnFebsIdsArrayGemB.Set(GetNrOfFebsInGemB());
  if (!l->fill("nFebsIdsArrayB", &fnFebsIdsArrayGemB)) return kFALSE;

  if (!l->fill("NrOfFebsInRpc", &fuFebsInRpc)) return kFALSE;

  fnFebsIdsArrayRpc.Set(GetNrOfFebsInRpc());
  if (!l->fill("nFebsIdsArrayRpc", &fnFebsIdsArrayRpc)) return kFALSE;

  fChannelsToPadXA.Set(GetNrOfFebs() * kuNbChanPerAsic);
  if (!l->fill("ChannelsToPadXA", &fChannelsToPadXA)) return kFALSE;

  fChannelsToPadYA.Set(GetNrOfFebs() * kuNbChanPerAsic);
  if (!l->fill("ChannelsToPadYA", &fChannelsToPadYA)) return kFALSE;

  fChannelsToPadXB.Set(GetNrOfFebs() * kuNbChanPerAsic);
  if (!l->fill("ChannelsToPadXB", &fChannelsToPadXB)) return kFALSE;

  fChannelsToPadYB.Set(GetNrOfFebs() * kuNbChanPerAsic);
  if (!l->fill("ChannelsToPadYB", &fChannelsToPadYB)) return kFALSE;

  fChannelsToPadXRpc.Set(GetNrOfFebsInRpc() * kuNbChanPerAsic);
  if (!l->fill("ChannelsToPadXRpc", &fChannelsToPadXRpc)) return kFALSE;

  fChannelsToPadYRpc.Set(GetNrOfFebsInRpc() * kuNbChanPerAsic);
  if (!l->fill("ChannelsToPadYRpc", &fChannelsToPadYRpc)) return kFALSE;

  fRealX.Set(2232);  // Number of Sectors in one GEM Module
  if (!l->fill("RealX", &fRealX)) return kFALSE;

  fRealPadSize.Set(2232);  // Number of Sectors in one GEM Module
  if (!l->fill("PadSize", &fRealPadSize)) return kFALSE;

  return kTRUE;
}
// -------------------------------------------------------------------------
Int_t CbmMuchUnpackPar::ElinkIdxToFebIdx(UInt_t uElink)
{
  //LOG(info) <<" uElink "<<uElink<<" kuNbElinksPerCrob "<<kuNbElinksPerCrob;
  if (uElink < kuNbElinksPerCrob) return kiCrobMapElinkFebIdx[uElink];
  else {
    LOG(warning) << "CbmMuchUnpackPar::ElinkIdxToFebIdx => Index out of bound, "
                 << "Elink is " << uElink << " returning crazy value!";
    return -1;
  }  // else of if( uElink < kuNbElinksPerCrob )
}
// -------------------------------------------------------------------------
/*
UInt_t CbmMuchUnpackPar::ElinkIdxToAsicIdxFebMuch(UInt_t uElink)
{
  if (uElink < kuNbElinksPerCrob) return kuCrobMapElinkFebMuch[uElink];
  else {
    LOG(warning) << "CbmMuchUnpackPar::ElinkIdxToAsicIdxFebMuch => Index out of bound, "
                 << "returning crazy value!";
    return 0xFFFF;
  }  // else of if( uElink < kuNbElinksPerCrob )
}
*/
// -------------------------------------------------------------------------
UInt_t CbmMuchUnpackPar::GetDpbId(UInt_t uDpbIdx)
{
  if (uDpbIdx < fuNrOfDpbs) return fiDbpIdArray[uDpbIdx];
  else {
    LOG(warning) << "CbmMuchUnpackPar::GetDpbId => Index out of bound, "
                 << "DPB Id is " << std::hex << uDpbIdx << " returning crazy value!";
    return 0xFFFFFFFF;
  }  // else of if( uDpbIdx < fuNrOfDpbs )
}
Bool_t CbmMuchUnpackPar::IsCrobActive(UInt_t uDpbIdx, UInt_t uCrobIdx)
{
  if (uDpbIdx < fuNrOfDpbs) {
    if (uCrobIdx < kuNbCrobsPerDpb) return 0 < fiCrobActiveFlag[uDpbIdx * kuNbCrobsPerDpb + uCrobIdx] ? kTRUE : kFALSE;
    else {
      LOG(warning) << "CbmMuchUnpackPar::IsCrobActive => Crob Index out of bound, "
                   << "returning default inactive!";
      return kFALSE;
    }  // else of if( uCrobIdx < kuNbCrobsPerDpb )
  }    // if( uDpbIdx < fuNrOfDpbs )
  else {
    LOG(warning) << "CbmMuchUnpackPar::IsCrobActive => Dpb Index out of bound, "
                 << "returning default inactive!";
    return kFALSE;
  }  // else of if( uDpbIdx < fuNrOfDpbs )
}
Bool_t CbmMuchUnpackPar::IsFebActive(UInt_t uFebInSystIdx)
{

  if (uFebInSystIdx < GetNrOfFebs()) {
    /// Always return true for now
    return kTRUE;
  }  // if( uFebInSystIdx < GetNrOfFebs() )
  else {
    LOG(warning) << "CbmMuchUnpackPar::IsFebActive => Feb Index out of bound, "
                 << "returning default inactive!";
    return kFALSE;
  }  // else of if( uFebInSystIdx < GetNrOfFebs() )
}
Bool_t CbmMuchUnpackPar::IsFebActive(UInt_t uDpbIdx, UInt_t uCrobIdx, UInt_t uFebIdx)
{
  if (uDpbIdx < fuNrOfDpbs) {
    if (uCrobIdx < kuNbCrobsPerDpb) {
      if (uFebIdx < kuNbFebsPerCrob) {
        UInt_t uIdx = (uDpbIdx * kuNbCrobsPerDpb + uCrobIdx) * kuNbFebsPerCrob + uFebIdx;
        return IsFebActive(uIdx);
      }  // if( uFebIdx < kuNbFebsPerCrob )
      else {
        LOG(warning) << "CbmMuchUnpackPar::IsFebActive => Feb Index out of bound, "
                     << "returning default inactive!";
        return kFALSE;
      }  // else of if( uFebIdx < kuNbCrobsPerDpb )
    }    // if( uCrobIdx < kuNbCrobsPerDpb )
    else {
      LOG(warning) << "CbmMuchUnpackPar::IsFebActive => Crob Index out of bound, "
                   << "returning default inactive!";
      return kFALSE;
    }  // else of if( uCrobIdx < kuNbCrobsPerDpb )
  }    // if( uDpbIdx < fuNrOfDpbs )
  else {
    LOG(warning) << "CbmMuchUnpackPar::IsFebActive => Dpb Index out of bound, "
                 << "returning default inactive!";
    return kFALSE;
  }  // else of if( uDpbIdx < fuNrOfDpbs )
}

//-----------------------Taken for new unpacking algo -----------------

/*Bool_t CbmMuchUnpackPar::IsFebPulser(UInt_t uFebInSystIdx)
{
  if (uFebInSystIdx < GetNrOfFebs()) {
    return (fiFebPulserFlag[uFebInSystIdx] ? kTRUE : kFALSE);
  }  // if( uFebInSystIdx < GetNrOfFebs() )
  else {
    LOG(warning) << "CbmMcbm2018StsPar::IsFebPulser => Feb Index out of bound, "
                 << "returning default standard FEB!";
    return kFALSE;
  }  // else of if( uFebInSystIdx < GetNrOfFebs() )
}
Bool_t CbmMuchUnpackPar::IsFebPulser(UInt_t uDpbIdx, UInt_t uCrobIdx, UInt_t uFebIdx)
{
  if (uDpbIdx < fuNrOfDpbs) {
    if (uCrobIdx < kuNbCrobsPerDpb) {
      if (uFebIdx < kuNbFebsPerCrob) {
        UInt_t uIdx = (uDpbIdx * kuNbCrobsPerDpb + uCrobIdx) * kuNbFebsPerCrob + uFebIdx;
        return IsFebPulser(uIdx);
      }  // if( uFebIdx < kuNbFebsPerCrob )
      else {
        LOG(warning) << "CbmMcbm2018StsPar::IsFebPulser => Feb Index out of bound, "
                     << "returning default standard FEB!";
        return kFALSE;
      }  // else of if( uFebIdx < kuNbCrobsPerDpb )
    }    // if( uCrobIdx < kuNbCrobsPerDpb )
    else {
      LOG(warning) << "CbmMcbm2018StsPar::IsFebPulser => Crob Index out of bound, "
                   << "returning default standard FEB!";
      return kFALSE;
    }  // else of if( uCrobIdx < kuNbCrobsPerDpb )
  }    // if( uDpbIdx < fuNrOfDpbs )
  else {
    LOG(warning) << "CbmMcbm2018StsPar::IsFebPulser => Dpb Index out of bound, "
                 << "returning default standard FEB!";
    return kFALSE;
  }  // else of if( uDpbIdx < fuNrOfDpbs )
}

Double_t CbmMuchUnpackPar::GetFebAdcGain(UInt_t uDpbIdx, UInt_t uCrobIdx, UInt_t uFebIdx)
{
  if (uDpbIdx < fuNrOfDpbs) {
    if (uCrobIdx < kuNbCrobsPerDpb) {
      if (uFebIdx < kuNbFebsPerCrob) {
        UInt_t uIdx = (uDpbIdx * kuNbCrobsPerDpb + uCrobIdx) * kuNbFebsPerCrob + uFebIdx;
        return fdFebAdcGain[uIdx];
      }  // if( uFebIdx < kuNbFebsPerCrob )
      else {
        LOG(warning) << "CbmMcbm2018StsPar::GetFebAdcGain => Feb Index out of bound, "
                     << "returning default value!";
        return 0.0;
      }  // else of if( uCrobIdx < kuNbCrobsPerDpb )
    }    // if( uCrobIdx < kuNbCrobsPerDpb )
    else {
      LOG(warning) << "CbmMcbm2018StsPar::GetFebAdcGain => Crob Index out of bound, "
                   << "returning default value!";
      return 0.0;
    }  // else of if( uCrobIdx < kuNbCrobsPerDpb )
  }    // if( uDpbIdx < fuNrOfDpbs )
  else {
    LOG(warning) << "CbmMcbm2018StsPar::GetFebAdcGain => Dpb Index out of bound, "
                 << "returning default value!";
    return 0.0;
  }  // else of if( uDpbIdx < fuNrOfDpbs )
}

Double_t CbmMuchUnpackPar::GetFebAdcOffset(UInt_t uDpbIdx, UInt_t uCrobIdx, UInt_t uFebIdx)
{
  if (uDpbIdx < fuNrOfDpbs) {
    if (uCrobIdx < kuNbCrobsPerDpb) {
      if (uFebIdx < kuNbFebsPerCrob) {
        UInt_t uIdx = (uDpbIdx * kuNbCrobsPerDpb + uCrobIdx) * kuNbFebsPerCrob + uFebIdx;
        return (fdFebAdcBase[uIdx] + fdFebAdcThrGain[uIdx] * fiFebAdcThrOffs[uIdx]);
      }  // if( uFebIdx < kuNbFebsPerCrob )
      else {
        LOG(warning) << "CbmMcbm2018StsPar::GetFebAdcOffset => Feb Index out of bound, "
                     << "returning default value!";
        return 0.0;
      }  // else of if( uCrobIdx < kuNbCrobsPerDpb )
    }    // if( uCrobIdx < kuNbCrobsPerDpb )
    else {
      LOG(warning) << "CbmMcbm2018StsPar::GetFebAdcOffset => Crob Index out of bound, "
                   << "returning default value!";
      return 0.0;
    }  // else of if( uCrobIdx < kuNbCrobsPerDpb )
  }    // if( uDpbIdx < fuNrOfDpbs )
  else {
    LOG(warning) << "CbmMcbm2018StsPar::GetFebAdcOffset => Dpb Index out of bound, "
                 << "returning default value!";
    return 0.0;
  }  // else of if( uDpbIdx < fuNrOfDpbs )
}
*/

//---------------------------------------------------------

Short_t CbmMuchUnpackPar::GetPadXA(UShort_t febid, UShort_t channelid)
{
  if (fChannelsToPadXA.GetSize() <= static_cast<Int_t>((febid * kuNbChanPerAsic) + channelid)) {
    LOG(debug) << "CbmMuchUnpackPar::GetPadXA => Index out of bounds: " << ((febid * kuNbChanPerAsic) + channelid)
               << " VS " << fChannelsToPadXA.GetSize() << " (" << febid << " and " << channelid << ")";
    return -2;
  }  // if( fChannelsToPadXA.GetSize () <= static_cast< Int_t >( (febid*kuNbChanPerAsic)+channelid ) )


  return fChannelsToPadXA[(febid * kuNbChanPerAsic) + channelid];
}
Short_t CbmMuchUnpackPar::GetPadYA(UShort_t febid, UShort_t channelid)
{
  if (fChannelsToPadYA.GetSize() <= static_cast<Int_t>((febid * kuNbChanPerAsic) + channelid)) {
    LOG(debug) << "CbmMuchUnpackPar::GetPadYA => Index out of bounds: " << ((febid * kuNbChanPerAsic) + channelid)
               << " VS " << fChannelsToPadYA.GetSize() << " (" << febid << " and " << channelid << ")";
    return -2;
  }  // if( fChannelsToPadXA.GetSize () <= static_cast< Int_t >( (febid*kuNbChanPerAsic)+channelid ) )

  return fChannelsToPadYA[(febid * kuNbChanPerAsic) + channelid];
}

Short_t CbmMuchUnpackPar::GetPadXB(UShort_t febid, UShort_t channelid)
{
  if (fChannelsToPadXB.GetSize() <= static_cast<Int_t>((febid * kuNbChanPerAsic) + channelid)) {
    LOG(debug) << "CbmMuchUnpackPar::GetPadXB => Index out of bounds: " << ((febid * kuNbChanPerAsic) + channelid)
               << " VS " << fChannelsToPadXB.GetSize() << " (" << febid << " and " << channelid << ")";
    return -2;
  }  // if( fChannelsToPadXB.GetSize () <= static_cast< Int_t >( (febid*kuNbChanPerAsic)+channelid ) )


  return fChannelsToPadXB[(febid * kuNbChanPerAsic) + channelid];
}
Short_t CbmMuchUnpackPar::GetPadYB(UShort_t febid, UShort_t channelid)
{
  if (fChannelsToPadYB.GetSize() <= static_cast<Int_t>((febid * kuNbChanPerAsic) + channelid)) {
    LOG(debug) << "CbmMuchUnpackPar::GetPadYB => Index out of bounds: " << ((febid * kuNbChanPerAsic) + channelid)
               << " VS " << fChannelsToPadYB.GetSize() << " (" << febid << " and " << channelid << ")";
    return -2;
  }  // if( fChannelsToPadYB.GetSize () <= static_cast< Int_t >( (febid*kuNbChanPerAsic)+channelid ) )

  return fChannelsToPadYB[(febid * kuNbChanPerAsic) + channelid];
}

Short_t CbmMuchUnpackPar::GetPadXRpc(UShort_t febid, UShort_t channelid)
{
  if (fChannelsToPadXRpc.GetSize() <= static_cast<Int_t>((febid * kuNbChanPerAsic) + channelid)) {
    LOG(debug) << "CbmMcbm2018MuchPar::GetPadXRpc => Index out of bounds: " << ((febid * kuNbChanPerAsic) + channelid)
               << " VS " << fChannelsToPadXRpc.GetSize() << " (" << febid << " and " << channelid << ")";
    return -2;
  }  // if( fChannelsToPadXRpc.GetSize () <= static_cast< Int_t >( (febid*kuNbChanPerAsic)+channelid ) )


  return fChannelsToPadXRpc[(febid * kuNbChanPerAsic) + channelid];
}
Short_t CbmMuchUnpackPar::GetPadYRpc(UShort_t febid, UShort_t channelid)
{
  if (fChannelsToPadYRpc.GetSize() <= static_cast<Int_t>((febid * kuNbChanPerAsic) + channelid)) {
    LOG(debug) << "CbmMcbm2018MuchPar::GetPadYRpc => Index out of bounds: " << ((febid * kuNbChanPerAsic) + channelid)
               << " VS " << fChannelsToPadYRpc.GetSize() << " (" << febid << " and " << channelid << ")";
    return -2;
  }  // if( fChannelsToPadYRpc.GetSize () <= static_cast< Int_t >( (febid*kuNbChanPerAsic)+channelid ) )

  return fChannelsToPadYRpc[(febid * kuNbChanPerAsic) + channelid];
}


int32_t CbmMuchUnpackPar::GetFebId(UInt_t uAsicIdx)
{
  if (uAsicIdx >= GetNrOfFebsInGemA() && uAsicIdx < (GetNrOfFebsInGemA() + GetNrOfFebsInRpc()))  //Check
    return fnFebsIdsArrayRpc[uAsicIdx - GetNrOfFebsInGemA()];                                    //Check Vikas
  else if (uAsicIdx >= (GetNrOfFebsInGemA() + GetNrOfFebsInRpc())
           && uAsicIdx < (GetNrOfFebsInGemA() + GetNrOfFebsInRpc() + GetNrOfFebsInGemB()))  //Check
    return fnFebsIdsArrayGemB[uAsicIdx - (GetNrOfFebsInGemA() + GetNrOfFebsInRpc())];
  else if (uAsicIdx < GetNrOfFebsInGemA())
    return fnFebsIdsArrayGemA[uAsicIdx];
  else {
    LOG(warning) << "CbmMuchUnpackPar::GetFebId => provided uAsicIdx : " << uAsicIdx
                 << " not in the range of :" << (GetNrOfFebsInGemA() + GetNrOfFebsInRpc() + GetNrOfFebsInGemB())
                 << "Returning large value -2";
    return -2;
  }
}

//GetModule() is not used in unpacker
UInt_t CbmMuchUnpackPar::GetModule(UInt_t uAsicIdx)
{
  if (uAsicIdx >= GetNrOfFebsInGemA()) {
    //if ((uAsicIdx % GetNrOfFebsInGemA()) < GetNrOfFebsInGemB()) return 1;
    if ((uAsicIdx % GetNrOfFebsInGemA()) < GetNrOfFebsInRpc()) return 1;
    else
      return 2;
  }  // if(uAsicIdx >= GetNrOfFebsInGemA())
  else
    return 0;
}

Double_t CbmMuchUnpackPar::GetRealX(Int_t SectorIndex)
{

  //LOG(info)<<" fChannelsToPadX.GetSize() "<<fChannelsToPadX.GetSize();
  if (SectorIndex < 0 || SectorIndex <= 97) {
    LOG(debug) << "CbmMuchUnpackPar::GetRealX => Index out of bounds: ";
    return -2;
  }  // if( fChannelsToPadY.GetSize () <= (febid*kuNbChanPerAsic)+channelid )

  return fRealX[SectorIndex];
}

Double_t CbmMuchUnpackPar::GetRealPadSize(Int_t SectorIndex)
{

  //LOG(info)<<" fChannelsToPadX.GetSize() "<<fChannelsToPadX.GetSize();
  if (SectorIndex < 0 || SectorIndex <= 97) {
    LOG(debug) << "CbmMuchUnpackPar::GetRealX => Index out of bounds: ";
    return -2;
  }  // if( fChannelsToPadY.GetSize () <= (febid*kuNbChanPerAsic)+channelid )

  return fRealPadSize[SectorIndex];
}

Double_t CbmMuchUnpackPar::GetRealX(Int_t Channel, Int_t Sector)
{
  Int_t PadIndex = Channel + 97 * Sector;
  if (Channel < 0 || Sector < 0) return -2;
  if (fRealX.GetSize() <= PadIndex) {
    LOG(info) << "CbmMuchUnpackPar::GetRealX => Index out of bounds: " << Channel << " " << Sector << " " << PadIndex;
    return -1;
  }  // if( fRealX.Size() <= PadIndex )

  return fRealX[PadIndex];
}
Double_t CbmMuchUnpackPar::GetRealPadSize(Int_t Channel, Int_t Sector)
{
  Int_t PadIndex = Channel + 97 * Sector;
  if (Channel < 0 || Sector < 0) return -2;
  if (fRealPadSize.GetSize() <= PadIndex) {
    LOG(info) << "CbmMuchUnpackPar::GetRealPadSize => Index out of bounds: " << Channel << " " << Sector << " "
              << PadIndex;
    return -1;
  }  // if( fRealPadSize.Size() <= PadIndex )

  return fRealPadSize[PadIndex];
}

ClassImp(CbmMuchUnpackPar)
