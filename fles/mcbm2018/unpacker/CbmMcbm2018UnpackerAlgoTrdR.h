/**
  * @file CbmMcbm2018UnpackerAlgoTrdR.h
  * @author Dennis Spicker <dspicker@ikf.uni-frankfurt.de>
  * @date 2020-01-20
 **/

#ifndef CbmMcbm2018UnpackerAlgoTrdR_H
#define CbmMcbm2018UnpackerAlgoTrdR_H

#include "TH1.h"
#include "TH2.h"
#include "TObjArray.h"
#include "TProfile.h"
#include "TString.h"

#include "CbmStar2019Algo.h"

#include "CbmTrdDigi.h"
#include "CbmTrdParSetAsic.h"
#include "CbmTrdParSetDigi.h"
#include "CbmTrdParSetGain.h"
#include "CbmTrdParSetGas.h"
#include "CbmTrdRawMessageSpadic.h"
//#include "CbmTrdRawToDigiR.h"
/// TODO: include the relevant functions of CbmTrdRawToDigiR into the unpacker-algo.
///       as first step one can copy CbmTrdRawToDigiR.h and CbmTrdRawToDigiR.cxx to the same folder as this file.

/**
  *  @class CbmMcbm2018UnpackerAlgoTrdR
  *  @brief Timeslice unpacker algorithm for Spadic v.2.2 .
  *
  *
 **/
class CbmMcbm2018UnpackerAlgoTrdR : public CbmStar2019Algo<CbmTrdDigi> {
public:
  /** Default Constructor */
  CbmMcbm2018UnpackerAlgoTrdR();
  /** Default Destructor */
  ~CbmMcbm2018UnpackerAlgoTrdR();

  /** \brief Copy constructor - not implemented **/
  CbmMcbm2018UnpackerAlgoTrdR(const CbmMcbm2018UnpackerAlgoTrdR&) = delete;
  /** \brief Copy assignment operator - not implemented **/
  CbmMcbm2018UnpackerAlgoTrdR&
  operator=(const CbmMcbm2018UnpackerAlgoTrdR&) = delete;

  virtual Bool_t Init();
  virtual void Reset();
  virtual void Finish();

  Bool_t InitContainers();
  Bool_t ReInitContainers();
  TList* GetParList();

  Bool_t InitParameters();

  Bool_t ProcessTs(const fles::Timeslice& ts);
  Bool_t ProcessTs(const fles::Timeslice& ts, size_t /*component*/) {
    return ProcessTs(ts);
  }

  /**
   *  @brief Unpacks one Microslice
   *  @param ts Timeslice that contains the Microslice
   *  @param uMsCompIdx Component ID. The contributing input channel.
   *  @param uMsIdx Index of the Microslice inside the Timeslice
   *  @return kTRUE if successfull. kFALSE if errors occur.
   **/
  Bool_t ProcessMs(const fles::Timeslice& ts, size_t uMsCompIdx, size_t uMsIdx);

  void AddMsComponentToList(size_t component, UShort_t usDetectorId);
  void SetNbMsInTs(size_t uCoreMsNb, size_t uOverlapMsNb);

  enum ECbmTrdUnpackerHistograms : Int_t {
    kBeginDefinedHistos         = 0,
    kRawMessage_Signalshape_all = 0,
    kRawMessage_Signalshape_St,
    kRawMessage_Signalshape_Nt,
    kRawMessage_Signalshape_filtered,
    kRawDistributionMapModule5,
    kRawHitType,
    kRawPulserDeltaT,
    kSpadic_Info_Types,
    kBeginDigiHistos,
    kDigiPulserDeltaT = kBeginDigiHistos,
    kDigiDeltaT  // Heavy histogram add with care
    ,
    kDigiMeanHitFrequency  // Heavy histogram add with care
    ,
    kDigiHitFrequency,
    kDigiRelativeTimeMicroslice,
    kDigiDistributionMap,
    kDigiDistributionMapSt,
    kDigiDistributionMapNt,
    kDigiChargeSpectrum,
    kDigiChargeSpectrumSt,
    kDigiChargeSpectrumNt,
    kDigiTriggerType,
    kEndDefinedHistos
  };
  void SetActiveHistograms(std::vector<bool> isActiveHistoVec) {
    fIsActiveHistoVec = isActiveHistoVec;
  }
  Bool_t
  CreateHistograms();  ///< Goes through fIsActiveHistoVec and creates the activated histograms
  Bool_t
  CreateHistogram(ECbmTrdUnpackerHistograms
                    iHisto);  ///< create the histogram correlated to iHisto

