/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "CbmBinnedTrackerQANew.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "CbmStsSetup.h"
#include "CbmMCDataManager.h"
#include "global/CbmGlobalTrack.h"
#include "CbmMatch.h"
#include "CbmTrdCluster.h"
#include "CbmMCTrack.h"
#include "CbmStsPoint.h"
#include "CbmMuchPoint.h"
#include "CbmMuchGeoScheme.h"
#include "CbmTofPoint.h"
#include "CbmStsHit.h"
#include "CbmStsDigi.h"
#include "CbmMuchPixelHit.h"
#include "CbmMuchCluster.h"
#include "CbmMuchDigi.h"
#include "CbmStsCluster.h"

using namespace std;

struct MCTrackDesc
{
    map<pair<ECbmModuleId, Int_t>, pair<UInt_t, set<Int_t> > > mcPointMap;// = map<pair<ECbmModuleId, Int_t>, pair<UInt_t, set<Int_t> > > ();// Map an MC point index to TRD station number and a set of reconstructed global track indices.
    Int_t pdg = -1;
    Int_t parentInd = -1;
};

static vector<vector<MCTrackDesc> > gMCTracks;

CbmBinnedTrackerQANew::CbmBinnedTrackerQANew()
{
}

InitStatus CbmBinnedTrackerQANew::Init()
{
   CbmStsSetup* stsSetup = CbmStsSetup::Instance();
   
   if (!stsSetup->IsInit())
      stsSetup->Init();
   
   fSettings = CbmBinnedSettings::Instance();
   
   fMinTrackLength += fSettings->Use(ECbmModuleId::kSts) ? fSettings->GetNofStsStations() : 0;
   fMinTrackLength += fSettings->Use(ECbmModuleId::kMuch) ? fSettings->GetNofMuchStations() : 0;
   fMinTrackLength += fSettings->Use(ECbmModuleId::kTrd) ? fSettings->GetNofTrdStations() : 0;
   fMinTrackLength += fSettings->Use(ECbmModuleId::kTof) ? 1 : 0;
   int canSkipHits = 0.3 * fMinTrackLength;
   fMinTrackLength -= canSkipHits;
   
   FairRootManager* ioman = FairRootManager::Instance();
    
   if (0 == ioman)
      LOG(fatal) <<  "No FairRootManager";
   
   CbmMCDataManager* mcManager = static_cast<CbmMCDataManager*> (ioman->GetObject("MCDataManager"));
   
   if (0 == mcManager)
      LOG(fatal) <<  "No MC data manager";
   
   fMCTracks = mcManager->InitBranch("MCTrack");
   
   if (0 == fMCTracks)
      LOG(fatal) <<  "No MC tracks in the input file";
   
   for (int i = 0; fMCTracks->Size(0, i) >= 0; ++i)
   {
      gMCTracks.push_back(vector<MCTrackDesc>());
      
      auto nofMcTracks = fMCTracks->Size(0, i);
      auto& eventTracks = gMCTracks.back();
      eventTracks.resize(nofMcTracks);
      
      for (Int_t j = 0; j < nofMcTracks; ++j)
      {
         MCTrackDesc& track = eventTracks[j];
         const CbmMCTrack* mcTrack = static_cast<const CbmMCTrack*> (fMCTracks->Get(0, i, j));
         track.pdg = mcTrack->GetPdgCode();
         track.parentInd = mcTrack->GetMotherId();
      }
   }// for (Int_t i = 0; fMCTracks->Size(0, i) >= 0; ++i)
   
   fGlobalTracks = static_cast<TClonesArray*> (ioman->GetObject("GlobalTrack"));
   
   if (0 == fGlobalTracks)
      LOG(fatal) <<  "No global tracks in the input file";
   
   if (fSettings->Use(ECbmModuleId::kSts))
   {
      fStsTracks = static_cast<TClonesArray*> (ioman->GetObject("StsTrack"));
   
      if (0 == fStsTracks)
         LOG(fatal) <<  "No sts tracks in the input file";
      
      fStsHits = static_cast<TClonesArray*> (ioman->GetObject("StsHit"));
   
      if (0 == fStsHits)
         LOG(fatal) <<  "No sts hits in the input file";
      
      fStsClusters = static_cast<TClonesArray*> (ioman->GetObject("StsCluster"));
   
      if (0 == fStsClusters)
         LOG(fatal) <<  "No sts clusters in the input file";
      
      fStsDigis = static_cast<TClonesArray*> (ioman->GetObject("StsDigi"));
   
      if (0 == fStsDigis)
         LOG(fatal) <<  "No sts digis in the input file";
      
      fStsDigiMatches = static_cast<TClonesArray*> (ioman->GetObject("StsDigiMatch"));

      if (0 == fStsDigiMatches)
         LOG(fatal) <<  "No sts digi matches in the input file";

      fStsPoints = mcManager->InitBranch("StsPoint");
   
      if (0 == fStsPoints)
         LOG(fatal) <<  "No sts MC points in the input file";
      
      for (Int_t i = 0; fStsPoints->Size(0, i) >= 0; ++i)
      {
         Int_t nofPoints = fStsPoints->Size(0, i);
         vector<MCTrackDesc>& tracks = gMCTracks[i];
      
         for (Int_t j = 0; j < nofPoints; ++j)
         {
            const CbmStsPoint* stsPoint = static_cast<const CbmStsPoint*> (fStsPoints->Get(0, i, j));
            Int_t trackId = stsPoint->GetTrackID();
            Int_t stationNumber = CbmStsSetup::Instance()->GetStationNumber(stsPoint->GetDetectorID());
            auto& trackDesk = tracks[trackId];
            trackDesk.mcPointMap[make_pair(ECbmModuleId::kSts, j)] = { stationNumber, set<Int_t>() };
         }
      }
   }// if (fSettings->Use(kSts))
   
   if (fSettings->Use(ECbmModuleId::kMuch))
   {
      fMuchTracks = static_cast<TClonesArray*> (ioman->GetObject("MuchTrack"));
   
      if (0 == fMuchTracks)
         LOG(fatal) <<  "No much tracks in the input file";
      
      fMuchHits = static_cast<TClonesArray*> (ioman->GetObject("MuchPixelHit"));
   
      if (0 == fMuchHits)
         LOG(fatal) <<  "No much hits in the input file";
      
      fMuchClusters = static_cast<TClonesArray*> (ioman->GetObject("MuchCluster"));
   
      if (0 == fMuchClusters)
         LOG(fatal) <<  "No much clusters in the input file";
      
      fMuchDigis = static_cast<TClonesArray*> (ioman->GetObject("MuchDigi"));
   
      if (0 == fMuchDigis)
         LOG(fatal) <<  "No much digis in the input file";
      
      fMuchDigiMatches = static_cast<TClonesArray*> (ioman->GetObject("MuchDigiMatch"));

      if (0 == fMuchDigiMatches)
         LOG(fatal) <<  "No much digi matches in the input file";

      fMuchPoints = mcManager->InitBranch("MuchPoint");
   
      if (0 == fMuchPoints)
         LOG(fatal) <<  "No much MC points in the input file";
      
      for (Int_t i = 0; fMuchPoints->Size(0, i) >= 0; ++i)
      {
         Int_t nofPoints = fMuchPoints->Size(0, i);
         vector<MCTrackDesc>& tracks = gMCTracks[i];
      
         for (Int_t j = 0; j < nofPoints; ++j)
         {
            const CbmMuchPoint* muchPoint = static_cast<const CbmMuchPoint*> (fMuchPoints->Get(0, i, j));
            Int_t trackId = muchPoint->GetTrackID();
            int muchStationNumber = CbmMuchGeoScheme::GetStationIndex(muchPoint->GetDetectorID());
            int layerNumber = CbmMuchGeoScheme::GetLayerIndex(muchPoint->GetDetectorID());
            int stationNumber = muchStationNumber * 3 + layerNumber;
            auto& trackDesk = tracks[trackId];
            trackDesk.mcPointMap[make_pair(ECbmModuleId::kMuch, j)] = { stationNumber, set<Int_t>() };
         }
      }
   }// if (fSettings->Use(kMuch))
   
   if (fSettings->Use(ECbmModuleId::kTrd))
   {
      fTrdTracks = static_cast<TClonesArray*> (ioman->GetObject("TrdTrack"));
   
      if (0 == fTrdTracks)
         LOG(fatal) <<  "No trd tracks in the input file";
      
      fTrdHits = static_cast<TClonesArray*> (ioman->GetObject("TrdHit"));
   
      if (0 == fTrdHits)
         LOG(fatal) <<  "No trd hits in the input file";

      fTrdClusters = static_cast<TClonesArray*> (ioman->GetObject("TrdCluster"));
   
      if (0 == fTrdClusters)
         LOG(fatal) <<  "No global tracks in the input file";
   
      fTrdDigiMatches = static_cast<TClonesArray*> (ioman->GetObject("TrdDigiMatch"));
   
      if (0 == fTrdDigiMatches)
         LOG(fatal) <<  "No trd hit to digi matches in the input file";
      
      fTrdPoints = mcManager->InitBranch("TrdPoint");
   
      if (0 == fTrdPoints)
         LOG(fatal) <<  "No trd MC points in the input file";
      
      for (Int_t i = 0; fTrdPoints->Size(0, i) >= 0; ++i)
      {
         Int_t nofPoints = fTrdPoints->Size(0, i);
         vector<MCTrackDesc>& tracks = gMCTracks[i];
      
         for (Int_t j = 0; j < nofPoints; ++j)
         {
            const CbmTrdPoint* trdPoint = static_cast<const CbmTrdPoint*> (fTrdPoints->Get(0, i, j));
            Int_t trackId = trdPoint->GetTrackID();
            int stationNumber = CbmTrdAddress::GetLayerId(trdPoint->GetModuleAddress());
            auto& trackDesk = tracks[trackId];
            trackDesk.mcPointMap[make_pair(ECbmModuleId::kTrd, j)] = { stationNumber, set<Int_t>() };
         }
      }
   }// if (fSettings->Use(kTrd))
   
   if (fSettings->Use(ECbmModuleId::kTof))
   {
      fTofHits = static_cast<TClonesArray*> (ioman->GetObject("TofHit"));
   
      if (0 == fTofHits)
         LOG(fatal) <<  "No tof hits in the input file";
   
      fTofHitDigiMatches = static_cast<TClonesArray*> (ioman->GetObject("TofDigiMatch"));
   
      if (0 == fTofHitDigiMatches)
         LOG(fatal) <<  "No tof hit to digi matches in the input file";
   
      fTofDigiPointMatches = static_cast<TClonesArray*> (ioman->GetObject("TofDigiMatchPoints"));
   
      if (0 == fTofDigiPointMatches)
         LOG(fatal) <<  "No tof digi to point matches in the input file";
      
      fTofDigis = static_cast<TClonesArray*> (ioman->GetObject("TofDigi"));
   
      if (0 == fTofDigis)
         LOG(fatal) <<  "No tof digis in the input file";
      
      fTofPoints = mcManager->InitBranch("TofPoint");
   
      if (0 == fTofPoints)
         LOG(fatal) <<  "No tof MC points in the input file";
      
      for (Int_t i = 0; fTofPoints->Size(0, i) >= 0; ++i)
      {
         Int_t nofPoints = fTofPoints->Size(0, i);
         vector<MCTrackDesc>& tracks = gMCTracks[i];
      
         for (Int_t j = 0; j < nofPoints; ++j)
         {
            const CbmTofPoint* tofPoint = static_cast<const CbmTofPoint*> (fTofPoints->Get(0, i, j));
            Int_t trackId = tofPoint->GetTrackID();
            auto& trackDesk = tracks[trackId];
            trackDesk.mcPointMap[make_pair(ECbmModuleId::kTof, j)] = { 0, set<Int_t>() };
         }
      }
   }// if (fSettings->Use(kTof))
   
   return kSUCCESS;
}

