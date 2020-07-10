#ifndef CBMMQCHANNELS_H_
#define CBMMQCHANNELS_H_

#include <string>
#include <vector>

class FairMQDevice;

class CbmMQChannels {
public:
  CbmMQChannels(std::vector<std::string>);

  bool IsChannelNameAllowed(std::string channelName);
  bool CheckChannels(FairMQDevice* device);

  std::vector<int> GetComponentsToSend() { return fComponentsToSend; }
  std::vector<std::vector<std::string>> GetChannelsToSend() {
    return fChannelsToSend;
  }

private:
  std::vector<std::string> fAllowedChannels {};
  std::vector<int> fComponentsToSend {};
  std::vector<std::vector<std::string>> fChannelsToSend {{}};
};

#endif
