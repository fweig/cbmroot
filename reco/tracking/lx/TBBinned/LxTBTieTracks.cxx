/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "LxTBTieTracks.h"
#include "data/CbmLitTrackParam.h"
#include "base/CbmLitToolFactory.h"
#include "utils/CbmLitConverter.h"
#include "CbmMuchPixelHit.h"
#include "CbmTrdHit.h"
#include "CbmMuch.h"

using std::list;

void LxTBBinnedDetector::AddStsTrack(const FairTrackParam& par, Double_t chiSq, Double_t time, Int_t selfId/*, Int_t eventId, Int_t fileId*/)
{
   LxTBBinnedStsTrack stsTrack = { par, chiSq, time, selfId/*, eventId, fileId*/ };
   fStsTracks.push_back(stsTrack);
}

struct TieHandlePoint : public LxTBBinndedLayer::PointHandler
{   
   TrackUpdatePtr fFilter;
   CbmLitTrackParam fPar;
   CbmLitTrackParam fOutPar;
   litfloat fChiSq;
   litfloat fZ;
   const LxTbBinnedPoint* fPoint;

   TieHandlePoint() : fFilter(), fPar(), fOutPar(), fChiSq(), fZ(), fPoint(nullptr) {}
   TieHandlePoint(const TieHandlePoint&) = delete;
   TieHandlePoint& operator=(const TieHandlePoint&) = delete;
   
   void HandlePoint(const LxTbBinnedPoint& point)
   {
      litfloat chiSq = std::numeric_limits<litfloat>::max();
      CbmLitPixelHit litHit;
      litHit.SetX(point.x);
      litHit.SetY(point.y);
      litHit.SetZ(fZ);
      litHit.SetDx(point.dx);
      litHit.SetDy(point.dy);
      litHit.SetDz(0);
      litHit.SetDxy(0);
      litHit.SetRefId(0);        
      CbmLitTrackParam par = fPar;
      fFilter->Update(&par, &litHit, chiSq);
      
      if (chiSq < 25 && chiSq < fChiSq)
      {
         fChiSq = chiSq;
         fPoint = &point;
         fOutPar = par;
      }
   }
};

LxTBBinnedDetector::LxTBBinnedDetector(int nofl, int nofxb, int nofyb, int noftb, int binSizeT) :
   fLayers(reinterpret_cast<LxTBBinndedLayer*> (new unsigned char[nofl * sizeof(LxTBBinndedLayer)])), fNofLayers(nofl), fStsTracks(), fMuchTracks(nullptr), fGlobalTracks(nullptr)
{
   for (int i = 0; i < fNofLayers; ++i)
      new (&fLayers[i]) LxTBBinndedLayer(nofxb, nofyb, noftb, binSizeT);
}

void LxTBBinnedDetector::Init()
{
   for (int i = 0; i < fNofLayers; ++i)
      fLayers[i].Init();
}

void LxTBBinnedDetector::Clear()
{
   fStsTracks.clear();
   
   for (int i = 0; i < fNofLayers; ++i)
      fLayers[i].Clear();
}

void LxTBBinnedDetector::SetTSBegin(unsigned long long tsLowBound)
{
   for (int i = 0; i < fNofLayers; ++i)
      fLayers[i].SetTSBegin(tsLowBound);
}

