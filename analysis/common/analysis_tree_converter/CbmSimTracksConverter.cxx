/* Copyright (C) 2020-2021 Physikalisches Institut, Eberhard Karls Universitaet Tuebingen, Tuebingen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Daniel Wielanek, Viktor Klochkov [committer] */

#include "CbmSimTracksConverter.h"

#include "CbmMCTrack.h"

#include "FairLogger.h"
#include "FairMCEventHeader.h"
#include "FairRootManager.h"

#include <TClonesArray.h>
#include <TDirectory.h>
#include <TFile.h>
#include <TRandom.h>
#include <TString.h>
#include <TTree.h>

#include <cassert>
#include <vector>

#include "AnalysisTree/TaskManager.hpp"
#include "UEvent.h"
#include "UParticle.h"
#include "URun.h"

ClassImp(CbmSimTracksConverter)

  void CbmSimTracksConverter::Init()
{

  assert(!out_branch_.empty());
  auto* ioman = FairRootManager::Instance();


  cbm_mc_tracks_ = (TClonesArray*) ioman->GetObject("MCTrack");
  cbm_header_    = (FairMCEventHeader*) ioman->GetObject(fEventHeaderBranch);

  AnalysisTree::BranchConfig sim_particles_branch(out_branch_, AnalysisTree::DetType::kParticle);
  sim_particles_branch.AddField<int>("mother_id", "id of mother particle, -1 for primaries");
  sim_particles_branch.AddField<int>("cbmroot_id", "track id in CbmRoot transport file");
  sim_particles_branch.AddField<int>("geant_process_id", "");
  sim_particles_branch.AddFields<int>({"n_hits_mvd", "n_hits_sts", "n_hits_trd"}, "Number of hits in the detector");

  //TDirectory* currentDir = gDirectory;
  if (fUnigenFile.Length() > 0) {
    fFile = new TFile(fUnigenFile, "READ");
    fFile->Print();
    if (fFile->IsOpen()) {
      fTree = fFile->Get<TTree>("events");
      if (fTree) fUseUnigen = kTRUE;
      fTree->SetBranchAddress("event", &fUnigenEvent);
      URun* run = fFile->Get<URun>("run");
      if (run == nullptr) {
        LOG(error) << "CbmSimTracksConverter: No run description in urqmd file!";
        delete fFile;
        fFile      = nullptr;
        fUseUnigen = kFALSE;
      }
      else {
        Double_t mProt = 0.938272;
        Double_t pTarg = run->GetPTarg();  // target momentum per nucleon
        Double_t pProj = run->GetPProj();  // projectile momentum per nucleon
        Double_t eTarg = TMath::Sqrt(pProj * pProj + mProt * mProt);
        fBetaCM        = pTarg / eTarg;
      }
    }
  }
  else {
    LOG(info) << "lack of unigen file" << fUnigenFile;
  }

  if (fUseUnigen) {
    sim_particles_branch.AddField<float>("xfreez", "x freezout coordinate fm/c");
    sim_particles_branch.AddField<float>("yfreez", "y freezout coordinate fm/c");
    sim_particles_branch.AddField<float>("zfreez", "z freezout coordinate fm/c");
    sim_particles_branch.AddField<float>("tfreez", "t freezout coordinate fm/c");
  }
  auto* man = AnalysisTree::TaskManager::GetInstance();
  man->AddBranch(out_branch_, sim_tracks_, sim_particles_branch);
}

