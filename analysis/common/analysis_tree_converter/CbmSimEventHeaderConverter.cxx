#include "FairMCEventHeader.h"
#include "FairRootManager.h"
#include "TClonesArray.h"
#include "cassert"

#include "CbmSimEventHeaderConverter.h"

ClassImp(CbmSimEventHeaderConverter)

  void CbmSimEventHeaderConverter::Init(std::map<std::string, void*>&) {
  assert(!in_branches_.empty() && !out_branch_.empty() && out_config_
         && out_tree_);
  auto* ioman = FairRootManager::Instance();
  assert(ioman != nullptr);
  cbm_header_ = (FairMCEventHeader*) ioman->GetObject(in_branches_[0].c_str());

  //  ***** SimEventHeader *******
  AnalysisTree::BranchConfig SimEventHeaderBranch(
    "SimEventHeader", AnalysisTree::DetType::kEventHeader);
  SimEventHeaderBranch.AddField<float>("psi_RP");
  SimEventHeaderBranch.AddField<float>("b");
  out_config_->AddBranchConfig(SimEventHeaderBranch);
  sim_event_header_ = new AnalysisTree::EventHeader(out_config_->GetLastId());
  sim_event_header_->Init(SimEventHeaderBranch);

  out_tree_->Branch(
    out_branch_.c_str(), "AnalysisTree::EventHeader", &sim_event_header_);
}

void CbmSimEventHeaderConverter::Exec() {
  if (!cbm_header_) {
    throw std::runtime_error(
      "CbmSimEventHeaderConverter::Exec - ERROR! No fHeader!");
  }

  TVector3 pos = {
    cbm_header_->GetX(), cbm_header_->GetY(), cbm_header_->GetZ()};
  sim_event_header_->SetVertexPosition3(pos);
  sim_event_header_->SetField(
    float(cbm_header_->GetRotZ()),
    out_config_->GetBranchConfig(sim_event_header_->GetId())
      .GetFieldId("psi_RP"));
  sim_event_header_->SetField(
    float(cbm_header_->GetB()),
    out_config_->GetBranchConfig(sim_event_header_->GetId()).GetFieldId("b"));
}
