/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Station3D.h
 * Author: tablyaz
 *
 * Created on August 7, 2017, 2:08 PM
 */

#ifndef STATION3D_H
#define STATION3D_H

#include "Station.h"
#include <list>
#include <utility>

class CbmBinned3DStation : public CbmBinnedStation
{
public:
    CbmBinned3DStation(ECbmModuleId stationType, Double_t minZ, Double_t maxZ, int nofYBins, int nofXBins, int nofTBins) :
        CbmBinnedStation(stationType, minZ, maxZ, nofYBins, nofXBins, nofTBins),
            fYBins(reinterpret_cast<CbmYBin*> (new unsigned char[nofYBins * sizeof(CbmYBin)]))
    {
        for (int i = 0; i < nofYBins; ++i)
            new(&fYBins[i]) CbmYBin(0, nofXBins, nofTBins);
    }
    
    void Clear()
    {
        CbmBinnedStation::Clear();
        
        for (int i = 0; i < fNofYBins; ++i)
        {
            CbmYBin& yBin = fYBins[i];
            yBin.SetUse(false);
            
            for (int j = 0; j < fNofXBins; ++j)
            {
                CbmXBin& xBin = yBin[j];
                xBin.SetUse(false);
                
                for (int k = 0; k < fNofTBins; ++k)
                {
                    CbmTBin& tBin = xBin[k];
                    tBin.SetUse(false);
                    tBin.Clear();
                }
            }
        }
    }
    
    void AddHit(ECbmModuleId type, const CbmPixelHit* hit, Int_t index)
    {
        Double_t y = hit->GetY();
        
        if (y < fMinY || y >= fMaxY)
            return;
        
        Double_t x = hit->GetX();
        
        if (x < fMinX || x >= fMaxX)
            return;
        
        Double_t t = hit->GetTime();
        
        //if (t < fMinT || t >= fMaxT)
            //return;
        
        int yInd = (y - fMinY) / fYBinSize;
        int xInd = (x - fMinX) / fXBinSize;
        int tInd = (t - fMinT) / fTBinSize;
        
        if (tInd < 0)
            tInd = 0;
        else if (tInd >= fNofTBins)
            tInd = fNofTBins - 1;
        
        CbmYBin& yBin = fYBins[yInd];
        CbmXBin& xBin = yBin[xInd];
        CbmTBin& tBin = xBin[tInd];
        tBin.AddHit(type, hit, index, fDefaultUse);
        SetDx(hit->GetDx());
        SetDy(hit->GetDy());
        SetDt(hit->GetTimeError());
        
        if (fDefaultUse)
        {
            yBin.SetUse(true);
            xBin.SetUse(true);
            tBin.SetUse(true);
        }
    }
    
    void IterateHits(std::function<void(CbmTBin::HitHolder&)> handleHit)
    {
        for (int i = 0; i < fNofYBins; ++i)
        {
            CbmYBin& yBin = fYBins[i];
            
            //if (!yBin.Use())
                //continue;
            
            for (int j = 0; j < fNofXBins; ++j)
            {
                CbmXBin& xBin = yBin[j];
            
                //if (!xBin.Use())
                    //continue;
                
                for (int k = 0; k < fNofTBins; ++k)
                {
                    CbmTBin& tBin = xBin[k];
            
                    //if (!tBin.Use())
                        //continue;
                    
                    for (std::list<CbmTBin::HitHolder>::iterator hi = tBin.HitsBegin(); hi != tBin.HitsEnd(); ++hi)
                    {
                        CbmTBin::HitHolder& hitHolder = *hi;
               
                        if (fStage > hitHolder.stage)
                            continue;
                  
                        handleHit(hitHolder);
                    }
                }
            }
        }
    }
    
    void SearchHits(const CbmTrackParam2& stateVec, Double_t stateZ, std::function<void(CbmTBin::HitHolder&)> handleHit);
    void SearchHits(Segment& segment, std::function<void(CbmTBin::HitHolder&)> handleHit);
    void SearchHits(Double_t minZ, Double_t maxZ, Double_t minY, Double_t maxY, Double_t minX, Double_t maxX, Double_t minT, Double_t maxT,
            std::function<void(CbmTBin::HitHolder&)> handleHit);
    
private:
    CbmBinned3DStation(const CbmBinned3DStation&) = delete;
    CbmBinned3DStation& operator=(const CbmBinned3DStation&) = delete;
    
private:
    CbmYBin* fYBins;
};

#endif /* STATION3D_H */
