/** @file CbmTrdFindVectors.cxx
  * @copyright Copyright (C) 2017-2019 Veksler and Baldin Laboratory of High Energy Physics, Joint Institute for Nuclear Research, Dubna
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Alexander Zinchenko [committer] **/

/** CbmTrdFindVectors.cxx
 *@author A.Zinchenko <Alexander.Zinchenko@jinr.ru>
 *@since 2017
 **/
#include "CbmTrdFindVectors.h"

#include "CbmDefs.h"
#include "CbmSetup.h"
//#include "CbmTrdModule.h"
//#include "CbmTrdDigiPar.h"
#include "CbmMCDataArray.h"
#include "CbmMCDataManager.h"
#include "CbmMatch.h"
#include "CbmMuchTrack.h"
#include "CbmTrdAddress.h"
#include "CbmTrdCluster.h"
#include "CbmTrdHit.h"
#include "CbmTrdPoint.h"

//#include "FairRunAna.h"
//#include "FairRuntimeDb.h"
#include "FairRootManager.h"

#include <TClonesArray.h>
#include <TGeoManager.h>
#include <TMath.h>

#include <iostream>

using std::cout;
using std::endl;
using std::map;
using std::multimap;
using std::pair;

// -----   Default constructor   -------------------------------------------
CbmTrdFindVectors::CbmTrdFindVectors()
  : FairTask("TrdFindVectors")
  , fTrackArray(NULL)
  , fNofTracks(0)
  , fHits(NULL)
  , fClusters(NULL)
  , fPoints(NULL)
  , fDigiMatches(NULL)
  , fZ0(99999)
  , fErrX(1.0)
  , fErrY(1.0)
  , fCutChi2(24.0)  // chi2/ndf = 6 for 4 hits
{
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
CbmTrdFindVectors::~CbmTrdFindVectors() {}
// -------------------------------------------------------------------------

// -----   Public method Init (abstract in base class)  --------------------
InitStatus CbmTrdFindVectors::Init()
{

  // Get and check FairRootManager
  FairRootManager* ioman = FairRootManager::Instance();
  if (ioman == NULL) Fatal("Init", "RootManager not instantiated!");

  // Create and register TrdTrack array (if necessary)
  //fTrackArray = static_cast<TClonesArray*> (ioman->GetObject("MuchVector"));
  //if (fTrackArray == NULL) {
  fTrackArray = new TClonesArray("CbmMuchTrack", 100);
  ioman->Register("TrdVector", "Trd", fTrackArray, kTRUE);
  //}
  fHits     = static_cast<TClonesArray*>(ioman->GetObject("TrdHit"));
  fClusters = static_cast<TClonesArray*>(ioman->GetObject("TrdCluster"));
  //fPoints = static_cast<TClonesArray*> (ioman->GetObject("TrdPoint"));
  CbmMCDataManager* mcManager = (CbmMCDataManager*) ioman->GetObject("MCDataManager");
  if (NULL == mcManager) LOG(fatal) << GetName() << ": No CbmMCDataManager!";
  fPoints      = mcManager->InitBranch("TrdPoint");
  fDigiMatches = static_cast<TClonesArray*>(ioman->GetObject("TrdDigiMatch"));

  TString tag;
  CbmSetup::Instance()->GetGeoTag(kTrd, tag);
  cout << " ******* digiPar ******** " << tag << endl;
  tag.Prepend("trd_");

  //CbmTrdGeoHandler geoHandler;
  //geoHandler.Init();

  // Go to TRD
  TGeoVolume* trdV = gGeoManager->GetVolume(tag);
  TGeoNode* trd    = NULL;
  gGeoManager->CdTop();
  TGeoNode* cave = gGeoManager->GetCurrentNode();
  for (Int_t iNode = 0; iNode < cave->GetNdaughters(); iNode++) {
    TGeoNode* node = cave->GetDaughter(iNode);
    TString name   = node->GetName();
    if (name.Contains("trd")) {
      trd = node;
      gGeoManager->CdDown(iNode);
      break;
    }
  }
  if (!trd) {
    cout << "-E- CbmTrdFindVectors::Init: Cannot find top TRD node" << endl;
    return kFATAL;
  }

  Int_t nlays = trdV->GetNdaughters();

  // Loop over layers
  for (Int_t i = 0; i < nlays; ++i) {
    TGeoVolume* lay = trdV->GetNode(i)->GetVolume();
    if (!(TString(lay->GetName()).Contains("layer"))) continue;
    gGeoManager->CdDown(i);
    Int_t nmod = lay->GetNdaughters();

    // Loop over modules
    for (Int_t j = 0; j < nmod; ++j) {
      TGeoVolume* mod = lay->GetNode(j)->GetVolume();
      if (!(TString(mod->GetName()).Contains("module"))) continue;
      gGeoManager->CdDown(j);
      Int_t nparts = mod->GetNdaughters();

      // Loop over module parts
      for (Int_t k = 0; k < nparts; ++k) {
        TGeoNode* part = mod->GetNode(k);
        if (!(TString(part->GetName()).Contains("gas"))) continue;
        gGeoManager->CdDown(k);
        Double_t posLoc[3] = {0}, posGlob[3] = {0};
        gGeoManager->LocalToMaster(posLoc, posGlob);
        cout << " gas " << gGeoManager->GetPath() << " " << std::setprecision(4) << posGlob[2] << endl;
        if (i == 0) fZ0 = posGlob[2];
        fDz[i] = posGlob[2] - fZ0;
        gGeoManager->CdUp();
        break;
      }
      gGeoManager->CdUp();
      break;
    }  // for (Int_t j = 0; j < nmod;

    gGeoManager->CdUp();
  }  // for (Int_t i = 0; i < nlays;

  //exit(0);
  ComputeMatrix();  // compute system matrices

  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----  SetParContainers -------------------------------------------------
void CbmTrdFindVectors::SetParContainers()
{
  /*
  fDigiPar = (CbmTrdDigiPar*) FairRunAna::Instance()->GetRuntimeDb()->getContainer("CbmTrdDigiPar");
  cout << " ******* digiPar ******** " << fDigiPar << endl;
  exit(0);
  */
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void CbmTrdFindVectors::Exec(Option_t* opt)
{

  fTrackArray->Delete();

  Int_t nVecs = fVectors.size();
  for (Int_t j = 0; j < nVecs; ++j)
    delete fVectors[j];
  fVectors.clear();

  // Do all processing

  // Get hits
  GetHits();

  // Build vectors
  MakeVectors();

  // Remove vectors with wrong orientation
  // (using empirical cuts for omega muons at 8 GeV)
  CheckParams();

  // Match vectors from 2 stations
  //MatchVectors();

  // Remove clones
  RemoveClones();

  // Store vectors
  StoreVectors();
}
// -------------------------------------------------------------------------

// -----   Public method Finish   ------------------------------------------
void CbmTrdFindVectors::Finish() {}
// -------------------------------------------------------------------------

// -----   Private method ComputeMatrix   ----------------------------------
void CbmTrdFindVectors::ComputeMatrix()
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
    //for (Int_t j = 0; j < fgkPlanes; j += 2) if (ipat & (3 << j)) ++nDouble; else break;
    for (Int_t j = 0; j < fgkPlanes; j += 1)
      if (ipat & (1 << j)) ++nDouble;
    if (nDouble < 3) continue;
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
    //fLus.insert(pair<Int_t,TDecompLU*>(ipat,lu));
    fLus[ipat] = lu;
    TMatrixDSym cov(4);
    cov.SetMatrixArray(coef.GetMatrixArray());
    cov.Invert();  // covar. matrix
    //fMatr.insert(pair<Int_t,TMatrixDSym*>(ipat,new TMatrixDSym(cov)));
    fMatr[ipat] = new TMatrixDSym(cov);
    TString buf = "";
    for (Int_t jp = 0; jp < fgkPlanes; ++jp)
      buf += Bool_t(ipat & (1 << jp));
    Info("ComputeMatrix", " Determinant: %s %i %f", buf.Data(), ipat, coef.Determinant());
    //if (ipat == 63) { coef.Print(); Info("ComputeMatrix"," Number of configurations: %i", nTot); }
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
void CbmTrdFindVectors::GetHits()
{
  // Group hits according to their plane number

  for (Int_t i = 0; i < fgkPlanes; ++i) {
    fHitPl[i].clear();
    fHitX[i].clear();
  }

  Int_t nHits = fHits->GetEntriesFast(), sel = 0;

  for (Int_t i = 0; i < nHits; ++i) {
    CbmTrdHit* hit = (CbmTrdHit*) fHits->UncheckedAt(i);

    //!!! For debug - select hits with certain track IDs
    sel = 1;  //SelectHitId(hit);
    if (!sel) continue;
    //
    /*
    Int_t address = hit->GetAddress();
    Int_t layer = CbmTrdAddress::GetLayerId(address); // layer 
    Int_t colId = CbmTrdAddress::GetColumnId(address);
    Int_t rowId = CbmTrdAddress::GetRowId(address);
    */
    Int_t layer = hit->GetPlaneId(), colId = 0;
    fHitPl[layer].insert(pair<Int_t, Int_t>(colId, i));
    fHitX[layer].insert(pair<Double_t, Int_t>(hit->GetX(), i));
  }
}
// -------------------------------------------------------------------------

// -----   Private method MakeVectors   ------------------------------------
void CbmTrdFindVectors::MakeVectors()
{
  // Make vectors

  Int_t nvec = fVectors.size();
  for (Int_t j = 0; j < nvec; ++j)
    delete fVectors[j];
  fVectors.clear();
  CbmTrdHit* hit = NULL;

  cout << " TRD hits: " << fHits->GetEntriesFast() << endl;
  for (Int_t lay = 0; lay < 2; ++lay) {
    Int_t patt = 0, flag = 1, nhits = fHitPl[lay].size();
    cout << " Hits: " << lay << " " << nhits << endl;
    multimap<Int_t, Int_t>::iterator mit;

    for (mit = fHitPl[lay].begin(); mit != fHitPl[lay].end(); ++mit) {
      Int_t indx  = mit->second;
      hit         = (CbmTrdHit*) fHits->UncheckedAt(indx);
      fXy[lay][0] = hit->GetX();
      fXy[lay][1] = hit->GetY();
      fXy[lay][2] = hit->GetDx();
      fXy[lay][3] = hit->GetDy();
      //fXy[lay][2] = TMath::Max (hit->GetDx(),fErrX[ista]);
      //fXy[lay][3] = TMath::Max (hit->GetDy(),fErrY[ista]);
      fXy[lay][4] = hit->GetZ();
      //fXyi[lay][0] = CbmMuchAddress::GetModuleIndex(hit->GetAddress()); // sector No.
      fXyi[lay][1] = indx;
      patt         = (1 << lay);
      //cout << lay << " " << hit->GetX() << " " << hit->GetY() << " " << hit->GetZ() << " " << endl;
      ProcessPlane(lay + 1, patt, flag);
    }
  }
}
// -------------------------------------------------------------------------

// -----   Private method CheckParams   ------------------------------------
void CbmTrdFindVectors::CheckParams()
{
  // Remove vectors with wrong orientation
  // using empirical cuts for omega muons at 8 Gev

  const Double_t cut[2] = {0.6, 0.5};  // !!! empirical !!!

  Int_t nvec = fVectors.size();

  for (Int_t iv = 0; iv < nvec; ++iv) {
    CbmMuchTrack* vec            = fVectors[iv];
    const FairTrackParam* params = vec->GetParamFirst();
    Double_t dTx                 = params->GetTx() - params->GetX() / params->GetZ();
    if (TMath::Abs(dTx) > cut[0]) vec->SetChiSq(-1.0);
    else {
      Double_t dTy = params->GetTy() - params->GetY() / params->GetZ();
      if (TMath::Abs(dTy) > cut[1]) vec->SetChiSq(-1.0);
    }
  }

  for (Int_t iv = nvec - 1; iv >= 0; --iv) {
    CbmMuchTrack* vec = fVectors[iv];
    if (vec->GetChiSq() < 0) {
      delete fVectors[iv];
      fVectors.erase(fVectors.begin() + iv);
    }
  }
  cout << " Vectors after parameter check: " << nvec << " " << fVectors.size() << endl;
}
// -------------------------------------------------------------------------

// -----   Private method RemoveClones   -----------------------------------
void CbmTrdFindVectors::RemoveClones()
{
  // Remove clone vectors (having at least 3 the same hits)

  //Int_t nthr = 3, planes[20];
  Int_t nthr = 2, planes[20];

  Int_t nvec = fVectors.size();

  // Do sorting according to "quality"
  multimap<Double_t, CbmMuchTrack*> qMap;
  multimap<Double_t, CbmMuchTrack*>::iterator it, it1;

  for (Int_t i = 0; i < nvec; ++i) {
    CbmMuchTrack* vec = fVectors[i];
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
      CbmTrdHit* hit = (CbmTrdHit*) fHits->UncheckedAt(vec->GetHitIndex(ih));
      Int_t lay      = hit->GetPlaneId();
      planes[lay]    = vec->GetHitIndex(ih);
    }

    it1 = it;
    for (++it1; it1 != qMap.end(); ++it1) {
      CbmMuchTrack* vec1 = it1->second;
      if (vec1->GetChiSq() < 0) continue;
      Int_t nsame = 0, same[fgkPlanes] = {0};

      Int_t nhits1 = vec1->GetNofHits();
      //nthr = TMath::Min(nhits,nhits1) / 2;
      //nthr = TMath::Min(nhits,nhits1) * 0.75;
      for (Int_t ih = 0; ih < nhits1; ++ih) {
        CbmTrdHit* hit = (CbmTrdHit*) fHits->UncheckedAt(vec1->GetHitIndex(ih));
        Int_t lay      = hit->GetPlaneId();
        if (planes[lay] >= 0) {
          if (vec1->GetHitIndex(ih) == planes[lay]) same[lay] = 1;
          //else same[lay] = 0;
        }
      }
      for (Int_t lay = 0; lay < fgkPlanes; ++lay)
        nsame += same[lay];

      if (nsame >= nthr) {
        // Too many the same hits
        Int_t clone = 0;
        if (nhits > nhits1 + 0) clone = 1;
        else if (vec->GetChiSq() * 1 <= vec1->GetChiSq())
          clone = 1;  // the same number of hits on 2 tracks
        //else if (vec->GetChiSq() * 1.5 <= vec1->GetChiSq()) clone = 1; // the same number of hits on 2 tracks
        if (clone) vec1->SetChiSq(-1.0);
      }
    }
  }  // for (it = qMap.begin();

  for (Int_t iv = nvec - 1; iv >= 0; --iv) {
    CbmMuchTrack* vec = fVectors[iv];
    if (vec->GetChiSq() < 0) {
      delete fVectors[iv];
      fVectors.erase(fVectors.begin() + iv);
    }
  }
  cout << " Vectors after clones removed: " << nvec << " " << fVectors.size() << endl;
}
// -------------------------------------------------------------------------

// -----   Private method StoreVectors   -----------------------------------
void CbmTrdFindVectors::StoreVectors()
{
  // Store vectors (CbmMuchTracks) into TClonesArray

  Int_t ntrs     = fTrackArray->GetEntriesFast();
  Int_t nHitsTot = fHits->GetEntriesFast();

  //set<Int_t> usedHits;
  Int_t nvec = fVectors.size();

  for (Int_t iv = 0; iv < nvec; ++iv) {
    CbmMuchTrack* tr = new ((*fTrackArray)[ntrs++]) CbmMuchTrack(*(fVectors[iv]));
    //cout << " Track: " << tr->GetNofHits() << endl;
    //for (Int_t j = 0; j < tr->GetNofHits(); ++j) cout << j << " " << tr->GetHitIndex(j) << " " << fVectors[ist][iv]->GetHitIndex(j) << endl;
    // Set hit flag (to check Lit tracking)
    Int_t nhits = tr->GetNofHits();
  }
}
// -------------------------------------------------------------------------

// -----   Private method ProcessPlane   -----------------------------------
void CbmTrdFindVectors::ProcessPlane(Int_t lay, Int_t patt, Int_t flag0)
{
  // Main processing engine (recursively adds layer hits to the vector)

  const Double_t cut[2]  = {0.6, 0.5};    // !!! empirical !!! - cut on Tx(y) - x(y)/z
  const Double_t cut1[2] = {0.25, 0.15};  // !!! empirical !!! - cut on Tx(y) - Tx(y) (between segments)

  Double_t pars[4] = {0.0};
  Int_t flag       = 0;
  multimap<Int_t, Int_t>::iterator mit;
  // Clear bits
  patt &= ~(1 << lay);

  for (mit = fHitPl[lay].begin(); mit != fHitPl[lay].end(); ++mit) {
    Int_t indx = mit->second;

    CbmTrdHit* hit = (CbmTrdHit*) fHits->UncheckedAt(indx);
    fXy[lay][0]    = hit->GetX();
    fXy[lay][1]    = hit->GetY();
    fXy[lay][2]    = hit->GetDx();
    fXy[lay][3]    = hit->GetDy();
    //fXy[lay][2] = TMath::Max (hit->GetDx(),fErrX[ista]);
    //fXy[lay][3] = TMath::Max (hit->GetDy(),fErrY[ista]);
    fXy[lay][4] = hit->GetZ();
    // Clear bits
    patt &= ~(1 << lay);

    // Check slopes
    Int_t lay0 = 0;
    if (!(patt & (1 << lay0))) ++lay0;
    ;
    if (TString::Itoa(patt, 2).CountChar('1') < 2) {
      // First segment
      Double_t dx = fXy[lay][0] - fXy[lay0][0];
      Double_t dz = fXy[lay][4] - fXy[lay0][4];
      //Double_t dTx = TMath::Abs(dx/dz) - TMath::Abs(fXy[lay][0]/fXy[lay][4]);
      Double_t dTx = dx / dz - fXy[lay][0] / fXy[lay][4];
      if (TMath::Abs(dTx) > cut[0]) continue;
      Double_t dy = fXy[lay][1] - fXy[lay0][1];
      //Double_t dTy = TMath::Abs(dy/dz) - TMath::Abs(fXy[lay][1]/fXy[lay][4]);
      Double_t dTy = dy / dz - fXy[lay][1] / fXy[lay][4];
      if (TMath::Abs(dTy) > cut[1]) continue;
    }
    else {
      Double_t dx = fXy[lay][0] - fXy[lay0][0];
      Double_t dz = fXy[lay][4] - fXy[lay0][4];
      Int_t lay1  = lay - 1;
      if (!(patt & (1 << lay1))) --lay1;
      ;
      Double_t dx1 = fXy[lay][0] - fXy[lay1][0];
      Double_t dz1 = fXy[lay][4] - fXy[lay1][4];
      Double_t dTx = dx / dz - dx1 / dz1;
      if (TMath::Abs(dTx) > cut1[0]) continue;
      Double_t dy  = fXy[lay][1] - fXy[lay0][1];
      Double_t dy1 = fXy[lay][1] - fXy[lay1][1];
      Double_t dTy = dy / dz - dy1 / dz1;
      if (TMath::Abs(dTy) > cut1[1]) continue;
    }

    fXyi[lay][1] = indx;
    flag         = 1;

    // Set bit
    patt |= (1 << lay);
    //cout << lay << " " << patt << " " << hit->GetX() << " " << hit->GetY() << " " << hit->GetZ() << " " << endl;

    if (lay < fgkPlanes - 1) ProcessPlane(lay + 1, patt, flag);
    else if (TString::Itoa(patt, 2).CountChar('1') > 2) {
      // Straight line fit of the vector with > 2 hits
      FindLine(patt, pars);
      Double_t chi2 = FindChi2(patt, pars);
      cout << " *** Stat: " << chi2 << " " << pars[0] << " " << pars[1] << endl;
      if (chi2 <= fCutChi2) AddVector(patt, chi2, pars);  // add vector to the temporary container
    }
  }  // for (mit = fHitPl[lay].begin();

  //if (flag == 0) {
  if (flag >= 0) {
    // Missing plane
    if (flag0 == 0) return;  // 2 consecutive missing planes
    // Clear bits
    patt &= ~(1 << lay);
    if (lay < fgkPlanes - 1) ProcessPlane(lay + 1, patt, flag);
    else if (TString::Itoa(patt, 2).CountChar('1') > 2) {
      // Straight line fit of the vector with > 2 hits
      FindLine(patt, pars);
      Double_t chi2 = FindChi2(patt, pars);
      cout << " *** Stat: " << chi2 << " " << pars[0] << " " << pars[1] << endl;
      if (chi2 <= fCutChi2) AddVector(patt, chi2, pars);  // add vector to the temporary container
    }
  }
}
// -------------------------------------------------------------------------

// -----   Private method FindLine   ---------------------------------------
void CbmTrdFindVectors::FindLine(Int_t patt, Double_t* pars)
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
Double_t CbmTrdFindVectors::FindChi2(Int_t patt, Double_t* pars)
{
  // Compute chi2 of the fit

  Double_t chi2 = 0, x = 0, y = 0;
  Bool_t onoff;

  for (Int_t i = 0; i < fgkPlanes; ++i) {
    onoff = patt & (1 << i);
    if (!onoff) continue;
    x = pars[0] + pars[2] * fDz[i];
    y = pars[1] + pars[3] * fDz[i];
    //Double_t dx = (x - fXy[i][0]) / fErrX;
    //Double_t dy = (y - fXy[i][1]) / fErrY;
    Double_t dx = (x - fXy[i][0]) / TMath::Max(fXy[i][2], fErrX);
    Double_t dy = (y - fXy[i][1]) / TMath::Max(fXy[i][3], fErrY);
    chi2 += dx * dx;
    chi2 += dy * dy;
  }
  //cout << " Chi2 = " << chi2 << endl;
  return chi2;
}
// -------------------------------------------------------------------------

// -----   Private method AddVector   --------------------------------------
void CbmTrdFindVectors::AddVector(Int_t patt, Double_t chi2, Double_t* pars)
{
  // Add vector to the temporary container (as a MuchTrack)

  Bool_t refit = kFALSE;  //kTRUE;
  TMatrixDSym cov(4);

  if (refit) {
    // Refit line with individual hit errors
    //cout << " Before: " << chi2 << endl;
    //Refit(patt, chi2, pars, cov);
    //cout << " After: " << chi2 << endl;
  }
  else {
    cov = *fMatr[patt];
    cov *= (fErrX * fErrX);  // the same error in X and Y
  }
  cov.ResizeTo(5, 5);
  cov(4, 4) = 1.0;

  CbmMuchTrack* track = new CbmMuchTrack();
  track->SetChiSq(chi2);
  FairTrackParam par(pars[0], pars[1], fZ0, pars[2], pars[3], 0.0, cov);
  track->SetParamFirst(&par);
  par.SetZ(fZ0 + fDz[fgkPlanes - 1]);
  par.SetX(pars[0] + fDz[fgkPlanes - 1] * pars[2]);
  par.SetY(pars[1] + fDz[fgkPlanes - 1] * pars[3]);
  track->SetParamLast(&par);
  //track->SetUniqueID(ista); // just to pass the value

  for (Int_t ipl = 0; ipl < fgkPlanes; ++ipl) {
    if (!(patt & (1 << ipl))) continue;
    track->AddHit(fXyi[ipl][1], kTRDHIT);
  }
  Int_t ndf = (track->GetNofHits() > 2) ? track->GetNofHits() * 2 - 4 : 1;
  track->SetNDF(ndf);
  SetTrackId(track);  // set track ID as its flag
  fVectors.push_back(track);
}
// -------------------------------------------------------------------------

// -----   Private method SetTrackId   -------------------------------------
void CbmTrdFindVectors::SetTrackId(CbmMuchTrack* vec)
{
  // Set vector ID as its flag (maximum track ID of its poins)

  map<Int_t, Int_t> ids;
  Int_t nhits = vec->GetNofHits(), id = 0;

  //*
  for (Int_t ih = 0; ih < nhits; ++ih) {
    CbmTrdHit* hit      = (CbmTrdHit*) fHits->UncheckedAt(vec->GetHitIndex(ih));
    CbmTrdCluster* clus = (CbmTrdCluster*) fClusters->UncheckedAt(hit->GetRefId());
    Int_t nDigis        = clus->GetNofDigis();

    for (Int_t j = 0; j < nDigis; ++j) {
      CbmMatch* digiM = (CbmMatch*) fDigiMatches->UncheckedAt(clus->GetDigi(j));
      Int_t np        = digiM->GetNofLinks();

      for (Int_t ip = 0; ip < np; ++ip) {
        CbmLink link = digiM->GetLink(ip);
        //CbmTrdPoint* point = (CbmTrdPoint*) fPoints->UncheckedAt(digiM->GetLink(ip).GetIndex());
        CbmTrdPoint* point = (CbmTrdPoint*) fPoints->Get(link.GetFile(), link.GetEntry(), link.GetIndex());
        id                 = point->GetTrackID();
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
  //*/
}
// -------------------------------------------------------------------------

ClassImp(CbmTrdFindVectors);
