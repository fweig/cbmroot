#include "CbmRichMCbmQa.h"

#include "TCanvas.h"
#include "TClonesArray.h"
#include "TEllipse.h"
#include "TF1.h"
#include "TGeoBBox.h"
#include "TGeoManager.h"
#include "TGeoNode.h"
#include "TH1.h"
#include "TH1D.h"
#include "TMarker.h"
#include "TStyle.h"
#include <TFile.h>


#include "CbmDrawHist.h"
#include "CbmGlobalTrack.h"
#include "CbmMCTrack.h"
#include "CbmMatchRecoToMC.h"
#include "CbmRichDraw.h"
#include "CbmRichGeoManager.h"
#include "CbmRichHit.h"
#include "CbmRichPoint.h"
#include "CbmRichRing.h"
#include "CbmTofHit.h"
#include "CbmTofPoint.h"
#include "CbmTrackMatchNew.h"
#include "CbmTrdTrack.h"
#include "FairMCPoint.h"
#include "FairTrackParam.h"

#include "CbmHistManager.h"
#include "CbmUtils.h"

#include <boost/assign/list_of.hpp>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;
using boost::assign::list_of;

CbmRichMCbmQa::CbmRichMCbmQa()
  : FairTask("CbmRichMCbmQa")
  , fHM(NULL)
  , fEventNum(0)
  , fOutputDir("")
  , fMCTracks(NULL)
  , fRichPoints(NULL)
  , fRichDigis(NULL)
  , fRichHits(NULL)
  , fRichRings(NULL)
  , fRichRingMatches(NULL)
  , fRefPlanePoints(NULL)
  , fGlobalTracks(NULL)
  , fTrdTracks(NULL)
  , fTofHits(NULL)
  , fTofPoints(NULL)
  , fTofHitMatches(NULL) {}

InitStatus CbmRichMCbmQa::Init() {
  cout << "CbmRichMCbmQa::Init" << endl;

  FairRootManager* ioman = FairRootManager::Instance();
  if (NULL == ioman) {
    Fatal("CbmRichMCbmQa::Init", "RootManager not instantised!");
  }

  fMCTracks = (TClonesArray*) ioman->GetObject("MCTrack");
  if (NULL == fMCTracks) { Fatal("CbmRichMCbmQa::Init", "No MC Tracks!"); }

  fRichPoints = (TClonesArray*) ioman->GetObject("RichPoint");
  if (NULL == fRichPoints) { Fatal("CbmRichMCbmQa::Init", "No Rich Points!"); }

  fRichDigis = (TClonesArray*) ioman->GetObject("RichDigi");
  if (NULL == fRichDigis) { Fatal("CbmRichMCbmQa::Init", "No Rich Digis!"); }

  fRichHits = (TClonesArray*) ioman->GetObject("RichHit");
  if (NULL == fRichHits) { Fatal("CbmRichMCbmQa::Init", "No RichHits!"); }

  fRichRings = (TClonesArray*) ioman->GetObject("RichRing");
  if (NULL == fRichRings) { Fatal("CbmRichMCbmQa::Init", "No RichRings!"); }

  fTofHits = (TClonesArray*) ioman->GetObject("TofHit");
  if (NULL == fTofHits) { Fatal("CbmRichMCbmQa::Init", "No TofHits!"); }

  fRichRingMatches = (TClonesArray*) ioman->GetObject("RichRingMatch");
  if (NULL == fRichRingMatches) {
    Fatal("CbmRichMCbmQa::Init", "No RichRingMatch array!");
  }

  fTofHitMatches = (TClonesArray*) ioman->GetObject("TofHitMatch");
  if (NULL == fTofHitMatches) {
    Fatal("CbmRichMCbmQa::Init", "No TofHitMatch array!");
  }

  fTofPoints = (TClonesArray*) ioman->GetObject("TofPoint");
  if (NULL == fTofPoints) { Fatal("CbmRichMCbmQa::Init", "No Tof Points!"); }

  fRefPlanePoints = (TClonesArray*) ioman->GetObject("RefPlanePoint");
  if (NULL == fRefPlanePoints) {
    Fatal("CbmRichMCbmQa::Init", "No RefPlanePoints!");
  }

  //    fGlobalTracks = (TClonesArray*) ioman->GetObject("GlobalTrack");
  //    if ( NULL == fGlobalTracks) { Fatal("CbmRichMCbmQa::Init","No Global Tracks!");}


  InitHistograms();

  return kSUCCESS;
}
/*
vector<Double_t> CbmRichMCbmQa::GetHistBins(Bool_t isX)
{
	set<Double_t> setBins;
	TGeoIterator geoIterator(gGeoManager->GetTopNode()->GetVolume());
	TGeoNode* curNode;
	TString pixelNameStr("pmt_pixel");
	geoIterator.Reset();
	Double_t halfPixelSize = 0.;
	while (curNode == geoIterator()) {
		TString nodeName(curNode->GetName());
		TString nodePath;
		if (TString(curNode->GetVolume()->GetName()).Contains(pixelNameStr)) {
			geoIterator.GetPath(nodePath);
			const TGeoMatrix* curMatrix = geoIterator.GetCurrentMatrix();
			const Double_t* curNodeTr = curMatrix->GetTranslation();
            const TGeoBBox* shape = (const TGeoBBox*)(curNode->GetVolume()->GetShape());
            Double_t lowEdge, upEdge;
            if (isX) {
            	halfPixelSize = shape->GetDX();
            	lowEdge = curNodeTr[0] - halfPixelSize;
            	upEdge = curNodeTr[0] + halfPixelSize;
            } else {
            	halfPixelSize = shape->GetDY();
            	lowEdge = curNodeTr[1] - halfPixelSize;
            	upEdge = curNodeTr[1] + halfPixelSize;
            }
            setBins.insert(lowEdge);
            setBins.insert(upEdge);
		}
	}

	vector<double> bins(setBins.begin(), setBins.end());
	sort (bins.begin(), bins.end());

	return bins;
}
 */

