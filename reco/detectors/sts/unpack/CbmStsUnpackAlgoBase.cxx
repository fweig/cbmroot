/* Copyright (C) 2021 Fair GmbH, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau, Dominik Smith [committer] */

#include "CbmStsUnpackAlgoBase.h"

CbmStsUnpackAlgoBase::CbmStsUnpackAlgoBase(std::string name) : CbmRecoUnpackAlgo(name) {}

CbmStsUnpackAlgoBase::~CbmStsUnpackAlgoBase() {}

// ---- GetParContainerRequest ----
std::vector<std::pair<std::string, std::shared_ptr<FairParGenericSet>>>*
  CbmStsUnpackAlgoBase::GetParContainerRequest(std::string /*geoTag*/, std::uint32_t /*runId*/)
{
  // Basepath for default Sts parameter sets (those connected to a geoTag)
  std::string basepath = Form("%s", fParFilesBasePath.data());
  std::string temppath = "";

  // // Get parameter container
  temppath = basepath + "mStsPar.par";
  fParContVec.emplace_back(std::make_pair(temppath, std::make_shared<CbmMcbm2018StsPar>()));

  return &fParContVec;
}

ClassImp(CbmStsUnpackAlgoBase)
