/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include <mutex>

#include "CbmMqHistoServer.h"

#include "CbmFlesCanvasTools.h"

#include "BoostSerializer.h"
#include "FairLogger.h"
#include "FairMQProgOptions.h"  // device->fConfig
#include "RootSerializer.h"

#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "THttpServer.h"
#include "TMessage.h"
#include "TObjArray.h"
#include "TProfile.h"
#include "TRootSniffer.h"
#include "TSystem.h"
#include "TEnv.h"

#include <boost/serialization/utility.hpp>

std::mutex mtx;
/*
Bool_t bMqHistoServerResetHistos = kFALSE;
Bool_t bMqHistoServerSaveHistos  = kFALSE;
*/
CbmMqHistoServer::CbmMqHistoServer()
  : FairMQDevice()
  , fsChannelNameHistosInput("histogram-in")
  , fsChannelNameHistosConfig("histo-conf")
  , fsChannelNameCanvasConfig("canvas-conf")
  , fsHistoFileName("HistosMonitorPulser.root")
  , fuHttpServerPort(8098)
  , fArrayHisto()
  , fvpsHistosFolder()
  , fvpsCanvasConfig()
  , fvHistos()
  , fvCanvas()
  , fNMessages(0)
  , fServer(nullptr)
  , fStopThread(false) {}

CbmMqHistoServer::~CbmMqHistoServer() {}

void CbmMqHistoServer::InitTask() {
  /// Read options from executable
  LOG(info) << "Init options for CbmMqHistoServer.";
  fsChannelNameHistosInput  = fConfig->GetValue<std::string>("ChNameIn");
  fsChannelNameHistosConfig = fConfig->GetValue<std::string>("ChNameHistCfg");
  fsChannelNameCanvasConfig = fConfig->GetValue<std::string>("ChNameCanvCfg");
  fsHistoFileName           = fConfig->GetValue<std::string>("HistoFileName");
  fuHttpServerPort          = fConfig->GetValue<uint32_t>("histport");

  /// Link channels to methods in order to process received messages
  OnData(fsChannelNameHistosInput, &CbmMqHistoServer::ReceiveData);
  OnData(fsChannelNameHistosConfig, &CbmMqHistoServer::ReceiveHistoConfig);
  OnData(fsChannelNameCanvasConfig, &CbmMqHistoServer::ReceiveCanvasConfig);

  fServer = new THttpServer(Form("http:%u", fuHttpServerPort));
  /// To avoid the server sucking all Histos from gROOT when no output file is used
  fServer->GetSniffer()->SetScanGlobalDir(kFALSE);
  const char *jsrootsys = gSystem->Getenv("JSROOTSYS");
  if (!jsrootsys)
    jsrootsys = gEnv->GetValue("HttpServ.JSRootPath", jsrootsys);

  LOG(info) << "JSROOT location: " << jsrootsys;

  //fServer->RegisterCommand("/Reset_Hist", "bMqHistoServerResetHistos=kTRUE");
  //fServer->RegisterCommand("/Save_Hist", "bMqHistoServerSaveHistos=kTRUE");

  //fServer->Restrict("/Reset_Hist", "allow=admin");
  //fServer->Restrict("/Save_Hist", "allow=admin");
}

