/* Copyright (C) 2006-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Ivan Kisel,  Sergey Gorbunov [committer], Irina Rostovtseva, Valentina Akishina, Maksym Zyzak */

/*
 *====================================================================
 *
 *  CBM Level 1 Reconstruction 
 *  
 *  Authors: I.Kisel,  S.Gorbunov, I. Rostovtseva (2016)
 *  
 *
 *  e-mail : ikisel@kip.uni-heidelberg.de 
 *
 *====================================================================
 *
 *  Read Event information to L1
 *
 *====================================================================
 */

#include "CbmEvent.h"
#include "CbmKF.h"
#include "CbmL1.h"
#include "CbmMCDataObject.h"
#include "CbmMatch.h"
#include "CbmMuchGeoScheme.h"
#include "CbmMuchPixelHit.h"
#include "CbmMuchPoint.h"
#include "CbmStsAddress.h"
#include "CbmStsCluster.h"
#include "CbmStsDigi.h"
#include "CbmStsHit.h"
#include "CbmStsPoint.h"
#include "CbmStsSetup.h"
#include "CbmTofDigiBdfPar.h"
#include "CbmTofHit.h"
#include "CbmTofPoint.h"
#include "CbmTrdHit.h"
#include "CbmTrdPoint.h"

#include "FairMCEventHeader.h"

#include "L1Algo/L1Algo.h"
#include "L1AlgoInputData.h"

//#include "CbmMvdHitMatch.h"


#include "TDatabasePDG.h"
#include "TRandom.h"

#include <iostream>

using std::cout;
using std::endl;
using std::find;

//#define MVDIDEALHITS
//#define STSIDEALHITS


static bool compareZ(const int& a, const int& b)
{
  //        return (CbmL1::Instance()->vMCPoints[a].z < CbmL1::Instance()->vMCPoints[b].z);
  const CbmL1* l1 = CbmL1::Instance();
  return l1->Get_Z_vMCPoint(a) < l1->Get_Z_vMCPoint(b);
}

struct TmpHit {  // used for sort Hits before writing in the normal arrays
  int iStripF,
    iStripB;  // indices of strips
  int iStation;
  int ExtIndex;            // index of hit in the TClonesArray array ( negative for MVD )
  double u_front, u_back;  // positions of strips
  double x, y, z;          // position of hit
  double xmc, ymc, p;      // mc-position of hit, momentum
  double tx, ty;           // mc-slope of mc point
  double dx, dy, dxy;
  double du, dv;
  int iMC;  // index of MCPoint in the vMCPoints array
  double time = 0., dt = 1.e10;
  int Det;
  int id;
  int track;

  static bool Compare(const TmpHit& a, const TmpHit& b)
  {
    return (a.iStation < b.iStation) || ((a.iStation == b.iStation) && (a.y < b.y));
  }

  void CreateHitFromPoint(const CbmL1MCPoint& point, int det, int nTmpHits, int nStripF, int ip, int& NStrips,
                          L1Station& st)
  {
    ExtIndex = 0;
    Det      = det;
    id       = nTmpHits;  //tmpHits.size();
    iStation = point.iStation;

    dt   = st.dt[0];
    time = point.time + gRandom->Gaus(0, dt);

    iStripF = nStripF + ip;  //firstDetStrip + ip;
    iStripB = iStripF;
    if (NStrips <= iStripF) { NStrips = iStripF + 1; }

    float f_sigma = 0.1;  // sqrt(st.frontInfo.sigma2[0]);
    //1.;
    float b_sigma = 0.1;  // sqrt(st.backInfo.sigma2[0]);//1.;

    dx  = f_sigma;
    dy  = b_sigma;
    dxy = 0;
    du  = f_sigma;
    dv  = b_sigma;

    x = point.x;  // + gRandom->Gaus(0, dx);
    y = point.y;  // + gRandom->Gaus(0, dy);
    z = point.z;

    xmc = point.x;
    ymc = point.y;

    track = point.ID;

    p = point.p;

    u_front = x * st.frontInfo.cos_phi[0] + y * st.frontInfo.sin_phi[0];
    u_back  = x * st.backInfo.cos_phi[0] + y * st.backInfo.sin_phi[0];
    iMC     = ip;
  }

  void SetHitFromPoint(const CbmL1MCPoint& point, L1Station& st)
  {
    x       = 0.5 * (point.xIn + point.xOut) + gRandom->Gaus(0, dx);
    y       = 0.5 * (point.yIn + point.yOut) + gRandom->Gaus(0, dy);
    z       = 0.5 * (point.zIn + point.zOut);
    time    = point.time + gRandom->Gaus(0, dt);
    u_front = x * st.frontInfo.cos_phi[0] + y * st.frontInfo.sin_phi[0];
    u_back  = x * st.backInfo.cos_phi[0] + y * st.backInfo.sin_phi[0];
  }
};

