/* Copyright (C) 2017-2020 IKF-UFra, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Valentina Akishina , Maksym Zyzak, Valentina Akishina [committer] */

/** @file CbmEventBuilderQA.cxx
 ** @author Valentina Akishina <v.akishina@gsi.de>, Maksym Zyzak <m.zyzak@gsi.de>
 ** @date 14.03.2017
 **/

// Cbm Headers ----------------------
#include "CbmEventBuilderQA.h"

#include "CbmMCTrack.h"
#include "CbmTrack.h"
#include "CbmTrackMatchNew.h"

#include "FairRunAna.h"

//KF Particle headers
#include "CbmEvent.h"
#include "CbmKFVertex.h"
#include "CbmL1PFFitter.h"
#include "CbmStsDigi.h"
#include "CbmStsHit.h"
#include "CbmStsPoint.h"
#include "CbmStsSensor.h"
#include "CbmStsSetup.h"
#include "CbmStsStation.h"
#include "CbmStsTrack.h"

#include "KFMCTrack.h"
#include "KFParticleMatch.h"
#include "KFParticleTopoReconstructor.h"
#include "KFTopoPerformance.h"
#include "L1Field.h"

//ROOT headers
#include "TClonesArray.h"
#include "TDatabasePDG.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"
#include "TObject.h"

//c++ and std headers
#include "CbmEbEventEfficiencies.h"
#include "CbmEbEventMatch.h"
#include "CbmEbMCEvent.h"
#include "CbmGlobalTrack.h"
#include "CbmMCDataArray.h"
#include "CbmMCDataManager.h"
#include "CbmMCEventList.h"
#include "CbmMCTrack.h"
#include "CbmMuchTrack.h"
#include "CbmRichRing.h"
#include "CbmStsTrack.h"
#include "CbmTrack.h"
#include "CbmTrackMatchNew.h"

#include <iomanip>
#include <iostream>

#include <cmath>

using std::map;
using std::vector;

struct TH1FParameters {
  TString name, title;
  int nbins;
  float xMin, xMax;
};

