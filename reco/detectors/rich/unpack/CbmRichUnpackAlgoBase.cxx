/* Copyright (C) 2021 Goethe-University Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig [committer] */

#include "CbmRichUnpackAlgoBase.h"

#include <FairParGenericSet.h>
#include <FairTask.h>
#include <Logger.h>

#include <Rtypes.h>
#include <RtypesCore.h>

#include <cstdint>


CbmRichUnpackAlgoBase::CbmRichUnpackAlgoBase(std::string name) : CbmRecoUnpackAlgo(name) {}

CbmRichUnpackAlgoBase::~CbmRichUnpackAlgoBase() {}

// ---- GetParContainerRequest ----
std::vector<std::pair<std::string, std::shared_ptr<FairParGenericSet>>>*
  CbmRichUnpackAlgoBase::GetParContainerRequest(std::string /*geoTag*/, std::uint32_t /*runId*/)
{
  // Basepath for default Trd parameter sets (those connected to a geoTag)
  std::string basepath = Form("%s", fParFilesBasePath.data());
  std::string temppath = "";

  // // Get parameter container
  temppath = basepath + "mRichPar_70.par";

  fParContVec.emplace_back(std::make_pair(temppath, std::make_shared<CbmMcbm2018RichPar>()));

  return &fParContVec;
}

// ---- calculateTime
double CbmRichUnpackAlgoBase::calculateTime(uint32_t epoch, uint32_t coarse, uint32_t fine)
{
  return ((double) epoch) * 2048. * 5. + ((double) coarse) * 5. - ((double) fine) * 0.005;
}

// ---- getLogHeader
std::string CbmRichUnpackAlgoBase::getLogHeader(CbmRichUnpackAlgoMicrosliceReader& reader)
{
  std::stringstream stream;
  stream << "[" << fNrProcessedTs << "-" << reader.GetWordCounter() << "/" << reader.GetSize() / 4 << " "
         << reader.GetWordAsHexString(reader.GetCurWord()) << "] ";
  return stream.str();
}

// ---- init
Bool_t CbmRichUnpackAlgoBase::init() { return kTRUE; }

// ---- initParSet(FairParGenericSet* parset) ----
Bool_t CbmRichUnpackAlgoBase::initParSet(FairParGenericSet* parset)
{
  LOG(info) << fName << "::initParSet - for container " << parset->ClassName();
  if (parset->IsA() == CbmMcbm2018RichPar::Class()) return initParSet(static_cast<CbmMcbm2018RichPar*>(parset));

  // If we do not know the derived ParSet class we return false
  LOG(error) << fName << "::initParSet - for container " << parset->ClassName() << " failed, since" << fName
             << "::initParSet() does not know the derived ParSet and what to do with it!";
  return kFALSE;
}

// ---- initParSet(CbmTrdParSetAsic* parset) ----
Bool_t CbmRichUnpackAlgoBase::initParSet(CbmMcbm2018RichPar* parset)
{
  LOG(debug) << fName << "::initParSetAsic - ";
  fUnpackPar = *parset;
  fUnpackPar.Print();

  LOG(info) << fName << "::initParSetRichMcbm2018 - Successfully initialized RICH unpacking parameters";

  return kTRUE;
}

// ---- isLog ----
bool CbmRichUnpackAlgoBase::isLog()
{
  //if (fTsCounter == 25215) return true;
  return false;
}


bool CbmRichUnpackAlgoBase::checkMaskedDiRICH(Int_t subSubEventId)
{
  for (unsigned int i = 0; i < fMaskedDiRICHes->size(); ++i) {
    if (fMaskedDiRICHes->at(i) == subSubEventId) return true;
  }

  return false;
}

ClassImp(CbmRichUnpackAlgoBase)
