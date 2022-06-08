/* Copyright (C) 2021 Goethe-University Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig [committer], Alexandru Bercuci */

/**
 * @file CbmTrdUnpackFaspAlgo.h
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

#ifndef CbmTrdUnpackFaspAlgo_H
#define CbmTrdUnpackFaspAlgo_H

#include "CbmRecoUnpackAlgo.tmpl"
#include "CbmTrdDigi.h"
#include "CbmTrdParFasp.h"
#include "CbmTrdParSetAsic.h"
#include "CbmTrdUnpackFaspMonitor.h"

#include "Timeslice.hpp"  // timeslice

#include <Rtypes.h>  // for types
#include <RtypesCore.h>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

// TODO to be defined in correlation with time offsets (AB 20.05.22)
#define NCRI 40  // no of CRI in the system (5/TRD-2D_FASP module)
#define NCOLS 8  // no of cols / FASP

class CbmTrdParSetDigi;
class CbmTrdUnpackFaspAlgo : public CbmRecoUnpackAlgo<CbmTrdDigi> {
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
  CbmTrdUnpackFaspAlgo();

  /** @brief Destroy the Cbm Trd Unpack Task object */
  virtual ~CbmTrdUnpackFaspAlgo();

  /** @brief Copy constructor - not implemented **/
  CbmTrdUnpackFaspAlgo(const CbmTrdUnpackFaspAlgo&) = delete;

  /** @brief Assignment operator - not implemented **/
  CbmTrdUnpackFaspAlgo& operator=(const CbmTrdUnpackFaspAlgo&) = delete;


  /** @brief Data structure for unpacking the FASP word */
  struct CbmTrdFaspContent {
    uint8_t ch;      ///< ch id in the FASP
    uint8_t type;    ///< message type 0 = epoch, 1 = data (not used for the moment)
    uint8_t tlab;    ///< time of the digi inside the epoch
    uint16_t data;   ///< ADC value
    uint32_t epoch;  ///< epoch id (not used for the moment)
    uint32_t mod;    ///< full module address according to CbmTrdAddress
    uint8_t crob;    ///< CROB id in the module
    uint8_t fasp;    ///< FASP id in the module
  };

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
  void PrintAsicMapping();

  /** @brief Introduce fasp index mapping*/
  void SetAsicMapping(const std::map<uint32_t, uint8_t[NFASPMOD]>& map);
  /** @brief Initialize CROB mapping for all modules*/
  void SetCrobMapping(const std::map<uint32_t, uint16_t[NCROBMOD]>& map);
  /** @brief Set a predefined monitor 
   *  @param monitor predefined unpacking monitor */
  void SetMonitor(std::shared_ptr<CbmTrdUnpackFaspMonitor> monitor) { fMonitor = monitor; }

protected:
  /** @brief Get message type from the FASP word */
  CbmTrdFaspMessageType mess_type(uint32_t wd);
  /** @brief Convert the FASP word into a DATA message */
  void mess_readDW(uint32_t wd, CbmTrdFaspContent* mess);
  /** @brief Convert the FASP word into a EPOCH message */
  void mess_readEW(uint32_t wd, CbmTrdFaspContent* mess);
  /** @brief Print FASP message */
  void mess_prt(CbmTrdFaspContent* mess);
  bool pushDigis(std::vector<CbmTrdUnpackFaspAlgo::CbmTrdFaspContent*> digis);
  /** @brief Time offset for digi wrt the TS start, expressed in 80 MHz clks. It contains:
   *  - relative offset of the MS wrt the TS
   *  - FASP epoch offset for current CROB
   *  - TRD2D system offset wrt to experiment time (e.g. T0) 
   */
  ULong64_t fTime;

  /** @brief Finish function for this algorithm base clase */
  void finish()
  {
    if (fMonitor) fMonitor->Finish();
  }

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
  Bool_t initParSet(FairParGenericSet* parset);

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
  std::map<uint32_t, uint8_t[NFASPMOD]>* fFaspMap  = nullptr;  ///> FASP mapping update wrt the default setting
  std::map<uint32_t, uint16_t[NCROBMOD]>* fCrobMap = nullptr;  ///> CRI mapping setting
  std::map<uint32_t, std::vector<CbmTrdDigi*>> fDigiBuffer;    ///> Map of buffered digi per pad
  /** @brief Potential (online) monitor for the unpacking process */
  std::shared_ptr<CbmTrdUnpackFaspMonitor> fMonitor = nullptr;
  std::vector<Int_t> fModuleId;
  CbmTrdParSetAsic fAsicPar;
  CbmTrdParSetDigi* fDigiSet = nullptr;

  ClassDef(CbmTrdUnpackFaspAlgo, 3)  // unpack FASP read-out detectors
};

#endif  // CbmTrdUnpackFaspAlgo_H
