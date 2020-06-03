/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Station.h
 * Author: tablyaz
 *
 * Created on August 7, 2017, 2:05 PM
 */

#ifndef STATION_H
#define STATION_H

#include "Bins.h"
#include <functional>
#include "HitReader.h"
#include <set>
#include <iostream>
#include "CbmTrackParam2.h"

const Double_t cbmBinnedSigma = 4;
const Double_t cbmBinnedSigmaSq = cbmBinnedSigma * cbmBinnedSigma;
extern Double_t cbmBinnedSOL;
const Double_t cbmBinnedCrazyChiSq = 1000000;

class CbmBinnedStation
{
public:
    struct Segment
    {
        CbmTBin::HitHolder* begin;
        CbmTBin::HitHolder* end;
        Double_t chiSq;
        std::list<Segment*> children;
        Segment* bestBranch;
        
        Segment(CbmTBin::HitHolder* beginHit, CbmTBin::HitHolder* endHit) : begin(beginHit), end(endHit), chiSq(cbmBinnedCrazyChiSq), children(), bestBranch(0) {}
        Segment(const Segment&) = default;
        Segment& operator=(const Segment&) = default;
    };
    
    struct SegmentComp
    {
        bool operator()(const Segment& s1, const Segment& s2) const
        {
            if (&s1.begin < &s2.begin)
                return true;
            else if (&s1.end < &s2.end)
                return true;
            else
                return false;
        }
    };
    
    /*struct KFParamsCoord
    {
        Double_t coord, tg, C11, C12, C21, C22;
    };

    struct KFParams
    {
        KFParamsCoord xParams;
        KFParamsCoord yParams;
        Double_t chi2;
    };*/
    
    struct KFStation
    {
        struct Q
        {
            Double_t Q11, Q12, Q21, Q22;
        };
        
        Q qs[2];
    };
    
public:
    /*static KFParams Extrapolate(const KFParams& params, Double_t deltaZ)
    {
        KFParams result = params;
        
        result.xParams.coord += params.xParams.tg * deltaZ;
        result.xParams.C12 += params.xParams.C22 * deltaZ;
        result.xParams.C11 += params.xParams.C12 * deltaZ + result.xParams.C12 * deltaZ;
        
        result.yParams.coord += params.yParams.tg * deltaZ;
        result.yParams.C12 += params.yParams.C22 * deltaZ;
        result.yParams.C11 += params.yParams.C12 * deltaZ + result.yParams.C12 * deltaZ;
        
        return result;
    }*/
    
