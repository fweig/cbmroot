/* Copyright (C) 2010 - 2021 Fair GmbH 
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith */

#include "CbmStsUnpackAlgoBase.h"

CbmStsUnpackAlgoBase::CbmStsUnpackAlgoBase(std::string name) : CbmRecoUnpackAlgo(name) {}

CbmStsUnpackAlgoBase::~CbmStsUnpackAlgoBase() {}

// ---- GetParContainerRequest ----
std::vector<std::pair<std::string, std::shared_ptr<FairParGenericSet>>>*
  CbmStsUnpackAlgoBase::GetParContainerRequest(std::string /*geoTag*/, std::uint32_t /*runId*/)
{
  // Basepath for default Trd parameter sets (those connected to a geoTag)
  std::string basepath = Form("%s", fParFilesBasePath.data());
  std::string temppath = "";

  // // Get parameter container
  temppath = basepath + "mStsPar.par";
  fParContVec.emplace_back(std::make_pair(temppath, std::make_shared<CbmMcbm2018StsPar>()));

  return &fParContVec;
}

// -------------------------------------------------------------------------
void CbmStsUnpackAlgoBase::SetTimeOffsetNsAsic(const uint32_t uAsicIdx, const double dOffsetIn)
{
  if (uAsicIdx >= fvdTimeOffsetNsAsics.size()) { fvdTimeOffsetNsAsics.resize(uAsicIdx + 1, 0.0); }
  fvdTimeOffsetNsAsics[uAsicIdx] = dOffsetIn;
}

ClassImp(CbmStsUnpackAlgoBase)
