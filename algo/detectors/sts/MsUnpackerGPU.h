/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau, Volker Friese [committer], Felix Weiglhofer */

#ifndef CBM_ALGO_STS_MS_UNPACKER_H
#define CBM_ALGO_STS_MS_UNPACKER_H 1

#include "CbmStsDigi.h"

#include "MicrosliceDescriptor.hpp"
#include "Timeslice.hpp"

#include "gpu/DeviceImage.h"

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
    uint32_t fNumElinks                      = 0;  ///< Number of elinks for this component   --EDIT
    uint32_t fElinkOffset                    = 0;  ///< Elink index offset for this component --EDIT
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

    /*
    --EDIT
    -- No usage anyways?
    std::string print()
    {
      std::stringstream ss;
      ss << "errors " << fNumNonHitOrTsbMessage << " | " << fNumErrElinkOutOfRange << " | "
         << fNumErrInvalidFirstMessage << " | " << fNumErrInvalidMsSize << " | " << fNumErrTimestampOverflow << " | ";
      return ss.str();
    }
    */
  };

  class MsUnpacker;

  struct TheUnpacker : xpu::constant<GPUReco, MsUnpacker> {};

  enum GpuConstants
  {
    #if XPU_IS_CUDA
        kBlockSize         = 32,
    #else  // HIP, values ignored on CPU
        kBlockSize         = 64,
    #endif
  };

  static constexpr u16 fSmemBufferSize = 1024;
  struct Unpack : xpu::kernel<GPUReco> {
    struct shared_memory {
      stsxyter::Message messageBuffer [fSmemBufferSize];
      CbmStsDigi digiBuffer [fSmemBufferSize];
      u64 digiCounter;
      u64 startNumDigis;};
    using block_size    = xpu::block_size<kBlockSize>;
    using constants     = xpu::cmem<TheUnpacker>;
    using context       = xpu::kernel_context<shared_memory, constants>;
    XPU_D void operator()(context&, const uint64_t currentTsTime);
  };

  struct CopyOut : xpu::kernel<GPUReco> {
    using block_size    = xpu::block_size<kBlockSize>;
    using constants     = xpu::cmem<TheUnpacker>;
    using context       = xpu::kernel_context<xpu::no_smem, constants>;
    XPU_D void operator()(context&);
  };

  /** @class UnpackSts
   ** @author Pierre-Alain Loizeau <p.-a.loizeau@gsi.de>
   ** @author Volker Friese <v.friese@gsi.de>
   ** @since 25 November 2021
   ** @brief Unpack algorithm for STS
   **/
  class MsUnpacker {

  public:
    /** @brief Default constructor **/
    MsUnpacker() = default;

    /** @brief Destructor **/
    ~MsUnpacker() = default;

    xpu::buffer<UnpackElinkPar> fElinkParams;                 ///< Parameters for each eLink, [size = nElinksTotal]
    xpu::buffer<UnpackPar> fParams;

    //Timeslice data / Input
    xpu::buffer<u16> fMsEquipmentId;                          ///< Equipment ID / Component number [size = nMicroslices]
    xpu::buffer<fles::MicrosliceDescriptor> fMsDescriptors;   ///< Microslice descriptors [size = nMicroslices]
    xpu::buffer<stsxyter::Message> fMsContent;                ///< Microslice content [size = nMessages]
    xpu::buffer<u64> fMsOffset;                               ///< Digi offset per MS for fMsDigis [size = nMicroslices]

    //Output
    xpu::buffer<u64> fMsDigiCount;                            ///< Digi count per MS [size = nMicroslices]
    xpu::buffer<UnpackMonitorData> fMsMonitor;                ///< Monitoring data per microslice [size = nMicroslices]
    xpu::buffer<CbmStsDigi> fMsDigis;                         ///< Digi data [size = nMessages]
    xpu::buffer<CbmStsDigi> fMsOutBuffer;                      ///< Digi data without message offsets [size = nDigis]
    

    /** @brief Algorithm execution
     ** @param  msIndex  Microslice index in timeslice
     ** @param  tTimeslice Unix start time of timeslice [ns]
     **/
    XPU_D void operator()(Unpack::context& ctx, const uint64_t currentTsTime) const;
    XPU_D void CopyOut(CopyOut::context& ctx) const;

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
    XPU_D void ProcessHitMessage(const stsxyter::Message& message, CbmStsDigi* digis, uint64_t& numDigis,
                                        const UnpackPar& unpackPar, UnpackElinkPar* elinkParams,
                                        UnpackMonitorData& monitor, TimeSpec& time) const;

    /** @brief Process an epoch message (TS_MSB)
     ** @param message SMX message (32-bit word)
     **/
    XPU_D void ProcessTsmsbMessage(const stsxyter::Message& message, TimeSpec& time) const;


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

    static constexpr u64 fEpochLengthInNs = fkEpochLength * fkClockCycleNom / fkClockCycleDen;
  };


}  // namespace cbm::algo::sts

#endif /* CBM_ALGO_STS_MS_UNPACKER_H */
