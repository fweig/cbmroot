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

#include <cstdint>
#include <iostream>
#include <vector>

class CbmSeedFinderQa {
public:
  /**
   * @brief Create the CbmSeedFinderQa object
   */
  CbmSeedFinderQa() {};
  CbmSeedFinderQa(const CbmSeedFinderQa&) = delete;
  CbmSeedFinderQa operator=(const CbmSeedFinderQa&) = delete;

  /** @brief Gather QA Information. 
  * @params WinStart Starting position of seed window.
  * @params WinStart End position of seed window.
  * @params vDigiMatch Input vector of digi matches (should match input data to MC events). 
  * @params seedTime Current seed. 
  **/
  void FillQaInfo(const int32_t WinStart, const int32_t WinEnd, const std::vector<CbmMatch>* vDigiMatch,
                  const double seedTime);

  /** @brief Output QA Information. */
  void OutputQa();

  /** @brief Destructor **/
  ~CbmSeedFinderQa() {};

  /** @brief Matches that link constructed event seeds to MC events. */
  std::vector<CbmMatch> fvEventMatches;
  /** @brief Full vector of all event seeds that is not cleared at the end of a timeslice. */
  std::vector<double> fvSeedTimesFull;
  /** @brief Counts how many digis contributed to a seed. */
  std::vector<int32_t> fvFullDigiCount;
  /** @brief Counts how many noise digis contributed to a seed. */
  std::vector<int32_t> fvNoiseDigiCount;
  /** @brief Ratio of digis from matched MC event (disregarding noise). */
  std::vector<double> fvCorrectDigiRatio;
  /** @brief Ratio of digis of matched events that were included in event seed. */
  std::vector<double> fvFoundDigiRatio;

private:
};
#endif  //CbmSeedFinderQa_h
