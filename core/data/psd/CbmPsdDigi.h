/* Copyright (C) 2012-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Nikolay Karpushkin, Volker Friese [committer], Pierre-Alain Loizeau */

/** \file CbmPsdDigi.h
 ** \author Nikolay Karpushkin <karpushkin@inr.ru>
 ** \date 09.10.2019
 **/

/** \class CbmPsdDigi
 ** \brief Data class for PSD digital information
 ** \version 1.0
 **
 ** Unique Address:        32 bits following CbmPsdAddress
 ** Time:                  64 bits double
 ** Energy deposition:     64 bits double
 **/

#ifndef CBMPSDDIGI_H
#define CBMPSDDIGI_H 1

#include "CbmDefs.h"        // for ECbmModuleId::kPsd
#include "CbmPsdAddress.h"  // for CbmPsdAddress

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDefNV
#include <RtypesCore.h>  // for Double_t, UInt_t, Int_t

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

#include <string>  // for string


class CbmPsdDigi {

public:
  /**@brief Default constructor.
       **/
  CbmPsdDigi();


  /** @brief Constructor with assignment
       ** @param address Unique channel address
       ** @param edep    Energy deposition
       ** @param time    Time [ns]
       **/
  CbmPsdDigi(UInt_t address, Double_t time, Double_t edep);


  /** @brief Constructor with detailed assignment.
       ** @param moduleID      Module Identifier
       ** @param sectionID     Section Identifier
       ** @param edep          Energy deposition
       ** @param time          Time [ns]
       **/
  CbmPsdDigi(UInt_t moduleId, UInt_t sectionId, Double_t time, Double_t edep);


  /**  Copy constructor **/
  CbmPsdDigi(const CbmPsdDigi&);


  /** Move constructor  **/
  CbmPsdDigi(CbmPsdDigi&&);


  /** Assignment operator  **/
  CbmPsdDigi& operator=(const CbmPsdDigi&);


  /** Move Assignment operator  **/
  CbmPsdDigi& operator=(CbmPsdDigi&&);


  /** Destructor **/
  ~CbmPsdDigi();


  /** @brief Class name (static)
       ** @return CbmPsdDigi
       **/
  static const char* GetClassName() { return "CbmPsdDigi"; }


  /** @brief Address
       ** @return Unique channel address (see CbmPsdAddress)
       **/
  UInt_t GetAddress() const { return fuAddress; };


  /** @brief Time
       ** @return Time [ns]
       **/
  Double_t GetTime() const { return fdTime; };


  /** @brief Charge
       ** @return Charge (energy deposition)
       **
       ** Alias for GetEdep(), for compatibility with template methods
       */
  Double_t GetCharge() const { return fdEdep; };


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


  /** @brief String output
       ** @return Info
       **/
  std::string ToString() const;


  UInt_t   fuAddress = 0;        /// Unique channel address
  Double_t fdTime = -1.;         /// Time of measurement [ns]
  Double_t fdEdep = 0.;          /// Energy deposition from FPGA [MeV]
  UInt_t   fuZL = 0;             /// ZeroLevel from waveform [adc counts] 
  Double_t fdAccum = 0;          /// FPGA FEE Accumulator  
  Double_t fdAdcTime = -1.;      /// Adc time of measurement 

  Double_t fdEdepWfm = 0.;       /// Energy deposition from waveform [MeV]
  Double_t fdAmpl = 0.;          /// Amplitude from waveform [mV]
  UInt_t   fuTimeMax = 0;       /// Time of maximum in waveform [adc samples] 

  Double_t fdFitAmpl = 0.;       /// Amplitude from fit of waveform [mV]
  Double_t fdFitZL = 0.;         /// ZeroLevel from fit of waveform [adc counts] 
  Double_t fdFitEdep = 0.;       /// Energy deposition from fit of waveform [MeV]
  Double_t fdFitR2 = 999.;       /// Quality of waveform fit [] -- good near 0
  Double_t fdFitTimeMax = -1.;   /// Time of maximum in fit of waveform [adc samples] 


  //LEGACY
  Float_t ffFitHarmonic1 = 0.;
  Float_t ffFitHarmonic2 = 0.;
  Float_t ffFitR2 = 999.;
  Float_t ffFitEdep = 0.;
  UInt_t  fuAmpl = 0;


private:

  /// BOOST serialization interface
  friend class boost::serialization::access;
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
    ar& fuTimeMax;

    ar& fdFitAmpl;
    ar& fdFitZL;
    ar& fdFitEdep;
    ar& fdFitR2;
    ar& fdFitTimeMax;
  }

  ClassDefNV(CbmPsdDigi, 5);
};

#endif  // CBMPSDDIGI_H
