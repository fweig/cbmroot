/** @file CbmTofHitFinderTBQA.cxx
  * @copyright Copyright (C) 2017-2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Pierre-Alain Loizeau [committer] **/

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "CbmTofHitFinderTBQA.h"

#include "CbmMCDataManager.h"
#include "CbmMatch.h"
#include "CbmTofAddress.h"
#include "CbmTofHit.h"
#include "CbmTofPoint.h"

#include <Logger.h>

#include "TH1.h"
#include <TFile.h>

using std::cout;
using std::endl;
using std::list;
using std::pair;
using std::set;
using std::vector;

/*
struct TofHitDesc
{
   CbmTofHit hit;
   list<const CbmTofPoint*> pointRefs;
};

struct TofPointDesc
{
   CbmTofPoint point;
   Int_t trackId;
   list<const CbmTofHit*> hitRefs;
};

struct TrackDesc
{
   list<CbmTofPoint*> tofPoints;
   list<CbmTofDigiExp> topDigis;
   list<CbmTofDigiExp> bottomDigis;
   list<TofHitDesc*> tofHits;
};

static vector<vector<TofHitDesc> > tofHitsAll;
static vector<vector<TofPointDesc> > tofPointsAll;
static vector<vector<TrackDesc> > tofTracksAll;
*/

struct QAMCTrack;

struct QAMCPoint {
  double x;
  double y;
  double t;
  int detId;
  QAMCTrack* track;
  bool isInit;

  //   QAMCPoint() : x(0.), y(0.), t(0.), detId(0), track(nullptr), isInit(false) {}
};

struct QAHit {
  double x;
  double dx;
  double y;
  double dy;
  double t;
  double dt;
  set<const QAMCPoint*> points;
  set<const QAMCTrack*> tracks;
  int detId;

  QAHit() : x(0.), dx(0.), y(0.), dy(0.), t(0.), dt(0.), points(), tracks(), detId(0) {}

  QAHit(double _x, double _dx, double _y, double _dy, double _t, double _dt, set<const QAMCPoint*> _points,
        set<const QAMCTrack*> _tracks, int _detId)
    : x(_x)
    , dx(_dx)
    , y(_y)
    , dy(_dy)
    , t(_t)
    , dt(_dt)
    , points(_points)
    , tracks(_tracks)
    , detId(_detId)
  {
  }
};

struct QAMCTrack {
  list<const QAMCPoint*> points;
  list<const QAHit*> hits;
  QAMCTrack() : points(), hits() {}
};

static vector<vector<QAMCTrack>> mcTracks;
static vector<vector<QAMCPoint>> mcPoints;
static vector<vector<QAHit>> hits;

static TH1F* deltaTHisto             = 0;
static TH1F* deltaXHisto             = 0;
static TH1F* deltaYHisto             = 0;
static TH1F* pullTHisto              = 0;
static TH1F* pullXHisto              = 0;
static TH1F* pullYHisto              = 0;
static TH1F* nofHitsHisto            = 0;
static TH1F* nofTracksDepositedHisto = 0;

CbmTofHitFinderTBQA::CbmTofHitFinderTBQA()
  : isEvByEv(false)
  , fTofHits(0)
  , fTofDigiMatchs(0)
  , fTofDigis(0)
  , fTofDigiPointMatchs(0)
  , fTofMCPoints(0)
  , fMCTracks(0)
  , fTimeSlice(0)
  , fEventList(0)
{
}