static Int_t gEventNumber = 0;
static int gNofRecoTracks = 0;
static int gNofMatchedRecoTracks = 0;
static int gNofClones = 0;

void CbmBinnedTrackerQANew::Exec(Option_t*)
{
    auto& eventMCTracks = gMCTracks[gEventNumber];
    Int_t nofGlobalTracks = fGlobalTracks->GetEntriesFast();
    
    for (Int_t i = 0; i < nofGlobalTracks; ++i)
    {
        const CbmGlobalTrack* globalTrack = static_cast<const CbmGlobalTrack*> (fGlobalTracks->At(i));
        Int_t stsTrackIndex = globalTrack->GetStsTrackIndex();
        Int_t muchTrackIndex = globalTrack->GetMuchTrackIndex();
        Int_t trdTrackIndex = globalTrack->GetTrdTrackIndex();
        Int_t tofHitIndex = globalTrack->GetTofHitIndex();
        
        map<void*, set<pair<ECbmModuleId, Int_t> > > mcTrackPtrs;
        Int_t nofTrackHits = 0;
        
        if (fSettings->Use(ECbmModuleId::kSts) && stsTrackIndex >= 0)
        {
            const CbmStsTrack* stsTrack = static_cast<const CbmStsTrack*> (fStsTracks->At(stsTrackIndex));
            Int_t nofStsHits = stsTrack->GetNofHits();
            nofTrackHits += nofStsHits;

            for (Int_t j = 0; j < nofStsHits; ++j)
            {
                Int_t stsHitInd = stsTrack->GetStsHitIndex(j);
                const CbmStsHit* stsHit = static_cast<const CbmStsHit*> (fStsHits->At(stsHitInd));
                //Int_t stationNumber = CbmStsSetup::Instance()->GetStationNumber(stsHit->GetAddress());
                Int_t frontClusterInd = stsHit->GetFrontClusterId();
                Int_t backClusterInd = stsHit->GetBackClusterId();
                const CbmStsCluster* frontCluster = static_cast<const CbmStsCluster*> (fStsClusters->At(frontClusterInd));
                Int_t nofFrontDigis = frontCluster->GetNofDigis();

                for (Int_t k = 0; k < nofFrontDigis; ++k)
                {
                    Int_t stsDigiInd = frontCluster->GetDigi(k);
                    //const CbmStsDigi* stsDigi = static_cast<const CbmStsDigi*> (fStsDigis->At(stsDigiInd));
                    const CbmMatch* stsDigiMatch = static_cast<const CbmMatch*> (fStsDigiMatches->At(stsDigiInd));
                    Int_t nofLinks = stsDigiMatch->GetNofLinks();

                    for (Int_t m = 0; m < nofLinks; ++m)
                    {
                        const CbmLink& link = stsDigiMatch->GetLink(m);
                        Int_t eventId = link.GetEntry();
                        Int_t mcPointId = link.GetIndex();
                        const CbmStsPoint* stsPoint = static_cast<const CbmStsPoint*> (fStsPoints->Get(0, eventId, mcPointId));
                        Int_t trackId = stsPoint->GetTrackID();
                        auto& mcTrack = eventMCTracks[trackId];
                        auto recoIter = mcTrack.mcPointMap.find(make_pair(ECbmModuleId::kSts, mcPointId));

                        if (recoIter != mcTrack.mcPointMap.end())
                            recoIter->second.second.insert(i);

                        mcTrackPtrs[&mcTrack].insert(make_pair(ECbmModuleId::kSts, j));
                    }
                }

                const CbmStsCluster* backCluster = static_cast<const CbmStsCluster*> (fStsClusters->At(backClusterInd));
                Int_t nofBackDigis = backCluster->GetNofDigis();

                for (Int_t k = 0; k < nofBackDigis; ++k)
                {
                    Int_t stsDigiInd = backCluster->GetDigi(k);
                    //const CbmStsDigi* stsDigi = static_cast<const CbmStsDigi*> (fStsDigis->At(stsDigiInd));
                    const CbmMatch* stsDigiMatch = static_cast<const CbmMatch*> (fStsDigiMatches->At(stsDigiInd));
                    Int_t nofLinks = stsDigiMatch->GetNofLinks();

                    for (Int_t m = 0; m < nofLinks; ++m)
                    {
                        const CbmLink& link = stsDigiMatch->GetLink(m);
                        Int_t eventId = link.GetEntry();
                        Int_t mcPointId = link.GetIndex();
                        const CbmStsPoint* stsPoint = static_cast<const CbmStsPoint*> (fStsPoints->Get(0, eventId, mcPointId));
                        Int_t trackId = stsPoint->GetTrackID();
                        auto& mcTrack = eventMCTracks[trackId];
                        auto recoIter = mcTrack.mcPointMap.find(make_pair(ECbmModuleId::kSts, mcPointId));

                        if (recoIter != mcTrack.mcPointMap.end())
                            recoIter->second.second.insert(i);

                        mcTrackPtrs[&mcTrack].insert(make_pair(ECbmModuleId::kSts, j));
                    }
                }
            }// for (Int_t i = 0; i < nofStsHits; ++i)
        }// if (fSettings->Use(kSts) && stsTrackIndex >= 0)
        
        if (fSettings->Use(ECbmModuleId::kMuch) && muchTrackIndex >= 0)
        {
            const CbmMuchTrack* muchTrack = static_cast<const CbmMuchTrack*> (fMuchTracks->At(muchTrackIndex));
            Int_t nofMuchHits = muchTrack->GetNofHits();
            nofTrackHits += nofMuchHits;

            for (Int_t j = 0; j < nofMuchHits; ++j)
            {
                Int_t muchHitInd = muchTrack->GetHitIndex(j);
                const CbmMuchPixelHit* muchHit = static_cast<const CbmMuchPixelHit*> (fMuchHits->At(muchHitInd));
                //Int_t muchStationNumber = CbmMuchGeoScheme::GetStationIndex(muchHit->GetAddress());
                //Int_t layerNumber = CbmMuchGeoScheme::GetLayerIndex(muchHit->GetAddress());
                //Int_t stationNumber = muchStationNumber * 3 + layerNumber;
                Int_t clusterId = muchHit->GetRefId();
                const CbmMuchCluster* cluster = static_cast<const CbmMuchCluster*> (fMuchClusters->At(clusterId));
                Int_t nofDigis = cluster->GetNofDigis();

                for (Int_t k = 0; k < nofDigis; ++k)
                {
                    Int_t digiId = cluster->GetDigi(k);
                    //const CbmMuchDigi* digi = static_cast<const CbmMuchDigi*> (fMuchDigis->At(digiId));
                    const CbmMatch* digiMatch = static_cast<const CbmMatch*> (fMuchDigiMatches->At(digiId));
                    Int_t nofLinks = digiMatch->GetNofLinks();

                    for (Int_t m = 0; m < nofLinks; ++m)
                    {
                        const CbmLink& link = digiMatch->GetLink(m);
                        Int_t eventId = link.GetEntry();
                        Int_t mcPointId = link.GetIndex();
                        const CbmMuchPoint* muchPoint = static_cast<const CbmMuchPoint*> (fMuchPoints->Get(0, eventId, mcPointId));
                        Int_t trackId = muchPoint->GetTrackID();
                        auto& mcTrack = eventMCTracks[trackId];
                        auto recoIter = mcTrack.mcPointMap.find(make_pair(ECbmModuleId::kMuch, mcPointId));

                        if (recoIter != mcTrack.mcPointMap.end())
                            recoIter->second.second.insert(i);

                        mcTrackPtrs[&mcTrack].insert(make_pair(ECbmModuleId::kMuch, j));
                    }
                }
            }
        }// if (fSettings->Use(ECbmModuleId::kMuch) && muchTrackIndex >= 0)

        if (fSettings->Use(ECbmModuleId::kTrd) && trdTrackIndex >= 0)
        {
            const CbmTrdTrack* trdTrack = static_cast<const CbmTrdTrack*> (fTrdTracks->At(trdTrackIndex));
            Int_t nofTrdHits = trdTrack->GetNofHits();
            nofTrackHits += nofTrdHits;

            for (Int_t j = 0; j < nofTrdHits; ++j)
            {
                Int_t trdHitInd = trdTrack->GetHitIndex(j);
                const CbmTrdHit* trdHit = static_cast<const CbmTrdHit*> (fTrdHits->At(trdHitInd));
                Int_t clusterId = trdHit->GetRefId();
                const CbmTrdCluster* cluster = static_cast<const CbmTrdCluster*> (fTrdClusters->At(clusterId));
                Int_t nofDigis = cluster->GetNofDigis();

                for (Int_t k = 0; k < nofDigis; ++k)
                {
                    Int_t digiId = cluster->GetDigi(k);
                    const CbmMatch* match = static_cast<const CbmMatch*> (fTrdDigiMatches->At(digiId));
                    Int_t nofLinks = match->GetNofLinks();

                    for (Int_t m = 0; m < nofLinks; ++m)
                    {
                        const CbmLink& link = match->GetLink(m);
                        Int_t eventId = link.GetEntry();
                        Int_t mcPointId = link.GetIndex();
                        const CbmTrdPoint* trdPoint = static_cast<const CbmTrdPoint*> (fTrdPoints->Get(0, eventId, mcPointId));
                        Int_t trackId = trdPoint->GetTrackID();
                        auto& mcTrack = eventMCTracks[trackId];
                        auto recoIter = mcTrack.mcPointMap.find(make_pair(ECbmModuleId::kTrd, mcPointId));

                        if (recoIter != mcTrack.mcPointMap.end())
                            recoIter->second.second.insert(i);

                        mcTrackPtrs[&mcTrack].insert(make_pair(ECbmModuleId::kTrd, j));
                    }
                }
            }// for (Int_t j = 0; j < nofTrdHits; ++j)
        }// if (fSettings->Use(kTrd) && trdTrackIndex >= 0)
        
        if (fSettings->Use(ECbmModuleId::kTof) && tofHitIndex >= 0)
        {
            ++nofTrackHits;
            const CbmMatch* tofHitMatch = static_cast<const CbmMatch*> (fTofHitDigiMatches->At(tofHitIndex));
            Int_t nofTofDigis = tofHitMatch->GetNofLinks();

            for (Int_t j = 0; j < nofTofDigis; ++j)
            {
                const CbmLink& digiLink = tofHitMatch->GetLink(j);
                Int_t digiInd = digiLink.GetIndex();
                const CbmMatch* pointMatch = static_cast<const CbmMatch*> (fTofDigiPointMatches->At(digiInd));
                Int_t nofPoints = pointMatch->GetNofLinks();

                for (Int_t k = 0; k < nofPoints; ++k)
                {
                    const CbmLink& pointLink = pointMatch->GetLink(k);
                    Int_t mcEventId = pointLink.GetEntry();
                    Int_t mcPointId = pointLink.GetIndex();
                    const CbmTofPoint* tofPoint = static_cast<const CbmTofPoint*> (fTofPoints->Get(0, mcEventId, mcPointId));
                    Int_t trackId = tofPoint->GetTrackID();
                    auto& mcTrack = eventMCTracks[trackId];
                    auto recoIter = mcTrack.mcPointMap.find(make_pair(ECbmModuleId::kTof, mcPointId));

                    if (recoIter != mcTrack.mcPointMap.end())
                        recoIter->second.second.insert(i);

                    mcTrackPtrs[&mcTrack].insert(make_pair(ECbmModuleId::kTof, 0));
                }
            }
        }// if (fSettings->Use(kTof) && tofHitIndex >= 0)
        
         ++gNofRecoTracks;
        
        for (const auto& j : mcTrackPtrs)
        {            
            if (j.second.size() >= size_t(0.7 * nofTrackHits))
            {
                ++gNofMatchedRecoTracks;
                break;
            }
        }
    }// for (Int_t i = 0; i < nofGlobalTracks; ++i)
    
    ++gEventNumber;
}//Exec()

