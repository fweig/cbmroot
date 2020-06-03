#include "Lx.h"
#ifdef MAKE_EFF_CALC
//#include <cmath>
#include <iostream>

using namespace std;

void LxFinder::MatchMCToReco()
{
  static Int_t referenceMCTracks = 0;
  static Int_t reconstructedRefTracks = 0;
  bool hasPositiveSignalMuon = false;
  bool hasNegativeSignalMuon = false;
  hasSignalInEvent = false;

  for (vector<LxMCTrack>::iterator i = MCTracks.begin(); i != MCTracks.end(); ++i)
  {
    LxMCTrack& mcTrack = *i;
    Int_t pdgCode = mcTrack.pdg;
    map<LxTrack*, Int_t> recoTracks;// Mapped value is the number of common points in MC and reconstructed tracks.
    memset(mcTrack.hitsOnStations, 0, sizeof(mcTrack.hitsOnStations));
    bool isSignal = true;
    //list<LxPoint*> mcTrackHits[LXSTATIONS][LXLAYERS];
    bool mcTrackMCPoints[LXSTATIONS][LXLAYERS];
    memset(mcTrackMCPoints, 0, sizeof(mcTrackMCPoints));

    for (vector<LxMCPoint*>::iterator j = mcTrack.Points.begin(); j != mcTrack.Points.end(); ++j)
    {
      LxMCPoint* pMCPoint = *j;
      mcTrackMCPoints[pMCPoint->stationNumber][pMCPoint->layerNumber] = true;

      for (list<LxPoint*>::iterator k = pMCPoint->lxPoints.begin(); k != pMCPoint->lxPoints.end(); ++k)
      {
        LxPoint* point = *k;
        mcTrack.hitsOnStations[point->layer->station->stationNumber][point->layer->layerNumber] = true;
        //mcTrackHits[point->layer->station->stationNumber][point->layer->layerNumber].push_back(point);
        LxTrack* track = point->track;

        if (0 == track)
          continue;

        if (track->matched)
          continue;

        if (track->clone)
          continue;

        map<LxTrack*, int>::iterator l = recoTracks.find(track);

        if (l != recoTracks.end())
          ++(l->second);
        else
          recoTracks[track] = 1;
      }// for (list<LxPoint*>::iterator k = pMCPoint->lxPoints.begin(); k != pMCPoint->lxPoints.end(); ++k)
    }// for (vector<LxMCPoint*>::iterator j = mcTrack.Points.begin(); j != mcTrack.Points.end(); ++j)

    bool enoughHits = true;
    bool enoughMCPoints = true;
    mcTrack.stationsWithHits = 0;
    mcTrack.layersWithHits = 0;
    Int_t mcpCount = 0;

    for (Int_t j = 0; j < LXSTATIONS; ++j)
    {
      Int_t hitsOnSt = 0;
      Int_t mcpOnSt = 0;

      for (Int_t k = 0; k < LXLAYERS; ++k)
      {
        if (mcTrack.hitsOnStations[j][k])
        {
          ++hitsOnSt;
          ++mcTrack.layersWithHits;
        }

        if (mcTrackMCPoints[j][k])
        {
          ++mcpOnSt;
          ++mcpCount;
        }
      }// for (Int_t k = 0; k < LXLAYERS; ++k)

      if (hitsOnSt < LXLAYERS)
      {
        if (j < caSpace.stationsInAlgo)
          enoughHits = false;
      }
      else
        ++mcTrack.stationsWithHits;

      if (j < caSpace.stationsInAlgo && mcpOnSt < LXLAYERS)
        enoughMCPoints = false;
    }// for (Int_t j = 0; j < LXSTATIONS; ++j)

    scaltype pt2 = mcTrack.px * mcTrack.px + mcTrack.py * mcTrack.py;

    if (!enoughMCPoints || mcTrack.mother_ID >= 0 || (pdgCode != 13 && pdgCode != -13) || (pPtCut && (mcTrack.p < 3.0 || pt2 < 1.0)))
      isSignal = false;
    else
      ++referenceMCTracks;

    if (!isSignal)
      continue;

    if (-13 == pdgCode)
      hasPositiveSignalMuon = true;
    else
      hasNegativeSignalMuon = true;

    LxTrack* matchTrack = 0;
    Int_t matchedPoints = 0;

    for (map<LxTrack*, Int_t>::iterator j = recoTracks.begin(); j != recoTracks.end(); ++j)
    {
      if (0 == matchTrack || matchedPoints < j->second + j->first->restoredPoints)
      {
        matchTrack = j->first;
        matchedPoints = j->second + j->first->restoredPoints;
      }
    }

    if (0 == matchTrack)
      continue;

    if (matchedPoints < mcpCount * 0.7)
      continue;

    ++reconstructedRefTracks;
  }// for (vector<LxMCTrack>::iterator i = MCTracks.begin(); i != MCTracks.end(); ++i)

  if (hasPositiveSignalMuon && hasNegativeSignalMuon)
  {
    hasSignalInEvent = true;
    ++signalCounter;
  }

  Double_t result = 100 * reconstructedRefTracks;
  result /= referenceMCTracks;
  cout << "LxFinder::MatchMCToReco(): efficiency = " << result << "% (" << reconstructedRefTracks << "/" << referenceMCTracks << ")" << endl;
}// void LxFinder::MatchMCToReco()

