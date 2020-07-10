//-----------------------------------------------------------
//-----------------------------------------------------------

#ifndef CbmKFTrackQA_HH
#define CbmKFTrackQA_HH

#include "FairTask.h"

#include "TString.h"

#include <map>
#include <vector>

class KFParticleTopoReconstructor;
class KFTopoPerformance;
class TClonesArray;
class TFile;
class TDirectory;
class TH1F;
class TH2F;
class TObject;
class CbmMCDataArray;

class CbmKFTrackQA : public FairTask {
public:
  // Constructors/Destructors ---------
  CbmKFTrackQA(const char* name    = "CbmKFTrackQA",
               Int_t iVerbose      = 0,
               TString outFileName = "CbmKFTrackQA.root");
  ~CbmKFTrackQA();

  void SetStsTrackBranchName(const TString& name) {
    fStsTrackBranchName = name;
  }
  void SetGlobalTrackBranchName(const TString& name) {
    fGlobalTrackBranchName = name;
  }
  void SetTofBranchName(const TString& name) { fTofBranchName = name; }
  void SetMCTrackBranchName(const TString& name) { fMCTracksBranchName = name; }
  void SetTrackMatchBranchName(const TString& name) {
    fStsTrackMatchBranchName = name;
  }
  void SetMuchTrackMatchBranchName(const TString& name) {
    fMuchTrackMatchBranchName = name;
  }
  void SetTrdBranchName(const TString& name) { fTrdBranchName = name; }
  void SetTrdHitBranchName(const TString& name) { fTrdHitBranchName = name; }
  void SetRichBranchName(const TString& name) { fRichBranchName = name; }
  void SetMuchTrackBranchName(const TString& name) {
    fMuchTrackBranchName = name;
  }
  Int_t GetZtoNStation(Double_t getZ);

  virtual InitStatus Init();
  virtual void Exec(Option_t* opt);
  virtual void Finish();

private:
  const CbmKFTrackQA& operator=(const CbmKFTrackQA&);
  CbmKFTrackQA(const CbmKFTrackQA&);

  void WriteHistosCurFile(TObject* obj);
  int GetHistoIndex(int pdg);

  //names of input branches
  TString fStsTrackBranchName;
  TString fGlobalTrackBranchName;
  TString fRichBranchName;
  TString fTrdBranchName;
  TString fTrdHitBranchName;
  TString fTofBranchName;
  TString fMuchTrackBranchName;
  TString fMCTracksBranchName;
  TString fStsTrackMatchBranchName;
  TString fRichRingMatchBranchName;
  TString fTrdTrackMatchBranchName;
  TString fTofHitMatchBranchName;
  TString fMuchTrackMatchBranchName;

  //input branches
  TClonesArray* fStsTrackArray;
  TClonesArray* fGlobalTrackArray;
  TClonesArray* fRichRingArray;
  TClonesArray* fTrdTrackArray;
  TClonesArray* fTrdHitArray;
  TClonesArray* fTofHitArray;
  TClonesArray* fMuchTrackArray;
  TClonesArray* fMCTrackArray;
  TClonesArray* fStsTrackMatchArray;
  TClonesArray* fRichRingMatchArray;
  TClonesArray* fTrdTrackMatchArray;
  TClonesArray* fTofHitMatchArray;
  TClonesArray* fMuchTrackMatchArray;

  CbmMCDataArray* fTofPoints;  // CbmTofPoint array

  //output file with histograms
  TString fOutFileName;
  TFile* fOutFile;
  TDirectory* fHistoDir;

  Int_t fNEvents;
  std::map<int, int> fPDGtoIndexMap;

  //histograms
  //STS
  static const int NStsHisto = 3;
  TH1F* hStsHisto
    [8]
    [NStsHisto];  //All tracks, electrons, muons, pion, kaon, protons, fragments, ghost
  TH1F* hStsFitHisto[8][10];
  //Much
  static const int NMuchHisto = 5;
  TH1F* hMuchHisto[3][NMuchHisto];  //Muons, Background, Ghost
    //RICH
  static const int NRichRingHisto2D = 3;  // r, axis a, axis b
  TH2F* hRichRingHisto2D
    [10]
    [NRichRingHisto2D];  //All tracks, electrons, muons, pions, kaons, protons, fragments, mismatch, ghost track, ghost ring
    //Trd
  static const int NTrdHisto = 2;
  TH1F* hTrdHisto
    [14]
    [NTrdHisto];  //All tracks, electrons, muons, pions, kaons, protons, fragments, mismatch, ghost track, ghost trd track, d, t, He3, He4
  static const int NTrdHisto2D = 1;
  TH2F* hTrdHisto2D[14][NTrdHisto2D];
  //Tof
  static const int NTofHisto2D = 2;
  TH2F* hTofHisto2D
    [14]
    [NTofHisto2D];  //All tracks, electrons, muons, pions, kaons, protons, fragments, mismatch, ghost hit, ghost tof hit, d, t, He3, He4


  ClassDef(CbmKFTrackQA, 1);
};

#endif
