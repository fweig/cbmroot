#include <cassert>
#include <vector>

#include "TClonesArray.h"

#include <FairMCPoint.h>
#include <FairRootManager.h>

#include "AnalysisTree/Matching.hpp"

#include <CbmGlobalTrack.h>
#include <CbmMCTrack.h>
#include <CbmTofHit.h>
#include <CbmTrackMatchNew.h>

#include "CbmTofHitsConverter.h"

ClassImp(CbmTofHitsConverter)

  void CbmTofHitsConverter::Init(std::map<std::string, void*>&) {

  assert(!in_branches_.empty() && !out_branch_.empty() && out_config_
         && out_tree_);
  auto* ioman = FairRootManager::Instance();

  cbm_tof_hits_      = (TClonesArray*) ioman->GetObject("TofHit");
  cbm_global_tracks_ = (TClonesArray*) ioman->GetObject("GlobalTrack");
  //  cbm_tof_match_ = (TClonesArray*) ioman->GetObject("TofHitMatch");
  //  cbm_tof_points_ = (TClonesArray*) ioman->GetObject("TofPoint");
  //  cbm_mc_tracks_ = (TClonesArray*) ioman->GetObject("MCTrack");
  //  cbm_sts_match_ = (TClonesArray*) ioman->GetObject("StsTrackMatch");

  AnalysisTree::BranchConfig tof_branch("TofHits", AnalysisTree::DetType::kHit);
  tof_branch.AddField<float>("mass2");
  tof_branch.AddField<float>("l");
  tof_branch.AddField<float>("t");
  tof_branch.AddField<float>("qp_tof");
  tof_branch.AddFields<float>({"dx", "dy", "dz"});
  tof_branch.AddField<int>("mc_pdg");
  tof_branch.AddField<bool>("is_correct_match");
  out_config_->AddBranchConfig(std::move(tof_branch));
  tof_hits_ = new AnalysisTree::HitDetector(out_config_->GetLastId());

  vtx_tracks_2_tof_ = new AnalysisTree::Matching(
    out_config_->GetBranchConfig(match_to_).GetId(), out_config_->GetLastId());
  out_config_->AddMatch(vtx_tracks_2_tof_);

  out_tree_->Branch(
    out_branch_.c_str(), "AnalysisTree::HitDetector", &tof_hits_);
  out_tree_->Branch((match_to_ + "2" + out_branch_).c_str(),
                    "AnalysisTree::Matching",
                    &vtx_tracks_2_tof_);
}

void CbmTofHitsConverter::ExtrapolateStraightLine(FairTrackParam* params,
                                                  float z) {

  const Float_t Tx    = params->GetTx();
  const Float_t Ty    = params->GetTy();
  const Float_t old_z = params->GetZ();
  const Float_t dz    = z - old_z;

  const Float_t x = params->GetX() + Tx * dz;
  const Float_t y = params->GetY() + Ty * dz;

  params->SetPosition({x, y, z});
}

