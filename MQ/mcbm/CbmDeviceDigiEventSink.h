/* Copyright (C) 2020-2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

/**
 * CbmDeviceDigiEventSink.h
 *
 * @since 2020-05-04
 * @author P.-A. Loizeau
 */

#ifndef CBMDEVICEDIGIEVTSINK_H_
#define CBMDEVICEDIGIEVTSINK_H_

/// CBM headers
#include "CbmDigiEvent.h"
#include "CbmEvent.h"
#include "CbmMqTMessage.h"
#include "CbmMuchDigi.h"
#include "CbmPsdDigi.h"
#include "CbmRichDigi.h"
#include "CbmStsDigi.h"
#include "CbmTofDigi.h"
#include "CbmTrdDigi.h"
#include "CbmTsEventHeader.h"

#include "TimesliceMetaData.h"

/// FAIRROOT headers
#include "FairMQDevice.h"

/// FAIRSOFT headers (geant, boost, ...)
#include "Rtypes.h"
#include "TClonesArray.h"
#include "TObjArray.h"

/// C/C++ headers
#include <chrono>
#include <map>
#include <vector>

class TCanvas;
class TFile;
class TH1;
class TProfile;
class TList;
class TClonesArray;
//class TimesliceMetaData;
class FairRunOnline;
class FairRootManager;

class CbmEventTimeslice {
  /// TODO: rename to CbmTsWithEvents
public:
  CbmEventTimeslice(FairMQParts& parts, bool bDigiEvtInput = false);
  ~CbmEventTimeslice();

  void ExtractSelectedData();
  std::vector<CbmDigiEvent>& GetSelectedData(){
    if (!fbDigiEvtInput) ExtractSelectedData();
    return fvDigiEvents;
  }

  /// Input Type
  bool fbDigiEvtInput = false;
  /// TS information in header
  CbmTsEventHeader fCbmTsEventHeader;
  /// Raw data
  std::vector<CbmTofDigi> fvDigiT0;
  std::vector<CbmStsDigi> fvDigiSts;
  std::vector<CbmMuchDigi> fvDigiMuch;
  std::vector<CbmTrdDigi> fvDigiTrd;
  std::vector<CbmTofDigi> fvDigiTof;
  std::vector<CbmRichDigi> fvDigiRich;
  std::vector<CbmPsdDigi> fvDigiPsd;
  /// extra Metadata
  TimesliceMetaData fTsMetaData;
  /// Raw events
  std::vector<CbmEvent> fvEvents;
  /// Digi events
  std::vector<CbmDigiEvent> fvDigiEvents;
};

class CbmDeviceDigiEventSink : public FairMQDevice {
public:
  CbmDeviceDigiEventSink();
  virtual ~CbmDeviceDigiEventSink();

protected:
  virtual void InitTask();
  bool HandleMissTsData(FairMQMessagePtr&, int);
  bool HandleData(FairMQParts&, int);
  bool HandleCommand(FairMQMessagePtr&, int);

private:
  /// Constants

  /// Control flags
  bool fbStoreFullTs         = false;  //! If true, store digis vectors with full TS in addition to selected events
  bool fbBypassConsecutiveTs = false;  //! Switch ON/OFF the bypass of the consecutive TS buffer before writing to file
  bool fbWriteMissingTs      = false;  //! Switch ON/OFF writing of empty TS to file for the missing ones (if no bypass)
  bool fbDisableCompression  = false;  //! Switch ON/OFF the ROOT file compression
  bool fbDigiEventInput      = false;  //! Switch ON/OFF the input of CbmDigiEvents instead of raw data + CbmEvents
  bool fbFillHistos          = false;  //! Switch ON/OFF filling of histograms
  bool fbInitDone            = false;  //! Keep track of whether the Init was already fully completed
  bool fbFinishDone          = false;  //! Keep track of whether the Finish was already called

  /// User settings parameters
  /// Algo enum settings
  std::string fsOutputFileName = "mcbm_digis_events.root";
  /// message queues
  std::string fsChannelNameMissedTs    = "missedts";
  std::string fsChannelNameDataInput   = "events";
  std::string fsChannelNameCommands    = "commands";
  std::string fsChannelNameHistosInput = "histogram-in";
  /// Output file/tree management
  int64_t fiTreeFileMaxSize = 10000000000LL;  //! Default value: ~10 GB
  /// Histograms management
  uint32_t fuPublishFreqTs  = 100;
  double_t fdMinPublishTime = 0.5;
  double_t fdMaxPublishTime = 5.0;

