/* Copyright (C) 2015-2020 GSI/JINR-LIT, Darmstadt/Dubna
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

#ifndef NO_ROOT
#include <Rtypes.h>      // for ClassDef
#endif

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

#include <cstdint>
#include <string>  // for basic_string, string

#ifndef DATA_RICH_CBMRICHDIGI_H_
#define DATA_RICH_CBMRICHDIGI_H_

class CbmRichDigi {
public:
  CbmRichDigi();

  CbmRichDigi(int32_t addr, double time, double tot);

  ~CbmRichDigi();

  /*
	 * \brief Inherited from CbmDigi
	 * @value Unique address of pixel channel
	 */
  int32_t GetAddress() const { return fAddress; }

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
  double GetCharge() const { return fToT; }

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
  double GetTime() const { return fTime; }

  /*
	 * \brief Get Time-over-threshold
	 * @value Time-over-threshold, pulse width [ns]
	 */
  double GetToT() const { return fToT; }

  /*
	 * \brief Set pixel Address
	 */
  void SetAddress(int32_t address) { fAddress = address; }

  /*
	 * \brief Set pixel Address
	 */
  void SetTime(double time) { fTime = time; }


  std::string ToString() const { return std::string {""}; }

private:
  /**
	 * \brief Unique pixel address
	 */
  int32_t fAddress;

  /**
	 * \brief Leading (rising) edge time
	 */
  double fTime;

  /**
	 * \brief Time-over-threshold, pulse width.
   * This variable is only used in real data analysis, for the simulation it is set to 0.
	 */
  double fToT;

  /// BOOST serialization interface
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int /*version*/)
  {
    ar& fAddress;
    ar& fTime;
    ar& fToT;
  }

#ifndef NO_ROOT
  ClassDefNV(CbmRichDigi, 3);
#endif
};

#endif /* DATA_RICH_CBMRICHDIGI_H_ */