void LxFinder::MatchRecoToMC()
{
  static Int_t recoTracksCount = 0;
  static Int_t matchedTracksCount = 0;
  LxTrack* signalMuPlus = 0;
  LxTrack* signalMuMinus = 0;
  list<LxTrack*> bgrMuPlusTracks;
  list<LxTrack*> bgrMuMinusTracks;

  for (list<LxTrack*>::iterator i = caSpace.tracks.begin(); i != caSpace.tracks.end(); ++i)
  {
    LxTrack* recoTrack = *i;

    if (recoTrack->clone)
      continue;

    ++recoTracksCount;

    map<LxMCTrack*, Int_t> mcTracks;

    for (Int_t j = 0; j < LXSTATIONS * LXLAYERS; ++j)
    {
      LxPoint* recoPoint = recoTrack->points[j];

      if (0 == recoPoint)
        continue;

      for (list<LxMCPoint*>::iterator k = recoPoint->mcPoints.begin(); k != recoPoint->mcPoints.end(); ++k)
      {
        LxMCPoint* mcPoint = *k;
        LxMCTrack* mcTrack = mcPoint->track;

        if (0 == mcTrack)
          continue;

        map<LxMCTrack*, Int_t>::iterator mcIter = mcTracks.find(mcTrack);

        if (mcIter == mcTracks.end())
          mcTracks[mcTrack] = 1;
        else
          ++(mcIter->second);
      }
    }// for (Int_t j = 0; j < LXSTATIONS * LXLAYERS; ++j)

    LxMCTrack* bestMatch = 0;
    Int_t matchedPoints = 0;

    for (map<LxMCTrack*, Int_t>::iterator j = mcTracks.begin(); j != mcTracks.end(); ++j)
    {
      if (0 == bestMatch || j->second > matchedPoints)
      {
        bestMatch = j->first;
        matchedPoints = j->second;
      }
    }

    if (0 == bestMatch)
      continue;

    Int_t recoPoints = 0;

    for (Int_t j = 0; j < LXSTATIONS * LXLAYERS; ++j)
    {
      LxPoint* recoPoint = recoTrack->points[j];

      if (0 != recoPoint)
        ++recoPoints;
    }

    if (matchedPoints < 0.7 * recoPoints)
      continue;

    ++matchedTracksCount;

    LxRay* ray0 = recoTrack->rays[0];
    LxPoint* point0 = ray0->end;
    scaltype x0 = point0->x - point0->z * ray0->tx;
    scaltype y0 = point0->y - point0->z * ray0->ty;

    if (0 != bestMatch && bestMatch->mother_ID < 0 && (-13 == bestMatch->pdg || 13 == bestMatch->pdg))
    {
      signalXAtZ0->Fill(x0);
      signalYAtZ0->Fill(y0);

      if (-13 == bestMatch->pdg)
        signalMuPlus = recoTrack;
      else
        signalMuMinus = recoTrack;
    }
    else
    {
      bgrXAtZ0->Fill(x0);
      bgrYAtZ0->Fill(y0);
      scaltype charge = ray0->tx - point0->x / point0->z;

      if (charge > 0)
        bgrMuPlusTracks.push_back(recoTrack);
      else if (charge < 0)
        bgrMuMinusTracks.push_back(recoTrack);
    }
  }// for (list<LxTrack*>::iterator i = caSpace.tracks.begin(); i != caSpace.tracks.end(); ++i)

  Double_t result = 100 * matchedTracksCount;
  result /= recoTracksCount;
  cout << "LxFinder::MatchRecoToMC(): efficiency = " << result << "% (" << matchedTracksCount << "/" << recoTracksCount << ")" << endl;

  if (0 != signalMuPlus && 0 != signalMuMinus)
  {
    scaltype x1 = signalMuPlus->rays[0]->end->x;
    scaltype y1 = signalMuPlus->rays[0]->end->y;
    scaltype z1 = signalMuPlus->rays[0]->end->z;
    scaltype x2 = signalMuMinus->rays[0]->end->x;
    scaltype y2 = signalMuMinus->rays[0]->end->y;
    scaltype z2 = signalMuMinus->rays[0]->end->z;
    scaltype deltaX = x1 - x2;
    scaltype deltaY = y1 - y2;
    scaltype deltaZ = z1 - z2;
    scaltype d = sqrt(deltaX * deltaX + deltaY * deltaY);
    signalInterTracksDistanceOn1st->Fill(d);
    scaltype aSq = x1 * x1 + y1 * y1 + z1 * z1;
    scaltype a = sqrt(aSq);
    scaltype bSq = x2 * x2 + y2 * y2 + z2 * z2;
    scaltype b = sqrt(bSq);
    scaltype cSq = deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ;
    scaltype cosC = (aSq + bSq - cSq) / (2 * a * b);
    scaltype C = acos(cosC);
    C *= 180;
    C /= 3.14159265;
    signalInterTracksAngle->Fill(C);
    signalInterTrackCorrDA->Fill(d, C);
  }// if (0 != signalMuPlus && 0 != signalMuMinus)

  scaltype bgrMuDistSq = 0;
  scaltype bgrMuAngle = 0;

  for (list<LxTrack*>::iterator i = bgrMuPlusTracks.begin(); i != bgrMuPlusTracks.end(); ++i)
  {
    LxTrack* mupt = *i;
    scaltype x1 = mupt->rays[0]->end->x;
    scaltype y1 = mupt->rays[0]->end->y;
    scaltype z1 = mupt->rays[0]->end->z;

    for (list<LxTrack*>::iterator j = bgrMuMinusTracks.begin(); j != bgrMuMinusTracks.end(); ++j)
    {
      LxTrack* mumt = *j;
      scaltype x2 = mumt->rays[0]->end->x;
      scaltype y2 = mumt->rays[0]->end->y;
      scaltype z2 = mumt->rays[0]->end->z;
      scaltype deltaX = x1 - x2;
      scaltype deltaY = y1 - y2;
      scaltype deltaZ = z1 - z2;
      scaltype dSq = deltaX * deltaX + deltaY * deltaY;

      if (dSq > bgrMuDistSq)
        bgrMuDistSq = dSq;

      scaltype aSq = x1 * x1 + y1 * y1 + z1 * z1;
      scaltype a = sqrt(aSq);
      scaltype bSq = x2 * x2 + y2 * y2 + z2 * z2;
      scaltype b = sqrt(bSq);
      scaltype cSq = deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ;
      scaltype cosC = (aSq + bSq - cSq) / (2 * a * b);
      scaltype C = acos(cosC);
      C *= 180;
      C /= 3.14159265;

      if (C > bgrMuAngle)
        bgrMuAngle = C;
    }// for (list<LxTrack*>::iterator j = bgrMuMinusTracks.begin(); j != bgrMuMinusTracks.end(); ++j)
  }// for (list<LxTrack*>::iterator i = bgrMuPlusTracks.begin(); i != bgrMuPlusTracks.end(); ++i)

  if (bgrMuDistSq > 0)
    bgrInterTracksDistanceOn1st->Fill(sqrt(bgrMuDistSq));

  if (bgrMuAngle > 0)
    bgrInterTracksAngle->Fill(bgrMuAngle);

  if (bgrMuDistSq > 0 && bgrMuAngle > 0)
    bgrInterTrackCorrDA->Fill(sqrt(bgrMuDistSq), bgrMuAngle);
}// void LxFinder::MatchRecoToMC()

#endif//MAKE_EFF_CALC
