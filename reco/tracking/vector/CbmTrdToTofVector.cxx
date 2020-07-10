/** CbmTrdToTofVector.cxx
 *@author A.Zinchenko <Alexander.Zinchenko@jinr.ru>
 *@since 2017
 **/
#include "CbmTrdToTofVector.h"
#include "CbmDefs.h"
#include "CbmMCDataArray.h"
#include "CbmMCDataManager.h"
#include "CbmMatch.h"
#include "CbmMuchTrack.h"
#include "CbmSetup.h"
#include "CbmTofAddress.h"
#include "CbmTofDigi.h"
#include "CbmTofHit.h"
#include "CbmTofPoint.h"

#include "FairRootManager.h"

#include <TClonesArray.h>
#include <TGeoBBox.h>
#include <TGeoManager.h>
#include <TMath.h>
#include <TMatrixDSym.h>
#include <TVectorD.h>

#include <iostream>

using std::cout;
using std::endl;
using std::map;
using std::multimap;
using std::pair;
using std::set;

// -----   Default constructor   -------------------------------------------
CbmTrdToTofVector::CbmTrdToTofVector()
  : FairTask("Trd2TofVec")
  , fTrackArray(NULL)
  , fNofTracks(0)
  , fHits(NULL)
  , fHitMatches(NULL)
  , fPoints(NULL)
  , fDigis(NULL)
  , fDigiMatches(NULL)
  , fTrdTracks(NULL)
  , fErrX(1.0)
  , fErrY(1.0)
  , fCutChi2(24.0)  // chi2/ndf = 6 for 4 hits
{}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
CbmTrdToTofVector::~CbmTrdToTofVector() {}
// -------------------------------------------------------------------------

