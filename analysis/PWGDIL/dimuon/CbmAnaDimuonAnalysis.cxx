//----------------------------------------
//
// 2019 A. Senger a.senger@gsi.de
//
//----------------------------------------
#define PBINNING 100, 0., 20.
#define THETABINNING 180, 0., 180.
#define PTBINNING 100, 0., 10.
#define YBINNING 100, -2., 6.
#define MBINNING 400, 0., 4.

#include "CbmAnaDimuonAnalysis.h"
#include "CbmAnaMuonCandidate.h"
#include "CbmGlobalTrack.h"
#include "CbmGlobalTrackFitterKF.h"
#include "CbmKFTrack.h"
#include "CbmKFVertex.h"
#include "CbmMCTrack.h"
#include "CbmMatch.h"
#include "CbmMuchGeoScheme.h"
#include "CbmMuchPixelHit.h"
#include "CbmMuchTrack.h"
#include "CbmStsKFTrackFitter.h"
#include "CbmStsTrack.h"
#include "CbmTofHit.h"
#include "CbmTofPoint.h"
#include "CbmTrackMatch.h"
#include "CbmTrackMatchNew.h"
#include "CbmTrdHit.h"
#include "CbmTrdTrack.h"
#include "CbmTrdTrackFitterKF.h"
#include "CbmVertex.h"

#include "FairRootManager.h"
#include "FairTrackParam.h"

#include "TClonesArray.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TLorentzVector.h"
#include "TMCProcess.h"
#include "TMath.h"
#include "TMultiLayerPerceptron.h"
#include "TProfile.h"
#include "TSystem.h"
#include "TTree.h"
#include "TVector3.h"

#include "PParticle.h"

#include "vector"
using std::vector;

#include <sys/stat.h>

#include <fstream>
#include <iostream>
using namespace std;

// -----   Default constructor   -------------------------------------------
CbmAnaDimuonAnalysis::CbmAnaDimuonAnalysis(TString name, TString setup)
  : FairTask("AnaDimuonAnalysis")
  , fEvent(0)
  , fMCTracks(NULL)
  , fStsTracks(NULL)
  , fStsTrackMatches(NULL)
  , fMuchTracks(NULL)
  , fMuchTrackMatches(NULL)
  , fGlobalTracks(NULL)
  , fTrdTracks(NULL)
  , fTofHit(NULL)
  , fMuPlus(new TClonesArray("CbmAnaMuonCandidate", 1))
  , fMuMinus(new TClonesArray("CbmAnaMuonCandidate", 1))
  , fParticles(NULL)
  , fInputTree(NULL)
  , fPlutoFile(NULL)
  , fFitter(NULL)
  , fFitterTRD(NULL)
  , fFitterGlobal(NULL)
  , fVertex(NULL)
  , fChi2StsCut(2)
  , fChi2MuchCut(3)
  , fChi2VertexCut(3)
  , fAnnCut(-1)
  , fNeurons(0)
  , fSigmaTofCut(2)
  , fMass(0.105658)
  , fUseCuts(kTRUE)
  , fUseMC(kTRUE)
  , fNofMuchCut(11)
  , fNofStsCut(7)
  , fNofTrdCut(1)
  ,
  //    fFileName("histo.root"),
  //    fEffFileName("eff_histo.root"),
  fGeoScheme(NULL)

{
  fPlutoFileName = name;
  fSetupName     = setup;
}
// -------------------------------------------------------------------------


// -----  SetParContainers -------------------------------------------------
void CbmAnaDimuonAnalysis::SetParContainers() {}
// -------------------------------------------------------------------------


