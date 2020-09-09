/** CbmMuchToTrdVectors.cxx
 *@author A.Zinchenko <Alexander.Zinchenko@jinr.ru>
 *@since 2018
 **/
#include "CbmMuchToTrdVectors.h"
//#include "CbmDefs.h"
//#include "CbmSetup.h"
#include "CbmMuchTrack.h"

#include <Logger.h>
#include "FairRootManager.h"
#include "FairTrackParam.h"

#include <TClonesArray.h>
//#include <TGeoManager.h>
#include <TMath.h>
#include <TMatrixF.h>
#include <TMatrixFSym.h>

#include <iostream>

using std::cout;
using std::endl;
using std::multimap;
using std::pair;

// -----   Default constructor   -------------------------------------------
CbmMuchToTrdVectors::CbmMuchToTrdVectors()
  : FairTask("MuchToTrdVectors")
  , fTrackArray(NULL)
  , fNofTracks(0)
  , fMuchTracks(NULL)
  , fTrdTracks(NULL)
  , fZ0(-1) {}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
CbmMuchToTrdVectors::~CbmMuchToTrdVectors() {}
// -------------------------------------------------------------------------

// -----   Public method Init (abstract in base class)  --------------------
InitStatus CbmMuchToTrdVectors::Init() {

  // Get and check FairRootManager
  FairRootManager* ioman = FairRootManager::Instance();
  if (ioman == NULL) Fatal("Init", "RootManager not instantiated!");

  // Create and register GlobalVector array
  fTrackArray = new TClonesArray("CbmMuchTrack", 100);
  ioman->Register("GlobalVector", "Much", fTrackArray, kTRUE);
  fMuchTracks = static_cast<TClonesArray*>(ioman->GetObject("MuchVectorTrack"));
  fTrdTracks  = static_cast<TClonesArray*>(ioman->GetObject("TrdVector"));

  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----  SetParContainers -------------------------------------------------
void CbmMuchToTrdVectors::SetParContainers() {
  /*
  fDigiPar = (CbmTrdDigiPar*) FairRunAna::Instance()->GetRuntimeDb()->getContainer("CbmTrdDigiPar");
  cout << " ******* digiPar ******** " << fDigiPar << endl;
  exit(0);
  */
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void CbmMuchToTrdVectors::Exec(Option_t* opt) {

  fTrackArray->Delete();

  // Do all processing
  Int_t nTrd = fTrdTracks->GetEntriesFast();
  if (nTrd == 0) return;

  GetMuchVectors();

  // Merge vectors
  MergeVectors();

  // Remove clones
  RemoveClones();
}
// -------------------------------------------------------------------------

// -----   Public method Finish   ------------------------------------------
void CbmMuchToTrdVectors::Finish() {}
// -------------------------------------------------------------------------

// -----   Private method GetMuchVectors   ---------------------------------
void CbmMuchToTrdVectors::GetMuchVectors() {
  // Get MUCH vectors, propagate to TRD

  const Int_t nMinSeg = 5;
  Int_t nMuch         = fMuchTracks->GetEntriesFast();
  fXmap.clear();

  /*
  if (fZ0 < 0) {
    // Get TRD first layer position
    CbmMuchTrack *tr = (CbmMuchTrack*) fTrdTracks->UncheckedAt(0);
    fZ0 = tr->GetParamFirst()->GetZ();
  }
  */

  TMatrixF matr = TMatrixF(5, 5);
  TMatrixF unit(TMatrixF::kUnit, matr);

  for (Int_t itr = 0; itr < nMuch; ++itr) {
    CbmMuchTrack* tr = (CbmMuchTrack*) fMuchTracks->UncheckedAt(itr);
    Int_t nvecTr     = tr->GetNofHits();
    if (nvecTr < nMinSeg) continue;  // track is too short

    if (fZ0 < 0) {
      // Get MUCH first layer position of the last station
      fZ0 = tr->GetParamFirst()->GetZ();
    }

    // Propagate to TRD
    FairTrackParam parFirst = *tr->GetParamFirst();
    /*
    Double_t zbeg = parFirst.GetZ();
    Double_t dz = fZ0 - zbeg;
    // Propagate params
    parFirst.SetX(parFirst.GetX() + dz * parFirst.GetTx());
    parFirst.SetY(parFirst.GetY() + dz * parFirst.GetTy());
    parFirst.SetZ(parFirst.GetZ() + dz);
    TMatrixFSym cov(5);
    parFirst.CovMatrix(cov);
    cov(4,4) = 1.0;
    //cov.Print();
    TMatrixF ff = unit;
    //ff.Print();
    //ff(0,2) = ff(1,3) = dz;
    ff(2,0) = ff(3,1) = dz;
    //cout << " Cov: " << dz << " " << cov(0,0) << " " << cov(1,1) << endl;
    TMatrixF cf(cov,TMatrixF::kMult,ff);
    TMatrixF fcf(ff,TMatrixF::kTransposeMult,cf);
    cov.SetMatrixArray(fcf.GetMatrixArray());
    //cout << " Cov1: " << dz << " " << cov(0,0) << " " << cov(1,1) << endl;
    cov.Invert(); // weight matrix
    parFirst.SetCovMatrix(cov); 
    */
    tr->SetParamLast(&parFirst);
    fXmap.insert(pair<Double_t, Int_t>(parFirst.GetX(), itr));
  }
}
// -------------------------------------------------------------------------

// -----   Private method MergeVectors   -----------------------------------
void CbmMuchToTrdVectors::MergeVectors() {
  // Merge MUCH and TRD tracks

  //const Double_t window = 25.0, chi2max = 24;
  const Double_t window = 35.0, chi2max = 24;
  multimap<Double_t, Int_t>::iterator mit, mitb, mite;
  FairTrackParam parOk;

  TMatrixF matr = TMatrixF(5, 5);
  TMatrixF unit(TMatrixF::kUnit, matr);

  Int_t nTrd = fTrdTracks->GetEntriesFast();

  for (Int_t itr = 0; itr < nTrd; ++itr) {
    CbmMuchTrack* tr = (CbmMuchTrack*) fTrdTracks->UncheckedAt(itr);
    //const FairTrackParam *par1 = tr->GetParamFirst();
    FairTrackParam par1 = *tr->GetParamFirst();
    // AZ
    Double_t zbeg = par1.GetZ();
    Double_t dz   = fZ0 - zbeg;
    // Propagate params
    par1.SetX(par1.GetX() + dz * par1.GetTx());
    par1.SetY(par1.GetY() + dz * par1.GetTy());
    par1.SetZ(par1.GetZ() + dz);
    TMatrixFSym cov(5);
    par1.CovMatrix(cov);
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
    cov.Invert();  // weight matrix
    par1.SetCovMatrix(cov);
    //AZ
    //Double_t xv = par1->GetX();
    //Double_t yv = par1->GetY();
    Double_t xv = par1.GetX();
    Double_t yv = par1.GetY();
    mitb        = fXmap.lower_bound(xv - window);  // lower X-window edge
    mite        = fXmap.upper_bound(xv + window);  // upper X-window edge
    TMatrixFSym w1(5);
    //par1->CovMatrix(w1);
    //Float_t pars1[5] = {(Float_t)par1->GetX(), (Float_t)par1->GetY(), (Float_t)par1->GetTx(), (Float_t)par1->GetTy(), 1.0};
    par1.CovMatrix(w1);
    Float_t pars1[5] = {(Float_t) par1.GetX(),
                        (Float_t) par1.GetY(),
                        (Float_t) par1.GetTx(),
                        (Float_t) par1.GetTy(),
                        1.0};
    TMatrixF p1(5, 1, pars1);
    TMatrixF wp1(w1, TMatrixF::kTransposeMult, p1);
    //Double_t x0 = parOk.GetX(), y0 = parOk.GetY();

    for (mit = mitb; mit != mite; ++mit) {
      Int_t indx           = mit->second;
      CbmMuchTrack* muchTr = (CbmMuchTrack*) fMuchTracks->UncheckedAt(indx);
      const FairTrackParam* par2 = muchTr->GetParamLast();
      if (par2->GetY() < yv - window || par2->GetY() > yv + window) continue;

      TMatrixFSym w2(5);
      par2->CovMatrix(w2);
      TMatrixFSym w20  = w2;
      Float_t pars2[5] = {(Float_t) par2->GetX(),
                          (Float_t) par2->GetY(),
                          (Float_t) par2->GetTx(),
                          (Float_t) par2->GetTy(),
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
      if (c2 < 0 || c2 > chi2max) continue;

      // Merged track parameters
      parOk.SetX(pMerge(0, 0));
      parOk.SetY(pMerge(1, 0));
      parOk.SetZ(par2->GetZ());
      parOk.SetTx(pMerge(2, 0));
      parOk.SetTy(pMerge(3, 0));
      parOk.SetCovMatrix(w2);
      AddTrack(muchTr, tr, indx, itr, parOk, c2);  // add track
    }
  }
}
// -------------------------------------------------------------------------

// -----   Private method AddTrack   ---------------------------------------
void CbmMuchToTrdVectors::AddTrack(CbmMuchTrack* tr1,
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

  track->SetPreviousTrackId(indx1);     // MUCH track index
  track->AddHit(indx2, kMUCHSTRAWHIT);  // TRD vector index

  if (tr1->GetFlag() == tr2->GetFlag())
    track->SetFlag(tr1->GetFlag());
  else
    track->SetFlag(-1);

  gLogger->Info(MESSAGE_ORIGIN,
                "CbmMuchToTrdVectors::AddTrack: trID1=%i, trID2=%i, chi2=%f",
                tr1->GetFlag(),
                tr2->GetFlag(),
                track->GetChiSq());
}
// -------------------------------------------------------------------------

// -----   Private method RemoveClones   -----------------------------------
void CbmMuchToTrdVectors::RemoveClones() {
  // Remove clone tracks (having the same MUCH track)

  Int_t nvec = fTrackArray->GetEntriesFast();

  // Do sorting according to "quality"
  multimap<Double_t, Int_t> qMap;
  multimap<Double_t, Int_t>::iterator it, it1;

  for (Int_t i = 0; i < nvec; ++i) {
    CbmMuchTrack* vec = (CbmMuchTrack*) fTrackArray->UncheckedAt(i);
    //Double_t qual = vec->GetNofHits() + (99 - TMath::Min(vec->GetChiSq(),99.0)) / 100;
    Double_t qual = vec->GetChiSq() / vec->GetNDF();
    qMap.insert(pair<Double_t, Int_t>(qual, i));
  }

  for (it = qMap.begin(); it != qMap.end(); ++it) {
    CbmMuchTrack* vec = (CbmMuchTrack*) fTrackArray->UncheckedAt(it->second);
    if (vec == NULL) continue;
    Int_t prevIndx = vec->GetPreviousTrackId();

    it1 = it;
    for (++it1; it1 != qMap.end(); ++it1) {
      CbmMuchTrack* vec1 =
        (CbmMuchTrack*) fTrackArray->UncheckedAt(it1->second);
      if (vec1 == NULL) continue;

      if (vec1->GetPreviousTrackId() == prevIndx) {
        fTrackArray->RemoveAt(it1->second);
        continue;
      }
    }
  }  // for (it = qMap.begin();

  fTrackArray->Compress();
  fNofTracks = fTrackArray->GetEntriesFast();

  cout << " Global vectors after clones removed: " << nvec << " " << fNofTracks
       << endl;
}
// -------------------------------------------------------------------------

ClassImp(CbmMuchToTrdVectors);
