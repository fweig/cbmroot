/* Copyright (C) 2015-2019 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Philipp Sitzmann [committer] */

// -------------------------------------------------------------------------
// -----              CbmMvdQa  source file                            -----
// -----              Created 12/01/15  by P. Sitzmann                 -----
// ------------------------------------------------------------------------

//-- Include from Cbm --//
#include "CbmMvdQa.h"

#include "CbmGlobalTrack.h"
#include "CbmLink.h"
#include "CbmMCTrack.h"
#include "CbmMatch.h"
#include "CbmMvdDetector.h"
#include "CbmMvdDigi.h"
#include "CbmMvdHit.h"
#include "CbmMvdPoint.h"
#include "CbmMvdSensor.h"
#include "CbmMvdStationPar.h"
#include "CbmStsTrack.h"
#include "CbmTrackMatchNew.h"
#include "CbmVertex.h"
#include "tools/CbmMvdGeoHandler.h"


//-- Include from Fair --//
#include <FairRootManager.h>
#include <FairTrackParam.h>
#include <Logger.h>


//-- Include from Root --//
#include "TCanvas.h"
#include "TClonesArray.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"
#include "TStyle.h"
#include <TFile.h>

//-- Include from C++ --//
#include <iostream>


using std::cout;
using std::endl;
using std::flush;

// -----   Default constructor   -------------------------------------------
CbmMvdQa::CbmMvdQa() : CbmMvdQa("MvdQa", 0, 0) {}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
CbmMvdQa::CbmMvdQa(const char* name, Int_t iMode, Int_t iVerbose)
  : FairTask(name, iVerbose)
  , foutFile(nullptr)
  , fNHitsOfLongTracks(0)
  , fEventNumber(0)
  , fminHitReq(0)
  , fMvdRecoRatio(0.)
  , fBadTrack(0)
  , fUsedTracks(0)
  , fnrOfMergedHits(0)
  , fFirstMvdPos(0)
  , fnrTrackslowP(0)
  , fnrTracksHighP(0)
  , flow(0)
  , fmid(0)
  , fhigh(0)
  , fStsTrackArray(nullptr)
  , fStsTrackArrayP(nullptr)
  , fStsTrackArrayN(nullptr)
  , fStsTrackMatches(nullptr)
  , fGlobalTrackArray(nullptr)
  , fListMCTracks(nullptr)
  , fMCTrackArrayP(nullptr)
  , fMCTrackArrayN(nullptr)
  , fMcPoints(nullptr)
  , fMvdDigis(nullptr)
  , fMvdCluster(nullptr)
  , fMvdHits(nullptr)
  , fMvdHitMatchArray(nullptr)
  , fMvdDigiMatchArray(nullptr)
  , fBadTracks(nullptr)
  , fInfoArray(nullptr)
  , fMC1F()
  , fMC2F()
  , fDigi1F()
  , fDigi2F()
  , fHits1F()
  , fHits2F()
  , fTracks1F()
  , fTracks2F()
  , fPrimVtx(nullptr)
  , fSecVtx(nullptr)
  , fDetector(nullptr)
  , useMcQa(kFALSE)
  , useDigiQa(kFALSE)
  , useHitQa(kFALSE)
  , useTrackQa(kFALSE)
  , fMode(iMode)
  , fdraw(kFALSE)
{
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmMvdQa::~CbmMvdQa() { ; }
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
InitStatus CbmMvdQa::Init()
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
  fBadTracks = new TClonesArray("CbmStsTrack", 5000);
  ioman->Register("BadTracks", "sts", fBadTracks, IsOutputBranchPersistent("BadTracks"));

  fStsTrackMatches  = (TClonesArray*) ioman->GetObject("StsTrackMatch");
  fStsTrackArray    = (TClonesArray*) ioman->GetObject("StsTrack");
  fGlobalTrackArray = (TClonesArray*) ioman->GetObject("GlobalTrack");

  if (!fStsTrackArray) { Fatal("CbmMvdQa: StsTrackArray not found (!)", " That's bad. "); }

  fMcPoints          = (TClonesArray*) ioman->GetObject("MvdPileUpMC");  // PileUp Mc points
  fMvdDigis          = (TClonesArray*) ioman->GetObject("MvdDigi");
  fMvdCluster        = (TClonesArray*) ioman->GetObject("MvdCluster");
  fMvdHits           = (TClonesArray*) ioman->GetObject("MvdHit");
  fMvdHitMatchArray  = (TClonesArray*) ioman->GetObject("MvdHitMatch");
  fMvdDigiMatchArray = (TClonesArray*) ioman->GetObject("MvdDigiMatch");

  if (fMvdHits->GetEntriesFast() != fMvdHitMatchArray->GetEntriesFast())
    cout << endl << "MvdHit and MvdHitMatch Arrays do not have the same size" << endl;

  //    fPrimVtx         = (CbmVertex*) ioman->GetObject("PrimaryVertex");
  // Get pointer to PrimaryVertex object from IOManager if it exists
  // The old name for the object is "PrimaryVertex" the new one
  // "PrimaryVertex." Check first for the new name
  fPrimVtx = dynamic_cast<CbmVertex*>(ioman->GetObject("PrimaryVertex."));
  if (nullptr == fPrimVtx) { fPrimVtx = dynamic_cast<CbmVertex*>(ioman->GetObject("PrimaryVertex")); }
  if (nullptr == fPrimVtx) {
    //      LOG(fatal) << "No primary vertex";
  }

  fListMCTracks = (TClonesArray*) ioman->GetObject("MCTrack");

  if (!fMcPoints) {
    cout << endl << "Mvd Pile Up Mc array missing";
    return kFATAL;
  }
  fDetector = CbmMvdDetector::Instance();
  fDetector->Init();

  CbmMvdStationPar* fPar = fDetector->GetParameterFile();
  fFirstMvdPos           = fPar->GetZPosition(0);

  fnrTrackslowP  = 0;
  fnrTracksHighP = 0;
  flow           = 0;
  fmid           = 0;
  fhigh          = 0;

  SetupHistograms();

  cout << "--------------------------------------------------------------------"
          "-----"
       << endl
       << "-I- " << GetName() << "::Init: "
       << " Finished Initialisation " << endl
       << "--------------------------------------------------------------------"
          "-----"
       << endl;
  return kSUCCESS;
}

