/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Tracker.h
 * Author: tablyaz
 *
 * Created on August 4, 2017, 2:23 PM
 */

#ifndef TRACKER_H
#define TRACKER_H

#include "Station.h"
#include "Station3D.h"
#include <iostream>

//#define CBM_BINNED_DEBUG

#ifdef CBM_BINNED_DEBUG
#include "Debug.h"
#endif//CBM_BINNED_DEBUG

using namespace std;

class CbmBinnedTracker
{
public:
    int fNofTrueSegments;
    int fNofWrongSegments;
    struct Track
    {
        Track(CbmTBin::HitHolder** hits, int length, const CbmTrackParam2& lastParam, Double_t chiSq) :
            fHits(new CbmTBin::HitHolder*[length]), fLength(0), fParams(new CbmTrackParam2[length]), fIsClone(false), fChiSq(chiSq)
        {
            for (int i = 0; i < length; ++i)
            {
                if (ECbmModuleId::kRef == hits[i]->type)
                    continue;
                
                hits[i]->stage = CbmBinnedTracker::Instance()->GetStage();
                fHits[fLength] = hits[i];
                fHits[fLength++]->tracks.push_back(this);
            }
            
            fParams[fLength - 1] = lastParam;
            Double_t chiSq2 = chiSq;
            
            CbmBinnedTracker* tracker = CbmBinnedTracker::Instance();
                                
            for (int i = fLength - 2; i >= 0; --i)
            {
                fParams[i] = tracker->fStationArray[i + 1]->Extrapolate(fParams[i + 1], fHits[i]->hit->GetZ());
                CbmBinnedStation::Update(fParams[i], fHits[i]->hit, chiSq2);
            }
        }
        
        ~Track()
        {
            delete[] fHits;
            delete[] fParams;
        }
        
        Track(const Track&) = delete;
        Track& operator=(const Track&) = delete;
        CbmTBin::HitHolder** fHits;
        int fLength;
        //CbmBinnedStation::KFParams* fParams;
        CbmTrackParam2* fParams;
        bool fIsClone;
        Double_t fChiSq;
    };
    
public:
    static CbmBinnedTracker* Instance();
    
public:
    CbmBinnedTracker(Double_t beamDx, Double_t beamDy) : fNofTrueSegments(0), fNofWrongSegments(0), fStations(), fStationArray(),
            fNofStations(0), fBeforeLastLevel(0), fChiSqCut(0), fTracks(), fBeamDx(beamDx), fBeamDxSq(beamDx * beamDx), fBeamDy(beamDy), fBeamDySq(beamDy * beamDy),
            fVertex(), fVertexPseudoStation(0), fCanSkipHits(-1/* Indicates: calculate the default value*/), fStage(0)
#ifdef CBM_BINNED_DEBUG
    , fDebug()
#endif//CBM_BINNED_DEBUG
    {
        fVertex.SetX(0);
        fVertex.SetY(0);
        fVertex.SetZ(0);
        fVertex.SetDx(beamDx);
        fVertex.SetDy(beamDy);
        fVertex.SetDxy(0);
        fVertex.SetZ(0);
        fVertex.SetTime(0);
        fVertex.SetTimeError(1000000000000);
    }
    
    ~CbmBinnedTracker()
    {
        delete fVertexPseudoStation;
    }
    
    CbmBinnedTracker(const CbmBinnedTracker&) = delete;
    CbmBinnedTracker& operator=(const CbmBinnedTracker&) = delete;
    
    void Init()
    {
        fVertexPseudoStation = new CbmBinned3DStation(ECbmModuleId::kRef, -0.1, 0.1, 1, 1, 1);
        fVertexPseudoStation->SetMinY(-0.1);
        fVertexPseudoStation->SetMaxY(0.1);
        fVertexPseudoStation->SetMinX(-0.1);
        fVertexPseudoStation->SetMaxX(0.1);
        fVertexPseudoStation->Init();
        fVertexPseudoStation->AddHit(ECbmModuleId::kRef, &fVertex, -1);
        
        for (std::map<Double_t, CbmBinnedStation*>::const_iterator i = fStations.begin(); i != fStations.end(); ++i)
        {
            fStationArray.push_back(i->second);
            std::cout << "Station min and max Zs: " << i->second->GetMinZ() << ", " << i->second->GetMaxZ() << std::endl;
        }
        
        if (fCanSkipHits < 0)
            fCanSkipHits = 0.3 * fStationArray.size();
        
        fChiSqCut = (fStations.size() - fCanSkipHits) * cbmBinnedSigmaSq * 3;
    }
    
    Double_t GetBeamDxSq() const { return fBeamDxSq; }
    Double_t GetBeamDySq() const { return fBeamDySq; }
    CbmPixelHit& GetVertex() { return fVertex; }
    
