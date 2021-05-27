/** @file CbmKFParticleFinderPID.h
  * @copyright Copyright (C) 2014-2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Maxim Zyzak, Volker Friese [orginator] **/

//-----------------------------------------------------------
//-----------------------------------------------------------

#ifndef CbmKFParticleFinderPID_HH
#define CbmKFParticleFinderPID_HH

#include "CbmMCDataArray.h"
#include "CbmMCEventList.h"

#include "FairTask.h"

#include "TString.h"

#include <vector>

class TClonesArray;
class TFile;
class TObject;
class CbmDigiManager;

class CbmKFParticleFinderPID : public FairTask {
public:
  // Constructors/Destructors ---------
  CbmKFParticleFinderPID(const char* name = "CbmKFParticleFinderPID", Int_t iVerbose = 0);
  ~CbmKFParticleFinderPID();

  void SetStsTrackBranchName(const TString& name) { fStsTrackBranchName = name; }
  void SetGlobalTrackBranchName(const TString& name) { fGlobalTrackBranchName = name; }
  void SetTofBranchName(const TString& name) { fTofBranchName = name; }
  void SetMCTrackBranchName(const TString& name) { fMCTracksBranchName = name; }
  void SetTrackMatchBranchName(const TString& name) { fTrackMatchBranchName = name; }
  void SetTrdBranchName(const TString& name) { fTrdBranchName = name; }
  void SetRichBranchName(const TString& name) { fRichBranchName = name; }
  void SetMuchTrackBranchName(const TString& name) { fMuchTrackBranchName = name; }

  virtual InitStatus Init();
  virtual void Exec(Option_t* opt);
  virtual void Finish();

  void SetPIDMode(int mode) { fPIDMode = mode; }
  void UseNoPID() { fPIDMode = 0; }
  void UseMCPID() { fPIDMode = 1; }
  void UseDetectorPID() { fPIDMode = 2; }
  void SetSIS100() { fSisMode = 0; }
  void SetSIS300() { fSisMode = 1; }

  void DoNotUseTRD() { fTrdPIDMode = 0; }
  void UseTRDWknPID() { fTrdPIDMode = 1; }
  void UseTRDANNPID() { fTrdPIDMode = 2; }

  void DoNotUseRICH() { fRichPIDMode = 0; }
  void UseRICHRvspPID() { fRichPIDMode = 1; }
  void UseRICHANNPID() { fRichPIDMode = 2; }

  void DoNotUseMuch() { fMuchMode = 0; }
  void UseMuch() { fMuchMode = 1; }

  void UseSTSdEdX() { fUseSTSdEdX = kTRUE; }
  void DoNotUseSTSdEdX() { fUseSTSdEdX = kFALSE; }
  void UseTRDdEdX() { fUseTRDdEdX = kTRUE; }
  void DoNotUseTRDdEdX() { fUseTRDdEdX = kFALSE; }

  //setters for MuCh cuts
  void SetNMinStsHitsForMuon(int cut) { fMuchCutsInt[0] = cut; }
  void SetNMinMuchHitsForLMVM(int cut) { fMuchCutsInt[1] = cut; }
  void SetNMinMuchHitsForJPsi(int cut) { fMuchCutsInt[2] = cut; }
  void SetMaxChi2ForStsMuonTrack(float cut) { fMuchCutsFloat[0] = cut; }
  void SetMaxChi2ForMuchMuonTrack(float cut) { fMuchCutsFloat[1] = cut; }

  const std::vector<int>& GetPID() const { return fPID; }

private:
  const CbmKFParticleFinderPID& operator=(const CbmKFParticleFinderPID&);
  CbmKFParticleFinderPID(const CbmKFParticleFinderPID&);

  void SetMCPID();
  void SetRecoPID();

  //names of input branches
  TString fStsTrackBranchName;
  TString fGlobalTrackBranchName;
  TString fStsHitBranchName;
  TString fStsClusterBranchName;
  TString fStsDigiBranchName;
  TString fTofBranchName;
  TString fMCTracksBranchName;
  TString fTrackMatchBranchName;
  TString fTrdBranchName;
  TString fTrdHitBranchName;
  TString fRichBranchName;
  TString fMuchTrackBranchName;

  //input branches
  TClonesArray* fTrackArray;        //input reco tracks
  TClonesArray* fGlobalTrackArray;  //input reco tracks
  TClonesArray* fStsHitArray;
  TClonesArray* fStsClusterArray;
  CbmDigiManager* fDigiManager;    //! Interface to digi branch
  TClonesArray* fTofHitArray;      //input reco tracks
  TClonesArray* fMCTrackArray;     //mc tracks
  TClonesArray* fTrackMatchArray;  //track match
  TClonesArray* fTrdTrackArray;
  TClonesArray* fTrdHitArray;
  TClonesArray* fRichRingArray;
  TClonesArray* fMuchTrackArray;  //input much tracks
  CbmMCDataArray* fMCTracks;

  //PID variables
  Int_t fPIDMode;
  Int_t fSisMode;
  Int_t fTrdPIDMode;
  Int_t fRichPIDMode;
  Int_t fMuchMode;
  Bool_t fUseSTSdEdX;
  Bool_t fUseTRDdEdX;

  bool fTimeSliceMode;

  //MuCh cuts
  float fMuchCutsFloat[2];
  int fMuchCutsInt[3];

  std::vector<int> fPID;

  ClassDef(CbmKFParticleFinderPID, 1);
};

#endif
