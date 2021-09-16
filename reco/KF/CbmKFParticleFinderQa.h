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
  void SetMCTrackBranchName(const TString& name) { fMCTracksBranchName = name; }
  void SetTrackMatchBranchName(const TString& name) { fTrackMatchBranchName = name; }

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

  //names of input branches
  TString fMCTracksBranchName;    //! Name of the input TCA with MC tracks
  TString fTrackMatchBranchName;  //! Name of the input TCA with track match

  //input branches
  CbmMCDataArray* fMCTrackArray;  //mc tracks
  TClonesArray* fMCTrackArrayEvent;
  CbmMCEventList* fEventList;      //mc event list in timeslice
  TClonesArray* fTrackMatchArray;  //track match

  // output arrays of particles
  TClonesArray* fRecParticles;    // output array of KF Particles
  TClonesArray* fMCParticles;     // output array of MC Particles
  TClonesArray* fMatchParticles;  // output array of match objects

  Bool_t fSaveParticles;
  Bool_t fSaveMCParticles;

  bool fTimeSliceMode;

  //output file with histograms
  TString fOutFileName;
  TFile* fOutFile;
  TString fEfffileName;
  //KF Particle QA
  KFTopoPerformance* fTopoPerformance;

  Int_t fPrintFrequency;
  Int_t fNEvents;
  Double_t fTime[5];

  //for super event analysis
  bool fSuperEventAnalysis;

  //for tests
  TString fReferenceResults;
  int fDecayToAnalyse;
  bool fCheckDecayQA;
  bool fTestOk;

  ClassDef(CbmKFParticleFinderQa, 1);
};

#endif
