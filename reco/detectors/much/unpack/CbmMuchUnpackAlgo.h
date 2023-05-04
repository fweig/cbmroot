/* Copyright (C) 2022 Fair GmbH, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

/**
 * @file CbmMuchUnpackAlgo.h
 * @brief MUCH unpacker algorithms
 * @version 0.1
 * @date 2022-02-22
 *
 *
 * This is the algorithmic part of the tsa data unpacking processes of the CbmMuch.
 *
 *
*/

#ifndef CbmMuchUnpackAlgo_H
#define CbmMuchUnpackAlgo_H


#include "CbmErrorMessage.h"  // REMARK see remark in CbmMuchUnpackConfig
#include "CbmMuchDigi.h"
#include "CbmMuchUnpackMonitor.h"
#include "CbmMuchUnpackPar.h"
#include "CbmRecoUnpackAlgo.tmpl"

#include "Timeslice.hpp"  // timeslice

#include <FairTask.h>  // for InitStatus

#include <Rtypes.h>  // for types
#include <RtypesCore.h>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>
#include <set>

class CbmMuchUnpackAlgo : public CbmRecoUnpackAlgo<CbmMuchDigi, CbmMuchDigi, CbmErrorMessage> {
public:
  /** @brief Create the Cbm Much Unpack Algo object */
  CbmMuchUnpackAlgo();

  /** @brief Create the Cbm Much Unpack Algo object */
  CbmMuchUnpackAlgo(std::string name);

  /** @brief Destroy the Cbm Much Unpack Algo object */
  virtual ~CbmMuchUnpackAlgo();

  /** @brief Copy constructor - not implemented **/
  CbmMuchUnpackAlgo(const CbmMuchUnpackAlgo&) = delete;

  /** @brief Assignment operator - not implemented **/
  CbmMuchUnpackAlgo& operator=(const CbmMuchUnpackAlgo&) = delete;

  // Setters

  /** @brief Set the minimum adc cut value @param[in] value */
  void SetMinAdcCut(uint32_t value) { fdAdcCut = value; }

  void SetMinAdcCut(uint32_t febid, uint32_t value) { fdAdcCut_perFeb[febid] = value; }

  /** @brief Set the time offset per Asic */
  void SetAsicTimeOffsetVec(std::vector<double> value) { fvdTimeOffsetNsAsics.swap(value); }

  /** @brief Enable/Disable the duplicate digis rejection, without or with same ADC checks */
  void SetDuplicatesRejection(bool bIn = true, bool bDiffAdc = true)
  {
    fbRejectDuplicateDigis = bIn;
    fbDupliWithoutAdc      = bDiffAdc;
  }

  /**
   * @brief Get the requested parameter containers. To be defined in the derived classes!
   * Return the required parameter containers together with the paths to the ascii
   * files to.
   *
   * @param[in] std::string geoTag as used in CbmSetup
   * @param[in] std::uint32_t runId for runwise defined parameters
   * @return fParContVec
  */
  virtual std::vector<std::pair<std::string, std::shared_ptr<FairParGenericSet>>>*
  GetParContainerRequest(std::string geoTag, std::uint32_t runId);

  /**
   * @brief Mask a Noisy Channel
   *
   * @param uFeb
   * @param uChan
   * @param bMasked
  */
  void MaskNoisyChannel(const uint32_t uFeb, const uint32_t uChan, const bool bMasked = true);

  /** @brief Set a predefined monitor @param monitor predefined unpacking monitor */
  void SetMonitor(std::shared_ptr<CbmMuchUnpackMonitor> monitor) { fMonitor = monitor; }

  /**
   * @brief Sets the name of the parameter file to be used.
   *
   * @param[in] std:string, path should not be included as set in the Config class
  */
  void SetParFileName(std::string sNewName) { fParFileName = sNewName; }

  void SetInactiveChannel(uint32_t channel){  fInactiveChannels.insert(channel);}

protected:
  /**
   * @brief Get the Asic Index
   *
   * @param dpbidx
   * @param crobidx
   * @param elinkidx
  */
  uint32_t getFebIndex(uint32_t dpbidx, uint32_t crobidx, uint16_t elinkidx);
  /** @brief Finish function for this algorithm base clase */
  virtual void finish()
  {
    if (fMonitor) fMonitor->Finish();
    LOG(info) << "Missed messages:  wrong address generated: " << fuSkippMessNoValidAddress;
    LOG(info) << "Missed messages: wrong FEB Position inPar: " << fuSkippMessNoValidFebPosition;
    LOG(info) << "Missed messages:  wrong Elink   generated: " << fuSkippMessNoValidElink;
    return;
  }

