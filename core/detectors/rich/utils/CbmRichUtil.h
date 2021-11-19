/* Copyright (C) 2017-2021 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer], Semen Lebedev */

#ifndef RICH_CbmRichUtil
#define RICH_CbmRichUtil

#include <map>
#include <vector>

#include <stdint.h>

class CbmDigiManager;
class TClonesArray;
class CbmMCDataArray;
class TH2D;

class CbmRichUtil {

public:
  static double GetRingTrackDistance(int globalTrackId);
  static double GetRingTrackDistanceX(int globalTrackId);
  static double GetRingTrackDistanceY(int globalTrackId);

  // Create PMT XY histograms
  static std::vector<double> GetPmtHistXbins();
  static std::vector<double> GetPmtHistYbins();
  static std::vector<double> GetPmtHistBins(bool isX);

  static std::map<std::pair<int, int>, int> CreateNofHitsInRingMap(TClonesArray* richHits, CbmMCDataArray* richPoints,
                                                                   CbmMCDataArray* mcTracks, CbmDigiManager* digiMan);

  static uint16_t GetDirichId(int Address) { return ((Address >> 16) & 0xFFFF); }

  static uint16_t GetDirichChannel(int Address) { return (Address & 0xFFFF); }

private:
  /**
	 * \brief Return a vector with total distance and x, y components. [0] - total distance, [1] - x component, [2] - y component
	 */
  static std::vector<double> GetRingTrackDistanceImpl(int globalTrackId);
};

#endif
