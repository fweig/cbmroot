/* Copyright (C) 2023 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith [committer] */

#ifndef CBM_ALGO_UNPACKSTSXPU_H
#define CBM_ALGO_UNPACKSTSXPU_H 1


#include "CbmStsDigi.h"

#include "MicrosliceDescriptor.hpp"
#include "Timeslice.hpp"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include <xpu/device.h>
#include <xpu/host.h>

#include "StsReadoutConfig.h"
#include "StsXyterMessage.h"


namespace cbm::algo
{

  /** @struct UnpackStsXpuElinkPar
   ** @author Volker Friese <v.friese@gsi.de>
   ** @since 25 November 2021
   ** @brief STS Unpacking parameters for one eLink / ASIC
   **/
  struct UnpackStsXpuElinkPar {
    int32_t fAddress     = 0;   ///< CbmStsAddress for the connected module
    uint32_t fAsicNr     = 0;   ///< Number of connected ASIC within the module
    uint64_t fTimeOffset = 0.;  ///< Time calibration parameter
    double fAdcOffset    = 0.;  ///< Charge calibration parameter
    double fAdcGain      = 0.;  ///< Charge calibration parameter
  };


  /** @struct UnpackStsXpuPar
   ** @author Volker Friese <v.friese@gsi.de>
   ** @since 25 November 2021
   ** @brief Parameters required for the STS unpacking (specific to one component)
   **/
  struct UnpackStsXpuPar {
    uint32_t fNumChansPerAsic   = 0;  ///< Number of channels per ASIC
    uint32_t fNumAsicsPerModule = 0;  ///< Number of ASICS per module
    uint32_t fNumElinks         = 0;  ///< Number of elinks for this component
    uint32_t fElinkOffset       = 0;  ///< Elink index offset for this component
  };


  /** @struct UnpackStsXpuMoni
   ** @author Volker Friese <v.friese@gsi.de>
   ** @since 2 December 2021
   ** @brief Monitoring data for STS unpacking
   **/
  struct UnpackStsXpuMonitorData {
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
  };


  /** @class UnpackStsXpu
   ** @author Pierre-Alain Loizeau <p.-a.loizeau@gsi.de>
   ** @author Volker Friese <v.friese@gsi.de>
   ** @since 25 November 2021
   ** @brief Unpack algorithm for STS
   **/
  class UnpackStsXpu {

  public:
    typedef std::pair<std::vector<CbmStsDigi>, UnpackStsXpuMonitorData> resultType;

    /** @brief Default constructor **/
    UnpackStsXpu() {};


    /** @brief Destructor **/
    ~UnpackStsXpu() {};


    /** @brief Algorithm execution
     ** @return STS digi data
     ** @param  ts      Timselice payload
     ** @param  config  Configuration data
     ** @return STS digi data
     **/
    resultType operator()(const fles::Timeslice* ts, StsReadoutConfig& config);


    struct StsXpuUnpack {
    };  // Identifier used by xpu to find where kernels are located


    // Run unpacker for each microslice
    XPU_EXPORT_KERNEL(StsXpuUnpack, Unpack, UnpackStsXpuPar* params, UnpackStsXpuElinkPar* elinkParams,
                      stsxyter::Message* content, uint64_t* msMessCount, uint64_t* msMessOffset, uint64_t* msStartTime,
                      uint32_t* msCompIdx, CbmStsDigi* digisOut, const uint64_t currentTsTime, int NElems);

    //Stores parameter structs for all elinks
    xpu::hd_buffer<UnpackStsXpuElinkPar> fElinkParams;


    //Stores parameter structs for all components
    xpu::hd_buffer<UnpackStsXpuPar> fParams;


  private:  // methods
    /** @brief Process a hit message
     ** @param message SMX message (32-bit word)
     ** @param digi buffer
     ** @param digi counter
     ** @param parameters for component
     ** @param parameter buffer for elinks
     ** @param reference to monitor object
     ** @param current epoch number within epoch cycle
     **/
    XPU_D static void ProcessHitMessage(const stsxyter::Message& message, CbmStsDigi* digis, uint64_t& numDigis,
                                        const UnpackStsXpuPar& unpackPar, UnpackStsXpuElinkPar* elinkParams,
                                        UnpackStsXpuMonitorData& monitor, const uint64_t currentEpochTime);

    /** @brief Process an epoch message (TS_MSB)
     ** @param message SMX message (32-bit word)
     ** @param current epoch number within epoch cycle
     ** @param current epoch time relative to timeslice in clock cycles
     ** @param current TS_MSB epoch cycle
     **/
    XPU_D static void ProcessTsmsbMessage(const stsxyter::Message& message, uint32_t& currentEpoch,
                                          uint64_t& currentEpochTime, uint64_t& currentCycle,
                                          const uint64_t currentTsTime);

  private:  // members
            ///// To do: Make these available on device somehow
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


} /* namespace cbm::algo */

#endif /* CBM_ALGO_UNPACKSTSXPU_H */
