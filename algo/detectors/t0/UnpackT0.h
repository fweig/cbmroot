/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith [committer] */

#ifndef CBM_ALGO_UNPACKT0_H
#define CBM_ALGO_UNPACKT0_H 1


#include "CbmTofDigi.h"

#include "MicrosliceDescriptor.hpp"
#include "Timeslice.hpp"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "CriGet4Mess001.h"

namespace cbm::algo
{


  /** @struct UnpackT0ElinkPar
   ** @author Volker Friese <v.friese@gsi.de>
   ** @since 25 November 2021
   ** @brief T0 Unpacking parameters for one eLink / ASIC
   **/
  struct UnpackT0ElinkPar {
    std::vector<uint32_t> fChannelUId;  ///< CbmT0Address for different channels
    uint64_t fTimeOffset = 0.;          ///< Time calibration parameter
  };


  /** @struct UnpackT0Par
   ** @author Volker Friese <v.friese@gsi.de>
   ** @since 25 November 2021
   ** @brief Parameters required for the STS unpacking (specific to one component)
   **/
  struct UnpackT0Par {
    std::vector<UnpackT0ElinkPar> fElinkParams = {};  ///< Parameters for each eLink
  };


  /** @struct UnpackT0Moni
   ** @author Volker Friese <v.friese@gsi.de>
   ** @since 2 December 2021
   ** @brief Monitoring data for STS unpacking
   **/
  struct UnpackT0MonitorData {
    uint32_t fNumNonHitOrTsbMessage     = 0;
    uint32_t fNumErrElinkOutOfRange     = 0;  ///< Elink not contained in parameters
    uint32_t fNumErrInvalidFirstMessage = 0;  ///< First message is not EPOCH
    uint32_t fNumErrInvalidLastMessage  = 0;  ///< Last message is not EndOfMs
    uint32_t fNumErrInvalidMsSize       = 0;  ///< Microslice size is not multiple of message size
    uint32_t fNumErrTimestampOverflow   = 0;  ///< Overflow in 64 bit time stamp
    uint32_t fNumErrInvalidStartEpoch   = 0;  ///< Microslice index doesn't match first epoch
    bool HasErrors()
    {
      uint32_t numErrors = fNumNonHitOrTsbMessage + fNumErrElinkOutOfRange + fNumErrInvalidFirstMessage
                           + fNumErrInvalidMsSize + fNumErrTimestampOverflow;
      return (numErrors > 0 ? true : false);
    }
  };


  /** @class UnpackT0
   ** @author Pierre-Alain Loizeau <p.-a.loizeau@gsi.de>
   ** @author Volker Friese <v.friese@gsi.de>
   ** @since 25 November 2021
   ** @brief Unpack algorithm for STS
   **/
  class UnpackT0 {

  public:
    typedef std::pair<std::vector<CbmTofDigi>, UnpackT0MonitorData> resultType;


    /** @brief Default constructor **/
    UnpackT0() {};


    /** @brief Destructor **/
    ~UnpackT0() {};


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
    void SetParams(std::unique_ptr<UnpackT0Par> params) { fParams = *(std::move(params)); }


  private:  // methods
    /** @brief Process a hit message
     ** @param message SMX message (32-bit word)
     ** @param digiVec Vector to append the created digi to
     ** @param monitor Reference to monitor object
     **/
    void ProcessHitMessage(const critof001::Message& message, std::vector<CbmTofDigi>& digiVec,
                           UnpackT0MonitorData& monitor) const;

    /** @brief Process an epoch message (TS_MSB)
     ** @param message SMX message (32-bit word)
     **/
    void ProcessEpochMessage(const critof001::Message& message);


  private:                            // members
    uint64_t fCurrentTsTime    = 0;   ///< Unix time of timeslice in units of epoch length
    uint32_t fCurrentEpochInTs = 0;   ///< Current epoch number relative to timeslice start epoch
    UnpackT0Par fParams        = {};  ///< Parameter container
  };


} /* namespace cbm::algo */

#endif /* CBM_ALGO_UNPACKT0_H */
