/* Copyright (C) 2019-2020 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Egor Ovcharenko [committer], Semen Lebedev */

/**
 * CbmMcbm2018UnpackerAlgoRich
 * E. Ovcharenko, Mar 2019
 * S. Lebedev, June 2021
 * based on other detectors' classes by P.-A. Loizeau
 */

#ifndef CbmMcbm2018UnpackerAlgoRich_H
#define CbmMcbm2018UnpackerAlgoRich_H

#include "CbmStar2019Algo.h"  // mother class

// STD
#include <bitset>
#include <iomanip>
#include <map>
#include <vector>

// ROOT
#include <Logger.h>

#include <TArrayD.h>
#include <TH2D.h>

// CbmRoot
#include "CbmRichDigi.h"

////class TList; // not really needed, already declared in the mother class
class CbmMcbm2018RichPar;


enum class CbmMcbm2018RichErrorType
{
  mtsError,
  tdcHeader,
  tdcTrailer,
  ctsHeader,
  ctsTrailer,
  subEventError
};

enum class CbmMcbm2018RichTdcWordType
{
  TimeData,
  Header,
  Epoch,
  Trailer,
  Debug,
  Error
};

class CbmMcbm2018RichTdcTimeData {
public:
  uint32_t fCoarse  = 0;  // 11 bits
  uint32_t fEdge    = 0;  // 1 bit
  uint32_t fFine    = 0;  // 10 bits
  uint32_t fChannel = 0;  // 7 bits

  std::string ToString()
  {
    std::stringstream stream;
    stream << "channel:" << fChannel << " coarse:" << fCoarse << " fine:" << fFine
           << " edge:" << ((fEdge == 1) ? "R" : "F");
    return stream.str();
  }

  bool IsRisingEdge() { return (fEdge == 1); }
};

class CbmMcbm2018RichTdcWordReader {
public:
  static CbmMcbm2018RichTdcWordType GetTdcWordType(uint32_t tdcWord)
  {
    uint32_t tdcTimeDataMarker = (tdcWord >> 31) & 0x1;  // 1 bit
    uint32_t tdcMarker         = (tdcWord >> 29) & 0x7;  // 3 bits

    if (tdcTimeDataMarker == 0x1) {
      // TODO: I also include tdcMarker == 0x5, some tdc time data words have this marker. Is it correct?
      if (tdcMarker == 0x4 || tdcMarker == 0x5) { return CbmMcbm2018RichTdcWordType::TimeData; }
      else {
        return CbmMcbm2018RichTdcWordType::Error;
      }
    }

    if (tdcMarker == 0x0) return CbmMcbm2018RichTdcWordType::Trailer;
    if (tdcMarker == 0x1) return CbmMcbm2018RichTdcWordType::Header;
    if (tdcMarker == 0x2) return CbmMcbm2018RichTdcWordType::Debug;
    if (tdcMarker == 0x3) return CbmMcbm2018RichTdcWordType::Epoch;

    return CbmMcbm2018RichTdcWordType::Error;
  }

  static void ProcessTimeData(uint32_t tdcWord, CbmMcbm2018RichTdcTimeData& outData)
  {
    outData.fCoarse  = static_cast<uint32_t>(tdcWord & 0x7ff);          // 11 bits
    outData.fEdge    = static_cast<uint32_t>((tdcWord >> 11) & 0x1);    // 1 bit
    outData.fFine    = static_cast<uint32_t>((tdcWord >> 12) & 0x3ff);  // 10 bits
    outData.fChannel = static_cast<uint32_t>((tdcWord >> 22) & 0x7f);   // 7 bits
  }

  static uint32_t ProcessEpoch(uint32_t tdcWord) { return static_cast<uint32_t>(tdcWord & 0xfffffff); }

  static uint16_t ProcessHeader(uint32_t tdcWord)
  {
    // for the moment just extract error bits
    return static_cast<uint16_t>(tdcWord & 0xff);  //8 bits
  }

  static uint16_t ProcessTrailer(uint32_t tdcWord)
  {
    // for the moment just extract error bits
    return static_cast<uint16_t>(tdcWord & 0xffff);
  }

  static void ProcessDebug(uint32_t tdcWord)
  {
    LOG(debug4) << "ProcessDebug is not implemented. tdcWord:0x" << std::hex << tdcWord << std::dec;
    // for the moment do nothing
  }
};

class CbmMcbm2018RichMicrosliceReader {
private:
  const uint8_t* fData = nullptr;
  size_t fSize         = 0;
  size_t fOffset       = 0;  // offset in bytes
  size_t fWordCounter  = 0;
  uint32_t fCurWord;

public:
  void SetData(const uint8_t* data, size_t size)
  {
    fData        = data;
    fSize        = size;
    fOffset      = 0;
    fWordCounter = 0;
    fCurWord     = 0;
  }

  const uint8_t* GetData() { return fData; }

  size_t GetSize() { return fSize; }

  size_t GetOffset() { return fOffset; }

  size_t GetWordCounter() { return fWordCounter; }

  uint32_t GetCurWord() { return fCurWord; }

  std::string GetWordAsHexString(uint32_t word)
  {
    std::stringstream stream;
    stream << "0x" << std::setfill('0') << std::setw(sizeof(uint32_t) * 2) << std::hex << word;
    return stream.str();
  }

