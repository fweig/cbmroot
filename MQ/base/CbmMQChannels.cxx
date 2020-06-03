#include "CbmMQChannels.h"
#include "FairMQDevice.h"

CbmMQChannels::CbmMQChannels(std::vector<std::string> allowedChannels)
  : fAllowedChannels{allowedChannels}
    {
      fChannelsToSend.resize(fAllowedChannels.size());
      for(auto &entry : fChannelsToSend) {
	entry.push_back("");
      }
      fComponentsToSend.resize(fAllowedChannels.size());	
    }
	
bool CbmMQChannels::IsChannelNameAllowed(std::string channelName)
{
  for(auto const &entry : fAllowedChannels) {
    std::size_t pos1 = channelName.find(entry);
    if (pos1!=std::string::npos) {
      const std::vector<std::string>::const_iterator pos =
	std::find(fAllowedChannels.begin(), fAllowedChannels.end(), entry);
      const std::vector<std::string>::size_type idx = pos-fAllowedChannels.begin();
      LOG(info) << "Found " << entry << " in " << channelName;
      LOG(info) << "Channel name " << channelName
		<< " found in list of allowed channel names at position " << idx;
      fComponentsToSend[idx]++;
      // The array is initialized with one empty string. If the string has still teh value from initialization
      // exchnge the value by the new channel name. In any other case add one more entry to the vector
      if (fChannelsToSend[idx].size() == 1 && fChannelsToSend[idx].at(0).empty()) {
	fChannelsToSend[idx].at(0) = channelName;
      } else {
	fChannelsToSend[idx].push_back(channelName);
      }
      return true;
    }
  }
  LOG(info) << "Channel name " << channelName
	    << " not found in list of allowed channel names.";
  LOG(info) << "The allowed channels are: ";
  for(auto const &entry : fAllowedChannels) {
     LOG(info) << entry;
  }
  LOG(error) << "Stop device.";
  return false;
}

bool CbmMQChannels::CheckChannels(FairMQDevice* device)
{
  // Get the information about created channels from the device
  // Check if the defined channels from the topology (by name)
  // are in the list of channels which are possible/allowed
  // for the device
  // The idea is to check at initilization if the devices are
  // properly connected. For the time beeing this is done with a
  // nameing convention. It is not avoided that someone sends other
  // data on this channel.
  int noChannel = device->fChannels.size();
  LOG(info) << "Number of defined output channels: " << noChannel;
  for(auto const &entry : device->fChannels) {
    LOG(info) << "Channel name: " << entry.first;
    if (!IsChannelNameAllowed(entry.first)) return false;
  }
  return true;
}