  /**
   * @brief Get the Full Time Stamp from raw time stamp
   *
   * @param usRawTs
   * @return uint64_t
  */
  uint64_t getFullTimeStamp(const uint16_t usRawTs);

  /**
   * @brief Intialisation at begin of run. Special inits of the derived algos.
   *
   * @retval Bool_t initOk
  */
  Bool_t init();

  /** @brief Initialize the DpbIdIndexMap with the information from the parset @param[in] parset parameter set for the Much unpacker */
  void initDpbIdIndexMap(CbmMuchUnpackPar* parset);

  /** @brief experts please add description here */
  void initInternalStatus(CbmMuchUnpackPar* parset);

  /**
   * @brief Handles the distribution of the hidden derived classes to their explicit functions.
   *
   * @param parset
   * @return Bool_t initOk
  */
  Bool_t initParSet(FairParGenericSet* parset);

  /**
   * @brief Initialize the parameters from CbmMuchUnpackPar.
   *
   * @param parset
   * @return Bool_t initOk
  */
  Bool_t initParSet(CbmMuchUnpackPar* parset);

  /** @brief Initialize and transfer the informations to the parameters storage vectors */

  /**
   * @brief Main loop over the sts xyter messages in the µSlices
   *
   * @param msContent
   * @param uSize
   * @param uMsIdx
   * @param bError
   * @param sMessPatt
   * @param vNbMessType
  */
  void loopMsMessages(const uint8_t* msContent, const uint32_t uSize, const size_t uMsIdx);

  /** @brief experts please add description */
  void printActiveCrobs(CbmMuchUnpackPar* parset, const std::vector<std::vector<bool>>& vbCrobActiveFlag);

  /** @brief experts please add description */
  void printAddressMaps(CbmMuchUnpackPar* parset,
                        const std::vector<std::vector<std::vector<int32_t>>>& /*viFebModuleIdx*/,
                        const std::vector<std::vector<std::vector<int32_t>>>& /*viFebModuleSide*/);

  /** @brief experts please add description marked as not used currently*/
  void processEpochInfo(const stsxyter::Message& /*mess*/) { return; };

  /** @brief experts please add description */
  void processErrorInfo(const stsxyter::Message& mess);

  /** @brief Process the information of the hit message and create a MuchDigi from it */
  void processHitInfo(const stsxyter::Message& mess);

  /** @brief experts please add description */
  void processStatusInfo(const stsxyter::Message& mess, uint32_t uIdx);

  /** @brief experts please add description */
  void processTsMsbInfo(const stsxyter::Message& mess, uint32_t uMessIdx, uint32_t uMsIdx);

  /** @brief experts please add description here */
  void refreshTsMsbFields(const uint32_t imslice, const size_t mstime);
  uint32_t CreateMuchAddress(uint32_t dpbidx, int32_t febId, uint32_t usChan);

  /**
   * @brief Set the Derived Ts Parameters
   *
   * In this function parameters required by the explicit algo connected to the timeslice can be set.
   *
   * @param itimeslice
   * @return true
   * @return false
  */
  bool setDerivedTsParameters(size_t /*itimeslice*/) { return true; }

  /**
   * @brief Unpack a given microslice. To be implemented in the derived unpacker algos.
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


  // Settings
  /** @brief Minimum adc cut to store a hit */
  uint32_t fdAdcCut = 0;

  /** @brief Minimum adc cut per Feb to store a hit */
  std::map<uint32_t, uint32_t> fdAdcCut_perFeb;

  /** @brief Time offsets per Asic??? @todo expert confirmation required */
  std::vector<double> fvdTimeOffsetNsAsics = {};

  /** @brief Enables the rejection of duplicate digis */
  bool fbRejectDuplicateDigis = false;

  /** @brief If rejecting duplicate digis, enables rejection even if ADC differs*/
  bool fbDupliWithoutAdc = true;

  /** @brief flag if channel mask is to be used or not. Set automatically via MaskNoisyChannels */
  bool fbUseChannelMask = false;

  /** @brief Vector of channel masks, [ NbFeb ][ NbCHanInFeb ], used only if fbUseChannelMask is true */
  std::vector<std::vector<bool>> fvvbMaskedChannels = {};  //!


  // Parameter members
  /** @brief Map of DPB Identifier to DPB index */
  std::map<uint32_t, uint32_t> fDpbIdIndexMap = {};  //!

  /** @brief Current dpb id */
  uint32_t fuCurrDpbIdx = 0;

  /** @brief Masked components to print out missing component only once */
  std::vector<bool> fvbMaskedComponents;

