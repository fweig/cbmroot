/** @file CbmTsaComponentSink.h
  * @copyright Copyright (C) 2018 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

/**
 * CbmTsaComponentSink.h
 *
 * @since 2018-04-24
 * @author F. Uhlig
 */

#ifndef CBMTSACOMPONENTSINK_H_
#define CBMTSACOMPONENTSINK_H_

#include "MicrosliceDescriptor.hpp"
#include "Timeslice.hpp"

#include "FairMQDevice.h"

class CbmTsaComponentSink : public FairMQDevice {
public:
  CbmTsaComponentSink();
  virtual ~CbmTsaComponentSink();

protected:
  virtual void InitTask();
  bool HandleData(FairMQMessagePtr&, int);

private:
  uint64_t fNumMessages;

  std::vector<std::string> fAllowedChannels = {"stscomponent", "tofcomponent", "trdcomponent"};

  bool CheckTimeslice(const fles::Timeslice& ts);
  void PrintMicroSliceDescriptor(const fles::MicrosliceDescriptor& mdsc);
  bool IsChannelNameAllowed(std::string channelName);
};

#endif /* CBMTSACOMPONENTSINK_H_ */
