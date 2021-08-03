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
  uint64_t GetTsIndex() { return fTsIndex; }
  /** Get the Start time of the this Timeslice linked to this event header*/
  uint64_t GetTsStartTime() { return fTsStartTime; }

  /** @brief Get the number of digis in this Ts */
  ULong64_t GetNDigisPsd(ULong64_t) { return fNDigisPsd; }
  /** @brief Get the number of digis in this Ts */
  ULong64_t GetNDigisRich(ULong64_t) { return fNDigisRich; }
  /** @brief Get the number of digis in this Ts */
  ULong64_t GetNDigisSts(ULong64_t) { return fNDigisSts; }
  /** @brief Get the number of digis in this Ts */
  ULong64_t GetNDigisTof(ULong64_t) { return fNDigisTof; }
  /** @brief Get the number of digis in this Ts */
  ULong64_t GetNDigisTrd1D(ULong64_t) { return fNDigisTrd; }
  /** @brief Get the number of digis in this Ts */
  ULong64_t GetNDigisTrd2D(ULong64_t) { return fNDigisTrd2D; }

  /** @brief Set the number of digis in this Ts */
  void SetNDigisPsd(ULong64_t value) { fNDigisPsd = value; }
  /** @brief Set the number of digis in this Ts */
  void SetNDigisRich(ULong64_t value) { fNDigisRich = value; }
  /** @brief Set the number of digis in this Ts */
  void SetNDigisSts(ULong64_t value) { fNDigisSts = value; }
  /** @brief Set the number of digis in this Ts */
  void SetNDigisTof(ULong64_t value) { fNDigisTof = value; }
  /** @brief Set the number of digis in this Ts */
  void SetNDigisTrd1D(ULong64_t value) { fNDigisTrd = value; }
  /** @brief Set the number of digis in this Ts */
  void SetNDigisTrd2D(ULong64_t value) { fNDigisTrd2D = value; }

  /** @brief Set the Ts Start Time @param value Start time of the TS */
  void SetTsIndex(uint32_t value) { fTsIndex = value; }
  /** @brief Set the Ts Start Time @param value Start time of the TS */
  void SetTsStartTime(uint64_t value) { fTsStartTime = value; }

protected:
  /** Timeslice index */
  uint64_t fTsIndex = 0;
  /** Timeslice start time */
  uint64_t fTsStartTime = 0;

  /** @brief nDigis in "this" timeslice measured by the PSD */
  ULong64_t fNDigisPsd = 0;
  /** @brief nDigis in "this" timeslice measured by the RICH */
  ULong64_t fNDigisRich = 0;
  /** @brief nDigis in "this" timeslice measured by the STS */
  ULong64_t fNDigisSts = 0;
  /** @brief nDigis in "this" timeslice measured by the TRD1D @remark the member is not renamed to fNDigisTrd1D for easier backwards compatibility */
  ULong64_t fNDigisTrd = 0;
  /** @brief nDigis in "this" timeslice measured by the TRD2D */
  ULong64_t fNDigisTrd2D = 0;
  /** @brief nDigis in "this" timeslice measured by the TOF */
  ULong64_t fNDigisTof = 0;

  ClassDef(CbmTsEventHeader, 4)
};
#endif
