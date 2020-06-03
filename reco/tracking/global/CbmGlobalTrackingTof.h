/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CbmGlobalTrackingTof.h
 * Author: tablyaz
 *
 * Created on January 31, 2017, 5:51 PM
 */

#ifndef CBMGLOBALTRACKINGTOF_H
#define CBMGLOBALTRACKINGTOF_H

#include <list>
#include "CbmGlobalTrackingDefs.h"
#include "FairTrackParam.h"
#include "TClonesArray.h"
#include <set>
#include <map>

#define CBM_GLOBALTB_TOF_3D

#ifdef CBM_GLOBALTB_TOF_3D
//#define CBM_GLOBALTB_TOF_3D_CUBOIDS
#endif//CBM_GLOBALTB_TOF_3D

#ifdef CBM_GLOBALTB_TOF_3D
#ifdef CBM_GLOBALTB_TOF_3D_CUBOIDS
struct Cuboid;
#endif//CBM_GLOBALTB_TOF_3D_CUBOIDS
struct ZBin;
#endif//CBM_GLOBALTB_TOF_3D

class CbmGlobalTrackingTofGeometry
{
public:
    struct Rect
    {
        scaltype minX;
        scaltype maxX;
        scaltype minY;
        scaltype maxY;
    };
    
    CbmGlobalTrackingTofGeometry();

    CbmGlobalTrackingTofGeometry(const CbmGlobalTrackingTofGeometry&) = delete;
    CbmGlobalTrackingTofGeometry& operator=(const CbmGlobalTrackingTofGeometry&) = delete;

    ~CbmGlobalTrackingTofGeometry();
    bool Read();
    
    void SetPdg(Int_t v)
    {
        fPDG = v;
    }
    
    void SetChi2Cut(Double_t v)
    {
        fChi2Cut = v;
    }
    
    int GetXInd(scaltype x) const
    {
        int ind = (x - fMinX) / fXBinSize;
        
        if (ind < 0)
            ind = 0;
        else if (ind >= fNofXBins)
            ind = fNofXBins - 1;
        
        return ind;
    }
    
    int GetYInd(scaltype y) const
    {
        int ind = (y - fMinY) / fYBinSize;
        
        if (ind < 0)
            ind = 0;
        else if (ind >= fNofYBins)
            ind = fNofYBins - 1;
        
        return ind;
    }
    
    int GetZInd(scaltype z) const
    {
        int ind = (z - fMinZ) / fZBinSize;
        
        if (ind < 0)
            ind = 0;
        else if (ind >= fNofZBins)
            ind = fNofZBins - 1;
        
        return ind;
    }
    
    int GetTInd(timetype t) const
    {
        int ind = (t - fStartTime) / fTBinSize;
        
        if (ind < 0)
            ind = 0;
        else if (ind >= fNofTBins)
            ind = fNofTBins - 1;
        
        return ind;
    }
    
    void SetNofTBins(int v)
    {
        fNofTBins = v;
    }
    
    void SetTBinSize(scaltype v)
    {
        fStartTime = v;
    }
    
    void SetStartTime(timetype v)
    {
        fTBinSize = v;
    }
    
    void SetTofHits(TClonesArray* v)
    {
        fTofHits = v;
    }
    
    void Clear();
    void Prepare(timetype startTime);
    void Find(FairTrackParam& trackParams, timetype trackTime, timetype errT, Int_t& tofHitInd, Double_t& length);
    
private:
    scaltype fC;
    Int_t fPDG;
    Double_t fChi2Cut;
#ifdef CBM_GLOBALTB_TOF_3D_CUBOIDS
    std::list<Cuboid*> fCuboids;
#endif//CBM_GLOBALTB_TOF_3D_CUBOIDS
    int fNofTBins;
    int fNofXBins;
    int fNofYBins;
    int fNofZBins;
    ZBin* fZBins;
    scaltype fTBinSize;
    scaltype fMinX;
    scaltype fMaxX;
    scaltype fXBinSize;
    scaltype fMinY;
    scaltype fMaxY;
    scaltype fYBinSize;
    scaltype fMinZ;
    scaltype fMaxZ;
    scaltype fZBinSize;
    timetype fStartTime;
    timetype fEndTime;
    TClonesArray* fTofHits;
};

#endif /* CBMGLOBALTRACKINGTOF_H */
