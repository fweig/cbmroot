/* Copyright (C) 2013-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau, Florian Uhlig [committer], Volker Friese */

/** CbmTofDigi.h
 ** @author Pierre-Alain Loizeau <loizeau@physi.uni-heidelberg.de>
 ** @date 07.06.2013
 **/

/** @class CbmTofDigi
 ** @brief Data class for expanded digital TOF information
 ** @brief Data level: TDC CALIB
 ** @version 1.0
 **
 ** The information is encoded into 3*4 bytes (2 double + 1 uint32_t).
 ** Unique Address:                32 bits following CbmTofAddress
 ** Calibrated Time [ps]:          32 bits double
 ** Calibrated Tot  [ps]:          32 bits double
 **
 ** In triggered setup, the time is relative to the trigger time, which
 ** is measured with a resolution of a few ns corresponding to the TDC
 ** system clock cycle.
 ** In free-streaming setups, the time is relative to the last epoch.
 **/

#ifndef CBMTOFDIGI_H
#define CBMTOFDIGI_H 1

#include "CbmDefs.h"        // for kTof
#include "CbmTofAddress.h"  // for CbmTofAddress

#ifndef NO_ROOT
#include <Rtypes.h>      // for ClassDef
#endif

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

#include <cstdint>
#include <string>  // for string

//class CbmMatch;
#ifndef CBMBMONDIGI_H
class CbmBmonDigi;  // For declaration of the conversion constructor without starting a cyclic dependency
#endif              /* CBMBMONDIGI_H */

class CbmTofDigi {
public:
  /**
       ** @brief Default constructor.
       **/
  CbmTofDigi();

  /**
       ** @brief Constructor with assignment.
       ** @param[in] address Unique channel address. (cf CbmTofAddress)
       ** @param[in] time    Absolute time [ps].
       ** @param[in] tot     Time Over Threshold [ps].
       **/
  CbmTofDigi(uint32_t address, double time, double tot);

  /**
       ** @brief Constructor with detailled assignment.
       ** @param[in] Sm      Super Module Id. (cf CbmTofAddress)
       ** @param[in] Rpc     Rpc Id. (cf CbmTofAddress)
       ** @param[in] Channel Channel Id. (cf CbmTofAddress)
       ** @param[in] time    Absolute time [ps].
       ** @param[in] tot     Time Over Threshold [ps].
       ** @param[in] Side    Channel Side (optional, used for strips). (cf CbmTofAddress)
       ** @param[in] Sm Type Super Module Type (optional). (cf CbmTofAddress)
       **/
  CbmTofDigi(uint32_t Sm, uint32_t Rpc, uint32_t Channel, double time, double tot, uint32_t Side = 0,
             uint32_t SmType = 0);

  /** @brief Constructor
   ** @param reference to CbmBmonDigi (equivalent content)
   **/
  CbmTofDigi(const CbmBmonDigi& digi);

  /** @brief Constructor
   ** @param pointer to const CbmBmonDigi object (equivalent content)
   **/
  CbmTofDigi(const CbmBmonDigi* digi);

  /**
       ** @brief Copy constructor.
       **/
  CbmTofDigi(const CbmTofDigi&) = default;


  /** Move constructor  **/
  CbmTofDigi(CbmTofDigi&&) = default;


  /** Assignment operator  **/
  CbmTofDigi& operator=(const CbmTofDigi&) = default;


  /** Move Assignment operator  **/
  CbmTofDigi& operator=(CbmTofDigi&&) = default;


  /**
       ** @brief Destructor.
       **/
  ~CbmTofDigi();

  /** Accessors **/
  /**
          ** @brief Inherited from CbmDigi.
          **/
  int32_t GetAddress() const { return fuAddress; };


  /** @brief Get the desired name of the branch for this obj in the cbm output tree  (static)
   ** @return "TofDigi"
   **/
  static const char* GetBranchName() { return "TofDigi"; }


  /** @brief Class name (static)
       ** @return  string CbmTofDigi
       **/
  static const char* GetClassName() { return "CbmTofDigi"; }

  static ECbmModuleId GetSystem() { return ECbmModuleId::kTof; }

  /**
          ** @brief Inherited from CbmDigi.
          **/
  double GetTime() const { return fdTime; };

  /**
          ** @brief Inherited from CbmDigi.
          **/
  double GetCharge() const { return fdTot; };
  /**
          ** @brief Alias for GetCharge.
          **/
  double GetTot() const { return GetCharge(); };
  /**
          ** @brief Sm.
          **/
  double GetSm() const { return CbmTofAddress::GetSmId(GetAddress()); };
  /**
          ** @brief Sm Type .
          **/
  double GetType() const { return CbmTofAddress::GetSmType(GetAddress()); };
  /**
          ** @brief Detector aka Module aka RPC .
          **/
  double GetRpc() const { return CbmTofAddress::GetRpcId(GetAddress()); };
  /**
          ** @brief Channel .
          **/
  double GetChannel() const { return CbmTofAddress::GetChannelId(GetAddress()); };
  /**
          ** @brief Channel Side.
          **/
  double GetSide() const { return CbmTofAddress::GetChannelSide(GetAddress()); };

  /** Modifiers **/
  void SetAddress(int32_t address) { fuAddress = address; };
  void SetAddress(uint32_t Sm, uint32_t Rpc, uint32_t Channel, uint32_t Side = 0, uint32_t SmType = 0);
  void SetTime(double time) { fdTime = time; };
  void SetTot(double tot) { fdTot = tot; };

  std::string ToString() const;


private:
  double fdTime;       ///< Absolute time [ps]
  double fdTot;        ///< Tot [ps]
  uint32_t fuAddress;  ///< Unique channel address

  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive& ar, const unsigned int /*version*/)
  {
    ar& fuAddress;
    ar& fdTime;
    ar& fdTot;
  }

#ifndef NO_ROOT
  ClassDefNV(CbmTofDigi, 3);
#endif
};
#endif  // CBMTOFDIGI_H
