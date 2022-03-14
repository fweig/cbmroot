/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

#ifndef CBMRECO_H
#define CBMRECO_H 1

#include "CbmDefs.h"

#include <TString.h>

#include <map>
#include <utility>


/** @class CbmRecoConfig
 ** @brief Configuration of reconstruction
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 14 March 2022
 **/
class CbmRecoConfig {
public:
  // --- Digi trigger
  ECbmModuleId fTriggerDet = ECbmModuleId::kNotExist;  // Trigger detector
  double fTriggerWin       = 0.;                       // Trigger window size [ns]
  size_t fTriggerThreshold = 0;                        // Minimum number if digis in trigger window
  double fTriggerDeadTime  = 0.;                       // Minimal time between two trigger [ns]
  // --- Event builder: (detector -> (tMin, tMax))
  std::map<ECbmModuleId, std::pair<double, double>> fEvtbuildWindows = {};
  // --- Branch persistence in output file
  bool fStoreTimeslice = false;
  bool fStoreTrigger   = false;
  bool fStoreEvents    = false;
  // --- Destructor
  virtual ~CbmRecoConfig() {};

  ClassDef(CbmRecoConfig, 1);
};


/** @class CbmReco
 ** @brief Main steering class of reconstruction in CBM
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 14 March 2022
 **
 ** The class instantiates the processing steps as FairTasks, and configure and executes the
 ** run as FairRunOnline.
 **
 ** Currently included processing steps:
 ** - unpacking (STS only)
 ** - trigger on STS digis
 ** - event building (STS only)
 **/
class CbmReco {

public:
  /** @brief Default constructor **/
  CbmReco() {};


  /** @brief Standard constructor
   ** @param source  Name of input file or input source
   ** @param outFile Name of output file
   ** @param numTs   Number of timeslices to process. If negative, all available will be used.
   ** @param config  Configuration
   **/
  CbmReco(TString source, TString outFile, int32_t numTs, const CbmRecoConfig& config);


  /** @brief Destructor **/
  virtual ~CbmReco() {};


  /** @brief Configure and execute run
   ** @return Number of processed timeslices. -1 if error encountered.
   **/
  int32_t Run();


private:
  TString fInputFileName  = "";
  TString fOutputFileName = "";
  int32_t fNumTs          = -1;
  CbmRecoConfig fConfig   = {};

  ClassDef(CbmReco, 1);
};

#endif /* CBMRECO_H */