    CbmTrackParam2 Extrapolate(const CbmTrackParam2& parIn, Double_t zOut)
    {
        CbmTrackParam2 parOut;
        parOut.SetZ(zOut);
        Double_t dz = zOut - parIn.GetZ();
        Double_t tx = parIn.GetTx();
        Double_t ty = parIn.GetTy();
        Double_t timeCoeff = dz / std::sqrt(1 + tx * tx + ty * ty) / cbmBinnedSOL;
        
        parOut.SetX(parIn.GetX() + tx * dz);
        parOut.SetY(parIn.GetY() + ty * dz);
        parOut.SetTx(parIn.GetTx());
        parOut.SetTy(parIn.GetTy());
        parOut.SetTime(parIn.GetTime() + tx * timeCoeff + ty * timeCoeff);

        Double_t covIn[15];
        parIn.CovMatrix(covIn);
        
        Double_t covOut[15];
        
        Double_t t1 = covIn[2];
        Double_t t4 = covIn[9];
        Double_t t6 = dz * t4 + t1;
        covOut[0] = dz * t1 + t6 * dz + covIn[0] + fScatXSq;
        Double_t t8 = covIn[6];
        Double_t t11 = covIn[10];
        Double_t t12 = dz * t11;
        Double_t t14 = t12 + covIn[3];
        Double_t covOut2 = t6;
        Double_t covOut3 = t14;
        Double_t t18 = covIn[11];
        covOut[1] = t14 * dz + dz * t8 + covIn[1];
        Double_t t21 = covIn[7];
        Double_t t24 = covIn[12];
        Double_t t26 = dz * t24 + t21;
        covOut[5] = dz * t21 + t26 * dz + covIn[5] + fScatYSq;
        Double_t covOut5 = t12 + t8;
        Double_t covOut6 = t26;
        Double_t t30 = covIn[13];
        covOut[2] = covOut2;
        covOut[6] = covOut5;
        covOut[9] = t4;
        Double_t covOut8 = t11;
        Double_t t34 = timeCoeff * covOut8;
        covOut[3] = covOut3;
        covOut[7] = covOut6;
        covOut[10] = covOut8;
        covOut[12] = t24;
        covOut[4] = covOut2 * timeCoeff + covOut3 * timeCoeff + dz * t18 + covIn[4];
        covOut[8] = covOut5 * timeCoeff + covOut6 * timeCoeff + dz * t30 + covIn[8];
        covOut[11] = timeCoeff * covOut[9] + t18 + t34;
        covOut[13] = timeCoeff * covOut[12] + t30 + t34;
        covOut[14] = timeCoeff * t18 + timeCoeff * t30 + covOut[11] * timeCoeff + covOut[13] * timeCoeff + covIn[14];

        parOut.SetCovMatrix(covOut);
   
        return parOut;
    }
    /*static CbmTrackParam2 Extrapolate(const CbmTrackParam2& parIn, Double_t zOut)
    {
        CbmTrackParam2 parOut = parIn;
        
        Double_t dz = zOut - parIn.GetZ();
        Double_t timeCoeff = dz / std::sqrt(1 + parIn.GetTx() * parIn.GetTx() + parIn.GetTy() * parIn.GetTy()) / cbmBinnedSOL;
        
        Double_t F_ini[5][5] = {
            { 1, 0, dz, 0, 0 },
            { 0, 1, 0, dz, 0 },
            { 0, 0, 1, 0, 0 },
            { 0, 0, 0, 1, 0 },
            { 0, 0, timeCoeff, timeCoeff, 1 }
        };
        
        TMatrixD F(5, 5);
        
        for (int i = 0; i < 5; ++i)
        {
            for (int j = 0; j < 5; ++j)
                F(i, j) = F_ini[i][j];
        }
        
        TMatrixD& X = parOut.GetVector();
        TMatrixD Xtmp = X;
        X.Mult(F, Xtmp);
        
        TMatrixD F_t(5, 5);
        F_t.Transpose(F);
        
        TMatrixD& P = parOut.GetCovMatrix();
        TMatrixD PF_t(5, 5);
        PF_t.Mult(P, F_t);
        P.Mult(F, PF_t);
        
        parOut.SetZ(zOut);
        
        return parOut;
    }*/
    