  Bool_t FillHistograms();
  Bool_t FillHistograms(CbmTrdDigi const& digi);
  Bool_t FillHistograms(CbmTrdRawMessageSpadic const& raw);
  Bool_t ResetHistograms();


  std::vector<CbmTrdRawMessageSpadic> GetRawMessageVector() {
    return *(fTrdRawMessageVector);
  }
  TString GetRefGeoTag() { return fRefGeoTag; }

  void SetMonitorMode(Bool_t bFlagIn = kTRUE) { fbMonitorMode = bFlagIn; }
  void SetDebugMonitorMode(Bool_t bFlagIn = kTRUE) {
    fbDebugMonitorMode = bFlagIn;
  }
  void SetWriteOutput(Bool_t bFlagIn = kTRUE) { fbWriteOutput = bFlagIn; }
  void SetDebugWriteOutput(Bool_t bFlagIn = kTRUE) {
    fbDebugWriteOutput = bFlagIn;
  }
  inline void SetTimeOffsetNs(Double_t dOffsetIn = 0.0) {
    fdTimeOffsetNs = dOffsetIn;
  }

  /**
	 *  @brief Call this when Spadic Average-Baseline feature is enabled.
	 **/
  void SetBaselineAvg(Bool_t bFlagIn = kTRUE) { fbBaselineAvg = bFlagIn; }

  /**
   *  @brief Set fTrdDigiVector to the address of pVector.
   *  @param pVector Pointer to the Digi output vector of the Unpacker task.
   *  @return kTRUE if fTrdDigiVector was nullptr before. kFALSE else.
   **/
  Bool_t SetDigiOutputPointer(std::vector<CbmTrdDigi>* const pVector);

  /**
   *  @brief Set fTrdRawMessageVector to the address of pVector.
   *  @param pVector Pointer to the RawMessage output vector of the Unpacker task.
   *  @return kTRUE if fTrdRawMessageVector was nullptr before. kFALSE else.
   **/
  Bool_t SetRawOutputPointer(
    std::vector<CbmTrdRawMessageSpadic>* const pVector,
    std::vector<std::pair<std::uint64_t, std::uint64_t>>* const qVector =
      nullptr);

  void SetRefGeoTag(TString geoTag) { fRefGeoTag = geoTag; }
  void SetFirstChannelsElinkEven(bool isEven) {
    fIsFirstChannelsElinkEven = isEven;
  }
  void SetMsSizeInNs(Double_t msSizeInNs) {
    fdMsSizeInNs = msSizeInNs;
  }  // TODO handle this with asic parameter files

private:
  UInt_t fdMsSizeInCC;
  std::shared_ptr<CbmTrdDigi> MakeDigi(CbmTrdRawMessageSpadic raw);

  /** Identifies the message type of a 64bit word inside a Microslice */
  Spadic::MsMessageType GetMessageType(const uint64_t msg);

  /** Identifies the InfoType of a 64bit InfoMessage word inside a Microslice */
  Spadic::MsInfoType GetInfoType(const uint64_t msg);

  CbmTrdRawMessageSpadic CreateRawMessage(const uint64_t word,
                                          fles::MicrosliceDescriptor msDesc);

  /** Helper function that returns how many rda words follow the som word, based on the number of samples in the msg.  */
  Int_t GetNumRda(Int_t nsamples);

  /** @brief Extract a Sample from a given SOM or RDA word
   *  @param word the 64bit Message Word
   *  @param sample Which sample to extract [0,31]
   *  @param multihit Set to true if current word belongs to a multihit message.
   *  @return The ADC value of the given sample, range [-255,255]. -256 if an error occurred.
   **/
  int16_t
  ExtractSample(const uint64_t word, uint8_t sample, Bool_t multihit = kFALSE);