    void AddStation(CbmBinnedStation* station)
    {
        if (fStations.empty())
            station->SetDefaultUse();
        
        fStations[station->GetMinZ()] = station;
        fNofStations = fStations.size();
        fBeforeLastLevel = fNofStations - 2;
    }
    
    void SetChiSqCut(Double_t v) { fChiSqCut = v; }
    
    char GetStage() const { return fStage; }
    
    void SetStage(char v)
    {
        fStage = v;
        fVertexPseudoStation->SetStage(v);
        
        for (std::vector<CbmBinnedStation*>::iterator i = fStationArray.begin(); i != fStationArray.end(); ++i)
        {
            CbmBinnedStation* aStation = *i;
            aStation->SetStage(v);
        }
    }
    
    void SetCanSkipHits(Int_t v) { fCanSkipHits = v; }
    
    void Reconstruct(Double_t startTime)
    {
        Clear();
        
        for (std::map<Double_t, CbmBinnedStation*>::iterator i = fStations.begin(); i != fStations.end(); ++i)
        {
            CbmBinnedStation* aStation = i->second;
            aStation->SetMinT(startTime);
        }
        
        CbmBinnedHitReader::Instance()->Read();        
        //ReconstructLocal();
        //ReconstructGlobal();
        
        for (int i = -1; i <= fCanSkipHits; ++i)
        {
            SetStage(i + 1);
            
            for (int j = 0; j <= (i == fCanSkipHits ? 0 : 1); ++j)
                SeedTracks(i, i + j + 1);
        }
        
        FindClones();
        
        std::cout << "Reconstructed " << count_if(fTracks.begin(), fTracks.end(), [](const Track* t) { return !t->fIsClone; }) << " tracks" << std::endl;
    }
    
    std::list<Track*>::const_iterator GetTracksBegin() const { return fTracks.begin(); }
    std::list<Track*>::const_iterator GetTracksEnd() const { return fTracks.end(); }
    
private:
    void Clear()
    {
        fVertexPseudoStation->SetStage(0);
        
        for (std::map<Double_t, CbmBinnedStation*>::iterator i = fStations.begin(); i != fStations.end(); ++i)
        {
            CbmBinnedStation* aStation = i->second;
            aStation->Clear();
        }
        
        for (std::list<Track*>::iterator i = fTracks.begin(); i != fTracks.end(); ++i)
            delete *i;
        
        fTracks.clear();
    }
    
    void ReconstructLocal()
    {
        std::map<Double_t, CbmBinnedStation*>::iterator i = fStations.begin();
        i->second->CreateSegmentsFromHits();
        
        for (; true;)
        {
            CbmBinnedStation* curStation = i->second;
            ++i;
            
            if (i == fStations.end())
                break;
            
            CbmBinnedStation* nextStation = i->second;
            curStation->IterateSegments(
                [&](CbmBinnedStation::Segment& segment)->void
                {
                    nextStation->SearchHits(segment,
                        [&](CbmTBin::HitHolder& /*hitHolder*/)->void
                        {                                                        
                        }
                    );
                }
            );
        }
        
#if 0//def CBM_BINNED_DEBUG
        static int nofRefTracks = 0;
        static int nofRecoRefHits[] = { 0, 0, 0, 0, 0, 0 };
        fDebug.Exec();
        int stNo = 0;
        CbmBinnedStation* firstStation = fStations.begin()->second;
        
        for (int i = 0; i < fDebug.fMCTracks.size(); ++i)
        {
            const CbmBinnedDebug::MCTrack& mcTrack = fDebug.fMCTracks[i];
            
            if (!mcTrack.isRef)
                continue;
            
            ++nofRefTracks;
            bool found[] = { false, false, false, false, false, false };
            firstStation->IterateSegments(
                [&](CbmBinnedStation::Segment& segment)->void
                {
                    CbmTBin::HitHolder* hitHolder = segment.end;
                    
                    if (fDebug.TrackHasHit(stNo, i, hitHolder->index))
                        found[stNo] = true;
                    
                    std::list<CbmBinnedStation::Segment*> tmp(segment.children);
                    
                    while(!tmp.empty())
                    {
                        ++stNo;
                        std::list<CbmBinnedStation::Segment*> tmp2;
                        
                        for (std::list<CbmBinnedStation::Segment*>::const_iterator j = tmp.begin(); j != tmp.end(); ++j)
                        {
                            const CbmBinnedStation::Segment* s = *j;
                            tmp2.insert(tmp2.end(), s->children.begin(), s->children.end());
                            CbmTBin::HitHolder* hh = s->end;
                            
                            if (fDebug.TrackHasHit(stNo, i, hh->index))
                                found[stNo] = true;
                        }
                        
                        tmp.clear();
                        tmp.splice(tmp.end(), tmp2);
                    }
                    
                    stNo = 0;
                }
            );
            
            std::cout << "Reco hits: ";
            
            for (int j = 0; j < sizeof(nofRecoRefHits) / sizeof(int); ++j)
            {
                if (found[j])
                    ++nofRecoRefHits[j];
                
                double proc = 100 * nofRecoRefHits[j];
                proc /= nofRefTracks;
                std::cout << "[" << proc << "%=" << nofRecoRefHits[j] << "/" << nofRefTracks << "]";
            }
            
            std::cout << std::endl << std::endl << std::endl;
        }
#endif//CBM_BINNED_DEBUG
    }
    
