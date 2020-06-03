/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Debug.h
 * Author: tablyaz
 *
 * Created on September 19, 2017, 12:40 PM
 */

#ifndef DEBUG_H
#define DEBUG_H

#include "FairRootManager.h"
#include "CbmMCDataArray.h"
#include "CbmMCDataManager.h"
#include "CbmStsHit.h"
#include "CbmTrdHit.h"
#include "CbmMatch.h"
#include "setup/CbmStsSetup.h"
#include "CbmStsPoint.h"
#include "CbmStsDigi.h"
#include "CbmTrdPoint.h"
#include "CbmTrdCluster.h"
#include "CbmMCTrack.h"

struct CbmBinnedDebug
{
    Int_t fEventNumber;
    FairRootManager* fIoman;
    CbmMCDataArray* fMCTrackArray;
    TClonesArray* fStsHitArray;
    TClonesArray* fStsClusterArray;
    TClonesArray* fStsDigiArray;
    CbmMCDataArray* fStsPointArray;
    TClonesArray* fTrdHitArray;
    TClonesArray* fTrdClusterArray;
    TClonesArray* fTrdDigiMatchArray;
    CbmMCDataArray* fTrdPointArray;
    
    struct MCTrack
    {
        bool isRef;
        std::set<Int_t> stsHits[2];
        std::set<Int_t> trdHits[4];
        
        MCTrack() : isRef(false) {}
    };
    
    std::vector<MCTrack> fMCTracks;
    
    CbmBinnedDebug() : fEventNumber(0), fIoman(0), fMCTrackArray(0), fStsHitArray(0), fStsClusterArray(0), fStsDigiArray(0), fStsPointArray(0), fTrdHitArray(0),
        fTrdClusterArray(0), fTrdDigiMatchArray(0), fTrdPointArray(0), fMCTracks()
    {
        fIoman = FairRootManager::Instance();
        CbmMCDataManager* mcManager = static_cast<CbmMCDataManager*> (fIoman->GetObject("MCDataManager"));
        fMCTrackArray = mcManager->InitBranch("MCTrack");
        fStsHitArray = static_cast<TClonesArray*> (fIoman->GetObject("StsHit"));
        fStsClusterArray = static_cast<TClonesArray*> (fIoman->GetObject("StsCluster"));
        fStsDigiArray = static_cast<TClonesArray*> (fIoman->GetObject("StsDigi"));
        fStsPointArray = mcManager->InitBranch("StsPoint");
        fTrdHitArray = static_cast<TClonesArray*> (fIoman->GetObject("TrdHit"));
        fTrdClusterArray = static_cast<TClonesArray*> (fIoman->GetObject("TrdCluster"));
        fTrdDigiMatchArray = static_cast<TClonesArray*> (fIoman->GetObject("TrdDigiMatch"));
        fTrdPointArray = mcManager->InitBranch("TrdPoint");
    }
    
    CbmBinnedDebug(const CbmBinnedDebug&) = delete;
    CbmBinnedDebug& operator=(const CbmBinnedDebug&) = delete;
    
