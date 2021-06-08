/* Copyright (C) 2014-2019 Veksler and Baldin Laboratory of High Energy Physics, Joint Institute for Nuclear Research, Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Alexander Zinchenko [committer] */

/** CbmMuchFindVectorsGem.cxx
 *@author A.Zinchenko <Alexander.Zinchenko@jinr.ru>
 *@since 2014
 **/
#include "CbmMuchFindVectorsGem.h"

#include "CbmMCDataArray.h"
#include "CbmMCDataManager.h"
#include "CbmMuchCluster.h"
#include "CbmMuchDigiMatch.h"
#include "CbmMuchMergeVectors.h"
#include "CbmMuchModule.h"
#include "CbmMuchModuleGem.h"
#include "CbmMuchPixelHit.h"
#include "CbmMuchPoint.h"
#include "CbmMuchStation.h"
#include "CbmMuchTrack.h"
#include "CbmSetup.h"

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
CbmMuchFindVectorsGem::CbmMuchFindVectorsGem()
  : FairTask("MuchFindVectorsGem")
  , fGeoScheme(CbmMuchGeoScheme::Instance())
  , fVecPool(NULL)
  , fTrackArray(NULL)
  , fNofTracks(0)
  , fHits(NULL)
  , fClusters(NULL)
  , fPoints(NULL)
  , fDigiMatches(NULL)
  , fTrdVectors(NULL)
  , fStatFirst(-1)