    Double_t GetChiSq(Double_t scatXSq, Double_t scatYSq, CbmBinnedStation::Segment& segment1, CbmBinnedStation::Segment& segment2) const
    {
        const CbmPixelHit* hit1 = segment1.begin->hit;
        const CbmPixelHit* hit2 = segment1.end->hit;
        
        if (hit2 != segment2.begin->hit)
            return cbmBinnedCrazyChiSq;
        
        /*Double_t chiSqCoeffX1 = stationNumber < 1 ? 1 : fStations[stationNumber - 1]->GetNofSigmaXSq() / cbmBinnedSigmaSq;
        Double_t chiSqCoeffY1 = stationNumber < 1 ? 1 : fStations[stationNumber - 1]->GetNofSigmaYSq() / cbmBinnedSigmaSq;
        Double_t chiSqCoeffX2 = fStations[stationNumber]->GetNofSigmaXSq() / cbmBinnedSigmaSq;
        Double_t chiSqCoeffY2 = fStations[stationNumber]->GetNofSigmaYSq() / cbmBinnedSigmaSq;
        Double_t chiSqCoeffX = fStations[stationNumber + 1]->GetNofSigmaXSq() / cbmBinnedSigmaSq;
        Double_t chiSqCoeffY = fStations[stationNumber + 1]->GetNofSigmaYSq() / cbmBinnedSigmaSq;*/
        
        const CbmPixelHit* hit = segment2.end->hit;
        Double_t x1 = hit1->GetX();
        Double_t y1 = hit1->GetY();
        Double_t z1 = hit1->GetZ();
//        Double_t t1 = hit1->GetTime(); (FU) not used
        Double_t dx1 = hit1->GetDx();
        Double_t dx1Sq = dx1 * dx1;
        Double_t dy1 = hit1->GetDy();
        Double_t dy1Sq = dy1 * dy1;
//        Double_t dt1 = hit1->GetTimeError(); (FU) not used
//        Double_t dt1Sq = dt1 * dt1; (FU) not used
        
        Double_t x2 = hit2->GetX();
        Double_t y2 = hit2->GetY();
        Double_t z2 = hit2->GetZ();
//        Double_t t2 = hit2->GetTime(); (FU) not used
        Double_t dx2 = hit2->GetDx();
        Double_t dx2Sq = dx2 * dx2;
        Double_t dy2 = hit2->GetDy();
        Double_t dy2Sq = dy2 * dy2;
//        Double_t dt2 = hit2->GetTimeError(); (FU) not used
//        Double_t dt2Sq = dt2 * dt2; (FU) not used
        
        Double_t x = hit->GetX();
        Double_t y = hit->GetY();
        Double_t z = hit->GetZ();
        //Double_t t = hit->GetTime(); (VF) unused
        Double_t dx = hit->GetDx();
        Double_t dxSq = dx * dx;
        Double_t dy = hit->GetDy();
        Double_t dySq = dy * dy;
        //Double_t dt = hit->GetTimeError(); (VF) unused
        //Double_t dtSq = dt * dt; (VF) unused
        
        
        /*dx1Sq *= chiSqCoeffX1;
        dy1Sq *= chiSqCoeffY1;
        dx2Sq *= chiSqCoeffX2;
        dy2Sq *= chiSqCoeffY2;
        dxSq *= chiSqCoeffX;
        dySq *= chiSqCoeffY;*/
        
        Double_t deltaZ12 = z2 - z1;
        Double_t deltaZ = z - (z1 + z2) / 2;
        Double_t coeff1 = (0.5 - deltaZ / deltaZ12);
        Double_t coeff1Sq = coeff1 * coeff1;
        Double_t coeff2 = (0.5 + deltaZ / deltaZ12);
        Double_t coeff2Sq = coeff2 * coeff2;
        
        Double_t x12 = coeff1 * x1 + coeff2 * x2;
        Double_t dx12Sq = coeff1Sq * dx1Sq + coeff2Sq * dx2Sq;
        Double_t y12 = coeff1 * y1 + coeff2 * y2;
        Double_t dy12Sq = coeff1Sq * dy1Sq + coeff2Sq * dy2Sq;
        
//        Double_t tx = (x2 - x1) / deltaZ12; (FU) not used
//        Double_t ty = (y2 - y1) / deltaZ12; (FU) not used

        /* (VF) not used.
        Double_t t12;
        Double_t dt12Sq;
        
        if (0 == dt1)
        {
            t12 = t2 + std::sqrt(1 + tx * tx + ty * ty) * (z - z2) / cbmBinnedSOL;
            dt12Sq = dt2Sq;
        }
        else
        {
            t12 = (t1 + t2) /2 + std::sqrt(1 + tx * tx + ty * ty) * deltaZ12 / cbmBinnedSOL;
            dt12Sq = (dt1Sq + dt2Sq) / 2;
        }
        */
        
        return (x - x12) * (x - x12) / (dx12Sq + dxSq + scatXSq)
                + (y - y12) * (y - y12) / (dy12Sq + dySq + scatYSq)
                //+ (t - t12) * (t - t12) / (dt12Sq + dtSq)
                ;
    }
    