bool CbmMqHistoServer::ReceiveData(FairMQMessagePtr& msg, int /*index*/) {
  TObject* tempObject = nullptr;

  Deserialize<RootSerializer>(*msg, tempObject);

  if (TString(tempObject->ClassName()).EqualTo("TObjArray")) {
    std::lock_guard<std::mutex> lk(mtx);
    TObjArray* arrayHisto = static_cast<TObjArray*>(tempObject);
    for (Int_t i = 0; i < arrayHisto->GetEntriesFast(); i++) {
      TObject* pObj = arrayHisto->At(i);

      if (nullptr != dynamic_cast<TProfile*>(pObj)) {
        if (!ReadHistogram<TProfile>(dynamic_cast<TProfile*>(pObj)))
          return false;
      }  // if( nullptr != dynamic_cast< TProfile *>( pObj ) )
      else if (nullptr != dynamic_cast<TH2*>(pObj)) {
        if (!ReadHistogram<TH2>(dynamic_cast<TH2*>(pObj))) return false;
      }  // if( nullptr != dynamic_cast< TH2 *>( pObj ) )
      else if (nullptr != dynamic_cast<TH1*>(pObj)) {
        if (!ReadHistogram<TH1>(dynamic_cast<TH1*>(pObj))) return false;
      }  // if( nullptr != dynamic_cast< TH1 *>( pObj ) )
      else
        LOG(warning) << "Unsupported object type for " << pObj->GetName();
    }  // for (Int_t i = 0; i < arrayHisto->GetEntriesFast(); i++)

    /// Need to use Delete instead of Clear to avoid memory leak!!!
    arrayHisto->Delete();

    /// If new histos received, try to prepare as many canvases as possible
    /// Should be expensive on start and cheap afterward
    if (!fbAllCanvasReady) {
      for (uint32_t uCanv = 0; uCanv < fvpsCanvasConfig.size(); ++uCanv) {
        /// Jump canvases already ready
        if (fvbCanvasReady[uCanv]) continue;

        /// Now come the expensive part as we unpack its config and check each histo
        fvbCanvasReady[uCanv] = PrepareCanvas(uCanv);
      }  // for( uint32_t uCanv = 0; uCanv < fvpsCanvasConfig.size(); ++uCanv )
    }    // if( !fbAllCanvasReady )
  }      // if (TString(tempObject->ClassName()).EqualTo("TObjArray"))
  else
    LOG(fatal)
      << "CbmMqHistoServer::ReceiveData => Wrong object type at input: "
      << tempObject->ClassName();

  fNMessages += 1;

  if (nullptr != tempObject) delete tempObject;
  /*
  /// TODO: control flags communication with histo server
  /// Idea: 1 req channel (per device or not mixup?), polling every N TS and/or M s
  if (bMqHistoServerResetHistos) {
    std::lock_guard<std::mutex> lk(mtx);
    //      LOG(info) << "Reset Monitor histos ";
    ResetHistograms();
    bMqHistoServerResetHistos = kFALSE;
  }  // if( bMqHistoServerResetHistos )

  if (bMqHistoServerSaveHistos) {
    std::lock_guard<std::mutex> lk(mtx);
    //      LOG(info) << "Save All histos & canvases";
    SaveHistograms();
    bMqHistoServerSaveHistos = kFALSE;
  }  // if( bMqHistoServerSaveHistos )
*/
  return true;
}

bool CbmMqHistoServer::ReceiveHistoConfig(FairMQMessagePtr& msg,
                                          int /*index*/) {
  std::pair<std::string, std::string> tempObject;

  Deserialize<BoostSerializer<std::pair<std::string, std::string>>>(*msg,
                                                                    tempObject);

  LOG(info) << " Received configuration for histo " << tempObject.first << " : "
            << tempObject.second;

  /// Check if histo name already received in previous messages
  /// Linear search should be ok as config is shared only at startup
  UInt_t uPrevHist = 0;
  for (uPrevHist = 0; uPrevHist < fvpsHistosFolder.size(); ++uPrevHist) {
    if (fvpsHistosFolder[uPrevHist].first == tempObject.first) break;
  }  // for( UInt_t uPrevHist = 0; uPrevHist < fvpsHistosFolder.size(); ++uPrevHist )

  if (uPrevHist < fvpsHistosFolder.size()) {
    LOG(info) << " Ignored new configuration for histo " << tempObject.first
              << " due to previously received one: " << tempObject.second;
    /// Not sure if we should return false here...
  }  // if( uPrevHist < fvpsHistosFolder.size() )
  else {
    fvpsHistosFolder.push_back(tempObject);
    fvHistos.push_back(std::pair<TNamed*, std::string>(nullptr, ""));
    fvbHistoRegistered.push_back(false);
    fbAllHistosRegistered = false;
  }  // else of if( uPrevHist < fvpsHistosFolder.size() )

  return true;
}