//fCutChi2(40.0)
{
  for (Int_t i = 0; i < 9; ++i)
    fCutChi2[i] = 12;  // chi2/ndf = 6 for 3 hits
  //for (Int_t i = 0; i < 9; ++i) fCutChi2[i] = 20;
  //AZ fCutChi2[0] *= 2;
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
CbmMuchFindVectorsGem::~CbmMuchFindVectorsGem()
{
  fTrackArray->Delete();
  fVecPool->Delete();

  for (map<Int_t, TDecompLU*>::iterator it = fLus.begin(); it != fLus.end(); ++it)
    delete it->second;
}
// -------------------------------------------------------------------------

// -----   Public method Init (abstract in base class)  --------------------
InitStatus CbmMuchFindVectorsGem::Init()
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
  else {
    // MuchVector already exists (from CbmMuchFindVector task)
    //Fatal("Init", "CbmMuchFindVectors should go after !!!");
  }
  fVecPool = new TClonesArray("CbmMuchTrack", 1000);

  /*
  CbmMuchFindHitsStraws *hitFinder = (CbmMuchFindHitsStraws*) 
    FairRun::Instance()->GetTask("CbmMuchFindHitsStraws");
  if (hitFinder == NULL) Fatal("CbmMuchFindTracks::Init", "CbmMuchFindHitsStraws not run!");
  fDiam = hitFinder->GetDiam(0);
  */

  fHits                       = static_cast<TClonesArray*>(ioman->GetObject("MuchPixelHit"));
  fClusters                   = static_cast<TClonesArray*>(ioman->GetObject("MuchCluster"));
  fDigiMatches                = static_cast<TClonesArray*>(ioman->GetObject("MuchDigiMatch"));
  fTrdVectors                 = static_cast<TClonesArray*>(ioman->GetObject("TrdVector"));
  CbmMCDataManager* mcManager = (CbmMCDataManager*) ioman->GetObject("MCDataManager");
  if (NULL == mcManager) LOG(fatal) << GetName() << ": No CbmMCDataManager!";
  //fPoints = static_cast<TClonesArray*> (ioman->GetObject("MuchPoint"));
  fPoints = mcManager->InitBranch("MuchPoint");

  // Find first GEM station and get some geo constants
  Int_t nSt = fGeoScheme->GetNStations();
  for (Int_t i = 0; i < nSt; ++i) {
    CbmMuchStation* st = fGeoScheme->GetStation(i);
    CbmMuchModule* mod = fGeoScheme->GetModule(i, 0, 0, 0);
    fRmax[i]           = 0;
    if (mod->GetDetectorType() == 2) continue;
    if (fStatFirst < 0) fStatFirst = CbmMuchAddress::GetStationIndex(st->GetDetectorId());
    //cout << " First station: " << fStatFirst << endl;
    Int_t nLays           = st->GetNLayers();
    fRmin[i - fStatFirst] = st->GetRmin();
    fRmax[i - fStatFirst] = st->GetRmax();
    for (Int_t lay = 0; lay < nLays; ++lay) {
      CbmMuchLayer* layer = st->GetLayer(lay);
      //Double_t phi = hitFinder->GetPhi(lay);
      for (Int_t iside = 0; iside < 2; ++iside) {
        CbmMuchLayerSide* side = layer->GetSide(iside);
        Int_t plane            = lay * 2 + iside;
        fDz[plane]             = side->GetZ();
        //fCosa[plane] = TMath::Cos(phi);
        //fSina[plane] = TMath::Sin(phi);
        if (plane == 0) fZ0[i - fStatFirst] = side->GetZ();
        fNsect[i - fStatFirst] = side->GetNModules() * 2;
        /*
	if (lay) {
	  fDtubes[i-fStatFirst][lay-1] = 
	    fRmax[i-fStatFirst] * TMath::Tan (TMath::ASin(fSina[plane]) - TMath::ASin(fSina[plane-2]));
	  fDtubes[i-fStatFirst][lay-1] = TMath::Abs(fDtubes[i-fStatFirst][lay-1]) / fDiam + 10;
	}
	*/
      }
    }
  }
  for (Int_t i = fgkPlanes - 1; i >= 0; --i) {
    fDz[i] -= fDz[0];
    //cout << fDz[i] << " ";
  }
  //cout << endl;

  // Get absorbers
  TString tag, fileName;
  CbmSetup::Instance()->GetGeoTag(kMuch, tag);
  //CbmSetup::Instance()->GetGeoFileName(kMuch, fileName);
  cout << " ******* MUCH tag ******** " << tag << endl;
  tag.Prepend("much_");

  Double_t dzAbs[9] = {0}, zAbs[9] = {0}, radlAbs[9] = {0}, xyzl[3] = {0}, xyzg[3] = {0};
  Int_t nAbs      = 0;
  TGeoVolume* vol = NULL;
  TGeoNode* much  = NULL;

  // Go to MUCH
  gGeoManager->CdTop();
  TGeoNode* cave = gGeoManager->GetCurrentNode();
  for (Int_t iNode = 0; iNode < cave->GetNdaughters(); iNode++) {
    TGeoNode* node = cave->GetDaughter(iNode);
    TString name   = node->GetName();
    if (name.Contains("much")) {
      much = node;
      gGeoManager->CdDown(iNode);
      break;
    }
  }

  TGeoVolume* muchV = gGeoManager->GetVolume(tag);
  Int_t ndaught     = muchV->GetNdaughters();

  // Loop over daughters
  for (Int_t i = 0; i < ndaught; ++i) {
    vol = muchV->GetNode(i)->GetVolume();
    if (!(TString(vol->GetName()).Contains("absorber"))) continue;
    gGeoManager->CdDown(i);
    nAbs = vol->GetNdaughters();
    //cout << " aaaaa " << nAbs << endl;

    for (Int_t iabs = 0; iabs < nAbs; ++iabs) {
      TGeoVolume* block = vol->GetNode(iabs)->GetVolume();
      gGeoManager->CdDown(iabs);
      TGeoVolume* abso = block->GetNode(0)->GetVolume();
      gGeoManager->CdDown(0);
      //cout << gGeoManager->GetPath() << endl;
      gGeoManager->LocalToMaster(xyzl, xyzg);
      zAbs[iabs] = xyzg[2];
      //cout << abso->GetName() << " " << abso->GetShape()->GetName() << " "
      //   << ((TGeoBBox*)abso->GetShape())->GetDZ() << " " << xyzg[2] << endl;
      dzAbs[iabs]     = ((TGeoBBox*) abso->GetShape())->GetDZ();
      radlAbs[iabs]   = abso->GetMaterial()->GetRadLen();
      fZabs0[iabs][0] = zAbs[iabs] - dzAbs[iabs];
      fZabs0[iabs][1] = zAbs[iabs] + dzAbs[iabs];
      fX0abs[iabs]    = radlAbs[iabs];
      gGeoManager->CdUp();
      gGeoManager->CdUp();
    }
  }

  // Combine 2 first absorbers into 1
  for (Int_t i = 1; i < nAbs; ++i) {
    if (i == 1) fZabs0[i - 1][1] = fZabs0[i][1];
    else {
      fZabs0[i - 1][0] = fZabs0[i][0];
      fZabs0[i - 1][1] = fZabs0[i][1];
      fX0abs[i - 1]    = fX0abs[i];
    }
  }
  --nAbs;
  fX0abs[nAbs] = 1.e9;  // flag

  cout << " \n !!! MUCH Absorbers: " << nAbs << "\n Zbeg, Zend, X0:";
  for (Int_t j = 0; j < nAbs; ++j)
    cout << " " << std::setprecision(4) << fZabs0[j][0] << ", " << fZabs0[j][1] << ", " << fX0abs[j] << ";";
  cout << endl << endl;

  // Station dependent errors
  //Double_t errs[9] = {0.4, 0.6, 1.2, 1.7, 0};
  Double_t errs[9] = {0.6, 0.8, 1.2, 1.7, 0};
  for (Int_t j = 0; j < fgkStat; ++j)
    fErrX[j] = fErrY[j] = errs[j];

  ComputeMatrix();  // compute system matrices

  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----  SetParContainers -------------------------------------------------
void CbmMuchFindVectorsGem::SetParContainers() {}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void CbmMuchFindVectorsGem::Exec(Option_t* opt)
{

  //fTrackArray->Delete();
  fVecPool->Delete();
  FairTask* strawHitFinder = FairRun::Instance()->GetTask("CbmMuchFindHitsStraws");
  if (strawHitFinder == NULL) fTrackArray->Delete();

  for (Int_t i = 0; i < fgkStat; ++i) {
    fVectors[i].clear();
    fSecVec[i].clear();
  }

  // Do all processing

  // Get hits
  GetHits();

  for (Int_t ista = fgkStat - 1; ista >= 0; --ista) {
    // Build vectors
    MakeVectors(ista);

    // Remove vectors with wrong orientation
    // (using empirical cuts for omega muons at 8 GeV)
    CheckParams(ista);

    // Match vectors from 2 stations
    //MatchVectors();

    // Remove clones
    RemoveClones(ista);
  }

  // Store vectors
  StoreVectors();
}
// -------------------------------------------------------------------------

// -----   Public method Finish   ------------------------------------------
void CbmMuchFindVectorsGem::Finish()
{
  fTrackArray->Delete();
  fVecPool->Delete();

  for (map<Int_t, TDecompLU*>::iterator it = fLus.begin(); it != fLus.end(); ++it)
    delete it->second;
}
// -------------------------------------------------------------------------

// -----   Private method ComputeMatrix   ----------------------------------
void CbmMuchFindVectorsGem::ComputeMatrix()
{
  // Compute system matrices for different hit plane patterns

  Double_t dz2[fgkPlanes];
  Bool_t onoff[fgkPlanes];

  for (Int_t i = 0; i < fgkPlanes; ++i) {
    dz2[i]   = fDz[i] * fDz[i];
    onoff[i] = kTRUE;
  }

  TMatrixD coef(4, 4);
  Int_t pattMax = 1 << fgkPlanes, nDouble = 0, nTot = 0;

  // Loop over doublet patterns
  for (Int_t ipat = 1; ipat < pattMax; ++ipat) {

    // Check if the pattern is valid: all doublets are active
    nDouble = 0;
    //AZ for (Int_t j = 0; j < fgkPlanes; j += 2) if (ipat & (3 << j)) ++nDouble; else break;
    for (Int_t j = 0; j < fgkPlanes; j += 2)
      if (ipat & (3 << j)) ++nDouble;
    if (nDouble < fgkPlanes / 2 - 1) continue;  // allow 2-hit vectors
    ++nTot;

    for (Int_t j = 0; j < fgkPlanes; ++j)
      onoff[j] = (ipat & (1 << j));

    coef = 0.0;
    for (Int_t i = 0; i < fgkPlanes; ++i) {
      if (!onoff[i]) continue;
      coef(0, 0) += 1;
      coef(0, 1) += 0;
      coef(0, 2) += fDz[i];
      coef(0, 3) += 0;
    }
    for (Int_t i = 0; i < fgkPlanes; ++i) {
      if (!onoff[i]) continue;
      coef(1, 0) += 0;
      coef(1, 1) += 1;
      coef(1, 2) += 0;
      coef(1, 3) += fDz[i];
    }
    for (Int_t i = 0; i < fgkPlanes; ++i) {
      if (!onoff[i]) continue;
      coef(2, 0) += fDz[i];
      coef(2, 1) += 0;
      coef(2, 2) += dz2[i];
      coef(2, 3) += 0;
    }
    for (Int_t i = 0; i < fgkPlanes; ++i) {
      if (!onoff[i]) continue;
      coef(3, 0) += 0;
      coef(3, 1) += fDz[i];
      coef(3, 2) += 0;
      coef(3, 3) += dz2[i];
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
    Info("ComputeMatrix", " Determinant: %s %i %f", buf.Data(), ipat, coef.Determinant());
    if (ipat == 63) {
      coef.Print();
      Info("ComputeMatrix", " Number of configurations: %i", nTot);
    }
    //cout << " Number of configurations: " << nTot << endl; }
    cov *= (1.2 * 1.2);
    cout << TMath::Sqrt(cov(0, 0)) << " " << TMath::Sqrt(cov(1, 1)) << " " << TMath::Sqrt(cov(2, 2)) << " "
         << TMath::Sqrt(cov(3, 3)) << endl;
    cov *= (1.7 * 1.7 / 1.2 / 1.2);
    cout << TMath::Sqrt(cov(0, 0)) << " " << TMath::Sqrt(cov(1, 1)) << " " << TMath::Sqrt(cov(2, 2)) << " "
         << TMath::Sqrt(cov(3, 3)) << endl;
  }
}
// -------------------------------------------------------------------------

// -----   Private method GetHits   ----------------------------------------
void CbmMuchFindVectorsGem::GetHits()
{
  // Group hits according to their plane number

  for (Int_t ista = 0; ista < fgkStat; ++ista) {
    for (Int_t i = 0; i < fgkPlanes; ++i) {
      fHitPl[ista][i].clear();
      fHitX[ista][i].clear();
    }
    //for (Int_t i = 0; i < fgkPlanes / 2; ++i) fHit2d[ista][i].clear();
  }

  Int_t nHits = fHits->GetEntriesFast(), nSelTu[fgkStat] = {0}, sel = 0;
  for (Int_t i = 0; i < nHits; ++i) {
    CbmMuchPixelHit* hit = (CbmMuchPixelHit*) fHits->UncheckedAt(i);

    //!!! For debug - select hits with certain track IDs
    sel = 1;  //SelectHitId(hit);
    if (!sel) continue;
    //

    Int_t detId           = hit->GetAddress();
    CbmMuchModule* module = fGeoScheme->GetModuleByDetId(detId);
    if (module->GetDetectorType() == 2) continue;  // skip straw hits
    UInt_t address = CbmMuchAddress::GetElementAddress(detId, kMuchModule);
    Int_t station3 = CbmMuchAddress::GetStationIndex(address);    // station
    Int_t doublet  = CbmMuchAddress::GetLayerIndex(address);      // doublet
    Int_t layer    = CbmMuchAddress::GetLayerSideIndex(address);  // layer in doublet
    Int_t sector   = CbmMuchAddress::GetModuleIndex(address);     // sector
    fHitPl[station3 - fStatFirst][doublet].insert(pair<Int_t, Int_t>(sector, i));
    fHitX[station3 - fStatFirst][doublet].insert(pair<Double_t, Int_t>(hit->GetX(), i));
    //cout << station3 << " " << doublet << " " << sector << endl;
    //if (fErrU < 0) fErrU = hit->GetDu();
    if (sel) ++nSelTu[station3 - fStatFirst];
  }
}
// -------------------------------------------------------------------------

// -----   Private method SelectHitId   ------------------------------------
Bool_t CbmMuchFindVectorsGem::SelectHitId(const CbmMuchPixelHit* hit)
{
  // Select hits with certain track IDs (for debugging) - FIXME (it is for straws)

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
        //CbmMuchPoint* point = (CbmMuchPoint*) fPoints->UncheckedAt(digiM->GetLink(ip).GetIndex());
        CbmLink link = digiM->GetLink(ip);
        //CbmMuchPoint* point = (CbmMuchPoint*)
        //fPoints->Get(0,digiM->GetLink(ip).GetEntry(),digiM->GetLink(ip).GetIndex());
        CbmMuchPoint* point = (CbmMuchPoint*) fPoints->Get(0, link.GetEntry(), link.GetIndex());
        id                  = point->GetTrackID();
        if (id == idSel[i]) return kTRUE;
      }
    }
  }
  return kFALSE;
}
// -------------------------------------------------------------------------

