/** @file CbmStsDigi.h
 ** @author V.Friese <v.friese@gsi.de>
 ** @since 28.08.2006
 ** @version 6
 **/


#ifndef CBMSTSDIGI_H
#define CBMSTSDIGI_H 1

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Int_t, Double_t, UShort_t, Long64_t, ULong64_t

#include <string>  // for string

#include "CbmDefs.h"  // for ECbmModuleId::kSts

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

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
  CbmStsDigi(Int_t address, Int_t channel, ULong64_t time, UShort_t charge)
    : fTime(time), fAddress(address), fChannel(channel), fCharge(charge) {}


  /** Destructor **/
  ~CbmStsDigi() {};


  /** Unique detector element address  (see CbmStsAddress)
   ** @value Unique address of readout channel
   **/
  Int_t GetAddress() const { return fAddress; }


  /** @brief Channel number in module
   ** @value Channel number
   **/
  UShort_t GetChannel() const { return fChannel; }


  /** @brief Class name (static)
   ** @return CbmStsDigi
   **/
  static const char* GetClassName() { return "CbmStsDigi"; }


  /** Charge
   ** @value Charge [ADC units]
   **/
  Double_t GetCharge() const { return Double_t(fCharge); }


  /** System ID (static)
  ** @return System identifier (EcbmModuleId)
  **/
  static ECbmModuleId GetSystem() { return ECbmModuleId::kSts; }


  /** Time of measurement
   ** @value Time [ns]
   **/
  Double_t GetTime() const { return Double_t(fTime); }


  template<class Archive>
  void serialize(Archive& ar, const unsigned int /*version*/) {
    ar& fTime;
    ar& fAddress;
    ar& fChannel;
    ar& fCharge;
  }


  /** Update Time of measurement
   ** @param New Time [ns]
   **/
  void SetTime(Double_t dNewTime) { fTime = dNewTime; }


  /** String output **/
  std::string ToString() const;


private:
  friend class boost::serialization::access;

  Long64_t fTime    = 0.;  ///< Time [ns]
  Int_t fAddress    = 0;   ///< Unique element address
  UShort_t fChannel = 0;   ///< Channel number
  UShort_t fCharge  = 0;   ///< Charge [ADC units]


  ClassDefNV(CbmStsDigi, 7);
};


#endif