void CbmRichMCbmQa::InitHistograms() {
  cout << "CbmRichMCbmQa::InitHistograms" << endl;

  fHM = new CbmHistManager();


  //RICH
  fHM->Create1<TH1D>(
    "fh_nof_rich_points",
    "fh_nof_rich_points;Nof RICH Points per event;Yield (a.u.)",
    1000,
    -.5,
    999.5);
  fHM->Create1<TH1D>("fh_nof_rich_hits",
                     "fh_nof_rich_hits;Nof RICH hits per event;Yield (a.u.)",
                     100,
                     -.5,
                     99.5);
  fHM->Create1<TH1D>("fh_nof_rich_rings",
                     "fh_nof_rich_rings;Nof RICH rings;# per event",
                     20,
                     -0.5,
                     19.5);
  fHM->Create1<TH1D>("fh_rich_ring_radius",
                     "fh_rich_ring_radius;Ring radius [cm];Yield (a.u.)",
                     200,
                     1.,
                     8.);


  //    vector<Double_t> xbins = GetHistBins(true);
  //    vector<Double_t> ybins = GetHistBins(false);
  //    fHM->Add("fh_rich_hits_xy", new TH2D("fh_rich_hits_xy", "fh_rich_hits_xy;X [cm];Y [cm];Yield (a.u.)", xbins.size() - 1, &xbins[0], ybins.size() - 1, &ybins[0]));
  //	cout << "binsize: " << xbins.size()  << "bin Ende Anfang: " << &xbins[0] << endl;

  const Double_t xMin = -35.5;
  const Double_t xMax = 35.5;
  const Double_t yMin = -40.5;
  const Double_t yMax = 40.5;


  // RICH hists
  fHM->Create2<TH2D>("fh_rich_hits_xy",
                     "fh_rich_hits_xy;X [cm];Y [cm];Yield",
                     71,
                     xMin,
                     xMax,
                     81,
                     yMin,
                     yMax);
  fHM->Create2<TH2D>("fh_rich_points_xy",
                     "fh_rich_points_xy;X [cm];Y [cm];Yield (a.u.)",
                     250,
                     xMin,
                     xMax,
                     250,
                     yMin,
                     yMax);

  fHM->Create1<TH1D>(
    "fh_hits_per_ring", "fh_hits_per_ring;Hits per Ring;Yield", 18, 2.5, 20.5);
  fHM->Create1<TH1D>("fh_dR", "fh_dR;dR [cm];Yield (a.u.)", 80, -0.8, 0.8);
  fHM->Create1<TH1D>(
    "fh_photon_energy", "fh_photon_energy;Momentum [eV]", 100, 0., 10.);
  fHM->Create2<TH2D>("fh_radius_momentum",
                     "fh_radius_momentum; Momentum [GeV];Radius [cm]; Yield",
                     500,
                     0.,
                     5.,
                     500,
                     0.,
                     5.);
  fHM->Create2<TH2D>("fh_ring_center_xy",
                     "fh_ring_center_xy;X [cm];Y [cm];Yield (a.u.)",
                     100,
                     xMin,
                     xMax,
                     100,
                     yMin,
                     yMax);
  fHM->Create2<TH2D>("fh_nonphoton_pmt_points_xy",
                     "fh_nonphoton_pmt_points_xy;X [cm]; Y [cm];Yield",
                     250.,
                     xMin,
                     xMax,
                     250,
                     yMin,
                     yMax);
  fHM->Create2<TH2D>("fh_radius_beta",
                     "fh_radius_beta; beta; Radius [cm]; Yield",
                     400,
                     0.7,
                     1.1,
                     500,
                     0.,
                     5.);
  fHM->Create1<TH1D>(
    "fh_beta_dis_all", "fh_beta_dis_all;beta;Yield; ", 300, 0.7, 1.);
  fHM->Create1<TH1D>(
    "fh_beta_dis_ring", "fh_beta_dis_ring;beta;Yield;", 300, 0.7, 1.);

  //    fHM->Create1<TH1D>("fh_nof_trd_hits","fh_nof_trd_hits; Nof Trd Hits; Yield(a.u.)", 5., -0.5, 4.5);

  //    fHM->Create1<TH1D>("fh_nof_sts_hits","fh_nof_sts_hits; Nof Sts Hits; Yield(a.u.)", 5., -0.5, 4.5);

  // TOF hists
  fHM->Create2<TH2D>("fh_eloss_tof",
                     "fh_eloss_tof; time of flight [ns]; energy loss; Yield",
                     500,
                     0.,
                     5.,
                     500,
                     0.,
                     5.);
  fHM->Create2<TH2D>("fh_radius_mass2",
                     "fh_radius_mass2; mass^2 [GeV^2]; Radius [cm]; Yield",
                     450,
                     -0.3,
                     1.2,
                     500,
                     0.,
                     5.);

  fHM->Create1<TH1D>(
    "number_of_events", "number_of_events; something", 1, 0.5, 1.5);
}