bool CbmMqHistoServer::ReceiveCanvasConfig(FairMQMessagePtr& msg,
                                           int /*index*/) {
  std::pair<std::string, std::string> tempObject;

  Deserialize<BoostSerializer<std::pair<std::string, std::string>>>(*msg,
                                                                    tempObject);

  LOG(info) << " Received configuration for canvas " << tempObject.first
            << " : " << tempObject.second;

  /// Check if canvas name already received in previous messages
  /// Linear search should be ok as config is shared only at startup
  uint32_t uPrevCanv = 0;
  for (uPrevCanv = 0; uPrevCanv < fvpsCanvasConfig.size(); ++uPrevCanv) {
    if (fvpsCanvasConfig[uPrevCanv].first == tempObject.first) break;
  }  // for( UInt_t uPrevCanv = 0; uPrevCanv < fvpsCanvasConfig.size(); ++uPrevCanv )

  if (uPrevCanv < fvpsCanvasConfig.size()) {
    LOG(warning) << " Ignored new configuration for histo " << tempObject.first
                 << " due to previously received one: " << tempObject.second;
    /// Not sure if we should return false here...
  }  // if( uPrevCanv < fvpsCanvasConfig.size() )
  else {
    fvpsCanvasConfig.push_back(tempObject);
    fvbCanvasReady.push_back(false);
    fbAllCanvasReady = false;

    fvCanvas.push_back(std::pair<TCanvas*, std::string>(nullptr, ""));
    fvbCanvasRegistered.push_back(false);
    fbAllCanvasRegistered = false;
  }  // else of if( uPrevCanv < fvpsCanvasConfig.size() )
  return true;
}

void CbmMqHistoServer::PreRun() {
  fStopThread = false;
  fThread     = std::thread(&CbmMqHistoServer::UpdateHttpServer, this);
}

void CbmMqHistoServer::UpdateHttpServer() {
  while (!fStopThread) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::lock_guard<std::mutex> lk(mtx);

    fServer->ProcessRequests();
  }
}

void CbmMqHistoServer::PostRun() {
  fStopThread = true;
  fThread.join();
}

template<class HistoT>
bool CbmMqHistoServer::ReadHistogram(HistoT* pHist) {
  int index1 = FindHistogram(pHist->GetName());
  if (-1 == index1) {
    HistoT* histogram_new = static_cast<HistoT*>(pHist->Clone());
    fArrayHisto.Add(histogram_new);

    LOG(info) << "Received new histo " << pHist->GetName();

    /// If new histo received, try to register it if configuration available
    if (!fbAllHistosRegistered) {
      for (uint32_t uHist = 0; uHist < fvpsHistosFolder.size(); ++uHist) {
        /// Jump histos already ready
        if (fvbHistoRegistered[uHist]) continue;

        /// Check if name matches one in config for others
        if (fvpsHistosFolder[uHist].first == histogram_new->GetName()) {
          fvHistos[uHist] = std::pair<TNamed*, std::string>(
            histogram_new, fvpsHistosFolder[uHist].second);
          fServer->Register(Form("/%s", fvHistos[uHist].second.data()),
                            fvHistos[uHist].first);
          fvbHistoRegistered[uHist] = true;

          LOG(info) << "registered histo " << fvHistos[uHist].first->GetName()
                    << " in folder " << fvHistos[uHist].second;


          /// Update flag telling whether all known histos are registered
          fbAllHistosRegistered = true;
          for (uint32_t uIdx = 0; uIdx < fvbHistoRegistered.size(); ++uIdx) {
            if (!fvbHistoRegistered[uIdx]) {
              fbAllHistosRegistered = false;
              break;
            }  // if( !fvbHistoRegistered[ uIdx ] )
          }  // for( uint32_t uIdx = 0; uIdx < fvbHistoRegistered.size(); ++uIdx )

          break;
        }  // if( fvpsHistosFolder[ uHist ].first == histogram_new->GetName() )
      }  // for( uint32_t uCanv = 0; uCanv < fvpsCanvasConfig.size(); ++uCanv )
    }    // if( !fbAllCanvasReady )
  }      // if (-1 == index1)
  else {
    HistoT* histogram_existing = dynamic_cast<HistoT*>(fArrayHisto.At(index1));
    if (nullptr == histogram_existing) {
      LOG(error) << "CbmMqHistoServer::ReadHistogram => "
                 << "Incompatible type found during update for histo "
                 << pHist->GetName();
      return false;
    }  // if( nullptr == histogram_existing )

    histogram_existing->Add(pHist);
  }  // else of if (-1 == index1)
  return true;
}

