#include <cassert>
#include <vector>

#include "TClonesArray.h"

#include <FairMCPoint.h>
#include <FairRootManager.h>

#include "AnalysisTree/Matching.hpp"

#include <AnalysisTree/TaskManager.hpp>
#include <CbmGlobalTrack.h>
#include <rich/CbmRichRing.h>

#include "CbmRichRingsConverter.h"

void CbmRichRingsConverter::Init() {

  assert(!out_branch_.empty());
  auto* ioman = FairRootManager::Instance();

  cbm_rich_rings_    = (TClonesArray*) ioman->GetObject("RichRing");
  cbm_global_tracks_ = (TClonesArray*) ioman->GetObject("GlobalTrack");

  AnalysisTree::BranchConfig rich_branch(out_branch_,
                                         AnalysisTree::DetType::kHit);
  rich_branch.AddField<float>("radius");
  rich_branch.AddFields<int>({"n_hits", "n_hits_on_ring"});
  rich_branch.AddFields<float>({"axis_a", "axis_b"});
  rich_branch.AddFields<float>({"center_x", "center_y"});
  rich_branch.AddField<float>("chi2_ov_ndf", "chi2/ndf ring fit");
  rich_branch.AddField<float>("phi_ellipse", "phi rotation angle of ellipse");
  rich_branch.AddField<float>("radial_pos", "sqrt(x**2+abs(y-110)**2)");
  rich_branch.AddField<float>("radial_angle",
                              "(0||1||2)*pi +- atan( abs((+-100-y)/-x) )");

  auto* man = AnalysisTree::TaskManager::GetInstance();
  man->AddBranch(out_branch_, rich_rings_, rich_branch);
  man->AddMatching(match_to_, out_branch_, vtx_tracks_2_rich_);
}

void CbmRichRingsConverter::FillRichRings() {

  assert(cbm_rich_rings_);
  rich_rings_->ClearChannels();
  vtx_tracks_2_rich_->Clear();

  auto* out_config_  = AnalysisTree::TaskManager::GetInstance()->GetConfig();
  const auto& branch = out_config_->GetBranchConfig(out_branch_);

  const int i_r            = branch.GetFieldId("radius");
  const int i_n_hits       = branch.GetFieldId("n_hits");
  const int i_axis         = branch.GetFieldId("axis_a");
  const int i_center       = branch.GetFieldId("center_x");
  const int i_chi2         = branch.GetFieldId("chi2_ov_ndf");
  const int i_radial_angle = branch.GetFieldId("radial_angle");
  const int i_radial_pos   = branch.GetFieldId("radial_pos");
  const int i_phi_ellipse  = branch.GetFieldId("phi_ellipse");

  const auto it = indexes_map_->find(match_to_);
  if (it == indexes_map_->end()) {
    throw std::runtime_error(match_to_
                             + " is not found to match with TOF hits");
  }
  auto rec_tracks_map = it->second;

  rich_rings_->Reserve(cbm_global_tracks_->GetEntries());

  for (Int_t igt = 0; igt < cbm_global_tracks_->GetEntries(); igt++) {
    const auto* global_track =
      static_cast<const CbmGlobalTrack*>(cbm_global_tracks_->At(igt));

    Int_t i_rich = global_track->GetRichRingIndex();
    if (i_rich < 0) continue;
    auto rich_ring = static_cast<CbmRichRing*>(cbm_rich_rings_->At(i_rich));
    //    auto rich_match = static_cast<CbmTrackMatchNew*>( cbm_rich_rings_->At(i_rich));
    //    Int_t itrdMC = (rich_match ? rich_match->GetMatchedLink().GetIndex() : -1);
    //    richProj = static_cast<FairTrackParam*>(fRichProjection->At(i));

    auto& ring = rich_rings_->AddChannel(branch);

    ring.SetPosition(rich_ring->GetCenterX(), rich_ring->GetCenterY(), 0.f);

    ring.SetField(int(rich_ring->GetNofHits()), i_n_hits);
    ring.SetField(int(rich_ring->GetNofHitsOnRing()), i_n_hits + 1);
    ring.SetField(float(rich_ring->GetAaxis()), i_axis);
    ring.SetField(float(rich_ring->GetBaxis()), i_axis + 1);
    ring.SetField(float(rich_ring->GetCenterX()), i_center);
    ring.SetField(float(rich_ring->GetCenterY()), i_center + 1);
    ring.SetField(float(rich_ring->GetRadius()), i_r);
    ring.SetField(float(rich_ring->GetNDF() > 0.
                          ? rich_ring->GetChi2() / rich_ring->GetNDF()
                          : -999.),
                  i_chi2);
    ring.SetField(float(rich_ring->GetRadialAngle()), i_radial_angle);
    ring.SetField(float(rich_ring->GetRadialPosition()), i_radial_pos);
    ring.SetField(float(rich_ring->GetPhi()), i_phi_ellipse);

    if (rec_tracks_map.empty()) { continue; }
    const Int_t stsTrackIndex = global_track->GetStsTrackIndex();
    if (rec_tracks_map.find(stsTrackIndex) != rec_tracks_map.end()) {
      vtx_tracks_2_rich_->AddMatch(rec_tracks_map.find(stsTrackIndex)->second,
                                   ring.GetId());
    }
  }
}

void CbmRichRingsConverter::Exec() { FillRichRings(); }

CbmRichRingsConverter::~CbmRichRingsConverter() {
  delete rich_rings_;
  delete vtx_tracks_2_rich_;
};
