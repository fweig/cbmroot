/* Copyright (C) 2009-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#include "CbmTrdDigi.h"

#include "CbmTrdAddress.h"  // for CbmTrdAddress

#include <Logger.h>  // for LOG

#include <TMath.h>  // for Ceil

#include <iomanip>  // for operator<<, setprecision, setw
#include <sstream>  // for operator<<, basic_ostream, stringstream
#include <string>   // for char_traits

using std::endl;
using std::string;
using std::stringstream;

/**
 * fInfo defition ATTf.ffnn nLLL.LMMM MMMM.pppp pppp.pppp
 * A - Asic type according to CbmTrdAsicType
 * T - trigger type according to CbmTrdTriggerType
 * f - flags according to CbmTrdDigiDef
 * n - error class
 * L - layer id in the TRD setup
 * M - module id in the layer
 * p - pad address within the module
 */

const Double_t CbmTrdDigi::fgClk[]       = {62.5, 12.5, 0.0};
const Float_t CbmTrdDigi::fgPrecission[] = {1.e3, 1., 0.0};
//_________________________________________________________________________________
CbmTrdDigi::CbmTrdDigi() : fInfo(0), fCharge(0), fTime(0) {}
//_________________________________________________________________________________
CbmTrdDigi::CbmTrdDigi(Int_t padChNr, Float_t chargeT, Float_t chargeR, ULong64_t time) : fTime(time)
{
  /** Fill data structure according to FASP representation  
 * A - Asic type according to CbmTrdAsicType
 * M - module id in the layer
 * L - layer id in the TRD setup
 * p - pad address within the module
 * 
 * fCharge definition tttt.tttt tttt.tttt rrrr.rrrr rrrr.rrrr
 * t - tilt paired charge
 * r - rectangle paired charge
 */
  SetAsic(eCbmTrdAsicType::kFASP);
  SetChannel(padChNr);
  SetCharge(chargeT, chargeR);
}

//_________________________________________________________________________________
CbmTrdDigi::CbmTrdDigi(Int_t padChNr, Int_t uniqueModuleId, Float_t charge, ULong64_t time, eTriggerType triggerType,
                       Int_t errClass)
  : fTime(time)
{
  /**
 * Fill data structure according to SPADIC representation  
 * A - Asic type according to CbmTrdAsicType
 * T - trigger type according to CbmTrdTriggerType
 * n - error class
 * M - module id in the layer
 * L - layer id in the TRD setup
 * p - pad address within the module
 * fCharge definition UInt_t(charge*fgPrecission)
*/
  SetAsic(eCbmTrdAsicType::kSPADIC);
  SetChannel(padChNr);
  SetAddress(uniqueModuleId);
  SetCharge(charge);
  SetTriggerType(triggerType);
  SetErrorClass(errClass);
}

// ---- Copy c'tor ----
CbmTrdDigi::CbmTrdDigi(const CbmTrdDigi& digi)
{
  fInfo   = digi.fInfo;
  fCharge = digi.fCharge;
  fTime   = digi.fTime;
}

//_________________________________________________________________________________
void CbmTrdDigi::AddCharge(CbmTrdDigi* sd, Double_t f)
{
  if (GetType() != eCbmTrdAsicType::kFASP) {
    LOG(warn) << "CbmTrdDigi::AddCharge(CbmTrdDigi*, Double_t) : Only available for "
                 "FASP. Use AddCharge(Double_t, Double_t) instead.";
    return;
  }
  Char_t dt = fCharge >> 24, dts = sd->fCharge >> 24;
  UInt_t t = ((fCharge & 0xfff000) >> 12), r = (fCharge & 0xfff), ts = ((sd->fCharge & 0xfff000) >> 12),
         rs = (sd->fCharge & 0xfff);
  // apply correction factor to charge
  Float_t tsf = f * ts / fgPrecission[static_cast<size_t>(eCbmTrdAsicType::kFASP)],
          rsf = f * rs / fgPrecission[static_cast<size_t>(eCbmTrdAsicType::kFASP)];
  ts          = tsf * fgPrecission[static_cast<size_t>(eCbmTrdAsicType::kFASP)];
  rs          = rsf * fgPrecission[static_cast<size_t>(eCbmTrdAsicType::kFASP)];

  if (t + ts < 0xfff) t += ts;
  else
    t = 0xfff;
  if (r + rs < 0xfff) r += rs;
  else
    r = 0xfff;
  dt += dts;
  fCharge = r | (t << 12);
  fCharge |= dt << 24;
}

