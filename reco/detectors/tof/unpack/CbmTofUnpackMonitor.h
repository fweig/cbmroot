/* Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer]  */

#ifndef CbmTofUnpackMonitor_H
#define CbmTofUnpackMonitor_H

#include "CbmMcbm2018TofPar.h"

#include "Rtypes.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TProfile2D.h"

#include <cstdint>

class TCanvas;

class CbmTofUnpackMonitor {
public:
  CbmTofUnpackMonitor();

  virtual ~CbmTofUnpackMonitor();

  /** @brief Init all required parameter informations and histograms */
  Bool_t Init(CbmMcbm2018TofPar* digiParSet);

  Bool_t CreateHistograms();
  Bool_t ResetHistograms();
  void DrawCanvases();

  Bool_t CreateBmonHistograms();
  Bool_t ResetBmonHistograms(Bool_t bResetTime);
  void DrawBmonCanvases();

  Bool_t CreateMsComponentSizeHistos(UInt_t component);
  Bool_t ResetMsComponentSizeHistos(UInt_t component);

  /** @brief Write all histograms and canvases to file */
  void Finish();

  void SetHistoFileName(TString nameIn) { fHistoFileName = nameIn; }

  inline void SetPulserTotLimits(UInt_t uMin, UInt_t uMax)
  {
    fuMinTotPulser = uMin;
    fuMaxTotPulser = uMax;
  }
  inline void SetSpillThreshold(UInt_t uCntLimit) { fuOffSpillCountLimit = uCntLimit; }
  inline void SetSpillThresholdNonPulser(UInt_t uCntLimit) { fuOffSpillCountLimitNonPulser = uCntLimit; }
  inline void SetSpillCheckInterval(Double_t dIntervalSec) { fdSpillCheckInterval = dIntervalSec; }
  inline void SetBmonChannelMap(UInt_t uChan0, UInt_t uChan1, UInt_t uChan2, UInt_t uChan3, UInt_t uChan4,
                                UInt_t uChan5, UInt_t uChan6, UInt_t uChan7)
  {
    fuBmonChanMap[0] = uChan0;
    fuBmonChanMap[1] = uChan1;
    fuBmonChanMap[2] = uChan2;
    fuBmonChanMap[3] = uChan3;
    fuBmonChanMap[4] = uChan4;
    fuBmonChanMap[5] = uChan5;
    fuBmonChanMap[6] = uChan6;
    fuBmonChanMap[7] = uChan7;
  }

  void SetLongDurationLimits(UInt_t uDurationSeconds, UInt_t uBinSize)
  {
    //fbLongHistoEnable     = kTRUE;
    fuLongHistoNbSeconds  = uDurationSeconds;
    fuLongHistoBinSizeSec = uBinSize;
  }

  void AddHistoToVector(TNamed* pointer, std::string sFolder = "")
  {
    fvpAllHistoPointers.push_back(std::pair<TNamed*, std::string>(pointer, sFolder));
  }
  std::vector<std::pair<TNamed*, std::string>> GetHistoVector() { return fvpAllHistoPointers; }

  void AddCanvasToVector(TCanvas* pointer, std::string sFolder = "")
  {
    fvpAllCanvasPointers.push_back(std::pair<TCanvas*, std::string>(pointer, sFolder));
  }
  std::vector<std::pair<TCanvas*, std::string>> GetCanvasVector() { return fvpAllCanvasPointers; }

  UInt_t GetMaxNbFlibLinks() { return kuMaxNbFlibLinks; }

  /// Fill Ms Component Size Histos
  void FillMsSize(UInt_t uMsComp, UInt_t uSize) { fvhMsSize[uMsComp]->Fill(uSize); }
  void FillMsSizeTime(UInt_t uMsComp, Double_t dTime, UInt_t uSize) { fvhMsSizeTime[uMsComp]->Fill(dTime, uSize); }


