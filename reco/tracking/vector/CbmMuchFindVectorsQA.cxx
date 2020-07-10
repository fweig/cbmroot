/** CbmMuchFindVectorsQA.cxx
 *@author A.Zinchenko <Alexander.Zinchenko@jinr.ru>
 *@since 2014
 **/
#include "CbmMuchFindVectorsQA.h"
#include "CbmMuchCluster.h"
#include "CbmMuchDigiMatch.h"
#include "CbmMuchFindHitsStraws.h"
#include "CbmMuchFindVectors.h"
#include "CbmMuchModule.h"
#include "CbmMuchPixelHit.h"
#include "CbmMuchPoint.h"
#include "CbmMuchStation.h"
#include "CbmMuchStrawHit.h"
#include "CbmMuchTrack.h"

#include "FairEventHeader.h"
#include "FairRootManager.h"
#include "FairRunAna.h"

#include <TClonesArray.h>
#include <TF1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TMath.h>
#include <TROOT.h>

#include <iostream>

using std::cout;
using std::endl;
using std::map;
using std::multiset;
using std::pair;
using std::set;

// -----   Default (stabdard) constructor   --------------------------------
CbmMuchFindVectorsQA::CbmMuchFindVectorsQA()
  : FairTask("MuchFindVectorsQA")
  , fGeoScheme(CbmMuchGeoScheme::Instance())
  , fStatFirst(-1)
  , fNstat(0) {}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
CbmMuchFindVectorsQA::~CbmMuchFindVectorsQA() {}
// -------------------------------------------------------------------------