    Double_t GetChiSqWithoutTime(CbmBinnedStation::Segment& segment1, CbmBinnedStation::Segment& segment2) const
    {
        const CbmPixelHit* hit1 = segment1.begin->hit;
        const CbmPixelHit* hit2 = segment1.end->hit;
        
        if (hit2 != segment2.begin->hit)
            return cbmBinnedCrazyChiSq;
        
        const CbmPixelHit* hit = segment2.end->hit;
        Double_t x1 = hit1->GetX();
        Double_t y1 = hit1->GetY();
        Double_t z1 = hit1->GetZ();
        //Double_t t1 = hit1->GetTime(); (VF) unused
        Double_t dx1 = hit1->GetDx();
        Double_t dx1Sq = dx1 * dx1;
        Double_t dy1 = hit1->GetDy();
        Double_t dy1Sq = dy1 * dy1;
        
        Double_t x2 = hit2->GetX();
        Double_t y2 = hit2->GetY();
        Double_t z2 = hit2->GetZ();
        //Double_t t2 = hit2->GetTime(); (VF) unused
        Double_t dx2 = hit2->GetDx();
        Double_t dx2Sq = dx2 * dx2;
        Double_t dy2 = hit2->GetDy();
        Double_t dy2Sq = dy2 * dy2;
        
        Double_t x = hit->GetX();
        Double_t y = hit->GetY();
        Double_t z = hit->GetZ();
        //Double_t t = hit->GetTime(); (VF) unused
        Double_t dx = hit->GetDx();
        Double_t dxSq = dx * dx;
        Double_t dy = hit->GetDy();
        Double_t dySq = dy * dy;
        
        Double_t deltaZ12 = z2 - z1;
        Double_t deltaZ = z - (z1 + z2) / 2;
        Double_t coeff1 = (0.5 - deltaZ / deltaZ12);
        Double_t coeff1Sq = coeff1 * coeff1;
        Double_t coeff2 = (0.5 + deltaZ / deltaZ12);
        Double_t coeff2Sq = coeff2 * coeff2;
        
        Double_t x12 = coeff1 * x1 + coeff2 * x2;
        Double_t dx12Sq = coeff1Sq * dx1Sq + coeff2Sq * dx2Sq;
        Double_t y12 = coeff1 * y1 + coeff2 * y2;
        Double_t dy12Sq = coeff1Sq * dy1Sq + coeff2Sq * dy2Sq;
        
        //Double_t tx = (x2 - x1) / deltaZ12; (VF) unused
        //Double_t ty = (y2 - y1) / deltaZ12; (VF) unused
        
        return (x - x12) * (x - x12) / (dx12Sq + dxSq) + (y - y12) * (y - y12) / (dy12Sq + dySq);
    }
    