InitStatus CbmTofHitFinderTBQA::Init()
{
  FairRootManager* ioman = FairRootManager::Instance();

  if (0 == ioman) LOG(fatal) << "No FairRootManager";

  fTofHits            = static_cast<TClonesArray*>(ioman->GetObject("TofHit"));
  fTofDigiMatchs      = static_cast<TClonesArray*>(ioman->GetObject("TofDigiMatch"));
  fTofDigis           = static_cast<TClonesArray*>(ioman->GetObject("TofDigiExp"));
  fTofDigiPointMatchs = static_cast<TClonesArray*>(ioman->GetObject("TofDigiMatchPoints"));

  CbmMCDataManager* mcManager = static_cast<CbmMCDataManager*>(ioman->GetObject("MCDataManager"));
  fTofMCPoints                = mcManager->InitBranch("TofPoint");
  fMCTracks                   = mcManager->InitBranch("MCTrack");
  /*tofHitsAll.resize(1000);
   tofPointsAll.resize(1000);
   tofTracksAll.resize(1000);
   
   for (int i = 0; i < 1000; ++i)
   {
      Int_t nofmct = fMCTracks->Size(0, i);
      
      for (int j = 0; j < nofmct; ++j)
         tofTracksAll[i].push_back( { {}, {}, {}, {} } );
      
      Int_t evSize = fTofMCPoints->Size(0, i);
      
      for (int j = 0; j < evSize; ++j)
      {
         CbmTofPoint tp = *static_cast<const CbmTofPoint*> (fTofMCPoints->Get(0, i, j));
         tofPointsAll[i].push_back( { tp, tp.GetTrackID(), {} } );
         tofTracksAll[i][tp.GetTrackID()].tofPoints.push_back(&(tofPointsAll[i].back().point));
      }
   }*/

  mcTracks.resize(1000);
  mcPoints.resize(1000);
  hits.resize(1000);

  fTimeSlice = static_cast<CbmTimeSlice*>(ioman->GetObject("TimeSlice."));

  if (0 == fTimeSlice) LOG(fatal) << "No time slice";

  fEventList = static_cast<CbmMCEventList*>(ioman->GetObject("MCEventList."));

  if (0 == fEventList) LOG(fatal) << "No event list";

  for (int i = 0; i < 1000; ++i) {
    vector<QAMCTrack>& evMcTracks = mcTracks[i];
    vector<QAMCPoint>& evMcPoints = mcPoints[i];

    Int_t nofmct = fMCTracks->Size(0, i);

    if (nofmct > 0) evMcTracks.resize(nofmct);

    Int_t nofmcp = fTofMCPoints->Size(0, i);

    if (nofmcp > 0) evMcPoints.resize(nofmcp);

    for (int j = 0; j < nofmcp; ++j) {
      const CbmTofPoint* tp = static_cast<const CbmTofPoint*>(fTofMCPoints->Get(0, i, j));
      int trackId           = tp->GetTrackID();
      evMcPoints[j]         = {tp->GetX(), tp->GetY(), tp->GetTime(), tp->GetDetectorID(), &evMcTracks[trackId], false};
      const QAMCPoint* lastPoint = &evMcPoints[j];
      evMcTracks[trackId].points.push_back(lastPoint);
    }
  }

  deltaTHisto             = new TH1F("deltaTHisto", "deltaTHisto", 100, -1., 1.);
  deltaXHisto             = new TH1F("deltaXHisto", "deltaXHisto", 100, -5., 5.);
  deltaYHisto             = new TH1F("deltaYHisto", "deltaYHisto", 100, -5., 5.);
  pullTHisto              = new TH1F("pullTHisto", "pullTHisto", 100, -5., 5.);
  pullXHisto              = new TH1F("pullXHisto", "pullXHisto", 100, -5., 5.);
  pullYHisto              = new TH1F("pullYHisto", "pullYHisto", 100, -5., 5.);
  nofHitsHisto            = new TH1F("nofHitsHisto", "nofHitsHisto", 5, 0., 5.);
  nofTracksDepositedHisto = new TH1F("nofTracksDepositedHisto", "nofTracksDepositedHisto", 8, 0., 8.);

  return kSUCCESS;
}

static Int_t currentEvN = 0;
// static int minEntry = 100000;   (VF) not used
// static int maxEntry = -100000;   (VF) not used
static int globalNofHits  = 0;
static int globalNofDigis = 0;