    static void Update(CbmTrackParam2& par, const CbmPixelHit* hit, Double_t& chiSq)
    {
        Double_t xIn = par.GetX();
        Double_t yIn = par.GetY();
        Double_t txIn = par.GetTx();
        Double_t tyIn = par.GetTy();
        Double_t timeIn = par.GetTime();
        
        Double_t covIn[15];
        par.CovMatrix(covIn);
        
        Double_t xMes = hit->GetX();
        Double_t yMes = hit->GetY();
        Double_t timeMes = hit->GetTime();
        
        Double_t dx = hit->GetDx();
        Double_t dy = hit->GetDy();
        Double_t dxy = hit->GetDxy();
        Double_t dt = hit->GetTimeError();
        
        Double_t xOut;
        Double_t yOut;
        Double_t txOut;
        Double_t tyOut;
        Double_t timeOut;
        Double_t covOut[15];

        Double_t t1 = covIn[0];
        Double_t t2 = dt * dt;
        Double_t t3 = dy * dy;
        Double_t t4 = t2 * t3;
        Double_t t5 = covIn[5];
        Double_t t7 = covIn[14];
        Double_t t10 = covIn[8];
        Double_t t11 = t10 * t10;
        Double_t t12 = t2 * t5 + t3 * t7 + t5 * t7 - t11 + t4;
        Double_t t14 = dx * dx;
        Double_t t15 = t2 * t14;
        Double_t t19 = t14 * t3;
        Double_t t21 = dxy * dxy;
        Double_t t23 = t2 * dxy;
        Double_t t24 = covIn[1];
        Double_t t27 = t2 * t1;
        Double_t t29 = t24 * t24;
        Double_t t31 = t14 * t5;
        Double_t t34 = t3 * t1;
        Double_t t36 = covIn[4];
        Double_t t37 = t36 * t36;
        Double_t t40 = dxy * t24;
        Double_t t43 = dxy * t36;
        Double_t t46 = t1 * t5;
        Double_t t50 = t24 * t36;
        Double_t t54 = -t1 * t11 + t4 * t1 + 2 * t43 * t10 + 2 * t50 * t10 - t14 * t11 + t15 * t3 + t15 * t5 + t19 * t7 - t2 * t21 - t2 * t29 - t21 * t7 - 2 * t23 * t24 + t27 * t5 - t29 * t7 - t3 * t37 + t31 * t7 + t34 * t7 - t37 * t5 - 2 * t40 * t7 + t46 * t7;
        Double_t t55 = 1 / t54;
        Double_t t61 = dxy * t7 - t36 * t10 + t2 * t24 + t24 * t7 + t23;
        Double_t t63 = t24 * t61 * t55;
        Double_t t68 = dxy * t10 + t24 * t10 - t3 * t36 - t36 * t5;
        Double_t t70 = t36 * t68 * t55;
        Double_t t71 = t1 * t12 * t55 - t63 + t70;
        Double_t t72 = xMes - xIn;
        Double_t t78 = t1 * t7 + t14 * t7 + t15 + t27 - t37;
        Double_t t83 = t1 * t10 + t14 * t10 - t43 - t50;
        Double_t t86 = -t1 * t61 * t55 + t24 * t78 * t55 - t36 * t83 * t55;
        Double_t t87 = yMes - yIn;
        Double_t t94 = t19 + t31 + t34 - t21 - 2 * t40 + t46 - t29;
        Double_t t97 = t1 * t68 * t55 - t24 * t83 * t55 + t36 * t94 * t55;
        Double_t t98 = timeMes - timeIn;
        xOut = t71 * t72 + t86 * t87 + t97 * t98 + xIn;
        Double_t t106 = t10 * t68 * t55 + t24 * t12 * t55 - t5 * t61 * t55;
        Double_t t111 = t10 * t83 * t55;
        Double_t t112 = t5 * t78 * t55 - t111 - t63;
        Double_t t120 = t10 * t94 * t55 + t24 * t68 * t55 - t5 * t83 * t55;
        yOut = t106 * t72 + t112 * t87 + t120 * t98 + yIn;
        Double_t t122 = covIn[2];
        Double_t t125 = covIn[6];
        Double_t t128 = covIn[11];
        Double_t t131 = t122 * t12 * t55 - t125 * t61 * t55 + t128 * t68 * t55;
        Double_t t139 = -t122 * t61 * t55 + t125 * t78 * t55 - t128 * t83 * t55;
        Double_t t147 = t122 * t68 * t55 - t125 * t83 * t55 + t128 * t94 * t55;
        txOut = t131 * t72 + t139 * t87 + t147 * t98 + txIn;
        Double_t t149 = covIn[3];
        Double_t t152 = covIn[7];
        Double_t t155 = covIn[13];
        Double_t t158 = t149 * t12 * t55 - t152 * t61 * t55 + t155 * t68 * t55;
        Double_t t166 = -t149 * t61 * t55 + t152 * t78 * t55 - t155 * t83 * t55;
        Double_t t174 = t149 * t68 * t55 - t152 * t83 * t55 + t155 * t94 * t55;
        tyOut = t158 * t72 + t166 * t87 + t174 * t98 + tyIn;
        Double_t t182 = -t10 * t61 * t55 + t36 * t12 * t55 + t7 * t68 * t55;
        Double_t t190 = t10 * t78 * t55 - t36 * t61 * t55 - t7 * t83 * t55;
        Double_t t194 = t7 * t94 * t55 - t111 + t70;
        timeOut = t182 * t72 + t190 * t87 + t194 * t98 + timeIn;
        covOut[0] = -t71 * t1 - t86 * t24 - t97 * t36 + t1;
        covOut[1] = -t97 * t10 - t71 * t24 - t86 * t5 + t24;
        covOut[5] = -t120 * t10 - t106 * t24 - t112 * t5 + t5;
        covOut[2] = -t71 * t122 - t86 * t125 - t97 * t128 + t122;
        covOut[6] = -t106 * t122 - t112 * t125 - t120 * t128 + t125;
        covOut[9] = -t131 * t122 - t139 * t125 - t147 * t128 + covIn[9];
        covOut[3] = -t71 * t149 - t86 * t152 - t97 * t155 + t149;
        covOut[7] = -t106 * t149 - t112 * t152 - t120 * t155 + t152;
        covOut[10] = -t131 * t149 - t139 * t152 - t147 * t155 + covIn[10];
        covOut[12] = -t158 * t149 - t166 * t152 - t174 * t155 + covIn[12];
        covOut[4] = -t86 * t10 - t71 * t36 - t97 * t7 + t36;
        covOut[8] = -t112 * t10 - t106 * t36 - t120 * t7 + t10;
        covOut[11] = -t139 * t10 - t131 * t36 - t147 * t7 + t128;
        covOut[13] = -t166 * t10 - t158 * t36 - t174 * t7 + t155;
        covOut[14] = -t190 * t10 - t182 * t36 - t194 * t7 + t7;
        
        t1 = xMes - xOut;
        t2 = dy * dy - covOut[5];
        t3 = dt * dt;
        t4 = covOut[8] * covOut[8];
        t5 = dxy * dxy;
        Double_t t6 = dx * dx;
        t7 = covOut[1] * covOut[1];
        Double_t t8 = t2 * t6;
        Double_t t9 = t2 * covOut[0];
        t10 = -t8 + t7 + t9 + t5;
        t6 = -covOut[0] + t6;
        t11 = covOut[4] * covOut[4];
        t12 = -dxy + covOut[1];
        Double_t t13 = dxy * covOut[1];
        t14 = t12 * covOut[8];
        t15 = 2;
        t5 = -t15 * (t14 * covOut[4] + t13 * (covOut[14] - t3)) - t10 * t3 - t11 * t2 - t4 * t6 - (t8 - t7 - t9 - t5) * covOut[14];
        t7 = yMes - yOut;
        t8 = -t12 * t3 + t12 * covOut[14] - covOut[4] * covOut[8];
        t9 = timeMes - timeOut;
        t14 = -t2 * covOut[4] - t14;
        t5 = 1 / t5;
        t12 = -t12 * covOut[4] - t6 * covOut[8];
        
        par.SetX(xOut);
        par.SetY(yOut);
        par.SetTx(txOut);
        par.SetTy(tyOut);
        par.SetTime(timeOut);
        par.SetCovMatrix(covOut);
        
        chiSq += t1 * (t1 * (t2 * t3 - t2 * covOut[14] - t4) - t7 * t8 - t9 * t14) * t5 - t7 * (t1 * t8 - t7 * (t3 * t6 - t6 * covOut[14] - t11) + t9 * t12) * t5 - t9 * (t1 * t14 + t12 * t7 - t9 * (t13 * t15 - t10)) * t5;
    }
    /*static void Update(CbmTrackParam2& par, const CbmPixelHit* hit, Double_t& chiSq)
    {
        TMatrixD& X = par.GetVector();
        TMatrixD& P = par.GetCovMatrix();
        TMatrixD H(3, 5);
        H(0, 0) = 1;
        H(1, 1) = 1;
        H(2, 4) = 1;
        
        TMatrixD Z(3, 1);
        Z(0, 0) = hit->GetX();
        Z(1, 0) = hit->GetY();
        Z(2, 0) = hit->GetTime();
        
        TMatrixD Y = Z;
        TMatrixD HX(3, 1);
        HX.Mult(H, X);
        Y.Minus(Z, HX);
        
        TMatrixD H_t(5, 3);
        H_t.Transpose(H);
        
        TMatrixD R(3, 3);
        R(0, 0) = hit->GetDx() * hit->GetDx();
        R(0, 1) = hit->GetDxy();
        R(1, 1) = hit->GetDy() * hit->GetDy();
        R(1, 0) = hit->GetDxy();
        R(2, 2) = hit->GetTimeError() * hit->GetTimeError();
        
        TMatrixD HP(3, 5);
        HP.Mult(H, P);
        
        TMatrixD HPH_t(3, 3);
        HPH_t.Mult(HP, H_t);
        
        TMatrixD S(3, 3);
        S.Plus(R, HPH_t);
        
        TMatrixD S_1 = S;
        S_1.InvertFast();
        
        TMatrixD H_tS_1(5, 3);
        H_tS_1.Mult(H_t, S_1);
        
        TMatrixD K(5, 3);// Optimal Kalman gain
        K.Mult(P, H_tS_1);
        
        TMatrixD KY(5, 1);
        KY.Mult(K, Y);
        
        X += KY;
        
        TMatrixD KHP(5, 5);
        KHP.Mult(K, HP);
        
        P -= KHP;
        
        //
        HX.Mult(H, X);
        Y.Minus(Z, HX);
        
        HP.Mult(H, P);
        HPH_t.Mult(HP, H_t);
        
        TMatrixD V = R;
        V -= HPH_t;
        
        TMatrixD V_1 = V;
        V_1.InvertFast();
        
        TMatrixD Y_t(1, 3);
        Y_t.Transpose(Y);
        
        TMatrixD V_1Y(3, 1);
        V_1Y.Mult(V_1, Y);
        
        TMatrixD Y_tV_1Y(1, 1);
        Y_tV_1Y.Mult(Y_t, V_1Y);
        
        chiSq += Y_tV_1Y(0, 0);
    }*/
    
public:
    CbmBinnedStation(ECbmModuleId stationType, Double_t minZ, Double_t maxZ, int nofYBins, int nofXBins, int nofTBins);
    CbmBinnedStation(const CbmBinnedStation&) = delete;
    CbmBinnedStation& operator=(const CbmBinnedStation&) = delete;
    
