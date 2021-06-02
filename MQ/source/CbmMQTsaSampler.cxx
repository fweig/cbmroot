/** @file CbmMQTsaSampler.cxx
  * @copyright Copyright (C) 2017-2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

/**
 *  CbmMQTsaSampler.cpp
 *
 * @since 2017-11-17
 * @author F. Uhlig
 */


#include "CbmMQTsaSampler.h"

#include "CbmMQDefs.h"

#include "TimesliceInputArchive.hpp"
#include "TimesliceSubscriber.hpp"

#include "FairMQLogger.h"
#include "FairMQProgOptions.h"  // device->fConfig

#include <boost/algorithm/string.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

namespace filesys = boost::filesystem;

#include <thread>  // this_thread::sleep_for

#include <algorithm>
#include <chrono>
#include <ctime>
#include <string>

#include <stdio.h>

using namespace std;

#include <stdexcept>

struct InitTaskError : std::runtime_error {
  using std::runtime_error::runtime_error;
};

CbmMQTsaSampler::CbmMQTsaSampler()
  : FairMQDevice()
  , fMaxTimeslices(0)
  , fFileName("")
  , fDirName("")
  , fInputFileList()
  , fFileCounter(0)
  , fHost("")
  , fPort(0)
  , fTSNumber(0)
  , fTSCounter(0)
  , fMessageCounter(0)
  , fSource(nullptr)
  , fTime()
{
}

void CbmMQTsaSampler::InitTask()
try {
  // Get the values from the command line options (via fConfig)
  fFileName      = fConfig->GetValue<string>("filename");
  fDirName       = fConfig->GetValue<string>("dirname");
  fHost          = fConfig->GetValue<string>("flib-host");
  fPort          = fConfig->GetValue<uint64_t>("flib-port");
  fMaxTimeslices = fConfig->GetValue<uint64_t>("max-timeslices");

  // Check which input is defined
  // Posibilities
  // filename && ! dirname : single file
  // filename with wildcards && diranme : all files with filename regex in the directory
  // host && port : connect to the flim server

  bool isGoodInputCombi {false};
  if (0 != fFileName.size() && 0 == fDirName.size() && 0 == fHost.size() && 0 == fPort) {
    isGoodInputCombi = true;
    // Create a Path object from given path string
    filesys::path pathObj(fFileName);
    if (!filesys::is_regular_file(pathObj)) { throw InitTaskError("Passed file name is no valid file"); }
    fInputFileList.push_back(fFileName);
    LOG(info) << "Filename: " << fFileName;
  }
  else if (0 != fFileName.size() && 0 != fDirName.size() && 0 == fHost.size() && 0 == fPort) {
    isGoodInputCombi      = true;
    filesys::path pathObj = fDirName;
    if (!filesys::is_directory(pathObj)) { throw InitTaskError("Passed directory name is no valid directory"); }
    if (fFileName.find("*") == std::string::npos) {
      // Normal file without wildcards
      pathObj += fFileName;
      if (!filesys::is_regular_file(pathObj)) { throw InitTaskError("Passed file name is no valid file"); }
      fInputFileList.push_back(pathObj.string());
      LOG(info) << "Filename: " << fInputFileList[0];
    }
    else {
      std::vector<filesys::path> v;

      // escape "." which have a special meaning in regex
      // change "*" to ".*" to find any number
      // e.g. tofget4_hd2018.*.tsa => tofget4_hd2018\..*\.tsa
      boost::replace_all(fFileName, ".", "\\.");
      boost::replace_all(fFileName, "*", ".*");

      // create regex
      const boost::regex my_filter(fFileName);

      // loop over all files in input directory
      for (auto&& x : filesys::directory_iterator(pathObj)) {
        // Skip if not a file
        if (!boost::filesystem::is_regular_file(x)) continue;

        // Skip if no match
        // x.path().leaf().string() means get from directory iterator the
        // current entry as filesys::path, from this extract the leaf
        // filename or directory name and convert it to a string to be
        // used in the regex:match
        boost::smatch what;
        if (!boost::regex_match(x.path().leaf().string(), what, my_filter)) continue;

        v.push_back(x.path());
      }

      // sort the files which match the regex in increasing order
      // (hopefully)
      std::sort(v.begin(), v.end());

      for (auto&& x : v)
        fInputFileList.push_back(x.string());

      LOG(info) << "The following files will be used in this order.";
      for (auto&& x : v)
        LOG(info) << "    " << x;
    }
  }
  else if (0 == fFileName.size() && 0 == fDirName.size() && 0 != fHost.size() && 0 != fPort) {
    isGoodInputCombi = true;
    LOG(info) << "Host: " << fHost;
    LOG(info) << "Port: " << fPort;
  }
  else {
    isGoodInputCombi = false;
  }

  if (!isGoodInputCombi) {
    throw InitTaskError("Wrong combination of inputs. Either file or wildcard file + directory "
                        "or host + port are allowed combination.");
  }


  LOG(info) << "MaxTimeslices: " << fMaxTimeslices;

  // Get the information about created channels from the device
  // Check if the defined channels from the topology (by name)
  // are in the list of channels which are possible/allowed
  // for the device
  // The idea is to check at initilization if the devices are
  // properly connected. For the time beeing this is done with a
  // nameing convention. It is not avoided that someone sends other
  // data on this channel.
  int noChannel = fChannels.size();
  LOG(info) << "Number of defined output channels: " << noChannel;
  for (auto const& entry : fChannels) {
    LOG(info) << "Channel name: " << entry.first;
    if (!IsChannelNameAllowed(entry.first)) throw InitTaskError("Channel name does not match.");
  }

  for (auto const& value : fComponentsToSend) {
    LOG(info) << "Value : " << value;
    if (value > 1) {
      throw InitTaskError("Sending same data to more than one output channel "
                          "not implemented yet.");
    }
  }

  if (0 == fFileName.size() && 0 != fHost.size()) {
    std::string connector = "tcp://" + fHost + ":" + std::to_string(fPort);
    LOG(info) << "Open TSPublisher at " << connector;
    fSource = new fles::TimesliceSubscriber(connector);
    if (!fSource) { throw InitTaskError("Could not connect to publisher."); }
  }
  else {
    if (false == OpenNextFile()) {
      throw InitTaskError("Could not open the first input file in the list, Doing nothing!");
    }
  }

  fTime = std::chrono::steady_clock::now();
}
catch (InitTaskError& e) {
  LOG(error) << e.what();
  // Wrapper defined in CbmMQDefs.h to support different FairMQ versions
  cbm::mq::ChangeState(this, cbm::mq::Transition::ErrorFound);
}