void CbmRichMCbmQa::Exec(Option_t* /*option*/) {
  fEventNum++;
  fHM->H1("number_of_events")->Fill(1);

  cout << "CbmRichMCbmQa, event No. " << fEventNum << endl;


  //    Int_t nofMCTracks = fMCTracks->GetEntriesFast();
  Int_t nofRichPoints = fRichPoints->GetEntriesFast();
  //    Int_t nofRichDigis = fRichDigis->GetEntriesFast();
  Int_t nofRichHits  = fRichHits->GetEntriesFast();
  Int_t nofRichRings = fRichRings->GetEntriesFast();
  //    Int_t nofRefPlanePoints = fRefPlanePoints->GetEntriesFast();
  Int_t nofTofHits = fTofHits->GetEntriesFast();
  //    Int_t nofGlobalTracks = fGlobalTracks->GetEntriesFast();

  fHM->H1("fh_nof_rich_points")->Fill(nofRichPoints);
  fHM->H1("fh_nof_rich_hits")->Fill(nofRichHits);
  fHM->H1("fh_nof_rich_rings")->Fill(nofRichRings);

  for (int i = 0; i < nofRichHits; i++) {
    CbmRichHit* hit = static_cast<CbmRichHit*>(fRichHits->At(i));
    if (hit == NULL) continue;
    fHM->H2("fh_rich_hits_xy")->Fill(hit->GetX(), hit->GetY());
  }


  //--------------------TofHits properties -----------------------------------

  for (int iT = 0; iT < nofTofHits; iT++) {
    CbmTofHit* tofHit = static_cast<CbmTofHit*>(fTofHits->At(iT));
    if (NULL == tofHit) continue;
    CbmTrackMatchNew* tofHitMatch = (CbmTrackMatchNew*) fTofHitMatches->At(iT);
    if (NULL == tofHitMatch) continue;
    Int_t tofPointIndex = tofHitMatch->GetMatchedLink().GetIndex();
    const CbmTofPoint* tofPoint =
      static_cast<const CbmTofPoint*>(fTofPoints->At(tofPointIndex));
    if (NULL == tofPoint) continue;
    Int_t mcTrackIdTofHit = tofPoint->GetTrackID();
    if (mcTrackIdTofHit < 0) continue;
    CbmMCTrack* mcTrackTof = (CbmMCTrack*) fMCTracks->At(mcTrackIdTofHit);
    if (NULL == mcTrackTof) continue;


    TVector3 vec;
    mcTrackTof->GetMomentum(vec);
    Double_t momTotal    = sqrt(vec.Px() * vec.Px() + vec.Py() * vec.Py()
                             + vec.Pz() * vec.Pz());  // GeV
    Double_t time        = tofHit->GetTime();
    Double_t timect      = 0.2998 * time;  //time in ns, timect in m
    Double_t trackLength = tofHit->GetR() / 100;
    Double_t beta        = trackLength / timect;
    Double_t mass2       = TMath::Power(momTotal, 2.)
                     * (TMath::Power(1 / beta, 2) - 1);  //m² = p²*((1/beta)²-1)


    for (int i = 0; i < nofRichPoints; i++) {
      CbmRichPoint* point = static_cast<CbmRichPoint*>(fRichPoints->At(i));
      if (point == NULL) continue;

      Int_t mcTrackIdPoint = point->GetTrackID();
      if (mcTrackIdPoint < 0) continue;
      CbmMCTrack* mcTrack = (CbmMCTrack*) fMCTracks->At(mcTrackIdPoint);
      if (mcTrack == NULL) continue;

      //if ( mcTrack->GetPdgCode() != 50000050) continue; // select only Cherenkov photons

      if (mcTrack->GetPdgCode() != 50000050) {  // fill in non photons
        if (mcTrackIdPoint == mcTrackIdTofHit) {
          fHM->H1("fh_beta_dis_all")->Fill(beta);
          break;
        }
      }

      Int_t motherId =
        mcTrack->GetMotherId();  //fill in mothers of Cherenkov photons
      CbmMCTrack* mcTrackMother = (CbmMCTrack*) fMCTracks->At(motherId);
      if (mcTrackMother == NULL) continue;

      if (motherId == mcTrackIdTofHit) {
        fHM->H1("fh_beta_dis_all")->Fill(beta);
        // one entry per tof hit only
        break;
      }
    }

    for (int iRing = 0; iRing < nofRichRings; iRing++) {
      CbmRichRing* ring = static_cast<CbmRichRing*>(fRichRings->At(iRing));
      if (NULL == ring) continue;
      CbmTrackMatchNew* ringMatch =
        (CbmTrackMatchNew*) fRichRingMatches->At(iRing);
      if (NULL == ringMatch) continue;
      Int_t mcTrackIdRing = ringMatch->GetMatchedLink().GetIndex();
      if (mcTrackIdRing < 0) continue;

      CbmMCTrack* mcTrackRing = (CbmMCTrack*) fMCTracks->At(mcTrackIdRing);
      if (mcTrackRing == NULL) continue;
      Double_t radius = ring->GetRadius();

      if (mcTrackIdTofHit == mcTrackIdRing) {
        fHM->H2("fh_radius_mass2")->Fill(mass2, radius);
        fHM->H2("fh_radius_beta")->Fill(beta, radius);
        fHM->H1("fh_beta_dis_ring")->Fill(beta);
        fHM->H2("fh_radius_momentum")->Fill(momTotal, radius);
        // one entry per tof hit only
        break;
      }
    }
  }


  /*
    for(int i = 0; i < nofRichPoints; i++) {
        CbmRichPoint* point= static_cast<CbmRichPoint*>(fRichPoints->At(i));
        if (point == NULL) continue;

        Int_t mcTrackIdPoint = point->GetTrackID();
      
        if (mcTrackIdPoint == NULL) continue;
        CbmMCTrack* mcTrack = (CbmMCTrack*)fMCTracks->At(mcTrackIdPoint);
        if (mcTrack == NULL) continue;
       
        
        Int_t pdg = TMath::Abs(mcTrack->GetPdgCode());
       // cout << "pdg: " << pdg << endl;
        fHM->H2("fh_rich_points_xy")->Fill(point->GetX(), point->GetY());   
        
        
        for(int iT = 0; iT < nofTofHits; iT++){
            CbmTofHit* tofHit = static_cast<CbmTofHit*>(fTofHits->At(iT));
            if(NULL == tofHit) continue;
            CbmTrackMatchNew* tofHitMatch = (CbmTrackMatchNew*) fTofHitMatches->At(iT);
            if(NULL == tofHitMatch) continue;
            Int_t tofPointIndex = tofHitMatch->GetMatchedLink().GetIndex();
            const CbmTofPoint* tofPoint = static_cast<const CbmTofPoint*>(fTofPoints->At(tofPointIndex));
            if(NULL == tofPoint) continue;
            Int_t mcTrackIdTofHit = tofPoint->GetTrackID();
            if(mcTrackIdTofHit < 0) continue;
            CbmMCTrack* mcTrackTof = (CbmMCTrack*)fMCTracks->At(mcTrackIdTofHit);
            if (NULL == mcTrackTof) continue;

           
            Double_t time = tofHit->GetTime();
            Double_t timect = 0.2998*time;          //time in ns, timect in m
            Double_t trackLength = tofHit->GetR()/100;
            Double_t beta = trackLength/timect;
            
            
            if(mcTrackIdPoint == mcTrackIdTofHit){
                fHM->H1("fh_beta_dis_all")->Fill(beta);
            }
        }
     
        for(int iRing = 0; iRing < nofRichRings; iRing++){
            CbmRichRing* ring = static_cast<CbmRichRing*>(fRichRings->At(iRing));
            if (NULL == ring) continue;
            CbmTrackMatchNew* ringMatch = (CbmTrackMatchNew*) fRichRingMatches->At(iRing);
            if (NULL == ringMatch) continue;
            Int_t mcTrackIdRing = ringMatch->GetMatchedLink().GetIndex();
            if (mcTrackIdRing < 0) continue;
        
        
            CbmMCTrack* mcTrackRing = (CbmMCTrack*)fMCTracks->At(mcTrackIdRing);
            if(mcTrackRing == NULL) continue;
            
            if(mcTrackIdTofHit == mcTrackIdRing){
               // cout << "mcTrackIdTofHit: " << mcTrackIdTofHit << endl;
               // cout << "mcTrackIdRing: " << mcTrackIdRing << endl;
                Int_t pdg = TMath::Abs(mcTrackRing->GetPdgCode());
                if(pdg != 50000050){
                    fHM->H2("fh_radius_mass2")->Fill(mass2, radius);
                    fHM->H2("fh_radius_beta")->Fill(beta, radius);
                    fHM->H1("fh_beta_dis_ring")->Fill(beta);
                    fHM->H2("fh_radius_momentum")->Fill(momTotal, radius);
                    
                }
            }
        }              
    }           

*/


  //--------------------TofHits properties -----------------------------------
  /*
    for(int iRing = 0; iRing < nofRichRings; iRing++){
        CbmRichRing* ring = static_cast<CbmRichRing*>(fRichRings->At(iRing));
        if (NULL == ring) continue;
        CbmTrackMatchNew* ringMatch = (CbmTrackMatchNew*) fRichRingMatches->At(iRing);
        if (NULL == ringMatch) continue;
        Int_t mcTrackIdRing = ringMatch->GetMatchedLink().GetIndex();
        if (mcTrackIdRing < 0) continue;
        
        
        CbmMCTrack* mcTrackRing = (CbmMCTrack*)fMCTracks->At(mcTrackIdRing);
        if(mcTrackRing == NULL) continue;
        Double_t radius = ring->GetRadius();
        
        Int_t motherId = mcTrackRing->GetMotherId();
       // Int_t pdg = TMath::Abs(mcTrackRing->GetPdgCode());
       // cout << "pdg" << pdg << endl;
        
      
        int nofHits = ring->GetNofHits();
        TVector3 vec1;
        mcTrackRing->GetMomentum(vec1);
        Double_t momTotal1 = sqrt(vec1.Px()*vec1.Px() + vec1.Py()*vec1.Py() + vec1.Pz()*vec1.Pz()); // GeV

        Double_t cX = ring->GetCenterX();
        Double_t cY = ring->GetCenterY();
        fHM->H2("fh_ring_center_xy")->Fill(cX,cY);
        fHM->H1("fh_hits_per_ring")->Fill(nofHits);
        fHM->H1("fh_rich_ring_radius")->Fill(radius);
        //fHM->H2("fh_radius_momentum")->Fill(momTotal1, radius);
        cout << "richRingId: " << mcTrackIdRing << endl;
        
        for (int iH = 0; iH < nofHits; iH++) {
            Int_t hitInd = ring->GetHit(iH);
            CbmRichHit* hit = (CbmRichHit*) fRichHits->At(hitInd);
            if (NULL == hit) continue;
            Double_t hitX = hit->GetX();
            Double_t hitY = hit->GetY();

            Double_t dR = radius - TMath::Sqrt( (cX - hitX)*( cX - hitX) + (cY - hitY)*(cY - hitY) );
            fHM->H1("fh_dR")->Fill(dR);
        }
     

        for(int iT = 0; iT < nofTofHits; iT++){
            
            CbmTofHit* tofHit = static_cast<CbmTofHit*>(fTofHits->At(iT));
            if(NULL == tofHit) continue;
            CbmTrackMatchNew* tofHitMatch = (CbmTrackMatchNew*) fTofHitMatches->At(iT);
            if(NULL == tofHitMatch) continue;
            Int_t tofPointIndex = tofHitMatch->GetMatchedLink().GetIndex();
            const CbmTofPoint* tofPoint = static_cast<const CbmTofPoint*>(fTofPoints->At(tofPointIndex));
            if(NULL == tofPoint) continue;
            Int_t mcTrackIdTofHit = tofPoint->GetTrackID();
            if(mcTrackIdTofHit < 0) continue;
            CbmMCTrack* mcTrackTof = (CbmMCTrack*)fMCTracks->At(mcTrackIdTofHit);
            if (NULL == mcTrackTof) continue;

            TVector3 vec;
            mcTrackTof->GetMomentum(vec);
            Double_t momTotal = sqrt(vec.Px()*vec.Px() + vec.Py()*vec.Py() + vec.Pz()*vec.Pz()); // GeV
            Double_t time = tofHit->GetTime();
            Double_t timect = 0.2998*time;          //time in ns, timect in m
            Double_t trackLength = tofHit->GetR()/100;
            Double_t beta = trackLength/timect;
           
            Double_t mass2 = TMath::Power(momTotal, 2.) * (TMath::Power(1/beta, 2) - 1);     //m² = p²*((1/beta)²-1)

            
            if(mcTrackIdTofHit == mcTrackIdRing){
                //cout << "mcTrackIdTofHit: " << mcTrackIdTofHit << endl;
                //cout << "mcTrackIdRing: " << mcTrackIdRing << endl;
                Int_t pdg = TMath::Abs(mcTrackRing->GetPdgCode());
                if(pdg != 50000050){
                  //  fHM->H2("fh_radius_mass2")->Fill(mass2, radius);
                  //  fHM->H2("fh_radius_beta")->Fill(beta, radius);
                  //  fHM->H1("fh_beta_dis_ring")->Fill(beta);
                  //  fHM->H2("fh_radius_momentum")->Fill(momTotal, radius);
                    
                }
            }
        }    
    }
    */
}

