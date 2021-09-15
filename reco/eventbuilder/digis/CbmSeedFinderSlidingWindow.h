/* Copyright (C) 2007-2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith [committer] */

/**
 * @file CbmSeedFinderSlidingWindow.h
 * @author Dominik Smith (d.smith@gsi.de)
 * @brief Class for sliding window seed finder
 * @version 0.1
 * @date 2021-09-14
 * 
 * @copyright Copyright (c) 2021
 * 
 *  This class creates event seeds which are later passed to the event builder, by sliding a 
 *  window of a fixed time duration through a vector of digis or of digi times and placing a
 *  seed at the center of the window whenever the minimum required number of digis is reached. 
 *  The purpose of this procedure is to speed up event building in the presence of noise in
 *  the reference detector(s).
*/

#ifndef CbmSeedFinderSlidingWindow_h
#define CbmSeedFinderSlidingWindow_h

#include <vector>

#include <stdint.h>

class CbmMatch;

class CbmSeedFinderSlidingWindow {
public:
  /**
   * @brief Create the CbmSeedFinderSlidingWindow object
   * @param vSeedTimes Output vector for the constructed seed times
   * @param minDigis Minimum number of digis which must be found in the seed window
   * @param dWindDur Size of sliding window
   * @param dDeadT ``Dead time'' i.e. time interval which is discarded after a seed is found
   */
  CbmSeedFinderSlidingWindow(std::vector<double>* vSeedTimes, int32_t minDigis, double dWindDur, double dDeadT)
    : fvSeedTimes(vSeedTimes)
    , fminDigis(minDigis)
    , fdWindDur(dWindDur)
    , fdDeadT(dDeadT) {};

  CbmSeedFinderSlidingWindow()                                  = delete;
  CbmSeedFinderSlidingWindow(const CbmSeedFinderSlidingWindow&) = delete;
  CbmSeedFinderSlidingWindow operator=(const CbmSeedFinderSlidingWindow&) = delete;

  /** @brief Enable or disable the generation of QA information. */
  void SetQa(bool doQA = true);
  /** @brief Output QA Information. */
  void OutputQa();

  /** @brief Destructor **/
  ~CbmSeedFinderSlidingWindow();

  /** @brief Function which builds event seeds 
   * @params Vector of input data (either digis or digi times). 
   * @params Optional vector of digi matches (should match input data to MC events, used for QA). 
  **/
  template<class inType>
  void FillSeedTimes(const std::vector<inType>* vIn, const std::vector<CbmMatch>* vDigiMatch = nullptr);

private:
  /** @brief Output of the algorithm. Stores seed times for current time slice. */
  std::vector<double>* fvSeedTimes = nullptr;

  /** @brief For QA. Matches that link constructed event seeds to MC events. */
  std::vector<CbmMatch>* fvEventMatches = nullptr;
  /** @brief For QA. Full vector of all event seeds that is not cleared at the end of a timeslice. */
  std::vector<double>* fvSeedTimesFull = nullptr;
  /** @brief For QA. Counts how many digis contributed to a seed. */
  std::vector<int32_t>* fvFullDigiCount = nullptr;
  /** @brief For QA. Counts how many noise digis contributed to a seed. */
  std::vector<int32_t>* fvNoiseDigiCount = nullptr;
  /** @brief For QA. Ratio of digis from matched MC event (disregarding noise). */
  std::vector<double>* fvCorrectDigiRatio = nullptr;
  /** @brief For QA. Ratio of digis of matched events that were included in event seed. */
  std::vector<double>* fvFoundDigiRatio = nullptr;

  /** @brief Minimum number of digis which must be found in the seed window. */
  int32_t fminDigis = 0;
  /** @brief Size of sliding window. */
  double fdWindDur = 0.;
  /** @brief ``Dead time'' i.e. time interval which is discarded after a seed is found. */
  double fdDeadT = 0.;

  /** @brief Fetches time at position i of either a digi vector or vector of times. */
  template<class inType>
  double GetTime(const std::vector<inType>* vIn, int32_t i);

  /** @brief For QA. Fills fvEventMatches (matches that link constructed event seeds to MC events). 
  * @params WinStart Starting position of seed window.
  * @params WinStart End position of seed window.
  * @params vDigiMatch Input vector of digi matches (should match input data to MC events). 
  */
  void FillEventMatch(int32_t WinStart, int32_t WinEnd, const std::vector<CbmMatch>* vDigiMatch);
};
#endif  //CbmSeedFinderSlidingWindow_tmpl