/// Repack data from Clones Arrays to L1 arrays
void CbmL1::ReadEvent(L1AlgoInputData* fData_, float& TsStart, float& TsLength, float& /*TsOverlap*/, int& FstHitinTs,
                      bool& areDataLeft, CbmEvent* event)
{
  if (fVerbose >= 10) cout << "ReadEvent: start." << endl;

  areDataLeft = false;  // no data left after reading the sub-timeslice

  fData_->Clear();

  // clear arrays for next event
  vMCPoints.clear();
  vMCPoints_in_Time_Slice.clear();
  vMCTracks.clear();
  vStsHits.clear();
  vRTracks.clear();
  vHitMCRef.clear();
  vHitStore.clear();
  dFEI2vMCPoints.clear();
  dFEI2vMCTracks.clear();

  if (fVerbose >= 10) cout << "ReadEvent: clear is done." << endl;

  L1Vector<TmpHit> tmpHits("CbmL1ReadEvent::tmpHits");

  {  // reserve enough space for hits
    int nHitsTotal = 0;
    if (listMvdHits) nHitsTotal += listMvdHits->GetEntriesFast();
    Int_t nEntSts = 0;
    if (listStsHits) {
      if (event) { nEntSts = event->GetNofData(ECbmDataType::kStsHit); }
      else {
        nEntSts = listStsHits->GetEntriesFast();
      }
      if (nEntSts < 0) nEntSts = 0;  // GetNofData() can return -1;
    }
    nHitsTotal += nEntSts;
    if (fMuchPixelHits) nHitsTotal += fMuchPixelHits->GetEntriesFast();
    if (listTrdHits) nHitsTotal += listTrdHits->GetEntriesFast();
    if (fTofHits) nHitsTotal += fTofHits->GetEntriesFast();
    tmpHits.reserve(nHitsTotal);
  }

  // -- produce Sts hits from space points --

  for (int i = 0; i < NStation; i++) {

    fData_->StsHitsStartIndex[i] = static_cast<L1HitIndex_t>(-1);
    fData_->StsHitsStopIndex[i]  = 0;
  }


  nMvdPoints      = 0;
  int nStsPoints  = 0;
  int nTrdPoints  = 0;
  int nMuchPoints = 0;
  int nTofPoints  = 0;

  // get MVD hits
  Int_t nMvdHits  = 0;
  Int_t nMuchHits = 0;
  Int_t nTrdHits  = 0;
  Int_t nTofHits  = 0;
  // get STS hits
  int nStsHits = 0;

  int firstTrdPoint  = 0;
  int firstStsPoint  = 0;
  int firstMuchPoint = 0;
  int firstTofPoint  = 0;

  if (fPerformance) {
    Fill_vMCTracks();
    vMCPoints.clear();
    vMCPoints.reserve(5 * vMCTracks.size() * NStation);
    vMCPoints_in_Time_Slice.clear();
    vMCPoints_in_Time_Slice.reserve(vMCPoints.capacity());

    for (DFSET::iterator set_it = vFileEvent.begin(); set_it != vFileEvent.end(); ++set_it) {
      Int_t iFile  = set_it->first;
      Int_t iEvent = set_it->second;

      if (fMvdPoints) {
        Int_t nMvdPointsInEvent = fMvdPoints->Size(iFile, iEvent);
        double maxDeviation     = 0;
        for (Int_t iMC = 0; iMC < nMvdPointsInEvent; iMC++) {
          CbmL1MCPoint MC;
          if (!ReadMCPoint(&MC, iMC, iFile, iEvent, 1)) {
            MC.iStation     = -1;
            L1Station* sta  = algo->vStations;
            double bestDist = 1.e20;
            for (Int_t iSt = 0; iSt < NMvdStations; iSt++) {
              // use z_in since z_out is sometimes very wrong
              // due to a problem in transport
              double d = (MC.zIn - sta[iSt].z[0]);
              if (fabs(d) < fabs(bestDist)) {
                bestDist    = d;
                MC.iStation = iSt;
              }
            }
            assert(MC.iStation >= 0);
            if (fabs(maxDeviation) < fabs(bestDist)) { maxDeviation = bestDist; }
            Double_t dtrck          = dFEI(iFile, iEvent, MC.ID);
            DFEI2I::iterator trk_it = dFEI2vMCTracks.find(dtrck);
            assert(trk_it != dFEI2vMCTracks.end());
            MC.ID = trk_it->second;
            vMCTracks[MC.ID].Points.push_back_no_warning(vMCPoints.size());
            dFEI2vMCPoints.insert(DFEI2I::value_type(dFEI(iFile, iEvent, iMC), vMCPoints.size()));
            vMCPoints.push_back(MC);
            vMCPoints_in_Time_Slice.push_back(0);
            nMvdPoints++;
          }
        }
        if (fVerbose > 2) { LOG(info) << "CbmL1ReadEvent: max deviation of Mvd points " << maxDeviation; }
        // ensure that the nominal station positions are not far from the sensors
        // assert(fabs(maxDeviation) < 1.);
      }

      firstStsPoint = vMCPoints.size();
      if (fStsPoints) {
        Int_t nMC           = fStsPoints->Size(iFile, iEvent);
        double maxDeviation = 0;
        for (Int_t iMC = 0; iMC < nMC; iMC++) {
          CbmL1MCPoint MC;
          if (!ReadMCPoint(&MC, iMC, iFile, iEvent, 0)) {
            MC.iStation     = -1;
            L1Station* sta  = algo->vStations + NMvdStations;
            double bestDist = 1.e20;
            for (Int_t iSt = 0; iSt < NStsStations; iSt++) {
              // use z_in since z_out is sometimes very wrong
              // due to a problem in transport
              double d = (MC.zIn - sta[iSt].z[0]);
              if (fabs(d) < fabs(bestDist)) {
                bestDist    = d;
                MC.iStation = NMvdStations + iSt;
              }
            }
            assert(MC.iStation >= 0);
            if (fabs(maxDeviation) < fabs(bestDist)) { maxDeviation = bestDist; }
            Double_t dtrck          = dFEI(iFile, iEvent, MC.ID);
            DFEI2I::iterator trk_it = dFEI2vMCTracks.find(dtrck);
            assert(trk_it != dFEI2vMCTracks.end());
            MC.ID = trk_it->second;
            vMCTracks[MC.ID].Points.push_back_no_warning(vMCPoints.size());
            dFEI2vMCPoints.insert(DFEI2I::value_type(dFEI(iFile, iEvent, iMC + nMvdPoints), vMCPoints.size()));
            vMCPoints.push_back(MC);
            vMCPoints_in_Time_Slice.push_back(0);
            nStsPoints++;
          }
        }
        if (fVerbose > 2) { LOG(info) << "CbmL1ReadEvent: max deviation of Sts points " << maxDeviation; }
        // ensure that the nominal station positions are not far from the sensors
        //  assert(fabs(maxDeviation) < 1.);
      }

      firstMuchPoint = vMCPoints.size();
      if (fMuchPoints) {
        Int_t nMC = fMuchPoints->Size(iFile, iEvent);
        for (Int_t iMC = 0; iMC < nMC; iMC++) {
          CbmL1MCPoint MC;
          if (!ReadMCPoint(&MC, iMC, iFile, iEvent, 2)) {
            MC.iStation    = -1;
            L1Station* sta = algo->vStations + NMvdStations + NStsStations;
            for (Int_t iSt = 0; iSt < NMuchStations; iSt++) {
              if (MC.z > sta[iSt].z[0] - 2.5) { MC.iStation = NMvdStations + NStsStations + iSt; }
            }
            assert(MC.iStation >= 0);
            Double_t dtrck          = dFEI(iFile, iEvent, MC.ID);
            DFEI2I::iterator trk_it = dFEI2vMCTracks.find(dtrck);
            assert(trk_it != dFEI2vMCTracks.end());
            MC.ID = trk_it->second;
            vMCTracks[MC.ID].Points.push_back_no_warning(vMCPoints.size());
            dFEI2vMCPoints.insert(
              DFEI2I::value_type(dFEI(iFile, iEvent, iMC + nMvdPoints + nStsPoints), vMCPoints.size()));
            vMCPoints.push_back(MC);
            vMCPoints_in_Time_Slice.push_back(0);
            nMuchPoints++;
          }
        }
      }

      firstTrdPoint = vMCPoints.size();
      if (fTrdPoints)
        for (Int_t iMC = 0; iMC < fTrdPoints->Size(iFile, iEvent); iMC++) {
          CbmL1MCPoint MC;
          if (!ReadMCPoint(&MC, iMC, iFile, iEvent, 3)) {
            MC.iStation    = -1;
            L1Station* sta = algo->vStations + NMvdStations + NStsStations + NMuchStations;
            for (Int_t iSt = 0; iSt < NTrdStations; iSt++) {
              if (MC.z > sta[iSt].z[0] - 4.0) { MC.iStation = NMvdStations + NStsStations + NMuchStations + iSt; }
            }
            if (MC.iStation < 0) continue;
            assert(MC.iStation >= 0);
            Double_t dtrck          = dFEI(iFile, iEvent, MC.ID);
            DFEI2I::iterator trk_it = dFEI2vMCTracks.find(dtrck);
            assert(trk_it != dFEI2vMCTracks.end());
            MC.ID = trk_it->second;
            vMCTracks[MC.ID].Points.push_back_no_warning(vMCPoints.size());
            dFEI2vMCPoints.insert(
              DFEI2I::value_type(dFEI(iFile, iEvent, iMC + nMvdPoints + nStsPoints + nMuchPoints), vMCPoints.size()));
            vMCPoints.push_back(MC);
            vMCPoints_in_Time_Slice.push_back(0);
            nTrdPoints++;
          }
        }


      firstTofPoint = vMCPoints.size();
      if (fTofPoints) {

        vector<float> TofPointToTrackdZ[NTOFStation];

        TofPointToTrack.resize(NTOFStation);

        for (Int_t i = 0; i < NTOFStation; i++) {

          TofPointToTrack[i].resize(vMCTracks.size(), 0);
          TofPointToTrackdZ[i].resize(vMCTracks.size());
        }

        for (int iSt = 0; iSt < NTOFStation; iSt++)
          for (unsigned int i = 0; i < TofPointToTrackdZ[iSt].size(); i++) {
            TofPointToTrackdZ[iSt][i] = 100000;
            TofPointToTrack[iSt][i]   = -1;
          }


        for (Int_t iMC = 0; iMC < fTofPoints->Size(iFile, iEvent); iMC++) {

          CbmL1MCPoint MC;


          if (!ReadMCPoint(&MC, iMC, iFile, iEvent, 4)) {
            Double_t dtrck          = dFEI(iFile, iEvent, MC.ID);
            DFEI2I::iterator trk_it = dFEI2vMCTracks.find(dtrck);
            if (trk_it == dFEI2vMCTracks.end()) continue;
            Int_t IND_Track = trk_it->second;

            MC.iStation    = -1;
            L1Station* sta = algo->vStations + NMvdStations + NStsStations + NMuchStations + NTrdStations;
            for (Int_t iSt = 0; iSt < NTOFStation; iSt++)
              MC.iStation = (MC.z > sta[iSt].z[0] - 15)
                              ? (NMvdStations + NStsStations + NMuchStations + NTrdStations + iSt)
                              : MC.iStation;
            if (MC.iStation < 0) continue;
            assert(MC.iStation >= 0);
            int iTofSta = MC.iStation - (NMvdStations + NStsStations + NMuchStations + NTrdStations);

            if (iTofSta >= 0) {
              float dz = TofPointToTrackdZ[iTofSta][IND_Track];
              if (fabs(sta[MC.iStation].z[0] - MC.z) < dz) { TofPointToTrack[iTofSta][IND_Track] = iMC; }
              TofPointToTrackdZ[iTofSta][IND_Track] = fabs(sta[MC.iStation].z[0] - MC.z);
            }
          }
        }

        for (int iTofSta = 0; iTofSta < NTOFStation; iTofSta++)
          for (unsigned int iMC = 0; iMC < TofPointToTrack[iTofSta].size(); iMC++) {

            if (TofPointToTrack[iTofSta][iMC] == -1) continue;

            CbmL1MCPoint MC;

            if (!ReadMCPoint(&MC, TofPointToTrack[iTofSta][iMC], iFile, iEvent, 4)) {

              MC.iStation    = -1;
              L1Station* sta = algo->vStations + NMvdStations + NStsStations + NMuchStations + NTrdStations;
              for (Int_t iSt = 0; iSt < NTOFStation; iSt++)
                MC.iStation = (MC.z > sta[iSt].z[0] - 15)
                                ? (NMvdStations + NStsStations + NMuchStations + NTrdStations + iSt)
                                : MC.iStation;

              if (MC.iStation < 0) continue;
              TofPointToTrack[iTofSta][iMC] = vMCPoints.size();
              vMCTracks[iMC].Points.push_back_no_warning(vMCPoints.size());

              MC.ID = iMC;

              vMCPoints.push_back(MC);
              vMCPoints_in_Time_Slice.push_back(0);

              dFEI2vMCPoints.insert(DFEI2I::value_type(
                dFEI(iFile, iEvent, TofPointToTrack[iTofSta][iMC] + nMvdPoints + nStsPoints + nMuchPoints + nTrdPoints),
                vMCPoints.size() - 1));
              nTofPoints++;
            }
          }
      }
    }  //time_slice

    for (unsigned int iTr = 0; iTr < vMCTracks.size(); iTr++) {

      sort(vMCTracks[iTr].Points.begin(), vMCTracks[iTr].Points.end(), compareZ);

      if (vMCTracks[iTr].mother_ID >= 0) {
        Double_t dtrck          = dFEI(vMCTracks[iTr].iFile, vMCTracks[iTr].iEvent, vMCTracks[iTr].mother_ID);
        DFEI2I::iterator map_it = dFEI2vMCTracks.find(dtrck);
        if (map_it == dFEI2vMCTracks.end()) vMCTracks[iTr].mother_ID = -2;
        else
          vMCTracks[iTr].mother_ID = map_it->second;
      }
    }  //iTr
    if (fVerbose >= 10) cout << "Points in vMCTracks are sorted." << endl;

  }  //fPerformance

  int NStrips = 0;

  // get MVD hits

  if (listMvdHits) {

    int firstDetStrip = NStrips;

    for (int j = 0; j < listMvdHits->GetEntriesFast(); j++) {
      TmpHit th;
      {
        CbmMvdHit* mh = L1_DYNAMIC_CAST<CbmMvdHit*>(listMvdHits->At(j));
        th.ExtIndex   = -(1 + j);
        th.id         = tmpHits.size();
        th.iStation   = mh->GetStationNr();

        int stIdx = StationIdxReverseConverter[mh->GetStationNr()];
        if (stIdx == -1) continue;
        th.iStation = stIdx;  //mh->GetStationNr() - 1;
        th.iStripF  = firstDetStrip + j;
        th.iStripB  = th.iStripF;
        if (NStrips <= th.iStripF) { NStrips = th.iStripF + 1; }

        TVector3 pos, err;
        mh->Position(pos);
        mh->PositionError(err);

        th.dx = mh->GetDx();
        th.dy = mh->GetDy();

        th.du  = mh->GetDx();
        th.dv  = mh->GetDy();
        th.dxy = mh->GetDxy();

        th.x = pos.X();
        th.y = pos.Y();
        th.z = pos.Z();

        L1Station& st = algo->vStations[th.iStation];
        th.u_front    = th.x * st.frontInfo.cos_phi[0] + th.y * st.frontInfo.sin_phi[0];
        th.u_back     = th.x * st.backInfo.cos_phi[0] + th.y * st.backInfo.sin_phi[0];
      }
      th.Det = 0;
      th.iMC = -1;
      if (fPerformance) {
        if (listMvdHitMatches) {
          CbmMatch* mvdHitMatch = L1_DYNAMIC_CAST<CbmMatch*>(listMvdHitMatches->At(j));
          if (mvdHitMatch->GetNofLinks() > 0)
            if (mvdHitMatch->GetLink(0).GetIndex() < nMvdPoints) {
              th.iMC = mvdHitMatch->GetLink(0).GetIndex();
#ifdef MVDIDEALHITS
//TODO
#endif
            }
        }
      }
      //if(  h.MC_Point >=0 ) // DEBUG !!!!
      {
        tmpHits.push_back(th);
        nMvdHits++;
      }
    }  // for j
  }    // if listMvdHits
  if (fVerbose >= 10) cout << "ReadEvent: mvd hits are gotten." << endl;

  if ((2 == fStsUseMcHit)) {  // SG!! create TRD hits from TRD points

    int firstDetStrip = NStrips;
    for (int ip = firstStsPoint; ip < firstStsPoint + nStsPoints; ip++) {
      const CbmL1MCPoint& p = vMCPoints[ip];
      //       int mcTrack           = p.ID;
      //       if (mcTrack < 0) continue;
      //       const CbmL1MCTrack& t = vMCTracks[mcTrack];
      //if (t.p < 1) continue;
      // if (t.Points.size() > 4) continue;
      // cout << "sts mc: station " << p.iStation - NMvdStations << " x " << p.x << " y " << p.y << " z " << p.z << " t "
      //            << p.time << " mc " << p.ID << " p " << p.p << endl;
      TmpHit th;
      int DetId = 1;
      th.CreateHitFromPoint(p, DetId, tmpHits.size(), firstDetStrip, ip, NStrips, algo->vStations[p.iStation]);
      tmpHits.push_back(th);
      nStsHits++;
    }
  }

  if (listStsHits && (2 != fStsUseMcHit)) {

    Int_t nEntSts = (event ? event->GetNofData(ECbmDataType::kStsHit) : listStsHits->GetEntriesFast());

    int firstDetStrip = NStrips;

    if (event) FstHitinTs = 0;

    for (Int_t j = FstHitinTs; j < nEntSts; j++) {
      Int_t hitIndex = 0;
      hitIndex       = (event ? event->GetIndex(ECbmDataType::kStsHit, j) : j);

      int hitIndexSort = 0;
      if (!fLegacyEventMode) hitIndexSort = StsIndex[hitIndex];
      else
        hitIndexSort = j;

      CbmStsHit* sh = L1_DYNAMIC_CAST<CbmStsHit*>(listStsHits->At(hitIndexSort));

      TmpHit th;
      {
        CbmStsHit* mh = L1_DYNAMIC_CAST<CbmStsHit*>(listStsHits->At(hitIndexSort));
        th.ExtIndex   = hitIndexSort;
        th.Det        = 1;
        int stIdx =
          StationIdxReverseConverter[CbmStsSetup::Instance()->GetStationNumber(mh->GetAddress()) + NMvdStationsGeom];

        if (stIdx == -1) continue;


        th.iStation = stIdx;  //mh->GetStationNr() - 1;
        th.iStripF  = firstDetStrip + mh->GetFrontClusterId();
        th.iStripB  = firstDetStrip + mh->GetBackClusterId();

        if (NStrips <= th.iStripF) { NStrips = th.iStripF + 1; }
        if (NStrips <= th.iStripB) { NStrips = th.iStripB + 1; }

        //Get time
        th.time = mh->GetTime();
        th.dt   = mh->GetTimeError();

        if (!fLegacyEventMode) { th.id = nMvdHits + hitIndex; }
        else {
          th.id = tmpHits.size();
        }

        /// stop if reco TS ends and many hits left
        if (!event)
          if ((th.time > (TsStart + TsLength)) && ((nEntSts - hitIndex) > 300)) {
            areDataLeft = true;  // there are unprocessed data left in the time slice
            break;
          }

        TVector3 pos, err;
        mh->Position(pos);
        mh->PositionError(err);

        th.x = pos.X();
        th.y = pos.Y();
        th.z = pos.Z();

        th.dx  = mh->GetDx();
        th.dy  = mh->GetDy();
        th.dxy = mh->GetDxy();

        th.du = mh->GetDu();
        th.dv = mh->GetDv();

        L1Station& st = algo->vStations[th.iStation];
        th.u_front    = th.x * st.frontInfo.cos_phi[0] + th.y * st.frontInfo.sin_phi[0];
        th.u_back     = th.x * st.backInfo.cos_phi[0] + th.y * st.backInfo.sin_phi[0];
      }
      th.iMC = -1;

      Int_t iMC = -1;

      if (fPerformance) {
        if (listStsClusterMatch) {
          const CbmMatch* frontClusterMatch =
            static_cast<const CbmMatch*>(listStsClusterMatch->At(sh->GetFrontClusterId()));
          const CbmMatch* backClusterMatch =
            static_cast<const CbmMatch*>(listStsClusterMatch->At(sh->GetBackClusterId()));
          CbmMatch stsHitMatch;

          for (Int_t iFrontLink = 0; iFrontLink < frontClusterMatch->GetNofLinks(); iFrontLink++) {
            const CbmLink& frontLink = frontClusterMatch->GetLink(iFrontLink);

            for (Int_t iBackLink = 0; iBackLink < backClusterMatch->GetNofLinks(); iBackLink++) {
              const CbmLink& backLink = backClusterMatch->GetLink(iBackLink);
              if (frontLink == backLink) {
                stsHitMatch.AddLink(frontLink);
                stsHitMatch.AddLink(backLink);
              }
            }
          }

          if (stsHitMatch.GetNofLinks() > 0) {
            Float_t bestWeight = 0.f;
            for (Int_t iLink = 0; iLink < stsHitMatch.GetNofLinks(); iLink++) {
              if (stsHitMatch.GetLink(iLink).GetWeight() > bestWeight) {
                bestWeight   = stsHitMatch.GetLink(iLink).GetWeight();
                Int_t iFile  = stsHitMatch.GetLink(iLink).GetFile();
                Int_t iEvent = stsHitMatch.GetLink(iLink).GetEntry();
                //                 if(fLegacyEventMode) //TODO Fix the event number in links
                //                   iEvent+=1;
                Int_t iIndex            = stsHitMatch.GetLink(iLink).GetIndex() + nMvdPoints;
                Double_t dtrck          = dFEI(iFile, iEvent, iIndex);
                DFEI2I::iterator trk_it = dFEI2vMCPoints.find(dtrck);
                if (trk_it == dFEI2vMCPoints.end()) continue;
                iMC = trk_it->second;
              }
            }
          }
#ifdef STSIDEALHITS
          // TODO
          // CbmStsPoint* point = L1_DYNAMIC_CAST<CbmStsPoint*>(listStsPts->At(s.ExtIndex));
          // h.z = 0.5 * (point->GetZOut() + point->GetZIn());
#endif
        }
        else {
          iMC = sh->GetRefId();  // TODO1: don't need this with FairLinks
        }
      }  //fPerformance

      if (iMC > -1) {
        th.iMC = iMC;
        // th.track = iMC;
      }

      tmpHits.push_back(th);
      nStsHits++;

    }  // for j
  }    // if listStsHits

  if ((2 == fMuchUseMcHit) && fUseMUCH) {  // SG!! create TRD hits from TRD points
    int firstDetStrip = NStrips;

    for (int ip = firstMuchPoint; ip < firstMuchPoint + nMuchPoints; ip++) {
      const CbmL1MCPoint& p = vMCPoints[ip];

      //       int mcTrack = p.ID;
      //       if (mcTrack < 0) continue;
      //       const CbmL1MCTrack& t = vMCTracks[mcTrack];
      //if (t.p < 1) continue;
      // if (t.Points.size() > 4) continue;

      TmpHit th;
      int DetId = 2;
      th.CreateHitFromPoint(p, DetId, tmpHits.size(), firstDetStrip, ip, NStrips, algo->vStations[p.iStation]);

      tmpHits.push_back(th);
      nMuchHits++;
    }
  }

  if (fUseMUCH && fMuchPixelHits && (2 != fMuchUseMcHit)) {

    Int_t nEnt = fMuchPixelHits->GetEntriesFast();

    int firstDetStrip = NStrips;

    for (int j = 0; j < nEnt; j++) {
      TmpHit th;
      {

        CbmMuchPixelHit* mh = static_cast<CbmMuchPixelHit*>(fMuchPixelHits->At(j));

        th.ExtIndex = j;
        th.Det      = 2;
        th.id       = tmpHits.size();


        Int_t stationNumber = CbmMuchGeoScheme::GetStationIndex(mh->GetAddress());
        Int_t layerNumber   = CbmMuchGeoScheme::GetLayerIndex(mh->GetAddress());

        int DetId = stationNumber * 3 + layerNumber;

        int stIdx = StationIdxReverseConverter[DetId + NMvdStationsGeom + NStsStationsGeom];
        if (stIdx == -1) continue;
        th.iStation = stIdx;  //mh->GetStationNr() - 1;

        //Get time
        th.time = mh->GetTime() - 14.5;
        th.dt   = mh->GetTimeError();


        //   th.iSector  = 0;
        th.iStripF = firstDetStrip + j;
        th.iStripB = th.iStripF;
        if (NStrips <= th.iStripF) { NStrips = th.iStripF + 1; }

        th.x = mh->GetX();
        th.y = mh->GetY();
        th.z = mh->GetZ();

        th.dx  = mh->GetDx();
        th.dy  = mh->GetDy();
        th.dxy = 0;

        th.du = mh->GetDx();
        th.dv = mh->GetDy();


        L1Station& st = algo->vStations[th.iStation];
        th.u_front    = th.x * st.frontInfo.cos_phi[0] + th.y * st.frontInfo.sin_phi[0];
        th.u_back     = th.x * st.backInfo.cos_phi[0] + th.y * st.backInfo.sin_phi[0];
      }
      th.iMC  = -1;
      int iMC = -1;


      if (fPerformance) {
        if (listMuchHitMatches) {
          CbmMatch* matchHitMatch = L1_DYNAMIC_CAST<CbmMatch*>(listMuchHitMatches->At(j));


          for (Int_t iLink = 0; iLink < matchHitMatch->GetNofLinks(); iLink++) {
            if (matchHitMatch->GetLink(iLink).GetIndex() < nMuchPoints) {
              iMC          = matchHitMatch->GetLink(iLink).GetIndex();
              Int_t iIndex = iMC + nMvdPoints + nStsPoints;

              Int_t iFile  = matchHitMatch->GetLink(0).GetFile();
              Int_t iEvent = matchHitMatch->GetLink(0).GetEntry();

              Double_t dtrck          = dFEI(iFile, iEvent, iIndex);
              DFEI2I::iterator trk_it = dFEI2vMCPoints.find(dtrck);
              if (trk_it == dFEI2vMCPoints.end()) continue;
              th.iMC = trk_it->second;
              if ((1 == fMuchUseMcHit) && (th.iMC > -1))
                th.SetHitFromPoint(vMCPoints[th.iMC], algo->vStations[th.iStation]);
            }
          }
        }
      }

      tmpHits.push_back(th);
      nMuchHits++;

    }  // for j
  }    // if listMvdHits

  if ((2 == fTrdUseMcHit) && fTrdHitMatches && fUseTRD) {  // SG!! create TRD hits from TRD points
    int firstDetStrip = NStrips;

    for (int ip = firstTrdPoint; ip < firstTrdPoint + nTrdPoints; ip++) {
      const CbmL1MCPoint& p = vMCPoints[ip];

      //       int mcTrack = p.ID;
      //       if (mcTrack < 0) continue;
      //       const CbmL1MCTrack& t = vMCTracks[mcTrack];

      TmpHit th;
      int DetId = 3;
      th.CreateHitFromPoint(p, DetId, tmpHits.size(), firstDetStrip, ip, NStrips, algo->vStations[p.iStation]);
      tmpHits.push_back(th);
      nTrdHits++;
    }
  }


  if (fUseTRD && listTrdHits && (2 != fTrdUseMcHit)) {
    int firstDetStrip = NStrips;
    vector<bool> mcUsed(nTrdPoints, 0);

    for (int j = 0; j < listTrdHits->GetEntriesFast(); j++) {
      TmpHit th;

      CbmTrdHit* mh = L1_DYNAMIC_CAST<CbmTrdHit*>(listTrdHits->At(j));
      th.ExtIndex   = j;
      th.Det        = 3;

      th.id = tmpHits.size();

      int sta = mh->GetPlaneId();

      int stIdx =
        StationIdxReverseConverter[mh->GetPlaneId() + NMvdStationsGeom + NStsStationsGeom + NMuchStationsGeom];
      if (stIdx == -1) continue;

      if ((fTrackingMode == L1Algo::TrackingMode::kMcbm) && (sta > 1) && (fMissingHits)) { sta = sta - 1; }

      th.iStation = stIdx;

      //  if (mh->GetPlaneId()==0) continue;
      //  if (mh->GetPlaneId()==1) continue;
      //  if (mh->GetPlaneId()==2) continue;
      //  if (mh->GetPlaneId()==3) continue;

      th.time = mh->GetTime();
      th.dt   = mh->GetTimeError();

      //   th.iSector  = 0;
      th.iStripF = firstDetStrip + j;
      th.iStripB = th.iStripF;  //TrdHitsOnStationIndex[num+1][k];
      if (NStrips <= th.iStripF) { NStrips = th.iStripF + 1; }

      TVector3 pos, err;
      mh->Position(pos);
      mh->PositionError(err);

      th.x = pos.X();
      th.y = pos.Y();
      th.z = pos.Z();


      th.dx  = fabs(mh->GetDx());
      th.dy  = fabs(mh->GetDy());
      th.dxy = 0;

      th.du = fabs(mh->GetDx());
      th.dv = fabs(mh->GetDy());

      L1Station& st = algo->vStations[th.iStation];
      th.u_front    = th.x * st.frontInfo.cos_phi[0] + th.y * st.frontInfo.sin_phi[0];
      th.u_back     = th.x * st.backInfo.cos_phi[0] + th.y * st.backInfo.sin_phi[0];

      th.iMC  = -1;
      int iMC = -1;

      if (fPerformance && fTrdHitMatches) {

        CbmMatch* trdHitMatch = L1_DYNAMIC_CAST<CbmMatch*>(fTrdHitMatches->At(j));

        if (trdHitMatch->GetNofLinks() > 0)
          if (trdHitMatch->GetLink(0).GetIndex() < nTrdPoints) {
            iMC    = trdHitMatch->GetLink(0).GetIndex();
            th.iMC = iMC + nMvdPoints + nStsPoints + nMuchPoints;
            //                                th.track = vMCPoints[th.iMC].ID;

            if ((1 == fTrdUseMcHit) && (th.iMC > -1))  //SG!!! replace hits by MC points

              th.SetHitFromPoint(vMCPoints[th.iMC], algo->vStations[th.iStation]);

            if (L1Algo::TrackingMode::kGlobal == fTrackingMode) {  //SG!!! replace hits by MC points

              CbmTrdPoint* pt =
                (CbmTrdPoint*) fTrdPoints->Get(trdHitMatch->GetLink(0).GetFile(), trdHitMatch->GetLink(0).GetEntry(),
                                               trdHitMatch->GetLink(0).GetIndex());

              th.dx = 0.1;
              th.du = 0.1;
              th.dy = 0.1;
              th.dv = 0.1;

              th.x    = 0.5 * (pt->GetXOut() + pt->GetXIn());  // + gRandom->Gaus(0, th.dx);
              th.y    = 0.5 * (pt->GetYOut() + pt->GetYIn());  // + gRandom->Gaus(0, th.dy);
              th.z    = 0.5 * (pt->GetZOut() + pt->GetZIn());
              th.time = pt->GetTime();
              th.dt   = 10000.;

              th.u_front = th.x * st.frontInfo.cos_phi[0] + th.y * st.frontInfo.sin_phi[0];
              th.u_back  = th.x * st.backInfo.cos_phi[0] + th.y * st.backInfo.sin_phi[0];
              th.dx      = 1.;
              th.du      = 1.;
              th.dy      = 1.;
              th.dv      = 1.;

              if (th.iMC < 0) continue;
              if (mcUsed[iMC]) continue;
              if (0) {
                int mcTrack = -1;
                float mcP   = 0;
                if (th.iMC >= 0) {
                  mcTrack = vMCPoints[th.iMC].ID;
                  if (mcTrack >= 0) { mcP = vMCTracks[mcTrack].p; }
                }
                if (mcP < 1.) continue;
              }
              mcUsed[iMC] = 1;
            }
          }
      }

      tmpHits.push_back(th);
      nTrdHits++;

    }  // for j
  }    // if listTrdHits

  if ((2 == fTofUseMcHit) && fUseTOF) {  // SG!! create TRD hits from TRD points
    int firstDetStrip = NStrips;

    for (int ip = firstTofPoint; ip < firstTofPoint + nTofPoints; ip++) {
      const CbmL1MCPoint& p = vMCPoints[ip];

      //       int mcTrack = p.ID;
      //  if (mcTrack < 0) continue;
      //const CbmL1MCTrack& t = vMCTracks[mcTrack];
      //if (t.p < 1) continue;
      //if (t.Points.size() > 4) continue;

      TmpHit th;

      int DetId = 4;
      th.CreateHitFromPoint(p, DetId, tmpHits.size(), firstDetStrip, ip, NStrips, algo->vStations[p.iStation]);
      tmpHits.push_back(th);

      nTofHits++;
    }
  }


  if (fUseTOF && fTofHits && (2 != fTofUseMcHit)) {

    int firstDetStrip = NStrips;

    for (int j = 0; j < fTofHits->GetEntriesFast(); j++) {
      TmpHit th;

      CbmTofHit* mh = L1_DYNAMIC_CAST<CbmTofHit*>(fTofHits->At(j));


      th.ExtIndex = j;
      th.Det      = 4;

      th.id = tmpHits.size();

      if (0x00202806 == mh->GetAddress() || 0x00002806 == mh->GetAddress()) continue;

      int sttof = -1;

      sttof = fTofDigiBdfPar->GetTrackingStation(mh);

      if (sttof < 0) continue;

      // if (fTofDigiBdfPar->GetTrackingStation(mh) == 0) sttof = 0;
      // if (fTofDigiBdfPar->GetTrackingStation(mh) == 1) sttof = 0;
      // if (fTofDigiBdfPar->GetTrackingStation(mh) == 2) sttof = 1;
      // if (fTofDigiBdfPar->GetTrackingStation(mh) == 3) sttof = 1;
      // if (fTofDigiBdfPar->GetTrackingStation(mh) == 4) sttof = 2;
      // if (fTofDigiBdfPar->GetTrackingStation(mh) == 5) sttof = 2;


      th.time = mh->GetTime();
      th.dt   = mh->GetTimeError();

      th.dx  = mh->GetDx();
      th.dy  = mh->GetDy();
      th.dxy = 0;

      th.du = mh->GetDx();
      th.dv = mh->GetDy();

      //   th.iSector  = 0;
      th.iStripF = firstDetStrip + j;
      th.iStripB = th.iStripF;
      if (NStrips <= th.iStripF) { NStrips = th.iStripF + 1; }

      TVector3 pos, err;
      mh->Position(pos);
      mh->PositionError(err);

      th.x = pos.X();
      th.y = pos.Y();
      th.z = pos.Z();

      if (fMissingHits)
        if ((th.x > 20) && (th.z > 270) && (fTofDigiBdfPar->GetTrackingStation(mh) == 1)) sttof = 2;
      if (th.z > 400) continue;

      int stIdx =
        StationIdxReverseConverter[sttof + NMvdStationsGeom + NStsStationsGeom + NMuchStationsGeom + NTrdStationsGeom];
      if (stIdx == -1) continue;
      th.iStation = stIdx;

      L1Station& st = algo->vStations[th.iStation];
      th.u_front    = th.x * st.frontInfo.cos_phi[0] + th.y * st.frontInfo.sin_phi[0];
      th.u_back     = th.x * st.backInfo.cos_phi[0] + th.y * st.backInfo.sin_phi[0];


      th.iMC = -1;
      //      int iMC = -1;


      if (fPerformance) {

        CbmMatch* matchHitMatch = L1_DYNAMIC_CAST<CbmMatch*>(fTofHitDigiMatches->At(j));


        if (0 < matchHitMatch->GetNofLinks()) {
          Int_t iMC    = matchHitMatch->GetLink(0).GetIndex();
          Int_t iFile  = matchHitMatch->GetLink(0).GetFile();
          Int_t iEvent = matchHitMatch->GetLink(0).GetEntry();

          CbmTofPoint* pt = L1_DYNAMIC_CAST<CbmTofPoint*>(fTofPoints->Get(iFile, iEvent, iMC));
          pt->GetTrackID();
          Double_t dtrck          = dFEI(iFile, iEvent, pt->GetTrackID());
          DFEI2I::iterator trk_it = dFEI2vMCPoints.find(dtrck);
          if (trk_it != dFEI2vMCPoints.end()) th.iMC = TofPointToTrack[sttof][trk_it->second];
          if ((1 == fTofUseMcHit) && (th.iMC > -1)) th.SetHitFromPoint(vMCPoints[th.iMC], algo->vStations[th.iStation]);
        }
      }

      tmpHits.push_back(th);
      nTofHits++;

    }  // for j
  }    // if listTofHits

  if (fVerbose >= 10) cout << "ReadEvent: sts hits are gotten." << endl;

  if (fVerbose > 1) {
    LOG(info) << "L1 ReadEvent: nhits mvd " << nMvdHits << " sts " << nStsHits << " much " << nMuchHits << " trd "
              << nTrdHits << " tof " << nTofHits << endl;
  }

  // sort hits
  int nHits = nMvdHits + nStsHits + nMuchHits + nTrdHits + nTofHits;


  sort(tmpHits.begin(), tmpHits.end(), TmpHit::Compare);

  // save strips in L1Algo
  fData_->NStsStrips = NStrips;
  fData_->fStripFlag.reset(NStrips, 0);
  int maxHitIndex = 0;

  for (int ih = 0; ih < nHits; ih++) {
    TmpHit& th                     = tmpHits[ih];
    char flag                      = th.iStation * 4;
    fData_->fStripFlag[th.iStripF] = flag;
    fData_->fStripFlag[th.iStripB] = flag;
    if (maxHitIndex < th.id) { maxHitIndex = th.id; }

  }  // ih

  if (fVerbose >= 10) { cout << "ReadEvent: strips are read." << endl; }

  // -- save hits --
  int nEffHits = 0;

  SortedIndex.reset(maxHitIndex + 1);

  vStsHits.reserve(nHits);
  fData_->vStsHits.reserve(nHits);

  vHitStore.reserve(nHits);
  vHitMCRef.reserve(nHits);

  /*
   * In this section the vStsHits, vHitStore and vHitMCRef vectors as well as fData->vStsHits are
   * filled. 
   */
  
  L1_SHOW(vStsHits.size());
  L1_SHOW(vHitStore.size());
  L1_SHOW(vHitMCRef.size());
  for (int i = 0; i < nHits; i++) {
    TmpHit& th = tmpHits[i];

    CbmL1HitStore s;
    s.Det      = th.Det;
    s.ExtIndex = th.ExtIndex;
    s.iStation = th.iStation;
    s.x        = th.x;
    s.y        = th.y;
    s.dx       = th.dx;
    s.dy       = th.dy;
    s.dxy      = th.dxy;
    s.time     = th.time;

    SortedIndex[th.id] = i;

    assert(th.iStripF >= 0 || th.iStripF < NStrips);
    assert(th.iStripB >= 0 || th.iStripB < NStrips);

    L1Hit h;
    h.f = th.iStripF;
    h.b = th.iStripB;

    h.ID = th.id;

    h.t  = th.time;
    h.dt = th.dt;
    //  h.track = th.track;
    //    h.dx  = th.dx;
    //    h.dy  = th.dy;
    h.du = th.du;
    h.dv = th.dv;
    h.u  = th.u_front;
    h.v  = th.u_back;
    //    h.dxy = th.dxy;
    //     h.p = th.p;
    //     h.q = th.q;
    // h.ista = th.iStation;

    h.z = th.z;


    // save hit  
    vStsHits.push_back(CbmL1Hit(fData->vStsHits.size(), th.ExtIndex, th.Det));

    vStsHits[vStsHits.size() - 1].x = th.x;
    vStsHits[vStsHits.size() - 1].y = th.y;
    vStsHits[vStsHits.size() - 1].t = th.time;

    vStsHits[vStsHits.size() - 1].ID = th.id;

    vStsHits[vStsHits.size() - 1].f = th.iStripF;
    vStsHits[vStsHits.size() - 1].b = th.iStripB;


    fData_->vStsHits.push_back(h);

    int sta = th.iStation;

    if (fData_->StsHitsStartIndex[sta] == static_cast<L1HitIndex_t>(-1)) fData_->StsHitsStartIndex[sta] = nEffHits;
    nEffHits++;

    fData_->StsHitsStopIndex[sta] = nEffHits;

    vHitStore.push_back(s);
    vHitMCRef.push_back(th.iMC);
  }
  L1_SHOW(&vStsHits);
  L1_SHOW(vStsHits.size());
  L1_SHOW(&(fData_->vStsHits));
  L1_SHOW((fData_->vStsHits).size());

  for (int i = 0; i < NStation; i++) {
    if (fData_->StsHitsStartIndex[i] == static_cast<L1HitIndex_t>(-1)) {
      fData_->StsHitsStartIndex[i] = fData_->StsHitsStopIndex[i];
    }
  }

  if (fVerbose >= 10) cout << "ReadEvent: mvd and sts are saved." << endl;

  /*
   * Translate gathered hits data to the L1Algo object. TODO: raplace it with L1DataManager functionality (S.Zharko)
   */

  algo->SetData(fData_->GetStsHits(), fData_->GetNStsStrips(), fData_->GetSFlag(), fData_->GetStsHitsStartIndex(),
                fData_->GetStsHitsStopIndex());

  if (fPerformance) {

    if (fVerbose >= 10) cout << "HitMatch is done." << endl;
    if (fVerbose >= 10) cout << "MCPoints and MCTracks are saved." << endl;
  }

  if (fVerbose >= 10) cout << "ReadEvent is done." << endl;


}  // void CbmL1::ReadEvent()