CbmEventBuilderQA::CbmEventBuilderQA(const char* name, Int_t iVerbose, TString outFileName)
  : FairTask(name, iVerbose)
  , fPointsInTracks()
  , fStsTrackBranchName("StsTrack")
  , fGlobalTrackBranchName("GlobalTrack")
  , fRichBranchName("RichRing")
  , fTrdBranchName("TrdTrack")
  , fTofBranchName("TofHit")
  , fMuchTrackBranchName("MuchTrack")
  , fMCTracksBranchName("MCTrack")
  , fStsTrackMatchBranchName("StsTrackMatch")
  , fRichRingMatchBranchName("RichRingMatch")
  , fTrdTrackMatchBranchName("TrdTrackMatch")
  , fTofHitMatchBranchName("TofHitMatch")
  , fMuchTrackMatchBranchName("MuchTrackMatch")
  , fStsDigis(0)
  , fStsTracks(0)
  , fMCTracks(0)
  , fStsHits(0)
  , fMvdPoints(0)
  , fStsPoints(0)
  , fEvents(0)
  , fStsTrackMatchArray(0)
  , fStsHitMatch(0)
  , fEventList(nullptr)
  , fOutFileName(outFileName)
  , fOutFile(0)
  , fHistoDir(0)  //, //fNEvents(0),
                  // fPDGtoIndexMap()
{
  TFile* curFile           = gFile;
  TDirectory* curDirectory = gDirectory;

  if (!(fOutFileName == "")) fOutFile = new TFile(fOutFileName.Data(), "RECREATE");
  else
    fOutFile = gFile;

  fOutFile->cd();
  fHistoDir = fOutFile->mkdir("4DQA");
  fHistoDir->cd();

  gDirectory->mkdir("TimeHisto");
  gDirectory->cd("TimeHisto");
  {
    TH1FParameters timeHisto[fNTimeHistos] = {
      {"NTracksVsTime", "NTracksVsTime", 12100, -100.f, 12000.f},
      {"TracksTimeResidual", "TracksTimeResidual", 300, -30.f, 30.f},
      {"TracksTimePull", "TracksTimePull", 100, -10.f, 10.f},
      {"NHitsVsTime", "NHitsVsTime", 12100, -100.f, 12000.f},
      {"HitsTimeResidual", "HitsTimeResidual", 300, -30.f, 30.f},
      {"HitsTimePull", "HitsTimePull", 100, -10.f, 10.f},
      {"NTracksInEventsVsTime", "NTracksInEventsVsTime", 12100, -100.f, 12000.f},
      {"NHitsInEventsVsTime", "NHitsInEventsVsTime", 12100, -100.f, 12000.f},
      {"NTracksVsMCTime", "NTracksVsMCTime", 12100, -100.f, 12000.f},
      {"NHitsVsMCTime", "NHitsVsMCTime", 12100, -100.f, 12000.f},
      {"dtDistribution", "dtDistribution", 100, 0.f, 50.f},
      {"NTracksInEventsVsTime1", "NTracksInEventsVsTime1", 120100, -100.f, 12000.f},
      {"NTracksInEventsVsTime2", "NTracksInEventsVsTime2", 120100, -100.f, 12000.f},
      {"NTracksInEventsVsTime3", "NTracksInEventsVsTime3", 120100, -100.f, 12000.f},
      {"NTracksInEventsVsTime4", "NTracksInEventsVsTime4", 120100, -100.f, 12000.f},
      {"NTracksInEventsVsTime5", "NTracksInEventsVsTime5", 120100, -100.f, 12000.f},

      {"NHitsInEventsVsTime1", "NHitsInEventsVsTime1", 12100, -100.f, 12000.f},
      {"NHitsInEventsVsTime2", "NHitsInEventsVsTime2", 12100, -100.f, 12000.f},
      {"NHitsInEventsVsTime3", "NHitsInEventsVsTime3", 12100, -100.f, 12000.f},
      {"NHitsInEventsVsTime4", "NHitsInEventsVsTime4", 12100, -100.f, 12000.f},
      {"NHitsInEventsVsTime5", "NHitsInEventsVsTime5", 12100, -100.f, 12000.f},

      {"Number of merged events", "Number of merged events", 6, -0.5f, 5.5f},
      {"Event length", "Event length", 100, 0.f, 50.f},
      {"NTracksPerEvent", "NTracksPerEvent", 250, 0.f, 250.f},
      {"TrackRecoTimePull", "TrackRecoTimePull", 100, -10.f, 10.f},
      {"TrackTimeEvent", "TrackTimeEvent", 100, -10.f, 10.f},
      {"TrackTimeNoEvent", "TrackTimeNoEvent", 100, -10.f, 10.f}};
    for (int iH = 0; iH < fNTimeHistos; iH++)
      fTimeHisto[iH] = new TH1F(timeHisto[iH].name.Data(), timeHisto[iH].title.Data(), timeHisto[iH].nbins,
                                timeHisto[iH].xMin, timeHisto[iH].xMax);

    fTimeHisto[0]->GetXaxis()->SetTitle("Time, ns");
    fTimeHisto[0]->GetYaxis()->SetTitle("Number of tracks");
    fTimeHisto[16]->GetYaxis()->SetTitle("Number of Events");
    fTimeHisto[16]->GetXaxis()->SetTitle("Number of MCEvents in Event");

    fTimeHisto[11]->SetLineColor(5);
    fTimeHisto[11]->SetFillColor(5);
    fTimeHisto[12]->SetLineColor(2);
    fTimeHisto[12]->SetFillColor(2);
    fTimeHisto[13]->SetLineColor(3);
    fTimeHisto[13]->SetFillColor(3);
    fTimeHisto[14]->SetLineColor(7);
    fTimeHisto[14]->SetFillColor(7);
    fTimeHisto[15]->SetLineColor(6);
    fTimeHisto[15]->SetFillColor(6);

    fTimeHisto[16]->SetLineColor(5);
    fTimeHisto[16]->SetFillColor(5);
    fTimeHisto[17]->SetLineColor(2);
    fTimeHisto[17]->SetFillColor(2);
    fTimeHisto[18]->SetLineColor(3);
    fTimeHisto[18]->SetFillColor(3);
    fTimeHisto[19]->SetLineColor(7);
    fTimeHisto[19]->SetFillColor(7);
    fTimeHisto[20]->SetLineColor(6);
    fTimeHisto[20]->SetFillColor(6);
  }
  gDirectory->cd("..");  //4DQA


  gFile      = curFile;
  gDirectory = curDirectory;
}