bool CbmMQTsaSampler::OpenNextFile()
{
  // First Close and delete existing source
  if (nullptr != fSource) delete fSource;

  if (fInputFileList.size() > 0) {
    fFileName = fInputFileList[0];
    fInputFileList.erase(fInputFileList.begin());
    LOG(info) << "Open the Flib input file " << fFileName;
    filesys::path pathObj(fFileName);
    if (!filesys::is_regular_file(pathObj)) {
      LOG(error) << "Input file " << fFileName << " doesn't exist.";
      return false;
    }
    fSource = new fles::TimesliceInputArchive(fFileName);
    if (!fSource) {
      LOG(error) << "Could not open input file.";
      return false;
    }
  }
  else {
    LOG(info) << "End of files list reached.";
    return false;
  }
  return true;
}

bool CbmMQTsaSampler::IsChannelNameAllowed(std::string channelName)
{

  for (auto const& entry : fAllowedChannels) {
    std::size_t pos1 = channelName.find(entry);
    if (pos1 != std::string::npos) {
      const vector<std::string>::const_iterator pos =
        std::find(fAllowedChannels.begin(), fAllowedChannels.end(), entry);
      const vector<std::string>::size_type idx = pos - fAllowedChannels.begin();
      LOG(info) << "Found " << entry << " in " << channelName;
      LOG(info) << "Channel name " << channelName << " found in list of allowed channel names at position " << idx;
      fComponentsToSend[idx]++;
      fChannelsToSend[idx].push_back(channelName);
      return true;
    }
  }
  LOG(info) << "Channel name " << channelName << " not found in list of allowed channel names.";
  LOG(error) << "Stop device.";
  return false;
}

bool CbmMQTsaSampler::ConditionalRun()
{


  auto timeslice = fSource->get();
  if (timeslice) {
    if (fTSCounter < fMaxTimeslices) {
      fTSCounter++;
      if (fTSCounter % 10000 == 0) LOG(info) << "Analyse Event " << fTSCounter;


      const fles::Timeslice& ts = *timeslice;
      //      auto tsIndex = ts.index();


      LOG(info) << "Found " << ts.num_components() << " different components in timeslice";


      CheckTimeslice(ts);

      for (unsigned int nrComp = 0; nrComp < ts.num_components(); ++nrComp) {
        CreateAndSendComponent(ts, nrComp);
      }
      return true;
    }
    else {
      if (false == OpenNextFile()) {
        CalcRuntime();
        return false;
      }
      else {
        return true;
      }
    }
  }
  else {
    if (false == OpenNextFile()) {
      CalcRuntime();
      return false;
    }
    else {
      return true;
    }
  }
}