/*
            if (ring) {
	            DrawEvent();
                    cout << "DrawEvent() called" << endl;
	        }
         */
void CbmRichMCbmQa::DrawHist() {
  cout.precision(4);

  SetDefaultDrawStyle();
  int numberOfEvents = fHM->H1("number_of_events")->GetEntries();
  //   fHM->ScaleByPattern("fh_.*", 1./fEventNum);
  fHM->ScaleByPattern("fh_.*", 1. / numberOfEvents);

  {
    fHM->CreateCanvas("number_of_events", "number_of_events", 600, 600);
    DrawH1(fHM->H1("number_of_events"));
  }

  {
    fHM->CreateCanvas(
      "richsp_radius_momentum", "richsp_radius_momentum", 1200, 600);
    DrawH2(fHM->H2("fh_radius_momentum"));
  }

  {
    fHM->CreateCanvas("richsp_radius_mass2", "richsp_radius_mass2", 1200, 600);
    DrawH2(fHM->H2("fh_radius_mass2"));
  }

  {
    fHM->CreateCanvas("richsp_radius_beta", "richsp_radius_beta", 1200, 600);
    DrawH2(fHM->H2("fh_radius_beta"));
  }

  {
    fHM->CreateCanvas("richsp_beta_dis_all", "richsp_beta_dis_all", 1200, 600);
    DrawH1(fHM->H1("fh_beta_dis_all"));
  }

  {
    fHM->CreateCanvas(
      "richsp_beta_dis_ring", "richsp_beta_dis_ring", 1200, 600);
    DrawH1(fHM->H1("fh_beta_dis_ring"));
  }

  {
    fHM->CreateCanvas("fh_nof_rich_rings", "fh_nof_rich_rings", 1200, 600);
    DrawH1(fHM->H1("fh_nof_rich_rings"));
  }


  {
    fHM->CreateCanvas("richsp_eloss_tof", "richsp_eloss_tof", 1200, 600);
    DrawH2(fHM->H2("fh_eloss_tof"));
  }

  {
    TCanvas* c = fHM->CreateCanvas(
      "richsp_nof_rich_hits_points", "richsp_nof_rich_hits_points", 1800, 600);
    c->Divide(3, 1);
    c->cd(1);
    DrawH1(fHM->H1("fh_nof_rich_points"));
    c->cd(2);
    DrawH1(fHM->H1("fh_nof_rich_hits"));
    c->cd(3);
    DrawH1(fHM->H1("fh_hits_per_ring"));
  }

  {
    TCanvas* c = fHM->CreateCanvas(
      "richsp_rich_points_hits_xy", "richsp_rich_points_hits_xy", 1200, 600);
    c->Divide(2, 1);
    c->cd(1);
    DrawH2(fHM->H2("fh_rich_points_xy"));
    c->cd(2);
    DrawH2(fHM->H2("fh_rich_hits_xy"));
  }

  {
    fHM->CreateCanvas(
      "richsp_rich_ring_radius", "richsp_rich_ring_radius", 600, 600);
    DrawH1(fHM->H1("fh_rich_ring_radius"));
    fHM->H1("fh_rich_ring_radius")->SetTitle("Ring Radius");
  }


  {
    fHM->CreateCanvas("richsp_dR", "richsp_dR", 600, 600);
    DrawH1andFitGauss(fHM->H1("fh_dR"), true, false);
    fHM->H1("fh_dR")->SetTitle("dR");
  }

  {
    TCanvas* c = fHM->CreateCanvas(
      "richsp_ring_center_xy", "richsp_ring_center_xy", 600, 600);
    c->SetLogz();
    DrawH2(fHM->H2("fh_ring_center_xy"));
    fHM->H1("fh_ring_center_xy")->SetTitle("Ring center");
  }

  {
    TCanvas* c = fHM->CreateCanvas("richsp_nonphoton_pmt_points_xy",
                                   "richsp_nonphoton_pmt_points_xy",
                                   600,
                                   600);
    c->SetLogz();
    DrawH2(fHM->H2("fh_nonphoton_pmt_points_xy"));
    fHM->H2("fh_nonphoton_pmt_points_xy")->SetTitle("Non photons on PMT plane");
  }
}

