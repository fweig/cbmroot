/** CbmMuchMergeVectors.cxx
 *@author A.Zinchenko <Alexander.Zinchenko@jinr.ru>
 *@since 2014
 **/
#include "CbmMuchMergeVectors.h"
#include "CbmKFTrack.h"
#include "CbmMuchDigiMatch.h"
#include "CbmMuchFindVectorsGem.h"
#include "CbmMuchModule.h"
#include "CbmMuchPoint.h"
#include "CbmMuchStation.h"
#include "CbmMuchTrack.h"
#include "CbmStsTrack.h"
//#include "CbmTrackMatch.h"
#include "CbmTrackMatchNew.h"

#include "FairEventHeader.h"
#include "FairField.h"
#include "FairRootManager.h"
#include <Logger.h>

#include <TClonesArray.h>
#include <TGeoManager.h>
#include <TMath.h>
#include <TMatrixD.h>
#include <TMatrixFLazy.h>
#include <TROOT.h>
#include <TVectorD.h>

#include <iostream>

using std::cout;
using std::endl;
using std::map;
using std::multimap;
using std::pair;
using std::set;

FILE* lun1 = 0x0;  //fopen("chi21.dat","w");

// -----   Default constructor   -------------------------------------------
CbmMuchMergeVectors::CbmMuchMergeVectors()
  : FairTask("MuchMergeVectors")
  , fGeoScheme(CbmMuchGeoScheme::Instance())
  , fTrackArray(NULL)
  , fNofTracks(0)
  , fHits(NULL)
  , fGemHits(NULL)
  , fPoints(NULL)
  , fDigiMatches(NULL)
  , fVecArray(NULL)
  , fTracksSts(NULL)
  , fTrStsMatch(NULL)
  , fTracksLit(NULL)
  , fStatFirst(-1)
