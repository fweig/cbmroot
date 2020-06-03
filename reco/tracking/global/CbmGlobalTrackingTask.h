/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CbmGlobalTrackingTask.h
 * Author: tablyaz
 *
 * Created on January 31, 2017, 3:33 PM
 */

#ifndef CBMGLOBALTRACKINGTASK_H
#define CBMGLOBALTRACKINGTASK_H

#include "CbmGlobalTrackingTof.h"
#include "FairTask.h"
#include "TClonesArray.h"
#include "CbmTimeSlice.h"
#include "CbmTofHit.h"
#include "CbmKFTrack.h"
#include "CbmVertex.h"

class CbmGlobalTrackingTask : public FairTask
{
public:
    CbmGlobalTrackingTask();

    CbmGlobalTrackingTask(const CbmGlobalTrackingTask&) = delete;
    CbmGlobalTrackingTask& operator=(const CbmGlobalTrackingTask&) = delete;

    InitStatus Init();// Overridden from FairTask
    void Exec(Option_t* opt);// Overridden from FairTask
    void Finish();// Overridden from FairTask
    
    void SetNofTBins(int v)
    {
        fTofGeometry.SetNofTBins(v);
    }
    
    void SetTBinSize(scaltype v)
    {
        fTofGeometry.SetTBinSize(v);
    }
    
    void SetPropagatePdg(Int_t v)
    {
        fTofGeometry.SetPdg(v);
    }
    
    void SetChi2Cut(Double_t v)
    {
        fTofGeometry.SetChi2Cut(v);
    }
    
private:
    Double_t CalcStsTrackLength(const CbmStsTrack* track);
    
private:
    CbmGlobalTrackingTofGeometry fTofGeometry;
    CbmTimeSlice* fTimeSlice;
    TClonesArray* fTofHits;
    TClonesArray* fStsTracks;
    TClonesArray* fStsHits;
    TClonesArray* fMvdHits;
    TClonesArray* fGlobalTracks;
    CbmKFTrack fKFTrack;
    CbmVertex* fPrimVertex;
    
ClassDef(CbmGlobalTrackingTask, 1)        
};

#endif /* CBMGLOBALTRACKINGTASK_H */

