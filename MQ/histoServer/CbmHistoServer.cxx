/* Copyright (C) 2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#include "CbmHistoServer.h"

#include <mutex>
//#include "CbmHistoCanvasDrawer.h"
#include <Logger.h>

#include "TH1.h"
#include "THttpServer.h"
#include "TMessage.h"
#include "TObjArray.h"

#include "RootSerializer.h"

std::mutex mtx;

CbmHistoServer::CbmHistoServer()
  : FairMQDevice()
  , fInputChannelName("histogram-in")
  , fArrayHisto()
  , fNMessages(0)
  , fServer("http:8088")
  //    , fCanvasDrawer(nullptr)
  , fStopThread(false)
{
}

CbmHistoServer::~CbmHistoServer() {}

void CbmHistoServer::InitTask()
{
  OnData(fInputChannelName, &CbmHistoServer::ReceiveData);

  /*
    if (fCanvasDrawer)
    {
        fCanvasDrawer->CreateCanvases(fServer);
    }
*/
}

bool CbmHistoServer::ReceiveData(FairMQMessagePtr& msg, int /*index*/)
{
  TObject* tempObject = nullptr;
  Deserialize<RootSerializer>(*msg, tempObject);

  if (TString(tempObject->ClassName()).EqualTo("TObjArray")) {
    std::lock_guard<std::mutex> lk(mtx);
    TObjArray* arrayHisto = static_cast<TObjArray*>(tempObject);
    TH1* histogram_new;
    TH1* histogram_existing;
    for (Int_t i = 0; i < arrayHisto->GetEntriesFast(); i++) {
      TObject* obj   = arrayHisto->At(i);
      TH1* histogram = static_cast<TH1*>(obj);
      int index1     = FindHistogram(histogram->GetName());
      if (-1 == index1) {
        histogram_new = static_cast<TH1*>(histogram->Clone());
        fArrayHisto.Add(histogram_new);
        fServer.Register("Histograms", histogram_new);
      }
      else {
        histogram_existing = static_cast<TH1*>(fArrayHisto.At(index1));
        histogram_existing->Add(histogram);
      }
    }

    arrayHisto->Clear();
  }

  fNMessages += 1;

  delete tempObject;

  return true;
}

void CbmHistoServer::PreRun()
{
  fStopThread = false;
  fThread     = std::thread(&CbmHistoServer::UpdateHttpServer, this);
}

void CbmHistoServer::UpdateHttpServer()
{
  while (!fStopThread) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::lock_guard<std::mutex> lk(mtx);

    /*
        if (fCanvasDrawer)
        {
            fCanvasDrawer->DrawHistograms(fArrayHisto);
        }
*/

    fServer.ProcessRequests();
  }
}

void CbmHistoServer::PostRun()
{
  fStopThread = true;
  fThread.join();
}

int CbmHistoServer::FindHistogram(const std::string& name)
{
  for (int i = 0; i < fArrayHisto.GetEntriesFast(); i++) {
    TObject* obj = fArrayHisto.At(i);
    if (TString(obj->GetName()).EqualTo(name)) { return i; }
  }
  return -1;
}
