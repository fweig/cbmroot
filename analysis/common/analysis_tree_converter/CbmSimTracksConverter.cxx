#include <cassert>
#include <vector>

#include "TClonesArray.h"

#include "FairRootManager.h"

#include "CbmMCTrack.h"
#include "CbmSimTracksConverter.h"

ClassImp(CbmSimTracksConverter)

  void CbmSimTracksConverter::Init(std::map<std::string, void*>&) {

  assert(!in_branches_.empty() && !out_branch_.empty() && out_config_
         && out_tree_);
  auto* ioman    = FairRootManager::Instance();
  cbm_mc_tracks_ = (TClonesArray*) ioman->GetObject(in_branches_[0].c_str());

  AnalysisTree::BranchConfig SimTracksBranch(out_branch_,
                                             AnalysisTree::DetType::kParticle);
  SimTracksBranch.AddField<int>("mother_id");
  out_config_->AddBranchConfig(std::move(SimTracksBranch));
  sim_tracks_ = new AnalysisTree::Particles(out_config_->GetLastId());
  out_tree_->Branch(
    out_branch_.c_str(), "AnalysisTree::Particles", &sim_tracks_);
}

void CbmSimTracksConverter::Exec() {
  assert(cbm_mc_tracks_);
  out_indexes_map_.clear();

  std::cout << "ReadMcTracks" << std::endl;
  sim_tracks_->ClearChannels();

  const int nMcTracks = cbm_mc_tracks_->GetEntries();
  const int imother_id =
    out_config_->GetBranchConfig(sim_tracks_->GetId()).GetFieldId("mother_id");
  sim_tracks_->Reserve(nMcTracks);

  for (int iMcTrack = 0; iMcTrack < nMcTracks; ++iMcTrack) {
    const auto* mctrack = (CbmMCTrack*) cbm_mc_tracks_->At(iMcTrack);
    if (mctrack->GetStartZ() > 200.)
      continue;  // to exclude shower in PSD etc //TODO check if better cut can
                 // be applied.

    auto* Track = sim_tracks_->AddChannel();
    Track->Init(out_config_->GetBranchConfig(sim_tracks_->GetId()));

    out_indexes_map_.insert(std::make_pair(iMcTrack, Track->GetId()));

    Track->SetMomentum(mctrack->GetPx(), mctrack->GetPy(), mctrack->GetPz());
    Track->SetMass(float(mctrack->GetMass()));
    Track->SetPid(int(mctrack->GetPdgCode()));

    if (mctrack->GetMotherId()
        == -1) {  // mother id should < than track id, so we can do it here
      Track->SetField(int(-1), imother_id);
    } else {
      auto p = out_indexes_map_.find(mctrack->GetMotherId());
      if (p == out_indexes_map_.end())  // match is not found
        Track->SetField(int(-999), imother_id);
      else
        Track->SetField(int(p->second), imother_id);
    }
  }
}
CbmSimTracksConverter::~CbmSimTracksConverter() { delete sim_tracks_; };