CbmEventBuilderQA::~CbmEventBuilderQA() {}


struct CbmBuildEventMCTrack {
  CbmBuildEventMCTrack() : fMCFileId(-1), fMCEventId(-1), fMCTrackId(-1), fRecoTrackId(), fRecoEventId() {}

  int fMCFileId;
  int fMCEventId;
  int fMCTrackId;

  vector<int> fRecoTrackId;
  vector<int> fRecoEventId;
};

InitStatus CbmEventBuilderQA::Init()
{
  //Get ROOT Manager
  FairRootManager* ioman = FairRootManager::Instance();

  if (ioman == 0) {
    Warning("CbmEventBuilderQA::Init", "RootManager not instantiated!");
    return kERROR;
  }

  CbmMCDataManager* mcManager = (CbmMCDataManager*) ioman->GetObject("MCDataManager");
  if (mcManager == nullptr) LOG(fatal) << GetName() << ": No CbmMCDataManager!";

  fMCTracks = (CbmMCDataArray*) mcManager->InitBranch("MCTrack");
  if (fMCTracks == nullptr) LOG(fatal) << GetName() << ": No MCTrack data!";

  fEventList = (CbmMCEventList*) ioman->GetObject("MCEventList.");
  if (fEventList == 0) {
    Error("CbmEventBuilderQA::Init", "MC Event List not found!");
    return kERROR;
  }

  // open MCTrack array
  fStsTracks = (TClonesArray*) ioman->GetObject("StsTrack");
  assert(fStsTracks);

  fStsTrackMatchArray = (TClonesArray*) ioman->GetObject("StsTrackMatch");
  if (fStsTrackMatchArray == 0) {
    Error("CbmEventBuilderQA::Init", "track match array not found!");
    return kERROR;
  }

  fStsHits = (TClonesArray*) ioman->GetObject("StsHit");
  assert(fStsHits);

  fStsPoints = mcManager->InitBranch("StsPoint");
  assert(fStsPoints);

  fMvdPoints = mcManager->InitBranch("StsPoint");
  assert(fStsPoints);

  // --- Get input array (CbmStsDigi)
  fStsHitMatch = (TClonesArray*) ioman->GetObject("StsHitMatch");
  assert(fStsHitMatch);

  // --- Get input array (CbmEvent)
  fEvents = dynamic_cast<TClonesArray*>(ioman->GetObject("CbmEvent"));
  if (nullptr == fEvents) { Fatal("CbmEventBuilderQA::Init", "No CbmEvent TClonesArray found!"); }

  return kSUCCESS;
}

