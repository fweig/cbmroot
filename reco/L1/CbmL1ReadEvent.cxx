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
#include "CbmTofHit.h"
#include "CbmTofPoint.h"
#include "CbmTofTrackingInterface.h"
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
  //        return (CbmL1::Instance()->fvMCPoints[a].z < CbmL1::Instance()->fvMCPoints[b].z);
  const CbmL1* l1 = CbmL1::Instance();
  return l1->Get_Z_vMCPoint(a) < l1->Get_Z_vMCPoint(b);
}

/// Local structure for a hit, containing both measured and MC information.
/// The structure is used to sort hits before writing them into L1 input arrays
///
struct TmpHit {
  int iStripF;          ///< index of front strip
  int iStripB;          ///< index of back strip
  int iStation;         ///< index of station
  int ExtIndex;         ///< index of hit in the external TClonesArray array (NOTE: negative for MVD)
  double u;             ///< position of hit along axis orthogonal to front strips [cm]
  double v;             ///< position of hit along axis orthogonal to back strips [cm]
  double x;             ///< position of hit along x axis [cm]
  double y;             ///< position of hit along y axis [cm]
  double z;             ///< position of hit along z axis [cm]
  double dx;            ///< hit position uncertainty along x axis [cm]
  double dy;            ///< hit position uncertainty along y axis [cm]
  double dxy;           ///< hit position covariance along x and y axes [cm2]
  double du;            ///< hit position uncertainty along axis orthogonal to front strips [cm]
  double dv;            ///< hit position uncertainty along axis orthogonal to back strips [cm]
  double xmc;           ///< MC position of hit [cm]
  double ymc;           ///< MC position of hit [cm]
  double p;             ///< MC total momentum [GeV/c]
  double tx;            ///< MC slopes of the mc point
  double ty;            ///< MC slopes of the mc point
  int iMC;              ///< index of MCPoint in the fvMCPoints array
  double time = 0.;     ///< time of the hit [ns]
  double dt   = 1.e10;  ///< time error of the hit [ns]
  int Det;
  int id;  ///< index of hit before hits sorting
  int track;

  /// Provides comparison of two hits.
  /// If two hits belong to different stations,
  /// the smallest hit belongs to the station with the smallest index. Otherwise, the smallest hit
  /// has the smallest y coordinate
  /// \param  a  Left hit
  /// \param  b  Right hit
  /// \return boolean: true - the left hit is smaller then the right one
  static bool Compare(const TmpHit& a, const TmpHit& b)
  {
    return (a.iStation < b.iStation) || ((a.iStation == b.iStation) && (a.y < b.y));
  }

  /// Creates a hit from the CbmL1MCPoint object
  /// \param point  constant reference to the input MC-point
  /// \param det
  /// \param nTmpHits
  /// \param nStripF
  /// \param ip
  /// \param NStrips
  /// \param st       reference to the station info object
  void CreateHitFromPoint(const CbmL1MCPoint& point, int ip, int det, int nTmpHits, int& NStrips, const L1Station& st)
  {
    ExtIndex = 0;
    Det      = det;
    id       = nTmpHits;
    iStation = point.iStation;

    dt   = st.dt[0];
    time = point.time + gRandom->Gaus(0, dt);

    iStripF = NStrips;
    iStripB = iStripF;
    NStrips++;

    dx  = sqrt(st.XYInfo.C00[0]);
    dy  = sqrt(st.XYInfo.C11[0]);
    dxy = st.XYInfo.C10[0];

    du = sqrt(st.frontInfo.sigma2[0]);
    dv = sqrt(st.backInfo.sigma2[0]);

    std::tie(u, v) = st.ConvXYtoUV<double>(point.x, point.y);

    u += gRandom->Gaus(0, du);
    v += gRandom->Gaus(0, dv);

    std::tie(x, y) = st.ConvUVtoXY<double>(u, v);
    z              = point.z;

    xmc = point.x;
    ymc = point.y;

    track = point.ID;
    p     = point.p;
    iMC   = ip;
  }

  /// Sets randomized position and time of the hit
  /// The positions are smeared within predefined errors dx, dy, dt; z coordinate
  /// of the hit is known precisely
  /// \param point  constant reference to the input MC-point
  /// \param st     reference to the station info object
  // TODO: Probably, L1Station& st parameter should be constant. Do we really want to modify a station here? (S.Zharko)
  void SetHitFromPoint(const CbmL1MCPoint& point, const L1Station& st, bool doSmear = 1)
  {
    x    = 0.5 * (point.xIn + point.xOut);
    y    = 0.5 * (point.yIn + point.yOut);
    z    = 0.5 * (point.zIn + point.zOut);
    time = point.time;
    u    = x * st.frontInfo.cos_phi[0] + y * st.frontInfo.sin_phi[0];
    v    = x * st.backInfo.cos_phi[0] + y * st.backInfo.sin_phi[0];
    if (doSmear) {
      x += gRandom->Gaus(0, dx);
      y += gRandom->Gaus(0, dy);
      time += gRandom->Gaus(0, dt);
    }
  }
};

