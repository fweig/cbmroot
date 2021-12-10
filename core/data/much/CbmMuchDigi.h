/* Copyright (C) 2007-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Mikhail Ryzhinskiy, Florian Uhlig, Volker Friese [committer], Pierre-Alain Loizeau */

/** CbmMuchDigi.h
 **@author M.Ryzhinskiy <m.ryzhinskiy@gsi.de>
 **@since 19.03.07
 **@version 1.0
 **
 **@author Vikas Singhal <vikas@vecc.gov.in>
 **@since 17/05/16
 **@version 2.0
 ** Data class for digital MUCH information
 ** Data level: RAW
 **
 **
 **/


#ifndef CBMMUCHDIGI_H
#define CBMMUCHDIGI_H 1

#include "CbmDefs.h"         // for kMuch
#include "CbmMuchAddress.h"  // for CbmMuchAddress, kMuchModule

#ifndef NO_ROOT
#include <Rtypes.h>      // for ClassDef
#endif

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

#include <cstdint>
#include <string>  // for string


class CbmMuchDigi {
public:
  /** Default Constructor */
  CbmMuchDigi() : fTime(0), fAddress(0), fCharge(0) {}

  /** Standard constructor
	** @param  address  Unique element address
	** @param  time     Measurement time [ns]
	** @param  charge   Charge [ADC units]
 	**/
  CbmMuchDigi(uint32_t address, uint16_t charge = 0, uint64_t time = 0)
    : fTime(time)
    , fAddress(address)
    , fCharge(charge)
  {
  }
  //fMatch will be created in the CbmMuchSignal and should be deleted by that class destructor only.
  //fMatch = new CbmMatch();}

  CbmMuchDigi(CbmMuchDigi* digi);
  CbmMuchDigi(const CbmMuchDigi&);
  CbmMuchDigi& operator=(const CbmMuchDigi&);

  /** Destructor **/
  virtual ~CbmMuchDigi() {}

  /** @brief Charge
         ** @return Return ADC value as charge
         **
         ** Alias for GetAdc, conversion factor should be added if needed.
         ** For compatibility with template methods
         **/
  double GetCharge() const { return fCharge; }

  //GetSystem is required due to CbmDigiManager
  /** System ID (static)
	** @return System identifier (EcbmModuleId)
	**/
  static ECbmModuleId GetSystem() { return ECbmModuleId::kMuch; }


  /** @brief Class name (static)
   ** @return CbmMuchDigi
   **/
  static const char* GetClassName() { return "CbmMuchDigi"; }


  uint16_t GetAdc() const { return fCharge; }


  virtual int32_t GetAddress() const { return static_cast<int32_t>(fAddress); }
  virtual double GetTime() const { return static_cast<double>(fTime); }

  // Setters
  void SetAdc(int32_t adc);
  void SetTime(uint64_t time);
  void SetSaturation(bool saturate) { fSaturationFlag = saturate; }
  void SetAddress(int32_t address) { fAddress = address; }

  // Specially for littrack
  // TODO remove after littrack fix
  int32_t GetDetectorId() const { return CbmMuchAddress::GetElementAddress(GetAddress(), kMuchModule); }
  int32_t GetChannelId() const { return GetAddress(); }
  int32_t GetADCCharge() const { return GetAdc(); }
  int32_t GetDTime() const { return 0; }

  std::string ToString() const { return std::string {""}; }

  template<class Archive>
  void serialize(Archive& ar, const unsigned int /*version*/)
  {
    ar& fAddress;
    ar& fTime;
    ar& fCharge;
    ar& fSaturationFlag;
  }


private:
  friend class boost::serialization::access;

  uint64_t fTime;     // Absolute Time Stamp[ns]
  uint32_t fAddress;  // Unique detector address
  uint16_t fCharge;   // Charge [ADC Units]

  //Below flag has to be set during the CbmMuchDigi Creation only.
  bool fSaturationFlag =
    0;  //If adc value crosses the Maximum Adc value of actual electronics then SaturationFlag will be set.

#ifndef NO_ROOT
  ClassDefNV(CbmMuchDigi, 4);
#endif
};
#endif
