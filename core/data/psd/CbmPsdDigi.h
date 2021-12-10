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

#ifndef NO_ROOT
#include <Rtypes.h>  // for ClassDefNV
#endif

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

#include <cstdint>
#include <string>  // for string


class CbmPsdDigi {

public:
  /**@brief Default constructor.
       **/
  CbmPsdDigi() {}


  /** @brief Constructor with assignment
       ** @param address Unique channel address
       ** @param time    Time [ns]
       ** @param edep    Energy deposition
       **/
  CbmPsdDigi(uint32_t address, double time, double edep) : fuAddress(address), fdTime(time), fdEdep(edep) {}


  /** @brief Constructor with detailed assignment.
       ** @param moduleID      Module Identifier
       ** @param sectionID     Section Identifier
       ** @param time          Time [ns]
       ** @param edep          Energy deposition
       **/
  CbmPsdDigi(uint32_t moduleId, uint32_t sectionId, double time, double edep) : fuAddress(0), fdTime(time), fdEdep(edep)
  {
    fuAddress = CbmPsdAddress::GetAddress(moduleId, sectionId);
  }


  /**  Copy constructor **/
  CbmPsdDigi(const CbmPsdDigi&);


  /** Move constructor  **/
  CbmPsdDigi(CbmPsdDigi&&);


  /** Assignment operator  **/
  CbmPsdDigi& operator=(const CbmPsdDigi&) = default;


  /** Move Assignment operator  **/
  CbmPsdDigi& operator=(CbmPsdDigi&&) = default;


  /** Destructor **/
  ~CbmPsdDigi() {}


  /** @brief Class name (static)
       ** @return CbmPsdDigi
       **/
  static const char* GetClassName() { return "CbmPsdDigi"; }


  /** @brief Address
       ** @return Unique channel address (see CbmPsdAddress)
       **/
  uint32_t GetAddress() const { return fuAddress; };


  /** @brief Get the desired name of the branch for this obj in the cbm output tree  (static)
   ** @return "PsdDigi"
   **/
  static const char* GetBranchName() { return "PsdDigi"; }


  /** @brief Time
       ** @return Time [ns]
       **/
  double GetTime() const { return fdTime; };


  /** @brief Charge
       ** @return Charge (energy deposition)
       **
       ** Alias for GetEdep(), for compatibility with template methods
       */
  double GetCharge() const { return fdEdep; };


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


  /** @brief String output
       ** @return Info
       **/
  std::string ToString() const;


private:
  uint32_t fuAddress = 0;    /// Unique channel address
  double fdTime      = -1.;  /// Time of measurement [ns]
  double fdEdep      = 0.;   /// Energy deposition from FPGA [MeV]

  /// BOOST serialization interface
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int /*version*/)
  {
    ar& fuAddress;
    ar& fdTime;
    ar& fdEdep;
  }

#ifndef NO_ROOT
  ClassDefNV(CbmPsdDigi, 5);
#endif
};

#endif  // CBMPSDDIGI_H