void CbmL1::Fill_vMCTracks()
{
  vMCTracks.clear();
  {
    Int_t nMCTracks = 0;
    for (DFSET::iterator set_it = vFileEvent.begin(); set_it != vFileEvent.end(); ++set_it) {
      Int_t iFile  = set_it->first;
      Int_t iEvent = set_it->second;
      nMCTracks += fMCTracks->Size(iFile, iEvent);
    }
    vMCTracks.reserve(nMCTracks);
  }

  int fileEvent = 0;
  for (DFSET::iterator set_it = vFileEvent.begin(); set_it != vFileEvent.end(); ++set_it, ++fileEvent) {
    Int_t iFile  = set_it->first;
    Int_t iEvent = set_it->second;

    auto header = dynamic_cast<FairMCEventHeader*>(fMcEventHeader->Get(iFile, iEvent));
    assert(header);
    if (fVerbose > 2) {
      LOG(info) << "mc event vertex at " << header->GetX() << " " << header->GetY() << " " << header->GetZ();
    }

    Int_t nMCTrack = fMCTracks->Size(iFile, iEvent);

    for (Int_t iMCTrack = 0; iMCTrack < nMCTrack; iMCTrack++) {
      CbmMCTrack* MCTrack = L1_DYNAMIC_CAST<CbmMCTrack*>(fMCTracks->Get(iFile, iEvent, iMCTrack));
      if (!MCTrack) continue;

      int mother_ID = MCTrack->GetMotherId();

      if (mother_ID < 0 && mother_ID != -2) mother_ID = -iEvent - 1;
      TVector3 vr;
      TLorentzVector vp;
      MCTrack->GetStartVertex(vr);
      MCTrack->Get4Momentum(vp);

      Int_t pdg  = MCTrack->GetPdgCode();
      Double_t q = 0, mass = 0.;
      if (pdg < 9999999 && ((TParticlePDG*) TDatabasePDG::Instance()->GetParticle(pdg))) {
        q    = TDatabasePDG::Instance()->GetParticle(pdg)->Charge() / 3.0;
        mass = TDatabasePDG::Instance()->GetParticle(pdg)->Mass();
      }

      Int_t IND_Track = vMCTracks.size();  //or iMCTrack?
      CbmL1MCTrack T(mass, q, vr, vp, IND_Track, mother_ID, pdg);
      //        CbmL1MCTrack T(mass, q, vr, vp, iMCTrack, mother_ID, pdg);
      T.time   = MCTrack->GetStartT();
      T.iFile  = iFile;
      T.iEvent = iEvent;
      // signal: primary tracks, displaced from the primary vertex
      T.isSignal = T.IsPrimary() && (T.z > header->GetZ() + 1.e-10);

      vMCTracks.push_back(T);
      //    Double_t dtrck =dFEI(iFile,iEvent,iMCTrack);
      dFEI2vMCTracks.insert(DFEI2I::value_type(dFEI(iFile, iEvent, iMCTrack), vMCTracks.size() - 1));
    }  //iTracks
  }    //Links

}  //Fill_vMCTracks

