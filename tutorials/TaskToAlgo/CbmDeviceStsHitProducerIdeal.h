/** @file CbmDeviceStsHitProducerIdeal.h
  * @copyright Copyright (C) 2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

/**
 * CbmDeviceStsHitProducerIdeal.h
 *
 * @since 2019-08-28
 * @author F. Uhlig
 */

#ifndef CBMDEVICEMSTSHITPRODUCERIDEAL_H_
#define CBMDEVICEMSTSHITPRODUCERIDEAL_H_

#include "CbmMQChannels.h"
#include "CbmStsHitProducerIdealAlgo.h"

#include "FairMQDevice.h"

#include "TMessage.h"

#include <string>
#include <vector>

class CbmTrdParSetGas;

class CbmDeviceStsHitProducerIdeal : public FairMQDevice {
public:
  CbmDeviceStsHitProducerIdeal();
  virtual ~CbmDeviceStsHitProducerIdeal();

protected:
  virtual void InitTask();
  bool HandleData(FairMQMessagePtr&, int);

private:
  uint64_t fMaxEvents;
  uint64_t fNumMessages;
  std::string fRunId;
  std::string fvmcworkdir;

  CbmTrdParSetGas* fTrdGasPar;

  std::vector<std::string> fAllowedChannels = {"StsPoint", "parameters"};

  std::vector<std::vector<std::string>> fChannelsToSend = {{}};
  std::vector<int> fComponentsToSend {};

  CbmMQChannels fChan {fAllowedChannels};

  CbmStsHitProducerIdealAlgo* fAlgo {new CbmStsHitProducerIdealAlgo()};

  bool IsChannelNameAllowed(std::string channelName);

  bool InitContainers();

  bool DoWork();

  bool SendData();

  void Finish();
};

// special class to expose protected TMessage constructor
class CbmMQTMessage : public TMessage {
public:
  CbmMQTMessage(void* buf, Int_t len) : TMessage(buf, len) { ResetBit(kIsOwner); }
};

#endif /* CBMDEVICESTSLOCALRECO_H_ */
