/**
 * CbmTsaComponentSink.cxx
 *
 * @since 2018-04-24
 * @author F. Uhlig
 */

#include "CbmTsaComponentSink.h"
#include "CbmMQDefs.h"

#include "StorableTimeslice.hpp"

#include "FairMQLogger.h"
#include "FairMQProgOptions.h" // device->fConfig

#include <boost/archive/binary_iarchive.hpp>

#include <string>

#include <stdexcept>
struct InitTaskError : std::runtime_error { using std::runtime_error::runtime_error; };

using namespace std;

CbmTsaComponentSink::CbmTsaComponentSink()
    : fNumMessages(0)
{
}

void CbmTsaComponentSink::InitTask()
try
{
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
    for(auto const &entry : fChannels) {
      LOG(info) << "Channel name: " << entry.first;
      if (!IsChannelNameAllowed(entry.first)) throw InitTaskError("Channel name does not match.");
      OnData(entry.first, &CbmTsaComponentSink::HandleData);
    }
} catch (InitTaskError& e) {
  LOG(error) << e.what();
  // Wrapper defined in CbmMQDefs.h to support different FairMQ versions
  cbm::mq::ChangeState(this, cbm::mq::Transition::ErrorFound);
}

bool CbmTsaComponentSink::IsChannelNameAllowed(std::string channelName)
{

  for(auto const &entry : fAllowedChannels) {
    std::size_t pos1 = channelName.find(entry);
    if (pos1!=std::string::npos) {
      const vector<std::string>::const_iterator pos =
         std::find(fAllowedChannels.begin(), fAllowedChannels.end(), entry);
      const vector<std::string>::size_type idx = pos-fAllowedChannels.begin();
      LOG(info) << "Found " << entry << " in " << channelName;
      LOG(info) << "Channel name " << channelName
              << " found in list of allowed channel names at position " << idx;
      return true;
    }
  }
  LOG(info) << "Channel name " << channelName
            << " not found in list of allowed channel names.";
  LOG(error) << "Stop device.";
  return false;
}



// handler is called whenever a message arrives on "data", with a reference to the message and a sub-channel index (here 0)
bool CbmTsaComponentSink::HandleData(FairMQMessagePtr& msg, int /*index*/)
{
// Don't do anything with the data
// Maybe add an message counter which counts the incomming messages and add
// an output
  fNumMessages++;
  LOG(info) << "Received message number "<<  fNumMessages 
            << " with size " << msg->GetSize(); 

  std::string msgStr(static_cast<char*>(msg->GetData()), msg->GetSize());
  std::istringstream iss(msgStr);
  boost::archive::binary_iarchive inputArchive(iss);

  fles::StorableTimeslice component{0};
  inputArchive >> component;

  CheckTimeslice(component);

  return true;
}

CbmTsaComponentSink::~CbmTsaComponentSink()
{
}

void CbmTsaComponentSink::PrintMicroSliceDescriptor(const fles::MicrosliceDescriptor& mdsc)
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

bool CbmTsaComponentSink::CheckTimeslice(const fles::Timeslice& ts)
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
    LOG(info) << "Sys ID: Ox" << std::hex << static_cast<int>(ts.descriptor(0,0).sys_id)
            << std::dec;

/*
    for (size_t m = 0; m < ts.num_microslices(c); ++m) {
      PrintMicroSliceDescriptor(ts.descriptor(c,m));
    }
*/
  }

  return true;
}