// -----   Private method GetDowns   ---------------------------------------
Int_t CbmMuchFindVectorsGem::GetDowns(Int_t ista, vector<pair<Double_t, Double_t>>& vecDowns)
{
  // Get extrapolated coordinates of vectors from the downstream station

  Int_t nvec = 0, straw = 0;
  if (ista != fgkStat - 1 && fRmax[ista + 1] < 1.0) {
    nvec  = fTrackArray->GetEntriesFast();
    straw = 1;
  }
  else if (ista != fgkStat - 1)
    nvec = fVectors[ista + 1].size();

  for (Int_t i = 0; i < nvec; ++i) {
    CbmMuchTrack* tr = (straw) ? (CbmMuchTrack*) fTrackArray->UncheckedAt(i) : fVectors[ista + 1][i];
    if (tr->GetUniqueID() != ista + 1) continue;
    FairTrackParam parFirst = *tr->GetParamFirst();
    Double_t zbeg           = parFirst.GetZ();
    Double_t dz             = fZ0[ista] + fDz[fgkPlanes - 1] - zbeg;
    // Propagate params
    Double_t x = parFirst.GetX() + dz * parFirst.GetTx();
    Double_t y = parFirst.GetY() + dz * parFirst.GetTy();
    vecDowns.push_back(pair<Double_t, Double_t>(x, y));
  }
  return vecDowns.size();
}
// -------------------------------------------------------------------------

