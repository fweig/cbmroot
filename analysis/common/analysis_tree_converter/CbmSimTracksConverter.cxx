#include <cassert>
#include <vector>

#include <AnalysisTree/TaskManager.hpp>

#include "TClonesArray.h"

#include "FairRootManager.h"

#include "CbmMCTrack.h"
#include "CbmSimTracksConverter.h"

ClassImp(CbmSimTracksConverter)

  void CbmSimTracksConverter::Init() {

  assert(!out_branch_.empty());
  auto* ioman    = FairRootManager::Instance();
  cbm_mc_tracks_ = (TClonesArray*) ioman->GetObject("MCTrack");

  AnalysisTree::BranchConfig sim_particles_branch(
    out_branch_, AnalysisTree::DetType::kParticle);
  sim_particles_branch.AddField<int>("mother_id",
                                     "id of mother particle, -1 for primaries");
  sim_particles_branch.AddField<int>("cbmroot_id",
                                     "track id in CbmRoot transport file");
  sim_particles_branch.AddField<int>("geant_process_id", "");
  sim_particles_branch.AddFields<int>(
    {"n_hits_mvd", "n_hits_sts", "n_hits_trd"},
    "Number of hits in the detector");

  auto* man = AnalysisTree::TaskManager::GetInstance();
  man->AddBranch(out_branch_, sim_tracks_, sim_particles_branch);
}

void CbmSimTracksConverter::Exec() {
  assert(cbm_mc_tracks_);
  out_indexes_map_.clear();

  sim_tracks_->ClearChannels();
  auto* out_config_  = AnalysisTree::TaskManager::GetInstance()->GetConfig();
  const auto& branch = out_config_->GetBranchConfig(out_branch_);

  const int nMcTracks  = cbm_mc_tracks_->GetEntries();
  const int imother_id = branch.GetFieldId("mother_id");
  const int igeant_id  = branch.GetFieldId("geant_process_id");
  const int in_hits    = branch.GetFieldId("n_hits_mvd");
  const int icbm_id    = branch.GetFieldId("cbmroot_id");

  sim_tracks_->Reserve(nMcTracks);

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
    if (mctrack->GetMotherId() == -1) {
      track.SetField(int(-1), imother_id);
    } else {
      auto p = out_indexes_map_.find(mctrack->GetMotherId());
      if (p == out_indexes_map_.end())  // match is not found
        track.SetField(int(-999), imother_id);
      else {
        track.SetField(int(p->second), imother_id);
      }
    }
  }
}
CbmSimTracksConverter::~CbmSimTracksConverter() { delete sim_tracks_; };
