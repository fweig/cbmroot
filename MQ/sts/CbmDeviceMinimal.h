/* Copyright (C) 2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

/**
 * CbmDeviceStsLocalReco.h
 *
 * @since 2019-08-28
 * @author F. Uhlig
 */

#ifndef CBMDEVICESTSLOCALRECO_H_
#define CBMDEVICESTSLOCALRECO_H_

#include "CbmMqTMessage.h"

#include "FairMQDevice.h"

#include <vector>

class CbmDeviceStsLocalReco : public FairMQDevice {
public:
  CbmDeviceStsLocalReco();
  virtual ~CbmDeviceStsLocalReco();

protected:
  virtual void InitTask();
  bool HandleData(FairMQMessagePtr&, int);

private:
  uint64_t fMaxTimeslices;
  uint64_t fNumMessages;
};

#endif /* CBMDEVICESTSLOCALRECO_H_ */