  uint32_t NextWord()
  {
    //mRichSupport::SwapBytes(4, fData + fOffset);
    //Int_t* dataPtr = (Int_t*) (fData + fOffset);
    uint32_t i = ((uint32_t*) (fData + fOffset))[0];
    //swap bytes
    i = (i >> 24) | ((i << 8) & 0x00FF0000) | ((i >> 8) & 0x0000FF00) | (i << 24);
    //i = ((i&0xFF)<<24) | (((i>>8)&0xFF)<<16) |   (((i>>16)&0xFF)<<8) | (((i>>24)&0xFF)<<0);

    fOffset += 4;
    fWordCounter++;
    fCurWord = i;
    //return (Int_t)(dataPtr[0]);
    return i;
  }

  bool IsNextPadding()
  {
    uint32_t nextWord = ((uint32_t*) (fData + fOffset))[0];
    if (nextWord == 0xffffffff) return true;
    return false;
  }

  bool IsLastSubSubEvent(uint32_t subSubEventSize)
  {
    uint32_t i = ((uint32_t*) (fData + fOffset + 4 * subSubEventSize))[0];
    i          = (i >> 24) | ((i << 8) & 0x00ff0000) | ((i >> 8) & 0x0000ff00) | (i << 24);
    if (i == 0x00015555) return true;
    return false;
  }
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

  std::string GetLogHeader(CbmMcbm2018RichMicrosliceReader& reader);

  void ProcessMicroslice(size_t const size, uint8_t const* const ptr);

  void ProcessTrbPacket(CbmMcbm2018RichMicrosliceReader& reader);

  void ProcessMbs(CbmMcbm2018RichMicrosliceReader& reader, bool isPrev);

  void ProcessHubBlock(CbmMcbm2018RichMicrosliceReader& reader);

  void ProcessCtsSubSubEvent(CbmMcbm2018RichMicrosliceReader& reader, uint32_t subSubEventSize, uint32_t subSubEventId);

  void ProcessSubSubEvent(CbmMcbm2018RichMicrosliceReader& reader, int nofTimeWords, uint32_t subSubEventId);

  double CalculateTime(uint32_t epoch, uint32_t coarse, uint32_t fine);

  bool IsLog();

  void ProcessTimeDataWord(CbmMcbm2018RichMicrosliceReader& reader, int iTdc, uint32_t epoch, uint32_t tdcWord,
                           uint32_t subSubEventId, std::vector<double>& raisingTime);

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
  void ErrorMsg(uint16_t errbits, CbmMcbm2018RichErrorType type, uint16_t tdcId = 0);

  Int_t GetPixelUID(Int_t fpgaID, Int_t ch) const
  {
    // First 16 bits are used for the FPGA ID, then
    // 8 bits unused and then 8 bits are used for the channel
    return ((fpgaID << 16) | (ch & 0x00FF));
  }

private:
  double fToTMin = -20.;
  double fToTMax = 100.;

  /// Control flags
  Bool_t fbMonitorMode      = false;  // Switch ON the filling of a minimal set of histograms
  Bool_t fbDebugMonitorMode = false;  // Switch ON the filling of a additional set of histograms
  Bool_t fRawDataMode       = false;

  uint64_t fTsCounter = 0;  //Counter of processed timeslices
  uint32_t fMsInd     = 0;  // Current microslice index

  double fMbsPrevTimeCh0 = 0.;
  double fMbsPrevTimeCh1 = 0.;

  std::map<uint32_t, double> fLastCh0ReTime;      //key:TDC ID, value:Full time of last rising edge from ch 0
  std::map<uint32_t, double> fPrevLastCh0ReTime;  // key:TDC ID, value:Full time of previous last rising edge from ch 0

  Bool_t fbDoToTCorr = true;  // kTRUE activates ToT correction from Parameterfile

  Double_t fdTimeOffsetNs = 0.;  // User settings: Data correction parameters

  /**
	 * Bug fix / shortcut, which helps to run correct ProcessTs method.
	 * Works only if there is one componentID assigned to the detector.
	 */
  Int_t fRICHcompIdx = 6;

  CbmMcbm2018RichPar* fUnpackPar = nullptr;  //!

  /**
	 * Current microslice index from the microslice descriptor
	 * CBM reference time //TODO clearify
	 */
  uint64_t fCurMSidx;  //!

public:  // histograms
  /**
	 *
	 */
  Bool_t CreateHistograms();

  /**
	 *
	 */
  Bool_t ResetHistograms();

  TH1D* GetTotH1(uint32_t fpgaID, uint32_t channel);
  TH2D* GetTotH2(uint32_t fpgaID);

  std::vector<TCanvas*> fcTot2d;

  TH2D* fhTdcErrors = nullptr;

  TH1* fhEventSize       = nullptr;
  TH2* fhSubEventSize    = nullptr;
  TH2* fhSubSubEventSize = nullptr;
  TH2* fhChnlSize        = nullptr;

  std::map<uint16_t, uint16_t> fMapFEE;

  std::map<uint32_t, std::map<Int_t, TH1D*>> fhTotMap;
  std::map<uint32_t, TH2D*> fhTot2dMap;

  uint64_t fdTsStartTime = 0;

  ClassDef(CbmMcbm2018UnpackerAlgoRich, 1);
};

#endif  // CbmMcbm2018UnpackerAlgoRich_H
