/* Copyright (C) 2009-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Valentina Akishina, Ivan Kisel, Sergey Gorbunov [committer], Igor Kulakov, Maksym Zyzak */

/*
 *=====================================================
 *
 *  CBM Level 1 4D Reconstruction
 *
 *  Authors: V.Akishina, I.Kisel,  S.Gorbunov, I.Kulakov, M.Zyzak
 *  Documentation: V.Akishina
 *
 *  e-mail : v.akishina@gsi.de
 *
 *=====================================================
 *
 *  Finds tracks using the Cellular Automaton algorithm
 *
 */

#include <chrono>

#include "L1Algo.h"
#include "L1Track.h"

void L1Algo::CaTrackFinder()
{
  //
  // The main CaTrackFinder routine
  // It splits the input data into sub-timeslices
  // and runs the track finder over the sub-slices
  //

  auto timerStart = std::chrono::high_resolution_clock::now();

  // ----- Reset data arrays -------------------------------------------------------------------------------------------
  fvHitKeyFlags.reset(fInputData.GetNhitKeys(), 0);

  fRecoTracks.clear();
  fRecoHits.clear();
  fRecoHits.reserve(2 * fInputData.GetNhits());
  fRecoTracks.reserve(2 * fInputData.GetNhits() / fParameters.GetNstationsActive());

  fSliceHitIds.clear();
  fSliceHitIds.reserve(fInputData.GetNhits());

  fvHitKeyFlags.reset(fInputData.GetNhitKeys(), 0);
  fHitTimeInfo.reset(fInputData.GetNhits());

  // TODO: move these values to CbmL1Parameters namespace (S.Zharko)
  const fscal tsLength    = 10000;  // length of sub-TS
  const fscal tsOverlap   = 15;     // min length of overlap region
  fscal minProtonMomentum = 0.1;

  fscal targX = fParameters.GetTargetPositionX()[0];
  fscal targY = fParameters.GetTargetPositionY()[0];
  fscal targZ = fParameters.GetTargetPositionZ()[0];

  fscal tsStart = std::numeric_limits<fscal>::max();  // starting time of sub-TS

  // calculate possible event time for the hits (fHitTimeInfo array)

  for (int iS = 0; iS < fParameters.GetNstationsActive(); ++iS) {

    const L1Station& st = fParameters.GetStation(iS);
    int nStationHits    = fInputData.GetNhits(iS);

    fscal maxTimeBeforeHit = std::numeric_limits<fscal>::min();

    // loop in the reverse order to fill L1HitTimeInfo::fMinTimeAfterHit fields

    for (int ih = 0; ih < nStationHits; ++ih) {
      L1HitIndex_t caHitId = fInputData.GetStartHitIndex(iS) + ih;
      const L1Hit& h       = fInputData.GetHit(caHitId);

      auto [x, y] = st.ConvUVtoXY(h.u, h.v);
      fscal dx    = x - targX;
      fscal dy    = y - targY;
      fscal dz    = h.z - targZ;
      fscal l     = sqrt(dx * dx + dy * dy + dz * dz);

      fscal timeOfFlightMin = l * L1TrackPar::kClightNsInv;
      fscal timeOfFlightMax =
        1.5 * l * sqrt(1. + L1TrackPar::kProtonMass * L1TrackPar::kProtonMass / minProtonMomentum / minProtonMomentum)
        * L1TrackPar::kClightNsInv;
      fscal dt = 3.5 * sqrt(h.dt2);

      L1HitTimeInfo& info = fHitTimeInfo[caHitId];
      info.fEventTimeMin  = h.t - dt - timeOfFlightMax;
      info.fEventTimeMax  = h.t + dt - timeOfFlightMin;

      if (maxTimeBeforeHit < info.fEventTimeMax) { maxTimeBeforeHit = info.fEventTimeMax; }
      info.fMaxTimeBeforeHit = maxTimeBeforeHit;

      if (tsStart > info.fEventTimeMax - 5) {
        tsStart = info.fEventTimeMax - 5;  // 5 ns margin
      }
    }

    fscal minTimeAfterHit = std::numeric_limits<fscal>::max();

    for (int ih = nStationHits - 1; ih >= 0; --ih) {
      L1HitIndex_t caHitId = fInputData.GetStartHitIndex(iS) + ih;
      L1HitTimeInfo& info  = fHitTimeInfo[caHitId];
      if (minTimeAfterHit > info.fEventTimeMin) { minTimeAfterHit = info.fEventTimeMin; }
      info.fMinTimeAfterHit = minTimeAfterHit;
    }
  }

  // cut data into sub-timeslices and process them one by one

  bool areDataLeft = true;  // is the whole TS processed
  int nSubSlices   = 0;

  L1HitIndex_t sliceFirstHit[L1Constants::size::kMaxNstations] {0};

  for (int iS = 0; iS < fParameters.GetNstationsActive(); ++iS) {
    sliceFirstHit[iS] = fInputData.GetStartHitIndex(iS);
  }

  while (areDataLeft) {

    nSubSlices++;

    // select the sub-slice hits
    fSliceHitIds.clear();

    areDataLeft = false;

    // TODO: skip empty regions and start the subslice with the earliest hit

    for (int iS = 0; iS < fParameters.GetNstationsActive(); ++iS) {
      fSliceHitIdsStartIndex[iS] = fSliceHitIds.size();

      for (L1HitIndex_t caHitId = sliceFirstHit[iS]; caHitId < fInputData.GetStopHitIndex(iS); ++caHitId) {
        L1HitTimeInfo& info = fHitTimeInfo[caHitId];
        const L1Hit& h      = fInputData.GetHit(caHitId);
        if (fvHitKeyFlags[h.f] || fvHitKeyFlags[h.b]) {  // the hit is already reconstructed
          continue;
        }
        if (info.fEventTimeMin > tsStart + tsLength) {  // the hit is too late for the sub slice
          areDataLeft = true;
          if (info.fMinTimeAfterHit > tsStart + tsLength) {
            // this hit and all later hits are out of the sub-slice
            break;
          }
        }
        else {
          if (tsStart <= info.fEventTimeMax) {  // the hit belongs to the sub-slice
            fSliceHitIds.push_back(caHitId);
            if (info.fMaxTimeBeforeHit < tsStart + tsLength - tsOverlap) {
              // this hit and all hits before are before the overlap
              sliceFirstHit[iS] = caHitId + 1;
            }
          }
        }  // else the hit has been alread processed in previous sub-slices
      }

      fSliceHitIdsStopIndex[iS] = fSliceHitIds.size();
    }

    CaTrackFinderSlice();
    L1KFTrackFitter();

    // save reconstructed tracks with no hits in the overlap region

    tsStart += tsLength - tsOverlap;

    // we should add hits from reconstructed but not stored tracks to the new sub-timeslice
    // we do it in a simple way by extending the tsStartNew
    // TODO: only add those hits from the region before tsStartNew that belong to the not stored tracks

    int trackFirstHit = 0;
    for (L1Vector<L1Track>::iterator it = fSliceRecoTracks.begin(); it != fSliceRecoTracks.end();
         trackFirstHit += it->NHits, it++) {

      bool isTrackCompletelyInOverlap = true;
      for (int ih = 0; ih < it->NHits; ih++) {
        int caHitId         = fSliceRecoHits[trackFirstHit + ih];
        L1HitTimeInfo& info = fHitTimeInfo[caHitId];
        if (info.fEventTimeMax < tsStart) {  // this hit is before the overlap
          isTrackCompletelyInOverlap = false;
          break;
        }
      }

      if (!areDataLeft) {  // don't reject tracks in the overlap when no more data are left
        isTrackCompletelyInOverlap = 0;
      }

      if (isTrackCompletelyInOverlap) {
        //
        // Don't save tracks from the overlap region, since they might have additional hits in the next subslice
        //

        // release the track hits
        for (int i = 0; i < it->NHits; i++) {
          int caHitId        = fSliceRecoHits[trackFirstHit + i];
          const auto& h      = fInputData.GetHit(caHitId);
          fvHitKeyFlags[h.f] = 0;
          fvHitKeyFlags[h.b] = 0;
        }
      }
      else {  // save the track
        fRecoTracks.push_back(*it);
        // mark the track hits as used
        for (int i = 0; i < it->NHits; i++) {
          int caHitId        = fSliceRecoHits[trackFirstHit + i];
          const auto& h      = fInputData.GetHit(caHitId);
          fvHitKeyFlags[h.f] = 1;
          fvHitKeyFlags[h.b] = 1;
          fRecoHits.push_back(caHitId);
        }
      }
    }  // sub-timeslice tracks

    tsStart -= 5;  // do 5 ns margin
  }

  auto timerEnd = std::chrono::high_resolution_clock::now();
  fCaRecoTime   = (double) (std::chrono::duration<double>(timerEnd - timerStart).count());
  std::cout << "CaTracker: nSubSlices processed = " << nSubSlices << std::endl;
}