bool CbmL1::ReadMCPoint(CbmL1MCPoint* MC, int iPoint, int file, int event, int MVD)
{
  TVector3 xyzI, PI, xyzO, PO;
  Int_t mcID    = -1;
  Double_t time = 0.f;
  if (MVD == 1) {
    CbmMvdPoint* pt = L1_DYNAMIC_CAST<CbmMvdPoint*>(fMvdPoints->Get(file, event, iPoint));  // file, event, object
    //CbmMvdPoint *pt = L1_DYNAMIC_CAST<CbmMvdPoint*> (Point);

    if (!pt) return 1;
    pt->Position(xyzI);
    pt->Momentum(PI);
    pt->PositionOut(xyzO);
    pt->MomentumOut(PO);
    mcID = pt->GetTrackID();
    time = pt->GetTime();
  }
  if (MVD == 0) {
    CbmStsPoint* pt = L1_DYNAMIC_CAST<CbmStsPoint*>(fStsPoints->Get(file, event, iPoint));  // file, event, object
    if (!pt) return 1;
    if (!fLegacyEventMode) {
      Double_t StartTime     = fTimeSlice->GetStartTime();
      Double_t EndTime       = fTimeSlice->GetEndTime();
      Double_t Time_MC_point = pt->GetTime() + fEventList->GetEventTime(event, file);

      if (StartTime > 0)
        if (Time_MC_point < StartTime) return 1;

      if (EndTime > 0)
        if (Time_MC_point > EndTime) return 1;
    }

    pt->Position(xyzI);
    pt->Momentum(PI);
    pt->PositionOut(xyzO);
    pt->MomentumOut(PO);
    mcID = pt->GetTrackID();
    time = pt->GetTime();
  }


  if (MVD == 2) {
    CbmMuchPoint* pt = L1_DYNAMIC_CAST<CbmMuchPoint*>(fMuchPoints->Get(file, event, iPoint));  // file, event, object
    if (!pt) return 1;
    if (!fLegacyEventMode) {
      Double_t StartTime     = fTimeSlice->GetStartTime();
      Double_t EndTime       = fTimeSlice->GetEndTime();
      Double_t Time_MC_point = pt->GetTime() + fEventList->GetEventTime(event, file);

      if (StartTime > 0)
        if (Time_MC_point < StartTime) return 1;

      if (EndTime > 0)
        if (Time_MC_point > EndTime) return 1;
    }

    pt->Position(xyzI);
    pt->Momentum(PI);
    pt->PositionOut(xyzO);
    pt->Momentum(PO);
    mcID = pt->GetTrackID();
    time = pt->GetTime();
  }


  if (MVD == 3) {
    CbmTrdPoint* pt = L1_DYNAMIC_CAST<CbmTrdPoint*>(fTrdPoints->Get(file, event, iPoint));  // file, event, object

    if (!pt) return 1;
    if (!fLegacyEventMode) {
      Double_t StartTime     = fTimeSlice->GetStartTime();                             // not used
      Double_t EndTime       = fTimeSlice->GetEndTime();                               // not used
      Double_t Time_MC_point = pt->GetTime() + fEventList->GetEventTime(event, file);  // not used

      if (StartTime > 0)
        if (Time_MC_point < StartTime) return 1;

      if (EndTime > 0)
        if (Time_MC_point > EndTime) return 1;
    }

    pt->Position(xyzI);
    pt->Momentum(PI);
    pt->PositionOut(xyzO);
    pt->MomentumOut(PO);
    mcID = pt->GetTrackID();

    time = pt->GetTime();
  }

  if (MVD == 4) {
    CbmTofPoint* pt = L1_DYNAMIC_CAST<CbmTofPoint*>(fTofPoints->Get(file, event, iPoint));  // file, event, object
    if (!pt) return 1;
    if (!fLegacyEventMode) {
      Double_t StartTime     = fTimeSlice->GetStartTime();
      Double_t EndTime       = fTimeSlice->GetEndTime();
      Double_t Time_MC_point = pt->GetTime() + fEventList->GetEventTime(event, file);

      if (StartTime > 0)
        if (Time_MC_point < StartTime) return 1;

      if (EndTime > 0)
        if (Time_MC_point > EndTime) return 1;
    }

    pt->Position(xyzI);
    pt->Momentum(PI);
    pt->Position(xyzO);
    pt->Momentum(PO);
    mcID = pt->GetTrackID();
    time = pt->GetTime();
  }

  TVector3 xyz = .5 * (xyzI + xyzO);
  TVector3 P   = .5 * (PI + PO);
  MC->x        = xyz.X();
  MC->y        = xyz.Y();
  MC->z        = xyz.Z();
  MC->px       = P.X();
  MC->py       = P.Y();
  MC->pz       = P.Z();
  MC->xIn      = xyzI.X();
  MC->yIn      = xyzI.Y();
  MC->zIn      = xyzI.Z();
  MC->pxIn     = PI.X();
  MC->pyIn     = PI.Y();
  MC->pzIn     = PI.Z();
  MC->xOut     = xyzO.X();
  MC->yOut     = xyzO.Y();
  MC->zOut     = xyzO.Z();
  MC->pxOut    = PO.X();
  MC->pyOut    = PO.Y();
  MC->pzOut    = PO.Z();
  MC->p        = sqrt(fabs(MC->px * MC->px + MC->py * MC->py + MC->pz * MC->pz));
  MC->ID       = mcID;
  MC->pointId  = iPoint;
  MC->file     = file;
  MC->event    = event;

  MC->time = time;

  if (MC->ID < 0) return 1;
  CbmMCTrack* MCTrack = L1_DYNAMIC_CAST<CbmMCTrack*>(fMCTracks->Get(file, event, MC->ID));
  if (!MCTrack) return 1;
  MC->pdg       = MCTrack->GetPdgCode();
  MC->mother_ID = MCTrack->GetMotherId();

  TParticlePDG* particlePDG = TDatabasePDG::Instance()->GetParticle(MC->pdg);
  if (particlePDG) {
    MC->q    = particlePDG->Charge() / 3.0;
    MC->mass = particlePDG->Mass();
  }

  return 0;
}

