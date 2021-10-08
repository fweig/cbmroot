/* Copyright (C) 2006-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese, Denis Bertini [committer], Florian Uhlig */

/** @file CbmStsDigi.h
 ** @author V.Friese <v.friese@gsi.de>
 ** @since 28.08.2006
 ** @version 6
 **/


#ifndef CBMSTSDIGI_H
#define CBMSTSDIGI_H 1

#include "CbmDefs.h"  // for ECbmModuleId::kSts

#ifndef NO_ROOT
#include <Rtypes.h>  // for ClassDef
#endif

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

#include <cassert>
#include <string>  // for string

/** @class CbmStsDigi
 ** @brief Data class for a single-channel message in the STS
 **
 ** The CbmStsDigi is the ROOT representation of the smallest information
 ** unit delivered by the CBM-STS by a single readout channel. It carries
 ** the channel address, the measurement time and the digitised charge
 ** as information.
 **/
class CbmStsDigi {

public:
  /** Default constructor **/
  CbmStsDigi() {}


  /** Standard constructor
   ** @param  address  Unique element address
   ** @param  channel  Channel number
   ** @param  time     Measurement time [ns]
   ** @param  charge   Charge [ADC units]
   **/
  CbmStsDigi(int32_t address, int32_t channel, uint64_t time, uint16_t charge)
    : fTime(time)
    , fAddress(address)
    , fChannel(channel)
    , fCharge(charge)
  {
  }


  /** Destructor **/
  ~CbmStsDigi() {};


  /** Unique detector element address  (see CbmStsAddress)
   ** @value Unique address of readout channel
   **/
  int32_t GetAddress() const { return fAddress; }


  /** @brief Get the desired name of the branch for this obj in the cbm output tree  (static)
   ** @return "StsDigi"
   **/
  static const char* GetBranchName() { return "StsDigi"; }


  /** @brief Channel number in module
   ** @value Channel number
   **/
  uint16_t GetChannel() const { return fChannel; }


  /** @brief Class name (static)
   ** @return CbmStsDigi
   **/
  static const char* GetClassName() { return "CbmStsDigi"; }


  /** Charge
   ** @value Charge [ADC units]
   **/
  double GetCharge() const { return double(fCharge); }


  /** System ID (static)
  ** @return System identifier (EcbmModuleId)
  **/
  static ECbmModuleId GetSystem() { return ECbmModuleId::kSts; }


  /** Time of measurement
   ** @value Time [ns]
   **/
  double GetTime() const { return double(fTime); }


  template<class Archive>
  void serialize(Archive& ar, const unsigned int /*version*/)
  {
    ar& fTime;
    ar& fAddress;
    ar& fChannel;
    ar& fCharge;
  }


  /** Update Time of measurement
   ** @param New Time [ns]
   **/
  void SetTime(double dNewTime)
  {
    // Future versions of StsDigi won't be able to store negative timestamps.
    assert(dNewTime >= 0);
    fTime = dNewTime;
  }


  /** String output **/
  std::string ToString() const;


private:
  friend class boost::serialization::access;

  int64_t fTime     = 0.;  ///< Time [ns]
  int32_t fAddress  = 0;   ///< Unique element address
  uint16_t fChannel = 0;   ///< Channel number
  uint16_t fCharge  = 0;   ///< Charge [ADC units]


#ifndef NO_ROOT
  ClassDefNV(CbmStsDigi, 7);
#endif
};


#endif
