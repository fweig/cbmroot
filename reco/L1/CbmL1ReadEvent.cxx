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

//#include "CbmMvdHitMatch.h"


#include "TDatabasePDG.h"
#include "TRandom.h"

#include <iostream>

using std::cout;
using std::endl;
using std::find;


static bool compareMcPointZ(const int& a, const int& b)
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
  void CreateHitFromPoint(const CbmL1MCPoint& point, int ip, int det, int nTmpHits, int& NStrips, const L1Station& st,
                          double du_, double dv_, double dt_, bool doSmear)
  {
    ExtIndex = 0;
    Det      = det;
    id       = nTmpHits;
    iStation = point.iStation;

    dt   = dt_;
    time = point.time;

    iStripF = NStrips;
    iStripB = iStripF;
    NStrips++;

    du = du_;
    dv = dv_;

    double c00, c10, c11;
    std::tie(c00, c10, c11) = st.FormXYCovarianceMatrix(du_ * du_, dv_ * dv_);

    dx  = sqrt(c00);
    dy  = sqrt(c11);
    dxy = c10;

    SetHitFromPoint(point, ip, st, doSmear);
  }

  /// Sets randomized position and time of the hit
  /// The positions are smeared within predefined errors dx, dy, dt; z coordinate
  /// of the hit is known precisely
  /// \param point  constant reference to the input MC-point
  /// \param st     reference to the station info object
  // TODO: Probably, L1Station& st parameter should be constant. Do we really want to modify a station here? (S.Zharko)
  void SetHitFromPoint(const CbmL1MCPoint& point, int ip, const L1Station& st, bool doSmear = 1)
  {
    std::tie(u, v) = st.ConvXYtoUV<double>(point.x, point.y);
    if (doSmear) {
      u += CbmL1::boundedGaus(du);
      v += CbmL1::boundedGaus(dv);
      time += CbmL1::boundedGaus(dt);
    }
    std::tie(x, y) = st.ConvUVtoXY<double>(u, v);
    z              = point.z;

    xmc   = point.x;
    ymc   = point.y;
    track = point.ID;
    p     = point.p;
    iMC   = ip;
  }
};


// *****************************************
// **     Hits and MC-points matching     **
// *****************************************

// ---------------------------------------------------------------------------------------------------------------------
//
template<>
int CbmL1::MatchHitWithMc<L1DetectorID::kMvd>(int iHit) const
{
  int iPoint = -1;
  if (fpMvdHitMatches) {
    int iHitExt          = -(1 + iHit);  // TODO: SZh 28.08.2022: this should be replaced with iHitExt = hit.extIdex
    const auto* hitMatch = dynamic_cast<CbmMatch*>(fpMvdHitMatches->At(iHitExt));
    assert(hitMatch);
    if (hitMatch->GetNofLinks() > 0 && hitMatch->GetLink(0).GetIndex() < fNpointsMvd) {
      iPoint = hitMatch->GetLink(0).GetIndex();
    }
  }
  return iPoint;
}

// ---------------------------------------------------------------------------------------------------------------------
//
template<>
int CbmL1::MatchHitWithMc<L1DetectorID::kSts>(int iHit) const
{
  int iPoint     = -1;
  const auto* sh = dynamic_cast<CbmStsHit*>(fpStsHits->At(iHit));

  // Match MC point
  if (fpStsClusterMatches) {
    const auto* clusterMatchF = static_cast<const CbmMatch*>(fpStsClusterMatches->At(sh->GetFrontClusterId()));
    const auto* clusterMatchB = static_cast<const CbmMatch*>(fpStsClusterMatches->At(sh->GetBackClusterId()));
    CbmMatch hitMatch;
    for (int iLinkF = 0; iLinkF < clusterMatchF->GetNofLinks(); ++iLinkF) {
      const auto& linkF = clusterMatchF->GetLink(iLinkF);
      for (int iLinkB = 0; iLinkB < clusterMatchB->GetNofLinks(); ++iLinkB) {
        const auto& linkB = clusterMatchB->GetLink(iLinkB);
        if (linkF == linkB) {
          hitMatch.AddLink(linkF);
          hitMatch.AddLink(linkB);
        }
      }
    }
    float bestWeight = 0.f;
    for (int iLink = 0; iLink < hitMatch.GetNofLinks(); ++iLink) {
      const CbmLink& link = hitMatch.GetLink(iLink);

      int iIndex = link.GetIndex();
      if (iIndex < 0) {
        iPoint = -1;
        break;
      }
      int iFile  = link.GetFile();
      int iEvent = link.GetEntry();

      if (fLegacyEventMode) {
        iFile  = fvFileEvent.begin()->first;
        iEvent = fvFileEvent.begin()->second;
      }

      auto itPoint = fmMCPointsLinksMap.find(CbmL1LinkKey(iIndex + fNpointsMvdAll, iEvent, iFile));
      assert(itPoint != fmMCPointsLinksMap.cend());

      if (link.GetWeight() > bestWeight) {
        bestWeight = link.GetWeight();
        iPoint     = itPoint->second;
      }
    }
  }  // Match MC point
  return iPoint;
}

