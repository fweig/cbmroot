/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/// \file   CaToolsMCTrack.h
/// \brief  Class represents a MC track for CA tracking QA and performance (header)
/// \since  22.11.2022
/// \author S.Zharko <s.zharko@gsi.de> (based on CbmL1MCTrack class by I.Kisel and S.Gorbunov)

#include "CaToolsMCTrack.h"

#include "CbmL1Hit.h"

#include <iomanip>
#include <sstream>

#include "CaToolsMCPoint.h"

using ca::tools::MCTrack;


// ---------------------------------------------------------------------------------------------------------------------
//
void MCTrack::Clear()
{
  MCTrack other(std::move(*this));
  ClearPointIndexes();
  ClearHitIndexes();
  ClearRecoTrackIndexes();
  ClearTouchTrackIndexes();
}

// ---------------------------------------------------------------------------------------------------------------------
//
void MCTrack::InitHitsInfo(const L1Vector<CbmL1HitDebugInfo>& vHits)
{
  // NOTE: vHits must be sorted over stations!
  fMaxNofHitsOnStation    = 0;
  fTotNofStationsWithHit  = 0;
  fNofConsStationsWithHit = 0;

  int iStPrev                    = -1;  // Station index of previous hit
  int currNofConsStationsWithHit = 0;   // Current number of consecutive stations with a point in MC track
  int currMaxNofHitsOnStation    = 0;   // Current max number of hits on station

  // Loop over hit indexes assigned to this track
  for (int iH : fvHitIndexes) {
    int iSt = vHits[iH].iStation;  // current index of active station
    // Check if the track is going to the backward direction and is not reconstructable
    if (iSt < iStPrev) {
      fMaxNofHitsOnStation    = 0;
      fNofConsStationsWithHit = 0;
      //fTotNofStationsWithHit  = 0;  // TODO: Check, if this is correct
      return;
    }

    // Check if this hit is on the same station as the previous one and update max number of hits within a station a
    // station
    if (iSt == iStPrev) { currMaxNofHitsOnStation++; }  // the same station
    else {  // the next station (reset hits counter and update number of stations)
      if (currMaxNofHitsOnStation > fMaxNofHitsOnStation) { fMaxNofHitsOnStation = currMaxNofHitsOnStation; }
      currMaxNofHitsOnStation = 1;
      fTotNofStationsWithHit++;
    }

    // Check if this hit is on the next station comparing with the previous hit and update the number of consecutive
    // stations
    if (iSt - iStPrev == 1) { currNofConsStationsWithHit++; }
    else if (iSt - iStPrev > 1) {
      if (currNofConsStationsWithHit > fNofConsStationsWithHit) {
        fNofConsStationsWithHit = currNofConsStationsWithHit;
      }
      currNofConsStationsWithHit = 1;
    }

    iStPrev = iSt;
  }  // Loop over hit indexes assigned to this track: end
  if (currMaxNofHitsOnStation > fMaxNofHitsOnStation) { fMaxNofHitsOnStation = currMaxNofHitsOnStation; }
  if (currNofConsStationsWithHit > fNofConsStationsWithHit) { fNofConsStationsWithHit = currNofConsStationsWithHit; }
}

// ---------------------------------------------------------------------------------------------------------------------
//
void MCTrack::InitPointsInfo(const L1Vector<MCPoint>& vPoints)
{
  fMaxNofPointsOnStation   = 0;
  fMaxNofPointsOnSensor    = 0;
  fTotNofStationsWithPoint = 0;

  int currMaxNofPointsOnStation = 0;   // current max number of points within station
  int currMaxNofPointsOnSensor  = 0;   // current max number of points within sensor
  int iStPrev                   = -1;  // index of station for previous point
  float zPrev                   = 0;   // Z position of previous point

  // Loop over point indexes assigned to this track
  for (int iP : fvPointIndexes) {
    const auto& point = vPoints[iP];           // current MC point object
    int iSt           = point.GetStationId();  // current index of active station

    // Check if this point is on the same station as the previous one and update max number of points within a station
    if (iSt == iStPrev) { currMaxNofPointsOnStation++; }  // the same station
    else {  // the next station (reset points counter and update number of stations)
      if (currMaxNofPointsOnStation > fMaxNofPointsOnStation) { fMaxNofPointsOnStation = currMaxNofPointsOnStation; }
      currMaxNofPointsOnStation = 1;
      fTotNofStationsWithPoint++;
    }

    // Check if this point is on the same sensor as the previous one and update max number of points within a sensor
    // NOTE: points sometimes have different z positions within a sensor, so the max number of points within a sensor
    //       might be calculated incorrectly. (TODO: remove this variable?)
    if (point.GetZ() == zPrev) { currMaxNofPointsOnSensor++; }
    else {
      if (currMaxNofPointsOnSensor > fMaxNofPointsOnSensor) { fMaxNofPointsOnSensor = currMaxNofPointsOnSensor; }
      currMaxNofPointsOnSensor = 1;
    }

    iStPrev = iSt;
    zPrev   = point.GetZ();
  }  // Loop over point indexes assigned to this track

  if (currMaxNofPointsOnStation > fMaxNofPointsOnStation) { fMaxNofPointsOnStation = currMaxNofPointsOnStation; }
  if (currMaxNofPointsOnSensor > fMaxNofPointsOnSensor) { fMaxNofPointsOnSensor = currMaxNofPointsOnSensor; }

  fNofConsStationsWithPoint        = 0;
  int currNofConsStationsWithPoint = 0;  // current number of consecutive stations with points
  iStPrev                          = -1;
  // Loop over point indexes assigned to this track
  for (int iP : fvPointIndexes) {
    int iSt = vPoints[iP].GetStationId();
    // Check if this point is on the next station comparing with the previous point and update the number of consecutive
    // stations
    if (iSt - iStPrev == 1) { currNofConsStationsWithPoint++; }
    else if (iSt - iStPrev > 1) {
      if (currNofConsStationsWithPoint > fNofConsStationsWithPoint) {
        fNofConsStationsWithPoint = currNofConsStationsWithPoint;
      }
      currNofConsStationsWithPoint = 1;
    }

    if (iSt <= iStPrev) { continue; }  // Tracks propagating in backward direction
    iStPrev = iSt;
  }
  if (currNofConsStationsWithPoint > fNofConsStationsWithPoint) {
    fNofConsStationsWithPoint = currNofConsStationsWithPoint;
  }
}

