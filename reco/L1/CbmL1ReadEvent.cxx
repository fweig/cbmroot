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

#include "CbmKF.h"
#include "CbmL1.h"
#include "CbmMatch.h"
#include "CbmMuchGeoScheme.h"
#include "CbmMuchPixelHit.h"
#include "CbmMuchPoint.h"
#include "CbmStsAddress.h"
#include "CbmStsSetup.h"
#include "CbmTofDigiBdfPar.h"
#include "CbmTofHit.h"
#include "CbmTofPoint.h"
#include "CbmTrdHit.h"
#include "CbmTrdPoint.h"

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
  double dx, dy, dxy;
  double du, dv;
  int iMC;  // index of MCPoint in the vMCPoints array
  double time = 0., dt = 1.e10;
  int Det;
  int id;

  static bool Compare(const TmpHit& a, const TmpHit& b)
  {
    return (a.iStation < b.iStation) || ((a.iStation == b.iStation) && (a.y < b.y));
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

    fData_->StsHitsStartIndex[i] = static_cast<THitI>(-1);
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

  L1Vector<CbmLink*> ToFPointsMatch("CbmL1ReadEvent::ToFPointsMatch");

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
        for (Int_t iMC = 0; iMC < nMvdPointsInEvent; iMC++) {
          CbmL1MCPoint MC;
          if (!ReadMCPoint(&MC, iMC, iFile, iEvent, 1)) {
            MC.iStation    = -1;
            L1Station* sta = algo->vStations;
            for (Int_t iSt = 0; iSt < NStsStations; iSt++) {
              if (MC.z > sta[iSt].z[0] - 1) { MC.iStation = iSt; }
            }
            assert(MC.iStation >= 0);
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
      }

      if (fStsPoints) {
        Int_t nMC = fStsPoints->Size(iFile, iEvent);
        for (Int_t iMC = 0; iMC < nMC; iMC++) {
          CbmL1MCPoint MC;
          if (!ReadMCPoint(&MC, iMC, iFile, iEvent, 0)) {
            MC.iStation    = -1;
            L1Station* sta = algo->vStations + NMvdStations;
            for (Int_t iSt = 0; iSt < NStsStations; iSt++) {
              if (MC.z > sta[iSt].z[0] - 2.5) { MC.iStation = NMvdStations + iSt; }
            }
            assert(MC.iStation >= 0);
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
      }

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

      ToFPointsMatch.clear();

      if (fTofPoints) {
        // TOF data arrays also contain fake beam-counter data. Select the real TOF points here.

        for (int j = 0; j < fTofHits->GetEntriesFast(); j++) {

          CbmLink* link = 0;

          CbmTofHit* mh = L1_DYNAMIC_CAST<CbmTofHit*>(fTofHits->At(j));

          if (0x00202806 == mh->GetAddress() || 0x00002806 == mh->GetAddress()) {
            ToFPointsMatch.push_back(link);
            continue;
          }

          CbmMatch* matchHitMatch = L1_DYNAMIC_CAST<CbmMatch*>(fTofHitDigiMatches->At(j));

          if (matchHitMatch->GetNofLinks() > 0) {

            link = (CbmLink*) &matchHitMatch->GetLink(0);

            CbmTofPoint* pt = (CbmTofPoint*) fTofPoints->Get(link->GetFile(), link->GetEntry(), link->GetIndex());

            for (int iLink = 1; iLink < matchHitMatch->GetNofLinks(); iLink++) {

              CbmLink* link1 = (CbmLink*) &(matchHitMatch->GetLink(iLink));

              CbmTofPoint* pt_cur =
                (CbmTofPoint*) fTofPoints->Get(link1->GetFile(), link1->GetEntry(), link1->GetIndex());

              TVector3 pos_cur, pos_old, pos_hit;


              pt_cur->Position(pos_cur);
              pt->Position(pos_old);
              mh->Position(pos_hit);

              if (fabs(pos_cur.Z() - pos_hit.Z()) < fabs(pos_old.Z() - pos_hit.Z())) {
                pt   = pt_cur;
                link = link1;
              }
            }

            ToFPointsMatch.push_back(link);
          }  // for j
        }    // if listTrdHits


        for (UInt_t iMC = 0; iMC < ToFPointsMatch.size(); iMC++) {

          if (ToFPointsMatch[iMC] == 0) continue;

          int eventNr = ToFPointsMatch[iMC]->GetEntry();

          if (eventNr != iEvent) continue;

          CbmL1MCPoint MC;

          if (!ReadMCPoint(&MC, ToFPointsMatch[iMC]->GetIndex(), ToFPointsMatch[iMC]->GetFile(),
                           ToFPointsMatch[iMC]->GetEntry(), 4)) {

            MC.iStation    = -1;
            L1Station* sta = algo->vStations + NMvdStations + NStsStations + NMuchStations + NTrdStations;
            for (Int_t iSt = 0; iSt < NTOFStation; iSt++)
              MC.iStation = (MC.z > sta[iSt].z[0] - 6)
                              ? (NMvdStations + NStsStations + NMuchStations + NTrdStations + iSt)
                              : MC.iStation;

            Double_t dtrck          = dFEI(iFile, eventNr, MC.ID);
            DFEI2I::iterator trk_it = dFEI2vMCTracks.find(dtrck);
            if (trk_it == dFEI2vMCTracks.end()) continue;

            Int_t IND_Track = trk_it->second;

            vMCTracks[IND_Track].Points.push_back_no_warning(vMCPoints.size());

            MC.ID = trk_it->second;

            vMCPoints.push_back(MC);
            vMCPoints_in_Time_Slice.push_back(0);

            dFEI2vMCPoints.insert(DFEI2I::value_type(
              dFEI(iFile, eventNr,
                   ToFPointsMatch[iMC]->GetIndex() + nMvdPoints + nStsPoints + nMuchPoints + nTrdPoints),
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
        th.iStripF    = firstDetStrip + j;
        th.iStripB    = th.iStripF;
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

  Int_t nEntSts = 0;
  if (listStsHits) {

    nEntSts = (event ? event->GetNofData(ECbmDataType::kStsHit) : listStsHits->GetEntriesFast());

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
        th.iStation =
          NMvdStations + CbmStsSetup::Instance()->GetStationNumber(mh->GetAddress());  //mh->GetStationNr() - 1;
        th.iStripF = firstDetStrip + mh->GetFrontClusterId();
        th.iStripB = firstDetStrip + mh->GetBackClusterId();

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

  if (fMuchPixelHits) {

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


        th.iStation = DetId + NMvdStations + NStsStations;
        //Get time
        th.time = mh->GetTime() - 14.5;
        th.dt   = mh->GetTimeError();


        //   th.iSector  = 0;
        th.iStripF = firstDetStrip + j;
        th.iStripB = th.iStripF;
        if (NStrips <= th.iStripF) { NStrips = th.iStripF + 1; }

        th.x = mh->GetX();
        th.y = mh->GetY() - 0.5;
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
              //                         th.track = vMCPoints[th.iMC].ID;
              //                            th.qp = vMCPoints[iMC].p;
              //                  if(matchHitMatch -> GetNofLinks() == 0) continue;
              //                    Float_t bestWeight = 0.f;
              //                    Float_t totalWeight = 0.f;
              //                   int iMCPoint = -1;
              //                 CbmLink link;
              //
              //                            CbmMuchPoint* pt = (CbmMuchPoint*) fMuchPoints->Get(
              //                              matchHitMatch->GetLink(0).GetFile(),
              //                              matchHitMatch->GetLink(0).GetEntry(),
              //                              matchHitMatch->GetLink(0).GetIndex());
              //              //  double mom = sqrt(pt->GetPxOut()*pt->GetPxOut()+pt->GetPyOut()*pt->GetPyOut()+pt->GetPzOut()*pt->GetPzOut());
              // //                   th.p = mom;
              // //                   th.q = pt->GetTrackID();//(L1_DYNAMIC_CAST<CbmMCTrack*>(fMCTracks->Get(link.GetFile(),link.GetEntry(),  pt->GetTrackID()) ))->GetCharge();
              //
              //                              th.x = pt->GetX( th.z );// + gRandom->Gaus(0,th.dx);
              //
              //                              th.y = pt->GetY(th.z);// + gRandom->Gaus(0,th.dy);
              //                   th.time = pt->GetTime();  //+ gRandom->Gaus(0,th.dt);
              //
              //                            L1Station& st = algo->vStations[th.iStation];
              //                            th.u_front =
              //                              th.x * st.frontInfo.cos_phi[0] + th.y * st.frontInfo.sin_phi[0];
              //                            th.u_back =
              //                              th.x * st.backInfo.cos_phi[0] + th.y * st.backInfo.sin_phi[0];
            }
          }
        }
      }

      tmpHits.push_back(th);
      nMuchHits++;

    }  // for j
  }    // if listMvdHits

  if (listTrdHits) {

    int firstDetStrip = NStrips;

    for (int j = 0; j < listTrdHits->GetEntriesFast(); j++) {
      TmpHit th;

      CbmTrdHit* mh = L1_DYNAMIC_CAST<CbmTrdHit*>(listTrdHits->At(j));
      th.ExtIndex   = j;
      th.Det        = 3;

      th.id = tmpHits.size();

      int sta = mh->GetPlaneId();

      if (sta > 1) sta = sta - 1;

      th.iStation = NMvdStations + sta + NStsStations + NMuchStations;

      // if (mh->GetPlaneId()==0) continue;
      // if (mh->GetPlaneId()==1) continue;
      // if (mh->GetPlaneId()==2) continue;
      // if (mh->GetPlaneId()==3) continue;


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

      if (fPerformance) {

        if (fTrdHitMatches) {

          CbmMatch* trdHitMatch = L1_DYNAMIC_CAST<CbmMatch*>(fTrdHitMatches->At(j));


          if (trdHitMatch->GetNofLinks() > 0)
            if (trdHitMatch->GetLink(0).GetIndex() < nTrdPoints) {
              iMC    = trdHitMatch->GetLink(0).GetIndex();
              th.iMC = iMC + nMvdPoints + nStsPoints + nMuchPoints;
              //                    th.track = vMCPoints[th.iMC].ID;
              //
              //                            CbmTrdPoint* pt = (CbmTrdPoint*) fTrdPoints->Get(
              //                              trdHitMatch->GetLink(0).GetFile(),
              //                              trdHitMatch->GetLink(0).GetEntry(),
              //                              trdHitMatch->GetLink(0).GetIndex());
              //
              //                              float min = 0.1;
              //
              //
              //                           th.dx = min;
              //                           th.du = min;
              //                           th.x = 0.5*(pt->GetXOut()+pt->GetXIn())+ gRandom->Gaus(0,th.dx);
              //
              //                           th.dy = min;
              //                           th.dv = min;
              //                           th.y = 0.5*(pt->GetYOut()+pt->GetYIn())+ gRandom->Gaus(0,th.dy);
              //
              //                th.time = pt->GetTime();
              //
              //                        L1Station& st = algo->vStations[th.iStation];
              //                        th.u_front =
              //                          th.x * st.frontInfo.cos_phi[0] + th.y * st.frontInfo.sin_phi[0];
              //                        th.u_back =
              //                          th.x * st.backInfo.cos_phi[0] + th.y * st.backInfo.sin_phi[0];
            }
        }
      }
      tmpHits.push_back(th);
      nTrdHits++;

      //}
    }  // for j
  }    // if listTrdHits


  if (fTofHits) {

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

      // if (fTofDigiBdfPar->GetTrackingStation(mh) == 0) sttof = 0;
      // if (fTofDigiBdfPar->GetTrackingStation(mh) == 1) sttof = 0;
      // if (fTofDigiBdfPar->GetTrackingStation(mh) == 2) sttof = 1;
      // if (fTofDigiBdfPar->GetTrackingStation(mh) == 3) sttof = 1;
      // if (fTofDigiBdfPar->GetTrackingStation(mh) == 4) sttof = 2;
      // if (fTofDigiBdfPar->GetTrackingStation(mh) == 5) sttof = 2;

      if ((th.x > 20) && (th.z > 270) && (fTofDigiBdfPar->GetTrackingStation(mh) == 1)) sttof = 2;

      th.iStation = sttof + NMvdStations + NStsStations + NMuchStations + NTrdStations;

      th.time = mh->GetTime();

      th.dt = mh->GetTimeError();

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

      if (th.z > 400) continue;

      L1Station& st = algo->vStations[th.iStation];
      th.u_front    = th.x * st.frontInfo.cos_phi[0] + th.y * st.frontInfo.sin_phi[0];
      th.u_back     = th.x * st.backInfo.cos_phi[0] + th.y * st.backInfo.sin_phi[0];


      th.iMC = -1;
      //      int iMC = -1;


      if (fPerformance) {

        if (ToFPointsMatch[j] == 0) continue;

        //  th.iMC = j+nMvdPoints+nStsPoints+nTrdPoints+nMuchPoints;

        Int_t iFile  = ToFPointsMatch[j]->GetFile();
        Int_t iEvent = ToFPointsMatch[j]->GetEntry();

        Int_t iIndex = ToFPointsMatch[j]->GetIndex() + nMvdPoints + nStsPoints + nMuchPoints + nTrdPoints;

        Double_t dtrck          = dFEI(iFile, iEvent, iIndex);
        DFEI2I::iterator trk_it = dFEI2vMCPoints.find(dtrck);
        if (trk_it == dFEI2vMCPoints.end()) continue;
        th.iMC = trk_it->second;
        //         th.track = vMCPoints[th.iMC].ID;
        //         th.qp = vMCPoints[th.iMC].p;

        //        CbmTofPoint* pt = (CbmTofPoint*) fTofPoints->Get(ToFPointsMatch[j]->GetFile(),ToFPointsMatch[j]->GetEntry(),ToFPointsMatch[j]->GetIndex());


        //                 th.x = pt->GetX() + gRandom->Gaus(0,th.dx);
        //
        //         th.y = pt->GetY()+ gRandom->Gaus(0,th.dy);
        //         th.time =  pt->GetTime()+ gRandom->Gaus(0,th.dt);
        //
        //                 L1Station &st = algo->vStations[th.iStation];
        //         th.u_front = th.x*st.frontInfo.cos_phi[0] + th.y*st.frontInfo.sin_phi[0];
        //         th.u_back  = th.x* st.backInfo.cos_phi[0] + th.y*st.backInfo.sin_phi[0];
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
  for (int ih = 0; ih < nHits; ih++) {
    TmpHit& th                     = tmpHits[ih];
    char flag                      = th.iStation * 4;
    fData_->fStripFlag[th.iStripF] = flag;
    fData_->fStripFlag[th.iStripB] = flag;
  }  // ih

  if (fVerbose >= 10) { cout << "ReadEvent: strips are read." << endl; }

  // -- save hits --
  int nEffHits    = 0;
  int maxHitIndex = nMvdHits + nStsHits + nMuchHits + nTrdHits;
  if (fTofHits) maxHitIndex += fTofHits->GetEntriesFast();

  SortedIndex.reset(std::max(listStsHits->GetEntriesFast(), maxHitIndex));

  vStsHits.reserve(nHits);
  fData_->vStsHits.reserve(nHits);

  vHitStore.reserve(nHits);
  vHitMCRef.reserve(nHits);

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

    if (fData_->StsHitsStartIndex[sta] == static_cast<THitI>(-1)) fData_->StsHitsStartIndex[sta] = nEffHits;
    nEffHits++;

    fData_->StsHitsStopIndex[sta] = nEffHits;

    vHitStore.push_back(s);
    vHitMCRef.push_back(th.iMC);
  }

  for (int i = 0; i < NStation; i++) {

    if (fData_->StsHitsStartIndex[i] == static_cast<THitI>(-1))
      fData_->StsHitsStartIndex[i] = fData_->StsHitsStopIndex[i];
  }

  if (fVerbose >= 10) cout << "ReadEvent: mvd and sts are saved." << endl;

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
  PrimVtx.MC_ID = 999;
  {
    CbmL1Vtx Vtxcurr;
    int nvtracks = 0, nvtrackscurr = 0;

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

    for (DFSET::iterator set_it = vFileEvent.begin(); set_it != vFileEvent.end(); ++set_it) {
      Int_t iFile  = set_it->first;
      Int_t iEvent = set_it->second;

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
        Double_t q = 1, mass = 0.;
        if (pdg < 9999999 && ((TParticlePDG*) TDatabasePDG::Instance()->GetParticle(pdg))) {
          q    = TDatabasePDG::Instance()->GetParticle(pdg)->Charge() / 3.0;
          mass = TDatabasePDG::Instance()->GetParticle(pdg)->Mass();
        }
        else
          q = 0;
        Int_t IND_Track = vMCTracks.size();  //or iMCTrack?
        CbmL1MCTrack T(mass, q, vr, vp, IND_Track, mother_ID, pdg);
        //        CbmL1MCTrack T(mass, q, vr, vp, iMCTrack, mother_ID, pdg);
        T.time   = MCTrack->GetStartT();
        T.iFile  = iFile;
        T.iEvent = iEvent;

        vMCTracks.push_back(T);
        //    Double_t dtrck =dFEI(iFile,iEvent,iMCTrack);
        dFEI2vMCTracks.insert(DFEI2I::value_type(dFEI(iFile, iEvent, iMCTrack), vMCTracks.size() - 1));

        if (T.mother_ID < 0) {                                             // vertex track
          if (PrimVtx.MC_ID == 999 || fabs(T.z - Vtxcurr.MC_z) > 1.e-7) {  // new vertex
            if (nvtrackscurr > nvtracks) {
              PrimVtx  = Vtxcurr;
              nvtracks = nvtrackscurr;
            }
            Vtxcurr.MC_x  = T.x;
            Vtxcurr.MC_y  = T.y;
            Vtxcurr.MC_z  = T.z;
            Vtxcurr.MC_ID = T.mother_ID;
            nvtrackscurr  = 1;
          }
          else
            nvtrackscurr++;
        }
      }  //iTracks
    }    //Links
    if (nvtrackscurr > nvtracks) PrimVtx = Vtxcurr;
  }  //PrimVtx

  if (fVerbose && PrimVtx.MC_ID == 999) cout << "No primary vertex !!!" << endl;
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
    //     if ( !fLegacyEventMode )
    //     {
    //       Double_t StartTime = fTimeSlice->GetStartTime();
    //       Double_t EndTime = fTimeSlice->GetEndTime();
    //       Double_t Time_MC_point =  pt->GetTime() + fEventList->GetEventTime(event, file);
    //       if (Time_MC_point < StartTime )
    //         return 1;
    //
    //       if (Time_MC_point > EndTime )
    //         return 1;
    //     }

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
      if (Time_MC_point < StartTime) return 1;

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
      //Double_t StartTime = fTimeSlice->GetStartTime();  // not used
      //Double_t EndTime = fTimeSlice->GetEndTime();      // not used
      //Double_t Time_MC_point =  pt->GetTime() + fEventList->GetEventTime(event, file); // not used
      //       if (Time_MC_point < StartTime )
      //         return 1;
      //
      //       if (Time_MC_point > EndTime )
      //         return 1;
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
      if (Time_MC_point < StartTime) return 1;

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

    if (hit.Det == 1) {
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

    if (hit.Det != 1) {  // the hit is not from STS
      int iP = vHitMCRef[iH];
      if (iP >= 0) {
        hit.mcPointIds.push_back_no_warning(iP);
        vMCPoints[iP].hitIds.push_back_no_warning(iH);
      }
    }

  }  // for hits
}
