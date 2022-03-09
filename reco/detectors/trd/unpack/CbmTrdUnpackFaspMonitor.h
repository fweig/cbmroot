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
 * This class is companion to the CbmTrdUnpackFaspAlgo and it provides online
 * visualization of unpacked digi (CbmTrdDigi) properties and FASP ASICs response 
 */

#ifndef CbmTrdUnpackFaspMonitor_H
#define CbmTrdUnpackFaspMonitor_H

#include "CbmTrdDigi.h"
#include "CbmTrdParSetAsic.h"
#include "CbmTrdParModDigi.h"
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
  friend class CbmTrdUnpackFaspAlgo;

public:
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
  Bool_t Init();

  /** @brief transfer the enums for the histos to be activated to the member vector */
  void SetActiveHistos(std::vector<eDigiHistos> vec) { fActiveDigiHistos.swap(vec); }

  /** @brief transfer the enums for the histos to be activated to the member vector */
  void SetActiveHistos(std::vector<eRawHistos> vec) { fActiveRawHistos.swap(vec); }

protected:
  /** @brief Init module and link asics properties
   *  @param madd module address to be added
   *  @param asics asic properties for the current module
   */
  void addParam(uint32_t madd, const CbmTrdParSetAsic* asics);
  /** @brief Init module pad-plane parameters
   *  @param madd module address to be checked
   *  @param digis pad-plane properties for the current module
   */
  void addParam(uint32_t madd, CbmTrdParModDigi* pp);
  /** @brief Create the actual TH1 shared_ptrs of the Digi histos */
  void createHisto(eDigiHistos kHisto);
  /** @brief Fill the given histo with the information from the digi. Reimplement from CbmTrdUnpackMonitor
   *  @param[in] digi CbmTrdDigi 
   *  @param[in] kHisto Histo definition
   *  @param[in] moduleid Unique module Id from which the digi came
   *  @param[out] histo pointer to the histo
  */
  virtual void fillHisto(CbmTrdDigi* digi, eDigiHistos kHisto, std::uint32_t moduleid, std::shared_ptr<TH1> histo);
  /** @brief Paralell implementation of the omonime function from CbmTrdUnpackMonitor 
   * for the case of FASP digis. The default values of the input parameters mark missing
   * information.
   *  @param[in] modid module address
   *  @param[in] ch module-wise channel address of the signal 
   *  @param[in] daqt DAQ time [clk] for the "ch" paired signal
   *  @return  
   */
  virtual std::uint64_t getDeltaT(uint32_t modid, int32_t ch = -1, uint64_t daqt = 0);

private:
  /** @brief save 1/clk for the FASP FEE in kHz*/
  const double fFaspInvClk = 1.e6 / CbmTrdDigi::Clk(CbmTrdDigi::eCbmTrdAsicType::kFASP);
  /** @brief Map of definitions for each module under monitoring :
   * std::get<0> : pad to FASP channel mapping 
   * std::get<1> : no of pad columns in the module 
   * std::get<2> : no of pad rows in the module
   */
  std::map<uint32_t, std::tuple<std::vector<int32_t>, uint8_t, uint8_t>> fModuleDef = {};

  ClassDef(CbmTrdUnpackFaspMonitor, 1)
};

#endif  // CbmTrdUnpackFaspMonitor_H
