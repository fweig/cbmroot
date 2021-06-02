/** @file CbmPsdDigi.h
  * @copyright Copyright (C) 2012-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Nikolay Karpushkin, Volker Friese [committer], Pierre-Alain Loizeau **/

/** \file CbmPsdDigi.h
 ** \author Nikolay Karpushkin <karpushkin@inr.ru>
 ** \date 09.10.2019
 **/

/** \class CbmPsdDigi
 ** \brief Data class for PSD digital information
 ** \version 1.0
 **
 ** The information is encoded into 3*4 bytes (2 Double_t + 1 UInt_t).
 ** Unique Address:        32 bits following CbmPsdAddress
 ** Energy deposition:     32 bits double
 ** Time:                  32 bits double
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
  CbmPsdDigi(UInt_t address, Double_t edep, Double_t time);


  /** @brief Constructor with detailed assignment.
       ** @param moduleID      Module Identifier
       ** @param sectionID     Section Identifier
       ** @param edep          Energy deposition
       ** @param time          Time [ns]
       **/
  CbmPsdDigi(UInt_t moduleId, UInt_t sectionId, Double_t edep, Double_t time);


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


  /** @brief Address
       ** @return Unique channel address (see CbmPsdAddress)
       **/
  Int_t GetAddress() const { return fuAddress; };


  /** @brief Charge
       ** @return Charge (energy deposition)
       **
       ** Alias for GetEdep(), for compatibility with template methods
       */
  Double_t GetCharge() const { return fdEdep; };


  /** @brief Class name (static)
       ** @return CbmPsdDigi
       **/
  static const char* GetClassName() { return "CbmPsdDigi"; }


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


  /** @brief Time
       ** @return Time [ns]
       **/
  Double_t GetTime() const { return fdTime; };


  /** Modifiers **/
  void SetAddress(Int_t address) { fuAddress = address; };
  void SetAddress(UInt_t moduleId, UInt_t sectionId);
  void SetEdep(Double_t edep) { fdEdep = edep; }
  void SetTime(Double_t time) { fdTime = time; }


  /** @brief String output
       ** @return Info
       **/
  std::string ToString() const;

  //additional
  UInt_t GetAmpl() const { return fuAmpl; };
  UInt_t GetZL() const { return fuZL; };
  Double_t GetEdepWfm() const { return fdEdepWfm; };
  void SetAmpl(UInt_t ampl) { fuAmpl = ampl; }
  void SetZL(UInt_t zl) { fuZL = zl; }
  void SetEdepWfm(Double_t edep) { fdEdepWfm = edep; }

  Float_t ffFitHarmonic1 = 0.;
  Float_t ffFitHarmonic2 = 0.;
  Float_t ffFitR2        = 999.;
  Float_t ffFitEdep      = 0.;

private:
  Double_t fdEdep  = 0.;   /// Energy deposition
  Double_t fdTime  = -1.;  /// Time of measurement
  UInt_t fuAddress = 0;    /// Unique channel address

  //additional
  UInt_t fuAmpl      = 0;
  UInt_t fuZL        = 0;
  Double_t fdEdepWfm = 0.;

  /// BOOST serialization interface
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int /*version*/)
  {
    ar& ffFitHarmonic1;
    ar& ffFitHarmonic2;
    ar& ffFitR2;
    ar& ffFitEdep;
    ar& fdEdep;
    ar& fdTime;
    ar& fuAddress;
    ar& fuAmpl;
    ar& fuZL;
    ar& fdEdepWfm;
  }

  ClassDefNV(CbmPsdDigi, 4);
};

#endif  // CBMPSDDIGI_H
