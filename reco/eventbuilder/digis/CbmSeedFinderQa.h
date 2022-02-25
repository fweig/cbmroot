/* Copyright (C) 2007-2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith [committer] */

/**
 * @file CbmSeedFinderQa.h
 * @author Dominik Smith (d.smith@gsi.de)
 * @brief Class for sliding window seed finder
 * @version 0.1
 * @date 2022-02-04
 * 
 * @copyright Copyright (c) 2022
 * 
 *  Qa output for the seed finder.
 */

#ifndef CbmSeedFinderQa_h
#define CbmSeedFinderQa_h

#include "CbmMatch.h"

#include <TFolder.h>

#include <cstdint>
#include <iostream>
#include <vector>

class TH1F;
class TH2I;
class CbmMCEventList;
class CbmQaCanvas;

class CbmSeedFinderQa {
public:
  /**
   * @brief Create the CbmSeedFinderQa object
   */
  CbmSeedFinderQa();
  CbmSeedFinderQa(const CbmSeedFinderQa&) = delete;
  CbmSeedFinderQa operator=(const CbmSeedFinderQa&) = delete;

  /** @brief Destructor **/
  ~CbmSeedFinderQa();

  /** @brief Matches that link constructed event seeds to MC events. */
  std::vector<CbmMatch> fvEventMatches;
  /** @brief Counts how many MC events contributed to a seed. */
  std::vector<uint32_t> fvLinkedMCEventsCount;
  /** @brief Full vector of all event seeds that is not cleared at the end of a timeslice. */
  std::vector<double> fvSeedTimesFull;
  /** @brief Counts how many digis contributed to a seed. */
  std::vector<int32_t> fvFullDigiCount;
  /** @brief Counts how many noise digis contributed to a seed. */
  std::vector<int32_t> fvNoiseDigiCount;
  /** @brief Ratio of digis from matched MC event. */
  std::vector<double> fvCorrectDigiRatio;
  /** @brief Ratio of digis from matched MC event (disregarding noise). */
  std::vector<double> fvCorrectDigiRatioNoNoise;
  /** @brief Ratio of digis of matched events that were included in event seed. */
  std::vector<double> fvFoundDigiRatio;
  /** @brief Difference between true event time and seed time. */
  std::vector<double> fvTimeOffset;

  /** @brief Matches that link constructed event seeds to MC events, current timeslice only. */
  std::vector<CbmMatch> fvEventMatchesPerTs;
  /** @brief Vector of event seeds, current TS only. */
  std::vector<double> fvSeedTimesPerTs;

  /** @brief Gather QA Information. 
  * @params WinStart Starting position of seed window.
  * @params WinStart End position of seed window.
  * @params vDigiMatch Input vector of digi matches (should match input data to MC events). 
  * @params seedTime Current seed. 
  **/
  void FillQaSeedInfo(const int32_t WinStart, const int32_t WinEnd, const std::vector<CbmMatch>* vDigiMatch,
                      const double seedTime);

  /** @brief Output QA Information. */
  void OutputQa();

  /** @brief Fill QA Information that uses the full list of MC events per TS. */
  void FillQaMCInfo();

  /** @brief Reset containers that are persistent for one TS. */
  void ResetPerTsStorage();

  /** @brief Finalize histograms and canvases and write to file. */
  void WriteHistos();

  /** @brief Initialize communication with FairRootManager (needed for MC events). */
  void Init();

private:
  TFolder* histFolder = nullptr;  /// subfolder for histograms
  TFolder fOutFolder;             /// output folder with histos and canvases

  /** Histograms **/
  TH1F* fhLinkedTriggersPerMCEvent  = nullptr;  /// linked triggers per MC event
  TH1F* fhMatchedTriggersPerMCEvent = nullptr;  /// matchted triggers per MC event
  TH1F* fhLinkedMCEventsPerTrigger  = nullptr;  /// linked MC events per trigger
  TH1F* fhCorrectDigiRatio          = nullptr;  /// correct digis per event
  TH1F* fhCorrectDigiRatioNoNoise   = nullptr;  /// correct digis per event, disregarding noise
  TH1F* fhNoiseDigiRatio            = nullptr;  /// noise digis per event
  TH1F* fhFoundDigiRatio            = nullptr;  /// digis found per event
  TH2I* fhCorrectVsFound            = nullptr;  /// correct digis per event vs found digis per event
  TH2I* fhCorrectVsFoundNoNoise     = nullptr;  /// correct digis per event vs found digis per event, disregarding noise
  TH1F* fhTimeOffset                = nullptr;  /// difference between true event time and seed time
  TH1F* fhTimeOffsetClosest         = nullptr;  /// difference between seed time and closest MC event time

  CbmQaCanvas* fCanv;  ///summary canvas

  CbmMCEventList* fEventList = nullptr;  // to access MC truth

  /** @brief Fill output histograms with data from current timeslice. */
  void FillHistos();
};
#endif  //CbmSeedFinderQa_h