void CbmBinnedTrackerQANew::Finish()
{
    int nofRefMCTRacks = 0;
    int nofMatchedRefMCTracks = 0;
    uint maxTrackLength = 0;
    
    for (const auto& i : gMCTracks)
    {
        for (const auto& j : i)
        {            
            set<pair<ECbmModuleId, UInt_t> > stationNofs;
            
            for (const auto& k : j.mcPointMap)
                stationNofs.insert(make_pair(k.first.first, k.second.first));
            
            if (stationNofs.size() > static_cast<size_t>(maxTrackLength))
                maxTrackLength = stationNofs.size();
            
            if (stationNofs.size() < static_cast<size_t>(fMinTrackLength))
                continue;
            
            map<Int_t, set<pair<ECbmModuleId, UInt_t> > > recoToStationMatches;
            
            for (const auto& k : j.mcPointMap)
            {   
                for (const auto& m : k.second.second)
                    recoToStationMatches[m].insert(make_pair(k.first.first, k.second.first));
            }
            
            int nofMatchedReco = 0;
            
            for (const auto& k : recoToStationMatches)
            {
                if (k.second.size() >= size_t(stationNofs.size() * 0.7))
                    ++nofMatchedReco;
            }
            
            if (nofMatchedReco > 1)
                gNofClones += nofMatchedReco - 1;
            
            //if (abs(j.pdg) != 11)
                //continue;
            
            if (fPrimaryParticlePdg >= 0 && (j.parentInd < 0 || i[j.parentInd].pdg != fPrimaryParticlePdg))
                continue;
            
            ++nofRefMCTRacks;
            
            if (nofMatchedReco > 0)
                ++nofMatchedRefMCTracks;
        }
    }
    
    cout << "Maximum MC track length: " << maxTrackLength << endl;
    
    double res = 100 * nofMatchedRefMCTracks;
    res /= nofRefMCTRacks;
    cout << "The reconstruction efficiency: " << res << " % = 100 * " << nofMatchedRefMCTracks << " / " << nofRefMCTRacks << endl;
    
    res = 100 * (gNofRecoTracks - gNofMatchedRecoTracks);
    res /= gNofRecoTracks;
    cout << "The share of ghosts: " << res << " % = 100 * (" << gNofRecoTracks << " - " << gNofMatchedRecoTracks << ") / " << gNofRecoTracks << endl;
    
    res = 100 * gNofClones;
    res /= gNofRecoTracks;
    cout << "The share of clones: " << res << " % = 100 * " << gNofClones << " / " << gNofRecoTracks << endl;
}

void CbmBinnedTrackerQANew::SetParContainers()
{
   fSettings = static_cast<CbmBinnedSettings*> (FairRun::Instance()->GetRuntimeDb()->getContainer("CbmBinnedSettings"));
}

ClassImp(CbmBinnedTrackerQANew)
