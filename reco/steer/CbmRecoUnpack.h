/** @file CbmRecoUnpack.h
 ** @copyright Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
 ** @license SPDX-License-Identifier: GPL-3.0-only
 ** @author Volker Friese, Pascal Raisig
 **/


#ifndef CBMRECOUNPACK_H
#define CBMRECOUNPACK_H 1

#include "CbmTrdUnpackConfig.h"

#include <MicrosliceDescriptor.hpp>
#include <Timeslice.hpp>

#include <TObject.h>
#include <type_traits>  // this is std::lib used for template is_member_function_pointer

#include <cstddef>
#include <memory>
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
   ** @param fileName  Name of (single) input file.
   **
   ** More input files can be added by the method AddInputFile.
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
   * @brief Set the Trd Unpack Config
   * 
   * @param config 
  */
  void SetUnpackConfig(std::shared_ptr<CbmTrdUnpackConfig> config) { fTrdConfig = config; }

  /** @brief Trigger the unpacking procedure **/
  void Unpack(std::unique_ptr<fles::Timeslice> ts);

private:
  static constexpr std::uint16_t fkFlesMvd  = static_cast<std::uint16_t>(fles::SubsystemIdentifier::MVD);
  static constexpr std::uint16_t fkFlesSts  = static_cast<std::uint16_t>(fles::SubsystemIdentifier::STS);
  static constexpr std::uint16_t fkFlesRich = static_cast<std::uint16_t>(fles::SubsystemIdentifier::RICH);
  static constexpr std::uint16_t fkFlesMuch = static_cast<std::uint16_t>(fles::SubsystemIdentifier::MUCH);
  static constexpr std::uint16_t fkFlesTrd  = static_cast<std::uint16_t>(fles::SubsystemIdentifier::TRD);
  static constexpr std::uint16_t fkFlesTof  = static_cast<std::uint16_t>(fles::SubsystemIdentifier::RPC);
  static constexpr std::uint16_t fkFlesPsd  = static_cast<std::uint16_t>(fles::SubsystemIdentifier::PSD);

  /** @brief Flag if extended debug output is to be printed or not*/
  bool fDoDebugPrints = false;

  /** @brief Sort a vector timewise vector type has to provide GetTime() */
  template<typename TVecobj>
  typename std::enable_if<std::is_member_function_pointer<decltype(&TVecobj::GetTime)>::value, void>::type
  timesort(std::vector<TVecobj>* vec = nullptr)
  {
    if (vec == nullptr) return;
    std::sort(vec->begin(), vec->end(),
              [](const TVecobj& a, const TVecobj& b) -> bool { return a.GetTime() < b.GetTime(); });
  }

  template<typename TVecobj>
  typename std::enable_if<!std::is_member_function_pointer<decltype(&TVecobj::GetTime)>::value, void>::type
  timesort(std::vector<TVecobj>* /*vec = nullptr*/)
  {
    LOG(debug) << "CbmRecoUnpack::timesort() object " << TVecobj::Class_Name()
               << " has no member function GetTime(). Hence, we can and will not timesort it!";
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
  */
  template<class TConfig, class TOptOutA = std::nullptr_t, class TOptOutB = std::nullptr_t>
  void unpack(const fles::Timeslice* ts, std::uint16_t icomp, TConfig config,
              std::vector<TOptOutA>* optouttargetvecA = nullptr, std::vector<TOptOutB>* optouttargetvecB = nullptr)
  {
    auto algo                        = config->GetUnpacker();
    std::vector<TOptOutA> optoutAvec = {};
    std::vector<TOptOutB> optoutBvec = {};
    if (optouttargetvecA) { algo->SetOptOutAVec(&optoutAvec); }
    if (optouttargetvecB) { algo->SetOptOutBVec(&optoutBvec); }

    // Run the actual unpacking
    auto digivec = algo->Unpack(ts, icomp);

    // Check if we want to write the output to somewhere (in pure online monitoring mode for example this can/would/should be skipped)
    if (config->GetOutputVec()) {
      // Lets do some timesorting
      timesort(&digivec);

      // Transfer the data from the timeslice vector to the target branch vector
      // Digis/default output retrieved as offered by the algorithm
      for (auto digi : digivec)
        config->GetOutputVec()->emplace_back(digi);
    }
    if (optouttargetvecA) {
      // Lets do some timesorting
      timesort(&optoutAvec);
      // Transfer the data from the timeslice vector to the target branch vector
      for (auto optoutA : optoutAvec)
        optouttargetvecA->emplace_back(optoutA);
    }
    if (optouttargetvecB) {
      // Second opt output is not time sorted to allow non GetTime data container.
      // Transfer the data from the timeslice vector to the target branch vector
      for (auto optoutB : optoutBvec)
        optouttargetvecB->emplace_back(optoutB);
    }


    // Check some numbers from this timeslice
    size_t nDigis = digivec.size();

    LOG(info) << "Component " << icomp << " connected to config " << config->GetName() << "   n-Digis " << nDigis
              << " processed in this timeslice.";
  }
  // ----------------------------------------------------------------------------


  /** @brief Configuration of the Trd unpacker. Provides the configured algorithm */
  std::shared_ptr<CbmTrdUnpackConfig> fTrdConfig = nullptr;  //!


  ClassDef(CbmRecoUnpack, 1);
};


#endif
