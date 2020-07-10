/** CbmTofDigi.h
 ** @author Pierre-Alain Loizeau <loizeau@physi.uni-heidelberg.de>
 ** @date 07.06.2013
 **/

/** @class CbmTofDigi
 ** @brief Data class for expanded digital TOF information
 ** @brief Data level: TDC CALIB
 ** @version 1.0
 **
 ** The information is encoded into 3*4 bytes (2 Double_t + 1 UInt_t).
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

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Double_t, UInt_t, Int_t, Bool_t, kTRUE

#include <string>  // for string

#include "CbmDefs.h"        // for kTof
#include "CbmTofAddress.h"  // for CbmTofAddress

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

//class CbmMatch;

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
  CbmTofDigi(UInt_t address, Double_t time, Double_t tot);

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
  CbmTofDigi(UInt_t Sm,
             UInt_t Rpc,
             UInt_t Channel,
             Double_t time,
             Double_t tot,
             UInt_t Side   = 0,
             UInt_t SmType = 0);

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
  Int_t GetAddress() const { return fuAddress; };


  /** @brief Class name (static)
       ** @return  string CbmTofDigi
       **/
  static const char* GetClassName() { return "CbmTofDigi"; }

  static ECbmModuleId GetSystem() { return ECbmModuleId::kTof; }

  /**
          ** @brief Inherited from CbmDigi.
          **/
  Double_t GetTime() const { return fdTime; };

  /**
          ** @brief Inherited from CbmDigi.
          **/
  Double_t GetCharge() const { return fdTot; };
  /**
          ** @brief Alias for GetCharge.
          **/
  Double_t GetTot() const { return GetCharge(); };
  /**
          ** @brief Sm.
          **/
  Double_t GetSm() const { return CbmTofAddress::GetSmId(GetAddress()); };
  /**
          ** @brief Sm Type .
          **/
  Double_t GetType() const { return CbmTofAddress::GetSmType(GetAddress()); };
  /**
          ** @brief Detector aka Module aka RPC .
          **/
  Double_t GetRpc() const { return CbmTofAddress::GetRpcId(GetAddress()); };
  /**
          ** @brief Channel .
          **/
  Double_t GetChannel() const {
    return CbmTofAddress::GetChannelId(GetAddress());
  };
  /**
          ** @brief Channel Side.
          **/
  Double_t GetSide() const {
    return CbmTofAddress::GetChannelSide(GetAddress());
  };

  /** Modifiers **/
  void SetAddress(Int_t address) { fuAddress = address; };
  void SetAddress(UInt_t Sm,
                  UInt_t Rpc,
                  UInt_t Channel,
                  UInt_t Side   = 0,
                  UInt_t SmType = 0);
  void SetTime(Double_t time) { fdTime = time; };
  void SetTot(Double_t tot) { fdTot = tot; };

  std::string ToString() const;


private:
  Double_t fdTime;   ///< Absolute time [ps]
  Double_t fdTot;    ///< Tot [ps]
  UInt_t fuAddress;  ///< Unique channel address

  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive& ar, const unsigned int /*version*/) {
    ar& fuAddress;
    ar& fdTime;
    ar& fdTot;
  }

  ClassDefNV(CbmTofDigi, 2);
};
#endif  // CBMTOFDIGI_H
