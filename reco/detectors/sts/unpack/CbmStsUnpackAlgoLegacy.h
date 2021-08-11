// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                  CbmStsUnpackAlgoLegacy                       -----
// -----               Created 26.01.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmStsUnpackAlgoLegacy_H
#define CbmStsUnpackAlgoLegacy_H

#include "CbmErrorMessage.h"  // REMARK see remark in CbmStsUnpackConfig
#include "CbmStsUnpackAlgoBase.h"

#include "StsXyterFinalHit.h"
#include "StsXyterMessage.h"

// CbmRoot

// C++11
#include <chrono>

// C/C++
#include <map>
#include <vector>

class CbmMcbm2018StsPar;
class CbmStsUnpackMonitor;

class CbmStsUnpackAlgoLegacy : public CbmStsUnpackAlgoBase {
public:
  CbmStsUnpackAlgoLegacy();
  ~CbmStsUnpackAlgoLegacy();

  /** @brief Copy constructor - not implemented **/
  CbmStsUnpackAlgoLegacy(const CbmStsUnpackAlgoLegacy&) = delete;

  /** @brief Assignment operator - not implemented **/
  CbmStsUnpackAlgoLegacy& operator=(const CbmStsUnpackAlgoLegacy&) = delete;

  virtual bool init();
  virtual void reset();

  bool InitParameters();

  //virtual bool Unpack(const fles::Timeslice& ts, const uint32_t uMsComp);

  inline void SetTimeOffsetNs(double dOffsetIn = 0.0) { fdTimeOffsetNs = dOffsetIn; }
  virtual void MaskNoisyChannel(const uint32_t uFeb, const uint32_t uChan, const bool bMasked = true);

protected:
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

private:
  /// Control flags
  std::vector<bool> fvbMaskedComponents;

  /// Settings from parameter file
  CbmMcbm2018StsPar* fUnpackPar;                //!
                                                /// Readout chain dimensions and mapping
  uint32_t fuNbFebs;                            //! Number of FEBs with StsXyter ASICs
  std::map<uint32_t, uint32_t> fDpbIdIndexMap;  //! Map of DPB Identifier to DPB index
  void InitDpbIdIndexMap();

  std::vector<std::vector<std::vector<int32_t>>>
    fviFebType;  //! FEB type, [ NbDpb ][ NbCrobPerDpb ][ NbFebsPerCrob ], 0 = A, 1 = B, -1 if inactive
  std::vector<int32_t> fviFebAddress;  //! STS address for each FEB, [ NbDpb * NbCrobPerDpb * NbFebsPerCrob ]
  std::vector<int32_t> fviFebSide;     //! Module side for each FEB, [ NbDpb * NbCrobPerDpb * NbFebsPerCrob ]
  std::vector<double> fvdFebAdcGain;   //! ADC gain in e-/b, [ NbDpb * NbCrobPerDpb * NbFebsPerCrob ]
  std::vector<double> fvdFebAdcOffs;   //! ADC offset in e-, [ NbDpb * NbCrobPerDpb * NbFebsPerCrob ]

  /**
   * @brief Handles the distribution of the hidden derived classes to their explicit functions.
   *
   * @param parset
   * @return Bool_t initOk
  */
  Bool_t initParSet(FairParGenericSet* parset);

  /**
   * @brief Initialize the parameters from CbmMcbm2018StsPar.
   *
   * @param parset
   * @return Bool_t initOk
  */
  Bool_t initParSet(CbmMcbm2018StsPar* parset);

  void InitInternalStatus();
  void InitTempVectors(std::vector<int32_t>* viModuleType, std::vector<int32_t>* viModAddress,
                       std::vector<std::vector<std::vector<int32_t>>>* viFebModuleIdx,
                       std::vector<std::vector<bool>>* vbCrobActiveFlag,
                       std::vector<std::vector<std::vector<int32_t>>>* viFebModuleSide);

  void PrintActiveCrobs(const std::vector<std::vector<bool>>& vbCrobActiveFlag);
  void PrintAddressMaps(const std::vector<std::vector<std::vector<int32_t>>>& viFebModuleIdx,
                        const std::vector<std::vector<std::vector<int32_t>>>& viFebModuleSide);

  /// Add the hits to the output buffer as Digis
  void AddHitsToDigiVect(std::vector<stsxyter::FinalHit>* vmHitsIn, std::vector<CbmStsDigi>* vDigiVectOut);

  /// Get full time stamp from raw time stamp
  int64_t GetFullTimeStamp(const uint16_t usRawTs);

  /// User settings: Data correction parameters
  double fdTimeOffsetNs;
  bool fbUseChannelMask;
  std::vector<std::vector<bool>>
    fvvbMaskedChannels;  //! Vector of channel masks, [ NbFeb ][ NbCHanInFeb ], used only if fbUseChannelMask is true

  /// Running indices
  /// TS/MS info
  uint64_t fulCurrentMsIdx;

  /// Current data properties
  uint32_t fuCurrDpbIdx;                       //! Index of the DPB from which the MS currently unpacked is coming
                                               /// Data format control
  std::vector<uint64_t> fvulCurrentTsMsb;      //! Current TS MSB for each DPB
  std::vector<uint32_t> fvuCurrentTsMsbCycle;  //! Current TS MSB cycle for DPB
                                               /// Starting state book-keeping
  double fdStartTime;     /** Time of first valid hit (TS_MSB available), used as reference for evolution plots**/
  double fdStartTimeMsSz; /** Time of first microslice, used as reference for evolution plots**/
  std::chrono::steady_clock::time_point
    ftStartTimeUnix; /** Time of run Start from UNIX system, used as reference for long evolution plots against reception time **/

  /// Hits time-sorting
  std::vector<stsxyter::FinalHit>
    fvmHitsInMs;  //! All hits (time in bins, ADC in bins, asic, channel) in last MS, sorted with "<" operator

  /// Duplicate hits suppression
  static const uint32_t kuMaxTsMsbDiffDuplicates = 8;
  std::vector<std::vector<uint16_t>> fvvusLastTsChan;  //! TS of last hit message for each channel, [ AsicIdx ][ Chan ]
  std::vector<std::vector<uint16_t>>
    fvvusLastAdcChan;  //! ADC of last hit message for each channel, [ AsicIdx ][ Chan ]
  std::vector<std::vector<uint16_t>>
    fvvusLastTsMsbChan;  //! TS MSB of last hit message for each channel, [ AsicIdx ][ Chan ]
  std::vector<std::vector<uint16_t>>
    fvvusLastTsMsbCycleChan;  //! TS MSB cycle of last hit message for each channel, [ AsicIdx ][ Chan ]

  void ProcessHitInfo(const stsxyter::Message& mess);
  void ProcessTsMsbInfo(const stsxyter::Message& mess, uint32_t uMessIdx = 0, uint32_t uMsIdx = 0);
  void ProcessEpochInfo(const stsxyter::Message& mess);
  void ProcessStatusInfo(const stsxyter::Message& mess, uint32_t uIdx);
  void ProcessErrorInfo(const stsxyter::Message& mess);

  void RefreshTsMsbFields(const size_t uMsIdx);
  void LoopMsMessages(const uint8_t* msContent, const uint32_t uSize, const size_t uMsIdx);

  ClassDef(CbmStsUnpackAlgoLegacy, 1)
};

#endif
