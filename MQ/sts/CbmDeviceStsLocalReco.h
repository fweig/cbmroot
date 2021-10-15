/* Copyright (C) 2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

/**
 * CbmDeviceStsLocalReco.h
 *
 * @since 2019-08-28
 * @author F. Uhlig
 */

#ifndef CBMDEVICEMSTSLOCALRECO_H_
#define CBMDEVICEMSTSLOCALRECO_H_

#include "CbmMqTMessage.h"

#include "FairMQDevice.h"

//#include "CbmStsDigitizeParameters.h"

//#include "FairGeoParSet.h"

//#include "Rtypes.h"
//#include "TObjArray.h"

#include <string>
#include <vector>

//class TList;
class CbmStsDigitizeParameters;
class CbmFieldPar;

class FairGeoParSet;
class FairField;

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
  std::string fRunId;
  std::string fvmcworkdir;

  CbmStsDigitizeParameters* fDigiPar;

  FairGeoParSet* fGeoPar;

  CbmFieldPar* fFieldPar;

  std::vector<std::string> fAllowedChannels             = {"stsdigi", "parameters"};
  std::vector<std::vector<std::string>> fChannelsToSend = {{}, {}};

  bool IsChannelNameAllowed(std::string channelName);

  bool InitContainers();

  bool DoWork();

  bool SendData();

  void Finish();


  FairField* createField();

  //    TList* fParCList;

  //    TObjArray fArrayHisto;
};

#endif /* CBMDEVICESTSLOCALRECO_H_ */
