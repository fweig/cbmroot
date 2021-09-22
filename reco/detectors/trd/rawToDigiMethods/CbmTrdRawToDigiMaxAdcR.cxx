/* Copyright (C) 2021 Goethe-University Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig [committer] */

#include "CbmTrdRawToDigiMaxAdcR.h"

#include <FairLogger.h>

#include <algorithm>
#include <iostream>
#include <iterator>

//---- CbmTrdRawToDigiMaxAdcR ----
CbmTrdRawToDigiMaxAdcR::CbmTrdRawToDigiMaxAdcR() : CbmTrdRawToDigiBaseR() {}

// --- GetCharge ----
Float_t CbmTrdRawToDigiMaxAdcR::GetMaxAdcValue(const std::vector<std::int16_t>* samples)
{
  // Safety for corrupted input samples
  if (samples->size() < fPeakingBinMin) {
    LOG(error) << "CbmTrdRawToDigiMaxAdcR::GetCharge samples.size() = " << samples->size()
               << " fPeakingBinMin = " << fPeakingBinMin;
    return 0;
  }

  // The signal should peak at the shaping time.
  // The corresponding sample is the peaking time divided by the sample length.
  auto itbegin = std::next(samples->begin(), fPeakingBinMin);

  // Check if the expected maximum position of the peaking bin exceeds the size of the vector
  auto nsamples      = samples->size();
  auto peakingBinMax = (nsamples - 1) > fPeakingBinMax ? fPeakingBinMax : nsamples;
  auto itend         = std::next(samples->begin(), peakingBinMax);

  // Get the maximum element
  auto itmax = std::max_element(itbegin, itend);

  Float_t charge = static_cast<Float_t>(*itmax);

  // Correct for the baseline
  charge -= GetBaseline(samples);


  // Remark: Due to the fact, that we store the charge UInt_t in the Digi values below 0 are not allowed.
  // In this case the above only appears if the baseline fluctuated above all values in the applied peaking range. This can only happen for forced neighbor triggers with a deposited charged that can not be separated from the baseline.
  return charge > 0 ? charge : 0;
}


ClassImp(CbmTrdRawToDigiMaxAdcR)