// ---------------------------------------------------------------------------------------------------------------------
//
template<>
int CbmL1::MatchHitWithMc<L1DetectorID::kMuch>(int iHit) const
{
  int iPoint               = -1;
  const auto* hitMatchMuch = dynamic_cast<CbmMatch*>(fpMuchHitMatches->At(iHit));
  if (hitMatchMuch) {
    for (int iLink = 0; iLink < hitMatchMuch->GetNofLinks(); ++iLink) {
      if (hitMatchMuch->GetLink(iLink).GetIndex() < fNpointsMuch) {
        int iMc = hitMatchMuch->GetLink(iLink).GetIndex();
        if (iMc < 0) {
          iPoint = -1;
          break;
        }

        int iFile  = hitMatchMuch->GetLink(iLink).GetFile();
        int iEvent = hitMatchMuch->GetLink(iLink).GetEntry();

        auto itPoint = fmMCPointsLinksMap.find(CbmL1LinkKey(iMc + fNpointsMvdAll + fNpointsStsAll, iEvent, iFile));
        assert(itPoint != fmMCPointsLinksMap.cend());
        iPoint = itPoint->second;
      }
    }
  }
  return iPoint;
}

// ---------------------------------------------------------------------------------------------------------------------
//
template<>
int CbmL1::MatchHitWithMc<L1DetectorID::kTrd>(int iHit) const
{
  int iPoint           = -1;
  const auto* hitMatch = dynamic_cast<const CbmMatch*>(fpTrdHitMatches->At(iHit));
  if (hitMatch) {
    int iMC = -1;
    if (hitMatch->GetNofLinks() > 0) {
      iMC = hitMatch->GetLink(0).GetIndex();
      assert(iMC >= 0 && iMC < fNpointsTrd);

      int iMc = hitMatch->GetLink(0).GetIndex();
      if (iMc < 0) return iPoint;
      int iIndex = iMc + fNpointsMvdAll + fNpointsStsAll + fNpointsMuchAll;
      int iFile  = hitMatch->GetLink(0).GetFile();
      int iEvent = hitMatch->GetLink(0).GetEntry();

      auto itPoint = fmMCPointsLinksMap.find(CbmL1LinkKey(iIndex, iEvent, iFile));
      assert(itPoint != fmMCPointsLinksMap.cend());
      if (itPoint == fmMCPointsLinksMap.cend()) return iPoint;
      iPoint = itPoint->second;
    }
  }
  return iPoint;
}

// ---------------------------------------------------------------------------------------------------------------------
//
template<>
int CbmL1::MatchHitWithMc<L1DetectorID::kTof>(int iHit) const
{
  int iPoint           = -1;
  const auto* hitMatch = dynamic_cast<const CbmMatch*>(fpTofHitMatches->At(iHit));
  if (hitMatch) {
    for (int iLink = 0; iLink < hitMatch->GetNofLinks(); ++iLink) {
      int iMc = hitMatch->GetLink(iLink).GetIndex();
      if (iMc < 0) return iPoint;
      int iFile  = hitMatch->GetLink(iLink).GetFile();
      int iEvent = hitMatch->GetLink(iLink).GetEntry();

      assert(iMc >= 0 && iMc < fpTofPoints->Size(iFile, iEvent));
      int iIndex   = iMc + fNpointsMvdAll + fNpointsStsAll + fNpointsMuchAll + fNpointsTrdAll;
      auto itPoint = fmMCPointsLinksMap.find(CbmL1LinkKey(iIndex, iEvent, iFile));
      if (itPoint == fmMCPointsLinksMap.cend()) { continue; }
      iPoint = itPoint->second;
    }
  }
  return iPoint;
}