void CbmRichMCbmQa::DrawEvent() {
  cout << "CbmRichMCbmQa::DrawEvent" << endl;

  stringstream ss;
  ss << "richsp_event_display_event_" << fEventNum;
  fHM->CreateCanvas(ss.str().c_str(), ss.str().c_str(), 705, 800);

  TH2D* pad =
    new TH2D("event_display_pad", ";X [cm];Y [cm]", 1, -65., 5., 1, -40., 40.);

  DrawH2(pad);
  pad->GetYaxis()->SetTitleOffset(0.9);
  gPad->SetLeftMargin(0.13);
  gPad->SetRightMargin(0.05);

  // Draw hits
  int nofHits = fRichHits->GetEntriesFast();
  for (int iH = 0; iH < nofHits; iH++) {
    CbmRichHit* hit = (CbmRichHit*) fRichHits->At(iH);
    if (NULL == hit) continue;
    TEllipse* hitDr = new TEllipse(hit->GetX(), hit->GetY(), 0.25);
    hitDr->SetFillColor(kRed);
    hitDr->SetLineColor(kRed);
    hitDr->Draw();
  }

  // Draw RICH MC Points
  int nofPoints = fRichPoints->GetEntriesFast();
  for (int iP = 0; iP < nofPoints; iP++) {
    CbmRichPoint* point = (CbmRichPoint*) fRichPoints->At(iP);
    if (NULL == point) continue;
    TEllipse* pointDr = new TEllipse(point->GetX(), point->GetY(), 0.15);
    pointDr->SetFillColor(kBlue);
    pointDr->SetLineColor(kBlue);
    pointDr->Draw();
  }

  /*    //Draw proton start XY
    for( int i = 0; i < fMCTracks->GetEntriesFast(); i++) {
        CbmMCTrack* mctrack= static_cast<CbmMCTrack*>(fMCTracks->At(i));
        if (mctrack == NULL) continue;
        Double_t pdg = mctrack->GetPdgCode();
        Double_t motherId = mctrack->GetMotherId();
        if (pdg == 2212 && motherId < 0) {
            TEllipse* pointDr = new TEllipse(mctrack->GetStartX(), mctrack->GetStartY(), 0.35);
            pointDr->SetFillColor(kGreen+3);
            pointDr->SetLineColor(kGreen+3);
            pointDr->Draw();

        }
    }
     */

  // Draw rings
  int nofRings = fRichRings->GetEntriesFast();
  for (int iR = 0; iR < nofRings; iR++) {
    CbmRichRing* ring = (CbmRichRing*) fRichRings->At(iR);
    if (NULL == ring) continue;
    DrawCircle(ring);
  }
}

void CbmRichMCbmQa::DrawCircle(CbmRichRing* ring) {
  TEllipse* circle =
    new TEllipse(ring->GetCenterX(), ring->GetCenterY(), ring->GetRadius());
  circle->SetFillStyle(0);
  circle->SetLineWidth(4);
  circle->SetLineColor(kBlack);
  circle->Draw();

  TEllipse* center = new TEllipse(ring->GetCenterX(), ring->GetCenterY(), 0.2);
  center->SetFillColor(kBlack);
  center->SetLineColor(kBlack);
  center->Draw();
}

void CbmRichMCbmQa::Finish() {
  //DrawHist();
  //fHM->SaveCanvasToImage(fOutputDir);
  fHM->WriteToFile();
}


void CbmRichMCbmQa::DrawFromFile(const string& fileName,
                                 const string& outputDir) {
  fOutputDir = outputDir;

  if (fHM != nullptr) delete fHM;

  fHM         = new CbmHistManager();
  TFile* file = new TFile(fileName.c_str());
  fHM->ReadFromFile(file);

  DrawHist();

  fHM->SaveCanvasToImage(fOutputDir);
}


ClassImp(CbmRichMCbmQa)
