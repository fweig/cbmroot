/** @file PerformanceConstants.h
  * @copyright Copyright (C) 2017 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Maxim Zyzak [orginator] **/

#ifndef _PerformanceConstants_h
#define _PerformanceConstants_h

namespace PerformanceConstants
{
  const double MinRecoMom   = 0.1;  // Extra set of tracks = (MinRecoMom, MinRefMom)
  const double MinRefMom    = 1.;   // Primary set of tracks = (MinRefMom, +inf)  //All reco tracks = (MinRecoMom, +inf)
  const double MinPurity    = 0.7;  // min NStationInRecoTrack/NStationInMCTrack
  const double MinNStations = 4.;   // min number of stations in track to be reconstructable
}  // namespace PerformanceConstants

#endif  //_Performance_h
