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

void CbmStsUnpackAlgoBase::SetWalkMap(const std::map<uint32_t, CbmStsParModule>& mapIn)
{
  for (auto& mapInEntry : mapIn) {
    const std::vector<CbmStsParAsic>& asicPars = (mapInEntry.second).GetAsicParams();
    uint32_t uNbAsics                          = asicPars.size();
    fWalkLookup[mapInEntry.first].resize(uNbAsics);
    LOG(debug) << Form("[STS-UNPACKER] setting TW parameters: %x\n", mapInEntry.first);
    for (uint32_t asicIdx = 0; asicIdx < uNbAsics; ++asicIdx) {
      fWalkLookup[mapInEntry.first][asicIdx].resize(32);
      for (uint32_t uAdc = 0; uAdc < 32; ++uAdc) {
        fWalkLookup[mapInEntry.first][asicIdx][uAdc] = asicPars[asicIdx].GetWalkCoef(uAdc);
      }
    }
  }
  fbUseTimeWalkCorrection = true;
}

ClassImp(CbmStsUnpackAlgoBase)
