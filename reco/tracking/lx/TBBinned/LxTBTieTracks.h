/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LxTBTieTracks.h
 * Author: tablyaz
 *
 * Created on September 1, 2016, 11:33 AM
 */

#ifndef LXTBTIETRACKS_H
#define LXTBTIETRACKS_H

#include "LxTBDefinitions.h"
#include "LxTBBinned.h"
#include "FairTrackParam.h"

class TClonesArray;

struct LxTBBinndedLayer
{
    scaltype z;
    int nofXBins;
    int nofYBins;
    int nofTBins;
    int lastXBin;
    int lastYBin;
    int lastTBin;
    scaltype minX;
    scaltype maxX;
    scaltype binSizeX;
    scaltype minY;
    scaltype maxY;
    scaltype binSizeY;
    timetype minT;
    timetype maxT;
    int binSizeT;
    int timeSliceLength;
    scaltype maxDx;
    scaltype maxDy;
    timetype maxDt;
    LxTbTYXBin* tyxBins;
    
    LxTBBinndedLayer(int nofxb, int nofyb, int noftb, int bst) 
      : z(), nofXBins(nofxb), nofYBins(nofyb), nofTBins(noftb), lastXBin(nofxb - 1), lastYBin(nofyb - 1), lastTBin(noftb - 1),
        minX(), maxX(), binSizeX(), minY(), maxY(), binSizeY(),
        minT(0), maxT(0), binSizeT(bst), timeSliceLength(nofTBins * binSizeT), maxDx(0), maxDy(0), maxDt(0),
        tyxBins(reinterpret_cast<LxTbTYXBin*> (new unsigned char[noftb * sizeof(LxTbTYXBin)]))
    {
        for (int i = 0; i < noftb; ++i)
            new (&tyxBins[i]) LxTbTYXBin(nofXBins, nofYBins);
    }
    LxTBBinndedLayer(const LxTBBinndedLayer&) = delete;
    LxTBBinndedLayer& operator=(const LxTBBinndedLayer&) = delete;
    
    ~LxTBBinndedLayer()
    {
        delete[] reinterpret_cast<unsigned char*> (tyxBins);
    }
    
    void Clear()
    {
        for (int i = 0; i < nofTBins; ++i)
            tyxBins[i].Clear();
    }
    
    void Init()
    {
        binSizeX = (maxX - minX) / nofXBins;
        binSizeY = (maxY - minY) / nofYBins;
    }
    
    void SetTSBegin(unsigned long long tsLowBound)
    {
        minT = tsLowBound;
        maxT = tsLowBound + timeSliceLength;
    }
    
    struct PointHandler
    {
        virtual ~PointHandler() {}
        virtual void HandlePoint(const LxTbBinnedPoint& point) = 0;
    };
    
    void Search(scaltype x, scaltype xDiv0, scaltype y, scaltype yDiv0, timetype t, PointHandler* pointHandler)
    {
        scaltype wX = NOF_SIGMAS * std::sqrt(xDiv0 + maxDx * maxDx);
        scaltype wY = NOF_SIGMAS * std::sqrt(yDiv0 + maxDy * maxDy);
        timetype wT = NOF_SIGMAS * std::sqrt(2.0) * maxDt;

        if (x + wX < minX || x - wX > maxX || y + wY < minY || y - wY > maxY || t + wT < minT || t - wT > maxT)
            return;

        int tIndMin = (t - wT - minT) / binSizeT;

        if (tIndMin < 0)
            tIndMin = 0;
        else if (tIndMin > lastTBin)
            tIndMin = lastTBin;

        int tIndMax = (t + wT - minT) / binSizeT;

        if (tIndMax < 0)
            tIndMax = 0;
        else if (tIndMax > lastTBin)
            tIndMax = lastTBin;

        int yIndMin = (y - wY - minY) / binSizeY;

        if (yIndMin < 0)
            yIndMin = 0;
        else if (yIndMin > lastYBin)
            yIndMin = lastYBin;

        int yIndMax = (y + wY - minY) / binSizeY;

        if (yIndMax < 0)
            yIndMax = 0;
        else if (yIndMax > lastYBin)
            yIndMax = lastYBin;

        int xIndMin = (x - wX - minX) / binSizeX;

        if (xIndMin < 0)
            xIndMin = 0;
        else if (xIndMin > lastXBin)
            xIndMin = lastXBin;

        int xIndMax = (x + wX - minX) / binSizeX;

        if (xIndMax < 0)
            xIndMax = 0;
        else if (xIndMax > lastXBin)
            xIndMax = lastXBin;

        for (int tInd = tIndMin; tInd <= tIndMax; ++tInd)
        {
            LxTbTYXBin& tyxBin = tyxBins[tInd];

            for (int yInd = yIndMin; yInd <= yIndMax; ++yInd)
            {
                LxTbYXBin& yxBin = tyxBin.yxBins[yInd];

                for (int xInd = xIndMin; xInd <= xIndMax; ++xInd)
                {
                    LxTbXBin& xBin = yxBin.xBins[xInd];

                    for (std::list<LxTbBinnedPoint>::iterator i = xBin.points.begin(); i != xBin.points.end(); ++i)
                    {
                        LxTbBinnedPoint& point = *i;
                        scaltype xDiv = xDiv0 + point.dx * point.dx;
                        scaltype wX_prec_sq = NOF_SIGMAS_SQ * xDiv;
                        scaltype yDiv = yDiv0 + point.dy * point.dy;
                        scaltype wY_prec_sq = NOF_SIGMAS_SQ * yDiv;
                        scaltype deltaX = point.x - x;
                        scaltype deltaXSq = deltaX * deltaX;
                        scaltype deltaY = point.y - y;
                        scaltype deltaYSq = deltaY * deltaY;
                        timetype deltaT = point.t - t;
                        timetype deltaTSq = deltaT * deltaT;

                        if (deltaXSq < wX_prec_sq && deltaYSq < wY_prec_sq && deltaTSq < wT * wT)
                        //{
                            //rPoint.neighbours.push_back(LxTbBinnedRay(deltaZ, rPoint, lPoint, deltaXSq / xDiv +
                                    //deltaYSq / yDiv + deltaTSq / (rPoint.dt * rPoint.dt + lPoint.dt * lPoint.dt)));
                            pointHandler->HandlePoint(point);
                        //}
                    }
                }
            }
        }
    }
};

struct LxTBBinnedStsTrack
{
    FairTrackParam fPar;
    Double_t fChiSq;
    Double_t fTime;
    Int_t fSelfId;
    //Int_t fEventId;
    //Int_t fFileId;
};

struct LxTBBinnedDetector
{
    LxTBBinndedLayer* fLayers;
    int fNofLayers;
    std::list<LxTBBinnedStsTrack> fStsTracks;
    TClonesArray* fMuchTracks;
    TClonesArray* fGlobalTracks;
    
    LxTBBinnedDetector(int nofl, int nofxb, int nofyb, int noftb, int binSizeT);
    LxTBBinnedDetector(const LxTBBinnedDetector&) = delete;
    LxTBBinnedDetector& operator=(const LxTBBinnedDetector&) = delete;

    void Init();
    void Clear();
    void SetTSBegin(unsigned long long tsLowBound);
    void AddStsTrack(const FairTrackParam& par, Double_t chiSq, Double_t time, Int_t selfId/*, Int_t eventId, Int_t fileId*/);
    void TieTracks(LxTbBinnedFinder& fFinder);

};

#endif /* LXTBTIETRACKS_H */

