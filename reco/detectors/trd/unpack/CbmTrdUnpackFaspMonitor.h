/* Copyright (C) 2022 Horia Hulubei National Institute of Physics and Nuclear Engineering, Bucharest
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Alexandru Bercuci [committer] */

/**
 * @file CbmTrdUnpackFaspMonitor.h
 * @author Alexandru Bercuci (abercuci@niham.nipne.ro)
 * @brief Monitor class for the unpacker algorithms (CbmTrdUnpackFasp) of FASP data
 * @version 0.1
 * @date 2022-03-08
 * 
 * @copyright Copyright (c) 2022
 * 
 * This class can be attached to an unpacker algorithm class. It will 
 * CbmTrdDigi and CbmTrdRawMessageSpadic 
 * 
 * 
*/

#ifndef CbmTrdUnpackFaspMonitor_H
#define CbmTrdUnpackFaspMonitor_H

#include "CbmTrdDigi.h"
#include "CbmTrdParSetDigi.h"
#include "CbmTrdSpadic.h"
#include "CbmTrdUnpackMonitor.h"

#include <MicrosliceDescriptor.hpp>
#include <Timeslice.hpp>

#include <FairRunOnline.h>
#include <FairTask.h>
#include <Logger.h>

#include <Rtypes.h>  // for types
#include <RtypesCore.h>
#include <TFile.h>
#include <TH1.h>
#include <THttpServer.h>  // for histogram server

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <cmath>

class CbmTrdUnpackFaspMonitor : public CbmTrdUnpackMonitor {
public:
//   /** @brief Enum for the predefined digi histograms. */
//   enum class eDigiHistos : size_t
//   {
//     kMap = 0,
//     kMap_St,
//     kMap_Nt,
//     kChannel,
//     kChannel_St,
//     kChannel_Nt,
//     kCharge,
//     kCharge_St,
//     kCharge_Nt,
//     kTriggerType,
//     kDigiDeltaT
//   };
// 
//   /** @brief Enum for the predefined raw histograms. */
//   enum class eRawHistos : size_t
//   {
//     kSignalshape = 0,
//     kSignalshape_St,
//     kSignalshape_Nt,
//     kMap,
//     kMap_St,
//     kMap_Nt,
//     kElinkId,
//     kSampleDistStdDev,
//     kSample0perChannel,
//     kHitType,
//   };
// 
//   /** @brief Enum for the predefined other histograms. */
//   enum class eOtherHistos : size_t
//   {
//     kSpadic_Info_Types = 0,
//     kMs_Flags
//   };

  /** @brief Create the Cbm Trd Unpack AlgoBase object */
  CbmTrdUnpackFaspMonitor(/* args */);

  /** @brief Destroy the Cbm Trd Unpack Task object */
  virtual ~CbmTrdUnpackFaspMonitor();

  /** @brief Copy constructor - not implemented **/
  CbmTrdUnpackFaspMonitor(const CbmTrdUnpackFaspMonitor&) = delete;

  /** @brief Assignment operator - not implemented **/
  CbmTrdUnpackFaspMonitor& operator=(const CbmTrdUnpackFaspMonitor&) = delete;

  /** @brief fill the stored digi histograms @param digi pointer to the digi @param raw pointer to the raw msg */
  void FillHistos(CbmTrdDigi* digi);

  /** @brief Actions at the end of the run, e.g. write histos to file if flag is set. */
  void Finish();

  // Runtime functions
  /** @brief Init all required parameter informations */
  Bool_t Init(CbmTrdParSetDigi* digiParSet);

  /** @brief transfer the enums for the histos to be activated to the member vector */
  void SetActiveHistos(std::vector<eDigiHistos> vec) { fActiveDigiHistos.swap(vec); }

  /** @brief transfer the enums for the histos to be activated to the member vector */
  void SetActiveHistos(std::vector<eRawHistos> vec) { fActiveRawHistos.swap(vec); }

protected:
  /** @brief Create the actual TH1 shared_ptrs of the Digi histos */
  void createHisto(eDigiHistos kHisto);

private:
  ClassDef(CbmTrdUnpackFaspMonitor, 1)
};

#endif  // CbmTrdUnpackFaspMonitor_H