void LxTBBinnedDetector::TieTracks(LxTbBinnedFinder&)
{
   //for (int i = 0; i < fFinder.nofTrackBins; ++i)
   //{
      //list<LxTbBinnedFinder::Chain*>& recoTracksBin = fFinder.recoTracks[i];
      
      //for (list<LxTbBinnedFinder::Chain*>::const_iterator j = recoTracksBin.begin(); j != recoTracksBin.end(); ++j)
         //recoTracks.push_back(*j);
   //}
   TrackPropagatorPtr propagator = CbmLitToolFactory::CreateTrackPropagator("lit");
   TrackUpdatePtr filter = CbmLitToolFactory::CreateTrackUpdate("kalman");
   //TrackFitterPtr fFitter = CbmLitToolFactory::CreateTrackFitter("lit_kalman");
   int muchTrackNo = 0;
   int globalTrackNo = 0;
   
   for (list<LxTBBinnedStsTrack>::const_iterator i = fStsTracks.begin(); i != fStsTracks.end(); ++i)
   {
      const LxTBBinnedStsTrack& stsTrack = *i;
      CbmGlobalTrack* globalTrack = new ((*fGlobalTracks)[globalTrackNo]) CbmGlobalTrack();
      globalTrack->SetStsTrackIndex(globalTrackNo++);
      Double_t qp = stsTrack.fPar.GetQp();
      
      if (0 == qp)// We want propagate J/psi tracks only.
         continue;
      
      Double_t p = 1 / qp;
      
      if (p < 3 )// We want propagate J/psi tracks only.
         continue;
      
      Double_t txSq = stsTrack.fPar.GetTx() * stsTrack.fPar.GetTx();
      Double_t tySq = stsTrack.fPar.GetTy() * stsTrack.fPar.GetTy();
      Double_t ptSq = p * p * (txSq + tySq) / (1 + txSq + tySq);
      
      if (ptSq < 1)// We want propagate J/psi tracks only.
         continue;
      
      Double_t trackChiSq = stsTrack.fChiSq;
      CbmTrackParam cbmPar;
      cbmPar.Set(stsTrack.fPar, stsTrack.fTime, 2);
      CbmLitTrackParam par;
      CbmLitConverterFairTrackParam::FairTrackParamToCbmLitTrackParam(&cbmPar, &par);
      CbmLitTrackParam prevPar;
      CbmLitTrackParam firstPar;
      bool firstTime = true;
      timetype t = stsTrack.fTime;
      int nofMissingHits = 0;
      
      struct PointData
      {
         const LxTbBinnedPoint* point;
         bool isTrd;
      };
      
      list<PointData> points;
      
      for (int j = 0; j < fNofLayers; ++j)
      {
         prevPar = par;
         
         if (propagator->Propagate(&par, fLayers[j].z, 13) == kLITERROR)
            break;
         
         scaltype x = par.GetX();
         scaltype y = par.GetY();
         timetype length = sqrt((par.GetX() - prevPar.GetX()) * (par.GetX() - prevPar.GetX()) + (par.GetY() - prevPar.GetY()) * (par.GetY() - prevPar.GetY()) +
            (par.GetZ() - prevPar.GetZ()) * (par.GetZ() - prevPar.GetZ()));
         t += 1.e9 * length / speedOfLight;
         scaltype sigmaXSq = par.GetCovariance(0);
         scaltype sigmaYSq = par.GetCovariance(6);
         TieHandlePoint pointHandler;
         pointHandler.fFilter = filter;
         pointHandler.fPar = par;
         pointHandler.fZ = fLayers[j].z;
         pointHandler.fChiSq = std::numeric_limits<litfloat>::max();
         pointHandler.fPoint = 0;
         fLayers[j].Search(x, sigmaXSq, y, sigmaYSq, t, &pointHandler);
         
         if (pointHandler.fPoint)
         {
            par = pointHandler.fOutPar;
            trackChiSq += pointHandler.fChiSq;
            PointData pd = { pointHandler.fPoint, j > 11 };
            points.push_back(pd);
            
            if (firstTime)
            {
               firstPar = par;
               firstTime = false;
            }
         }
         else
            ++nofMissingHits;
         
         if (nofMissingHits > 4)
            break;
      }
      
      if (points.empty())
         continue;
      
      CbmMuchTrack* muchTrack = new ((*fMuchTracks)[muchTrackNo]) CbmMuchTrack();
      globalTrack->SetMuchTrackIndex(muchTrackNo++);
      Int_t ndf = points.size() * 2 - 5;
      muchTrack->SetChiSq(trackChiSq);
      muchTrack->SetNDF(ndf < 1 ? 1 : ndf);
      muchTrack->SetPreviousTrackId(stsTrack.fSelfId);
      muchTrack->SetFlag(kLITGOOD);
      FairTrackParam parLast, parFirst;
      CbmLitConverterFairTrackParam::CbmLitTrackParamToFairTrackParam(&par, &parLast);
      CbmLitConverterFairTrackParam::CbmLitTrackParamToFairTrackParam(&firstPar, &parFirst);
      muchTrack->SetParamLast(&parLast);
      muchTrack->SetParamFirst(&parFirst);
      
      for (list<PointData>::const_iterator j = points.begin(); j != points.end(); ++j)
      {
         const PointData pd = *j;
         muchTrack->AddHit(pd.point->refId, pd.isTrd ? kTRDHIT : kMUCHPIXELHIT);
      }
   }
}
