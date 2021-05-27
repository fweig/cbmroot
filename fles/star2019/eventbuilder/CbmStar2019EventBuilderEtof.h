// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                     CbmStar2019EventBuilderEtof                   -----
// -----               Created 14.11.2018 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmStar2019EventBuilderEtof_H
#define CbmStar2019EventBuilderEtof_H

#include "CbmMcbmUnpack.h"

#include "Timeslice.hpp"

#include "TStopwatch.h"
#include "TString.h"

class CbmStar2019EventBuilderEtofAlgo;

/*
 ** Function to send sub-event block to the STAR DAQ system
 *       trg_word received is packed as:
 *
 *       trg_cmd|daq_cmd|tkn_hi|tkn_mid|tkn_lo
*/
extern "C" int star_rhicf_write(unsigned int trg_word, void* dta, int bytes);

class TH1;
class TH2;
class TProfile;

class CbmStar2019EventBuilderEtof : public CbmMcbmUnpack {
public:
  CbmStar2019EventBuilderEtof(UInt_t uNbGdpb = 1);
  virtual ~CbmStar2019EventBuilderEtof();

  virtual Bool_t Init();
  virtual Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);
  virtual void Reset();

  virtual void Finish();

  void SetParContainers();

  Bool_t InitContainers();

  Bool_t ReInitContainers();

  void SetSandboxMode(Bool_t bSandboxMode = kTRUE) { fbSandboxMode = bSandboxMode; }
  void SetEventDumpEnable(Bool_t bDumpEna = kTRUE);
  inline void SetMonitorMode(Bool_t bFlagIn = kTRUE) { fbMonitorMode = bFlagIn; }
  inline void SetDebugMonitorMode(Bool_t bFlagIn = kTRUE) { fbDebugMonitorMode = bFlagIn; }
  inline void SetStoreLostEventMsg(Bool_t bFlagIn = kTRUE) { fbStoreLostEventMsg = bFlagIn; }
  inline void SetAddStatusToEvent(Bool_t bFlagIn = kTRUE) { fbAddStatusToEvent = bFlagIn; }
  inline void SetPulserTotLimits(UInt_t uMin, UInt_t uMax)
  {
    fuMinTotPulser = uMin;
    fuMaxTotPulser = uMax;
  }

  /// Temp until we change from CbmMcbmUnpack to something else
  void AddMsComponentToList(size_t component, UShort_t usDetectorId);
  void SetNbMsInTs(size_t /*uCoreMsNb*/, size_t /*uOverlapMsNb*/) {};

  void UseDaqBuffer(Bool_t) {};

private:
  /// Control flags
  Bool_t fbMonitorMode;        //! Switch ON the filling of a minimal set of histograms
  Bool_t fbDebugMonitorMode;   //! Switch ON the filling of a additional set of histograms
  Bool_t fbStoreLostEventMsg;  //! Switch ON the insertion of the LostEvent messages from GET4s with the critical errors
  Bool_t fbAddStatusToEvent;   //! Switch ON the readout and insertion of STATUS pattern message (default is true)
  Bool_t fbSandboxMode;        //! Switch OFF the emission of data toward the STAR DAQ
  Bool_t fbEventDumpEna;       //! Switch ON the dumping of the events to a binary file

  /// Parameters management
  TList* fParCList;

  /// User settings: Data selection parameters
  UInt_t fuMinTotPulser;
  UInt_t fuMaxTotPulser;
  TString fsHistoFileName;

  /// Statistics & first TS rejection
  uint64_t fulTsCounter;

  /// Processing algo
  CbmStar2019EventBuilderEtofAlgo* fEventBuilderAlgo;

  /// Processing speed watch
  Double_t fdMsSizeInNs;       //! Size of a single MS, [nanoseconds]
  Double_t fdTsCoreSizeInSec;  //! Total size of the core MS in a TS, [seconds]
  TStopwatch fTimer;
  Double_t fdRealTime;
  Double_t fdRealTimeMin;
  Double_t fdRealTimeMax;
  Double_t fdRealTimeAll;
  Double_t fdRealTimeMinAll;
  Double_t fdRealTimeMaxAll;
  uint64_t fulNbEvents;
  uint64_t fulNbEventsSinceLastPrintout;
  TH1* fhRealTimeDistr;         //! Exclude from dictionary because ?!?!
  TH2* fhRealTimeEvo;           //! Exclude from dictionary because ?!?!
  TProfile* fhMeanRealTimeEvo;  //! Exclude from dictionary because ?!?!

  /// Event dump to binary file
  std::fstream* fpBinDumpFile;
  const UInt_t kuBinDumpBegWord = 0xFEEDBEAF;
  const UInt_t kuBinDumpEndWord = 0xFAEBDEEF;

  Bool_t SaveHistograms();

  CbmStar2019EventBuilderEtof(const CbmStar2019EventBuilderEtof&);
  CbmStar2019EventBuilderEtof operator=(const CbmStar2019EventBuilderEtof&);

  ClassDef(CbmStar2019EventBuilderEtof, 1)
};

#endif