// -----   Private method MakeVectors   ------------------------------------
void CbmMuchFindVectorsGem::MakeVectors(Int_t ista0)
{
  // Make vectors for stations - either all or using windows defined
  // by vectors from the next (downstream) station

  const Double_t window0 = 5.0;  //7.0; //10.0;
  //const Double_t window0 = 7.5; //10.0;
  Int_t nvec = 0;

  //for (Int_t ista = 0; ista < fgkStat; ++ista) {
  //for (Int_t ista = fgkStat-1; ista >= 0; --ista) {
  for (Int_t ista = ista0; ista >= ista0; --ista) {
    fVectors[ista].clear();
    CbmMuchPixelHit* hit = NULL;

    // Get vectors from the downstream station
    if (fRmax[ista] < 1.0) continue;
    //Int_t lay2 = 0, patt = 0, flag = 0, nhits = fHitPl[ista][lay2].size();
    Int_t lay2 = fgkPlanes / 2 - 1, patt = 0, flag = 0, nhits = fHitPl[ista][lay2].size();
    cout << " Hits: " << ista << " " << lay2 << " " << nhits << endl;
    vector<pair<Double_t, Double_t>> vecDowns;
    //nvec = GetDowns(ista, vecDowns);
    multimap<Double_t, Int_t>::iterator mit, mitb, mite, mit1;
    Double_t window = window0;
    //if (ista == 0) window /= 1.5;

    //if (ista == fgkStat - 1) { // no constraints for st. 4
    if (ista >= fgkStat - 2) {  // no constraints for st. 3 and 4
      // All-GEM configuration
      //if (CbmSetup::Instance()->IsActive(kTrd)) {
      if (0) {
        // Use TRD vectors for guidance
        nvec = GetTrdVectors(vecDowns);
      }
      else {
        // No TRD present
        nvec = 2;  // account for missing last layer
        vecDowns.push_back(pair<Double_t, Double_t>(0.0, 0.0));
        vecDowns.push_back(pair<Double_t, Double_t>(0.0, 0.0));
        window = 999.0;
      }
    }
    else
      nvec = GetDowns(ista, vecDowns);
    Int_t indsta0 = 1 + ista * fgkPlanes;

    for (Int_t iv = 0; iv < nvec; ++iv) {
      if (ista > 1 && iv == 1) --lay2;  // take last but one layer
      Double_t xv = vecDowns[iv].first, yv = vecDowns[iv].second;
      mitb = fHitX[ista][lay2].lower_bound(xv - window);  // lower X-window edge
      mite = fHitX[ista][lay2].upper_bound(xv + window);  // upper X-window edge
      /*
      if (mitb == mite && ista > 1) { 
	// If no hit in the last layer of the last or last but one stations - take last but one layer
	--lay2;
	mitb = fHitX[ista][lay2].lower_bound(xv-window); // lower X-window edge
	mite = fHitX[ista][lay2].upper_bound(xv+window); // upper X-window edge
      }
      */
      mit1           = mite;
      Int_t inWin    = 0;
      Double_t ywin1 = yv - window, ywin2 = yv + window;

      for (mit = mitb; mit != mite; ++mit) {
        if (mit1 != mite) {
          fHitX[ista][lay2].erase(mit1);  // remove processed hit
          mit1 = mite;
        }
        Int_t indx = mit->second;
        hit        = (CbmMuchPixelHit*) fHits->UncheckedAt(indx);
        if (hit->GetY() < ywin1 || hit->GetY() > ywin2) continue;
        //Int_t lay = hit->GetPlaneId() - 1 - ista * fgkPlanes;
        Int_t lay   = hit->GetPlaneId() - indsta0;
        fXy[lay][0] = hit->GetX();
        fXy[lay][1] = hit->GetY();
        //fXy[lay][2] = hit->GetDx();
        //fXy[lay][3] = hit->GetDy();
        fXy[lay][2]  = TMath::Max(hit->GetDx(), fErrX[ista]);
        fXy[lay][3]  = TMath::Max(hit->GetDy(), fErrY[ista]);
        fXy[lay][4]  = hit->GetZ();
        fXyi[lay][0] = CbmMuchAddress::GetModuleIndex(hit->GetAddress());  // sector No.
        fXyi[lay][1] = indx;
        //patt = lay + 1;
        patt = (1 << lay);
        //cout << lay << " " << hit->GetX() << " " << hit->GetY() << " " << hit->GetZ() << " " << endl;
        //ProcessPlane(ista, lay2+1, patt, flag);
        ProcessPlane(ista, lay2 - 1, patt, flag);
        mit1  = mit;
        inWin = 1;
      }

      if (inWin == 0 && ista == 1) {
        //if (0) {
        // No hits inside window - increase window size and repeat (for one station only)
        window = window0 * 1.5;
        mitb   = fHitX[ista][lay2].lower_bound(xv - window);  // lower X-window edge
        mite   = fHitX[ista][lay2].upper_bound(xv + window);  // upper X-window edge
        mit1   = mite;

        for (mit = mitb; mit != mite; ++mit) {
          //AZ if (mit1 != mite) fHitX[ista][lay2].erase(mit1); // remove processed hit
          if (mit1 != mite) {
            fHitX[ista][lay2].erase(mit1);
            break;
          }  // remove processed hit
          mit1       = mite;
          Int_t indx = mit->second;
          hit        = (CbmMuchPixelHit*) fHits->UncheckedAt(indx);
          if (hit->GetY() < yv - window || hit->GetY() > yv + window) continue;
          Int_t lay   = hit->GetPlaneId() - indsta0;
          fXy[lay][0] = hit->GetX();
          fXy[lay][1] = hit->GetY();
          //fXy[lay][2] = hit->GetDx();
          //fXy[lay][3] = hit->GetDy();
          fXy[lay][2]  = TMath::Max(hit->GetDx(), fErrX[ista]);
          fXy[lay][3]  = TMath::Max(hit->GetDy(), fErrY[ista]);
          fXy[lay][4]  = hit->GetZ();
          fXyi[lay][0] = CbmMuchAddress::GetModuleIndex(hit->GetAddress());  // sector No.
          fXyi[lay][1] = indx;
          //patt = lay + 1;
          patt = (1 << lay);
          //cout << lay << " " << hit->GetX() << " " << hit->GetY() << " " << hit->GetZ() << " " << endl;
          //ProcessPlane(ista, lay2+1, patt, flag);
          ProcessPlane(ista, lay2 - 1, patt, flag);
          mit1 = mit;
        }
        window = window0;
      }
    }  // for (Int_t iv = 0;
  }
}
// -------------------------------------------------------------------------

