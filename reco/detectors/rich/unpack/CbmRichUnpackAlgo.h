/* Copyright (C) 2010 - 2021 Goethe-University Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig */

/**
 * @file CbmRichUnpackAlgo.h
 * @author Pascal Raisig (praisig@ikf.uni-frankfurt.de)
 * @brief Baseclass for the TrdR unpacker algorithms
 * @version 0.1
 * @date 2021-04-21
 * 
 * @copyright Copyright (c) 2021
 * 
 * This is the base class for the algorithmic part of the tsa data unpacking 
 * processes of the CbmTrd.
 * The actual translation from tsa to digi happens in the derived classes. 
 * 
 * 
*/

#ifndef CbmRichUnpackAlgo_H
#define CbmRichUnpackAlgo_H

// #include "CbmMcbm2018RichPar.h"
#include "../../core/detectors/rich/CbmMcbm2018RichPar.h"
#include "CbmRecoUnpackAlgo.tmpl"
#include "CbmRichDigi.h"

#include "Timeslice.hpp"  // timeslice

#include <Rtypes.h>  // for types
#include <RtypesCore.h>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>


class CbmRichUnpackAlgo : public CbmRecoUnpackAlgo<CbmRichDigi> {
public:
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
  /** @brief Create the Cbm Trd Unpack AlgoBase object */
  CbmRichUnpackAlgo();

  /** @brief Destroy the Cbm Trd Unpack Task object */
  virtual ~CbmRichUnpackAlgo();

  /** @brief Copy constructor - not implemented **/
  CbmRichUnpackAlgo(const CbmRichUnpackAlgo&) = delete;

  /** @brief Assignment operator - not implemented **/
  CbmRichUnpackAlgo& operator=(const CbmRichUnpackAlgo&) = delete;

  /**
   * @brief Get the requested parameter containers. To be defined in the derived classes!
   * Return the required parameter containers together with the paths to the ascii 
   * files to.
   *  
   * @param[in] std::string geoTag as used in CbmSetup
   * @param[in] std::uint32_t runId for runwise defined parameters
   * @return fParContVec
  */
  virtual std::vector<std::pair<std::string, std::shared_ptr<FairParGenericSet>>>*
  GetParContainerRequest(std::string geoTag, std::uint32_t runId);

protected:
  double calculateTime(uint32_t epoch, uint32_t coarse, uint32_t fine);

  /** @brief Finish function for this algorithm base clase */
  void finish()
  {
    finishDerived();
    // Finish the monitor if we have one
    // if (fMonitor) fMonitor->Finish();
  }

  /** @brief Function that allows special calls during Finish in the derived algos */
  virtual void finishDerived() { return; }

  std::string getLogHeader(CbmMcbm2018RichMicrosliceReader& reader);

  Int_t getPixelUID(Int_t fpgaID, Int_t ch) const
  {
    // First 16 bits are used for the FPGA ID, then
    // 8 bits unused and then 8 bits are used for the channel
    return ((fpgaID << 16) | (ch & 0x00FF));
  }

  /**
   * @brief Intialisation at begin of run. Special inits of the derived algos.
   * 
   * @retval Bool_t initOk
  */
  Bool_t init();

  /**
   * @brief Handles the distribution of the hidden derived classes to their explicit functions.
   * 
   * @param parset 
   * @return Bool_t initOk 
  */
  Bool_t initParSet(FairParGenericSet* parset);

  /**
   * @brief Handles the distribution of the hidden derived classes to their explicit functions.
   * 
   * @param parset 
   * @return Bool_t initOk 
  */
  Bool_t initParSet(CbmMcbm2018RichPar* parset);

  bool isLog();

  void processTrbPacket(CbmMcbm2018RichMicrosliceReader& reader);

  void processMbs(CbmMcbm2018RichMicrosliceReader& reader, bool isPrev);

  void processHubBlock(CbmMcbm2018RichMicrosliceReader& reader);

  void processCtsSubSubEvent(CbmMcbm2018RichMicrosliceReader& reader, uint32_t subSubEventSize, uint32_t subSubEventId);

  void processSubSubEvent(CbmMcbm2018RichMicrosliceReader& reader, int nofTimeWords, uint32_t subSubEventId);

  void processTimeDataWord(CbmMcbm2018RichMicrosliceReader& reader, int iTdc, uint32_t epoch, uint32_t tdcWord,
                           uint32_t subSubEventId, std::vector<double>& raisingTime);

  /**
	 * Write a gidi object into the output collection
	 */
  void writeOutputDigi(Int_t fpgaID, Int_t channel, Double_t time, Double_t tot);

  /**
   * @brief Set the Derived Ts Parameters
   * 
   * In this function parameters required by the explicit algo connected to the timeslice can be set.
   * 
   * @param itimeslice 
   * @return true 
   * @return false 
  */
  bool setDerivedTsParameters(size_t itimeslice) { return true; }

  /**
   * @brief Unpack a given microslice. To be implemented in the derived unpacker algos.
   * 
   * @param ts timeslice pointer
   * @param icomp index to the component to be unpacked
   * @param imslice index of the microslice to be unpacked
   * @return true 
   * @return false 
   * 
   * @remark The content of the µslice can only be accessed via the timeslice. Hence, we need to pass the pointer to the full timeslice
  */
  bool unpack(const fles::Timeslice* ts, std::uint16_t icomp, UInt_t imslice);

  /** @brief Fles Subsystem Identifier for the TRD R data */
  static constexpr int fgkFlesSubsystemIdTrdR = static_cast<int>(fles::SubsystemIdentifier::RICH);

  /** @brief Parameters for the unpacking */
  CbmMcbm2018RichPar fUnpackPar;

  double fMbsPrevTimeCh0 = 0.;
  double fMbsPrevTimeCh1 = 0.;

  double fToTMin = -20.;
  double fToTMax = 100.;

  std::map<uint32_t, double> fLastCh0ReTime;      //key:TDC ID, value:Full time of last rising edge from ch 0
  std::map<uint32_t, double> fPrevLastCh0ReTime;  // key:TDC ID, value:Full time of previous last rising edge from ch 0

  Bool_t fbDoToTCorr = true;  // kTRUE activates ToT correction from Parameterfile

private:
  ClassDef(CbmRichUnpackAlgo, 2)
};

#endif  // CbmRichUnpackAlgo_H
