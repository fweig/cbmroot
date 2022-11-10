/* Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer]  */

#ifndef CbmTzdUnpackAlgo_H
#define CbmTzdUnpackAlgo_H

#include "CbmErrorMessage.h"
#include "CbmMcbm2018TofPar.h"
#include "CbmRecoUnpackAlgo.tmpl"
#include "CbmTofDigi.h"
#include "CbmTofUnpackAlgo.h"
#include "CbmTofUnpackMonitor.h"
#include "CbmTzdDigi.h"

#include "Timeslice.hpp"  // timeslice

#include <Logger.h>

#include <Rtypes.h>  // for types
#include <RtypesCore.h>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

class CbmTzdUnpackAlgo : public CbmRecoUnpackAlgo<CbmTzdDigi, CbmErrorMessage> {
public:
  /** @brief Create the Cbm Trd Unpack AlgoBase object */
  CbmTzdUnpackAlgo();

  /** @brief Destroy the Cbm Trd Unpack Task object */
  virtual ~CbmTzdUnpackAlgo();

  /** @brief Copy constructor - not implemented **/
  CbmTzdUnpackAlgo(const CbmTzdUnpackAlgo&) = delete;

  /** @brief Assignment operator - not implemented **/
  CbmTzdUnpackAlgo& operator=(const CbmTzdUnpackAlgo&) = delete;

  /**
   * @brief Initialisation at begin of run. Forwards to TOF unpacker algo instance.
   *
   * @retval Bool_t initOk   If not kTRUE, task will be set inactive.
  */
  Bool_t Init()
  {
    LOG(info) << fName << "::Init()";

    return fTofAlgo.Init();
  }

  /**
   * @brief Unpack a given timeslice component. Overload to forward inner calls to TOF unpacker algo instance.
   *
   * @param ts timeslice pointer
   * @param icomp index to the component to be unpacked
   * @return true
   * @return false
   *
   * @remark The content of the component can only be accessed via the timeslice. Hence, we need to pass the pointer to the full timeslice
  */
  std::vector<CbmTzdDigi> Unpack(const fles::Timeslice* ts, std::uint16_t icomp)
  {

    bool unpackOk = true;

    // Clear the default return vector
    fOutputVec.clear();
    fTofAlgo.GetOutputVec().clear();

    /// On first TS, extract the TS parameters from header (by definition stable over time).
    if (fIsFirstTs) getTimesliceParams(ts);

    // Get the index of the current timeslice
    fTsIndex = ts->index();

    // Get the number of the current timeslice (the index increases currently via nthTimeslice* fNrCoreMsPerTs)
    size_t itimeslice = fTsIndex / fNrCoreMsPerTs;

    // Set further parameters required by the explicit algorithm
    setDerivedTsParameters(itimeslice);

    auto nrMsToLoop = fDoIgnoreOverlappMs ? fNrCoreMsPerTs : fNrMsPerTs;

    /// Loop over choosen microslices (all or core only)
    for (UInt_t imslice = 0; imslice < nrMsToLoop; imslice++) {
      unpackOk &= fTofAlgo.unpack(ts, icomp, imslice);
      if (!unpackOk) {
        /** @todo add potential counter for corrupted microslices */
        continue;
      }
    }

    /// Give opportunity to finalize component (e.g. copy from temp buffers) if necessary
    FinalizeComponent();

    auto ndigis = fOutputVec.size();
    fNrCreatedDigis += ndigis;

    // Get the input(output) data sizes
    fSumInDataSize += ts->size_component(icomp) / 1.0e6;
    fSumOutDataSize += ndigis * GetOutputObjSize() / 1.0e6;

    ++fNrProcessedTs;
    return fOutputVec;
  }

  // Getters
  /**
   * @brief Get the requested parameter containers. To be defined in the derived classes!
   * Return the required parameter containers together with the paths to the ascii
   * files to.
   * Forwards to TOF unpacker algo instance.
   *
   * @param[in] std::string geoTag as used in CbmSetup
   * @param[in] std::uint32_t runId for runwise defined parameters
   * @return fParContVec
  */
  virtual std::vector<std::pair<std::string, std::shared_ptr<FairParGenericSet>>>*
  GetParContainerRequest(std::string geoTag, std::uint32_t runId)
  {
    return fTofAlgo.GetParContainerRequest(geoTag, runId);
  };

  /**
   * @brief Get a given output vector connected to the tree, if called after calling InitUnpacker().
   * Forwards to TOF unpacker algo instance.
   *
   * @return std::vector<CbmErrorMessage>*
  */
  std::vector<CbmErrorMessage>* GetOptOutAVec() { return fTofAlgo.GetOptOutAVec(); }

  /**
   * @brief Get a given output vector connected to the tree, if called after calling InitUnpacker().
   * Forwards to TOF unpacker algo instance.
   *
   * @return std::vector<std::nullptr_t>*
  */
  std::vector<std::nullptr_t>* GetOptOutBVec() { return fTofAlgo.GetOptOutBVec(); }

  /**
   * @brief Get the Output Obj Size
   *
   * @return size_t
  */
  static size_t GetOutputObjSize() { return sizeof(CbmTzdDigi); }