    virtual ~CbmBinnedStation() {}
    ECbmModuleId GetStationType() const { return fStationType; }
    Double_t GetMinZ() const { return fMinZ; }
    Double_t GetMaxZ() const { return fMaxZ; }
    void SetMinZ(Double_t v) { fMinZ = v; }
    void SetMaxZ(Double_t v) { fMaxZ = v; }
    void SetMinY(Double_t v) { fMinY = v; }
    void SetMaxY(Double_t v) { fMaxY = v; }
    void SetMinX(Double_t v) { fMinX = v; }
    void SetMaxX(Double_t v) { fMaxX = v; }
    
    void SetTBinSize(Double_t v) { fTBinSize = v; }
    
    void SetMinT(Double_t v)
    {
        fMinT = v;
        fMaxT = fMinT + fNofTBins * fTBinSize;
    }
    
    void SetDefaultUse() { fDefaultUse = true; }
    
    Double_t GetDx() const { return fDx; }
    
    void SetDx(Double_t v)
    {
        if (v > fDx)
        {
            fDx = v;
            fDxSq = v * v;
        }
    }
    
    Double_t GetDy() const { return fDy; }
    
    void SetDy(Double_t v)
    {
        if (v > fDy)
        {
            fDy = v;
            fDySq = v * v;
        }
    }
    
