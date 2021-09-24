/* Copyright (C) 2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

/**
 * CbmDeviceMcbmMonitorPulser.h
 *
 * @since 2020-05-04
 * @author P.-A Loizeau
 */

#ifndef CBMDEVICEMCBMMONITORPULSER_H_
#define CBMDEVICEMCBMMONITORPULSER_H_

#include "CbmMuchBeamTimeDigi.h"
#include "CbmPsdDigi.h"
#include "CbmRichDigi.h"
#include "CbmStsDigi.h"
#include "CbmTofDigi.h"
#include "CbmTrdDigi.h"

#include "FairMQDevice.h"

#include "Rtypes.h"
#include "TMessage.h"
#include "TObjArray.h"

#include <chrono>
#include <map>
#include <vector>

class TH1;
class TH2;
class TProfile;
class TList;
//class CbmMcbm2018MonitorAlgoTof;
class TimesliceMetaData;

class CbmDeviceMcbmMonitorPulser : public FairMQDevice {
public:
  CbmDeviceMcbmMonitorPulser();
  virtual ~CbmDeviceMcbmMonitorPulser();

protected:
  virtual void InitTask();
  bool HandleData(FairMQParts&, int);

private:
  /// Constants
  /*********************** SHOULD GO IN ALGO ****************************/
  static const UInt_t kuNbChanSMX      = 128;
  static const UInt_t kuMaxNbStsDpbs   = 2;
  static const UInt_t kuMaxNbMuchDpbs  = 6;
  static const UInt_t kuMaxNbMuchAsics = 36;
  static const UInt_t kuDefaultAddress = 0xFFFFFFFF;
  static const UInt_t kuMaxChannelSts  = 3000;
  /*********************** SHOULD GO IN ALGO ****************************/

  /// Control flags
  Bool_t fbDebugMonitorMode      = kFALSE;  //! Switch ON the filling of a additional set of histograms
  Bool_t fbIgnoreCriticalErrors  = kTRUE;   //! If ON not printout at all for critical errors
  Bool_t fbComponentsAddedToList = kFALSE;

  /// User settings parameters
  std::string fsChannelNameDataInput    = "unpts_0";
  std::string fsChannelNameCommands     = "commands";
  std::string fsChannelNameHistosInput  = "histogram-in";
  std::string fsChannelNameHistosConfig = "histo-conf";
  std::string fsChannelNameCanvasConfig = "canvas-conf";
  uint32_t fuHistoryHistoSize           = 3600;
  uint32_t fuMinTotPulser               = 185;
  uint32_t fuMaxTotPulser               = 195;
  uint32_t fuPublishFreqTs              = 100;
  double_t fdMinPublishTime             = 0.5;
  double_t fdMaxPublishTime             = 5.0;

  /// List of MQ channels names
  std::vector<std::string> fsAllowedChannels = {fsChannelNameDataInput};

  /// Parameters management
  /*
      TList* fParCList = nullptr;
*/

  /// Statistics & first TS rejection
  uint64_t fulNumMessages                                = 0;
  uint64_t fulTsCounter                                  = 0;
  std::chrono::system_clock::time_point fLastPublishTime = std::chrono::system_clock::now();

  /// Data reception
  /// TS MetaData storage
  TimesliceMetaData* fTsMetaData = nullptr;
  /// Digis storage
  std::vector<CbmTofDigi> fvDigiT0            = {};
  std::vector<CbmStsDigi> fvDigiSts           = {};
  std::vector<CbmMuchBeamTimeDigi> fvDigiMuch = {};
  std::vector<CbmTrdDigi> fvDigiTrd           = {};
  std::vector<CbmTofDigi> fvDigiTof           = {};
  std::vector<CbmRichDigi> fvDigiRich         = {};
  std::vector<CbmPsdDigi> fvDigiPsd           = {};

  /// Processing algo
  //      CbmMcbm2018MonitorAlgoTof * fMonitorAlgo;