  /// Fill general histograms
  //  void FillMsCntEvo(ULong64_t MsIdx) { fhMsCntEvo->Fill(MsIdx * 1e-9); }
  void FillHitMonitoringHistos(const double_t& dMsTime, const uint32_t& uCurrCompIdx, const uint32_t& uGet4Id,
                               const uint32_t& uRawCh, const uint32_t& uRemapCh, const uint32_t& uTot);
  void FillEpochMonitoringHistos(const uint32_t& uCurrCompIdx, const uint32_t& uGet4Id, const bool& bSyncFlag,
                                 const bool& bDataLoss, const bool& bEpochLoss, const bool& bMissmMatch);
  void FillScmMonitoringHistos(const uint32_t& uCurrCompIdx, const uint32_t& uGet4Id, const uint32_t& uCh,
                               const uint32_t& uEdge, const uint32_t& uType);
  void FillSysMonitoringHistos(const uint32_t& uCurrCompIdx, const uint32_t& uGet4Id, const uint32_t& uType);
  void FillErrMonitoringHistos(const uint32_t& uCurrCompIdx, const uint32_t& uGet4Id, const uint32_t& uCh,
                               const uint32_t& uType);

  /// Fill BMon histograms
  void CheckBmonSpillLimits(const double_t& dMsTime);
  void FillHitBmonMonitoringHistos(const double_t& dMsTime, const uint32_t& uCurrCompIdx, const uint32_t& uGet4Id,
                                   const uint32_t& uTot);

  void FillErrBmonMonitoringHistos(const double_t& dMsTime, const uint32_t& uCurrCompIdx, const uint32_t& uGet4Id,
                                   const bool& bErrEvtLost);

  /** @brief Activate the BMon mode */
  void SetBmonMode(bool value) { fBmonMode = value; }

  /** @brief Read the Bmon mode */
  bool GetBmonMode() { return fBmonMode; }

private:
  TString fHistoFileName = "data/mon.tof.root";

  /// Settings from parameter file
  CbmMcbm2018TofPar* fUnpackPar = nullptr;
  /// Readout chain dimensions and mapping
  UInt_t fuNbOfComps           = 0;  //! Total number of Components in the system
  UInt_t fuNbOfGet4PerComp     = 0;  //! Max number of Get4 per component
  UInt_t fuNbOfChannelsPerGet4 = 0;  //! Number of channels per Get4, constant
  UInt_t fuNbOfChannelsPerComp = 0;  //! Number of channels per Component, recalculated
  UInt_t fuNbOfGet4InSyst      = 0;  //! Total/maximum number of Get4 in system

  /// Rate evolution histos
  Double_t fdStartTime = -1.0; /** Time of first valid hit (epoch available), used as reference for evolution plots**/
  UInt_t fuHistoryHistoSize  = 3600; /** Size in seconds of the evolution histograms **/
  double_t dFirstTsStartTime = 0;
  //Bool_t fbLongHistoEnable;
  UInt_t fuLongHistoNbSeconds  = 3600;
  UInt_t fuLongHistoBinSizeSec = 10;
  UInt_t fuLongHistoBinNb      = 1;

  /** @brief Flag if debug mode is active or not */
  bool fBmonMode = false;

  /// ---> Constants
  static const UInt_t kuMaxNbFlibLinks  = 32;
  static const UInt_t kuBytesPerMessage = 8;
  static const UInt_t kuNbChanBmon      = 16;
  static const UInt_t kuNbSpillPlots    = 5;

  /// ---> User settings: Data correction parameters
  UInt_t fuMinTotPulser                = 185;
  UInt_t fuMaxTotPulser                = 189;
  UInt_t fuOffSpillCountLimit          = 200;
  UInt_t fuOffSpillCountLimitNonPulser = 80;
  Double_t fdSpillCheckInterval        = 1.0;
  /// Map from electronics channel to BMon strip
  /// 2022 mapping: Y[0-3] on c0, Y[4-7] on c1, X[0-3] on c2, X[4-7] on c3
  /// Y not cabled for diamond but pulser there
  UInt_t fuBmonChanMap[kuNbChanBmon] = {8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7};

  /// MS size histograms
  TH1* fvhMsSize[kuMaxNbFlibLinks];
  TProfile* fvhMsSizeTime[kuMaxNbFlibLinks];

