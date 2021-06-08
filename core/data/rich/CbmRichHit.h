/* Copyright (C) 2006-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: B. Polichtchouk, Andrey Lebedev, Florian Uhlig, Denis Bertini [committer] */

/**
 * \file CbmRichHit.h
 * \author B. Polichtchouk
 *
 * Hits of MC tracks in Rich Photodetector including
 * detector geometry and efficiency
 **/

#ifndef CBMRICHHIT_H_
#define CBMRICHHIT_H_

#include "CbmPixelHit.h"  // for CbmPixelHit

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Double_t, Int_t

#include <string>  // for string

class CbmRichHit : public CbmPixelHit {

public:
  /**
   * \brief Default constructor.
   **/
  CbmRichHit();

  /**
   * \brief Constructor with input hit coordinates.
   */
  CbmRichHit(Double_t x, Double_t y);

  /**
   * \brief Constructor with input hit coordinates, timestamp and ToT.
   */
  CbmRichHit(Double_t x, Double_t y, Double_t ts, Double_t tot);

  /**
   * \brief Destructor.
   **/
  virtual ~CbmRichHit();

  /**
   * \brief Inherited from CbmBaseHit.
   */
  virtual std::string ToString() const;

  /**
   * \brief Inherited from CbmBaseHit.
   */
  virtual Int_t GetPlaneId() const { return 0; }

  /** Modifiers **/
  virtual void SetPmtId(Int_t det) { fPmtId = det; }
  //virtual void SetNPhotons (Int_t n) { fNPhotons = n; }
  //virtual void SetAmplitude(Double_t amp) { fAmplitude = amp; }
  void SetToT(Double_t tot) { fToT = tot; }

  /** Accessors **/
  virtual Int_t GetPmtId() const { return fPmtId; }
  //virtual Int_t GetNPhotons() const { return fNPhotons; }
  //virtual Double_t GetAmplitude() const { return fAmplitude; }
  Double_t GetToT() const { return fToT; }

  /** Outdated. Use CbmHit::GetTime() and SetTime() instead. **/
  // Double_t GetTimestamp() const { return GetTime(); }
  // void SetTimestamp(Double_t ts) { SetTime(ts); }

private:
  Int_t fPmtId;  // photomultiplier number
  //Int_t fNPhotons; // number of photons in this hit
  //Double32_t fAmplitude; // hit amplitude

  Double_t fToT;  // hit time-over-threshold

  ClassDef(CbmRichHit, 3)
};

#endif  // CBMRICHHIT_H_