  // Control flags
  Bool_t
    fbMonitorMode;  ///< Switch ON the filling of a minimal set of histograms.
  Bool_t
    fbDebugMonitorMode;  ///< Switch ON the filling of a additional set of histograms.
  Bool_t
    fbWriteOutput;  ///< If ON the output Vector of digis is written to disk.
  Bool_t
    fbDebugWriteOutput;  ///< If ON the raw messages output vector is filled and written to disk.
  Bool_t
    fbBaselineAvg;  ///< Set to true if Baseline Averaging is activated in Spadic.

  /// User settings: Data correction parameters
  Double_t fdTimeOffsetNs = 0.0;

  /// Output Digi vector
  std::vector<CbmTrdDigi>* fTrdDigiVector;

  /// Output Spadic raw messages for debugging
  std::vector<CbmTrdRawMessageSpadic>* fTrdRawMessageVector;

  /// vector< pair< fulltime, word > >
  std::vector<std::pair<std::uint64_t, std::uint64_t>>* fSpadicInfoMsgVector;

  //std::map< TString, std::shared_ptr<TH1> > fHistoMap ;

  /// Stores all Histograms.
  std::vector<bool> fIsActiveHistoVec;
  TObjArray fHistoArray;
  std::vector<std::uint64_t> fLastDigiTimeVec;

  /**
   *  @brief Instance of RawToDigi class.
   *
   *  This class provides conversion from raw spadic data to CbmTrdDigis via
   *  CbmTrdDigi* MakeDigi(std::vector<Int_t> samples, Int_t channel,Int_t module,Int_t layer,ULong64_t time);
   **/
  //CbmTrdRawToDigiR *fRawToDigi ;

  /// Running Indices
  ULong64_t fNbTimeslices;  ///< Counts overall number of Timeslices
  ULong64_t fCurrTsIdx;     ///< Index of current Timeslice
  UInt_t fMsIndex;          ///< Index of current MS within the TS
  Double_t
    fTsStartTime;  ///< Time in ns of current TS from the index of the first MS first componen
  Double_t
    fTsStopTimeCore;  ///< End Time in ns of current TS Core from the index of the first MS first componen
  Double_t
    fMsTime;  ///< Start Time in ns of current MS from its index field in header
  ULong64_t
    fSpadicEpoch;  ///< Epoch counter (30 bits). Counts overflow of Spadic timestamp (11 bits).

  ULong64_t fLastFulltime;

  /// Counters
  ULong64_t fNbSpadicRawMsg;    ///< Number of Spadic Raw Messages.
  ULong64_t fNbWildRda;         ///< Number of RDA Words outside of a Message.
  ULong64_t fNbSpadicErrorMsg;  ///< Number of Spadic error Messages.
  ULong64_t
    fNbUnkownWord;  ///< Number of unknown data words in the Microslice stream.
  ULong64_t fNbSpadicEpochMsg;  ///< Number of Spadic Epoch Messages.

  /// Parameters and Address mapping
  TList*
    fParContList;  ///< List containing the required trd parameter containers
  TString
    fRefGeoTag;  ///< Naming tag for the reference geometry, parameters are loaded according to this tag
  CbmTrdParSetAsic* fAsicPar;  ///< CbmTrdParameter container
  CbmTrdParSetDigi* fDigiPar;  ///< CbmTrdParameter container
  CbmTrdParSetGas* fGasPar;    ///< CbmTrdParameter container
  CbmTrdParSetGain* fGainPar;  ///< CbmTrdParameter container
  std::map<std::uint64_t, Int_t>
    fSpadicMap;  ///< Map to retrieve asic address from CriId/CrobId/ElinkId (see CbmTrdHardwareSetupR)
  std::map<Int_t, std::vector<Int_t>>
    fAsicChannelMap;  ///< Map to retrieve module channelId from asicAddress and asicChannel
  bool
    fIsFirstChannelsElinkEven;  ///< define if the first 16 channels (00..15) are found on the even (set true) or odd (false) eLinkId, default for mCbm2020 is false thus, initialized as false

  /// Constants
  static const UInt_t kBytesPerWord = 8;

  ClassDef(CbmMcbm2018UnpackerAlgoTrdR, 1)
};

#endif