    Double_t GetDt() const { return fDt; }
    
    void SetDt(Double_t v)
    {
        if (v > fDt)
        {
            fDt = v;
            fDtSq = v * v;
        }
    }
    
    Double_t GetScatX() const { return fScatX; }
    Double_t GetScatXSq() const { return fScatXSq; }
    
    void SetScatX(Double_t v)
    {
        fScatX = v;
        fScatXSq = v * v;
    }
    
    Double_t GetScatY() const { return fScatY; }
    Double_t GetScatYSq() const { return fScatYSq; }
    
    void SetScatY(Double_t v)
    {
        fScatY = v;
        fScatYSq = v * v;
    }
    
    Double_t GetNofSigmaX() const { return fNofSigmasX; }
    Double_t GetNofSigmaXSq() const { return fNofSigmasXSq; }
    
    void SetNofSigmaX(Double_t v)
    {
        fNofSigmasX = v;
        fNofSigmasXSq = v * v;
    }
    
    Double_t GetNofSigmaY() const { return fNofSigmasY; }
    Double_t GetNofSigmaYSq() const { return fNofSigmasYSq; }
    
    void SetNofSigmaY(Double_t v)
    {
        fNofSigmasY = v;
        fNofSigmasYSq = v * v;
    }
    
    int GetXInd(Double_t v) const
    {
        int ind = (v - fMinX) / fXBinSize;
        
        if (ind < 0)
            ind = 0;
        else if (ind >= fNofXBins)
            ind = fNofXBins - 1;
        
        return ind;
    }
    
