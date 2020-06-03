/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LxTBMLTask.h
 * Author: tablyaz
 *
 * Created on October 14, 2016, 11:35 AM
 */

#ifndef LXTBMLTASK_H
#define LXTBMLTASK_H

#include "FairTask.h"
#include "CbmMCDataArray.h"
#include "LxTBBinned2.h"

class LxTBMLFinder : public FairTask
{
public:
   struct Chain
   {
        LxTbBinnedPoint* points[NOF_STATIONS][NOF_LAYERS];
        scaltype chi2;

        Chain(LxTbBinnedPoint* pts[NOF_STATIONS][NOF_LAYERS], scaltype Chi2) : chi2(Chi2)
        {
            for (int i = 0; i < NOF_STATIONS; ++i)
            {
                for (int j = 0; j < NOF_LAYERS; ++j)
                    points[i][j] = new LxTbBinnedPoint(*pts[i][j]);
            }
        }
        
        Chain(const Chain& origingal) : chi2(origingal.chi2)
        {
            for (int i = 0; i < NOF_STATIONS; ++i)
            {
                for (int j = 0; j < NOF_LAYERS; ++j)
                    points[i][j] = new LxTbBinnedPoint(*origingal.points[i][j]);
            }
        }

        ~Chain()
        {
            for (int i = 0; i < NOF_STATIONS; ++i)
            {
                for (int j = 0; j < NOF_LAYERS; ++j)
                    delete points[i][j];
            }
        }
   };
   
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
        Int_t pointInds[NOF_STATIONS][NOF_LAYERS];
        bool isSignal;
        bool isPos;
        
        TrackDataHolder() : isSignal(false), isPos(false)
        {
            for (int i = 0; i < NOF_STATIONS; ++i)
            {
                for (int j = 0; j < NOF_LAYERS; ++j)
                    pointInds[i][j] = -1;
            }
        }
    };
#endif//LXTB_QA
    
    LxTBMLFinder();
    InitStatus Init();// Overridden from FairTask
    void Exec(Option_t* opt);// Overridden from FairTask
    void Finish();// Overridden from FairTask
    
    void SetEvByEv(bool v)
    {
        fIsEvByEv = v;
        fNofTBins = fIsEvByEv ? 5 : 1000;
    }
    
    void SetNEvents(int v)
    {
        fNEvents = v;
    }
    
private:
    void* fReconstructor;
    bool fIsEvByEv;
    int fNofXBins;
    int fNofYBins;
    int fNofTBins;
    
    CbmMCDataArray* fMuchMCPoints;
    TClonesArray* fMuchPixelHits;
    TClonesArray* fMuchClusters;
    TClonesArray* fMuchPixelDigiMatches;
    std::list<Chain*> recoTracks;
    
#ifdef LXTB_QA
    std::vector<std::vector<PointDataHolder> > fMuchPoints;
    std::vector<std::vector<TrackDataHolder> > fMCTracks;
    std::vector<Double_t> fEventTimes;
#endif//LXTB_QA
    int fNEvents;
    
    ClassDef(LxTBMLFinder, 1)
};

#endif /* LXTBMLTASK_H */

