/* Copyright (C) 2010-2020 Frankfurt Institute for Advanced Studies, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Ivan Kisel,  Sergey Gorbunov, Igor Kulakov [committer], Maksym Zyzak */

/*
 *====================================================================
 *
 *  CBM Level 1 Reconstruction 
 *  
 *  Authors: I.Kisel,  S.Gorbunov
 *
 *  e-mail : ikisel@kip.uni-heidelberg.de 
 *
 *====================================================================
 *
 *  L1 Monte Carlo information
 *
 *====================================================================
 */

#include "CbmL1MCTrack.h"

#include "CbmL1.h"
#include "CbmL1Constants.h"

#include <iomanip>
#include <sstream>

#include "L1Algo/L1Algo.h"
#include "L1Algo/L1Hit.h"

CbmL1MCTrack::CbmL1MCTrack(double mass_, double q_, TVector3 vr, TLorentzVector vp, int _ID, int _mother_ID, int _pdg,
                           unsigned int _process_ID)
  : mass(mass_)
  , q(q_)
  , p(vp.P())
  , x(vr.X())
  , y(vr.Y())
  , z(vr.Z())
  , px(vp.Px())
  , py(vp.Py())
  , pz(vp.Pz())
  , ID(_ID)
  , mother_ID(_mother_ID)
  , pdg(_pdg)
  , process_ID(_process_ID)
{
}

// CbmL1MCTrack::CbmL1MCTrack(TmpMCPoints &mcPoint, TVector3 vr, TLorentzVector vp, int ID, int mother_ID):
//      ID(_ID), mother_ID(_mother_ID)
// {
//   mass = mcPoint->mass;
//   q = mcPoint->q;
//   pdg  = mcPoint->pdg;
//
//   x = vr.X();
//   y = vr.Y();
//   z = vr.Z();
//   px = vp.Px();
//   py = vp.Py();
//   pz = vp.Pz();
//   p = sqrt( fabs(px*px + py*py + pz*pz ));
// };

void CbmL1MCTrack::Init()
{
  CbmL1* L1 = CbmL1::Instance();
  // get Hits
  Hits.clear();
  for (unsigned int iP = 0; iP < Points.size(); iP++) {
    CbmL1MCPoint* point = &(L1->fvMCPoints[Points[iP]]);
    for (unsigned int iH = 0; iH < point->hitIds.size(); iH++) {
      const int iih = point->hitIds[iH];
      if (std::find(Hits.begin(), Hits.end(), iih) == Hits.end()) Hits.push_back_no_warning(iih);
    }
  }

  CalculateMCCont();
  CountHitStations();
  CalculateMaxNStaMC();
  CalculateIsReconstructable();
}

void CbmL1MCTrack::CalculateMCCont()
{
  CbmL1* L1 = CbmL1::Instance();

  int nPoints     = Points.size();
  nMCContStations = 0;
  int istaold = -1, ncont = 0;
  for (int ih = 0; ih < nPoints; ih++) {
    CbmL1MCPoint& h = L1->fvMCPoints[Points[ih]];
    int ista        = h.iStation;
    if (ista - istaold == 1) ncont++;
    else if (ista - istaold > 1) {
      if (nMCContStations < ncont) nMCContStations = ncont;
      ncont = 1;
    }
    if (ista <= istaold) continue;  // backward direction
    istaold = ista;
  }
  if (nMCContStations < ncont) nMCContStations = ncont;
}


void CbmL1MCTrack::CountHitStations()
{
  CbmL1* L1 = CbmL1::Instance();

  int stationNhits[L1Constants::size::kMaxNstations] {0};

  for (unsigned int iH = 0; iH < Hits.size(); iH++) {
    CbmL1HitDebugInfo& sh = L1->fvHitDebugInfo[Hits[iH]];
    stationNhits[sh.iStation]++;
  }

  maxNStaHits      = 0;
  nStations        = 0;
  nHitContStations = 0;
  int ncont        = 0;

  for (int ista = 0; ista < L1->fpAlgo->GetParameters()->GetNstationsActive(); ista++) {
    if (maxNStaHits < stationNhits[ista]) { maxNStaHits = stationNhits[ista]; }
    if (stationNhits[ista] > 0) {
      nStations++;
      ncont++;
    }
    else {
      if (nHitContStations < ncont) { nHitContStations = ncont; }
      ncont = 0;
    }
  }
  if (nHitContStations < ncont) nHitContStations = ncont;
}