  /** @brief Number of FEBs with StsXyter ASICs */
  uint32_t fuNbFebs = 0;  //!

  /** @brief ADC cuts for FEBs */
  std::vector<uint32_t> fvbFebAdcCut = {};

  /** @brief Number of eLinks per CROB */
  uint32_t fNrElinksPerCrob = 0;  //!

  /** @brief Number of ASICs per CROB */
  uint32_t fNrAsicsPerCrob = 0;  //!

  /** @brief Number of ASICs per FEB */
  uint32_t fNrAsicsPerFeb = 0;  //!

  /** @brief Number of Channels per Asic */
  uint32_t fNrChsPerAsic = 0;  //!

  /** @brief Number of Channels per FEB */
  uint32_t fNrChsPerFeb = 0;  //!

  /** @brief Number of CROBs per DPB */
  uint32_t fNrCrobPerDpb = 0;  //!

  /** @brief Number of FEBs per CROB */
  uint32_t fNrFebsPerCrob = 0;  //!

  /** @brief Vector used for the translation between eLink index and FEB index*/
  std::vector<int> fElinkIdxToFebIdxVec = {};

  /** @brief Vector used for the translation between eLink index and Asic index first is feb type A second is feb type b*/
  std::vector<std::pair<uint32_t, uint32_t>> fElinkIdxToAsicIdxVec = {};

  /** @brief  FEB type, [ NbDpb ][ NbCrobPerDpb ][ NbFebsPerCrob ], 0 = A, 1 = B, -1 if inactive */
  std::vector<std::vector<std::vector<int32_t>>> fviFebType = {};  //!

  /** @brief Pulser flag for each FEB, [ NbDpb * NbCrobPerDpb * NbFebsPerCrob ] */
  std::vector<bool> fvbFebPulser;  //!
  /** @brief STS address for each FEB, [ NbDpb * NbCrobPerDpb * NbFebsPerCrob ] */
  std::vector<int32_t> fviFebAddress;  //!
  /** @brief Module side for each FEB, [ NbDpb * NbCrobPerDpb * NbFebsPerCrob ] */
  std::vector<int32_t> fviFebSide;  //!
  /** @brief ADC gain in e-/b, [ NbDpb * NbCrobPerDpb * NbFebsPerCrob ] */
  std::vector<double> fvdFebAdcGain;  //!
  /** @brief ADC offset in e-, [ NbDpb * NbCrobPerDpb * NbFebsPerCrob ] */
  std::vector<double> fvdFebAdcOffs;  //!

  // Internal variables
  /** @brief Current TS MSB for each DPB */
  std::vector<uint64_t> fvulCurrentTsMsb = {};  //!
  /** @brief Current TS MSB cycle for DPB */
  std::vector<uint32_t> fvuCurrentTsMsbCycle = {};  //!

  /** @brief Start time of the TS expressed in TS_MSB instead of ns */
  uint64_t fulTsStartInTsMsb = 0;
  /** @brief Current TS MSB cycle for DPB relative to TS start */
  std::vector<uint64_t> fulTsMsbIndexInTs = {};

  /// Duplicate hits suppression
  static const uint32_t kuMaxTsMsbDiffDuplicates = 8;
  /** @brief TS of last hit message for each channel, [ AsicIdx ][ Chan ] */
  std::vector<std::vector<uint16_t>> fvvusLastTsChan = {};  //!
  /** @brief ADC of last hit message for each channel, [ AsicIdx ][ Chan ] */
  std::vector<std::vector<uint16_t>> fvvusLastAdcChan = {};  //!
  /** @brief TS MSB in TS of last hit message for each channel, [ AsicIdx ][ Chan ] */
  std::vector<std::vector<uint64_t>> fvvulLastTsMsbChan = {};  //!

  // Monitoring
  /** @brief Potential (online) monitor for the unpacking process */
  std::shared_ptr<CbmMuchUnpackMonitor> fMonitor = nullptr;

  /** @brief Current µSlice time */
  uint64_t fMsStartTime = 0;

  std::set<uint32_t> fInactiveChannels = {};  
  
  uint64_t fMaskedNoisyHitsFromCbmMuchAddress = 0;

private:
  CbmMuchUnpackPar* fUnpackPar           = nullptr;  //! For static/inline mapping functions
  uint64_t fuSkippMessNoValidAddress     = 0;
  uint64_t fuSkippMessNoValidFebPosition = 0;
  uint64_t fuSkippMessNoValidElink       = 0;
  /// Settings from parameter file
  std::string fParFileName = "mMuchPar.par";
  ClassDef(CbmMuchUnpackAlgo, 2)
};

#endif  // CbmMuchUnpackAlgo_H
