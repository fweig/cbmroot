/** @file CbmDeviceTriggerHandlerEtof.cxx
  * @copyright Copyright (C) 2019 PI-UHd, GSI
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Norbert Herrmann [committer] **/

/**
 * CbmDeviceTriggerHandlerEtof.cxx
 *
 * @since 2019-11-15
 * @author N. Herrmann
 */

#include "CbmDeviceTriggerHandlerEtof.h"

#include "CbmMQDefs.h"

#include "FairEventHeader.h"
#include "FairFileHeader.h"
#include "FairGeoParSet.h"
#include "FairMQLogger.h"
#include "FairMQProgOptions.h"  // device->fConfig
#include "FairRootFileSink.h"
#include "FairRootManager.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"

#include <thread>  // this_thread::sleep_for

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>

#include <chrono>
#include <iomanip>
#include <stdexcept>
#include <string>
struct InitTaskError : std::runtime_error {
  using std::runtime_error::runtime_error;
};

static std::chrono::steady_clock::time_point dctime = std::chrono::steady_clock::now();
static double dSize                                 = 0.;

using namespace std;

CbmDeviceTriggerHandlerEtof::CbmDeviceTriggerHandlerEtof()
  : fNumMessages(0)
  , fiMsgCnt(0)
  , fbMonitorMode(kFALSE)
  , fbDebugMonitorMode(kFALSE)
  , fbSandboxMode(kFALSE)
  , fbEventDumpEna(kFALSE)
  , fdEvent(0.)
{
}

CbmDeviceTriggerHandlerEtof::~CbmDeviceTriggerHandlerEtof() {}

void CbmDeviceTriggerHandlerEtof::InitTask()
try {
  // Get the information about created channels from the device
  // Check if the defined channels from the topology (by name)
  // are in the list of channels which are possible/allowed
  // for the device
  // The idea is to check at initilization if the devices are
  // properly connected. For the time beeing this is done with a
  // nameing convention. It is not avoided that someone sends other
  // data on this channel.
  int noChannel = fChannels.size();
  LOG(info) << "Number of defined input channels: " << noChannel;
  for (auto const& entry : fChannels) {
    LOG(info) << "Channel name: " << entry.first;
    if (!IsChannelNameAllowed(entry.first)) throw InitTaskError("Channel name does not match.");
    if (entry.first != "syscmd") OnData(entry.first, &CbmDeviceTriggerHandlerEtof::HandleData);
    else
      OnData(entry.first, &CbmDeviceTriggerHandlerEtof::HandleMessage);
  }
  InitWorkspace();
}
catch (InitTaskError& e) {
  LOG(error) << e.what();
  cbm::mq::ChangeState(this, cbm::mq::Transition::ErrorFound);
}

bool CbmDeviceTriggerHandlerEtof::IsChannelNameAllowed(std::string channelName)
{
  for (auto const& entry : fAllowedChannels) {
    std::size_t pos1 = channelName.find(entry);
    if (pos1 != std::string::npos) {
      const vector<std::string>::const_iterator pos =
        std::find(fAllowedChannels.begin(), fAllowedChannels.end(), entry);
      const vector<std::string>::size_type idx = pos - fAllowedChannels.begin();
      LOG(info) << "Found " << entry << " in " << channelName;
      LOG(info) << "Channel name " << channelName << " found in list of allowed channel names at position " << idx;
      return true;
    }
  }
  LOG(info) << "Channel name " << channelName << " not found in list of allowed channel names.";
  LOG(error) << "Stop device.";
  return false;
}

Bool_t CbmDeviceTriggerHandlerEtof::InitWorkspace()
{
  LOG(info) << "Init work space for CbmDeviceTriggerHandlerEtof.";

  // steering variables
  fbSandboxMode = fConfig->GetValue<bool>("SandboxMode");

  return kTRUE;
}

// handler is called whenever a message arrives on "data", with a reference to the message and a sub-channel index (here 0)
//bool CbmDeviceTriggerHandlerEtof::HandleData(FairMQMessagePtr& msg, int /*index*/)
bool CbmDeviceTriggerHandlerEtof::HandleData(FairMQParts& parts, int /*index*/)
{
  // Don't do anything with the data
  // Maybe add an message counter which counts the incomming messages and add
  // an output
  fNumMessages++;
  LOG(debug) << "Received message " << fNumMessages << " with " << parts.Size() << " parts"
             << ", size0: " << parts.At(0)->GetSize();

  uint TrigWord {0};
  std::string msgStrE(static_cast<char*>(parts.At(0)->GetData()), (parts.At(0))->GetSize());
  std::istringstream issE(msgStrE);
  boost::archive::binary_iarchive inputArchiveE(issE);
  inputArchiveE >> TrigWord;

  char* pDataBuff = static_cast<char*>(parts.At(1)->GetData());
  int iBuffSzByte = parts.At(1)->GetSize();

  // Send Subevent to STAR
  LOG(debug) << "Send Data for event " << fdEvent << ", TrigWord " << TrigWord << " with size " << iBuffSzByte
             << Form(" at %p ", pDataBuff);
  if (kFALSE == fbSandboxMode) { star_rhicf_write(TrigWord, pDataBuff, iBuffSzByte); }
  dSize += iBuffSzByte;
  if (0 == (int) fdEvent % 10000) {
    std::chrono::duration<double> deltatime = std::chrono::steady_clock::now() - dctime;
    LOG(info) << "Processed " << fdEvent << " events,  delta-time: " << deltatime.count()
              << ", rate: " << dSize * 1.E-6 / deltatime.count() << "MB/s";
    dctime = std::chrono::steady_clock::now();
    dSize  = 0.;
  }
  fdEvent++;

  return kTRUE;
}

/************************************************************************************/

bool CbmDeviceTriggerHandlerEtof::HandleMessage(FairMQMessagePtr& msg, int /*index*/)
{
  const char* cmd    = (char*) (msg->GetData());
  const char cmda[4] = {*cmd};
  LOG(info) << "Handle message " << cmd << ", " << cmd[0];

  // only one implemented so far "Stop"
  if (strcmp(cmda, "STOP")) {
    cbm::mq::ChangeState(this, cbm::mq::Transition::Ready);
    cbm::mq::LogState(this);
    cbm::mq::ChangeState(this, cbm::mq::Transition::DeviceReady);
    cbm::mq::LogState(this);
    cbm::mq::ChangeState(this, cbm::mq::Transition::Idle);
    cbm::mq::LogState(this);
    cbm::mq::ChangeState(this, cbm::mq::Transition::End);
    cbm::mq::LogState(this);
    //    ChangeState(fair::mq::Transition(STOP));
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  return true;
}