// -----   Private method GetTrdVectors   ----------------------------------
Int_t CbmMuchFindVectorsGem::GetTrdVectors(vector<pair<Double_t, Double_t>>& vecDowns)
{
  // Get extrapolated coordinates of vectors from the TRD

  Int_t nvec = fTrdVectors->GetEntriesFast();

  for (Int_t i = 0; i < nvec; ++i) {
    CbmMuchTrack* tr        = (CbmMuchTrack*) fTrdVectors->UncheckedAt(i);
    FairTrackParam parFirst = *tr->GetParamFirst();
    Double_t zbeg           = parFirst.GetZ();
    Double_t dz             = fZ0[fgkStat - 1] + fDz[fgkPlanes - 1] - zbeg;
    // Propagate params
    Double_t x = parFirst.GetX() + dz * parFirst.GetTx();
    Double_t y = parFirst.GetY() + dz * parFirst.GetTy();
    vecDowns.push_back(pair<Double_t, Double_t>(x, y));
  }
  return vecDowns.size();
}
// -------------------------------------------------------------------------

// -----   Private method ProcessPlane   -----------------------------------
void CbmMuchFindVectorsGem::ProcessPlane(Int_t ista, Int_t lay2, Int_t patt, Int_t flag)
{
  // Main processing engine (recursively adds layer hits to the vector)

  //const Double_t cut[2] = {0.8, 0.8}; // !!! empirical !!!
  //const Double_t cut[2] = {0.5, 0.5}; // !!! empirical !!!
  //AZ const Double_t cut[2][2] = {{0.5, 0.5},{0.6,0.6}}; // !!! empirical !!!
  //const Double_t cut[2][2] = {{0.5,0.6}, {0.5,0.6}}; // !!! empirical !!!
  //const Double_t cut[2][2] = {{0.45,0.55}, {0.45,0.55}}; // !!! empirical !!!
  const Double_t cut[2][2] = {{0.4, 0.5}, {0.4, 0.5}};  // !!! empirical !!!

  Double_t pars[4] = {0.0};
  Int_t nhits      = fHitPl[ista][lay2].size();
  //Int_t sec0 = (patt & (1 << lay2*2-1)) ? fXyi[lay2*2-1][0] : fXyi[lay2*2-2][0];
  Int_t sec0 = (patt & (1 << lay2 * 2 + 3)) ? fXyi[lay2 * 2 + 3][0] : fXyi[lay2 * 2 + 2][0];
  multimap<Int_t, Int_t>::iterator it;
  pair<multimap<Int_t, Int_t>::iterator, multimap<Int_t, Int_t>::iterator> ret;
  Int_t indsta0 = 1 + ista * fgkPlanes;

  for (Int_t dsec = -1; dsec < 2; ++dsec) {
    Int_t isec = sec0 + dsec;
    if (isec < 0) isec += fNsect[ista];
    else if (isec == fNsect[ista])
      isec = 0;

    ret = fHitPl[ista][lay2].equal_range(isec);
    for (it = ret.first; it != ret.second; ++it) {
      Int_t indx = it->second, sector = it->first;

      CbmMuchPixelHit* hit = (CbmMuchPixelHit*) fHits->UncheckedAt(indx);
      //Int_t lay = hit->GetPlaneId() - 1 - ista * fgkPlanes;
      Int_t lay   = hit->GetPlaneId() - indsta0;
      fXy[lay][0] = hit->GetX();
      fXy[lay][1] = hit->GetY();
      //fXy[lay][2] = hit->GetDx();
      //fXy[lay][3] = hit->GetDy();
      fXy[lay][2] = TMath::Max(hit->GetDx(), fErrX[ista]);
      fXy[lay][3] = TMath::Max(hit->GetDy(), fErrY[ista]);
      fXy[lay][4] = hit->GetZ();

      // Check slopes
      //AZ Int_t lay0 = fgkPlanes - 1;
      //AZ if (!(patt & (1 << lay0))) --lay0;;
      Int_t lay0   = TString::Itoa(patt, 2).Length() - 1;
      Double_t dx  = fXy[lay][0] - fXy[lay0][0];
      Double_t dz  = fXy[lay][4] - fXy[lay0][4];
      Double_t dTx = TMath::Abs(dx / dz - fXy[lay][0] / fXy[lay][4]);
      if (TMath::Abs(dTx) > cut[0][TMath::Min(ista, 1)]) continue;
      Double_t dy  = fXy[lay][1] - fXy[lay0][1];
      Double_t dTy = TMath::Abs(dy / dz - fXy[lay][1] / fXy[lay][4]);
      if (TMath::Abs(dTy) > cut[1][TMath::Min(ista, 1)]) continue;

      fXyi[lay][0] = sector;
      fXyi[lay][1] = indx;

      // Clear bits
      patt &= ~(1 << lay2 * 2);
      patt &= ~(1 << lay2 * 2 + 1);
      // Set bit
      patt |= (1 << lay);
      //cout << lay << " " << patt << " " << hit->GetX() << " " << hit->GetY() << " " << hit->GetZ() << " " << endl;

      //if (lay2 < fgkPlanes / 2 - 1) ProcessPlane(ista, lay2+1, patt, flag);
      if (lay2 > 0) ProcessPlane(ista, lay2 - 1, patt, flag);
      else {
        // Straight line fit of the vector
        FindLine(patt, pars);
        Double_t chi2 = FindChi2(ista, patt, pars);
        //cout << " *** Stat: " << ista << " " << id << " " << indx1 << " " << indx2 << " " << chi2 << " " << pars[0] << " " << pars[1] << endl;
        if (chi2 <= fCutChi2[ista]) AddVector(ista, patt, chi2, pars);  // add vector to the temporary container
      }
    }  // for (it = ret.first;
  }    // for (Int_t dsec = -1; dsec < 2;
}
// -------------------------------------------------------------------------

