/* Copyright (C) 2015-2019 IKF-UFrac
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Samir Amar-Youcef [committer] */

// -------------------------------------------------------------------------
// -----              CbmMvdClusterAna  source file                   -----
// -----              Created 27/04/15  by S. Amar-Youcef             -----
// ------------------------------------------------------------------------

//-- Include from Cbm --//
#include "CbmMvdClusterAna.h"
// #include "CbmStsTrack.h"
#include "CbmMvdCluster.h"
#include "CbmMvdDigi.h"
#include "CbmMvdDigiMatch.h"
#include "CbmMvdHit.h"
#include "CbmMvdHitMatch.h"
#include "CbmMvdPoint.h"

// #include "CbmVertex.h"
#include "CbmLink.h"
#include "CbmMCTrack.h"
#include "CbmMatch.h"
#include "CbmTrackMatchNew.h"

// #include "base/CbmLitToolFactory.h"
// #include "data/CbmLitTrackParam.h"
// #include "utils/CbmLitConverter.h"


//-- Include from Fair --//
#include "FairTrackParam.h"
#include <Logger.h>


//-- Include from Root --//
#include "TCanvas.h"
#include "TF1.h"
#include "TGeoBBox.h"
#include "TGeoManager.h"
#include "TH1.h"
#include "TH2.h"
#include "TLegend.h"
#include "TMath.h"
#include "TProfile.h"
#include "TROOT.h"

//-- Include from C++ --//
#include <iomanip>
#include <iostream>
using namespace std;
using std::cout;
using std::endl;
using std::flush;