void CbmTofHitFinderTBQA::Exec(Option_t*)
{
  /*vector<TofHitDesc>& tofHitsEv = tofHitsAll[currentEvN];
   Int_t nofHits = fTofHits->GetEntriesFast();
   
   for (Int_t i = 0; i < nofHits; ++i)
   {
      CbmTofHit hit = *static_cast<const CbmTofHit*> (fTofHits->At(i));
      tofHitsEv.push_back( { hit, {} } );
      TofHitDesc& lastHit = tofHitsEv.back();
      int nofRefs = lastHit.pointRefs.size();
      const CbmMatch* pDigiMatch = static_cast<const CbmMatch*> (fTofDigiMatchs->At(i));
      Int_t nofDigis = pDigiMatch->GetNofLinks();
      set<pair<Int_t, Int_t> > trackIds;

      for (Int_t j = 0; j < nofDigis; ++j)
      {
         const CbmLink& lnk = pDigiMatch->GetLink(j);
         Int_t digiInd = lnk.GetIndex();
         Int_t digiEvN = lnk.GetEntry();
         CbmTofDigiExp* pDigi = static_cast<CbmTofDigiExp*> (fTofDigis->At(digiInd));
         const CbmMatch* pPointMatch = pDigi->GetMatch();
         Int_t nofPoints = pPointMatch->GetNofLinks();
         
         for (Int_t k = 0; k < nofPoints; ++k)
         {
            const CbmLink& pointLnk = pPointMatch->GetLink(k);
            Int_t evN = pointLnk.GetEntry() - 1;
            Int_t pointInd = pointLnk.GetIndex();
            
            int qq = 0;
            
            if (evN >= tofPointsAll.size())
               ++qq;
            else if (pointInd >= tofPointsAll[evN].size())
               ++qq;
            
            lastHit.pointRefs.push_back(&(tofPointsAll[evN][pointInd].point));
            tofPointsAll[evN][pointInd].hitRefs.push_back(&(lastHit.hit));
            
            if (tofPointsAll[evN][pointInd].trackId >= tofTracksAll[evN].size())
               ++qq;
            
            if (tofPointsAll[evN][pointInd].trackId < 0)
               ++qq;
            
            trackIds.insert( { evN, tofPointsAll[evN][pointInd].trackId } );
         }
      }
      
      for (set<pair<Int_t, Int_t> >::const_iterator j = trackIds.begin(); j != trackIds.end(); ++j)
         tofTracksAll[j->first][j->second].tofHits.push_back(&lastHit);
   }
   
   Int_t nofAllDigis = fTofDigis->GetEntriesFast();
   
   for (Int_t i = 0; i < nofAllDigis; ++i)
   {
      const CbmTofDigiExp* pDigi = static_cast<const CbmTofDigiExp*> (fTofDigis->At(i));
      const CbmMatch* pPointMatch = pDigi->GetMatch();
      Int_t nofPoints = pPointMatch->GetNofLinks();
      set<pair<Int_t, Int_t> > trackIds;
      
      for (Int_t j = 0; j < nofPoints; ++j)
      {
         const CbmLink& pointLnk = pPointMatch->GetLink(j);
         
         int entry = pointLnk.GetEntry();
         
         if (minEntry > entry)
            minEntry = entry;
         
         if (maxEntry < entry)
            maxEntry = entry;
         
         Int_t evN = pointLnk.GetEntry() - 1;
         Int_t pointInd = pointLnk.GetIndex();
         trackIds.insert( { evN, tofPointsAll[evN][pointInd].trackId } );
      }
      
      for (set<pair<Int_t, Int_t> >::const_iterator j = trackIds.begin(); j != trackIds.end(); ++j)
      {
         if (0 == pDigi->GetSide())
            tofTracksAll[j->first][j->second].topDigis.push_back(*pDigi);
         else
            tofTracksAll[j->first][j->second].bottomDigis.push_back(*pDigi);
      }
   }*/

  hits.push_back({});
  vector<QAHit>& evHits = hits[currentEvN];
  Int_t nofHits         = fTofHits->GetEntriesFast();
  evHits.resize(nofHits);
  globalNofHits += nofHits;
  Int_t nofDigis = fTofDigis->GetEntriesFast();
  globalNofDigis += nofDigis;

  for (int i = 0; i < nofHits; ++i) {
    const CbmTofHit* hit     = static_cast<const CbmTofHit*>(fTofHits->At(i));
    const CbmMatch* hitMatch = static_cast<const CbmMatch*>(fTofDigiMatchs->At(i));
    //      evHits[i] = { hit->GetX(), hit->GetDx(), hit->GetY(), hit->GetDy(), hit->GetTime(), hit->GetTimeError(), {}, {}, CbmTofAddress::GetModFullId(hit->GetAddress()) } ;
    evHits[i]      = QAHit {hit->GetX(),
                       hit->GetDx(),
                       hit->GetY(),
                       hit->GetDy(),
                       hit->GetTime(),
                       hit->GetTimeError(),
                       set<const QAMCPoint*> {},
                       set<const QAMCTrack*> {},
                       CbmTofAddress::GetModFullId(hit->GetAddress())};
    QAHit& lastHit = evHits[i];

    nofDigis = hitMatch->GetNofLinks();

    for (int j = 0; j < nofDigis; ++j) {
      const CbmLink& lnk = hitMatch->GetLink(j);
      Int_t digiInd      = lnk.GetIndex();
      // CbmTofDigiExp* pDigi = static_cast<CbmTofDigiExp*> (fTofDigis->At(digiInd));   (VF) not used
      const CbmMatch* pPointMatch = static_cast<const CbmMatch*>(fTofDigiPointMatchs->At(digiInd));

      Int_t nofPoints = pPointMatch->GetNofLinks();

      for (Int_t k = 0; k < nofPoints; ++k) {
        const CbmLink& pointLnk = pPointMatch->GetLink(k);
        Int_t evN               = pointLnk.GetEntry() - 1;
        Int_t pointInd          = pointLnk.GetIndex();
        lastHit.points.insert(&mcPoints[evN][pointInd]);
        lastHit.tracks.insert(mcPoints[evN][pointInd].track);

        if (!mcPoints[evN][pointInd].isInit) {
          double eventTime = fEventList->GetEventTime(evN + 1, 0);
          mcPoints[evN][pointInd].t += eventTime;
          mcPoints[evN][pointInd].isInit = true;
        }
      }
    }

    set<QAMCTrack*> tracks;

    for (set<const QAMCPoint*>::const_iterator j = lastHit.points.begin(); j != lastHit.points.end(); ++j) {
      const QAMCPoint* point = *j;
      tracks.insert(point->track);
    }

    for (set<QAMCTrack*>::iterator j = tracks.begin(); j != tracks.end(); ++j) {
      QAMCTrack* track = *j;
      track->hits.push_back(&lastHit);
    }
  }

  ++currentEvN;
}

