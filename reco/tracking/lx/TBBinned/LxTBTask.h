/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LxTBTask.h
 * Author: Timur Ablyazimov
 *
 * Created on 14 Mar 2016
 */

#ifndef LXTBTASK_H
#define LXTBTASK_H

#include "LxTBDefinitions.h"
#include "FairTask.h"
#include "CbmMuchPoint.h"
#include "CbmTrdPoint.h"
#include <vector>

#define CUR_NOF_TRD_LAYERS 4
#define CUR_LAST_TRD_LAYER CUR_NOF_TRD_LAYERS - 1

#include "TGeoManager.h"
#include "CbmPixelHit.h"
#include "CbmMCDataArray.h"
#include "CbmStsTrack.h"
#include "LxTBTieTracks.h"

#define CUR_NOF_STATIONS 4
#define CUR_LAST_STATION CUR_NOF_STATIONS - 1
//#define CUR_NOF_TIMEBINS 5
//#define CUR_LAST_TIMEBIN CUR_NOF_TIMEBINS - 1
#define CUR_TIMEBIN_LENGTH 100

#include "LxTBBinned.h"

class LxTBFinder : public FairTask
{
public:
#ifdef LXTB_QA
    struct PointDataHolder
    {
        Double_t x;
        Double_t y;
        Double_t z;
        Double_t t;
        Int_t eventId;
        Int_t trackId;
        Int_t pointId;
        Int_t stationNumber;
        Int_t layerNumber;
    };
    
    struct TrackDataHolder
    {
        Int_t pointInds[CUR_NOF_STATIONS];
        Int_t trdPointInds[CUR_NOF_TRD_LAYERS];
        bool isSignalShort;
        bool isSignalLong;
        bool isPos;
        
        TrackDataHolder() : isSignalShort(false), isSignalLong(false), isPos(false)
        {
            for (int i = 0; i < CUR_NOF_STATIONS; ++i)
                pointInds[i] = -1;
            
            for (int i = 0; i < CUR_NOF_TRD_LAYERS; ++i)
                trdPointInds[i] = -1;
        }
    };
#endif//LXTB_QA
    
public:
    LxTBFinder();
    LxTBFinder(const LxTBFinder&) = delete;
    LxTBFinder& operator=(const LxTBFinder&) = delete;

    InitStatus Init();// Overridden from FairTask
    void Exec(Option_t* opt);// Overridden from FairTask
    void Finish();// Overridden from FairTask
    
    void SetEvByEv(bool v = true)
    {
        isEvByEv = v;
        
        if (isEvByEv)
#ifdef LXTB_EMU_TS
            nof_timebins = 1000;
#else//LXTB_EMU_TS
            nof_timebins = 5;
#endif//LXTB_EMU_TS
        else
            nof_timebins = 1000;
        
        last_timebin = nof_timebins - 1;
    }
    
    void SetUseTrd(bool v = true) { useTrd = v; }
    void SetUseIdeal(bool v = true) { useIdeal = v; }
    void SetUseAsciiSig(bool v = true) { useAsciiSig = v; }
    void SetSignalParticle(const char* name) { fSignalParticle = name; }
    void SetNEvents(Int_t v) { fNEvents = v; }
    
private:
    void HandleGeometry();
#ifdef LXTB_QA
    void AddHit(const CbmPixelHit* hit, Int_t stationNumber, Int_t refId, bool isTrd);
#else
    void AddHit(const CbmPixelHit* hit, Int_t stationNumber, Int_t refId);
#endif//LXTB_QA
    
#ifdef LXTB_TIE
    void AddLayerHit(const CbmPixelHit* hit, Int_t layerNumber, Int_t refId, bool isTrd);
    void AddStsTrack(const CbmStsTrack& stsTrack, Int_t selfId);
#endif//LXTB_TIE
    
    std::vector<std::vector<PointDataHolder> > fMuchPoints;
    CbmMCDataArray* fMuchMCPoints;
    TClonesArray* fMuchPixelHits;
    TClonesArray* fMuchClusters;
    TClonesArray* fMuchPixelDigiMatches;
    
    std::vector<std::vector<PointDataHolder> > fTrdPoints;
    CbmMCDataArray* fTrdMCPoints;
    TClonesArray* fTrdHits;
    TClonesArray* fTrdClusters;
    TClonesArray* fTrdDigiMatches;
    
#ifdef LXTB_QA
    TClonesArray* fMvdDigis;
    TClonesArray* fStsDigis;
    TClonesArray* fTofDigis;
#endif//LXTB_QA
    
#ifdef LXTB_QA    
    std::vector<std::vector<TrackDataHolder> > fMCTracks;
#endif//LXTB_QA
    bool isEvByEv;
    LxTbBinnedFinder* fFinder;
    std::list<LxTbBinnedFinder::Chain*> recoTracks;
    bool hasTrd;
    bool useTrd;
    bool useIdeal;
    bool useAsciiSig;
    const char* fSignalParticle;
    unsigned int nof_timebins;
    unsigned int last_timebin;
    Int_t fNEvents;
    std::vector<Double_t> fEventTimes;
#ifdef LXTB_TIE
    TClonesArray* fStsHits;
    TClonesArray* fStsTracks;
    TClonesArray* fStsClusters;
    TClonesArray* fStsDigiMatches;
    LxTBBinnedDetector* fDetector;
#endif//LXTB_TIE
    int dummy;

ClassDef(LxTBFinder, 1)
};

#endif /* LXTBTASK_H */
