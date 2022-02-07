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

#include <cstddef>
#include <cstdint>
#include <vector>

class CbmMatch;
class CbmSeedFinderQa;

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

  /** @brief Returns number of seed times currently stored in buffer. */
  size_t GetNofSeeds() { return fvSeedTimes->size(); }

  /** @brief Initializes QA object if set. */
  void Init();

private:
  /** @brief Processes QA info. */
  CbmSeedFinderQa* fQa = nullptr;

  /** @brief Output of the algorithm. Stores seed times for current time slice. */
  std::vector<double>* fvSeedTimes = nullptr;

  /** @brief Minimum number of digis which must be found in the seed window. */
  int32_t fminDigis = 0;
  /** @brief Size of sliding window. */
  double fdWindDur = 0.;
  /** @brief ``Dead time'' i.e. time interval which is discarded after a seed is found. */
  double fdDeadT = 0.;

  /** @brief Fetches time at position i of either a digi vector or vector of times. */
  template<class inType>
  double GetTime(const std::vector<inType>* vIn, int32_t i);
};
#endif  //CbmSeedFinderSlidingWindow_h
