/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer], Pierre-Alain Loizeau, Pascal Raisig  */

/** @file CbmRecoUnpack.h
 ** @copyright Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
 ** @license SPDX-License-Identifier: GPL-3.0-only
 ** @author Volker Friese, Pascal Raisig
 **/


#ifndef CBMRECOUNPACK_H
#define CBMRECOUNPACK_H 1

#include "CbmPsdUnpackConfig.h"
#include "CbmRichUnpackConfig.h"
#include "CbmStsUnpackConfig.h"
#include "CbmTofUnpackConfig.h"
#include "CbmTrdUnpackConfig.h"
#include "CbmTrdUnpackFaspConfig.h"
#include "CbmTsEventHeader.h"

#include <MicrosliceDescriptor.hpp>
#include <Timeslice.hpp>

#include <FairParAsciiFileIo.h>
#include <FairRootManager.h>

#include <RtypesCore.h>
#include <THnSparse.h>
#include <TObject.h>
#include <type_traits>  // this is std::lib used for template is_member_function_pointer

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <ctime>
#include <memory>
#include <utility>
#include <vector>


/** @class CbmRecoUnpack
 ** @brief Main steering class for unpacking in cbmroot
 ** @author Volker Friese
 ** @since 2 June 2021
 **
 ** This class runs the unpackers for each input time-slice component.
 **/
class CbmRecoUnpack : public TObject {

public:
  /** @brief Constructor
   **
   */
  CbmRecoUnpack();


  /** @brief Destructor **/
  ~CbmRecoUnpack() {};

  /** @brief Copy constructor - not implemented **/
  CbmRecoUnpack(const CbmRecoUnpack&) = delete;

  /** @brief Assignment operator - not implemented **/
  CbmRecoUnpack& operator=(const CbmRecoUnpack&) = delete;

  /** @brief Actions at the end of the run **/
  void Finish();

  /** @brief Initialisation **/
  Bool_t Init();

  /** @brief Clear the output vectors as preparation of the next timeslice. Called via FairSource::Reset() */
  void Reset();

  /**
   * @brief Set the Debug Printout Flag
   *
   * @param value
  */
  void SetDebugPrintout(bool value = true) { fDoDebugPrints = value; }

  /**
   * @brief (De)Activate the performance profiling based on histograms
   *
   * @param value
  */
  void SetDoPerfProfiling(bool value = true) { fDoPerfProf = value; }

  /**
   * @brief Set the performance profiling Output Filename
   *
   * @param value
  */
  void SetOutputFilename(std::string value)
  {
    if (!value.empty()) fOutfilename = value;
  }

  /**
   * @brief Enable/disable a full time sorting. If off, time sorting happens per link/FLIM source
   *
   * @param value
  */
  void SetTimeSorting(bool bIn = true) { bOutputFullTimeSorting = bIn; }

  /** @brief Set the Psd Unpack Config @param config */
  void SetUnpackConfig(std::shared_ptr<CbmPsdUnpackConfig> config) { fPsdConfig = config; }

  /** @brief Set the Rich Unpack Config @param config */
  void SetUnpackConfig(std::shared_ptr<CbmRichUnpackConfig> config) { fRichConfig = config; }

  /** @brief Set the Sts Unpack Config @param config */
  void SetUnpackConfig(std::shared_ptr<CbmStsUnpackConfig> config) { fStsConfig = config; }

  // /** @brief Set the Tof Unpack Config @param config */
  void SetUnpackConfig(std::shared_ptr<CbmTofUnpackConfig> config) { fTofConfig = config; }

  /** @brief Set the Trd Unpack Config @param config */
  void SetUnpackConfig(std::shared_ptr<CbmTrdUnpackConfig> config) { fTrd1DConfig = config; }

  /** @brief Set the Trd2D Unpack Config @param config */
  void SetUnpackConfig(std::shared_ptr<CbmTrdUnpackFaspConfig> config) { fTrd2DConfig = config; }

  /** @brief Trigger the unpacking procedure **/
  void Unpack(std::unique_ptr<fles::Timeslice> ts);

private:
  static constexpr std::uint16_t fkFlesMvd   = static_cast<std::uint16_t>(fles::SubsystemIdentifier::MVD);
  static constexpr std::uint16_t fkFlesSts   = static_cast<std::uint16_t>(fles::SubsystemIdentifier::STS);
  static constexpr std::uint16_t fkFlesRich  = static_cast<std::uint16_t>(fles::SubsystemIdentifier::RICH);
  static constexpr std::uint16_t fkFlesPsd   = static_cast<std::uint16_t>(fles::SubsystemIdentifier::PSD);
  static constexpr std::uint16_t fkFlesMuch  = static_cast<std::uint16_t>(fles::SubsystemIdentifier::MUCH);
  static constexpr std::uint16_t fkFlesTrd   = static_cast<std::uint16_t>(fles::SubsystemIdentifier::TRD);
  static constexpr std::uint16_t fkFlesTrd2D = static_cast<std::uint16_t>(fles::SubsystemIdentifier::TRD2D);
  static constexpr std::uint16_t fkFlesTof   = static_cast<std::uint16_t>(fles::SubsystemIdentifier::RPC);

  /** @brief Flag if extended debug output is to be printed or not*/
  bool fDoDebugPrints = false;  //!

  /** @brief Flag if performance profiling should be activated or not.*/
  bool fDoPerfProf = false;  //!

  /** @brief Map to store a name for the unpackers and the processed amount of digis, key = fkFlesId*/
  std::map<std::uint16_t, std::pair<std::string, size_t>> fNameMap = {};  //!

  /** @brief Map to store the cpu and wall time, key = fkFlesId*/
  std::map<std::uint16_t, std::pair<double, double>> fTimeMap = {};  //!