//fCutChi2(40)//200.0)
{
  for (Int_t i = 0; i < 9; ++i)
    fCutChi2[i] = 40;
  fCutChi2[0] *= 2;
  //fCutChi2[fgkStat-2] *= 2;
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
CbmMuchMergeVectors::~CbmMuchMergeVectors() {
  fTrackArray->Delete();

  for (map<Int_t, TMatrixDSym*>::iterator it = fMatr.begin(); it != fMatr.end();
       ++it)
    delete it->second;
}
// -------------------------------------------------------------------------

// -----   Public method Init (abstract in base class)  --------------------
InitStatus CbmMuchMergeVectors::Init() {

  // Get and check FairRootManager
  FairRootManager* ioman = FairRootManager::Instance();
  if (ioman == NULL) Fatal("Init", "RootManager not instantiated!");

  // Create and register MuchTrack array
  fTrackArray = new TClonesArray("CbmMuchTrack", 100);
  ioman->Register("MuchVectorTrack", "Much", fTrackArray, kTRUE);

  CbmMuchFindHitsStraws* hitFinder =
    (CbmMuchFindHitsStraws*) FairRun::Instance()->GetTask(
      "CbmMuchFindHitsStraws");
  //if (hitFinder == NULL) Fatal("Init", "CbmMuchFindHitsStraws not run!");
  //fDiam = hitFinder->GetDiam(0);

  fGemHits    = static_cast<TClonesArray*>(ioman->GetObject("MuchPixelHit"));
  fPoints     = static_cast<TClonesArray*>(ioman->GetObject("MuchPoint"));
  fVecArray   = static_cast<TClonesArray*>(ioman->GetObject("MuchVector"));
  fTracksSts  = static_cast<TClonesArray*>(ioman->GetObject("StsTrack"));
  fTrStsMatch = static_cast<TClonesArray*>(ioman->GetObject("StsTrackMatch"));
  fTracksLit  = static_cast<TClonesArray*>(ioman->GetObject("MuchTrack"));

  // Find first straw station and get some geo constants
  Int_t nSt = fGeoScheme->GetNStations();
  for (Int_t i = 0; i < nSt; ++i) {
    CbmMuchStation* st = fGeoScheme->GetStation(i);
    CbmMuchModule* mod = fGeoScheme->GetModule(i, 0, 0, 0);
    fRmin[i]           = st->GetRmin();
    fRmax[i]           = st->GetRmax();
    if (mod->GetDetectorType() == 2) {
      if (fStatFirst < 0)
        fStatFirst = CbmMuchAddress::GetStationIndex(st->GetDetectorId());
      //cout << " First station: " << fStatFirst << endl;
      Int_t nLays = st->GetNLayers();
      fNdoubl     = nLays;
      for (Int_t lay = 0; lay < nLays; ++lay) {
        CbmMuchLayer* layer = st->GetLayer(lay);
        Double_t phi        = hitFinder->GetPhi(lay);
        for (Int_t iside = 0; iside < 2; ++iside) {
          CbmMuchLayerSide* side = layer->GetSide(iside);
          Int_t plane            = lay * 2 + iside;
          fDz[plane]             = side->GetZ();
          fCosa[plane]           = TMath::Cos(phi);
          fSina[plane]           = TMath::Sin(phi);
          if (plane == 0) fZ0[i - fStatFirst] = side->GetZ();
        }
      }
    }
  }
  for (Int_t i = fgkPlanes - 1; i >= 0; --i) {
    fDz[i] -= fDz[0];
    //cout << fDz[i] << " ";
  }
  //cout << endl;

  // Get absorbers from GEM vector finder
  FairTask* vecFinder = FairRun::Instance()->GetTask("VectorFinder");
  vecFinder->GetListOfTasks()->ls();
  CbmMuchFindVectorsGem* gemFinder =
    (CbmMuchFindVectorsGem*) vecFinder->GetListOfTasks()->FindObject(
      "MuchFindVectorsGem");
  if (gemFinder == NULL) Fatal("Init", "CbmMuchFindVectorsGem not run!");

  Int_t nAbs = gemFinder->GetAbsorbers(fZabs0, fX0abs);

  cout << " \n !!! MUCH Absorbers: " << nAbs << "\n Zbeg, Zend, X0:";
  for (Int_t j = 0; j < nAbs; ++j)
    cout << " " << std::setprecision(4) << fZabs0[j][0] << ", " << fZabs0[j][1]
         << ", " << fX0abs[j] << ";";
  cout << endl << endl;

  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----  SetParContainers -------------------------------------------------
void CbmMuchMergeVectors::SetParContainers() {}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void CbmMuchMergeVectors::Exec(Option_t* opt) {

  //gErrorIgnoreLevel = kInfo; //debug level
  //gErrorIgnoreLevel = kWarning; //debug level
  gLogger->SetLogScreenLevel("INFO");
  //gLogger->SetLogScreenLevel("WARNING");

  fTrackArray->Delete();

  // Do all processing

  // Get vectors
  GetVectors();

  // Match vectors
  MatchVectors();

  // Merge vectors
  MergeVectors();

  // Select final tracks - remove ghosts
  SelectTracks();

  // Add first station (as a filter)
  //AddStation1();
}
// -------------------------------------------------------------------------

// -----   Public method Finish   ------------------------------------------
void CbmMuchMergeVectors::Finish() {
  fTrackArray->Clear();

  for (map<Int_t, TMatrixDSym*>::iterator it = fMatr.begin(); it != fMatr.end();
       ++it)
    delete it->second;
}
// -------------------------------------------------------------------------

// -----   Private method GetVectors   -------------------------------------
void CbmMuchMergeVectors::GetVectors() {
  // Group vectors according to the station number

  std::map<Int_t, CbmMuchTrack*>::iterator it;
  for (Int_t ista = 0; ista < fgkStat; ++ista) {
    if (ista == 0) {
      // STS tracks
      for (it = fVectors[ista].begin(); it != fVectors[ista].end(); ++it)
        if (it->first >= 0) delete it->second;
    }
    fVectors[ista].clear();
  }

  Int_t nVecs = fVecArray->GetEntriesFast(), sel = 0;
  for (Int_t i = 0; i < nVecs; ++i) {
    CbmMuchTrack* vec = (CbmMuchTrack*) fVecArray->UncheckedAt(i);

    //!!! For debug - select hits with certain track IDs
    sel = 1;  //SelectHitId(hit);
    if (!sel) continue;
    //

    Int_t ista = vec->GetUniqueID() + 1;  // offset due to STS tracks
    fVectors[ista].insert(pair<Int_t, CbmMuchTrack*>(i, vec));
  }

  // Get STS tracks and extrapolate them to the first absorber face
  if (fTracksSts == NULL) return;
  Int_t nSts = fTracksSts->GetEntriesFast(), ista = 0;
  FairTrackParam param, parRear;
  Double_t pMin = 0.25;
  //Double_t pMin = 0.7; //AZ

  for (Int_t i = 0; i < nSts; ++i) {
    CbmStsTrack* tr = (CbmStsTrack*) fTracksSts->UncheckedAt(i);
    if (tr->GetNofHits() < 4) continue;  // too few hits
    Double_t ppp = 1. / TMath::Abs(tr->GetParamLast()->GetQp());
    //if (ppp < pMin + 0.1) continue; // too low momentum
    if (ppp < pMin + 0.05) continue;  // too low momentum
    CbmKFTrack kfTrack = CbmKFTrack(*tr, 0);
    if (kfTrack.Extrapolate(fZabs0[ista][0]))
      continue;  // extrapolation error to absorber front face
    kfTrack.GetTrackParam(param);
    if (kfTrack.Extrapolate(fZabs0[ista][1]))
      continue;  // extrapolation error to absorber rear face
    kfTrack.GetTrackParam(parRear);
    Double_t r2 = param.GetX() * param.GetX() + param.GetY() * param.GetY();
    if (TMath::Sqrt(r2) > fRmax[0]) continue;  // outside outer acceptance
    // Take into account track angle
    Double_t cos2th =
      1.0 + param.GetTx() * param.GetTx() + param.GetTy() * param.GetTy();
    //if (ppp / TMath::Sqrt(cos2th) < pMin + 0.1) continue; // too low momentum
    if (ppp / TMath::Sqrt(cos2th) < pMin + 0.05) continue;  // too low momentum
    CbmMuchTrack* vec = new CbmMuchTrack();
    param.SetQp(1.0 / ppp);
    vec->SetParamFirst(&param);
    vec->SetPreviousTrackId(i);  // store STS track index
    vec->SetUniqueID(9);         // station "No. 9"
    parRear.SetQp(TMath::Abs(parRear.GetQp()));
    vec->SetParamLast(&parRear);
    //CbmTrackMatch *trMatch = (CbmTrackMatch*) fTrStsMatch->UncheckedAt(i);
    //vec->SetFlag(trMatch->GetMCTrackId());
    if (fTrStsMatch) {
      CbmTrackMatchNew* trMatch =
        (CbmTrackMatchNew*) fTrStsMatch->UncheckedAt(i);
      vec->SetFlag(trMatch->GetMatchedLink().GetIndex());
      //} else vec->SetFlag(SetStsTrackId(i));
    } else if (tr->GetMatch() && tr->GetMatch()->GetNofLinks()) {
      vec->SetFlag(tr->GetMatch()->GetMatchedLink().GetIndex());
    } else
      vec->SetFlag(-1);
    fVectors[ista].insert(pair<Int_t, CbmMuchTrack*>(i + nVecs, vec));
    //cout << " Before absorber: " << i << " " << nVecs << " " << i+nVecs << endl;
  }

  // Extrapolate STS tracks through the first absorber
  TMatrixF matr = TMatrixF(5, 5);
  TMatrixF unit(TMatrixF::kUnit, matr);
  for (it = fVectors[ista].begin(); it != fVectors[ista].end(); ++it) {
    CbmMuchTrack* tr        = it->second;
    FairTrackParam parFirst = *tr->GetParamFirst();
    FairTrackParam parLast  = *tr->GetParamLast();
    parFirst                = parLast;
    TMatrixFSym cov(5);
    parFirst.CovMatrix(cov);
    /*
    Double_t zbeg = parFirst.GetZ();
    Double_t dz = fZabs0[0][1] - zbeg;
    // Propagate params
    //parFirst.SetX(parFirst.GetX() + dz * parFirst.GetTx());
    //parFirst.SetY(parFirst.GetY() + dz * parFirst.GetTy());
    //parFirst.SetZ(parFirst.GetZ() + dz);
    parFirst.SetX(parLast.GetX());
    parFirst.SetY(parLast.GetY());
    parFirst.SetZ(parLast.GetZ());
    parFirst.SetTx(parLast.GetTx());
    parFirst.SetTy(parLast.GetTy());
    TMatrixFSym cov(5);
    parFirst.CovMatrix(cov);
    cov(4,4) = 1.0;
    TMatrixF ff = unit;
    ff(2,0) = ff(3,1) = dz;
    TMatrixF cf(cov,TMatrixF::kMult,ff);
    TMatrixF fcf(ff,TMatrixF::kTransposeMult,cf);
    cov.SetMatrixArray(fcf.GetMatrixArray());
    */
    PassAbsorber(0, fZabs0[0], fX0abs[0], parFirst, cov, 1);
    parFirst.SetCovMatrix(cov);
    //tr->SetParamLast(&parFirst);
    tr->SetParamFirst(&parFirst);
    //cout << " After absorber: " << it->first << " " << parFirst.GetX() << " " << parFirst.GetY() << endl;
  }
}
// -------------------------------------------------------------------------

// -----   Private method SetStsTrackId   ----------------------------------
/*
Int_t CbmMuchMergeVectors::SetStsTrackId(Int_t indx)
{
  // Set STS track ID

  CbmStsTrack *tr = (CbmStsTrack*) fTracksSts->UncheckedAt(indx);
  Int_t nHits = tr->GetNofStsHits();

  for (Int_t i = 0; i < nHits; ++i) {
    CbmStsHit *hit = fStsHits->UncheckedAt(tr->GetStsHitIndex(i));
  }
}
*/
// -------------------------------------------------------------------------

// -----   Private method MatchVectors   -----------------------------------
void CbmMuchMergeVectors::MatchVectors() {
  // Match vectors (CbmMuchTracks) from 2 stations going upstream -
  // remove vectors without matching

  const Double_t window0 = 7.0;  //10.0; //
  TMatrixF matr          = TMatrixF(5, 5);
  TMatrixF unit(TMatrixF::kUnit, matr);
  map<Int_t, CbmMuchTrack*>::iterator it, it1;
  multimap<Double_t, pair<Int_t, CbmMuchTrack*>> xMap[2];
  multimap<Double_t, pair<Int_t, CbmMuchTrack*>>::iterator mit, mit1, mitb,
    mite;
  map<CbmMuchTrack*, Int_t> matchOK[2];
  TMatrixFSym cov(5);

  //AZ for (Int_t iabs = 2; iabs >= 0; --iabs) { // last absorber has been checked already
  for (Int_t iabs = 3; iabs >= 0;
       --iabs) {  // last absorber has been checked already
    Double_t window = window0;
    if (iabs == fgkStat - 2) window = window0 * 2.0;
    //Int_t ibeg = fTrackArray->GetEntriesFast();
    xMap[0].clear();
    xMap[1].clear();
    matchOK[0].clear();
    matchOK[1].clear();

    for (Int_t ist = 0; ist < 2; ++ist) {
      // Propagate vectors to the absorber faces
      Int_t ista = iabs - 1 + ist + 1;

      for (it = fVectors[ista].begin(); it != fVectors[ista].end(); ++it) {
        CbmMuchTrack* tr        = it->second;
        FairTrackParam parFirst = *tr->GetParamFirst();
        parFirst.CovMatrix(cov);
        cov(4, 4) = 1.0;
        if (ista > 0) {  // STS tracks has been propagated already
          Double_t zbeg = parFirst.GetZ();
          Double_t dz   = fZabs0[iabs][1] - zbeg;
          // Propagate params
          parFirst.SetX(parFirst.GetX() + dz * parFirst.GetTx());
          parFirst.SetY(parFirst.GetY() + dz * parFirst.GetTy());
          parFirst.SetZ(parFirst.GetZ() + dz);
          TMatrixF ff = unit;
          ff(2, 0) = ff(3, 1) = dz;
          //cout << " Cov: " << dz << " " << cov(0,0) << " " << cov(1,1) << endl;
          TMatrixF cf(cov, TMatrixF::kMult, ff);
          TMatrixF fcf(ff, TMatrixF::kTransposeMult, cf);
          cov.SetMatrixArray(fcf.GetMatrixArray());
          if (ist == 0 && iabs > 0)
            PassAbsorber(
              ist + iabs * 2, fZabs0[iabs], fX0abs[iabs], parFirst, cov, 0);
          //if (ist) cov.Print(); ;
        }
        cov.Invert();  // weight matrix
        parFirst.SetCovMatrix(cov);
        //tr->SetParamFirst(&parFirst);
        tr->SetParamLast(&parFirst);
        xMap[ist].insert(
          pair<Double_t, pair<Int_t, CbmMuchTrack*>>(parFirst.GetX(), *it));
        matchOK[ist][tr] = -1;
      }
    }  // for (Int_t ist = 0; ist < 2;

    Int_t ista0 = iabs - 1 + 1, ista1 = iabs + 1;

    for (mit = xMap[0].begin(); mit != xMap[0].end(); ++mit) {
      CbmMuchTrack* tr1    = mit->second.second;
      FairTrackParam parOk = *tr1->GetParamLast();
      FairTrackParam par1  = *tr1->GetParamLast();
      TMatrixFSym w1(5);
      par1.CovMatrix(w1);
      Float_t pars1[5] = {(Float_t) par1.GetX(),
                          (Float_t) par1.GetY(),
                          (Float_t) par1.GetTx(),
                          (Float_t) par1.GetTy(),
                          1.0};
      TMatrixF p1(5, 1, pars1);
      TMatrixF wp1(w1, TMatrixF::kTransposeMult, p1);
      Double_t x0 = parOk.GetX(), y0 = parOk.GetY();
      mitb = xMap[1].lower_bound(x0 - window);  // lower X-window edge
      mite = xMap[1].upper_bound(x0 + window);  // upper X-window edge

      for (mit1 = mitb; mit1 != mite; ++mit1) {
        CbmMuchTrack* tr2   = mit1->second.second;
        FairTrackParam par2 = *tr2->GetParamLast();
        if (par2.GetY() - y0 < -window) continue;
        if (par2.GetY() - y0 > window) continue;
        TMatrixFSym w2(5);
        par2.CovMatrix(w2);
        TMatrixFSym w20  = w2;
        Float_t pars2[5] = {(Float_t) par2.GetX(),
                            (Float_t) par2.GetY(),
                            (Float_t) par2.GetTx(),
                            (Float_t) par2.GetTy(),
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
        if (c2 < 0 || c2 > fCutChi2[iabs]) continue;
        matchOK[0][tr1] = 1;
        matchOK[1][tr2] = 1;
        // Merged track parameters
        /*
	parOk.SetX(pMerge(0,0));
	parOk.SetY(pMerge(1,0));
	parOk.SetZ(par2.GetZ());
	parOk.SetTx(pMerge(2,0));
	parOk.SetTy(pMerge(3,0));
	parOk.SetCovMatrix(w2);
	*/
        //AddTrack(ista0, tr1, tr2, mit->second.first, mit1->second.first, parOk, c2); // add track
      }  // for (mit1 = xMap[1].begin();
    }    // for (mit = xMap[0].begin();

    for (Int_t ist = 0; ist < 2; ++ist) {
      Int_t ista = iabs - 1 + ist + 1;

      for (it = fVectors[ista].begin(); it != fVectors[ista].end(); ++it) {
        CbmMuchTrack* tr = it->second;
        if (matchOK[ist][tr] > 0) continue;
        if (ista == 0) delete tr;
        fVectors[ista].erase(it);
      }
      cout << " MergeVectors: vectors after matching in station " << ista
           << ": " << fVectors[ista].size() << endl;
    }
  }  // for (Int_t iabs = 2; iabs >= 0;
}
// -------------------------------------------------------------------------

// -----   Private method MergeVectors   -----------------------------------
void CbmMuchMergeVectors::MergeVectors() {
  // Match vectors (CbmMuchTracks) from 2 stations

  const Double_t window0 = 5.0;  //10.0; //
  TMatrixF matr          = TMatrixF(5, 5);
  TMatrixF unit(TMatrixF::kUnit, matr);
  map<Int_t, CbmMuchTrack*>::iterator it, it1;
  multimap<Double_t, pair<Int_t, CbmMuchTrack*>> xMap[2];
  multimap<Double_t, pair<Int_t, CbmMuchTrack*>>::iterator mit, mit1;

  for (Int_t iabs = 0; iabs <= 3; ++iabs) {
    Int_t ibeg = fTrackArray->GetEntriesFast();
    xMap[0].clear();
    xMap[1].clear();

    for (Int_t ist = 0; ist < 2; ++ist) {
      // Propagate vectors to the absorber faces
      Int_t ista = iabs - 1 + ist + 1;
      //if (iabs == 1 && ist == 0) --ista; // !!! take extrapolated STS tracks - skip first station
      Int_t imerged = (fVectors[ista].begin()->first < 0)
                        ? 1
                        : 0;  // merged vectors stored with negative station No.
      for (it = fVectors[ista].begin(); it != fVectors[ista].end(); ++it) {
        if (imerged && it->first >= 0)
          break;  // for merged vectors: skip original (unmerged) vectors
        CbmMuchTrack* tr        = it->second;
        FairTrackParam parFirst = *tr->GetParamFirst();
        //if (ist == 0) parFirst = *tr->GetParamLast();
        Double_t zbeg = parFirst.GetZ();
        //Double_t dz = zAbs0[iabs][ist] - zbeg;
        Double_t dz = fZabs0[iabs][1] - zbeg;
        //Double_t dz = fZabs0[iabs][0] - zbeg;
        //Double_t dz = (zAbs0[iabs][0] + zAbs0[iabs][1]) / 2 - zbeg;
        // Propagate params
        parFirst.SetX(parFirst.GetX() + dz * parFirst.GetTx());
        parFirst.SetY(parFirst.GetY() + dz * parFirst.GetTy());
        parFirst.SetZ(parFirst.GetZ() + dz);
        TMatrixFSym cov(5);
        parFirst.CovMatrix(cov);
        cov(4, 4) = 1.0;
        //cov.Print();
        TMatrixF ff = unit;
        //ff.Print();
        //ff(0,2) = ff(1,3) = dz;
        ff(2, 0) = ff(3, 1) = dz;
        //cout << " Cov: " << dz << " " << cov(0,0) << " " << cov(1,1) << endl;
        TMatrixF cf(cov, TMatrixF::kMult, ff);
        TMatrixF fcf(ff, TMatrixF::kTransposeMult, cf);
        cov.SetMatrixArray(fcf.GetMatrixArray());
        //cout << " Cov1: " << dz << " " << cov(0,0) << " " << cov(1,1) << endl;
        // Go thru absorber
        //if (ist) { cov.Print(); cout << " -0- " << endl; }
        //AZ if (ist == 0) {
        if (ist == 0 && iabs > 0) {
          if (iabs == 1)
            PassAbsorber(ist + iabs * 2,
                         fZabs0[iabs],
                         fX0abs[iabs],
                         parFirst,
                         cov,
                         1);  // STS track
          else
            PassAbsorber(
              ist + iabs * 2, fZabs0[iabs], fX0abs[iabs], parFirst, cov, 1);
        }
        //if (ist) cov.Print(); ;
        cov.Invert();  // weight matrix
        parFirst.SetCovMatrix(cov);
        //tr->SetParamFirst(&parFirst);
        tr->SetParamLast(&parFirst);
        xMap[ist].insert(
          pair<Double_t, pair<Int_t, CbmMuchTrack*>>(parFirst.GetX(), *it));

        //Info("MergeVectors", "Absorber %i, station %i, trID %i, X = %f, Y = %f, Z = %f", iabs,ista,
        //   tr->GetFlag(),parFirst.GetX(),parFirst.GetY(),parFirst.GetZ());
        //cov.Print();
      }
    }  // for (Int_t ist = 0; ist < 2;

    Int_t ista0 = iabs - 1 + 1, ista1 = iabs + 1;
    //if (iabs == 1 && ista0 == 1) --ista0; // !!! take extrapolated STS tracks - skip first station
    map<Int_t, CbmMuchTrack*> mapMerged;
    Double_t window = window0;
    if (iabs == 3) window *= 2;

    for (mit = xMap[0].begin(); mit != xMap[0].end(); ++mit) {
      //Int_t imerged0 = (fVectors[ista0].begin()->first < 0) ? 1: 0;
      //if (iabs && !imerged0) break; //!!! no track merging occured for previous absorber
      //Int_t imerged1 = (fVectors[ista1].begin()->first < 0) ? 1: 0;
      //if (imerged0 && it->first >= 0) break; // for merged tracks: exclude original vectors
      CbmMuchTrack* tr1    = mit->second.second;
      FairTrackParam parOk = *tr1->GetParamLast();
      //if (1./parOk.GetQp() < 0.1) continue; // too low momentum
      if (1. / parOk.GetQp() < 0.05) continue;  // too low momentum
      FairTrackParam par1 = *tr1->GetParamLast();
      TMatrixFSym w1(5);
      par1.CovMatrix(w1);
      Float_t pars1[5] = {(Float_t) par1.GetX(),
                          (Float_t) par1.GetY(),
                          (Float_t) par1.GetTx(),
                          (Float_t) par1.GetTy(),
                          1.0};
      TMatrixF p1(5, 1, pars1);
      TMatrixF wp1(w1, TMatrixF::kTransposeMult, p1);
      Double_t x0 = parOk.GetX(), y0 = parOk.GetY();

      for (mit1 = xMap[1].begin(); mit1 != xMap[1].end(); ++mit1) {
        //if (imerged1 && it1->first >= 0) break; // for merged tracks: exclude original vectors
        CbmMuchTrack* tr2 = mit1->second.second;
        //if (tr2->GetUniqueID() == 2 && fNdoubl == 3 && 1./parOk.GetQp() < 0.05+0.37) break; // exclude low-mom. tracks - for merged straw stations (0.37 - energy loss in last absorber)
        //FairTrackParam par2 = *tr2->GetParamFirst();
        FairTrackParam par2 = *tr2->GetParamLast();
        if (par2.GetX() - x0 < -window) continue;
        if (par2.GetX() - x0 > window) break;
        if (par2.GetY() - y0 < -window) continue;
        if (par2.GetY() - y0 > window) continue;
        TMatrixFSym w2(5);
        par2.CovMatrix(w2);
        TMatrixFSym w20  = w2;
        Float_t pars2[5] = {(Float_t) par2.GetX(),
                            (Float_t) par2.GetY(),
                            (Float_t) par2.GetTx(),
                            (Float_t) par2.GetTy(),
                            1.0};
        TMatrixF p2(5, 1, pars2);
        TMatrixF wp2(w2, TMatrixF::kTransposeMult, p2);
        wp2 += wp1;
        w2 += w1;
        w2.Invert();
        TMatrixF p12(w2, TMatrixF::kTransposeMult, wp2);
        //p12.Print();

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
        if (c2 < 0 || c2 > fCutChi2[iabs]) continue;
        // Merged track parameters
        parOk.SetX(pMerge(0, 0));
        parOk.SetY(pMerge(1, 0));
        parOk.SetZ(par2.GetZ());
        parOk.SetTx(pMerge(2, 0));
        parOk.SetTy(pMerge(3, 0));
        parOk.SetCovMatrix(w2);
        AddTrack(ista0,
                 tr1,
                 tr2,
                 mit->second.first,
                 mit1->second.first,
                 parOk,
                 c2);  // add track
        Int_t evNo = FairRun::Instance()->GetEventHeader()->GetMCEntryNumber();
        //fprintf(lun1,"%6d %6d %6d %10.3e \n",evNo,tr1->GetFlag(),tr2->GetFlag(),c2);
        /* Debug
	TMatrixFSym covA = w1;
	covA.Invert();
	if (lun1 && tr1->GetFlag() == tr2->GetFlag() && tr1->GetFlag() < 2) fprintf(lun1,"%6d %10.3e %10.3e %10.3e %10.3e %10.3e %10.3e %10.3e %10.3e\n",evNo,par1.GetX()-par2.GetX(),par1.GetY()-par2.GetY(),par1.GetTx()-par2.GetTx(),par1.GetTy()-par2.GetTy(),TMath::Sqrt(covA(0,0)),TMath::Sqrt(covA(1,1)),TMath::Sqrt(covA(2,2)),TMath::Sqrt(covA(3,3)));
	*/
      }  // for (it1 = fVectors[ista1].begin();
    }    // for (it = fVectors[ista0].begin();

    // Remove clones for each absorber
    //RemoveClones(ibeg, iabs, mapMerged);
    RemoveClones(ibeg, -1, mapMerged);
    //if (iabs == 3) RemoveClones(ibeg, -1, mapMerged); // !!! remove clones
    //else RemoveClones(ibeg, 1, mapMerged); // !!! do not remove clones
    if (mapMerged.size() == 0) break;  // no merging with STS tracks !!!

    // Add merged tracks
    for (it = mapMerged.begin(); it != mapMerged.end(); ++it) {
      fVectors[ista1][it->first] = it->second;
    }

  }  // for (Int_t iabs = 0; iabs <= 3;
}
// -------------------------------------------------------------------------

// -----   Private method PassAbsorber   -----------------------------------
void CbmMuchMergeVectors::PassAbsorber(Int_t ist,
                                       Double_t* zabs,
                                       Double_t x0,
                                       FairTrackParam& parFirst,
                                       TMatrixFSym& cov,
                                       Int_t pFlag) {
  // Go thru absorber

  Double_t x  = parFirst.GetX();
  Double_t y  = parFirst.GetY();
  Double_t z  = parFirst.GetZ();
  Double_t tx = parFirst.GetTx();
  Double_t ty = parFirst.GetTy();
  //Double_t dz = zabs[0] - z;
  Double_t dz = (zabs[0] - zabs[1]) / 1;

  // Propagate params
  //parFirst.SetX(x + dz * tx);
  //parFirst.SetY(y + dz * ty);
  //parFirst.SetZ(z + dz);
  //return;

  Double_t aaa = 1.0 + tx * tx + ty * ty;
  dz *= TMath::Sqrt(aaa);  //
  Double_t l  = TMath::Abs(dz);
  Double_t l2 = l * l;
  Double_t l3 = l2 * l;

  TMatrixFSym covMS(5);
  //covMS(0,0) = l3 / 3 * (1.0 + tx * tx);
  //covMS(1,1) = l3 / 3 * (1.0 + ty * ty);
  //covMS(2,2) = l * (1.0 + tx * tx);
  //covMS(3,3) = l * (1.0 + ty * ty);
  covMS(0, 0) = l2 / 3 * (1.0 + tx * tx);
  covMS(1, 1) = l2 / 3 * (1.0 + ty * ty);
  covMS(2, 2) = 1 * (1.0 + tx * tx);
  covMS(3, 3) = 1 * (1.0 + ty * ty);

  //covMS(0,1) = covMS(1,0) = l3 / 3 * tx * ty;
  covMS(0, 1) = covMS(1, 0) = l2 / 3 * tx * ty;

  Int_t icor = (ist % 2 == 0) ? 1 : -1;
  //icor = -1; //

  //covMS(0,2) = covMS(2,0) = icor * l2 / 2 * (1.0 + tx * tx);
  //covMS(1,2) = covMS(2,1) = icor * l2 / 2 * tx * ty;
  covMS(0, 2) = covMS(2, 0) = icor * l / 2 * (1.0 + tx * tx);
  covMS(1, 2) = covMS(2, 1) = icor * l / 2 * tx * ty;

  //covMS(0,3) = covMS(3,0) = icor * l2 / 2 * tx * ty;
  //covMS(1,3) = covMS(3,1) = icor * l2 / 2 * (1.0 + ty * ty);
  //covMS(2,3) = covMS(3,2) = l * tx * ty;
  covMS(0, 3) = covMS(3, 0) = icor * l / 2 * tx * ty;
  covMS(1, 3) = covMS(3, 1) = icor * l / 2 * (1.0 + ty * ty);
  covMS(2, 3) = covMS(3, 2) = 1 * tx * ty;

  covMS *= aaa;

  Double_t dxx0 = l / x0, angle = 0.0;
  if (pFlag == 0)
    angle =
      0.0136 / 0.985 / 0.6
      * (1. + 0.038 * TMath::Log(dxx0));  // mu at p = 0.6 - peak at 8A GeV
  //if (pFlag == 0) angle = 0.0136 / 0.997 / 1.3 * (1. + 0.038 * TMath::Log(dxx0)); // mu at p = 1.3 GeV
  else {
    Double_t pmom = 1.0 / TMath::Abs(parFirst.GetQp());
    Double_t beta = pmom / TMath::Sqrt(pmom * pmom + 0.106 * 0.106);
    angle         = 0.0136 / beta / pmom * (1. + 0.038 * TMath::Log(dxx0));
  }
  //covMS *= (angle * angle / x0);
  covMS *= (angle * angle * dxx0);
  cov += covMS;
  if (pFlag == 0) return;  // no momentum update

  // Momentum update due to energy loss
  Double_t pLoss[6] = {0.25, 0.25, 0.25, 0.37, 0, 0};
  Double_t ppp      = 1. / parFirst.GetQp();
  ppp -= (pLoss[ist / 2] * TMath::Sqrt(aaa));  // ppp is always non-negative
  parFirst.SetQp(1. / ppp);
}
// -------------------------------------------------------------------------

// -----   Private method AddTrack   ---------------------------------------
void CbmMuchMergeVectors::AddTrack(Int_t ista0,
                                   CbmMuchTrack* tr1,
                                   CbmMuchTrack* tr2,
                                   Int_t indx1,
                                   Int_t indx2,
                                   FairTrackParam& parOk,
                                   Double_t c2) {
  // Store merged vector (CbmMuchTracks) into TClonesArray

  Int_t ntrs = fTrackArray->GetEntriesFast();

  CbmMuchTrack* track = new ((*fTrackArray)[ntrs]) CbmMuchTrack();
  //track->SetParamFirst(tr2->GetParamFirst());
  track->SetParamFirst(&parOk);
  if (tr1->GetUniqueID() == -9) {
    // Exclude STS track chi2
    track->SetChiSq(c2 + tr2->GetChiSq());
    track->SetNDF(4 + tr2->GetNDF());
  } else {
    track->SetChiSq(c2 + tr1->GetChiSq() + tr2->GetChiSq());
    track->SetNDF(4 + tr1->GetNDF() + tr2->GetNDF());
  }
  track->SetUniqueID(tr2->GetUniqueID());  // station number
  //track->SetPreviousTrackId(tr1->GetPreviousTrackId());
  track->SetPreviousTrackId(indx1);
  if (indx1 >= 0) {
    if (ista0) track->AddHit(indx1, kMUCHSTRAWHIT);  // add vector index
    //else track->AddHit(indx1, kHIT); // add STS vector index
    else
      track->AddHit(tr1->GetPreviousTrackId(), kHIT);  // add STS track index
    //if (indx2 < 0) track->SetPreviousTrackId(-indx2 - 1); // index of previous track
  } else {
    // Merged track
    Int_t nmerged = tr1->GetNofHits();
    for (Int_t j = 0; j < nmerged; ++j)
      track->AddHit(tr1->GetHitIndex(j),
                    tr1->GetHitType(j));  // add vector index
    if (indx1 < 0)
      track->SetPreviousTrackId(-indx1 - 1);  // index of previous track
  }
  if (indx2 >= 0)
    track->AddHit(indx2, kMUCHSTRAWHIT);  // add vector index
  else {
    // Merged track
    Int_t nmerged = tr2->GetNofHits();
    for (Int_t j = 0; j < nmerged; ++j)
      track->AddHit(tr2->GetHitIndex(j),
                    tr2->GetHitType(j));  // add vector index
  }
  if (tr1->GetFlag() == tr2->GetFlag())
    track->SetFlag(tr1->GetFlag());
  else
    track->SetFlag(-1);

  //Info("AddTrack", "trID1=%i, trID2=%i, chi2=%f, merged vectors %i", tr1->GetFlag(),tr2->GetFlag(),track->GetChiSq(),track->GetNofHits());
  gLogger->Info(MESSAGE_ORIGIN,
                "CbmMuchMergeVectors::AddTrack: ista=%i, trID1=%i, trID2=%i, "
                "chi2=%f, merged vectors %i",
                ista0,
                tr1->GetFlag(),
                tr2->GetFlag(),
                track->GetChiSq(),
                track->GetNofHits());
}
// -------------------------------------------------------------------------

// -----   Private method RemoveClones   -----------------------------------
void CbmMuchMergeVectors::RemoveClones(Int_t ibeg,
                                       Int_t iabs,
                                       map<Int_t, CbmMuchTrack*>& mapMerged) {
  // Remove clone tracks (having at least 1 the same vector)

  Int_t ntrs = fTrackArray->GetEntriesFast();

  if (iabs != 1) {  // Do not remove clones when merging stations 0 and 1
    multimap<Double_t, Int_t> c2merge;
    for (Int_t i1 = ibeg; i1 < ntrs; ++i1) {
      CbmMuchTrack* tr1 = (CbmMuchTrack*) fTrackArray->UncheckedAt(i1);
      /*
      Double_t qual = 0.0;
      if (tr1->GetUniqueID() == 9) {
	CbmMuchTrack *trLit = (CbmMuchTrack*) fTracksLit->UncheckedAt(tr1->GetHitIndex(0));
	qual = trLit->GetNofHits() + (499 - TMath::Min(tr1->GetChiSq(),499.0)) / 500;
      } else qual = tr1->GetNofHits() + (499 - TMath::Min(tr1->GetChiSq(),499.0)) / 500;
      */
      Double_t qual =
        tr1->GetNofHits()
        + (499 - TMath::Min(tr1->GetChiSq() / tr1->GetNDF(), 499.0)) / 500;
      c2merge.insert(pair<Double_t, Int_t>(-qual, i1));
    }

    multimap<Double_t, Int_t>::iterator it, it1;
    for (it = c2merge.begin(); it != c2merge.end(); ++it) {
      CbmMuchTrack* tr1 = (CbmMuchTrack*) fTrackArray->UncheckedAt(it->second);
      if (tr1 == NULL) continue;
      Int_t nvecs1 = tr1->GetNofHits();

      it1 = it;
      for (++it1; it1 != c2merge.end(); ++it1) {
        CbmMuchTrack* tr2 =
          (CbmMuchTrack*) fTrackArray->UncheckedAt(it1->second);
        if (tr2 == NULL) continue;
        Int_t nvecs2 = tr2->GetNofHits();
        if (tr2->GetUniqueID() != tr1->GetUniqueID()) continue;

        Bool_t over = kFALSE;
        for (Int_t iv1 = 0; iv1 < nvecs1; ++iv1) {
          for (Int_t iv2 = iv1; iv2 < nvecs2; ++iv2) {
            //for (Int_t iv1 = 0; iv1 < 2; ++iv1) {
            //for (Int_t iv2 = iv1; iv2 < 2; ++iv2) {
            if (iv2 != iv1) continue;
            if (tr1->GetHitType(iv1) != tr2->GetHitType(iv2)) continue;
            //if (tr1->GetUniqueID() == 9 && iv1 == 1) continue; // !!! share vectors in 1st station
            if (tr1->GetHitIndex(iv1) != tr2->GetHitIndex(iv2)) continue;
            // Count number of overlaps for the given vector
            if (iv2) {
              CbmMuchTrack* vec =
                (CbmMuchTrack*) fVecArray->UncheckedAt(tr2->GetHitIndex(iv2));
              Int_t clones = vec->GetPreviousTrackId();
              ++clones;
              if (clones == 0) ++clones;
              vec->SetPreviousTrackId(clones);
            }
            gLogger->Info(MESSAGE_ORIGIN,
                          "CbmMuchMergeVectors:RemoveClones: qual1 %f, qual2 "
                          "%f, trID1 %i, trID2 %i, ista %i, p1 %f, p2 %f",
                          it->first,
                          it1->first,
                          tr1->GetFlag(),
                          tr2->GetFlag(),
                          iv1,
                          1 / tr1->GetParamFirst()->GetQp(),
                          1 / tr2->GetParamFirst()->GetQp());
            fTrackArray->RemoveAt(it1->second);
            over = kTRUE;
            break;
          }
          if (over) break;
        }
      }
    }
    fTrackArray->Compress();
  }  // if (iabs != 1)

  ntrs = fTrackArray->GetEntriesFast();
  // Add track to the map (with negative index)
  for (Int_t i1 = ibeg; i1 < ntrs; ++i1) {
    CbmMuchTrack* tr1  = (CbmMuchTrack*) fTrackArray->UncheckedAt(i1);
    mapMerged[-i1 - 1] = tr1;
  }
}
// -------------------------------------------------------------------------

// -----   Private method SelectTracks   -----------------------------------
void CbmMuchMergeVectors::SelectTracks() {
  // Remove ghost tracks (having at least N the same hits (i.e. fired tubes))

  const Int_t nMax[2] = {2, 2}, nPl = 40;
  //Int_t nVecMin = (fNdoubl == 3) ? 4 : 5;
  Int_t nVecMin = 4;
  Int_t planes[nPl], ntrs = fTrackArray->GetEntriesFast();

  multimap<Double_t, Int_t> c2merge;
  for (Int_t i = 0; i < ntrs; ++i) {
    CbmMuchTrack* tr = (CbmMuchTrack*) fTrackArray->UncheckedAt(i);
    if (tr->GetNofHits() < nVecMin) continue;
    /*
    Double_t qual = 0.0;
    if (tr->GetUniqueID() == 9) {
      CbmMuchTrack *trLit = (CbmMuchTrack*) fTracksLit->UncheckedAt(tr->GetHitIndex(0));
      qual = trLit->GetNofHits() + (499 - TMath::Min(tr->GetChiSq(),499.0)) / 500;
    } else qual = tr->GetNofHits() + (499 - TMath::Min(tr->GetChiSq(),499.0)) / 500;
    */
    Double_t qual =
      tr->GetNofHits()
      + (499 - TMath::Min(tr->GetChiSq() / tr->GetNDF(), 499.0)) / 500;
    c2merge.insert(pair<Double_t, Int_t>(-qual, i));
  }
  if (c2merge.size() < 2) return;

  multimap<Double_t, Int_t>::iterator it, it1;
  for (it = c2merge.begin(); it != c2merge.end(); ++it) {
    CbmMuchTrack* tr1 = (CbmMuchTrack*) fTrackArray->UncheckedAt(it->second);
    if (tr1 == NULL) continue;
    Int_t nvecs1 = tr1->GetNofHits();
    for (Int_t j = 0; j < nPl; ++j)
      planes[j] = -1;

    for (Int_t iv = 0; iv < nvecs1; ++iv) {
      if (tr1->GetHitType(iv) == kHIT) {
        // STS track
        planes[nPl - 1] = tr1->GetHitIndex(iv);
        continue;
      }
      CbmMuchTrack* vec =
        (CbmMuchTrack*) fVecArray->UncheckedAt(tr1->GetHitIndex(iv));
      TClonesArray* hits = fHits;
      if (vec->GetUniqueID() < 2 || fStatFirst < 0) hits = fGemHits;
      Int_t nh = vec->GetNofHits();
      for (Int_t ih = 0; ih < nh; ++ih) {
        CbmHit* hit = (CbmHit*) hits->UncheckedAt(vec->GetHitIndex(ih));
        Int_t ipl   = hit->GetPlaneId() - 1;
        planes[ipl] = vec->GetHitIndex(ih);
      }
    }

    it1 = it;
    for (++it1; it1 != c2merge.end(); ++it1) {
      CbmMuchTrack* tr2 = (CbmMuchTrack*) fTrackArray->UncheckedAt(it1->second);
      if (tr2 == NULL) continue;
      //if (tr2->GetUniqueID() != tr1->GetUniqueID()) continue;
      Int_t nvecs2 = tr2->GetNofHits(), nover[2] = {0};
      Bool_t over = kFALSE;

      for (Int_t iv = 0; iv < nvecs2; ++iv) {
        if (tr2->GetHitType(iv) == kHIT) {
          // STS track
          if (planes[nPl - 1] >= 0 && planes[nPl - 1] == tr2->GetHitIndex(iv)) {
            // The same STS track
            gLogger->Info(MESSAGE_ORIGIN,
                          "Track quality: qual1 %f, qual2 %f, trID1 %i, trID2 "
                          "%i, the same STS track: %i",
                          it->first,
                          it1->first,
                          tr1->GetFlag(),
                          tr2->GetFlag(),
                          tr2->GetHitIndex(iv));
            fTrackArray->RemoveAt(it1->second);
            break;
          }
          if (tr2->GetUniqueID() != tr1->GetUniqueID()) break;
          continue;
        }
        CbmMuchTrack* vec =
          (CbmMuchTrack*) fVecArray->UncheckedAt(tr2->GetHitIndex(iv));
        Int_t nh           = vec->GetNofHits();
        TClonesArray* hits = fHits;
        if (vec->GetUniqueID() < 2 || fStatFirst < 0) hits = fGemHits;
        for (Int_t ih = 0; ih < nh; ++ih) {
          CbmHit* hit = (CbmHit*) hits->UncheckedAt(vec->GetHitIndex(ih));
          Int_t ipl   = hit->GetPlaneId() - 1;
          if (planes[ipl] < 0) continue;
          if (planes[ipl] == vec->GetHitIndex(ih)) {
            if (hits == fGemHits)
              ++nover[0];
            else
              ++nover[1];
            if (nover[0] >= nMax[0] || nover[1] >= nMax[1]) {
              //cout << ipl << " " << vec->GetHitIndex(ih) << endl;
              gLogger->Info(MESSAGE_ORIGIN,
                            "Track quality: qual1 %f, qual2 %f, trID1 %i, "
                            "trID2 %i, overlaps: %i, %i",
                            it->first,
                            it1->first,
                            tr1->GetFlag(),
                            tr2->GetFlag(),
                            nover[0],
                            nover[1]);
              fTrackArray->RemoveAt(it1->second);
              over = kTRUE;
              break;
            }
          }
        }
        if (over) break;
      }  // for (Int_t iv = 0; iv < nvecs2;

    }  // for (++it1; it1 != c2merge.end();
  }
  fTrackArray->Compress();
}
// -------------------------------------------------------------------------

// ------   Private method AddStation1   -----------------------------------
void CbmMuchMergeVectors::AddStation1() {
  // Add vector from the first station as a filter

  const Int_t nVecsMin = 4;
  map<Int_t, CbmMuchTrack*>::iterator it, it1;
  TMatrixF matr = TMatrixF(5, 5);
  TMatrixF unit(TMatrixF::kUnit, matr);

  if (fVectors[0].size() == 0) return;
  //if (fVectors[0].begin()->first >= 0) return; // no merged tracks were found
  if (fVectors[0].begin()->second->GetNofHits() != nVecsMin)
    return;  // no long tracks were found

  for (Int_t ist = 0; ist < 1; ++ist) {

    // Propagate vectors to the absorber faces
    Int_t iabs = 0, ista = 1;
    for (it = fVectors[ista].begin(); it != fVectors[ista].end(); ++it) {
      //if (imerged && it->first >= 0) break; // for merged vectors: skip original (unmerged) vectors
      CbmMuchTrack* tr        = it->second;
      FairTrackParam parFirst = *tr->GetParamFirst();
      //if (ist == 0) parFirst = *tr->GetParamLast();
      Double_t zbeg = parFirst.GetZ();
      Double_t dz   = fZabs0[iabs][1] - zbeg;
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
      cov.Invert();  // weight matrix
      parFirst.SetCovMatrix(cov);
      tr->SetParamFirst(&parFirst);
    }  // for (it = fVectors[ista].begin();
  }    // for (Int_t ist = 0; ist < 1;

  // Get STS tracks extrapolated through the first absorber
  Int_t nvecs = fVecArray->GetEntriesFast();
  for (it = fVectors[0].begin(); it != fVectors[0].end(); ++it) {
    if (it->first >= 0) break;  // no merged tracks anymore
    Int_t indSts      = (it->second)->GetHitIndex(0);  // STS track index
    CbmMuchTrack* tr1 = fVectors[0][nvecs + indSts];
    //cout << " STS index: " << indSts << " " << tr1->GetParamFirst()->GetX() << " " << tr1->GetParamFirst()->GetY() << endl;

    // Merge with vectors from first station
    FairTrackParam parOk = *tr1->GetParamFirst();
    FairTrackParam par1  = *tr1->GetParamFirst();
    TMatrixFSym w1(5);
    par1.CovMatrix(w1);
    w1.Invert();  // weight matrix
    Float_t pars1[5] = {(Float_t) par1.GetX(),
                        (Float_t) par1.GetY(),
                        (Float_t) par1.GetTx(),
                        (Float_t) par1.GetTy(),
                        1.0};
    TMatrixF p1(5, 1, pars1);
    TMatrixF wp1(w1, TMatrixF::kTransposeMult, p1);
    Double_t c2min = 999999.0;
    //cout << " STS " << endl;
    //par1.Print();
    //cout << TMath::Sqrt(par1.GetCovariance(0,0)) << " " << TMath::Sqrt(par1.GetCovariance(1,1)) << endl;

    Int_t iabs = 0, ista1 = 1;
    for (it1 = fVectors[ista1].begin(); it1 != fVectors[ista1].end(); ++it1) {
      CbmMuchTrack* tr2   = it1->second;
      FairTrackParam par2 = *tr2->GetParamFirst();
      //cout << " MUCH " << endl;
      //par2.Print();
      //cout << TMath::Sqrt(par2.GetCovariance(0,0)) << " " << TMath::Sqrt(par2.GetCovariance(1,1)) << endl;
      TMatrixFSym w2(5);
      par2.CovMatrix(w2);
      //w2.Invert(); // weight matrix
      TMatrixFSym w20  = w2;
      Float_t pars2[5] = {(Float_t) par2.GetX(),
                          (Float_t) par2.GetY(),
                          (Float_t) par2.GetTx(),
                          (Float_t) par2.GetTy(),
                          1.0};
      TMatrixF p2(5, 1, pars2);
      TMatrixF wp2(w2, TMatrixF::kTransposeMult, p2);
      wp2 += wp1;
      w2 += w1;
      w2.Invert();
      TMatrixF p12(w2, TMatrixF::kTransposeMult, wp2);
      //p12.Print();

      // Compute Chi2
      TMatrixF p122 = p12;
      p12 -= p1;
      TMatrixF wDp1(w1, TMatrixF::kMult, p12);
      TMatrixF chi21(p12, TMatrixF::kTransposeMult, wDp1);
      p122 -= p2;
      TMatrixF wDp2(w20, TMatrixF::kMult, p122);
      TMatrixF chi22(p122, TMatrixF::kTransposeMult, wDp2);
      Double_t c2 = chi21(0, 0) + chi22(0, 0);
      //cout << " Chi2: " << chi21(0,0) << " " << chi22(0,0) << " " << c2 << endl;
      if (c2 < 0 || c2 > fCutChi2[iabs]) continue;
      // Add track
      if (c2 < c2min) c2min = c2;
      //AddTrack(0, tr1, tr2, it->first, it1->first, parOk, c2); // add track
    }  // for (it1 = fVectors[ista1].begin();
    if (c2min / 4 > 5) {
      gLogger->Info(
        MESSAGE_ORIGIN, "Stat.1: removed track: c2min %f", c2min / 4);
      fTrackArray->RemoveAt(-it->first - 1);
    } else
      cout << " Chi2: " << c2min / 4 << endl;
  }  // for (it = fVectors[0].begin();
  fTrackArray->Compress();
}
// -------------------------------------------------------------------------

// -----   Private method AddTrack1   --------------------------------------
void CbmMuchMergeVectors::AddTrack1(Int_t ista0,
                                    CbmMuchTrack* tr1,
                                    CbmMuchTrack* tr2,
                                    Int_t indx1,
                                    Int_t indx2,
                                    FairTrackParam& parOk,
                                    Double_t c2) {
  // Store merged vector (CbmMuchTracks) into TClonesArray

  Int_t ntrs = fTrackArray->GetEntriesFast();

  CbmMuchTrack* track = new ((*fTrackArray)[ntrs]) CbmMuchTrack();
  track->SetParamFirst(&parOk);
  track->SetChiSq(c2 + tr1->GetChiSq() + tr2->GetChiSq());
  track->SetNDF(4 + tr1->GetNDF() + tr2->GetNDF());
  track->SetUniqueID(tr1->GetUniqueID());  // station number
  //track->SetPreviousTrackId(tr1->GetPreviousTrackId());
  track->SetPreviousTrackId(indx2);
  // Add vectors
  Int_t nmerged = tr1->GetNofHits();
  for (Int_t j = 0; j < nmerged; ++j) {
    if (j == 0)
      track->AddHit(tr1->GetHitIndex(j), tr1->GetHitType(j));  // STS
    else if (j == 1)
      track->AddHit(indx2, kMUCHSTRAWHIT);  // add stat.1 vector index
    else
      track->AddHit(tr1->GetHitIndex(j - 1), tr1->GetHitType(j - 1));
  }
  if (tr1->GetFlag() == tr2->GetFlag())
    track->SetFlag(tr1->GetFlag());
  else
    track->SetFlag(-1);

  Info("AddTrack",
       "trID1=%i, trID2=%i, chi2=%f, merged vectors %i",
       tr1->GetFlag(),
       tr2->GetFlag(),
       track->GetChiSq(),
       track->GetNofHits());
}
// -------------------------------------------------------------------------

ClassImp(CbmMuchMergeVectors);
