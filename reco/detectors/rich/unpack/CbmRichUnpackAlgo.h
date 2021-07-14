/* Copyright (C) 2010 - 2021 Goethe-University Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig */

/**
 * @file CbmRichUnpackAlgo.h
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

#ifndef CbmRichUnpackAlgo_H
#define CbmRichUnpackAlgo_H

#include "CbmRecoUnpackAlgo.tmpl"
#include "CbmRichDigi.h"

#include "Timeslice.hpp"  // timeslice

#include <Rtypes.h>  // for types
#include <RtypesCore.h>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

class CbmRichUnpackAlgo : public CbmRecoUnpackAlgo<CbmRichDigi> {
public:
  /** @brief Create the Cbm Trd Unpack AlgoBase object */
  CbmRichUnpackAlgo();

  /** @brief Destroy the Cbm Trd Unpack Task object */
  virtual ~CbmRichUnpackAlgo();

  /** @brief Copy constructor - not implemented **/
  CbmRichUnpackAlgo(const CbmRichUnpackAlgo&) = delete;

  /** @brief Assignment operator - not implemented **/
  CbmRichUnpackAlgo& operator=(const CbmRichUnpackAlgo&) = delete;

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
  GetParContainerRequest(std::string geoTag, std::uint32_t runId)
  {
    return {};
  };

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
   * @brief Intialisation at begin of run. Special inits of the derived algos.
   * 
   * @retval Bool_t initOk
  */
  Bool_t init() { return kTRUE; }

  /**
   * @brief Handles the distribution of the hidden derived classes to their explicit functions.
   * 
   * @param parset 
   * @return Bool_t initOk 
  */
  Bool_t initParSet(FairParGenericSet* parset) { return kTRUE; }

  /**
   * @brief Set the Derived Ts Parameters
   * 
   * In this function parameters required by the explicit algo connected to the timeslice can be set.
   * 
   * @param itimeslice 
   * @return true 
   * @return false 
  */
  bool setDerivedTsParameters(size_t itimeslice) { return true; }

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
  bool unpack(const fles::Timeslice* ts, std::uint16_t icomp, UInt_t imslice) { return true; }

  /** @brief Fles Subsystem Identifier for the TRD R data */
  static constexpr int fgkFlesSubsystemIdTrdR = static_cast<int>(fles::SubsystemIdentifier::RICH);

private:
  ClassDef(CbmRichUnpackAlgo, 2)
};

#endif  // CbmRichUnpackAlgo_H