    void Exec()
    {
        fMCTracks.clear();
        Int_t nofMCTracks = fMCTrackArray->Size(0, fEventNumber);
        fMCTracks.resize(nofMCTracks);
        Int_t nofStsHits = fStsHitArray->GetEntriesFast();
        
        for (Int_t i = 0; i < nofStsHits; ++i)
        {
            const CbmStsHit* stsHit = static_cast<const CbmStsHit*> (fStsHitArray->At(i));
            Int_t stationNumber = CbmStsSetup::Instance()->GetStationNumber(stsHit->GetAddress());
            Int_t frontClusterInd = stsHit->GetFrontClusterId();
            Int_t backClusterInd = stsHit->GetBackClusterId();
            const CbmStsCluster* frontCluster = static_cast<const CbmStsCluster*> (fStsClusterArray->At(frontClusterInd));
            Int_t nofFrontDigis = frontCluster->GetNofDigis();

            for (Int_t j = 0; j < nofFrontDigis; ++j)
            {
                Int_t stsDigiInd = frontCluster->GetDigi(j);
                const CbmStsDigi* stsDigi = static_cast<const CbmStsDigi*> (fStsDigiArray->At(stsDigiInd));
                const CbmMatch* match = stsDigi->GetMatch();
                Int_t nofLinks = match->GetNofLinks();

                for (Int_t k = 0; k < nofLinks; ++k)
                {
                    const CbmLink& link = match->GetLink(k);
                    Int_t eventId = link.GetEntry();
                    Int_t mcPointId = link.GetIndex();
                    const CbmStsPoint* stsPoint = static_cast<const CbmStsPoint*> (fStsPointArray->Get(0, eventId, mcPointId));
                    Int_t trackId = stsPoint->GetTrackID();
                    MCTrack& mcTrack = fMCTracks[trackId];
                    mcTrack.stsHits[stationNumber].insert(i);
                }
            }

            const CbmStsCluster* backCluster = static_cast<const CbmStsCluster*> (fStsClusterArray->At(backClusterInd));
            Int_t nofBackDigis = backCluster->GetNofDigis();

            for (Int_t j = 0; j < nofBackDigis; ++j) {
                Int_t stsDigiInd = backCluster->GetDigi(j);
                const CbmStsDigi* stsDigi = static_cast<const CbmStsDigi*> (fStsDigiArray->At(stsDigiInd));
                const CbmMatch* match = stsDigi->GetMatch();
                Int_t nofLinks = match->GetNofLinks();

                for (Int_t k = 0; k < nofLinks; ++k)
                {
                    const CbmLink& link = match->GetLink(k);
                    Int_t eventId = link.GetEntry();
                    Int_t mcPointId = link.GetIndex();
                    const CbmStsPoint* stsPoint = static_cast<const CbmStsPoint*> (fStsPointArray->Get(0, eventId, mcPointId));
                    Int_t trackId = stsPoint->GetTrackID();
                    MCTrack& mcTrack = fMCTracks[trackId];
                    mcTrack.stsHits[stationNumber].insert(i);
                }
            }
        }
        
        Int_t nofTrdHits = fTrdHitArray->GetEntriesFast();
   
        for (Int_t i = 0; i < nofTrdHits; ++i)
        {
            const CbmTrdHit* trdHit = static_cast<const CbmTrdHit*> (fTrdHitArray->At(i));
            Int_t stationNumber = trdHit->GetPlaneId();
            Int_t clusterId = trdHit->GetRefId();
            const CbmTrdCluster* cluster = static_cast<const CbmTrdCluster*> (fTrdClusterArray->At(clusterId));
            Int_t nofDigis = cluster->GetNofDigis();

            for (Int_t j = 0; j < nofDigis; ++j)
            {
                Int_t digiId = cluster->GetDigi(j);
                const CbmMatch* match = static_cast<const CbmMatch*> (fTrdDigiMatchArray->At(digiId));
                Int_t nofLinks = match->GetNofLinks();

                for (Int_t k = 0; k < nofLinks; ++k)
                {
                    const CbmLink& link = match->GetLink(k);
                    Int_t eventId = link.GetEntry();
                    Int_t mcPointId = link.GetIndex();
                    const CbmTrdPoint* trdPoint = static_cast<const CbmTrdPoint*> (fTrdPointArray->Get(0, eventId, mcPointId));
                    Int_t trackId = trdPoint->GetTrackID();
                    MCTrack& mcTrack = fMCTracks[trackId];
                    mcTrack.trdHits[stationNumber].insert(i);
                }
            }
        }
        
        for (int i = 0; i < nofMCTracks; ++i)
        {
            MCTrack& mcTrack = fMCTracks[i];
            const CbmMCTrack* track = static_cast<const CbmMCTrack*> (fMCTrackArray->Get(0, fEventNumber, i));
            mcTrack.isRef = track->GetMotherId() < 0;
            
            if (!mcTrack.isRef)
                continue;
            
            for (int j = 0; j < 2; ++j)
            {
                if (mcTrack.stsHits[j].empty())
                {
                    mcTrack.isRef = false;
                    break;
                }
            }
            
            if (!mcTrack.isRef)
                continue;
            
            for (int j = 0; j < 4; ++j)
            {
                if (mcTrack.trdHits[j].empty())
                {
                    mcTrack.isRef = false;
                    break;
                }
            }
        }
        
        ++fEventNumber;
    }
    
    bool TrackHasHit(int stNo, Int_t trackInd, Int_t hitInd)
    {
        if (stNo < 0)
            return -1;
        else if (stNo < 2)
            return TrackHasHitSts(stNo, trackInd, hitInd);
        else if (stNo < 6)
            return TrackHasHitTrd(stNo - 2, trackInd, hitInd);
        else
            return -1;
    }
    
    bool TrackHasHitSts(int stNo, Int_t trackInd, Int_t hitInd)
    {
        return fMCTracks[trackInd].stsHits[stNo].find(hitInd) != fMCTracks[trackInd].stsHits[stNo].end();
    }
    
    bool TrackHasHitTrd(int stNo, Int_t trackInd, Int_t hitInd)
    {
        return fMCTracks[trackInd].trdHits[stNo].find(hitInd) != fMCTracks[trackInd].trdHits[stNo].end();
    }
};

#endif /* DEBUG_H */