  /** @brief Get the global system time offset
   * Forwards to TOF unpacker algo instance.
   *
   * @remark in princible this should go to parameters
   */
  int32_t GetSystemTimeOffset() { return fTofAlgo.GetSystemTimeOffset(); }

  // Setters
  /**
   * @brief Set the Do Ignore Overlapp µslices flag
   *
   * @param value
  */
  void SetDoIgnoreOverlappMs(bool value = false) { fTofAlgo.SetDoIgnoreOverlappMs(value); }

  /** @brief Set the optional output A vector @param vec */
  void SetOptOutAVec(std::vector<CbmErrorMessage>* vec) { fTofAlgo.SetOptOutAVec(vec); }

  /** @brief Set the optional output B vector @param vec */
  void SetOptOutBVec(std::vector<std::nullptr_t>* vec) { fTofAlgo.SetOptOutBVec(vec); }

  /**
   * @brief Set the base path to the parameter containers.
   *
   * @param value
  */
  void SetParFilesBasePath(std::string value) { fTofAlgo.SetParFilesBasePath(value); }

  /** @brief Set the global system time offset @remark in princible this should go to parameters */
  void SetSystemTimeOffset(int32_t value) { fTofAlgo.SetSystemTimeOffset(value); }

  /** @brief Set the start time of the current TS */
  void SetTsStartTime(size_t value) { fTofAlgo.SetTsStartTime(value); }

  /**
   * @brief Sets the flag enabling the epoch offset hack for the July 2021 data. Default is enable.
   * Forwards to TOF unpacker algo instance.
   *
   * @param[in] Optional: boolean flag value, default is true
  */
  void SetFlagEpochCountHack2021(bool bFlagin = true) { fTofAlgo.SetFlagEpochCountHack2021(bFlagin); }

  /**
   * @brief Sets the flag switching to a request of CbmMcbm2018BmonPar. Default is enable.
   * Forwards to TOF unpacker algo instance.
   *
   * @param[in] Optional: boolean flag value, default is true
  */
  void SetFlagBmonParMode(bool bFlagin = true) { fTofAlgo.SetFlagBmonParMode(bFlagin); }

  /**
   * @brief Sets the name of the parameter file to be used.
   * Forwards to TOF unpacker algo instance.
   *
   * @param[in] std:string, path should not be included as set in the Config class
  */
  void SetParFileName(std::string sNewName) { fTofAlgo.SetParFileName(sNewName); }

  /** @brief Set a predefined monitor @param monitor predefined unpacking monitor
   * Forwards to TOF unpacker algo instance.
  */
  void SetMonitor(std::shared_ptr<CbmTofUnpackMonitor> monitor) { fTofAlgo.SetMonitor(monitor); }

protected:
  /** @brief Finish function for this algorithm base clase. Forwards to TOF unpacker algo instance. */
  void finish() { fTofAlgo.finish(); }

  /** @brief Function that allows special calls during Finish in the derived algos.
   *  Forwards to TOF unpacker algo instance.
  */
  void finishDerived() { fTofAlgo.finishDerived(); }

  /**
   * @brief Initialisation at begin of run. Forwards to TOF unpacker algo instance.
   *
   * @retval Bool_t initOk
  */
  Bool_t init() { return fTofAlgo.init(); }

  /**
   * @brief Handles the distribution of the hidden derived classes to their explicit functions.
   * Forwards to TOF unpacker algo instance.
   *
   * @param parset
   * @return Bool_t initOk
  */
  Bool_t initParSet(FairParGenericSet* parset) { return fTofAlgo.initParSet(parset); }

  /**
   * @brief Handles the distribution of the hidden derived classes to their explicit functions.
   * Forwards to TOF unpacker algo instance.
   *
   * @param parset
   * @return Bool_t initOk
  */
  Bool_t initParSet(CbmMcbm2018TofPar* parset) { return fTofAlgo.initParSet(parset); }


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
   * @brief Copy the buffer from the internal (TOF) algo to the new Digi Type storage
  */
  virtual void FinalizeComponent()
  {
    fOutputVec.reserve(fTofAlgo.GetOutputVec().size());
    for (CbmTofDigi internalDigi : fTofAlgo.GetOutputVec()) {
      fOutputVec.push_back(CbmTzdDigi(internalDigi));
    }
    return;
  }

  /**
   * @brief Unpack a given microslice. To be implemented in the derived unpacker algos.
   * Unused as "Unpack(timeslice, component)" is overloaded, but necessary to avoir pure virtual class.
   *
   * @param ts timeslice pointer
   * @param icomp index to the component to be unpacked
   * @param imslice index of the microslice to be unpacked
   * @return true
   * @return false
   *
   * @remark The content of the µslice can only be accessed via the timeslice. Hence, we need to pass the pointer to the full timeslice
  */
  virtual bool unpack(const fles::Timeslice* /*ts*/, std::uint16_t /*icomp*/, UInt_t /*imslice*/) { return true; }

private:
  /** @brief Actual unpacker algo */
  CbmTofUnpackAlgo fTofAlgo;

  ClassDef(CbmTzdUnpackAlgo, 1)
};

#endif  // CbmTzdUnpackAlgo_H
