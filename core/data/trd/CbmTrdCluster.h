/**
 * \file CbmTrdCluster.h
 * \author Florian Uhlig <f.uhlig@gsi.de>
 * \brief Data Container for TRD clusters.
 **/

#ifndef CBMTRDCLUSTER_H
#define CBMTRDCLUSTER_H

#include <Rtypes.h>      // for CLRBIT, SETBIT, TESTBIT, ClassDefr
#include <RtypesCore.h>  // for Int_t, UShort_t, Bool_t, kTRUE, UChar_t, Opt...

#include <string>  // for string
#include <vector>  // for vector

#include "CbmCluster.h"  // for CbmCluster

/**
 * \class CbmTrdCluster
 * \author Florian Uhlig <f.uhlig@gsi.de>
 * \brief Data Container for TRD clusters.
 */
class CbmTrdCluster : public CbmCluster {
public:
  enum CbmTrdClusterDef {
    kTriang = 5  ///< set type of pads on which the cluster is reconstructed
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
  CbmTrdCluster(const std::vector<Int_t>& indices, Int_t address);
  /**
   * \brief Constructor starting from first digit.
   * \param[in] address global module address 
   * \param[in] idx global digi index in the TClonesArray
   * \param[in] ch RO channel address within the module 
   * \param[in] r module row for the RO channel 
   * \param[in] time relative buffer DAQ time 
   */
  CbmTrdCluster(Int_t address, Int_t idx, Int_t ch, Int_t r, Int_t time);
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
  Bool_t
  AddDigi(Int_t idx, Int_t channel = -1, Int_t terminator = 0, Int_t dt = 0);
  /** \brief reset cluster data*/
  void Clear(Option_t*);
  /** Accessors **/
  UShort_t GetNCols() const { return fNCols; }
  UShort_t GetNRows() const { return fNRows & 0x1f; }
  UShort_t GetEndCh() const { return fStartCh + fNCols - 1; }
  UShort_t GetRow() const { return GetNRows(); }
  UShort_t GetStartCh() const { return fStartCh; }
  UShort_t GetStartTime() const { return fStartTime; }
  Bool_t HasTrianglePads() const { return TESTBIT(fNRows, kTriang); }
  Bool_t HasOpenStart() const { return TESTBIT(fNRows, kProfileStart); }
  Bool_t HasOpenStop() const { return TESTBIT(fNRows, kProfileStop); }

  /** \brief Query on RO channel list 
   * \param[in] channel RO channel for digi
   * \return -1 before range; 0 in range; 1 after range; -2 cluster empty of digits
   */
  Int_t IsChannelInRange(Int_t ch) const;
  /** \brief Merge current cluster with info from second 
   * \param[in] second cluster to be added
   * \return success or fail
   */
  Bool_t Merge(CbmTrdCluster* second);
  /** \brief Initialize basic parameters of the cluster
   * \param[in] address global module address
   * \param[in] row cluster row in the module
   * \param[in] time cluster time in time buffer
   */
  void ReInit(Int_t address, Int_t row, Int_t time);
  /** Setters **/
  void SetNCols(UShort_t ncols) { fNCols = ncols; }
  void SetNRows(UShort_t nrows) {
    fNRows &= (7 << 5);
    fNRows |= (nrows & 0x1f);
  }
  void SetTrianglePads(Bool_t set = kTRUE) {
    set ? SETBIT(fNRows, kTriang) : CLRBIT(fNRows, kTriang);
  }
  void SetProfileStart(Bool_t set = kTRUE) {
    set ? SETBIT(fNRows, kProfileStart) : CLRBIT(fNRows, kProfileStart);
  }
  void SetProfileStop(Bool_t set = kTRUE) {
    set ? SETBIT(fNRows, kProfileStop) : CLRBIT(fNRows, kProfileStop);
  }

  /** \brief Extended functionality*/
  virtual std::string ToString() const;

protected:
  UChar_t fNCols;  // number of columns with charge above threshold
  UChar_t
    fNRows;  // cluster row info plus extra. Use dedicated getters for the correct value
  UShort_t fStartCh;    //! channel address of first channel
  UShort_t fStartTime;  //! start time of cluster in clk units wrt buffer start

  ClassDef(CbmTrdCluster, 4)  // cluster of digi for the TRD detector
};
#endif
