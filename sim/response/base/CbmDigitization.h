/* Copyright (C) 2018-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** @file CbmDigitization.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 22.05.2018
 **/

#ifndef CBMDIGITIZATION_H
#define CBMDIGITIZATION_H 1

#include "CbmDaq.h"
#include "CbmDefs.h"
#include "CbmDigitizationSource.h"
#include "CbmDigitizeInfo.h"

#include <Logger.h>

#include "TList.h"
#include "TNamed.h"
#include "TString.h"
#include "TSystem.h"

#include <map>
#include <vector>

class TGeoManager;
class CbmDigitizeBase;


class CbmDigitization : public TNamed {

public:
  /** @brief Constructor **/
  CbmDigitization();


  /** @brief Destructor  **/
  virtual ~CbmDigitization();


  /** @brief Add an input file
     ** @param inputId   Unique input identifier
     ** @param fileName  Name of input file (MC)
     ** @param eventRate Rate for events from input file [1/s]
     ** @param mode      Tree access mode (kRegular / kRepeat / kRandom)
     **/
  void AddInput(UInt_t inputId, TString fileName, Double_t eventRate = -1.,
                ECbmTreeAccess mode = ECbmTreeAccess::kRegular);


  /** @brief Add an input file
     ** @param fileName  Name of input file (MC)
     ** @param eventRate Rate for events from input file [1/s]
     ** @param mode      Tree access mode (kRegular / kRepeat / kRandom)
     **
     ** Shortcut for legacy reasons, when only one input file is used.
     ** This will set the inputId to zero. Repeated use will lead to abort.
     **/
  void AddInput(TString fileName, Double_t eventRate = -1., ECbmTreeAccess mode = ECbmTreeAccess::kRegular)
  {
    AddInput(0, fileName, eventRate, mode);
  }

  /** @brief Add an ASCII parameter file
     ** @param fileName  Name of parameter file
     ** @return kTRUE is file is found
     **
     ** All ASCII parameter files will be concatenated and used
     ** as second input to the runtime database.
     */
  Bool_t AddParameterAsciiFile(TString fileName);


  /** @brief Deactivate a system for digitisation
     ** @param system System ID (ECbmModuleId)
     **
     ** The digitiser for this system will not be run even if
     ** the MCPoint branch is present in the input tree.
     **/
  void Deactivate(ECbmModuleId system);


  /** @brief Deactivate all systems except the specified one
     ** @param system System ID (ECbmModuleId)
     **
     ** Only the digitiser of the specified system will run.
     **/
  void DeactivateAllBut(ECbmModuleId system);


  /** @brief Embed an input file into another one
     ** @param inputId   Unique input identifier
     ** @param fileName  Name of input file (MC)
     ** @param targetInputId ID of the input to be embedded into
     ** @param mode      Tree access mode (kRegular / kRepeat / kRandom)
     **/
  void EmbedInput(UInt_t inputId, TString fileName, UInt_t targetInputId,
                  ECbmTreeAccess mode = ECbmTreeAccess::kRegular);


  /** @brief Write run info (default is kTRUE)
     ** @param choice  If kTRUE, run info will be written
     **
     ** The run info comprises CPU and memory consumption for each event.
     ** It will be written to a separate ROOT file.
     */
  void GenerateRunInfo(Bool_t choice = kTRUE) { fGenerateRunInfo = choice; }

  /** @brief Initialize the branches, digitizers and parameter files
     **        With default values
     */
  void DefaultInit();

  /** @brief Process all events from input **/
  void Run() { Run(-1, -1); }


  /** @brief Process nEvents from input, starting with the first event
     ** @param nEvents  Number of events to process
     **/
  void Run(Int_t nEvents) { Run(0, nEvents); }


  /** @brief Process input from event1 to event2
     ** @param event1  First event to process
     ** @param event2  Last event to process
     **/
  void Run(Int_t event1, Int_t event2);


  /** @brief Set creation of links to MC
     ** @param Choice If kTRUE, the match objects will be created
     **/
  void SetCreateMatches(Bool_t choice = kTRUE) { fCreateMatches = choice; }


  /** @brief Set a digitizer explicitly
     ** @param system System ID (ECbmModuleId)
     ** @param digitizer Pointer to digitizer instance
     ** @param branch Input branch name (MCPoint)
     ** @param persistent if kTRUE, the output (digis) will be persistent
     **
     ** This method has to be called from the macro level, if a digitizer
     ** different from the default one or with different settings than the
     ** default ones is to be used. The respective digitizer has to be
     ** instantiated before. The ownership is passed to this class.
     **/
  void SetDigitizer(ECbmModuleId system, CbmDigitizeBase* digitizer, TString branch = "", Bool_t persistent = kTRUE);