// -----   Public method Init (abstract in base class)  --------------------
InitStatus CbmAnaDimuonAnalysis::Init() {
  // Get and check FairRootManager
  FairRootManager* fManager = FairRootManager::Instance();
  fMCTracks                 = (TClonesArray*) fManager->GetObject("MCTrack");
  if (nullptr == fMCTracks) LOG(FATAL) << "No MCTrack in input";

  fStsTracks = (TClonesArray*) fManager->GetObject("StsTrack");
  if (nullptr == fStsTracks) LOG(FATAL) << "No StsTrack in input";

  if (fUseMC)
    fStsTrackMatches = (TClonesArray*) fManager->GetObject("StsTrackMatch");
  if (nullptr == fStsTrackMatches && fUseMC)
    LOG(FATAL) << "No StsTrackMatch in input";

  fMuchTracks = (TClonesArray*) fManager->GetObject("MuchTrack");
  if (nullptr == fMuchTracks) LOG(FATAL) << "No MuchTrack in input";

  if (fUseMC)
    fMuchTrackMatches = (TClonesArray*) fManager->GetObject("MuchTrackMatch");
  if (nullptr == fMuchTrackMatches && fUseMC)
    LOG(FATAL) << "No MuchTrackMatch in input";

  fGlobalTracks = (TClonesArray*) fManager->GetObject("GlobalTrack");
  if (nullptr == fGlobalTracks) LOG(FATAL) << "No GlobalTrack in input";

  fTrdTracks = (TClonesArray*) fManager->GetObject("TrdTrack");
  if (nullptr == fTrdTracks) LOG(FATAL) << "No TrdTrack in input";

  fTofHit = (TClonesArray*) fManager->GetObject("TofHit");
  if (nullptr == fTofHit) LOG(FATAL) << "No TofHit in input";

  fVertex = dynamic_cast<CbmVertex*>(fManager->GetObject("PrimaryVertex."));

  fEvent = 0;

  fManager->Register("MuPlus", "Much", fMuPlus, kTRUE);
  fManager->Register("MuMinus", "Much", fMuMinus, kTRUE);

  fGeoScheme = CbmMuchGeoScheme::Instance();
  //    fGeoScheme->Init(fDigiFileName);
  //  fLastStationIndex = fGeoScheme->GetNStations()-1;
  //  fNLayers = 0;
  //  for (Int_t i=0;i<=fLastStationIndex;i++){
  //    fNLayers+=fGeoScheme->GetLayerSides(i).size()/2;
  //  }
  fFitter = new CbmStsKFTrackFitter();
  fFitter->Init();

  fFitterGlobal = new CbmGlobalTrackFitterKF();
  fFitterGlobal->Init();

  fFitterTRD = new CbmTrdTrackFitterKF();
  fFitterTRD->Init();
  fFitterTRD->SetPid(13);

  YPt_pluto = new TH2D("YPt_pluto", "PLUTO signal", YBINNING, PTBINNING);

  if (fPlutoFileName != "") {
    if (fPlutoFileName.Contains("root")) {
      fParticles = new TClonesArray("PParticle", 100);
      fPlutoFile = new TFile(fPlutoFileName.Data());
      fInputTree = (TTree*) fPlutoFile->Get("data");
      fInputTree->SetBranchAddress("Particles", &fParticles);
      for (int iEvent = 0; iEvent < fInputTree->GetEntries(); iEvent++) {
        fInputTree->GetEntry(iEvent);
        Int_t NofPart       = fParticles->GetEntriesFast();
        PParticle* Part1    = (PParticle*) fParticles->At(NofPart - 2);
        PParticle* Part2    = (PParticle*) fParticles->At(NofPart - 1);
        TLorentzVector mom1 = Part1->Vect4();
        TLorentzVector mom2 = Part2->Vect4();
        TLorentzVector Mom  = mom1 + mom2;
        YPt_pluto->Fill(
          Mom.Rapidity(), Mom.Pt(), 1. / (Double_t) fInputTree->GetEntries());
      }
    }
  }

  TString title1 = "STS accepted MC signal";
  TString title2 = "STS+MUCH accepted MC signal";
  TString title3 = "STS+MUCH+TRD accepted MC signal";
  TString title4 = "STS+MUCH+TRD+TOF accepted MC signal";

  YPt_StsAcc = new TH2D("YPt_StsAcc", title1, YBINNING, PTBINNING);
  YPt_StsAcc->GetXaxis()->SetTitle("Y");
  YPt_StsAcc->GetYaxis()->SetTitle("P_{t} (GeV/c)");

  YPt_StsMuchAcc = (TH2D*) YPt_StsAcc->Clone("YPt_StsMuchAcc");
  YPt_StsMuchAcc->SetTitle(title2);
  YPt_StsMuchTrdAcc = (TH2D*) YPt_StsAcc->Clone("YPt_StsMuchTrdAcc");
  YPt_StsMuchTrdAcc->SetTitle(title3);
  YPt_StsMuchTrdTofAcc = (TH2D*) YPt_StsAcc->Clone("YPt_StsMuchTrdTofAcc");
  YPt_StsMuchTrdTofAcc->SetTitle(title4);

  YPtM = new TH3D(
    "YPtM", "Reconstrcuted YPtM spectrum", YBINNING, PTBINNING, MBINNING);

  acc_P[0][0] = new TProfile("signal_accP_Sts", title1, PBINNING);
  acc_P[0][0]->GetXaxis()->SetTitle("P (GeV/c)");
  acc_P[0][0]->GetYaxis()->SetTitle("%");

  acc_P[1][0] = (TProfile*) acc_P[0][0]->Clone("signal_accP_StsMuch");
  acc_P[1][0]->SetTitle(title2);
  acc_P[2][0] = (TProfile*) acc_P[0][0]->Clone("signal_accP_StsMuchTrd");
  acc_P[2][0]->SetTitle(title3);
  acc_P[3][0] = (TProfile*) acc_P[0][0]->Clone("signal_accP_StsMuchTrdTof");
  acc_P[3][0]->SetTitle(title4);

  acc_Theta[0][0] = new TProfile("signal_accTheta_Sts", title1, THETABINNING);
  acc_Theta[0][0]->GetXaxis()->SetTitle("#Theta (#circ)");
  acc_Theta[0][0]->GetYaxis()->SetTitle("%");

  acc_Theta[1][0] =
    (TProfile*) acc_Theta[0][0]->Clone("signal_accTheta_StsMuch");
  acc_Theta[1][0]->SetTitle(title2);
  acc_Theta[2][0] =
    (TProfile*) acc_Theta[0][0]->Clone("signal_accTheta_StsMuchTrd");
  acc_Theta[2][0]->SetTitle(title3);
  acc_Theta[3][0] =
    (TProfile*) acc_Theta[0][0]->Clone("signal_accTheta_StsMuchTrdTof");
  acc_Theta[3][0]->SetTitle(title4);

  title1 = "STS accepted MC #mu+";
  title2 = "STS+MUCH accepted MC #mu+";
  title3 = "STS+MUCH+TRD accepted MC #mu+";
  title4 = "STS+MUCH+TRD+TOF accepted MC #mu+";

  acc_P[0][1] = (TProfile*) acc_P[0][0]->Clone("muPl_accP_Sts");
  acc_P[0][1]->SetTitle(title1);
  acc_P[1][1] = (TProfile*) acc_P[0][0]->Clone("muPl_accP_StsMuch");
  acc_P[1][1]->SetTitle(title2);
  acc_P[2][1] = (TProfile*) acc_P[0][0]->Clone("muPl_accP_StsMuchTrd");
  acc_P[2][1]->SetTitle(title3);
  acc_P[3][1] = (TProfile*) acc_P[0][0]->Clone("muPl_accP_StsMuchTrdTof");
  acc_P[3][1]->SetTitle(title4);

  acc_Theta[0][1] = (TProfile*) acc_Theta[0][0]->Clone("muPl_accTheta_Sts");
  acc_Theta[0][1]->SetTitle(title1);
  acc_Theta[1][1] = (TProfile*) acc_Theta[0][0]->Clone("muPl_accTheta_StsMuch");
  acc_Theta[1][1]->SetTitle(title2);
  acc_Theta[2][1] =
    (TProfile*) acc_Theta[0][0]->Clone("muPl_accTheta_StsMuchTrd");
  acc_Theta[2][1]->SetTitle(title3);
  acc_Theta[3][1] =
    (TProfile*) acc_Theta[0][0]->Clone("muPl_accTheta_StsMuchTrdTof");
  acc_Theta[3][1]->SetTitle(title4);

  title1 = "STS accepted MC #mu-";
  title2 = "STS+MUCH accepted MC #mu-";
  title3 = "STS+MUCH+TRD accepted MC #mu-";
  title4 = "STS+MUCH+TRD+TOF accepted MC #mu-";

  acc_P[0][2] = (TProfile*) acc_P[0][0]->Clone("muMn_accP_Sts");
  acc_P[0][2]->SetTitle(title1);
  acc_P[1][2] = (TProfile*) acc_P[0][0]->Clone("muMn_accP_StsMuch");
  acc_P[1][2]->SetTitle(title2);
  acc_P[2][2] = (TProfile*) acc_P[0][0]->Clone("muMn_accP_StsMuchTrd");
  acc_P[2][2]->SetTitle(title3);
  acc_P[3][2] = (TProfile*) acc_P[0][0]->Clone("muMn_accP_StsMuchTrdTof");
  acc_P[3][2]->SetTitle(title4);

  acc_Theta[0][2] = (TProfile*) acc_Theta[0][0]->Clone("muMn_accTheta_Sts");
  acc_Theta[0][2]->SetTitle(title1);
  acc_Theta[1][2] = (TProfile*) acc_Theta[0][0]->Clone("muMn_accTheta_StsMuch");
  acc_Theta[1][2]->SetTitle(title2);
  acc_Theta[2][2] =
    (TProfile*) acc_Theta[0][0]->Clone("muMn_accTheta_StsMuchTrd");
  acc_Theta[2][2]->SetTitle(title3);
  acc_Theta[3][2] =
    (TProfile*) acc_Theta[0][0]->Clone("muMn_accTheta_StsMuchTrdTof");
  acc_Theta[3][2]->SetTitle(title4);

  TString title0 =
    "reconstructed MC signal after primary vertex cut (PVC)";  // PVC: chi2 of STS track in target
  title1 =
    "reconstructed MC signal after PVC and STS cuts (StsCs)";  // StsCs: chi2 of STS track and number of STS hits
  title2 =
    "reconstructed MC signal after PVC+StsCs and MUCH cuts (MuchCs)";  // MuchCs: chi2 of MUCH track and number of MUCH hits
  title3 =
    "reconstructed MC signal after PVC+StsCs+MuchCs and TRD cut (TrdC)";  // TrdC: number of TRD hits
  title4 =
    "reconstructed MC signal after PVC+StsCs+MuchCs+TrdC and TOF cut";  // TOF: cut using mass distribution from time measurement

  effReco_P[0][0] = (TProfile*) acc_P[0][0]->Clone("signal_effRecoP_VtxSts");
  effReco_P[0][0]->SetTitle(title1);
  effReco_P[1][0] =
    (TProfile*) acc_P[0][0]->Clone("signal_effRecoP_VtxStsMuch");
  effReco_P[1][0]->SetTitle(title2);
  effReco_P[2][0] =
    (TProfile*) acc_P[0][0]->Clone("signal_effRecoP_VtxStsMuchTrd");
  effReco_P[2][0]->SetTitle(title3);
  effReco_P[3][0] =
    (TProfile*) acc_P[0][0]->Clone("signal_effRecoP_VtxStsMuchTrdTof");
  effReco_P[3][0]->SetTitle(title4);

  effReco_Theta[0][0] =
    (TProfile*) acc_Theta[0][0]->Clone("signal_effRecoTheta_VtxSts");
  effReco_Theta[0][0]->SetTitle(title1);
  effReco_Theta[1][0] =
    (TProfile*) acc_Theta[0][0]->Clone("signal_effRecoTheta_VtxStsMuch");
  effReco_Theta[1][0]->SetTitle(title2);
  effReco_Theta[2][0] =
    (TProfile*) acc_Theta[0][0]->Clone("signal_effRecoTheta_VtxStsMuchTrd");
  effReco_Theta[2][0]->SetTitle(title3);
  effReco_Theta[3][0] =
    (TProfile*) acc_Theta[0][0]->Clone("signal_effRecoTheta_VtxStsMuchTrdTof");
  effReco_Theta[3][0]->SetTitle(title4);

  eff4pi_P[0][0] = (TProfile*) acc_P[0][0]->Clone("signal_eff4piP_Vtx");
  eff4pi_P[0][0]->SetTitle(title0);
  eff4pi_P[1][0] = (TProfile*) acc_P[0][0]->Clone("signal_eff4piP_VtxSts");
  eff4pi_P[1][0]->SetTitle(title1);
  eff4pi_P[2][0] = (TProfile*) acc_P[0][0]->Clone("signal_eff4piP_VtxStsMuch");
  eff4pi_P[2][0]->SetTitle(title2);
  eff4pi_P[3][0] =
    (TProfile*) acc_P[0][0]->Clone("signal_eff4piP_VtxStsMuchTrd");
  eff4pi_P[3][0]->SetTitle(title3);
  eff4pi_P[4][0] =
    (TProfile*) acc_P[0][0]->Clone("signal_eff4piP_VtxStsMuchTrdTof");
  eff4pi_P[4][0]->SetTitle(title4);

  eff4pi_Theta[0][0] =
    (TProfile*) acc_Theta[0][0]->Clone("signal_eff4piTheta_Vtx");
  eff4pi_Theta[0][0]->SetTitle(title0);
  eff4pi_Theta[1][0] =
    (TProfile*) acc_Theta[0][0]->Clone("signal_eff4piTheta_VtxSts");
  eff4pi_Theta[1][0]->SetTitle(title1);
  eff4pi_Theta[2][0] =
    (TProfile*) acc_Theta[0][0]->Clone("signal_eff4piTheta_VtxStsMuch");
  eff4pi_Theta[2][0]->SetTitle(title2);
  eff4pi_Theta[3][0] =
    (TProfile*) acc_Theta[0][0]->Clone("signal_eff4piTheta_VtxStsMuchTrd");
  eff4pi_Theta[3][0]->SetTitle(title3);
  eff4pi_Theta[4][0] =
    (TProfile*) acc_Theta[0][0]->Clone("signal_eff4piTheta_VtxStsMuchTrdTof");
  eff4pi_Theta[4][0]->SetTitle(title4);

  YPt_VtxReco = (TH2D*) YPt_StsAcc->Clone("YPt_VtxReco");
  YPt_VtxReco->SetTitle(title0);
  YPt_VtxStsReco = (TH2D*) YPt_StsAcc->Clone("YPt_VtxStsReco");
  YPt_VtxStsReco->SetTitle(title1);
  YPt_VtxStsMuchReco = (TH2D*) YPt_StsAcc->Clone("YPt_VtxStsMuchReco");
  YPt_VtxStsMuchReco->SetTitle(title2);
  YPt_VtxStsMuchTrdReco = (TH2D*) YPt_StsAcc->Clone("YPt_VtxStsMuchTrdReco");
  YPt_VtxStsMuchTrdReco->SetTitle(title3);
  YPt_VtxStsMuchTrdTofReco =
    (TH2D*) YPt_StsAcc->Clone("YPt_VtxStsMuchTrdTofReco");
  YPt_VtxStsMuchTrdTofReco->SetTitle(title4);

  title0 = "reconstructed MC #mu+ after primary vertex cut (PVC)";
  title1 = "reconstructed MC #mu+ after PVC and STS cuts (StsCs)";
  title2 = "reconstructed MC #mu+ after PVC+StsCs and MUCH cuts (MuchCs)";
  title3 = "reconstructed MC #mu+ after PVC+StsCs+MuchCs and TRD cut (TrdC)";
  title4 = "reconstructed MC #mu+ after PVC+StsCs+MuchCs+TrdC and TOF cut";

  effReco_P[0][1] = (TProfile*) acc_P[0][0]->Clone("muPl_effRecoP_VtxSts");
  effReco_P[0][1]->SetTitle(title1);
  effReco_P[1][1] = (TProfile*) acc_P[0][0]->Clone("muPl_effRecoP_VtxStsMuch");
  effReco_P[1][1]->SetTitle(title2);
  effReco_P[2][1] =
    (TProfile*) acc_P[0][0]->Clone("muPl_effRecoP_VtxStsMuchTrd");
  effReco_P[2][1]->SetTitle(title3);
  effReco_P[3][1] =
    (TProfile*) acc_P[0][0]->Clone("muPl_effRecoP_VtxStsMuchTrdTof");
  effReco_P[3][1]->SetTitle(title4);

  effReco_Theta[0][1] =
    (TProfile*) acc_Theta[0][0]->Clone("muPl_effRecoTheta_VtxSts");
  effReco_Theta[0][1]->SetTitle(title1);
  effReco_Theta[1][1] =
    (TProfile*) acc_Theta[0][0]->Clone("muPl_effRecoTheta_VtxStsMuch");
  effReco_Theta[1][1]->SetTitle(title2);
  effReco_Theta[2][1] =
    (TProfile*) acc_Theta[0][0]->Clone("muPl_effRecoTheta_VtxStsMuchTrd");
  effReco_Theta[2][1]->SetTitle(title3);
  effReco_Theta[3][1] =
    (TProfile*) acc_Theta[0][0]->Clone("muPl_effRecoTheta_VtxStsMuchTrdTof");
  effReco_Theta[3][1]->SetTitle(title4);

  eff4pi_P[0][1] = (TProfile*) acc_P[0][0]->Clone("muPl_eff4piP_Vtx");
  eff4pi_P[0][1]->SetTitle(title0);
  eff4pi_P[1][1] = (TProfile*) acc_P[0][0]->Clone("muPl_eff4piP_VtxSts");
  eff4pi_P[1][1]->SetTitle(title1);
  eff4pi_P[2][1] = (TProfile*) acc_P[0][0]->Clone("muPl_eff4piP_VtxStsMuch");
  eff4pi_P[2][1]->SetTitle(title2);
  eff4pi_P[3][1] = (TProfile*) acc_P[0][0]->Clone("muPl_eff4piP_VtxStsMuchTrd");
  eff4pi_P[3][1]->SetTitle(title3);
  eff4pi_P[4][1] =
    (TProfile*) acc_P[0][0]->Clone("muPl_eff4piP_VtxStsMuchTrdTof");
  eff4pi_P[4][1]->SetTitle(title4);

  eff4pi_Theta[0][1] =
    (TProfile*) acc_Theta[0][0]->Clone("muPl_eff4piTheta_Vtx");
  eff4pi_Theta[0][1]->SetTitle(title0);
  eff4pi_Theta[1][1] =
    (TProfile*) acc_Theta[0][0]->Clone("muPl_eff4piTheta_VtxSts");
  eff4pi_Theta[1][1]->SetTitle(title1);
  eff4pi_Theta[2][1] =
    (TProfile*) acc_Theta[0][0]->Clone("muPl_eff4piTheta_VtxStsMuch");
  eff4pi_Theta[2][1]->SetTitle(title2);
  eff4pi_Theta[3][1] =
    (TProfile*) acc_Theta[0][0]->Clone("muPl_eff4piTheta_VtxStsMuchTrd");
  eff4pi_Theta[3][1]->SetTitle(title3);
  eff4pi_Theta[4][1] =
    (TProfile*) acc_Theta[0][0]->Clone("muPl_eff4piTheta_VtxStsMuchTrdTof");
  eff4pi_Theta[4][1]->SetTitle(title4);

  title0 = "reconstructed MC #mu- after primary vertex cut (PVC)";
  title1 = "reconstructed MC #mu- after PVC and STS cuts (StsCs)";
  title2 = "reconstructed MC #mu- after PVC+StsCs and MUCH cuts (MuchCs)";
  title3 = "reconstructed MC #mu- after PVC+StsCs+MuchCs and TRD cut (TrdC)";
  title4 = "reconstructed MC #mu- after PVC+StsCs+MuchCs+TrdC and TOF cut";

  effReco_P[0][2] = (TProfile*) acc_P[0][0]->Clone("muMn_effRecoP_VtxSts");
  effReco_P[0][2]->SetTitle(title1);
  effReco_P[1][2] = (TProfile*) acc_P[0][0]->Clone("muMn_effRecoP_VtxStsMuch");
  effReco_P[1][2]->SetTitle(title2);
  effReco_P[2][2] =
    (TProfile*) acc_P[0][0]->Clone("muMn_effRecoP_VtxStsMuchTrd");
  effReco_P[2][2]->SetTitle(title3);
  effReco_P[3][2] =
    (TProfile*) acc_P[0][0]->Clone("muMn_effRecoP_VtxStsMuchTrdTof");
  effReco_P[3][2]->SetTitle(title4);

  effReco_Theta[0][2] =
    (TProfile*) acc_Theta[0][0]->Clone("muMn_effRecoTheta_VtxSts");
  effReco_Theta[0][2]->SetTitle(title1);
  effReco_Theta[1][2] =
    (TProfile*) acc_Theta[0][0]->Clone("muMn_effRecoTheta_VtxStsMuch");
  effReco_Theta[1][2]->SetTitle(title2);
  effReco_Theta[2][2] =
    (TProfile*) acc_Theta[0][0]->Clone("muMn_effRecoTheta_VtxStsMuchTrd");
  effReco_Theta[2][2]->SetTitle(title3);
  effReco_Theta[3][2] =
    (TProfile*) acc_Theta[0][0]->Clone("muMn_effRecoTheta_VtxStsMuchTrdTof");
  effReco_Theta[3][2]->SetTitle(title4);

  eff4pi_P[0][2] = (TProfile*) acc_P[0][0]->Clone("muMn_eff4piP_Vtx");
  eff4pi_P[0][2]->SetTitle(title0);
  eff4pi_P[1][2] = (TProfile*) acc_P[0][0]->Clone("muMn_eff4piP_VtxSts");
  eff4pi_P[1][2]->SetTitle(title1);
  eff4pi_P[2][2] = (TProfile*) acc_P[0][0]->Clone("muMn_eff4piP_VtxStsMuch");
  eff4pi_P[2][2]->SetTitle(title2);
  eff4pi_P[3][2] = (TProfile*) acc_P[0][0]->Clone("muMn_eff4piP_VtxStsMuchTrd");
  eff4pi_P[3][2]->SetTitle(title3);
  eff4pi_P[4][2] =
    (TProfile*) acc_P[0][0]->Clone("muMn_eff4piP_VtxStsMuchTrdTof");
  eff4pi_P[4][2]->SetTitle(title4);

  eff4pi_Theta[0][2] =
    (TProfile*) acc_Theta[0][0]->Clone("muMn_eff4piTheta_Vtx");
  eff4pi_Theta[0][2]->SetTitle(title0);
  eff4pi_Theta[1][2] =
    (TProfile*) acc_Theta[0][0]->Clone("muMn_eff4piTheta_VtxSts");
  eff4pi_Theta[1][2]->SetTitle(title1);
  eff4pi_Theta[2][2] =
    (TProfile*) acc_Theta[0][0]->Clone("muMn_eff4piTheta_VtxStsMuch");
  eff4pi_Theta[2][2]->SetTitle(title2);
  eff4pi_Theta[3][2] =
    (TProfile*) acc_Theta[0][0]->Clone("muMn_eff4piTheta_VtxStsMuchTrd");
  eff4pi_Theta[3][2]->SetTitle(title3);
  eff4pi_Theta[4][2] =
    (TProfile*) acc_Theta[0][0]->Clone("muMn_eff4piTheta_VtxStsMuchTrdTof");
  eff4pi_Theta[4][2]->SetTitle(title4);

  BgSup[0] = new TH1D("h0", "all STS tracks", PBINNING);
  BgSup[0]->GetXaxis()->SetTitle("P (GeV/c)");
  BgSup[0]->GetYaxis()->SetTitle("suppression");

  title0 = "reconstructed tracks after primary vertex cut (PVC)";
  title1 = "reconstructed tracks after PVC and STS cuts (StsCs)";
  title2 = "reconstructed tracks after PVC+StsCs and MUCH cuts (MuchCs)";
  title3 = "reconstructed tracks after PVC+StsCs+MuchCs and TRD cut (TrdC)";
  title4 = "reconstructed tracks after PVC+StsCs+MuchCs+TrdC and TOF cut";

  BgSup[1] = (TH1D*) BgSup[0]->Clone("h1");
  BgSup[1]->SetTitle(title0);
  BgSup[2] = (TH1D*) BgSup[0]->Clone("h2");
  BgSup[2]->SetTitle(title1);
  BgSup[3] = (TH1D*) BgSup[0]->Clone("h3");
  BgSup[3]->SetTitle(title2);
  BgSup[4] = (TH1D*) BgSup[0]->Clone("h4");
  BgSup[4]->SetTitle(title3);
  BgSup[5] = (TH1D*) BgSup[0]->Clone("h5");
  BgSup[5]->SetTitle(title4);

  TString dir  = getenv("VMCWORKDIR");
  TString name = dir + "/parameters/much/TOF8gev_fitParam_sigma"
                 + std::to_string(fSigmaTofCut) + "." + fSetupName + ".root";

  TFile* FF = new TFile(name);

  TTree* MinParamMu = (TTree*) FF->Get("MinParam");
  MinParamMu->SetBranchAddress("p0", &p0min);
  MinParamMu->SetBranchAddress("p1", &p1min);
  MinParamMu->SetBranchAddress("p2", &p2min);
  MinParamMu->GetEntry(0);

  TTree* MaxParamMu = (TTree*) FF->Get("MaxParam");
  MaxParamMu->SetBranchAddress("p0", &p0max);
  MaxParamMu->SetBranchAddress("p1", &p1max);
  MaxParamMu->SetBranchAddress("p2", &p2max);
  MaxParamMu->GetEntry(0);

  if (fNeurons > 0)
    fFileAnnName = dir + "/parameters/much/muid_ann_" + std::to_string(fNeurons)
                   + "_" + +fSetupName + "_weights.txt";
  else
    fFileAnnName =
      dir + "/parameters/much/muid_ann_16_sis100_muon_lmvm_weights.txt";

  FILE* file = fopen(fFileAnnName.Data(), "r");
  char buffer[100];

  if (fgets(buffer, 100, file) == NULL) {
    LOG(info) << "======================================================";
    LOG(info) << "The ANN weights file " << fFileAnnName << " does not exist";

    fFileAnnName =
      dir + "/parameters/much/muid_ann_16_sis100_muon_lmvm_weights.txt";

    LOG(info) << "The default ANN weights file " << fFileAnnName
              << " will be used";
    LOG(info) << "======================================================";
    fNeurons = 16;
  }

  if (fAnnCut > 0) fUseCuts = kFALSE;

  return kSUCCESS;
}
// -------------------------------------------------------------------------