    /*void ReconstructGlobal()
    {
        std::map<Double_t, CbmBinnedStation*>::reverse_iterator stationIter = fStations.rbegin();
        CbmBinnedStation* lastStation = (stationIter++)->second;
        lastStation->NulifySegments();
        
        int stN = 4;
        CbmBinnedStation* nextStation = lastStation;
        
        for (; stationIter != fStations.rend(); ++stationIter)
        {
            CbmBinnedStation* station = stationIter->second;
            station->IterateSegments(
            [&](CbmBinnedStation::Segment& segment)->void
            {
                CbmBinnedStation::Segment* closest = 0;
                Double_t clDist = 0;
                CbmBinnedStation::Segment* segChiSq = 0;
                Double_t minSegChiSq = 0;
                
                for (std::list<CbmBinnedStation::Segment*>::iterator i = segment.children.begin(); i != segment.children.end(); ++i)
                {
                    CbmBinnedStation::Segment* child = *i;
                    
                    if (child->chiSq > fChiSqCut)
                        continue;
                    
                    Double_t chiSq = GetChiSq(nextStation->GetScatXSq(), nextStation->GetScatYSq(), segment, *child) + child->chiSq;
                    
                    if (chiSq > fChiSqCut)
                        continue;
                    
                    if (segment.chiSq > chiSq)
                    {
                        segment.bestBranch = child;
                        segment.chiSq = chiSq;
                    }
                }
            }
            );
            --stN;
            nextStation = station;
        }
        
        fStations.begin()->second->IterateSegments(
            [&](CbmBinnedStation::Segment& segment)->void
            {
                if (segment.chiSq > fChiSqCut)
                    return;
                
                CbmTBin::HitHolder* trackHolders[fNofStations];
                CbmBinnedStation::Segment* segChain = &segment;
                int i = 0;
                
                while (0 != segChain)
                {
                    trackHolders[i++] = &segChain->end;
                    segChain = segChain->bestBranch;
                }
                
                Track* aCandidate = new Track(trackHolders, fNofStations, segment.chiSq);                
                fTracks.push_back(aCandidate);
            }
        );
    }*/
    /*void TraverseTrackCandidates(int level, CbmBinnedStation::Segment** trackStart, CbmTBin::HitHolder** hhs, Double_t chiSq, std::list<Track*>& candidates, Double_t scatXSqs[6], Double_t scatYSqs[6])
    {              
        CbmBinnedStation::Segment* segment = trackStart[level];

        for (std::list<CbmBinnedStation::Segment*>::iterator i = segment->children.begin(); i != segment->children.end(); ++i)
        {
            CbmBinnedStation::Segment* childSegment = *i;
            Double_t deltaChiSq = GetChiSq(scatXSqs[level + 1], scatYSqs[level + 1], *segment, *childSegment);
            Double_t chiSq2 = chiSq + deltaChiSq;            
            trackStart[level + 1] = childSegment;
            hhs[level + 1] = childSegment->end;

            if (level == 4)
            {
                Track* aCandidate = new Track(hhs, 6, chiSq2);                
                candidates.push_back(aCandidate);
            }
            else
                TraverseTrackCandidates(level + 1, trackStart, hhs, chiSq2, candidates, scatXSqs, scatYSqs);
        }
    }*/
    
    //KFStation fKFStations[6];

    /*void KFAddPointCoord(int stationNumber, CbmBinnedStation::KFParamsCoord& param, const CbmBinnedStation::KFParamsCoord& prevParam,
        Double_t m, Double_t V, Double_t& chi2, Double_t z, Double_t prevZ, int coordNumber)
    {
        //const KFStation& station = fKFStations[stationNumber];
        const CbmBinnedStation* station = fStationArray[stationNumber];
        CbmBinnedStation::KFStation::Q Q = { 0 == coordNumber ? station->GetScatXSq() : station->GetScatYSq(), 0, 0, 0 };
        Double_t deltaZ = z - prevZ;
        Double_t deltaZSq = deltaZ * deltaZ;

        // Extrapolate.
        param.coord += prevParam.tg * deltaZ; // params[k].tg is unchanged.

        // Filter.
        param.C11 += prevParam.C12 * deltaZ + prevParam.C21 * deltaZ + prevParam.C22 * deltaZSq + Q.Q11;
        param.C12 += prevParam.C22 * deltaZ + Q.Q12;
        param.C21 += prevParam.C22 * deltaZ + Q.Q21;
        param.C22 += Q.Q22;

        Double_t S = 1.0 / (V + param.C11);
        Double_t Kcoord = param.C11 * S;
        Double_t Ktg = param.C21 * S;
        Double_t dzeta = m - param.coord;
        param.coord += Kcoord * dzeta;
        param.tg += Ktg * dzeta;
        param.C21 -= param.C11 * Ktg;
        param.C22 -= param.C12 * Ktg;
        param.C11 *= 1.0 - Kcoord;
        param.C12 *= 1.0 - Kcoord;
        chi2 += dzeta * S * dzeta;
    }

    void KFAddPoint(int stationNumber, CbmBinnedStation::KFParams& param, const CbmBinnedStation::KFParams& prevParam, Double_t m[2], Double_t V[2], Double_t z, Double_t prevZ)
    {
        KFAddPointCoord(stationNumber, param.xParams, prevParam.xParams, m[0], V[0], param.chi2, z, prevZ, 0);
        KFAddPointCoord(stationNumber, param.yParams, prevParam.yParams, m[1], V[1], param.chi2, z, prevZ, 1);
    }*/
    
    void TraverseTrackCandidates(int level, CbmBinnedStation::Segment** trackStart, CbmTBin::HitHolder** hhs, ECbmModuleId* hts, std::list<Track*>& candidates,
      CbmTrackParam2 kfParamsPrev, Double_t chiSqPrev)
    {              
        CbmBinnedStation::Segment* segment = trackStart[level];
        const CbmPixelHit* hit = segment->end->hit;
        CbmTrackParam2 kfParams = fStationArray[level]->Extrapolate(kfParamsPrev, hit->GetZ());
        Double_t chiSq = chiSqPrev;
        CbmBinnedStation::Update(kfParams, hit, chiSq);
        
        //CbmBinnedSettings* settings = CbmBinnedSettings::Instance(); (VF) unused
        
        if (level == fNofStations - 1)
        {
            Track* aCandidate = new Track(hhs, fNofStations, kfParams, chiSq);
            candidates.push_back(aCandidate);
            return;
        }

        for (std::list<CbmBinnedStation::Segment*>::iterator i = segment->children.begin(); i != segment->children.end(); ++i)
        {
            CbmBinnedStation::Segment* childSegment = *i;         
            trackStart[level + 1] = childSegment;
            hhs[level + 1] = childSegment->end;
            TraverseTrackCandidates(level + 1, trackStart, hhs, hts, candidates, kfParams, chiSq);
        }
    }

