/* Copyright (C) 2018-2020 Horia Hulubei National Institute of Physics and Nuclear Engineering, Bucharest
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Alexandru Bercuci[committer] */

#ifndef CBMTRDDIGIREC_H
#define CBMTRDDIGIREC_H

#include "CbmTrdDigi.h"

/** @class CbmTrdDigiRec
 ** @brief Extend the TRD(2D) digi class to incorporate FEE calibration. 
 ** @author Alexandru Bercucic <abercuci@niham.nipne.ro>
 ** @since 01.10.2021
 ** @date 01.10.2021
 **
 ** The digi class contains the information as it is produced by the FEE (ASIC/GETS)
 ** The variation from channel to channel is captured by running the pulser on anode wires
 ** using various signal values, frequencies, etc. The calibrated baselines, gains, jitter, 
 ** etc. are transported via the parameter files and are applied to the data within the digRec
 ** class which is in the end used to calculate the TRD hit parameters.
 **/
class CbmTrdDigiRec : public CbmTrdDigi {
  friend class CbmTrdModuleRecT;

public:
  /** \brief Default constructor*/
  CbmTrdDigiRec();
  /** \brief Wrap CbmTrdDigi constructor*/
  CbmTrdDigiRec(const CbmTrdDigi& d, Double_t* g = NULL, Double_t* t = NULL);
  virtual ~CbmTrdDigiRec() { ; }

  /** \brief Return calibrated tilt signal
   * \param[out] on flag signal exists */
  Double_t GetTiltCharge(Bool_t& on) const { return GetCharge(0, on); }
  /** \brief Return calibrated tilt time [ns]*/
  Double_t GetTiltTime() const { return GetTime(0); }
  /** \brief Return calibrated rect signal
   * \param[out] on flag signal exists */
  Double_t GetRectCharge(Bool_t& on) const { return GetCharge(1, on); }
  /** \brief Return calibrated rect time [ns]*/
  Double_t GetRectTime() const { return GetTime(1); }
  /** \brief Return calibrated signal
   * \param[in] typ tilt [0], rect [1] 
   * \param[out] on flag signal exists
   */
  Double_t GetCharge(Int_t typ, Bool_t& on) const;
  /** \brief Return calibrated time
   * \param[in] typ tilt [0], rect [1] 
  */
  Double_t GetTime(Int_t typ) const;
  Bool_t HasRectOvf() const { return TESTBIT(fStatus, 1); }
  Bool_t HasTiltOvf() const { return TESTBIT(fStatus, 0); }
  /** \brief Init FEE gain and time walk corrections */
  void Init(Double_t g[2], Double_t t[3]);

protected:
  /** \brief Constructor and merger*/
  CbmTrdDigiRec(const CbmTrdDigi& dt, const CbmTrdDigi& dr, Double_t* g = NULL, Double_t* t = NULL);

private:
  UChar_t fStatus;             //< bit map to store calibration flags
  Double_t fG[2];              //< FEE gain correction for channel T & R
  Double_t fT[3];              //< FEE time walk  correction as function of charge
  ClassDef(CbmTrdDigiRec, 1);  // Wrapper around the RAW TRD digi (CbmTrdDigi) to acount for calibration
};

#endif