  /** @brief Set event-by-event mode
     ** @param choice  If kTRUE, digitisation will be done event-by-event
     **
     ** In the event-by-event mode, one time slice will be created for
     ** each input event. There will be no interference between events.
     **/
  void SetEventMode(Bool_t choice = kTRUE) { fEventMode = choice; }


  /** @brief Set the monitor file name
     ** @param fileName  Name of monitor file
     **
     ** The monitor file stores information on the resource usage.
     ** If a file name is specified, a corresponding file will
     ** be created and filled. By default, monitoring is not recorded.
     **/
  void SetMonitorFile(const char* fileName) { fMoniFile = fileName; }


  /** @brief Set the output file name
     ** @param path  Name of output file
     ** @param overwrite Overwrite output file if already existing
     **
     ** If the directory of the file does not exist, it will be created.
     **/
  void SetOutputFile(TString fileName, Bool_t overwrite = kFALSE);


  /** @brief Set the parameter file name
     ** @param fileName  Name of output file
     **/
  void SetParameterRootFile(TString fileName);


  /** @brief Set production of inter-event noise
     ** @param Choice If kTRUE, the digitizer will produce noise
     **/
  void SetProduceNoise(Bool_t choice = kTRUE) { fProduceNoise = choice; }


  /** @brief Set the start time of the run
     ** @param time Start time of run [ns]
     **
     ** A default of 1000 ns is hard-coded in the constructor of
     ** CbmDigitizationSource. It can be changed with this method.
     **/
  void SetStartTime(Double_t time) { fSource->SetTimeStart(time); }


  /** @brief Set length of the time-slices
     ** @param length  Length of time-slices [ns]
     **
     ** The raw data will be sorted into time-slices of the specified
     ** length.
     **
     ** If the time-slice length is negative (default), one time-slice
     ** for all input data will be created.
     **/
  void SetTimeSliceLength(Double_t length) { fTimeSliceLength = length; }


  /** @brief Store all time-slices
      ** @param choice If kTRUE; also empty slices will be stored.
      **
      ** By default, only time slices containing data are filled into the tree.
      **/
  void StoreAllTimeSlices(Bool_t choice = kTRUE) { fDaq->StoreAllTimeSlices(choice); }

  /** @brief Get the pointer on a given digitizer if existing
     ** @param system System ID (ECbmModuleId)
     ** @return digitizer Pointer to digitizer instance if found, NULL otherwise
     **
     ** This method can be called from the macro level, between CbmDigitization
     ** instantiation and the call to the Run method (which itself calls the
     ** CbmRunAna Init method) to set specific parameters.
     **/
  CbmDigitizeBase* GetDigitizer(ECbmModuleId system);


private:
  Bool_t fIsInit;
  Bool_t fEventMode;
  Double_t fTimeSliceLength;
  Bool_t fProduceNoise;
  Bool_t fCreateMatches;
  std::map<ECbmModuleId, CbmDigitizeInfo*> fDigitizers;  //!
  CbmDaq* fDaq;
  CbmDigitizationSource* fSource;  ///< Input source
  TString fOutFile;                ///< Output data (digis)
  TString fParRootFile;            ///< ROOT parameter file
  TList fParAsciiFiles;            ///< ASCII parameter files
  TString fMoniFile;               ///< Resource monitoring information
  Bool_t fOverwriteOutput;
  Bool_t fGenerateRunInfo;
  Int_t fRun;


  /** @brief Copy constructor forbidden **/
  CbmDigitization(const CbmDigitization&) = delete;


  /** @brief Assignment operator forbidden **/
  CbmDigitization operator=(const CbmDigitization&) = delete;


  /** @brief Check the presence of input branches.
     ** @return Number of required branches (MCPoint) found in the tree.
     **
     ** The branch list is searched for the input branches (MCPoint)
     ** required by the digitizers. For each present input branch,
     ** the corresponding digitizer will be instantiated. In addition,
     ** the run number is extracted from the MCEventHeader.
     **/
  Int_t CheckInput();


  /** @brief Instantiate the default digitisers for the active systems
     ** @return Number of instantiated digitisers
     **
     ** For systems not having explicitly defined their digitizers,
     ** the default digitisers are instantiated.
     **/
  Int_t CreateDefaultDigitizers();


  /** @brief Get the geometry tag of a system from a TGeoManager
     ** @param system  Detector system (ECbmModuleId)
     ** @param geo     Pointer to TGeoManager
     ** @return Geometry tag
     **
     ** The implementation assumes that the top-level volume name of each
     ** system in the geometry contains the geometry tag in the form of
     ** e.g., sts_v16g
     **/
  TString GetGeoTag(ECbmModuleId system, TGeoManager* geo);


  /** @brief Default settings for digitizers **/
  void SetDefaultBranches();


  ClassDef(CbmDigitization, 3);
};

#endif /* CBMDIGITIZATION_H */