// ---------------------------------------------------------------------------------------------------------------------
//
void MCTrack::SortPointIndexes(const std::function<bool(const int& lhs, const int& rhs)>& cmpFn)
{
  std::sort(fvPointIndexes.begin(), fvPointIndexes.end(), cmpFn);
}

// ---------------------------------------------------------------------------------------------------------------------
//
std::string MCTrack::ToString(int verbose, bool header) const
{
  using std::setfill;
  using std::setw;
  std::stringstream msg;
  if (header) {
    if (verbose > 0) {
      msg << setw(8) << setfill(' ') << "ID" << ' ';
      msg << setw(8) << setfill(' ') << "Mother" << ' ';
      msg << setw(8) << setfill(' ') << "PDG" << ' ';
      if (verbose > 1) {
        msg << setw(8) << setfill(' ') << "N h." << ' ';
        msg << setw(8) << setfill(' ') << "N p." << ' ';
        msg << setw(8) << setfill(' ') << "N r.tr." << ' ';
        if (verbose > 2) { msg << setw(8) << setfill(' ') << "N t.tr." << ' '; }
        msg << setw(12) << setfill(' ') << "zVTX [cm]" << ' ';
        msg << setw(12) << setfill(' ') << "t [ns]" << ' ';
        msg << setw(12) << setfill(' ') << "p [GeV/c]" << ' ';
      }
      msg << setw(8) << setfill(' ') << "rec-able" << ' ';
      msg << setw(8) << setfill(' ') << "rec-ed" << ' ';
    }
  }
  else {
    if (verbose > 0) {
      msg << setw(8) << setfill(' ') << fId << ' ';
      msg << setw(8) << setfill(' ') << fMotherId << ' ';
      msg << setw(8) << setfill(' ') << fPdgCode << ' ';
      if (verbose > 1) {
        msg << setw(8) << setfill(' ') << GetNofHits() << ' ';
        msg << setw(8) << setfill(' ') << GetNofPoints() << ' ';
        msg << setw(8) << setfill(' ') << GetNofRecoTracks() << ' ';
        if (verbose > 2) { msg << setw(8) << setfill(' ') << GetNofTouchTracks() << ' '; }
        msg << setw(12) << setfill(' ') << GetStartZ() << ' ';
        msg << setw(12) << setfill(' ') << GetStartT() << ' ';
        msg << setw(12) << setfill(' ') << GetP() << ' ';
      }
      msg << setw(8) << setfill(' ') << IsReconstructable() << ' ';
      msg << setw(8) << setfill(' ') << IsReconstructed() << ' ';
      if (verbose > 1) {
        msg << "\n\t- point indexes: ";
        for (int index : fvPointIndexes) {
          msg << index << ' ';
        }
        msg << "\n\t- hit indexes: ";
        for (int index : fvHitIndexes) {
          msg << index << ' ';
        }
        msg << "\n\t- reconstructed track indexes: ";
        for (int index : fvRecoTrackIndexes) {
          msg << index << ' ';
        }
        if (verbose > 2) {
          msg << "\n\t- touch track indexes: ";
          for (int index : fvTouchTrackIndexes) {
            msg << index << ' ';
          }
        }
      }
    }
  }
  return msg.str();
}