// **************************
// **     Event reader     **
// **************************

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmL1::ReadEvent(float& TsStart, float& TsLength, float& /*TsOverlap*/, int& FstHitinTs, bool& areDataLeft,
                      CbmEvent* event)
{
  static int nCalls = 0;

  if (fVerbose >= 10) cout << "ReadEvent: start." << endl;

  areDataLeft = false;  // no data left after reading the sub-timeslice


  // clear arrays for next event
  fvMCPoints.clear();                         /* <CbmL1MCPoint> */
  fvMCTracks.clear();                         /* <CbmL1MCTrack> */
  fvExternalHits.clear();                     /* <CbmL1Hit>     */
  fvRecoTracks.clear(); /* <CbmL1Track>   */  // TODO: Move from this function to more suitable place (S.Zharko)
  fvHitPointIndexes.clear(); /* <int>: indexes of MC-points in fvMCPoints (by index of fpAlgo->vHits) */
  fvHitStore.clear();        /* <CbmL1HitStore> */

  fmMCPointsLinksMap.clear();
  fmMCTracksLinksMap.clear();


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

  fNpointsMvd  = 0;
  fNpointsSts  = 0;
  fNpointsTrd  = 0;
  fNpointsMuch = 0;
  fNpointsTof  = 0;

  // get MVD hits
  int nMvdHits  = 0;
  int nStsHits  = 0;
  int nMuchHits = 0;
  int nTrdHits  = 0;
  int nTofHits  = 0;

  int firstMvdPoint  = 0;
  int firstStsPoint  = 0;
  int firstMuchPoint = 0;
  int firstTrdPoint  = 0;
  int firstTofPoint  = 0;

  /*
   * MC hits and tracks gathering: START
   *
   * If performance is studied, i.e. fPerformanse > 0, MC-information is used for
   * efficiencies calculation. Otherwise this 
   */

  if (fPerformance) {
    // Fill fvMCTracks and fmMCTracksLinksMap
    Fill_vMCTracks();

    // Fill fvMCPoints and fmMCPointsLinksMap
    fvMCPoints.clear();
    fvMCPoints.reserve(5 * fvMCTracks.size() * fNStations);
    fvMCPointIndexesTs.clear();
    fvMCPointIndexesTs.reserve(fvMCPoints.capacity());

    fNpointsMvdAll  = 0;
    fNpointsStsAll  = 0;
    fNpointsMuchAll = 0;
    fNpointsTrdAll  = 0;
    fNpointsTofAll  = 0;


    for (DFSET::iterator set_it = fvFileEvent.begin(); set_it != fvFileEvent.end(); ++set_it) {
      Int_t iFile  = set_it->first;
      Int_t iEvent = set_it->second;
      if (fUseMVD) fNpointsMvdAll += fpMvdPoints->Size(iFile, iEvent);
      if (fUseSTS) fNpointsStsAll += fpStsPoints->Size(iFile, iEvent);
      if (fUseMUCH) fNpointsMuchAll += fpMuchPoints->Size(iFile, iEvent);
      if (fUseTRD) fNpointsTrdAll += fpTrdPoints->Size(iFile, iEvent);
      if (fUseTOF) fNpointsTofAll += fpTofPoints->Size(iFile, iEvent);
    }

    for (DFSET::iterator set_it = fvFileEvent.begin(); set_it != fvFileEvent.end(); ++set_it) {
      Int_t iFile   = set_it->first;
      Int_t iEvent  = set_it->second;
      firstMvdPoint = fvMCPoints.size();
      if (fUseMVD && fpMvdPoints) {
        Int_t fNpointsMvdInEvent = fpMvdPoints->Size(iFile, iEvent);
        double maxDeviation      = 0;
        for (Int_t iMC = 0; iMC < fNpointsMvdInEvent; iMC++) {
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
              double d = (MC.zIn - sta[iStActive].fZ[0]);
              if (fabs(d) < fabs(bestDist)) {
                bestDist    = d;
                MC.iStation = iStActive;
              }
            }
            assert(MC.iStation >= 0);
            if (fabs(maxDeviation) < fabs(bestDist)) { maxDeviation = bestDist; }
            auto itTrack = fmMCTracksLinksMap.find(CbmL1LinkKey(MC.ID, iEvent, iFile));
            assert(itTrack != fmMCTracksLinksMap.cend());
            MC.ID = itTrack->second;
            fvMCTracks[MC.ID].Points.push_back_no_warning(fvMCPoints.size());


            fmMCPointsLinksMap[CbmL1LinkKey(iMC, iEvent, iFile)] = fvMCPoints.size();
            fvMCPoints.push_back(MC);
            fvMCPointIndexesTs.push_back(0);
            fNpointsMvd++;
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
              double d = (MC.zIn - sta[iStActive].fZ[0]);
              if (fabs(d) < fabs(bestDist)) {
                bestDist    = d;
                MC.iStation = iStActive;
              }
            }
            assert(MC.iStation >= 0);
            if (fabs(maxDeviation) < fabs(bestDist)) { maxDeviation = bestDist; }
            auto itTrack = fmMCTracksLinksMap.find(CbmL1LinkKey(MC.ID, iEvent, iFile));
            assert(itTrack != fmMCTracksLinksMap.cend());
            MC.ID = itTrack->second;
            fvMCTracks[MC.ID].Points.push_back_no_warning(fvMCPoints.size());
            fmMCPointsLinksMap[CbmL1LinkKey(iMC + fNpointsMvdAll, iEvent, iFile)] = fvMCPoints.size();
            fvMCPoints.push_back(MC);
            fvMCPointIndexesTs.push_back(0);
            fNpointsSts++;
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
              assert(iStActive != -1);
              if (MC.z > sta[iStActive].fZ[0] - 2.5) { MC.iStation = iStActive; }
            }
            assert(MC.iStation >= 0);
            auto itTrack = fmMCTracksLinksMap.find(CbmL1LinkKey(MC.ID, iEvent, iFile));
            assert(itTrack != fmMCTracksLinksMap.cend());
            MC.ID = itTrack->second;
            fvMCTracks[MC.ID].Points.push_back_no_warning(fvMCPoints.size());
            fmMCPointsLinksMap[CbmL1LinkKey(iMC + fNpointsMvdAll + +fNpointsStsAll, iEvent, iFile)] = fvMCPoints.size();
            fvMCPoints.push_back(MC);
            fvMCPointIndexesTs.push_back(0);
            fNpointsMuch++;
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

              assert(iStActive != -1);
              if (MC.z > sta[iStActive].fZ[0] - 20.0) { MC.iStation = iStActive; }
            }
            assert(MC.iStation >= 0);
            auto itTrack = fmMCTracksLinksMap.find(CbmL1LinkKey(MC.ID, iEvent, iFile));
            assert(itTrack != fmMCTracksLinksMap.cend());
            MC.ID = itTrack->second;
            fvMCTracks[MC.ID].Points.push_back_no_warning(fvMCPoints.size());
            fmMCPointsLinksMap[CbmL1LinkKey(iMC + fNpointsMvdAll + fNpointsStsAll + fNpointsMuchAll, iEvent, iFile)] =
              fvMCPoints.size();
            fvMCPoints.push_back(MC);
            fvMCPointIndexesTs.push_back(0);
            fNpointsTrd++;
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
            Int_t iMC = pHitMatch->GetLink(iLink).GetIndex();
            if (iMC < 0) continue;
            if (pHitMatch->GetLink(iLink).GetFile() != iFile) continue;
            if (pHitMatch->GetLink(iLink).GetEntry() != iEvent) continue;
            isTofPointMatched[iMC] = 1;
          }
        }

        for (Int_t iMC = 0; iMC < fpTofPoints->Size(iFile, iEvent); iMC++) {
          if (isTofPointMatched[iMC] == 0) continue;
          CbmL1MCPoint MC;
          if (!ReadMCPoint(&MC, iMC, iFile, iEvent, 4)) {
            auto itTrack = fmMCTracksLinksMap.find(CbmL1LinkKey(MC.ID, iEvent, iFile));
            assert(itTrack != fmMCTracksLinksMap.cend());
            int iTrack = itTrack->second;

            MC.iStation          = -1;
            const L1Station* sta = fpAlgo->GetParameters()->GetStations().begin();

            float dist = 1000;
            int iSta   = -1;
            for (int iSt = 0; iSt < fNTofStationsGeom; iSt++) {
              int iStActive = fpAlgo->GetParameters()->GetStationIndexActive(iSt, L1DetectorID::kTof);
              if (iStActive == -1) { continue; }
              if (fabs(MC.z - sta[iStActive].fZ[0]) < dist) {
                dist = fabs(MC.z - sta[iStActive].fZ[0]);
                iSta = iSt;
              }
            }
            MC.iStation = fpAlgo->GetParameters()->GetStationIndexActive(iSta, L1DetectorID::kTof);
            assert(MC.iStation >= 0);
            if (iSta >= 0) {
              if (fabs(sta[iSta].fZ[0] - MC.z) < TofPointToTrackdZ[iSta][iTrack]) {
                fTofPointToTrack[iSta][iTrack]  = iMC;
                TofPointToTrackdZ[iSta][iTrack] = fabs(sta[iSta].fZ[0] - MC.z);
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
              int iMC =
                fTofPointToTrack[iTofSta][iTrack] + fNpointsMvdAll + +fNpointsStsAll + fNpointsMuchAll + fNpointsTrdAll;
              fmMCPointsLinksMap[CbmL1LinkKey(iMC, iEvent, iFile)] = fvMCPoints.size();
              fvMCPoints.push_back(MC);
              fvMCPointIndexesTs.push_back(0);
              fNpointsTof++;
            }
          }
      }
    }  //time_slice

    for (unsigned int iTr = 0; iTr < fvMCTracks.size(); iTr++) {

      sort(fvMCTracks[iTr].Points.begin(), fvMCTracks[iTr].Points.end(), compareMcPointZ);

      if (fvMCTracks[iTr].mother_ID >= 0) {
        auto iFile                = fvMCTracks[iTr].iFile;
        auto iEvent               = fvMCTracks[iTr].iEvent;
        auto iMother              = fvMCTracks[iTr].mother_ID;
        auto itTrack              = fmMCTracksLinksMap.find(CbmL1LinkKey(iMother, iEvent, iFile));
        fvMCTracks[iTr].mother_ID = ((itTrack != fmMCTracksLinksMap.cend()) ? itTrack->second : -2);
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
  if (fUseMVD && fpMvdHits && (2 != fMvdUseMcHit)) {

    int firstDetStrip = NStrips;

    for (int j = 0; j < fpMvdHits->GetEntriesFast(); j++) {
      TmpHit th;
      {
        CbmMvdHit* h = L1_DYNAMIC_CAST<CbmMvdHit*>(fpMvdHits->At(j));
        th.ExtIndex  = -(1 + j);
        th.id        = tmpHits.size();
        th.iStation  = h->GetStationNr();

        int stIdx = fpAlgo->GetParameters()->GetStationIndexActive(h->GetStationNr(), L1DetectorID::kMvd);
        if (stIdx == -1) continue;
        th.iStation = stIdx;
        th.iStripF  = firstDetStrip + j;
        th.iStripB  = th.iStripF;
        if (NStrips <= th.iStripF) { NStrips = th.iStripF + 1; }

        TVector3 pos, err;
        h->Position(pos);
        h->PositionError(err);

        th.dx = h->GetDx();
        th.dy = h->GetDy();

        th.du  = h->GetDx();
        th.dv  = h->GetDy();
        th.dxy = h->GetDxy();

        th.x = pos.X();
        th.y = pos.Y();
        th.z = pos.Z();

        const L1Station& st = fpAlgo->GetParameters()->GetStation(th.iStation);
        th.u                = th.x * st.frontInfo.cos_phi[0] + th.y * st.frontInfo.sin_phi[0];
        th.v                = th.x * st.backInfo.cos_phi[0] + th.y * st.backInfo.sin_phi[0];
      }
      th.Det = 0;
      th.iMC = fPerformance ? MatchHitWithMc<L1DetectorID::kMvd>(th.ExtIndex) : -1;
      if (1 == fMvdUseMcHit && th.iMC >= 0) {
        th.SetHitFromPoint(fvMCPoints[th.iMC], th.iMC, fpAlgo->GetParameters()->GetStation(th.iStation));
      }
      //if(  th.iMC >=0 ) // DEBUG !!!!
      {
        tmpHits.push_back(th);
        nMvdHits++;
      }
    }  // for j
  }    // if fpMvdHits

  if (fUseMVD && (2 == fMvdUseMcHit)) {  // create hits from points

    for (int ip = firstMvdPoint; ip < firstMvdPoint + fNpointsMvd; ip++) {
      const CbmL1MCPoint& p = fvMCPoints[ip];
      TmpHit th;
      int DetId = 0;
      double du = 5.e-4;
      double dt = 1000.;
      th.CreateHitFromPoint(p, ip, DetId, tmpHits.size(), NStrips, fpAlgo->GetParameters()->GetStation(p.iStation), du,
                            du, dt, true);
      tmpHits.push_back(th);
      nMvdHits++;
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

      // ***********************************
      // ** Fill the temporary hit object **
      // ***********************************

      TmpHit th;

      // Fill reconstructed information
      {
        CbmStsHit* h = L1_DYNAMIC_CAST<CbmStsHit*>(fpStsHits->At(hitIndexSort));
        th.ExtIndex  = hitIndexSort;
        th.Det       = 1;
        int stIdx    = fpAlgo->GetParameters()->GetStationIndexActive(
          CbmStsSetup::Instance()->GetStationNumber(h->GetAddress()), L1DetectorID::kSts);

        if (stIdx == -1) continue;


        th.iStation = stIdx;
        th.iStripF  = firstDetStrip + h->GetFrontClusterId();
        th.iStripB  = firstDetStrip + h->GetBackClusterId();

        if (NStrips <= th.iStripF) { NStrips = th.iStripF + 1; }
        if (NStrips <= th.iStripB) { NStrips = th.iStripB + 1; }

        //Get time
        th.time = h->GetTime();
        th.dt   = h->GetTimeError();

        if (!fLegacyEventMode) { th.id = nMvdHits + hitIndex; }
        else {
          th.id = tmpHits.size();
        }

        /// stop if reco TS ends and many hits left
        if (!event) {
          if ((th.time > (TsStart + TsLength)) && ((nEntSts - hitIndex) > 300)) {
            areDataLeft = true;  // there are unprocessed data left in the time slice
            break;
          }
        }

        TVector3 pos, err;
        h->Position(pos);
        h->PositionError(err);

        th.x = pos.X();
        th.y = pos.Y();
        th.z = pos.Z();

        th.dx  = h->GetDx();
        th.dy  = h->GetDy();
        th.dxy = h->GetDxy();

        th.du = h->GetDu();
        th.dv = h->GetDv();

        const L1Station& st = fpAlgo->GetParameters()->GetStation(th.iStation);
        th.u                = th.x * st.frontInfo.cos_phi[0] + th.y * st.frontInfo.sin_phi[0];
        th.v                = th.x * st.backInfo.cos_phi[0] + th.y * st.backInfo.sin_phi[0];
      }
      th.iMC = fPerformance ? MatchHitWithMc<L1DetectorID::kSts>(hitIndexSort) : -1;

      if (1 == fStsUseMcHit && th.iMC >= 0) {
        th.SetHitFromPoint(fvMCPoints[th.iMC], th.iMC, fpAlgo->GetParameters()->GetStation(th.iStation));
      }

      tmpHits.push_back(th);
      nStsHits++;

    }  // for j
  }    // if fpStsHits

  if (fUseSTS && (2 == fStsUseMcHit)) {  // create hits from points

    for (int ip = firstStsPoint; ip < firstStsPoint + fNpointsSts; ip++) {
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
      double du = 10.e-4;
      double dt = 5.;
      th.CreateHitFromPoint(p, ip, DetId, tmpHits.size(), NStrips, fpAlgo->GetParameters()->GetStation(p.iStation), du,
                            du, dt, true);
      tmpHits.push_back(th);
      nStsHits++;
    }
  }


  //
  // Get MuCh hits
  //

  if (fUseMUCH && fpMuchPixelHits && (2 != fMuchUseMcHit)) {

    Int_t nEnt = (event ? event->GetNofData(ECbmDataType::kMuchPixelHit) : fpMuchPixelHits->GetEntriesFast());

    int firstDetStrip = NStrips;

    for (int j = 0; j < nEnt; j++) {
      TmpHit th;
      {


        Int_t hitIndex = 0;
        hitIndex       = (event ? event->GetIndex(ECbmDataType::kMuchPixelHit, j) : j);

        CbmMuchPixelHit* h = static_cast<CbmMuchPixelHit*>(fpMuchPixelHits->At(hitIndex));

        th.ExtIndex = hitIndex;
        th.Det      = 2;
        th.id       = tmpHits.size();

        Int_t stationNumber = CbmMuchGeoScheme::GetStationIndex(h->GetAddress());
        Int_t layerNumber   = CbmMuchGeoScheme::GetLayerIndex(h->GetAddress());

        int DetId = stationNumber * 3 + layerNumber;

        int stIdx = fpAlgo->GetParameters()->GetStationIndexActive(DetId, L1DetectorID::kMuch);
        if (stIdx == -1) continue;
        th.iStation = stIdx;  //h->GetStationNr() - 1;

        //Get time
        th.time = h->GetTime();
        th.dt   = h->GetTimeError();


        //   th.iSector  = 0;
        th.iStripF = firstDetStrip + hitIndex;
        th.iStripB = th.iStripF;
        if (NStrips <= th.iStripF) { NStrips = th.iStripF + 1; }

        th.x = h->GetX();
        th.y = h->GetY();
        th.z = h->GetZ();

        th.dx  = h->GetDx();
        th.dy  = h->GetDy();
        th.dxy = 0;

        th.du = h->GetDx();
        th.dv = h->GetDy();


        const L1Station& st = fpAlgo->GetParameters()->GetStation(th.iStation);
        th.u                = th.x * st.frontInfo.cos_phi[0] + th.y * st.frontInfo.sin_phi[0];
        th.v                = th.x * st.backInfo.cos_phi[0] + th.y * st.backInfo.sin_phi[0];
      }
      th.iMC = fPerformance ? MatchHitWithMc<L1DetectorID::kMuch>(th.ExtIndex) : -1;
      if (1 == fMuchUseMcHit && th.iMC > -1) {
        th.SetHitFromPoint(fvMCPoints[th.iMC], th.iMC, fpAlgo->GetParameters()->GetStation(th.iStation));
      }
      //int iMC = -1;
      //if (fPerformance) {
      //  if (fpMuchHitMatches) {
      //    CbmMatch* matchHitMatch = L1_DYNAMIC_CAST<CbmMatch*>(fpMuchHitMatches->At(j));


      //    for (Int_t iLink = 0; iLink < matchHitMatch->GetNofLinks(); iLink++) {
      //      if (matchHitMatch->GetLink(iLink).GetIndex() < fNpointsMuch) {
      //        iMC          = matchHitMatch->GetLink(iLink).GetIndex();
      //        Int_t iIndex = iMC + fNpointsMvd + fNpointsSts;

      //        Int_t iFile  = matchHitMatch->GetLink(0).GetFile();
      //        Int_t iEvent = matchHitMatch->GetLink(0).GetEntry();

      //        auto itPoint = fmMCPointsLinksMap.find(CbmL1LinkKey(iIndex, iEvent, iFile));
      //        if (itPoint == fmMCPointsLinksMap.cend()) continue;
      //        th.iMC = itPoint->second;
      //        if ((1 == fMuchUseMcHit) && (th.iMC > -1))
      //          th.SetHitFromPoint(fvMCPoints[th.iMC], fpAlgo->GetParameters()->GetStation(th.iStation));
      //      }
      //    }
      //  }
      //}

      tmpHits.push_back(th);
      nMuchHits++;

    }  // for j
  }    // if listMuchHits

  if ((2 == fMuchUseMcHit) && fUseMUCH) {  // create hits from points

    for (int ip = firstMuchPoint; ip < firstMuchPoint + fNpointsMuch; ip++) {
      const CbmL1MCPoint& p = fvMCPoints[ip];

      //       int mcTrack = p.ID;
      //       if (mcTrack < 0) continue;
      //       const CbmL1MCTrack& t = fvMCTracks[mcTrack];
      //if (t.p < 1) continue;
      // if (t.Points.size() > 4) continue;

      TmpHit th;
      int DetId = 2;
      double du = 100.e-4;
      double dt = 3.9;
      th.CreateHitFromPoint(p, ip, DetId, tmpHits.size(), NStrips, fpAlgo->GetParameters()->GetStation(p.iStation), du,
                            du, dt, true);

      tmpHits.push_back(th);
      nMuchHits++;
    }
  }


  //
  // Get TRD hits
  //

  if (fUseTRD && fpTrdHits && (2 != fTrdUseMcHit)) {

    assert(fpTrdHits);

    vector<bool> mcUsed(fNpointsTrd, 0);

    Int_t nEntTrd = (event ? event->GetNofData(ECbmDataType::kTrdHit) : fpTrdHits->GetEntriesFast());

    for (int iHit = 0; iHit < nEntTrd; iHit++) {
      TmpHit th;

      Int_t hitIndex = 0;
      hitIndex       = (event ? event->GetIndex(ECbmDataType::kTrdHit, iHit) : iHit);

      CbmTrdHit* h = L1_DYNAMIC_CAST<CbmTrdHit*>(fpTrdHits->At(hitIndex));

      if ((L1Algo::TrackingMode::kGlobal == fTrackingMode) && (int) h->GetClassType() != 1) {
        // SGtrd2d!! skip TRD-1D hit
        continue;
      }

      th.ExtIndex = hitIndex;
      th.Det      = 3;
      th.id       = tmpHits.size();


      int stIdx = fpAlgo->GetParameters()->GetStationIndexActive(h->GetPlaneId(), L1DetectorID::kTrd);
      assert(stIdx != -1);
      //if (stIdx == -1) continue;

      th.iStation = stIdx;

      //  if (h->GetPlaneId()==0) continue;
      //  if (h->GetPlaneId()==1) continue;
      //  if (h->GetPlaneId()==2) continue;
      //  if (h->GetPlaneId()==3) continue;

      th.time = h->GetTime();
      th.dt   = h->GetTimeError();

      //   th.iSector  = 0;
      th.iStripF = NStrips;
      th.iStripB = th.iStripF;  //TrdHitsOnStationIndex[num+1][k];
      NStrips++;

      TVector3 pos, err;
      h->Position(pos);
      h->PositionError(err);

      th.x = pos.X();
      th.y = pos.Y();
      th.z = pos.Z();

      th.dx  = fabs(h->GetDx());
      th.dy  = fabs(h->GetDy());
      th.dxy = 0;

      th.du = fabs(h->GetDx());
      th.dv = fabs(h->GetDy());

      const L1Station& st = fpAlgo->GetParameters()->GetStation(th.iStation);

      std::tie(th.u, th.v) = st.ConvXYtoUV<double>(th.x, th.y);

      th.iMC   = fPerformance ? MatchHitWithMc<L1DetectorID::kTrd>(th.ExtIndex) : -1;
      th.track = (th.iMC > -1) ? fvMCPoints[th.iMC].ID : -1;
      //int iMcTrd = -1;
      //if (fPerformance && fpTrdHitMatches) {
      //  CbmMatch* trdHitMatch = L1_DYNAMIC_CAST<CbmMatch*>(fpTrdHitMatches->At(iHit));
      //  if (trdHitMatch->GetNofLinks() > 0) {
      //    iMcTrd = trdHitMatch->GetLink(0).GetIndex();
      //    assert(iMcTrd >= 0 && iMcTrd < fNpointsTrd);
      //    th.iMC   = iMcTrd + fNpointsMvd + fNpointsSts + fNpointsMuch;
      //    th.track = fvMCPoints[th.iMC].ID;
      //  }
      //}

      if (1 == fTrdUseMcHit) {  // replace hit by MC points

        assert(fPerformance && fpTrdHitMatches);

        if (th.iMC < 0) continue;  // skip noise hits
        int iMcTrd = th.iMC - fNpointsMvd - fNpointsSts - fNpointsMuch;
        assert(iMcTrd >= 0 && iMcTrd < fNpointsTrd);
        if (mcUsed[iMcTrd]) continue;  // one hit per MC point
        bool doSmear = true;
        if (0) {  // SGtrd2d!! debug: artificial errors
          th.dx = .1;
          th.du = .1;
          th.dy = .1;
          th.dv = .1;
        }
        th.SetHitFromPoint(fvMCPoints[th.iMC], th.iMC, fpAlgo->GetParameters()->GetStation(th.iStation), doSmear);
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
  }    // read TRD hits

  if (fUseTRD && (2 == fTrdUseMcHit)) {  // create hits from MC points
    for (int ip = firstTrdPoint; ip < firstTrdPoint + fNpointsTrd; ip++) {
      const CbmL1MCPoint& p = fvMCPoints[ip];
      //       int mcTrack = p.ID;
      //       if (mcTrack < 0) continue;
      //       const CbmL1MCTrack& t = fvMCTracks[mcTrack];
      TmpHit th;
      int DetId = 3;
      double du = 0.1;
      double dt = 10.;
      th.CreateHitFromPoint(p, ip, DetId, tmpHits.size(), NStrips, fpAlgo->GetParameters()->GetStation(p.iStation), du,
                            du, dt, true);
      tmpHits.push_back(th);
      nTrdHits++;
    }
  }

  //
  // Get ToF hits
  //

  if (fUseTOF && fpTofHits && (2 != fTofUseMcHit)) {
    int firstDetStrip = NStrips;

    Int_t nEntTof = (event ? event->GetNofData(ECbmDataType::kTofHit) : fpTofHits->GetEntriesFast());

    for (int j = 0; j < nEntTof; j++) {


      Int_t hitIndex = 0;
      hitIndex       = (event ? event->GetIndex(ECbmDataType::kTofHit, j) : j);

      TmpHit th;

      CbmTofHit* h = L1_DYNAMIC_CAST<CbmTofHit*>(fpTofHits->At(hitIndex));


      th.ExtIndex = hitIndex;
      th.Det      = 4;

      th.id = tmpHits.size();

      if (0x00202806 == h->GetAddress() || 0x00002806 == h->GetAddress()) continue;  // TODO: Why? (S.Zharko)

      if (5 == CbmTofAddress::GetSmType(h->GetAddress())) continue;

      int sttof = CbmTofTrackingInterface::Instance()->GetTrackingStationIndex(h);

      if (sttof < 0) continue;

      th.time = h->GetTime();
      th.dt   = h->GetTimeError();

      th.dx  = h->GetDx();
      th.dy  = h->GetDy();
      th.dxy = 0;

      th.du = h->GetDx();
      th.dv = h->GetDy();

      //   th.iSector  = 0;
      th.iStripF = firstDetStrip + j;
      th.iStripB = th.iStripF;
      if (NStrips <= th.iStripF) { NStrips = th.iStripF + 1; }

      TVector3 pos, err;
      h->Position(pos);
      h->PositionError(err);

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

      th.iMC = fPerformance ? MatchHitWithMc<L1DetectorID::kTof>(th.ExtIndex) : -1;

      if (1 == fTofUseMcHit && th.iMC > -1) {
        th.SetHitFromPoint(fvMCPoints[th.iMC], th.iMC, fpAlgo->GetParameters()->GetStation(th.iStation));
      }

      tmpHits.push_back(th);
      nTofHits++;

    }  // for j
  }    // if listTofHits

  if ((2 == fTofUseMcHit) && fUseTOF) {  // create hits from points
    for (int ip = firstTofPoint; ip < firstTofPoint + fNpointsTof; ip++) {
      const CbmL1MCPoint& p = fvMCPoints[ip];

      //       int mcTrack = p.ID;
      //  if (mcTrack < 0) continue;
      //const CbmL1MCTrack& t = fvMCTracks[mcTrack];
      //if (t.p < 1) continue;
      //if (t.Points.size() > 4) continue;

      TmpHit th;
      int DetId = 4;
      double du = 0.1;
      double dt = 0.075;
      th.CreateHitFromPoint(p, ip, DetId, tmpHits.size(), NStrips, fpAlgo->GetParameters()->GetStation(p.iStation), du,
                            du, dt, true);
      tmpHits.push_back(th);
      nTofHits++;
    }
  }


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
  int maxHitIndex = 0;

  for (int ih = 0; ih < nHits; ih++) {
    TmpHit& th = tmpHits[ih];
    if (maxHitIndex < th.id) { maxHitIndex = th.id; }
  }  // ih

  if (fVerbose >= 10) { cout << "ReadEvent: strips are read." << endl; }


  // ----- Fill and save fvExternalHits, fvHitStore and fvHitPointIndexes vectors as well as fpData->vHits
  fvSortedHitsIndexes.reset(maxHitIndex + 1);

  fvExternalHits.reserve(nHits);

  fvHitStore.reserve(nHits);
  fvHitPointIndexes.reserve(nHits);

  fIODataManager.ResetInputData();
  fIODataManager.ReserveNhits(nHits);
  fIODataManager.SetNhitKeys(NStrips);


  // ----- Fill
  for (int iHit = 0; iHit < nHits; ++iHit) {
    TmpHit& th = tmpHits[iHit];

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

    fvSortedHitsIndexes[th.id] = iHit;

    assert(th.iStripF >= 0 || th.iStripF < NStrips);
    assert(th.iStripB >= 0 || th.iStripB < NStrips);

    L1Hit h;
    h.iSt = th.iStation;
    h.f   = th.iStripF;
    h.b   = th.iStripB;
    h.ID  = th.id;
    h.z   = th.z;
    h.u   = th.u;
    h.v   = th.v;
    h.t   = th.time;
    h.dt2 = th.dt * th.dt;
    h.du2 = th.du * th.du;
    h.dv2 = th.dv * th.dv;


    // save hit
    fvExternalHits.push_back(CbmL1Hit(iHit, th.ExtIndex, th.Det));

    fvExternalHits[iHit].x = th.x;
    fvExternalHits[iHit].y = th.y;
    fvExternalHits[iHit].t = th.time;

    fvExternalHits[iHit].ID = th.id;

    fvExternalHits[iHit].f = th.iStripF;
    fvExternalHits[iHit].b = th.iStripB;

    // TODO: Here one should fill in the fvExternalHits[iHit].mcPointIds

    fIODataManager.PushBackHit(h);

    fvHitStore.push_back(s);
    fvHitPointIndexes.push_back(th.iMC);
  }

  if (fVerbose >= 1) cout << "ReadEvent: mvd and sts are saved." << endl;

  // ----- Send data from IODataManager to L1Algo --------------------------------------------------------------------
  if (1 == fSTAPDataMode) { WriteSTAPAlgoInputData(nCalls); }
  if (2 == fSTAPDataMode) { ReadSTAPAlgoInputData(nCalls); }
  // TODO: SZh: If we read data from file, we don't need to collect them above. This should be addressed
  fIODataManager.SendInputData(fpAlgo);

  if (fPerformance) {
    if (fVerbose >= 10) cout << "HitMatch is done." << endl;
    if (fVerbose >= 10) cout << "MCPoints and MCTracks are saved." << endl;
  }
  if (fVerbose >= 10) cout << "ReadEvent is done." << endl;

  ++nCalls;
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
      Int_t pdg              = MCTrack->GetPdgCode();
      unsigned int processID = MCTrack->GetGeantProcessId();
      Double_t q = 0, mass = 0.;
      if (pdg < 9999999 && ((TParticlePDG*) TDatabasePDG::Instance()->GetParticle(pdg))) {
        q    = TDatabasePDG::Instance()->GetParticle(pdg)->Charge() / 3.0;
        mass = TDatabasePDG::Instance()->GetParticle(pdg)->Mass();
      }
      // TODO: Add light nuclei (d, t, He3, He4): they are common in tracking but not accounted in TDatabasePDG (S.Zharko)

      //cout << "mc track " << iMCTrack << " pdg " << pdg << " z " << vr.Z() << endl;

      Int_t iTrack = fvMCTracks.size();  //or iMCTrack?
      CbmL1MCTrack T(mass, q, vr, vp, iTrack, mother_ID, pdg, processID);
      //        CbmL1MCTrack T(mass, q, vr, vp, iMCTrack, mother_ID, pdg);
      T.time   = MCTrack->GetStartT();
      T.iFile  = iFile;
      T.iEvent = iEvent;
      // signal: primary tracks, displaced from the primary vertex
      T.isSignal = T.IsPrimary() && (T.z > header->GetZ() + 1.e-10);

      fvMCTracks.push_back(T);
      fmMCTracksLinksMap[CbmL1LinkKey(iMCTrack, iEvent, iFile)] = fvMCTracks.size() - 1;
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
      Double_t StartTime     = fTimeSlice->GetStartTime();                              // not used
      Double_t EndTime       = fTimeSlice->GetEndTime();                                // not used
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

// TODO: Duplicated code (from CbmL1::ReadEvent)
void CbmL1::HitMatch()
{
  const int NHits = fvExternalHits.size();
  for (int iH = 0; iH < NHits; iH++) {
    CbmL1Hit& hit = fvExternalHits[iH];


    int iP = fvHitPointIndexes[iH];
    if (iP >= 0) {
      hit.mcPointIds.push_back_no_warning(iP);
      fvMCPoints[iP].hitIds.push_back_no_warning(iH);
    }

  }  // for hits
}
