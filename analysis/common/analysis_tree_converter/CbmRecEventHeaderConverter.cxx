#include "CbmVertex.h"
#include "FairMCEventHeader.h"
#include "FairRootManager.h"
#include "TClonesArray.h"
#include "cassert"
#include "iostream"
#include <AnalysisTree/TaskManager.hpp>

#include "CbmRecEventHeaderConverter.h"

ClassImp(CbmRecEventHeaderConverter)

  void CbmRecEventHeaderConverter::Init() {
  assert(!out_branch_.empty());
  auto* ioman = FairRootManager::Instance();
  assert(ioman != nullptr);
  cbm_header_      = (FairMCEventHeader*) ioman->GetObject("MCEventHeader.");
  cbm_prim_vertex_ = (CbmVertex*) ioman->GetObject("PrimaryVertex.");

  //  ***** RecEventHeader *******
  AnalysisTree::BranchConfig RecEventHeaderBranch(
    "RecEventHeader", AnalysisTree::DetType::kEventHeader);
  RecEventHeaderBranch.AddField<float>("vtx_chi2", "primiry vertex fit chi2");
  RecEventHeaderBranch.AddField<float>("Epsd",
                                       "GeV, full energy deposit in PSD");
  RecEventHeaderBranch.AddField<int>("M", "total multiplicity in STS");
  RecEventHeaderBranch.AddField<int>("evt_id", "event identifier");

  auto* man = AnalysisTree::TaskManager::GetInstance();
  man->AddBranch(out_branch_, rec_event_header_, RecEventHeaderBranch);
  rec_event_header_->Init(RecEventHeaderBranch);
}

void CbmRecEventHeaderConverter::Exec() {

  auto* out_config_  = AnalysisTree::TaskManager::GetInstance()->GetConfig();
  const auto& branch = out_config_->GetBranchConfig(out_branch_);

  if (!cbm_prim_vertex_) {
    std::cout << "WARNING! No fPrimVtx!" << std::endl;

    rec_event_header_->SetVertexPosition3({-999., -999., -999.});
    rec_event_header_->SetField(float(-999.), branch.GetFieldId("vtx_chi2"));
    return;
  }

  //  std::cout << cbm_prim_vertex_->GetX() << " " << cbm_prim_vertex_->GetY() << " " << cbm_prim_vertex_->GetZ() << std::endl;

  rec_event_header_->SetVertexPosition3({cbm_prim_vertex_->GetX(),
                                         cbm_prim_vertex_->GetY(),
                                         cbm_prim_vertex_->GetZ()});
  //  rec_event_header_ -> SetField(int(cbm_sts_tracks_->GetEntries()),
  //  config_.GetBranchConfig(rec_event_header_->GetId() ).GetFieldId("M"));
  rec_event_header_->SetField(int(cbm_header_->GetEventID()),
                              branch.GetFieldId("evt_id"));
  rec_event_header_->SetField(
    float(cbm_prim_vertex_->GetChi2() / cbm_prim_vertex_->GetNDF()),
    branch.GetFieldId("vtx_chi2"));
}