  /// Array of histograms to send to the histogram server
  TObjArray fArrayHisto = {};
  /// Vector of string pairs with ( HistoName, FolderPath ) to send to the histogram server
  std::vector<std::pair<std::string, std::string>> fvpsHistosFolder = {};
  /// Vector of string pairs with ( CanvasName, CanvasConfig ) to send to the histogram server
  /// Format of Can config is "NbPadX(U);NbPadY(U);ConfigPad1(s);....;ConfigPadXY(s)"
  /// Format of Pad config is "GrixX(b),GridY(b),LogX(b),LogY(b),LogZ(b),HistoName(s),DrawOptions(s)"
  std::vector<std::pair<std::string, std::string>> fvpsCanvasConfig = {};

  bool IsChannelNameAllowed(std::string channelName);
  Bool_t InitContainers();
  void Finish();
  bool SendHistograms();

  /*********************** SHOULD GO IN ALGO ****************************/
  void CheckInterSystemOffset();

  template<class Digi>
  Int_t FillSystemOffsetHistos(TH1* histo, TH2* histoEvo, TH2* histoEvoLong, TProfile* profMeanEvo, TH2* histoAFCK,
                               const Double_t T0Time, const Int_t offsetRange, Int_t iStartDigi,
                               ECbmModuleId iDetId = ECbmModuleId::kLastModule);

  Int_t CalcNrBins(Int_t);
  void CreateHistos();

  /// Variables to store the previous digi time
  Double_t fPrevTimeT0   = 0.;
  Double_t fPrevTimeSts  = 0.;
  Double_t fPrevTimeMuch = 0.;
  Double_t fPrevTimeTrd  = 0.;
  Double_t fPrevTimeTof  = 0.;
  Double_t fPrevTimeRich = 0.;
  Double_t fPrevTimePsd  = 0.;

  /// Variables to store the first digi fitting the previous T0 hits
  /// => Time-order means the time window for following one can only be in a later digi
  Int_t fPrevT0FirstDigiSts  = 0;
  Int_t fPrevT0FirstDigiMuch = 0;
  Int_t fPrevT0FirstDigiTrd  = 0;
  Int_t fPrevT0FirstDigiTof  = 0;
  Int_t fPrevT0FirstDigiRich = 0;
  Int_t fPrevT0FirstDigiPsd  = 0;

  /// User settings: Data correction parameters
  /// Charge cut
  UInt_t fuMinTotPulserT0     = 182;
  UInt_t fuMaxTotPulserT0     = 190;
  UInt_t fuMinAdcPulserSts    = 90;
  UInt_t fuMaxAdcPulserSts    = 100;
  UInt_t fuMinAdcPulserMuch   = 5;
  UInt_t fuMaxAdcPulserMuch   = 15;
  UInt_t fuMinChargePulserTrd = 0;
  UInt_t fuMaxChargePulserTrd = 70000;
  UInt_t fuMinTotPulserTof    = 182;
  UInt_t fuMaxTotPulserTof    = 190;
  UInt_t fuMinTotPulserRich   = 90;
  UInt_t fuMaxTotPulserRich   = 105;
  UInt_t fuMinAdcPulserPsd    = 90;
  UInt_t fuMaxAdcPulserPsd    = 100;
  /// Channel selection
  UInt_t fuStsAddress   = kuDefaultAddress;
  UInt_t fuStsFirstCha  = kuMaxChannelSts;
  UInt_t fuStsLastChan  = kuMaxChannelSts;
  UInt_t fuMuchAsic     = kuMaxNbMuchAsics;
  UInt_t fuMuchFirstCha = kuNbChanSMX;
  UInt_t fuMuchLastChan = kuNbChanSMX;
  UInt_t fuTrdAddress   = kuDefaultAddress;
  UInt_t fuPsdAddress   = kuDefaultAddress;

  //
  Int_t fNrTs = 0;

  Int_t fOffsetRange     = 1000;
  Int_t fStsOffsetRange  = 1000;
  Int_t fMuchOffsetRange = 1000;
  Int_t fTrdOffsetRange  = 1000;
  Int_t fTofOffsetRange  = 1000;
  Int_t fRichOffsetRange = 1000;
  Int_t fPsdOffsetRange  = 1000;

  Int_t fBinWidth = 1;

