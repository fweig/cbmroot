/** CbmMuchMergeVectorsQA.cxx
 *@author A.Zinchenko <Alexander.Zinchenko@jinr.ru>
 *@since 2014
 **/
#include "CbmMuchMergeVectorsQA.h"
//#include "CbmMuchFindVectors.h"
#include "CbmMuchDigiMatch.h"
#include "CbmMuchFindHitsStraws.h"
#include "CbmMuchModule.h"
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

using std::set;
using std::cout;
using std::endl;

// -----   Default (stabdard) constructor   --------------------------------
CbmMuchMergeVectorsQA::CbmMuchMergeVectorsQA()
  : FairTask("MuchMergeVectorsQA"),
    fGeoScheme(CbmMuchGeoScheme::Instance()),
    fStatFirst(-1),
    fNstat(0)
{
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
CbmMuchMergeVectorsQA::~CbmMuchMergeVectorsQA()
{
}
// -------------------------------------------------------------------------

// -----   Public method Init (abstract in base class)  --------------------
InitStatus CbmMuchMergeVectorsQA::Init()
{

  // Get and check FairRootManager
  FairRootManager* ioman = FairRootManager::Instance();
  if (ioman == NULL) Fatal("Init", "RootManager not instantiated!");

  //CbmMuchFindHitsStraws *hitFinder = (CbmMuchFindHitsStraws*) 
  //FairRun::Instance()->GetTask("CbmMuchFindHitsStraws");
  //if (hitFinder == NULL) Fatal("CbmMuchFindTracks::Init", "CbmMuchFindHitsStraws not run!");

  fTracks      = static_cast<TClonesArray*> (ioman->GetObject("MuchVectorTrack"));
  fVectors     = static_cast<TClonesArray*> (ioman->GetObject("MuchVector"));
  fMCTracks    = static_cast<TClonesArray*> (ioman->GetObject("MCTrack"));
  fPoints      = static_cast<TClonesArray*> (ioman->GetObject("MuchPoint"));
  fHits        = static_cast<TClonesArray*> (ioman->GetObject("MuchStrawHit"));
  fHitsGem     = static_cast<TClonesArray*> (ioman->GetObject("MuchPixelHit"));
  fDigis       = static_cast<TClonesArray*> (ioman->GetObject("MuchStrawDigi"));
  fDigisGem    = static_cast<TClonesArray*> (ioman->GetObject("MuchDigi"));
  fDigiMatches = static_cast<TClonesArray*> (ioman->GetObject("MuchStrawDigiMatch"));
  fDigiMatchesGem = static_cast<TClonesArray*> (ioman->GetObject("MuchDigiMatch"));
  fClusters    = static_cast<TClonesArray*> (ioman->GetObject("MuchCluster"));

  // Get straw system configuration
  Int_t nSt = fGeoScheme->GetNStations();

  for (Int_t i = 0; i < nSt; ++i) {
    CbmMuchStation* st = fGeoScheme->GetStation(i);
    CbmMuchModule* mod = fGeoScheme->GetModule(i, 0, 0, 0);
    Int_t stat = CbmMuchAddress::GetStationIndex(st->GetDetectorId()); 
    if (mod->GetDetectorType() == 2 && fStatFirst < 0) fStatFirst = stat;
    ++fNstat;
    Int_t nLays = st->GetNLayers();
    fNdoubl[stat] = nLays;

    for (Int_t lay = 0; lay < nLays; ++lay) {
      CbmMuchLayer* layer = st->GetLayer(lay);

      for (Int_t iside = 0; iside < 2; ++iside) {
	CbmMuchLayerSide* side = layer->GetSide(iside);
	Int_t plane = lay * 2 + iside;
	fZpos[stat][plane] = side->GetZ();
      }
    }
  }
  
  if (fStatFirst < 0) fStatFirst = 99; // all GEM stations
 
  // Create directory for histograms
  TDirectory *dir0 = (TDirectory*) gROOT->FindObjectAny("muchQA");
  TDirectory *dir = new TDirectory("muchQA1","","",dir0);
  dir->cd();

  BookHistos();

  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----  SetParContainers -------------------------------------------------
void CbmMuchMergeVectorsQA::SetParContainers()
{
}
// -------------------------------------------------------------------------

// -----   Private method BookHistos   -------------------------------------
void CbmMuchMergeVectorsQA::BookHistos()
{
  // Book histograms

  fhChi2mat   = new TH1D*[fNstat+2];

  for (Int_t ist = 0; ist < fNstat; ++ist) {
    //Int_t stat = fStatFirst + ist;
    Int_t stat = 1 + ist;
    fhChi2mat[ist] = new TH1D(Form("hChi2mat%i",stat),Form("Chi2 of matching in station %i",stat),100,0,50);
  }
  fhChi2mat[fNstat] = new TH1D("hChi2mat","Chi2 of matching in 2 stations",100,0,100);
  fhChi2mat[fNstat+1] = new TH1D("hChi2Abs","Chi2 of 2 vector matching",100,0,50);
  fhSim = new TH1D("hSim","Number of reconstructable muons",10,0,10);
  fhRec = new TH1D("hRec","Number of reconstructed muons",10,0,10);
  fhMatchMult = new TH1D("hMatchMult","Multiplicity of matching",10,0,10);
  fhMatchOver = new TH1D("hMatchOver","Matching overlaps",10,0,10);
  fhOverlap = new TH1D("hOverlap","Matching overlap flag",10,0,10);
  fhSimRec = new TH2D("hSimRec","Reco muons vs simulated",4,0,4,4,0,4);
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void CbmMuchMergeVectorsQA::Exec(
				 Option_t* opt)
{
  // Do all processing

  // Check efficiency
  CheckEffic();

}
// -------------------------------------------------------------------------

// -----   Private method CheckMatch   -------------------------------------
Bool_t CbmMuchMergeVectorsQA::CheckMatch(CbmMuchTrack *vec)
{
  // Check matching quality of the vector

  /*
  Int_t nhits = vec->GetNofHits(), nthr = nhits / 2, id = 0;
  //Int_t nhits = vec->GetNofHits(), nthr = nhits - 1, id = 0;
  Int_t ista = vec->GetPreviousTrackId();
  map<Int_t,Int_t> ids;

  for (Int_t ih = 0; ih < nhits; ++ih) {
    CbmMuchStrawHit *hit = (CbmMuchStrawHit*) fHits->UncheckedAt(vec->GetHitIndex(ih));
    //if (hit->GetFlag() > 1) { cout << " hit " << hit->GetFlag() << endl; }
    if (hit->GetFlag() % 2) {
    //if (0) {
      // Mirror hit
      id = -1; 
      if (ids.find(id) == ids.end()) ids.insert(pair<Int_t,Int_t>(id,1));
      else ++ids[id];
    } else {
      CbmMuchDigiMatch* digiM = (CbmMuchDigiMatch*) fDigiMatches->UncheckedAt(hit->GetRefId());
      Int_t np = digiM->GetNofLinks();
      for (Int_t ip = 0; ip < np; ++ip) {
	//CbmMuchPoint* point = (CbmMuchPoint*) fPoints->UncheckedAt(digiM->GetLink(0).GetIndex());
	CbmMuchPoint* point = (CbmMuchPoint*) fPoints->UncheckedAt(digiM->GetLink(ip).GetIndex());
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
  //vec->SetFlag(idmax);
  if (maxim <= nthr || idmax < 0) return kFALSE;

  if (idmax > 1) return kFALSE; // !!! look only at muons from Pluto

  fhNhitsOk[ista]->Fill(vec->GetNofHits());
  fhIds[ista]->Fill(idmax);
  Int_t evID = FairRunAna::Instance()->GetEventHeader()->GetMCEntryNumber() + 1;
  fhIdVsEv[ista]->Fill(evID,idmax);

  // Fill true track parameters
  Double_t zp = 0.0, parsTr[4] = {0.0};
  Int_t ok = 0;
  for (Int_t ih = 0; ih < nhits; ++ih) {
    CbmMuchStrawHit *hit = (CbmMuchStrawHit*) fHits->UncheckedAt(vec->GetHitIndex(ih));
    CbmMuchDigiMatch* digiM = (CbmMuchDigiMatch*) fDigiMatches->UncheckedAt(hit->GetRefId());
    Int_t np = digiM->GetNofLinks();
    for (Int_t ip = 0; ip < np; ++ip) {
      CbmMuchPoint* point = (CbmMuchPoint*) fPoints->UncheckedAt(digiM->GetLink(ip).GetIndex());
      id = point->GetTrackID();
      if (id != idmax) continue;
      parsTr[0] = (point->GetXIn() + point->GetXOut()) / 2;
      parsTr[1] = (point->GetYIn() + point->GetYOut()) / 2;
      zp = (point->GetZIn() + point->GetZOut()) / 2;
      parsTr[2] = point->GetPx() / point->GetPz();
      parsTr[3] = point->GetPy() / point->GetPz();
      ok = 1;
      break;
    }
    if (ok) break;
  }
  // Adjust track coordinates
  const FairTrackParam *params = vec->GetParamFirst();
  parsTr[0] += parsTr[2] * (params->GetZ() - zp);
  parsTr[1] += parsTr[3] * (params->GetZ() - zp);
  fhDx[ista]->Fill(params->GetX()-parsTr[0]);
  fhDy[ista]->Fill(params->GetY()-parsTr[1]);
  fhDtx[ista]->Fill(params->GetTx()-parsTr[2]);
  fhDty[ista]->Fill(params->GetTy()-parsTr[3]);
  fhDtxOk[ista]->Fill(params->GetTx()-params->GetX()/params->GetZ());
  fhDtyOk[ista]->Fill(params->GetTy()-params->GetY()/params->GetZ());
  cout << " Good: " << idmax << " " << zp << " " << params->GetZ() << " " << parsTr[0] << " " << params->GetX() << " " << parsTr[1] << " " << params->GetY() << " " << parsTr[2] << " " << params->GetTx() << " " << parsTr[3] << " " << params->GetTy() << " " << vec->GetChiSq() << endl;

  // Fill tube difference between doublets
  map<Int_t,Int_t> tubes[2];
  Int_t id0 = -1;
  for (Int_t ih = 0; ih < nhits; ++ih) {
    CbmMuchStrawHit *hit = (CbmMuchStrawHit*) fHits->UncheckedAt(vec->GetHitIndex(ih));
    CbmMuchDigiMatch* digiM = (CbmMuchDigiMatch*) fDigiMatches->UncheckedAt(hit->GetRefId());
    Int_t np = digiM->GetNofLinks();
    for (Int_t ip = 0; ip < np; ++ip) {
      CbmMuchPoint* point = (CbmMuchPoint*) fPoints->UncheckedAt(digiM->GetLink(ip).GetIndex());
      id = point->GetTrackID();
      if (id > 1) continue; // look only at muons from vector mesons
      if (id0 < 0) id0 = id;
      break;
    }
    if (id0 < 0 || id != id0) continue;
    Int_t lay = fGeoScheme->GetLayerIndex(hit->GetAddress());
    tubes[id].insert(pair<Int_t,Int_t>(lay,hit->GetTube()));
  }
  Double_t slope[2] = {0.08, 0.06};
  for (Int_t j = 0; j < 2; ++j) {
    // Loop over trackID
    Int_t nd = tubes[j].size();
    if (nd < 2) continue;
    Int_t tubeNos[4] = {1000, 1000, 1000, 1000};
    map<Int_t,Int_t>::iterator it = tubes[j].begin();
    tubeNos[it->first] = it->second;
    ++it;
    for ( ; it != tubes[j].end(); ++it) tubeNos[it->first] = it->second;
    for (Int_t lay = 1; lay < 4; ++lay) {
      if (tubeNos[lay] < 1000 & tubeNos[lay-1] < 1000) 
	fhDtube[ista][lay-1]->Fill(tubeNos[lay]-tubeNos[lay-1]);
      if (lay == 2 && tubeNos[lay] < 1000 & tubeNos[lay-2] < 1000) 
	fhDtube2[ista][lay-2]->Fill(tubeNos[lay-2],tubeNos[lay]-tubeNos[lay-2]);
      if (lay == 3 && tubeNos[lay] < 1000 & tubeNos[lay-2] < 1000) {
	//fhDtube2[ista][lay-2]->Fill(tubeNos[lay-2],tubeNos[lay]-tubeNos[lay-2]-slope[ista]*tubeNos[lay-2]);
	Double_t dzz = (fZpos[ista][lay*2]-fZpos[ista][lay*2-4]) / fZpos[ista][lay*2-4];
	fhDtube2[ista][lay-2]->Fill(tubeNos[lay-2],tubeNos[lay]-tubeNos[lay-2]-dzz*tubeNos[lay-2]);
      }
    }
    if (ok) break;
  }
  */
  
  return kTRUE;
}
// -------------------------------------------------------------------------

// -----   Private method CheckEffic   -------------------------------------
void CbmMuchMergeVectorsQA::CheckEffic()
{
  // Check efficiency of the vector reco

  // Find "reconstructable" vectors - having points in all straw doublets
  // !!! Take only trackID = 0, 1 (from signal mesons)
  const Int_t nMu = 2;
  set<Int_t> doublets[20][nMu], singlets[20][nMu];
  Int_t nPoints = fPoints->GetEntriesFast();
  Double_t xp[7][20][nMu], yp[7][20][nMu];
  Bool_t lstraws = kFALSE;

  for (Int_t ip = 0; ip < nPoints; ++ip) {
    CbmMuchPoint *p = (CbmMuchPoint*) fPoints->UncheckedAt(ip);
    Int_t id = p->GetTrackID();
    if (id > 1) continue;
    //if (p->GetZ() < fZpos[0][0] - 2.0) continue;
    Int_t ista = fGeoScheme->GetStationIndex(p->GetDetectorId()); // - fStatFirst;
    Int_t lay = fGeoScheme->GetLayerIndex(p->GetDetectorId());
    Int_t side = fGeoScheme->GetLayerSideIndex(p->GetDetectorId());

    doublets[ista][id].insert(lay);
    singlets[ista][id].insert(lay*2+side);
    xp[ista][lay*2+side][id] = (p->GetXIn() + p->GetXOut()) / 2;
    yp[ista][lay*2+side][id] = (p->GetYIn() + p->GetYOut()) / 2;

    if (!lstraws) {
      if (fGeoScheme->GetModuleByDetId(p->GetDetectorId())->GetDetectorType() == 2) lstraws = kTRUE;
    }
  }

  Int_t muons[7][nMu] = {{0},{0}}, nMuVec = 0, nMuRec = 0;
  for (Int_t ista = 0; ista < fNstat; ++ista) {
    if (doublets[ista][0].size() == fNdoubl[ista]) { muons[ista][0] = 1; ++nMuVec; }
    if (doublets[ista][1].size() == fNdoubl[ista]) { muons[ista][1] = 1; ++nMuVec; }
  }
  //if ((muons[0][0] == 0 || muons[1][0] == 0) && (muons[0][1] == 0 || muons[1][1] == 0)) return; 
  if (nMuVec == 0) return;
  nMuVec = 0;
  for (Int_t mu = 0; mu < nMu; ++mu) {
    Int_t simOk = 0;
    for (Int_t ista = 0; ista < fNstat; ++ista) simOk += muons[ista][mu];
    nMuVec += simOk / fNstat;
  }

  // Match merged vectors
  Int_t ntracks = fTracks->GetEntriesFast();
  Int_t pluto = 0; // !!! set to 1 to check exact matching for Pluto 
  Double_t errxS[6] = {0.33, 0.50, 0.2, 0.2, 1, 1};
  Double_t erryS[6] = {0.33, 0.50, 1.9, 1.9, 1, 1}, chi2cut = 10.0; // 2 GEMS, 2 straws
  //Double_t errxS[6] = {0.33, 0.50, 0.03, 0.03, 1, 1};
  //Double_t erryS[6] = {0.33, 0.50, 0.26, 0.26, 1, 1}, chi2cut = 10.0; // 2 GEMS, 2 straws
  Double_t errxG[6] = {0.33, 0.54, 1.0, 1.7, 1, 1};
  Double_t erryG[6] = {0.33, 0.54, 1.0, 1.7, 1, 1}; // 4 GEMS
  Double_t chi2tot = 0.0, chi2min = 0.0, chi2[10][nMu] = {{0.0},{0.0}};
  Int_t iMatch[nMu] = {0};
  Double_t *errx = errxG, *erry = erryG;
  if (lstraws) { errx = errxS; erry = erryS; }

  for (Int_t mu = 0; mu < nMu; ++mu) {
    iMatch[mu] = -1;
    //fhSim->Fill(muons[0][mu]+muons[1][mu]);
    //if (muons[0][mu] == 0 || muons[1][mu] == 0) continue; 
    fhSim->Fill(muons[2][mu]+muons[3][mu]); // stations 3 and 4
    if (muons[2][mu] == 0 || muons[3][mu] == 0) continue; 
    for (Int_t j = 0; j < 10; ++j) chi2[j][mu] = 999999.0;
    chi2tot = chi2min = 999999.0;
    Int_t nmatch = 0;

    for (Int_t itr = 0; itr < ntracks; ++itr) {
      CbmMuchTrack *track = (CbmMuchTrack*) fTracks->UncheckedAt(itr);
      Int_t nvecs = track->GetNofHits();
      if (nvecs < 5) continue;
      Double_t c2[10] = {0.0}, c2tot = 0.0;
 
      for (Int_t ivec = 0; ivec < nvecs; ++ivec) {
	if (track->GetHitType(ivec) != kMUCHSTRAWHIT) continue; // skip STS vector
	CbmMuchTrack *vec = (CbmMuchTrack*) fVectors->UncheckedAt(track->GetHitIndex(ivec));
	Int_t nhits = vec->GetNofHits();
	Int_t ista = vec->GetUniqueID();
	//if (ivec == 0 && ista < 2 || ivec > 0 && ista == 2) continue; // only 2 last stations 

	Int_t id = vec->GetFlag();
	if (pluto && id != mu) continue; // !!! this is only for Pluto sample - exact ID match!!!
	const FairTrackParam *params = vec->GetParamFirst();
	//cout << " Oops: " << ivec << " " << params->GetZ() << endl; 
      	Int_t nm = 0;
	TClonesArray *hitArray = (ista < fStatFirst) ? fHitsGem : fHits;
	
	for (Int_t ih = 0; ih < nhits; ++ih) {
	  //CbmMuchStrawHit *hit = (CbmMuchStrawHit*) fHits->UncheckedAt(vec->GetHitIndex(ih));
	  CbmHit *hit = (CbmHit*) hitArray->UncheckedAt(vec->GetHitIndex(ih));
	  Int_t lay = fGeoScheme->GetLayerIndex(hit->GetAddress());
	  Int_t side = fGeoScheme->GetLayerSideIndex(hit->GetAddress());
	  Int_t plane = lay * 2 + side;
	  if (singlets[ista][mu].find(plane) == singlets[ista][mu].end()) continue;
	  Double_t x = params->GetX() + params->GetTx() * (fZpos[ista][plane] - params->GetZ());
	  Double_t y = params->GetY() + params->GetTy() * (fZpos[ista][plane] - params->GetZ());
	  Double_t dx = x - xp[ista][plane][mu];
	  Double_t dy = y - yp[ista][plane][mu];
	  c2[ista] += dx * dx / errx[ista] / errx[ista];
	  c2[ista] += dy * dy / erry[ista] / erry[ista];
	  ++nm;
	  //cout << nm << " " << dx << " " << dy << endl;
	}
	nm *= 2;
	if (nm > 4) c2[ista] /= (nm - 4); // per NDF
	c2tot += c2[ista];
      } // for (Int_t ivec = 0; ivec < nvecs;
      //if (pluto) fhChi2mat[ista]->Fill(chi2[id]); // !!! this is only for Pluto sample - exact ID match!!!
      //if (c2[2] + c2[3] < chi2[2][mu] + chi2[3][mu]) {
      if (c2tot < chi2tot * 1.2 && track->GetChiSq() < chi2min || 
	  c2tot < chi2tot && track->GetChiSq() < chi2min * 1.2) {
	for (Int_t j = 0; j < 10; ++j) chi2[j][mu] = c2[j];
	chi2tot = c2tot;
	chi2min = track->GetChiSq();
	iMatch[mu] = itr;
      }
      //if (c2[0] < chi2cut && c2[1] < chi2cut && track->GetChiSq() < chi2cut * 2) ++nmatch;
      if (c2[0] < chi2cut && c2[1] < chi2cut && c2[2] < chi2cut && c2[3] < chi2cut && track->GetChiSq() < chi2cut * 4) ++nmatch;
      //cout << nvecs << " " << c2tot << endl;
    } // for (Int_t itr = 0; itr < ntracks;
    cout << " Nover " << ntracks << " " << nmatch << endl;
    if (iMatch[mu] < 0) continue;
    fhMatchOver->Fill(nmatch);
    nMuRec += TMath::Min(1,nmatch);

    for (Int_t ista = 0; ista < fNstat; ++ista) fhChi2mat[ista]->Fill(chi2[ista][mu]); 
    //fhChi2mat[fNstat]->Fill(chi2[2][mu]+chi2[3][mu]); 
    fhChi2mat[fNstat]->Fill(chi2tot); 
    CbmMuchTrack *track = (CbmMuchTrack*) fTracks->UncheckedAt(iMatch[mu]);
    fhChi2mat[fNstat+1]->Fill(track->GetChiSq());
    //if (track->GetChiSq() > 20) { cout << track->GetChiSq() << " " << chi2tot << endl; exit(0); }

    nmatch = 0;
    for (Int_t ista = 0; ista < fNstat; ++ista) 
      if (chi2[ista][mu] < chi2cut) ++nmatch; 
    fhMatchMult->Fill(nmatch);
  } // for (Int_t mu = 0; mu < nMu;
  if (iMatch[0] == iMatch[1] && iMatch[0] >= 0) fhOverlap->Fill(1); // the same track matches 2 muons
  fhSimRec->Fill(nMuVec,nMuRec);
}
// -------------------------------------------------------------------------

// -----   Public method Finish   ------------------------------------------
void CbmMuchMergeVectorsQA::Finish()
{

  TDirectory *dir0 = (TDirectory*) gROOT->FindObjectAny("muchQA");
  TDirectory *dir = (TDirectory*) dir0->FindObjectAny("muchQA1");
  gDirectory->mkdir("muchQA1");
  gDirectory->cd("muchQA1");
  dir->GetList()->Write();
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------

ClassImp(CbmMuchMergeVectorsQA);
