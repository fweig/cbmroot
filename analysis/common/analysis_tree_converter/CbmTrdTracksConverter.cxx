#include "CbmTrdTracksConverter.h"

#include <CbmGlobalTrack.h>
#include <CbmTrackMatchNew.h>
#include <CbmTrdHit.h>
#include <CbmTrdTrack.h>

#include <FairMCPoint.h>
#include <FairRootManager.h>

#include "TClonesArray.h"

#include <AnalysisTree/TaskManager.hpp>
#include <cassert>
#include <vector>

#include "AnalysisTree/Matching.hpp"

ClassImp(CbmTrdTracksConverter)

  void CbmTrdTracksConverter::Init()
{

  assert(!out_branch_.empty());
  auto* ioman = FairRootManager::Instance();

  cbm_trd_tracks_    = (TClonesArray*) ioman->GetObject("TrdTrack");
  cbm_global_tracks_ = (TClonesArray*) ioman->GetObject("GlobalTrack");
  cbm_trd_hits_      = (TClonesArray*) ioman->GetObject("TrdHit");

  AnalysisTree::BranchConfig trd_branch(out_branch_, AnalysisTree::DetType::kTrack);
  trd_branch.AddFields<float>({"energy_loss_0", "energy_loss_1", "energy_loss_2", "energy_loss_3"},
                              "keV(?), Energy loss per TRD station");
  trd_branch.AddFields<float>({"pid_like_e", "pid_like_pi", "pid_like_k", "pid_like_p"},
                              "Probability to be a given particle specie");
  trd_branch.AddField<float>("chi2_ov_ndf", "chi2 divided by NDF of the track fit");
  trd_branch.AddFields<float>({"pT_out", "p_out"}, "Momentum at last point (?)");
  trd_branch.AddField<int>("n_hits", "Number of hits");

  auto* man = AnalysisTree::TaskManager::GetInstance();
  man->AddBranch(out_branch_, trd_tracks_, trd_branch);
  man->AddMatching(match_to_, out_branch_, vtx_tracks_2_trd_);
}

void CbmTrdTracksConverter::FillTrdTracks()
{

  assert(cbm_trd_tracks_);
  trd_tracks_->ClearChannels();
  vtx_tracks_2_trd_->Clear();

  auto* out_config_  = AnalysisTree::TaskManager::GetInstance()->GetConfig();
  const auto& branch = out_config_->GetBranchConfig(out_branch_);

  const int i_e_loss_i = branch.GetFieldId("energy_loss_0");
  //  const int i_ann = branch.GetFieldId("pid_ann");
  const int i_pid_like    = branch.GetFieldId("pid_like_e");
  const int i_chi2_ov_ndf = branch.GetFieldId("chi2_ov_ndf");
  const int i_pT_out      = branch.GetFieldId("pT_out");
  const int i_n_hits      = branch.GetFieldId("n_hits");

  const auto it = indexes_map_->find(match_to_);
  if (it == indexes_map_->end()) { throw std::runtime_error(match_to_ + " is not found to match with TOF hits"); }
  auto rec_tracks_map = it->second;

  trd_tracks_->Reserve(cbm_global_tracks_->GetEntries());

  for (Int_t igt = 0; igt < cbm_global_tracks_->GetEntries(); igt++) {
    const auto* global_track = static_cast<const CbmGlobalTrack*>(cbm_global_tracks_->At(igt));

    Int_t itrd = global_track->GetTrdTrackIndex();
    if (itrd < 0) continue;

    auto trd_track = static_cast<CbmTrdTrack*>(cbm_trd_tracks_->At(itrd));
    //    auto trd_match = static_cast<CbmTrackMatchNew*>(cbm_trd_tracks_->At(itrd));
    //    Int_t itrdMC   = (trd_match ? trd_match->GetMatchedLink().GetIndex() : -1);

    auto& track = trd_tracks_->AddChannel(branch);
    TVector3 mom, mom_last;
    trd_track->GetParamFirst()->Momentum(mom);
    trd_track->GetParamLast()->Momentum(mom_last);

    track.SetMomentum3(mom);
    track.SetField(int(trd_track->GetNofHits()), i_n_hits);
    //    track.SetField(float(trd_track->GetPidANN()), i_ann);
    //    track.SetField(float(trd_track->GetPidWkn()), i_ann+1);

    track.SetField(float(trd_track->GetPidLikeEL()), i_pid_like);
    track.SetField(float(trd_track->GetPidLikePI()), i_pid_like + 1);
    track.SetField(float(trd_track->GetPidLikeKA()), i_pid_like + 2);
    track.SetField(float(trd_track->GetPidLikePR()), i_pid_like + 3);
    //    track.SetField(float(trd_track->GetPidLikeMU()), i_pid_like + 4);

    track.SetField(float(trd_track->GetNDF() > 0. ? trd_track->GetChiSq() / trd_track->GetNDF() : -999.),
                   i_chi2_ov_ndf);

    track.SetField(float(mom_last.Pt()), i_pT_out);
    track.SetField(float(mom_last.Mag()), i_pT_out + 1);

    for (int i = 0; i < 4; ++i) {
      track.SetField(0.f, i_e_loss_i + i);
    }

    for (Int_t ihit = 0; ihit < trd_track->GetNofHits(); ihit++) {
      Int_t idx = trd_track->GetHitIndex(ihit);
      auto* hit = (CbmTrdHit*) cbm_trd_hits_->At(idx);
      if (hit) {
        //        std::cout << hit->GetELoss()*1e6 << "  " << hit->GetPlaneId() << std::endl;
        track.SetField(float(hit->GetELoss() * 1e6), i_e_loss_i + hit->GetPlaneId());
      }
    }

    if (rec_tracks_map.empty()) { continue; }
    const Int_t stsTrackIndex = global_track->GetStsTrackIndex();
    if (rec_tracks_map.find(stsTrackIndex) != rec_tracks_map.end()) {
      vtx_tracks_2_trd_->AddMatch(rec_tracks_map.find(stsTrackIndex)->second, track.GetId());
    }
  }
}

void CbmTrdTracksConverter::Exec() { FillTrdTracks(); }

CbmTrdTracksConverter::~CbmTrdTracksConverter()
{
  delete trd_tracks_;
  delete vtx_tracks_2_trd_;
};