bool CbmMQTsaSampler::CreateAndSendComponent(const fles::Timeslice& ts, int nrComp)
{

  // Check if component has to be send. If the corresponding channel
  // is connected create the new timeslice and send it to the
  // correct channel

  LOG(info) << "SysID: " << static_cast<int>(ts.descriptor(nrComp, 0).sys_id);
  const vector<int>::const_iterator pos =
    std::find(fSysId.begin(), fSysId.end(), static_cast<int>(ts.descriptor(nrComp, 0).sys_id));
  if (pos != fSysId.end()) {
    const vector<std::string>::size_type idx = pos - fSysId.begin();
    if (fComponentsToSend[idx] > 0) {
      LOG(info) << "Create timeslice component for link " << nrComp;

      fles::StorableTimeslice component {static_cast<uint32_t>(ts.num_microslices(nrComp), ts.index())};
      component.append_component(ts.num_microslices(0));

      for (size_t m = 0; m < ts.num_microslices(nrComp); ++m) {
        component.append_microslice(0, m, ts.descriptor(nrComp, m), ts.content(nrComp, m));
      }
      if (!SendData(component, idx)) return false;
      return true;
    }
  }
  return true;
}

bool CbmMQTsaSampler::SendData(const fles::StorableTimeslice& component, int idx)
{
  // serialize the timeslice and create the message
  std::stringstream oss;
  boost::archive::binary_oarchive oa(oss);
  oa << component;
  std::string* strMsg = new std::string(oss.str());

  FairMQMessagePtr msg(NewMessage(
    const_cast<char*>(strMsg->c_str()),  // data
    strMsg->length(),                    // size
    [](void* /*data*/, void* object) { delete static_cast<std::string*>(object); },
    strMsg));  // object that manages the data

  // TODO: Implement sending same data to more than one channel
  // Need to create new message (copy message??)
  if (fComponentsToSend[idx] > 1) { LOG(info) << "Need to copy FairMessage"; }

  // in case of error or transfer interruption,
  // return false to go to IDLE state
  // successfull transfer will return number of bytes
  // transfered (can be 0 if sending an empty message).

  LOG(info) << "Send data to channel " << fChannelsToSend[idx][0];
  if (Send(msg, fChannelsToSend[idx][0]) < 0) {
    LOG(error) << "Problem sending data";
    return false;
  }

  fMessageCounter++;
  LOG(info) << "Send message " << fMessageCounter << " with a size of " << msg->GetSize();

  return true;
}


CbmMQTsaSampler::~CbmMQTsaSampler() {}

void CbmMQTsaSampler::CalcRuntime()
{
  std::chrono::duration<double> run_time = std::chrono::steady_clock::now() - fTime;

  LOG(info) << "Runtime: " << run_time.count();
  LOG(info) << "No more input data";
}


void CbmMQTsaSampler::PrintMicroSliceDescriptor(const fles::MicrosliceDescriptor& mdsc)
{
  LOG(info) << "Header ID: Ox" << std::hex << static_cast<int>(mdsc.hdr_id) << std::dec;
  LOG(info) << "Header version: Ox" << std::hex << static_cast<int>(mdsc.hdr_ver) << std::dec;
  LOG(info) << "Equipement ID: " << mdsc.eq_id;
  LOG(info) << "Flags: " << mdsc.flags;
  LOG(info) << "Sys ID: Ox" << std::hex << static_cast<int>(mdsc.sys_id) << std::dec;
  LOG(info) << "Sys version: Ox" << std::hex << static_cast<int>(mdsc.sys_ver) << std::dec;
  LOG(info) << "Microslice Idx: " << mdsc.idx;
  LOG(info) << "Checksum: " << mdsc.crc;
  LOG(info) << "Size: " << mdsc.size;
  LOG(info) << "Offset: " << mdsc.offset;
}

bool CbmMQTsaSampler::CheckTimeslice(const fles::Timeslice& ts)
{
  if (0 == ts.num_components()) {
    LOG(error) << "No Component in TS " << ts.index();
    return 1;
  }
  LOG(info) << "Found " << ts.num_components() << " different components in timeslice";

  for (size_t c = 0; c < ts.num_components(); ++c) {
    LOG(info) << "Found " << ts.num_microslices(c) << " microslices in component " << c;
    LOG(info) << "Component " << c << " has a size of " << ts.size_component(c) << " bytes";
    LOG(info) << "Component " << c << " has the system id 0x" << std::hex
              << static_cast<int>(ts.descriptor(c, 0).sys_id) << std::dec;
    LOG(info) << "Component " << c << " has the system id 0x" << static_cast<int>(ts.descriptor(c, 0).sys_id);

    /*
    for (size_t m = 0; m < ts.num_microslices(c); ++m) {
      PrintMicroSliceDescriptor(ts.descriptor(c,m));
    }
*/
  }

  return true;
}
