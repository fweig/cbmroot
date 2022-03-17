/* Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer]  */

#include "CbmBmonUnpackConfig.h"

#include "CbmTofUnpackAlgo.h"

#include <Logger.h>

#include <Rtypes.h>
#include <RtypesCore.h>

#include <memory>
#include <vector>

CbmBmonUnpackConfig::CbmBmonUnpackConfig(std::string detGeoSetupTag, UInt_t runid)
  : CbmTofUnpackConfig(detGeoSetupTag, runid)
{
}

CbmBmonUnpackConfig::~CbmBmonUnpackConfig() {}


/**
 * @brief Register the output vectors (if needed) to the FairRoot Manager)
 * FIXME: To be removed with the Manager include whenever the similar code in the CbmRecoUnpack task work as expected
 *
*/
void CbmBmonUnpackConfig::RegisterOutput(FairRootManager* ioman)
{
  if (fDoWriteOutput && fOutputVec) {
    /// Instance of TOF unpacker used for Bmon unpacking
    ioman->RegisterAny("T0Digi", fOutputVec, kTRUE);
    LOG(info) << fName << "::registerBranchToTree(CbmBmonDigi)";
  }

  /** @todo for the optional outputs it is more complicated to check if they exist. Needs exceptions for the std::nullptr_t. Should be added at some point in time. */
  if (fDoWriteOptOutA && fOptOutAVec) {
    ioman->RegisterAny(fOptoutABranchName.data(), fOptOutAVec, kTRUE);
    LOG(info) << fName << "::registerBranchToTree(" << fOptoutABranchName.data() << ")";
  }

  if (fDoWriteOptOutB && fOptOutBVec) {
    ioman->RegisterAny(fOptoutBBranchName.data(), fOptOutBVec, kTRUE);
    LOG(info) << fName << "::registerBranchToTree(" << fOptoutBBranchName.data() << ")";
  }
}

ClassImp(CbmBmonUnpackConfig)
