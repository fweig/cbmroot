//----------------------------------------
//
// 2019 A. Senger a.senger@gsi.de
//
// 2020 A. Senger: add ANN pID
//
//----------------------------------------


#ifndef CBMANADIMUONANALYSIS_H_
#define CBMANADIMUONANALYSIS_H_ 1

#include "FairTask.h"

#include <vector>

class CbmAnaDimuonAnalysis;
class CbmAnaMuonCandidate;
class CbmTrackMatch;
class CbmMatch;
class CbmMuchTrack;
class CbmTofHit;
class CbmMuchGeoScheme;
class CbmStsKFTrackFitter;
class CbmTrdTrackFitterKF;
class CbmGlobalTrackFitterKF;
class CbmVertex;
class TClonesArray;
class TLorentzVector;
class TString;
class TH1D;
class TH2D;
class TH3D;
class TProfile;
class TMultiLayerPerceptron;

class CbmAnaDimuonAnalysis : public FairTask {
public:
  CbmAnaDimuonAnalysis(TString name, TString setup);

  virtual ~CbmAnaDimuonAnalysis() {}
  virtual InitStatus Init();
  virtual void Exec(Option_t* opt);
  virtual void Finish();
  virtual void SetParContainers();

  void SetChi2StsCut(Double_t cut) { fChi2StsCut = cut; }
  void SetChi2MuchCut(Double_t cut) { fChi2MuchCut = cut; }
  void SetChi2VertexCut(Double_t cut) { fChi2VertexCut = cut; }

  void SetNofMuchCut(Int_t cut) { fNofMuchCut = cut; }
  void SetNofStsCut(Int_t cut) { fNofStsCut = cut; }
  void SetNofTrdCut(Int_t cut) { fNofTrdCut = cut; }
  void SetAnnCut(Double_t cut, Int_t neurons)
  {
    fAnnCut  = cut;
    fNeurons = neurons;
  }

  void SetSigmaTofCut(Int_t cut) { fSigmaTofCut = cut; }

  void UseCuts(Bool_t cut) { fUseCuts = cut; }
  void UseMC(Bool_t useMC) { fUseMC = useMC; }

  //  void SetHistoFileName(TString name) {fFileName = name; }
  //  void SetEffFileName(TString name)   {fEffFileName = name; }

  Double_t CalculateAnnValue(CbmAnaMuonCandidate* mu);
  Double_t CalculateAnnValue(Double_t P, Double_t M, Double_t Chi2Vertex, Double_t Chi2STS, Double_t Chi2MUCH,
                             Double_t Chi2TRD, Int_t NofSTS, Int_t NofMUCH, Int_t NofTRD, Int_t NofTOF);

  void FillProfile(TProfile* profile, Double_t param, Bool_t trigger);

private:
  Int_t fEvent;
  TClonesArray* fMCTracks;
  TClonesArray* fStsTracks;
  TClonesArray* fStsTrackMatches;
  TClonesArray* fMuchTracks;
  TClonesArray* fMuchTrackMatches;
  TClonesArray* fGlobalTracks;
  TClonesArray* fTrdTracks;
  TClonesArray* fTofHit;
  TClonesArray* fMuPlus;
  TClonesArray* fMuMinus;
  TClonesArray* fParticles;

  TTree* fInputTree;
  TFile* fPlutoFile;

  CbmStsKFTrackFitter* fFitter;
  CbmTrdTrackFitterKF* fFitterTRD;
  CbmGlobalTrackFitterKF* fFitterGlobal;

  CbmVertex* fVertex;

  Double_t fChi2StsCut;
  Double_t fChi2MuchCut;
  Double_t fChi2VertexCut;
  Double_t fAnnCut;
  Int_t fNeurons;
  Int_t fSigmaTofCut;

  Double_t fMass;

  Bool_t fUseCuts;
  Bool_t fUseMC;

  Int_t fNofMuchCut;
  Int_t fNofStsCut;
  Int_t fNofTrdCut;

  Double_t p0min, p1min, p2min;
  Double_t p0max, p1max, p2max;

  TString fFileAnnName;
  //  TString fEffFileName;
  //  TString fFileName;
  TString fPlutoFileName;
  TString fSetupName;

  CbmMuchGeoScheme* fGeoScheme;

  TH2D *YPt_pluto, *YPt_StsAcc, *YPt_StsMuchAcc, *YPt_StsMuchTrdAcc, *YPt_StsMuchTrdTofAcc;
  TH2D *YPt_VtxReco, *YPt_VtxStsReco, *YPt_VtxStsMuchReco, *YPt_VtxStsMuchTrdReco, *YPt_VtxStsMuchTrdTofReco;
  TH3D* YPtM;

  TProfile *acc_P[4][3], *acc_Theta[4][3];
  TProfile *effReco_P[4][3], *effReco_Theta[4][3];
  TProfile *eff4pi_P[5][3], *eff4pi_Theta[5][3];

  TH1D* BgSup[6];

  CbmAnaDimuonAnalysis(const CbmAnaDimuonAnalysis&);
  CbmAnaDimuonAnalysis operator=(const CbmAnaDimuonAnalysis&);

  ClassDef(CbmAnaDimuonAnalysis, 2);
};

#endif
