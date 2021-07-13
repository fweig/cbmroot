/* Copyright (C) 2010 - 2021 Goethe-University Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig, David Schledt */

/**
 * @file CbmTrdUnpackAlgoR.h
 * @author Pascal Raisig (praisig@ikf.uni-frankfurt.de)
 * @author David Schledt
 * @brief Unpacker algorithm for the TrdR 2021 data
 * @version 1.0
 * @date 2021-04-21
 * 
 * @copyright Copyright (c) 2021
 * 
 * Implementation of the TrdR unpacker currently valid from mcbm 2021 and after
 * 
*/

#ifndef CbmTrdUnpackAlgoR_H
#define CbmTrdUnpackAlgoR_H

#include "CbmTrdRawMessageSpadic.h"
#include "CbmTrdUnpackAlgoBaseR.h"

#include <MicrosliceDescriptor.hpp>  // fles subsystemId
#include <Timeslice.hpp>             // timeslice

#include <FairTask.h>  // for InitStatus

#include <Rtypes.h>  // for types
#include <RtypesCore.h>

#include <cstdint>
#include <vector>

#include <cmath>

class CbmTrdUnpackAlgoR : public CbmTrdUnpackAlgoBaseR {

private:
  /* data */

public:
  /** @brief Create the Cbm Trd Unpack AlgoBase object */
  CbmTrdUnpackAlgoR(/* args */);

  /** @brief Destroy the Cbm Trd Unpack Task object */
  virtual ~CbmTrdUnpackAlgoR();

  /** @brief Copy constructor - not implemented **/
  CbmTrdUnpackAlgoR(const CbmTrdUnpackAlgoR&) = delete;

  /** @brief Assignment operator - not implemented **/
  CbmTrdUnpackAlgoR& operator=(const CbmTrdUnpackAlgoR&) = delete;

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
   * @brief Digest the aditional flags stored in the 4 "cccc" bits of the EPO messages.
   * @param frame 
   * @param criId id of the cri that send the µSlice
   * @param criobId id of the crob that send the µSlice (currently not used set to 0 062021 PR)
   * @param elinkId id of the elink from which the info message frame came
   * @return Spadic::MsInfoType
  */
  Spadic::MsInfoType digestBufInfoFlags(const std::uint32_t frame, std::uint16_t criId, std::uint8_t crobId,
                                        std::uint16_t elinkId);

  /**
   * @brief Digest a info message run all default information forwarding from the msg.
   * @param frame 
   * @param criId id of the cri that send the µSlice
   * @param criobId id of the crob that send the µSlice (currently not used set to 0 062021 PR)
   * @param elinkId id of the elink from which the info message frame came
  */
  void digestInfoMsg(const std::uint32_t frame, std::uint16_t criId, std::uint8_t crobId, std::uint16_t elinkId);

  /**
   * @brief Digest the flags of the currently unpacked µSlice.
   * @param flags flags stored in the µSlice descriptor 
   * @param criId id of the cri that send the µSlice
   * @param criobId id of the crob that send the µSlice (currently not used set to 0 062021 PR)
  */
  void digestMsFlags(const std::uint16_t flags, std::uint16_t criId, std::uint8_t crobId);

  /** @brief Extract all adc samples from a given adcbuffer container */

  /**
   * @brief Extract one adc sample from a given adcbuffer
   * 
   * @param[in] adcbuffer 
   * @param[in,out] nadcbits 
   * @return std::int16_t 
  */
  std::int16_t extractSample(size_t* adcbuffer, size_t* nadcbits);

  /**
   * @brief Extract the baseline average sample from a given adcbuffer.
   * Depending on the Spadic settings sample-0 is a plain sample or the averaged 
   * baseline calculation. The latter is not a 9 bit signed integer, but a 9 bit 
   * floating point number 7 digits before the point and 2 afterwards.
   * @param[in] adcbuffer 
   * @param[in,out] nadcbits 
   * @return std::float_t
  */
  std::float_t extractAvgSample(size_t* adcbuffer, size_t* nadcbits);

  /** @brief Additional explicit finish function of this algo implementation. */
  void finishDerived();

  /** @brief Identify the InfoType of a 64bit InfoMessage word inside a Microslice */
  Spadic::MsInfoType getInfoType(const std::uint32_t frame, std::uint16_t criId, std::uint8_t crobId,
                                 std::uint16_t elinkId);

  /** @brief Identify the message type of a given 32bit frame inside a Microslice */
  Spadic::MsMessageType getMessageType(const std::uint32_t frame);

  /**
   * @brief Get the ts_msb information from the TS_MSB(kEPO) frame. We take the first of the 3
   * The 3 redundant TS_MSB sets are already compared at the FPGA level.
   * @param frame
   * @return ts_msb value
  */
  std::uint8_t getTsMsb(const std::uint32_t frame);

  /**
   * @brief Create a CbmTrdRawMessageSpadic from the hit message input. 
   * 
   * @param word 
   * @param criId id of the cri that send the µSlice
   * @param criobId id of the crob that send the µSlice (currently not used set to 0 062021 PR)
   * @param istream
   * @return CbmTrdRawMessageSpadic 
   *
   * @todo Check if we can get rid of the future obsolete microslice stuff.
  */
  virtual CbmTrdRawMessageSpadic makeRaw(const std::uint32_t frame, std::uint16_t criId, std::uint8_t crobId,
                                         std::uint16_t elinkId, std::uint8_t istream);

  /**
   * @brief Create an actual digi from the raw message
   * 
   * @param raw 
  */
  void makeDigi(CbmTrdRawMessageSpadic raw);

  /** @brief Up to now we do not need this function for this algorithm */
  bool setDerivedTsParameters(size_t /*itimeslice*/) { return true; }

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
  bool unpack(const fles::Timeslice* ts, std::uint16_t icomp, UInt_t imslice);

  // Parameter storage members

  /** @brief Counter for the ts_msb used to reconstruct the time */
  std::vector<std::uint8_t> fNrTsMsbVec = {};

  /** @brief Counter for the ts_msb used to reconstruct the time */
  size_t fNrNonMajorTsMsb = 0;

  /** @brief Number of SOM to RDA/EOM mismatches */
  size_t fNrElinkMis = 0;

  /** @brief Start time of the current µSlice in Spadic CC */
  size_t fMsStartTimeCC = 0;

  // Constants
  /** @brief Bytes per spadic frame stored in the microslices */
  static const std::uint8_t fBytesPerWord = 8;
  /** @brief Number of streams per word
   * For the msg format used from 2021 ongoing we have 2 parallel streams per word. All data from eLinks 0..20 go to one stream and 21..41 to the other */
  static const std::uint8_t fStreamsPerWord = 2;


private:
  ClassDef(CbmTrdUnpackAlgoR, 2)
};

#endif  // CbmTrdUnpackAlgoR_H
