/* Copyright (C) 2010 - 2021 Goethe-University Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig */

/**
 * @file CbmTrdUnpackAlgoLegacy2020R.h
 * @author Pascal Raisig (praisig@ikf.uni-frankfurt.de)
 * @brief Unpacker algorithms for the TrdR 2020 data
 * @version 0.1
 * @date 2021-04-21
 * 
 * @copyright Copyright (c) 2021
 * 
 * Implementation of the TrdR unpacker for the Spadic v2.2 data from 2020.
 * See https://git.cbm.gsi.de/dschledt/dpb_design/-/blob/57fca5f2/project/spadicDPB_realMS/src/spadicDPB_realMS.vhd#L673
 * 
*/

#ifndef CbmTrdUnpackAlgoLegacy2020R_H
#define CbmTrdUnpackAlgoLegacy2020R_H

#include "CbmMcbm2020TrdTshiftPar.h"
#include "CbmTrdRawMessageSpadic.h"
#include "CbmTrdSpadic.h"
#include "CbmTrdUnpackAlgoBaseR.h"

#include "Timeslice.hpp"             // timeslice
#include <MicrosliceDescriptor.hpp>  // fles subsystemId

#include <FairTask.h>  // for InitStatus

#include <Rtypes.h>  // for types
#include <RtypesCore.h>

#include <cstddef>
#include <cstdint>

class CbmTrdUnpackAlgoLegacy2020R : public CbmTrdUnpackAlgoBaseR {

private:
  /* data */

public:
  /** @brief Create the Cbm Trd Unpack AlgoBase object */
  CbmTrdUnpackAlgoLegacy2020R(/* args */);

  /** @brief Destroy the Cbm Trd Unpack Task object */
  virtual ~CbmTrdUnpackAlgoLegacy2020R();

  /** @brief Copy constructor - not implemented **/
  CbmTrdUnpackAlgoLegacy2020R(const CbmTrdUnpackAlgoLegacy2020R&) = delete;

  /** @brief Assignment operator - not implemented **/
  CbmTrdUnpackAlgoLegacy2020R& operator=(const CbmTrdUnpackAlgoLegacy2020R&) = delete;

  // Getters
  /**
   * @brief Get the requested parameter containers.
   * Return the required parameter containers together with the paths to the ascii 
   * files to.
   *  
   * @param[in] std::string geoTag as used in CbmSetup
   * @param[in] std::uint32_t runId for runwise defined parameters
   * @return fParContVec
  */
  virtual std::vector<std::pair<std::string, std::shared_ptr<FairParGenericSet>>>*
  GetParContainerRequest(std::string geoTag, std::uint32_t runId);

protected:
  /**
   * @brief Extract the epoch time reference information from the epoch word
   * 
   * @param word 
   * @return size_t 
  */
  size_t extractEpoch(const size_t word);

  /**
   * @brief Extract the given sample from the given rda word
   * 
   *  @param word the 64bit Message Word
   *  @param sample Which sample to extract [0,31]
   *  @param multihit Set to true if current word belongs to a multihit message.
   *  @return The ADC value of the given sample, range [-255,255]. -256 if an error occurred.
  */
  std::int16_t extractSample(const size_t word, const Spadic::MsMessageType msgType, std::uint32_t isample,
                             bool multihit = false);

  const std::vector<std::uint8_t> fExtractSampleIndicesVec = {4, 5, 6, 0, 1, 2, 3, 4, 5, 6, 0, 1, 2, 3, 4, 5,
                                                              6, 0, 1, 2, 3, 4, 5, 6, 0, 1, 2, 3, 4, 5, 6, 0};

  /** @brief Additional explicit finish function of this algo implementation. */
  void finishDerived();

  /** @brief Identify the InfoType of a 64bit InfoMessage word inside a Microslice */
  Spadic::MsInfoType getInfoType(const size_t msg);

  /** @brief Identify the message type of a 64bit word inside a Microslice */
  Spadic::MsMessageType getMessageType(const size_t msg);

  /**
   * @brief Get the Nr Of Rda Words required for the amount of ADC samples
   * 
   * @param nsamples 
   * @return std::uint8_t 
  */
  std::uint8_t getNrOfRdaWords(std::uint8_t nsamples);

  /**
   * @brief Create a CbmTrdRawMessageSpadic from the hit message input. 
   * 
   * @param word 
   * @param msDesc 
   * @return CbmTrdRawMessageSpadic 
   *
   * @todo Check if we can get rid of the future obsolete microslice stuff.
  */
  virtual CbmTrdRawMessageSpadic makeRaw(const size_t word, fles::MicrosliceDescriptor msDesc);

  /**
   * @brief Create an actual digi from the raw message
   * 
   * @param raw 
   * @return std::shared_ptr<CbmTrdDigi> 
  */
  void makeDigi(CbmTrdRawMessageSpadic raw);

  /**
   * @brief Set the Derived Ts Parameters
   * 
   * In this function parameters required by the explicit algo connected to the timeslice are set.
   * 
   * @param itimeslice 
   * @return true 
   * @return false 
  */
  virtual bool setDerivedTsParameters(size_t itimeslice);

  /**
   * @brief Unpack a given microslice.
   * 
   * @param ts timeslice pointer
   * @param icomp index to the component to be unpacked
   * @param imslice index of the microslice to be unpacked
   * @return true 
   * @return false 
   * 
   * @remark The content of the µslice can only be accessed via the timeslice. Hence, we need to pass the pointer to the full timeslice
  */
  virtual bool unpack(const fles::Timeslice* ts, std::uint16_t icomp, UInt_t imslice);

  // Parameter storage members

  /** @brief Vector containing the timeshift parameters for the correction of the µSlice timeshifts for a given tsIdx.*/
  std::vector<std::int32_t>* fTimeshiftsParVec = nullptr;

  /** @brief Time of the current epoch marker */
  size_t fEpoch = 0;

  /** @brief Microslice length [CC]. To be overwritten in the version specific unpackers. */
  size_t fMsLengthCC = 1.28e6 / CbmTrdSpadic::GetClockCycle();

  // Constants
  /** @brief Bytes per word stored in the microslices */
  static const std::uint8_t fBytesPerWord = 8;

private:
  ClassDef(CbmTrdUnpackAlgoLegacy2020R, 2)
};

#endif  // CbmTrdUnpackAlgoLegacy2020R_H
