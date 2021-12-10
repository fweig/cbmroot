/* Copyright (C) 2009-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** CbmStsTrackMatch.h
 *@author V.Friese <v.friese@gsi.de>
 *@since 07.05.2009
 **
 ** Data structure describing the matching of a reconstructed track
 ** with a Monte Carlo track on the base of corresponding hits/points.
 ** This requires matching of hits to MC points.
 **/


#ifndef CBMTRACKMATCH_H
#define CBMTRACKMATCH_H 1

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <TObject.h>     // for TObject

#include <cstdint>

class CbmTrackMatch : public TObject {

public:
  /** Default constructor **/
  CbmTrackMatch();


  /** Standard constructor 
  *@param mcTrackID   Index of matched MCTrack
  *@param nTrue       Number of true hits (belonging to matched MCTrack)
  *@param nWrong      Number of wrong Hits (from other MCTracks)
  *@param nFake       Number of fake hits (not belonging to any MCTrack)
  *@param nTracks     Number of MCTracks with common hits
  **/
  CbmTrackMatch(int32_t mcTrackID, int32_t nTrue, int32_t nWrong, int32_t nFake, int32_t nTracks);


  /** Destructor **/
  virtual ~CbmTrackMatch();


  /** Index of matched MC track **/
  int32_t GetMCTrackId() const { return fMCTrackId; };

  /** Number of true hits on track (from matched MC track) **/
  int32_t GetNofTrueHits() const { return fNofTrueHits; };

  /** Number of wrong hits on track (from other MC tracks) **/
  int32_t GetNofWrongHits() const { return fNofWrongHits; };

  /** Number of fake hits on track (from no MC track) **/
  int32_t GetNofFakeHits() const { return fNofFakeHits; };

  /** Number of MCTracks with common hits **/
  int32_t GetNofMCTracks() const { return fNofMCTracks; };


private:
  /** Index of matched CbmMCTrack  **/
  int32_t fMCTrackId;

  /** Number of true hits (belonging to the matched MCTrack) **/
  int32_t fNofTrueHits;

  /** Number of wrong hits (belonging to other MCTracks) **/
  int32_t fNofWrongHits;

  /** Number of fake hits (belonging to no MCTrack) **/
  int32_t fNofFakeHits;

  /** Number of MCTrackx with common hits **/
  int32_t fNofMCTracks;


  ClassDef(CbmTrackMatch, 1);
};


#endif
