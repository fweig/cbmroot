/* Copyright (C) 2010 - 2021 Goethe-University Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig */

/**
 * @file CbmStsUnpackAlgo.h
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

#ifndef CbmStsUnpackAlgo_H
#define CbmStsUnpackAlgo_H

#include "CbmMcbm2018StsPar.h"
#include "CbmStsUnpackAlgoBase.h"


class CbmStsUnpackAlgo : public CbmStsUnpackAlgoBase {
public:
  /** @brief Create the Cbm Trd Unpack AlgoBase object */
  CbmStsUnpackAlgo();

  /** @brief Destroy the Cbm Trd Unpack Task object */
  virtual ~CbmStsUnpackAlgo();

  /** @brief Copy constructor - not implemented **/
  CbmStsUnpackAlgo(const CbmStsUnpackAlgo&) = delete;

  /** @brief Assignment operator - not implemented **/
  CbmStsUnpackAlgo& operator=(const CbmStsUnpackAlgo&) = delete;

  /**
   * @brief Mask a Noisy Channel
   *
   * @param uFeb
   * @param uChan
   * @param bMasked
  */
  virtual void MaskNoisyChannel(const uint32_t uFeb, const uint32_t uChan, const bool bMasked = true);

  /** @brief Set a predefined monitor @param monitor predefined unpacking monitor */
  void SetMonitor(std::shared_ptr<CbmStsUnpackMonitor> monitor) { fMonitor = monitor; }

protected:
  /**
   * @brief Get the Asic Index
   *
   * @param dpbidx
   * @param crobidx
   * @param elinkidx
  */
  uint32_t getAsicIndex(uint32_t dpbidx, uint32_t crobidx, uint16_t elinkidx);

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

  /** @brief Initialize the DpbIdIndexMap with the information from the parset @param[in] parset parameter set for the Sts unpacker */
  void initDpbIdIndexMap(CbmMcbm2018StsPar* parset);

  /** @brief experts please add description here */
  void initInternalStatus(CbmMcbm2018StsPar* parset);

  /**
   * @brief Handles the distribution of the hidden derived classes to their explicit functions.
   *
   * @param parset
   * @return Bool_t initOk
  */
  Bool_t initParSet(FairParGenericSet* parset);

  /**
   * @brief Initialize the parameters from CbmMcbm2018StsPar.
   *
   * @param parset
   * @return Bool_t initOk
  */
  Bool_t initParSet(CbmMcbm2018StsPar* parset);

  /** @brief Initialize and transfer the informations to the parameters storage vectors */
  void initTempVectors(CbmMcbm2018StsPar* parset, std::vector<int32_t>* viModuleType,
                       std::vector<int32_t>* viModAddress,
                       std::vector<std::vector<std::vector<int32_t>>>* viFebModuleIdx,
                       std::vector<std::vector<bool>>* vbCrobActiveFlag,
                       std::vector<std::vector<std::vector<int32_t>>>* viFebModuleSide);
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
  void printActiveCrobs(CbmMcbm2018StsPar* parset, const std::vector<std::vector<bool>>& vbCrobActiveFlag);

  /** @brief experts please add description */
  void printAddressMaps(CbmMcbm2018StsPar* parset, const std::vector<std::vector<std::vector<int32_t>>>& viFebModuleIdx,
                        const std::vector<std::vector<std::vector<int32_t>>>& viFebModuleSide);

  /** @brief experts please add description marked as not used currently*/
  void processEpochInfo(const stsxyter::Message& /*mess*/) { return; };

  /** @brief experts please add description */
  void processErrorInfo(const stsxyter::Message& mess);

  /** @brief Process the information of the hit message and create a StsDigi from it */
  void processHitInfo(const stsxyter::Message& mess);

  /** @brief experts please add description */
  void processStatusInfo(const stsxyter::Message& mess, uint32_t uIdx);

  /** @brief experts please add description */
  void processTsMsbInfo(const stsxyter::Message& mess, uint32_t uMessIdx, uint32_t uMsIdx);

  /** @brief experts please add description here */
  void refreshTsMsbFields(const uint32_t imslice, const size_t mstime);

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

  // Monitoring
  /** @brief Potential (online) monitor for the unpacking process */
  std::shared_ptr<CbmStsUnpackMonitor> fMonitor = nullptr;

  /** @brief Current µSlice time */
  uint64_t fMsStartTime = 0;

  // Parameter members
  /** @brief Map of DPB Identifier to DPB index */
  std::map<uint32_t, uint32_t> fDpbIdIndexMap = {};  //!

  /** @brief Current dpb id */
  uint32_t fuCurrDpbIdx = 0;

  /** @brief Masked components to print out missing component only once */
  std::vector<bool> fvbMaskedComponents;

  /** @brief Number of FEBs with StsXyter ASICs */
  uint32_t fuNbFebs = 0;  //!

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

  /** @brief flag if channel mask is to be used or not. Set automatically via MaskNoisyChannels */
  bool fbUseChannelMask = false;

  /** @brief Vector of channel masks, [ NbFeb ][ NbCHanInFeb ], used only if fbUseChannelMask is true */
  std::vector<std::vector<bool>> fvvbMaskedChannels = {};  //!

  /** @brief  FEB type, [ NbDpb ][ NbCrobPerDpb ][ NbFebsPerCrob ], 0 = A, 1 = B, -1 if inactive */
  std::vector<std::vector<std::vector<int32_t>>> fviFebType = {};  //!

  /** @brief STS address for each FEB, [ NbDpb * NbCrobPerDpb * NbFebsPerCrob ] */
  std::vector<int32_t> fviFebAddress;  //!
  /** @brief Module side for each FEB, [ NbDpb * NbCrobPerDpb * NbFebsPerCrob ] */
  std::vector<int32_t> fviFebSide;  //!
  /** @brief ADC gain in e-/b, [ NbDpb * NbCrobPerDpb * NbFebsPerCrob ] */
  std::vector<double> fvdFebAdcGain;  //!
  /** @brief ADC offset in e-, [ NbDpb * NbCrobPerDpb * NbFebsPerCrob ] */
  std::vector<double> fvdFebAdcOffs;  //!

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
  /** @brief TS MSB of last hit message for each channel, [ AsicIdx ][ Chan ] */
  std::vector<std::vector<uint16_t>> fvvusLastTsMsbChan = {};  //!
  /** @brief TS MSB cycle of last hit message for each channel, [ AsicIdx ][ Chan ] */
  std::vector<std::vector<uint16_t>> fvvusLastTsMsbCycleChan = {};  //!

  /** @brief Time of first valid hit (TS_MSB available), used as reference for evolution plots @todo move this to the monitor the algo does not need it!*/
  double fdStartTime = 0;

  /** @brief Time of first microslice, used as reference for evolution plots @todo move this to the monitor the algo does not need it!*/
  double fdStartTimeMsSz = 0;

private:
  ClassDef(CbmStsUnpackAlgo, 2)
};

#endif  // CbmStsUnpackAlgo_H