  /// List of MQ channels names
  std::vector<std::string> fsAllowedChannels = {fsChannelNameDataInput};

  /// Parameters management
  //      TList* fParCList = nullptr;
  //      Bool_t InitParameters( TList* fParCList );

  /// Statistics & missed TS detection
  uint64_t fuPrevTsIndex                                 = 0;
  uint64_t fulNumMessages                                = 0;
  uint64_t fulTsCounter                                  = 0;
  uint64_t fulMissedTsCounter                            = 0;
  uint64_t fulProcessedEvents                            = 0;
  uint64_t fulLastFullTsCounter                          = 0;
  uint64_t fulLastMissTsCounter                          = 0;
  uint64_t fulLastProcessedEvents                        = 0;
  std::chrono::system_clock::time_point fLastPublishTime = std::chrono::system_clock::now();
  std::chrono::system_clock::time_point fLastFillTime    = std::chrono::system_clock::now();
  std::chrono::system_clock::time_point fStartTime       = std::chrono::system_clock::now();

  /// Control Commands reception
  bool fbReceivedEof      = false;
  uint64_t fuLastTsIndex  = 0;
  uint64_t fuTotalTsCount = 0;

  /// Data reception
  /// Event (TS) header
  CbmTsEventHeader* fEvtHeader = nullptr;
  /// TS MetaData storage
  TClonesArray* fTimeSliceMetaDataArray = nullptr;  //!
  TimesliceMetaData* fTsMetaData        = nullptr;
  /// CbmEvents
  std::vector<CbmDigiEvent>* fEventsSel = nullptr;  //! output container of CbmEvents
  /// Full TS Digis storage (optional usage, controlled by fbStoreFullTs!)
  std::vector<CbmTofDigi>* fvDigiT0    = nullptr;
  std::vector<CbmStsDigi>* fvDigiSts   = nullptr;
  std::vector<CbmMuchDigi>* fvDigiMuch = nullptr;
  std::vector<CbmTrdDigi>* fvDigiTrd   = nullptr;
  std::vector<CbmTofDigi>* fvDigiTof   = nullptr;
  std::vector<CbmRichDigi>* fvDigiRich = nullptr;
  std::vector<CbmPsdDigi>* fvDigiPsd   = nullptr;

  /// Storage for re-ordering
  /// Missed TS vector
  std::vector<uint64_t> fvulMissedTsIndices = {};
  /// Buffered TS
  std::map<uint64_t, CbmEventTimeslice> fmFullTsStorage = {};

  /// Data storage
  FairRunOnline* fpRun           = nullptr;
  FairRootManager* fpFairRootMgr = nullptr;

  /// Array of histograms to send to the histogram server
  TObjArray fArrayHisto = {};
  /// Vector of string pairs with ( HistoName, FolderPath ) to send to the histogram server
  std::vector<std::pair<std::string, std::string>> fvpsHistosFolder = {};
  /// Vector of string pairs with ( CanvasName, CanvasConfig ) to send to the histogram server
  /// Format of Can config is "NbPadX(U);NbPadY(U);ConfigPad1(s);....;ConfigPadXY(s)"
  /// Format of Pad config is "GrixX(b),GridY(b),LogX(b),LogY(b),LogZ(b),HistoName(s),DrawOptions(s)"
  std::vector<std::pair<std::string, std::string>> fvpsCanvasConfig = {};
  /// Flag indicating whether the histograms and canvases configurations were already published
  bool fbConfigSent = false;

  TProfile* fhFullTsBuffSizeEvo;
  TProfile* fhMissTsBuffSizeEvo;
  TH1* fhFullTsProcEvo;
  TH1* fhMissTsProcEvo;
  TH1* fhTotalTsProcEvo;
  TH1* fhTotalEventsEvo;
  TCanvas* fcEventSinkAllHist;

  /// Internal methods
  bool IsChannelNameAllowed(std::string channelName);
  bool InitHistograms();
  bool ResetHistograms(bool bResetStartTime = false);
  void CheckTsQueues();
  void PrepareTreeEntry(CbmEventTimeslice unpTs);
  void DumpTreeEntry();
  bool SendHistoConfAndData();
  bool SendHistograms();
  void Finish();
};

#endif /* CBMDEVICEDIGIEVTSINK_H_ */
