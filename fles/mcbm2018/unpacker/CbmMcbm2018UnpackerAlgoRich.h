/** @file CbmMcbm2018UnpackerAlgoRich.h
  * @copyright Copyright (C) 2019-2020 Justus-Liebig-Universitaet Giessen, Giessen
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Egor Ovcharenko [orginator] **/

/**
 * CbmMcbm2018UnpackerAlgoRich
 * E. Ovcharenko, Mar 2019
 * based on other detectors' classes by P.-A. Loizeau
 */

#ifndef CbmMcbm2018UnpackerAlgoRich_H
#define CbmMcbm2018UnpackerAlgoRich_H

#include "CbmStar2019Algo.h"  // mother class

// STD
#include <map>
#include <vector>

// ROOT
#include <TArrayD.h>
#include <TH2D.h>

// CbmRoot
#include "CbmMcbm2018UnpackerUtilRich.h"
#include "CbmRichDigi.h"

////class TList; // not really needed, already declared in the mother class
class CbmMcbm2018RichPar;

#define RISINGEDGEID 1
#define FALLINGEDGEID 0

#define TOTMIN -20.
#define TOTMAX 100.

enum class TrbNetState
{
  IDLE,
  HEADER,
  EPOCH,
  TDC,
  TRAILER,
  CTS,
  DEBUG
};

enum class RichErrorType
{
  mtsError,
  tdcHeader,
  tdcTrailer,
  ctsHeader,
  ctsTrailer,
  subEventError
};

class CbmMcbm2018UnpackerAlgoRich : public CbmStar2019Algo<CbmRichDigi> {
public:
  CbmMcbm2018UnpackerAlgoRich();

  virtual ~CbmMcbm2018UnpackerAlgoRich();

  virtual Bool_t Init();

  virtual void Reset();

  virtual void Finish();

  virtual Bool_t InitContainers();

  virtual Bool_t ReInitContainers();

  virtual TList* GetParList();

  Bool_t InitParameters();

  /**
  	 * Copied from other detectors without any brain effort...
	 */
  void AddMsComponentToList(size_t component, UShort_t usDetectorId);

  virtual Bool_t ProcessTs(const fles::Timeslice& ts);

  virtual Bool_t ProcessTs(const fles::Timeslice& ts, size_t component);

  virtual Bool_t ProcessMs(const fles::Timeslice& ts, size_t uMsCompIdx, size_t uMsIdx);

  Bool_t DebugMs(const fles::Timeslice& ts, size_t uMsCompIdx, size_t uMsIdx);

  void SetMonitorMode(Bool_t bFlagIn = kTRUE) { fbMonitorMode = bFlagIn; }

  inline void SetTimeOffsetNs(Double_t dOffsetIn = 0.0) { fdTimeOffsetNs = dOffsetIn; }

  inline void DoTotCorr(Bool_t bDoToTCorr = kTRUE) { fbDoToTCorr = bDoToTCorr; }

  void SetRawDataMode(Bool_t bDebug = kFALSE) { fRawDataMode = bDebug; }

  Int_t Debug(const uint8_t* ptr, const size_t size);

private:
  /**
	 * Initialize space required for monitoring.
	 * This depends on the parameters read from the par file.
	 * This method should be called once after parameters import.
	 */
  void InitStorage();

  void ProcessMicroslice(size_t const size, uint8_t const* const ptr);

  /**
	 * Including header
	 */
  Int_t ProcessTRBevent(size_t const size, uint8_t const* const ptr);

  /**
	 *
	 */
  Int_t ProcessTRBeventHeader(size_t const size, uint8_t const* const ptr);

  /**
	 * Including header - ?
	 * Return number of processed bytes
	 */
  Int_t ProcessSKIPsubevent(size_t const size, uint8_t const* const ptr);

  Int_t ProcessCTSsubevent(size_t const size, uint8_t const* const ptr);

  Int_t ProcessTRBsubevent(size_t const size, uint8_t const* const ptr);

