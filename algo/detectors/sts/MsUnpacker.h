/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau, Volker Friese [committer], Felix Weiglhofer */

#ifndef CBM_ALGO_STS_MS_UNPACKER_H
#define CBM_ALGO_STS_MS_UNPACKER_H 1

#include "CbmStsDigi.h"

#include "MicrosliceDescriptor.hpp"
#include "Timeslice.hpp"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "Prelude.h"
#include "StsXyterMessage.h"

namespace cbm::algo::sts
{


  /** @struct UnpackStsElinkPar
   ** @author Volker Friese <v.friese@gsi.de>
   ** @since 25 November 2021
   ** @brief STS Unpacking parameters for one eLink / ASIC
   **/
  struct UnpackElinkPar {
    int32_t fAddress     = 0;   ///< CbmStsAddress for the connected module
    uint32_t fAsicNr     = 0;   ///< Number of connected ASIC within the module
    uint64_t fTimeOffset = 0.;  ///< Time calibration parameter
    double fAdcOffset    = 0.;  ///< Charge calibration parameter
    double fAdcGain      = 0.;  ///< Charge calibration parameter
  };


  /** @struct UnpackStsPar
   ** @author Volker Friese <v.friese@gsi.de>
   ** @since 25 November 2021
   ** @brief Parameters required for the STS unpacking (specific to one component)
   **/
  struct UnpackPar {
    uint32_t fNumChansPerAsic                = 0;   ///< Number of channels per ASIC
    uint32_t fNumAsicsPerModule              = 0;   ///< Number of ASICS per module
    std::vector<UnpackElinkPar> fElinkParams = {};  ///< Parameters for each eLink
  };


  /** @struct UnpackStsMoni
   ** @author Volker Friese <v.friese@gsi.de>
   ** @since 2 December 2021
   ** @brief Monitoring data for STS unpacking
   **/
  struct UnpackMonitorData {
    uint32_t fNumNonHitOrTsbMessage     = 0;
    uint32_t fNumErrElinkOutOfRange     = 0;  ///< Elink not contained in parameters
    uint32_t fNumErrInvalidFirstMessage = 0;  ///< First message is not TS_MSB or second is not EPOCH
    uint32_t fNumErrInvalidMsSize       = 0;  ///< Microslice size is not multiple of message size
    uint32_t fNumErrTimestampOverflow   = 0;  ///< Overflow in 64 bit time stamp
    bool HasErrors()
    {
      uint32_t numErrors = fNumNonHitOrTsbMessage + fNumErrElinkOutOfRange + fNumErrInvalidFirstMessage
                           + fNumErrInvalidMsSize + fNumErrTimestampOverflow;
      return (numErrors > 0 ? true : false);
    }
    std::string print()
    {
      std::stringstream ss;
      ss << "errors " << fNumNonHitOrTsbMessage << " | " << fNumErrElinkOutOfRange << " | "
         << fNumErrInvalidFirstMessage << " | " << fNumErrInvalidMsSize << " | " << fNumErrTimestampOverflow << " | ";
      return ss.str();
    }
  };

  /** @class UnpackSts
   ** @author Pierre-Alain Loizeau <p.-a.loizeau@gsi.de>
   ** @author Volker Friese <v.friese@gsi.de>
   ** @since 25 November 2021
   ** @brief Unpack algorithm for STS
   **/
  class MsUnpacker {

  public:
    // Parameters
    std::vector<u32> fNumChansPerAsic;    ///< Number of channels per ASIC, [size = nComponents]
    std::vector<u32> fNumAsicsPerModule;  ///< Number of ASICS per module, [size = nComponents]

    /**
     *  Offset of first eLink per component, last entry is total size of elinks [size = nComponents + 1]
     */
    std::vector<u32> fElinkOffsetPerComponent;
    std::vector<UnpackElinkPar> fElinkParams;  ///< Parameters for each eLink, [size = nElinksTotal]

    // Timeslice data / Input
    std::vector<u16> fMsEquipmentId;                         ///< Equipment ID / Component number [size = nMicroslices]
    std::vector<fles::MicrosliceDescriptor> fMsDescriptors;  ///< Microslice descriptors [size = nMicroslices]
    std::vector<const u8*> fMsContent;                       ///< Microslice content [size = nMicroslices]

    // Output
    std::vector<std::vector<CbmStsDigi>> fMsDigis;  ///< Digi data per microslice [size = nMicroslices]
    std::vector<UnpackMonitorData> fMsMonitor;      ///< Monitoring data per microslice [size = nMicroslices]

    /** @brief Default constructor **/
    MsUnpacker() = default;

    /** @brief Destructor **/
    ~MsUnpacker() = default;


    /** @brief Algorithm execution
     ** @param  msIndex  Microslice index in timeslice
     ** @param  tTimeslice Unix start time of timeslice [ns]
     **/
    void operator()(const u32 msIndex, const uint64_t tTimeslice);

  private:  // private datastructures
    /**
     * @brief Structure to hold the current time information for the current microslice
     */
    struct TimeSpec {
      u64 fCurrentTsTime    = 0;  ///< Unix time of timeslice in units of epoch length
      u64 fCurrentCycle     = 0;  ///< Current epoch cycle
      u32 fCurrentEpoch     = 0;  ///< Current epoch number within epoch cycle
      u64 fCurrentEpochTime = 0;  ///< Current epoch time relative to timeslice in clock cycles
    };

  private:  // methods
    /** @brief Process a hit message
     ** @param message SMX message (32-bit word)
     ** @param digiVec Vector to append the created digi to
     ** @param monitor Reference to monitor object
     **/
    void ProcessHitMessage(const u32 msIndex, const TimeSpec& time, const stsxyter::Message& message,
                           std::vector<CbmStsDigi>& digiVec, UnpackMonitorData& monitor) const;

    /** @brief Process an epoch message (TS_MSB)
     ** @param message SMX message (32-bit word)
     **/
    void ProcessTsmsbMessage(const stsxyter::Message& message, TimeSpec& time) const;


  private:  // members
    /** Number of TS_MSB epochs per cycle **/
    static constexpr uint64_t fkEpochsPerCycle = stsxyter::kuTsMsbNbTsBinsBinning;

    /** Length of TS_MSB epoch in clock cycles **/
    static constexpr uint64_t fkEpochLength = stsxyter::kuHitNbTsBinsBinning;

    /** Clock cycle nominator [ns] and denominator. The clock cycle in ns is nominator / denominator. **/
    static constexpr uint32_t fkClockCycleNom = stsxyter::kulClockCycleNom;
    static constexpr uint32_t fkClockCycleDen = stsxyter::kulClockCycleDen;

    /** Epoch cycle length in ns **/
    static constexpr uint64_t fkCycleLength = (fkEpochsPerCycle * fkEpochLength * fkClockCycleNom) / fkClockCycleDen;
  };


}  // namespace cbm::algo::sts

#endif /* CBM_ALGO_STS_MS_UNPACKER_H */
