/* Copyright (C) 2014-2018 Veksler and Baldin Laboratory of High Energy Physics, Joint Institute for Nuclear Research, Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Alexander Zinchenko [committer] */

/** CbmMuchFindVectors.cxx
 *@author A.Zinchenko <Alexander.Zinchenko@jinr.ru>
 *@since 2014
 **/
#include "CbmMuchFindVectors.h"

#include "CbmMuchDigiMatch.h"
#include "CbmMuchMergeVectors.h"
#include "CbmMuchModule.h"
#include "CbmMuchPoint.h"
#include "CbmMuchStation.h"
#include "CbmMuchTrack.h"

#include "FairEventHeader.h"
#include "FairRootManager.h"

#include <TClonesArray.h>
#include <TGeoManager.h>
#include <TMath.h>
#include <TMatrixD.h>
#include <TMatrixFLazy.h>
#include <TVectorD.h>

#include <iostream>

using std::cout;
using std::endl;
using std::map;
using std::multimap;
using std::pair;
using std::set;
using std::vector;

//FILE *lun = fopen("chi2.dat","w");

// -----   Default constructor   -------------------------------------------
CbmMuchFindVectors::CbmMuchFindVectors()
  : FairTask("MuchFindVectors")
  , fGeoScheme(CbmMuchGeoScheme::Instance())
  , fTrackArray(NULL)
  , fNofTracks(0)
  , fHits(NULL)
  , fPoints(NULL)
  , fDigiMatches(NULL)
  , fStatFirst(-1)
  , fErrU(-1.0)
  , fDiam(0.0)
  , fCutChi2(24.0)
  ,  // chi2/ndf=6 for 8 hits
  //fCutChi2(20.0), // chi2/ndf=5 for 8 hits
  fMinHits(10)
{
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
CbmMuchFindVectors::~CbmMuchFindVectors()
{
  fTrackArray->Delete();
  for (Int_t i = 0; i < fgkStat; ++i) {
    Int_t nVecs = fVectors[i].size();
    for (Int_t j = 0; j < nVecs; ++j)
      delete fVectors[i][j];
    //nVecs = fVectorsHigh[i].size();
    //for (Int_t j = 0; j < nVecs; ++j) delete fVectorsHigh[i][j];
  }
  for (map<Int_t, TDecompLU*>::iterator it = fLus.begin(); it != fLus.end(); ++it)
    delete it->second;
}
// -------------------------------------------------------------------------

// -----   Public method Init (abstract in base class)  --------------------
InitStatus CbmMuchFindVectors::Init()
{

  // Get and check FairRootManager
  FairRootManager* ioman = FairRootManager::Instance();
  if (ioman == NULL) Fatal("Init", "RootManager not instantiated!");

  // Create and register MuchTrack array (if necessary)
  fTrackArray = static_cast<TClonesArray*>(ioman->GetObject("MuchVector"));
  if (fTrackArray == NULL) {
    fTrackArray = new TClonesArray("CbmMuchTrack", 100);
    ioman->Register("MuchVector", "Much", fTrackArray, kTRUE);
  }

  CbmMuchFindHitsStraws* hitFinder = (CbmMuchFindHitsStraws*) FairRun::Instance()->GetTask("CbmMuchFindHitsStraws");
  //if (hitFinder == NULL) Fatal("Init", "CbmMuchFindHitsStraws is not run!");
  if (hitFinder == NULL) return kSUCCESS;  // no straws

  fDiam = hitFinder->GetDiam(0);

  fHits        = static_cast<TClonesArray*>(ioman->GetObject("MuchStrawHit"));
  fPoints      = static_cast<TClonesArray*>(ioman->GetObject("MuchPoint"));
  fDigiMatches = static_cast<TClonesArray*>(ioman->GetObject("MuchStrawDigiMatch"));

  // Find first straw station and get some geo constants
  Int_t nSt = fGeoScheme->GetNStations();
  for (Int_t i = 0; i < nSt; ++i) {
    CbmMuchStation* st = fGeoScheme->GetStation(i);
    CbmMuchModule* mod = fGeoScheme->GetModule(i, 0, 0, 0);
    if (mod->GetDetectorType() == 2) {
      if (fStatFirst < 0) fStatFirst = CbmMuchAddress::GetStationIndex(st->GetDetectorId());
      //cout << " First station: " << fStatFirst << endl;
      Int_t nLays           = st->GetNLayers();
      fRmin[i - fStatFirst] = st->GetRmin();
      fRmax[i - fStatFirst] = st->GetRmax();
      for (Int_t lay = 0; lay < nLays; ++lay) {
        CbmMuchLayer* layer = st->GetLayer(lay);
        Double_t phi        = hitFinder->GetPhi(lay);
        for (Int_t iside = 0; iside < 2; ++iside) {
          CbmMuchLayerSide* side = layer->GetSide(iside);
          Int_t plane            = lay * 2 + iside;
          if (plane == 0) fZ0[i - fStatFirst] = side->GetZ();
          //plane += (i - fStatFirst) * fgkPlanes;
          fDz[plane]   = side->GetZ();
          fCosa[plane] = TMath::Cos(phi);
          fSina[plane] = TMath::Sin(phi);
          if (lay) {
            fDtubes[i - fStatFirst][lay - 1] =
              fRmax[i - fStatFirst] * TMath::Tan(TMath::ASin(fSina[plane]) - TMath::ASin(fSina[plane - 2]));
            fDtubes[i - fStatFirst][lay - 1] = TMath::Abs(fDtubes[i - fStatFirst][lay - 1]) / fDiam + 10;
          }
        }
      }
    }
  }
  for (Int_t i = fgkPlanes - 1; i >= 0; --i) {
    fDz[i] -= fDz[0];
    //cout << fDz[i] << " ";
  }
  //cout << endl;

  // Get absorbers
  Double_t dzAbs[9] = {0}, zAbs[9] = {0}, radlAbs[9] = {0}, xyzl[3] = {0}, xyzg[3] = {0};
  Int_t nAbs      = 0;
  TGeoVolume* vol = 0x0;
  for (Int_t i = 1; i < 10; ++i) {
    TString abso = "muchabsorber0";
    abso += i;
    vol = gGeoManager->GetVolume(abso);
    if (vol == 0x0) break;
    TString path = "/cave_1/much_0/";
    path += abso;
    path += "_0";
    gGeoManager->cd(path);
    gGeoManager->LocalToMaster(xyzl, xyzg);
    zAbs[nAbs] = xyzg[2];
    cout << vol->GetName() << " " << vol->GetShape()->GetName() << " " << ((TGeoBBox*) vol->GetShape())->GetDZ()
         << endl;
    //dzAbs[nAbs] = ((TGeoCone*) vol->GetShape())->GetDz();
    dzAbs[nAbs]     = ((TGeoBBox*) vol->GetShape())->GetDZ();
    radlAbs[nAbs]   = vol->GetMaterial()->GetRadLen();
    fZabs0[nAbs][0] = zAbs[nAbs] - dzAbs[nAbs];
    fZabs0[nAbs][1] = zAbs[nAbs] + dzAbs[nAbs];
    fX0abs[nAbs]    = radlAbs[nAbs];
    ++nAbs;
  }

  cout << " \n !!! MUCH Absorbers: " << nAbs << "\n Zbeg, Zend, X0:";
  for (Int_t j = 0; j < nAbs; ++j)
    cout << " " << fZabs0[j][0] << ", " << fZabs0[j][1] << ", " << fX0abs[j] << ";";
  cout << endl << endl;

  if (fStatFirst < 0) return kSUCCESS;  // only GEM configuration

  ComputeMatrix();  // compute system matrices
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----  SetParContainers -------------------------------------------------
void CbmMuchFindVectors::SetParContainers() {}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void CbmMuchFindVectors::Exec(Option_t* opt)
{

  fTrackArray->Delete();
  if (fStatFirst < 0) return;  // only GEM configuration

  //FairTask *vecFinderGem = (FairTask*) FairRun::Instance()->GetTask("MuchFindVectorsGem");
  //if (vecFinderGem == NULL) fTrackArray->Delete();

  for (Int_t i = 0; i < fgkStat; ++i) {
    Int_t nVecs = fVectors[i].size();
    for (Int_t j = 0; j < nVecs; ++j)
      delete fVectors[i][j];
    fVectors[i].clear();
    //nVecs = fVectorsHigh[i].size();
    //for (Int_t j = 0; j < nVecs; ++j) delete fVectorsHigh[i][j];
    fVectorsHigh[i].clear();
  }

  // Do all processing

  // Get hits
  GetHits();

  // Build vectors
  MakeVectors();

  // Remove vectors with wrong orientation
  // (using empirical cuts for omega muons at 8 GeV)
  CheckParams();

  // Match vectors from 2 stations
  MatchVectors();

  // Go to the high resolution mode processing
  Double_t err = fErrU;
  //fErrU = 0.02;
  fErrU = 0.04;
  if (fErrU < 0.1) HighRes();
  fErrU = err;

  // Remove clones
  //RemoveClones();

  // Remove short tracks
  RemoveShorts();

  // Store vectors
  StoreVectors();
}
// -------------------------------------------------------------------------

// -----   Public method Finish   ------------------------------------------
void CbmMuchFindVectors::Finish()
{
  fTrackArray->Clear();
  for (Int_t i = 0; i < fgkStat; ++i) {
    Int_t nVecs = fVectors[i].size();
    for (Int_t j = 0; j < nVecs; ++j)
      delete fVectors[i][j];
    //nVecs = fVectorsHigh[i].size();
    //for (Int_t j = 0; j < nVecs; ++j) delete fVectorsHigh[i][j];
  }
  for (map<Int_t, TDecompLU*>::iterator it = fLus.begin(); it != fLus.end(); ++it)
    delete it->second;
}
// -------------------------------------------------------------------------

// -----   Private method ComputeMatrix   ----------------------------------
void CbmMuchFindVectors::ComputeMatrix()
{
  // Compute system matrices for different hit plane patterns

  Double_t cos2[fgkPlanes], sin2[fgkPlanes], sincos[fgkPlanes], dz2[fgkPlanes];
  Bool_t onoff[fgkPlanes];

  for (Int_t i = 0; i < fgkPlanes; ++i) {
    cos2[i]   = fCosa[i] * fCosa[i];
    sin2[i]   = fSina[i] * fSina[i];
    sincos[i] = fSina[i] * fCosa[i];
    dz2[i]    = fDz[i] * fDz[i];
    onoff[i]  = kTRUE;
  }

  TMatrixD coef(4, 4);
  Int_t pattMax = 1 << fgkPlanes, nDouble = 0, nTot = 0;

  // Loop over doublet patterns
  for (Int_t ipat = 1; ipat < pattMax; ++ipat) {

    // Check if the pattern is valid:
    // either all doublets are active or 3 the first ones (for high resolution mode)
    nDouble = 0;
    for (Int_t j = 0; j < fgkPlanes; j += 2)
      if (ipat & (3 << j)) ++nDouble;
      else
        break;
    if ((ipat & (3 << 6)) == 0) ++nDouble;  // 3 doublets
    if (nDouble < fgkPlanes / 2) continue;
    ++nTot;

    for (Int_t j = 0; j < fgkPlanes; ++j)
      onoff[j] = (ipat & (1 << j));

    coef = 0.0;
    for (Int_t i = 0; i < fgkPlanes; ++i) {
      if (!onoff[i]) continue;
      coef(0, 0) += cos2[i];
      coef(0, 1) += sincos[i];
      coef(0, 2) += fDz[i] * cos2[i];
      coef(0, 3) += fDz[i] * sincos[i];
    }
    for (Int_t i = 0; i < fgkPlanes; ++i) {
      if (!onoff[i]) continue;
      coef(1, 0) += sincos[i];
      coef(1, 1) += sin2[i];
      coef(1, 2) += fDz[i] * sincos[i];
      coef(1, 3) += fDz[i] * sin2[i];
    }
    for (Int_t i = 0; i < fgkPlanes; ++i) {
      if (!onoff[i]) continue;
      coef(2, 0) += fDz[i] * cos2[i];
      coef(2, 1) += fDz[i] * sincos[i];
      coef(2, 2) += dz2[i] * cos2[i];
      coef(2, 3) += dz2[i] * sincos[i];
    }
    for (Int_t i = 0; i < fgkPlanes; ++i) {
      if (!onoff[i]) continue;
      coef(3, 0) += fDz[i] * sincos[i];
      coef(3, 1) += fDz[i] * sin2[i];
      coef(3, 2) += dz2[i] * sincos[i];
      coef(3, 3) += dz2[i] * sin2[i];
    }

    TDecompLU* lu = new TDecompLU(4);
    lu->SetMatrix(coef);
    lu->SetTol(0.1 * lu->GetTol());
    lu->Decompose();
    fLus.insert(pair<Int_t, TDecompLU*>(ipat, lu));
    TMatrixDSym cov(4);
    cov.SetMatrixArray(coef.GetMatrixArray());
    cov.Invert();  // covar. matrix
    fMatr.insert(pair<Int_t, TMatrixDSym*>(ipat, new TMatrixDSym(cov)));
    TString buf = "";
    for (Int_t jp = 0; jp < fgkPlanes; ++jp)
      buf += Bool_t(ipat & (1 << jp));
    cout << " Determinant: " << buf << " " << ipat << " " << coef.Determinant() << endl;
    if (ipat == 255) {
      coef.Print();
      cout << " Number of configurations: " << nTot << endl;
    }
    cov *= (0.02 * 0.02);
    cout << TMath::Sqrt(cov(0, 0)) << " " << TMath::Sqrt(cov(1, 1)) << " " << TMath::Sqrt(cov(2, 2)) << " "
         << TMath::Sqrt(cov(3, 3)) << endl;
  }
}
// -------------------------------------------------------------------------

// -----   Private method GetHits   ----------------------------------------
void CbmMuchFindVectors::GetHits()
{
  // Group hits according to their plane number

  for (Int_t ista = 0; ista < fgkStat; ++ista) {
    for (Int_t i = 0; i < fgkPlanes; ++i)
      fHitPl[ista][i].clear();
    for (Int_t i = 0; i < fgkPlanes / 2; ++i)
      fHit2d[ista][i].clear();
  }

  Int_t nHits = fHits->GetEntriesFast(), nSelTu[fgkStat] = {0}, sel = 0;
  for (Int_t i = 0; i < nHits; ++i) {
    CbmMuchStrawHit* hit = (CbmMuchStrawHit*) fHits->UncheckedAt(i);

    //!!! For debug - select hits with certain track IDs
    sel = 1;  //SelectHitId(hit);
    if (!sel) continue;
    //

    Int_t detId           = hit->GetAddress();
    CbmMuchModule* module = fGeoScheme->GetModuleByDetId(detId);
    if (module->GetDetectorType() != 2) continue;  // skip GEM hits
    UInt_t address = CbmMuchAddress::GetElementAddress(detId, kMuchModule);
    Int_t station3 = CbmMuchAddress::GetStationIndex(address);    // station
    Int_t doublet  = CbmMuchAddress::GetLayerIndex(address);      // doublet
    Int_t layer    = CbmMuchAddress::GetLayerSideIndex(address);  // layer in doublet
    //cout << hit->GetZ() << " " << station3 << " " << doublet << " " << layer << endl;
    //Int_t plane = (station3 - fStatFirst) * 8 + doublet * 2 + layer;
    Int_t plane = doublet * 2 + layer;
    fHitPl[station3 - fStatFirst][plane].insert(pair<Int_t, Int_t>(hit->GetTube() + 1000 * hit->GetSegment(), i));
    if (fErrU < 0) fErrU = hit->GetDu();
    if (sel) ++nSelTu[station3 - fStatFirst];
  }

  // Merge neighbouring hits from 2 layers of 1 doublet.
  // If there is no neighbour, takes hit from a single layer (to account for inefficiency)
  Int_t nlay2           = fgkPlanes / 2, indx1, indx2, tube1, tube2, next1, next2;
  CbmMuchStrawHit *hit1 = NULL, *hit2 = NULL;

  for (Int_t ista = 0; ista < fgkStat; ++ista) {
    // Loop over stations
    Int_t nSelDouble = 0;

    for (Int_t i1 = 0; i1 < nlay2; ++i1) {
      /* Debug
      cout << " Doublet: " << ista << " " << i1 << " " << fHitPl[ista][i1*2].size() << " " << fHitPl[ista][i1*2+1].size() << endl << " Layer 1: " << endl;
      for (multimap<Int_t,Int_t>::iterator it = fHitPl[ista][i1*2].begin(); it != fHitPl[ista][i1*2].end(); ++it) 
	cout << it->second << " ";
      cout << endl;
      for (multimap<Int_t,Int_t>::iterator it = fHitPl[ista][i1*2].begin(); it != fHitPl[ista][i1*2].end(); ++it) 
	cout << it->first << " ";
      cout << endl << " Layer 2: " << endl;
      for (multimap<Int_t,Int_t>::iterator it = fHitPl[ista][i1*2+1].begin(); it != fHitPl[ista][i1*2+1].end(); ++it) 
	cout << it->second << " ";
      cout << endl;
      for (multimap<Int_t,Int_t>::iterator it = fHitPl[ista][i1*2+1].begin(); it != fHitPl[ista][i1*2+1].end(); ++it) 
	cout << it->first << " ";
      cout << endl;
      */

      // Loop over doublets
      multimap<Int_t, Int_t>::iterator it1 = fHitPl[ista][i1 * 2].begin(), it2 = fHitPl[ista][i1 * 2 + 1].begin();
      multimap<Int_t, Int_t>::iterator it1end = fHitPl[ista][i1 * 2].end(), it2end = fHitPl[ista][i1 * 2 + 1].end();
      set<Int_t> tubeOk[2];
      next1 = next2 = 1;
      if (it1 == it1end) next1 = 0;
      if (it2 == it2end) next2 = 0;

      while (next1 || next2) {
        // Loop over tubes
        if (next1) {
          indx1 = it1->second;
          hit1  = (CbmMuchStrawHit*) fHits->UncheckedAt(indx1);
          tube1 = it1->first;
        }
        if (next2) {
          indx2 = it2->second;
          hit2  = (CbmMuchStrawHit*) fHits->UncheckedAt(indx2);
          tube2 = it2->first;
        }

        if (it2 != it2end && tube2 < tube1 || it1 == it1end) {
          // Single layer hit2 ?
          if (tubeOk[1].find(tube2) == tubeOk[1].end()) {
            sel = SelDoubleId(-1, indx2);
            nSelDouble += sel;
            if (sel) fHit2d[ista][i1].push_back(pair<Int_t, Int_t>(-1, indx2));
            tubeOk[1].insert(tube2);
          }
          ++it2;
          next2 = 1;
          next1 = 0;
          if (it2 == fHitPl[ista][i1 * 2 + 1].end()) next2 = 0;
          continue;
        }
        if (it1 != it1end && tube2 > tube1 + 1 || it2 == it2end) {
          // Single layer hit1 ?
          if (tubeOk[0].find(tube1) == tubeOk[0].end()) {
            sel = SelDoubleId(indx1, -1);
            nSelDouble += sel;
            if (sel) fHit2d[ista][i1].push_back(pair<Int_t, Int_t>(indx1, -1));
            tubeOk[0].insert(tube1);
          }
          ++it1;
          next1 = 1;
          next2 = 0;
          if (it1 == fHitPl[ista][i1 * 2].end()) next1 = 0;
          continue;
        }
        // Double layer hit
        sel = SelDoubleId(indx1, indx2);
        nSelDouble += sel;
        if (sel) fHit2d[ista][i1].push_back(pair<Int_t, Int_t>(indx1, indx2));
        tubeOk[0].insert(tube1);
        tubeOk[1].insert(tube2);
        if (tube2 == tube1) {
          ++it2;
          next2 = 1;
          next1 = 0;
          if (it2 == fHitPl[ista][i1 * 2 + 1].end()) {
            next2 = 0;
            next1 = 1;
            if (it1 == fHitPl[ista][i1 * 2].end()) next1 = 0;
          }
        }
        else {
          ++it1;
          next1 = 1;
          next2 = 0;
          if (it1 == fHitPl[ista][i1 * 2].end()) {
            next1 = 0;
            next2 = 1;
            if (it2 == fHitPl[ista][i1 * 2 + 1].end()) next2 = 0;
          }
        }
        continue;
      }  // while (next1...
    }
    cout << " Selected tubes: " << ista << " " << nSelTu[ista] << endl;

    cout << " Selected doublets: " << ista << " " << nSelDouble << endl;
  }
}
// -------------------------------------------------------------------------

// -----   Private method SelectHitId   ------------------------------------
Bool_t CbmMuchFindVectors::SelectHitId(const CbmMuchStrawHit* hit)
{
  // Select hits with certain track IDs (for debugging)

  Int_t nSel = 2, idSel[2] = {0, 1}, id = 0;

  for (Int_t i = 0; i < nSel; ++i) {
    if (hit->GetFlag() % 2) {
      //if (0) {
      // Mirror hit
      return kFALSE;
    }
    else {
      CbmMuchDigiMatch* digiM = (CbmMuchDigiMatch*) fDigiMatches->UncheckedAt(hit->GetRefId());
      Int_t np                = digiM->GetNofLinks();
      for (Int_t ip = 0; ip < np; ++ip) {
        CbmMuchPoint* point = (CbmMuchPoint*) fPoints->UncheckedAt(digiM->GetLink(ip).GetIndex());
        id                  = point->GetTrackID();
        if (id == idSel[i]) return kTRUE;
      }
    }
  }
  return kFALSE;
}
// -------------------------------------------------------------------------

// -----   Private method SelDoubleId   ------------------------------------
Bool_t CbmMuchFindVectors::SelDoubleId(Int_t indx1, Int_t indx2)
{
  // Select doublets with certain track IDs (for debugging)

  return kTRUE;  // no selection

  Int_t nId = 2, idSel[2] = {0, 1}, id = 0;
  CbmMuchStrawHit* hit    = NULL;
  CbmMuchDigiMatch* digiM = NULL;
  CbmMuchPoint* point     = NULL;

  if (indx1 >= 0) {
    hit = (CbmMuchStrawHit*) fHits->UncheckedAt(indx1);
    if (hit->GetFlag() % 2 == 0) {
      // Not a mirror hit
      for (Int_t i = 0; i < nId; ++i) {
        digiM    = (CbmMuchDigiMatch*) fDigiMatches->UncheckedAt(hit->GetRefId());
        Int_t np = digiM->GetNofLinks();
        for (Int_t ip = 0; ip < np; ++ip) {
          point = (CbmMuchPoint*) fPoints->UncheckedAt(digiM->GetLink(ip).GetIndex());
          id    = point->GetTrackID();
          if (id == idSel[i]) return kTRUE;
        }
      }
    }
  }
  if (indx2 >= 0) {
    hit = (CbmMuchStrawHit*) fHits->UncheckedAt(indx2);
    if (hit->GetFlag() % 2 == 0) {
      // Not a mirror hit
      for (Int_t i = 0; i < nId; ++i) {
        digiM    = (CbmMuchDigiMatch*) fDigiMatches->UncheckedAt(hit->GetRefId());
        Int_t np = digiM->GetNofLinks();
        for (Int_t ip = 0; ip < np; ++ip) {
          point = (CbmMuchPoint*) fPoints->UncheckedAt(digiM->GetLink(ip).GetIndex());
          id    = point->GetTrackID();
          if (id == idSel[i]) return kTRUE;
        }
      }
    }
  }
  return kFALSE;
}
// -------------------------------------------------------------------------

// -----   Private method MakeVectors   ------------------------------------
void CbmMuchFindVectors::MakeVectors()
{
  // Make vectors for stations

  for (Int_t ista = 0; ista < fgkStat; ++ista) {
    Int_t nvec = fVectors[ista].size();
    for (Int_t j = 0; j < nvec; ++j)
      delete fVectors[ista][j];
    fVectors[ista].clear();
    Int_t lay2 = 0, patt = 0, flag = 0, ndoubl = fHit2d[ista][lay2].size();
    CbmMuchStrawHit* hit = NULL;

    cout << " Doublets: " << ista << " " << ndoubl << endl;
    for (Int_t id = 0; id < ndoubl; ++id) {
      Int_t indx1 = fHit2d[ista][lay2][id].first;
      Int_t indx2 = fHit2d[ista][lay2][id].second;
      if (indx1 >= 0) {
        hit              = (CbmMuchStrawHit*) fHits->UncheckedAt(indx1);
        fUz[lay2 * 2][0] = hit->GetU();
        //fUz[lay2*2][2] = hit->GetPhi();
        fUzi[lay2 * 2][0] = hit->GetSegment();
        fUzi[lay2 * 2][1] = indx1;
        fUzi[lay2 * 2][2] = hit->GetTube();
      }
      if (indx2 >= 0) {
        hit                  = (CbmMuchStrawHit*) fHits->UncheckedAt(indx2);
        fUz[lay2 * 2 + 1][0] = hit->GetU();
        //fUz[lay2*2+1][2] = hit->GetPhi();
        fUzi[lay2 * 2 + 1][0] = hit->GetSegment();
        fUzi[lay2 * 2 + 1][1] = indx2;
        fUzi[lay2 * 2 + 1][2] = hit->GetTube();
      }
      Bool_t ind1 = indx1 + 1;
      Bool_t ind2 = indx2 + 1;
      patt        = ind1;
      patt |= (ind2 << 1);
      //cout << plane0 << " " << hit->GetX() << " " << hit->GetY() << " " << hit->GetZ() << " " << hit->GetU() << " " << u << endl;
      ProcessDouble(ista, lay2 + 1, patt, flag, hit->GetTube(), hit->GetSegment());
    }
  }
}
// -------------------------------------------------------------------------

// -----   Private method ProcessDouble   ----------------------------------
void CbmMuchFindVectors::ProcessDouble(Int_t ista, Int_t lay2, Int_t patt, Int_t flag, Int_t tube0, Int_t segment0)
{
  // Main processing engine (recursively adds doublet hits to the vector)

  // !!! Tube differences between the same views for mu from omega at 8 GeV !!!
  const Int_t dTubes2 = 30;
  Double_t pars[4]    = {0.0};

  Int_t ndoubl = fHit2d[ista][lay2].size();

  for (Int_t id = 0; id < ndoubl; ++id) {
    Int_t indx1          = fHit2d[ista][lay2][id].first;
    Int_t indx2          = fHit2d[ista][lay2][id].second;
    CbmMuchStrawHit* hit = (CbmMuchStrawHit*) fHits->UncheckedAt(TMath::Max(indx1, indx2));
    Int_t segment        = hit->GetSegment();
    Int_t tube           = hit->GetTube();
    //if (segment != segment0) continue; // do not combine two half-stations - wrong due to stereo angles

    if (TMath::Abs(tube - tube0) > fDtubes[ista][lay2 - 1]) continue;  // !!! cut

    // Check the same views
    Int_t ok = 1;
    for (Int_t il = 0; il < lay2; ++il) {
      Int_t pl = il * 2;
      if (TMath::Abs(fSina[pl] - fSina[lay2 * 2]) < 0.01) {
        // The same views
        Int_t seg = fUzi[pl][0];
        if (!(patt & (1 << pl))) seg = fUzi[pl + 1][0];
        if (segment != seg) {
          ok = 0;
          break;
        }
        // Check tube difference
        Double_t z = fDz[pl];
        Int_t tu   = fUzi[pl][2];
        if (!(patt & (1 << pl))) {
          z  = fDz[pl + 1];
          tu = fUzi[pl + 1][2];
        }
        z += fZ0[ista];
        Double_t dzz = (hit->GetZ() - z) / z;
        if (TMath::Abs(tube - tu - dzz * tu) > dTubes2) {
          ok = 0;
          break;
        }  // !!! cut
      }
    }
    if (!ok) continue;  // !!! cut

    /*
    if (lay2 > 1) {
      // Tube difference with previous to previous doublet
      Int_t pl = (lay2 - 2) * 2;
      Int_t tu = fUzi[pl][2];
      if (patt & (1 << pl+1)) tu = fUzi[pl+1][2];
      Double_t dtu = tube - tu;
      if (lay2 == 3) dtu -= slope[ista] * tu;
      //if (TMath::Abs(dtu) > dTubes2[lay2-2]) continue; // !!! cut
    }
    */

    if (indx1 >= 0) {
      hit              = (CbmMuchStrawHit*) fHits->UncheckedAt(indx1);
      fUz[lay2 * 2][0] = hit->GetU();
      //fUz[lay2*2][2] = hit->GetPhi();
      fUzi[lay2 * 2][0] = hit->GetSegment();
      fUzi[lay2 * 2][1] = indx1;
      fUzi[lay2 * 2][2] = hit->GetTube();
    }
    if (indx2 >= 0) {
      hit                  = (CbmMuchStrawHit*) fHits->UncheckedAt(indx2);
      fUz[lay2 * 2 + 1][0] = hit->GetU();
      //fUz[lay2*2+1][2] = hit->GetPhi();
      fUzi[lay2 * 2 + 1][0] = hit->GetSegment();
      fUzi[lay2 * 2 + 1][1] = indx2;
      fUzi[lay2 * 2 + 1][2] = hit->GetTube();
    }

    // Check intersection
    //if (!IntersectViews(ista, lay2, indx1, indx2, patt)) continue;

    Bool_t ind1 = indx1 + 1;
    Bool_t ind2 = indx2 + 1;
    // Clear bits
    patt &= ~(1 << lay2 * 2);
    patt &= ~(1 << lay2 * 2 + 1);
    // Set bits
    patt |= (ind1 << lay2 * 2);
    patt |= (ind2 << lay2 * 2 + 1);
    //cout << plane << " " << patt << " " << hit->GetX() << " " << hit->GetY() << " " << hit->GetZ() << " " << hit->GetU() << " " << u << endl;

    if (lay2 + 1 < fgkPlanes / 2) ProcessDouble(ista, lay2 + 1, patt, flag, tube, segment);
    else {
      // Straight line fit of the vector
      FindLine(patt, pars);
      Double_t chi2 = FindChi2(ista, patt, pars);
      //cout << " *** Stat: " << ista << " " << id << " " << indx1 << " " << indx2 << " " << chi2 << " " << pars[0] << " " << pars[1] << endl;
      if (chi2 <= fCutChi2) AddVector(ista, patt, chi2, pars);  // add vector to the temporary container
    }
  }  // for (id = 0; id < ndoubl;
}
// -------------------------------------------------------------------------

// -----   Private method IntersectViews   ---------------------------------
Bool_t CbmMuchFindVectors::IntersectViews(Int_t ista, Int_t curLay, Int_t indx1, Int_t indx2, Int_t patt)
{
  // Intersect 2 views

  Int_t lay2 = curLay * 2;
  if (indx1 < 0) ++lay2;
  Double_t u2 = fUz[lay2][0];

  Int_t lay1 = curLay * 2 - 2;
  if (!(patt & (1 << lay1))) ++lay1;
  Double_t u1 = fUz[lay1][0];

  Double_t yint = u1 * fCosa[lay2] - u2 * fCosa[lay1];
  yint /= (fSina[lay1] * fCosa[lay2] - fSina[lay2] * fCosa[lay1]);
  if (TMath::Abs(yint) > fRmax[ista] + 10.0) return kFALSE;  // outside outer dim. + safety margin

  Double_t xint = u2 / fCosa[lay2] - yint * fSina[lay2] / fCosa[lay2];

  Double_t v1 = -xint * fSina[lay1] + yint * fCosa[lay1];
  Double_t v2 = -xint * fSina[lay2] + yint * fCosa[lay2];

  cout << xint << " " << yint << " " << v1 << " " << v2 << " " << fUzi[lay1][0] << " " << fUzi[lay2][0] << endl;
  if (TMath::Abs(v1) > 10.0 && v1 * fUzi[lay1][0] < 0) cout << " Outside !!! " << endl;
  if (TMath::Abs(v2) > 10.0 && v2 * fUzi[lay2][0] < 0) cout << " Outside !!! " << endl;
  if (TMath::Abs(v1) > 30.0 && v1 * fUzi[lay1][0] < 0) return kFALSE;
  if (TMath::Abs(v2) > 30.0 && v2 * fUzi[lay2][0] < 0) return kFALSE;
  return kTRUE;
}
// -------------------------------------------------------------------------

// -----   Private method AddVector   --------------------------------------
void CbmMuchFindVectors::AddVector(Int_t ista, Int_t patt, Double_t chi2, Double_t* pars, Bool_t lowRes)
{
  // Add vector to the temporary container (as a MuchTrack)

  CbmMuchTrack* track = new CbmMuchTrack();
  track->SetChiSq(chi2);
  TMatrixDSym cov(*fMatr[patt]);
  cov *= (fErrU * fErrU);
  //cov *= (0.2 * 0.2); //
  cov.ResizeTo(5, 5);
  FairTrackParam par(pars[0], pars[1], fZ0[ista], pars[2], pars[3], 0.0, cov);
  track->SetParamFirst(&par);
  par.SetZ(fZ0[ista] + fDz[fgkPlanes - 1]);
  par.SetX(pars[0] + fDz[fgkPlanes - 1] * pars[2]);
  par.SetY(pars[1] + fDz[fgkPlanes - 1] * pars[3]);
  track->SetParamLast(&par);
  track->SetUniqueID(ista + fStatFirst);  // just to pass the value

  for (Int_t ipl = 0; ipl < fgkPlanes; ++ipl) {
    if (!(patt & (1 << ipl))) continue;
    track->AddHit(fUzi[ipl][1], kMUCHSTRAWHIT);
    if (lowRes) continue;
    // Store selected hit coordinate (resolved left-right ambig.) as data member fDphi
    CbmMuchStrawHit* hit = (CbmMuchStrawHit*) fHits->UncheckedAt(fUzi[ipl][1]);
    hit->SetDphi(fUz[ipl][0]);
  }
  Int_t ndf = (track->GetNofHits() > 4) ? track->GetNofHits() - 4 : 1;
  track->SetNDF(ndf);
  SetTrackId(track);  // set track ID as its flag
  if (lowRes) fVectors[ista].push_back(track);
  else
    fVectorsHigh[ista].push_back(track);
}
// -------------------------------------------------------------------------

// -----   Private method SetTrackId   -------------------------------------
void CbmMuchFindVectors::SetTrackId(CbmMuchTrack* vec)
{
  // Set vector ID as its flag (maximum track ID of its poins)

  map<Int_t, Int_t> ids;
  Int_t nhits = vec->GetNofHits(), id = 0;

  for (Int_t ih = 0; ih < nhits; ++ih) {
    CbmMuchStrawHit* hit = (CbmMuchStrawHit*) fHits->UncheckedAt(vec->GetHitIndex(ih));
    if (hit->GetFlag() % 2) {
      //if (0) {
      // Mirror hit
      id = -1;
      if (ids.find(id) == ids.end()) ids.insert(pair<Int_t, Int_t>(id, 1));
      else
        ++ids[id];
    }
    else {
      CbmMuchDigiMatch* digiM = (CbmMuchDigiMatch*) fDigiMatches->UncheckedAt(hit->GetRefId());
      Int_t np                = digiM->GetNofLinks();
      for (Int_t ip = 0; ip < np; ++ip) {
        //CbmMuchPoint* point = (CbmMuchPoint*) fPoints->UncheckedAt(digiM->GetLink(0).GetIndex());
        CbmMuchPoint* point = (CbmMuchPoint*) fPoints->UncheckedAt(digiM->GetLink(ip).GetIndex());
        id                  = point->GetTrackID();
        //if (np > 1) cout << ip << " " << id << endl;
        if (ids.find(id) == ids.end()) ids.insert(pair<Int_t, Int_t>(id, 1));
        else
          ++ids[id];
      }
      //if (np > 1) { cout << " digi " << digiM->GetNofLinks() << " " << ista << " " << hit->GetX() << " " << hit->GetY() << endl; exit(0); }
    }
  }
  Int_t maxim = -1, idmax = -9;
  for (map<Int_t, Int_t>::iterator it = ids.begin(); it != ids.end(); ++it) {
    if (it->second > maxim) {
      maxim = it->second;
      idmax = it->first;
    }
  }
  // Set vector ID as its flag
  vec->SetFlag(idmax);
}
// -------------------------------------------------------------------------

// -----   Private method FindLine   ---------------------------------------
void CbmMuchFindVectors::FindLine(Int_t patt, Double_t* pars)
{
  // Fit of hits to the straight line

  // Solve system of linear equations
  Bool_t ok = kFALSE, onoff;
  TVectorD b(4);
  for (Int_t i = 0; i < fgkPlanes; ++i) {
    onoff = patt & (1 << i);
    if (!onoff) continue;
    b[0] += fUz[i][0] * fCosa[i];
    b[1] += fUz[i][0] * fSina[i];
    b[2] += fUz[i][0] * fDz[i] * fCosa[i];
    b[3] += fUz[i][0] * fDz[i] * fSina[i];
  }

  //lu.Print();
  TVectorD solve = fLus[patt]->Solve(b, ok);
  //TVectorD solve = lu.TransSolve(b, ok);
  //lu.Print();
  for (Int_t i = 0; i < 4; ++i)
    pars[i] = solve[i];
  //for (Int_t i = 0; i < 4; ++i) { cout << pars[i] << " "; if (i == 3) cout << endl; }
  //Double_t y1 = cosa / sina * (uz[1][0] * cosa - uz[0][0]) + uz[1][0] * sina;
  //Double_t y2 = -cosa / sina * (uz[2][0] * cosa - uz[0][0]) - uz[2][0] * sina;
  //cout << " OK " << ok << " " << y1 << " " << y2 << endl;
}
// -------------------------------------------------------------------------

// -----   Private method FindChi2   ---------------------------------------
Double_t CbmMuchFindVectors::FindChi2(Int_t ista, Int_t patt, Double_t* pars)
{
  // Compute chi2 of the fit

  Double_t chi2 = 0, x = 0, y = 0, u = 0, errv = 1.;
  if (fErrU > 0.1) errv = 10;
  static Int_t first = 1;
  if (first) {
    first = 0;
    /*
    mcFile->Get("FairBaseParSet");
    cout << gGeoManager << " " << gGeoManager->GetVolume("muchstation05") << " " << gGeoManager->GetVolume("muchstation06") << endl;
    for (Int_t i = 0; i < 2; ++i) {
      TString volName = "muchstation0";
      volName += (i+4);
      TGeoVolume *vol = gGeoManager->GetVolume(volName);
      TGeoTube *shape = (TGeoTube*) vol->GetShape(); 
      rad[i] = shape->GetRmin();
    }
    cout << " Rads: " << rad[0] << " " << rad[1] << endl;
    */
  }

  fChi2Map.clear();
  Bool_t onoff;
  for (Int_t i = 0; i < fgkPlanes; ++i) {
    onoff = patt & (1 << i);
    if (!onoff) continue;
    x           = pars[0] + pars[2] * fDz[i];
    y           = pars[1] + pars[3] * fDz[i];
    u           = x * fCosa[i] + y * fSina[i];
    Double_t du = (u - fUz[i][0]) / fErrU, du2 = du * du;
    chi2 += du2;
    multimap<Double_t, Int_t>::iterator it = fChi2Map.insert(pair<Double_t, Int_t>(du2, i));
    //cout << " " << i << " " << x << " " << y << " " << u << " " <<  fUz[i][0] << " " << du*du << endl;

    // Edge effect
    //if (errv < 2.0) continue; // skip for high-res. mode
    Int_t iseg  = fUzi[i][0];
    Double_t v  = -x * fSina[i] + y * fCosa[i];
    Double_t v0 = 0;
    //cout << v << " " << iseg << endl;
    if (TMath::Abs(fUz[i][0]) > fRmin[ista] && v * iseg > 0) continue;

    if (TMath::Abs(fUz[i][0]) < fRmin[ista]) {
      v0 = TMath::Sign(TMath::Sqrt(fRmin[ista] * fRmin[ista] - fUz[i][0] * fUz[i][0]),
                       iseg * 1.);  // beam hole
      if ((v - v0) * iseg > 0) continue;
    }
    Double_t dv = (v - v0) / errv, dv2 = dv * dv;
    chi2 += dv2;
    fChi2Map.erase(it);
    fChi2Map.insert(pair<Double_t, Int_t>(du2 + dv2, i));
  }
  //cout << " Chi2 = " << chi2 << endl;
  return chi2;
}
// -------------------------------------------------------------------------

// -----   Private method CheckParams   ------------------------------------
void CbmMuchFindVectors::CheckParams()
{
  // Remove vectors with wrong orientation
  // using empirical cuts for omega muons at 8 Gev

  Double_t cut[2] = {0.6, 0.7};  // !!! empirical !!!

  for (Int_t ista = 0; ista < fgkStat; ++ista) {
    Int_t nvec = fVectors[ista].size();

    for (Int_t iv = 0; iv < nvec; ++iv) {
      CbmMuchTrack* vec            = fVectors[ista][iv];
      const FairTrackParam* params = vec->GetParamFirst();
      Double_t dTx                 = params->GetTx() - params->GetX() / params->GetZ();
      Double_t dTy                 = params->GetTy() - params->GetY() / params->GetZ();
      if (TMath::Abs(dTx) > cut[0] || TMath::Abs(dTy) > cut[1]) vec->SetChiSq(-1.0);
    }

    for (Int_t iv = nvec - 1; iv >= 0; --iv) {
      CbmMuchTrack* vec = fVectors[ista][iv];
      if (vec->GetChiSq() < 0) {
        delete fVectors[ista][iv];
        fVectors[ista].erase(fVectors[ista].begin() + iv);
      }
    }
    cout << " Vectors after parameter check in station " << ista << ": " << nvec << " " << fVectors[ista].size()
         << endl;
  }
}
// -------------------------------------------------------------------------

// -----   Private method HighRes   ----------------------------------------
void CbmMuchFindVectors::HighRes()
{
  // High resolution processing (resolve ghost hits and make high resolution vectors)

  for (Int_t ista = 0; ista < fgkStat; ++ista) {
    Int_t nvec = fVectors[ista].size();

    for (Int_t iv = 0; iv < nvec; ++iv) {
      CbmMuchTrack* vec = fVectors[ista][iv];
      Int_t nhits = vec->GetNofHits(), patt = 0, size0 = fVectorsHigh[ista].size();
      Double_t uu[fgkPlanes][2];

      for (Int_t ih = 0; ih < nhits; ++ih) {
        CbmMuchStrawHit* hit = (CbmMuchStrawHit*) fHits->UncheckedAt(vec->GetHitIndex(ih));
        Int_t lay            = fGeoScheme->GetLayerIndex(hit->GetAddress());
        Int_t side           = fGeoScheme->GetLayerSideIndex(hit->GetAddress());
        Int_t plane          = lay * 2 + side;
        uu[plane][0]         = hit->GetU();
        uu[plane][1]         = hit->GetDouble()[2];
        patt |= (1 << plane);
        fUzi[plane][0] = hit->GetSegment();
        fUzi[plane][1] = vec->GetHitIndex(ih);
      }

      // Number of hit combinations is 2
      // - left-right ambiguity resolution does not really work for doublets
      Int_t nCombs = (patt & 1) ? 2 : 1, flag = 1, nok = nCombs - 1;
      fFailed.clear();

      for (Int_t icomb = -1; icomb < nCombs; icomb += 2) {
        fUz[0][0] = (nCombs == 2) ? uu[0][0] + uu[0][1] * icomb : 0.0;
        ProcessSingleHigh(ista, 1, patt, flag, nok, uu);
      }
      if (Int_t(fVectorsHigh[ista].size()) > size0) continue;  // successful fit
      // Failed fit
      RemoveOutliers(ista, patt, uu);

    }  // for (Int_t iv = 0;
  }    // for (Int_t ista = 0;

  MoveVectors();  // move vectors from one container to another, i.e. drop low resolution ones
}
// -------------------------------------------------------------------------

// -----   Private method ProcessDoubleHigh   ------------------------------
void CbmMuchFindVectors::ProcessSingleHigh(Int_t ista, Int_t plane, Int_t patt, Int_t flag, Int_t nok,
                                           Double_t uu[fgkPlanes][2])
{
  // Main processing engine for high resolution mode
  // (recursively adds singlet hits to the vector)

  Double_t pars[4]      = {0.0};
  const Double_t thresh = 10.0;  // chi2-threshold

  // Number of hit combinations is 2
  Int_t nCombs = (patt & (1 << plane)) ? 2 : 1;
  nok += (nCombs - 1);

  for (Int_t icomb = -1; icomb < nCombs; icomb += 2) {
    fUz[plane][0] = (nCombs == 2) ? uu[plane][0] + uu[plane][1] * icomb : 0.0;
    if (plane == fgkPlanes - 1 || nok == fMinHits && flag) {
      // Straight line fit of the vector
      Int_t patt1 = patt & ((1 << plane + 1) - 1);  // apply mask
      FindLine(patt1, pars);
      Double_t chi2 = FindChi2(ista, patt1, pars);
      /*
      TString str;
      str += patt1;
      str = TString::BaseConvert(str,10,2);
      cout << " *** Stat: " << ista << " " << plane << " " << str.CountChar('1') << " " << chi2 << " " 
	   << pars[0] << " " << pars[1] << endl;
      for (multimap<Double_t,Int_t>::reverse_iterator rit = fChi2Map.rbegin(); rit != fChi2Map.rend(); ++rit) 
	cout << rit->first << " ";
      cout << endl;
      */
      if (icomb > -1) flag = 0;
      if (chi2 > fCutChi2) {
        /*
        Double_t qual = 0.0, c2sum = 0.0, outl = 0.0;                           
        for (multimap<Double_t,Int_t>::reverse_iterator rit = fChi2Map.rbegin(); rit != fChi2Map.rend(); ++rit) {
	  if (rit->first > thresh) { outl += 1; c2sum += rit->first; }
	  else break;
	}
        qual = outl + TMath::Min(c2sum,999.) / 1000;
        fFailed.insert(pair<Double_t,multimap<Double_t,Int_t> >(qual,fChi2Map));
	*/
        Int_t lrbits = 0;
        for (Int_t j = 0; j <= plane; ++j) {
          if (!(patt1 & (1 << j))) continue;
          if (fUz[j][0] > uu[j][0]) lrbits |= (1 << j);
        }
        fFailed.insert(pair<Int_t, multimap<Double_t, Int_t>>(lrbits, fChi2Map));
        continue;  // too high chi2 - do not extend line
      }
      //if (plane + 1 < fgkPlanes) ProcessSingleHigh(ista, plane + 1, patt, flag, nok, uu);
      if (plane + 1 < fgkPlanes) ProcessSingleHigh(ista, plane + 1, patt, 0, nok, uu);
      else
        AddVector(ista, patt, chi2, pars,
                  kFALSE);  // add vector to the temporary container
    }
    else {
      ProcessSingleHigh(ista, plane + 1, patt, flag, nok, uu);
    }
  }
}
// -------------------------------------------------------------------------

// -----   Private method MoveVectors   ------------------------------------
void CbmMuchFindVectors::MoveVectors()
{
  // Drop low-resolution vectors and move high-res. ones to their container

  for (Int_t ista = 0; ista < fgkStat; ++ista) {
    Int_t nVecs = fVectors[ista].size();
    for (Int_t j = 0; j < nVecs; ++j)
      delete fVectors[ista][j];
    fVectors[ista].clear();

    nVecs = fVectorsHigh[ista].size();
    for (Int_t j = 0; j < nVecs; ++j)
      fVectors[ista].push_back(fVectorsHigh[ista][j]);
  }
}
// -------------------------------------------------------------------------

// -----   Private method RemoveClones   -----------------------------------
void CbmMuchFindVectors::RemoveClones()
{
  // Remove clone vectors (having at least 1 the same hit in each doublet (min. 4 the same hits))

  Int_t nthr = 4, planes[20];

  for (Int_t ista = 0; ista < fgkStat; ++ista) {
    Int_t nvec = fVectors[ista].size();

    // Do sorting according to "quality"
    multimap<Double_t, CbmMuchTrack*> qMap;
    multimap<Double_t, CbmMuchTrack*>::iterator it, it1;

    for (Int_t iv = 0; iv < nvec; ++iv) {
      CbmMuchTrack* vec = fVectors[ista][iv];
      Double_t qual     = vec->GetNofHits() + (99 - TMath::Min(vec->GetChiSq(), 99.0)) / 100;
      qMap.insert(pair<Double_t, CbmMuchTrack*>(-qual, vec));
    }

    for (it = qMap.begin(); it != qMap.end(); ++it) {
      CbmMuchTrack* vec = it->second;
      if (vec->GetChiSq() < 0) continue;
      for (Int_t j = 0; j < fgkPlanes; ++j)
        planes[j] = -1;

      Int_t nhits = vec->GetNofHits();
      for (Int_t ih = 0; ih < nhits; ++ih) {
        CbmMuchStrawHit* hit   = (CbmMuchStrawHit*) fHits->UncheckedAt(vec->GetHitIndex(ih));
        Int_t lay              = fGeoScheme->GetLayerIndex(hit->GetAddress());
        Int_t side             = fGeoScheme->GetLayerSideIndex(hit->GetAddress());
        planes[lay * 2 + side] = vec->GetHitIndex(ih);
        //cout << iv << " " << lay*2+side << " " << vec->GetHitIndex(ih) << endl;
      }

      it1 = it;
      for (++it1; it1 != qMap.end(); ++it1) {
        CbmMuchTrack* vec1 = it1->second;
        if (vec1->GetChiSq() < 0) continue;
        Int_t nsame = 0, same[fgkPlanes / 2] = {0};

        Int_t nhits1 = vec1->GetNofHits();
        //nthr = TMath::Min(nhits,nhits1) / 2;
        //nthr = TMath::Min(nhits,nhits1) * 0.75;
        for (Int_t ih = 0; ih < nhits1; ++ih) {
          CbmMuchStrawHit* hit = (CbmMuchStrawHit*) fHits->UncheckedAt(vec1->GetHitIndex(ih));
          Int_t lay            = fGeoScheme->GetLayerIndex(hit->GetAddress());
          Int_t side           = fGeoScheme->GetLayerSideIndex(hit->GetAddress());
          if (planes[lay * 2 + side] >= 0) {
            if (vec1->GetHitIndex(ih) == planes[lay * 2 + side]) same[lay] = 1;
            //else same[lay] = 0;
          }
        }
        for (Int_t lay = 0; lay < fgkPlanes / 2; ++lay)
          nsame += same[lay];

        if (nsame >= nthr) {
          // Too many the same hits
          Int_t clone = 0;
          if (nhits > nhits1 + 0) clone = 1;
          //else if (vec->GetChiSq() * 3.0 <= vec1->GetChiSq()) vec1->SetChiSq(-1.0); // the same number of hits on 2 tracks
          else if (fgkPlanes > -8 && vec->GetChiSq() * 1 <= vec1->GetChiSq())
            clone = 1;  // the same number of hits on 2 tracks
          if (clone) {
            vec1->SetChiSq(-1.0);
            /* Debug
	    const FairTrackParam *params = vec->GetParamFirst();
	    const FairTrackParam *params1 = vec1->GetParamFirst();
	    if (vec1->GetFlag() < 2) cout << " Remove: " << vec->GetFlag() << " " << vec1->GetFlag() << " " << nhits << " " << nhits1 << " " << nsame << " " << vec->GetChiSq() << " " << vec1->GetChiSq() << " " << params->GetX() << " " << params1->GetX() << " " << params->GetY() << " " << params1->GetY() << endl;
	    //
	    Double_t dx = params->GetX() - params1->GetX();
	    dx /= 0.2;
	    Double_t dy = params->GetY() - params1->GetY();
	    dy /= 1.5;
	    Double_t chi2 = dx * dx + dy * dy;
	    if (chi2 < 10) vec1->SetChiSq(-1.0);
	    */
          }
        }
      }
    }

    for (Int_t iv = nvec - 1; iv >= 0; --iv) {
      CbmMuchTrack* vec = fVectors[ista][iv];
      if (vec->GetChiSq() < 0) {
        delete fVectors[ista][iv];
        fVectors[ista].erase(fVectors[ista].begin() + iv);
      }
    }
    cout << " Vectors after clones removed: " << nvec << " " << fVectors[ista].size() << endl;

  }  // for (Int_t ista = 0; ista < fgkStat;
}
// -------------------------------------------------------------------------

// -----   Private method RemoveShorts   -----------------------------------
void CbmMuchFindVectors::RemoveShorts()
{
  // Remove short tracks

  Int_t nshort = fgkPlanes / 2, planes[20];

  for (Int_t ista = 0; ista < fgkStat; ++ista) {
    Int_t nvec   = fVectors[ista].size();
    Int_t remove = 0;

    for (Int_t iv = 0; iv < nvec; ++iv) {
      CbmMuchTrack* vec = fVectors[ista][iv];
      if (vec->GetChiSq() < 0) continue;
      Int_t nhits = vec->GetNofHits();
      if (nhits != nshort) continue;
      set<Int_t> overlap;
      //multiset<Int_t> overlap1;
      for (Int_t j = 0; j < fgkPlanes; ++j)
        planes[j] = -1;
      for (Int_t ih = 0; ih < nhits; ++ih) {
        CbmMuchStrawHit* hit   = (CbmMuchStrawHit*) fHits->UncheckedAt(vec->GetHitIndex(ih));
        Int_t lay              = fGeoScheme->GetLayerIndex(hit->GetAddress());
        Int_t side             = fGeoScheme->GetLayerSideIndex(hit->GetAddress());
        planes[lay * 2 + side] = vec->GetHitIndex(ih);
      }

      for (Int_t iv1 = iv + 1; iv1 < nvec; ++iv1) {
        CbmMuchTrack* vec1 = fVectors[ista][iv1];
        if (vec1->GetChiSq() < 0) continue;
        Int_t nhits1 = vec1->GetNofHits();

        // Compare hits
        for (Int_t ih = 0; ih < nhits1; ++ih) {
          CbmMuchStrawHit* hit = (CbmMuchStrawHit*) fHits->UncheckedAt(vec1->GetHitIndex(ih));
          Int_t lay            = fGeoScheme->GetLayerIndex(hit->GetAddress());
          Int_t side           = fGeoScheme->GetLayerSideIndex(hit->GetAddress());
          if (vec1->GetHitIndex(ih) == planes[lay * 2 + side] && planes[lay * 2 + side] >= 0) overlap.insert(ih);
        }
        //if (overlap.size() == nshort) {
        if (overlap.size() > 0) {
          // Hits are shared with other tracks
          remove = 1;
          break;
        }
      }  // for (Int_t iv1 = iv + 1;
      if (remove) vec->SetChiSq(-1.0);
    }  // for (Int_t iv = 0; iv < nvec;

    for (Int_t iv = nvec - 1; iv >= 0; --iv) {
      CbmMuchTrack* vec = fVectors[ista][iv];
      if (vec->GetChiSq() < 0) {
        delete fVectors[ista][iv];
        fVectors[ista].erase(fVectors[ista].begin() + iv);
      }
    }
    cout << " Vectors after shorts removed: " << nvec << " " << fVectors[ista].size() << endl;

  }  // for (Int_t ista = 0;
}
// -------------------------------------------------------------------------

// -----   Private method StoreVectors   -----------------------------------
void CbmMuchFindVectors::StoreVectors()
{
  // Store vectors (CbmMuchTracks) into TClonesArray

  Int_t ntrs     = fTrackArray->GetEntriesFast();
  Int_t nHitsTot = fHits->GetEntriesFast();

  for (Int_t ist = 0; ist < fgkStat; ++ist) {
    set<Int_t> usedHits;
    Int_t nvec = fVectors[ist].size();

    for (Int_t iv = 0; iv < nvec; ++iv) {
      CbmMuchTrack* tr = new ((*fTrackArray)[ntrs++]) CbmMuchTrack(*(fVectors[ist][iv]));
      //cout << " Track: " << tr->GetNofHits() << endl;
      //for (Int_t j = 0; j < tr->GetNofHits(); ++j) cout << j << " " << tr->GetHitIndex(j) << " " << fVectors[ist][iv]->GetHitIndex(j) << endl;
      // Set hit flag (to check Lit tracking)
      Int_t nhits = tr->GetNofHits();
      /*
      for (Int_t j = 0; j < nhits; ++j) {
	CbmMuchStrawHit *hit = (CbmMuchStrawHit*) fHits->UncheckedAt(tr->GetHitIndex(j));
	if (usedHits.find(tr->GetHitIndex(j)) == usedHits.end()) {
	  hit->SetFlag(ntrs-1);
	  usedHits.insert(tr->GetHitIndex(j));
	} else {
	  // Ugly interim solution for the tracking - create new hit
	  CbmMuchStrawHit *hitNew = new ((*fHits)[nHitsTot++]) CbmMuchStrawHit(*hit);
	  hitNew->SetFlag(ntrs-1);
	  usedHits.insert(nHitsTot-1);
	}
      }
      */
    }
  }
}
// -------------------------------------------------------------------------

// -----   Private method CountBits   --------------------------------------
//This is better when most bits in x are 0
//It uses 3 arithmetic operations and one comparison/branch per "1" bit in x.
// Wikipedia "Hamming weight"
Int_t CbmMuchFindVectors::CountBits(Int_t x)
{

  Int_t count;
  for (count = 0; x; count++)
    x &= x - 1;
  return count;
}
// -------------------------------------------------------------------------

// -----   Private method MatchVectors   -----------------------------------
void CbmMuchFindVectors::MatchVectors()
{
  // Match vectors from 2 stations

  const Int_t iabs            = 3;
  CbmMuchMergeVectors* merger = (CbmMuchMergeVectors*) FairRun::Instance()->GetTask("MuchMergeVectors");
  TMatrixF matr               = TMatrixF(5, 5);
  TMatrixF unit(TMatrixF::kUnit, matr);

  for (Int_t ista = 0; ista < fgkStat; ++ista) {
    Int_t nvec = fVectors[ista].size();

    // Propagate vectors to the absorber face
    for (Int_t iv = 0; iv < nvec; ++iv) {
      CbmMuchTrack* vec       = fVectors[ista][iv];
      FairTrackParam parFirst = *vec->GetParamFirst();
      Double_t zbeg           = parFirst.GetZ();
      Double_t dz             = fZabs0[iabs][0] - zbeg;
      // Propagate params
      parFirst.SetX(parFirst.GetX() + dz * parFirst.GetTx());
      parFirst.SetY(parFirst.GetY() + dz * parFirst.GetTy());
      parFirst.SetZ(parFirst.GetZ() + dz);
      TMatrixFSym cov(5);
      parFirst.CovMatrix(cov);
      cov(4, 4)   = 1.0;
      TMatrixF ff = unit;
      ff(2, 0) = ff(3, 1) = dz;
      TMatrixF cf(cov, TMatrixF::kMult, ff);
      TMatrixF fcf(ff, TMatrixF::kTransposeMult, cf);
      cov.SetMatrixArray(fcf.GetMatrixArray());
      if (ista == 1) merger->PassAbsorber(ista + iabs * 2, fZabs0[iabs], fX0abs[iabs], parFirst, cov, 0);
      cov.Invert();  // weight matrix
      parFirst.SetCovMatrix(cov);
      vec->SetParamLast(&parFirst);
    }
  }

  Int_t ista0 = 0, ista1 = 1;
  vector<Int_t> matchOK[2];
  Int_t nvec0 = fVectors[ista0].size(), nvec1 = fVectors[ista1].size();
  matchOK[0].assign(nvec0, -1);
  matchOK[1].assign(nvec1, -1);

  for (Int_t iv = 0; iv < nvec0; ++iv) {
    CbmMuchTrack* tr1    = fVectors[ista0][iv];
    FairTrackParam parOk = *tr1->GetParamLast();
    FairTrackParam par1  = *tr1->GetParamLast();
    TMatrixFSym w1(5);
    par1.CovMatrix(w1);
    Float_t pars1[5] = {(Float_t) par1.GetX(), (Float_t) par1.GetY(), (Float_t) par1.GetTx(), (Float_t) par1.GetTy(),
                        1.0};
    TMatrixF p1(5, 1, pars1);
    TMatrixF wp1(w1, TMatrixF::kTransposeMult, p1);

    for (Int_t iv1 = 0; iv1 < nvec1; ++iv1) {
      CbmMuchTrack* tr2   = fVectors[ista1][iv1];
      FairTrackParam par2 = *tr2->GetParamLast();
      TMatrixFSym w2(5);
      par2.CovMatrix(w2);
      TMatrixFSym w20  = w2;
      Float_t pars2[5] = {(Float_t) par2.GetX(), (Float_t) par2.GetY(), (Float_t) par2.GetTx(), (Float_t) par2.GetTy(),
                          1.0};
      TMatrixF p2(5, 1, pars2);
      TMatrixF wp2(w2, TMatrixF::kTransposeMult, p2);
      wp2 += wp1;
      w2 += w1;
      w2.Invert();
      TMatrixF p12(w2, TMatrixF::kTransposeMult, wp2);

      // Compute Chi2
      TMatrixF p122   = p12;
      TMatrixF pMerge = p12;
      p12 -= p1;
      TMatrixF wDp1(w1, TMatrixF::kMult, p12);
      TMatrixF chi21(p12, TMatrixF::kTransposeMult, wDp1);
      p122 -= p2;
      TMatrixF wDp2(w20, TMatrixF::kMult, p122);
      TMatrixF chi22(p122, TMatrixF::kTransposeMult, wDp2);
      Double_t c2 = chi21(0, 0) + chi22(0, 0);
      //cout << " Chi2: " << chi21(0,0) << " " << chi22(0,0) << " " << c2 << endl;
      if (c2 < 0 || c2 > fCutChi2 * 2) continue;
      matchOK[0][iv] = matchOK[1][iv1] = 1;  // match OK
      // Merged track parameters
      parOk.SetX(pMerge(0, 0));
      parOk.SetY(pMerge(1, 0));
      parOk.SetZ(par2.GetZ());
      parOk.SetTx(pMerge(2, 0));
      parOk.SetTy(pMerge(3, 0));
      parOk.SetCovMatrix(w2);
      //AddTrack(ista0, tr1, tr2, it->first, it1->first, parOk, c2); // add track
      //Int_t evNo = FairRun::Instance()->GetEventHeader()->GetMCEntryNumber();
    }
  }

  for (Int_t ista = 0; ista < fgkStat; ++ista) {
    Int_t nvec = fVectors[ista].size();

    for (Int_t iv = nvec - 1; iv >= 0; --iv) {
      if (matchOK[ista][iv] > 0) continue;
      fVectors[ista].erase(fVectors[ista].begin() + iv);
    }
  }

  cout << " Vectors after matching: " << fVectors[0].size() << " " << fVectors[1].size() << endl;
}
// -------------------------------------------------------------------------

// -----   Private method RemoveOutliers   ---------------------------------
void CbmMuchFindVectors::RemoveOutliers(Int_t ista, Int_t patt0, Double_t uu[fgkPlanes][2])
{
  // Find line with the lowest number of outliers and smallest sum of their chi2
  // - remove outliers and refit

  const Double_t thresh = 6.0;  // chi2-cut
  Double_t qualmin = 99.0, qual = 0.0;
  multimap<Int_t, multimap<Double_t, Int_t>>::iterator mit = fFailed.begin(), mit0 = mit;
  Int_t patt = patt0, pattmin = patt0;

  for (; mit != fFailed.end(); ++mit) {
    multimap<Double_t, Int_t>& chi2s = mit->second;
    Double_t c2sum = 0.0, outl = 0.0;
    patt = patt0;

    for (multimap<Double_t, Int_t>::reverse_iterator rit = chi2s.rbegin(); rit != chi2s.rend(); ++rit) {
      if (rit->first <= thresh) break;
      outl += 1;
      c2sum += rit->first;
      patt &= ~(1 << rit->second);
    }
    // Check doublets
    Int_t nDouble = 0;
    for (Int_t j = 0; j < fgkPlanes; j += 2)
      if (patt & (3 << j)) ++nDouble;
      else
        break;
    if (nDouble < fgkPlanes / 2) continue;
    qual = outl + TMath::Min(c2sum, 999.) / 1000;
    if (qual < qualmin) {
      qualmin = qual;
      mit0    = mit;
      pattmin = patt;
    }
  }

  if (Int_t(qualmin) > 3) return;  // too many outliers

  Int_t nCombs = (patt & 1) ? 2 : 1, flag = 1, nok = nCombs - 1;

  for (Int_t icomb = -1; icomb < nCombs; icomb += 2) {
    fUz[0][0] = (nCombs == 2) ? uu[0][0] + uu[0][1] * icomb : 0.0;
    //if (ichoice > 1 && nCombs > 1 && icomb != vecTmp.GetLR(0)) continue;
    //ProcessSingleHigh(ichoice, 1, patt, 1, 1, uu, &vecTmp);
    ProcessSingleHigh(ista, 1, pattmin, flag, nok, uu);
  }
}
// -------------------------------------------------------------------------

ClassImp(CbmMuchFindVectors);
