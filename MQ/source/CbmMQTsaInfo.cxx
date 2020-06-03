/**
 *  CbmMQTsaInfo.cpp
 *
 * @since 2017-11-17
 * @author F. Uhlig
 */


#include "CbmMQTsaInfo.h"
#include "CbmMQDefs.h"

#include "FairMQLogger.h"
#include "FairMQProgOptions.h" // device->fConfig

#include "TimesliceSubscriber.hpp"
#include "TimesliceInputArchive.hpp"

#include <boost/archive/binary_oarchive.hpp>

#include <stdio.h>
#include <ctime>
#include <thread> // this_thread::sleep_for
#include <chrono>

using namespace std;

#include <stdexcept>

struct InitTaskError : std::runtime_error { using std::runtime_error::runtime_error; };


CbmMQTsaInfo::CbmMQTsaInfo()
    : FairMQDevice()
    , fMaxTimeslices(0)
    , fFileName("")
    , fInputFileList()
    , fFileCounter(0)
    , fHost("")
    , fPort(0)
    , fTSNumber(0)
    , fTSCounter(0)
    , fMessageCounter(0)
    , fTime()
{
}

void CbmMQTsaInfo::InitTask()
try
{
    // Get the values from the command line options (via fConfig)
    fFileName = fConfig->GetValue<string>("filename");
    fHost = fConfig->GetValue<string>("flib-host");
    fPort = fConfig->GetValue<uint64_t>("flib-port");
    fMaxTimeslices = fConfig->GetValue<uint64_t>("max-timeslices");


    LOG(info) << "Filename: " << fFileName;
    LOG(info) << "Host: " << fHost;
    LOG(info) << "Port: " << fPort;

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
    for(auto const &entry : fChannels) {
      LOG(info) << "Channel name: " << entry.first;
      if (!IsChannelNameAllowed(entry.first)) throw InitTaskError("Channel name does not match.");
    }

  if ( 0 == fFileName.size() && 0 != fHost.size() ) {
    std::string connector = "tcp://" + fHost + ":" + std::to_string(fPort);
    LOG(info) << "Open TSPublisher at " << connector;
    fSource = new fles::TimesliceSubscriber(connector);
    if ( !fSource) {
      throw InitTaskError("Could not connect to publisher.");
    }
  } else {
    LOG(info) << "Open the Flib input file " << fFileName;
    // Check if the input file exist
    FILE* inputFile = fopen(fFileName.c_str(), "r");
    if ( ! inputFile )  {
      throw InitTaskError("Input file doesn't exist.");
    }
    fclose(inputFile);
    fSource = new fles::TimesliceInputArchive(fFileName);
    if ( !fSource) {
      throw InitTaskError("Could not open input file.");
    }

  }
  fTime = std::chrono::steady_clock::now();
} catch (InitTaskError& e) {
  LOG(error) << e.what();
  // Wrapper defined in CbmMQDefs.h to support different FairMQ versions
  cbm::mq::ChangeState(this, cbm::mq::Transition::ErrorFound);
}

bool CbmMQTsaInfo::IsChannelNameAllowed(std::string channelName)
{
  if ( std::find(fAllowedChannels.begin(), fAllowedChannels.end(),
       channelName) != fAllowedChannels.end() ) {
    LOG(info) << "Channel name " << channelName
              << " found in list of allowed channel names.";
    return true;
  } else {
    LOG(info) << "Channel name " << channelName
              << " not found in list of allowed channel names.";
    LOG(error) << "Stop device.";
    return false;
  }
}

bool CbmMQTsaInfo::ConditionalRun()
{


  auto timeslice = fSource->get();
  if (timeslice) {
    fTSCounter++;
    if (fTSCounter % 10000 == 0)  LOG(info) << "Analyse Event " << fTSCounter;


    const fles::Timeslice& ts = *timeslice;
//    auto tsIndex = ts.index();


    LOG(info) << "Found " << ts.num_components()
              << " different components in timeslice";

    CheckTimeslice(ts);

    if (fTSCounter < fMaxTimeslices) {
      return true;
    } else {
      CalcRuntime();
      return false;
    }
  } else {
    CalcRuntime();
    return false;
  }

}


CbmMQTsaInfo::~ CbmMQTsaInfo()
{
}

void CbmMQTsaInfo::CalcRuntime()
{
  std::chrono::duration<double> run_time =  
     std::chrono::steady_clock::now() - fTime;

  LOG(info) << "Runtime: " << run_time.count();
  LOG(info) << "No more input data";
}



void CbmMQTsaInfo::PrintMicroSliceDescriptor(const fles::MicrosliceDescriptor& mdsc)
{
  LOG(info) << "Header ID: Ox" << std::hex << static_cast<int>(mdsc.hdr_id)
            << std::dec;
  LOG(info) << "Header version: Ox" << std::hex << static_cast<int>(mdsc.hdr_ver)
            << std::dec;
  LOG(info) << "Equipement ID: " << mdsc.eq_id;
  LOG(info) << "Flags: " << mdsc.flags;
  LOG(info) << "Sys ID: Ox" << std::hex << static_cast<int>(mdsc.sys_id)
            << std::dec;
  LOG(info) << "Sys version: Ox" << std::hex << static_cast<int>(mdsc.sys_ver)
            << std::dec;
  LOG(info) << "Microslice Idx: " << mdsc.idx;
  LOG(info) << "Checksum: " << mdsc.crc;
  LOG(info) << "Size: " << mdsc.size;
  LOG(info) << "Offset: " << mdsc.offset;
}

bool CbmMQTsaInfo::CheckTimeslice(const fles::Timeslice& ts)
{
  if ( 0 == ts.num_components() ) {
    LOG(error) << "No Component in TS " << ts.index();
    return 1;
  }
  LOG(info) << "Found " << ts.num_components()
            << " different components in timeslice";
  
  for (size_t c = 0; c < ts.num_components(); ++c) {
    LOG(info) << "Found " << ts.num_microslices(c) 
              << " microslices in component " << c;
    LOG(info) << "Component " << c << " has a size of "
              << ts.size_component(c) << " bytes";
    LOG(info) << "Component " << c << " has the system id 0x"
              << std::hex << static_cast<int>(ts.descriptor(c,0).sys_id)
              << std::dec;

/*
    for (size_t m = 0; m < ts.num_microslices(c); ++m) {
      PrintMicroSliceDescriptor(ts.descriptor(c,m));
    }
*/
  }

  return true;
}