  /// TOF system general histograms
  /// ---> Per GET4 in system
  TH2* fhGet4MessType    = nullptr;
  TH2* fhGet4EpochFlags  = nullptr;
  TH2* fhGet4ScmType     = nullptr;
  TH2* fhGet4SysMessType = nullptr;
  TH2* fhGet4ErrorsType  = nullptr;
  /// ---> Per GET4 in Component
  std::vector<TH2*> fvhCompGet4MessType = {};
  std::vector<TH2*> fvhCompGet4ChScm    = {};
  std::vector<TH2*> fvhCompGet4ChErrors = {};
  /// ---> Per raw channel in Component
  std::vector<TH1*> fvhCompRawChCount = {};
  std::vector<TH2*> fvhCompRawChRate  = {};
  std::vector<TH2*> fvhCompRawChTot   = {};
  /// ---> Per remapped (PADI) channel in Component
  std::vector<TH1*> fvhCompRemapChCount = {};
  std::vector<TH2*> fvhCompRemapChRate  = {};
  std::vector<TH2*> fvhCompRemapChTot   = {};
  /// ---> Pulser
  TH1* fhPulserChCounts = nullptr;
  TH2* fhPulserChEvo    = nullptr;

  /// BMon specific histograms
  /// ---> Channel rate plots
  std::vector<UInt_t> fvuBmonHitCntChanMs           = std::vector<UInt_t>(kuNbChanBmon, 0);
  std::vector<UInt_t> fvuBmonErrorCntChanMs         = std::vector<UInt_t>(kuNbChanBmon, 0);
  std::vector<UInt_t> fvuBmonEvtLostCntChanMs       = std::vector<UInt_t>(kuNbChanBmon, 0);
  std::vector<TH1*> fvhBmonMsgCntEvoChan            = std::vector<TH1*>(kuNbChanBmon, nullptr);
  std::vector<TH2*> fvhBmonMsgCntPerMsEvoChan       = std::vector<TH2*>(kuNbChanBmon, nullptr);
  std::vector<TH1*> fvhBmonHitCntEvoChan            = std::vector<TH1*>(kuNbChanBmon, nullptr);
  std::vector<TH2*> fvhBmonHitCntPerMsEvoChan       = std::vector<TH2*>(kuNbChanBmon, nullptr);
  std::vector<TH1*> fvhBmonErrorCntEvoChan          = std::vector<TH1*>(kuNbChanBmon, nullptr);
  std::vector<TH2*> fvhBmonErrorCntPerMsEvoChan     = std::vector<TH2*>(kuNbChanBmon, nullptr);
  std::vector<TH1*> fvhBmonEvtLostCntEvoChan        = std::vector<TH1*>(kuNbChanBmon, nullptr);
  std::vector<TH2*> fvhBmonEvtLostCntPerMsEvoChan   = std::vector<TH2*>(kuNbChanBmon, nullptr);
  std::vector<TProfile*> fvhBmonErrorFractEvoChan   = std::vector<TProfile*>(kuNbChanBmon, nullptr);
  std::vector<TH2*> fvhBmonErrorFractPerMsEvoChan   = std::vector<TH2*>(kuNbChanBmon, nullptr);
  std::vector<TProfile*> fvhBmonEvtLostFractEvoChan = std::vector<TProfile*>(kuNbChanBmon, nullptr);
  std::vector<TH2*> fvhBmonEvtLostFractPerMsEvoChan = std::vector<TH2*>(kuNbChanBmon, nullptr);
  /// ---> Channels maps without cuts
  TH1* fhBmonCompMapAll       = nullptr;
  TH2* fhBmonCompGet4         = nullptr;
  TH1* fhBmonGet4Map          = nullptr;
  TH2* fhBmonGet4MapEvo       = nullptr;
  TH1* fhBmonChannelMapAll    = nullptr;
  TH2* fhBmonChannelTotAll    = nullptr;
  TH2* fhBmonHitMapEvoAll     = nullptr;
  TH2* fhBmonHitTotEvoAll     = nullptr;
  TH1* fhBmonChanHitMapAll    = nullptr;
  TH2* fhBmonChanHitMapEvoAll = nullptr;
  /// ---> No Pulser cut
  TH1* fhBmonCompMap       = nullptr;
  TH1* fhBmonChannelMap    = nullptr;
  TH2* fhBmonHitMapEvo     = nullptr;
  TH2* fhBmonHitTotEvo     = nullptr;
  TH1* fhBmonChanHitMap    = nullptr;
  TH2* fhBmonChanHitMapEvo = nullptr;
  /// ---> No Pulser cut + Spill detection
  std::vector<TH1*> fvhBmonCompMapSpill    = {};
  std::vector<TH1*> fvhBmonChannelMapSpill = {};
  TH1* fhBmonHitsPerSpill                  = nullptr;
  /// ---> Global Rate
  TH1* fhBmonMsgCntEvo            = nullptr;
  TH1* fhBmonHitCntEvo            = nullptr;
  TH1* fhBmonErrorCntEvo          = nullptr;
  TH1* fhBmonLostEvtCntEvo        = nullptr;
  TProfile* fhBmonErrorFractEvo   = nullptr;
  TProfile* fhBmonLostEvtFractEvo = nullptr;
  /// ---> Global Rate per MS
  TH2* fhBmonMsgCntPerMsEvo       = nullptr;
  TH2* fhBmonHitCntPerMsEvo       = nullptr;
  TH2* fhBmonErrorCntPerMsEvo     = nullptr;
  TH2* fhBmonLostEvtCntPerMsEvo   = nullptr;
  TH2* fhBmonErrorFractPerMsEvo   = nullptr;
  TH2* fhBmonLostEvtFractPerMsEvo = nullptr;
  /// ---> Pulser
  TH1* fhBmonChannelMapPulser = nullptr;
  TH2* fhBmonHitMapEvoPulser  = nullptr;

