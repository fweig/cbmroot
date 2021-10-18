/* Copyright (C) 2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

/**
 * CbmDeviceUnpack.h
 *
 * @since 2020-05-04
 * @author P.-A. Loizeau
 */

#ifndef CBMDEVICEUNPACK_H_
#define CBMDEVICEUNPACK_H_

#include "CbmMqTMessage.h"
#include "CbmTsEventHeader.h"

#include "Timeslice.hpp"

#include "FairMQDevice.h"
#include "FairParGenericSet.h"

#include "Rtypes.h"
#include "TObjArray.h"

#include <map>
#include <vector>

class TList;
class CbmPsdUnpackConfig;
class CbmRichUnpackConfig;
class CbmStsUnpackConfig;
class CbmTofUnpackConfig;
class CbmTrdUnpackConfigFasp2D;
class CbmTrdUnpackConfig;

class TimesliceMetaData;

class CbmTrdSpadic;

class CbmDeviceUnpack : public FairMQDevice {
public:
  CbmDeviceUnpack();
  virtual ~CbmDeviceUnpack();

protected:
  virtual void InitTask();
  bool HandleData(FairMQMessagePtr&, int);
  bool HandleCommand(FairMQMessagePtr&, int);

  /** @brief Set the Sts Unpack Config @param config */
  void SetUnpackConfig(std::shared_ptr<CbmStsUnpackConfig> config) { fStsConfig = config; }

  // /** @brief Set the Tof Unpack Config @param config */
  void SetUnpackConfig(std::shared_ptr<CbmTofUnpackConfig> config) { fTofConfig = config; }

  /** @brief Set the Trd Unpack Config @param config */
  void SetUnpackConfig(std::shared_ptr<CbmTrdUnpackConfig> config) { fTrd1DConfig = config; }

  /** @brief Set the Trd2D Unpack Config @param config */
  void SetUnpackConfig(std::shared_ptr<CbmTrdUnpackConfigFasp2D> config) { fTrd2DConfig = config; }

  /** @brief Set the Rich Unpack Config @param config */
  void SetUnpackConfig(std::shared_ptr<CbmRichUnpackConfig> config) { fRichConfig = config; }

  /** @brief Set the Psd Unpack Config @param config */
  void SetUnpackConfig(std::shared_ptr<CbmPsdUnpackConfig> config) { fPsdConfig = config; }

private:
  /// Constants
  static const uint16_t kusSysIdSts  = 0x10;
  static const uint16_t kusSysIdMuch = 0x50;
  static const uint16_t kusSysIdTrd  = 0x40;
  static const uint16_t kusSysIdTof  = 0x60;
  static const uint16_t kusSysIdT0   = 0x90;
  static const uint16_t kusSysIdRich = 0x30;
  static const uint16_t kusSysIdPsd  = 0x80;

  static constexpr std::uint16_t fkFlesMvd   = static_cast<std::uint16_t>(fles::SubsystemIdentifier::MVD);
  static constexpr std::uint16_t fkFlesSts   = static_cast<std::uint16_t>(fles::SubsystemIdentifier::STS);
  static constexpr std::uint16_t fkFlesMuch  = static_cast<std::uint16_t>(fles::SubsystemIdentifier::MUCH);
  static constexpr std::uint16_t fkFlesTrd   = static_cast<std::uint16_t>(fles::SubsystemIdentifier::TRD);
  static constexpr std::uint16_t fkFlesTrd2D = static_cast<std::uint16_t>(fles::SubsystemIdentifier::TRD2D);
  static constexpr std::uint16_t fkFlesTof   = static_cast<std::uint16_t>(fles::SubsystemIdentifier::RPC);
  static constexpr std::uint16_t fkFlesRich  = static_cast<std::uint16_t>(fles::SubsystemIdentifier::RICH);
  static constexpr std::uint16_t fkFlesPsd   = static_cast<std::uint16_t>(fles::SubsystemIdentifier::PSD);


  /// Control flags
  Bool_t fbIgnoreOverlapMs       = false;  //! Ignore Overlap Ms: all fuOverlapMsNb MS at the end of timeslice
  Bool_t fbComponentsAddedToList = kFALSE;

  /** @brief Flag if extended debug output is to be printed or not*/
  bool fDoDebugPrints = false;  //!
  /** @brief Flag if performance profiling should be activated or not.*/
  bool fDoPerfProf = false;  //!
  /** @brief Flag to Enable/disable a full time sorting. If off, time sorting happens per link/FLIM source */
  bool fbOutputFullTimeSorting = false;