int CbmMqHistoServer::FindHistogram(const std::string& name) {
  for (int iHist = 0; iHist < fArrayHisto.GetEntriesFast(); ++iHist) {
    TObject* obj = fArrayHisto.At(iHist);
    if (TString(obj->GetName()).EqualTo(name)) {
      return iHist;
    }  // if( TString( obj->GetName() ).EqualTo( name ) )
  }    // for( int iHist = 0; iHist < fArrayHisto.GetEntriesFast(); ++iHist )
  return -1;
}

bool CbmMqHistoServer::ResetHistograms() {
  for (int iHist = 0; iHist < fArrayHisto.GetEntriesFast(); ++iHist) {
    dynamic_cast<TH1*>(fArrayHisto.At(iHist))->Reset();
  }  // for( int iHist = 0; iHist < fArrayHisto.GetEntriesFast(); ++iHist )
  return true;
}
bool CbmMqHistoServer::PrepareCanvas(uint32_t uCanvIdx) {
  CanvasConfig conf(
    ExtractCanvasConfigFromString(fvpsCanvasConfig[uCanvIdx].second));

  /// First check if all objects to be drawn are present
  uint32_t uNbPads(conf.GetNbPads());
  for (uint32_t uPadIdx = 0; uPadIdx < uNbPads; ++uPadIdx) {
    uint32_t uNbObj(conf.GetNbObjsInPad(uPadIdx));
    for (uint32_t uObjIdx = 0; uObjIdx < uNbObj; ++uObjIdx) {
      std::string sName(conf.GetObjName(uPadIdx, uObjIdx));
      /// Check for empty pads!
      if ("nullptr" != sName) {
        if (FindHistogram(sName) < 0) {
          return false;
        }  // if( FindHistogram( conf.GetObjName( uPadIdx, uObjIdx ) ) < 0 )
      }    // if( "nullptr" != sName )
    }      // for( uint32_t uObjIdx = 0; uObjIdx < uNbObj; ++uObjIdx )
  }        // for( uint32_t uPadIdx = 0; uPadIdx < uNbPads; ++uPadIdx )

  /// Create new canvas and pads
  TCanvas* pNewCanv =
    new TCanvas(conf.GetName().data(), conf.GetTitle().data());
  pNewCanv->Divide(conf.GetNbPadsX(), conf.GetNbPadsY());

  /// Loop on pads
  for (uint32_t uPadIdx = 0; uPadIdx < uNbPads; ++uPadIdx) {
    pNewCanv->cd(1 + uPadIdx);

    /// Pad settings
    gPad->SetGrid(conf.GetGridx(uPadIdx), conf.GetGridy(uPadIdx));
    gPad->SetLogx(conf.GetLogx(uPadIdx));
    gPad->SetLogy(conf.GetLogy(uPadIdx));
    gPad->SetLogz(conf.GetLogz(uPadIdx));

    /// Add objects (we know they are there
    uint32_t uNbObj(conf.GetNbObjsInPad(uPadIdx));
    for (uint32_t uObjIdx = 0; uObjIdx < uNbObj; ++uObjIdx) {
      std::string sName(conf.GetObjName(uPadIdx, uObjIdx));
      if ("nullptr" != sName) {
        TObject* pObj = fArrayHisto[FindHistogram(sName)];

        if (nullptr != dynamic_cast<TProfile*>(pObj)) {
          dynamic_cast<TProfile*>(pObj)->Draw(
            conf.GetOption(uPadIdx, uObjIdx).data());
        }  // if( nullptr != dynamic_cast< TProfile *>( pObj ) )
        else if (nullptr != dynamic_cast<TH2*>(pObj)) {
          dynamic_cast<TH2*>(pObj)->Draw(
            conf.GetOption(uPadIdx, uObjIdx).data());
        }  // if( nullptr != dynamic_cast< TH2 *>( pObj ) )
        else if (nullptr != dynamic_cast<TH1*>(pObj)) {
          dynamic_cast<TH1*>(pObj)->Draw(
            conf.GetOption(uPadIdx, uObjIdx).data());
        }  // if( nullptr != dynamic_cast< TH1 *>( pObj ) )
        else
          LOG(warning) << "Unsupported object type for " << sName
                       << " when preparing canvas " << conf.GetName();
      }  // if( "nullptr" != sName )
    }    // for( uint32_t uObjIdx = 0; uObjIdx < uNbObj; ++uObjIdx )
  }      // for( uint32_t uPadIdx = 0; uPadIdx < uNbPads; ++uPadIdx )

  fvCanvas[uCanvIdx] = std::pair<TCanvas*, std::string>(pNewCanv, "canvases");
  fServer->Register(Form("/%s", fvCanvas[uCanvIdx].second.data()),
                    fvCanvas[uCanvIdx].first);
  fvbCanvasRegistered[uCanvIdx] = true;

  LOG(info) << "registered canvas " << fvCanvas[uCanvIdx].first->GetName()
            << " in folder " << fvCanvas[uCanvIdx].second;

  /// Update flag telling whether all known canvases are registered
  fbAllCanvasRegistered = true;
  for (uint32_t uIdx = 0; uIdx < fvbCanvasRegistered.size(); ++uIdx) {
    if (!fvbCanvasRegistered[uIdx]) {
      fbAllCanvasRegistered = false;
      break;
    }  // if( !fvbCanvasRegistered[ uIdx ] )
  }    // for( uint32_t uIdx = 0; uIdx < fvbCanvasRegistered.size(); ++uIdx )

  return true;
}

