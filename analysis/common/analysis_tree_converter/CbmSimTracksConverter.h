/* Copyright (C) 2020-2021 Physikalisches Institut, Eberhard Karls Universitaet Tuebingen, Tuebingen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Daniel Wielanek, Viktor Klochkov [committer] */

#ifndef ANALYSIS_TREE_SIMTRACKSCONVERTER_H_
#define ANALYSIS_TREE_SIMTRACKSCONVERTER_H_

#include "CbmConverterTask.h"

#include <TString.h>

#include "AnalysisTree/Detector.hpp"

class TClonesArray;
class UEvent;
class TFile;
class TTree;
class FairMCEventHeader;

class CbmSimTracksConverter final : public CbmConverterTask {


public:
  explicit CbmSimTracksConverter(std::string out_branch_name, std::string match_to = "", std::string unigen_name = "",
                                 std::string mc_eventheader = "MCEventHeader.")
    : CbmConverterTask(std::move(out_branch_name), std::move(match_to))
    , fUnigenEvent(nullptr)
    , fFile(nullptr)
    , fTree(nullptr)
    , fUnigenFile(unigen_name)
    , fEventHeaderBranch(mc_eventheader)
    , fEntry(0)
    , fUseUnigen(kFALSE)
    , fDeltaPhi(0)
    , fBetaCM(0) {};

  ~CbmSimTracksConverter() final;

  void Init() final;
  void Exec() final;
  void Finish() final {};

private:
  AnalysisTree::Particles* sim_tracks_ {nullptr};
  FairMCEventHeader* cbm_header_ {nullptr};
  TClonesArray* cbm_mc_tracks_ {nullptr};
  UEvent* fUnigenEvent;
  TFile* fFile;
  TTree* fTree;
  TString fUnigenFile;
  TString fEventHeaderBranch;
  Int_t fEntry;
  Bool_t fUseUnigen;
  Double_t fDeltaPhi;
  Double_t fBetaCM;  // CM velocity in the lab frame

  ClassDef(CbmSimTracksConverter, 1)
};

#endif
