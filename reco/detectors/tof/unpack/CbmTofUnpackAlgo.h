/* Copyright (C) 2021 Goethe-University Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig [committer] */

/**
 * @file CbmTofUnpackAlgo.h
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

#ifndef CbmTofUnpackAlgo_H
#define CbmTofUnpackAlgo_H

#include "CbmErrorMessage.h"
#include "CbmMcbm2018TofPar.h"
#include "CbmRecoUnpackAlgo.tmpl"
#include "CbmTofDigi.h"

#include "Timeslice.hpp"  // timeslice

#include <Rtypes.h>  // for types
#include <RtypesCore.h>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

#include "CriGet4Mess001.h"

class CbmTofUnpackAlgo : public CbmRecoUnpackAlgo<CbmTofDigi, CbmErrorMessage> {
public:
  /** @brief Create the Cbm Trd Unpack AlgoBase object */
  CbmTofUnpackAlgo();

  /** @brief Destroy the Cbm Trd Unpack Task object */
  virtual ~CbmTofUnpackAlgo();

  /** @brief Copy constructor - not implemented **/
  CbmTofUnpackAlgo(const CbmTofUnpackAlgo&) = delete;

  /** @brief Assignment operator - not implemented **/
  CbmTofUnpackAlgo& operator=(const CbmTofUnpackAlgo&) = delete;

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
   * @brief Sets the flag enabling the epoch offset hack for the July 2021 data. Default is enable.
   *
   * @param[in] Optional: boolean flag value, default is true
  */
  void SetFlagEpochCountHack2021(bool bFlagin = true) { fbEpochCountHack2021 = bFlagin; }

protected:
  /** @brief Finish function for this algorithm base clase */
  void finish()
  {
    finishDerived();
    // Finish the monitor if we have one
    // if (fMonitor) fMonitor->Finish();
  }

  /** @brief Function that allows special calls during Finish in the derived algos */
  virtual void finishDerived() { return; }

  /**
   * @brief Initialisation at begin of run. Special inits of the derived algos.
   *
   * @retval Bool_t initOk
  */
  Bool_t init();

  /**
   * @brief Handles the distribution of the hidden derived classes to their explicit functions.
   *
   * @param parset
   * @return Bool_t initOk
  */
  Bool_t initParSet(FairParGenericSet* parset);

  /**
   * @brief Handles the distribution of the hidden derived classes to their explicit functions.
   *
   * @param parset
   * @return Bool_t initOk
  */
  Bool_t initParSet(CbmMcbm2018TofPar* parset);

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

private:
  /// Buffers processing
  void ProcessEpSupprBuffer();

  /// Message processing methods
  void ExtractTsStartEpoch(const uint64_t& ulTsStart);
  void ProcessEpoch(const critof001::Message& mess, uint32_t uMesgIdx);
  void ProcessEndOfMsEpoch();
  void ProcessHit(const critof001::Message& mess);

  inline Int_t GetArrayIndex(Int_t gdpbId, Int_t get4Id) { return gdpbId * fuNrOfGet4PerGdpb + get4Id; }


  /// Settings from parameter file
  CbmMcbm2018TofPar* fUnpackPar = nullptr;  //! For static/inline mapping functions

  /// Readout chain dimensions and mapping
  UInt_t fuNrOfGdpbs                       = 0;   //! Total number of GDPBs in the system
  std::map<UInt_t, UInt_t> fGdpbIdIndexMap = {};  //! gDPB ID to index map
  UInt_t fuNrOfFeePerGdpb                  = 0;   //! Number of FEBs per GDPB
  UInt_t fuNrOfGet4PerFee                  = 0;   //! Number of GET4s per FEE
  UInt_t fuNrOfChannelsPerGet4             = 0;   //! Number of channels in each GET4
  UInt_t fuNrOfChannelsPerFee              = 0;   //! Number of channels in each FEE
  UInt_t fuNrOfGet4                        = 0;   //! Total number of Get4 chips in the system
  UInt_t fuNrOfGet4PerGdpb                 = 0;   //! Number of GET4s per GDPB
  UInt_t fuNrOfChannelsPerGdpb             = 0;   //! Number of channels per GDPB

  /// Detector Mapping
  UInt_t fuNrOfGbtx              = 0;
  UInt_t fuNrOfModules           = 0;
  std::vector<Int_t> fviNrOfRpc  = {};
  std::vector<Int_t> fviRpcType  = {};
  std::vector<Int_t> fviRpcSide  = {};
  std::vector<Int_t> fviModuleId = {};
  std::vector<Int_t> fviRpcChUId = {};

  /// Running indices
  UInt_t fuMapWarnToPrint     = 100;
  ULong64_t fulCurrentTsIdx   = 0;  //! Idx of the current TS
  ULong64_t fulCurrentMsIdx   = 0;  //! Idx of the current MS in TS (0 to fuTotalMsNb)
  size_t fuCurrentMsSysId     = 0;  //! SysId of the current MS in TS (0 to fuTotalMsNb)
  UInt_t fuCurrentEquipmentId = 0;  //! Current equipment ID, tells from which DPB the current MS is originating
  UInt_t fuCurrDpbId          = 0;  //! Temp holder until Current equipment ID is properly filled in MS
  UInt_t fuCurrDpbIdx         = 0;  //! Index of the DPB from which the MS currently unpacked is coming
  UInt_t fuGet4Id =
    0;  //! running number (0 to fuNrOfGet4PerGdpb) of the Get4 chip of a unique GDPB for current message
  UInt_t fuGet4Nr = 0;  //! running number (0 to fuNrOfGet4) of the Get4 chip in the system for current message
  /// Data format control: Current time references for each GDPB: merged epoch marker, epoch cycle, full epoch [fuNrOfGdpbs]
  ULong64_t fulCurrentEpoch = 0;  //! Current epoch index

  /// Control flags
  bool fbEpochCountHack2021             = false;
  std::vector<bool> fvbMaskedComponents = {};
  bool fbLastEpochGood                  = false;

  /// Book-keeping members
  uint32_t fuProcEpochUntilError = 0;
  uint64_t fulTsStartInEpoch     = 0;
  uint64_t fulEpochIndexInTs     = 0;

  ClassDef(CbmTofUnpackAlgo, 2)
};

#endif  // CbmTofUnpackAlgo_H