bool CbmMqHistoServer::SaveHistograms() {
  /// (Re-)Create ROOT file to store the histos
  TDirectory* oldDir = NULL;
  TFile* histoFile   = NULL;
  // Store current directory position to allow restore later
  oldDir = gDirectory;
  // open separate histo file in recreate mode
  histoFile = new TFile(fsHistoFileName.data(), "RECREATE");

  if (nullptr == histoFile) return false;

  /// Register the histos in the HTTP server
  for (UInt_t uHisto = 0; uHisto < fvHistos.size(); ++uHisto) {
    /// Make sure we end up in chosen folder
    TString sFolder = fvHistos[uHisto].second.data();
    if (nullptr == gDirectory->Get(sFolder)) gDirectory->mkdir(sFolder);
    gDirectory->cd(sFolder);

    /// Write plot
    fvHistos[uHisto].first->Write();

    histoFile->cd();
  }  // for( UInt_t uHisto = 0; uHisto < fvHistos.size(); ++uHisto )

  for (UInt_t uCanvas = 0; uCanvas < fvCanvas.size(); ++uCanvas) {
    /// Make sure we end up in chosen folder
    TString sFolder = fvCanvas[uCanvas].second.data();
    if (nullptr == gDirectory->Get(sFolder)) gDirectory->mkdir(sFolder);
    gDirectory->cd(sFolder);

    /// Write plot
    fvCanvas[uCanvas].first->Write();

    histoFile->cd();
  }  // for( UInt_t uHisto = 0; uHisto < fvCanvas.size(); ++uHisto )

  // Restore original directory position
  oldDir->cd();
  histoFile->Close();

  return true;
}