    void ReconstructGlobal()
    {
        std::map<Double_t, CbmBinnedStation*>::iterator startStationIter = fStations.begin();        
        CbmBinnedStation* startStation = startStationIter->second;
        CbmBinnedStation* secondStation = fStationArray[1];
        startStation->IterateSegments(
            [&](CbmBinnedStation::Segment& segment)->void
            {
                CbmBinnedStation::Segment* segments[fNofStations];
                segments[0] = &segment;
                CbmTBin::HitHolder* trackHolders[fNofStations];
                ECbmModuleId hitTypes[fNofStations];
                trackHolders[0] = segment.end;
                hitTypes[0] = secondStation->GetStationType();
                std::list<Track*> candidates;
                
                //TraverseTrackCandidates(0, segments, trackHolders, 0, candidates, scatXSqs, scatYSqs);
                const CbmPixelHit* p1 = segment.begin->hit;
                //const CbmPixelHit* p2 = segment.end->hit; (VF) unused
                CbmTrackParam2 kfParams;
                kfParams.SetX(p1->GetX());
                kfParams.SetTx(0);
                kfParams.SetY(p1->GetY());
                kfParams.SetTy(0);
                //kfParams.SetQp(1);
                kfParams.SetZ(p1->GetZ());
                kfParams.SetTime(p1->GetTime());
                /*Double_t C[21];
                std::fill_n(C, 21, 0);
                C[0] = p1->GetDx() * p1->GetDx();// Cxx
                C[6] = p1->GetDy() * p1->GetDy();// Cyy
                C[11] = 1;// Ctxtx
                C[15] = 1;// Ctyty
                C[18] = 1;// Cqpqp
                C[20] = p1->GetTimeError() * p1->GetTimeError();// Ctt
                kfParams.SetCovMatrix(C);*/
                kfParams.SetCovXX(p1->GetDx() * p1->GetDx());
                kfParams.SetCovYY(p1->GetDy() * p1->GetDy());
                kfParams.SetCovTxTx(1);
                kfParams.SetCovTyTy(1);
                kfParams.SetCovTT(p1->GetTimeError() * p1->GetTimeError());
                Double_t chiSq = 0;
                TraverseTrackCandidates(0, segments, trackHolders, hitTypes, candidates, kfParams, chiSq);
                Track* bestCandidate = 0;

                for (std::list<Track*>::iterator i = candidates.begin(); i != candidates.end(); ++i)
                {
                    Track* aCandidate = *i;

                    if (0 == bestCandidate || aCandidate->fChiSq < bestCandidate->fChiSq)
                    {
                        delete bestCandidate;
                        bestCandidate = aCandidate;
                    }
                    else
                        delete aCandidate;
                }

                if (0 != bestCandidate)
                    fTracks.push_back(bestCandidate);
            }
        );
        
#ifdef CBM_BINNED_DEBUG
        static int nofRefTracks = 0;
        static int nofRecoRefHits[] = { 0, 0, 0, 0, 0, 0 };
        fDebug.Exec();
        CbmBinnedStation* firstStation = fStations.begin()->second;
        
        for (int i = 0; i < fDebug.fMCTracks.size(); ++i)
        {
            const CbmBinnedDebug::MCTrack& mcTrack = fDebug.fMCTracks[i];
            
            if (!mcTrack.isRef)
                continue;
            
            for (std::list<Track*>::const_iterator j = fTracks.begin(); j != fTracks.end(); ++j)
            {
                const Track* recoTrack = *j;
                const CbmTBin::HitHolder* firstHit = recoTrack->fHits[0];
                
                if (!fDebug.TrackHasHit(0, i, firstHit->index))
                    continue;
                
                ++nofRecoRefHits[0];
                ++nofRefTracks;
                
                for (int k = 1; k < 6; ++k)
                {
                    if (fDebug.TrackHasHit(k, i, recoTrack->fHits[k]->index))
                        ++nofRecoRefHits[k];
                }
            }
        }
        
        std::cout << "Reco hits: ";

        for (int j = 0; j < sizeof (nofRecoRefHits) / sizeof (int); ++j)
        {
            double proc = 100 * nofRecoRefHits[j];
            proc /= nofRefTracks;
            std::cout << "[" << proc << "%=" << nofRecoRefHits[j] << "/" << nofRefTracks << "]";
        }

        std::cout << std::endl << std::endl << std::endl;
#endif//CBM_BINNED_DEBUG
    }
    