void CbmEventBuilderQA::Exec(Option_t* /*opt*/)
{
  fPointsInTracks.clear();

  int nMCEvents = fEventList->GetNofEvents();


  vector<vector<int>> mcHitMap(nMCEvents);
  vector<CbmEbMCEvent> fMCEvents(nMCEvents);

  //Fill Histo for Hits

  UInt_t nHits = fStsHits->GetEntriesFast();


  for (UInt_t iHit = 0; iHit < nHits; iHit++) {
    CbmStsHit* hit = (CbmStsHit*) fStsHits->At(iHit);

    float hit_time = hit->GetTime();

    fTimeHisto[3]->Fill(hit_time);


    CbmMatch* stsHitMatch = (CbmMatch*) fStsHitMatch->At(iHit);
    if (stsHitMatch->GetNofLinks() == 0) continue;
    Float_t bestWeight  = 0.f;
    Float_t totalWeight = 0.f;
    //    Int_t mcHitId = -1;
    int mcEvent  = -1;
    int iMCPoint = -1;
    CbmLink link;

    for (int iLink = 0; iLink < stsHitMatch->GetNofLinks(); iLink++) {
      totalWeight += stsHitMatch->GetLink(iLink).GetWeight();
      if (stsHitMatch->GetLink(iLink).GetWeight() > bestWeight) {
        bestWeight = stsHitMatch->GetLink(iLink).GetWeight();
        iMCPoint   = stsHitMatch->GetLink(iLink).GetIndex();
        link       = stsHitMatch->GetLink(iLink);
        mcEvent    = link.GetEntry();
      }
    }
    if (bestWeight / totalWeight < 0.7 || iMCPoint < 0) continue;


    int iCol = (mcEvent) % 5;

    if (iCol >= 0) fTimeHisto[16 + iCol]->Fill(hit_time);

    CbmStsPoint* stsMcPoint = (CbmStsPoint*) fStsPoints->Get(link.GetFile(), link.GetEntry(), link.GetIndex());
    double mcTime           = stsMcPoint->GetTime() + fEventList->GetEventTime(link.GetEntry() + 1, link.GetFile());

    fTimeHisto[7]->Fill(mcTime);

    double residual = hit_time - mcTime;
    double pull     = residual / 5.;

    fTimeHisto[4]->Fill(residual);
    fTimeHisto[5]->Fill(pull);
  }

  ///Fill Histo for reco Tracks
  UInt_t nTracks = fStsTracks->GetEntriesFast();

  vector<bool> IsTrackReconstructed(nTracks, 0);
  vector<bool> IsTrackReconstructable(nTracks, 0);

  const int iMCFile = 0;
  if (fStsPoints) {
    fPointsInTracks.resize(nMCEvents);
    for (int iMCEvent = 0; iMCEvent < nMCEvents; iMCEvent++) {
      const int nMCTracksInEvent = fMCTracks->Size(iMCFile, iMCEvent);
      fPointsInTracks[iMCEvent].resize(nMCTracksInEvent);

      unsigned int nStsPoints = fStsPoints->Size(iMCFile, iMCEvent);

      for (unsigned int iStsPoint = 0; iStsPoint < nStsPoints; iStsPoint++) {
        CbmStsPoint* point = (CbmStsPoint*) fStsPoints->Get(iMCFile, iMCEvent, iStsPoint);
        const int iMCTrack = point->GetTrackID();
        fPointsInTracks[iMCEvent][iMCTrack].push_back(iStsPoint);
      }
    }
  }

  for (unsigned int iTrack = 0; iTrack < nTracks; iTrack++) {

    CbmStsTrack* track = (CbmStsTrack*) fStsTracks->At(iTrack);

    fTimeHisto[0]->Fill(track->GetTime());


    UInt_t NHits = track->GetNofStsHits();

    for (UInt_t iHit = 0; iHit < NHits; iHit++) {

      CbmMatch* stsHitMatch = (CbmMatch*) fStsHitMatch->At(iHit);
      if (stsHitMatch->GetNofLinks() == 0) continue;
      Float_t bestWeight  = 0.f;
      Float_t totalWeight = 0.f;
      //       Int_t mcHitId = -1;
      //       int mcEvent = -1;
      int iMCPoint = -1;
      CbmLink link;
      for (int iLink = 0; iLink < stsHitMatch->GetNofLinks(); iLink++) {
        totalWeight += stsHitMatch->GetLink(iLink).GetWeight();
        if (stsHitMatch->GetLink(iLink).GetWeight() > bestWeight) {
          bestWeight = stsHitMatch->GetLink(iLink).GetWeight();
          iMCPoint   = stsHitMatch->GetLink(iLink).GetIndex();
          link       = stsHitMatch->GetLink(iLink);
        }
      }
      if (bestWeight / totalWeight < 0.7 || iMCPoint < 0) continue;


      //     CbmStsPoint* stsMcPoint = (CbmStsPoint*) fStsPoints->Get(link.GetFile(),link.GetEntry(),link.GetIndex());
      //     double mcTime = stsMcPoint->GetTime() + fEventList->GetEventTime(link.GetEntry()+1, link.GetFile());

      //fTimeHisto[8]->Fill(mcTime);
    }

    CbmTrackMatchNew* stsTrackMatch = (CbmTrackMatchNew*) fStsTrackMatchArray->At(iTrack);
    if (stsTrackMatch->GetNofLinks() == 0) continue;
    Float_t bestWeight  = 0.f;
    Float_t totalWeight = 0.f;
    Int_t mcTrackId     = -1;
    int mcEvent         = -1;
    CbmLink link;
    for (int iLink = 0; iLink < stsTrackMatch->GetNofLinks(); iLink++) {
      totalWeight += stsTrackMatch->GetLink(iLink).GetWeight();
      if (stsTrackMatch->GetLink(iLink).GetWeight() > bestWeight) {
        bestWeight   = stsTrackMatch->GetLink(iLink).GetWeight();
        int iMCTrack = stsTrackMatch->GetLink(iLink).GetIndex();
        link         = stsTrackMatch->GetLink(iLink);


        mcEvent   = link.GetEntry();
        mcTrackId = iMCTrack;
      }
    }
    if (bestWeight / totalWeight < 0.7 || mcTrackId < 0) continue;

    IsTrackReconstructed[iTrack] = 1;

    CbmMCTrack* mcTrack = (CbmMCTrack*) fMCTracks->Get(0, mcEvent, mcTrackId);

    double mcTime   = mcTrack->GetStartT() + fEventList->GetEventTime(mcEvent + 1, link.GetFile());
    double residual = track->GetTime() - mcTime;
    double pull     = residual / track->GetTimeError();
    fTimeHisto[1]->Fill(residual);
    fTimeHisto[2]->Fill(pull);
  }


  vector<vector<int>> mcTrackMap(nMCEvents);
  vector<CbmBuildEventMCTrack> mcTracks;

  float tEvent = 0.f;
  //  int eventId = 0;


  for (int iMCEvent = 0; iMCEvent < nMCEvents; iMCEvent++) {
    unsigned int nMCTracks = fMCTracks->Size(0, iMCEvent);

    mcTrackMap[iMCEvent].resize(nMCTracks);

    fTimeHisto[10]->Fill(fEventList->GetEventTime(iMCEvent + 1, 0)
                         - tEvent);  // +fEventList->GetEventTime(iMCEvent+1, 0) );
    tEvent = fEventList->GetEventTime(iMCEvent + 1, 0);

    int nReconstructableMCTracks = 0;
    for (unsigned int iMC = 0; iMC < nMCTracks; iMC++) {
      CbmMCTrack* mcTrack = (CbmMCTrack*) fMCTracks->Get(0, iMCEvent, iMC);

      if (CalculateIsReconstructable(0, iMCEvent, iMC)) nReconstructableMCTracks++;

      fTimeHisto[8]->Fill(mcTrack->GetStartT() + fEventList->GetEventTime(iMCEvent + 1, 0));

      CbmBuildEventMCTrack newMCTrack;
      newMCTrack.fMCFileId  = 0;
      newMCTrack.fMCEventId = iMCEvent;
      newMCTrack.fMCTrackId = iMC;

      mcTrackMap[iMCEvent][iMC] = mcTracks.size();
      vector<int>& trackIds     = fMCEvents[iMCEvent].GetMCTrackIds();
      trackIds.push_back(mcTracks.size());

      mcTracks.push_back(newMCTrack);

      //     if(mcTrack->GetMotherId()<eventId)
      //     {
      //       fTimeHisto[10]->Fill( mcTrack->GetStartT() - tEvent);// +fEventList->GetEventTime(iMCEvent+1, 0) );
      //       tEvent = mcTrack->GetStartT();
      //       eventId = mcTrack->GetMotherId();
      //     }
    }
    if (nReconstructableMCTracks > 2) fMCEvents[iMCEvent].SetReconstructable(1);
  }

  for (int iTrack = 0; iTrack < fStsTrackMatchArray->GetEntriesFast(); iTrack++) {
    CbmTrackMatchNew* stsTrackMatch = (CbmTrackMatchNew*) fStsTrackMatchArray->At(iTrack);
    if (stsTrackMatch->GetNofLinks() == 0) continue;
    Float_t bestWeight  = 0.f;
    Float_t totalWeight = 0.f;
    Int_t mcTrackId     = -1;
    int mcEvent         = -1;
    CbmLink link;
    for (int iLink = 0; iLink < stsTrackMatch->GetNofLinks(); iLink++) {
      totalWeight += stsTrackMatch->GetLink(iLink).GetWeight();
      if (stsTrackMatch->GetLink(iLink).GetWeight() > bestWeight) {
        bestWeight   = stsTrackMatch->GetLink(iLink).GetWeight();
        int iMCTrack = stsTrackMatch->GetLink(iLink).GetIndex();
        link         = stsTrackMatch->GetLink(iLink);
        mcEvent      = link.GetEntry();
        mcTrackId    = iMCTrack;
      }
    }
    if (bestWeight / totalWeight < 0.7 || mcTrackId < 0) continue;

    fMCEvents[mcEvent].GetRecoTrackIds().push_back(iTrack);
  }

  vector<CbmEbEventMatch> eventMatches;

  for (int iEvent = 0; iEvent < fEvents->GetEntriesFast(); iEvent++) {
    CbmEvent* event = (CbmEvent*) fEvents->At(iEvent);


    vector<int> tracksId;
    Int_t nEventTracks = event->GetNofData(ECbmDataType::kStsTrack);

    fTimeHisto[23]->Fill(nEventTracks);

    CbmEbEventMatch EventMatch;
    EventMatch.SetNEventTracks(nEventTracks);
    vector<int> tracks;

    float tLastTrack  = 0;
    float tFirstTrack = 100000000000000;

    for (int iTr = 0; iTr < nEventTracks; iTr++) {
      const int stsTrackIndex = event->GetStsTrackIndex(iTr);

      tracks.push_back(stsTrackIndex);

      CbmTrackMatchNew* stsTrackMatch = (CbmTrackMatchNew*) fStsTrackMatchArray->At(stsTrackIndex);
      if (stsTrackMatch->GetNofLinks() == 0) continue;
      Float_t bestWeight  = 0.f;
      Float_t totalWeight = 0.f;
      Int_t mcTrackId     = -1;
      int mcEvent         = -1;
      CbmLink link;
      for (int iLink = 0; iLink < stsTrackMatch->GetNofLinks(); iLink++) {
        totalWeight += stsTrackMatch->GetLink(iLink).GetWeight();
        if (stsTrackMatch->GetLink(iLink).GetWeight() > bestWeight) {
          bestWeight   = stsTrackMatch->GetLink(iLink).GetWeight();
          int iMCTrack = stsTrackMatch->GetLink(iLink).GetIndex();
          link         = stsTrackMatch->GetLink(iLink);


          mcEvent   = link.GetEntry();
          mcTrackId = iMCTrack;
        }
      }
      if (bestWeight / totalWeight < 0.7 || mcTrackId < 0) continue;

      CbmMCTrack* mcTrack = (CbmMCTrack*) fMCTracks->Get(0, mcEvent, mcTrackId);

      CbmStsTrack* track = (CbmStsTrack*) fStsTracks->At(stsTrackIndex);

      if ((track->GetTime()) > tLastTrack) tLastTrack = (track->GetTime());
      if ((track->GetTime()) < tFirstTrack) tFirstTrack = (track->GetTime());

      double mcTime = mcTrack->GetStartT() + fEventList->GetEventTime(mcEvent + 1, link.GetFile());

      fTimeHisto[24]->Fill(fabs(track->GetTime() - mcTime) / track->GetTimeError());

      if (fMCEvents[mcEvent].IsReconstructable()) EventMatch.AddTrack(mcEvent);

      for (int iTr1 = iTr + 1; iTr1 < nEventTracks; iTr1++) {
        if (iTr1 == iTr) continue;
        const int stsTrackIndex1 = event->GetStsTrackIndex(iTr1);
        CbmStsTrack* track1      = (CbmStsTrack*) fStsTracks->At(stsTrackIndex1);

        fTimeHisto[25]->Fill(fabs(track->GetTime() - track1->GetTime())
                             / sqrt(track->GetTimeError() + track1->GetTimeError()));
      }

      for (int iEvent1 = 0; iEvent1 < fEvents->GetEntriesFast(); iEvent1++) {

        if (iEvent == iEvent1) continue;
        CbmEvent* event1 = (CbmEvent*) fEvents->At(iEvent1);

        Int_t nTracks1 = event1->GetNofData(ECbmDataType::kStsTrack);
        for (int iTr1 = 0; iTr1 < nTracks1; iTr1++) {
          const int stsTrackIndex1 = event1->GetStsTrackIndex(iTr1);
          CbmStsTrack* track1      = (CbmStsTrack*) fStsTracks->At(stsTrackIndex1);

          if (fabs(track->GetTime() - track1->GetTime()) > 8.5) continue;

          fTimeHisto[26]->Fill(fabs(track->GetTime() - track1->GetTime())
                               / sqrt(track->GetTimeError() + track1->GetTimeError()));
        }
      }
    }

    EventMatch.SetTracks(tracks);
    fTimeHisto[22]->Fill(tLastTrack - tFirstTrack);
    eventMatches.push_back(EventMatch);

    for (std::map<int, int>::iterator it = EventMatch.GetMCEvents().begin(); it != EventMatch.GetMCEvents().end();
         ++it) {

      fMCEvents[it->first].AddRecoEvent(iEvent);
    }
  }


  vector<SortEvents> Ev;

  for (int iMCEvent = 0; iMCEvent < nMCEvents; iMCEvent++) {
    int nMcPoints = fStsPoints->Size(0, iMCEvent);
    for (int i = 0; i < nMcPoints; i++) {
      CbmStsPoint* stsMcPoint = (CbmStsPoint*) fStsPoints->Get(0, iMCEvent, i);
      double mcTime           = stsMcPoint->GetTime() + fEventList->GetEventTime(iMCEvent + 1, 0);
      fTimeHisto[9]->Fill(mcTime);
    }
  }


  int nEvents = fEvents->GetEntriesFast();

  for (int k = 0; k < nEvents; k++) {
    SortEvents Event1;

    CbmEvent* event = (CbmEvent*) fEvents->At(k);

    Event1.Event            = event;
    const int stsTrackIndex = event->GetStsTrackIndex(0);
    CbmStsTrack* track      = (CbmStsTrack*) fStsTracks->At(stsTrackIndex);
    Event1.track            = *track;
    Ev.push_back(Event1);
  }


  std::sort(Ev.begin(), Ev.end(), CompareTrackTime);

  for (int k = 0; k < nEvents; k++) {
    CbmEvent* event = Ev[k].Event;  //(CbmEvent*) fEvents->At(k);

    int iCol = k % 5;
    for (int i = 0; i < event->GetNofData(ECbmDataType::kStsTrack); i++) {
      const int stsTrackIndex = event->GetStsTrackIndex(i);
      CbmStsTrack* track      = (CbmStsTrack*) fStsTracks->At(stsTrackIndex);

      fTimeHisto[6]->Fill(track->GetTime());


      fTimeHisto[11 + iCol]->Fill(track->GetTime());

      UInt_t NHits = track->GetNofStsHits();

      for (UInt_t iHit = 0; iHit < NHits; iHit++) {
        CbmStsHit* hit = (CbmStsHit*) fStsHits->At(track->GetStsHitIndex(iHit));
        fTimeHisto[7]->Fill(hit->GetTime());
      }
    }
  }


  CbmEbEventEfficiencies fEventEfficiency;

  CbmEbEventEfficiencies eventEfficiency;  // efficiencies for current event
  for (unsigned int iEvent = 0; iEvent < eventMatches.size(); iEvent++)
    eventEfficiency.AddGhost(eventMatches[iEvent].IsGhost());

  for (unsigned int iMCEvent = 0; iMCEvent < fMCEvents.size(); iMCEvent++) {
    if (fMCEvents[iMCEvent].IsReconstructable() && fMCEvents[iMCEvent].NMCTracks() > 1) {
      const vector<int>& recoEvents = fMCEvents[iMCEvent].GetRecoEvents();  // for length calculations
      bool reco                     = 0;
      if (recoEvents.size() == 1) reco = 1;
      // number of cloned events
      int nclones = 0;
      if (fMCEvents[iMCEvent].NClones() > 0) nclones = 1;
      eventEfficiency.Inc(reco, nclones, "reconstructable");
    }
    //  if( fMCEvents[iMCEvent].IsReconstructed() )
    if (fMCEvents[iMCEvent].GetRecoTrackIds().size() > 2) {
      const vector<int>& recoEvents = fMCEvents[iMCEvent].GetRecoEvents();  // for length calculations
      bool reco                     = 0;
      if (recoEvents.size() == 1) reco = 1;
      // number of cloned events
      int nclones = 0;
      if (fMCEvents[iMCEvent].NClones() > 0) nclones = 1;
      eventEfficiency.Inc(reco, nclones, "reconstructed");
    }
  }  // for mcTracks
  eventEfficiency.IncNEvents();
  fEventEfficiency += eventEfficiency;
  eventEfficiency.CalcEff();
  fEventEfficiency.CalcEff();

  std::cout << "Event reconstruction efficiency" << std::endl;
  fEventEfficiency.PrintEff();
  cout << endl;


  //   int iDMcTrack;
  for (unsigned int iM = 0; iM < eventMatches.size(); iM++) {
    //     cout<<eventMatches[iM].NMCEvents()<<" Nevents"<<endl;

    fTimeHisto[21]->Fill(eventMatches[iM].NMCEvents());
  }
}

