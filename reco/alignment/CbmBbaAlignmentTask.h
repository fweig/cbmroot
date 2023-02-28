/* Copyright (C) 2023-2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: S.Gorbunov[committer], N.Bluhme */

/// @file    CbmBbaAlignmentTask.h
/// @author  Sergey Gorbunov
/// @author  Nora Bluhme
/// @date    20.01.2023
/// @brief   Task class for alignment
///

#ifndef CbmBbaAlignmentTask_H
#define CbmBbaAlignmentTask_H

#include "CbmMvdHit.h"
#include "CbmStsHit.h"
#include "CbmStsTrack.h"

#include "FairTask.h"

class TClonesArray;
class TFile;
class TDirectory;
class TH1F;

///
/// an example of alignment using BBA package
///
/// you need to switch to the double precision in /algo/ca/CaSimdVc.h
/// by uncommenting this line there:
///
/// typedef Vc::double_v fvec;
///
class CbmBbaAlignmentTask : public FairTask {
public:
  // Constructors/Destructors ---------
  CbmBbaAlignmentTask(const char* name = "CbmBbaAlignmentTask", Int_t iVerbose = 0,
                      TString histoFileName = "CbmBbaAlignmentHisto.root");
  ~CbmBbaAlignmentTask();

  Int_t GetZtoNStation(Double_t getZ);

  InitStatus Init();
  void Exec(Option_t* opt);
  void Finish();

private:
  const CbmBbaAlignmentTask& operator=(const CbmBbaAlignmentTask&);
  CbmBbaAlignmentTask(const CbmBbaAlignmentTask&);

  void WriteHistosCurFile(TObject* obj);
  int GetHistoIndex(int pdg);

  void ApplyAlignment(const std::vector<double>& par);

  double CostFunction(const std::vector<double>& par);

  //input data arrays
  TClonesArray* fInputMvdHits {nullptr};
  TClonesArray* fInputStsHits {nullptr};
  TClonesArray* fInputStsTracks {nullptr};

  TClonesArray* fInputMcTracks {nullptr};         // Mc info for debugging
  TClonesArray* fInputStsTrackMatches {nullptr};  // Mc info for debugging

  // collection of selected tracks and hits
  std::vector<CbmStsTrack> fTracks;
  std::vector<CbmMvdHit> fMvdHits;
  std::vector<CbmStsHit> fStsHits;

  // temporary array with aligned hits
  std::vector<CbmStsHit> fStsHitsAligned;

  // array with pdg hypothesis for tracks
  std::vector<int> fTrackPdg;

  //output file with histograms
  TString fHistoFileName {"CbmBbaAlignmentHisto.root"};
  TFile* fHistoFile {nullptr};
  TDirectory* fHistoDir {nullptr};

  Int_t fNEvents {0};

  Int_t fMaxNtracks {10000};

  double fCostIdeal {1.e10};

  //histograms
  TH1F* hTestHisto {nullptr};

  ClassDef(CbmBbaAlignmentTask, 1);
};

#endif