    int GetYInd(Double_t v) const
    {
        int ind = (v - fMinY) / fYBinSize;
        
        if (ind < 0)
            ind = 0;
        else if (ind >= fNofYBins)
            ind = fNofYBins - 1;
        
        return ind;
    }
    
    int GetTInd(Double_t v) const
    {
        int ind = (v - fMinT) / fTBinSize;
        
        if (ind < 0)
            ind = 0;
        else if (ind >= fNofTBins)
            ind = fNofTBins - 1;
        
        return ind;
    }
    
    void SetStage(char v) { fStage = v; }
    
    virtual void Init()
    {
        fYBinSize = (fMaxY - fMinY) / fNofYBins;
        fXBinSize = (fMaxX - fMinX) / fNofXBins;
    }
    
    virtual void Clear()
    {
        fDx = 0;
        fDxSq = 0;
        fDy = 0;
        fDySq = 0;
        fDt = 0;
        fDtSq = 0;
        fStage = 0;
        fVertexHolder.stage = std::numeric_limits<char>::max();
        fSegments.clear();
    }
    
    virtual void AddHit(ECbmModuleId type, const CbmPixelHit* hit, Int_t index) = 0;
    virtual void IterateHits(std::function<void(CbmTBin::HitHolder&)> handleHit) = 0;
    virtual void SearchHits(const CbmTrackParam2& stateVec, Double_t stateZ, std::function<void(CbmTBin::HitHolder&)> handleHit) = 0;
    virtual void SearchHits(Segment& segment, std::function<void(CbmTBin::HitHolder&)> handleHit) = 0;
    virtual void SearchHits(Double_t minZ, Double_t maxZ, Double_t minY, Double_t maxY, Double_t minX, Double_t maxX, Double_t minT, Double_t maxT,
        std::function<void(CbmTBin::HitHolder&)> handleHit) = 0;
    
    void IterateSegments(std::function<void(Segment&)> handleSegment)
    {
        for (std::set<Segment, SegmentComp>::iterator i = fSegments.begin(); i != fSegments.end(); ++i)
            handleSegment(const_cast<Segment&> (*i));
    }
    
    void CreateSegmentsFromHits()
    {
        IterateHits(
            [&](CbmTBin::HitHolder& hitHolder)->void
            {
                Segment segment(&fVertexHolder, &hitHolder);
                fSegments.insert(segment);
            }
        );
    }
    
    void NulifySegments()
    {
        for (std::set<Segment, SegmentComp>::iterator i = fSegments.begin(); i != fSegments.end(); ++i)
        {
            Segment& segment = const_cast<Segment&> (*i);
            segment.chiSq = 0;
        }
    }
    
protected:
    ECbmModuleId fStationType;
    Double_t fMinZ;
    Double_t fMaxZ;
    int fNofYBins;
    int fNofXBins;
    int fNofTBins;
    Double_t fYBinSize;
    Double_t fXBinSize;
    Double_t fTBinSize;
    Double_t fMinY;
    Double_t fMaxY;
    Double_t fMinX;
    Double_t fMaxX;
    Double_t fMinT;
    Double_t fMaxT;
    Double_t fDx;
    Double_t fDxSq;
    Double_t fDy;
    Double_t fDySq;
    Double_t fDt;
    Double_t fDtSq;
    bool fDefaultUse;
    Double_t fScatX;
    Double_t fScatXSq;
    Double_t fScatY;
    Double_t fScatYSq;
    
public:
    std::set<Segment, SegmentComp> fSegments;
    CbmTBin fVertexBin;
    CbmTBin::HitHolder fVertexHolder;
    
protected:
    Double_t fNofSigmasX;
    Double_t fNofSigmasXSq;
    Double_t fNofSigmasY;
    Double_t fNofSigmasYSq;
    char fStage;
};

#endif /* STATION_H */