// -----   Private method AddVector   --------------------------------------
void CbmMuchFindVectorsGem::AddVector(Int_t ista, Int_t patt, Double_t chi2, Double_t* pars)
{
  // Add vector to the temporary container (as a MuchTrack)

  Bool_t refit = kFALSE;  //kTRUE;
  TMatrixDSym cov(4);

  if (refit) {
    // Refit line with individual hit errors
    //cout << " Before: " << chi2 << endl;
    Refit(patt, chi2, pars, cov);
    //cout << " After: " << chi2 << endl;
  }
  else {
    cov = *fMatr[patt];
    cov *= (fErrX[ista] * fErrX[ista]);  // the same error in X and Y
  }
  cov.ResizeTo(5, 5);
  cov(4, 4) = 1.0;

  Int_t npool = fVecPool->GetEntriesFast();
  //CbmMuchTrack *track = new ((*fVecPool)[npool]) CbmMuchTrack();
  CbmMuchTrack* track = (CbmMuchTrack*) fVecPool->ConstructedAt(npool);
  track->SetChiSq(chi2);
  FairTrackParam par(pars[0], pars[1], fZ0[ista], pars[2], pars[3], 0.0, cov);
  track->SetParamFirst(&par);
  par.SetZ(fZ0[ista] + fDz[fgkPlanes - 1]);
  par.SetX(pars[0] + fDz[fgkPlanes - 1] * pars[2]);
  par.SetY(pars[1] + fDz[fgkPlanes - 1] * pars[3]);
  track->SetParamLast(&par);
  track->SetUniqueID(ista);  // just to pass the value

  for (Int_t ipl = 0; ipl < fgkPlanes; ++ipl) {
    if (!(patt & (1 << ipl))) continue;
    track->AddHit(fXyi[ipl][1], kMUCHPIXELHIT);
  }
  Int_t ndf = (track->GetNofHits() > 2) ? track->GetNofHits() * 2 - 4 : 1;
  track->SetNDF(ndf);
  SetTrackId(track);  // set track ID as its flag
  fVectors[ista].push_back(track);
}
// -------------------------------------------------------------------------