// -----   Default constructor   -------------------------------------------
CbmMvdClusterAna::CbmMvdClusterAna()
  : FairTask("MvdClusterAna")
  , fMcPoints(NULL)
  , fMvdDigis(NULL)
  , fMvdClusters(NULL)
  , fMvdHits(NULL)
  , fMvdDigisMatch(NULL)
  , fMvdClustersMatch(NULL)
  , fMvdHitsMatch(NULL)
  , fListMCTracks(NULL)
  , fStsTrackArray(NULL)
  , fStsTrackMatches(NULL)
  , fMvdHisto1()
  , fMvdHisto2()
  , fProf()
  , fNrMcPointsAll(-1)
  , fNrHitsAll(-1)
  , fMcperDigi()
  , fMcperHit()
  , fPixelpitch()
{
  ;
}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
CbmMvdClusterAna::CbmMvdClusterAna(const char* name, Int_t iVerbose)
  : FairTask(name, iVerbose)
  , fMcPoints(NULL)
  , fMvdDigis(NULL)
  , fMvdClusters(NULL)
  , fMvdHits(NULL)
  , fMvdDigisMatch(NULL)
  , fMvdClustersMatch(NULL)
  , fMvdHitsMatch(NULL)
  , fListMCTracks(NULL)
  , fStsTrackArray(NULL)
  , fStsTrackMatches(NULL)
  , fMvdHisto1()
  , fMvdHisto2()
  , fProf()
  , fNrMcPointsAll(-1)
  , fNrHitsAll(-1)
  , fMcperDigi()
  , fMcperHit()
  , fPixelpitch()
{
  ;
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmMvdClusterAna::~CbmMvdClusterAna() { ; }
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
InitStatus CbmMvdClusterAna::Init()
{
  cout << "--------------------------------------------------------------------"
          "-----"
       << endl
       << "-I- " << GetName() << "::Init: "
       << " Start Initilisation " << endl
       << "--------------------------------------------------------------------"
          "-----"
       << endl;

  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman) {
    cout << "-E- " << GetName() << "::Init: "
         << "RootManager not instantised!" << endl;
    return kFATAL;
  }

  gGeoManager = (TGeoManager*) gROOT->FindObject("FAIRGeom");

  fMcPoints    = (TClonesArray*) ioman->GetObject("MvdPoint");
  fMvdDigis    = (TClonesArray*) ioman->GetObject("MvdDigi");
  fMvdClusters = (TClonesArray*) ioman->GetObject("MvdCluster");
  fMvdHits     = (TClonesArray*) ioman->GetObject("MvdHit");

  fMvdDigisMatch    = (TClonesArray*) ioman->GetObject("MvdDigiMatch");
  fMvdClustersMatch = (TClonesArray*) ioman->GetObject("MvdClusterMatch");
  fMvdHitsMatch     = (TClonesArray*) ioman->GetObject("MvdHitMatch");

  fListMCTracks = (TClonesArray*) ioman->GetObject("MCTrack");
  // 	fStsTrackArray		= (TClonesArray*) ioman->GetObject("StsTrack");
  // 	fStsTrackMatches	= (TClonesArray*) ioman->GetObject("StsTrackMatch");

  // 	fPrimVtx         = (CbmVertex*) ioman->GetObject("PrimaryVertex");
  /*
  // Get pointer to PrimaryVertex object from IOManager if it exists
  // The old name for the object is "PrimaryVertex" the new one
  // "PrimaryVertex." Check first for the new name
  fPrimVtx = dynamic_cast<CbmVertex*>(ioman->GetObject("PrimaryVertex."));
  if (nullptr == fPrimVtx) {
    fPrimVtx = dynamic_cast<CbmVertex*>(ioman->GetObject("PrimaryVertex"));
  }
  if (nullptr == fPrimVtx) { 
    LOG(fatal) << "No PrimaryVertex array!";
  }
*/
  // 	fListMCTracks    = (TClonesArray*) ioman->GetObject("MCTrack");
  // 	fExtrapolator = CbmLitToolFactory::Instance()->CreateTrackExtrapolator("rk4");


  fMvdHisto1[0]  = new TH1F("Momentum", "Momentum", 100, 0, 30);
  fMvdHisto1[1]  = new TH1F("Angle", "Angle", 200, 0, 2);
  fMvdHisto1[2]  = new TH1F("DigisPerHit", "DigisPerHit", 100, 0, 100);
  fMvdHisto1[3]  = new TH1F("xResidual", "xResidual", 500, -50, 50);
  fMvdHisto1[4]  = new TH1F("yResidual", "yResidual", 500, -50, 50);
  fMvdHisto1[5]  = new TH1F("zResidual", "zResidual", 2000, -20, 20);
  fMvdHisto1[6]  = new TH1F("DistancePixX", "DistancePixX", 100, -2, 2);
  fMvdHisto1[7]  = new TH1F("DistancePixY", "DistancePixY", 100, -2, 2);
  fMvdHisto1[8]  = new TH1F("ClusterShape", "ClusterShape", 10, 0, 10);
  fMvdHisto1[9]  = new TH1F("ChargeSpectrum", "ChargeSpectrum", 50000, 0, 10000);
  fMvdHisto1[10] = new TH1F("res_x_shape_0", "res_x_shape_0", 500, -50, 50);
  fMvdHisto1[11] = new TH1F("res_x_shape_1", "res_x_shape_1", 500, -50, 50);
  fMvdHisto1[12] = new TH1F("res_x_shape_2", "res_x_shape_2", 500, -50, 50);
  fMvdHisto1[13] = new TH1F("res_x_shape_3", "res_x_shape_3", 500, -50, 50);
  fMvdHisto1[14] = new TH1F("res_x_shape_4", "res_x_shape_4", 500, -50, 50);
  fMvdHisto1[15] = new TH1F("res_x_shape_5", "res_x_shape_5", 500, -50, 50);
  fMvdHisto1[16] = new TH1F("res_x_shape_6", "res_x_shape_6", 500, -50, 50);
  fMvdHisto1[17] = new TH1F("res_x_shape_7", "res_x_shape_7", 500, -50, 50);
  fMvdHisto1[18] = new TH1F("res_x_shape_8", "res_x_shape_8", 500, -50, 50);
  fMvdHisto1[19] = new TH1F("res_x_shape_9", "res_x_shape_9", 500, -50, 50);
  fMvdHisto1[20] = new TH1F("res_y_shape_0", "res_y_shape_0", 500, -50, 50);
  fMvdHisto1[21] = new TH1F("res_y_shape_1", "res_y_shape_1", 500, -50, 50);
  fMvdHisto1[22] = new TH1F("res_y_shape_2", "res_y_shape_2", 500, -50, 50);
  fMvdHisto1[23] = new TH1F("res_y_shape_3", "res_y_shape_3", 500, -50, 50);
  fMvdHisto1[24] = new TH1F("res_y_shape_4", "res_y_shape_4", 500, -50, 50);
  fMvdHisto1[25] = new TH1F("res_y_shape_5", "res_y_shape_5", 500, -50, 50);
  fMvdHisto1[26] = new TH1F("res_y_shape_6", "res_y_shape_6", 500, -50, 50);
  fMvdHisto1[27] = new TH1F("res_y_shape_7", "res_y_shape_7", 500, -50, 50);
  fMvdHisto1[28] = new TH1F("res_y_shape_8", "res_y_shape_8", 500, -50, 50);
  fMvdHisto1[29] = new TH1F("res_y_shape_9", "res_y_shape_9", 500, -50, 50);
  fMvdHisto1[30] = new TH1F("DigisPerMC", "DigisPerMC", 100, 0, 100);
  fMvdHisto1[31] = new TH1F("HitsPerMC", "HitsPerMC", 11, 0, 11);
  fMvdHisto1[32] = new TH1F("McPerHit (merged)", "McPerHit (merged)", 11, 0, 11);
  fMvdHisto1[33] = new TH1F("xPull", "xPull", 500, -50, 50);
  fMvdHisto1[34] = new TH1F("yPull", "yPull", 500, -50, 50);
  fMvdHisto1[35] = new TH1F("McPerDigi", "McPerDigi", 6, 0, 6);
  fMvdHisto1[36] = new TH1F("McPerHit", "McPerHit", 11, 0, 11);

  fMvdHisto2[0]  = new TH2F("dxpdyp", "dxpdyp", 100, -1, 1, 100, -1, 1);
  fMvdHisto2[1]  = new TH2F("dpnorm", "dpnorm", 100, -1, 1, 100, -1, 1);
  fMvdHisto2[2]  = new TH2F("mom_dx", "mom_dx", 100, 0, 30, 100, -50, 50);
  fMvdHisto2[3]  = new TH2F("mom_dy", "mom_dy", 100, 0, 30, 100, -50, 50);
  fMvdHisto2[4]  = new TH2F("ang_dx", "ang_dx", 100, 0, 2, 100, -50, 50);
  fMvdHisto2[5]  = new TH2F("ang_dy", "ang_dy", 100, 0, 2, 100, -50, 50);
  fMvdHisto2[6]  = new TH2F("dnr_dx", "dnr_dx", 100, 0, 100, 100, -50, 50);
  fMvdHisto2[7]  = new TH2F("dnr_dy", "dnr_dy", 100, 0, 100, 100, -50, 50);
  fMvdHisto2[8]  = new TH2F("dxp_dx", "dxp_dx", 50, 0, 1, 100, -50, 50);
  fMvdHisto2[9]  = new TH2F("dxp_dy", "dxp_dy", 50, 0, 1, 100, -50, 50);
  fMvdHisto2[10] = new TH2F("cha_dx", "cha_dx", 100, 0, 10000, 100, -50, 50);
  fMvdHisto2[11] = new TH2F("cha_dy", "cha_dy", 100, 0, 10000, 100, -50, 50);

  fMvdHisto2[12] = new TH2F("mom_dz", "mom_dz", 100, 0, 30, 2000, -20, 20);
  fMvdHisto2[13] = new TH2F("ang_dz", "ang_dz", 200, 0, 2, 2000, -20, 20);
  fMvdHisto2[14] = new TH2F("dx_dy", "dx_dy", 100, -50, 50, 100, -50, 50);

  fMvdHisto2[15] = new TH2F("mom_cha", "mom_cha", 100, 0, 3, 1000, 0, 10000);
  fMvdHisto2[16] = new TH2F("ang_cha", "ang_cha", 200, 0, 2, 1000, 0, 10000);
  fMvdHisto2[17] = new TH2F("mom_chacut", "mom_chacut", 100, 0, 3, 1000, 0, 10000);
  cout << "--------------------------------------------------------------------"
          "-----"
       << endl
       << "-I- " << GetName() << "::Init: "
       << " Finished Initilisation " << endl
       << "--------------------------------------------------------------------"
          "-----"
       << endl;

  fNrMcPointsAll = 0;
  fNrHitsAll     = 0;

  for (Int_t i = 0; i < 6; i++) {
    fMcperDigi[i] = 0;
  }
  for (Int_t i = 0; i < 11; i++) {
    fMcperHit[i] = 0;
  }

  return kSUCCESS;
}
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
void CbmMvdClusterAna::Exec(Option_t* /*opt*/)
{
  Int_t nMcpoints = fMcPoints->GetEntriesFast();  // Number of Monte Carlo Points
  Int_t nDigis    = fMvdDigis->GetEntriesFast();  // Number of Mvd Digis
  // Int_t nClusters = fMvdClusters->GetEntriesFast();		// Number of reconstructed Mvd Clusters
  Int_t nHits = fMvdHits->GetEntriesFast();  // Number of reconstructed Mvd Hits

  // Int_t nDigisMatch		= fMvdDigisMatch	->GetEntriesFast();		// Number of Matches from Digis to Monte Carlo
  // Int_t nClustersMatch	= fMvdClustersMatch	->GetEntriesFast();		// Number of Matches from Reconstructed Mvd Clusters to Mvd Hits (?)
  // Int_t nHitsMatch		= fMvdHitsMatch		->GetEntriesFast();		// Number of Matches from Reconstructed Mvd Hits to Monte Carlo

  // Int_t nMcTracks 		= fListMCTracks		->GetEntriesFast();
  // 	Int_t nTracks			= fStsTrackArray	->GetEntriesFast();		// Number of Tracks

  // 	cout<<"MC Points    : "<< nMcpoints		<<endl;
  // 	cout<<"Hits         : "<< nHits			<<endl;
  // 	cout<<"HitMatchs    : "<< nHitsMatch	<<endl;
  // 	cout<<"Clusters     : "<< nClusters		<<endl;
  // 	cout<<"ClusterMatch : "<< nClustersMatch<<endl;
  // 	cout<<"Digis        : "<< nDigis		<<endl;
  // 	cout<<"DigiMatchs   : "<< nDigisMatch	<<endl;
  // 	cout<<"MC Tracks    : "<< nMcTracks		<<endl;
  // 	cout<<"Tracks       : "<< nTracks		<<endl;
  // 	cout<<"------"<<endl;
  // -------------------
  CbmMvdPoint* mvdPoint;  // Monte Carlo Point
  CbmMvdDigi* mvdDigi;    // Digi
  CbmMvdCluster* mvdCluster;
  CbmMvdHit* mvdHit;  // Digitized Hit

  CbmMvdDigiMatch* mvdDigiMatch;  // Digi to MC point
  CbmMatch* mvdClusterMatch;      // Cluster to (?)
  CbmMvdHitMatch* mvdHitMatch;    // Hit to MC point

  CbmMCTrack* mcTrack;
  // CbmStsTrack		* stsTrack;
  //  CbmTrackMatchNew* trackMatch;
  //  CbmMatch		* mvdMatch;			// Reco Track to Hit Match
  // -------------------
  typedef map<pair<Int_t, Int_t>, Int_t>::iterator it_type;
  map<pair<Int_t, Int_t>, Int_t> digiMap;
  pair<Int_t, Int_t> digiCoor;
  Int_t digiCharge;

  TVector3 cOrth;
  TVector3 cVect;

  Int_t* digiList;
  Double_t gloMC[3];
  Double_t locMC[3];
  Double_t gloHit[3];
  Double_t locHit[3];
  Double_t locRef[3];

  Double_t pitchx, pitchy;

  Int_t MAXHITS = nHits;
  Int_t count;
  Int_t charge;
  UInt_t shape;
  // 	bool		correctshape;
  Double_t ARR_momentum[MAXHITS];
  Double_t ARR_angle[MAXHITS];
  Int_t ARR_digis[MAXHITS];
  Double_t ARR_dx[MAXHITS];
  Double_t ARR_dy[MAXHITS];
  Double_t ARR_dz[MAXHITS];
  Double_t ARR_dxp[MAXHITS];
  Double_t ARR_dyp[MAXHITS];
  UInt_t ARR_shape[MAXHITS];
  Int_t ARR_charge[MAXHITS];
  Int_t XAXIS[1000];
  Int_t YAXIS[1000];
  Int_t xaxis, yaxis, xaxismin, xaxismax, yaxismin, yaxismax, xrel, yrel;
  Int_t POS;
  // -------------------
  fNrMcPointsAll += nMcpoints;
  fNrHitsAll += nHits;
  // -------------------
  mvdDigi        = (CbmMvdDigi*) fMvdDigis->At(0);
  pitchx         = mvdDigi->GetPixelSizeX();
  pitchy         = mvdDigi->GetPixelSizeY();
  fPixelpitch[0] = pitchx;
  fPixelpitch[1] = pitchy;

  TGeoVolume* CurrentVolume;
  TGeoBBox* VolumeShape;
  // -------------------
  std::map<std::pair<std::pair<Int_t, Int_t>, TString>, std::vector<int>> DigisMap;
  std::map<std::pair<std::pair<Int_t, Int_t>, TString>, std::vector<int>>::iterator it;
  std::pair<std::pair<Int_t, Int_t>, TString> DigiStation;
  std::pair<Int_t, Int_t> Digi;
  std::vector<int> McContrToHitList;
  std::vector<int> McContrList;  // vector of Mc Points which contribute to a Digi
  std::map<Int_t, Int_t> McInHit;
  std::map<Int_t, Int_t>::iterator it2;
  std::vector<int> DigisInMc(nMcpoints, 0);
  std::map<Int_t, std::vector<int>> McsInHit;
  std::map<Int_t, std::vector<int>> HitsInMc;
  std::map<Int_t, std::vector<int>>::iterator it3;

  DigisMap.clear();
  McInHit.clear();
  McsInHit.clear();
  HitsInMc.clear();

  // -------------------
  // Analyze Digis:

  for (int iDigi = 0; iDigi < nDigis; iDigi++) {

    mvdDigi      = (CbmMvdDigi*) fMvdDigis->At(iDigi);
    mvdDigiMatch = (CbmMvdDigiMatch*) fMvdDigisMatch->At(iDigi);

    Int_t nMatchedIndex = mvdDigiMatch->GetMatchedLink().GetIndex();
    mvdPoint            = (CbmMvdPoint*) fMcPoints->At(nMatchedIndex);  // Get matched MC Point from Digi

    // 		mcTrack		= (CbmMCTrack*)		fListMCTracks->At	( mvdpoint->GetTrackID() );						// Get matched MC Track from MC Point

    gloMC[0] = (mvdPoint->GetXOut() + mvdPoint->GetX()) / 2.;
    gloMC[1] = (mvdPoint->GetYOut() + mvdPoint->GetY()) / 2.;
    gloMC[2] = (mvdPoint->GetZOut() + mvdPoint->GetZ()) / 2.;

    gGeoManager->FindNode(gloMC[0], gloMC[1], gloMC[2]);

    CurrentVolume = gGeoManager->GetCurrentVolume();
    // 		VolumeShape		= (TGeoBBox*)CurrentVolume->GetShape();

    Digi.first  = mvdDigi->GetPixelX();
    Digi.second = mvdDigi->GetPixelY();

    DigiStation.first  = Digi;
    DigiStation.second = CurrentVolume->GetName();

    McContrList.clear();

    for (Int_t iLink = 0; iLink < mvdDigiMatch->GetNofLinks(); iLink++) {
      McContrList.push_back(mvdDigiMatch->GetLink(iLink).GetIndex());
    }

    // 		it = DigisMap.find(DigiStation);
    // 		if (it == DigisMap.end())
    // 		{
    DigisMap[DigiStation] = McContrList;
    // 		}
    // 		else	// Should not be possible
    // 		{
    // 		}
  }

  // -------------------
  // Analyze Hits and Clusters

  for (Int_t iHit = 0; iHit < nHits; iHit++) {
    cout << endl << "run next Hit " << iHit << " of " << nHits << " Hits" << endl;
    mvdHit          = (CbmMvdHit*) fMvdHits->At(iHit);
    mvdCluster      = (CbmMvdCluster*) fMvdClusters->At(iHit);
    mvdHitMatch     = (CbmMvdHitMatch*) fMvdHitsMatch->At(iHit);
    mvdClusterMatch = (CbmMatch*) fMvdClustersMatch->At(iHit);

    Int_t nHitMatch = mvdHitMatch->GetMatchedLink().GetIndex();
    mvdPoint        = (CbmMvdPoint*) fMcPoints->At(nHitMatch);

    mcTrack = (CbmMCTrack*) fListMCTracks->At(mvdPoint->GetTrackID());

    gloMC[0] = (mvdPoint->GetXOut() + mvdPoint->GetX()) / 2.;
    gloMC[1] = (mvdPoint->GetYOut() + mvdPoint->GetY()) / 2.;
    gloMC[2] = (mvdPoint->GetZOut() + mvdPoint->GetZ()) / 2.;

    gloHit[0] = mvdHit->GetX();
    gloHit[1] = mvdHit->GetY();
    gloHit[2] = mvdHit->GetZ();

    gGeoManager->FindNode(gloMC[0], gloMC[1], gloMC[2]);
    gGeoManager->MasterToLocal(gloMC, locMC);
    gGeoManager->MasterToLocal(gloHit, locHit);

    CurrentVolume = gGeoManager->GetCurrentVolume();
    VolumeShape   = (TGeoBBox*) CurrentVolume->GetShape();

    locRef[0] = (mvdHit->GetIndexCentralX() + 0.5) * pitchx - VolumeShape->GetDX();
    locRef[1] = (mvdHit->GetIndexCentralY() + 0.5) * pitchy - VolumeShape->GetDY();
    locRef[2] = 0;

    cOrth.SetXYZ(0., 0., mvdPoint->GetZOut() - mvdPoint->GetZ());
    cVect.SetXYZ(mvdPoint->GetXOut() - mvdPoint->GetX(), mvdPoint->GetYOut() - mvdPoint->GetY(),
                 mvdPoint->GetZOut() - mvdPoint->GetZ());

    digiMap  = mvdCluster->GetPixelMap();
    digiList = mvdCluster->GetDigiList();

    count  = 0;
    charge = 0;
    shape  = 0;

    McInHit.clear();
    McContrToHitList.clear();
    cout << endl << "ping" << endl;
    for (it_type iterator = digiMap.begin(); iterator != digiMap.end(); iterator++) {
      digiCoor   = iterator->first;
      digiCharge = iterator->second;
      xaxis      = digiCoor.first;
      yaxis      = digiCoor.second;

      XAXIS[count] = xaxis;
      YAXIS[count] = yaxis;

      if (count == 0) {
        xaxismin = xaxis;
        xaxismax = xaxis;
        yaxismin = yaxis;
        yaxismax = yaxis;
      }
      else {
        if (xaxismin > xaxis) { xaxismin = xaxis; }
        if (xaxismax < xaxis) { xaxismax = xaxis; }
        if (yaxismin > yaxis) { yaxismin = yaxis; }
        if (yaxismax < yaxis) { yaxismax = yaxis; }
      }

      charge += digiCharge;
      count++;

      DigiStation.first  = digiCoor;
      DigiStation.second = CurrentVolume->GetName();

      it = DigisMap.find(DigiStation);

      McContrList = it->second;  // Mcs contributing to a Digi

      // 			if (it != DigisMap.end())
      // 			{
      fMcperDigi[McContrList.size()]++;  // Number of MC Points contributing to a Digi
      cout << endl << "ping 2" << endl;
      for (Int_t iMc = 0; iMc < McContrList.size(); iMc++) {
        if (std::find(McContrToHitList.begin(), McContrToHitList.end(), McContrList[iMc]) == McContrToHitList.end()) {
          McContrToHitList.push_back(McContrList[iMc]);
        }

        it2 = McInHit.find(McContrList[iMc]);

        if (it2 != McInHit.end()) { McInHit[McContrList[iMc]]++; }
        else {
          McInHit[McContrList[iMc]] = 1;
        }
      }
      // 			}
      // 			else // should not be possible
      // 			{
      // 			}
    }

    McsInHit[iHit] = McContrToHitList;

    if (McInHit.size() < 11) { fMcperHit[McInHit.size()]++; }
    else {
      fMcperHit[10]++;
    }
    count = 0;
    cout << endl << "ping 3" << endl;
    for (std::map<Int_t, Int_t>::iterator iterator = McInHit.begin(); iterator != McInHit.end(); iterator++) {
      DigisInMc[iterator->first] += iterator->second;
      count++;

      it3 = HitsInMc.find(iterator->first);

      if (it3 != HitsInMc.end()) {
        McContrList = it3->second;
        McContrList.push_back(iHit);
      }
      else {
        McContrList.clear();
        McContrList.push_back(iHit);
      }

      HitsInMc[iterator->first] = McContrList;
    }
    cout << endl << "ping 4" << endl;
    if ((xaxismax - xaxismin) < 4 && (yaxismax - yaxismin) < 4) {
      shape = 0;

      for (int i = 0; i < mvdCluster->GetNofDigis(); i++) {
        xrel = XAXIS[i] - xaxismin;
        yrel = YAXIS[i] - yaxismin;
        POS  = xrel + 5 * yrel;
        shape += TMath::Power(2, POS);
      }

      if (shape == 3) { shape = 0; }
      else if (shape == 99) {
        shape = 1;
      }
      else if (shape == 33) {
        shape = 2;
      }
      else if (shape == 7) {
        shape = 3;
      }
      else if (shape == 67) {
        shape = 4;
      }
      else if (shape == 97) {
        shape = 5;
      }
      else if (shape == 35) {
        shape = 6;
      }
      else if (shape == 98) {
        shape = 7;
      }
      else if (shape == 1) {
        shape = 8;
      }
      else {
        shape = 9;
      }
    }
    else {
      shape = 9;
    }
    cout << endl << "ping 5" << endl;
    ARR_momentum[iHit] = mcTrack->GetP();
    ARR_angle[iHit]    = cVect.Angle(cOrth);
    ARR_digis[iHit]    = mvdCluster->GetNofDigis();
    ARR_dx[iHit]       = 10000 * (locHit[0] - locMC[0]);
    ARR_dy[iHit]       = 10000 * (locHit[1] - locMC[1]);
    ARR_dz[iHit]       = 10000 * (gloHit[2] - gloMC[2]);
    ARR_dxp[iHit]      = -(int) (((locMC[0] + VolumeShape->GetDX()) / (1 * pitchx)) - 0.5)
                    + (double) (((locMC[0] + VolumeShape->GetDX()) / (1 * pitchx)) - 0.5);
    ARR_dyp[iHit] = -(int) (((locMC[1] + VolumeShape->GetDY()) / (1 * pitchy)) - 0.5)
                    + (double) (((locMC[1] + VolumeShape->GetDY()) / (1 * pitchy)) - 0.5);
    ARR_shape[iHit]  = shape;
    ARR_charge[iHit] = charge;
    cout << endl << "finished this hit" << endl;
  }

  // -------------------
  for (Int_t iHit = 0; iHit < nHits; iHit++) {
    cout << endl << "fill stuff" << endl;
    fMvdHisto1[0]->Fill(ARR_momentum[iHit]);
    fMvdHisto1[1]->Fill(ARR_angle[iHit]);
    fMvdHisto1[2]->Fill(ARR_digis[iHit]);
    fMvdHisto1[3]->Fill(ARR_dx[iHit]);
    fMvdHisto1[4]->Fill(ARR_dy[iHit]);
    fMvdHisto1[5]->Fill(ARR_dz[iHit]);
    fMvdHisto1[6]->Fill(ARR_dxp[iHit]);
    fMvdHisto1[7]->Fill(ARR_dyp[iHit]);
    fMvdHisto1[8]->Fill(ARR_shape[iHit]);
    fMvdHisto1[9]->Fill(ARR_charge[iHit]);
    // fMvdHisto1[10+ARR_shape[iHit]]->Fill(ARR_dx[iHit]);
    // fMvdHisto1[20+ARR_shape[iHit]]->Fill(ARR_dy[iHit]);

    fMvdHisto2[0]->Fill(ARR_dxp[iHit], ARR_dyp[iHit], ARR_digis[iHit]);
    fMvdHisto2[0]->Fill(ARR_dxp[iHit], -ARR_dyp[iHit], ARR_digis[iHit]);
    fMvdHisto2[0]->Fill(-ARR_dxp[iHit], ARR_dyp[iHit], ARR_digis[iHit]);
    fMvdHisto2[0]->Fill(-ARR_dxp[iHit], -ARR_dyp[iHit], ARR_digis[iHit]);
    fMvdHisto2[1]->Fill(ARR_dxp[iHit], ARR_dyp[iHit]);
    fMvdHisto2[1]->Fill(ARR_dxp[iHit], -ARR_dyp[iHit]);
    fMvdHisto2[1]->Fill(-ARR_dxp[iHit], ARR_dyp[iHit]);
    fMvdHisto2[1]->Fill(-ARR_dxp[iHit], -ARR_dyp[iHit]);
    fMvdHisto2[2]->Fill(ARR_momentum[iHit], ARR_dx[iHit]);
    fMvdHisto2[3]->Fill(ARR_momentum[iHit], ARR_dy[iHit]);
    fMvdHisto2[4]->Fill(ARR_angle[iHit], ARR_dx[iHit]);
    fMvdHisto2[5]->Fill(ARR_angle[iHit], ARR_dy[iHit]);
    fMvdHisto2[6]->Fill(ARR_digis[iHit], ARR_dx[iHit]);
    fMvdHisto2[7]->Fill(ARR_digis[iHit], ARR_dy[iHit]);
    // 		fMvdHisto2[8]->Fill(ARR_dxp[iHit], ARR_dx[iHit]);
    // 		fMvdHisto2[9]->Fill(ARR_dyp[iHit], ARR_dy[iHit]);
    fMvdHisto2[10]->Fill(ARR_charge[iHit], ARR_dx[iHit]);
    fMvdHisto2[11]->Fill(ARR_charge[iHit], ARR_dy[iHit]);
    fMvdHisto2[12]->Fill(ARR_momentum[iHit], ARR_dz[iHit]);
    fMvdHisto2[13]->Fill(ARR_angle[iHit], ARR_dz[iHit]);
    fMvdHisto2[14]->Fill(ARR_dx[iHit], ARR_dy[iHit]);

    fMvdHisto1[33]->Fill(ARR_dx[iHit] / 3.8);
    fMvdHisto1[34]->Fill(ARR_dy[iHit] / 4.8);

    fMvdHisto2[15]->Fill(ARR_momentum[iHit], ARR_charge[iHit]);
    fMvdHisto2[16]->Fill(ARR_angle[iHit], ARR_charge[iHit]);

    if (ARR_angle[iHit] < 0.3) { fMvdHisto2[17]->Fill(ARR_momentum[iHit], ARR_charge[iHit]); }
  }

  for (int iMc = 0; iMc < nMcpoints; iMc++) {
    fMvdHisto1[30]->Fill(DigisInMc[iMc]);
  }

  for (std::map<Int_t, std::vector<int>>::iterator iterator = HitsInMc.begin(); iterator != HitsInMc.end();
       iterator++) {
    McContrList = iterator->second;
    fMvdHisto1[31]->Fill(McContrList.size());
  }

  bool criteria;

  for (std::map<Int_t, std::vector<int>>::iterator iterator = McsInHit.begin(); iterator != McsInHit.end();
       iterator++) {
    criteria = true;

    McContrToHitList = iterator->second;

    for (int i = 0; i < McContrToHitList.size(); i++) {
      it3 = HitsInMc.find(McContrToHitList[i]);

      if (it3 != HitsInMc.end()) {
        if ((it3->second).size() != 1) { criteria = false; }
      }
    }

    if (criteria) { fMvdHisto1[32]->Fill(McContrToHitList.size()); }
  }
  // -------------------
}
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
void CbmMvdClusterAna::Finish()
{
  cout << "======================" << endl;
  cout << "'Mvd QA Output' Start!" << endl;
  cout << "======================" << endl;
  // -------------------
  cout << "----------------------" << endl;
  cout << "Pixelpitch: " << endl;
  cout << "x: " << fPixelpitch[0] << endl;
  cout << "y: " << fPixelpitch[1] << endl;
  // -------------------
  int sum;
  cout << "----------------------" << endl;
  cout << "MC points per Digi:" << endl;
  sum = 0;
  for (Int_t i = 0; i < 6; i++) {
    sum += fMcperDigi[i];
  }
  for (Int_t i = 0; i < 6; i++) {
    fMvdHisto1[35]->Fill(i, fMcperDigi[i]);
    printf(" %2i  %10i    %6.5f \n", i, fMcperDigi[i], 1. * fMcperDigi[i] / sum);
  }
  // -------------------
  cout << "----------------------" << endl;
  cout << "Digis per MC Point:" << endl;
  for (Int_t i = 0; i < 11; i++) {
    if (fMvdHisto1[30]->GetBinCenter(i) >= 0)
      printf(" %2i  %10i    %6.5f \n", (int) (fMvdHisto1[30]->GetBinCenter(i)),
             (int) (fMvdHisto1[30]->GetBinContent(i)),
             1. * (int) (fMvdHisto1[30]->GetBinContent(i)) * 1. / fMvdHisto1[30]->GetEntries());
  }
  cout << "  .." << endl;
  // -------------------
  cout << "----------------------" << endl;
  cout << "Digis per Hit:" << endl;
  for (Int_t i = 0; i < 11; i++) {
    if (fMvdHisto1[2]->GetBinCenter(i) >= 0)
      printf(" %2i  %10i    %6.5f \n", (int) (fMvdHisto1[2]->GetBinCenter(i)), (int) (fMvdHisto1[2]->GetBinContent(i)),
             1. * (int) (fMvdHisto1[2]->GetBinContent(i)) * 1. / fMvdHisto1[2]->GetEntries());
  }
  cout << "  .." << endl;
  // -------------------
  cout << "----------------------" << endl;
  cout << "Hits per MC Point:" << endl;
  for (Int_t i = 0; i < 11; i++) {
    if (fMvdHisto1[31]->GetBinCenter(i) >= 0)
      printf(" %2i  %10i    %6.5f \n", (int) (fMvdHisto1[31]->GetBinCenter(i)),
             (int) (fMvdHisto1[31]->GetBinContent(i)),
             1. * (int) (fMvdHisto1[31]->GetBinContent(i)) * 1. / fMvdHisto1[31]->GetEntries());
  }
  cout << "  .." << endl;
  // -------------------
  cout << "----------------------" << endl;
  cout << "MC Points per Hit:" << endl;
  sum = 0;
  for (Int_t i = 0; i < 11; i++) {
    sum += fMcperHit[i];
  }
  for (Int_t i = 0; i < 11; i++) {
    fMvdHisto1[36]->Fill(i, fMcperHit[i]);
    printf(" %2i  %10i    %6.5f \n", i, fMcperHit[i], 1. * fMcperHit[i] / sum);
  }
  // -------------------
  cout << "----------------------" << endl;
  cout << "MC Points per Hit (merged Clusters):" << endl;
  for (Int_t i = 0; i < 11; i++) {
    if (fMvdHisto1[32]->GetBinCenter(i) >= 0)
      printf(" %2i  %10i    %6.5f \n", (int) (fMvdHisto1[32]->GetBinCenter(i)),
             (int) (fMvdHisto1[32]->GetBinContent(i)),
             1. * (int) (fMvdHisto1[32]->GetBinContent(i)) * 1. / fMvdHisto1[32]->GetEntries());
  }
  cout << "  .." << endl;
  // -------------------
  cout << "----------------------" << endl;
  // -------------------
  Double_t xpar[3];
  Double_t ypar[3];

  TF1* gFitx = new TF1("gausx", "gaus", -50, 50);
  TF1* gFity = new TF1("gausy", "gaus", -50, 50);

  fMvdHisto1[3]->Fit(gFitx, "QRN0");
  fMvdHisto1[4]->Fit(gFity, "QRN0");

  gFitx->GetParameters(&xpar[0]);
  gFity->GetParameters(&ypar[0]);

  cout << setw(40) << "---------------------------------------------------------------------" << endl;
  cout << "Resolution:" << endl;
  cout << setw(40) << "---------------------------------------------------------------------" << endl;
  cout << setw(9) << "  Shape";
  cout << setw(10) << "  Mean(x)";
  cout << setw(10) << "  RMS(x)";
  cout << setw(10) << "  Mean(y)";
  cout << setw(10) << "  RMS(y)";
  cout << setw(10) << "  Mean(z)";
  cout << setw(10) << "  RMS(z)";
  cout << endl;
  cout << setw(40) << "---------------------------------------------------------------------";
  cout << endl;

  for (int i = 0; i < 10; i++) {
    printf("%6i    %+5.4f   %+5.4f   %+5.4f   %+5.4f  \n", i, fMvdHisto1[10 + i]->GetMean(),
           fMvdHisto1[10 + i]->GetRMS(), fMvdHisto1[20 + i]->GetMean(), fMvdHisto1[20 + i]->GetRMS());
  }
  cout << setw(40) << "---------------------------------------------------------------------" << endl;
  printf("%10s%+5.4f   %+5.4f   %+5.4f   %+5.4f   %+5.4f   %+5.4f  \n", "All       ", fMvdHisto1[3]->GetMean(),
         fMvdHisto1[3]->GetRMS(), fMvdHisto1[4]->GetMean(), fMvdHisto1[4]->GetRMS(), fMvdHisto1[5]->GetMean(),
         fMvdHisto1[5]->GetRMS());
  printf("%10s%+5.4f   %+5.4f   %+5.4f   %+5.4f  \n", "All (Fit) ", xpar[1], xpar[2], ypar[1], ypar[2]);

  cout << setw(40) << "---------------------------------------------------------------------" << endl;
  fMvdHisto1[33]->Fit(gFitx, "QRN0");
  fMvdHisto1[34]->Fit(gFity, "QRN0");

  gFitx->GetParameters(&xpar[0]);
  gFity->GetParameters(&ypar[0]);
  cout << "Pulls:" << endl;
  printf("%10s%+5.4f   %+5.4f   %+5.4f   %+5.4f  \n", "All       ", fMvdHisto1[33]->GetMean(), fMvdHisto1[33]->GetRMS(),
         fMvdHisto1[34]->GetMean(), fMvdHisto1[34]->GetRMS());
  printf("%10s%+5.4f   %+5.4f   %+5.4f   %+5.4f  \n", "All (Fit) ", xpar[1], xpar[2], ypar[1], ypar[2]);
  cout << "(Using 3.8(x) and 4.8(y) as nominal resolution!)" << endl;
  cout << setw(40) << "---------------------------------------------------------------------" << endl;
  cout << setw(40) << "---------------------------------------------------------------------" << endl;
  // -------------------
  cout << "Hit Reco Efficiency: " << float(1. * fNrHitsAll / fNrMcPointsAll) << "\t( " << fNrHitsAll << "\t"
       << fNrMcPointsAll << " )" << endl;
  cout << setw(40) << "---------------------------------------------------------------------" << endl;
  // -------------------
  cout << "======================" << endl;
  cout << "'Mvd QA Output' End!" << endl;
  cout << "======================" << endl;

  // -------------------
  TCanvas* a = new TCanvas("Residuals", "Residuals");
  a->Divide(3, 3);

  a->cd(1);
  fMvdHisto1[3]->Draw();
  fMvdHisto1[3]->GetXaxis()->SetTitle("Residual in x [um]");
  fMvdHisto1[3]->GetYaxis()->SetTitle("Entries");

  a->cd(2);
  fMvdHisto1[4]->Draw();
  fMvdHisto1[4]->GetXaxis()->SetTitle("Residual in y [um]");
  fMvdHisto1[4]->GetYaxis()->SetTitle("Entries");

  a->cd(3);
  fMvdHisto1[5]->Draw();
  fMvdHisto1[5]->GetXaxis()->SetTitle("Residual in z [um]");
  fMvdHisto1[5]->GetYaxis()->SetTitle("Entries");

  a->cd(4);
  fMvdHisto1[33]->Draw();
  fMvdHisto1[33]->GetXaxis()->SetTitle("Pull in x [um]");
  fMvdHisto1[33]->GetYaxis()->SetTitle("Entries");

  a->cd(5);
  fMvdHisto1[34]->Draw();
  fMvdHisto1[34]->GetXaxis()->SetTitle("Pull in y [um]");
  fMvdHisto1[34]->GetYaxis()->SetTitle("Entries");

  int col[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 11};

  TLegend* leg = new TLegend(0.1, 0.7, 0.48, 0.9);

  a->cd(7);
  for (int i = 0; i < 10; i++) {
    if (i == 0) {
      fMvdHisto1[10 + i]->Draw();
      fMvdHisto1[10 + i]->GetXaxis()->SetTitle("Residual in x [um]");
      fMvdHisto1[10 + i]->GetYaxis()->SetTitle("Entries");
    }
    else {
      fMvdHisto1[10 + i]->Draw("same");
    }
    fMvdHisto1[10 + i]->SetLineColor(col[i]);
    leg->AddEntry(fMvdHisto1[10 + i], Form("Shape %i", i));
  }
  leg->Draw();

  a->cd(8);
  for (int i = 0; i < 10; i++) {
    if (i == 0) {
      fMvdHisto1[20 + i]->Draw();
      fMvdHisto1[20 + i]->GetXaxis()->SetTitle("Residual in y [um]");
      fMvdHisto1[20 + i]->GetYaxis()->SetTitle("Entries");
    }
    else {
      fMvdHisto1[20 + i]->Draw("same");
    }
    fMvdHisto1[20 + i]->SetLineColor(col[i]);
  }
  leg->Draw();
  // -------------------
  TCanvas* b = new TCanvas("Matches", "Matches");
  b->Divide(3, 2);

  b->cd(1);
  fMvdHisto1[35]->Draw();
  fMvdHisto1[35]->GetXaxis()->SetTitle("Mc Points per Digi");
  fMvdHisto1[35]->GetYaxis()->SetTitle("Entries");

  b->cd(2);
  fMvdHisto1[30]->Draw();
  fMvdHisto1[30]->GetXaxis()->SetTitle("Digis per Mc Point");
  fMvdHisto1[30]->GetYaxis()->SetTitle("Entries");

  b->cd(3);
  fMvdHisto1[2]->Draw();
  fMvdHisto1[2]->GetXaxis()->SetTitle("Digis per Hit");
  fMvdHisto1[2]->GetYaxis()->SetTitle("Entries");

  b->cd(4);
  fMvdHisto1[31]->Draw();
  fMvdHisto1[31]->GetXaxis()->SetTitle("Hits per Mc Point");
  fMvdHisto1[31]->GetYaxis()->SetTitle("Entries");

  b->cd(5);
  fMvdHisto1[36]->Draw();
  fMvdHisto1[36]->GetXaxis()->SetTitle("Mc Points per Hit");
  fMvdHisto1[36]->GetYaxis()->SetTitle("Entries");

  b->cd(6);
  fMvdHisto1[32]->Draw();
  fMvdHisto1[32]->GetXaxis()->SetTitle("Mc Points per Hit (merged)");
  fMvdHisto1[32]->GetYaxis()->SetTitle("Entries");
  // -------------------
  TCanvas* d = new TCanvas("ResAna", "ResAna");
  d->Divide(4, 2);

  fProf[0] = fMvdHisto2[2]->ProfileX("0", 1, -1, "s");
  fProf[1] = fMvdHisto2[3]->ProfileX("1", 1, -1, "s");
  fProf[2] = fMvdHisto2[4]->ProfileX("2", 1, -1, "s");
  fProf[3] = fMvdHisto2[5]->ProfileX("3", 1, -1, "s");
  fProf[4] = fMvdHisto2[6]->ProfileX("4", 1, -1, "s");
  fProf[5] = fMvdHisto2[7]->ProfileX("5", 1, -1, "s");
  fProf[6] = fMvdHisto2[10]->ProfileX("6", 1, -1, "s");
  fProf[7] = fMvdHisto2[11]->ProfileX("7", 1, -1, "s");

  d->cd(1);
  fProf[0]->Draw();
  fProf[0]->GetXaxis()->SetTitle("Momentum [GeV]");
  fProf[0]->GetYaxis()->SetTitle("Residual in x [um]");

  d->cd(5);
  fProf[1]->Draw();
  fProf[1]->GetXaxis()->SetTitle("Momentum [GeV]");
  fProf[1]->GetYaxis()->SetTitle("Residual in y [um]");

  d->cd(2);
  fProf[2]->Draw();
  fProf[2]->GetXaxis()->SetTitle("Angle [rad]");
  fProf[2]->GetYaxis()->SetTitle("Residual in x [um]");

  d->cd(6);
  fProf[3]->Draw();
  fProf[3]->GetXaxis()->SetTitle("Angle [rad]");
  fProf[3]->GetYaxis()->SetTitle("Residual in y [um]");

  d->cd(3);
  fProf[4]->Draw();
  fProf[4]->GetXaxis()->SetTitle("Digis per Hit");
  fProf[4]->GetYaxis()->SetTitle("Residual in x [um]");

  d->cd(7);
  fProf[5]->Draw();
  fProf[5]->GetXaxis()->SetTitle("Digis per Hit");
  fProf[5]->GetYaxis()->SetTitle("Residual in y [um]");

  d->cd(4);
  fProf[6]->Draw();
  fProf[6]->GetXaxis()->SetTitle("Charge");
  fProf[6]->GetYaxis()->SetTitle("Residual in x [um]");

  d->cd(8);
  fProf[7]->Draw();
  fProf[7]->GetXaxis()->SetTitle("Charge");
  fProf[7]->GetYaxis()->SetTitle("Residual in y [um]");

  TCanvas* c = new TCanvas("Rest", "Rest");
  c->Divide(6, 2);

  c->cd(1);
  fMvdHisto1[0]->Draw();
  fMvdHisto1[0]->GetXaxis()->SetTitle("Momentum [GeV]");
  fMvdHisto1[0]->GetYaxis()->SetTitle("Entries");

  c->cd(2);
  fMvdHisto1[1]->Draw();
  fMvdHisto1[1]->GetXaxis()->SetTitle("Angle [rad]");
  fMvdHisto1[1]->GetYaxis()->SetTitle("Entries");

  c->cd(3);
  fMvdHisto1[2]->Draw();
  fMvdHisto1[2]->GetXaxis()->SetTitle("Digis per Hit");
  fMvdHisto1[2]->GetYaxis()->SetTitle("Entries");

  c->cd(4);
  fMvdHisto2[14]->Draw("colz");
  fMvdHisto2[14]->GetXaxis()->SetTitle("Residual in x [um]");
  fMvdHisto2[14]->GetYaxis()->SetTitle("Residual in y [um]");
  fMvdHisto2[14]->GetZaxis()->SetTitle("Entries");

  c->cd(5);
  fMvdHisto2[15]->Draw("colz");
  fMvdHisto2[15]->GetXaxis()->SetTitle("Momentum [GeV]");
  fMvdHisto2[15]->GetYaxis()->SetTitle("Charge");
  fMvdHisto2[15]->GetZaxis()->SetTitle("Entries");

  c->cd(6);
  fMvdHisto2[16]->Draw("colz");
  fMvdHisto2[16]->GetXaxis()->SetTitle("Angle [rad]");
  fMvdHisto2[16]->GetYaxis()->SetTitle("Charge");
  fMvdHisto2[16]->GetZaxis()->SetTitle("Entries");

  c->cd(7);
  fMvdHisto1[6]->Draw();
  fMvdHisto1[6]->GetXaxis()->SetTitle("Distance to Pixel in x [Pixelpitch]");
  fMvdHisto1[6]->GetYaxis()->SetTitle("Entries");

  c->cd(8);
  fMvdHisto1[7]->Draw();
  fMvdHisto1[7]->GetXaxis()->SetTitle("Distance to Pixel in y [Pixelpitch]");
  fMvdHisto1[7]->GetYaxis()->SetTitle("Entries");

  c->cd(9);
  fMvdHisto1[8]->Draw();
  fMvdHisto1[8]->GetXaxis()->SetTitle("Clustershape");
  fMvdHisto1[8]->GetYaxis()->SetTitle("Entries");

  c->cd(10);
  fMvdHisto1[9]->Draw();
  fMvdHisto1[9]->GetXaxis()->SetTitle("Charge");
  fMvdHisto1[9]->GetYaxis()->SetTitle("Entries");

  fProf[8]  = fMvdHisto2[15]->ProfileX("8", 1, -1, "");
  fProf[9]  = fMvdHisto2[16]->ProfileX("9", 1, -1, "");
  fProf[10] = fMvdHisto2[17]->ProfileX("10", 1, -1, "");

  c->cd(11);
  fProf[8]->Draw();
  fProf[8]->GetXaxis()->SetTitle("Momentum [GeV]");
  fProf[8]->GetYaxis()->SetTitle("Charge");
  fProf[10]->Draw("same");
  fProf[10]->SetLineColor(2);

  c->cd(12);
  fProf[9]->Draw();
  fProf[9]->GetXaxis()->SetTitle("Angle [rad]");
  fProf[9]->GetYaxis()->SetTitle("Charge");

  // 	fMvdHisto2[0]->Divide(fMvdHisto2[1]);
  // 	TCanvas* e=new TCanvas();
  // 	e->cd(1);
  // 	fMvdHisto2[0]->Draw("colz");
  // 	fMvdHisto2[0]->GetXaxis()->SetTitle("Distance in x MC to Pix [Pixelpitch]");
  // 	fMvdHisto2[0]->GetYaxis()->SetTitle("Distance in y MC to Pix [Pixelpitch]");
  // 	fMvdHisto2[0]->GetZaxis()->SetTitle("Cluster Multiplicity");


  //
}
// -------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
ClassImp(CbmMvdClusterAna);