void CbmL1::ReadEvent(L1AlgoInputData* fData_, float& TsStart, float& TsLength, float& /*TsOverlap*/, int& FstHitinTs,
                      bool& areDataLeft, CbmEvent* event)
{
  if (fVerbose >= 10) cout << "ReadEvent: start." << endl;

  areDataLeft = false;  // no data left after reading the sub-timeslice

  fData_->Clear();

  // clear arrays for next event
  fvMCPoints.clear();                         /* <CbmL1MCPoint> */
  fvMCTracks.clear();                         /* <CbmL1MCTrack> */
  fvExternalHits.clear();                     /* <CbmL1Hit>     */
  fvRecoTracks.clear(); /* <CbmL1Track>   */  // TODO: Move from this function to more suitable place (S.Zharko)
  fvHitPointIndexes.clear();       /* <int>: indexes of MC-points in fvMCPoints (by index of fpAlgo->vHits) */
  fvHitStore.clear();              /* <CbmL1HitStore> */
  dFEI2vMCPoints.clear();          /* dFEI vs MC-point index: dFEI = index * 10000 + fileID + eventID * 0.0001 */
  dFEI2vMCTracks.clear();          /* dFEI vs MC-track index: dFEI = index * 10000 + fileID + eventID * 0.0001 */


  if (fVerbose >= 10) cout << "ReadEvent: clear is done." << endl;

  L1Vector<TmpHit> tmpHits("CbmL1ReadEvent::tmpHits");

  {  // reserve enough space for hits
    int nHitsTotal = 0;
    if (fUseMVD && fpMvdHits) nHitsTotal += fpMvdHits->GetEntriesFast();
    Int_t nEntSts = 0;
    if (fUseSTS && fpStsHits) {
      if (event) { nEntSts = event->GetNofData(ECbmDataType::kStsHit); }
      else {
        nEntSts = fpStsHits->GetEntriesFast();
      }
      if (nEntSts < 0) nEntSts = 0;  // GetNofData() can return -1;
    }
    nHitsTotal += nEntSts;
    if (fUseMUCH && fpMuchPixelHits) nHitsTotal += fpMuchPixelHits->GetEntriesFast();
    if (fUseTRD && fpTrdHits) nHitsTotal += fpTrdHits->GetEntriesFast();
    if (fUseTOF && fpTofHits) nHitsTotal += fpTofHits->GetEntriesFast();
    tmpHits.reserve(nHitsTotal);
  }

  // -- produce Sts hits from space points --

  for (int i = 0; i < fNStations; i++) {

    fData_->HitsStartIndex[i] = static_cast<L1HitIndex_t>(-1);
    fData_->HitsStopIndex[i]  = 0;
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

  /*
   * MC hits and tracks gathering: START
   *
   * If performance is studied, i.e. fPerformanse > 0, MC-information is used for
   * efficiencies calculation. Otherwise this 
   */

  if (fPerformance) {
    // Fill fvMCTracks and dFEI2vMCTracks
    Fill_vMCTracks();

    // Fill fvMCPoints and dFEI2vMCPoints
    fvMCPoints.clear();
    fvMCPoints.reserve(5 * fvMCTracks.size() * fNStations);
    fvMCPointIndexesTs.clear();
    fvMCPointIndexesTs.reserve(fvMCPoints.capacity());

    for (DFSET::iterator set_it = fvFileEvent.begin(); set_it != fvFileEvent.end(); ++set_it) {
      Int_t iFile  = set_it->first;
      Int_t iEvent = set_it->second;

      if (fUseMVD && fpMvdPoints) {
        Int_t nMvdPointsInEvent = fpMvdPoints->Size(iFile, iEvent);
        double maxDeviation     = 0;
        for (Int_t iMC = 0; iMC < nMvdPointsInEvent; iMC++) {
          CbmL1MCPoint MC;
          if (!ReadMCPoint(&MC, iMC, iFile, iEvent, 1)) {
            MC.iStation          = -1;
            const L1Station* sta = fpAlgo->GetParameters()->GetStations().begin();
            double bestDist      = 1.e20;
            for (Int_t iSt = 0; iSt < fNMvdStationsGeom; iSt++) {
              // use z_in since z_out is sometimes very wrong
              // due to a problem in transport
              int iStActive = fpAlgo->GetParameters()->GetStationIndexActive(iSt, L1DetectorID::kMvd);
              if (iStActive == -1) { continue; }
              double d = (MC.zIn - sta[iStActive].z[0]);
              if (fabs(d) < fabs(bestDist)) {
                bestDist    = d;
                MC.iStation = iStActive;
              }
            }
            assert(MC.iStation >= 0);
            if (fabs(maxDeviation) < fabs(bestDist)) { maxDeviation = bestDist; }
            Double_t dtrck          = dFEI(iFile, iEvent, MC.ID);
            DFEI2I::iterator trk_it = dFEI2vMCTracks.find(dtrck);
            assert(trk_it != dFEI2vMCTracks.end());
            MC.ID = trk_it->second;
            fvMCTracks[MC.ID].Points.push_back_no_warning(fvMCPoints.size());
            dFEI2vMCPoints.insert(DFEI2I::value_type(dFEI(iFile, iEvent, iMC), fvMCPoints.size()));
            fvMCPoints.push_back(MC);
            fvMCPointIndexesTs.push_back(0);
            nMvdPoints++;
          }
        }
        if (fVerbose > 2) { LOG(info) << "CbmL1ReadEvent: max deviation of Mvd points " << maxDeviation; }
        // ensure that the nominal station positions are not far from the sensors
        // assert(fabs(maxDeviation) < 1.);
      }  // fpMvdPoints

      firstStsPoint = fvMCPoints.size();
      if (fUseSTS && fpStsPoints) {
        Int_t nMC           = fpStsPoints->Size(iFile, iEvent);
        double maxDeviation = 0;
        for (Int_t iMC = 0; iMC < nMC; iMC++) {
          CbmL1MCPoint MC;
          if (!ReadMCPoint(&MC, iMC, iFile, iEvent, 0)) {
            MC.iStation          = -1;
            const L1Station* sta = fpAlgo->GetParameters()->GetStations().begin();
            double bestDist      = 1.e20;
            for (Int_t iSt = 0; iSt < fNStsStationsGeom; iSt++) {
              int iStActive = fpAlgo->GetParameters()->GetStationIndexActive(iSt, L1DetectorID::kSts);
              if (iStActive == -1) { continue; }
              // use z_in since z_out is sometimes very wrong
              // due to a problem in transport
              double d = (MC.zIn - sta[iStActive].z[0]);
              if (fabs(d) < fabs(bestDist)) {
                bestDist    = d;
                MC.iStation = iStActive;
              }
            }
            assert(MC.iStation >= 0);
            if (fabs(maxDeviation) < fabs(bestDist)) { maxDeviation = bestDist; }
            Double_t dtrck          = dFEI(iFile, iEvent, MC.ID);
            DFEI2I::iterator trk_it = dFEI2vMCTracks.find(dtrck);
            assert(trk_it != dFEI2vMCTracks.end());
            MC.ID = trk_it->second;
            fvMCTracks[MC.ID].Points.push_back_no_warning(fvMCPoints.size());
            dFEI2vMCPoints.insert(DFEI2I::value_type(dFEI(iFile, iEvent, iMC + nMvdPoints), fvMCPoints.size()));
            fvMCPoints.push_back(MC);
            fvMCPointIndexesTs.push_back(0);
            nStsPoints++;
          }
        }
        if (fVerbose > 2) { LOG(info) << "CbmL1ReadEvent: max deviation of Sts points " << maxDeviation; }
        // ensure that the nominal station positions are not far from the sensors
        //  assert(fabs(maxDeviation) < 1.);
      }  // fpStsPoints

      firstMuchPoint = fvMCPoints.size();
      if (fUseMUCH && fpMuchPoints) {
        Int_t nMC = fpMuchPoints->Size(iFile, iEvent);
        for (Int_t iMC = 0; iMC < nMC; iMC++) {
          CbmL1MCPoint MC;
          if (!ReadMCPoint(&MC, iMC, iFile, iEvent, 2)) {
            MC.iStation          = -1;
            const L1Station* sta = fpAlgo->GetParameters()->GetStations().begin();
            for (Int_t iSt = 0; iSt < fNMuchStationsGeom; iSt++) {
              int iStActive = fpAlgo->GetParameters()->GetStationIndexActive(iSt, L1DetectorID::kMuch);
              if (iStActive == -1) { continue; }
              if (MC.z > sta[iStActive].z[0] - 2.5) { MC.iStation = iStActive; }
            }
            assert(MC.iStation >= 0);
            Double_t dtrck          = dFEI(iFile, iEvent, MC.ID);
            DFEI2I::iterator trk_it = dFEI2vMCTracks.find(dtrck);
            assert(trk_it != dFEI2vMCTracks.end());
            MC.ID = trk_it->second;
            fvMCTracks[MC.ID].Points.push_back_no_warning(fvMCPoints.size());
            dFEI2vMCPoints.insert(
              DFEI2I::value_type(dFEI(iFile, iEvent, iMC + nMvdPoints + nStsPoints), fvMCPoints.size()));
            fvMCPoints.push_back(MC);
            fvMCPointIndexesTs.push_back(0);
            nMuchPoints++;
          }
        }
      }  // fpMuchPoints

      firstTrdPoint = fvMCPoints.size();

      if (fUseTRD && fpTrdPoints) {
        for (Int_t iMC = 0; iMC < fpTrdPoints->Size(iFile, iEvent); iMC++) {
          CbmL1MCPoint MC;
          if (!ReadMCPoint(&MC, iMC, iFile, iEvent, 3)) {
            MC.iStation          = -1;
            const L1Station* sta = fpAlgo->GetParameters()->GetStations().begin();
            for (Int_t iSt = 0; iSt < fNTrdStationsGeom; iSt++) {
              int iStActive = fpAlgo->GetParameters()->GetStationIndexActive(iSt, L1DetectorID::kTrd);
              if (iStActive == -1) { continue; }
              if (MC.z > sta[iStActive].z[0] - 4.0) { MC.iStation = iStActive; }
            }
            if (MC.iStation < 0) continue;
            assert(MC.iStation >= 0);
            Double_t dtrck          = dFEI(iFile, iEvent, MC.ID);
            DFEI2I::iterator trk_it = dFEI2vMCTracks.find(dtrck);
            assert(trk_it != dFEI2vMCTracks.end());
            MC.ID = trk_it->second;
            fvMCTracks[MC.ID].Points.push_back_no_warning(fvMCPoints.size());
            dFEI2vMCPoints.insert(
              DFEI2I::value_type(dFEI(iFile, iEvent, iMC + nMvdPoints + nStsPoints + nMuchPoints), fvMCPoints.size()));
            fvMCPoints.push_back(MC);
            fvMCPointIndexesTs.push_back(0);
            nTrdPoints++;
          }
        }
      }  // fpTrdPoints


      firstTofPoint = fvMCPoints.size();
      if (fUseTOF && fpTofPoints) {

        vector<float> TofPointToTrackdZ[fNTofStations];

        fTofPointToTrack.resize(fNTofStations);

        for (Int_t i = 0; i < fNTofStations; i++) {

          fTofPointToTrack[i].resize(fvMCTracks.size(), 0);
          TofPointToTrackdZ[i].resize(fvMCTracks.size());
        }

        for (int iSt = 0; iSt < fNTofStations; iSt++)
          for (unsigned int i = 0; i < TofPointToTrackdZ[iSt].size(); i++) {
            TofPointToTrackdZ[iSt][i] = 100000;
            fTofPointToTrack[iSt][i]  = -1;
          }

        std::vector<char> isTofPointMatched;
        isTofPointMatched.resize(fpTofPoints->Size(iFile, iEvent), 0);

        for (int iHit = 0; iHit < fpTofHits->GetEntriesFast(); iHit++) {
          CbmMatch* pHitMatch = L1_DYNAMIC_CAST<CbmMatch*>(fpTofHitMatches->At(iHit));
          for (int iLink = 0; iLink < pHitMatch->GetNofLinks(); iLink++) {
            Int_t iMC              = pHitMatch->GetLink(iLink).GetIndex();
            isTofPointMatched[iMC] = 1;
          }
        }


        for (Int_t iMC = 0; iMC < fpTofPoints->Size(iFile, iEvent); iMC++) {
          if (isTofPointMatched[iMC] == 0) continue;
          CbmL1MCPoint MC;
          if (!ReadMCPoint(&MC, iMC, iFile, iEvent, 4)) {
            Double_t dtrck = dFEI(iFile, iEvent, MC.ID);
            auto trk_it    = dFEI2vMCTracks.find(dtrck);
            if (trk_it == dFEI2vMCTracks.end()) continue;
            Int_t iTrack = trk_it->second;

            MC.iStation          = -1;
            const L1Station* sta = fpAlgo->GetParameters()->GetStations().begin();

            float dist = 1000;
            int iSta   = -1;
            for (int iSt = 0; iSt < fNTofStationsGeom; iSt++) {
              int iStActive = fpAlgo->GetParameters()->GetStationIndexActive(iSt, L1DetectorID::kTof);
              if (iStActive == -1) { continue; }
              if (fabs(MC.z - sta[iStActive].z[0]) < dist) {
                dist = fabs(MC.z - sta[iStActive].z[0]);
                iSta = iSt;
              }
            }
            MC.iStation = fpAlgo->GetParameters()->GetStationIndexActive(iSta, L1DetectorID::kTof);
            if (MC.iStation < 0) continue;
            assert(MC.iStation >= 0);

            if (iSta >= 0) {
              if (fabs(sta[iSta].z[0] - MC.z) < TofPointToTrackdZ[iSta][iTrack]) {
                fTofPointToTrack[iSta][iTrack]  = iMC;
                TofPointToTrackdZ[iSta][iTrack] = fabs(sta[iSta].z[0] - MC.z);
              }
            }
          }
        }

        for (int iTofSta = 0; iTofSta < fNTofStations; iTofSta++)
          for (unsigned int iTrack = 0; iTrack < fTofPointToTrack[iTofSta].size(); iTrack++) {

            if (fTofPointToTrack[iTofSta][iTrack] < 0) continue;

            CbmL1MCPoint MC;

            if (!ReadMCPoint(&MC, fTofPointToTrack[iTofSta][iTrack], iFile, iEvent, 4)) {

              MC.iStation = (fNMvdStations + fNStsStations + fNMuchStations + fNTrdStations + iTofSta);
              fvMCTracks[iTrack].Points.push_back_no_warning(fvMCPoints.size());

              MC.ID = iTrack;

              dFEI2vMCPoints.insert(DFEI2I::value_type(
                dFEI(iFile, iEvent,
                     fTofPointToTrack[iTofSta][iTrack] + nMvdPoints + nStsPoints + nMuchPoints + nTrdPoints),
                fvMCPoints.size()));

              fvMCPoints.push_back(MC);

              fvMCPointIndexesTs.push_back(0);
              nTofPoints++;
            }
          }
      }
    }  //time_slice

    for (unsigned int iTr = 0; iTr < fvMCTracks.size(); iTr++) {

      sort(fvMCTracks[iTr].Points.begin(), fvMCTracks[iTr].Points.end(), compareZ);

      if (fvMCTracks[iTr].mother_ID >= 0) {
        Double_t dtrck          = dFEI(fvMCTracks[iTr].iFile, fvMCTracks[iTr].iEvent, fvMCTracks[iTr].mother_ID);
        DFEI2I::iterator map_it = dFEI2vMCTracks.find(dtrck);
        if (map_it == dFEI2vMCTracks.end()) fvMCTracks[iTr].mother_ID = -2;
        else
          fvMCTracks[iTr].mother_ID = map_it->second;
      }
    }  //iTr
    if (fVerbose >= 10) cout << "Points in fvMCTracks are sorted." << endl;

  }  //fPerformance

  /*
   * MC hits and tracks gathering: END
   */

  /*
   * Measured hits gathering: START
   *
   * In this section the measured hits from different detector subsystems are reformatted according to TmpHit structure
   * (NOTE: independent from particular detector design) and then pushed to the tmpHit vector. In the performance study mode
   * matching with MC points is done
   */

  // TODO: Refactor this code: MC information and matches should be collected separately in a separate class (S.Zharko)

  int NStrips = 0;
  //
  // get MVD hits
  if (fUseMVD && fpMvdHits) {

    int firstDetStrip = NStrips;

    for (int j = 0; j < fpMvdHits->GetEntriesFast(); j++) {
      TmpHit th;
      {
        CbmMvdHit* mh = L1_DYNAMIC_CAST<CbmMvdHit*>(fpMvdHits->At(j));
        th.ExtIndex   = -(1 + j);
        th.id         = tmpHits.size();
        th.iStation   = mh->GetStationNr();

        int stIdx = fpAlgo->GetParameters()->GetStationIndexActive(mh->GetStationNr(), L1DetectorID::kMvd);
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

        const L1Station& st = fpAlgo->GetParameters()->GetStation(th.iStation);
        th.u                = th.x * st.frontInfo.cos_phi[0] + th.y * st.frontInfo.sin_phi[0];
        th.v                = th.x * st.backInfo.cos_phi[0] + th.y * st.backInfo.sin_phi[0];
      }
      th.Det = 0;
      th.iMC = -1;
      if (fPerformance) {
        if (fpMvdHitMatches) {
          CbmMatch* mvdHitMatch = L1_DYNAMIC_CAST<CbmMatch*>(fpMvdHitMatches->At(j));
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
  }    // if fpMvdHits
  if (fVerbose >= 10) cout << "ReadEvent: MVD hits collected." << endl;

  if (fUseSTS && (2 == fStsUseMcHit)) {  // create hits from points

    for (int ip = firstStsPoint; ip < firstStsPoint + nStsPoints; ip++) {
      const CbmL1MCPoint& p = fvMCPoints[ip];
      //       int mcTrack           = p.ID;
      //       if (mcTrack < 0) continue;
      //       const CbmL1MCTrack& t = fvMCTracks[mcTrack];
      //if (t.p < 1) continue;
      // if (t.Points.size() > 4) continue;
      // cout << "sts mc: station " << p.iStation - fNMvdStations << " x " << p.x << " y " << p.y << " z " << p.z << " t "
      //            << p.time << " mc " << p.ID << " p " << p.p << endl;
      TmpHit th;
      int DetId = 1;
      th.CreateHitFromPoint(p, ip, DetId, tmpHits.size(), NStrips, fpAlgo->GetParameters()->GetStation(p.iStation));
      tmpHits.push_back(th);
      nStsHits++;
    }
  }

  //
  // Get STS hits
  //
  if (fUseSTS && fpStsHits && (2 != fStsUseMcHit)) {

    Int_t nEntSts = (event ? event->GetNofData(ECbmDataType::kStsHit) : fpStsHits->GetEntriesFast());

    int firstDetStrip = NStrips;

    if (event) FstHitinTs = 0;

    for (Int_t j = FstHitinTs; j < nEntSts; j++) {
      Int_t hitIndex = 0;
      hitIndex       = (event ? event->GetIndex(ECbmDataType::kStsHit, j) : j);

      int hitIndexSort = 0;
      if (!fLegacyEventMode) hitIndexSort = fvSortedStsHitsIndexes[hitIndex];
      else
        hitIndexSort = j;

      CbmStsHit* sh = L1_DYNAMIC_CAST<CbmStsHit*>(fpStsHits->At(hitIndexSort));

      // ***********************************
      // ** Fill the temporary hit object **
      // ***********************************

      TmpHit th;

      // Fill reconstructed information
      {
        CbmStsHit* mh = L1_DYNAMIC_CAST<CbmStsHit*>(fpStsHits->At(hitIndexSort));
        th.ExtIndex   = hitIndexSort;
        th.Det        = 1;
        int stIdx     = fpAlgo->GetParameters()->GetStationIndexActive(
          CbmStsSetup::Instance()->GetStationNumber(mh->GetAddress()), L1DetectorID::kSts);

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

        const L1Station& st = fpAlgo->GetParameters()->GetStation(th.iStation);
        th.u                = th.x * st.frontInfo.cos_phi[0] + th.y * st.frontInfo.sin_phi[0];
        th.v                = th.x * st.backInfo.cos_phi[0] + th.y * st.backInfo.sin_phi[0];
      }
      th.iMC = -1;

      Int_t iMC = -1;

      if (fPerformance) {
        /// TODO: Matching should be done in CbmMatchRecoToMC (S.Zharko)
        if (fpStsClusterMatches) {
          const CbmMatch* frontClusterMatch =
            static_cast<const CbmMatch*>(fpStsClusterMatches->At(sh->GetFrontClusterId()));
          const CbmMatch* backClusterMatch =
            static_cast<const CbmMatch*>(fpStsClusterMatches->At(sh->GetBackClusterId()));
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
  }    // if fpStsHits

  //
  // Get MuCh hits
  //
  if ((2 == fMuchUseMcHit) && fUseMUCH) {  // create hits from points

    for (int ip = firstMuchPoint; ip < firstMuchPoint + nMuchPoints; ip++) {
      const CbmL1MCPoint& p = fvMCPoints[ip];

      //       int mcTrack = p.ID;
      //       if (mcTrack < 0) continue;
      //       const CbmL1MCTrack& t = fvMCTracks[mcTrack];
      //if (t.p < 1) continue;
      // if (t.Points.size() > 4) continue;

      TmpHit th;
      int DetId = 2;
      th.CreateHitFromPoint(p, ip, DetId, tmpHits.size(), NStrips, fpAlgo->GetParameters()->GetStation(p.iStation));

      tmpHits.push_back(th);
      nMuchHits++;
    }
  }

  if (fUseMUCH && fpMuchPixelHits && (2 != fMuchUseMcHit)) {

    Int_t nEnt = fpMuchPixelHits->GetEntriesFast();

    int firstDetStrip = NStrips;

    for (int j = 0; j < nEnt; j++) {
      TmpHit th;
      {

        CbmMuchPixelHit* mh = static_cast<CbmMuchPixelHit*>(fpMuchPixelHits->At(j));

        th.ExtIndex = j;
        th.Det      = 2;
        th.id       = tmpHits.size();


        Int_t stationNumber = CbmMuchGeoScheme::GetStationIndex(mh->GetAddress());
        Int_t layerNumber   = CbmMuchGeoScheme::GetLayerIndex(mh->GetAddress());

        int DetId = stationNumber * 3 + layerNumber;

        int stIdx = fpAlgo->GetParameters()->GetStationIndexActive(DetId, L1DetectorID::kMuch);
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


        const L1Station& st = fpAlgo->GetParameters()->GetStation(th.iStation);
        th.u                = th.x * st.frontInfo.cos_phi[0] + th.y * st.frontInfo.sin_phi[0];
        th.v                = th.x * st.backInfo.cos_phi[0] + th.y * st.backInfo.sin_phi[0];
      }
      th.iMC  = -1;
      int iMC = -1;


      if (fPerformance) {
        if (fpMuchHitMatches) {
          CbmMatch* matchHitMatch = L1_DYNAMIC_CAST<CbmMatch*>(fpMuchHitMatches->At(j));


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
                th.SetHitFromPoint(fvMCPoints[th.iMC], fpAlgo->GetParameters()->GetStation(th.iStation));
            }
          }
        }
      }

      tmpHits.push_back(th);
      nMuchHits++;

    }  // for j
  }    // if listMuchHits

  //
  // Get TRD hits
  //

  if (fUseTRD) {

    if (2 == fTrdUseMcHit) {  // create hits from MC points
      for (int ip = firstTrdPoint; ip < firstTrdPoint + nTrdPoints; ip++) {
        const CbmL1MCPoint& p = fvMCPoints[ip];
        //       int mcTrack = p.ID;
        //       if (mcTrack < 0) continue;
        //       const CbmL1MCTrack& t = fvMCTracks[mcTrack];
        TmpHit th;
        int DetId = 3;
        th.CreateHitFromPoint(p, ip, DetId, tmpHits.size(), NStrips, fpAlgo->GetParameters()->GetStation(p.iStation));
        tmpHits.push_back(th);
        nTrdHits++;
      }
    }
    else {  // read hits

      assert(fpTrdHits);

      vector<bool> mcUsed(nTrdPoints, 0);

      for (int iHit = 0; iHit < fpTrdHits->GetEntriesFast(); iHit++) {
        TmpHit th;

        CbmTrdHit* mh = L1_DYNAMIC_CAST<CbmTrdHit*>(fpTrdHits->At(iHit));

        if ((L1Algo::TrackingMode::kGlobal == fTrackingMode) && (int) mh->GetClassType() != 1) {
          // SGtrd2d!! skip TRD-1D hit
          continue;
        }

        th.ExtIndex = iHit;
        th.Det      = 3;
        th.id       = tmpHits.size();

        int stIdx = fpAlgo->GetParameters()->GetStationIndexActive(mh->GetPlaneId(), L1DetectorID::kTrd);
        if (stIdx == -1) continue;

        th.iStation = stIdx;

        //  if (mh->GetPlaneId()==0) continue;
        //  if (mh->GetPlaneId()==1) continue;
        //  if (mh->GetPlaneId()==2) continue;
        //  if (mh->GetPlaneId()==3) continue;

        th.time = mh->GetTime();
        th.dt   = mh->GetTimeError();

        //   th.iSector  = 0;
        th.iStripF = NStrips;
        th.iStripB = th.iStripF;  //TrdHitsOnStationIndex[num+1][k];
        NStrips++;

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

        const L1Station& st = fpAlgo->GetParameters()->GetStation(th.iStation);

        std::tie(th.u, th.v) = st.ConvXYtoUV<double>(th.x, th.y);

        th.iMC     = -1;
        th.track   = -1;
        int iMcTrd = -1;
        if (fPerformance && fpTrdHitMatches) {
          CbmMatch* trdHitMatch = L1_DYNAMIC_CAST<CbmMatch*>(fpTrdHitMatches->At(iHit));
          if (trdHitMatch->GetNofLinks() > 0) {
            iMcTrd = trdHitMatch->GetLink(0).GetIndex();
            assert(iMcTrd >= 0 && iMcTrd < nTrdPoints);
            th.iMC   = iMcTrd + nMvdPoints + nStsPoints + nMuchPoints;
            th.track = fvMCPoints[th.iMC].ID;
          }
        }

        if (1 == fTrdUseMcHit) {  // replace hit by MC points

          assert(fPerformance && fpTrdHitMatches);

          if (th.iMC < 0) continue;      // skip noise hits
          if (mcUsed[iMcTrd]) continue;  // one hit per MC point
          bool doSmear = true;
          if (0) {  // SGtrd2d!! debug: artificial errors
            th.dx = .1;
            th.du = .1;
            th.dy = .1;
            th.dv = .1;
          }
          th.SetHitFromPoint(fvMCPoints[th.iMC], fpAlgo->GetParameters()->GetStation(th.iStation), doSmear);
          mcUsed[iMcTrd] = 1;
        }  // replace hit by MC points

        if (0) {  // select specific tracks
          int mcTrack = -1;
          float mcP   = 0;
          if (th.iMC >= 0) {
            mcTrack = fvMCPoints[th.iMC].ID;
            if (mcTrack >= 0) { mcP = fvMCTracks[mcTrack].p; }
          }
          if (mcP < 1.) continue;
        }

        tmpHits.push_back(th);
        nTrdHits++;
      }  // for fpTrdHits
    }
  }  // read TRD hits

  //
  // Get ToF hits
  //
  if ((2 == fTofUseMcHit) && fUseTOF) {  // create hits from points
    for (int ip = firstTofPoint; ip < firstTofPoint + nTofPoints; ip++) {
      const CbmL1MCPoint& p = fvMCPoints[ip];

      //       int mcTrack = p.ID;
      //  if (mcTrack < 0) continue;
      //const CbmL1MCTrack& t = fvMCTracks[mcTrack];
      //if (t.p < 1) continue;
      //if (t.Points.size() > 4) continue;

      TmpHit th;
      int DetId = 4;
      th.CreateHitFromPoint(p, ip, DetId, tmpHits.size(), NStrips, fpAlgo->GetParameters()->GetStation(p.iStation));
      tmpHits.push_back(th);
      nTofHits++;
    }
  }


  if (fUseTOF && fpTofHits && (2 != fTofUseMcHit)) {

    int firstDetStrip = NStrips;

    for (int j = 0; j < fpTofHits->GetEntriesFast(); j++) {
      TmpHit th;

      CbmTofHit* mh = L1_DYNAMIC_CAST<CbmTofHit*>(fpTofHits->At(j));


      th.ExtIndex = j;
      th.Det      = 4;

      th.id = tmpHits.size();

      if (0x00202806 == mh->GetAddress() || 0x00002806 == mh->GetAddress()) continue;  // TODO: Why? (S.Zharko)

      int sttof = CbmTofTrackingInterface::Instance()->GetTrackingStationIndex(mh);

      if (sttof < 0) continue;

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

      if (th.z > 400) continue;  // is it still needed here? (S.Zharko)

      int stIdx = fpAlgo->GetParameters()->GetStationIndexActive(sttof, L1DetectorID::kTof);
      if (stIdx == -1) continue;
      th.iStation = stIdx;

      const L1Station& st = fpAlgo->GetParameters()->GetStation(th.iStation);
      th.u                = th.x * st.frontInfo.cos_phi[0] + th.y * st.frontInfo.sin_phi[0];
      th.v                = th.x * st.backInfo.cos_phi[0] + th.y * st.backInfo.sin_phi[0];


      th.iMC = -1;
      //      int iMC = -1;


      if (fPerformance) {

        CbmMatch* matchHitMatch = L1_DYNAMIC_CAST<CbmMatch*>(fpTofHitMatches->At(j));

        for (int iLink = 0; iLink < matchHitMatch->GetNofLinks(); iLink++)  //matchHitMatch->GetNofLinks(); k++)
        {
          Int_t iMC    = matchHitMatch->GetLink(iLink).GetIndex();
          Int_t iFile  = matchHitMatch->GetLink(iLink).GetFile();
          Int_t iEvent = matchHitMatch->GetLink(iLink).GetEntry();

          Int_t iIndex = iMC + nMvdPoints + nStsPoints + nMuchPoints + nTrdPoints;

          //CbmTofPoint* pt = L1_DYNAMIC_CAST<CbmTofPoint*>(fTofPoints->Get(iFile, iEvent, iMC));
          // pt->GetTrackID();
          Double_t dtrck          = dFEI(iFile, iEvent, iIndex);
          DFEI2I::iterator trk_it = dFEI2vMCPoints.find(dtrck);
          if (trk_it == dFEI2vMCPoints.end()) continue;

          th.iMC = trk_it->second;
          if ((1 == fTofUseMcHit) && (th.iMC > -1))
            th.SetHitFromPoint(fvMCPoints[th.iMC], fpAlgo->GetParameters()->GetStation(th.iStation));
        }
      }

      tmpHits.push_back(th);
      nTofHits++;

    }  // for j
  }    // if listTofHits

  if (fVerbose > 1) {
    LOG(info) << "L1 ReadEvent: nhits mvd " << nMvdHits << " sts " << nStsHits << " much " << nMuchHits << " trd "
              << nTrdHits << " tof " << nTofHits << endl;
  }

  // total number of hits
  int nHits = nMvdHits + nStsHits + nMuchHits + nTrdHits + nTofHits;

  /*
   * Measured hits gathering: END
   */


  //
  //  Hits sorting
  //
  //  Two hits are compared as follows. If the hits are measured with two different stations, the smallest hit has the smallest
  //  station ID. If the hits are measured within one station, the smallest hit has the smallest y position coordinate.
  //
  sort(tmpHits.begin(), tmpHits.end(), TmpHit::Compare);

  // ----- Save strips into L1Algo
  fData_->fNstrips = NStrips;
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


  // ----- Fill and save fvExternalHits, fvHitStore and fvHitPointIndexes vectors as well as fpData->vHits
  int nEffHits = 0;

  fvSortedHitsIndexes.reset(maxHitIndex + 1);

  fvExternalHits.reserve(nHits);
  fData_->vHits.reserve(nHits);

  fvHitStore.reserve(nHits);
  fvHitPointIndexes.reserve(nHits);


  // ----- Fill
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

    fvSortedHitsIndexes[th.id] = i;

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
    h.u  = th.u;
    h.v  = th.v;
    //    h.dxy = th.dxy;
    //     h.p = th.p;
    //     h.q = th.q;
    // h.ista = th.iStation;

    h.z = th.z;


    // save hit
    fvExternalHits.push_back(CbmL1Hit(fpData->vHits.size(), th.ExtIndex, th.Det));

    fvExternalHits[fvExternalHits.size() - 1].x = th.x;
    fvExternalHits[fvExternalHits.size() - 1].y = th.y;
    fvExternalHits[fvExternalHits.size() - 1].t = th.time;

    fvExternalHits[fvExternalHits.size() - 1].ID = th.id;

    fvExternalHits[fvExternalHits.size() - 1].f = th.iStripF;
    fvExternalHits[fvExternalHits.size() - 1].b = th.iStripB;


    fData_->vHits.push_back(h);

    int iSt = th.iStation;

    if (fData_->HitsStartIndex[iSt] == static_cast<L1HitIndex_t>(-1)) fData_->HitsStartIndex[iSt] = nEffHits;
    nEffHits++;

    fData_->HitsStopIndex[iSt] = nEffHits;

    fvHitStore.push_back(s);
    fvHitPointIndexes.push_back(th.iMC);
  }

  for (int i = 0; i < fNStations; i++) {
    if (fData_->HitsStartIndex[i] == static_cast<L1HitIndex_t>(-1)) {
      fData_->HitsStartIndex[i] = fData_->HitsStopIndex[i];
    }
  }

  if (fVerbose >= 10) cout << "ReadEvent: mvd and sts are saved." << endl;

  /*
   * Translate gathered hits data to the L1Algo object. TODO: raplace it with L1DataManager functionality (S.Zharko)
   */

  fpAlgo->SetData(fData_->GetHits(), fData_->GetNstrips(), fData_->GetSFlag(), fData_->GetHitsStartIndex(),
                  fData_->GetHitsStopIndex());

  if (fPerformance) {
    if (fVerbose >= 10) cout << "HitMatch is done." << endl;
    if (fVerbose >= 10) cout << "MCPoints and MCTracks are saved." << endl;
  }
  if (fVerbose >= 10) cout << "ReadEvent is done." << endl;
}  // void CbmL1::ReadEvent()
//
//--------------------------------------------------------------------------------------------------
//
void CbmL1::Fill_vMCTracks()
{
  fvMCTracks.clear();

  // Count the total number of tracks in this event and reserve memory
  {
    Int_t nMCTracks = 0;
    for (DFSET::iterator set_it = fvFileEvent.begin(); set_it != fvFileEvent.end(); ++set_it) {
      Int_t iFile  = set_it->first;
      Int_t iEvent = set_it->second;
      nMCTracks += fpMCTracks->Size(iFile, iEvent);
    }
    fvMCTracks.reserve(nMCTracks);
  }

  int fileEvent = 0;
  /* Loop over MC event */
  for (DFSET::iterator set_it = fvFileEvent.begin(); set_it != fvFileEvent.end(); ++set_it, ++fileEvent) {
    Int_t iFile  = set_it->first;
    Int_t iEvent = set_it->second;

    auto header = dynamic_cast<FairMCEventHeader*>(fpMcEventHeader->Get(iFile, iEvent));
    assert(header);

    Int_t nMCTrack = fpMCTracks->Size(iFile, iEvent);
    /* Loop over MC tracks */
    for (Int_t iMCTrack = 0; iMCTrack < nMCTrack; iMCTrack++) {
      CbmMCTrack* MCTrack = L1_DYNAMIC_CAST<CbmMCTrack*>(fpMCTracks->Get(iFile, iEvent, iMCTrack));
      if (!MCTrack) { continue; }

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
      // TODO: Add light nuclei (d, t, He3, He4): they are common in tracking but not accounted in TDatabasePDG (S.Zharko)

      //cout << "mc track " << iMCTrack << " pdg " << pdg << " z " << vr.Z() << endl;

      Int_t iTrack = fvMCTracks.size();  //or iMCTrack?
      CbmL1MCTrack T(mass, q, vr, vp, iTrack, mother_ID, pdg);
      //        CbmL1MCTrack T(mass, q, vr, vp, iMCTrack, mother_ID, pdg);
      T.time   = MCTrack->GetStartT();
      T.iFile  = iFile;
      T.iEvent = iEvent;
      // signal: primary tracks, displaced from the primary vertex
      T.isSignal = T.IsPrimary() && (T.z > header->GetZ() + 1.e-10);

      fvMCTracks.push_back(T);
      //    Double_t dtrck =dFEI(iFile,iEvent,iMCTrack);
      dFEI2vMCTracks.insert(DFEI2I::value_type(dFEI(iFile, iEvent, iMCTrack), fvMCTracks.size() - 1));
    }  //iTracks
  }    //Links

}  //Fill_vMCTracks
//
//--------------------------------------------------------------------------------------------------
//
// TODO: Probably, we should reduce code here, rewriting this funciton as a template from CbmMvdPoint (S.Zharko)
bool CbmL1::ReadMCPoint(CbmL1MCPoint* MC, int iPoint, int file, int event, int MVD)
{
  TVector3 xyzI, PI, xyzO, PO;
  Int_t mcID    = -1;
  Double_t time = 0.f;

  if (MVD == 1) {
    CbmMvdPoint* pt = L1_DYNAMIC_CAST<CbmMvdPoint*>(fpMvdPoints->Get(file, event, iPoint));  // file, event, object
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
    CbmStsPoint* pt = L1_DYNAMIC_CAST<CbmStsPoint*>(fpStsPoints->Get(file, event, iPoint));  // file, event, object
    if (!pt) return 1;
    if (!fLegacyEventMode) {
      Double_t StartTime     = fTimeSlice->GetStartTime();
      Double_t EndTime       = fTimeSlice->GetEndTime();
      Double_t Time_MC_point = pt->GetTime() + fpEventList->GetEventTime(event, file);

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
    CbmMuchPoint* pt = L1_DYNAMIC_CAST<CbmMuchPoint*>(fpMuchPoints->Get(file, event, iPoint));  // file, event, object
    if (!pt) return 1;
    if (!fLegacyEventMode) {
      Double_t StartTime     = fTimeSlice->GetStartTime();
      Double_t EndTime       = fTimeSlice->GetEndTime();
      Double_t Time_MC_point = pt->GetTime() + fpEventList->GetEventTime(event, file);

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
    CbmTrdPoint* pt = L1_DYNAMIC_CAST<CbmTrdPoint*>(fpTrdPoints->Get(file, event, iPoint));  // file, event, object

    if (!pt) return 1;
    if (!fLegacyEventMode) {
      Double_t StartTime     = fTimeSlice->GetStartTime();                             // not used
      Double_t EndTime       = fTimeSlice->GetEndTime();                               // not used
      Double_t Time_MC_point = pt->GetTime() + fpEventList->GetEventTime(event, file);  // not used

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
    CbmTofPoint* pt = L1_DYNAMIC_CAST<CbmTofPoint*>(fpTofPoints->Get(file, event, iPoint));  // file, event, object
    if (!pt) return 1;
    if (!fLegacyEventMode) {
      Double_t StartTime     = fTimeSlice->GetStartTime();
      Double_t EndTime       = fTimeSlice->GetEndTime();
      Double_t Time_MC_point = pt->GetTime() + fpEventList->GetEventTime(event, file);

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
  CbmMCTrack* MCTrack = L1_DYNAMIC_CAST<CbmMCTrack*>(fpMCTracks->Get(file, event, MC->ID));
  if (!MCTrack) return 1;
  MC->pdg       = MCTrack->GetPdgCode();
  MC->mother_ID = MCTrack->GetMotherId();

  TParticlePDG* particlePDG = TDatabasePDG::Instance()->GetParticle(MC->pdg);
  if (particlePDG) {
    MC->q    = particlePDG->Charge() / 3.0;
    MC->mass = particlePDG->Mass();
  }
  // TODO: Add light nuclei (d, t, He3, He4): they are common in tracking but not accounted in TDatabasePDG (S.Zharko)

  return 0;
}
//
//--------------------------------------------------------------------------------------------------
//
bool CbmL1::ReadMCPoint(CbmL1MCPoint* /*MC*/, int /*iPoint*/, int /*MVD*/) { return 0; }
//
//--------------------------------------------------------------------------------------------------
//
// TODO: Duplicated code (from CbmL1::ReadEvent)
void CbmL1::HitMatch()
{
  const int NHits = fvExternalHits.size();
  for (int iH = 0; iH < NHits; iH++) {
    CbmL1Hit& hit = fvExternalHits[iH];

    if ((hit.Det == 1) && (2 != fStsUseMcHit)) {
      CbmStsHit* sh = L1_DYNAMIC_CAST<CbmStsHit*>(fpStsHits->At(fvExternalHits[iH].extIndex));

      int iP = -1;

      if (fpStsClusterMatches) {

        const CbmMatch* frontClusterMatch =
          static_cast<const CbmMatch*>(fpStsClusterMatches->At(sh->GetFrontClusterId()));
        const CbmMatch* backClusterMatch =
          static_cast<const CbmMatch*>(fpStsClusterMatches->At(sh->GetBackClusterId()));
        CbmMatch stsHitMatch;

        Float_t Sum_of_front = 0;  // total weight of all front links
        Float_t Sum_of_back  = 0;  // total weight of all back links


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
            iFile  = fvFileEvent.begin()->first;
            iEvent = fvFileEvent.begin()->second;
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
        fvMCPoints[iP].hitIds.push_back_no_warning(iH);
      }
      else {
        int idPoint = fvHitPointIndexes[iH];
        if (idPoint >= 0) {
          hit.mcPointIds.push_back_no_warning(idPoint);
          fvMCPoints[idPoint].hitIds.push_back_no_warning(iH);
        }
      }  // if no clusters
    }

    if ((hit.Det != 1) || (2 == fStsUseMcHit)) {  // the hit is not from STS
      int iP = fvHitPointIndexes[iH];
      if (iP >= 0) {
        hit.mcPointIds.push_back_no_warning(iP);
        fvMCPoints[iP].hitIds.push_back_no_warning(iH);
      }
    }

  }  // for hits
}
