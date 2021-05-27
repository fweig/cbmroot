/** @file CbmPsdModulesConverter.cxx
  * @copyright Copyright (C) 2020-2021 Physikalisches Institut, Eberhard Karls Universität Tübingen, Tübingen
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Viktor Klochkov [orginator], Viktor Klochkov [orginator] **/

#include "CbmPsdModulesConverter.h"

#include "CbmPsdHit.h"

#include "FairRootManager.h"

#include "TClonesArray.h"

#include <AnalysisTree/TaskManager.hpp>
#include <cassert>
#include <vector>

#include "AnalysisTree/Detector.hpp"

ClassImp(CbmPsdModulesConverter)

  void CbmPsdModulesConverter::Init()
{
  assert(!out_branch_.empty());
  auto* ioman = FairRootManager::Instance();
  assert(ioman != nullptr);
  cbm_psd_hits_ = (TClonesArray*) ioman->GetObject("PsdHit");

  AnalysisTree::BranchConfig psd_branch(out_branch_, AnalysisTree::DetType::kModule);

  auto* man = AnalysisTree::TaskManager::GetInstance();
  man->AddBranch(out_branch_, psd_modules_, psd_branch);
}


void CbmPsdModulesConverter::Exec()
{
  assert(cbm_psd_hits_);
  psd_modules_->ClearChannels();

  CbmPsdHit* hit {nullptr};
  Float_t psd_energy {0.};

  auto* data_header  = AnalysisTree::TaskManager::GetInstance()->GetDataHeader();
  auto* config       = AnalysisTree::TaskManager::GetInstance()->GetConfig();
  const auto& branch = config->GetBranchConfig(out_branch_);

  const int n_psd_modules = data_header->GetModulePositions(0).GetNumberOfChannels();
  psd_modules_->Reserve(n_psd_modules);
  for (int i = 0; i < n_psd_modules; ++i) {
    auto& module = psd_modules_->AddChannel(branch);
    module.SetSignal(0.f);
  }

  const int nPsdHits = cbm_psd_hits_->GetEntriesFast();
  for (int i = 0; i < nPsdHits; ++i) {
    hit = (CbmPsdHit*) cbm_psd_hits_->At(i);
    if (hit == nullptr) continue;
    auto& module = psd_modules_->Channel(hit->GetModuleID() - 1);
    module.SetNumber(i + 1);
    module.SetSignal(hit->GetEdep());
    psd_energy += hit->GetEdep();
  }
}

void CbmPsdModulesConverter::Finish() {}

CbmPsdModulesConverter::~CbmPsdModulesConverter() { delete psd_modules_; };