    void SeedTracks(int leftStationNo, int rightStationNo)
    {
        int skippedHits = rightStationNo - leftStationNo - 1;
        CbmBinnedStation* leftStation = 0 > leftStationNo ? fVertexPseudoStation : fStationArray[leftStationNo];
        leftStation->IterateHits(
            [this, &leftStationNo, &rightStationNo, &skippedHits](CbmTBin::HitHolder& leftHitHolder)->void
            {
#ifdef __MACH__
                CbmTBin::HitHolder* trackHoldersBuf[fNofStations + 1];
                CbmTBin::HitHolder** trackHolders = trackHoldersBuf;
                CbmTBin::HitHolder* tmpTrackHoldersBuf[fNofStations + 1];
                CbmTBin::HitHolder** tmpTrackHolders = tmpTrackHoldersBuf;
#else //__MACH__
                CbmTBin::HitHolder* trackHolders[fNofStations + 1];
                CbmTBin::HitHolder* tmpTrackHolders[fNofStations + 1];
#endif//__MACH__
                tmpTrackHolders[0] = &leftHitHolder;
                const CbmPixelHit* leftHit = leftHitHolder.hit;
                CbmTrackParam2 kfParams;
                kfParams.SetX(leftHit->GetX());
                kfParams.SetTx(0);
                kfParams.SetY(leftHit->GetY());
                kfParams.SetTy(0);
                kfParams.SetZ(leftHit->GetZ());
                kfParams.SetTime(leftHit->GetTime());
                kfParams.SetCovXX(leftHit->GetDx() * leftHit->GetDx());
                kfParams.SetCovYY(leftHit->GetDy() * leftHit->GetDy());
                kfParams.SetCovTxTx(1);
                kfParams.SetCovTyTy(1);
                kfParams.SetCovTT(leftHit->GetTimeError() * leftHit->GetTimeError());
                Double_t chiSq = 0;
                int bestLength = 2;
                CbmTrackParam2 bestKfParams = kfParams;
                Double_t bestChiSq = cbmBinnedCrazyChiSq;
                CbmBinnedStation* rightStation = fStationArray[rightStationNo];
                rightStation->IterateHits(
                    [this, &leftStationNo, &rightStationNo, &skippedHits, &trackHolders, &tmpTrackHolders, &kfParams, &chiSq,
                        &bestLength, &bestKfParams, &bestChiSq, &rightStation](CbmTBin::HitHolder& rightHitHolder)->void
                    {
                        tmpTrackHolders[1] = &rightHitHolder;
                        const CbmPixelHit* rightHit = rightHitHolder.hit;
                        CbmTrackParam2 updKfParams = rightStation->Extrapolate(kfParams, rightHit->GetZ());
                        Double_t updChiSq = chiSq;
                        CbmBinnedStation::Update(updKfParams, rightHit, updChiSq);

                        if (updChiSq > fChiSqCut)
                            return;
                        
                        if (0 > leftStationNo)// Have to reset parameters here
                        {
                            bestChiSq = cbmBinnedCrazyChiSq;
                            bestLength = 2;
                            bestKfParams = kfParams;
                        }
                        
                        FollowTracks(0 > leftStationNo, skippedHits, rightStationNo, 3, tmpTrackHolders, updKfParams, updChiSq, bestLength, trackHolders, bestKfParams, bestChiSq);
                        
                        if (0 > leftStationNo)// Have to save track here
                        {
                            if (bestLength - 1 >= fNofStations - fCanSkipHits)
                            {
                                Track* aTrack = new Track(trackHolders, bestLength, bestKfParams, bestChiSq);
                                fTracks.push_back(aTrack);
                            }
                        }
                    }// Lambda
                );// rightStation->IterateHits()
                
                if (0 <= leftStationNo)// Have to save track here
                {
                    if (bestLength >= fNofStations - fCanSkipHits)
                    {
                        Track* aTrack = new Track(trackHolders, bestLength, bestKfParams, bestChiSq);
                        fTracks.push_back(aTrack);
                    }
                }
            }
        );
    }
    
