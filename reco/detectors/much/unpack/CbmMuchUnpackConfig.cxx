/* Copyright (C) 2022 Fair GmbH, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

#include "CbmMuchUnpackConfig.h"

#include "CbmMuchUnpackAlgo.h"

#include <Logger.h>

#include <Rtypes.h>
#include <RtypesCore.h>

#include <memory>
#include <vector>

CbmMuchUnpackConfig::CbmMuchUnpackConfig(std::string detGeoSetupTag, UInt_t runid)
  : CbmRecoUnpackConfig("CbmMuchUnpackConfig", detGeoSetupTag, runid)
{
}

CbmMuchUnpackConfig::~CbmMuchUnpackConfig() {}

// ---- Init ----
void CbmMuchUnpackConfig::InitAlgo()
{
  // Set the minimum adc cut
  fAlgo->SetMinAdcCut(fdAdcCut);

  // Set the minimum adc cut Feb independent
  for (auto cut = fdAdcCut_perFeb.begin(); cut != fdAdcCut_perFeb.end(); cut++) {
    fAlgo->SetMinAdcCut(cut->first, cut->second);
  }

  // Set the single asics time offsets
  fAlgo->SetAsicTimeOffsetVec(fvdTimeOffsetNsAsics);

  // Set the flags for duplicate digis rejections
  fAlgo->SetDuplicatesRejection(fbRejectDuplicateDigis, fbDupliWithoutAdc);
  
  // --- Read list of inactive channels
  if (!fInactiveChannelFileName.IsNull()) {
    LOG(info) << GetName() << ": Reading inactive channels from " << fInactiveChannelFileName;
    auto result = ReadInactiveChannels();
    if (!result.second) {
      LOG(error) << GetName() << ": Error in reading from file! Task will be inactive.";
      return;
    }
    //LOG(info) << GetName() << ": " << std::get<0>(result) << " lines read from file, " << fInactiveChannels.size()
     //         << " channels set inactive";
  }


  if (fMonitor) { fAlgo->SetMonitor(fMonitor); }

  // Now we have all information required to initialise the algorithm
  fAlgo->Init();

  // Mask the noisy channels set by the user
  for (auto chmask : fvChanMasks)
    fAlgo->MaskNoisyChannel(chmask.uFeb, chmask.uChan, chmask.bMasked);
    
}

// ---- chooseAlgo ----
std::shared_ptr<CbmMuchUnpackAlgo> CbmMuchUnpackConfig::chooseAlgo()
{

  if (fDoLog) LOG(info) << fName << "::Init - chooseAlgo";

  // Default unpacker selection
  // Unpacker algo from mcbm 2021-2022 on and hopefully default for a long time.
  auto algo = std::make_shared<CbmMuchUnpackAlgo>();
  LOG(info) << fName << "::chooseAlgo() - selected algo = " << algo->Class_Name();

  // Make sure the par file name picked by the user is sent to the algo
  algo->SetParFileName(fsParFileName);

  return algo;

  LOG(error) << fName
             << "::Init - chooseAlgo() - no algorithm created something went wrong. We can not work like this!";
  return nullptr;
}


// -----   Read list of inactive channels   ---------------------------------
std::pair<size_t, bool> CbmMuchUnpackConfig::ReadInactiveChannels()
{
 
  if (fInactiveChannelFileName.IsNull()) return std::make_pair(0, true);
 
  FILE* channelFile = fopen(fInactiveChannelFileName.Data(), "r");
  if (channelFile == nullptr) return std::make_pair(0, false);
 
  size_t nLines    = 0;
  uint32_t channel = 0;
  while (fscanf(channelFile, "%u", &channel) == 1) {
    fAlgo->SetInactiveChannel(channel);
    nLines++;
  }
  bool success = feof(channelFile);
 
  fclose(channelFile);
  LOG(info) << " Number of Noisy Channels in CbmAddress Format : " << nLines;
  return std::make_pair(nLines, success);
}
// --------------------------------------------------------------------------


ClassImp(CbmMuchUnpackConfig)