void CbmSimTracksConverter::Exec()
{
  assert(cbm_mc_tracks_);
  out_indexes_map_.clear();

  if (fUseUnigen) {
    fTree->GetEntry(fEntry++);
    const Double_t unigen_phi = fUnigenEvent->GetPhi();
    const Double_t mc_phi     = cbm_header_->GetRotZ();
    fDeltaPhi                 = mc_phi - unigen_phi;
  }
  sim_tracks_->ClearChannels();
  auto* out_config_  = AnalysisTree::TaskManager::GetInstance()->GetConfig();
  const auto& branch = out_config_->GetBranchConfig(out_branch_);

  const int nMcTracks  = cbm_mc_tracks_->GetEntriesFast();
  const int imother_id = branch.GetFieldId("mother_id");
  const int igeant_id  = branch.GetFieldId("geant_process_id");
  const int in_hits    = branch.GetFieldId("n_hits_mvd");
  const int icbm_id    = branch.GetFieldId("cbmroot_id");

  sim_tracks_->Reserve(nMcTracks);
  int freezX = 0, freezY = 0, freezZ = 0, freezT = 0;
  if (fUseUnigen) {
    freezX = out_config_->GetBranchConfig(sim_tracks_->GetId()).GetFieldId("xfreez");
    freezY = out_config_->GetBranchConfig(sim_tracks_->GetId()).GetFieldId("yfreez");
    freezZ = out_config_->GetBranchConfig(sim_tracks_->GetId()).GetFieldId("zfreez");
    freezT = out_config_->GetBranchConfig(sim_tracks_->GetId()).GetFieldId("tfreez");
  }

  const Double_t nsTofmc = 1. / (0.3356 * 1E-15);

  for (int iMcTrack = 0; iMcTrack < nMcTracks; ++iMcTrack) {
    const auto* mctrack = (CbmMCTrack*) cbm_mc_tracks_->At(iMcTrack);
    if (mctrack->GetPdgCode() == 50000050) {  //Cherenkov
      continue;
    }
    auto& track = sim_tracks_->AddChannel(branch);

    out_indexes_map_.insert(std::make_pair(iMcTrack, track.GetId()));

    track.SetMomentum(mctrack->GetPx(), mctrack->GetPy(), mctrack->GetPz());
    track.SetMass(float(mctrack->GetMass()));
    track.SetPid(int(mctrack->GetPdgCode()));
    track.SetField(int(mctrack->GetGeantProcessId()), igeant_id);
    track.SetField(int(mctrack->GetNPoints(ECbmModuleId::kMvd)), in_hits);
    track.SetField(int(mctrack->GetNPoints(ECbmModuleId::kSts)), in_hits + 1);
    track.SetField(int(mctrack->GetNPoints(ECbmModuleId::kTrd)), in_hits + 2);
    track.SetField(int(mctrack->GetUniqueID()), icbm_id);

    // mother id should < than track id, so we can do it here
    if (mctrack->GetMotherId() == -1) { track.SetField(int(-1), imother_id); }
    else {
      auto p = out_indexes_map_.find(mctrack->GetMotherId());
      if (p == out_indexes_map_.end())  // match is not found
        track.SetField(int(-999), imother_id);
      else {
        track.SetField(int(p->second), imother_id);
      }
    }
    if (fUseUnigen) {
      if (mctrack->GetMotherId() == -1) {          // is primary
        if (iMcTrack >= fUnigenEvent->GetNpa()) {  // skip embedded (?) tracks
          track.SetField(float(gRandom->Gaus(0, 200)), freezX);
          track.SetField(float(gRandom->Gaus(0, 200)), freezY);
          track.SetField(float(gRandom->Gaus(0, 200)), freezZ);
          track.SetField(float(gRandom->Gaus(0, 200)), freezT);
        }
        else {
          UParticle* p            = fUnigenEvent->GetParticle(iMcTrack);
          TLorentzVector boostedX = p->GetPosition();
          boostedX.Boost(0, 0, -fBetaCM);
          boostedX.RotateZ(fDeltaPhi);
          track.SetField(float(boostedX.X()), freezX);
          track.SetField(float(boostedX.Y()), freezY);
          track.SetField(float(boostedX.Z()), freezZ);
          track.SetField(float(boostedX.T()), freezT);
        }
      }
      else {
        Double_t x = mctrack->GetStartX() - cbm_header_->GetX();
        Double_t y = mctrack->GetStartY() - cbm_header_->GetY();
        Double_t z = mctrack->GetStartZ() - cbm_header_->GetZ();
        Double_t t = mctrack->GetStartT() - cbm_header_->GetT();

        track.SetField(float(x * 1.E+13), freezX);
        track.SetField(float(y * 1.E+13), freezY);
        track.SetField(float(z * 1.E+13), freezZ);
        track.SetField(float(t * nsTofmc), freezT);
      }
    }
  }
}
CbmSimTracksConverter::~CbmSimTracksConverter()
{
  delete sim_tracks_;
  if (fFile) {
    // fFile->Close();
    // delete fFile;
  }
};
