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

#include "FairTask.h"

class TClonesArray;
class TFile;
class TDirectory;
class TH1F;

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

  //input branches
  TClonesArray* fStsTrackArray {nullptr};
  TClonesArray* fMCTrackArray {nullptr};
  TClonesArray* fStsTrackMatchArray {nullptr};

  //output file with histograms
  TString fHistoFileName {"CbmBbaAlignmentHisto.root"};
  TFile* fHistoFile {nullptr};
  TDirectory* fHistoDir {nullptr};

  Int_t fNEvents {0};

  //histograms
  TH1F* hTestHisto {nullptr};

  ClassDef(CbmBbaAlignmentTask, 1);
};

#endif
