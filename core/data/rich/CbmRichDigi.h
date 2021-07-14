/* Copyright (C) 2015-2020 GSI/JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev, Volker Friese, Andrey Lebedev [committer], Florian Uhlig */

/*
 * CbmRichDigi.h
 *
 *  Created on: Dec 17, 2015
 *      Author: slebedev
 *  Modified on: Mar 25, 2019
 *              e.ovcharenko
 */

//TODO implement copy constructor and operator= ?

#include "CbmDefs.h"  // for kRich

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Double_t, Int_t

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

#include <string>  // for basic_string, string

#ifndef DATA_RICH_CBMRICHDIGI_H_
#define DATA_RICH_CBMRICHDIGI_H_

class CbmRichDigi {
public:
  CbmRichDigi();

  CbmRichDigi(Int_t addr, Double_t time, Double_t tot);

  ~CbmRichDigi();

  /*
	 * \brief Inherited from CbmDigi
	 * @value Unique address of pixel channel
	 */
  Int_t GetAddress() const { return fAddress; }

  /** @brief Get the desired name of the branch for this obj in the cbm output tree  (static)
   ** @return "RichDigi"
   **/
  static const char* GetBranchName() { return "RichDigi"; }

  /** @brief Charge
	 ** @return Returns TOT as charge
	 **
         ** Alias for GetToT, conversion factor should be added if needed.
	 ** For compatibility with template methods
	 **/
  Double_t GetCharge() const { return fToT; }

  /** @brief Class name (static)
   ** @return CbmRichDigi
   **/
  static const char* GetClassName() { return "CbmRichDigi"; }

  /** @brief System identifier
	 ** @return kRich (ECbmModuleId), static
	 **/
  static ECbmModuleId GetSystem() { return ECbmModuleId::kRich; }

  /*
	 * @brief Time
	 * @value Time [ns]
	 */
  Double_t GetTime() const { return fTime; }

  /*
	 * \brief Get Time-over-threshold
	 * @value Time-over-threshold, pulse width [ns]
	 */
  Double_t GetToT() const { return fToT; }

  /*
	 * \brief Set pixel Address
	 */
  void SetAddress(Int_t address) { fAddress = address; }

  /*
	 * \brief Set pixel Address
	 */
  void SetTime(Double_t time) { fTime = time; }


  std::string ToString() const { return std::string {""}; }

private:
  /**
	 * \brief Unique pixel address
	 */
  Int_t fAddress;

  /**
	 * \brief Leading (rising) edge time
	 */
  Double_t fTime;

  /**
	 * \brief Time-over-threshold, pulse width
	 */
  Double_t fToT;

  /// BOOST serialization interface
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int /*version*/)
  {
    ar& fAddress;
    ar& fTime;
    ar& fToT;
  }

  ClassDefNV(CbmRichDigi, 3);
};

#endif /* DATA_RICH_CBMRICHDIGI_H_ */
