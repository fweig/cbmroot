/* Copyright (C) 2017-2021 Faculty of Physics, Warsaw University of Technology, Warsaw
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Daniel Wielanek [committer] */

/*
 * NicaCbmDstSource.cxx
 *
 *  Created on: 27 mar 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CbmAnaTreeSource.h"

#include "CbmAnaTreeContainer.h"

#include <FairRootManager.h>
#include <Logger.h>

#include <TBranch.h>
#include <TChain.h>
#include <TFile.h>

#include <fstream>

#include "AnalysisTree/Configuration.hpp"


CbmAnaTreeSource::CbmAnaTreeSource() : CbmAnaTreeSource("", "aTree") {}

CbmAnaTreeSource::CbmAnaTreeSource(TString inFile, TString treeName)
  : fNFiles(1)
  , fTreeName(treeName)
  , fChain(nullptr)
  , fFileName(nullptr)
  , fContainerReco(nullptr)
  , fContainerSim(nullptr)
{
  fFileName    = new TString[1];
  fFileName[0] = inFile;
}

CbmAnaTreeSource::~CbmAnaTreeSource()
{
  if (fFileName) delete[] fFileName;
}

Bool_t CbmAnaTreeSource::Init()
{
  LOG(debug) << "Init source ";
  FairRootManager* mngr = FairRootManager::Instance();
  fChain                = new TChain(fTreeName);  //new name of tree?
  TString first_file_name;
  if (fFileName[0].EndsWith(".root")) {
    for (int i = 0; i < fNFiles; i++) {
      LOG(debug) << "CbmAnaTree: opening single file" << fFileName[i];
      fChain->Add(fFileName[i]);
    }
    first_file_name = fFileName[0];
  }
  else {  // this is long list
    std::ifstream list;
    list.open(fFileName[0]);
    bool first = true;
    do {
      TString temp;
      list >> temp;
      if (temp.Length() > 1) {
        fChain->Add(temp);
        if (first) {
          first_file_name = temp;
          first           = false;
        }
      }
      else {
        break;
      }
      LOG(debug) << "Adding file " << temp << " to chain";
    } while (!list.eof());
    list.close();
  }
  LOG(debug) << "load container ";
  fContainerReco   = new CbmAnaTreeRecoSourceContainer();
  fContainerSim    = new CbmAnaTreeMcSourceContainer();
  Bool_t recoAvail = fContainerReco->ConnectToTree(fChain);
  Bool_t simAvail  = fContainerSim->ConnectToTree(fChain);
  if (recoAvail == kFALSE) {
    delete fContainerReco;
    fContainerReco = nullptr;
  }
  if (simAvail == kFALSE) {
    delete fContainerSim;
    fContainerSim = nullptr;
  }
  LOG(debug) << "load conf ";
  LoadConf(first_file_name);
  mngr->SetInChain(fChain, -1);
  if (fContainerReco) {
    mngr->Register("CbmAnaTreeSourceContainer.", "CbmAnaTreeSourceContainer", fContainerReco, kFALSE);
  }
  if (fContainerSim) {
    mngr->Register("CbmAnaTreeMcSourceContainer.", "CbmAnaTreeMcSourceContainer", fContainerSim, kFALSE);
  }
  return kTRUE;
}

Int_t CbmAnaTreeSource::ReadEvent(UInt_t unsignedInt)
{
  fChain->GetEntry(unsignedInt);
  return 0;
}

void CbmAnaTreeSource::Close() {}

void CbmAnaTreeSource::LoadConf(TString name)
{
  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  TFile* f = new TFile(name);
  LOG_IF(fatal, !f) << "Could not open file " << name;
  // TTree *tree = f->Get<TTree>("aTree");
  //  AnalysisTree::Configuration* conf = (AnalysisTree::Configuration*) f->Get("Configuration");
  AnalysisTree::Configuration* conf = f->Get<AnalysisTree::Configuration>("Configuration");
  LOG_IF(fatal, !conf) << "Could not read AnalysisTree configuration from file " << name;
  if (fContainerReco) {
    fContainerReco->GetFieldIds().vtx_px   = conf->GetBranchConfig("VtxTracks").GetFieldId("px");
    fContainerReco->GetFieldIds().vtx_py   = conf->GetBranchConfig("VtxTracks").GetFieldId("py");
    fContainerReco->GetFieldIds().vtx_pz   = conf->GetBranchConfig("VtxTracks").GetFieldId("pz");
    fContainerReco->GetFieldIds().vtx_dcax = conf->GetBranchConfig("VtxTracks").GetFieldId("dcax");
    fContainerReco->GetFieldIds().vtx_dcay = conf->GetBranchConfig("VtxTracks").GetFieldId("dcay");
    fContainerReco->GetFieldIds().vtx_dcaz = conf->GetBranchConfig("VtxTracks").GetFieldId("dcaz");

    fContainerReco->GetFieldIds().vtx_chi2    = conf->GetBranchConfig("VtxTracks").GetFieldId("chi2");
    fContainerReco->GetFieldIds().vtx_vtxchi2 = conf->GetBranchConfig("VtxTracks").GetFieldId("vtx_chi2");
    fContainerReco->GetFieldIds().vtx_q       = conf->GetBranchConfig("VtxTracks").GetFieldId("q");
    fContainerReco->GetFieldIds().vtx_nhits   = conf->GetBranchConfig("VtxTracks").GetFieldId("nhits");
    fContainerReco->GetFieldIds().vtx_mvdhits = conf->GetBranchConfig("VtxTracks").GetFieldId("nhits_mvd");

    fContainerReco->GetFieldIds().tof_mass2 = conf->GetBranchConfig("TofHits").GetFieldId("mass2");

    fContainerReco->GetFieldIds().vtx_x    = conf->GetBranchConfig("VtxTracks").GetFieldId("x");
    fContainerReco->GetFieldIds().vtx_cx0  = conf->GetBranchConfig("VtxTracks").GetFieldId("cx0");
    fContainerReco->GetFieldIds().vtx_cov1 = conf->GetBranchConfig("VtxTracks").GetFieldId("cov1");
  }
  if (fContainerSim) {
    fContainerSim->GetFieldIds().px        = conf->GetBranchConfig("SimParticles").GetFieldId("px");
    fContainerSim->GetFieldIds().py        = conf->GetBranchConfig("SimParticles").GetFieldId("py");
    fContainerSim->GetFieldIds().pz        = conf->GetBranchConfig("SimParticles").GetFieldId("pz");
    fContainerSim->GetFieldIds().mass      = conf->GetBranchConfig("SimParticles").GetFieldId("mass");
    fContainerSim->GetFieldIds().pdg       = conf->GetBranchConfig("SimParticles").GetFieldId("pid");
    fContainerSim->GetFieldIds().motherId  = conf->GetBranchConfig("SimParticles").GetFieldId("mother_id");
    fContainerSim->GetFieldIds().event_b   = conf->GetBranchConfig("SimEventHeader").GetFieldId("b");
    fContainerSim->GetFieldIds().event_psi = conf->GetBranchConfig("SimEventHeader").GetFieldId("psi_RP");
    fContainerSim->GetFieldIds().freezX    = conf->GetBranchConfig("SimParticles").GetFieldId("xfreez");
    fContainerSim->GetFieldIds().freezY    = conf->GetBranchConfig("SimParticles").GetFieldId("yfreez");
    fContainerSim->GetFieldIds().freezZ    = conf->GetBranchConfig("SimParticles").GetFieldId("zfreez");
    fContainerSim->GetFieldIds().freezT    = conf->GetBranchConfig("SimParticles").GetFieldId("tfreez");
  }

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;

  f->Close();
  delete f;
}

void CbmAnaTreeSource::AddFile(TString file)
{
  TString* temp = fFileName;
  fFileName     = new TString[fNFiles + 1];
  for (int i = 0; i < fNFiles; i++) {
    fFileName[i] = temp[i];
  }
  delete[] temp;
  fFileName[fNFiles] = file;
  fNFiles            = fNFiles + 1;
}

Int_t CbmAnaTreeSource::CheckMaxEventNo(Int_t /*int1*/) { return fChain->GetEntries(); }