// -----   Public method Init (abstract in base class)  --------------------
InitStatus CbmTrdToTofVector::Init() {

  // Get and check FairRootManager
  FairRootManager* ioman = FairRootManager::Instance();
  if (ioman == NULL) Fatal("Init", "RootManager not instantiated!");

  // Create and register TrdTrack array (if necessary)
  //fTrackArray = static_cast<TClonesArray*> (ioman->GetObject("MuchVector"));
  //if (fTrackArray == NULL) {
  fTrackArray = new TClonesArray("CbmMuchTrack", 100);
  ioman->Register("TofVector", "Tof", fTrackArray, kTRUE);
  //}
  fHits       = static_cast<TClonesArray*>(ioman->GetObject("TofHit"));
  fHitMatches = static_cast<TClonesArray*>(ioman->GetObject("TofDigiMatch"));
  //fPoints = static_cast<TClonesArray*> (ioman->GetObject("TofPoint"));
  CbmMCDataManager* mcManager =
    (CbmMCDataManager*) ioman->GetObject("MCDataManager");
  if (NULL == mcManager) LOG(fatal) << GetName() << ": No CbmMCDataManager!";
  fPoints = mcManager->InitBranch("TofPoint");
  fDigis  = static_cast<TClonesArray*>(ioman->GetObject("TofDigi"));
  fDigiMatches =
    static_cast<TClonesArray*>(ioman->GetObject("TofDigiMatchPoints"));
  fTrdTracks = static_cast<TClonesArray*>(ioman->GetObject("TrdVector"));

  TString tag;
  CbmSetup::Instance()->GetGeoTag(kTof, tag);
  cout << " ******* TOF tag ******** " << tag << endl;
  tag.Prepend("tof_");

  //CbmTofGeoHandler geoHandler;
  //geoHandler.Init();

  // Get TOF location in Z
  TGeoVolume* tofV = gGeoManager->GetVolume(tag);
  TGeoBBox* shape  = (TGeoBBox*) tofV->GetShape();
  shape->GetAxisRange(3, fZ[0], fZ[1]);
  cout << " TOF span in Z: " << fZ[0] << " " << fZ[1] << endl;

  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----  SetParContainers -------------------------------------------------
void CbmTrdToTofVector::SetParContainers() {
  /*
  fDigiPar = (CbmTrdDigiPar*) FairRunAna::Instance()->GetRuntimeDb()->getContainer("CbmTrdDigiPar");
  cout << " ******* digiPar ******** " << fDigiPar << endl;
  exit(0);
  */
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void CbmTrdToTofVector::Exec(Option_t* opt) {

  fTrackArray->Delete();

  // Do all processing

  // Get hits
  GetHits();

  // Build vectors (extrapolate TRD vectors thru the thickness of TOF)
  MakeVectors();

  // Remove vectors with wrong orientation
  // (using empirical cuts for omega muons at 8 GeV)
  //CheckParams();

  // Match TOF hits to TRD vectors
  MatchTofToTrd();

  // Remove clones
  //RemoveClones();

  // Store vectors
  StoreVectors();
}
// -------------------------------------------------------------------------

// -----   Public method Finish   ------------------------------------------
void CbmTrdToTofVector::Finish() {}
// -------------------------------------------------------------------------

// -----   Private method GetHits   ----------------------------------------
void CbmTrdToTofVector::GetHits() {
  // Arrange hits according to their Phi-angle

  fHitX.clear();
  fHitY.clear();
  fHitIds.clear();
  fHitTime.clear();

  Int_t nHits = fHits->GetEntriesFast(), sel = 0;

  for (Int_t i = 0; i < nHits; ++i) {
    CbmTofHit* hit = (CbmTofHit*) fHits->UncheckedAt(i);

    //!!! For debug - select hits with certain track IDs
    sel = 1;  //SelectHitId(hit);
    if (!sel) continue;
    //
    fHitX.insert(pair<Double_t, Int_t>(hit->GetX(), i));
    fHitY.insert(pair<Double_t, Int_t>(hit->GetY(), i));
    CbmMatch* match = (CbmMatch*) fHitMatches->UncheckedAt(i);
    Int_t nlinks    = match->GetNofLinks();
    set<Int_t> ids;
    fHitIds[i] = ids;

    for (Int_t il = 0; il < nlinks; ++il) {
      const CbmLink link = match->GetLink(il);
      CbmTofDigi* digi   = (CbmTofDigi*) fDigis->UncheckedAt(link.GetIndex());
      //AZ CbmMatch *digiM = (CbmMatch*) fDigiMatches->UncheckedAt(link.GetIndex());
      CbmMatch* digiM = digi->GetMatch();
      Int_t npoints   = digiM->GetNofLinks();

      for (Int_t ip = 0; ip < npoints; ++ip) {
        const CbmLink link1 = digiM->GetLink(ip);
        //CbmTofPoint *point = (CbmTofPoint*) fPoints->UncheckedAt(link1.GetIndex());
        CbmTofPoint* point = (CbmTofPoint*) fPoints->Get(
          link1.GetFile(), link1.GetEntry(), link1.GetIndex());
        fHitIds[i].insert(point->GetTrackID());
        if (fHitTime.find(i) == fHitTime.end())
          fHitTime[i] = point->GetTime();
        else
          fHitTime[i] = TMath::Min(point->GetTime(), fHitTime[i]);
        //cout << point->GetTrackID() << endl;
      }
    }
  }
}
// -------------------------------------------------------------------------

// -----   Private method MakeVectors   ------------------------------------
void CbmTrdToTofVector::MakeVectors() {
  // Make vectors (extrapolate TRD vectors through the TOF thickness)

  Int_t nvec = fVectors.size();
  for (Int_t j = 0; j < nvec; ++j)
    delete fVectors[j];
  fVectors.clear();

  fLineX.clear();

  Int_t nTrd = fTrdTracks->GetEntriesFast();
  cout << " TRD tracks: " << nTrd << endl;

  for (Int_t iv = 0; iv < nTrd; ++iv) {
    CbmMuchTrack* tr            = (CbmMuchTrack*) fTrdTracks->UncheckedAt(iv);
    const FairTrackParam& param = *tr->GetParamLast();
    TLine line;
    line.SetUniqueID(iv);

    for (Int_t i = 0; i < 2; ++i) {
      Double_t dz = fZ[i] - param.GetZ();
      if (i == 0) {
        line.SetX1(param.GetX() + param.GetTx() * dz);
        line.SetY1(param.GetY() + param.GetTy() * dz);
      } else {
        line.SetX2(param.GetX() + param.GetTx() * dz);
        line.SetY2(param.GetY() + param.GetTy() * dz);
      }
    }
    fLineX.insert(
      pair<Double_t, TLine>(TMath::Min(line.GetX1(), line.GetX2()), line));
  }
}
// -------------------------------------------------------------------------

// -----   Private method MatchTofToTrd   ----------------------------------
void CbmTrdToTofVector::MatchTofToTrd() {
  // Match TOF hits to TRD vectors

  //const Double_t window = 30.0;
  const Double_t window = 50.0;
  multimap<Double_t, Int_t>::iterator mitb, mite, mit1;
  multimap<Double_t, pair<Int_t, Int_t>> rads;
  map<pair<Int_t, Int_t>, pair<Double_t, Double_t>> dtdl;
  set<Int_t> setVec, setHit;

  for (multimap<Double_t, TLine>::iterator mit = fLineX.begin();
       mit != fLineX.end();
       ++mit) {
    Double_t rmin = 999999, dtmin = 0, dlmin = 0;
    TVector3 trd(mit->second.GetX2() - mit->second.GetX1(),
                 mit->second.GetY2() - mit->second.GetY1(),
                 0.0);
    Double_t trdLeng = trd.Mag();
    Int_t indVec     = mit->second.GetUniqueID();
    CbmMuchTrack* tr = (CbmMuchTrack*) fTrdTracks->UncheckedAt(indVec);
    Int_t id = tr->GetFlag(), ihit = -1;

    // Select TOF hits for matching (apply coordinate windows)
    mitb = fHitX.lower_bound(mit->first - window);
    mite = fHitX.upper_bound(
      TMath::Max(mit->second.GetX1(), mit->second.GetX2()) + window);
    set<Int_t> inds;
    for (mit1 = mitb; mit1 != mite; ++mit1)
      inds.insert(mit1->second);
    mitb = fHitY.lower_bound(
      TMath::Min(mit->second.GetY1(), mit->second.GetY2()) - window);
    mite = fHitY.upper_bound(
      TMath::Max(mit->second.GetY1(), mit->second.GetY2()) + window);

    for (mit1 = mitb; mit1 != mite; ++mit1) {
      if (inds.find(mit1->second) == inds.end()) continue;  // outside window
      CbmTofHit* hit = (CbmTofHit*) fHits->UncheckedAt(mit1->second);
      TVector3 tof(hit->GetX() - mit->second.GetX1(),
                   hit->GetY() - mit->second.GetY1(),
                   0.0);
      Double_t dt = TMath::Abs(trd.Cross(tof).Z()) / trdLeng;
      Double_t dl = trd * tof / trdLeng;
      if (dl > trdLeng)
        dl -= trdLeng;
      else if (dl > 0)
        dl = 0;
      Double_t rad = dl * dl + dt * dt;
      // Save matches
      rads.insert(pair<Double_t, pair<Int_t, Int_t>>(
        rad, pair<Int_t, Int_t>(indVec, mit1->second)));
      dtdl[pair<Int_t, Int_t>(indVec, mit1->second)] =
        pair<Double_t, Double_t>(dt, dl);
      setVec.insert(indVec);
      setHit.insert(mit1->second);
      /*
      if (rad < rmin) {
	rmin = rad;
	dtmin = dt;
	dlmin = dl;
	ihit = mit1->second;
      }
      */
    }
    /*
    if (ihit >= 0) {
      Bool_t match = kFALSE;
      if (fHitIds[ihit].find(id) != fHitIds[ihit].end()) match = kTRUE;
      //cout << dtmin << " " << dlmin << " " << id << " " << match << endl;
      CbmMuchTrack *trTof = new CbmMuchTrack(*tr);
      trTof->SetPreviousTrackId(mit->second.GetUniqueID()); // index of TRD vector
      FairTrackParam param;
      param.SetX(dtmin); // use storage
      param.SetY(dlmin);
      trTof->SetParamLast(&param);
      trTof->SetFlag(match);
      trTof->SetUniqueID(id);
      fVectors.push_back(trTof);
    }
    */
  }  // for (multimap<Double_t,TLine>::iterator mit = fLineX.begin();

  // Create vectors
  for (multimap<Double_t, pair<Int_t, Int_t>>::iterator rmit = rads.begin();
       rmit != rads.end();
       ++rmit) {
    Int_t indVec = rmit->second.first, ihit = rmit->second.second;
    if (setVec.find(indVec) == setVec.end())
      continue;                                       // already matched vector
    if (setHit.find(ihit) == setHit.end()) continue;  // already matched hit

    Bool_t match     = kFALSE;
    CbmMuchTrack* tr = (CbmMuchTrack*) fTrdTracks->UncheckedAt(indVec);
    Int_t id         = tr->GetFlag();
    if (fHitIds[ihit].find(id) != fHitIds[ihit].end()) match = kTRUE;
    //cout << dtmin << " " << dlmin << " " << id << " " << match << endl;
    CbmMuchTrack* trTof = new CbmMuchTrack(*tr);
    trTof->SetPreviousTrackId(indVec);  // index of TRD vector
    FairTrackParam param;
    param.SetX(dtdl[rmit->second].first);  // use storage
    param.SetY(dtdl[rmit->second].second);
    param.SetZ(rmit->first);
    trTof->SetParamLast(&param);
    trTof->SetFlag(match);
    trTof->SetUniqueID(id);
    trTof->SetNDF(ihit);              // TOF hit index - use storage
    trTof->SetChiSq(fHitTime[ihit]);  // min. time - use storage
    fVectors.push_back(trTof);

    setVec.erase(indVec);
    if (setVec.size() == 0) break;
    setHit.erase(ihit);
    if (setHit.size() == 0) break;
  }
}
// -------------------------------------------------------------------------

// -----   Private method CheckParams   ------------------------------------
void CbmTrdToTofVector::CheckParams() {
  // Remove vectors with wrong orientation
  // using empirical cuts for omega muons at 8 Gev

  const Double_t cut[2] = {0.6, 0.6};  // !!! empirical !!!

  Int_t nvec = fVectors.size();

  for (Int_t iv = 0; iv < nvec; ++iv) {
    CbmMuchTrack* vec            = fVectors[iv];
    const FairTrackParam* params = vec->GetParamFirst();
    Double_t dTx = params->GetTx() - params->GetX() / params->GetZ();
    if (TMath::Abs(dTx) > cut[0])
      vec->SetChiSq(-1.0);
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
  cout << " Vectors after parameter check: " << nvec << " " << fVectors.size()
       << endl;
}
// -------------------------------------------------------------------------

// -----   Private method RemoveClones   -----------------------------------
void CbmTrdToTofVector::RemoveClones() {
  // Remove clone vectors (having at least 3 the same hits)

  //Int_t nthr = 3, planes[20];
  Int_t nthr = 2, planes[20];

  Int_t nvec = fVectors.size();

  // Do sorting according to "quality"
  multimap<Double_t, CbmMuchTrack*> qMap;
  multimap<Double_t, CbmMuchTrack*>::iterator it, it1;

  for (Int_t i = 0; i < nvec; ++i) {
    CbmMuchTrack* vec = fVectors[i];
    Double_t qual =
      vec->GetNofHits() + (99 - TMath::Min(vec->GetChiSq(), 99.0)) / 100;
    qMap.insert(pair<Double_t, CbmMuchTrack*>(-qual, vec));
  }

  for (it = qMap.begin(); it != qMap.end(); ++it) {
    CbmMuchTrack* vec = it->second;
    if (vec->GetChiSq() < 0) continue;
    for (Int_t j = 0; j < fgkPlanes; ++j)
      planes[j] = -1;

    Int_t nhits = vec->GetNofHits();
    for (Int_t ih = 0; ih < nhits; ++ih) {
      CbmTofHit* hit = (CbmTofHit*) fHits->UncheckedAt(vec->GetHitIndex(ih));
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
        CbmTofHit* hit = (CbmTofHit*) fHits->UncheckedAt(vec1->GetHitIndex(ih));
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
        if (nhits > nhits1 + 0)
          clone = 1;
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
  cout << " CbmTrdToTofVector:: Vectors after clones removed: " << nvec << " "
       << fVectors.size() << endl;
}
// -------------------------------------------------------------------------

// -----   Private method StoreVectors   -----------------------------------
void CbmTrdToTofVector::StoreVectors() {
  // Store vectors (CbmMuchTracks) into TClonesArray

  Int_t ntrs     = fTrackArray->GetEntriesFast();
  Int_t nHitsTot = fHits->GetEntriesFast();

  //set<Int_t> usedHits;
  Int_t nvec = fVectors.size();

  for (Int_t iv = 0; iv < nvec; ++iv) {
    CbmMuchTrack* tr =
      new ((*fTrackArray)[ntrs++]) CbmMuchTrack(*(fVectors[iv]));
    //cout << " Track: " << tr->GetNofHits() << endl;
    //for (Int_t j = 0; j < tr->GetNofHits(); ++j) cout << j << " " << tr->GetHitIndex(j) << " " << fVectors[ist][iv]->GetHitIndex(j) << endl;
    // Set hit flag (to check Lit tracking)
    //Int_t nhits = tr->GetNofHits();

    // Save IDs of contributing points to the hit
    CbmMatch* match = new CbmMatch;
    Int_t ihit      = fVectors[iv]->GetNDF();
    for (set<Int_t>::iterator sit = fHitIds[ihit].begin();
         sit != fHitIds[ihit].end();
         ++sit)
      match->AddLink(1.0, *sit);
    tr->SetMatch(match);
  }
}
// -------------------------------------------------------------------------

// -----   Private method ProcessPlane   -----------------------------------
void CbmTrdToTofVector::ProcessPlane(Int_t lay, Int_t patt, Int_t flag0) {
  // Main processing engine (recursively adds layer hits to the vector)

  //const Double_t cut[2] = {0.8, 0.8}; // !!! empirical !!!
  const Double_t cut[2] = {0.6, 0.6};  // !!! empirical !!!

  Double_t pars[4] = {0.0};
  Int_t flag       = 0;
  multimap<Int_t, Int_t>::iterator mit;
  // Clear bits
  patt &= ~(1 << lay);
  /*
  for (mit = fHitPl[lay].begin(); mit != fHitPl[lay].end(); ++mit) {
    Int_t indx = mit->second;

    CbmTofHit *hit = (CbmTofHit*) fHits->UncheckedAt(indx);
    fXy[lay][0] = hit->GetX();
    fXy[lay][1] = hit->GetY();
    fXy[lay][2] = hit->GetDx();
    fXy[lay][3] = hit->GetDy();
    //fXy[lay][2] = TMath::Max (hit->GetDx(),fErrX[ista]);
    //fXy[lay][3] = TMath::Max (hit->GetDy(),fErrY[ista]);
    fXy[lay][4] = hit->GetZ();
    // Clear bits
    patt &= ~(1 << lay); 

    // Check slopes
    Int_t lay0 = 0;
    if (!(patt & (1 << lay0))) ++lay0;;
    Double_t dx = fXy[lay][0] - fXy[lay0][0];
    Double_t dz = fXy[lay][4] - fXy[lay0][4];
    Double_t dTx = TMath::Abs(dx/dz) - TMath::Abs(fXy[lay][0]/fXy[lay][4]);
    if (TMath::Abs(dTx) > cut[0]) continue;
    Double_t dy = fXy[lay][1] - fXy[lay0][1];
    Double_t dTy = TMath::Abs(dy/dz) - TMath::Abs(fXy[lay][1]/fXy[lay][4]);
    if (TMath::Abs(dTy) > cut[1]) continue;

    fXyi[lay][1] = indx;
    flag = 1;

    // Set bit
    patt |= (1 << lay);
    //cout << lay << " " << patt << " " << hit->GetX() << " " << hit->GetY() << " " << hit->GetZ() << " " << endl;

    if (lay < fgkPlanes - 1) ProcessPlane(lay+1, patt, flag);
    else if (TString::Itoa(patt,2).CountChar('1') > 2) {
      // Straight line fit of the vector with > 2 hits
      FindLine(patt, pars);
      Double_t chi2 = FindChi2(patt, pars);
      cout << " *** Stat: " << chi2 << " " << pars[0] << " " << pars[1] << endl;
      if (chi2 <= fCutChi2) AddVector(patt, chi2, pars); // add vector to the temporary container
    }
  } // for (mit = fHitPl[lay].begin();

  //if (flag == 0) {
  if (flag >= 0) {
    // Missing plane
    if (flag0 == 0) return; // 2 consecutive missing planes
    // Clear bits
    patt &= ~(1 << lay); 
    if (lay < fgkPlanes - 1) ProcessPlane(lay+1, patt, flag);
    else if (TString::Itoa(patt,2).CountChar('1') > 2) {
      // Straight line fit of the vector with > 2 hits
      FindLine(patt, pars);
      Double_t chi2 = FindChi2(patt, pars);
      cout << " *** Stat: " << chi2 << " " << pars[0] << " " << pars[1] << endl;
      if (chi2 <= fCutChi2) AddVector(patt, chi2, pars); // add vector to the temporary container
    }
  }
  */
}
// -------------------------------------------------------------------------

// -----   Private method FindLine   ---------------------------------------
void CbmTrdToTofVector::FindLine(Int_t patt, Double_t* pars) {
  // Fit of hits to the straight line

  // Solve system of linear equations
  Bool_t ok = kFALSE, onoff;
  TVectorD b(4);
  /*
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
  for (Int_t i = 0; i < 4; ++i) pars[i] = solve[i];
  //for (Int_t i = 0; i < 4; ++i) { cout << pars[i] << " "; if (i == 3) cout << endl; }
  //Double_t y1 = cosa / sina * (uz[1][0] * cosa - uz[0][0]) + uz[1][0] * sina;
  //Double_t y2 = -cosa / sina * (uz[2][0] * cosa - uz[0][0]) - uz[2][0] * sina;
  //cout << " OK " << ok << " " << y1 << " " << y2 << endl;
  */
}
// -------------------------------------------------------------------------

// -----   Private method FindChi2   ---------------------------------------
Double_t CbmTrdToTofVector::FindChi2(Int_t patt, Double_t* pars) {
  // Compute chi2 of the fit

  Double_t chi2 = 0, x = 0, y = 0;
  Bool_t onoff;
  /*
  for (Int_t i = 0; i < fgkPlanes; ++i) {
    onoff = patt & (1 << i);
    if (!onoff) continue;
    x = pars[0] + pars[2] * fDz[i];
    y = pars[1] + pars[3] * fDz[i];
    //Double_t dx = (x - fXy[i][0]) / fErrX;
    //Double_t dy = (y - fXy[i][1]) / fErrY;
    Double_t dx = (x - fXy[i][0]) / TMath::Max(fXy[i][2],fErrX);
    Double_t dy = (y - fXy[i][1]) / TMath::Max(fXy[i][3],fErrY);
    chi2 += dx * dx;
    chi2 += dy * dy;
  }
  */
  return chi2;
}
// -------------------------------------------------------------------------

// -----   Private method AddVector   --------------------------------------
void CbmTrdToTofVector::AddVector(Int_t patt, Double_t chi2, Double_t* pars) {
  // Add vector to the temporary container (as a MuchTrack)

  Bool_t refit = kFALSE;  //kTRUE;
  TMatrixDSym cov(4);
  /*
  if (refit) {
    // Refit line with individual hit errors
    //cout << " Before: " << chi2 << endl;
    //Refit(patt, chi2, pars, cov);
    //cout << " After: " << chi2 << endl;
  } else {
    cov = *fMatr[patt];
    cov *= (fErrX * fErrX); // the same error in X and Y
  }
  cov.ResizeTo(5,5);
  cov(4,4) = 1.0;

  CbmMuchTrack *track = new CbmMuchTrack();
  track->SetChiSq(chi2);
  FairTrackParam par(pars[0], pars[1], fZ0, pars[2], pars[3], 0.0, cov);
  track->SetParamFirst(&par);
  par.SetZ(fZ0 + fDz[fgkPlanes-1]);
  par.SetX(pars[0] + fDz[fgkPlanes-1] * pars[2]);
  par.SetY(pars[1] + fDz[fgkPlanes-1] * pars[3]);
  track->SetParamLast(&par);
  //track->SetUniqueID(ista); // just to pass the value

  for (Int_t ipl = 0; ipl < fgkPlanes; ++ipl) {
    if (!(patt & (1 << ipl))) continue;
    track->AddHit(fXyi[ipl][1], kTRDHIT);
  }
  Int_t ndf = (track->GetNofHits() > 2) ? track->GetNofHits() * 2 - 4 : 1;
  track->SetNDF(ndf);
  SetTrackId(track); // set track ID as its flag 
  fVectors.push_back(track);
  */
}
// -------------------------------------------------------------------------

// -----   Private method SetTrackId   -------------------------------------
void CbmTrdToTofVector::SetTrackId(CbmMuchTrack* vec) {
  // Set vector ID as its flag (maximum track ID of its poins)

  map<Int_t, Int_t> ids;
  Int_t nhits = vec->GetNofHits(), id = 0;

  /*
  for (Int_t ih = 0; ih < nhits; ++ih) {
    CbmTofHit *hit = (CbmTofHit*) fHits->UncheckedAt(vec->GetHitIndex(ih));
    CbmTrdCluster *clus = (CbmTrdCluster*) fClusters->UncheckedAt(hit->GetRefId());
    Int_t nDigis = clus->GetNofDigis();

    for (Int_t j = 0; j < nDigis; ++j) {
      CbmMatch* digiM = (CbmMatch*) fDigiMatches->UncheckedAt(clus->GetDigi(j));
      Int_t np = digiM->GetNofLinks();

      for (Int_t ip = 0; ip < np; ++ip) {
	//CbmMuchPoint* point = (CbmMuchPoint*) fPoints->UncheckedAt(digiM->GetLink(0).GetIndex());
	CbmTrdPoint* point = (CbmTrdPoint*) fPoints->UncheckedAt(digiM->GetLink(ip).GetIndex());
	id = point->GetTrackID();
	//if (np > 1) cout << ip << " " << id << endl;
	if (ids.find(id) == ids.end()) ids.insert(pair<Int_t,Int_t>(id,1));
	else ++ids[id];
      }
      //if (np > 1) { cout << " digi " << digiM->GetNofLinks() << " " << ista << " " << hit->GetX() << " " << hit->GetY() << endl; exit(0); }
    }
  }
  Int_t maxim = -1, idmax = -9;
  for (map<Int_t,Int_t>::iterator it = ids.begin(); it != ids.end(); ++it) {
    if (it->second > maxim) {
      maxim = it->second;
      idmax = it->first;
    }
  }
  // Set vector ID as its flag
  vec->SetFlag(idmax);
  */
}
// -------------------------------------------------------------------------

ClassImp(CbmTrdToTofVector);