  /// Canvases
  /// ---> Generic
  TCanvas* fcSummaryGet4s          = nullptr;
  std::vector<TCanvas*> fvcSumComp = {};
  /// ---> BMon
  TCanvas* fcBmonSummary             = nullptr;
  TCanvas* fcBmonSummaryMap          = nullptr;
  TCanvas* fcBmonHitMaps             = nullptr;
  TCanvas* fcBmonGenCntsPerMs        = nullptr;
  TCanvas* fcBmonSpillCounts         = nullptr;
  TCanvas* fcBmonSpillCountsHori     = nullptr;
  TCanvas* fcBmonSpillCompCountsHori = nullptr;

  /// Spill detection
  Bool_t fbSpillOn                      = kTRUE;
  UInt_t fuCurrentSpillIdx              = 0;
  UInt_t fuCurrentSpillPlot             = 0;
  Double_t fdStartTimeSpill             = -1.0;
  Double_t fdBmonLastInterTime          = -1.0;
  UInt_t fuBmonCountsLastInter          = 0;
  UInt_t fuBmonNonPulserCountsLastInter = 0;

  /// For monitoring of internal processes.
  /// => Pointers should be filled with TH1*, TH2*, TProfile*, ...
  /// ==> To check if object N is of type T, use "T ObjectPointer = dynamic_cast<T>( fvpAllHistoPointers[N].first );" and check for nullptr
  /// ==> To get back the original class name use "fvpAllHistoPointers[N].first->ClassName()" which returns a const char * (e.g. "TH1I")
  /// ===> Usage example with feeding a THttpServer:
  /// ===> #include "TH2.h"
  /// ===> std::string sClassName = vHistos[ uHisto ].first.ClassName();
  /// ===> if( !strncmp( sClassName, "TH1", 3 ) )
  /// ===>    server->Register( vHistos[ uHisto ].second.data(), dynamic_cast< TH1 * >(vHistos[ uHisto ].first) );
  /// ===> else if( !strncmp( sClassName, "TH2", 3 ) )
  /// ===>    server->Register( vHistos[ uHisto ].second.data(), dynamic_cast< TH2 * >(vHistos[ uHisto ].first) );
  std::vector<std::pair<TNamed*, std::string>>
    fvpAllHistoPointers;  //! Vector of pointers to histograms + optional folder name
  std::vector<std::pair<TCanvas*, std::string>>
    fvpAllCanvasPointers;  //! Vector of pointers to canvases + optional folder name

  CbmTofUnpackMonitor(const CbmTofUnpackMonitor&);
  CbmTofUnpackMonitor operator=(const CbmTofUnpackMonitor&);

  ClassDef(CbmTofUnpackMonitor, 1)
};

#endif
