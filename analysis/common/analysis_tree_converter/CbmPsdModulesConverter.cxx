#include <cassert>
#include <vector>

#include "TClonesArray.h"

#include "FairRootManager.h"

#include "AnalysisTree/DataHeader.hpp"
#include "AnalysisTree/Detector.hpp"

#include "CbmPsdHit.h"
#include "CbmPsdModulesConverter.h"

ClassImp(CbmPsdModulesConverter)

  void CbmPsdModulesConverter::Init(std::map<std::string, void*>&) {
  assert(!in_branches_.empty() && !out_branch_.empty() && out_config_
         && out_tree_);
  auto* ioman = FairRootManager::Instance();
  assert(ioman != nullptr);
  cbm_psd_hits_ = (TClonesArray*) ioman->GetObject("PsdHit");

  AnalysisTree::BranchConfig psd_branch(out_branch_,
                                        AnalysisTree::DetType::kModule);
  out_config_->AddBranchConfig(std::move(psd_branch));
  psd_modules_ = new AnalysisTree::ModuleDetector(out_config_->GetLastId());
  out_tree_->Branch(
    out_branch_.c_str(), "AnalysisTree::ModuleDetector", &psd_modules_);
}

void CbmPsdModulesConverter::Exec() {
  assert(cbm_psd_hits_);
  psd_modules_->ClearChannels();

  CbmPsdHit* hit {nullptr};
  Float_t psd_energy {0.};

  //   //TODO fix this logic. At the moment I don't know how to access number of
  //   PSD modules from her hit = (CbmPsdHit*) cbm_psd_hits_->At(nPsdHits-1); //
  //   last hit const int last_module_id = hit->GetModuleID()-1;
  const int n_psd_modules =
    data_header_->GetModulePositions(0).GetNumberOfChannels();
  psd_modules_->Reserve(n_psd_modules);
  for (int i = 0; i < n_psd_modules; ++i) {
    auto* module = psd_modules_->AddChannel();
    module->SetSignal(0.f);
  }

  const int nPsdHits = cbm_psd_hits_->GetEntriesFast();
  for (int i = 0; i < nPsdHits; ++i) {
    hit = (CbmPsdHit*) cbm_psd_hits_->At(i);
    if (hit == nullptr) continue;
    auto& module = psd_modules_->GetChannel(hit->GetModuleID() - 1);
    module.SetNumber(i + 1);
    module.SetSignal(hit->GetEdep());
    psd_energy += hit->GetEdep();
  }
  //  rec_event_header_ -> SetField(psd_energy,
  //  config_.GetBranchConfig(rec_event_header_->GetId() ).GetFieldId("Epsd"));
}

void CbmPsdModulesConverter::Finish() {
  delete psd_modules_;
  delete cbm_psd_hits_;
}
CbmPsdModulesConverter::~CbmPsdModulesConverter() { delete psd_modules_; };
