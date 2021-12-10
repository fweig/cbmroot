/* Copyright (C) 2021 Institute for Nuclear Research, Moscow
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Nikolay Karpushkin [committer] */

/** \file CbmPsdDsp.h
 ** \author Nikolay Karpushkin <karpushkin@inr.ru>
 ** \date 09.10.2019
 **/

/** \class CbmPsdDsp
 ** \brief Data class for PSD digital signal processing (DSP)
 ** \version 1.0
 **/

#ifndef CbmPsdDsp_H
#define CbmPsdDsp_H 1

#include "CbmDefs.h"        // for ECbmModuleId::kPsd
#include "CbmPsdAddress.h"  // for CbmPsdAddress

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDefNV

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

#include <cstdint>
#include <string>  // for string


class CbmPsdDsp {

public:
  /**@brief Default constructor.
       **/
  CbmPsdDsp();


  /** @brief Constructor with detailed assignment.
       **/
  CbmPsdDsp(uint32_t address, double time, double ts_time, double edep, uint32_t zl, double accum, double adc_time,
            double edep_wfm, double ampl, uint32_t minimum, uint32_t time_max, std::vector<std::uint16_t> wfm,
            double fit_ampl, double fit_zl, double fit_edep, double fit_r2, double fit_time_max,
            std::vector<std::uint16_t> fit_wfm);


  /**  Copy constructor **/
  CbmPsdDsp(const CbmPsdDsp&);


  /** Move constructor  **/
  CbmPsdDsp(CbmPsdDsp&&);


  /** Assignment operator  **/
  CbmPsdDsp& operator=(const CbmPsdDsp&);


  /** Move Assignment operator  **/
  CbmPsdDsp& operator=(CbmPsdDsp&&);


  /** Destructor **/
  ~CbmPsdDsp();


  /** @brief Class name (static)
       ** @return CbmPsdDsp
       **/
  static const char* GetClassName() { return "CbmPsdDsp"; }


  /** @brief Address
       ** @return Unique channel address (see CbmPsdAddress)
       **/
  uint32_t GetAddress() const { return fuAddress; };


  /** @brief Time
       ** @return Time [ns]
       **/
  double GetTime() const { return fdTime; };


  /** @brief TsTime
       ** @return TsTime [ns]
       **/
  double GetTsTime() const { return fdTsTime; };


  /** @brief Energy deposit
       ** @return Energy deposit
       **/
  double GetEdep() const { return fdEdep; };


  /** @brief Module Identifier
       ** @return Module number
       **/
  double GetModuleID() const { return CbmPsdAddress::GetModuleId(GetAddress()); }


  /** @brief Section Identifier
       ** @return Section number
       **/
  double GetSectionID() const { return CbmPsdAddress::GetSectionId(GetAddress()); }


  /** @brief System identifier
       ** @return System ID (ECbmModuleId)
       **/
  static ECbmModuleId GetSystem() { return ECbmModuleId::kPsd; }


  /** Modifiers **/
  void SetAddress(uint32_t address) { fuAddress = address; };
  void SetAddress(uint32_t moduleId, uint32_t sectionId);
  void SetTime(double time) { fdTime = time; }
  void SetEdep(double edep) { fdEdep = edep; }

  uint32_t fuAddress = 0;    /// Unique channel address
  double fdTime      = -1.;  /// Time of measurement relative to TS [ns]
  double fdTsTime    = -1.;  /// Time of TimeSlice of measurement. Relative to first TS [ns]
  double fdEdep      = 0.;   /// Energy deposition from FPGA [MeV]
  uint32_t fuZL      = 0;    /// ZeroLevel from waveform [adc counts]
  double fdAccum     = 0;    /// FPGA FEE Accumulator
  double fdAdcTime   = -1.;  /// Adc time of measurement

  double fdEdepWfm            = 0.;  /// Energy deposition from waveform [MeV]
  double fdAmpl               = 0.;  /// Amplitude from waveform [mV]
  uint32_t fuMinimum          = 0;   /// Minimum of waveform [adc samples]
  uint32_t fuTimeMax          = 0;   /// Time of maximum in waveform [adc samples]
  std::vector<uint16_t> fuWfm = std::vector<uint16_t>(32, 0);

  double fdFitAmpl               = 0.;    /// Amplitude from fit of waveform [mV]
  double fdFitZL                 = 0.;    /// ZeroLevel from fit of waveform [adc counts]
  double fdFitEdep               = 0.;    /// Energy deposition from fit of waveform [MeV]
  double fdFitR2                 = 999.;  /// Quality of waveform fit [] -- good near 0
  double fdFitTimeMax            = -1.;   /// Time of maximum in fit of waveform [adc samples]
  std::vector<uint16_t> fuFitWfm = std::vector<uint16_t>(32, 0);

  template<class Archive>
  void serialize(Archive& ar, const unsigned int /*version*/)
  {
    ar& fuAddress;
    ar& fdTime;
    ar& fdTsTime;
    ar& fdEdep;
    ar& fuZL;
    ar& fdAccum;
    ar& fdAdcTime;

    ar& fdEdepWfm;
    ar& fdAmpl;
    ar& fuMinimum;
    ar& fuTimeMax;
    ar& fuWfm;

    ar& fdFitAmpl;
    ar& fdFitZL;
    ar& fdFitEdep;
    ar& fdFitR2;
    ar& fdFitTimeMax;
    ar& fuFitWfm;
  }

private:
  /// BOOST serialization interface
  friend class boost::serialization::access;


  ClassDefNV(CbmPsdDsp, 1);
};

#endif  // CbmPsdDsp_H
