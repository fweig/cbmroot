/* Copyright (C) 2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

/**
 * CbmDeviceMcbmEventBuilderWin.h
 *
 * @since 2020-05-24
 * @author P.-A. Loizeau
 */

#ifndef CBMDEVICEMCBMEVTBUILDERWIN_H_
#define CBMDEVICEMCBMEVTBUILDERWIN_H_

/// CBM headers
#include "CbmMcbm2019TimeWinEventBuilderAlgo.h"
#include "CbmMqTMessage.h"
#include "CbmMuchBeamTimeDigi.h"
#include "CbmPsdDigi.h"
#include "CbmRichDigi.h"
#include "CbmStsDigi.h"
#include "CbmTofDigi.h"
#include "CbmTrdDigi.h"

/// FAIRROOT headers
#include "FairMQDevice.h"

/// FAIRSOFT headers (geant, boost, ...)
#include "Rtypes.h"
#include "TObjArray.h"

/// C/C++ headers
#include <chrono>
#include <map>
#include <vector>

class TList;
class TClonesArray;
class FairRunOnline;
class TimesliceMetaData;

class CbmDeviceMcbmEventBuilderWin : public FairMQDevice {
public:
  CbmDeviceMcbmEventBuilderWin();
  virtual ~CbmDeviceMcbmEventBuilderWin();

protected:
  virtual void InitTask();
  bool HandleData(FairMQParts&, int);
  bool HandleCommand(FairMQMessagePtr&, int);

private:
  /// Constants

  /// Control flags
  Bool_t fbIgnoreTsOverlap = kFALSE;  //! Ignore data in Overlap part of the TS
  Bool_t fbFillHistos      = kTRUE;   //! Switch ON/OFF filling of histograms

  /// User settings parameters
  /// Algo enum settings
  std::string fsEvtOverMode                = "NoOverlap";
  std::string fsRefDet                     = "kT0";
  std::vector<std::string> fvsAddDet       = {};
  std::vector<std::string> fvsDelDet       = {};
  std::vector<std::string> fvsSetTrigWin   = {};
  std::vector<std::string> fvsSetTrigMinNb = {};
  /// message queues
  std::string fsChannelNameDataInput    = "unpts_0";
  std::string fsChannelNameDataOutput   = "events";
  std::string fsChannelNameCommands     = "commands";
  std::string fsChannelNameHistosInput  = "histogram-in";
  std::string fsChannelNameHistosConfig = "histo-conf";
  std::string fsChannelNameCanvasConfig = "canvas-conf";
  /// Histograms management
  uint32_t fuPublishFreqTs  = 100;
  double_t fdMinPublishTime = 0.5;
  double_t fdMaxPublishTime = 5.0;

  /// List of MQ channels names
  std::vector<std::string> fsAllowedChannels = {fsChannelNameDataInput};

  /// Parameters management
  //      TList* fParCList = nullptr;
  //      Bool_t InitParameters( TList* fParCList );

  /// Statistics & first TS rejection
  uint64_t fulNumMessages                                = 0;
  uint64_t fulTsCounter                                  = 0;
  std::chrono::system_clock::time_point fLastPublishTime = std::chrono::system_clock::now();

  /// Processing algos
  CbmMcbm2019TimeWinEventBuilderAlgo* fpAlgo = nullptr;

  /// TS MetaData stable values storage
  size_t fuNbCoreMsPerTs    = 0;        //!
  size_t fuNbOverMsPerTs    = 0;        //!
  Double_t fdMsSizeInNs     = 1280000;  //! Size of a single MS, [nanoseconds]
  Double_t fdTsCoreSizeInNs = -1.0;     //! Total size of the core MS in a TS, [nanoseconds]
  Double_t fdTsOverSizeInNs = -1.0;     //! Total size of the overlap MS in a TS, [nanoseconds]
  Double_t fdTsFullSizeInNs = -1.0;     //! Total size of all MS in a TS, [nanoseconds]

  /// Data reception
  /// TS MetaData storage
  TClonesArray* fTimeSliceMetaDataArray = nullptr;  //!
  TimesliceMetaData* fTsMetaData        = nullptr;
  /// Digis storage
  std::vector<CbmTofDigi>* fvDigiT0            = nullptr;
  std::vector<CbmStsDigi>* fvDigiSts           = nullptr;
  std::vector<CbmMuchBeamTimeDigi>* fvDigiMuch = nullptr;
  std::vector<CbmTrdDigi>* fvDigiTrd           = nullptr;
  std::vector<CbmTofDigi>* fvDigiTof           = nullptr;
  std::vector<CbmRichDigi>* fvDigiRich         = nullptr;
  std::vector<CbmPsdDigi>* fvDigiPsd           = nullptr;
  /// Data emission
  TClonesArray* fEvents = nullptr;  //! output container of CbmEvents
  //      std::vector< CbmEvent * > &        fEventVector;    //! vector with all created events

  /// Internal data registration (for FairRootManager -> DigiManager links)
  FairRunOnline* fpRun = nullptr;

  /// Array of histograms to send to the histogram server
  TObjArray fArrayHisto = {};
  /// Vector of string pairs with ( HistoName, FolderPath ) to send to the histogram server
  std::vector<std::pair<std::string, std::string>> fvpsHistosFolder = {};
  /// Vector of string pairs with ( CanvasName, CanvasConfig ) to send to the histogram server
  /// Format of Can config is "NbPadX(U);NbPadY(U);ConfigPad1(s);....;ConfigPadXY(s)"
  /// Format of Pad config is "GrixX(b),GridY(b),LogX(b),LogY(b),LogZ(b),HistoName(s),DrawOptions(s)"
  std::vector<std::pair<std::string, std::string>> fvpsCanvasConfig = {};

  bool IsChannelNameAllowed(std::string channelName);
  //      Bool_t InitContainers();
  void Finish();
  bool SendEvents(FairMQParts& partsIn);
  bool SendHistograms();
};

#endif /* CBMDEVICEMCBMEVTBUILDERWIN_H_ */
