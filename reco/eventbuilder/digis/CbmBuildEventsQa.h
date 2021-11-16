/* Copyright (C) 2016-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer], Dominik Smith */

/** @file CbmBuildEventsQa.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 20.09.2016
 **/
#ifndef CBMBUILDEVENTSQA_H_
#define CBMBUILDEVENTSQA_H 1

#include "CbmDefs.h"

#include <FairTask.h>

#include <TFolder.h>

class TClonesArray;
class TH1F;
class TH2I;
class CbmDigiManager;
class CbmEvent;
class CbmQaCanvas;

/** @class CbmBuildEventsQA
 ** @brief Simple QA task for associating digis to events
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 20.09.2016
 ** @version 1.0
 **
 ** The task constructs a match from CbmEvent to CbmMCEvent
 ** and calculates the percentage of correctly assigned digis
 ** based on their matched link (largest weight).
 **/
class CbmBuildEventsQa : public FairTask {

public:
  /** Constructor **/
  CbmBuildEventsQa();

  /** Destructor **/
  virtual ~CbmBuildEventsQa();

  /** Task execution **/
  virtual void Exec(Option_t* opt);
  void Finish();

  /** Add a reference detector **/
  void AddRefDetector(ECbmModuleId RefDetector) { fRefDetectors.push_back(RefDetector); }

private:
  CbmDigiManager* fDigiMan = nullptr;     //!
  std::vector<ECbmModuleId> fSystems {};  //  List of detector systems
  TClonesArray* fEvents;                  ///< Input array (class CbmEvent)
  int fNofEntries = 0;                    ///< Number of processed entries

  TFolder* histFolder = nullptr;  /// subfolder for histograms
  TFolder fOutFolder;             /// output folder with histos and canvases

  /** Task initialisation **/
  virtual InitStatus Init();
  void DeInit();

  /** Histograms **/
  TH1F* fhCorrectDigiRatioAll        = nullptr;  /// correct digis per event for all detectors
  TH1F* fhCorrectDigiRatioAllNoNoise = nullptr;  /// correct digis per event for all detectors, disregarding noise
  TH1F* fhNoiseDigiRatioAll          = nullptr;  /// noise digis per event for all detectors
  TH1F* fhFoundDigiRatioAll          = nullptr;  /// digis found per event for all detectors
  TH2I* fhCorrectVsFoundAll          = nullptr;  ///  correct digis per event vs found digis per event, all detectors
  TH2I* fhCorrectVsFoundAllNoNoise =
    nullptr;  ///  correct digis per event vs found digis per event, all detectors, disregarding noise

  CbmQaCanvas* fCanvAllSystems;  ///summary canvas

  std::map<ECbmModuleId, TH1F*> fhMapSystemsCorrectDigi;            // histograms for subsystems
  std::map<ECbmModuleId, TH1F*> fhMapSystemsCorrectDigiNoNoise;     // histograms for subsystems
  std::map<ECbmModuleId, TH1F*> fhMapSystemsNoiseDigi;              // histograms for subsystems
  std::map<ECbmModuleId, TH1F*> fhMapSystemsFoundDigi;              // histograms for subsystems
  std::map<ECbmModuleId, TH2I*> fhMapSystemsCorrectVsFound;         // histograms for subsystems
  std::map<ECbmModuleId, TH2I*> fhMapSystemsCorrectVsFoundNoNoise;  // histograms for subsystems

  std::map<ECbmModuleId, CbmQaCanvas*> fCanvMapSystems;  //summary canvas per system

  /** Initialize histograms and output folder+  **/
  void InitHistograms();

  /** Match a reconstructed event to MC events+
		 ** @param event Pointer to reconstructed event
		 **/
  void MatchEvent(CbmEvent* event);

  /** Read out the best matched MC event that isn't noise+
		 ** @param event Pointer to reconstructed event
		 **/
  int getMatchedMcEventNoNoise(const CbmEvent* event);

  ECbmDataType GetDigiType(ECbmModuleId system);

  std::vector<ECbmModuleId> fRefDetectors;  //  Detectors used for MC matching

  CbmBuildEventsQa(const CbmBuildEventsQa&);
  CbmBuildEventsQa& operator=(const CbmBuildEventsQa&);

  ClassDef(CbmBuildEventsQa, 1);
};

#endif /* CBMBUILDEVENTSQA_H */
