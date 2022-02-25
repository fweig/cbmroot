/* Copyright (C) 2010-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

/**
 * \file CbmTrdCluster.h
 * \author Florian Uhlig <f.uhlig@gsi.de>
 * \brief Data Container for TRD clusters.
 **/

#ifndef CBMTRDCLUSTER_H
#define CBMTRDCLUSTER_H

#include "CbmCluster.h"  // for CbmCluster

#include <Rtypes.h>      // for CLRBIT, SETBIT, TESTBIT, ClassDefr

#include <cstdint>
#include <string>  // for string
#include <vector>  // for vector

/**
 * \class CbmTrdCluster
 * \author Florian Uhlig <f.uhlig@gsi.de>
 * \brief Data Container for TRD clusters.
 */
class CbmTrdCluster : public CbmCluster {
public:
  enum eCbmTrdClusterDef
  {
    kFasp = 5  ///< set type of FEE digis contained
      ,
    kProfileStart  ///< only for triangular if no T in first col
      ,
    kProfileStop  ///< only for triangular if no R in last col
  };
  /**
   * \brief Default constructor.
   */
  CbmTrdCluster();
  CbmTrdCluster(const CbmTrdCluster& ref);
  CbmTrdCluster(const std::vector<int32_t>& indices, int32_t address);
  /**
   * \brief Constructor starting from first digit.
   * \param[in] address global module address 
   * \param[in] idx global digi index in the TClonesArray
   * \param[in] ch RO channel address within the module 
   * \param[in] r module row for the RO channel 
   * \param[in] time relative buffer DAQ time 
   */
  CbmTrdCluster(int32_t address, int32_t idx, int32_t ch, int32_t r, int32_t time);
  /**
   * \brief Destructor.
   */
  virtual ~CbmTrdCluster();

  /** \brief Append digi to cluster
   * \param[in] idx index of digi in TClonesArray
   * \param[in] channel RO channel for digi
   * \param[in] terminator state of digi for triangular pads: 0 if complete, -/+ if T/R misses 
   * \param[in] dt update start time of cluster if current digi is prompt
   * \return true if successful
   */
  bool AddDigi(int32_t idx, int32_t channel = -1, int32_t terminator = 0, int32_t dt = 0);
  /** \brief reset cluster data*/
  void Clear(Option_t*);
  /** Accessors **/
  uint16_t GetNCols() const { return fNCols; }
  uint16_t GetNRows() const { return fNRows & 0x1f; }
  uint16_t GetEndCh() const { return fStartCh + fNCols - 1; }
  uint16_t GetRow() const { return GetNRows(); }
  uint16_t GetStartCh() const { return fStartCh; }
  uint32_t GetStartTime() const { return fStartTime; }
  bool HasFaspDigis() const { return TESTBIT(fNRows, kFasp); }
  bool HasOpenStart() const { return TESTBIT(fNRows, kProfileStart); }
  bool HasOpenStop() const { return TESTBIT(fNRows, kProfileStop); }

  /** \brief Query on RO channel list 
   * \param[in] channel RO channel for digi
   * \return -1 before range; 0 in range; 1 after range; -2 cluster empty of digits
   */
  int32_t IsChannelInRange(int32_t ch) const;
  /** \brief Merge current cluster with info from second 
   * \param[in] second cluster to be added
   * \param[in] typ the type of pad-plane of the source chamber; true if Trd2d 
   * \return success or fail
   */
  bool Merge(CbmTrdCluster* second, bool typ = true);
  /** \brief Initialize basic parameters of the cluster
   * \param[in] address global module address
   * \param[in] row cluster row in the module
   * \param[in] time cluster time in time buffer
   */
  void ReInit(int32_t address, int32_t row, int32_t time);
  /** Setters **/
  void SetNCols(uint16_t ncols) { fNCols = ncols; }
  void SetNRows(uint16_t nrows)
  {
    fNRows &= (7 << 5);
    fNRows |= (nrows & 0x1f);
  }
  void SetFaspDigis(bool set = true) { set ? SETBIT(fNRows, kFasp) : CLRBIT(fNRows, kFasp); }
  void SetProfileStart(bool set = true) { set ? SETBIT(fNRows, kProfileStart) : CLRBIT(fNRows, kProfileStart); }
  void SetProfileStop(bool set = true) { set ? SETBIT(fNRows, kProfileStop) : CLRBIT(fNRows, kProfileStop); }

  /** \brief Extended functionality*/
  virtual std::string ToString() const;

protected:
  uint8_t fNCols(0);          //! number of columns with charge above threshold
  uint8_t fNRows(0x1f);       //! cluster row info plus extra meta data. Use dedicated getters for the correct value
  uint16_t fStartCh(0xffff);  //! channel address of first channel
  uint32_t fStartTime(0xffffffff);  //! start time of cluster in clk units wrt buffer start

  ClassDef(CbmTrdCluster, 5)  // cluster of digi for the TRD detector
};
#endif