  /**
	 * Including TDC header, but not including TRB subsubevent header
	 * Return number of processed bytes
	 */
  Int_t ProcessTRBsubsubevent(size_t const size, uint8_t const* const ptr, Int_t const hubOffset, size_t const hubSize);

  /**
	 * Process a word written out by the TDC - TIMESTEMP, HEADER, TRAILER, DEBUG, EPOCH, ...
	 */
  Int_t ProcessTDCword(uint8_t const* const ptr, Int_t const word, size_t const size);

  /**
	 * Process specifically a TIMESTAMP type of word
	 */
  void ProcessTimestampWord(Int_t tdcData);

  /**
	 * Write a gidi object into the output collection
	 */
  void WriteOutputDigi(Int_t fpgaID, Int_t channel, Double_t time, Double_t tot, uint64_t MSidx);

  /**
	 * Method which is called at the end of the timeslice processing
	 */
  void FinalizeTs();

  /**
     * Write Errors to Histograms
     */
  void ErrorMsg(uint16_t errbits, RichErrorType type, uint16_t tdcAddr = 0);

  Int_t GetPixelUID(Int_t fpgaID, Int_t ch) const
  {
    // First 16 bits are used for the FPGA ID, then
    // 8 bits unused and then 8 bits are used for the channel
    return ((fpgaID << 16) | (ch & 0x00FF));
  }

  void findTDCAlignmentError(uint8_t const* const ptr, size_t const size);

private:  // data members
  /// Control flags
  Bool_t fbMonitorMode;  //! Switch ON the filling of a minimal set of histograms

  Bool_t fbDebugMonitorMode;  //! Switch ON the filling of a additional set of histograms

  Bool_t fRawDataMode;

  Bool_t fError;  //! flag for an error in the datastream

  TrbNetState fTrbState;  // State of TrbNet (HEADER,TRAILER,...)

  uint32_t fErrorCorr;  // Offset to correct a error in datastream

  /// User setting: kTRUE activates ToT correction from Parameterfile
  Bool_t fbDoToTCorr;

  Bool_t fSkipMs;
  /// User settings: Data correction parameters
  Double_t fdTimeOffsetNs;

  size_t fDataSize = 0;


  /**
	 * Bug fix / shortcut, which helps to run correct ProcessTs method.
	 * Works only if there is one componentID assigned to the detector.
	 */
  Int_t fRICHcompIdx;

  /**
	 * Unpacker parameters object
	 */
  CbmMcbm2018RichPar* fUnpackPar;  //!

  /**
	 * Counter of processed timeslices
	 */
  uint64_t fTScounter;

  /**
	 * Current microslice ID
	 */
  Int_t fCurMSid;  //!

  /**
	 * Current microslice index from the microslice descriptor
	 * CBM reference time //TODO clearify
	 */
  uint64_t fCurMSidx;  //!

  /**
	 * Global word counter within current microslice
	 */
  Int_t fGwordCnt;  //!

  /**
	 * Flag indicating that we are in the subsubevent
	 */
  Bool_t fInSubSubEvent;  //!

  /**
	 * Current epoch value
	 */
  UInt_t fCurEpochCounter;  //!

  /**
	 * Current subsubevent ID
	 */
  Int_t fSubSubEvId;  //!

  /**
	 * Flag to mark the last DiRICH on a Hub
	 */
  Bool_t fLastFeeOnHub = false;

  std::vector<Int_t> fTDCAlignmentErrorPositions;

  Int_t fTdcWordCorrectionCnt = 0;

  Int_t fTdcWordCorrectionGlobalCnt = 0;

  Int_t fSkipCnt = 0;

private:  // Stored timestamps
  /**
	 * Full time of the last rising edge from ch 0 of CTS
	 */
  Double_t fLastCTSch0_re_time;  //!

  /**
	 * Full time of the last rising edge from ch 2 of CTS
	 */
  Double_t fLastCTSch2_re_time;  //!

  /**
	 * Full time of the last falling edge from ch 2 of CTS
	 */
  Double_t fLastCTSch2_fe_time;  //!

  /**
	 * Full time of the last rising edge from ch 0 of CTS from the previous microslice
	 */
  Double_t fPrevLastCTSch0_re_time;  //!

