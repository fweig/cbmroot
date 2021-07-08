/* Copyright (C) 2012-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Nikolay Karpushkin, Volker Friese [committer], Pierre-Alain Loizeau */

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
#include <RtypesCore.h>  // for Double_t, UInt_t, Int_t

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

#include <string>  // for string


class CbmPsdDsp {

public:
  /**@brief Default constructor.
       **/
  CbmPsdDsp();


  /** @brief Constructor with detailed assignment.
       **/
  CbmPsdDsp(UInt_t address, Double_t time, Double_t edep, UInt_t zl, Double_t accum, Double_t adc_time,
            Double_t edep_wfm, Double_t ampl, UInt_t minimum, UInt_t time_max, std::vector<std::uint16_t> wfm,
            Double_t fit_ampl, Double_t fit_zl, Double_t fit_edep, Double_t fit_r2, Double_t fit_time_max,
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
  UInt_t GetAddress() const { return fuAddress; };


  /** @brief Time
       ** @return Time [ns]
       **/
  Double_t GetTime() const { return fdTime; };


  /** @brief Energy deposit
       ** @return Energy deposit
       **/
  Double_t GetEdep() const { return fdEdep; };


  /** @brief Module Identifier
       ** @return Module number
       **/
  Double_t GetModuleID() const { return CbmPsdAddress::GetModuleId(GetAddress()); }


  /** @brief Section Identifier
       ** @return Section number
       **/
  Double_t GetSectionID() const { return CbmPsdAddress::GetSectionId(GetAddress()); }


  /** @brief System identifier
       ** @return System ID (ECbmModuleId)
       **/
  static ECbmModuleId GetSystem() { return ECbmModuleId::kPsd; }


  /** Modifiers **/
  void SetAddress(UInt_t address) { fuAddress = address; };
  void SetAddress(UInt_t moduleId, UInt_t sectionId);
  void SetTime(Double_t time) { fdTime = time; }
  void SetEdep(Double_t edep) { fdEdep = edep; }

private:
  /// BOOST serialization interface
  friend class boost::serialization::access;

  UInt_t fuAddress   = 0;    /// Unique channel address
  Double_t fdTime    = -1.;  /// Time of measurement [ns]
  Double_t fdEdep    = 0.;   /// Energy deposition from FPGA [MeV]
  UInt_t fuZL        = 0;    /// ZeroLevel from waveform [adc counts]
  Double_t fdAccum   = 0;    /// FPGA FEE Accumulator
  Double_t fdAdcTime = -1.;  /// Adc time of measurement

  Double_t fdEdepWfm          = 0.;  /// Energy deposition from waveform [MeV]
  Double_t fdAmpl             = 0.;  /// Amplitude from waveform [mV]
  UInt_t fuMinimum            = 0;   /// Minimum of waveform [adc samples]
  UInt_t fuTimeMax            = 0;   /// Time of maximum in waveform [adc samples]
  std::vector<uint16_t> fuWfm = std::vector<uint16_t>(32, 0);

  Double_t fdFitAmpl             = 0.;    /// Amplitude from fit of waveform [mV]
  Double_t fdFitZL               = 0.;    /// ZeroLevel from fit of waveform [adc counts]
  Double_t fdFitEdep             = 0.;    /// Energy deposition from fit of waveform [MeV]
  Double_t fdFitR2               = 999.;  /// Quality of waveform fit [] -- good near 0
  Double_t fdFitTimeMax          = -1.;   /// Time of maximum in fit of waveform [adc samples]
  std::vector<uint16_t> fuFitWfm = std::vector<uint16_t>(32, 0);

  template<class Archive>
  void serialize(Archive& ar, const unsigned int /*version*/)
  {
    ar& fuAddress;
    ar& fdTime;
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

  ClassDefNV(CbmPsdDsp, 1);
};

#endif  // CbmPsdDsp_H
