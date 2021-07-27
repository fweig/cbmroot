/* Copyright (C) 2010 - 2021 Goethe-University Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig */

/**
 * @file CbmTrdUnpackAlgoBaseR.h
 * @author Pascal Raisig (praisig@ikf.uni-frankfurt.de)
 * @brief Baseclass for the TrdR unpacker algorithms
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

#ifndef CbmTrdUnpackAlgoBaseR_H
#define CbmTrdUnpackAlgoBaseR_H

#include "CbmMcbm2020TrdTshiftPar.h"
#include "CbmRecoUnpackAlgo.tmpl"
#include "CbmTrdDigi.h"
#include "CbmTrdParSetAsic.h"
#include "CbmTrdRawMessageSpadic.h"
#include "CbmTrdRawToDigiBaseR.h"
#include "CbmTrdSpadic.h"
#include "CbmTrdUnpackMonitor.h"

#include "Timeslice.hpp"  // timeslice

#include <FairTask.h>  // for InitStatus

#include <Rtypes.h>  // for types
#include <RtypesCore.h>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

class CbmTrdUnpackAlgoBaseR : public CbmRecoUnpackAlgo<CbmTrdDigi, CbmTrdRawMessageSpadic, std::pair<size_t, size_t>> {
public:
  /** @brief Create the Cbm Trd Unpack AlgoBase object */
  CbmTrdUnpackAlgoBaseR(std::string name);

  /** @brief Destroy the Cbm Trd Unpack Task object */
  virtual ~CbmTrdUnpackAlgoBaseR();

  /** @brief Copy constructor - not implemented **/
  CbmTrdUnpackAlgoBaseR(const CbmTrdUnpackAlgoBaseR&) = delete;

  /** @brief Assignment operator - not implemented **/
  CbmTrdUnpackAlgoBaseR& operator=(const CbmTrdUnpackAlgoBaseR&) = delete;

  // Setters
  /** @brief Set a predefined monitor @param monitor predefined unpacking monitor */
  void SetMonitor(std::shared_ptr<CbmTrdUnpackMonitor> monitor) { fMonitor = monitor; }

  /**
   * @brief Set the Raw To Digi Method
   * 
   * @param value 
  */
  void SetRawToDigiMethod(std::shared_ptr<CbmTrdRawToDigiBaseR> value) { fRTDMethod = value; }

  /**
   * @brief Set the Spadic Object
   * 
   * @param value 
  */
  void SetSpadicObject(std::shared_ptr<CbmTrdSpadic> value) { fSpadic = value; }

protected:
  /**
   * @brief Handle the output created by the explicit algorithms. E.g. write to output vectors.
   * 
   * @param digi 
   * @param raw 
  */
  void digestOutput(std::unique_ptr<CbmTrdDigi> digi, CbmTrdRawMessageSpadic raw);

  /** @brief Finish function for this algorithm base clase */
  void finish()
  {
    finishDerived();
    // Finish the monitor if we have one
    if (fMonitor) fMonitor->Finish();
  }

  /** @brief Function that allows special calls during Finish in the derived algos */
  virtual void finishDerived() = 0;

  /**
   * @brief Get the Asic Address (CbmAddress scheme) for the given hardware Ids
   * 
   * @param criid 
   * @param crobid 
   * @param elinkid 
   * @return std::uint32_t 
  */
  virtual std::uint32_t getAsicAddress(std::uint32_t criid, std::uint32_t crobid, std::uint32_t elinkid);

  /**
   * @brief Get the Channel Id (CbmAddress scheme) for the given hardware Ids
   * 
   * @param asicaddress 
   * @param elinkid 
   * @param elinkchannelid 
   * @return std::uint32_t 
  */
  std::uint32_t getChannelId(std::uint32_t asicaddress, std::uint32_t elinkid, std::uint32_t elinkchannelid);

  /**
   * @brief Additional initialisation function for all BaseR derived algorithms.
   * 
   * @return Bool_t initOk 
  */
  virtual Bool_t init();

  // Initialise par set, the base function handles the casting to distribute the pointers to their explicit functions

  /**
   * @brief Handles the distribution of the hidden derived classes to their explicit functions.
   * 
   * @param parset 
   * @return Bool_t initOk 
  */
  Bool_t initParSet(FairParGenericSet* parset);

  /**
   * @brief Transfer parameters from ParSetAsic container to members.
   *  
   * Currently we mainly transfer here the addressing, other parameters are yet to come.
   *  
   * @param parset 
   * @return Bool_t initOk 
  */
  Bool_t initParSet(CbmTrdParSetAsic* parset);

  /**
   * @brief Transfer parameters from ParSetDigi container to members.
   *  
   * The monitor needs this to extract module informations, e.g. ncols and nrows.
   *  
   * @param parset 
   * @return Bool_t initOk 
  */
  Bool_t initParSet(CbmTrdParSetDigi* parset);

  /**
   * @brief Transfer parameters from CbmMcbm2020TrdTshiftPar container to members.
   *  
   * These are mCBM 2020 specific parameters needed to correct in run timeshifts. 
   *  
   * @param parset 
   * @return Bool_t initOk 
  */
  Bool_t initParSet(CbmMcbm2020TrdTshiftPar* parset);


  // Monitoring
  /** @brief Potential (online) monitor for the unpacking process */
  std::shared_ptr<CbmTrdUnpackMonitor> fMonitor = nullptr;

  // Parameter storage members
  /** @brief Spadic software reprensentation object */
  std::shared_ptr<CbmTrdSpadic> fSpadic = nullptr;

  /** @brief raw to digi extraction method, set in the task */
  std::shared_ptr<CbmTrdRawToDigiBaseR> fRTDMethod = nullptr;

  /** @brief Map to retrieve asic address from CriId/CrobId/ElinkId (see CbmTrdHardwareSetupR) */
  std::map<size_t, Int_t> fSpadicAddressMap = {};

  /** @brief Map to retrieve module channelId from asicAddress and asicChannel */
  std::map<Int_t, std::vector<Int_t>> fAsicChannelMap = {};

  /** @brief Map containing the timeshift parameters for the correction of the µSlice timeshifts. The keys are the tsIdx, if no key is found, the shifts of the previous tsIdx are used again */
  std::map<size_t, std::vector<Int_t>> fTimeshiftsMap = {};

  /** @brief Define if the first 16 channels (00..15) are found on the even (set true) or odd (false) eLinkId.
   * Default for mCbm2020 is false thus, initialized as false */
  Bool_t fIsFirstChannelsElinkEven = kFALSE;

  /** @brief Number of rda frames outside of a SOM frame range */
  size_t fNrWildRda = 0;

  /** @brief Number of eom frames outside of a SOM frame range */
  size_t fNrWildEom = 0;

  /** @brief Number of missing EOM frames to finish a SOM frame */
  size_t fNrMissingEom = 0;

  /** @brief Number of wild null words, should only appear at the end of a µSlice */
  size_t fNrWildNul = 0;

  /** @brief Number of unknown words */
  size_t fNrUnknownWords = 0;

  /** @brief length of one ts_msb in [ns] */
  static constexpr std::uint16_t fTsMsbLength = 16000;

  /** @brief length of one ts_msb in [cc] */
  size_t fTsMsbLengthCC = fTsMsbLength / CbmTrdSpadic::GetClockCycle();

private:
  ClassDef(CbmTrdUnpackAlgoBaseR, 2)
};

#endif  // CbmTrdUnpackAlgoBaseR_H