//_________________________________________________________________________________
void CbmTrdDigi::AddCharge(Double_t c, Double_t f)
{
  if (GetType() != eCbmTrdAsicType::kSPADIC) {
    LOG(warn) << "CbmTrdDigi::AddCharge(Double_t, Double_t) : Only available "
                 "for SPADIC. Use AddCharge(CbmTrdDigi*, Double_t) instead.";
    return;
  }
  SetCharge(GetCharge() + f * c);
}

//_________________________________________________________________________________
Int_t CbmTrdDigi::GetAddressChannel() const
{
  /**  Returns index of the read-out unit in the module in the format row x ncol + col
 */
  return (fInfo >> fgkRoOffset) & 0xfff;
}

//_________________________________________________________________________________
Int_t CbmTrdDigi::GetAddressModule() const
{
  /**  Convert internal representation of module address to CBM address as defined in CbmTrdAddress
 */
  return CbmTrdAddress::GetAddress(Layer(), Module(), 0, 0, 0);
}

//_________________________________________________________________________________
Double_t CbmTrdDigi::GetCharge() const
{
  if (GetType() != eCbmTrdAsicType::kSPADIC) {
    LOG(warn) << "CbmTrdDigi::GetCharge() : Use Double_t GetCharge(Double_t "
                 "&tilt) instead.";
    return 0;
  }
  return fCharge / fgPrecission[static_cast<size_t>(eCbmTrdAsicType::kSPADIC)];
}

//_________________________________________________________________________________
Double_t CbmTrdDigi::GetCharge(Double_t& tilt, Int_t& dt) const
{
  /** Retrieve signal information for FASP.
 * Memory allocation of 32 bits: tttt.tttt TTTT.TTTT TTTT.RRRR RRRR.RRRR
 *    t : time difference of rectangular to tilt pads
 *    T : tilt pads signal
 *    R : Rectangular pads signal
 */
  if (GetType() != eCbmTrdAsicType::kFASP) {
    LOG(warn) << "CbmTrdDigi::GetCharge(Double_t &) : Use Double_t GetCharge() "
                 "instead.";
    return 0;
  }
  Char_t toff = fCharge >> 24;
  dt          = toff;
  tilt        = ((fCharge & 0xfff000) >> 12) / fgPrecission[static_cast<size_t>(eCbmTrdAsicType::kFASP)];
  return (fCharge & 0xfff) / fgPrecission[static_cast<size_t>(eCbmTrdAsicType::kFASP)];
}

//_________________________________________________________________________________
Double_t CbmTrdDigi::GetChargeError() const { return 0; }

//_________________________________________________________________________________
Bool_t CbmTrdDigi::IsFlagged(const Int_t iflag) const
{
  if (iflag < 0 || iflag >= kNflags) return kFALSE;
  return (fInfo >> (fgkFlgOffset + iflag)) & 0x1;
}

//_________________________________________________________________________________
void CbmTrdDigi::SetAddress(Int_t address)
{
  SetLayer(CbmTrdAddress::GetLayerId(address));
  SetModule(CbmTrdAddress::GetModuleId(address));
}

//_________________________________________________________________________________
void CbmTrdDigi::SetAsic(eCbmTrdAsicType ty)
{
  if (ty == eCbmTrdAsicType::kSPADIC) CLRBIT(fInfo, fgkTypOffset);
  else
    SETBIT(fInfo, fgkTypOffset);
}

