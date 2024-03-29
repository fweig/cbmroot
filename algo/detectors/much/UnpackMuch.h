/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau, Volker Friese [committer] */

#ifndef CBM_ALGO_UNPACKMUCH_H
#define CBM_ALGO_UNPACKMUCH_H 1


#include "CbmMuchDigi.h"

#include "MicrosliceDescriptor.hpp"
#include "Timeslice.hpp"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <sstream>
#include <vector>

#include "StsXyterMessage.h"

namespace cbm::algo
{


  /** @struct UnpackMuchElinkPar
   ** @author Volker Friese <v.friese@gsi.de>
   ** @since 25 November 2021
   ** @brief STS Unpacking parameters for one eLink / ASIC
   **/
  struct UnpackMuchElinkPar {
    std::vector<uint32_t> fAddress;  ///< CbmMuchAddress for different channels
    uint64_t fTimeOffset = 0.;       ///< Time calibration parameter
  };


  /** @struct UnpackMuchPar
   ** @author Volker Friese <v.friese@gsi.de>
   ** @since 25 November 2021
   ** @brief Parameters required for the STS unpacking (specific to one component)
   **/
  struct UnpackMuchPar {
    std::vector<UnpackMuchElinkPar> fElinkParams = {};  ///< Parameters for each eLink
  };


  /** @struct UnpackMuchMoni
   ** @author Volker Friese <v.friese@gsi.de>
   ** @since 2 December 2021
   ** @brief Monitoring data for STS unpacking
   **/
  struct UnpackMuchMonitorData {
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


  /** @class UnpackMuch
   ** @author Pierre-Alain Loizeau <p.-a.loizeau@gsi.de>
   ** @author Volker Friese <v.friese@gsi.de>
   ** @since 25 November 2021
   ** @brief Unpack algorithm for STS
   **/
  class UnpackMuch {

  public:
    typedef std::pair<std::vector<CbmMuchDigi>, UnpackMuchMonitorData> resultType;


    /** @brief Default constructor **/
    UnpackMuch() {};


    /** @brief Destructor **/
    ~UnpackMuch() {};


    /** @brief Algorithm execution
     ** @param  msContent  Microslice payload
     ** @param  msDescr    Microslice descriptor
     ** @param  tTimeslice Unix start time of timeslice [ns]
     ** @return STS digi data
     **/
    resultType operator()(const uint8_t* msContent, const fles::MicrosliceDescriptor& msDescr,
                          const uint64_t tTimeslice);

    /** @brief Set the parameter container
     ** @param params Pointer to parameter container
     **/
    void SetParams(std::unique_ptr<UnpackMuchPar> params) { fParams = *(std::move(params)); }


  private:  // methods
    /** @brief Process a hit message
     ** @param message SMX message (32-bit word)
     ** @param digiVec Vector to append the created digi to
     ** @param monitor Reference to monitor object
     **/
    void ProcessHitMessage(const stsxyter::Message& message, std::vector<CbmMuchDigi>& digiVec,
                           UnpackMuchMonitorData& monitor) const;

    /** @brief Process an epoch message (TS_MSB)
     ** @param message SMX message (32-bit word)
     **/
    void ProcessTsmsbMessage(const stsxyter::Message& message);


  private:                            // members
    uint64_t fCurrentTsTime    = 0;   ///< Unix time of timeslice in units of epoch length
    uint64_t fCurrentCycle     = 0;   ///< Current epoch cycle
    uint32_t fCurrentEpoch     = 0;   ///< Current epoch number within epoch cycle
    uint64_t fCurrentEpochTime = 0;   ///< Current epoch time relative to timeslice in clock cycles
    UnpackMuchPar fParams      = {};  ///< Parameter container

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

#endif /* CBM_ALGO_UNPACKSTS_H */
