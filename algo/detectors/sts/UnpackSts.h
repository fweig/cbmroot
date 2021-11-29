/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau, Volker Friese [committer] */

#ifndef CBM_ALGO_UNPACKSTS_H
#define CBM_ALGO_UNPACKSTS_H 1


#include "CbmStsDigi.h"

#include "MicrosliceDescriptor.hpp"
#include "Timeslice.hpp"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "StsXyterMessage.h"

namespace cbm::algo
{


  /** @struct UnpackStsAsicPar
   ** @author Volker Friese <v.friese@gsi.de>
   ** @since 25 November 2021
   ** @brief Unpacking parameters for one eLink / ASIC
   **/
  struct UnpackStsElinkPar {
    uint32_t fAddress    = 0;   ///< CbmStsAddress for the connected module
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
  struct UnpackStsPar {

    uint32_t fNumChansPerAsic   = 0;              ///< Number of channels per ASIC
    uint32_t fNumAsicsPerModule = 0;              ///< Number of ASICS per module
    uint64_t fEpochsPerCycle    = 0;              ///< TS_MSB epochs per epoch cycle
    uint64_t fEpochLength       = 0;              ///< Length of TS_MSB epoch in clock cycles
    uint32_t fClockCycleNom     = 0;              ///< Clock cycle nominator [ns]
    uint32_t fClockCycleDen     = 0.;             ///< Clock cycle denominator
    std::vector<UnpackStsElinkPar> fElinkParams;  ///< Parameters for each eLink

    size_t GetNumElinks() const { return fElinkParams.size(); }

    const UnpackStsElinkPar& GetElinkPar(size_t eLink) const
    {
      assert(eLink < GetNumElinks());
      return fElinkParams[eLink];
    }
  };


  /** @class UnpackSts
   ** @author Pierre-Alain Loizeau <p.-a.loizeau@gsi.de>
   ** @author Volker Friese <v.friese@gsi.de>
   ** @since 25 November 2021
   ** @brief Unpack algorithm for STS
   **/
  class UnpackSts {

  public:
    /** @brief Default constructor **/
    UnpackSts() {};


    /** @brief Destructor **/
    ~UnpackSts() {};


    /** @brief Algorithm execution
     ** @param  msContent  Microslice payload
     ** @param  msDescr    Microslice descriptor
     ** @param  tTimeslice Unix start time of timeslice [ns]
     ** @return STS digi data
     **/
    std::vector<CbmStsDigi> operator()(const uint8_t* msContent, const fles::MicrosliceDescriptor& msDescr,
                                       const uint64_t tTimeslice);

    /** @brief Set the parameter container
     ** @param params Pointer to parameter container
     **/
    void SetParams(std::unique_ptr<UnpackStsPar> params)
    {
      fParams      = std::move(params);
      fCycleLength = (fParams->fEpochsPerCycle * fParams->fEpochLength * fParams->fClockCycleNom);
      fCycleLength /= fParams->fClockCycleDen;
    }


  private:
    /** @brief Process an epoch message (TS_MSB)
     ** @param message SMX message (32-bit word)
     ** @param digiVec Vector to append the created digi to
     **/
    void ProcessTsmsbMessage(const stsxyter::Message& message);

    /** @brief Process a hit message
     ** @param message SMX message (32-bit word)
     ** @param digiVec Vector to append the created digi to
     **/
    void ProcessHitMessage(const stsxyter::Message& message, std::vector<CbmStsDigi>& digiVec) const;

  private:
    uint64_t fCurrentTsTime    = 0;  ///< Unix time of timeslice in ns
    uint64_t fCurrentCycle     = 0;  ///< Current epoch cycle
    uint32_t fCurrentEpoch     = 0;  ///< Current epoch number within epoch cycle
    uint64_t fCurrentEpochTime = 0;  ///< Unix time of current epoch in clock cycles
    uint64_t fCycleLength      = 0;  ///< Epoch cycle length in ns

    std::unique_ptr<UnpackStsPar> fParams = nullptr;  ///< Parameter container
  };


} /* namespace cbm::algo */

#endif /* CBM_ALGO_UNPACKSTS_H */
