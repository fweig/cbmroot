/* Copyright (C) 2010 - 2021 Goethe-University Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig, Alexandru Bercuci */

/**
 * @file CbmTrdUnpackAlgoFasp2D.h
 * @author Alexandru Bercuci
 * @author Pascal Raisig (praisig@ikf.uni-frankfurt.de)
 * @brief Trd FASP unpacking algorithm
 * @version 0.1
 * @date 2021-04-21
 * 
 * @copyright Copyright (c) 2021
 * 
 * This is the base class for the algorithmic part of the tsa data unpacking 
 * processes of the CbmTrd.
 * The actual translation from tsa to digi happens in the derived classes. 
 * 
 * 
*/

#ifndef CbmTrdUnpackAlgoFasp2D_H
#define CbmTrdUnpackAlgoFasp2D_H

#include "CbmRecoUnpackAlgo.tmpl"
#include "CbmTrdDigi.h"

#include "Timeslice.hpp"  // timeslice

#include <Rtypes.h>  // for types
#include <RtypesCore.h>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

#define NCRI 40  // no of CRI in the system (1/TRD-2D_FASP module)
#define NCOLS 8  // no of cols / FASP

class CbmTrdUnpackAlgoFasp2D : public CbmRecoUnpackAlgo<CbmTrdDigi> {
public:
  /** @brief Bytes per FASP frame stored in the microslices (32 bits words) 
   * - DATA WORD -
   * ffff.ffdd dddd.dddd dddd.tttt ttta.cccc
   * f - FASP id
   * d - ADC signal
   * t - time label inside epoch
   * a - word type (1)
   * c - channel id
   * - EPOCH WORD -
   * ffff.fftt tttt.tttt tttt.tttt ttta.cccc
   * f - FASP id
   * t - epoch index 
   * a - word type (0)
   * c - channel id
   */
  enum CbmTrdFaspMessage
  {
    kMessCh    = 4,
    kMessType  = 1,
    kMessTlab  = 7,
    kMessData  = 14,
    kMessFasp  = 6,
    kMessEpoch = 21
  };
  enum CbmTrdFaspMessageType
  {
    kEpoch = 0,
    kData
  };

  /** @brief Create the Cbm Trd Unpack AlgoBase object */
  CbmTrdUnpackAlgoFasp2D();

  /** @brief Destroy the Cbm Trd Unpack Task object */
  virtual ~CbmTrdUnpackAlgoFasp2D();

  /** @brief Copy constructor - not implemented **/
  CbmTrdUnpackAlgoFasp2D(const CbmTrdUnpackAlgoFasp2D&) = delete;

  /** @brief Assignment operator - not implemented **/
  CbmTrdUnpackAlgoFasp2D& operator=(const CbmTrdUnpackAlgoFasp2D&) = delete;


  /** @brief Data structure for unpacking the FASP word */
  struct CbmTrdFaspContent {
    uint8_t ch;
    uint8_t type;
    uint8_t tlab;
    uint16_t data;
    uint32_t epoch;
    uint8_t fasp;
    uint8_t cri;
  };

  // Setters

protected:
  /** @brief Get message type from the FASP word */
  CbmTrdFaspMessageType mess_type(uint32_t wd);
  /** @brief Convert the FASP word into a DATA message */
  void mess_readDW(uint32_t wd, CbmTrdFaspContent* mess);
  /** @brief Convert the FASP word into a EPOCH message */
  void mess_readEW(uint32_t wd, CbmTrdFaspContent* mess);
  /** @brief Print FASP message */
  void mess_prt(CbmTrdFaspContent* mess);
  bool pushDigis(std::map<UChar_t, std::vector<CbmTrdUnpackAlgoFasp2D::CbmTrdFaspContent*>> digis);
  ULong64_t fTime[NCRI];

  /** @brief Finish function for this algorithm base clase */
  void finish() { return; }

  /**
   * @brief Additional initialisation function for all BaseR derived algorithms.
   * 
   * @return Bool_t initOk 
  */
  virtual Bool_t init() { return kTRUE; }

  // Initialise par set, the base function handles the casting to distribute the pointers to their explicit functions

  /**
   * @brief Handles the distribution of the hidden derived classes to their explicit functions.
   * 
   * @param parset 
   * @return Bool_t initOk 
  */
  Bool_t initParSet(FairParGenericSet* /*parset*/) { return kTRUE; }


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

  // Constants
  /** @brief Bytes per FASP frame stored in the microslices (32 bits words) 
   * - DATA WORD -
   * ffff.ffdd dddd.dddd dddd.tttt ttta.cccc
   * f - FASP id
   * d - ADC signal
   * t - time label inside epoch
   * a - word type (1)
   * c - channel id
   * - EPOCH WORD -
   * ffff.fftt tttt.tttt tttt.tttt ttta.cccc
   * f - FASP id
   * t - epoch index 
   * a - word type (0)
   * c - channel id
   */
  static const std::uint8_t fBytesPerWord = 4;

private:
  void prt_wd(uint32_t w);

  ClassDef(CbmTrdUnpackAlgoFasp2D, 2)
};

#endif  // CbmTrdUnpackAlgoFasp2D_H