// -----   Public method Init (abstract in base class)  --------------------
InitStatus CbmMuchFindVectorsQA::Init() {

  // Get and check FairRootManager
  FairRootManager* ioman = FairRootManager::Instance();
  if (ioman == NULL)
    Fatal("CbmMuchFindTracksQA::Init", "RootManager not instantiated!");

  //CbmMuchFindHitsStraws *hitFinder = (CbmMuchFindHitsStraws*)
  //FairRun::Instance()->GetTask("CbmMuchFindHitsStraws");
  //if (hitFinder == NULL) Fatal("CbmMuchFindTracks::Init", "CbmMuchFindHitsStraws not run!");

  fVectors  = static_cast<TClonesArray*>(ioman->GetObject("MuchVector"));
  fMCTracks = static_cast<TClonesArray*>(ioman->GetObject("MCTrack"));
  fPoints   = static_cast<TClonesArray*>(ioman->GetObject("MuchPoint"));
  fHits     = static_cast<TClonesArray*>(ioman->GetObject("MuchStrawHit"));
  fHitsGem  = static_cast<TClonesArray*>(ioman->GetObject("MuchPixelHit"));
  fDigis    = static_cast<TClonesArray*>(ioman->GetObject("MuchStrawDigi"));
  fDigisGem = static_cast<TClonesArray*>(ioman->GetObject("MuchDigi"));
  fDigiMatches =
    static_cast<TClonesArray*>(ioman->GetObject("MuchStrawDigiMatch"));
  fDigiMatchesGem =
    static_cast<TClonesArray*>(ioman->GetObject("MuchDigiMatch"));
  fClusters = static_cast<TClonesArray*>(ioman->GetObject("MuchCluster"));

  // Get straw system configuration
  Int_t nSt = fGeoScheme->GetNStations();

  for (Int_t i = 0; i < nSt; ++i) {
    CbmMuchStation* st = fGeoScheme->GetStation(i);
    CbmMuchModule* mod = fGeoScheme->GetModule(i, 0, 0, 0);
    Int_t stat         = CbmMuchAddress::GetStationIndex(st->GetDetectorId());
    if (mod->GetDetectorType() == 2 && fStatFirst < 0) fStatFirst = stat;
    ++fNstat;
    Int_t nLays   = st->GetNLayers();
    fNdoubl[stat] = nLays;

    for (Int_t lay = 0; lay < nLays; ++lay) {
      CbmMuchLayer* layer = st->GetLayer(lay);

      for (Int_t iside = 0; iside < 2; ++iside) {
        CbmMuchLayerSide* side = layer->GetSide(iside);
        Int_t plane            = lay * 2 + iside;
        fZpos[stat][plane]     = side->GetZ();
      }
    }
  }
  if (fStatFirst < 0) fStatFirst = 99;  // all GEM stations

  // Create directory for histograms
  TDirectory* dir = new TDirectory("muchQA", "", "");
  dir->cd();

  BookHistos();

  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----  SetParContainers -------------------------------------------------
void CbmMuchFindVectorsQA::SetParContainers() {}
// -------------------------------------------------------------------------

// -----   Private method BookHistos   -------------------------------------
void CbmMuchFindVectorsQA::BookHistos() {
  // Book histograms

  fhNvec      = new TH1D*[fNstat];
  fhNdoubl    = new TH1D*[fNstat];
  fhNhits     = new TH1D*[fNstat];
  fhNhitsOk   = new TH1D*[fNstat];
  fhChi2      = new TH1D*[fNstat];
  fhNgood     = new TH1D*[fNstat];
  fhNghost    = new TH1D*[fNstat];
  fhChi2ok    = new TH1D*[fNstat];
  fhChi2bad   = new TH1D*[fNstat];
  fhDx        = new TH1D*[fNstat];
  fhDy        = new TH1D*[fNstat];
  fhDtx       = new TH1D*[fNstat];
  fhDty       = new TH1D*[fNstat];
  fhIds       = new TH1D*[fNstat];
  fhIdVsEv    = new TH2D*[fNstat];
  fhDtxAll    = new TH1D*[fNstat];
  fhDtyAll    = new TH1D*[fNstat];
  fhDtxOk     = new TH1D*[fNstat];
  fhDtyOk     = new TH1D*[fNstat];
  fhShort     = new TH1D*[fNstat];
  fhOverlap   = new TH2D*[fNstat];
  fhChi2mat   = new TH1D*[fNstat];
  fhMatchMult = new TH1D*[fNstat];
  fhOccup     = new TH1D*[fNstat];
  //fhSim     = new TH1D*[fNstat];
  //fhRec     = new TH1D*[fNstat];
  fhDx12 = new TH2D*[fNstat];
  fhDx23 = new TH2D*[fNstat];
  fhDy12 = new TH2D*[fNstat];
  fhDy23 = new TH2D*[fNstat];

  for (Int_t ist = 0; ist < fNstat; ++ist) {
    Int_t stat  = ist;
    fhNvec[ist] = new TH1D(Form("hNvec%i", stat),
                           Form("Number of vectors in station %i", stat),
                           200,
                           0,
                           1000);
    fhNdoubl[ist] =
      new TH1D(Form("hNdoubl%i", stat),
               Form("Number of tubes in lay. 1 in station %i", stat),
               200,
               0,
               200);
    fhNhits[ist] = new TH1D(Form("hNhits%i", stat),
                            Form("Number of hits/vector in station %i", stat),
                            20,
                            0,
                            20);
    fhNhitsOk[ist] =
      new TH1D(Form("hNhitsOk%i", stat),
               Form("Number of hits/good vector in station %i", stat),
               20,
               0,
               20);
    fhChi2[ist]  = new TH1D(Form("hChi2all%i", stat),
                           Form("Chi2 of all vectors in station %i", stat),
                           200,
                           0,
                           200);
    fhNgood[ist] = new TH1D(Form("hNgood%i", stat),
                            Form("Number of good vectors in station %i", stat),
                            100,
                            0,
                            100);
    fhNghost[ist] =
      new TH1D(Form("hNghost%i", stat),
               Form("Number of ghost vectors in station %i", stat),
               200,
               0,
               200);
    fhChi2ok[ist]  = new TH1D(Form("hChi2ok%i", stat),
                             Form("Chi2 of good vectors in station %i", stat),
                             200,
                             0,
                             200);
    fhChi2bad[ist] = new TH1D(Form("hChi2bad%i", stat),
                              Form("Chi2 of ghost vectors in station %i", stat),
                              200,
                              0,
                              200);
    fhDx[ist]      = new TH1D(
      Form("hDx%i", stat), Form("Xrec-Xmc in station %i", stat), 100, -2, 2);
    fhDy[ist] = new TH1D(
      Form("hDy%i", stat), Form("Yrec-Ymc in station %i", stat), 100, -15, 15);
    fhDtx[ist] = new TH1D(Form("hDtx%i", stat),
                          Form("TXrec-TXmc in station %i", stat),
                          100,
                          -0.2,
                          0.2);
    fhDty[ist] = new TH1D(
      Form("hDty%i", stat), Form("TYrec-TYmc in station %i", stat), 100, -1, 1);
    fhIds[ist] = new TH1D(Form("hIds%i", stat),
                          Form("Good track ID in station %i", stat),
                          100,
                          0,
                          100);
    fhIdVsEv[ist] =
      new TH2D(Form("hIdVsEv%i", stat),
               Form("Good track ID vs ev. No. in station %i", stat),
               100,
               -1,
               99,
               20,
               -1,
               19);
    fhIdVsEv[ist]->SetOption("box");
    fhDtxAll[ist]   = new TH1D(Form("hDtxAll%i", stat),
                             Form("TXvec-TXhit in station %i", stat),
                             100,
                             -2,
                             2);
    fhDtyAll[ist]   = new TH1D(Form("hDtyAll%i", stat),
                             Form("TYvec-TYhit in station %i", stat),
                             100,
                             -2,
                             2);
    fhDtxOk[ist]    = new TH1D(Form("hDtxOk%i", stat),
                            Form("TXvec-TXhit in station %i (ok)", stat),
                            100,
                            -2,
                            2);
    fhDtyOk[ist]    = new TH1D(Form("hDtyOk%i", stat),
                            Form("TYvec-TYhit in station %i (ok)", stat),
                            100,
                            -2,
                            2);
    fhDtube[ist][0] = new TH1D(Form("hDtube21_%i", stat),
                               Form("Dtube 21 vs tube in station %i", stat),
                               100,
                               -50,
                               50);
    fhDtube[ist][1] = new TH1D(Form("hDtube32_%i", stat),
                               Form("Dtube 32 vs tube in station %i", stat),
                               100,
                               -100,
                               100);
    fhDtube[ist][2] = new TH1D(Form("hDtube43_%i", stat),
                               Form("Dtube 43 vs tube in station %i", stat),
                               100,
                               -200,
                               200);
    fhShort[ist]    = new TH1D(Form("hShort%i", stat),
                            Form("Number of shared hits in station %i", stat),
                            20,
                            0,
                            20);
    fhOverlap[ist]  = new TH2D(Form("hOverlap%i", stat),
                              Form("Overlap multipl. in station %i", stat),
                              20,
                              0,
                              20,
                              20,
                              0,
                              20);
    fhChi2mat[ist]  = new TH1D(Form("hChi2mat%i", stat),
                              Form("Chi2 of matching in station %i", stat),
                              100,
                              0,
                              50);
    fhMatchMult[ist] =
      new TH1D(Form("hMatchMult%i", stat),
               Form("Multiplicity of matching in station %i", stat),
               10,
               0,
               10);
    fhOccup[ist]     = new TH1D(Form("hOccup%i", stat),
                            Form("Occupancy in station %i", stat),
                            600,
                            -300,
                            300);
    fhDtube2[ist][0] = new TH2D(Form("hDtube31_%i", stat),
                                Form("Dtube 31 vs tube in station %i", stat),
                                100,
                                -300,
                                300,
                                100,
                                -200,
                                200);
    fhDtube2[ist][1] = new TH2D(Form("hDtube42_%i", stat),
                                Form("Dtube 42 vs tube in station %i", stat),
                                100,
                                -300,
                                300,
                                100,
                                -100,
                                100);
    fhMCFit[ist][0]  = new TH1D(Form("hMCFitX_%i", stat),
                               Form("Chi2 of fit in X in station %i", stat),
                               100,
                               0,
                               50);
    fhMCFit[ist][1]  = new TH1D(Form("hMCFitY_%i", stat),
                               Form("Chi2 of fit in Y in station %i", stat),
                               100,
                               0,
                               50);
    fhDx12[ist]      = new TH2D(Form("hDx12_%i", stat),
                           Form("Dx2 vs Dx1 in station %i", stat),
                           50,
                           -10,
                           10,
                           50,
                           -10,
                           10);
    fhDx23[ist]      = new TH2D(Form("hDx23_%i", stat),
                           Form("Dx3 vs Dx2 in station %i", stat),
                           50,
                           -10,
                           10,
                           50,
                           -10,
                           10);
    fhDy12[ist]      = new TH2D(Form("hDy12_%i", stat),
                           Form("Dy2 vs Dy1 in station %i", stat),
                           50,
                           -10,
                           10,
                           50,
                           -10,
                           10);
    fhDy23[ist]      = new TH2D(Form("hDy23_%i", stat),
                           Form("Dy3 vs Dy2 in station %i", stat),
                           50,
                           -10,
                           10,
                           50,
                           -10,
                           10);
  }
  fhSim    = new TH1D("hSim", "Number of reconstructable muons", 10, 0, 10);
  fhRec    = new TH1D("hRec", "Number of reconstructed muons", 10, 0, 10);
  fhZXY[0] = new TH1D("hZX", "Z - X", 180, 0, 45);
  fhZXY[1] = new TH1D("hZY", "Z - Y", 180, 0, 45);
  fhEvents = new TH1D("hEvents", "Number of processed events", 5, 0, 5);
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void CbmMuchFindVectorsQA::Exec(Option_t* opt) {
  // Do all processing

  Int_t* mult    = new Int_t[fNstat];
  Int_t* multOk  = new Int_t[fNstat];
  Int_t* multBad = new Int_t[fNstat];
  for (Int_t i = 0; i < fNstat; ++i)
    mult[i] = multOk[i] = multBad[i] = 0;
  fhEvents->Fill(1);

  Int_t nvec = fVectors->GetEntriesFast();
  cout << " nvec " << nvec << endl;
  for (Int_t iv = 0; iv < nvec; ++iv) {
    CbmMuchTrack* vec            = (CbmMuchTrack*) fVectors->UncheckedAt(iv);
    Int_t ista                   = vec->GetUniqueID();
    const FairTrackParam* params = vec->GetParamFirst();
    ++mult[ista];
    fhChi2[ista]->Fill(vec->GetChiSq());
    fhNhits[ista]->Fill(vec->GetNofHits());
    fhDtxAll[ista]->Fill(params->GetTx() - params->GetX() / params->GetZ());
    fhDtyAll[ista]->Fill(params->GetTy() - params->GetY() / params->GetZ());
    if (CheckMatch(vec)) {
      // "Good" vector
      ++multOk[ista];
      fhChi2ok[ista]->Fill(vec->GetChiSq());
    } else {
      // Ghost
      ++multBad[ista];
      fhChi2bad[ista]->Fill(vec->GetChiSq());
    }
  }
  cout << mult[0] << " " << mult[1] << endl;

  for (Int_t i = 0; i < fNstat; ++i) {
    fhNvec[i]->Fill(mult[i]);
    fhNgood[i]->Fill(multOk[i]);
    fhNghost[i]->Fill(multBad[i]);
  }

  // Check short tracks
  CheckShorts(fHits);

  // Check efficiency
  CheckEffic();

  // For straws: fill occupancy histos as well (for completeness)
  // and number of hits in the first layer of the first doublet
  Int_t nhits = fHits->GetEntriesFast(), nDouble[10] = {0};
  for (Int_t i = 0; i < nhits; ++i) {
    CbmMuchStrawHit* hit = (CbmMuchStrawHit*) fHits->UncheckedAt(i);
    Int_t ista           = fGeoScheme->GetStationIndex(hit->GetAddress());
    fhOccup[ista]->Fill(hit->GetTube());
    Int_t lay  = fGeoScheme->GetLayerIndex(hit->GetAddress());
    Int_t side = fGeoScheme->GetLayerSideIndex(hit->GetAddress());
    if (lay == 0 && side == 0) ++nDouble[ista];
  }
  //for (Int_t ista = 0; ista < 2; ++ista) fhNdoubl[ista]->Fill(nDouble[ista]);
  for (Int_t ista = 0; ista < fNstat; ++ista)
    fhNdoubl[ista]->Fill(nDouble[ista]);

  delete[] mult;
  delete[] multOk;
  delete[] multBad;
}
// -------------------------------------------------------------------------

// -----   Private method CheckMatch   -------------------------------------
Bool_t CbmMuchFindVectorsQA::CheckMatch(CbmMuchTrack* vec) {
  // Check matching quality of the vector

  Int_t ista = vec->GetUniqueID();
  if (ista < fStatFirst) return CheckMatchGem(vec);

  Int_t nhits = vec->GetNofHits(), nthr = nhits / 2, id = 0;
  //Int_t nhits = vec->GetNofHits(), nthr = nhits - 1, id = 0;
  map<Int_t, Int_t> ids;

  for (Int_t ih = 0; ih < nhits; ++ih) {
    CbmMuchStrawHit* hit =
      (CbmMuchStrawHit*) fHits->UncheckedAt(vec->GetHitIndex(ih));
    //if (hit->GetFlag() > 1) { cout << " hit " << hit->GetFlag() << endl; /*exit(0);*/ }
    if (hit->GetFlag() % 2) {
      //if (0) {
      // Mirror hit
      id = -1;
      if (ids.find(id) == ids.end())
        ids.insert(pair<Int_t, Int_t>(id, 1));
      else
        ++ids[id];
    } else {
      CbmMuchDigiMatch* digiM =
        (CbmMuchDigiMatch*) fDigiMatches->UncheckedAt(hit->GetRefId());
      Int_t np = digiM->GetNofLinks();
      for (Int_t ip = 0; ip < np; ++ip) {
        //CbmMuchPoint* point = (CbmMuchPoint*) fPoints->UncheckedAt(digiM->GetLink(0).GetIndex());
        CbmMuchPoint* point =
          (CbmMuchPoint*) fPoints->UncheckedAt(digiM->GetLink(ip).GetIndex());
        id = point->GetTrackID();
        //if (np > 1) cout << ip << " " << id << endl;
        if (ids.find(id) == ids.end())
          ids.insert(pair<Int_t, Int_t>(id, 1));
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
  //vec->SetFlag(idmax);
  if (maxim <= nthr || idmax < 0) return kFALSE;

  if (idmax > 1) return kFALSE;  // !!! look only at muons from Pluto

  fhNhitsOk[ista]->Fill(vec->GetNofHits());
  fhIds[ista]->Fill(idmax);
  Int_t evID = FairRunAna::Instance()->GetEventHeader()->GetMCEntryNumber() + 1;
  fhIdVsEv[ista]->Fill(evID, idmax);

  // Fill true track parameters
  Double_t zp = 0.0, parsTr[4] = {0.0};
  Int_t ok = 0;
  for (Int_t ih = 0; ih < nhits; ++ih) {
    CbmMuchStrawHit* hit =
      (CbmMuchStrawHit*) fHits->UncheckedAt(vec->GetHitIndex(ih));
    CbmMuchDigiMatch* digiM =
      (CbmMuchDigiMatch*) fDigiMatches->UncheckedAt(hit->GetRefId());
    Int_t np = digiM->GetNofLinks();
    for (Int_t ip = 0; ip < np; ++ip) {
      CbmMuchPoint* point =
        (CbmMuchPoint*) fPoints->UncheckedAt(digiM->GetLink(ip).GetIndex());
      id = point->GetTrackID();
      if (id != idmax) continue;
      parsTr[0] = (point->GetXIn() + point->GetXOut()) / 2;
      parsTr[1] = (point->GetYIn() + point->GetYOut()) / 2;
      zp        = (point->GetZIn() + point->GetZOut()) / 2;
      parsTr[2] = point->GetPx() / point->GetPz();
      parsTr[3] = point->GetPy() / point->GetPz();
      ok        = 1;
      break;
    }
    if (ok) break;
  }
  // Adjust track coordinates
  const FairTrackParam* params = vec->GetParamFirst();
  parsTr[0] += parsTr[2] * (params->GetZ() - zp);
  parsTr[1] += parsTr[3] * (params->GetZ() - zp);
  fhDx[ista]->Fill(params->GetX() - parsTr[0]);
  fhDy[ista]->Fill(params->GetY() - parsTr[1]);
  fhDtx[ista]->Fill(params->GetTx() - parsTr[2]);
  fhDty[ista]->Fill(params->GetTy() - parsTr[3]);
  fhDtxOk[ista]->Fill(params->GetTx() - params->GetX() / params->GetZ());
  fhDtyOk[ista]->Fill(params->GetTy() - params->GetY() / params->GetZ());
  cout << " Good: " << idmax << " " << zp << " " << params->GetZ() << " "
       << parsTr[0] << " " << params->GetX() << " " << parsTr[1] << " "
       << params->GetY() << " " << parsTr[2] << " " << params->GetTx() << " "
       << parsTr[3] << " " << params->GetTy() << " " << vec->GetChiSq() << endl;

  // Fill tube difference between doublets
  map<Int_t, Int_t> tubes[2];
  Int_t id0 = -1;
  // Check configuration
  Int_t combi = 0;
  if (fNdoubl[fNstat - 1] == 3)
    combi = 3;  // combined stations for 3-layer config.

  for (Int_t ih = 0; ih < nhits; ++ih) {
    CbmMuchStrawHit* hit =
      (CbmMuchStrawHit*) fHits->UncheckedAt(vec->GetHitIndex(ih));
    CbmMuchDigiMatch* digiM =
      (CbmMuchDigiMatch*) fDigiMatches->UncheckedAt(hit->GetRefId());
    Int_t np = digiM->GetNofLinks();
    for (Int_t ip = 0; ip < np; ++ip) {
      CbmMuchPoint* point =
        (CbmMuchPoint*) fPoints->UncheckedAt(digiM->GetLink(ip).GetIndex());
      id = point->GetTrackID();
      if (id > 1) continue;  // look only at muons from vector mesons
      if (id0 < 0) id0 = id;
      break;
    }
    if (id0 < 0 || id != id0) continue;
    Int_t stat = fGeoScheme->GetStationIndex(hit->GetAddress()) - fStatFirst;
    Int_t lay  = fGeoScheme->GetLayerIndex(hit->GetAddress());
    tubes[id].insert(pair<Int_t, Int_t>(lay + stat * combi, hit->GetTube()));
  }
  Double_t slope[2] = {0.08, 0.06};
  for (Int_t j = 0; j < 2; ++j) {
    // Loop over trackID
    Int_t nd = tubes[j].size();
    if (nd < 2) continue;
    Int_t tubeNos[6]               = {1000, 1000, 1000, 1000, 1000, 1000};
    map<Int_t, Int_t>::iterator it = tubes[j].begin();
    tubeNos[it->first]             = it->second;
    ++it;
    for (; it != tubes[j].end(); ++it)
      tubeNos[it->first] = it->second;
    for (Int_t lay = 1; lay < 4; ++lay) {
      if (tubeNos[lay] < 1000 & tubeNos[lay - 1] < 1000)
        fhDtube[ista][lay - 1]->Fill(tubeNos[lay] - tubeNos[lay - 1]);
      if (combi == 0) {
        // 4-layer config.
        if (lay == 2 && tubeNos[lay] < 1000 & tubeNos[lay - 2] < 1000)
          fhDtube2[ista][lay - 2]->Fill(tubeNos[lay - 2],
                                        tubeNos[lay] - tubeNos[lay - 2]);
        if (lay == 3 && tubeNos[lay] < 1000 & tubeNos[lay - 2] < 1000) {
          //fhDtube2[ista][lay-2]->Fill(tubeNos[lay-2],tubeNos[lay]-tubeNos[lay-2]-slope[ista]*tubeNos[lay-2]);
          Double_t dzz = (fZpos[ista][lay * 2] - fZpos[ista][lay * 2 - 4])
                         / fZpos[ista][lay * 2 - 4];
          fhDtube2[ista][lay - 2]->Fill(tubeNos[lay - 2],
                                        tubeNos[lay] - tubeNos[lay - 2]
                                          - dzz * tubeNos[lay - 2]);
        }
      } else {
        // 3-layer config.
        if (lay == 3 && tubeNos[lay] < 1000 & tubeNos[lay - 3] < 1000)
          fhDtube2[ista][lay - 3]->Fill(tubeNos[lay - 3],
                                        tubeNos[lay] - tubeNos[lay - 3]);
        if (lay == 1 && tubeNos[lay] < 1000 & tubeNos[lay + 3] < 1000) {
          //fhDtube2[ista][lay-2]->Fill(tubeNos[lay-2],tubeNos[lay]-tubeNos[lay-2]-slope[ista]*tubeNos[lay-2]);
          Double_t dzz = (fZpos[ista + 1][lay * 2] - fZpos[ista][lay * 2])
                         / fZpos[ista][lay * 2];
          fhDtube2[ista][lay]->Fill(
            tubeNos[lay], tubeNos[lay + 3] - tubeNos[lay] - dzz * tubeNos[lay]);
        }
      }
    }
    if (ok) break;
  }

  return kTRUE;
}
// -------------------------------------------------------------------------

// -----   Private method CheckMatchGem   ----------------------------------
Bool_t CbmMuchFindVectorsQA::CheckMatchGem(CbmMuchTrack* vec) {
  // Check matching quality of the vector for GEM stations

  Int_t ista  = vec->GetUniqueID();
  Int_t nhits = vec->GetNofHits(), nthr = nhits / 2, id = 0;
  //Int_t nhits = vec->GetNofHits(), nthr = nhits - 1, id = 0;
  map<Int_t, Int_t> ids;

  for (Int_t ih = 0; ih < nhits; ++ih) {
    CbmMuchPixelHit* hit =
      (CbmMuchPixelHit*) fHitsGem->UncheckedAt(vec->GetHitIndex(ih));
    CbmMuchCluster* clus =
      (CbmMuchCluster*) fClusters->UncheckedAt(hit->GetRefId());
    Int_t nDigis = clus->GetNofDigis();
    for (Int_t j = 0; j < nDigis; ++j) {
      CbmMuchDigiMatch* digiM =
        (CbmMuchDigiMatch*) fDigiMatchesGem->UncheckedAt(clus->GetDigi(j));
      Int_t np = digiM->GetNofLinks();
      for (Int_t ip = 0; ip < np; ++ip) {
        //CbmMuchPoint* point = (CbmMuchPoint*) fPoints->UncheckedAt(digiM->GetLink(0).GetIndex());
        CbmMuchPoint* point =
          (CbmMuchPoint*) fPoints->UncheckedAt(digiM->GetLink(ip).GetIndex());
        id = point->GetTrackID();
        //if (np > 1) cout << ip << " " << id << endl;
        ids[id]++;
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
  //cout << " GEMS: " << ista << " " << nhits << " " << maxim << " " << idmax << endl;
  // Set vector ID as its flag
  //vec->SetFlag(idmax);
  if (maxim <= nthr || idmax < 0) return kFALSE;

  if (idmax > 1) return kFALSE;  // !!! look only at muons from Pluto

  fhNhitsOk[ista]->Fill(vec->GetNofHits());
  fhIds[ista]->Fill(idmax);
  Int_t evID = FairRunAna::Instance()->GetEventHeader()->GetMCEntryNumber() + 1;
  fhIdVsEv[ista]->Fill(evID, idmax);

  // Fill true track parameters
  Double_t zp = 0.0, parsTr[4] = {0.0};
  Int_t ok = 0;

  for (Int_t ih = 0; ih < nhits; ++ih) {
    CbmMuchPixelHit* hit =
      (CbmMuchPixelHit*) fHitsGem->UncheckedAt(vec->GetHitIndex(ih));
    CbmMuchCluster* clus =
      (CbmMuchCluster*) fClusters->UncheckedAt(hit->GetRefId());
    Int_t nDigis = clus->GetNofDigis();
    for (Int_t j = 0; j < nDigis; ++j) {
      CbmMuchDigiMatch* digiM =
        (CbmMuchDigiMatch*) fDigiMatchesGem->UncheckedAt(clus->GetDigi(j));
      Int_t np = digiM->GetNofLinks();
      for (Int_t ip = 0; ip < np; ++ip) {
        CbmMuchPoint* point =
          (CbmMuchPoint*) fPoints->UncheckedAt(digiM->GetLink(ip).GetIndex());
        id = point->GetTrackID();
        if (id != idmax) continue;
        parsTr[0] = (point->GetXIn() + point->GetXOut()) / 2;
        parsTr[1] = (point->GetYIn() + point->GetYOut()) / 2;
        zp        = (point->GetZIn() + point->GetZOut()) / 2;
        parsTr[2] = point->GetPx() / point->GetPz();
        parsTr[3] = point->GetPy() / point->GetPz();
        ok        = 1;
        break;
      }
      if (ok) break;
    }
    if (ok) break;
  }
  // Adjust track coordinates
  const FairTrackParam* params = vec->GetParamFirst();
  parsTr[0] += parsTr[2] * (params->GetZ() - zp);
  parsTr[1] += parsTr[3] * (params->GetZ() - zp);
  fhDx[ista]->Fill(params->GetX() - parsTr[0]);
  fhDy[ista]->Fill(params->GetY() - parsTr[1]);
  fhDtx[ista]->Fill(params->GetTx() - parsTr[2]);
  fhDty[ista]->Fill(params->GetTy() - parsTr[3]);
  fhDtxOk[ista]->Fill(params->GetTx() - params->GetX() / params->GetZ());
  fhDtyOk[ista]->Fill(params->GetTy() - params->GetY() / params->GetZ());
  cout << " Good: " << idmax << " " << zp << " " << params->GetZ() << " "
       << parsTr[0] << " " << params->GetX() << " " << parsTr[1] << " "
       << params->GetY() << " " << parsTr[2] << " " << params->GetTx() << " "
       << parsTr[3] << " " << params->GetTy() << " " << vec->GetChiSq() << endl;

  return kTRUE;
}
// -------------------------------------------------------------------------

// -----   Private method CheckShorts   ------------------------------------
void CbmMuchFindVectorsQA::CheckShorts(TClonesArray* hitArray) {
  // Check short tracks

  Int_t nvec   = fVectors->GetEntriesFast();
  Int_t nhits0 = 4;
  for (Int_t iv = 0; iv < nvec; ++iv) {
    CbmMuchTrack* vec = (CbmMuchTrack*) fVectors->UncheckedAt(iv);
    Int_t nhits       = vec->GetNofHits();
    if (nhits != nhits0) continue;
    Int_t ista = vec->GetUniqueID();
    set<Int_t> overlap;
    multiset<Int_t> overlap1;

    for (Int_t iv1 = iv + 1; iv1 < nvec; ++iv1) {
      CbmMuchTrack* vec1 = (CbmMuchTrack*) fVectors->UncheckedAt(iv1);
      if (vec1->GetUniqueID() != ista) continue;
      Int_t nhits1 = vec1->GetNofHits();

      // Compare hits
      Int_t ih1 = 0;
      for (Int_t ih = 0; ih < nhits; ++ih) {
        //CbmMuchStrawHit *hit = (CbmMuchStrawHit*) hitArray->UncheckedAt(vec->GetHitIndex(ih));
        CbmHit* hit = (CbmHit*) hitArray->UncheckedAt(vec->GetHitIndex(ih));
        Int_t lay   = fGeoScheme->GetLayerIndex(hit->GetAddress());
        Int_t side  = fGeoScheme->GetLayerSideIndex(hit->GetAddress());
        Int_t plane = lay * 2 + side;

        for (; ih1 < nhits1; ++ih1) {
          //CbmMuchStrawHit *hit1 = (CbmMuchStrawHit*) hitArray->UncheckedAt(vec1->GetHitIndex(ih1));
          CbmHit* hit1 =
            (CbmHit*) hitArray->UncheckedAt(vec1->GetHitIndex(ih1));
          Int_t lay1   = fGeoScheme->GetLayerIndex(hit1->GetAddress());
          Int_t side1  = fGeoScheme->GetLayerSideIndex(hit1->GetAddress());
          Int_t plane1 = lay1 * 2 + side1;
          if (plane1 < plane) continue;
          if (plane1 > plane) break;
          if (hit == hit1) {
            overlap.insert(ih);
            overlap1.insert(ih);
          }
        }
      }
      if (overlap.size() == nhits0) {
        // All hits are shared with other tracks
        //break;
      }
    }  // for (Int_t iv1 = iv + 1;
    fhShort[ista]->Fill(overlap.size());
    if (overlap.size()) fhOverlap[ista]->Fill(overlap.size(), overlap1.size());
  }  // for (Int_t iv = 0; iv < nvec;
}
// -------------------------------------------------------------------------

// -----   Private method CheckEffic   -------------------------------------
void CbmMuchFindVectorsQA::CheckEffic() {
  // Check efficiency of the vector reco

  // Find "reconstructable" vectors - having points in all doublets (GEM or straw)
  // !!! Take only trackID = 0, 1 (from signal mesons)
  const Int_t nMu = 2;
  set<Int_t> doublets[20][nMu], singlets[20][nMu];
  Int_t nPoints = fPoints->GetEntriesFast();
  Double_t xp[7][20][nMu], yp[7][20][nMu];
  Bool_t lstraws = kFALSE;

  for (Int_t ip = 0; ip < nPoints; ++ip) {
    CbmMuchPoint* p = (CbmMuchPoint*) fPoints->UncheckedAt(ip);
    Int_t id        = p->GetTrackID();
    if (id > 1) continue;
    if (p->GetZ() < fZpos[0][0] - 2.0) continue;
    Int_t ista = fGeoScheme->GetStationIndex(p->GetDetectorId());
    Int_t lay  = fGeoScheme->GetLayerIndex(p->GetDetectorId());
    Int_t side = fGeoScheme->GetLayerSideIndex(p->GetDetectorId());

    doublets[ista][id].insert(lay);
    singlets[ista][id].insert(lay * 2 + side);
    xp[ista][lay * 2 + side][id] = (p->GetXIn() + p->GetXOut()) / 2;
    yp[ista][lay * 2 + side][id] = (p->GetYIn() + p->GetYOut()) / 2;

    if (!lstraws) {
      if (fGeoScheme->GetModuleByDetId(p->GetDetectorId())->GetDetectorType()
          == 2)
        lstraws = kTRUE;
    }
  }

  Int_t muons[7][nMu] = {{0}, {0}}, nMuVec = 0;
  for (Int_t ista = 0; ista < fNstat; ++ista) {
    if (doublets[ista][0].size() == fNdoubl[ista]) {
      fhSim->Fill(ista);
      muons[ista][0] = 1;
      ++nMuVec;
    }
    if (doublets[ista][1].size() == fNdoubl[ista]) {
      fhSim->Fill(ista);
      muons[ista][1] = 1;
      ++nMuVec;
    }
  }
  //if (muons[0][0] == 0 && muons[0][1] == 0 && muons[1][0] == 0 && muons[1][1] == 0) return;
  if (nMuVec == 0) return;

  // Fit to straight lines in 2 projections
  /*
  for (Int_t ista = 0; ista < fNstat; ++ista) {

    for (Int_t mu = 0; mu < 2; ++ mu) {
      if (muons[ista][mu] == 0) continue;

      fhZXY[0]->Reset();
      fhZXY[1]->Reset();
      for (Int_t plane = 0; plane < nLays2; ++plane) {
	if (singlets[ista][mu].find(plane) == singlets[ista][mu].end()) continue;
	fhZXY[0]->Fill(zPos[ista][plane]-zPos[ista][0]+5, xp[ista][plane][mu]);
	Int_t ib = fhZXY[1]->Fill(zPos[ista][plane]-zPos[ista][0]+5, yp[ista][plane][mu]);
	fhZXY[0]->SetBinError(ib, 0.1);
	fhZXY[1]->SetBinError(ib, 0.1);
      }
      for (Int_t jxy = 0; jxy < 2; ++jxy) {
	fhZXY[jxy]->Fit("pol1","Q0");
	TF1 *f = fhZXY[jxy]->GetFunction("pol1");
	fhMCFit[ista][jxy]->Fill(f->GetChisquare() / f->GetNDF());
      }
    }
  }
  */

  // Match reconstructed vectors
  Int_t nvec  = fVectors->GetEntriesFast();
  Int_t pluto = 1;  // !!! set to 1 to check exact matching for Pluto
  //Double_t errx = 0.22, erry = 1.41, chi2cut = 10.0; // 10 deg
  //Double_t errx = 0.2, erry = 1.9, chi2cut = 10.0; // 5 doublets
  Double_t errxS[6] = {0.33, 0.50, 0.2, 0.2, 1, 1};
  Double_t erryS[6] = {0.33, 0.50, 1.9, 1.9, 1, 1},
           chi2cut  = 10.0;  // 2 GEMS, 2 straws
  //Double_t errxS[6] = {0.33, 0.50, 0.03, 0.03, 1, 1};
  //Double_t erryS[6] = {0.33, 0.50, 0.26, 0.26, 1, 1}, chi2cut = 10.0; // 2 GEMS, 2 straws
  Double_t errxG[6]    = {0.33, 0.54, 1.0, 1.7, 1, 1};
  Double_t erryG[6]    = {0.33, 0.54, 1.0, 1.7, 1, 1};  // 4 GEMS
  Int_t nMatch[7][nMu] = {{0}, {0}}, combi = 0;
  Double_t *errx = errxG, *erry = erryG;
  if (lstraws) {
    errx = errxS;
    erry = erryS;
    if (fNdoubl[fNstat - 1] == 3)
      combi = 6;  // combined stations for 3-layer config.
  }

  for (Int_t iv = 0; iv < nvec; ++iv) {
    CbmMuchTrack* vec = (CbmMuchTrack*) fVectors->UncheckedAt(iv);
    Int_t nhits       = vec->GetNofHits();
    Int_t ista        = vec->GetUniqueID();
    if (muons[ista][0] == 0 && muons[ista][1] == 0) continue;
    Int_t id = vec->GetFlag();
    if (pluto && id > 1)
      continue;  // !!! this is only for Pluto sample - exact ID match!!!
    const FairTrackParam* params = vec->GetParamFirst();
    TClonesArray* hitArray       = (ista < fStatFirst) ? fHitsGem : fHits;

    Double_t chi2[nMu] = {0.0};
    for (Int_t mu = 0; mu < nMu; ++mu) {
      if (!muons[ista][mu]) {
        chi2[mu] = 999999.;
        continue;
      }
      Int_t nm     = 0;
      Double_t dx0 = 0, dy0 = 0;

      for (Int_t ih = 0; ih < nhits; ++ih) {
        //CbmMuchStrawHit *hit = (CbmMuchStrawHit*) fHits->UncheckedAt(vec->GetHitIndex(ih));
        CbmHit* hit = (CbmHit*) hitArray->UncheckedAt(vec->GetHitIndex(ih));
        Int_t stat =
          fGeoScheme->GetStationIndex(hit->GetAddress()) - fStatFirst;
        Int_t lay   = fGeoScheme->GetLayerIndex(hit->GetAddress());
        Int_t side  = fGeoScheme->GetLayerSideIndex(hit->GetAddress());
        Int_t plane = lay * 2 + side;
        plane += stat * combi;
        if (singlets[ista][mu].find(plane) == singlets[ista][mu].end())
          continue;
        Double_t x = params->GetX()
                     + params->GetTx() * (fZpos[ista][plane] - params->GetZ());
        Double_t y = params->GetY()
                     + params->GetTy() * (fZpos[ista][plane] - params->GetZ());
        Double_t dx = x - xp[ista][plane][mu];
        Double_t dy = y - yp[ista][plane][mu];
        chi2[mu] += dx * dx / errx[ista] / errx[ista];
        chi2[mu] += dy * dy / erry[ista] / erry[ista];
        ++nm;
        if (lay) {
          if (lay == 1) {
            fhDx12[ista]->Fill(dx0, dx);
            fhDy12[ista]->Fill(dy0, dy);
          } else if (lay == 2) {
            fhDx23[ista]->Fill(dx0, dx);
            fhDy23[ista]->Fill(dy0, dy);
          }
        }
        dx0 = dx;
        dy0 = dy;
        //cout << nm << " " << dx << " " << dy << endl;
      }
      nm *= 2;
      if (nm > 4) chi2[mu] /= (nm - 4);  // per NDF
    }
    if (pluto)
      fhChi2mat[ista]->Fill(
        chi2[id]);  // !!! this is only for Pluto sample - exact ID match!!!
    else
      fhChi2mat[ista]->Fill(TMath::Min(chi2[0], chi2[1]));
    if (chi2[0] < chi2cut) ++nMatch[ista][0];
    if (chi2[1] < chi2cut) ++nMatch[ista][1];
    cout << chi2[0] << " " << chi2[1] << endl;
  }  // for (Int_t iv = 0; iv < nvec;
  for (Int_t ista = 0; ista < fNstat; ++ista) {
    if (combi && ista == fNstat - 1) continue;  // 2 combined stations
    for (Int_t id = 0; id < nMu; ++id) {
      if (muons[ista][id]) {
        if (combi && ista == fNstat - 2 && muons[ista + 1][id] == 0)
          continue;  // 2 combined stations
        fhMatchMult[ista]->Fill(nMatch[ista][id]);
        if (nMatch[ista][id] == 0)
          cout << " !!! No match found !!! " << ista << " " << id << endl;
      }
    }
  }
}
// -------------------------------------------------------------------------

// -----   Public method Finish   ------------------------------------------
void CbmMuchFindVectorsQA::Finish() {
  for (Int_t ista = 0; ista < fNstat; ++ista) {
    if (fNdoubl[ista])
      fhOccup[ista]->Scale(1. / 2 / fNdoubl[ista] / 2 / fhEvents->GetEntries());
  }

  TDirectory* dir = (TDirectory*) gROOT->FindObjectAny("muchQA");
  gDirectory->mkdir("muchQA");
  gDirectory->cd("muchQA");
  dir->GetList()->Write();
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------

ClassImp(CbmMuchFindVectorsQA);
