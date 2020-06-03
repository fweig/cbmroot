/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CbmBinnedTrackerQA.h
 * Author: tablyaz
 *
 * Created on August 24, 2017, 6:21 PM
 */

#ifndef CBMBINNEDTRACKERQA_H
#define CBMBINNEDTRACKERQA_H

#include "FairTask.h"
#include "TClonesArray.h"
#include "CbmMCDataArray.h"
#include "CbmStsTrack.h"
#include "CbmMuchTrack.h"
#include "CbmTrdTrack.h"
#include "CbmTofHit.h"
#include "CbmTrdHit.h"
#include "CbmTrdPoint.h"
#include "Settings.h"
#include <functional>

class CbmBinnedTrackerQA : public FairTask
{
public:
    CbmBinnedTrackerQA();
    CbmBinnedTrackerQA(const CbmBinnedTrackerQA&) = delete;
    CbmBinnedTrackerQA& operator=(const CbmBinnedTrackerQA&) = delete;
    InitStatus Init();// Overridden from FairTask
    void Exec(Option_t* opt);// Overridden from FairTask
    void Finish();// Overridden from FairTask
    void SetParContainers();
    
    void SetPrimaryParticle(EPrimaryParticleId v)
    {
        fPrimaryParticleIds.clear();
        fPrimaryParticleIds.push_back(v);
    }
    
    void AddPrimaryParticle(EPrimaryParticleId v)
    {
        fPrimaryParticleIds.push_back(v);
    }
    
private:
    void HandleSts(Int_t stsTrackIndex, std::map<Int_t, std::set<Int_t> >& mcTrackIds, std::set<Int_t>* globalTrackMCRefs, Int_t* globalTracksHitInds);
    void HandleMuch(Int_t muchTrackIndex, std::map<Int_t, std::set<Int_t> >& mcTrackIds, std::set<Int_t>* globalTrackMCRefs, Int_t* globalTracksHitInds);
    void HandleTrd(Int_t trdTrackIndex, std::map<Int_t, std::set<Int_t> >& mcTrackIds, std::set<Int_t>* globalTrackMCRefs, Int_t* globalTracksHitInds);
    void HandleTof(Int_t globalTrackIndex, Int_t tofHitIndex, std::map<Int_t, std::set<Int_t> >& mcTrackIds, std::set<Int_t>* globalTrackMCRefs, Int_t* globalTracksHitInds);
    void IterateTrdHits(std::function<void(const CbmTrdHit*, const CbmTrdPoint*)> handleData);
    
private:
    std::list<EPrimaryParticleId> fPrimaryParticleIds;
    bool fIsOnlyPrimary;
    CbmBinnedSettings* fSettings;
    TClonesArray* fGlobalTracks;
    TClonesArray* fStsTracks;
    TClonesArray* fMuchTracks;
    TClonesArray* fTrdTracks;
    TClonesArray* fStsHits;
    TClonesArray* fMuchHits;
    TClonesArray* fTrdHits;
    TClonesArray* fTofHits;
    TClonesArray* fStsClusters;
    TClonesArray* fMuchClusters;
    TClonesArray* fTrdClusters;
    
    TClonesArray* fTrdDigiMatches;
    TClonesArray* fTofHitDigiMatches;
    TClonesArray* fTofDigiPointMatches;
    
    TClonesArray* fStsDigis;
    TClonesArray* fStsDigiMatches;
    TClonesArray* fMuchDigis;
    TClonesArray* fMuchDigiMatches;
    TClonesArray* fTrdDigis;
    TClonesArray* fTofDigis;
    
    CbmMCDataArray* fMCTracks;
    CbmMCDataArray* fStsPoints;
    CbmMCDataArray* fMuchPoints;
    CbmMCDataArray* fTrdPoints;
    CbmMCDataArray* fTofPoints;
    ClassDef(CbmBinnedTrackerQA, 1)
};

#endif /* CBMBINNEDTRACKERQA_H */

