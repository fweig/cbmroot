/* Copyright (C) 2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#ifndef FAIRMQEXHISTOSERVER
#define FAIRMQEXHISTOSERVER

#include "FairMQDevice.h"

#include "THttpServer.h"
#include "TObjArray.h"
#include <thread>

#include <memory>
#include <string>

//class FairMQExHistoCanvasDrawer;

class CbmHistoServer : public FairMQDevice {
public:
  CbmHistoServer();


  virtual ~CbmHistoServer();

  void UpdateHttpServer();

  /*
    void SetCanvasDrawer(std::unique_ptr<FairMQExHistoCanvasDrawer> canvasDrawer)
    {
        fCanvasDrawer = std::move(canvasDrawer);
    }
*/

protected:
  virtual void InitTask();

  bool ReceiveData(FairMQMessagePtr& msg, int index);

  virtual void PreRun();

  virtual void PostRun();

private:
  std::string fInputChannelName;

  TObjArray fArrayHisto;

  int fNMessages;

  THttpServer fServer;

  //    std::unique_ptr<FairMQExHistoCanvasDrawer> fCanvasDrawer;

  std::thread fThread;
  bool fStopThread;

  int FindHistogram(const std::string& name);
};

#endif