void CbmL1MCTrack::CalculateMaxNStaMC()
{
  CbmL1* L1 = CbmL1::Instance();

  maxNStaMC         = 0;
  maxNSensorMC      = 0;
  nMCStations       = 0;
  int lastSta       = -1;
  float lastz       = -1;
  int cur_maxNStaMC = 0, cur_maxNSensorMC = 0;
  for (unsigned int iH = 0; iH < Points.size(); iH++) {
    CbmL1MCPoint& mcP = L1->fvMCPoints[Points[iH]];
    if (mcP.iStation == lastSta) cur_maxNStaMC++;
    else {  // new station
      if (cur_maxNStaMC > maxNStaMC) maxNStaMC = cur_maxNStaMC;
      cur_maxNStaMC = 1;
      lastSta       = mcP.iStation;
      nMCStations++;
    }

    if (mcP.z == lastz)  // TODO: works incorrect because of different z
      cur_maxNSensorMC++;
    else {  // new z
      if (cur_maxNSensorMC > maxNSensorMC) maxNSensorMC = cur_maxNSensorMC;
      cur_maxNSensorMC = 1;
      lastz            = mcP.z;
    }
  };
  if (cur_maxNStaMC > maxNStaMC) maxNStaMC = cur_maxNStaMC;
  if (cur_maxNSensorMC > maxNSensorMC) maxNSensorMC = cur_maxNSensorMC;
  //   cout << pdg << " " << p << " " << Points.size() << " > " << maxNStaMC << " " << maxNSensorMC << endl;
};  // void CbmL1MCTrack::CalculateMaxNStaMC()


void CbmL1MCTrack::CalculateIsReconstructable()
{
  CbmL1* L1 = CbmL1::Instance();

  bool f = 1;

  // reject very slow tracks from analysis
  f &= (p > CbmL1Constants::MinRecoMom);
  // detected at least in 4 stations
  //   f &= (nMCContStations >= 4);

  // maximul 4 layers for a station.
  //   f &= (maxNStaHits <= 4);
  f &= (maxNStaMC <= 4);
  //   f &= (maxNSensorMC <= 1);
  if (L1->fPerformance == 3) isReconstructable = f & (nMCContStations >= CbmL1Constants::MinNStations);  // L1-MC
  if (L1->fPerformance == 2) isReconstructable = f & (nStations >= CbmL1Constants::MinNStations);  // QA definition
  if (L1->fPerformance == 1)
    isReconstructable = f & (nHitContStations >= CbmL1Constants::MinNStations);  // L1 definition

  if (Points.size() > 0) {
    isAdditional = f & (nHitContStations == nStations) & (nMCContStations == nStations) & (nMCStations == nStations)
                   & (nHitContStations >= 3) & (L1->fvMCPoints[Points[0]].iStation == 0);
    isAdditional &= !isReconstructable;
  }
  else
    isAdditional = 0;
};  // bool CbmL1MCTrack::IsReconstructable()

std::string CbmL1MCTrack::ToString(int verbose, bool header) const
{
  using std::setfill;
  using std::setw;
  std::stringstream msg;
  if (header) {
    if (verbose > 0) {
      msg << setw(8) << setfill(' ') << "ID" << ' ';
      msg << setw(8) << setfill(' ') << "Mother" << ' ';
      msg << setw(8) << setfill(' ') << "PDG" << ' ';
      if (verbose > 1) {
        msg << setw(8) << setfill(' ') << "N h." << ' ';
        msg << setw(8) << setfill(' ') << "N p." << ' ';
        msg << setw(8) << setfill(' ') << "N r.tr." << ' ';
        if (verbose > 2) { msg << setw(8) << setfill(' ') << "N t.tr." << ' '; }
        msg << setw(12) << setfill(' ') << "zVTX [cm]" << ' ';
        msg << setw(12) << setfill(' ') << "t [ns]" << ' ';
        msg << setw(12) << setfill(' ') << "p [GeV/c]" << ' ';
      }
      msg << setw(8) << setfill(' ') << "rec-able" << ' ';
      msg << setw(8) << setfill(' ') << "rec-ed" << ' ';
    }
  }
  else {
    if (verbose > 0) {
      msg << setw(8) << setfill(' ') << ID << ' ';
      msg << setw(8) << setfill(' ') << mother_ID << ' ';
      msg << setw(8) << setfill(' ') << pdg << ' ';
      if (verbose > 1) {
        msg << setw(8) << setfill(' ') << Hits.size() << ' ';
        msg << setw(8) << setfill(' ') << Points.size() << ' ';
        msg << setw(8) << setfill(' ') << rTracks.size() << ' ';
        if (verbose > 2) { msg << setw(8) << setfill(' ') << tTracks.size() << ' '; }
        msg << setw(12) << setfill(' ') << z << ' ';
        msg << setw(12) << setfill(' ') << time << ' ';
        msg << setw(12) << setfill(' ') << p << ' ';
      }
      msg << setw(8) << setfill(' ') << IsReconstructable() << ' ';
      msg << setw(8) << setfill(' ') << IsReconstructed() << ' ';
      if (verbose > 1) {
        msg << "\n\t- point indexes: ";
        for (int index : Points) {
          msg << index << ' ';
        }
        msg << "\n\t- hit indexes: ";
        for (int index : Hits) {
          msg << index << ' ';
        }
        msg << "\n\t- reconstructed track indexes: ";
        for (auto* index : rTracks) {
          msg << index << ' ';
        }
        if (verbose > 2) {
          msg << "\n\t- touch track indexes: ";
          for (auto* index : tTracks) {
            msg << index << ' ';
          }
        }
      }
    }
  }
  return msg.str();
}
