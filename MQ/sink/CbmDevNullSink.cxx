/** @file CbmDevNullSink.cxx
  * @copyright Copyright (C) 2017-2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

/**
 * CbmDevNullSink.cxx
 *
 * @since 2017-11-17
 * @author F. Uhlig
 */

#include "CbmDevNullSink.h"

#include "FairMQLogger.h"
#include "FairMQProgOptions.h"  // device->fConfig

using namespace std;

CbmDevNullSink::CbmDevNullSink() : FairMQDevice {}, fNumMessages {0} {}

void CbmDevNullSink::Init() {}

void CbmDevNullSink::InitTask()
{
  // register a handler for data arriving on any channel
  int noChannel = fChannels.size();
  LOG(info) << "Number of defined input channels: " << noChannel;
  for (auto const& entry : fChannels) {
    LOG(info) << "Channel name: " << entry.first;
    OnData(entry.first, &CbmDevNullSink::HandleData);
  }
}

// handler is called whenever a message arrives on "data", with a reference to the message and a sub-channel index (here 0)
bool CbmDevNullSink::HandleData(FairMQMessagePtr& msg, int /*index*/)
{
  // Don't do anything with the data
  // Maybe add an message counter which counts the incomming messages and add
  // an output
  fNumMessages++;
  LOG(info) << "Received message number " << fNumMessages << " with size " << msg->GetSize();
  return true;
}

CbmDevNullSink::~CbmDevNullSink() {}