bool CbmEventBuilderQA::CalculateIsReconstructable(const int iMCFile, const int iMCEvent, const int iMCTrack)
{

  CbmMCTrack* mcTrack = (CbmMCTrack*) fMCTracks->Get(iMCFile, iMCEvent, iMCTrack);

  bool f = 1;

  // reject very slow tracks from analysis
  f &= (mcTrack->GetP() > 0.1);
  // detected at least in 4 stations
  //   f &= (nMCContStations >= 4);

  int maxNStaMC    = 0;  // max number of mcPoints on station
  int maxNSensorMC = 0;  // max number of mcPoints with same z
  int nMCStations  = 0;

  maxNStaMC           = 0;
  maxNSensorMC        = 0;
  nMCStations         = 0;
  int lastSta         = -1;
  float lastz         = -1;
  int nMCContStations = 0;
  int istaold = -1, ncont = 0;
  int cur_maxNStaMC = 0, cur_maxNSensorMC = 0;
  for (unsigned int iH = 0; iH < fPointsInTracks[iMCEvent][iMCTrack].size(); iH++) {

    int iStsPoint      = fPointsInTracks[iMCEvent][iMCTrack][iH];
    CbmStsPoint* point = (CbmStsPoint*) fStsPoints->Get(iMCFile, iMCEvent, iStsPoint);

    int currentStation = -1;
    for (int iStation = 0; iStation < CbmStsSetup::Instance()->GetNofStations(); iStation++) {
      CbmStsStation* station = CbmStsSetup::Instance()->GetStation(iStation);
      const float zStation   = station->GetZ();

      if (fabs(point->GetZ() - zStation) < 2.5) {
        currentStation = iStation;
        break;
      }
    }
    if (currentStation < 0) continue;

    if (currentStation == lastSta) cur_maxNStaMC++;
    else {  // new station
      if (cur_maxNStaMC > maxNStaMC) maxNStaMC = cur_maxNStaMC;
      cur_maxNStaMC = 1;
      lastSta       = currentStation;
      nMCStations++;
    }

    if (point->GetZ() == lastz)  // TODO: works incorrect because of different z
      cur_maxNSensorMC++;
    else {  // new z
      if (cur_maxNSensorMC > maxNSensorMC) maxNSensorMC = cur_maxNSensorMC;
      cur_maxNSensorMC = 1;
      lastz            = point->GetZ();
    }

    int ista = currentStation;
    if (ista - istaold == 1) ncont++;
    else if (ista - istaold > 1) {
      if (nMCContStations < ncont) nMCContStations = ncont;
      ncont = 1;
    }
    if (ista <= istaold) continue;  // backward direction
    istaold = ista;
  };

  if (nMCContStations < ncont) nMCContStations = ncont;

  if (cur_maxNStaMC > maxNStaMC) maxNStaMC = cur_maxNStaMC;
  if (cur_maxNSensorMC > maxNSensorMC) maxNSensorMC = cur_maxNSensorMC;
  //   cout << pdg << " " << p << " " << Points.size() << " > " << maxNStaMC << " " << maxNSensorMC << endl;


  // maximul 4 layers for a station.
  //   f &= (maxNStaHits <= 4);
  f &= (maxNStaMC <= 4);

  bool isReconstructableTrack = f & (nMCContStations >= 4);  // L1-MC

  return (isReconstructableTrack);
};

void CbmEventBuilderQA::Finish()
{
  TDirectory* curr   = gDirectory;
  TFile* currentFile = gFile;
  // Open output file and write histograms

  fOutFile->cd();
  WriteHistosCurFile(fHistoDir);
  if (!(fOutFileName == "")) {
    fOutFile->Close();
    fOutFile->Delete();
  }
  gFile      = currentFile;
  gDirectory = curr;
}

void CbmEventBuilderQA::WriteHistosCurFile(TObject* obj)
{

  if (!obj->IsFolder()) obj->Write();
  else {
    TDirectory* cur    = gDirectory;
    TFile* currentFile = gFile;

    TDirectory* sub = cur->GetDirectory(obj->GetName());
    sub->cd();
    TList* listSub = (static_cast<TDirectory*>(obj))->GetList();
    TIter it(listSub);
    while (TObject* obj1 = it())
      WriteHistosCurFile(obj1);
    cur->cd();
    gFile      = currentFile;
    gDirectory = cur;
  }
}


ClassImp(CbmEventBuilderQA);