static void SaveHisto(TH1* histo)
{
  TFile* curFile    = TFile::CurrentFile();
  TString histoName = histo->GetName();
  histoName += ".root";
  TFile fh(histoName.Data(), "RECREATE");
  histo->Write();
  fh.Close();
  delete histo;
  TFile::CurrentFile() = curFile;
}

void CbmTofHitFinderTBQA::Finish()
{
  /*cout << "min entry = " << minEntry << endl;
   cout << "max entry = " << maxEntry << endl;
   int nofHits = 0;
   int nofPoints = 0;
   int nofPointsWithHits = 0;
   int nofMCTracks = 0;
   int nofMCTracksWithHits = 0;
   int nofMCTracksWithDigis = 0;
   int nofMCTracksWithBothDigis = 0;
   
   for (int i = 0; i < 1000; ++i)
   {
      nofHits += tofHitsAll[i].size();
      nofPoints += tofPointsAll[i].size();
      
      for (int j = 0; j < tofPointsAll[i].size(); ++j)
      {
         if (!tofPointsAll[i][j].hitRefs.empty())
            ++nofPointsWithHits;
      }
      
      for (int j = 0; j < tofTracksAll[i].size(); ++j)
      {
         if (!tofTracksAll[i][j].tofPoints.empty())
         {
            ++nofMCTracks;
            
            if (!tofTracksAll[i][j].tofHits.empty())
            {
               ++nofMCTracksWithHits;
               
               for(list<TofHitDesc*>::const_iterator k = tofTracksAll[i][j].tofHits.begin(); k != tofTracksAll[i][j].tofHits.end(); ++k)
               {
                  const TofHitDesc* hitDesc = *k;
                  const CbmTofHit* hit = &(hitDesc->hit);
                  set<Int_t> detIds;
                  int nofRefs = hitDesc->pointRefs.size();
                  
                  for (list<const CbmTofPoint*>::const_iterator l = hitDesc->pointRefs.begin(); l != hitDesc->pointRefs.end(); ++l)
                  {
                     const CbmTofPoint* point = *l;
                     detIds.insert(point->GetDetectorID());
                  }
                  
                  Double_t x = 0;
                  Double_t y = 0;
                  int nofPts = 0;

                  for(list<CbmTofPoint*>::const_iterator l = tofTracksAll[i][j].tofPoints.begin(); l != tofTracksAll[i][j].tofPoints.end(); ++l)
                  {
                     const CbmTofPoint* point = *l;
                     
                     if (detIds.find(point->GetDetectorID()) == detIds.end())
                        continue;
                     
                     x += point->GetX();
                     y += point->GetY();
                     ++nofPts;
                  }
                  
                  if (nofPts > 0)
                  {
                     deltaXHisto->Fill(hit->GetX() - x / nofPts);
                     deltaYHisto->Fill(hit->GetY() - y / nofPts);
                  }
                  
                  nofHitsHisto->Fill(tofTracksAll[i][j].tofHits.size());
               }
            }
            
            if (!tofTracksAll[i][j].topDigis.empty() || !tofTracksAll[i][j].bottomDigis.empty())
               ++nofMCTracksWithDigis;
            
            if (!tofTracksAll[i][j].topDigis.empty() && !tofTracksAll[i][j].bottomDigis.empty())
            {
               ++nofMCTracksWithBothDigis;
               
               if (tofTracksAll[i][j].tofHits.empty())
               {
                  for (list<CbmTofDigiExp>::const_iterator k = tofTracksAll[i][j].topDigis.begin(); k != tofTracksAll[i][j].topDigis.end(); ++k)
                  {
                     for (list<CbmTofDigiExp>::const_iterator l = tofTracksAll[i][j].bottomDigis.begin(); l != tofTracksAll[i][j].bottomDigis.end(); ++l)
                        deltaTHisto->Fill(k->GetTime() - l->GetTime());
                  }
               }
            }
         }
      }
   }
   
   cout << "NOF hits = " << nofHits << ", NOF MC points = " << nofPoints << ", NOF MC points with hits = " << nofPointsWithHits << ", NOF MC tracks = " << nofMCTracks
      << ", NOF MC tracks with hits = " << nofMCTracksWithHits << ", NOF MC tracks with digis = " << nofMCTracksWithDigis << ", NOF MC tracks with both digis = "
      << nofMCTracksWithBothDigis << endl;*/

  int nofTracksTof = 0;
  int nofTracksHit = 0;
  int nofMCEvents  = 0;

  for (vector<vector<QAMCTrack>>::const_iterator i = mcTracks.begin(); i != mcTracks.end(); ++i) {
    const vector<QAMCTrack>& evTracks = *i;

    if (!evTracks.empty()) ++nofMCEvents;

    for (vector<QAMCTrack>::const_iterator j = evTracks.begin(); j != evTracks.end(); ++j) {
      const QAMCTrack& track = *j;

      if (!track.points.empty()) {
        ++nofTracksTof;

        if (!track.hits.empty()) ++nofTracksHit;
      }

      for (list<const QAHit*>::const_iterator k = track.hits.begin(); k != track.hits.end(); ++k) {
        const QAHit* hit = *k;
        //set<int> detIds;
        set<const QAMCPoint*> hitPoints;

        //if (hit->points.empty())
        //continue;

        double x = 0;
        double y = 0;
        double t = 0;

        /*for (set<const QAMCPoint*>::const_iterator l = hit->points.begin(); l != hit->points.end(); ++l)
            {
               const QAMCPoint* point = *l;
               //x += point->x;
               //y += point->y;
               //t += point->t;
               detIds.insert(point->detId);
            }*/

        for (list<const QAMCPoint*>::const_iterator l = track.points.begin(); l != track.points.end(); ++l) {
          const QAMCPoint* point = *l;

          //if (detIds.find(point->detId) != detIds.end())
          if (point->isInit && hit->detId == point->detId) hitPoints.insert(point);
        }

        if (hitPoints.empty()) continue;

        for (set<const QAMCPoint*>::const_iterator l = hitPoints.begin(); l != hitPoints.end(); ++l) {
          const QAMCPoint* point = *l;
          x += point->x;
          y += point->y;
          t += point->t;
        }

        int nofPoints = hitPoints.size();  //hit->points.size();//hitPoints.size();
        double deltaX = hit->x - x / nofPoints;
        double deltaY = hit->y - y / nofPoints;
        double deltaT = hit->t - t / nofPoints;
        deltaXHisto->Fill(deltaX);
        deltaYHisto->Fill(deltaY);
        deltaTHisto->Fill(deltaT);
        pullXHisto->Fill(deltaX / hit->dx);
        pullYHisto->Fill(deltaY / hit->dy);
        pullTHisto->Fill(deltaT / hit->dt);
      }
    }
  }

  int nofHitEvents  = 0;
  int nofHits       = 0;
  int nofSingleHits = 0;

  for (vector<vector<QAHit>>::const_iterator i = hits.begin(); i != hits.end(); ++i) {
    const vector<QAHit>& evHits = *i;

    if (!evHits.empty()) ++nofHitEvents;

    nofHits += evHits.size();

    for (vector<QAHit>::const_iterator j = evHits.begin(); j != evHits.end(); ++j) {
      const QAHit& hit = *j;
      nofTracksDepositedHisto->Fill(hit.tracks.size());

      if (hit.tracks.size() == 1) ++nofSingleHits;
    }
  }

  SaveHisto(deltaTHisto);
  SaveHisto(deltaXHisto);
  SaveHisto(deltaYHisto);
  SaveHisto(pullTHisto);
  SaveHisto(pullXHisto);
  SaveHisto(pullYHisto);
  SaveHisto(nofHitsHisto);
  SaveHisto(nofTracksDepositedHisto);

  double eff = 100 * nofTracksHit;
  eff /= nofTracksTof;
  cout << "Clustering efficiency: " << eff << " % [ " << nofTracksHit << " / " << nofTracksTof << " ]" << endl;
  cout << "NOF hit events: " << nofHitEvents << endl;
  cout << "NOF hits: " << nofHits << endl;
  cout << "NOF hits2: " << globalNofHits << endl;
  cout << "NOF digis: " << globalNofDigis << endl;
  cout << "NOF MC Events: " << nofMCEvents << endl;
  eff = 100 * nofSingleHits;
  eff /= nofHits;
  cout << "Pure hits: " << eff << " % [ " << nofSingleHits << " / " << nofHits << " ]" << endl;
}

ClassImp(CbmTofHitFinderTBQA)
