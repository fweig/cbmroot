/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */


#ifndef CBMTZDDIGI_H
#define CBMTZDDIGI_H 1

#include "CbmDefs.h"

#include <cstdint>

/** @class CbmTzdDigi
 ** @brief Data class for a signal in the t-zero detector
 **
 ** The current implementation is a placeholder, storing just the event time. To be replaced
 ** with a real detector measurement model once available.
 **/
class CbmTzdDigi {

public:
  /** @brief Constructor
   ** @param time Measurement time [ns]
   ** @param charge Measured charge]
   **/
  CbmTzdDigi(double time = -1., float charge = -1.) : fTime(time), fCharge(charge) {};


  /** @brief Destructor **/
  virtual ~CbmTzdDigi() {};


  /** @brief Get the desired name of the branch for this objects in the cbm output tree  (static)
   ** @return "BmonDigi"
   **/
  static const char* GetBranchName() { return "TzdDigi"; }


  /** @brief Class name (static)
   ** @return CbmTzdDigi
   **/
  static const char* GetClassName() { return "CbmTzdDigi"; }


  /** @brief Charge
   ** @return Charge
   **/
  double GetCharge() const { return fCharge; }


  /** @brief Time
   ** @return Time of measurement [ns]
   **/
  double GetTime() const { return fTime; }


  /** @brief Set the measured charge
   ** @param charge Charge
   **/
  void SetCharge(float charge) { fCharge = charge; }


  /** @brief Set the measurement time
   ** @param time Measurement time [ns]
   **/
  void SetTime(double time) { fTime = time; }


private:
  int32_t fAddress = ToIntegralType<ECbmModuleId>(ECbmModuleId::kT0);  ///< Unique CBM address
  double fTime     = -1.;                                              ///< Time of signal in TZD [ns]
  float fCharge    = -1.;                                              ///< Charge
};

#endif /* CBMTZDDIGI_H */
