/* Copyright (C) 2006-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese, Denis Bertini [committer], Florian Uhlig */

/** @file CbmStsDigi.h
 ** @author V.Friese <v.friese@gsi.de>
 ** @author Felix Weiglhofer <weiglhofer@fias.uni-frankfurt.de>
 ** @since 28.08.2006
 ** @version 6
 **/


#ifndef CBMSTSDIGI_H
#define CBMSTSDIGI_H 1

#include "CbmDefs.h"  // for ECbmModuleId::kSts
#include "CbmStsAddress.h"

#include <xpu/defines.h>  // for XPU_D

#if !defined(NO_ROOT) && !XPU_IS_HIP_CUDA
#include <Rtypes.h>  // for ClassDef
#endif


#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

#include <cassert>
#include <string>  // for string

/** @class CbmStsDigi
 ** @brief Data class for a single-channel message in the STS
 **
 ** The CbmStsDigi is the ROOT representation of the smallest information
 ** unit delivered by the CBM-STS by a single readout channel. It carries
 ** the channel address, the measurement time and the digitised charge
 ** as information.
 **/
class CbmStsDigi {

public:
  /** Default constructor **/
  CbmStsDigi() = default;


  /** Standard constructor
   ** @param  address  Unique element address
   ** @param  channel  Channel number
   ** @param  time     Measurement time [ns]
   ** @param  charge   Charge [ADC units]
   **/
  XPU_D CbmStsDigi(int32_t address, int32_t channel, double time, uint16_t charge)
  {
    // StsDigi is not able to store negative timestamps.
    assert(time >= 0);
    // StsDigi has a maximal time range of ~2.1s (2 147 483 647 ns).
    assert(time <= kMaxTimestamp);
    PackAddressAndTime(address, time);
    PackChannelAndCharge(channel, charge);
  }

  /** Destructor **/
  ~CbmStsDigi() = default;

  /** Unique detector element address  (see CbmStsAddress)
   ** @value Unique address of readout channel
   **/
  XPU_D int32_t GetAddress() const { return UnpackAddress(); }


  /** @brief Get the desired name of the branch for this obj in the cbm output tree  (static)
   ** @return "StsDigi"
   **/
  static const char* GetBranchName() { return "StsDigi"; }


  /** @brief Channel number in module
   ** @value Channel number
   **/
  XPU_D uint16_t GetChannel() const { return UnpackChannel(); }


  /** @brief Class name (static)
   ** @return CbmStsDigi
   **/
  static const char* GetClassName() { return "CbmStsDigi"; }

#if XPU_IS_CPU
  /** Charge
   ** @value Charge [ADC units]
   **/
  double GetCharge() const { return static_cast<double>(UnpackCharge()); }
#endif

  XPU_D uint16_t GetChargeU16() const { return UnpackCharge(); }


  /** System ID (static)
  ** @return System identifier (EcbmModuleId)
  **/
  static ECbmModuleId GetSystem() { return ECbmModuleId::kSts; }

#if XPU_IS_CPU
  /** Time of measurement
   ** @value Time [ns]
   **/
  double GetTime() const { return static_cast<double>(UnpackTime()); }
#endif

  XPU_D uint32_t GetTimeU32() const { return UnpackTime(); }


  template<class Archive>
  void serialize(Archive& ar, const unsigned int /*version*/)
  {
    ar& fTime;
    ar& fChannelAndCharge;
    ar& fAddress;
  }


  /** Update Time of measurement
   ** @param New Time [ns]
   **/
  XPU_D void SetTime(double dNewTime)
  {
    // StsDigi is not able to store negative timestamps.
    assert(dNewTime >= 0);
    // StsDigi has a maximal time range of ~2.1s (2 147 483 647 ns).
    assert(dNewTime <= kMaxTimestamp);
    PackTime(dNewTime);
  }

  XPU_D void SetChannel(uint16_t channel) { PackChannelAndCharge(channel, UnpackCharge()); }

  XPU_D void SetCharge(uint16_t charge) { PackChannelAndCharge(UnpackChannel(), charge); }

  XPU_D void SetAddress(int32_t address) { PackAddressAndTime(address, UnpackTime()); }


  /** Set new channel and charge.
   **
   ** Slightly more efficient than calling both individual setters.
   **/
  XPU_D void SetChannelAndCharge(uint16_t channel, uint16_t charge) { PackChannelAndCharge(channel, charge); }

  /** Set new address and time at once.
   **
   ** Slightly more efficient than calling both individual setters.
   **/
  XPU_D void SetAddressAndTime(int32_t address, uint32_t time) { PackAddressAndTime(address, time); }


  /** String output **/
  std::string ToString() const;


private:
  friend class boost::serialization::access;


  static constexpr int kNumAdcBits              = 5;
  static constexpr uint32_t kAdcMask            = (1u << kNumAdcBits) - 1u;
  static constexpr int kNumLowerAddrBits        = 16;
  static constexpr int kNumTimestampBits        = 31;
  static constexpr uint32_t kTimestampMask      = (1u << kNumTimestampBits) - 1u;
  static constexpr uint32_t kMaxTimestamp       = kTimestampMask;
  static constexpr uint32_t kTimeAddressBitMask = ~kTimestampMask;

// CUDA requires types in shared memory to have trivial constructors / destructors
#if XPU_IS_HIP_CUDA
#define CPU_ONLY(x)
#else
#define CPU_ONLY(x) x
#endif

  uint32_t fTime CPU_ONLY(= 0);              ///< Time [ns] in lower 31 bits, highest bit is the 17th address bit.
  uint16_t fChannelAndCharge CPU_ONLY(= 0);  ///< Channel number (lower 11 bits) and charge [ADC Units] in upper 5 bits.
  uint16_t fAddress CPU_ONLY(= 0);           ///< Unique element address (lower 16 bits of 17)

#undef CPU_ONLY


  XPU_D void PackTime(uint32_t newTime) { fTime = (fTime & kTimeAddressBitMask) | (newTime & kTimestampMask); }
  XPU_D uint32_t UnpackTime() const { return fTime & kTimestampMask; }


  XPU_D void PackChannelAndCharge(uint16_t channel, uint16_t charge)
  {
    fChannelAndCharge = (channel << kNumAdcBits) | charge;
  }
  XPU_D uint16_t UnpackChannel() const { return fChannelAndCharge >> kNumAdcBits; }
  XPU_D uint16_t UnpackCharge() const { return fChannelAndCharge & kAdcMask; }

  XPU_D void PackAddressAndTime(int32_t newAddress, uint32_t newTime)
  {
    int32_t packedAddr = CbmStsAddress::PackDigiAddress(newAddress);

    uint32_t highestBitAddr = packedAddr >> kNumLowerAddrBits;
    uint32_t lowerAddr      = packedAddr & ((1 << kNumLowerAddrBits) - 1);

    fAddress = lowerAddr;
    fTime    = (highestBitAddr << kNumTimestampBits) | (kTimestampMask & newTime);
  }

  XPU_D int32_t UnpackAddress() const
  {
    int32_t highestBitAddr = fTime >> kNumTimestampBits;
    int32_t packedAddress  = (highestBitAddr << kNumLowerAddrBits) | int32_t(fAddress);
    return CbmStsAddress::UnpackDigiAddress(packedAddress);
  }

#if !defined(NO_ROOT) && !XPU_IS_HIP_CUDA
  ClassDefNV(CbmStsDigi, 8);
#endif
};


#endif