bool CbmL1::ReadMCPoint(CbmL1MCPoint* /*MC*/, int /*iPoint*/, int /*MVD*/) { return 0; }

/// Procedure for match hits and MCPoints.
/// Read information about correspondence between hits and mcpoints and fill CbmL1MCPoint::hitIds and CbmL1Hit::mcPointIds arrays
/// should be called after fill of algo
void CbmL1::HitMatch()
{
  const int NHits = vStsHits.size();
  for (int iH = 0; iH < NHits; iH++) {
    CbmL1Hit& hit = vStsHits[iH];

    if ((hit.Det == 1) && (2 != fStsUseMcHit)) {
      CbmStsHit* sh = L1_DYNAMIC_CAST<CbmStsHit*>(listStsHits->At(vStsHits[iH].extIndex));

      int iP = -1;

      if (listStsClusterMatch) {

        const CbmMatch* frontClusterMatch =
          static_cast<const CbmMatch*>(listStsClusterMatch->At(sh->GetFrontClusterId()));
        const CbmMatch* backClusterMatch =
          static_cast<const CbmMatch*>(listStsClusterMatch->At(sh->GetBackClusterId()));
        CbmMatch stsHitMatch;

        Float_t Sum_of_front = 0;
        Float_t Sum_of_back  = 0;


        for (Int_t iFrontLink = 0; iFrontLink < frontClusterMatch->GetNofLinks(); iFrontLink++) {
          const CbmLink& frontLink = frontClusterMatch->GetLink(iFrontLink);
          Sum_of_front             = Sum_of_front + frontLink.GetWeight();
        }

        for (Int_t iBackLink = 0; iBackLink < backClusterMatch->GetNofLinks(); iBackLink++) {
          const CbmLink& backLink = backClusterMatch->GetLink(iBackLink);
          Sum_of_back             = Sum_of_back + backLink.GetWeight();
        }

        for (Int_t iFrontLink = 0; iFrontLink < frontClusterMatch->GetNofLinks(); iFrontLink++) {
          const CbmLink& frontLink = frontClusterMatch->GetLink(iFrontLink);

          // Float_t Fraction_front = frontLink.GetWeight()/Sum_of_front;

          for (Int_t iBackLink = 0; iBackLink < backClusterMatch->GetNofLinks(); iBackLink++) {
            const CbmLink& backLink = backClusterMatch->GetLink(iBackLink);

            // Float_t  Fraction_back = backLink.GetWeight()/Sum_of_back;

            if (frontLink == backLink) {
              stsHitMatch.AddLink(frontLink);
              stsHitMatch.AddLink(backLink);
            }
          }
        }

        Float_t bestWeight  = 0.f;
        Float_t totalWeight = 0.f;
        for (Int_t iLink = 0; iLink < stsHitMatch.GetNofLinks(); iLink++) {
          const CbmLink& link = stsHitMatch.GetLink(iLink);
          Int_t iFile         = link.GetFile();
          Int_t iEvent        = link.GetEntry();
          Int_t iIndex        = link.GetIndex();


          if (fLegacyEventMode) {
            iFile  = vFileEvent.begin()->first;
            iEvent = vFileEvent.begin()->second;
          }

          Double_t dpnt           = dFEI(iFile, iEvent, nMvdPoints + iIndex);
          DFEI2I::iterator pnt_it = dFEI2vMCPoints.find(dpnt);


          assert(pnt_it != dFEI2vMCPoints.end());

          totalWeight += link.GetWeight();
          if (link.GetWeight() > bestWeight) {
            bestWeight = link.GetWeight();
            iP         = pnt_it->second;
          }
        }
      }  //mach cluster

      if (iP >= 0) {
        hit.mcPointIds.push_back_no_warning(iP);
        vMCPoints[iP].hitIds.push_back_no_warning(iH);
      }
      else {
        int idPoint = vHitMCRef[iH];
        if (idPoint >= 0) {
          hit.mcPointIds.push_back_no_warning(idPoint);
          vMCPoints[idPoint].hitIds.push_back_no_warning(iH);
        }
      }  // if no clusters
    }

    if ((hit.Det != 1) || (2 == fStsUseMcHit)) {  // the hit is not from STS
      int iP = vHitMCRef[iH];
      if (iP >= 0) {
        hit.mcPointIds.push_back_no_warning(iP);
        vMCPoints[iP].hitIds.push_back_no_warning(iH);
      }
    }

  }  // for hits
}
