/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef CBMMQHISTOSERVER_H
#define CBMMQHISTOSERVER_H

#include "FairMQDevice.h"

#include "THttpServer.h"
#include "TObjArray.h"
#include <thread>

#include <memory>
#include <string>

class TNamed;
class TCanvas;

class CbmMqHistoServer : public FairMQDevice {
public:
  CbmMqHistoServer();

  virtual ~CbmMqHistoServer();

  void UpdateHttpServer();

protected:
  virtual void InitTask();

  bool ReceiveData(FairMQMessagePtr& msg, int index);

  bool ReceiveHistoConfig(FairMQMessagePtr& msg, int index);

  bool ReceiveCanvasConfig(FairMQMessagePtr& msg, int index);

  bool ReceiveConfigAndData(FairMQParts& msg, int index);

  virtual void PreRun();

  virtual void PostRun();

private:
  /// Parameters
  std::string fsChannelNameHistosInput;
  std::string fsChannelNameHistosConfig;
  std::string fsChannelNameCanvasConfig;
  std::string fsHistoFileName;
  uint32_t fuHttpServerPort;

  /// Array of histograms with unique names
  TObjArray fArrayHisto;
  /// Vector of string with ( HistoName, FolderPath ) to send to the histogram server
  std::vector<std::pair<std::string, std::string>> fvpsHistosFolder;
  /// Vector of string pairs with ( CanvasName, CanvasConfig ) to send to the histogram server
  /// Format of Can config is "Name;Title;NbPadX(U);NbPadY(U);ConfigPad1(s);....;ConfigPadXY(s)"
  /// Format of Pad config is "GrixX(b),GridY(b),LogX(b),LogY(b),LogZ(b),HistoName(s),DrawOptions(s)"
  std::vector<std::pair<std::string, std::string>> fvpsCanvasConfig;
  std::vector<bool> fvbCanvasReady;
  bool fbAllCanvasReady;

  std::vector<std::pair<TNamed*, std::string>> fvHistos;  //! Vector of Histos pointers and folder path
  std::vector<bool> fvbHistoRegistered;
  bool fbAllHistosRegistered;
  std::vector<std::pair<TCanvas*, std::string>> fvCanvas;  //! Vector of Canvas pointers and folder path
  std::vector<bool> fvbCanvasRegistered;
  bool fbAllCanvasRegistered;

  /// Internal status
  int fNMessages;

  THttpServer* fServer;

  std::thread fThread;
  bool fStopThread;

  template<class HistoT>
  bool ReadHistogram(HistoT* pHist);
  int FindHistogram(const std::string& name);
  bool PrepareCanvas(uint32_t uCanvIdx);

  bool ResetHistograms();
  bool SaveHistograms();
};

#endif  // CBMMQHISTOSERVER_H