  /**
	 * Full time of the last rising edge from ch 2 of CTS from the previous microslice
	 */
  Double_t fPrevLastCTSch2_re_time;  //!

  /**
	 * Full time of the last falling edge from ch 2 of CTS from the previous microslice
	 */
  Double_t fPrevLastCTSch2_fe_time;  //!

  /**
	 * Full time of the last rising edge from ch 0 of each TDC
	 */
  TArrayD fLastCh0_re_time;  //!

  /**
	 * Full time of the previous last rising edge from ch 0 of each TDC (from the previous microslice)
	 */
  TArrayD fPrevLastCh0_re_time;  //!

private:  // digi building
  void ProcessRisingEdge(Int_t subSubEvId, Int_t channel, Double_t time);

  void ProcessFallingEdge(Int_t subSubEvId, Int_t channel, Double_t time);

  /**
	 * Buffer for rising edges. It is filled during unpacking whenever
	 * the rising edge is encountered. Afterwards, when a falling edge
	 * is encountered, corresponding rising edge is searched here and
	 * removed if found.
	 */
  std::vector<CbmMcbmRichEdge> fRisingEdgesBuf;  //! Exclude from ROOT dictionnary due to missing empty constructor!!

  /**
	 * Buffer of falling edges for which corresponding rising edges were not found
	 */
  std::vector<CbmMcbmRichEdge> fFallingEdgesBuf;  //! Exclude from ROOT dictionnary due to missing empty constructor!!

public:  // histograms
  /**
	 *
	 */
  Bool_t CreateHistograms();

  /**
	 *
	 */
  //	Bool_t FillHistograms();

  /**
	 *
	 */
  Bool_t ResetHistograms();

  TH1D* GetTotH1(Int_t fpgaID, Int_t channel);
  TH2D* GetTotH2(Int_t fpgaID);
  /*
	TH2D* fhTDCch0re_minusCTSch0re; //!
	TH2D* fhTDCch0re_minusCTSch2re; //!
	TH2D* fhTDCch0re_minusCTSch2fe; //!
	TH2D* fhTDCch0re_minusPrevCTSch0re; //!
	TH2D* fhTDCch0re_minusPrevCTSch2re; //!
	TH2D* fhTDCch0re_minusPrevCTSch2fe; //!

	std::vector<TH2D*> fhTDCre_minusTDCch0re; //!

	std::vector<TH2D*> fhTDCre_minusPrevTDCch0re; //!
*/
  std::vector<TH2D*> fhTDCre_corrected1;  //!

  std::vector<TH2D*> fhTDCre_corrected2;  //!

  std::vector<TCanvas*> fcTot2d;  //!

  TH1* fhVectorSize     = nullptr;
  TH1* fhVectorCapacity = nullptr;

  //TH2* fhDigisInChnl       = nullptr;
  //TH2* fhDigisInDiRICH     = nullptr;

  TH2D* fhTdcErrors   = nullptr;
  TH2D* fhEventErrors = nullptr;

  TH2D* fhDiRICHWords = nullptr;
  TH2D* fhChnlWords   = nullptr;

  TH1* fhEventSize       = nullptr;
  TH2* fhSubEventSize    = nullptr;
  TH2* fhSubSubEventSize = nullptr;
  TH2* fhChnlSize        = nullptr;

  std::array<unsigned int, 33> fChnlMsgCnt;

  bool fDebugPrint = 0;
  std::map<uint16_t, uint16_t> fMapFEE;

  std::map<Int_t, std::map<Int_t, TH1D*>> fhTotMap;

  std::map<Int_t, TH2D*> fhTot2dMap;

  size_t fuTsMaxVectorSize     = 0;
  Double_t fdCapacityIncFactor = 1.1;

  inline void SetVectCapInc(Double_t dIncFact) { fdCapacityIncFactor = dIncFact; }

  ClassDef(CbmMcbm2018UnpackerAlgoRich, 1);
};

#endif  // CbmMcbm2018UnpackerAlgoRich_H
