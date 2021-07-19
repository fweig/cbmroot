/* Copyright (C) 2010 - 2021 Goethe-University Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig */

#ifndef CbmTsEventHeader_H
#define CbmTsEventHeader_H

#include <FairEventHeader.h>

#include <RtypesCore.h>

#include <cstddef>
#include <cstdint>

class CbmTsEventHeader : public FairEventHeader {

public:
  /** Default constructor */
  CbmTsEventHeader();

  /** Default destructor */
  virtual ~CbmTsEventHeader() {};

  /** Get the Start time of the this Timeslice linked to this event header*/
  uint64_t GetTsStartTime() { return fTsStartTime; }

  /** @brief Get the number of digis in this Ts */
  ULong64_t GetNDigisPsd(ULong64_t) { return fNDigisPsd; }
  /** @brief Get the number of digis in this Ts */
  ULong64_t GetNDigisRich(ULong64_t) { return fNDigisRich; }
  /** @brief Get the number of digis in this Ts */
  ULong64_t GetNDigisSts(ULong64_t) { return fNDigisSts; }
  /** @brief Get the number of digis in this Ts */
  ULong64_t GetNDigisTrd(ULong64_t) { return fNDigisTrd; }
  /** @brief Get the number of digis in this Ts */
  ULong64_t GetNDigisTrd2D(ULong64_t) { return fNDigisTrd2D; }
  /** @brief Get the number of digis in this Ts */
  ULong64_t GetNDigisTof(ULong64_t) { return fNDigisTof; }

  /** @brief Set the Ts Start Time @param value Start time of the TS */
  void SetTsStartTime(uint64_t value) { fTsStartTime = value; }

  /** @brief Set the number of digis in this Ts */
  void SetNDigisPsd(ULong64_t value) { fNDigisPsd = value; }
  /** @brief Set the number of digis in this Ts */
  void SetNDigisRich(ULong64_t value) { fNDigisRich = value; }
  /** @brief Set the number of digis in this Ts */
  void SetNDigisSts(ULong64_t value) { fNDigisSts = value; }
  /** @brief Set the number of digis in this Ts */
  void SetNDigisTrd(ULong64_t value) { fNDigisTrd = value; }
  /** @brief Set the number of digis in this Ts */
  void SetNDigisTrd2D(ULong64_t value) { fNDigisTrd2D = value; }
  /** @brief Set the number of digis in this Ts */
  void SetNDigisTof(ULong64_t value) { fNDigisTof = value; }


protected:
  /** Timeslice start time */
  uint64_t fTsStartTime = 0;

  ULong64_t fNDigisPsd   = 0;
  ULong64_t fNDigisRich  = 0;
  ULong64_t fNDigisSts   = 0;
  ULong64_t fNDigisTrd   = 0;
  ULong64_t fNDigisTrd2D = 0;
  ULong64_t fNDigisTof   = 0;


  ClassDef(CbmTsEventHeader, 3)
};
#endif