// -----   Public method Exec   --------------------------------------------
void CbmAnaDimuonAnalysis::Exec(Option_t* /*opt*/) {
  Int_t nMCTracks     = fMCTracks->GetEntriesFast();
  Int_t nStsTracks    = fStsTracks->GetEntriesFast();
  Int_t nMuchTracks   = fMuchTracks->GetEntriesFast();
  Int_t nGlobalTracks = fGlobalTracks->GetEntriesFast();

  LOG(DEBUG) << "------------------------";
  LOG(DEBUG) << GetName() << ": Event " << fEvent;
  LOG(DEBUG) << "Number of tracks: MC - " << nMCTracks << ", global - "
             << nGlobalTracks << ", STS - " << nStsTracks << ", MUCH - "
             << nMuchTracks;
  LOG(DEBUG) << "------------------------";

  TLorentzVector pMC1, pMC2, M;

  struct CbmMuon {
    Bool_t Mu;
    Bool_t Nsts;
    Bool_t Nmuch;
    Bool_t Ntrd;
    Bool_t Ntof;
    Bool_t Chi2V;
    Bool_t Chi2sts;
    Bool_t Chi2much;
    CbmMuon()
      : Mu(kFALSE)
      , Nsts(kFALSE)
      , Nmuch(kFALSE)
      , Ntrd(kFALSE)
      , Ntof(kFALSE)
      , Chi2V(kFALSE)
      , Chi2sts(kFALSE)
      , Chi2much(kFALSE) {
      ;
    }
  };

  struct CbmMuonMC {
    Bool_t Mu;
    Bool_t Nsts;
    Bool_t Nmuch;
    Bool_t Ntrd;
    Bool_t Ntof;
    CbmMuonMC()
      : Mu(kFALSE), Nsts(kFALSE), Nmuch(kFALSE), Ntrd(kFALSE), Ntof(kFALSE) {
      ;
    }
  };
  //----------------- Sort MC tracks for acceptance histograms

  CbmMuon muPl_reco;
  CbmMuon muMn_reco;
  CbmMuon signal_reco;

  CbmMuonMC muPl_mc;
  CbmMuonMC muMn_mc;
  CbmMuonMC signal_mc;

  for (Int_t iMCTrack = 0; iMCTrack < nMCTracks; iMCTrack++) {
    CbmMCTrack* mcTrack = (CbmMCTrack*) fMCTracks->At(iMCTrack);
    if (mcTrack->GetGeantProcessId() != kPPrimary) continue;
    if (TMath::Abs(mcTrack->GetPdgCode()) != 13) continue;
    if (mcTrack->GetCharge() < 0) {
      muMn_mc.Mu = kTRUE;
      if (mcTrack->GetNPoints(ECbmModuleId::kSts) >= fNofStsCut) {
        muMn_mc.Nsts = kTRUE;
        if (mcTrack->GetNPoints(ECbmModuleId::kMuch) >= fNofMuchCut) {
          muMn_mc.Nmuch = kTRUE;
          if (mcTrack->GetNPoints(ECbmModuleId::kTrd) >= fNofTrdCut) {
            muMn_mc.Ntrd = kTRUE;
            if (mcTrack->GetNPoints(ECbmModuleId::kTof) >= 1)
              muMn_mc.Ntof = kTRUE;
          }
        }
      }
    } else {
      muPl_mc.Mu = kTRUE;
      if (mcTrack->GetNPoints(ECbmModuleId::kSts) >= fNofStsCut) {
        muPl_mc.Nsts = kTRUE;
        if (mcTrack->GetNPoints(ECbmModuleId::kMuch) >= fNofMuchCut) {
          muPl_mc.Nmuch = kTRUE;
          if (mcTrack->GetNPoints(ECbmModuleId::kTrd) >= fNofTrdCut) {
            muPl_mc.Ntrd = kTRUE;
            if (mcTrack->GetNPoints(ECbmModuleId::kTof) >= 1)
              muPl_mc.Ntof = kTRUE;
          }
        }
      }
    }
  }

  if (muPl_mc.Mu && muMn_mc.Mu) signal_mc.Mu = kTRUE;
  if (muPl_mc.Nsts && muMn_mc.Nsts) signal_mc.Nsts = kTRUE;
  if (muPl_mc.Nmuch && muMn_mc.Nmuch) signal_mc.Nmuch = kTRUE;
  if (muPl_mc.Ntrd && muMn_mc.Ntrd) signal_mc.Ntrd = kTRUE;
  if (muPl_mc.Ntof && muMn_mc.Ntof) signal_mc.Ntof = kTRUE;
  //-----------------

  fMuPlus->Clear();
  fMuMinus->Clear();

  Int_t iMuPlus  = 0;
  Int_t iMuMinus = 0;

  for (Int_t iTrack = 0; iTrack < nGlobalTracks; iTrack++) {

    Bool_t analysis = kFALSE;

    //----------------- Global track parameters

    CbmGlobalTrack* globalTrack = (CbmGlobalTrack*) fGlobalTracks->At(iTrack);

    //    Double_t chi2global = globalTrack->GetChi2()/globalTrack->GetNDF();

    Int_t iMuchTrack = globalTrack->GetMuchTrackIndex();
    Int_t iStsTrack  = globalTrack->GetStsTrackIndex();
    Int_t iTrdTrack  = globalTrack->GetTrdTrackIndex();
    Int_t iTofHit    = globalTrack->GetTofHitIndex();

    if (iStsTrack < 0) continue;

    //----------------- STS track parameters

    CbmStsTrack* stsTrack = (CbmStsTrack*) fStsTracks->At(iStsTrack);
    if (!stsTrack) continue;

    Int_t nStsHits      = stsTrack->GetNofHits();
    Double_t chi2vertex = fFitter->GetChiToVertex(stsTrack);
    Double_t chi2sts    = 1000;
    if (stsTrack->GetNDF() != 0)
      chi2sts = stsTrack->GetChiSq() / stsTrack->GetNDF();

    FairTrackParam par;
    TLorentzVector mom;
    TVector3 p;

    fFitter->Extrapolate(stsTrack, fVertex->GetZ(), &par);
    par.Momentum(p);
    mom.SetVectM(p, fMass);

    Double_t momentum = mom.P();

    BgSup[0]->Fill(momentum);

    Int_t q = par.GetQp() > 0 ? 1 : -1;

    //----------------- MUCH track parameters

    Int_t nMuchHits   = 0;
    Double_t chi2much = 1000;

    if (iMuchTrack > -1) {
      CbmMuchTrack* muchTrack = (CbmMuchTrack*) fMuchTracks->At(iMuchTrack);
      if (muchTrack) {
        nMuchHits = muchTrack->GetNofHits();
        if (muchTrack->GetNDF() != 0)
          chi2much = muchTrack->GetChiSq() / muchTrack->GetNDF();
      }
    }

    //----------------- TRD track parameters

    Int_t nTrdHits   = 0;
    Double_t chi2trd = 1000;

    if (iTrdTrack > -1) {
      CbmTrdTrack* trdTrack = (CbmTrdTrack*) fTrdTracks->At(iTrdTrack);
      if (trdTrack) {
        nTrdHits = trdTrack->GetNofHits();
        if (trdTrack->GetNDF() != 0)
          chi2trd =
            trdTrack->GetChiSq() / trdTrack->GetNDF();  // study of TRD chi2 !
      }
    }

    //----------------- TOF hit parameters

    Int_t nTofHits = 0;
    Double_t mass  = -1000;

    if (iTofHit > -1) {
      CbmTofHit* th = (CbmTofHit*) fTofHit->At(iTofHit);
      if (th) {
        nTofHits = 1;

        Double_t time = th->GetTime();
        Double_t beta =
          globalTrack->GetLength() * 0.01 / (time * 1e-9 * TMath::C());

        TVector3 momL;

        FairTrackParam* stpl = (FairTrackParam*) globalTrack->GetParamLast();
        stpl->Momentum(momL);

        if (beta != 0) mass = momL.Mag() * momL.Mag() * (1. / beta / beta - 1.);
      }
    }
    //----------------- STS MC matching

    Int_t isMu   = 0;
    Int_t stsPDG = 0;

    if (fUseMC) {

      CbmTrackMatchNew* stsMatch =
        (CbmTrackMatchNew*) fStsTrackMatches->At(iStsTrack);

      if (stsMatch) {
        if (stsMatch->GetNofLinks() != 0) {
          int stsMcTrackId    = stsMatch->GetMatchedLink().GetIndex();
          CbmMCTrack* mcTrack = (CbmMCTrack*) fMCTracks->At(stsMcTrackId);
          if (mcTrack) {
            int pdg = TMath::Abs(mcTrack->GetPdgCode());
            stsPDG  = pdg;
            if (mcTrack->GetGeantProcessId() == kPPrimary && pdg == 13) {
              isMu = 1;
              if (mcTrack->GetCharge() < 0) {
                muMn_reco.Mu = kTRUE;

                if (chi2vertex <= fChi2VertexCut) {
                  muMn_reco.Chi2V = kTRUE;

                  if (nStsHits >= fNofStsCut && chi2sts <= fChi2StsCut) {
                    muMn_reco.Nsts = kTRUE;

                    if (nMuchHits >= fNofMuchCut && chi2much <= fChi2MuchCut) {
                      muMn_reco.Nmuch = kTRUE;

                      if (nTrdHits >= fNofTrdCut) {
                        muMn_reco.Ntrd = kTRUE;

                        if (mass > (p0min + p1min * momentum
                                    + p2min * momentum * momentum)
                            && mass < (p0max + p1max * momentum
                                       + p2max * momentum * momentum)
                            && fAnnCut < 0)
                          muMn_reco.Ntof = kTRUE;
                      }
                    }
                  }
                }
              } else {
                muPl_reco.Mu = kTRUE;
                if (chi2vertex <= fChi2VertexCut) {
                  muPl_reco.Chi2V = kTRUE;

                  if (nStsHits >= fNofStsCut && chi2sts <= fChi2StsCut) {
                    muPl_reco.Nsts = kTRUE;

                    if (nMuchHits >= fNofMuchCut && chi2much <= fChi2MuchCut) {
                      muPl_reco.Nmuch = kTRUE;

                      if (nTrdHits >= fNofTrdCut) {
                        muPl_reco.Ntrd = kTRUE;

                        if (mass > (p0min + p1min * momentum
                                    + p2min * momentum * momentum)
                            && mass < (p0max + p1max * momentum
                                       + p2max * momentum * momentum)
                            && fAnnCut < 0)
                          muPl_reco.Ntof = kTRUE;
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    if (muPl_reco.Mu && muMn_reco.Mu) signal_reco.Mu = kTRUE;
    if (muPl_reco.Chi2V && muMn_reco.Chi2V) signal_reco.Chi2V = kTRUE;
    if (muPl_reco.Nsts && muMn_reco.Nsts) signal_reco.Nsts = kTRUE;
    if (muPl_reco.Nmuch && muMn_reco.Nmuch) signal_reco.Nmuch = kTRUE;
    if (muPl_reco.Ntrd && muMn_reco.Ntrd) signal_reco.Ntrd = kTRUE;
    if (muPl_reco.Ntof && muMn_reco.Ntof && fAnnCut < 0)
      signal_reco.Ntof = kTRUE;

    if (chi2vertex <= fChi2VertexCut) {
      BgSup[1]->Fill(momentum);

      if (nStsHits >= fNofStsCut && chi2sts <= fChi2StsCut) {
        BgSup[2]->Fill(momentum);

        if (nMuchHits >= fNofMuchCut && chi2much <= fChi2MuchCut) {
          BgSup[3]->Fill(momentum);

          if (nTrdHits >= fNofTrdCut) {
            BgSup[4]->Fill(momentum);

            if (mass > (p0min + p1min * momentum + p2min * momentum * momentum)
                && mass
                     < (p0max + p1max * momentum + p2max * momentum * momentum)
                && fAnnCut < 0)
              BgSup[5]->Fill(momentum);
          }
        }
      }
    }

    //----------------- Muon track candidates
    Double_t id = -1;

    if (!fUseCuts && fAnnCut < 0)
      analysis = kTRUE;
    else if (fUseCuts) {
      if (nStsHits >= fNofStsCut && nMuchHits >= fNofMuchCut
          && nTrdHits >= fNofTrdCut && nTofHits >= 1
          && chi2vertex <= fChi2VertexCut && chi2sts <= fChi2StsCut
          && chi2much <= fChi2MuchCut
          && (mass > (p0min + p1min * momentum + p2min * momentum * momentum)
              && mass
                   < (p0max + p1max * momentum + p2max * momentum * momentum)))
        analysis = kTRUE;
    } else if (!fUseCuts && fAnnCut > 0) {
      id = CalculateAnnValue(mom.P(),
                             mass,
                             chi2vertex,
                             chi2sts,
                             chi2much,
                             chi2trd,
                             nStsHits,
                             nMuchHits,
                             nTrdHits,
                             nTofHits);
      if (id > fAnnCut) {
        analysis = kTRUE;
        BgSup[5]->Fill(momentum);
        if (isMu == 1 && q > 0)
          muPl_reco.Ntof = kTRUE;
        else if (isMu == 1 && q < 0)
          muMn_reco.Ntof = kTRUE;
        if (muPl_reco.Ntof && muMn_reco.Ntof) signal_reco.Ntof = kTRUE;
      }
    }

    if (!analysis) continue;

    CbmAnaMuonCandidate* mu;
    if (q > 0) {
      new ((*fMuPlus)[iMuPlus++]) CbmAnaMuonCandidate();
      mu = (CbmAnaMuonCandidate*) (*fMuPlus)[iMuPlus - 1];
    } else {
      new ((*fMuMinus)[iMuMinus++]) CbmAnaMuonCandidate();
      mu = (CbmAnaMuonCandidate*) (*fMuMinus)[iMuMinus - 1];
    }

    mu->SetNStsHits(nStsHits);
    mu->SetNMuchHits(nMuchHits);
    mu->SetNTrdHits(nTrdHits);
    mu->SetNTofHits(nTofHits);

    mu->SetChiToVertex(chi2vertex);
    mu->SetChiSts(chi2sts);
    mu->SetChiMuch(chi2much);
    mu->SetChiTrd(chi2trd);

    mu->SetTrueMu(isMu);
    mu->SetStsPdg(stsPDG);
    mu->SetTofM(mass);
    mu->SetSign(q);
    mu->SetMomentum(mom);

    if (!fUseCuts && fAnnCut < 0) id = CalculateAnnValue(mu);
    mu->SetAnnId(id);
  }

  //----------------- Fill YPtM spectrum

  int NofPlus  = fMuPlus->GetEntriesFast();
  int NofMinus = fMuMinus->GetEntriesFast();
  for (int iPart = 0; iPart < NofPlus; iPart++) {
    CbmAnaMuonCandidate* mu_pl = (CbmAnaMuonCandidate*) fMuPlus->At(iPart);
    TLorentzVector* P_pl       = mu_pl->GetMomentum();
    for (int jPart = 0; jPart < NofMinus; jPart++) {
      CbmAnaMuonCandidate* mu_mn = (CbmAnaMuonCandidate*) fMuMinus->At(jPart);
      TLorentzVector* P_mn       = mu_mn->GetMomentum();
      M                          = *P_pl + *P_mn;
      YPtM->Fill(M.Rapidity(), M.Pt(), M.M());
    }
  }

  //----------------- Fill acceptance and efficiency spectra for MC signal muons and signal signal
  //----------------- Fill YPt spectra for signal signal

  if (fPlutoFileName != "") {
    fInputTree->GetEntry(fEvent);
    Int_t NofPart       = fParticles->GetEntriesFast();
    PParticle* Part1    = (PParticle*) fParticles->At(NofPart - 1);
    PParticle* Part2    = (PParticle*) fParticles->At(NofPart - 2);
    TLorentzVector mom1 = Part1->Vect4();
    TLorentzVector mom2 = Part2->Vect4();

    if (muPl_mc.Mu) {
      FillProfile(acc_P[0][1], mom2.P(), muPl_mc.Nsts);
      FillProfile(acc_P[1][1], mom2.P(), muPl_mc.Nmuch);
      FillProfile(acc_P[2][1], mom2.P(), muPl_mc.Ntrd);
      FillProfile(acc_P[3][1], mom2.P(), muPl_mc.Ntof);
      FillProfile(
        acc_Theta[0][1], mom2.Theta() * TMath::RadToDeg(), muPl_mc.Nsts);
      FillProfile(
        acc_Theta[1][1], mom2.Theta() * TMath::RadToDeg(), muPl_mc.Nmuch);
      FillProfile(
        acc_Theta[2][1], mom2.Theta() * TMath::RadToDeg(), muPl_mc.Ntrd);
      FillProfile(
        acc_Theta[3][1], mom2.Theta() * TMath::RadToDeg(), muPl_mc.Ntof);
      if (muPl_mc.Nsts) {
        FillProfile(effReco_P[0][1], mom2.P(), muPl_reco.Nsts);
        FillProfile(effReco_Theta[0][1],
                    mom2.Theta() * TMath::RadToDeg(),
                    muPl_reco.Nsts);
      }
      if (muPl_mc.Nmuch) {
        FillProfile(effReco_P[1][1], mom2.P(), muPl_reco.Nmuch);
        FillProfile(effReco_Theta[1][1],
                    mom2.Theta() * TMath::RadToDeg(),
                    muPl_reco.Nmuch);
      }
      if (muPl_mc.Ntrd) {
        FillProfile(effReco_P[2][1], mom2.P(), muPl_reco.Ntrd);
        FillProfile(effReco_Theta[2][1],
                    mom2.Theta() * TMath::RadToDeg(),
                    muPl_reco.Ntrd);
      }
      if (muPl_mc.Ntof) {
        FillProfile(effReco_P[3][1], mom2.P(), muPl_reco.Ntof);
        FillProfile(effReco_Theta[3][1],
                    mom2.Theta() * TMath::RadToDeg(),
                    muPl_reco.Ntof);
      }
    }

    if (muPl_reco.Mu) {
      FillProfile(eff4pi_P[0][1], mom2.P(), muPl_reco.Chi2V);
      FillProfile(eff4pi_P[1][1], mom2.P(), muPl_reco.Nsts);
      FillProfile(eff4pi_P[2][1], mom2.P(), muPl_reco.Nmuch);
      FillProfile(eff4pi_P[3][1], mom2.P(), muPl_reco.Ntrd);
      FillProfile(eff4pi_P[4][1], mom2.P(), muPl_reco.Ntof);
      FillProfile(
        eff4pi_Theta[0][1], mom2.Theta() * TMath::RadToDeg(), muPl_reco.Chi2V);
      FillProfile(
        eff4pi_Theta[1][1], mom2.Theta() * TMath::RadToDeg(), muPl_reco.Nsts);
      FillProfile(
        eff4pi_Theta[2][1], mom2.Theta() * TMath::RadToDeg(), muPl_reco.Nmuch);
      FillProfile(
        eff4pi_Theta[3][1], mom2.Theta() * TMath::RadToDeg(), muPl_reco.Ntrd);
      FillProfile(
        eff4pi_Theta[4][1], mom2.Theta() * TMath::RadToDeg(), muPl_reco.Ntof);
    }

    if (muMn_mc.Mu) {
      FillProfile(acc_P[0][2], mom1.P(), muMn_mc.Nsts);
      FillProfile(acc_P[1][2], mom1.P(), muMn_mc.Nmuch);
      FillProfile(acc_P[2][2], mom1.P(), muMn_mc.Ntrd);
      FillProfile(acc_P[3][2], mom1.P(), muMn_mc.Ntof);
      FillProfile(
        acc_Theta[0][2], mom1.Theta() * TMath::RadToDeg(), muMn_mc.Nsts);
      FillProfile(
        acc_Theta[1][2], mom1.Theta() * TMath::RadToDeg(), muMn_mc.Nmuch);
      FillProfile(
        acc_Theta[2][2], mom1.Theta() * TMath::RadToDeg(), muMn_mc.Ntrd);
      FillProfile(
        acc_Theta[3][2], mom1.Theta() * TMath::RadToDeg(), muMn_mc.Ntof);
      if (muMn_mc.Nsts) {
        FillProfile(effReco_P[0][2], mom1.P(), muMn_reco.Nsts);
        FillProfile(effReco_Theta[0][2],
                    mom1.Theta() * TMath::RadToDeg(),
                    muMn_reco.Nsts);
      }
      if (muMn_mc.Nmuch) {
        FillProfile(effReco_P[1][2], mom1.P(), muMn_reco.Nmuch);
        FillProfile(effReco_Theta[1][2],
                    mom1.Theta() * TMath::RadToDeg(),
                    muMn_reco.Nmuch);
      }
      if (muMn_mc.Ntrd) {
        FillProfile(effReco_P[2][2], mom1.P(), muMn_reco.Ntrd);
        FillProfile(effReco_Theta[2][2],
                    mom1.Theta() * TMath::RadToDeg(),
                    muMn_reco.Ntrd);
      }
      if (muMn_mc.Ntof) {
        FillProfile(effReco_P[3][2], mom1.P(), muMn_reco.Ntof);
        FillProfile(effReco_Theta[3][2],
                    mom1.Theta() * TMath::RadToDeg(),
                    muMn_reco.Ntof);
      }
    }

    if (muMn_reco.Mu) {
      FillProfile(eff4pi_P[0][2], mom1.P(), muMn_reco.Chi2V);
      FillProfile(eff4pi_P[1][2], mom1.P(), muMn_reco.Nsts);
      FillProfile(eff4pi_P[2][2], mom1.P(), muMn_reco.Nmuch);
      FillProfile(eff4pi_P[3][2], mom1.P(), muMn_reco.Ntrd);
      FillProfile(eff4pi_P[4][2], mom1.P(), muMn_reco.Ntof);
      FillProfile(
        eff4pi_Theta[0][2], mom1.Theta() * TMath::RadToDeg(), muMn_reco.Chi2V);
      FillProfile(
        eff4pi_Theta[1][2], mom1.Theta() * TMath::RadToDeg(), muMn_reco.Nsts);
      FillProfile(
        eff4pi_Theta[2][2], mom1.Theta() * TMath::RadToDeg(), muMn_reco.Nmuch);
      FillProfile(
        eff4pi_Theta[3][2], mom1.Theta() * TMath::RadToDeg(), muMn_reco.Ntrd);
      FillProfile(
        eff4pi_Theta[4][2], mom1.Theta() * TMath::RadToDeg(), muMn_reco.Ntof);
    }

    TLorentzVector Mom = mom1 + mom2;
    if (signal_mc.Mu) {
      FillProfile(acc_P[0][0], Mom.P(), signal_mc.Nsts);
      FillProfile(acc_P[1][0], Mom.P(), signal_mc.Nmuch);
      FillProfile(acc_P[2][0], Mom.P(), signal_mc.Ntrd);
      FillProfile(acc_P[3][0], Mom.P(), signal_mc.Ntof);
      FillProfile(
        acc_Theta[0][0], Mom.Theta() * TMath::RadToDeg(), signal_mc.Nsts);
      FillProfile(
        acc_Theta[1][0], Mom.Theta() * TMath::RadToDeg(), signal_mc.Nmuch);
      FillProfile(
        acc_Theta[2][0], Mom.Theta() * TMath::RadToDeg(), signal_mc.Ntrd);
      FillProfile(
        acc_Theta[3][0], Mom.Theta() * TMath::RadToDeg(), signal_mc.Ntof);
      if (signal_mc.Nsts) {
        FillProfile(effReco_P[0][0], Mom.P(), signal_reco.Nsts);
        FillProfile(effReco_Theta[0][0],
                    Mom.Theta() * TMath::RadToDeg(),
                    signal_reco.Nsts);
        YPt_StsAcc->Fill(Mom.Rapidity(), Mom.Pt());
      }
      if (signal_mc.Nmuch) {
        FillProfile(effReco_P[1][0], Mom.P(), signal_reco.Nmuch);
        FillProfile(effReco_Theta[1][0],
                    Mom.Theta() * TMath::RadToDeg(),
                    signal_reco.Nmuch);
        YPt_StsMuchAcc->Fill(Mom.Rapidity(), Mom.Pt());
      }
      if (signal_mc.Ntrd) {
        FillProfile(effReco_P[2][0], Mom.P(), signal_reco.Ntrd);
        FillProfile(effReco_Theta[2][0],
                    Mom.Theta() * TMath::RadToDeg(),
                    signal_reco.Ntrd);
        YPt_StsMuchTrdAcc->Fill(Mom.Rapidity(), Mom.Pt());
      }
      if (signal_mc.Ntof) {
        FillProfile(effReco_P[3][0], Mom.P(), signal_reco.Ntof);
        FillProfile(effReco_Theta[3][0],
                    Mom.Theta() * TMath::RadToDeg(),
                    signal_reco.Ntof);
        YPt_StsMuchTrdTofAcc->Fill(Mom.Rapidity(), Mom.Pt());
      }
    }
    if (signal_reco.Mu) {
      if (signal_reco.Chi2V) YPt_VtxReco->Fill(Mom.Rapidity(), Mom.Pt());
      if (signal_reco.Nsts) YPt_VtxStsReco->Fill(Mom.Rapidity(), Mom.Pt());
      if (signal_reco.Nmuch) YPt_VtxStsMuchReco->Fill(Mom.Rapidity(), Mom.Pt());
      if (signal_reco.Ntrd)
        YPt_VtxStsMuchTrdReco->Fill(Mom.Rapidity(), Mom.Pt());
      if (signal_reco.Ntof)
        YPt_VtxStsMuchTrdTofReco->Fill(Mom.Rapidity(), Mom.Pt());
    }
    FillProfile(eff4pi_P[0][0], Mom.P(), signal_reco.Chi2V);
    FillProfile(eff4pi_P[1][0], Mom.P(), signal_reco.Nsts);
    FillProfile(eff4pi_P[2][0], Mom.P(), signal_reco.Nmuch);
    FillProfile(eff4pi_P[3][0], Mom.P(), signal_reco.Ntrd);
    FillProfile(eff4pi_P[4][0], Mom.P(), signal_reco.Ntof);
    FillProfile(
      eff4pi_Theta[0][0], Mom.Theta() * TMath::RadToDeg(), signal_reco.Chi2V);
    FillProfile(
      eff4pi_Theta[1][0], Mom.Theta() * TMath::RadToDeg(), signal_reco.Nsts);
    FillProfile(
      eff4pi_Theta[2][0], Mom.Theta() * TMath::RadToDeg(), signal_reco.Nmuch);
    FillProfile(
      eff4pi_Theta[3][0], Mom.Theta() * TMath::RadToDeg(), signal_reco.Ntrd);
    FillProfile(
      eff4pi_Theta[4][0], Mom.Theta() * TMath::RadToDeg(), signal_reco.Ntof);
  }
  fEvent++;
}
// -------------------------------------------------------------------------
// used default ANN (16 neurons) weights generated for 8 GeV/c Au beam (UrQMD) and omega->µµ (PLUTO)
// sis100_muon_lmvm setup
//
Double_t CbmAnaDimuonAnalysis::CalculateAnnValue(CbmAnaMuonCandidate* mu) {

  Double_t ID = -1;
  Double_t Chi2Vertex, Chi2STS, Chi2MUCH, Chi2TRD;
  Int_t NofSTS, NofMUCH, NofTRD, NofTOF, type;
  Double_t P, M;
  TLorentzVector* PP = mu->GetMomentum();
  M                  = mu->GetTofM();
  P                  = PP->P();
  Chi2Vertex         = mu->GetChiToVertex();
  Chi2STS            = mu->GetChiSts();
  Chi2MUCH           = mu->GetChiMuch();
  Chi2TRD            = mu->GetChiTrd();
  NofSTS             = mu->GetNStsHits();
  NofMUCH            = mu->GetNMuchHits();
  NofTRD             = mu->GetNTrdHits();
  NofTOF             = mu->GetNTofHits();

  Double_t MUCHchi2 = 10.;
  Double_t STSchi2  = 10.;
  Double_t Vchi2    = 10.;

  if ((Chi2Vertex <= Vchi2 && Chi2Vertex >= 0)
      && (Chi2STS <= STSchi2 && Chi2STS >= 0)
      && (Chi2MUCH <= MUCHchi2 && Chi2MUCH >= 0) && NofTOF > 0) {
    Double_t params[9];

    TTree* simu = new TTree("MonteCarlo", "Filtered Monte Carlo Events");
    simu->Branch("P", &P, "P/D");
    simu->Branch("M", &M, "M/D");
    simu->Branch("Chi2Vertex", &Chi2Vertex, "Chi2Vertex/D");
    simu->Branch("Chi2STS", &Chi2STS, "Chi2STS/D");
    simu->Branch("Chi2MUCH", &Chi2MUCH, "Chi2MUCH/D");
    simu->Branch("Chi2TRD", &Chi2TRD, "Chi2TRD/D");
    simu->Branch("NofSTS", &NofSTS, "NofSTS/I");
    simu->Branch("NofMUCH", &NofMUCH, "NofMUCH/I");
    simu->Branch("NofTRD", &NofTRD, "NofTRD/I");
    simu->Branch("type", &type, "type/I");

    simu->Fill();

    TMultiLayerPerceptron* mlp =
      new TMultiLayerPerceptron("@P,@M,@Chi2Vertex,@Chi2STS,@Chi2MUCH,@Chi2TRD,"
                                "@NofSTS,@NofMUCH,@NofTRD:16:type",
                                simu);
    mlp->LoadWeights(fFileAnnName);

    params[0] = P;
    params[1] = M;
    params[2] = Chi2Vertex;
    params[3] = Chi2STS;
    params[4] = Chi2MUCH;
    params[5] = Chi2TRD;
    params[6] = (Double_t) NofSTS;
    params[7] = (Double_t) NofMUCH;
    params[8] = (Double_t) NofTRD;
    ID        = mlp->Evaluate(0, params);

    mlp->Delete();
    simu->Delete();
  }

  return ID;
}
// -------------------------------------------------------------------------
Double_t CbmAnaDimuonAnalysis::CalculateAnnValue(Double_t P,
                                                 Double_t M,
                                                 Double_t Chi2Vertex,
                                                 Double_t Chi2STS,
                                                 Double_t Chi2MUCH,
                                                 Double_t Chi2TRD,
                                                 Int_t NofSTS,
                                                 Int_t NofMUCH,
                                                 Int_t NofTRD,
                                                 Int_t NofTOF) {

  Double_t ID = -1;

  Double_t MUCHchi2 = 10.;
  Double_t STSchi2  = 10.;
  Double_t Vchi2    = 10.;

  if ((Chi2Vertex <= Vchi2 && Chi2Vertex >= 0)
      && (Chi2STS <= STSchi2 && Chi2STS >= 0)
      && (Chi2MUCH <= MUCHchi2 && Chi2MUCH >= 0) && NofTOF > 0) {
    Int_t type;
    Double_t params[9];

    TTree* simu = new TTree("MonteCarlo", "Filtered Monte Carlo Events");
    simu->Branch("P", &P, "P/D");
    simu->Branch("M", &M, "M/D");
    simu->Branch("Chi2Vertex", &Chi2Vertex, "Chi2Vertex/D");
    simu->Branch("Chi2STS", &Chi2STS, "Chi2STS/D");
    simu->Branch("Chi2MUCH", &Chi2MUCH, "Chi2MUCH/D");
    simu->Branch("Chi2TRD", &Chi2TRD, "Chi2TRD/D");
    simu->Branch("NofSTS", &NofSTS, "NofSTS/I");
    simu->Branch("NofMUCH", &NofMUCH, "NofMUCH/I");
    simu->Branch("NofTRD", &NofTRD, "NofTRD/I");
    simu->Branch("type", &type, "type/I");

    simu->Fill();

    TString input;
    input.Form("@P,@M,@Chi2Vertex,@Chi2STS,@Chi2MUCH,@Chi2TRD,@NofSTS,@NofMUCH,"
               "@NofTRD:%d:type",
               fNeurons);
    TMultiLayerPerceptron* mlp = new TMultiLayerPerceptron(input.Data(), simu);
    mlp->LoadWeights(fFileAnnName);

    params[0] = P;
    params[1] = M;
    params[2] = Chi2Vertex;
    params[3] = Chi2STS;
    params[4] = Chi2MUCH;
    params[5] = Chi2TRD;

    params[6] = (Double_t) NofSTS;
    params[7] = (Double_t) NofMUCH;
    params[8] = (Double_t) NofTRD;

    ID = mlp->Evaluate(0, params);

    mlp->Delete();
    simu->Delete();
  }
  return ID;
}
// -------------------------------------------------------------------------
void CbmAnaDimuonAnalysis::FillProfile(TProfile* profile,
                                       Double_t param,
                                       Bool_t trigger) {
  if (trigger)
    profile->Fill(param, 100);
  else
    profile->Fill(param, 0);
}
// -----   Public method Finish   ------------------------------------------
void CbmAnaDimuonAnalysis::Finish() {

  TString name;

  if (fPlutoFileName != "") {

    if (fAnnCut > 0)
      name = Form("YPt_histo_ANN_%1.2f.root", fAnnCut);
    else
      name = "YPt_histo.root";

    TFile* f = new TFile(name, "recreate");

    YPt_StsAcc->Scale(1. / (Double_t) fEvent);
    YPt_StsMuchAcc->Scale(1. / (Double_t) fEvent);
    YPt_StsMuchTrdAcc->Scale(1. / (Double_t) fEvent);
    YPt_StsMuchTrdTofAcc->Scale(1. / (Double_t) fEvent);

    YPt_VtxReco->Scale(1. / (Double_t) fEvent);
    YPt_VtxStsReco->Scale(1. / (Double_t) fEvent);
    YPt_VtxStsMuchReco->Scale(1. / (Double_t) fEvent);
    YPt_VtxStsMuchTrdReco->Scale(1. / (Double_t) fEvent);
    YPt_VtxStsMuchTrdTofReco->Scale(1. / (Double_t) fEvent);

    YPt_pluto->Write();
    YPt_StsAcc->Write();
    YPt_StsMuchAcc->Write();
    YPt_StsMuchTrdAcc->Write();
    YPt_StsMuchTrdTofAcc->Write();

    if (fAnnCut < 0) {
      YPt_VtxReco->Write();
      YPt_VtxStsReco->Write();
      YPt_VtxStsMuchReco->Write();
      YPt_VtxStsMuchTrdReco->Write();
    }
    YPt_VtxStsMuchTrdTofReco->Write();

    f->Close();

    if (fAnnCut > 0)
      name = Form("eff_histo_ANN_%1.2f.root", fAnnCut);
    else
      name = "eff_histo.root";
    TFile* ff = new TFile(name, "recreate");

    TDirectory* dir1 = ff->mkdir("mu Plus");
    dir1->cd();

    TDirectory* dir1a = dir1->mkdir("accepted mu Plus");
    dir1a->cd();
    for (int j = 0; j < 4; j++)
      acc_P[j][1]->Write();
    for (int j = 0; j < 4; j++)
      acc_Theta[j][1]->Write();

    dir1->cd();
    TDirectory* dir1b = dir1->mkdir("reconstructed mu Plus");
    dir1b->cd();
    if (fAnnCut < 0) {
      for (int j = 0; j < 3; j++)
        effReco_P[j][1]->Write();
      for (int j = 0; j < 3; j++)
        effReco_Theta[j][1]->Write();
      for (int j = 0; j < 4; j++)
        eff4pi_P[j][1]->Write();
      for (int j = 0; j < 4; j++)
        eff4pi_Theta[j][1]->Write();
    }
    effReco_P[3][1]->Write();
    effReco_Theta[3][1]->Write();
    eff4pi_P[4][1]->Write();
    eff4pi_Theta[4][1]->Write();

    TDirectory* dir2 = ff->mkdir("mu Minus");
    dir2->cd();

    TDirectory* dir2a = dir2->mkdir("accepted mu Minus");
    dir2a->cd();
    for (int j = 0; j < 4; j++)
      acc_P[j][2]->Write();
    for (int j = 0; j < 4; j++)
      acc_Theta[j][2]->Write();

    dir2->cd();
    TDirectory* dir2b = dir2->mkdir("reconstructed mu Minus");
    dir2b->cd();
    if (fAnnCut < 0) {
      for (int j = 0; j < 3; j++)
        effReco_P[j][2]->Write();
      for (int j = 0; j < 3; j++)
        effReco_Theta[j][2]->Write();
      for (int j = 0; j < 4; j++)
        eff4pi_P[j][2]->Write();
      for (int j = 0; j < 4; j++)
        eff4pi_Theta[j][2]->Write();
    }
    effReco_P[3][2]->Write();
    effReco_Theta[3][2]->Write();
    eff4pi_P[4][2]->Write();
    eff4pi_Theta[4][2]->Write();

    TDirectory* dir3 = ff->mkdir("signal");
    dir3->cd();

    TDirectory* dir3a = dir3->mkdir("accepted signal");
    dir3a->cd();
    for (int j = 0; j < 4; j++)
      acc_P[j][0]->Write();
    for (int j = 0; j < 4; j++)
      acc_Theta[j][0]->Write();

    dir3->cd();
    TDirectory* dir3b = dir3->mkdir("reconstructed signal");
    dir3b->cd();
    if (fAnnCut < 0) {
      for (int j = 0; j < 3; j++)
        effReco_P[j][0]->Write();
      for (int j = 0; j < 3; j++)
        effReco_Theta[j][0]->Write();
      for (int j = 0; j < 4; j++)
        eff4pi_P[j][0]->Write();
      for (int j = 0; j < 4; j++)
        eff4pi_Theta[j][0]->Write();
    }
    effReco_P[3][0]->Write();
    effReco_Theta[3][0]->Write();
    eff4pi_P[4][0]->Write();
    eff4pi_Theta[4][0]->Write();

    ff->Close();
    //  fPlutoFile->Close();
  } else {
    if (fAnnCut > 0)
      name = Form("sup_histo_ANN_%1.2f.root", fAnnCut);
    else
      name = "sup_histo.root";
    TFile* f = new TFile(name, "recreate");

    if (fAnnCut < 0)
      for (int j = 0; j < 5; j++)
        BgSup[j]->Write();
    BgSup[5]->Write();
    f->Close();
  }

  if (fAnnCut > 0)
    name = Form("YPtM_ANN_%1.2f.root", fAnnCut);
  else
    name = "YPtM.root";
  TFile* f = new TFile(name, "recreate");

  YPtM->Scale(1. / (Double_t) fEvent);
  YPtM->Write();
  f->Close();
}
// -------------------------------------------------------------------------


ClassImp(CbmAnaDimuonAnalysis);