    void FollowTracks(bool isPrimary, int skippedHits, int prevStationNo,
        int length, CbmTBin::HitHolder** trackHolders, CbmTrackParam2 kfParams, Double_t chiSq,
        int& bestLength, CbmTBin::HitHolder** bestTrackHolders, CbmTrackParam2& bestKfParams, Double_t& bestChiSq)
    {
        Double_t prevZ = trackHolders[length - 2]->hit->GetZ();
        
        for (char i = 0; i <= (prevStationNo < fNofStations - 2 ? 1 : 0) && skippedHits + i <= fCanSkipHits; ++i)
        {
            Int_t stationNo = prevStationNo + i + 1;
            CbmBinnedStation* aStation = fStationArray[stationNo];
            bool save = true;
            aStation->SearchHits(kfParams, prevZ,
                [this, &isPrimary, &skippedHits, &length, &trackHolders, &kfParams, &chiSq,
                    &bestLength, &bestTrackHolders, &bestKfParams, &bestChiSq, &i, &stationNo, &aStation, &save](CbmTBin::HitHolder& hitHolder)->void
                {
                    trackHolders[length - 1] = &hitHolder;
                    const CbmPixelHit* hit = hitHolder.hit;
                    CbmTrackParam2 updKfParams = aStation->Extrapolate(kfParams, hit->GetZ());                    
                    Double_t updChiSq = chiSq;
                    CbmBinnedStation::Update(updKfParams, hit, updChiSq);
                    
                    Double_t chiSqCut = fChiSqCut;
                    
                    if (length - (isPrimary ? 1 : 0) > fNofStations - fCanSkipHits)
                        chiSqCut += (length - (isPrimary ? 1 : 0) - fNofStations + fCanSkipHits) * cbmBinnedSigmaSq * 3;
                
                    if (updChiSq > chiSqCut || updChiSq > bestChiSq)
                        return;
                    else
                    {
                        save = false;
                        
                        if (stationNo < fNofStations - 1)
                        {
                            FollowTracks(isPrimary, skippedHits + i, stationNo, length + 1, trackHolders, updKfParams, updChiSq,
                                bestLength, bestTrackHolders, bestKfParams, bestChiSq);
                        }
                        else
                        {
                            if (length - (isPrimary ? 1 : 0) >= fNofStations - fCanSkipHits && (length > bestLength || (length == bestLength && updChiSq < bestChiSq)))
                            {
                                for (int j = 0; j < length; ++j)
                                    bestTrackHolders[j] = trackHolders[j];
                        
                                bestKfParams = updKfParams;
                                bestLength = length;
                                bestChiSq = chiSq;
                            }
                        }
                    }
                }// Lambda
            );// SearchHits
            
            if (save)
            {
                --length;
                
                if (length - (isPrimary ? 1 : 0) >= fNofStations - fCanSkipHits && (length > bestLength || (length == bestLength && chiSq < bestChiSq)))
                {
                    for (int j = 0; j < length; ++j)
                        bestTrackHolders[j] = trackHolders[j];
                        
                    bestKfParams = kfParams;
                    bestLength = length;
                    bestChiSq = chiSq;
                }
            }
        }
    }
    
    void FindClones()
    {
        for (std::list<Track*>::iterator i = fTracks.begin(); i != fTracks.end(); ++i)
        {
            Track* track = *i;
            
            if (track->fIsClone)
                continue;
            
            std::map<Track*, int> cloneNofs;
            
            for (int j = 0; j < track->fLength; ++j)
            {
                CbmTBin::HitHolder* hit = track->fHits[j];
                std::set<Track*> neighbourTracks;
                
                for (std::list<void*>::iterator k = hit->tracks.begin(); k != hit->tracks.end(); ++k)
                {
                    Track* track2 = static_cast<Track*> (*k);
                    
                    if (track2 == track || track2->fIsClone)
                        continue;
                    
                    neighbourTracks.insert(track2);
                }
                
                for (std::set<Track*>::iterator k = neighbourTracks.begin(); k != neighbourTracks.end(); ++k)
                {
                    Track* track2 = *k;
                    std::map<Track*, int>::iterator cni = cloneNofs.find(track2);
                    
                    if (cni == cloneNofs.end())
                        cloneNofs[track2] = 1;
                    else
                        ++cni->second;
                }
            }
            
            for (std::map<Track*, int>::iterator j = cloneNofs.begin(); j != cloneNofs.end(); ++j)
            {
                Track* track2 = j->first;
                auto minLength = track->fLength < track2->fLength ? track->fLength : track2->fLength;
                
                if (j->second < int(0.7 * minLength))
                    continue;
                
                if (track2->fLength > track->fLength)
                {
                    track->fIsClone = true;
                    break;
                }
                else if (track2->fLength == track->fLength)
                {
                    if (track2->fChiSq < track->fChiSq)
                    {
                        track->fIsClone = true;
                        break;
                    }
                    else
                        track2->fIsClone = true;
                }
                else
                    track2->fIsClone = true;
                
            }
        }
    }
    
private:
    std::map<Double_t, CbmBinnedStation*> fStations;
    std::vector<CbmBinnedStation*> fStationArray;
    int fNofStations;
    int fBeforeLastLevel;
    Double_t fChiSqCut;
    std::list<Track*> fTracks;
    Double_t fBeamDx;
    Double_t fBeamDxSq;
    Double_t fBeamDy;
    Double_t fBeamDySq;
    CbmPixelHit fVertex;
    CbmBinnedStation* fVertexPseudoStation;
    Int_t fCanSkipHits;
    char fStage;
#ifdef CBM_BINNED_DEBUG
    CbmBinnedDebug fDebug;
#endif
    
    friend class CbmBinnedTrackerQA;
    friend struct CbmBinnedTracker::Track;
};

#endif /* TRACKER_H */
