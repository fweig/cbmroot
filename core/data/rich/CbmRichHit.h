/* Copyright (C) 2006-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Boris Polichtchouk, Andrey Lebedev, Florian Uhlig, Denis Bertini [committer] */

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

#include <cstdint>
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
  CbmRichHit(double x, double y);

  /**
   * \brief Constructor with input hit coordinates, timestamp and ToT.
   */
  CbmRichHit(double x, double y, double ts, double tot);

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
  virtual int32_t GetPlaneId() const { return 0; }

  /** Modifiers **/
  virtual void SetPmtId(int32_t det) { fPmtId = det; }
  //virtual void SetNPhotons (int32_t n) { fNPhotons = n; }
  //virtual void SetAmplitude(double amp) { fAmplitude = amp; }
  void SetToT(double tot) { fToT = tot; }

  /** Accessors **/
  virtual int32_t GetPmtId() const { return fPmtId; }
  //virtual int32_t GetNPhotons() const { return fNPhotons; }
  //virtual double GetAmplitude() const { return fAmplitude; }
  double GetToT() const { return fToT; }

  /** Outdated. Use CbmHit::GetTime() and SetTime() instead. **/
  // double GetTimestamp() const { return GetTime(); }
  // void SetTimestamp(double ts) { SetTime(ts); }

private:
  int32_t fPmtId;  // photomultiplier number
  //int32_t fNPhotons; // number of photons in this hit
  //double fAmplitude; // hit amplitude

  double fToT;  // hit time-over-threshold

  ClassDef(CbmRichHit, 3)
};

#endif  // CBMRICHHIT_H_