void CbmTofHitsConverter::FillTofHits() {
  assert(cbm_tof_hits_);
  tof_hits_->ClearChannels();
  vtx_tracks_2_tof_->Clear();

  const int i_mass2 =
    out_config_->GetBranchConfig(tof_hits_->GetId()).GetFieldId("mass2");
  const int i_qp =
    out_config_->GetBranchConfig(tof_hits_->GetId()).GetFieldId("qp_tof");
  const int i_dx =
    out_config_->GetBranchConfig(tof_hits_->GetId()).GetFieldId("dx");
  const int i_t =
    out_config_->GetBranchConfig(tof_hits_->GetId()).GetFieldId("t");
  const int i_l =
    out_config_->GetBranchConfig(tof_hits_->GetId()).GetFieldId("l");
  //  const int i_is_correct =
  //  out_config_->GetBranchConfig(tof_hits_->GetId()).GetFieldId("is_correct_match");
  //  const int i_pdg =
  //  out_config_->GetBranchConfig(tof_hits_->GetId()).GetFieldId("mc_pdg");

  const auto it = indexes_map_->find(match_to_);
  if (it == indexes_map_->end()) {
    throw std::runtime_error(match_to_
                             + " is not found to match with TOF hits");
  }
  auto rec_tracks_map = it->second;

  tof_hits_->Reserve(cbm_global_tracks_->GetEntries());

  for (Int_t igt = 0; igt < cbm_global_tracks_->GetEntries(); igt++) {
    const auto* globalTrack =
      static_cast<const CbmGlobalTrack*>(cbm_global_tracks_->At(igt));
    const Int_t tofHitIndex = globalTrack->GetTofHitIndex();
    if (tofHitIndex < 0) continue;

    const auto* tofHit =
      static_cast<const CbmTofHit*>(cbm_tof_hits_->At(tofHitIndex));

    FairTrackParam param_last = *(globalTrack->GetParamLast());
    TVector3 p_tof;
    param_last.Momentum(p_tof);

    const Float_t p = p_tof.Mag();
    const Int_t q   = param_last.GetQp() > 0 ? 1 : -1;
    const Float_t l =
      globalTrack->GetLength();  // l is calculated by global tracking
    const Float_t time = tofHit->GetTime();
    const Float_t beta = l / (time * 29.9792458);
    const Float_t m2   = p * p * (1. / (beta * beta) - 1.);

    const Float_t hitX = tofHit->GetX();
    const Float_t hitY = tofHit->GetY();
    const Float_t hitZ = tofHit->GetZ();

    ExtrapolateStraightLine(&param_last, tofHit->GetZ());

    auto* Hit = tof_hits_->AddChannel();
    Hit->Init(out_config_->GetBranchConfig(tof_hits_->GetId()));
    Hit->SetPosition(hitX, hitY, hitZ);
    Hit->SetSignal(time);
    Hit->SetField(m2, i_mass2);
    Hit->SetField(float(q * p_tof.Mag()), i_qp);
    Hit->SetField(float(param_last.GetX() - hitX), i_dx);
    Hit->SetField(float(param_last.GetY() - hitY), i_dx + 1);
    Hit->SetField(float(param_last.GetZ() - hitZ), i_dx + 2);
    Hit->SetField(l, i_l);
    Hit->SetField(time, i_t);

    if (rec_tracks_map.empty()) { return; }
    const Int_t stsTrackIndex = globalTrack->GetStsTrackIndex();
    if (rec_tracks_map.find(stsTrackIndex) != rec_tracks_map.end()) {
      vtx_tracks_2_tof_->AddMatch(rec_tracks_map.find(stsTrackIndex)->second,
                                  Hit->GetId());
    }

    //    const auto* tofMatch =
    //    dynamic_cast<CbmMatch*>(cbm_tof_match_->At(tofHitIndex)); if(tofMatch
    //    != nullptr && tofMatch->GetNofLinks()>0) {
    //
    //    const auto* tofPoint = dynamic_cast<FairMCPoint*>(
    //    cbm_tof_points_->At(tofMatch->GetMatchedLink().GetIndex()) );
    //
    //    Int_t itofMC = (tofPoint ? tofPoint->GetTrackID() : -1 );
    //    if(itofMC >= 0){
    //      const auto* mc_track = dynamic_cast<const
    //      CbmMCTrack*>(cbm_mc_tracks_->At(itofMC));
    //      Hit->SetField(mc_track->GetPdgCode(), i_pdg);
    //
    //      const Int_t stsTrackIndex = globalTrack->GetStsTrackIndex();
    //      if(stsTrackIndex<0) return;
    //
    //      auto* match = (CbmTrackMatchNew*) cbm_sts_match_->At(stsTrackIndex);
    //      if(match == nullptr || match->GetNofLinks() == 0) continue;
    //      const int mc_id_sts = match->GetMatchedLink().GetIndex();
    //      Hit->SetField( bool(mc_id_sts == itofMC), i_is_correct);
    //    }
    //  }
  }
}

void CbmTofHitsConverter::Exec() { FillTofHits(); }

CbmTofHitsConverter::~CbmTofHitsConverter() {
  delete tof_hits_;
  delete vtx_tracks_2_tof_;
};