//_________________________________________________________________________________
void CbmTrdDigi::SetCharge(Float_t cT, Float_t cR, Int_t dt)
{
  /** Load signal information for FASP.
 * Memory allocation of 32 bits: tttt.tttt TTTT.TTTT TTTT.RRRR RRRR.RRRR
 *    t : time difference of rectangular to tilt pads (8 bits)
 *    T : tilt pads signal (12 bits)
 *    R : Rectangular pads signal (12 bits)
 */
  UInt_t r    = UInt_t(cR * fgPrecission[static_cast<size_t>(eCbmTrdAsicType::kFASP)]),
         t    = UInt_t(cT * fgPrecission[static_cast<size_t>(eCbmTrdAsicType::kFASP)]);
  Char_t toff = dt;
  if (dt > 127) toff = 127;
  else if (dt < -127)
    toff = -127;
  if (r > 0xfff) r = 0xfff;
  if (t > 0xfff) t = 0xfff;
  fCharge = r | (t << 12);
  fCharge |= toff << 24;
}

//_________________________________________________________________________________
void CbmTrdDigi::SetCharge(Float_t c)
{

  fCharge = UInt_t(c * fgPrecission[static_cast<size_t>(eCbmTrdAsicType::kSPADIC)]);
}

//_________________________________________________________________________________
void CbmTrdDigi::SetFlag(const Int_t iflag, Bool_t set)
{
  if (iflag < 0 || iflag >= kNflags) return;
  if (set) SETBIT(fInfo, fgkFlgOffset + iflag);
  else
    CLRBIT(fInfo, fgkFlgOffset + iflag);
}

//_________________________________________________________________________________
void CbmTrdDigi::SetTime(Double_t t) { fTime = ULong64_t(TMath::Ceil(t / Clk(GetType()))); }

//_________________________________________________________________________________
void CbmTrdDigi::SetTimeOffset(Char_t t)
{
  if (GetType() != eCbmTrdAsicType::kFASP) return;
  fCharge <<= 8;
  fCharge >>= 8;
  fCharge |= t << 24;
}

//_________________________________________________________________________________
void CbmTrdDigi::SetTriggerType(const eTriggerType triggerType)
{
  if (triggerType < eTriggerType::kBeginTriggerTypes || triggerType >= eTriggerType::kNTrg) return;
  const Int_t ttype = static_cast<Int_t>(triggerType);
  fInfo |= (ttype << fgkTrgOffset);
}

//_________________________________________________________________________________
void CbmTrdDigi::SetTriggerType(const Int_t ttype)
{
  if (ttype < static_cast<Int_t>(eTriggerType::kBeginTriggerTypes) || ttype >= static_cast<Int_t>(eTriggerType::kNTrg))
    return;
  fInfo |= (ttype << fgkTrgOffset);
}


//_________________________________________________________________________________
string CbmTrdDigi::ToString() const
{
  stringstream ss;
  ss << "CbmTrdDigi(" << (GetType() == eCbmTrdAsicType::kFASP ? "T)" : "R)")
     << " | moduleAddress=" << GetAddressModule() << " | layer=" << Layer() << " | moduleId=" << Module()
     << " | pad=" << GetAddressChannel() << " | time[ns]=" << std::fixed << std::setprecision(1) << GetTime();
  if (GetType() == eCbmTrdAsicType::kFASP) {
    Int_t trg(GetTriggerType()), dt;
    Double_t t, r = GetCharge(t, dt);
    Bool_t ttrg(trg & 1), rtrg((trg & 2) >> 1);
    ss << " | pu=" << (IsPileUp() ? "y" : "n") << " | mask=" << (IsMasked() ? "y" : "n") << " |charge=" << std::fixed
       << std::setw(6) << std::setprecision(1) << t << (!ttrg && t > 0 ? '*' : ' ') << "/" << r
       << (!rtrg && r > 0 ? '*' : ' ') << "[" << dt << "]";
  }
  else {
    ss << " | charge=" << GetCharge() << " TriggerType=" << GetTriggerType() << " ErrorClass=" << GetErrorClass();
  }
  ss << endl;
  return ss.str();
}

ClassImp(CbmTrdDigi)