  /// User settings parameters
  std::string fsSetupName             = "mcbm_beam_2021_07";
  uint32_t fuRunId                    = 1588;
  std::string fsChannelNameDataInput  = "fullts";
  std::string fsChannelNameDataOutput = "unpts_0";
  std::string fsChannelNameCommands   = "commands";
  UInt_t fuDigiMaskedIdT0             = 0x00005006;
  UInt_t fuDigiMaskId                 = 0x0001FFFF;

  /// List of MQ channels names
  std::vector<std::string> fsAllowedChannels = {fsChannelNameDataInput};

  /// Parameters management
  //      TList* fParCList = nullptr;
  Bool_t InitParameters(std::vector<std::pair<std::string, std::shared_ptr<FairParGenericSet>>>* reqparvec);

  /// Statistics & first TS rejection
  uint64_t fulNumMessages = 0;
  uint64_t fulTsCounter   = 0;
  /** @brief Map to store a name for the unpackers and the processed amount of digis, key = fkFlesId*/
  std::map<std::uint16_t, std::pair<std::string, size_t>> fNameMap = {};  //!
  /** @brief Map to store the cpu and wall time, key = fkFlesId*/
  std::map<std::uint16_t, std::pair<double, double>> fTimeMap = {};  //!
  /** @brief Map to store the in and out data amount, key = fkFlesId*/
  std::map<std::uint16_t, std::pair<double, double>> fDataSizeMap = {};  //!

  /// Configuration of the unpackers. Provides the configured algorithm
  std::shared_ptr<CbmStsUnpackConfig> fStsConfig         = nullptr;
  std::shared_ptr<CbmTrdUnpackConfigFasp2D> fTrd2DConfig = nullptr;
  std::shared_ptr<CbmTrdUnpackConfig> fTrd1DConfig       = nullptr;
  std::shared_ptr<CbmTofUnpackConfig> fTofConfig         = nullptr;
  std::shared_ptr<CbmRichUnpackConfig> fRichConfig       = nullptr;
  std::shared_ptr<CbmPsdUnpackConfig> fPsdConfig         = nullptr;

  /// Pointer to the Timeslice header conatining start time and index
  CbmTsEventHeader* fCbmTsEventHeader = nullptr;

  /// Time offsets
  std::vector<std::string> fvsSetTimeOffs = {};

  /// TS MetaData storage: stable so should be moved somehow to parameters handling (not transmitted with each TS
  size_t fuNbCoreMsPerTs    = 0;     //!
  size_t fuNbOverMsPerTs    = 0;     //!
  Double_t fdMsSizeInNs     = 0;     //! Size of a single MS, [nanoseconds]
  Double_t fdTsCoreSizeInNs = -1.0;  //! Total size of the core MS in a TS, [nanoseconds]
  Double_t fdTsOverSizeInNs = -1.0;  //! Total size of the overlap MS in a TS, [nanoseconds]
  Double_t fdTsFullSizeInNs = -1.0;  //! Total size of all MS in a TS, [nanoseconds]
  TimesliceMetaData* fTsMetaData;

