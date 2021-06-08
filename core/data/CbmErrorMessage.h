/* Copyright (C) 2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

/** @file CbmErrorMessage.h
 ** @author Pierre-Alain Loizeau <p.-a.loizeau@gsi.de>
 ** @date 19.02.2020
 **/


#ifndef CBMERRORMESSAGE_H
#define CBMERRORMESSAGE_H 1

/// CbmRoot (+externals) headers
#include "CbmDefs.h"

/// FairRoot headers

/// Fairsoft (Root, Boost, ...) headers
#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for UInt_t, Double_t, Int_t

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

/// C/C++ headers
#include <memory>  // for unique_ptr
#include <string>  // for string

/** @class CbmErrorMessage
 ** @brief Base class for persistent representation of error messages information.
 ** @author Pierre-Alain Loizeau <p.-a.loizeau@gsi.de>
 ** @since 19.02.2020
 ** @version 19.02.2020
 **
 ** CbmErrorMessage is a base class for the ROOT representation of
 ** the error message delivered by the detector readout chains.
 ** The available information fields are the system ID, the time stamp,
 ** an origin index (address), the flags and (optionally) a payload.
 **
 ** The base class only provides bulk setters/getters, with the idea that
 ** derived class will provide direct accessors to specific flags/payloads.
 **/
class CbmErrorMessage {

public:
  /** Default constructor  **/
  CbmErrorMessage() {};

  /**
       * \brief Standard constructor.
       * \param[in] sysId    System ID from ECbmModuleId enum.
       * \param[in] dTime    Error time [ns].
       * \param[in] uAddress Some address for the error source.
       * \param[in] uFlags   Flags/error pattern, 32b available.
       * \param[in] uPayload Optional error payload, 32b available.
       **/
  CbmErrorMessage(ECbmModuleId sysId, Double_t dTime, UInt_t uAddress, UInt_t uFlags, UInt_t uPayload = 0);


  /** Destructor  **/
  ~CbmErrorMessage();


  /** @brief Class name (static)
      ** @return CbmErrorMessage
      **/
  static const char* GetClassName() { return "CbmErrorMessage"; }


  /** @brief System (enum DetectorId) **/
  ECbmModuleId GetSystemId() const { return fModuleId; }


  /** @brief Absolute time [ns]  **/
  Double_t GetTime() const { return fdTime; }


  /** @brief Origin address  **/
  UInt_t GetAddress() const { return fuAddress; }


  /** @brief Flags (bitfield)  **/
  UInt_t GetFlags() const { return fuFlags; }


  /** @brief Payload (optional)  **/
  UInt_t GetPayload() const { return fuPayload; }


  /** @brief Output information **/
  std::string ToString() const;


  template<class Archive>
  void serialize(Archive& ar, const unsigned int /*version*/)
  {
    ar& fModuleId;
    ar& fdTime;
    ar& fuAddress;
    ar& fuFlags;
    ar& fuPayload;
  }

private:
  friend class boost::serialization::access;

  ECbmModuleId fModuleId = ECbmModuleId::kLastModule;
  Double_t fdTime        = -1.0;
  UInt_t fuAddress       = 0;
  UInt_t fuFlags         = 0;
  UInt_t fuPayload       = 0;


  ClassDefNV(CbmErrorMessage, 1);
};

#endif
