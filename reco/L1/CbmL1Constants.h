/** @file CbmL1Constants.h
  * @copyright Copyright (C) 2012 Frankfurt Institute for Advanced Studies, Goethe-Universität Frankfurt, Frankfurt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Igor Kulakov [committer] **/

#ifndef CBML1CONSTANTS_H_
#define CBML1CONSTANTS_H_

namespace CbmL1Constants
{
  /// Performance constants
  const double MinRecoMom   = 0.1;  // Extra set of tracks = (MinRecoMom, MinRefMom)
  const double MinRefMom    = 1.;   // Primary set of tracks = (MinRefMom, +inf)  //All reco tracks = (MinRecoMom, +inf)
  const double MinPurity    = 0.7;  // min NStationInRecoTrack/NStationInMCTrack
  const double MinNStations = 4.;   // min number of stations in track to be reconstructable
}  // namespace CbmL1Constants

#endif
