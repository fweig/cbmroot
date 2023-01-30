/* Copyright (C) 2014-2018 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer], Maksym Zyzak */

//-----------------------------------------------------------
//-----------------------------------------------------------

#ifndef CbmKFParticleFinderQa_HH
#define CbmKFParticleFinderQa_HH

#include "FairTask.h"

#include "TString.h"

#include <vector>

class KFParticleTopoReconstructor;
class KFTopoPerformance;
class TClonesArray;
class CbmMCEventList;
class CbmMCDataArray;
class TFile;
class TObject;

class CbmKFParticleFinderQa : public FairTask {
public:
  // Constructors/Destructors ---------
  CbmKFParticleFinderQa(const char* name = "CbmKFParticleFinderQa", Int_t iVerbose = 0,
                        const KFParticleTopoReconstructor* tr = 0, TString outFileName = "CbmKFParticleFinderQa.root");
  ~CbmKFParticleFinderQa();

  void SetEffFileName(const TString& name) { fEfffileName = name; }

  virtual InitStatus Init();
  virtual void Exec(Option_t* opt);
  virtual void Finish();

  void SetPrintEffFrequency(Int_t n);

  void SaveParticles(Bool_t b = 1) { fSaveParticles = b; }
  void SaveMCParticles(Bool_t b = 1) { fSaveMCParticles = b; }

  // Set SE analysis
  void SetSuperEventAnalysis() { fSuperEventAnalysis = 1; }

  //Tests
  void SetCheckDecayQA() { fCheckDecayQA = true; }
  void SetReferenceResults(TString t) { fReferenceResults = t; }
  void SetDecayToAnalyse(int iDecay) { fDecayToAnalyse = iDecay; }
  bool IsTestPassed() { return fTestOk; }

private:
  const CbmKFParticleFinderQa& operator=(const CbmKFParticleFinderQa&);
  CbmKFParticleFinderQa(const CbmKFParticleFinderQa&);

  void WriteHistosCurFile(TObject* obj);
  void FitDecayQAHistograms(float sigma[14], const bool saveReferenceResults = false) const;
  void CheckDecayQA();

  //input branches
  TClonesArray* fRecoEvents {nullptr};      //! Array of CbmEvent objects
  CbmMCDataArray* fMCTrackArray {nullptr};  //mc tracks
  TClonesArray* fMCTrackArrayEvent {nullptr};
  CbmMCEventList* fMcEventList {nullptr};    //mc event list in timeslice
  TClonesArray* fTrackMatchArray {nullptr};  //track match

  // output arrays of particles
  TClonesArray* fRecParticles {nullptr};    // output array of KF Particles
  TClonesArray* fMCParticles {nullptr};     // output array of MC Particles
  TClonesArray* fMatchParticles {nullptr};  // output array of match objects

  Bool_t fSaveParticles {false};
  Bool_t fSaveMCParticles {false};

  bool fLegacyEventMode {false};  // event-by-event mode where data is stored in an old-fasion way

  //output file with histograms
  TString fOutFileName {"CbmKFParticleFinderQa.root"};
  TFile* fOutFile {nullptr};
  TString fEfffileName {"Efficiency.txt"};

  //KF Particle QA
  KFTopoPerformance* fTopoPerformance {nullptr};

  Int_t fPrintFrequency {100};
  Int_t fNEvents {0};
  Double_t fTime[5];

  //for super event analysis
  bool fSuperEventAnalysis {false};

  //for tests
  TString fReferenceResults {"./"};
  int fDecayToAnalyse {-1};
  bool fCheckDecayQA {false};
  bool fTestOk {false};

  ClassDef(CbmKFParticleFinderQa, 1);
};

#endif