// -----   Private method SetTrackId   -------------------------------------
void CbmMuchFindVectorsGem::SetTrackId(CbmMuchTrack* vec)
{
  // Set vector ID as its flag (maximum track ID of its poins)

  map<Int_t, Int_t> ids;
  Int_t nhits = vec->GetNofHits(), id = 0;

  for (Int_t ih = 0; ih < nhits; ++ih) {
    CbmMuchPixelHit* hit = (CbmMuchPixelHit*) fHits->UncheckedAt(vec->GetHitIndex(ih));
    CbmMuchCluster* clus = (CbmMuchCluster*) fClusters->UncheckedAt(hit->GetRefId());
    Int_t nDigis         = clus->GetNofDigis();
    for (Int_t j = 0; j < nDigis; ++j) {
      CbmMuchDigiMatch* digiM = (CbmMuchDigiMatch*) fDigiMatches->UncheckedAt(clus->GetDigi(j));
      Int_t np                = digiM->GetNofLinks();
      for (Int_t ip = 0; ip < np; ++ip) {
        //CbmMuchPoint* point = (CbmMuchPoint*) fPoints->UncheckedAt(digiM->GetLink(0).GetIndex());
        //cout << nhits << " " << ih << " " << j << " " << nDigis << " " << ip << " " << np << " " << fPoints->GetEntriesFast() << " " << fHits->GetEntriesFast() << endl;
        CbmLink link = digiM->GetLink(ip);
        //cout << nhits << " " << ih << " " << j << " " << nDigis << " " << ip << " " << np << " " << fPoints->Size(link.GetFile(),link.GetEntry()) << " " << fHits->GetEntriesFast() << endl;
        //CbmMuchPoint* point = (CbmMuchPoint*) fPoints->UncheckedAt(digiM->GetLink(ip).GetIndex());
        CbmMuchPoint* point = (CbmMuchPoint*) fPoints->Get(0, link.GetEntry(), link.GetIndex());
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
void CbmMuchFindVectorsGem::FindLine(Int_t patt, Double_t* pars)
{
  // Fit of hits to the straight line

  // Solve system of linear equations
  Bool_t ok = kFALSE, onoff;
  TVectorD b(4);
  for (Int_t i = 0; i < fgkPlanes; ++i) {
    onoff = patt & (1 << i);
    if (!onoff) continue;
    b[0] += fXy[i][0];
    b[1] += fXy[i][1];
    b[2] += fXy[i][0] * fDz[i];
    b[3] += fXy[i][1] * fDz[i];
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
Double_t CbmMuchFindVectorsGem::FindChi2(Int_t ista, Int_t patt, Double_t* pars)
{
  // Compute chi2 of the fit

  Double_t chi2 = 0, x = 0, y = 0;
  Bool_t onoff;

  for (Int_t i = 0; i < fgkPlanes; ++i) {
    onoff = patt & (1 << i);
    if (!onoff) continue;
    x           = pars[0] + pars[2] * fDz[i];
    y           = pars[1] + pars[3] * fDz[i];
    Double_t dx = (x - fXy[i][0]) / fErrX[ista];
    Double_t dy = (y - fXy[i][1]) / fErrY[ista];
    chi2 += dx * dx;
    chi2 += dy * dy;
  }
  //cout << " Chi2 = " << chi2 << endl;
  return chi2;
}
// -------------------------------------------------------------------------

// -----   Private method CheckParams   ------------------------------------
void CbmMuchFindVectorsGem::CheckParams(Int_t ista0)
{
  // Remove vectors with wrong orientation
  // using empirical cuts for omega muons at 8 Gev

  //AZ const Double_t cut[2] = {0.6, 0.6}; // !!! empirical !!!
  const Double_t cut[2] = {0.7, 0.7};  // !!! empirical !!!

  //AZ for (Int_t ista = 0; ista < fgkStat; ++ista) {
  for (Int_t ista = ista0; ista <= ista0; ++ista) {
    Int_t nvec = fVectors[ista].size();

    for (Int_t iv = 0; iv < nvec; ++iv) {
      CbmMuchTrack* vec            = fVectors[ista][iv];
      const FairTrackParam* params = vec->GetParamFirst();
      Double_t dTx                 = params->GetTx() - params->GetX() / params->GetZ();
      if (TMath::Abs(dTx) > cut[0]) vec->SetChiSq(-1.0);
      else {
        Double_t dTy = params->GetTy() - params->GetY() / params->GetZ();
        if (TMath::Abs(dTy) > cut[1]) vec->SetChiSq(-1.0);
      }
    }

    for (Int_t iv = nvec - 1; iv >= 0; --iv) {
      CbmMuchTrack* vec = fVectors[ista][iv];
      if (vec->GetChiSq() < 0) fVectors[ista].erase(fVectors[ista].begin() + iv);
    }
    cout << " Vectors after parameter check in station " << ista << ": " << nvec << " " << fVectors[ista].size()
         << endl;

    nvec = fVectors[ista].size();
    for (Int_t iv = 0; iv < nvec; ++iv) {
      CbmMuchTrack* vec    = fVectors[ista][iv];
      CbmMuchPixelHit* hit = (CbmMuchPixelHit*) fHits->UncheckedAt(vec->GetHitIndex(0));
      UInt_t address       = hit->GetAddress();
      Int_t isec           = CbmMuchAddress::GetModuleIndex(address);  // sector
      fSecVec[ista].insert(pair<Int_t, CbmMuchTrack*>(isec, vec));
    }
  }
}
// -------------------------------------------------------------------------

// -----   Private method HighRes   ----------------------------------------
/*
void CbmMuchFindVectorsGem::HighRes()
{
  // High resolution processing (resolve ghost hits and make high resolution vectors)

  for (Int_t ista = 0; ista < fgkStat; ++ista) {
    Int_t nvec = fVectors[ista].size();

    for (Int_t iv = 0; iv < nvec; ++iv) {
      CbmMuchTrack *vec = fVectors[ista][iv];
      Int_t nhits = vec->GetNofHits(), patt = 0;
      Double_t uu[fgkPlanes][2];

      for (Int_t ih = 0; ih < nhits; ++ih) {
	CbmMuchPixelHit *hit = (CbmMuchPixelHit*) fHits->UncheckedAt(vec->GetHitIndex(ih));
	Int_t lay = fGeoScheme->GetLayerIndex(hit->GetAddress());
	Int_t side = fGeoScheme->GetLayerSideIndex(hit->GetAddress());
	Int_t plane = lay*2+side;
	uu[plane][0] = hit->GetU();
	uu[plane][1] = hit->GetDouble()[2];
	patt |= (1 << plane);
	fUzi[plane][0] = hit->GetSegment();
	fUzi[plane][1] = vec->GetHitIndex(ih);
      }

      // Number of hit combinations is 2 
      // - left-right ambiguity resolution does not really work for doublets
      Int_t nCombs = (patt & 1) ? 2 : 1, flag = 1, nok = nCombs - 1;

      for (Int_t icomb = -1; icomb < nCombs; icomb += 2) {
	fUz[0][0] = (nCombs == 2) ? uu[0][0] + uu[0][1] * icomb : 0.0;
	ProcessSingleHigh(ista, 1, patt, flag, nok, uu);
      }
      
    } // for (Int_t iv = 0; 
  } // for (Int_t ista = 0; 

  MoveVectors(); // move vectors from one container to another, i.e. drop low resolution ones

}
*/
// -------------------------------------------------------------------------

// -----   Private method ProcessDoubleHigh   ------------------------------
/*
void CbmMuchFindVectorsGem::ProcessSingleHigh(Int_t ista, Int_t plane, Int_t patt, Int_t flag,
					      Int_t nok, Double_t uu[fgkPlanes][2]) 
{
  // Main processing engine for high resolution mode
  // (recursively adds singlet hits to the vector)
 
  Double_t pars[4] = {0.0};

  // Number of hit combinations is 2
  Int_t nCombs = (patt & (1 << plane)) ? 2 : 1;
  nok += (nCombs - 1);

  for (Int_t icomb = -1; icomb < nCombs; icomb += 2) {
    fUz[plane][0] = (nCombs == 2) ? uu[plane][0] + uu[plane][1] * icomb : 0.0;
    if (plane == fgkPlanes - 1 || nok == fMinHits && flag) {
      // Straight line fit of the vector
      Int_t patt1 = patt & ((1 << plane + 1) - 1); // apply mask
      FindLine(patt1, pars);
      Double_t chi2 = FindChi2(ista, patt1, pars);
      //cout << " *** Stat: " << ista << " " << plane << " " << chi2 << " " << pars[0] << " " << pars[1] << endl;
      if (icomb > -1) flag = 0;
      if (chi2 > fCutChi2[ista]) continue; // too high chi2 - do not extend line
      //if (plane + 1 < fgkPlanes) ProcessSingleHigh(ista, plane + 1, patt, flag, nok, uu);
      if (plane + 1 < fgkPlanes) ProcessSingleHigh(ista, plane + 1, patt, 0, nok, uu);
      else AddVector(ista, patt, chi2, pars, kFALSE); // add vector to the temporary container
    } else {
      ProcessSingleHigh(ista, plane + 1, patt, flag, nok, uu);
    }
  }

}
*/
// -------------------------------------------------------------------------

// -----   Private method RemoveClones   -----------------------------------
void CbmMuchFindVectorsGem::RemoveClones(Int_t ista0)
{
  // Remove clone vectors (having at least 1 the same hit)

  Int_t nthr = 2, planes[20];
  //Int_t nthr = 1, planes[20];
  pair<multimap<Int_t, CbmMuchTrack*>::iterator, multimap<Int_t, CbmMuchTrack*>::iterator> ret;
  multimap<Int_t, CbmMuchTrack*>::iterator itsec;

  //AZ for (Int_t ista = 0; ista < fgkStat; ++ista) {
  for (Int_t ista = ista0; ista <= ista0; ++ista) {
    // Process according to sector number of the first hit in vector
    Int_t nvec = fVectors[ista].size();

    for (Int_t isec = 0; isec < fNsect[ista]; ++isec) {
      ret = fSecVec[ista].equal_range(isec);
      // Do sorting according to "quality"
      multimap<Double_t, CbmMuchTrack*> qMap;
      multimap<Double_t, CbmMuchTrack*>::iterator it, it1;

      for (itsec = ret.first; itsec != ret.second; ++itsec) {
        CbmMuchTrack* vec = itsec->second;
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
          CbmMuchPixelHit* hit   = (CbmMuchPixelHit*) fHits->UncheckedAt(vec->GetHitIndex(ih));
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
            CbmMuchPixelHit* hit = (CbmMuchPixelHit*) fHits->UncheckedAt(vec1->GetHitIndex(ih));
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
            //else if (vec->GetChiSq() * 1 <= vec1->GetChiSq()) clone = 1; // the same number of hits on 2 tracks
            else if (vec->GetChiSq() * 1.5 <= vec1->GetChiSq())
              clone = 1;  // the same number of hits on 2 tracks
            if (clone) vec1->SetChiSq(-1.0);
          }
        }
      }  // for (it = qMap.begin();
    }    // for (Int_t isec = 0; isec < fNsect[ista];

    for (Int_t iv = nvec - 1; iv >= 0; --iv) {
      CbmMuchTrack* vec = fVectors[ista][iv];
      if (vec->GetChiSq() < 0) fVectors[ista].erase(fVectors[ista].begin() + iv);
    }
    cout << " Vectors after clones removed: " << nvec << " " << fVectors[ista].size() << endl;

  }  // for (Int_t ista = 0; ista < fgkStat;
}
// -------------------------------------------------------------------------

// -----   Private method StoreVectors   -----------------------------------
void CbmMuchFindVectorsGem::StoreVectors()
{
  // Store vectors (CbmMuchTracks) into TClonesArray

  Int_t ntrs     = fTrackArray->GetEntriesFast();
  Int_t nHitsTot = fHits->GetEntriesFast();

  for (Int_t ist = 0; ist < fgkStat; ++ist) {
    //set<Int_t> usedHits;
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
Int_t CbmMuchFindVectorsGem::CountBits(Int_t x)
{

  Int_t count;
  for (count = 0; x; count++)
    x &= x - 1;
  return count;
}
// -------------------------------------------------------------------------

// -----   Private method Refit   ------------------------------------------
void CbmMuchFindVectorsGem::Refit(Int_t patt, Double_t& chi2, Double_t* pars, TMatrixDSym& cov)
{
  // Refit line with individual hit errors

  Double_t dz2[fgkPlanes], errx2[fgkPlanes], erry2[fgkPlanes];
  Bool_t onoff[fgkPlanes];

  for (Int_t i = 0; i < fgkPlanes; ++i) {
    dz2[i]   = fDz[i] * fDz[i];
    onoff[i] = kTRUE;
  }

  TMatrixD coef(4, 4);

  for (Int_t j = 0; j < fgkPlanes; ++j) {
    onoff[j] = (patt & (1 << j));
    if (!onoff[j]) continue;
    errx2[j] = fXy[j][2] * fXy[j][2];
    erry2[j] = fXy[j][3] * fXy[j][3];
  }

  coef = 0.0;
  for (Int_t i = 0; i < fgkPlanes; ++i) {
    if (!onoff[i]) continue;
    coef(0, 0) += 1 / errx2[i];
    coef(0, 1) += 0;
    coef(0, 2) += fDz[i] / errx2[i];
    coef(0, 3) += 0;
  }
  for (Int_t i = 0; i < fgkPlanes; ++i) {
    if (!onoff[i]) continue;
    coef(1, 0) += 0;
    coef(1, 1) += 1 / erry2[i];
    coef(1, 2) += 0;
    coef(1, 3) += fDz[i] / erry2[i];
  }
  for (Int_t i = 0; i < fgkPlanes; ++i) {
    if (!onoff[i]) continue;
    coef(2, 0) += fDz[i] / errx2[i];
    coef(2, 1) += 0;
    coef(2, 2) += dz2[i] / errx2[i];
    coef(2, 3) += 0;
  }
  for (Int_t i = 0; i < fgkPlanes; ++i) {
    if (!onoff[i]) continue;
    coef(3, 0) += 0;
    coef(3, 1) += fDz[i] / erry2[i];
    coef(3, 2) += 0;
    coef(3, 3) += dz2[i] / erry2[i];
  }

  /*
  TDecompLU *lu = new TDecompLU(4);
  lu->SetMatrix(coef);
  lu->SetTol(0.1*lu->GetTol());
  lu->Decompose();
  */
  TDecompLU lu(4);
  lu.SetMatrix(coef);
  lu.SetTol(0.1 * lu.GetTol());
  lu.Decompose();
  //fLus.insert(pair<Int_t,TDecompLU*>(ipat,lu));

  TVectorD b(4);
  for (Int_t i = 0; i < fgkPlanes; ++i) {
    if (!onoff[i]) continue;
    b[0] += fXy[i][0] / errx2[i];
    b[1] += fXy[i][1] / erry2[i];
    b[2] += fXy[i][0] * fDz[i] / errx2[i];
    b[3] += fXy[i][1] * fDz[i] / erry2[i];
  }

  Bool_t ok = kFALSE;
  //TVectorD solve = fLus[patt]->Solve(b, ok);
  //TVectorD solve = lu->Solve(b, ok);
  TVectorD solve = lu.Solve(b, ok);
  for (Int_t i = 0; i < 4; ++i)
    pars[i] = solve[i];

  cov.SetMatrixArray(coef.GetMatrixArray());
  cov.Invert();  // covar. matrix

  // Compute Chi2
  Double_t x = 0, y = 0;
  chi2 = 0;

  for (Int_t i = 0; i < fgkPlanes; ++i) {
    if (!onoff[i]) continue;
    x           = pars[0] + pars[2] * fDz[i];
    y           = pars[1] + pars[3] * fDz[i];
    Double_t dx = (x - fXy[i][0]) / fXy[i][2];
    Double_t dy = (y - fXy[i][1]) / fXy[i][3];
    chi2 += dx * dx;
    chi2 += dy * dy;
  }
  //cout << " Chi2 = " << chi2 << endl;
}
// -------------------------------------------------------------------------

// -----   Private method MatchVectors   -----------------------------------
void CbmMuchFindVectorsGem::MatchVectors()
{
  // Match vectors from 2 stations

  const Int_t iabs            = 1;
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
      if (c2 < 0 || c2 > fCutChi2[0]) continue;
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

// ------   Public method GetAbsorbers   -----------------------------------
Int_t CbmMuchFindVectorsGem::GetAbsorbers(Double_t zabs[9][2], Double_t* x0abs)
{
  // Get information about absorbers

  Int_t nabs = 0;

  while (fX0abs[nabs] < 1.e+8) {
    zabs[nabs][0] = fZabs0[nabs][0];
    zabs[nabs][1] = fZabs0[nabs][1];
    x0abs[nabs]   = fX0abs[nabs];
    ++nabs;
  }
  return nabs;
}

// -------------------------------------------------------------------------

ClassImp(CbmMuchFindVectorsGem);