  bool IsChannelNameAllowed(std::string channelName);
  Bool_t InitContainers();
  Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);
  void Finish();
  bool SendUnpData();

  std::shared_ptr<CbmTrdSpadic> GetTrdSpadic(bool useAvgBaseline);

  /** @brief Sort a vector timewise vector type has to provide GetTime() */
  template<typename TVecobj>
  typename std::enable_if<std::is_same<TVecobj, std::nullptr_t>::value == true, void>::type
  timesort(std::vector<TVecobj>* /*vec = nullptr*/)
  {
    LOG(debug) << "CbmDeviceUnpack::timesort() got an object that has no member function GetTime(). Hence, we can and "
                  "will not timesort it!";
  }

  template<typename TVecobj>
  typename std::enable_if<!std::is_member_function_pointer<decltype(&TVecobj::GetTime)>::value, void>::type
  timesort(std::vector<TVecobj>* /*vec = nullptr*/)
  {
    LOG(debug) << "CbmDeviceUnpack::timesort() " << TVecobj::Class_Name()
               << "is  an object that has no member function GetTime(). Hence, we can and "
                  "will not timesort it!";
  }

  template<typename TVecobj>
  typename std::enable_if<std::is_member_function_pointer<decltype(&TVecobj::GetTime)>::value, void>::type
  timesort(std::vector<TVecobj>* vec = nullptr)
  {
    if (vec == nullptr) return;
    std::sort(vec->begin(), vec->end(),
              [](const TVecobj& a, const TVecobj& b) -> bool { return a.GetTime() < b.GetTime(); });
  }

  /**
   * @brief Template for the unpacking call of a given algorithm.
   *
   * @tparam TAlgo Algorithm to be called
   * @tparam TOutput Output element types
   * @tparam TOptoutputs Optional output element types
   * @param ts Timeslice
   * @param icomp Component number
   * @param algo Algorithm to be used for this component
   * @param outtargetvec Target vector for the output elements
   * @param optoutputvecs Target vectors for optional outputs
   * @return std::pair<ndigis, std::pair<cputime, walltime>>
  */
  template<class TConfig, class TOptOutA = std::nullptr_t, class TOptOutB = std::nullptr_t>
  size_t unpack(const std::uint16_t subsysid, const fles::Timeslice* ts, std::uint16_t icomp, TConfig config,
                std::vector<TOptOutA>* optouttargetvecA = nullptr, std::vector<TOptOutB>* optouttargetvecB = nullptr)
  {

    auto wallstarttime        = std::chrono::high_resolution_clock::now();
    std::clock_t cpustarttime = std::clock();

    auto algo                        = config->GetUnpacker();
    std::vector<TOptOutA> optoutAvec = {};
    std::vector<TOptOutB> optoutBvec = {};
    if (optouttargetvecA) { algo->SetOptOutAVec(&optoutAvec); }
    if (optouttargetvecB) { algo->SetOptOutBVec(&optoutBvec); }

    // Set the start time of the current TS for this algorithm
    algo->SetTsStartTime(ts->start_time());

    // Run the actual unpacking
    auto digivec = algo->Unpack(ts, icomp);

    // Check if we want to write the output to somewhere (in pure online monitoring mode for example this can/would/should be skipped)
    if (config->GetOutputVec()) {
      // Lets do some time-sorting if we are not doing it later
      if (!fbOutputFullTimeSorting) timesort(&digivec);

      // Transfer the data from the timeslice vector to the target branch vector
      // Digis/default output retrieved as offered by the algorithm
      for (auto digi : digivec)
        config->GetOutputVec()->emplace_back(digi);
    }
    if (optouttargetvecA) {
      // Lets do some timesorting
      if (!fbOutputFullTimeSorting) timesort(&optoutAvec);
      // Transfer the data from the timeslice vector to the target branch vector
      for (auto optoutA : optoutAvec)
        optouttargetvecA->emplace_back(optoutA);
    }
    if (optouttargetvecB) {
      // Second opt output is not time sorted to allow non GetTime data container.
      // Lets do some timesorting
      timesort(&optoutAvec);
      // Transfer the data from the timeslice vector to the target branch vector
      for (auto optoutB : optoutBvec)
        optouttargetvecB->emplace_back(optoutB);
    }

    std::clock_t cpuendtime = std::clock();
    auto wallendtime        = std::chrono::high_resolution_clock::now();

    // Cpu time in [µs]
    auto cputime = 1e6 * (cpuendtime - cpustarttime) / CLOCKS_PER_SEC;
    algo->AddCpuTime(cputime);
    // Real time in [µs]
    auto walltime = std::chrono::duration<double, std::micro>(wallendtime - wallstarttime).count();
    algo->AddWallTime(walltime);


    // Check some numbers from this timeslice
    size_t nDigis = digivec.size();
    LOG(debug) << "Component " << icomp << " connected to config " << config->GetName() << "   n-Digis " << nDigis
               << " processed in walltime(cputime) = " << walltime << "(" << cputime << cputime << ") µs"
               << "this timeslice.";

    if (fDoPerfProf) {
      auto timeit = fTimeMap.find(subsysid);
      timeit->second.first += cputime;
      timeit->second.second += walltime;

      auto datait = fDataSizeMap.find(subsysid);
      datait->second.first += ts->size_component(icomp) / 1.0e6;
      datait->second.second += nDigis * algo->GetOutputObjSize() / 1.0e6;

      fNameMap.find(subsysid)->second.second += nDigis;
    }

    return nDigis;
  }
};

#endif /* CBMDEVICEMCBMUNPACK_H_ */
