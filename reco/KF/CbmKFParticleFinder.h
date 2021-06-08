/* Copyright (C) 2014-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Maxim Zyzak, Volker Friese [committer] */

//-----------------------------------------------------------
//-----------------------------------------------------------

#ifndef CbmKFParticleFinder_HH
#define CbmKFParticleFinder_HH

#include "CbmStsTrack.h"

#include "FairTask.h"

#include "TString.h"

#include <vector>

class CbmKFParticleFinderPID;
class KFParticleTopoReconstructor;
class TClonesArray;
class KFParticleFinder;
class KFPTrackVector;
class CbmMCEventList;
class CbmMCDataArray;
class CbmVertex;

struct KFFieldVector {
  float fField[10];
};

class CbmKFParticleFinder : public FairTask {
public:
  // Constructors/Destructors ---------
  CbmKFParticleFinder(const char* name = "CbmKFParticleFinder", Int_t iVerbose = 0);
  ~CbmKFParticleFinder();

  void UseMCPV() { fPVFindMode = 0; }
  void ReconstructSinglePV() { fPVFindMode = 1; }
  void RconstructMultiplePV() { fPVFindMode = 2; }
  void UseReconstructedPV() { fPVFindMode = 3; }

  void SetStsTrackBranchName(const TString& name) { fStsTrackBranchName = name; }

  virtual InitStatus Init();
  virtual void Exec(Option_t* opt);
  virtual void Finish();

  const KFParticleTopoReconstructor* GetTopoReconstructor() const { return fTopoReconstructor; }

  void SetPIDInformation(CbmKFParticleFinderPID* pid) { fPID = pid; }

  // set cuts
  void SetPrimaryProbCut(float prob);

  // Set SE analysis
  void SetSuperEventAnalysis();

  //KF Particle Finder cuts
  void SetMaxDistanceBetweenParticlesCut(float cut);
  void SetLCut(float cut);
  void SetChiPrimaryCut2D(float cut);
  void SetChi2Cut2D(float cut);
  void SetLdLCut2D(float cut);
  void SetLdLCutXiOmega(float cut);
  void SetChi2TopoCutXiOmega(float cut);
  void SetChi2CutXiOmega(float cut);
  void SetChi2TopoCutResonances(float cut);
  void SetChi2CutResonances(float cut);
  void SetPtCutLMVM(float cut);
  void SetPCutLMVM(float cut);
  void SetPtCutJPsi(float cut);
  void SetPtCutCharm(float cut);
  void SetChiPrimaryCutCharm(float cut);
  void SetLdLCutCharmManybodyDecays(float cut);
  void SetChi2TopoCutCharmManybodyDecays(float cut);
  void SetChi2CutCharmManybodyDecays(float cut);
  void SetLdLCutCharm2D(float cut);
  void SetChi2TopoCutCharm2D(float cut);
  void SetChi2CutCharm2D(float cut);

  //Add decay to the reconstruction list. By default, if list is empty - all decays are reconstructed.
  void AddDecayToReconstructionList(int pdg);

private:
  double InversedChi2Prob(double p, int ndf) const;
  void FillKFPTrackVector(KFPTrackVector* tracks, const std::vector<CbmStsTrack>& vRTracks,
                          const std::vector<KFFieldVector>& vField, const std::vector<int>& pdg,
                          const std::vector<int>& trackId, const std::vector<float>& vChiToPrimVtx,
                          bool atFirstPoint = 1) const;

  const CbmKFParticleFinder& operator=(const CbmKFParticleFinder&);
  CbmKFParticleFinder(const CbmKFParticleFinder&);

  //direct access to the KF Particle Finder object
  KFParticleFinder* GetKFParticleFinder();

  //names of input branches
  TString fStsTrackBranchName;  //! Name of the input TCA with reco tracks

  //input branches
  TClonesArray* fTrackArray;
  TClonesArray* fEvents;
  CbmMCDataArray* fMCTrackArray;     //mc tracks in timeslices
  TClonesArray* fMCTrackArrayEvent;  //mc tracks in event-by-event mode
  CbmMCEventList* fEventList;        //mc event list in timeslice
  CbmVertex* fCbmPrimVertex;         //mc primary vertex

  //topology reconstructor
  KFParticleTopoReconstructor* fTopoReconstructor;
  //   KFParticleTopoReconstructor* eventTopoReconstructor;
  int fPVFindMode;

  //PID information
  CbmKFParticleFinderPID* fPID;

  //for super event analysis
  bool fSuperEventAnalysis;
  std::vector<CbmStsTrack> fSETracks;
  std::vector<KFFieldVector> fSEField;
  std::vector<int> fSEpdg;
  std::vector<int> fSETrackId;
  std::vector<float> fSEChiPrim;

  bool fTimeSliceMode;

  ClassDef(CbmKFParticleFinder, 1);
};

#endif