  /** @brief Map to store the in and out data amount, key = fkFlesId*/
  std::map<std::uint16_t, std::pair<double, double>> fDataSizeMap = {};  //!

  /** @brief Run the performance profiling based on the fTimeMap and fDataSizeMap members. */
  void performanceProfiling();

  /**
   * @brief Init the performance profiling maps for a given unpacker
   *
   * @param subsysid Subsystem Identifier casted to std::uint16_t
   * @param name Name of the unpacker
  */
  void initPerformanceMaps(std::uint16_t subsysid, std::string name);


  template<class TConfig>
  void RegisterOutputs(FairRootManager* ioman, std::shared_ptr<TConfig> config)
  {
    /*
 * FIXME: Compiling but leading to a segfault at runtime
    auto pOutVect = config->GetOutputVec();
    if (pOutVect) {
      ioman->RegisterAny(config->GetOutputBranchName().data(), pOutVect, kTRUE);
      LOG(info) << "CbmRecoUnpack::RegisterOutputs() " << config->GetOutputBranchName() << " at " << pOutVect;
    }

    auto pOutVectOptA = config->GetOptOutAVec();
    if (pOutVectOptA) {
      ioman->RegisterAny(config->GetOutputBranchNameOptA().data(), pOutVectOptA, kTRUE);
      LOG(info) << "CbmRecoUnpack::RegisterOutputs() " << config->GetOutputBranchNameOptA() << " at " << pOutVectOptA;
    }

    auto pOutVectOptB = config->GetOptOutBVec();
    if (pOutVectOptB) {
      ioman->RegisterAny(config->GetOutputBranchNameOptB().data(), pOutVectOptB, kTRUE);
      LOG(info) << "CbmRecoUnpack::RegisterOutputs() " << config->GetOutputBranchNameOptB() << " at " << pOutVectOptB;
    }
*/
    /// Alternative which compiles and run but forces to keep a Fairroot dependency in the Config template
    config->RegisterOutput(ioman);
  }

  /**
   * @brief Initialise the parameter containers requested by the algorithm
   *
   * @return Bool_t initOk
  */
  virtual Bool_t initParContainers(std::vector<std::pair<std::string, std::shared_ptr<FairParGenericSet>>>* reqparvec)
  {
    LOG(debug) << GetName() << "::Init - initParContainers";
    if (!reqparvec) {
      LOG(info) << GetName() << "::Init - initParContainers - empty requirements vector no parameters initialized.";
      return kTRUE;
    }

    // Now get the actual ascii files and init the containers with the asciiIo
    for (auto& pair : *reqparvec) {
      auto filepath = pair.first;
      auto parset   = pair.second;
      FairParAsciiFileIo asciiInput;
      if (!filepath.empty()) {
        if (asciiInput.open(filepath.data())) { parset->init(&asciiInput); }
      }
    }
    return kTRUE;
  }

  /** @brief Sort a vector timewise vector type has to provide GetTime() */
  template<typename TVecobj>
  typename std::enable_if<std::is_same<TVecobj, std::nullptr_t>::value == true, void>::type
  timesort(std::vector<TVecobj>* /*vec = nullptr*/)
  {
    LOG(debug) << "CbmRecoUnpack::timesort() got an object that has no member function GetTime(). Hence, we can and "
                  "will not timesort it!";
  }

  template<typename TVecobj>
  typename std::enable_if<!std::is_member_function_pointer<decltype(&TVecobj::GetTime)>::value, void>::type
  timesort(std::vector<TVecobj>* /*vec = nullptr*/)
  {
    LOG(debug) << "CbmRecoUnpack::timesort() " << TVecobj::Class_Name()
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
      if (!bOutputFullTimeSorting) timesort(&digivec);

      // Transfer the data from the timeslice vector to the target branch vector
      // Digis/default output retrieved as offered by the algorithm
      for (auto digi : digivec)
        config->GetOutputVec()->emplace_back(digi);
    }
    if (optouttargetvecA) {
      // Lets do some timesorting
      if (!bOutputFullTimeSorting) timesort(&optoutAvec);
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
  // ----------------------------------------------------------------------------

  /** @brief Configuration of the Psd unpacker. Provides the configured algorithm */
  std::shared_ptr<CbmPsdUnpackConfig> fPsdConfig = nullptr;  //!

  /** @brief Configuration of the Rich unpacker. Provides the configured algorithm */
  std::shared_ptr<CbmRichUnpackConfig> fRichConfig = nullptr;  //!

  /** @brief Configuration of the Sts unpacker. Provides the configured algorithm */
  std::shared_ptr<CbmStsUnpackConfig> fStsConfig = nullptr;  //!

  /** @brief Configuration of the Tof unpacker. Provides the configured algorithm */
  std::shared_ptr<CbmTofUnpackConfig> fTofConfig = nullptr;  //!

  /** @brief Configuration of the Trd unpacker. Provides the configured algorithm */
  std::shared_ptr<CbmTrdUnpackConfig> fTrd1DConfig = nullptr;  //!

  /** @brief Configuration of the Trd unpacker. Provides the configured algorithm */
  std::shared_ptr<CbmTrdUnpackFaspConfig> fTrd2DConfig = nullptr;  //!

  /** @brief Pointer to the Timeslice start time used to write it to the output tree @remark since we hand this to the FairRootManager it also wants to delete it and we do not have to take care of deletion */
  CbmTsEventHeader* fCbmTsEventHeader = nullptr;

  /** @brief Name of the performance profiling output file */
  std::string fOutfilename = "CbmRecoUnpack.perf.root";

  /** @brief Flag to Enable/disable a full time sorting. If off, time sorting happens per link/FLIM source */
  bool bOutputFullTimeSorting = false;


  ClassDef(CbmRecoUnpack, 1);
};


#endif