  TH1* fT0StsDiff       = nullptr;
  TH1* fT0MuchDiff      = nullptr;
  TH1* fT0TrdDiff       = nullptr;
  TH1* fT0TofDiff       = nullptr;
  TH1* fT0RichDiff      = nullptr;
  TH1* fT0PsdDiff       = nullptr;
  TH2* fT0PsdDiffCharge = nullptr;

  TH2* fT0StsDiffEvo  = nullptr;
  TH2* fT0MuchDiffEvo = nullptr;
  TH2* fT0TrdDiffEvo  = nullptr;
  TH2* fT0TofDiffEvo  = nullptr;
  TH2* fT0RichDiffEvo = nullptr;
  TH2* fT0PsdDiffEvo  = nullptr;

  TH2* fT0StsDiffEvoLong  = nullptr;
  TH2* fT0MuchDiffEvoLong = nullptr;
  TH2* fT0TrdDiffEvoLong  = nullptr;
  TH2* fT0TofDiffEvoLong  = nullptr;
  TH2* fT0RichDiffEvoLong = nullptr;
  TH2* fT0PsdDiffEvoLong  = nullptr;

  Double_t fdStartTime     = -1;
  TProfile* fT0StsMeanEvo  = nullptr;
  TProfile* fT0MuchMeanEvo = nullptr;
  TProfile* fT0TrdMeanEvo  = nullptr;
  TProfile* fT0TofMeanEvo  = nullptr;
  TProfile* fT0RichMeanEvo = nullptr;
  TProfile* fT0PsdMeanEvo  = nullptr;

  TH1* fT0T0Diff     = nullptr;
  TH1* fStsStsDiff   = nullptr;
  TH1* fMuchMuchDiff = nullptr;
  TH1* fTrdTrdDiff   = nullptr;
  TH1* fTofTofDiff   = nullptr;
  TH1* fRichRichDiff = nullptr;
  TH1* fPsdPsdDiff   = nullptr;

  TH2* fT0StsNb  = nullptr;
  TH2* fT0MuchNb = nullptr;
  TH2* fT0TrdNb  = nullptr;
  TH2* fT0TofNb  = nullptr;
  TH2* fT0RichNb = nullptr;
  TH2* fT0PsdNb  = nullptr;

  Int_t fiT0Nb   = 0;
  Int_t fiStsNb  = 0;
  Int_t fiMuchNb = 0;
  Int_t fiTrdNb  = 0;
  Int_t fiTofNb  = 0;
  Int_t fiRichNb = 0;
  Int_t fiPsdNb  = 0;

  TH1* fT0Address = nullptr;
  TH1* fT0Channel = nullptr;

  TH2* fT0StsDpbDiff = nullptr;
  TH2* fT0StsDpbDiffEvo[kuMaxNbStsDpbs];
  TH1* fStsDpbCntsEvo[kuMaxNbStsDpbs];

  TH2* fT0MuchRocDiff  = nullptr;
  TH2* fT0MuchAsicDiff = nullptr;
  TH2* fT0MuchAsicDiffEvo[kuMaxNbMuchAsics];

  TH2* fDigisPerAsicEvo = nullptr;
  Double_t fdLastMuchDigi[kuMaxNbMuchAsics][kuNbChanSMX];
  Double_t fdLastMuchDigiPulser[kuMaxNbMuchAsics][kuNbChanSMX];
  TH2* fSameChanDigisDistEvo = nullptr;

  Double_t fdLastT0DigiPulser = 0;

  TH2* fDigiTimeEvoT0   = nullptr;
  TH2* fDigiTimeEvoSts  = nullptr;
  TH2* fDigiTimeEvoMuch = nullptr;
  TH2* fDigiTimeEvoTof  = nullptr;
  /*********************** SHOULD GO IN ALGO ****************************/
};

// special class to expose protected TMessage constructor
class CbmMQTMessage : public TMessage {
public:
  CbmMQTMessage(void* buf, Int_t len) : TMessage(buf, len) { ResetBit(kIsOwner); }
};


#endif /* CBMDEVICEMCBMMONITORPULSER_H_ */