// -------------------------------------------------------------------------
void CbmMvdQa::SetupHistograms()
{
  if (useMcQa) SetupMCHistograms();
  if (useDigiQa) SetupDigiHistograms();
  if (useHitQa) SetupHitHistograms();
  if (useTrackQa) SetupTrackHistograms();
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMvdQa::SetupMCHistograms()
{
  //   fMC1F[] = new TH1F("fMC1F[]","",100, 0, 100);
  //      fMC1F[]->GetXaxis()->SetTitle("");
  //      fMC1F[]->GetYaxis()->SetTitle("");

  fMC1F[0] = new TH1F("fMC1F[0]", "Matching efficientcy in the mvd", 100, 0, 1.5);
  fMC1F[0]->GetXaxis()->SetTitle(" 1 - (Incorrect / Total) ");
  fMC1F[0]->GetYaxis()->SetTitle("Entries");

  //   fMC2F[] = new TH2F("fMC2F[]","",100, 0, 100, 100, 0, 100);
  //      fMC2F[]->GetXaxis()->SetTitle("");
  //      fMC2F[]->GetYaxis()->SetTitle("");

  fMC2F[0] = new TH2F("fMC2F[0]", "MC-Distribution on the first Station", 100, 0, 10, 100, 0, 10);
  fMC2F[0]->GetXaxis()->SetTitle("X-Pos[cm]");
  fMC2F[0]->GetYaxis()->SetTitle("Y-Pos[cm]");
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMvdQa::SetupDigiHistograms()
{
  //   fDigi1F[] = new TH1F("fDigi1F[]","",100, 0, 100);
  //      fDigi1F[]->GetXaxis()->SetTitle("");
  //      fDigi1F[]->GetYaxis()->SetTitle("");

  fDigi1F[0] = new TH1F("fDigi1F[0]", "Number of digis per MC-Point", 100, 0, 30);
  fDigi1F[0]->GetXaxis()->SetTitle("number of digis");
  fDigi1F[0]->GetYaxis()->SetTitle("entries");

  fDigi1F[1] = new TH1F("fDigi1F[1]", "Number of MC-Point per Digi", 100, 0, 10);
  fDigi1F[1]->GetXaxis()->SetTitle("number of MC-Points");
  fDigi1F[1]->GetYaxis()->SetTitle("entries");

  //   fDigi2F[] = new TH2F("fDigi2F[]","",100, 0, 100, 100, 0, 100);
  //      fDigi2F[]->GetXaxis()->SetTitle("");
  //      fDigi2F[]->GetYaxis()->SetTitle("");

  fDigi2F[0] =
    new TH2F("fDigi2F[0]", "Distribution of digis on worst spot on the first MVD statio", 100, -4, 0, 100, -2, 5);
  fDigi2F[0]->GetXaxis()->SetTitle("x [cm]");
  fDigi2F[0]->GetYaxis()->SetTitle("y [cm]");

  fDigi2F[1] = new TH2F("fDigi2F[1]", "Distribution of digis on first MVD station", 100, -4, 4, 100, -4, 4);
  fDigi2F[1]->GetXaxis()->SetTitle("x [cm]");
  fDigi2F[1]->GetYaxis()->SetTitle("y [cm]");

  fDigi2F[2] = new TH2F("fDigi2F[2]", "Distribution of digis on second MVD station", 100, -8, 8, 100, 8, 8);
  fDigi2F[2]->GetXaxis()->SetTitle("x [cm]");
  fDigi2F[2]->GetYaxis()->SetTitle("y [cm]");

  fDigi2F[3] = new TH2F("fDigi2F[3]", "Distribution of digis on third MVD station", 100, -12, 12, 100, -12, 12);
  fDigi2F[3]->GetXaxis()->SetTitle("x [cm]");
  fDigi2F[3]->GetYaxis()->SetTitle("y [cm]");

  fDigi2F[4] = new TH2F("fDigi2F[4]", "Distribution of digis on fourth MVD station", 100, -16, 16, 100, -16, 16);
  fDigi2F[4]->GetXaxis()->SetTitle("x [cm]");
  fDigi2F[4]->GetYaxis()->SetTitle("y [cm]");
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMvdQa::SetupHitHistograms()
{
  //   fHits1F[] = new TH1F("fHits1F[]","",100, 0, 100);
  //     fHits1F[]->GetXaxis()->SetTitle("");
  //     fHits1F[]->GetYaxis()->SetTitle("");

  fHits1F[0] = new TH1F("fHits1F[0]", "Hit per Digi", 100, 0, 3);
  fHits1F[0]->GetXaxis()->SetTitle("number of hits");
  fHits1F[0]->GetYaxis()->SetTitle("entries");

  fHits1F[1] = new TH1F("fHits1F[1]", "Digis Per Hit", 100, 0, 12);
  fHits1F[1]->GetXaxis()->SetTitle("number of digis");
  fHits1F[1]->GetYaxis()->SetTitle("entries");

  fHits1F[2] = new TH1F("fHits1F[2]", "Error in x position of reconstructed hit", 1000, -100, 100);
  fHits1F[2]->GetXaxis()->SetTitle("x_hit - x_mc [mu m]");
  fHits1F[2]->GetYaxis()->SetTitle("entries");

  fHits1F[3] = new TH1F("fHits1F[3]", "Error in y position of reconstructed hit", 1000, -100, 100);
  fHits1F[3]->GetXaxis()->SetTitle("y_hit - y_mc [mu m]");
  fHits1F[3]->GetYaxis()->SetTitle("entries");

  fHits1F[4] = new TH1F("fHits1F[4]", "Pull in x position of reconstructed hit", 1000, -20, 20);
  fHits1F[4]->GetXaxis()->SetTitle("x_error / xRes");
  fHits1F[4]->GetYaxis()->SetTitle("entries");

  fHits1F[5] = new TH1F("fHits1F[5]", "Pull in y position of reconstructed hit", 1000, -20, 20);
  fHits1F[5]->GetXaxis()->SetTitle("y_error / yRes");
  fHits1F[5]->GetYaxis()->SetTitle("entries");


  fHits2F[0] =
    new TH2F("fHits2F[0]", "Distribution of Hits in worst spot on the first Station", 100, -2.1, -0.4, 100, -1.5, 1.5);
  fHits2F[0]->GetXaxis()->SetTitle("x [cm]");
  fHits2F[0]->GetYaxis()->SetTitle("y [cm]");

  //   fHits2F[] = new TH2F("fHits2F[]","",100, 0, 100, 100, 0, 100);
  //      fHits2F[]->GetXaxis()->SetTitle("");
  //      fHits2F[]->GetYaxis()->SetTitle("");
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMvdQa::SetupTrackHistograms()
{

  fTracks1F[0] = new TH1F("fTracks1F[0]", "reconstructed tracks in bins of p", 300, 0, 3);
  fTracks1F[0]->GetXaxis()->SetTitle("Momentum [GeV]");
  fTracks1F[0]->GetYaxis()->SetTitle("Entries");

  fTracks1F[1] = new TH1F("fTracks1F[1]", "track reconstruction efficiency in bins of p", 300, 0, 3);
  fTracks1F[1]->GetXaxis()->SetTitle("Momentum [GeV]");
  fTracks1F[1]->GetYaxis()->SetTitle("correct reco / all reco");

  fTracks1F[2] = new TH1F("fTracks1F[2]", "tracks with 4 hits in the mvd", 200, 0, 15);
  fTracks1F[2]->GetXaxis()->SetTitle("Momentum [GeV]");
  fTracks1F[2]->GetYaxis()->SetTitle("Entries");

  fTracks1F[3] = new TH1F("fTracks1F[3]", "tracks with 4 correct hits in the mvd", 200, 0, 15);
  fTracks1F[3]->GetXaxis()->SetTitle("Momentum [GeV]");
  fTracks1F[3]->GetYaxis()->SetTitle("Entries");

  fTracks1F[4] = new TH1F("fTracks1F[4]", "correct reconstructed tracks with 4 hits in the mvd", 200, 0, 15);
  fTracks1F[4]->GetXaxis()->SetTitle("Momentum [GeV]");
  fTracks1F[4]->GetYaxis()->SetTitle("Entries");

  fTracks1F[5] = new TH1F("fTracks1F[5]", "Resolution in x at z = z Mc Vertex", 1000, -0.02, 0.02);
  fTracks1F[5]->GetXaxis()->SetTitle("x [cm]");
  fTracks1F[5]->GetYaxis()->SetTitle("Entries");

  fTracks1F[6] = new TH1F("fTracks1F[6]", "Resolution in y at z = z Mc Vertex", 1000, -0.02, 0.02);
  fTracks1F[6]->GetXaxis()->SetTitle("y [cm]");
  fTracks1F[6]->GetYaxis()->SetTitle("Entries");

  fTracks1F[7] = new TH1F("fTracks1F[7]", "Resolution in x at z = z Mc Vertex, 4 mvd hits", 1000, -0.02, 0.02);
  fTracks1F[7]->GetXaxis()->SetTitle("x [cm]");
  fTracks1F[7]->GetYaxis()->SetTitle("Entries");

  fTracks1F[8] = new TH1F("fTracks1F[8]", "Resolution in y at z = z Mc Vertex, 4 mvd hits", 1000, -0.02, 0.02);
  fTracks1F[8]->GetXaxis()->SetTitle("y [cm]");
  fTracks1F[8]->GetYaxis()->SetTitle("Entries");

  fTracks1F[9] = new TH1F("fTracks1F[9]", "Resolution in x at z = z Mc Vertex, hit in first mvd", 1000, -0.02, 0.02);
  fTracks1F[9]->GetXaxis()->SetTitle("x [cm]");
  fTracks1F[9]->GetYaxis()->SetTitle("Entries");

  fTracks1F[10] = new TH1F("fTracks1F[10]", "Resolution in y at z = z Mc Vertex, hit in first mvd", 1000, -0.02, 0.02);
  fTracks1F[10]->GetXaxis()->SetTitle("y [cm]");
  fTracks1F[10]->GetYaxis()->SetTitle("Entries");

  fTracks1F[11] =
    new TH1F("fTracks1F[11]", "Resolution in x at z = z Mc Vertex, no hit in first mvd", 1000, -0.02, 0.02);
  fTracks1F[11]->GetXaxis()->SetTitle("x [cm]");
  fTracks1F[11]->GetYaxis()->SetTitle("Entries");

  fTracks1F[12] =
    new TH1F("fTracks1F[12]", "Resolution in y at z = z Mc Vertex, no hit in first mvd", 1000, -0.02, 0.02);
  fTracks1F[12]->GetXaxis()->SetTitle("y [cm]");
  fTracks1F[12]->GetYaxis()->SetTitle("Entries");

  fTracks1F[13] =
    new TH1F("fTracks1F[13]", "Resolution in x at z = z Mc Vertex, true hit in first mvd", 1000, -0.02, 0.02);
  fTracks1F[13]->GetXaxis()->SetTitle("x [cm]");
  fTracks1F[13]->GetYaxis()->SetTitle("Entries");

  fTracks1F[14] =
    new TH1F("fTracks1F[14]", "Resolution in y at z = z Mc Vertex, true hit in first mvd", 1000, -0.02, 0.02);
  fTracks1F[14]->GetXaxis()->SetTitle("y [cm]");
  fTracks1F[14]->GetYaxis()->SetTitle("Entries");

  fTracks1F[15] =
    new TH1F("fTracks1F[15]", "Resolution in x at z = z Mc Vertex, wrong hit in first mvd", 1000, -0.02, 0.02);
  fTracks1F[15]->GetXaxis()->SetTitle("x [cm]");
  fTracks1F[15]->GetYaxis()->SetTitle("Entries");

  fTracks1F[16] =
    new TH1F("fTracks1F[16]", "Resolution in y at z = z Mc Vertex, wrong hit in first mvd", 1000, -0.02, 0.02);
  fTracks1F[16]->GetXaxis()->SetTitle("y [cm]");
  fTracks1F[16]->GetYaxis()->SetTitle("Entries");

  fTracks1F[17] =
    new TH1F("fTracks1F[17]", "Chi Squard over NDF of Tracks with wrong attached hit in first Mvd", 100, 0, 10);
  fTracks1F[17]->GetXaxis()->SetTitle("chi^2 / NDF");
  fTracks1F[17]->GetYaxis()->SetTitle("Entries");

  fTracks1F[18] = new TH1F("fTracks1F[18]", "Chi Squard over NDF of Tracks with correct hit in first Mvd", 100, 0, 10);
  fTracks1F[18]->GetXaxis()->SetTitle("chi^2 / NDF");
  fTracks1F[18]->GetYaxis()->SetTitle("Entries");

  fTracks1F[19] = new TH1F("fTracks1F[19]", "Chi Squard over NDF of all Tracks with hit in first Mvd", 100, 0, 10);
  fTracks1F[19]->GetXaxis()->SetTitle("chi^2 / NDF");
  fTracks1F[19]->GetYaxis()->SetTitle("Entries");

  fTracks1F[20] = new TH1F("fTracks1F[20]", "Impactparam Tracks with ChiSq/NDF > 1", 100, 0, 0.10);
  fTracks1F[20]->GetXaxis()->SetTitle("impactparam R [cm]");
  fTracks1F[20]->GetYaxis()->SetTitle("Entries");

  fTracks1F[21] = new TH1F("fTracks1F[21]", "Impactparam Tracks with ChiSq/NDF <= 1", 100, 0, 0.1);
  fTracks1F[21]->GetXaxis()->SetTitle("impactparam R [cm]");
  fTracks1F[21]->GetYaxis()->SetTitle("Entries");

  fTracks1F[22] = new TH1F("fTracks1F[22]",
                           "Resolution in x at z = z Mc Vertex, wrong hit in "
                           "first mvd, Positive Tracks",
                           100, -0.1, 0.1);
  fTracks1F[22]->GetXaxis()->SetTitle("x [cm]");
  fTracks1F[22]->GetYaxis()->SetTitle("Entries");

  fTracks1F[23] = new TH1F("fTracks1F[23]",
                           "Resolution in x at z = z Mc Vertex, wrong hit in "
                           "first mvd, Negative Tracks",
                           100, -0.1, 0.1);
  fTracks1F[23]->GetXaxis()->SetTitle("x [cm]");
  fTracks1F[23]->GetYaxis()->SetTitle("Entries");

  fTracks1F[24] =
    new TH1F("fTracks1F[24]", "Impactparam X Tracks with ChiSq/NDF > 1, wrong hit in first station", 100, -0.10, 0.10);
  fTracks1F[24]->GetXaxis()->SetTitle("x [cm]");
  fTracks1F[24]->GetYaxis()->SetTitle("Entries");

  fTracks1F[25] =
    new TH1F("fTracks1F[25]", "Impactparam x Tracks with ChiSq/NDF <= 1, wrong hit in first station", 100, -0.10, 0.10);
  fTracks1F[25]->GetXaxis()->SetTitle("x [cm]");
  fTracks1F[25]->GetYaxis()->SetTitle("Entries");

  fTracks1F[26] = new TH1F("fTracks1F[26]",
                           "Impactparam X Tracks with momentum >= 1 GeV, wrong "
                           "hit in first station, p Tracks",
                           100, -0.10, 0.10);
  fTracks1F[26]->GetXaxis()->SetTitle("x [cm]");
  fTracks1F[26]->GetYaxis()->SetTitle("Entries");

  fTracks1F[27] = new TH1F("fTracks1F[27]",
                           "Impactparam X Tracks with momentum < 1 GeV, wrong "
                           "hit in first station, p Tracks",
                           100, -0.10, 0.10);
  fTracks1F[27]->GetXaxis()->SetTitle("x [cm]");
  fTracks1F[27]->GetYaxis()->SetTitle("Entries");

  fTracks1F[28] = new TH1F("fTracks1F[28]",
                           "Impactparam X Tracks with momentum >= 1 GeV, wrong "
                           "hit in first station, n Tracks",
                           100, -0.10, 0.10);
  fTracks1F[28]->GetXaxis()->SetTitle("x [cm]");
  fTracks1F[28]->GetYaxis()->SetTitle("Entries");

  fTracks1F[29] = new TH1F("fTracks1F[29]",
                           "Impactparam X Tracks with momentum < 1 GeV, wrong "
                           "hit in first station, n Tracks",
                           100, -0.10, 0.10);
  fTracks1F[29]->GetXaxis()->SetTitle("x [cm]");
  fTracks1F[29]->GetYaxis()->SetTitle("Entries");

  fTracks1F[30] =
    new TH1F("fTracks1F[30]", "mc x position of correct hit - x pos of wrongly attached hit", 100, -0.01, 0.01);
  fTracks1F[30]->GetXaxis()->SetTitle("x [mu m]");
  fTracks1F[30]->GetYaxis()->SetTitle("Entries");

  fTracks1F[31] =
    new TH1F("fTracks1F[31]", "mc y position of correct hit - y pos of wrongly attached hit", 100, -0.01, 0.01);
  fTracks1F[31]->GetXaxis()->SetTitle("y [mu m]");
  fTracks1F[31]->GetYaxis()->SetTitle("Entries");

  fTracks1F[32] =
    new TH1F("fTracks1F[32]", "Resolution in x at z = z Mc Vertex, 4 mvd hits, trueOverAll == 1", 1000, -0.02, 0.02);
  fTracks1F[32]->GetXaxis()->SetTitle("x [cm]");
  fTracks1F[32]->GetYaxis()->SetTitle("Entries");

  fTracks1F[33] =
    new TH1F("fTracks1F[33]", "Resolution in y at z = z Mc Vertex, 4 mvd hits, trueOverAll == 1", 1000, -0.02, 0.02);
  fTracks1F[33]->GetXaxis()->SetTitle("y [cm]");
  fTracks1F[33]->GetYaxis()->SetTitle("Entries");


  fTracks2F[0] = new TH2F("fTracks2F[0]", "Momentumresolution all Tracks", 200, 0, 15, 100, -0.2, 0.2);
  fTracks2F[0]->GetXaxis()->SetTitle("MC-Momentum [GeV]");
  fTracks2F[0]->GetYaxis()->SetTitle("MC-Momentum - Reco-Momentum [GeV]");

  fTracks2F[1] =
    new TH2F("fTracks2F[1]", "Momentumresolution correct reconstructed Tracks", 200, 0, 15, 100, -0.2, 0.2);
  fTracks2F[1]->GetXaxis()->SetTitle("MC-Momentum [GeV]");
  fTracks2F[1]->GetYaxis()->SetTitle("MC-Momentum - Reco-Momentum [GeV]");

  fTracks2F[2] =
    new TH2F("fTracks2F[2]", "Momentumresolution Tracks with 4 hits in the mvd", 200, 0, 15, 100, -0.2, 0.2);
  fTracks2F[2]->GetXaxis()->SetTitle("MC-Momentum [GeV]");
  fTracks2F[2]->GetYaxis()->SetTitle("MC-Momentum - Reco-Momentum [GeV]");

  fTracks2F[3] =
    new TH2F("fTracks2F[3]", "Momentumresolution Tracks with 4 correct hits in the mvd", 200, 0, 15, 100, -0.2, 0.2);
  fTracks2F[3]->GetXaxis()->SetTitle("MC-Momentum [GeV]");
  fTracks2F[3]->GetYaxis()->SetTitle("MC-Momentum - Reco-Momentum [GeV]");

  fTracks2F[4] = new TH2F("fTracks2F[4]", "Momentumresolution correct reconstructed Tracks with 4 hits in the mvd", 200,
                          0, 15, 100, -0.2, 0.2);
  fTracks2F[4]->GetXaxis()->SetTitle("MC-Momentum [GeV]");
  fTracks2F[4]->GetYaxis()->SetTitle("MC-Momentum - Reco-Momentum [GeV]");

  fTracks2F[5] =
    new TH2F("fTracks2F[5]", "Momentumresolution Tracks with hit in the first mvd station", 200, 0, 15, 100, -0.2, 0.2);
  fTracks2F[5]->GetXaxis()->SetTitle("MC-Momentum [GeV]");
  fTracks2F[5]->GetYaxis()->SetTitle("MC-Momentum - Reco-Momentum [GeV]");

  fTracks2F[6] = new TH2F("fTracks2F[6]", "Momentumresolution Tracks with no hit in the first mvd station", 200, 0, 15,
                          100, -0.2, 0.2);
  fTracks2F[6]->GetXaxis()->SetTitle("MC-Momentum [GeV]");
  fTracks2F[6]->GetYaxis()->SetTitle("MC-Momentum - Reco-Momentum [GeV]");

  fTracks2F[7] =
    new TH2F("fTracks2F[7]", "Position of wrong attached Hit in first Mvd Station", 200, -3, 3, 200, -3, 3);
  fTracks2F[7]->GetXaxis()->SetTitle("x [cm]");
  fTracks2F[7]->GetYaxis()->SetTitle("y [cm]");

  fTracks2F[8] = new TH2F("fTracks2F[8]",
                          "Position of wrong attached Hit in first Mvd "
                          "Station, with good ChiSq over NDF",
                          200, -3, 3, 200, -3, 3);
  fTracks2F[8]->GetXaxis()->SetTitle("x [cm]");
  fTracks2F[8]->GetYaxis()->SetTitle("y [cm]");

  fTracks2F[9] = new TH2F("fTracks2F[9]",
                          "Propability to pick up the wrong hit on the first "
                          "MVD Station 2 bins cut at 1.5 GeV",
                          2, 0, 1, 2, 0, 1);
  fTracks2F[9]->GetXaxis()->SetBinLabel(1, "0 - 1 GeV");
  fTracks2F[9]->GetXaxis()->SetBinLabel(2, "1 - 1.5 GeV");
  fTracks2F[9]->GetYaxis()->SetBinLabel(1, "wrong Hit");
  fTracks2F[9]->GetYaxis()->SetBinLabel(2, "correct Hit");

  fTracks2F[10] = new TH2F("fTracks2F[10]", "Vertexresolution of wrong attached Hit in first Mvd Station", 200, -0.04,
                           0.04, 200, -0.01, 0.01);
  fTracks2F[10]->GetXaxis()->SetTitle("X at glVertex [cm]");
  fTracks2F[10]->GetYaxis()->SetTitle("Y at glVertex [cm] ");

  fTracks2F[11] = new TH2F("fTracks2F[11]",
                           "Propability to pick up the wrong hit on the first "
                           "MVD Station 2 bins cut at 0.5 , 1 GeV",
                           3, 0, 1.5, 2, 0, 1);
  fTracks2F[11]->GetXaxis()->SetBinLabel(1, "0 - 0.5 GeV");
  fTracks2F[11]->GetXaxis()->SetBinLabel(2, "0.5 - 1 GeV");
  fTracks2F[11]->GetXaxis()->SetBinLabel(3, "1 - 1.5 GeV");
  fTracks2F[11]->GetYaxis()->SetBinLabel(1, "wrong Hit");
  fTracks2F[11]->GetYaxis()->SetBinLabel(2, "correct Hit");
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMvdQa::Exec(Option_t* /*opt*/)
{
  fEventNumber++;
  if (fEventNumber % 10 == 0) LOG(info) << "CbmMvdQa is running Event " << fEventNumber;

  if (useMcQa) ExecMCQa();
  if (useDigiQa) ExecDigiQa();
  if (useHitQa) ExecHitQa();
  if (useTrackQa) ExecTrackQa();
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMvdQa::ExecMCQa()
{
  Int_t nrMcPoints = fMcPoints->GetEntriesFast();

  for (Int_t i = 0; i < nrMcPoints; ++i) {
    CbmMvdPoint* curMc = (CbmMvdPoint*) fMcPoints->At(i);
    if (curMc->GetZ() < fFirstMvdPos + 1) { fMC2F[0]->Fill(curMc->GetX(), curMc->GetY()); }
  }
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMvdQa::ExecDigiQa()
{
  Int_t nrDigis    = fMvdDigis->GetEntriesFast();
  Int_t nrSensor   = fDetector->GetSensorArraySize();
  Int_t nrMcPoints = fMcPoints->GetEntriesFast();

  Int_t digisPerMcPoints[nrMcPoints];  // = {0};
  Int_t McPointsPerDigi[nrDigis];      // = {0};

  for (Int_t i = 0; i < nrDigis; i++) {
    CbmMvdDigi* curDigi    = (CbmMvdDigi*) fMvdDigis->At(i);
    CbmMatch* curMatch     = (CbmMatch*) fMvdDigiMatchArray->At(i);
    Int_t nrOfContributors = curMatch->GetNofLinks();
    for (Int_t j = 0; j < nrOfContributors; j++) {
      digisPerMcPoints[curMatch->GetLink(j).GetIndex()]++;
    }
    McPointsPerDigi[i] = nrOfContributors;

    for (Int_t n = 0; n < nrSensor; n++) {
      CbmMvdSensor* curSens = fDetector->GetSensor(n);
      Double_t lab[3]       = {0., 0., 0.};
      if (curSens->GetDetectorID() == curDigi->GetDetectorId()) {
        curSens->PixelToTop(curDigi->GetPixelX(), curDigi->GetPixelY(), lab);
        if (curSens->GetZ() < 6) fDigi2F[1]->Fill(lab[0], lab[1]);
        else if (curSens->GetZ() < 11)
          fDigi2F[2]->Fill(lab[0], lab[1]);
        else if (curSens->GetZ() < 16)
          fDigi2F[3]->Fill(lab[0], lab[1]);
        else
          fDigi2F[4]->Fill(lab[0], lab[1]);

        if (lab[0] > -2 && lab[0] <= -0.5 && lab[1] >= -1.5 && lab[1] <= 1.5) { fDigi2F[0]->Fill(lab[0], lab[1]); }
      }
    }
  }

  for (Int_t i = 0; i < nrMcPoints; i++)
    fDigi1F[0]->Fill(digisPerMcPoints[i]);

  for (Int_t i = 0; i < nrDigis; i++)
    fDigi1F[1]->Fill(McPointsPerDigi[i]);
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMvdQa::ExecHitQa()
{
  Int_t nrHits = fMvdHits->GetEntriesFast();
  // Int_t nrDigis = fMvdDigis->GetEntriesFast();

  Int_t DigisPerHit[nrHits];

  Float_t xRes = 3.8;
  Float_t yRes = 4.8;

  for (Int_t k = 0; k < nrHits; k++) {
    CbmMvdHit* curHit  = (CbmMvdHit*) fMvdHits->At(k);
    CbmMatch* curMatch = (CbmMatch*) fMvdHitMatchArray->At(k);
    CbmMvdPoint* curMc = (CbmMvdPoint*) fMcPoints->At(curMatch->GetMatchedLink().GetIndex());
    Float_t xErr       = 10000 * (curHit->GetX() - curMc->GetX());
    Float_t yErr       = 10000 * (curHit->GetY() - curMc->GetY());


    CbmMvdCluster* curCluster = (CbmMvdCluster*) fMvdCluster->At(curHit->GetClusterIndex());
    DigisPerHit[k]            = curCluster->GetTotalDigisInCluster();

    if (curHit->GetX() > -2 && curHit->GetX() <= -0.5 && curHit->GetY() >= -1.5 && curHit->GetY() <= 1.5) {
      fHits2F[0]->Fill(curHit->GetX(), curHit->GetY());
    }

    fHits1F[2]->Fill(xErr);
    fHits1F[3]->Fill(yErr);
    fHits1F[4]->Fill(xErr / xRes);
    fHits1F[5]->Fill(yErr / yRes);
  }

  for (Int_t i = 0; i < nrHits; i++)
    fHits1F[1]->Fill(DigisPerHit[i]);
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMvdQa::ExecTrackQa()
{

  CbmStsTrack* stsTrack;

  Int_t nTracks       = fStsTrackArray->GetEntriesFast();
  Int_t nMcTracks     = fListMCTracks->GetEntriesFast();
  Int_t nGlobalTracks = fGlobalTrackArray->GetEntriesFast();
  Int_t mcMatchId;
  Int_t nHitsMvd;

  CbmTrackMatchNew* trackMatch;
  CbmMCTrack* mcTrack;
  CbmGlobalTrack* glTrack;

  Bool_t hasHitFirst, hasHitFirstTrue;

  Float_t glX, glY, ChiSqOverNDF, glP, mcP, trueOverAll, glQP;
  Float_t mcPosFirst[3] = {0};
  Float_t hitFirst[2];

  LOG(debug) << " CbmMvdQa: Entries: " << nTracks << " StsTracks from " << nMcTracks << " McTracks";

  // --- Loop over reconstructed tracks ---
  for (Int_t itr = 0; itr < nGlobalTracks; itr++) {
    glTrack = (CbmGlobalTrack*) fGlobalTrackArray->At(itr);

    stsTrack   = (CbmStsTrack*) fStsTrackArray->At(glTrack->GetStsTrackIndex());
    trackMatch = (CbmTrackMatchNew*) fStsTrackMatches->At(glTrack->GetStsTrackIndex());
    mcMatchId  = trackMatch->GetMatchedLink().GetIndex();
    if (mcMatchId > -1) mcTrack = (CbmMCTrack*) fListMCTracks->At(mcMatchId);
    else
      continue;

    SetMatches(mcMatchId, stsTrack);
    hasHitFirst     = HasHitFirstMvd(stsTrack);
    hasHitFirstTrue = HasHitFirstTrue(mcMatchId, stsTrack);

    glX          = glTrack->GetParamVertex()->GetX();
    glY          = glTrack->GetParamVertex()->GetY();
    ChiSqOverNDF = stsTrack->GetChiSq() / stsTrack->GetNDF();
    glQP         = stsTrack->GetParamFirst()->GetQp();
    glP          = fabs(1 / glQP);
    mcP          = mcTrack->GetP();
    GetFirstMCPos(stsTrack, mcPosFirst);
    GetFirstMvdHitPos(stsTrack, hitFirst);

    trueOverAll = trackMatch->GetTrueOverAllHitsRatio();
    nHitsMvd    = stsTrack->GetNofMvdHits();

    if (hasHitFirst) {
      if (mcP < 1.5) {
        fnrTrackslowP++;
        if (mcP >= 1) fhigh++;
        else if (mcP >= 0.5)
          fmid++;
        else
          flow++;
      }
      else
        fnrTracksHighP++;
    }


    fTracks1F[0]->Fill(glP, 1);
    if (trueOverAll == 1.) fTracks1F[1]->Fill(glP, 1);

    if (nHitsMvd == 4) {
      fTracks1F[2]->Fill(glP, 1);
      if (fMvdRecoRatio == 1.) fTracks1F[3]->Fill(glP, 1);
      if (trueOverAll == 1.) {
        fTracks1F[4]->Fill(glP, 1);
        fTracks1F[32]->Fill(glX);
        fTracks1F[33]->Fill(glY);
      }
    }

    fTracks1F[5]->Fill(glX);
    fTracks1F[6]->Fill(glY);

    if (nHitsMvd == 4) {
      fTracks1F[7]->Fill(glX);
      fTracks1F[8]->Fill(glY);
    }

    if (hasHitFirst) {
      fTracks1F[9]->Fill(glX);
      fTracks1F[10]->Fill(glY);
      fTracks1F[19]->Fill(ChiSqOverNDF);
    }
    else {
      fTracks1F[11]->Fill(glX);
      fTracks1F[12]->Fill(glY);
    }

    if (hasHitFirstTrue) {
      fTracks1F[13]->Fill(glX);
      fTracks1F[14]->Fill(glY);
      fTracks1F[18]->Fill(ChiSqOverNDF);
    }

    if (!hasHitFirstTrue && hasHitFirst) {
      fTracks1F[15]->Fill(glX);
      fTracks1F[16]->Fill(glY);
      fTracks2F[10]->Fill(glX, glY);
      fTracks1F[17]->Fill(ChiSqOverNDF);

      fTracks1F[30]->Fill(mcPosFirst[0] - hitFirst[0]);
      fTracks1F[31]->Fill(mcPosFirst[1] - hitFirst[1]);

      if (glQP >= 0) {
        fTracks1F[22]->Fill(glX);
        if (mcP >= 1) fTracks1F[26]->Fill(glX);
        else
          fTracks1F[27]->Fill(glX);
      }
      if (glQP < 0) {
        fTracks1F[23]->Fill(glX);
        if (mcP >= 1) fTracks1F[28]->Fill(glX);
        else
          fTracks1F[29]->Fill(glX);
      }
      if (ChiSqOverNDF > 1) fTracks1F[24]->Fill(glX);
      else
        fTracks1F[25]->Fill(glX);
    }

    if (ChiSqOverNDF > 1) { fTracks1F[20]->Fill(sqrt((glX * glX) + (glY * glY))); }
    else {
      fTracks1F[21]->Fill(sqrt((glX * glX) + (glY * glY)));
    }


    fTracks2F[0]->Fill(mcP, mcP - glP);
    if (trueOverAll == 1.) fTracks2F[1]->Fill(mcP, mcP - glP);

    if (nHitsMvd == 4) {
      fTracks2F[2]->Fill(mcP, mcP - glP);
      if (fMvdRecoRatio == 1.) fTracks2F[3]->Fill(mcP, mcP - glP);
      if (trueOverAll == 1.) fTracks2F[4]->Fill(mcP, mcP - glP);
    }

    if (hasHitFirst) fTracks2F[5]->Fill(mcP, mcP - glP);
    else
      fTracks2F[6]->Fill(mcP, mcP - glP);

    if (!hasHitFirstTrue && hasHitFirst) {
      fTracks2F[7]->Fill(hitFirst[0], hitFirst[1]);
      if (ChiSqOverNDF <= 1) fTracks2F[8]->Fill(hitFirst[0], hitFirst[1]);
      if (mcP < 1.5) {
        fTracks2F[9]->Fill(0.1, 0.1);
        if (mcP >= 1) fTracks2F[11]->Fill(1.25, 0.1);
        else if (mcP >= 0.5)
          fTracks2F[11]->Fill(0.75, 0.1);
        else
          fTracks2F[11]->Fill(0.25, 0.1);
      }
      else
        fTracks2F[9]->Fill(0.70, 0.1);
    }

    if (hasHitFirstTrue) {
      if (mcP < 1.5) {
        fTracks2F[9]->Fill(0.1, 0.70);
        if (mcP >= 1) fTracks2F[11]->Fill(1.25, 0.7);
        else if (mcP >= 0.5)
          fTracks2F[11]->Fill(0.75, 0.7);
        else
          fTracks2F[11]->Fill(0.25, 0.7);
      }
      else
        fTracks2F[9]->Fill(0.70, 0.70);
    }
  }
}
// -------------------------------------------------------------------------
void CbmMvdQa::GetFirstMvdHitPos(CbmStsTrack* stsTrack, Float_t* hitPos)
{
  Int_t nrMvdHits = stsTrack->GetNofMvdHits();
  for (Int_t itr = 0; itr < nrMvdHits; itr++) {
    CbmMvdHit* curHit = (CbmMvdHit*) fMvdHits->At(stsTrack->GetMvdHitIndex(itr));
    if (curHit->GetZ() < fFirstMvdPos + 1) {
      hitPos[0] = curHit->GetX();
      hitPos[1] = curHit->GetY();
      return;
    }
  }
}
// -------------------------------------------------------------------------
Bool_t CbmMvdQa::HasHitFirstMvd(CbmStsTrack* stsTrack)
{
  Int_t nrMvdHits = stsTrack->GetNofMvdHits();
  for (Int_t itr = 0; itr < nrMvdHits; itr++) {
    CbmMvdHit* curHit = (CbmMvdHit*) fMvdHits->At(stsTrack->GetMvdHitIndex(itr));
    if (curHit->GetZ() < fFirstMvdPos + 1) return kTRUE;
  }

  return kFALSE;
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
Bool_t CbmMvdQa::HasHitFirstTrue(Int_t MCtrackID, CbmStsTrack* stsTrack)
{
  Int_t nrOfMvdHits        = stsTrack->GetNofMvdHits();
  Int_t nrOfLinks          = 0;
  Int_t mcTrackId          = 0;
  const CbmMvdPoint* point = NULL;
  for (Int_t iHit = 0; iHit < nrOfMvdHits; iHit++) {
    CbmMatch* mvdMatch = (CbmMatch*) fMvdHitMatchArray->At(stsTrack->GetMvdHitIndex(iHit));
    if (mvdMatch) { nrOfLinks = mvdMatch->GetNofLinks(); }
    else {
      continue;  // any kind of error in the matching
    }
    for (Int_t iLink = 0; iLink < nrOfLinks; iLink++) {
      Int_t pointIndex = mvdMatch->GetLink(iLink).GetIndex();
      if (pointIndex < fMcPoints->GetEntriesFast()) point = (CbmMvdPoint*) fMcPoints->At(pointIndex);
      if (NULL == point) {
        continue;  //delta or background event
      }
      else
        mcTrackId = point->GetTrackID();
      if (mcTrackId == MCtrackID && point->GetZOut() < fFirstMvdPos + 1) { return kTRUE; }
    }
  }

  return kFALSE;
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMvdQa::GetFirstMCPos(CbmStsTrack* stsTrack, Float_t* pos)
{
  Int_t nrOfMvdHits        = stsTrack->GetNofMvdHits();
  Int_t nrOfLinks          = 0;
  const CbmMvdPoint* point = NULL;
  for (Int_t iHit = 0; iHit < nrOfMvdHits; iHit++) {
    CbmMatch* mvdMatch = (CbmMatch*) fMvdHitMatchArray->At(stsTrack->GetMvdHitIndex(iHit));
    if (mvdMatch) { nrOfLinks = mvdMatch->GetNofLinks(); }
    else {
      continue;  // any kind of error in the matching
    }
    for (Int_t iLink = 0; iLink < nrOfLinks; iLink++) {
      Int_t pointIndex = mvdMatch->GetLink(iLink).GetIndex();
      if (pointIndex < fMcPoints->GetEntriesFast()) point = (CbmMvdPoint*) fMcPoints->At(pointIndex);
      if (NULL == point) {
        continue;  //delta or background event
      }
      else if (point->GetZOut() < fFirstMvdPos + 1) {
        pos[0] = point->GetX();
        pos[1] = point->GetY();
        pos[2] = point->GetZOut();
        return;
      }
    }
  }
}
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
void CbmMvdQa::SetMatches(Int_t MCtrackID, CbmStsTrack* stsTrack)
{

  Int_t nrOfMvdHits    = stsTrack->GetNofMvdHits();
  Int_t mcTrackId      = 0;
  Float_t falseCounter = 0;
  Bool_t hasTrack;
  Int_t nrOfLinks          = 0;
  fMvdRecoRatio            = 0.;
  const CbmMvdPoint* point = NULL;
  for (Int_t iHit = 0; iHit < nrOfMvdHits; iHit++) {
    hasTrack           = kFALSE;
    CbmMatch* mvdMatch = (CbmMatch*) fMvdHitMatchArray->At(stsTrack->GetMvdHitIndex(iHit));
    if (mvdMatch) {
      nrOfLinks = mvdMatch->GetNofLinks();
      if (nrOfLinks > 1) fnrOfMergedHits++;
    }
    else {
      continue;  // any kind of error in the matching
    }
    for (Int_t iLink = 0; iLink < nrOfLinks; iLink++) {
      Int_t pointIndex = mvdMatch->GetLink(iLink).GetIndex();
      if (pointIndex < fMcPoints->GetEntriesFast()) point = (CbmMvdPoint*) fMcPoints->At(pointIndex);
      if (NULL == point) {
        continue;  //delta or background event
      }
      else
        mcTrackId = point->GetTrackID();
      if (mcTrackId == MCtrackID) { hasTrack = kTRUE; }
    }
    if (!hasTrack) falseCounter++;
  }
  Float_t counter = (Float_t) falseCounter / (Float_t) nrOfMvdHits;
  fMvdRecoRatio   = (Float_t) 1.0 - (Float_t) counter;


  if (useMcQa) fMC1F[0]->Fill(fMvdRecoRatio);
}
// -------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
void CbmMvdQa::Finish()
{
  foutFile->cd();

  if (useMcQa) FinishMCQa();
  if (useDigiQa) FinishDigiQa();
  if (useHitQa) FinishHitQa();
  if (useTrackQa) FinishTrackQa();

  foutFile->Write();
}
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
void CbmMvdQa::FinishMCQa()
{
  if (fdraw) {
    TCanvas* mcCanvas1 = new TCanvas();
    mcCanvas1->cd();
    fMC1F[0]->Draw();
  }

  fMC1F[0]->Write();
}
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
void CbmMvdQa::FinishDigiQa()
{
  if (fdraw) {
    TCanvas* digiCanvas1a = new TCanvas();
    digiCanvas1a->cd();
    fDigi1F[0]->Draw();

    TCanvas* digiCanvas1b = new TCanvas();
    digiCanvas1b->cd();
    fDigi1F[1]->Draw();
  }

  fDigi1F[0]->Write();
  fDigi1F[1]->Write();
}
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
void CbmMvdQa::FinishHitQa()
{
  if (fdraw) {
    //    TCanvas* hitCanvas1a = new TCanvas();
    //    fHits1F[0]->Draw();

    TCanvas* hitCanvas1b = new TCanvas();
    hitCanvas1b->cd();
    fHits1F[1]->Draw();

    TCanvas* hitCanvas1c = new TCanvas();
    hitCanvas1c->cd();
    fHits1F[2]->Draw();

    TCanvas* hitCanvas1d = new TCanvas();
    hitCanvas1d->cd();
    fHits1F[3]->Draw("COL");

    TCanvas* hitCanvas1e = new TCanvas();
    hitCanvas1e->cd();
    fHits1F[4]->Draw();

    TCanvas* hitCanvas1f = new TCanvas();
    hitCanvas1f->cd();
    fHits1F[5]->Draw();


    TCanvas* hitCanvas2a = new TCanvas();
    hitCanvas2a->cd();
    fHits2F[0]->Draw("COL");
  }

  for (Int_t i = 0; i < 6; i++) {
    fHits1F[i]->Write();
  }


  fHits2F[0]->Write();
}
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
void CbmMvdQa::FinishTrackQa()
{

  fTracks1F[1]->Divide(fTracks1F[0]);

  if (fdraw) {
    TCanvas* TrackCanvas1 = new TCanvas();
    TrackCanvas1->Divide(3, 2);
    TrackCanvas1->cd(1);
    fTracks1F[0]->Draw();
    TrackCanvas1->cd(2);
    fTracks1F[1]->Draw();
    TrackCanvas1->cd(3);
    fTracks1F[2]->Draw();
    TrackCanvas1->cd(4);
    fTracks1F[3]->Draw();
    TrackCanvas1->cd(5);
    fTracks1F[4]->Draw();

    TCanvas* TrackCanvas1b = new TCanvas();
    TrackCanvas1b->Divide(2, 2);
    TrackCanvas1b->cd(1);
    fTracks1F[5]->Draw();
    TrackCanvas1b->cd(2);
    fTracks1F[6]->Draw();
    TrackCanvas1b->cd(3);
    fTracks1F[7]->Draw();
    TrackCanvas1b->cd(4);
    fTracks1F[8]->Draw();

    TCanvas* TrackCanvas1c = new TCanvas();
    TrackCanvas1c->Divide(2, 2);
    TrackCanvas1c->cd(1);
    fTracks1F[9]->Draw();
    TrackCanvas1c->cd(2);
    fTracks1F[10]->Draw();
    TrackCanvas1c->cd(3);
    fTracks1F[11]->Draw();
    TrackCanvas1c->cd(4);
    fTracks1F[12]->Draw();

    TCanvas* TrackCanvas1d = new TCanvas();
    TrackCanvas1d->Divide(2, 2);
    TrackCanvas1d->cd(1);
    fTracks1F[13]->Draw();
    TrackCanvas1d->cd(2);
    fTracks1F[14]->Draw();
    TrackCanvas1d->cd(3);
    fTracks1F[15]->Draw();
    TrackCanvas1d->cd(4);
    fTracks1F[16]->Draw();

    TCanvas* TrackCanvas1e = new TCanvas();
    TrackCanvas1e->Divide(2, 2);
    TrackCanvas1e->cd(1);
    fTracks1F[17]->Draw();
    TrackCanvas1e->cd(2);
    fTracks1F[18]->Draw();
    TrackCanvas1e->cd(3);
    fTracks1F[19]->Draw();
    TrackCanvas1e->cd(4);
    //fTracks1F[16]->Draw();

    TCanvas* TrackCanvas1f = new TCanvas();
    TrackCanvas1f->Divide(2, 2);
    TrackCanvas1f->cd(1);
    fTracks1F[20]->Draw();
    TrackCanvas1f->cd(2);
    fTracks1F[21]->Draw();
    TrackCanvas1f->cd(3);
    //fTracks1F[19]->Draw();
    TrackCanvas1f->cd(4);
    //fTracks1F[16]->Draw();

    TCanvas* TrackCanvas1g = new TCanvas();
    TrackCanvas1g->Divide(2, 2);
    TrackCanvas1g->cd(1);
    fTracks1F[22]->Draw();
    TrackCanvas1g->cd(2);
    fTracks1F[23]->Draw();
    TrackCanvas1g->cd(3);
    fTracks1F[24]->Draw();
    TrackCanvas1g->cd(4);
    fTracks1F[25]->Draw();

    TCanvas* TrackCanvas1h = new TCanvas();
    TrackCanvas1h->Divide(2, 2);
    TrackCanvas1h->cd(1);
    fTracks1F[26]->Draw();
    TrackCanvas1h->cd(2);
    fTracks1F[27]->Draw();
    TrackCanvas1h->cd(3);
    fTracks1F[28]->Draw();
    TrackCanvas1h->cd(4);
    fTracks1F[29]->Draw();

    TCanvas* TrackCanvas1i = new TCanvas();
    TrackCanvas1i->Divide(2, 2);
    TrackCanvas1i->cd(1);
    fTracks1F[30]->Draw();
    TrackCanvas1i->cd(2);
    fTracks1F[31]->Draw();
  }

  for (Int_t k = 0; k < f1FSize; k++) {
    fTracks1F[k]->Write();
  }

  fTracks2F[9]->SetBinContent(1, 1, 100 * (fTracks2F[9]->GetBinContent(1, 1) / fnrTrackslowP));
  fTracks2F[9]->SetBinContent(1, 2, 100 * (fTracks2F[9]->GetBinContent(1, 2) / fnrTrackslowP));
  fTracks2F[9]->SetBinContent(2, 1, 100 * (fTracks2F[9]->GetBinContent(2, 1) / fnrTracksHighP));
  fTracks2F[9]->SetBinContent(2, 2, 100 * (fTracks2F[9]->GetBinContent(2, 2) / fnrTracksHighP));

  fTracks2F[11]->SetBinContent(1, 1, 100 * (fTracks2F[11]->GetBinContent(1, 1) / flow));
  fTracks2F[11]->SetBinContent(1, 2, 100 * (fTracks2F[11]->GetBinContent(1, 2) / flow));
  fTracks2F[11]->SetBinContent(2, 1, 100 * (fTracks2F[11]->GetBinContent(2, 1) / fmid));
  fTracks2F[11]->SetBinContent(2, 2, 100 * (fTracks2F[11]->GetBinContent(2, 2) / fmid));
  fTracks2F[11]->SetBinContent(3, 1, 100 * (fTracks2F[11]->GetBinContent(3, 1) / fhigh));
  fTracks2F[11]->SetBinContent(3, 2, 100 * (fTracks2F[11]->GetBinContent(3, 2) / fhigh));

  if (fdraw) {
    TCanvas* TrackCanvas2 = new TCanvas();
    TrackCanvas2->Divide(3, 2);
    TrackCanvas2->cd(1);
    fTracks2F[0]->Draw("colz");
    TrackCanvas2->cd(2);
    fTracks2F[1]->Draw("colz");
    TrackCanvas2->cd(3);
    fTracks2F[2]->Draw("colz");
    TrackCanvas2->cd(4);
    fTracks2F[3]->Draw("colz");
    TrackCanvas2->cd(5);
    fTracks2F[4]->Draw("colz");


    TCanvas* TrackCanvas2b = new TCanvas();
    TrackCanvas2b->Divide(2, 2);
    TrackCanvas2b->cd(1);
    fTracks2F[5]->Draw("colz");
    TrackCanvas2b->cd(2);
    fTracks2F[6]->Draw("colz");


    TCanvas* BadTracks = new TCanvas();
    BadTracks->Divide(2, 2);
    BadTracks->cd(1);
    fTracks2F[7]->Draw("colz");
    BadTracks->cd(2);
    fTracks2F[8]->Draw("colz");
    BadTracks->cd(3);
    fTracks2F[9]->DrawCopy("TEXT");
    BadTracks->cd(4);
    fTracks2F[10]->Draw("colz");

    fTracks2F[9]->SetContour(10);
    fTracks2F[11]->SetContour(10);

    gStyle->SetPaintTextFormat("4.1f %");

    TCanvas* asd = new TCanvas();
    asd->cd();
    fTracks2F[9]->SetMarkerSize(4);
    fTracks2F[9]->Draw("TEXT COLZ");
    TCanvas* asd2 = new TCanvas();
    asd2->cd();
    fTracks2F[11]->SetMarkerSize(4);
    fTracks2F[11]->Draw("TEXT COLZ");
  }


  for (Int_t i = 0; i < f2FSize; i++) {
    fTracks2F[i]->Write();
  }
}
//-----------------------------------------------------------------------------------------


/*
// -------------------------------------------------------------------------
Double_t  CbmMvdQa::GetImpactParameterRadius( CbmLitTrackParam t ){

    Double_t imx  = t.GetX();
    Double_t imy  = t.GetY();
    Double_t imr  = sqrt(imx*imx + imy*imy);

   return  imr;
}
// -------------------------------------------------------------------------


//--------------------------------------------------------------------------
Double_t  CbmMvdQa::GetImpactParameterX( CbmLitTrackParam t ){

Double_t imx  = t.GetX();

   return  imx;
}
// -------------------------------------------------------------------------


//--------------------------------------------------------------------------
Double_t  CbmMvdQa::GetImpactParameterY( CbmLitTrackParam t ){

    Double_t imy  = t.GetY();

   return  imy;
}
// -------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------
Double_t  CbmMvdQa::GetTransverseMomentum( CbmLitTrackParam t ){

const std::vector<double> state= t.GetStateVector();

Float_t pz   = GetMomentumZ(t);
Float_t px = state[2] * pz;
Float_t py = state[3] * pz;

Double_t pt = sqrt(px * px + py * py);

    return  pt;
}
//-----------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------
Double_t  CbmMvdQa::GetMomentum( CbmLitTrackParam t ){

    Float_t p = fabs(1.F / t.GetQp());

    return  p;
}
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
Double_t  CbmMvdQa::GetMomentumZ( CbmLitTrackParam t ){

   const std::vector<double> state= t.GetStateVector();

Float_t pz   = sqrt(GetMomentum(t) * GetMomentum(t) / (state[2] * state[2] + state[3] * state[3] + 1));

    return  pz;
}
//-----------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------
Double_t  CbmMvdQa::GetAngle( CbmLitTrackParam t ){

    Float_t angle_rad = std::atan(GetTransverseMomentum(t) / GetMomentumZ(t));
    Float_t rad_to_grad = ( 180 / TMath::Pi() );
    Float_t angle_grad = angle_rad * rad_to_grad;
//cout << endl << "calc angle of " << angle_grad << " from rad angle " << angle_rad << endl;
    return  angle_grad;
}
//-----------------------------------------------------------------------------------------
 */
//-----------------------------------------------------------------------------------------
ClassImp(CbmMvdQa);
